/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#include "drv_mipirx_kapi.h"
#include "drv_mipirx_port.h"
#include "drv_mipirx_proc.h"
#include "ot_mipi_rx.h"
#include "hal_mipirx_mipi_ctrl.h"
#include "drv_mipirx_comm.h"
#include "drv_mipirx_osal.h"
#include "hal_mipirx_custom.h"
#include "hal_mipirx_comm.h"
#include "hal_mipirx_phy.h"
#include "hal_mipirx_crg.h"
#include "hal_mipirx_sensor.h"
#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
#include "hal_mipirx_lvds_ctrl.h"
#endif

#define MIPIRX_INTERRUPT_COUNT_INTERVAL_MS  1000
#define MIPIRX_INTERRUPT_COUNT_THRESHOLD    100
#define MIPIRX_INTERRUPT_MASK_TIME_MS       5000

static td_u32 g_irq_num;
static td_bool g_mipirx_init = TD_FALSE;
static mipirx_port_ctx *g_mipirx_ctx;
static td_u32 g_mipirx_interrupt_count = 0;
static osal_timer g_mipirx_interval_timer;
static osal_timer g_mipirx_mask_timer;

static mipirx_port_ctx *drv_mipirx_get_ctx(td_void)
{
    return g_mipirx_ctx;
}

static td_u32 mipi_rx_interrupt_route(td_u32 irq, td_void *dev_id)
{
    td_u32 port_id;
    td_u32 phy_id;
    for (port_id = 0; port_id < MIPIRX_PORT_NUM_MAX; ++port_id) {
        for (phy_id = 0; phy_id < MIPIRX_PHY_NUM_MAX; ++phy_id) {
            hal_mipirx_phy_count_err_irq(phy_id);
        }
        hal_mipirx_mipi_ctrl_count_err(port_id);
#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
        hal_mipirx_lvds_ctrl_count_err(port_id);
#endif
        hal_mipirx_align_err(port_id);
    }
    ++g_mipirx_interrupt_count;
    return OSAL_IRQ_HANDLED;
}

static td_void mipi_rx_mask_timer_handler(td_ulong arg)
{
    osal_timer_mod(&g_mipirx_interval_timer, MIPIRX_INTERRUPT_COUNT_INTERVAL_MS);
    osal_irq_enable(g_irq_num);
}

static td_void mipi_rx_interval_timer_handler(td_ulong arg)
{
    if (g_mipirx_interrupt_count > MIPIRX_INTERRUPT_COUNT_THRESHOLD) {
        osal_irq_disable(g_irq_num);
        osal_timer_mod(&g_mipirx_mask_timer, MIPIRX_INTERRUPT_MASK_TIME_MS);
    } else {
        osal_timer_mod(&g_mipirx_interval_timer, MIPIRX_INTERRUPT_COUNT_INTERVAL_MS);
    }
    g_mipirx_interrupt_count = 0;
}

static td_void drv_mipirx_reset_cxt(td_void)
{
    td_u8 i;
    td_u8 j;

    memset_s(g_mipirx_ctx, sizeof(mipirx_port_ctx), 0, sizeof(mipirx_port_ctx));
    g_mipirx_ctx->lane_mode = LANE_DIVIDE_MODE_MAX;

    for (i = 0; i < MIPIRX_PORT_NUM_MAX; ++i) {
        g_mipirx_ctx->mipirx_port_cfg[i].work_mode = MIPIRX_INPUT_MODE_MAX;
        g_mipirx_ctx->mipirx_port_cfg[i].data_rate = MIPIRX_DATA_RATE_MAX;
        g_mipirx_ctx->mipirx_port_cfg[i].data_type = MIPIRX_DATA_TYPE_MAX;
        for (j = 0; j < MIPIRX_TOTAL_LANE_NUM_MAX; ++j) {
            g_mipirx_ctx->mipirx_port_cfg[i].lane_id[j] = -1;
        }
        for (j = 0; j < MIPIRX_PHY_NUM_MAX; ++j) {
            g_mipirx_ctx->lane_bit_map[i].phy_lane_bit[j] = 0;
        }
    }
}

static td_s32 drv_mipirx_init_ctx(td_void)
{
    g_mipirx_ctx = (mipirx_port_ctx *) osal_vmalloc(sizeof(mipirx_port_ctx));
    if (g_mipirx_ctx == TD_NULL) {
        return OT_ERR_MIPIRX_FAILED_MALLOC;
    }
    drv_mipirx_reset_cxt();
    return TD_SUCCESS;
}

static td_s32 drv_mipirx_timer_init(td_void)
{
    td_s32 ret = TD_SUCCESS;
    g_mipirx_interval_timer.handler = mipi_rx_interval_timer_handler;
    g_mipirx_interval_timer.interval = MIPIRX_INTERRUPT_COUNT_INTERVAL_MS;
    ret = osal_timer_init(&g_mipirx_interval_timer);
    if (ret != TD_SUCCESS) {
        mipirx_log_err("mipirx interval timer init failed.\n");
        return ret;
    }
    g_mipirx_mask_timer.handler = mipi_rx_mask_timer_handler;
    g_mipirx_mask_timer.interval = MIPIRX_INTERRUPT_MASK_TIME_MS;
    ret = osal_timer_init(&g_mipirx_mask_timer);
    if (ret != TD_SUCCESS) {
        mipirx_log_err("mipirx mask timer init failed.\n");
        return ret;
    }
    osal_timer_mod(&g_mipirx_interval_timer, MIPIRX_INTERRUPT_COUNT_INTERVAL_MS);
    return ret;
}

static td_void drv_mipirx_timer_deinit(td_void)
{
    osal_timer_destroy(&g_mipirx_interval_timer);
    osal_timer_destroy(&g_mipirx_mask_timer);
}

td_s32 drv_mipirx_kapi_init(td_void *pdev)
{
    td_s32 ret;

    ret = hal_mipirx_reg_base_init(TD_NULL);
    if (ret != TD_SUCCESS) {
        hal_mipirx_reg_base_deinit();
        return ret;
    }
    ret = drv_mipirx_get_irq_num(pdev, &g_irq_num);
    if (ret != TD_SUCCESS) {
        hal_mipirx_reg_base_deinit();
        return ret;
    }

    ret = drv_mipirx_register_irq(g_irq_num, mipi_rx_interrupt_route);
    if (ret != TD_SUCCESS) {
        hal_mipirx_reg_base_deinit();
        return ret;
    }

    ret = drv_mipirx_timer_init();
    if (ret != TD_SUCCESS) {
        drv_mipirx_unregister_irq(g_irq_num, mipi_rx_interrupt_route);
        hal_mipirx_reg_base_deinit();
        return ret;
    }

    ret = drv_mipirx_init_mutex();
    if (ret < 0) {
        drv_mipirx_timer_deinit();
        drv_mipirx_unregister_irq(g_irq_num, mipi_rx_interrupt_route);
        hal_mipirx_reg_base_deinit();
        return ret;
    }

    ret = drv_mipirx_init_ctx();
    if (ret < 0) {
        drv_mipirx_timer_deinit();
        drv_mipirx_unregister_irq(g_irq_num, mipi_rx_interrupt_route);
        hal_mipirx_reg_base_deinit();
        return ret;
    }
    ret = drv_mipirx_add_proc(drv_mipirx_get_ctx());
    if (ret != TD_SUCCESS) {
        drv_mipirx_timer_deinit();
        osal_vfree((td_void *) drv_mipirx_get_ctx());
        drv_mipirx_unregister_irq(g_irq_num, mipi_rx_interrupt_route);
        hal_mipirx_reg_base_deinit();
        return ret;
    }

    return TD_SUCCESS;
}

td_void drv_mipirx_kapi_deinit(td_void)
{
    drv_mipirx_del_proc();
    drv_mipirx_unregister_irq(g_irq_num, mipi_rx_interrupt_route);
    drv_mipirx_timer_deinit();
    hal_mipirx_reg_base_deinit();
    drv_mipirx_deinit_mutex();
    osal_vfree((td_void *) drv_mipirx_get_ctx());
    g_mipirx_init = TD_FALSE;
}

#ifdef CONFIG_OT_SYS_KOL_SUPPORT
td_bool drv_mipirx_is_init(td_void)
{
    return g_mipirx_init;
}

mipirx_lane_mode drv_mipirx_get_lane_mode(td_void)
{
    mipirx_port_ctx *mipirx_ctx = drv_mipirx_get_ctx();
    return mipirx_ctx->lane_mode;
}

td_s32 drv_mipirx_kapi_resume_set_lane_mode(td_void)
{
    td_u8 phy_id;
    mipirx_lane_mode lane_mode;
    mipirx_port_ctx *mipirx_ctx = drv_mipirx_get_ctx();

    if (drv_mipirx_interruptible_mutex_lock()) {
        return -ERESTARTSYS;
    }

    lane_mode = mipirx_ctx->lane_mode;
    for (phy_id = 0; phy_id < MIPIRX_PHY_NUM_MAX; ++phy_id) {
        hal_mipirx_phy_set_mode_link(phy_id, lane_mode);
        hal_mipirx_phy_set_skew(phy_id);
    }
    hal_mipirx_phy_set_lane_mode(lane_mode);

    drv_mipirx_mutex_unlock();
    return TD_SUCCESS;
}

td_void drv_mipirx_kapi_crg_enable_sys_clock(td_bool enable)
{
    hal_mipirx_crg_enable_sys_clock(enable);
}
#endif

td_s32 drv_mipirx_kapi_set_lane_mode(mipirx_lane_mode lane_mode)
{
    td_u8 phy_id;
    mipirx_port_ctx *mipirx_ctx = drv_mipirx_get_ctx();

    if (drv_mipirx_interruptible_mutex_lock()) {
        return -ERESTARTSYS;
    }

    if (lane_mode >= LANE_DIVIDE_MODE_MAX) {
        drv_mipirx_mutex_unlock();
        return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }

    if (lane_mode == mipirx_ctx->lane_mode) {
        drv_mipirx_mutex_unlock();
        return TD_SUCCESS;
    }

    drv_mipirx_reset_cxt();

    hal_mipirx_crg_enable_sys_clock(TD_TRUE);

    for (phy_id = 0; phy_id < MIPIRX_PHY_NUM_MAX; ++phy_id) {
        hal_mipirx_phy_set_mode_link(phy_id, lane_mode);
        hal_mipirx_phy_set_skew(phy_id);
    }
    mipirx_ctx->lane_mode = lane_mode;
    hal_mipirx_phy_set_lane_mode(lane_mode);
    g_mipirx_init = TD_TRUE;
    drv_mipirx_mutex_unlock();
    return TD_SUCCESS;
}

td_s32 drv_mipirx_kapi_param_check(td_u8 port_id)
{
    if (port_id >= MIPIRX_PORT_NUM_MAX) {
        mipirx_log_err("mipirx invalid port id\n");
        return OT_ERR_MIPIRX_INVALID_PORT_ID;
    }
    if (g_mipirx_init == TD_FALSE) {
        mipirx_log_err("mipirx not set lane mode\n");
        return OT_ERR_MIPIRX_NOT_SET_LANE_MODE;
    }
    return TD_SUCCESS;
}

td_s32 drv_mipirx_kapi_enable_mipi_clock(td_u8 port_id, td_bool enable)
{
    td_s32 ret;

    ret = drv_mipirx_kapi_param_check(port_id);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    if (drv_mipirx_interruptible_mutex_lock()) {
        return -ERESTARTSYS;
    }
    hal_mipirx_crg_enable_pix_clock(port_id, enable);
    drv_mipirx_mutex_unlock();
    return TD_SUCCESS;
}

td_s32 drv_mipirx_kapi_reset_mipi(td_u8 port_id, td_bool reset)
{
    td_s32 ret;

    ret = drv_mipirx_kapi_param_check(port_id);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    if (drv_mipirx_interruptible_mutex_lock()) {
        return -ERESTARTSYS;
    }
    hal_mipirx_crg_reset_pix_clock(port_id, reset);
    drv_mipirx_mutex_unlock();
    return TD_SUCCESS;
}

td_s32 drv_mipirx_kapi_enable_sensor_clock(td_u8 port_id, td_bool enable)
{
    td_s32 ret;

    ret = drv_mipirx_kapi_param_check(port_id);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    if (drv_mipirx_interruptible_mutex_lock()) {
        return -ERESTARTSYS;
    }

    hal_mipirx_sensor_enable_clock(port_id, enable);
    drv_mipirx_mutex_unlock();
    return TD_SUCCESS;
}

td_s32 drv_mipirx_kapi_reset_sensor(td_u8 port_id, td_bool reset)
{
    td_s32 ret;

    ret = drv_mipirx_kapi_param_check(port_id);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    if (drv_mipirx_interruptible_mutex_lock()) {
        return -ERESTARTSYS;
    }
    hal_mipirx_sensor_reset_clock(port_id, reset);
    drv_mipirx_mutex_unlock();
    return TD_SUCCESS;
}

static td_s32 drv_mipirx_kapi_check_img_rect(mipirx_img_rect *img_rect)
{
    if (img_rect->width < MIPIRX_MIN_WIDTH || img_rect->height < MIPIRX_MIN_HEIGHT) {
        return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }

    if (img_rect->width > MIPIRX_MAX_WIDTH || img_rect->height > MIPIRX_MAX_HEIGHT) {
        return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }
    return TD_SUCCESS;
}

static td_s32 drv_mipirx_kapi_mipi_check_wdr(mipirx_port_cfg *attr)
{
    td_u8 i;
    if (attr->mipi_attr.wdr_mode >= MIPI_WDR_MODE_MAX) {
        return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }

    if ((attr->mipi_attr.wdr_mode != MIPI_WDR_MODE_NONE) &&
        (attr->data_type >= MIPIRX_DATA_TYPE_YUV420_8BIT_NORMAL)) {
        return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }

    if (attr->mipi_attr.wdr_mode == MIPI_WDR_MODE_DT) {
        for (i = 0; i < MIPIRX_VC_NUM_MAX; i++) {
            /* data_type must be the CSI-2 reserve Type [0x38, 0x3f] */
            if (attr->mipi_attr.wdr_data_type[i] < 0x38 || attr->mipi_attr.wdr_data_type[i] > 0x3f) {
                return OT_ERR_MIPIRX_ILLEGAL_PARAM;
            }
        }
    }
    return TD_SUCCESS;
}

#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
static td_s32 drv_mipirx_kapi_lvds_check_wdr(mipirx_port_cfg *attr)
{
    if (attr->lvds_attr.sync_mode >= MIPIRX_LVDS_SYNC_MODE_MAX) {
        return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }

    if (attr->lvds_attr.wdr_attr.wdr_type >= LVDS_WDR_TYPE_MAX) {
        return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }

    if (attr->lvds_attr.wdr_attr.wdr_num >= LVDS_WDR_NUM_MAX) {
        return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}
#endif

static td_s32 drv_mipirx_kapi_check_mipi(mipirx_port_cfg *attr)
{
    if (attr->data_type >= MIPIRX_DATA_TYPE_MAX) {
        return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }
    return drv_mipirx_kapi_mipi_check_wdr(attr);
}

#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
static td_s32 drv_mipirx_kapi_check_lvds(mipirx_port_cfg *attr)
{
    td_s32 ret;

    if (attr->data_type >= MIPIRX_DATA_TYPE_YUV420_8BIT_NORMAL) {
        return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }

    if (attr->lvds_attr.data_endian >= LVDS_ENDIAN_MAX) {
        return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }

    if (attr->lvds_attr.sync_code_endian >= LVDS_ENDIAN_MAX) {
        return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }

    ret = drv_mipirx_kapi_lvds_check_wdr(attr);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return TD_SUCCESS;
}
#else
static td_s32 drv_mipirx_kapi_check_lvds(mipirx_port_cfg *attr)
{
    ot_unused(attr);
    mipirx_log_err("mipirx lvds mode not support!\n");
    return OT_ERR_MIPIRX_MODE_NOT_SUPPORT;
}
#endif

static td_bool mipi_rx_check_lane_valid(td_u8 port_id, td_s8 lane_id, mipirx_lane_mode mode)
{
    td_bool lane_valid = TD_FALSE;

    switch (mode) {
        case MODE_4_LANE:
            if (port_id == 0 && (lane_id >= 0 && lane_id <= 3)) { /* mipi_dev 0: lane 0,1,2,3 */
                lane_valid = TD_TRUE;
            }
            break;
        case MODE_2_PLUS_2_LANE:
            if (port_id == 0 && (lane_id == 0 || lane_id == 2)) { /* mipi_dev 0: lane 0,2 */
                lane_valid = TD_TRUE;
            } else if (port_id == 1 && (lane_id == 1 || lane_id == 3)) { /* mipi_dev 1: lane 1,3 */
                lane_valid = TD_TRUE;
            }
            break;
        default:
            break;
    }
    return lane_valid;
}

static td_s32 drv_mipirx_kapi_check_lane_id(mipirx_port_cfg *attr)
{
    td_u8 i;
    td_u8 j;
    td_u8 lane_cnt = 0;
    td_bool lane_valid = TD_FALSE;
    mipirx_port_ctx *mipirx_ctx = drv_mipirx_get_ctx();

    for (i = 0; i < MIPIRX_TOTAL_LANE_NUM_MAX; ++i) {
        if (attr->lane_id[i] == -1) {
            continue;
        }

        ++lane_cnt;

        if (attr->lane_id[i] >= MIPIRX_TOTAL_LANE_NUM_MAX || attr->lane_id[i] < -1) {
            return OT_ERR_MIPIRX_INVALID_LANE_ID;
        }

        for (j = i + 1; j < MIPIRX_TOTAL_LANE_NUM_MAX; ++j) {
            if (attr->lane_id[i] == attr->lane_id[j]) {
                return OT_ERR_MIPIRX_INVALID_LANE_ID;
            }
        }

        lane_valid = mipi_rx_check_lane_valid(attr->port_id, attr->lane_id[i], mipirx_ctx->lane_mode);
        if (!lane_valid) {
            return OT_ERR_MIPIRX_INVALID_LANE_ID;
        }
    }

    if (lane_cnt == 0) {
        return OT_ERR_MIPIRX_INVALID_LANE_ID;
    }
    return TD_SUCCESS;
}

static td_s32 drv_mipirx_kapi_check_attr(mipirx_port_cfg *attr)
{
    td_s32 ret;

    ret = drv_mipirx_kapi_param_check(attr->port_id);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    if (attr->work_mode >= MIPIRX_INPUT_MODE_MAX || attr->data_rate != MIPIRX_DATA_RATE_X1) {
        return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }

    ret = drv_mipirx_kapi_check_img_rect(&attr->img_rect);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = drv_mipirx_kapi_check_lane_id(attr);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if (attr->work_mode == MIPIRX_INPUT_MODE_MIPI) {
        return drv_mipirx_kapi_check_mipi(attr);
    } else {
        return drv_mipirx_kapi_check_lvds(attr);
    }
}

static td_void drv_mipirx_phy_cfg_lanes(mipirx_port_cfg *attr)
{
    td_u8 phy_id;
    td_u8 phy_lane_bit;
    mipirx_port_ctx *mipirx_ctx = drv_mipirx_get_ctx();
    mipirx_lane_bit_map *cur_lane = &mipirx_ctx->lane_bit_map[attr->port_id];

    mipirx_get_phy_lane_bit_map(attr->lane_id, MIPIRX_LANE_NUM, cur_lane, MIPIRX_PHY_NUM_MAX);
    for (phy_id = 0; phy_id < MIPIRX_PHY_NUM_MAX; ++phy_id) {
        if (mipirx_check_phy_link_port(phy_id, cur_lane)) {
            hal_mipirx_phy_set_work_mode(phy_id, attr->work_mode);
            phy_lane_bit = mipirx_get_phy_lane_bit(phy_id, mipirx_ctx->lane_bit_map, MIPIRX_PORT_NUM_MAX);
            hal_mipirx_phy_cfg_lanes(phy_id, phy_lane_bit);
        }
    }
}

static td_void drv_mipirx_mipi_set_mipi_dev_attr(mipirx_port_cfg *attr)
{
    hal_mipirx_ctrl_set_work_mode(attr->port_id);
    hal_mipirx_mipi_ctrl_set_rect(attr->port_id, &attr->img_rect, TD_TRUE);
    hal_mipirx_mipi_ctrl_set_mipi_dt(attr->port_id, attr->data_type);
    hal_mipirx_mipi_ctrl_set_wdr_mode(attr->port_id, &attr->mipi_attr, attr->data_type);
    hal_mipirx_mipi_ctrl_set_data_rate(attr->port_id, attr->data_rate);
    hal_mipirx_mipi_ctrl_set_lane_id(attr->port_id, attr->lane_id, MIPIRX_LANE_NUM);
    hal_mipirx_mipi_ctrl_set_cli_mode(mipirx_get_lane_bit_map(attr->lane_id, MIPIRX_LANE_NUM));
    hal_mipirx_mipi_ctrl_enable_mem_clk(attr->port_id, TD_TRUE);
    drv_mipirx_phy_cfg_lanes(attr);
}

#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
static td_u32 drv_mipirx_get_lvds_shift_code(mipirx_data_type input_data_type, mipirx_lvds_wdr_type wdr_type)
{
    if (wdr_type >= LVDS_WDR_DOL_4TH_CODE && wdr_type <= LVDS_WDR_DOL_5TH_CODE) {
        if (input_data_type == MIPIRX_DATA_TYPE_RAW_8BIT) {
            return 1 << 8; /* 8 bits one pixel component */
        } else {
            return 1 << 10; /* 10 bits one pixel component */
        }
    }
    return 0;
}

static td_u8 drv_mipirx_lvds_get_lane_num(mipirx_port_cfg *attr)
{
    td_u8 i;
    td_u8 valid_lane_num = 0;
    for (i = 0; i < MIPIRX_TOTAL_LANE_NUM_MAX; ++i) {
        if (attr->lane_id[i] != -1) {
            ++valid_lane_num;
        }
    }
    return valid_lane_num;
}
#endif

static td_void drv_mipirx_irq_open(td_u8 port_id)
{
    mipirx_port_ctx *mipirx_ctx = drv_mipirx_get_ctx();
    td_u32 phy_num;

    for (phy_num = 0; phy_num < MIPIRX_PHY_NUM_MAX; ++phy_num) {
        hal_mipirx_phy_irq_open(phy_num);
    }
    if (mipirx_ctx->mipirx_port_cfg->work_mode == MIPIRX_INPUT_MODE_MIPI) {
        hal_mipirx_mipi_ctrl_irq_open(port_id);
    } else {
#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
        hal_mipirx_lvds_ctrl_err_irq_open(port_id);
#endif
    }

    hal_mipirx_align_irq_open(port_id);
}

#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
static td_void drv_mipirx_lvds_set_sync_code(mipirx_port_cfg *attr)
{
    mipirx_port_ctx *mipirx_ctx = drv_mipirx_get_ctx();
    td_u32 i;
    td_u8 phy_lane_bit;
    td_u32 shift_code = drv_mipirx_get_lvds_shift_code(attr->data_type, attr->lvds_attr.wdr_attr.wdr_type);

    hal_mipirx_lvds_ctrl_set_sync_code(attr->port_id, attr->lane_id, attr->lvds_attr.sync_code, MIPIRX_LANE_NUM);
    hal_mipirx_lvds_ctrl_set_nxt_sync_code(attr->port_id, attr->lane_id, attr->lvds_attr.sync_code,
        MIPIRX_TOTAL_LANE_NUM_MAX, shift_code);

    for (i = 0; i < MIPIRX_PHY_NUM_MAX; ++i) {
        phy_lane_bit = mipirx_ctx->lane_bit_map[attr->port_id].phy_lane_bit[i];
        if (phy_lane_bit > 0) {
            hal_mipirx_phy_set_sync_dct(i, attr->data_type, attr->lvds_attr.data_endian, phy_lane_bit);
            hal_mipirx_phy_set_sync_code(i, attr->lane_id, attr->lvds_attr.sync_code, MIPIRX_TOTAL_LANE_NUM_MAX,
                shift_code);
        }
    }
}

static td_void drv_mipirx_lvds_set_lvds_dev_attr(mipirx_port_cfg *attr)
{
    hal_mipirx_lvds_ctrl_set_work_mode(attr->port_id);
    hal_mipirx_lvds_set_cropping_area(attr->port_id, &attr->img_rect, TD_TRUE, drv_mipirx_lvds_get_lane_num(attr));
    hal_mipirx_set_lvds_wdr_mode(attr->port_id, &attr->lvds_attr.wdr_attr);
    hal_mipirx_set_lvds_ctrl_mode(attr->port_id, &attr->lvds_attr, attr->data_type);
    hal_mipirx_set_lvds_data_rate(attr->port_id, attr->data_rate);
    hal_mipirx_mipi_ctrl_enable_mem_clk(attr->port_id, TD_TRUE);
    hal_mipirx_lvds_ctrl_enable_clr_clock(attr->port_id, TD_TRUE);
    hal_mipirx_mipi_ctrl_set_lane_id(attr->port_id, attr->lane_id, MIPIRX_LANE_NUM);
    hal_mipirx_lvds_ctrl_set_cli_mode(mipirx_get_lane_bit_map(attr->lane_id, MIPIRX_LANE_NUM));
    drv_mipirx_phy_cfg_lanes(attr);
    drv_mipirx_lvds_set_sync_code(attr);
}
#endif

#ifdef CONFIG_OT_SYS_KOL_SUPPORT
td_s32 drv_mipirx_kapi_resume_set_attr(td_u8 port_id)
{
    mipirx_port_ctx *mipirx_ctx = drv_mipirx_get_ctx();

    if (port_id >= MIPIRX_PORT_NUM_MAX) {
        return TD_FAILURE;
    }
    return drv_mipirx_kapi_set_attr(&mipirx_ctx->mipirx_port_cfg[port_id]);
}
#endif

td_s32 drv_mipirx_kapi_set_attr(mipirx_port_cfg *attr)
{
    td_s32 ret = TD_SUCCESS;
    mipirx_port_ctx *mipirx_ctx = drv_mipirx_get_ctx();
    mipirx_port_cfg *old_cfg;

    if (attr == TD_NULL) {
        return OT_ERR_MIPIRX_NULL_PTR;
    }
    old_cfg = &mipirx_ctx->mipirx_port_cfg[attr->port_id];
    if (drv_mipirx_interruptible_mutex_lock()) {
        return -ERESTARTSYS;
    }

    ret = drv_mipirx_kapi_check_attr(attr);
    if (ret != TD_SUCCESS) {
        drv_mipirx_mutex_unlock();
        return ret;
    }

    if (attr->work_mode == MIPIRX_INPUT_MODE_MIPI) {
        drv_mipirx_mipi_set_mipi_dev_attr(attr);
    } else {
#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
        drv_mipirx_lvds_set_lvds_dev_attr(attr);
#endif
    }

    memcpy_s(old_cfg, sizeof(mipirx_port_cfg), attr, sizeof(mipirx_port_cfg));
    drv_mipirx_irq_open(attr->port_id);
    drv_mipirx_mutex_unlock();
    return TD_SUCCESS;
}

td_s32 drv_mipirx_kapi_set_phy_mode(td_u8 port_id, mipirx_work_mode mode)
{
    td_u32 i;
    td_s32 ret;

    mipirx_port_ctx *mipirx_ctx = drv_mipirx_get_ctx();
    mipirx_port_cfg *port_cfg = TD_NULL;

    ret = drv_mipirx_kapi_param_check(port_id);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    if (mode >= MIPIRX_INPUT_MODE_MAX) {
        return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }
    port_cfg = &mipirx_ctx->mipirx_port_cfg[port_id];
    if (port_cfg->work_mode == MIPIRX_INPUT_MODE_MAX) {
        return OT_ERR_MIPIRX_MODE_NOT_SUPPORT;
    }

    if (drv_mipirx_interruptible_mutex_lock()) {
        return -ERESTARTSYS;
    }

    port_cfg->work_mode = mode;
    for (i = 0; i < MIPIRX_PHY_NUM_MAX; i++) {
        if (mipirx_check_phy_link_port(i, &mipirx_ctx->lane_bit_map[port_id])) {
            hal_mipirx_phy_set_work_mode(i, mode);
        }
    }
    drv_mipirx_mutex_unlock();
    return TD_SUCCESS;
}

static td_bool mipirx_check_ext_data_bit_width(td_u16 bit_width)
{
    if (bit_width < MIPIRX_MIN_EXT_DATA_TYPE_BIT_WIDTH ||
        bit_width > MIPIRX_MAX_EXT_DATA_TYPE_BIT_WIDTH) {
        if (bit_width != MIPIRX_EXT_DATA_TYPE_EM_BIT_WIDTH) {
            return TD_FALSE;
        }
    }
    return TD_TRUE;
}

#ifdef CONFIG_OT_SYS_KOL_SUPPORT
td_s32 drv_mipirx_kapi_resume_set_user_def_dt(td_u8 port_id)
{
    mipirx_port_ctx *mipirx_ctx = drv_mipirx_get_ctx();

    if (port_id >= MIPIRX_PORT_NUM_MAX) {
        return TD_FAILURE;
    }
    return drv_mipirx_kapi_set_user_def_dt(port_id, &mipirx_ctx->mipirx_port_cfg[port_id].mipi_attr.udf_dt);
}
#endif

td_s32 drv_mipirx_kapi_set_user_def_dt(td_u8 port_id, mipirx_mipi_udf_dt *data_type)
{
    mipirx_port_ctx *mipirx_ctx = drv_mipirx_get_ctx();
    mipirx_port_cfg *port_cfg;
    td_u8 i;
    td_s32 ret;

    ret = drv_mipirx_kapi_param_check(port_id);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    if (data_type == TD_NULL) {
        return OT_ERR_MIPIRX_NULL_PTR;
    }
    port_cfg = &mipirx_ctx->mipirx_port_cfg[port_id];
    if (port_cfg->work_mode != MIPIRX_INPUT_MODE_MIPI) {
        return OT_ERR_MIPIRX_MODE_NOT_SUPPORT;
    }
    if (data_type->num > MIPIRX_MAX_EXT_DATA_TYPE_NUM) {
        return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }
    
    for (i = 0; i < data_type->num; i++) {
        if (!mipirx_check_ext_data_bit_width(data_type->data_bit_width[i])) {
            return OT_ERR_MIPIRX_ILLEGAL_PARAM;
        }

        if (data_type->data_bit_width[i] % 2 != 0) { /* 2: bit width must be even value */
            return OT_ERR_MIPIRX_ILLEGAL_PARAM;
        }
    }
    if (drv_mipirx_interruptible_mutex_lock()) {
        return -ERESTARTSYS;
    }
    hal_mipirx_mipi_ctrl_set_user_defined_dt(port_id, data_type, port_cfg->data_type);
    memcpy_s(&port_cfg->mipi_attr.udf_dt, sizeof(mipirx_mipi_udf_dt), data_type, sizeof(mipirx_mipi_udf_dt));
    drv_mipirx_mutex_unlock();
    return TD_SUCCESS;
}

