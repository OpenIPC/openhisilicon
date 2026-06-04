/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include "drv_mipirx_osal.h"
#include "hal_mipirx_custom.h"
#include "drv_mipirx_comm.h"
#include "drv_mipirx_kapi.h"
#include "drv_mipirx_kapi_struct.h"
#include "ot_mipi_rx.h"
#include "drv_mipirx_ioctl.h"

#define OT_MEDIA_DYNAMIC_MINOR 255
#define CHECK_DATA_TYPE(src, dst) do { \
    if ((src) == (dst)) { \
        return TD_TRUE; \
    }} while (0)

static osal_dev *g_mipi_rx_dev = TD_NULL;

static int mipirx_open(td_void *private_data)
{
    ot_unused(private_data);
    return 0;
}

static int mipirx_release(td_void *private_data)
{
    ot_unused(private_data);
    return 0;
}

static osal_fileops g_mipirx_fops = {
    .open = mipirx_open,
    .release = mipirx_release,
};

#ifdef CONFIG_OT_SYS_KOL_SUPPORT
td_s32 mipirx_suspend(void *private_data);
td_s32 mipirx_resume(void *private_data);
static osal_pmops g_mipirx_pm_ops = {
    .pm_suspend = mipirx_suspend,
    .pm_resume = mipirx_resume,
};
#endif

#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
static td_void mipirx_ioctl_lvds_set_sync_code(mipirx_port_cfg *attr, combo_dev_attr_t *data)
{
    td_u8 i, j;
    for (i = 0; i < MIPIRX_LANE_NUM; i++) {
        for (j = 0; j < MIPIRX_WDR_VC_NUM; j++) {
            memcpy_s(attr->lvds_attr.sync_code[i][j], sizeof(attr->lvds_attr.sync_code[i][j]),
                data->lvds_attr.sync_code[i][j], sizeof(data->lvds_attr.sync_code[i][j]));
        }
    }
}

static td_void mipirx_ioctl_set_lvds_attr(mipirx_port_cfg *attr, combo_dev_attr_t *data)
{
    lvds_wdr_mode_t wdr_mode;
    lvds_vsync_type_t sync_type;

    attr->lvds_attr.sync_mode = (mipirx_lvds_sync_mode)data->lvds_attr.sync_mode;
    wdr_mode = data->lvds_attr.wdr_mode;
    sync_type = data->lvds_attr.vsync_attr.sync_type;
    if (wdr_mode == OT_LVDS_WDR_MODE_NONE) {
        attr->lvds_attr.wdr_attr.wdr_type = LVDS_WDR_NONE;
        attr->lvds_attr.wdr_attr.wdr_num = LVDS_WDR_NUM_NONE;
    } else if (wdr_mode == OT_LVDS_WDR_MODE_2F && sync_type == LVDS_VSYNC_NORMAL) {
        attr->lvds_attr.wdr_attr.wdr_type = LVDS_WDR_INDEPENDENT_SOF;
        attr->lvds_attr.wdr_attr.wdr_num = LVDS_WDR_2F;
    } else if (wdr_mode == OT_LVDS_WDR_MODE_2F && sync_type == LVDS_VSYNC_SHARE) {
        attr->lvds_attr.wdr_attr.wdr_type = LVDS_WDR_SHARE_SOF;
        attr->lvds_attr.wdr_attr.wdr_num = LVDS_WDR_2F;
    } else if (wdr_mode == OT_LVDS_WDR_MODE_DOL_2F && sync_type == LVDS_VSYNC_NORMAL
               && data->lvds_attr.fid_attr.fid_type == LVDS_FID_IN_SAV) {
        attr->lvds_attr.wdr_attr.wdr_type = LVDS_WDR_DOL_4TH_CODE;
        attr->lvds_attr.wdr_attr.wdr_num = LVDS_WDR_2F;
    } else if (wdr_mode == OT_LVDS_WDR_MODE_DOL_2F && sync_type == LVDS_VSYNC_NORMAL
               && data->lvds_attr.fid_attr.fid_type == LVDS_FID_IN_DATA) {
        attr->lvds_attr.wdr_attr.wdr_type = LVDS_WDR_DOL_5TH_CODE;
        attr->lvds_attr.wdr_attr.wdr_num = LVDS_WDR_2F;
    } else if (wdr_mode == OT_LVDS_WDR_MODE_DOL_2F && sync_type == LVDS_VSYNC_HCONNECT
               && data->lvds_attr.fid_attr.fid_type == LVDS_FID_NONE) {
        attr->lvds_attr.wdr_attr.wdr_type = LVDS_WDR_DOL_HCONNECT;
        attr->lvds_attr.wdr_attr.wdr_num = LVDS_WDR_2F;
        attr->lvds_attr.wdr_attr.hblank1 = data->lvds_attr.vsync_attr.hblank1;
        attr->lvds_attr.wdr_attr.hblank2 = data->lvds_attr.vsync_attr.hblank2;
    } else {
        attr->lvds_attr.wdr_attr.wdr_type = LVDS_WDR_TYPE_MAX;
        attr->lvds_attr.wdr_attr.wdr_num = LVDS_WDR_NUM_MAX;
    }
    attr->lvds_attr.data_endian = (mipirx_lvds_endian)data->lvds_attr.data_endian;
    attr->lvds_attr.sync_code_endian = (mipirx_lvds_endian)data->lvds_attr.sync_code_endian;
    mipirx_ioctl_lvds_set_sync_code(attr, data);
}

static td_s32 mipirx_ioctl_check_lvds_wdr(combo_dev_attr_t *wdr)
{
    if (wdr->lvds_attr.wdr_mode == OT_LVDS_WDR_MODE_2F) {
        // invalid sync_type, must be LVDS_VSYNC_NORMAL or LVDS_VSYNC_SHARE
        if (wdr->lvds_attr.vsync_attr.sync_type != LVDS_VSYNC_NORMAL &&
            wdr->lvds_attr.vsync_attr.sync_type != LVDS_VSYNC_SHARE) {
            return OT_ERR_MIPIRX_ILLEGAL_PARAM;
        }
    } else if (wdr->lvds_attr.wdr_mode == OT_LVDS_WDR_MODE_DOL_2F) {
        // if using WDR-DOL, must be LVDS_SYNC_MODE_SAV
        if (wdr->lvds_attr.sync_mode != LVDS_SYNC_MODE_SAV) {
            return OT_ERR_MIPIRX_ILLEGAL_PARAM;
        }
        if (wdr->lvds_attr.vsync_attr.sync_type == LVDS_VSYNC_NORMAL) {
            // invalid fid_type, must be LVDS_FID_IN_SAV or LVDS_FID_IN_DATA
            if (wdr->lvds_attr.fid_attr.fid_type != LVDS_FID_IN_SAV &&
                wdr->lvds_attr.fid_attr.fid_type != LVDS_FID_IN_DATA) {
                return OT_ERR_MIPIRX_ILLEGAL_PARAM;
            }
        } else if (wdr->lvds_attr.vsync_attr.sync_type == LVDS_VSYNC_HCONNECT) {
            // invalid fid_type, must be LVDS_FID_NONE or LVDS_FID_IN_DATA
            if (wdr->lvds_attr.fid_attr.fid_type != LVDS_FID_NONE &&
                wdr->lvds_attr.fid_attr.fid_type != LVDS_FID_IN_DATA) {
                return OT_ERR_MIPIRX_ILLEGAL_PARAM;
            }
        } else { // invalid sync_type, must be LVDS_VSYNC_NORMAL or LVDS_VSYNC_HCONNECT
            return OT_ERR_MIPIRX_ILLEGAL_PARAM;
        }
    }
    return TD_SUCCESS;
}

static td_s32 mipirx_ioctl_convert_cfg_lvds_info(combo_dev_attr_t *input_cfg, mipirx_port_cfg * port_cfg)
{
    td_s32 ret;
    td_u8 i;

    port_cfg->data_type = (mipirx_data_type) input_cfg->lvds_attr.input_data_type;
    for (i = 0; i < MIPIRX_TOTAL_LANE_NUM_MAX; i++) {
        port_cfg->lane_id[i] = input_cfg->lvds_attr.lane_id[i];
    }

    ret = mipirx_ioctl_check_lvds_wdr(input_cfg);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    mipirx_ioctl_set_lvds_attr(port_cfg, input_cfg);
    return TD_SUCCESS;
}
#else
static td_s32 mipirx_ioctl_convert_cfg_lvds_info(combo_dev_attr_t *input_cfg, mipirx_port_cfg * port_cfg)
{
    ot_unused(input_cfg);
    ot_unused(port_cfg);
    mipirx_log_err("mipirx lvds mode not support!\n");
    return OT_ERR_MIPIRX_MODE_NOT_SUPPORT;
}
#endif

static td_s32 mipirx_ioctl_check_data_type_support(mipirx_data_type data_type)
{
#ifdef CONFIG_OT_MIPIRX_SUPPORT_DATA_TYPE_RAW_8BIT
    CHECK_DATA_TYPE(data_type, MIPIRX_DATA_TYPE_RAW_8BIT);
#endif
#ifdef CONFIG_OT_MIPIRX_SUPPORT_DATA_TYPE_RAW_10BIT
    CHECK_DATA_TYPE(data_type, MIPIRX_DATA_TYPE_RAW_10BIT);
#endif
#ifdef CONFIG_OT_MIPIRX_SUPPORT_DATA_TYPE_RAW_12BIT
    CHECK_DATA_TYPE(data_type, MIPIRX_DATA_TYPE_RAW_12BIT);
#endif
#ifdef CONFIG_OT_MIPIRX_SUPPORT_DATA_TYPE_RAW_14BIT
    CHECK_DATA_TYPE(data_type, MIPIRX_DATA_TYPE_RAW_14BIT);
#endif
#ifdef CONFIG_OT_MIPIRX_SUPPORT_DATA_TYPE_YUV420_8BIT_NORMAL
    CHECK_DATA_TYPE(data_type, MIPIRX_DATA_TYPE_YUV420_8BIT_NORMAL);
#endif
#ifdef CONFIG_OT_MIPIRX_SUPPORT_DATA_TYPE_YUV420_8BIT_LEGACY
    CHECK_DATA_TYPE(data_type, MIPIRX_DATA_TYPE_YUV420_8BIT_LEGACY);
#endif
#ifdef CONFIG_OT_MIPIRX_SUPPORT_DATA_TYPE_YUV422_8BIT
    CHECK_DATA_TYPE(data_type, MIPIRX_DATA_TYPE_YUV422_8BIT);
#endif
#ifdef CONFIG_OT_MIPIRX_SUPPORT_DATA_TYPE_YUV422_PACKED
    CHECK_DATA_TYPE(data_type, MIPIRX_DATA_TYPE_YUV422_PACKED);
#endif
    return TD_FALSE;
}

static td_s32 mipirx_ioctl_convert_cfg_mipi_info(combo_dev_attr_t *input_cfg, mipirx_port_cfg * port_cfg)
{
    td_u8 i;

    port_cfg->data_type = (mipirx_data_type) input_cfg->mipi_attr.input_data_type;
    if (mipirx_ioctl_check_data_type_support(port_cfg->data_type) != TD_TRUE) {
        mipirx_log_err("mipirx data type not support!\n");
        return OT_ERR_MIPIRX_MODE_NOT_SUPPORT;
    }
    port_cfg->mipi_attr.wdr_mode = (mipirx_mipi_wdr_mode) input_cfg->mipi_attr.wdr_mode;
    memcpy_s(port_cfg->mipi_attr.wdr_data_type, sizeof(port_cfg->mipi_attr.wdr_data_type),
        input_cfg->mipi_attr.data_type, sizeof(port_cfg->mipi_attr.wdr_data_type));
    for (i = 0; i < MIPIRX_TOTAL_LANE_NUM_MAX; i++) {
        port_cfg->lane_id[i] = input_cfg->mipi_attr.lane_id[i];
    }
    return TD_SUCCESS;
}

static td_s32 mipirx_ioctl_convert_cfg_info(combo_dev_attr_t *input_cfg, mipirx_port_cfg *port_cfg)
{
    port_cfg->port_id = (td_u8)input_cfg->devno;
    port_cfg->work_mode = (mipirx_work_mode)input_cfg->input_mode;
    if (input_cfg->img_rect.x < 0 || input_cfg->img_rect.y < 0) {
        return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }
    port_cfg->img_rect.x = input_cfg->img_rect.x;
    port_cfg->img_rect.y = input_cfg->img_rect.y;
    port_cfg->img_rect.width = input_cfg->img_rect.width;
    port_cfg->img_rect.height = input_cfg->img_rect.height;
    port_cfg->data_rate = (mipirx_data_rate)input_cfg->data_rate;

    if (port_cfg->work_mode == MIPIRX_INPUT_MODE_MIPI) {
        return mipirx_ioctl_convert_cfg_mipi_info(input_cfg, port_cfg);
    } else if (port_cfg->work_mode >= MIPIRX_INPUT_MODE_SUBLVDS && port_cfg->work_mode <= MIPIRX_INPUT_MODE_HISPI) {
        return mipirx_ioctl_convert_cfg_lvds_info(input_cfg, port_cfg);
    } else {
        return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }
}

static td_s32 mipirx_ioctl_set_dev_attr(td_u32 cmd, td_void *arg, td_void *private_data)
{
    td_s32 ret;
    combo_dev_attr_t *input_cfg;
    mipirx_port_cfg port_cfg = {0};

    ot_unused(cmd);
    ot_unused(private_data);
    if (arg == TD_NULL) {
        return OT_ERR_MIPIRX_NULL_PTR;
    }
    input_cfg = (combo_dev_attr_t *)arg;
    ret = mipirx_ioctl_convert_cfg_info(input_cfg, &port_cfg);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    ret = drv_mipirx_kapi_set_attr(&port_cfg);
    if (ret != TD_SUCCESS) {
        mipirx_log_err("mipirx ioctl set port_cfg failed!\n");
        return ret;
    }

    return TD_SUCCESS;
}

static td_s32 mipi_rx_ioctl_set_ext_data_type(td_u32 cmd, td_void *arg, td_void *private_data)
{
    ext_data_type_t *data = (ext_data_type_t *) arg;
    td_u8 id;
    td_u8 i;
    mipirx_mipi_udf_dt data_type_info = {0};

    ot_unused(cmd);
    ot_unused(private_data);

    if (arg == TD_NULL) {
        return OT_ERR_MIPIRX_NULL_PTR;
    }
    id = (td_u8) data->devno;
    data_type_info.num = data->num;
    for (i = 0; i < MIPIRX_MAX_EXT_DATA_TYPE_NUM; i++) {
        data_type_info.data_bit_width[i] = data->ext_data_bit_width[i];
        data_type_info.data_type[i] = data->ext_data_type[i];
    }

    return drv_mipirx_kapi_set_user_def_dt(id, &data_type_info);
}

#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
static td_s32 mipi_rx_ioctl_set_phy_cmvmode(td_u32 cmd, td_void *arg, td_void *private_data)
{
    phy_cmv_t *data;
    mipirx_work_mode mode;
    td_u8 id;

    if (arg == TD_NULL) {
        return OT_ERR_MIPIRX_NULL_PTR;
    }
    data = (phy_cmv_t *)arg;
    id = (td_u8)data->devno;
    ot_unused(cmd);
    ot_unused(private_data);
    switch (data->cmv_mode) {
        case PHY_CMV_GE1200MV:
            mode = MIPIRX_INPUT_MODE_LVDS;
            break;
        case PHY_CMV_LT1200MV:
            mode = MIPIRX_INPUT_MODE_SUBLVDS;
            break;
        default:
            return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }

    return drv_mipirx_kapi_set_phy_mode(id, mode);
}
#else
static td_s32 mipi_rx_ioctl_set_phy_cmvmode(td_u32 cmd, td_void *arg, td_void *private_data)
{
    ot_unused(cmd);
    ot_unused(arg);
    ot_unused(private_data);
    mipirx_log_err("mipirx lvds mode not support!\n");
    return OT_ERR_MIPIRX_ILLEGAL_PARAM;
}
#endif

static td_s32 mipi_rx_ioctl_reset_sensor(td_u32 cmd, td_void *arg, td_void *private_data)
{
    sns_rst_source_t port_id;

    if (arg == TD_NULL) {
        return OT_ERR_MIPIRX_NULL_PTR;
    }
    ot_unused(cmd);
    ot_unused(private_data);

    port_id = *(sns_rst_source_t *)arg;

    return drv_mipirx_kapi_reset_sensor((td_u8)port_id, TD_TRUE);
}

static td_s32 mipi_rx_ioctl_unreset_sensor(td_u32 cmd, td_void *arg, td_void *private_data)
{
    sns_rst_source_t port_id;
    td_s32 ret;

    if (arg == TD_NULL) {
        return OT_ERR_MIPIRX_NULL_PTR;
    }
    ot_unused(cmd);
    ot_unused(private_data);

    port_id = *(sns_rst_source_t *) arg;
    ret = drv_mipirx_kapi_reset_sensor((td_u8) port_id, TD_FALSE);
    return ret;
}

static td_s32 mipi_rx_ioctl_reset_mipi(td_u32 cmd, td_void *arg, td_void *private_data)
{
    td_s32 ret;
    combo_dev_t port_id;

    if (arg == TD_NULL) {
        return OT_ERR_MIPIRX_NULL_PTR;
    }
    ot_unused(cmd);
    ot_unused(private_data);
    port_id = *(combo_dev_t *)arg;
    ret = drv_mipirx_kapi_reset_mipi((td_u8)port_id, TD_TRUE);
    if (ret != TD_SUCCESS) {
        mipirx_log_err("mipirx%u reset failed!\n", port_id);
        return ret;
    }
    return TD_SUCCESS;
}

static td_s32 mipi_rx_ioctl_unreset_mipi(td_u32 cmd, td_void *arg, td_void *private_data)
{
    td_s32 ret;
    combo_dev_t port_id;

    if (arg == TD_NULL) {
        return OT_ERR_MIPIRX_NULL_PTR;
    }
    port_id = *(combo_dev_t *)arg;
    ot_unused(cmd);
    ot_unused(private_data);

    ret = drv_mipirx_kapi_reset_mipi((td_u8)port_id, TD_FALSE);
    if (ret != TD_SUCCESS) {
        mipirx_log_err("mipirx%u unreset failed!\n", port_id);
        return ret;
    }
    return TD_SUCCESS;
}

static td_s32 mipi_rx_ioctl_set_hs_mode(td_u32 cmd, td_void *arg, td_void *private_data)
{
    lane_divide_mode_t mode;
    mipirx_lane_mode kapi_lane_mode = LANE_DIVIDE_MODE_MAX;
    ot_unused(cmd);
    ot_unused(private_data);

    if (arg == TD_NULL) {
        return OT_ERR_MIPIRX_NULL_PTR;
    }

    mode = *(lane_divide_mode_t *)arg;
    if (mode == LANE_DIVIDE_MODE_0) {
        kapi_lane_mode = MODE_4_LANE;
    } else if (mode == LANE_DIVIDE_MODE_1) {
        kapi_lane_mode = MODE_2_PLUS_2_LANE;
    } else {
        return OT_ERR_MIPIRX_ILLEGAL_PARAM;
    }

    return drv_mipirx_kapi_set_lane_mode(kapi_lane_mode);
}

static td_s32 mipi_rx_ioctl_enable_mipi_clock(td_u32 cmd, td_void *arg, td_void *private_data)
{
    combo_dev_t port_id;

    if (arg == TD_NULL) {
        return OT_ERR_MIPIRX_NULL_PTR;
    }
    ot_unused(cmd);
    ot_unused(private_data);

    port_id = *(combo_dev_t *) arg;
    return drv_mipirx_kapi_enable_mipi_clock((td_u8) port_id, TD_TRUE);
}

static td_s32 mipi_rx_ioctl_disable_mipi_clock(td_u32 cmd, td_void *arg, td_void *private_data)
{
    combo_dev_t port_id;

    if (arg == TD_NULL) {
        return OT_ERR_MIPIRX_NULL_PTR;
    }
    ot_unused(cmd);
    ot_unused(private_data);

    port_id = *(combo_dev_t *)arg;
    return drv_mipirx_kapi_enable_mipi_clock((td_u8)port_id, TD_FALSE);
}

static td_s32 mipi_rx_ioctl_enable_sensor_clock(td_u32 cmd, td_void *arg, td_void *private_data)
{
    sns_clk_source_t port_id;

    if (arg == TD_NULL) {
        return OT_ERR_MIPIRX_NULL_PTR;
    }
    ot_unused(cmd);
    ot_unused(private_data);

    port_id = *(sns_clk_source_t *)arg;
    return drv_mipirx_kapi_enable_sensor_clock((td_u8)port_id, TD_TRUE);
}

static td_s32 mipi_rx_ioctl_disable_sensor_clock(td_u32 cmd, td_void *arg, td_void *private_data)
{
    sns_clk_source_t port_id;

    if (arg == TD_NULL) {
        return OT_ERR_MIPIRX_NULL_PTR;
    }
    ot_unused(cmd);
    ot_unused(private_data);

    port_id = *(sns_clk_source_t *)arg;
    return drv_mipirx_kapi_enable_sensor_clock((td_u8)port_id, TD_FALSE);
}

static osal_ioctl_cmd g_mipirx_cmd_list[] = {
    { OT_MIPI_SET_DEV_ATTR,        mipirx_ioctl_set_dev_attr },
    { OT_MIPI_SET_EXT_DATA_TYPE,   mipi_rx_ioctl_set_ext_data_type },
    { OT_MIPI_SET_PHY_CMVMODE,     mipi_rx_ioctl_set_phy_cmvmode },
    { OT_MIPI_RESET_SENSOR,        mipi_rx_ioctl_reset_sensor },
    { OT_MIPI_UNRESET_SENSOR,      mipi_rx_ioctl_unreset_sensor },
    { OT_MIPI_RESET_MIPI,          mipi_rx_ioctl_reset_mipi },
    { OT_MIPI_UNRESET_MIPI,        mipi_rx_ioctl_unreset_mipi },
    { OT_MIPI_SET_HS_MODE,         mipi_rx_ioctl_set_hs_mode },
    { OT_MIPI_ENABLE_MIPI_CLOCK,   mipi_rx_ioctl_enable_mipi_clock },
    { OT_MIPI_DISABLE_MIPI_CLOCK,  mipi_rx_ioctl_disable_mipi_clock },
    { OT_MIPI_ENABLE_SENSOR_CLOCK, mipi_rx_ioctl_enable_sensor_clock },
    { OT_MIPI_DISABLE_SENSOR_CLOCK, mipi_rx_ioctl_disable_sensor_clock },
};

void mipirx_set_ioctl_cmd_list(osal_fileops *mipi_rx_fops)
{
    mipi_rx_fops->cmd_list = g_mipirx_cmd_list;
    mipi_rx_fops->cmd_cnt = sizeof(g_mipirx_cmd_list) / sizeof(g_mipirx_cmd_list[0]);
}

static td_s32 mipirx_register_device(td_void)
{
    td_s32 ret;

    g_mipi_rx_dev = osal_dev_create(MIPIRX_DEV_NAME);
    if (g_mipi_rx_dev == TD_NULL) {
        return OT_ERR_MIPIRX_NULL_PTR;
    }
    mipirx_set_ioctl_cmd_list(&g_mipirx_fops);
    g_mipi_rx_dev->fops = &g_mipirx_fops;
    g_mipi_rx_dev->minor = OT_MEDIA_DYNAMIC_MINOR;
#ifdef CONFIG_OT_SYS_KOL_SUPPORT
    g_mipi_rx_dev->pmops = &g_mipirx_pm_ops;
#endif

    ret = osal_dev_register(g_mipi_rx_dev);
    if (ret < 0) {
        mipirx_log_err("mipirx dev ioctl register device failed\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_void mipirx_destroy_device(td_void)
{
    osal_dev_unregister(g_mipi_rx_dev);
    osal_dev_destroy(g_mipi_rx_dev);
    g_mipi_rx_dev = TD_NULL;
}

td_s32 drv_mipirx_ioctl_init(td_void)
{
    td_s32 ret;

    ret = mipirx_register_device();
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return TD_SUCCESS;
}

td_void drv_mipirx_ioctl_deinit(td_void)
{
    mipirx_destroy_device();
}

#ifdef CONFIG_OT_SYS_KOL_SUPPORT
static td_u32 mipirx_get_max_port_id(td_void)
{
    mipirx_lane_mode lane_mode;

    lane_mode = drv_mipirx_get_lane_mode();
    if (lane_mode == MODE_4_LANE) {
        return 1;
    } else if (lane_mode == MODE_2_PLUS_2_LANE) {
        return 2; /* 2 sensor */
    } else {
        return 0;
    }
}

td_s32 mipirx_suspend(void *private_data)
{
    td_u8 port_id;
    td_u32 max_port_id;

    if (drv_mipirx_is_init() != TD_TRUE) {
        return TD_SUCCESS;
    }

    max_port_id = mipirx_get_max_port_id();
    for (port_id = 0; port_id < max_port_id; port_id++) {
        (td_void)drv_mipirx_kapi_reset_mipi(port_id, TD_TRUE); // reset mipi
        (td_void)drv_mipirx_kapi_enable_mipi_clock(port_id, TD_FALSE); // disable mipi clk
    }
    drv_mipirx_kapi_crg_enable_sys_clock(TD_FALSE); // disable mipi bus
    return TD_SUCCESS;
}

td_s32 mipirx_resume(void *private_data)
{
    td_u8 port_id;
    td_u32 max_port_id;

    if (drv_mipirx_is_init() != TD_TRUE) {
        return TD_SUCCESS;
    }

    drv_mipirx_kapi_crg_enable_sys_clock(TD_TRUE); // enable mipi bus
    drv_mipirx_kapi_resume_set_lane_mode();

    max_port_id = mipirx_get_max_port_id();
    for (port_id = 0; port_id < max_port_id; port_id++) {
        (td_void)drv_mipirx_kapi_enable_mipi_clock(port_id, TD_TRUE); // enable mipi clk
        (td_void)drv_mipirx_kapi_reset_mipi(port_id, TD_TRUE); // reset mipi
        (td_void)drv_mipirx_kapi_resume_set_attr(port_id); // set attr use ctx's mipirx_port_cfg
        (td_void)drv_mipirx_kapi_resume_set_user_def_dt(port_id); // set ext data type attr use ctx's udf_dt
        (td_void)drv_mipirx_kapi_reset_mipi(port_id, TD_FALSE); // unreset mipi
    }
    return TD_SUCCESS;
}
#endif
