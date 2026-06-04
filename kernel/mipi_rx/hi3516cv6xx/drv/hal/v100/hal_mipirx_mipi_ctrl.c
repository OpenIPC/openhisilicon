/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include "drv_mipirx_comm.h"
#include "hal_mipirx_comm.h"
#include "hal_mipirx_ctrl_reg.h"
#include "hal_mipirx_global_reg.h"
#include "hal_mipirx_custom.h"
#include "hal_mipirx_sys_reg.h"
#include "hal_mipirx_mipi_ctrl.h"

#define MIPIRX_FRAME_CNT 10

typedef struct {
    mipirx_data_type input_data_type;
    td_u32 data_type_value;
} mipirx_data_type_map;

typedef struct {
    mipirx_data_type input_data_type;
    td_u32 data_bit_width_value;
} mipirx_data_bit_width_map;

static mipirx_mipi_err_cnt mipi_err_cnt[MIPIRX_PORT_NUM_MAX];
static mipirx_align_err align_err[MIPIRX_PORT_NUM_MAX];

static const mipirx_data_type_map g_mipirx_data_type_map[] = {
    {MIPIRX_DATA_TYPE_RAW_8BIT, 0x2a},
    {MIPIRX_DATA_TYPE_RAW_10BIT, 0x2b},
    {MIPIRX_DATA_TYPE_RAW_12BIT, 0x2c},
    {MIPIRX_DATA_TYPE_RAW_14BIT, 0x2d},
    {MIPIRX_DATA_TYPE_YUV420_8BIT_NORMAL, 0x18},
    {MIPIRX_DATA_TYPE_YUV420_8BIT_LEGACY, 0x1a},
    {MIPIRX_DATA_TYPE_YUV422_8BIT, 0x1e},
    {MIPIRX_DATA_TYPE_YUV422_PACKED, 0x1e},
};

static const mipirx_data_bit_width_map g_mipirx_data_bit_width_map[] = {
    {MIPIRX_DATA_TYPE_RAW_8BIT, 0x0},
    {MIPIRX_DATA_TYPE_RAW_10BIT, 0x1},
    {MIPIRX_DATA_TYPE_RAW_12BIT, 0x2},
    {MIPIRX_DATA_TYPE_RAW_14BIT, 0x3},
    {MIPIRX_DATA_TYPE_YUV420_8BIT_NORMAL, 0x0},
    {MIPIRX_DATA_TYPE_YUV420_8BIT_LEGACY, 0x0},
    {MIPIRX_DATA_TYPE_YUV422_8BIT, 0x0},
    {MIPIRX_DATA_TYPE_YUV422_PACKED, 0x0},
};

mipirx_mipi_err_cnt *hal_mipi_get_ctl_err_cnt_ctx(td_u8 port_id)
{
    if (port_id >= MIPIRX_PORT_NUM_MAX) {
        return TD_NULL;
    }
    return &mipi_err_cnt[port_id];
}

mipirx_align_err *hal_mipi_get_align_err_ctx(td_u8 port_id)
{
    if (port_id >= MIPIRX_PORT_NUM_MAX) {
        return TD_NULL;
    }
    return &align_err[port_id];
}

td_void hal_mipirx_mipi_ctrl_set_rect(td_u8 port_id, const mipirx_img_rect *p_img_rect, td_bool enable)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);
    if (enable) {
        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CROP_START_CHN0, CFG_MIPI_START_X_CHN0_M, p_img_rect->x);
        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CROP_START_CHN0, CFG_MIPI_START_Y_CHN0_M, p_img_rect->y);

        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CROP_START_CHN1, CFG_MIPI_START_X_CHN1_M, p_img_rect->x);
        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CROP_START_CHN1, CFG_MIPI_START_Y_CHN1_M, p_img_rect->y);

        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CROP_START_CHN2, CFG_MIPI_START_X_CHN2_M, p_img_rect->x);
        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CROP_START_CHN2, CFG_MIPI_START_Y_CHN2_M, p_img_rect->y);

        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CROP_START_CHN3, CFG_MIPI_START_X_CHN3_M, p_img_rect->x);
        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CROP_START_CHN3, CFG_MIPI_START_Y_CHN3_M, p_img_rect->y);

        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_IMGSIZE, CFG_MIPI_IMGHEIGHT_M, p_img_rect->height - SIZE_1);
        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_IMGSIZE, CFG_MIPI_IMGWIDTH_M, p_img_rect->width - SIZE_1);
    }
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CTRL_MODE_PIXEL, CFG_CROP_EN_M, enable);
}

static td_void mipirx_mipi_ctrl_set_yuv_user_def(td_u32 *ctrl_addr, mipirx_data_type input_data_type,
    td_bool enable)
{
    td_u32 bit_width;
    td_u32 yuv_data_type;
    td_u8 len;
    td_u8 i;

    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CTRL_MODE_HS, CFG_USER_DEF_EN_M, enable);
    if (enable) {
        len = mipirx_array_size(g_mipirx_data_type_map);
        for (i = 0; i < len; i++) {
            if (input_data_type == g_mipirx_data_type_map[i].input_data_type) {
                yuv_data_type = g_mipirx_data_type_map[i].data_type_value;
                break;
            }
        }

        len = mipirx_array_size(g_mipirx_data_bit_width_map);
        for (i = 0; i < len; i++) {
            if (input_data_type == g_mipirx_data_bit_width_map[i].input_data_type) {
                bit_width = g_mipirx_data_bit_width_map[i].data_bit_width_value;
                break;
            }
        }

        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USER_DEF, CFG_USER_DEF3_M, yuv_data_type);
        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USER_DEF, CFG_USER_DEF2_M, yuv_data_type);
        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USER_DEF, CFG_USER_DEF1_M, yuv_data_type);
        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USER_DEF, CFG_USER_DEF0_M, yuv_data_type);

        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USERDEF_DT, CFG_USER_DEF3_DT_M, bit_width);
        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USERDEF_DT, CFG_USER_DEF2_DT_M, bit_width);
        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USERDEF_DT, CFG_USER_DEF1_DT_M, bit_width);
        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USERDEF_DT, CFG_USER_DEF0_DT_M, bit_width);
    }
}

td_void hal_mipirx_mipi_ctrl_set_mipi_dt(td_u8 port_id, mipirx_data_type input_data_type)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CTRL_MODE_PIXEL, CFG_MIPI_YUV_420_NOLEGACY_EN_M, 0);
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CTRL_MODE_PIXEL, CFG_MIPI_YUV_420_LEGACY_EN_M, 0);
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CTRL_MODE_PIXEL, CFG_MIPI_YUV_422_EN_M, 0);

    switch (input_data_type) {
        case MIPIRX_DATA_TYPE_YUV420_8BIT_NORMAL:
            mipirx_mipi_ctrl_set_yuv_user_def(ctrl_addr, input_data_type, TD_TRUE);
            hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CTRL_MODE_PIXEL, CFG_MIPI_YUV_420_NOLEGACY_EN_M, 1);
            break;
        case MIPIRX_DATA_TYPE_YUV420_8BIT_LEGACY:
            mipirx_mipi_ctrl_set_yuv_user_def(ctrl_addr, input_data_type, TD_TRUE);
            hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CTRL_MODE_PIXEL, CFG_MIPI_YUV_420_LEGACY_EN_M, 1);
            break;
        case MIPIRX_DATA_TYPE_YUV422_8BIT:
        case MIPIRX_DATA_TYPE_YUV422_PACKED:
            mipirx_mipi_ctrl_set_yuv_user_def(ctrl_addr, input_data_type, TD_TRUE);
            hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CTRL_MODE_PIXEL, CFG_MIPI_YUV_422_EN_M, 1);
            break;
        default:
            mipirx_mipi_ctrl_set_yuv_user_def(ctrl_addr, input_data_type, TD_FALSE);
            break;
    }
}

static td_void mipirx_mipi_ctrl_set_mipi_wdr_dol_mode(td_u32 *ctrl_addr)
{
    // enable dol mode
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CTRL_MODE_PIXEL, CFG_MIPI_DOL_MODE_M, 1);
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_DOL_ID_CODE0, CFG_ID_CODE_REG0_M, 0x241);
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_DOL_ID_CODE0, CFG_ID_CODE_REG1_M, 0x242);
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_DOL_ID_CODE1, CFG_ID_CODE_REG2_M, 0x244);
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_DOL_ID_CODE1, CFG_ID_CODE_REG3_M, 0x251);
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_DOL_ID_CODE2, CFG_ID_CODE_REG4_M, 0x252);
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_DOL_ID_CODE2, CFG_ID_CODE_REG5_M, 0x254);
}

static td_void mipirx_mipi_ctrl_set_mipi_wdr_dt_mode(td_u32 *ctrl_addr, const mipirx_mipi_cfg *mipi_conf,
    mipirx_data_type input_data_type)
{
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CTRL_MODE_HS, CFG_USER_DEF_EN_M, 1);

    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USER_DEF, CFG_USER_DEF1_M, mipi_conf->wdr_data_type[1]);
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USER_DEF, CFG_USER_DEF0_M, mipi_conf->wdr_data_type[0]);

    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USERDEF_DT, CFG_USER_DEF1_DT_M, input_data_type);
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USERDEF_DT, CFG_USER_DEF0_DT_M, input_data_type);
}

static td_void mipirx_mipi_ctrl_set_mipi_wdr_vc_mode(td_u32 *ctrl_addr)
{
    // disable dol mode
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CTRL_MODE_PIXEL, CFG_MIPI_DOL_MODE_M, 0);
    // enable vc mode
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CTRL_MODE_HS, CFG_VC_MODE_M, 1);
}

static td_void mipirx_mipi_ctrl_close_wdr_mode(td_u32 *ctrl_addr)
{
    // disable dol mode
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CTRL_MODE_PIXEL, CFG_MIPI_DOL_MODE_M, 0);
    // disable vc mode
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CTRL_MODE_HS, CFG_VC_MODE_M, 0);
}


td_void hal_mipirx_mipi_ctrl_set_data_rate(td_u8 port_id, mipirx_data_rate data_rate)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);
    if (data_rate == MIPIRX_DATA_RATE_X1) {
        hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CTRL_MODE_PIXEL, CFG_MIPI_DOUBLE_PIX_EN_M, 0);
    } else {
        return;
    }
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CTRL_MODE_PIXEL, CFG_SYNC_CLEAR_EN_M, 1);
}

td_u32 hal_mipirx_ctrl_get_lane_map(td_u8 port_id, td_s16 lane_idx)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);

    if (lane_idx < 0) {
        return 0;
    }

    switch (lane_idx) {
        case SIZE_0:  /* 0 -- lane_id 0 */
            return hal_mipirx_reg_readb(ctrl_addr, REG_LANE_ID0_CHN, CFG_LANE0_ID_M);
        case SIZE_1:  /* 1 -- lane_id 1 */
            return hal_mipirx_reg_readb(ctrl_addr, REG_LANE_ID0_CHN, CFG_LANE1_ID_M);
        case SIZE_2:  /* 2 -- lane_id 2 */
            return hal_mipirx_reg_readb(ctrl_addr, REG_LANE_ID0_CHN, CFG_LANE2_ID_M);
        case SIZE_3:  /* 3 -- lane_id 3 */
            return hal_mipirx_reg_readb(ctrl_addr, REG_LANE_ID0_CHN, CFG_LANE3_ID_M);
        default:
            break;
    }
    return 0;
}

static td_void mipirx_mipi_ctrl_set_lane_id(td_u8 port_id, td_u8 lane_idx, td_u8 lane_id)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);
    if (port_id == 1) {
        lane_id = lane_id / SIZE_2;
    }
    switch (lane_id) {
        case SIZE_0:  /* 0 -- lane_id 0 */
            hal_mipirx_reg_writeb(ctrl_addr, REG_LANE_ID0_CHN, CFG_LANE0_ID_M, lane_idx);
            break;
        case SIZE_1:  /* 1 -- lane_id 1 */
            hal_mipirx_reg_writeb(ctrl_addr, REG_LANE_ID0_CHN, CFG_LANE1_ID_M, lane_idx);
            break;
        case SIZE_2:  /* 2 -- lane_id 2 */
            hal_mipirx_reg_writeb(ctrl_addr, REG_LANE_ID0_CHN, CFG_LANE2_ID_M, lane_idx);
            break;
        case SIZE_3:  /* 3 -- lane_id 3 */
            hal_mipirx_reg_writeb(ctrl_addr, REG_LANE_ID0_CHN, CFG_LANE3_ID_M, lane_idx);
            break;
        default:
            break;
    }
}

td_void hal_mipirx_mipi_ctrl_set_wdr_mode(td_u8 port_id, const mipirx_mipi_cfg *mipi_dev,
    mipirx_data_type input_data_type)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);
    switch (mipi_dev->wdr_mode) {
        case MIPI_WDR_MODE_NONE:
            mipirx_mipi_ctrl_close_wdr_mode(ctrl_addr);
            break;
        case MIPI_WDR_MODE_VC:
            mipirx_mipi_ctrl_set_mipi_wdr_vc_mode(ctrl_addr);
            break;
        case MIPI_WDR_MODE_DT:
            mipirx_mipi_ctrl_set_mipi_wdr_dt_mode(ctrl_addr, mipi_dev, input_data_type);
            break;
        case MIPI_WDR_MODE_DOL:
            mipirx_mipi_ctrl_set_mipi_wdr_dol_mode(ctrl_addr);
            break;
        default:
            break;
    }
}

td_void hal_mipirx_ctrl_set_work_mode(td_u8 port_id)
{
    td_u32 *sys_addr = hal_mipirx_get_sys_addr();

    switch (port_id) {
        case 0:
            hal_mipirx_reg_writeb(sys_addr, REG_MIPI_WORK_MODE, CFG_MIPI0_WORK_MODE_M, 0x0);
            break;
        case 1:
            hal_mipirx_reg_writeb(sys_addr, REG_MIPI_WORK_MODE, CFG_MIPI1_WORK_MODE_M, 0x0);
            break;
        default:
            break;
    }
}

td_void hal_mipirx_mipi_ctrl_set_lane_id(td_u8 port_id, const td_s8 *p_lane_id, td_u8 size)
{
    td_u8 i;
    for (i = 0; i < size; i++) {
        if (p_lane_id[i] != -1) {
            mipirx_mipi_ctrl_set_lane_id(port_id, i, p_lane_id[i]);
        }
    }
}

td_void hal_mipirx_mipi_ctrl_enable_mem_clk(td_u8 port_id, td_bool enable)
{
    td_u32 *global_addr = hal_mipirx_get_global_addr();
    switch (port_id) {
        case SIZE_0:
            hal_mipirx_reg_writeb(global_addr, REG_CHN0_MEM_CTRL, CFG_CHN0_MEM_CK_GT_M, enable);
            break;
        case SIZE_1:
            hal_mipirx_reg_writeb(global_addr, REG_CHN1_MEM_CTRL, CFG_CHN1_MEM_CK_GT_M, enable);
            break;
        case SIZE_2:
            hal_mipirx_reg_writeb(global_addr, REG_CHN2_MEM_CTRL, CFG_CHN2_MEM_CK_GT_M, enable);
            break;
        case SIZE_3:
            hal_mipirx_reg_writeb(global_addr, REG_CHN3_MEM_CTRL, CFG_CHN3_MEM_CK_GT_M, enable);
            break;
        default:
            break;
    }
}

td_void hal_mipirx_mipi_ctrl_set_cli_mode(td_u16 lane_bit_map)
{
    td_u32 *global_addr = hal_mipirx_get_global_addr();

    if (lane_bit_map & 0x5) {
        hal_mipirx_reg_writeb(global_addr, REG_PHYCFG_MODE, CFG_PHYCIL0_0_CFG_MODE_M, 0);
    }
    if (lane_bit_map & 0xa) {
        hal_mipirx_reg_writeb(global_addr, REG_PHYCFG_MODE, CFG_PHYCIL0_1_CFG_MODE_M, 0);
    }
    hal_mipirx_reg_writeb(global_addr, REG_PHYCFG_MODE, CFG_PHYCIL0_CFG_MODE_SEL_M, 0);
}

static td_void mipirx_mipi_ctrl_set_user_defined_dt(td_u32 *ctrl_addr, const mipirx_mipi_udf_dt *usr_data_type)
{
    td_u8 i;
    for (i = 0; i < usr_data_type->num; i++) {
        switch (i) {
            case SIZE_0:
                hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USER_DEF, CFG_USER_DEF1_M, usr_data_type->data_type[0]);
                hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USERDEF_DT, CFG_USER_DEF1_DT_M,
                    usr_data_type->data_bit_width[0]);
                break;
            case SIZE_1:
                hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USER_DEF, CFG_USER_DEF2_M, usr_data_type->data_type[1]);
                hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USERDEF_DT, CFG_USER_DEF2_DT_M,
                    usr_data_type->data_bit_width[1]);
                break;
            case SIZE_2:
                hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USER_DEF, CFG_USER_DEF3_M, usr_data_type->data_type[SIZE_2]);
                hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USERDEF_DT, CFG_USER_DEF3_DT_M,
                    usr_data_type->data_bit_width[SIZE_2]);
                break;
            default:
                break;
        }
    }
}

td_void hal_mipirx_mipi_ctrl_set_user_defined_dt(td_u8 port_id, const mipirx_mipi_udf_dt *usr_data_type,
    mipirx_data_type input_data_type)
{
    td_u8 i;
    td_u8 len;
    td_u16 input_dt;
    td_u16 input_bit_width;
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);

    len = mipirx_array_size(g_mipirx_data_type_map);
    for (i = 0; i < len; i++) {
        if (input_data_type == g_mipirx_data_type_map[i].input_data_type) {
            input_dt = g_mipirx_data_type_map[i].data_type_value;
            break;
        }
    }

    len = mipirx_array_size(g_mipirx_data_bit_width_map);
    for (i = 0; i < len; i++) {
        if (input_data_type == g_mipirx_data_bit_width_map[i].input_data_type) {
            input_bit_width = g_mipirx_data_bit_width_map[i].data_bit_width_value;
            break;
        }
    }

    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CTRL_MODE_HS, CFG_USER_DEF_EN_M, TD_TRUE);
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USER_DEF, CFG_USER_DEF0_M, input_dt);
    hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_USERDEF_DT, CFG_USER_DEF0_DT_M, input_bit_width);
    mipirx_mipi_ctrl_set_user_defined_dt(ctrl_addr, usr_data_type);
}

td_void hal_mipirx_mipi_ctrl_get_mipi_imgsize(td_u8 port_id, td_u8 vc, mipirx_img_rect *img_size)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);

    switch (vc) {
        case SIZE_0:
            img_size->width = hal_mipirx_reg_readb(ctrl_addr, REG_MIPI_IMGSIZE0_STATIS, CFG_IMGWIDTH_STATIS_VC0_M);
            img_size->height = hal_mipirx_reg_readb(ctrl_addr, REG_MIPI_IMGSIZE0_STATIS, CFG_IMGHEIGHT_STATIS_VC0_M);
            break;
        case SIZE_1:
            img_size->width = hal_mipirx_reg_readb(ctrl_addr, REG_MIPI_IMGSIZE1_STATIS, CFG_IMGWIDTH_STATIS_VC1_M);
            img_size->height = hal_mipirx_reg_readb(ctrl_addr, REG_MIPI_IMGSIZE1_STATIS, CFG_IMGHEIGHT_STATIS_VC1_M);
            break;
        case SIZE_2:
            img_size->width = hal_mipirx_reg_readb(ctrl_addr, REG_MIPI_IMGSIZE2_STATIS, CFG_IMGWIDTH_STATIS_VC2_M);
            img_size->height = hal_mipirx_reg_readb(ctrl_addr, REG_MIPI_IMGSIZE2_STATIS, CFG_IMGHEIGHT_STATIS_VC2_M);
            break;
        case SIZE_3:
            img_size->width = hal_mipirx_reg_readb(ctrl_addr, REG_MIPI_IMGSIZE3_STATIS, CFG_IMGWIDTH_STATIS_VC3_M);
            img_size->height = hal_mipirx_reg_readb(ctrl_addr, REG_MIPI_IMGSIZE3_STATIS, CFG_IMGHEIGHT_STATIS_VC3_M);
            break;
        default:
            mipirx_log_err("invalid mipirx vc channel %u.\n", vc);
            break;
    }
}

static td_void mipirx_mipi_ctrl_count_crc_err(td_u32 err, td_u32 port_id)
{
    if (err & CFG_CRC_ERR_VC0_ST_M) {
        mipi_err_cnt[port_id].vc0_err_crc_cnt++;
    }
    if (err & CFG_CRC_ERR_VC1_ST_M) {
        mipi_err_cnt[port_id].vc1_err_crc_cnt++;
    }
    if (err & CFG_CRC_ERR_VC2_ST_M) {
        mipi_err_cnt[port_id].vc2_err_crc_cnt++;
    }
    if (err & CFG_CRC_ERR_VC3_ST_M) {
        mipi_err_cnt[port_id].vc3_err_crc_cnt++;
    }
}

static td_void mipirx_mipi_ctrl_count_ecc_err(td_u32 err,  td_u32 port_id)
{
    if (err & CFG_ECC_ERR_VC0_ST_M) {
        mipi_err_cnt[port_id].vc0_err_ecc_cnt++;
    }
    if (err & CFG_ECC_ERR_VC1_ST_M) {
        mipi_err_cnt[port_id].vc1_err_ecc_cnt++;
    }
    if (err & CFG_ECC_ERR_VC2_ST_M) {
        mipi_err_cnt[port_id].vc2_err_ecc_cnt++;
    }
    if (err & CFG_ECC_ERR_VC3_ST_M) {
        mipi_err_cnt[port_id].vc3_err_ecc_cnt++;
    }
    if (err & CFG_ECC_ERR_MULT_ST_M) {
        mipi_err_cnt[port_id].err_ecc_mult_cnt++;
    }
}

static td_void mipirx_mipi_ctrl_count_frame_err(td_u32 err, td_u32 port_id)
{
    if (err & CFG_FRAME_NUM_ERR_VC0_ST_M) {
        mipi_err_cnt[port_id].vc0_err_frame_num_cnt++;
    }
    if (err & CFG_FRAME_NUM_ERR_VC1_ST_M) {
        mipi_err_cnt[port_id].vc1_err_frame_num_cnt++;
    }
    if (err & CFG_FRAME_NUM_ERR_VC2_ST_M) {
        mipi_err_cnt[port_id].vc2_err_frame_num_cnt++;
    }
    if (err & CFG_FRAME_NUM_ERR_VC3_ST_M) {
        mipi_err_cnt[port_id].vc3_err_frame_num_cnt++;
    }
    if (err & CFG_FRAME_S_E_NUM_MISMATCH_VC0_ST_M) {
        mipi_err_cnt[port_id].vc0_err_frame_s_e_num_mismatch_cnt++;
    }
    if (err & CFG_FRAME_S_E_NUM_MISMATCH_VC1_ST_M) {
        mipi_err_cnt[port_id].vc1_err_frame_s_e_num_mismatch_cnt++;
    }
    if (err & CFG_FRAME_S_E_NUM_MISMATCH_VC2_ST_M) {
        mipi_err_cnt[port_id].vc2_err_frame_s_e_num_mismatch_cnt++;
    }
    if (err & CFG_FRAME_S_E_NUM_MISMATCH_VC3_ST_M) {
        mipi_err_cnt[port_id].vc3_err_frame_s_e_num_mismatch_cnt++;
    }
}

static td_void mipirx_mipi_ctrl_count_fifo_err(td_u32 err, td_u8 port_id)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);
    if (err & CFG_INT_CFIFO_WRERR_ST_M) {
        mipi_err_cnt[port_id].cmd_fifo_wrerr_cnt++;
    }
    if (err & CFG_INT_DFIFO_WRERR_ST_M) {
        mipi_err_cnt[port_id].data_fifo_wrerr_cnt++;
    }
    if (err & CFG_INT_VSYNC_ST_M) {
        mipi_err_cnt[port_id].vsync_cnt++;
        if (mipi_err_cnt[port_id].vsync_cnt >= MIPIRX_FRAME_CNT) {
            hal_mipirx_reg_writeb(ctrl_addr, REG_MIPI_CTRL_INT_MSK, CFG_INT_VSYNC_MSK_M, 0);
        }
    }
    if (err & CFG_MEMORY_NOEMPTY_WRITE_ST_M) {
        mipi_err_cnt[port_id].data_fifo_not_empty_cnt++;
    }
    if (err & CFG_CMDFIFO_NOEMPTY_ST_M) {
        mipi_err_cnt[port_id].cmd_fifo_not_empty_cnt++;
    }
    if (err & CFG_INT_CFIFO_RDERR_ST_M) {
        mipi_err_cnt[port_id].cmd_fifo_rderr_cnt++;
    }
    if (err & CFG_INT_DFIFO_RDERR_ST_M) {
        mipi_err_cnt[port_id].data_fifo_rderr_cnt++;
    }
}

td_void hal_mipirx_mipi_ctrl_count_err(td_u8 port_id)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);
    td_u32 *global_addr = hal_mipirx_get_global_addr();
    td_u32 err, global_err;

    global_err = hal_mipirx_reg_read(ctrl_addr, REG_CHN_INT_RAW);
    if (global_err == 0) {
        return;
    }
    hal_mipirx_reg_write(ctrl_addr, REG_CHN_INT_RAW, 0xFFFFFFFF);
    err = hal_mipirx_reg_read(ctrl_addr, REG_MIPI_CRC_INTR_ST);
    if (err != 0) {
        hal_mipirx_reg_write(ctrl_addr, REG_MIPI_CRC_INTR_RAW, 0xFFFFFFFF);
        mipirx_mipi_ctrl_count_crc_err(err, port_id);
        mipirx_mipi_ctrl_count_ecc_err(err, port_id);
        mipirx_mipi_ctrl_count_frame_err(err, port_id);
    }

    err = hal_mipirx_reg_read(ctrl_addr, REG_MIPI_CTRL_INT);
    if (err != 0) {
        hal_mipirx_reg_write(ctrl_addr, REG_MIPI_CTRL_INT_RAW, 0xFFFFFFFF);
        mipirx_mipi_ctrl_count_fifo_err(err, port_id);
    }
    hal_mipirx_reg_write(global_addr, REG_MIPI_INT_RAW, 0xFFFFFFFF);
}

static td_void mipirx_align_err_cnt(td_u32 err, td_u32 port_id)
{
    if (err & CFG_ERR_FULL_ST_M) {
        align_err[port_id].fifo_ovfl++;
    }
    if (err & CFG_ERR_LANE0_ST_M) {
        align_err[port_id].lane0_fifo_ovfl++;
    }
    if (err & CFG_ERR_LANE1_ST_M) {
        align_err[port_id].lane1_fifo_ovfl++;
    }
    if (err & CFG_ERR_LANE2_ST_M) {
        align_err[port_id].lane2_fifo_ovfl++;
    }
    if (err & CFG_ERR_LANE3_ST_M) {
        align_err[port_id].lane3_fifo_ovfl++;
    }
}

td_void hal_mipirx_align_err(td_u8 port_id)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);
    td_u32 err;

    err = hal_mipirx_reg_read(ctrl_addr, REG_ALIGN_INT);
    if (err != 0) {
        hal_mipirx_reg_write(ctrl_addr, REG_ALIGN_INT_RAW, 0xFFFFFFFF);
        mipirx_align_err_cnt(err, port_id);
    }
}

td_void hal_mipirx_align_irq_open(td_u8 port_id)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);

    hal_mipirx_reg_write(ctrl_addr, REG_ALIGN_INT_MSK, 0x1f);
    hal_mipirx_reg_write(ctrl_addr, REG_CHN_INT_MASK, 0xf);
}

td_void hal_mipirx_mipi_ctrl_irq_open(td_u8 port_id)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);
    td_u32 *global_addr = hal_mipirx_get_global_addr();
    hal_mipirx_reg_write(ctrl_addr, REG_MIPI_CTRL_INT_MSK, 0x30113);
    hal_mipirx_reg_write(ctrl_addr, REG_MIPI_CRC_INTR_MSK, 0xff01ff);
    hal_mipirx_reg_writeb(global_addr, REG_MIPI_INT_MSK, CFG_INT_CHN0_MASK_M << port_id, TD_TRUE);
}

td_void hal_mipirx_mipi_ctrl_clean_err_cnt(td_void)
{
    memset_s(mipi_err_cnt, sizeof(mipi_err_cnt), 0, sizeof(mipi_err_cnt));
    memset_s(align_err, sizeof(align_err), 0, sizeof(align_err));
}
