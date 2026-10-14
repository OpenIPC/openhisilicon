#include <stdlib.h>
#include <string.h>
#include "cv2005.h"
#include "cv2005_ctrl.h"
#include "cv2005_ex.h"
#include "xmedia_isp.h"

#define CV2005_NAME                  "CV2005"
#define CV2005_SPECS_MAX_NUM         1
#define CV2005_HMAX_1920X1080_LINEAR 0x44E

#ifdef __linux__
#define CV2005_ISP_DEFAULT_SUPPORT
#endif

#define CV2005_2M_1080P_BIT_RATE_LINEAR 390

#define CV2005_REG_ADDR_STANDBY 0x3000

#define CV2005_RES_IS_1920X1080(w, h) ((w) == 1920 && (h) == 1080)

#define CV2005_ERR_MODE_PRINT(sns_attr)                                                                                \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sns_attr->width,                \
                     sns_attr->height, sns_attr->wdr_mode);                                                            \
    } while (0)

// awb static param for Fuji Lens New IR_Cut
#define CV2005_CALIBRATE_STATIC_TEMP       5000
#define CV2005_CALIBRATE_STATIC_WB_R_GAIN  369
#define CV2005_CALIBRATE_STATIC_WB_GR_GAIN 256
#define CV2005_CALIBRATE_STATIC_WB_GB_GAIN 256
#define CV2005_CALIBRATE_STATIC_WB_B_GAIN  660

// Calibration results for Auto WB Planck
#define CV2005_CALIBRATE_AWB_P1 108
#define CV2005_CALIBRATE_AWB_P2 8
#define CV2005_CALIBRATE_AWB_Q1 (-140)
#define CV2005_CALIBRATE_AWB_A1 219225
#define CV2005_CALIBRATE_AWB_B1 128
#define CV2005_CALIBRATE_AWB_C1 (-152450)

// Rgain and Bgain of the golden sample
#define CV2005_GOLDEN_RGAIN 0
#define CV2005_GOLDEN_BGAIN 0

#define CV2005_AGAIN_INDEX_MAX 249

sensor_context *g_cv2005_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define CV2005_SET_CTX(dev, sns_ctx) g_cv2005_ctx[dev] = sns_ctx
#define CV2005_GET_CTX(dev, sns_ctx) sns_ctx = g_cv2005_ctx[dev]

static xmedia_u32 g_cv2005_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_cv2005_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_cv2005_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_cv2005_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };
static xmedia_s32 g_cv2005_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

static const xmedia_sensor_property g_cv2005_property[CV2005_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB,
          XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, CV2005_2M_1080P_BIT_RATE_LINEAR,
    },
};

static xmedia_u32 g_cv2005_again_table[CV2005_AGAIN_INDEX_MAX] = {
    1024,  1028,  1032,  1036,  1040,  1044,  1049,  1053,  1057,  1061,  1066,  1070,  1074,  1078,  1083,  1087,
    1093,  1097,  1102,  1106,  1111,  1115,  1120,  1125,  1129,  1135,  1140,  1145,  1150,  1155,  1160,  1165,
    1170,  1176,  1181,  1186,  1192,  1197,  1202,  1208,  1213,  1220,  1225,  1231,  1237,  1242,  1248,  1254,
    1261,  1267,  1273,  1279,  1285,  1291,  1297,  1305,  1311,  1317,  1324,  1330,  1337,  1345,  1352,  1358,
    1365,  1372,  1379,  1386,  1395,  1402,  1409,  1417,  1424,  1433,  1441,  1448,  1456,  1464,  1473,  1481,
    1490,  1498,  1506,  1516,  1524,  1533,  1542,  1551,  1561,  1570,  1579,  1589,  1598,  1609,  1618,  1628,
    1638,  1649,  1659,  1670,  1680,  1692,  1702,  1713,  1724,  1736,  1748,  1759,  1772,  1783,  1795,  1808,
    1821,  1833,  1846,  1860,  1873,  1886,  1900,  1914,  1927,  1942,  1956,  1971,  1986,  2001,  2016,  2032,
    2048,  2064,  2081,  2097,  2115,  2131,  2148,  2167,  2184,  2203,  2221,  2241,  2260,  2279,  2300,  2319,
    2341,  2361,  2383,  2405,  2427,  2450,  2473,  2497,  2521,  2545,  2570,  2596,  2621,  2648,  2675,  2702,
    2731,  2760,  2788,  2819,  2850,  2881,  2912,  2945,  2979,  3014,  3048,  3084,  3121,  3158,  3197,  3236,
    3277,  3319,  3361,  3405,  3449,  3495,  3542,  3591,  3641,  3693,  3745,  3799,  3855,  3913,  3972,  4033,
    4096,  4161,  4228,  4298,  4369,  4443,  4520,  4599,  4681,  4767,  4855,  4946,  5041,  5140,  5243,  5349,
    5461,  5578,  5699,  5826,  5958,  6096,  6241,  6394,  6554,  6722,  6899,  7085,  7282,  7490,  7710,  7944,
    8192,  8456,  8738,  9040,  9362,  9709,  10082, 10486, 10923, 11397, 11915, 12483, 13107, 13797, 14563, 15420,
    16384, 17477, 18725, 20165, 21845, 23832, 26214, 29127, 32768
};

static const xmedia_sensor_capability g_cv2005_capability = {
    .max_width   = 1920,
    .max_height  = 1080,
    .max_fps     = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported  = XMEDIA_FALSE,
    .mipi_lanes_supported  = XMEDIA_SENSOR_MIPI_LANES_2L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = CV2005_ADDR_BYTE,
        .data_byte_num     = CV2005_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { CV2005_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x01 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { CV2005_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x00 },
    },
};

static xmedia_sensor_trig_attr g_cv2005_trig_attr = {
    .trig_mode  = 0,
    .hs_enable  = XMEDIA_FALSE,
    .vs_enable  = XMEDIA_TRUE,
    .hs_invert  = 0,
    .vs_invert  = 0,

    .hs_time    = 0,
    .vs_time    = (1.0 / 30.0) * 1000000,
    .vs_active  = 1 * ((1.0 / ( CV2005_VMAX_1920X1080_LINEAR * 30.0 )) * 1000000), // the time of 1 line
};

static xmedia_s32 cv2005_ctx_init(xmedia_u32 dev)
{
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    CV2005_GET_CTX(dev, cv2005_ctx);
    if (cv2005_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    cv2005_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (cv2005_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(cv2005_ctx, 0, sizeof(sensor_context));
    cv2005_ctx->i2c_addr         = CV2005_I2C_ADDR;
    cv2005_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_2L;
    cv2005_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    cv2005_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    cv2005_ctx->size.width       = 1920;
    cv2005_ctx->size.height      = 1080;
    cv2005_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    cv2005_ctx->img_mode             = CV2005_2LANE_1920X1080_LINEAR_MODE;
    cv2005_ctx->fl_std               = CV2005_VMAX_1920X1080_LINEAR;
    cv2005_ctx->fl[SENSOR_CUR_FRAME] = CV2005_VMAX_1920X1080_LINEAR;
    cv2005_ctx->fl[SENSOR_PRE_FRAME] = CV2005_VMAX_1920X1080_LINEAR;

    CV2005_SET_CTX(dev, cv2005_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void cv2005_ctx_exit(xmedia_u32 dev)
{
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_FREE(cv2005_ctx);
    CV2005_SET_CTX(dev, XMEDIA_NULL);
}

static xmedia_s32 cv2005_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    for (i = 0; i < CV2005_SPECS_MAX_NUM; i++) {
        if (g_cv2005_property[i].width == cv2005_ctx->size.width &&
            g_cv2005_property[i].height == cv2005_ctx->size.height &&
            g_cv2005_property[i].wdr_mode == cv2005_ctx->wdr_mode) {
            memcpy(property, &g_cv2005_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n", cv2005_ctx->size.width,
                 cv2005_ctx->size.height, cv2005_ctx->wdr_mode);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 cv2005_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL && work_mode != XMEDIA_SENSOR_WORK_MODE_SLAVE) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    cv2005_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "No support mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    cv2005_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        cv2005_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        cv2005_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        cv2005_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 cv2005_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    cv2005_ctx->i2c_addr = slave_addr;
    ret                  = cv2005_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    *slave_addr = cv2005_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    cv2005_ctx->init_mode = init_mode;

    ret = cv2005_i2c_init(dev, cv2005_ctx->bus_info.i2c_dev, cv2005_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    cv2005_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    ret = cv2005_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    cv2005_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    cv2005_ctx->mirror_en = mirror_en;
    ret                   = cv2005_set_mirror_flip(dev, mirror_en, cv2005_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    cv2005_ctx->flip_en = flip_en;
    ret                 = cv2005_set_mirror_flip(dev, cv2005_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_set_image_mode(sensor_context *cv2005_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (cv2005_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (CV2005_RES_IS_1920X1080(sns_attr->width, sns_attr->height)) {
            *image_mode        = CV2005_2LANE_1920X1080_LINEAR_MODE;
            cv2005_ctx->fl_std = CV2005_VMAX_1920X1080_LINEAR;
        } else {
            CV2005_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        CV2005_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    cv2005_ctx->size.width  = sns_attr->width;
    cv2005_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            cv2005_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(cv2005_ctx->wdr_int_time, 0, sizeof(cv2005_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

static xmedia_void cv2005_init_common_reg_info(sensor_context *cv2005_ctx)
{
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_EXP_H].delay_frame_num = 2;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_EXP_H].reg_addr        = CV2005_REG_ADDR_EXP_H;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_EXP_M].delay_frame_num = 2;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_EXP_M].reg_addr        = CV2005_REG_ADDR_EXP_M;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_EXP_L].delay_frame_num = 2;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_EXP_L].reg_addr        = CV2005_REG_ADDR_EXP_L;

    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_AGAIN].delay_frame_num = 2;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_AGAIN].reg_addr        = CV2005_REG_ADDR_AGAIN;

    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_DGAIN_H].delay_frame_num = 2;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_DGAIN_H].reg_addr        = CV2005_REG_ADDR_DGAIN_H;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_DGAIN_L].delay_frame_num = 2;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_DGAIN_L].reg_addr        = CV2005_REG_ADDR_DGAIN_L;

    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_VMAX_H].delay_frame_num = 2;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_VMAX_H].reg_addr        = CV2005_REG_ADDR_VMAX_H;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_VMAX_M].delay_frame_num = 2;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_VMAX_M].reg_addr        = CV2005_REG_ADDR_VMAX_M;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_VMAX_L].delay_frame_num = 2;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_VMAX_L].reg_addr        = CV2005_REG_ADDR_VMAX_L;

    return;
}

static xmedia_s32 cv2005_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = cv2005_ctx->bus_info.i2c_dev;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = CV2005_REG_MAX_NUM;

    for (i = 0; i < cv2005_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = cv2005_ctx->i2c_addr;
        cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = CV2005_ADDR_BYTE;
        cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = CV2005_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    cv2005_init_common_reg_info(cv2005_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    // Set wdr mode
    ret = cv2005_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = cv2005_set_image_mode(cv2005_ctx, &cv2005_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    cv2005_ctx->fl[SENSOR_CUR_FRAME] = cv2005_ctx->fl_std;
    cv2005_ctx->fl[SENSOR_PRE_FRAME] = cv2005_ctx->fl[SENSOR_CUR_FRAME];

    ret = cv2005_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef CV2005_ISP_DEFAULT_SUPPORT
    // TO-DO: alg add param
    isp_default->ainr             = XMEDIA_NULL;
    isp_default->anti_false_color = XMEDIA_NULL;
    isp_default->blc              = &g_cv2005_blc_attr;
    isp_default->bnr              = XMEDIA_NULL;
    isp_default->ca               = XMEDIA_NULL;
    isp_default->clut_attr        = XMEDIA_NULL;
    isp_default->crosstalk        = XMEDIA_NULL;
    isp_default->csc              = XMEDIA_NULL;
    isp_default->dehaze           = XMEDIA_NULL;
    isp_default->demosaic         = XMEDIA_NULL;
    isp_default->dpc_dynamic      = XMEDIA_NULL;
    isp_default->dpc_static       = XMEDIA_NULL;
    isp_default->drc              = XMEDIA_NULL;
    isp_default->expander         = XMEDIA_NULL;
    isp_default->fpn              = XMEDIA_NULL;
    isp_default->gamma            = XMEDIA_NULL;
    isp_default->gcac             = XMEDIA_NULL;
    isp_default->hlc              = XMEDIA_NULL;
    isp_default->lcac             = XMEDIA_NULL;
    isp_default->lce              = XMEDIA_NULL;
    isp_default->mlsc_attr        = XMEDIA_NULL;
    isp_default->mlsc_lut         = XMEDIA_NULL;
    isp_default->radial_crop      = XMEDIA_NULL;
    isp_default->rgbir            = XMEDIA_NULL;
    isp_default->rlsc_attr        = XMEDIA_NULL;
    isp_default->rlsc_lut         = XMEDIA_NULL;
    isp_default->sharpen          = XMEDIA_NULL;
    isp_default->stnr             = XMEDIA_NULL;
    isp_default->wdr              = XMEDIA_NULL;
    return XMEDIA_SUCCESS;
#else
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from cv2005!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 cv2005_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (cv2005_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x104;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    if (cv2005_ctx->mirror_en && cv2005_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (cv2005_ctx->mirror_en && (!cv2005_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!cv2005_ctx->mirror_en) && cv2005_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < CV2005_SPECS_MAX_NUM; i++) {
        if (g_cv2005_property[i].width == cv2005_ctx->size.width &&
            g_cv2005_property[i].height == cv2005_ctx->size.height &&
            g_cv2005_property[i].wdr_mode == cv2005_ctx->wdr_mode) {
            *bayer_pattern = g_cv2005_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= CV2005_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_cv2005_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_get_trig_attr(xmedia_sensor_trig_attr *trig_attr)
{
    SENSOR_CHECK_PTR_RETURN(trig_attr);
    memcpy(trig_attr, &g_cv2005_trig_attr, sizeof(xmedia_sensor_trig_attr));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    cv2005_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 cv2005_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_cv2005_exposure[dev]      = init_param->exposure;
    g_cv2005_sample_r_gain[dev] = init_param->sample_rgain;
    g_cv2005_sample_b_gain[dev] = init_param->sample_bgain;

    g_cv2005_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_cv2005_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_cv2005_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256;
    ae_sns_dft->full_lines_max = CV2005_FULL_LINES_MAX;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 0;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy  = 0.015625; // 1 / 64

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 4 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->ispdgain_sep_thd    = 32 << ae_sns_dft->ispdgain_shift;

    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_32_0;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    // lines per second: VTS * max_fps
    ae_sns_dft->lines_per_500ms  = (sns_ctx->fl_std * 30) / 2;
    ae_sns_dft->man_ratio_enable = XMEDIA_FALSE;

    SENSOR_PRINT("man_ratio_enable: %d \n", ae_sns_dft->man_ratio_enable);

    return XMEDIA_SUCCESS;
}

static xmedia_void cv2005_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 32768;
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 2000;
    ae_sns_dft->min_dgain        = 64;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_cv2005_exposure[dev] ? g_cv2005_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - CV2005_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 2;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = 2;

    return;
}

static xmedia_s32 cv2005_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = cv2005_get_ae_common_default(dev, cv2005_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (cv2005_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            cv2005_get_ae_linear_default(dev, cv2005_ctx, ae_sns_dft);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", cv2005_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    int_time = cv2005_ctx->fl[0] - int_time;

    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_EXP_H].data = (int_time & 0xF0000) >> 16;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_EXP_M].data = SENSOR_HIGH_8BITS(int_time);
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_EXP_L].data = SENSOR_LOW_8BITS(int_time);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;
    xmedia_u8  againmax = CV2005_AGAIN_INDEX_MAX - 1;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_cv2005_again_table[againmax]) {
        *again_db = againmax;
    } else {
        for (i = 1; i < CV2005_AGAIN_INDEX_MAX; i++) {
            if (*again_lin < g_cv2005_again_table[i]) {
                *again_db = i - 1;
                break;
            }
        }
    }

    *again_lin = g_cv2005_again_table[*again_db];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_DGAIN_H].data = SENSOR_HIGH_8BITS(dgain);
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_DGAIN_L].data = SENSOR_LOW_8BITS(dgain);
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_AGAIN].data   = again;

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 cv2005_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    // awb_sns_dft->wb_ref_temp = CV2005_CALIBRATE_STATIC_TEMP;

    // awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = CV2005_CALIBRATE_STATIC_WB_R_GAIN;
    // awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = CV2005_CALIBRATE_STATIC_WB_GR_GAIN;
    // awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = CV2005_CALIBRATE_STATIC_WB_GB_GAIN;
    // awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = CV2005_CALIBRATE_STATIC_WB_B_GAIN;

    // // 0 ~ 5: point index on the awb curve param
    // awb_sns_dft->wb_para[0] = CV2005_CALIBRATE_AWB_P1;
    // awb_sns_dft->wb_para[1] = CV2005_CALIBRATE_AWB_P2;
    // awb_sns_dft->wb_para[2] = CV2005_CALIBRATE_AWB_Q1;
    // awb_sns_dft->wb_para[3] = CV2005_CALIBRATE_AWB_A1;
    // awb_sns_dft->wb_para[4] = CV2005_CALIBRATE_AWB_B1;
    // awb_sns_dft->wb_para[5] = CV2005_CALIBRATE_AWB_C1;

    awb_sns_dft->golden_rgain    = CV2005_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain    = CV2005_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (cv2005_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_cv2005_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_cv2005_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", cv2005_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_cv2005_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_cv2005_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_cv2005_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_cv2005_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_cv2005_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

xmedia_s32 cv2005_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case CV2005_2LANE_1920X1080_LINEAR_MODE:
            *max_fps = 30.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_standby(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = cv2005_write_reg(dev, CV2005_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_resume(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = cv2005_write_reg(dev, CV2005_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *cv2005_ctx   = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < cv2005_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < cv2005_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= cv2005_write_reg(dev, cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    if (cv2005_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = cv2005_init_image(dev, cv2005_ctx->img_mode, cv2005_ctx->work_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = cv2005_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = cv2005_set_mirror_flip(dev, cv2005_ctx->mirror_en, cv2005_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = cv2005_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_cv2005_dev_map[index] == SENSOR_DEV_INVALID) {
            g_cv2005_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 cv2005_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_cv2005_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 cv2005_get_hmax_vmax(xmedia_u32 dev, xmedia_u32 *hmax, xmedia_u32 *vmax)
{
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(hmax);
    SENSOR_CHECK_PTR_RETURN(vmax);

    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    *vmax = cv2005_ctx->fl[SENSOR_CUR_FRAME];

    switch (cv2005_ctx->img_mode) {
        case CV2005_2LANE_1920X1080_LINEAR_MODE:
            if (cv2005_ctx->work_mode == XMEDIA_SENSOR_WORK_MODE_NORMAL) {
                *hmax = CV2005_HMAX_1920X1080_LINEAR << 1; // TODO: 待确认寄存器值是否就是真正的hmax
            } else {
                SENSOR_TRACE(MODULE_DBG_ERR, "Not support work mode: %d\n", cv2005_ctx->work_mode);
                return XMEDIA_ERRCODE_NOT_SUPPORT;
            }
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{
    switch (img_mode) {
        case CV2005_2LANE_1920X1080_LINEAR_MODE:
            *min_fps = 0.1;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case CV2005_2LANE_1920X1080_LINEAR_MODE:
            *vmax = CV2005_VMAX_1920X1080_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_u32   vmax;
    xmedia_float max_fps, min_fps;

    ret = cv2005_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = cv2005_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = cv2005_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is (%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, CV2005_FULL_LINES_MAX);

    SENSOR_PRINT("cv2005 set fps = %f\n", fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    for (i = 0; i < cv2005_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            cv2005_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &cv2005_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&cv2005_ctx->regs_info[SENSOR_PRE_FRAME], &cv2005_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    cv2005_ctx->fl[SENSOR_PRE_FRAME] = cv2005_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    ret = cv2005_calc_fps(fps, cv2005_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_VMAX_H].data = (full_lines >> 16) & 0xF;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_VMAX_M].data = SENSOR_HIGH_8BITS(full_lines);
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    cv2005_ctx->fps                  = fps;
    cv2005_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    cv2005_ctx->fl_std               = cv2005_ctx->fl[SENSOR_CUR_FRAME];

    ae_sns_dft->fps            = fps;
    ae_sns_dft->full_lines_std = cv2005_ctx->fl_std;
    ae_sns_dft->full_lines     = cv2005_ctx->fl_std;
    ae_sns_dft->max_int_time   = cv2005_ctx->fl[SENSOR_CUR_FRAME] - CV2005_EXP_OFFSET_LINEAR;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = cv2005_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = cv2005_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = cv2005_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = vmax_max_fps * max_fps / min_fps;

    return XMEDIA_SUCCESS;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 cv2005_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 * 注意：2to1 wdr模式下，曝光比和ae wdr mode 由AE传入，调用本接口前，AE需先调用cv2005_get_wdr_max_inttime
 */
static xmedia_s32 cv2005_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *cv2005_ctx = XMEDIA_NULL;
    xmedia_u32      vmax_min_fps;
    xmedia_s32      ret;
    xmedia_float    min_fps;

    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    ret = cv2005_get_min_fps_vmax(cv2005_ctx->img_mode, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = cv2005_get_min_fps(cv2005_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                       = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    cv2005_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_VMAX_H].data = (full_lines >> 16) & 0xF;
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_VMAX_M].data = SENSOR_HIGH_8BITS(full_lines);
    cv2005_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV2005_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    ae_sns_dft->full_lines   = cv2005_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = cv2005_ctx->fl[SENSOR_CUR_FRAME] - CV2005_EXP_OFFSET_LINEAR;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    ret = cv2005_get_min_fps(cv2005_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = cv2005_get_max_fps(cv2005_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = cv2005_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv2005_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, CV2005_NAME, sizeof(CV2005_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property    = cv2005_get_property;
    info->isp_func.pfn_sensor_set_work_mode   = cv2005_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes  = cv2005_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror          = cv2005_mirror;
    info->isp_func.pfn_sensor_flip            = cv2005_flip;
    info->isp_func.pfn_sensor_set_bus_info    = cv2005_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr    = cv2005_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param  = cv2005_set_init_param;
    info->isp_func.pfn_sensor_start           = cv2005_start;
    info->isp_func.pfn_sensor_stop            = cv2005_stop;
    info->isp_func.pfn_sensor_standby         = cv2005_standby;
    info->isp_func.pfn_sensor_resume          = cv2005_resume;
    info->isp_func.pfn_sensor_write_reg       = cv2005_write_reg;
    info->isp_func.pfn_sensor_read_reg        = cv2005_read_reg;
    info->isp_func.pfn_sensor_init            = cv2005_init;
    info->isp_func.pfn_sensor_exit            = cv2005_exit;
    info->isp_func.pfn_sensor_set_attr        = cv2005_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = cv2005_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = cv2005_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = cv2005_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = cv2005_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = cv2005_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = cv2005_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = cv2005_get_fps;
    info->isp_func.pfn_sensor_get_trig_attr       = cv2005_get_trig_attr;
    info->isp_func.pfn_sensor_get_hmax_vmax       = cv2005_get_hmax_vmax;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = cv2005_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = cv2005_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = cv2005_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = cv2005_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = cv2005_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = cv2005_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = cv2005_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = cv2005_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = cv2005_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = cv2005_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 cv2005_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = cv2005_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = cv2005_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = cv2005_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "CV2005 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 cv2005_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *cv2005_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = cv2005_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    CV2005_GET_CTX(dev, cv2005_ctx);
    SENSOR_CHECK_PTR_RETURN(cv2005_ctx);

    if (cv2005_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "CV2005 unregister function failed!\n");
        return ret;
    }

    cv2005_ctx_exit(dev);
    g_cv2005_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;
}
