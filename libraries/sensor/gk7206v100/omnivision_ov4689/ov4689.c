#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ov4689.h"
#include "ov4689_ctrl.h"
#include "ov4689_ex.h"

#define OV4689_NAME          "OV4689"
#define OV4689_SPECS_MAX_NUM 1
#define OV4689_BIT_RATE      720

#ifdef __linux__
//#define OV4689_ISP_DEFAULT_SUPPORT
#endif

#define OV4689_RES_IS_1440P(w, h) ((w) == 2688 && (h) == 1520)

#define OV4689_ERR_MODE_PRINT(sensor_attr)                                                                            \
    do {                                                                                                              \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sensor_attr->width,            \
                     sensor_attr->height, sensor_attr->wdr_mode);                                                     \
    } while (0)

#define OV4689_CALIBRATE_STATIC_TEMP 5000

// awb static param for Fuji Lens New IR_Cut
#define OV4689_CALIBRATE_STATIC_WB_R_GAIN  366
#define OV4689_CALIBRATE_STATIC_WB_GR_GAIN 256
#define OV4689_CALIBRATE_STATIC_WB_GB_GAIN 256
#define OV4689_CALIBRATE_STATIC_WB_B_GAIN  533

// Calibration results for Auto WB Planck
#define OV4689_CALIBRATE_AWB_P1 137
#define OV4689_CALIBRATE_AWB_P2 (-13)
#define OV4689_CALIBRATE_AWB_Q1 (-132)
#define OV4689_CALIBRATE_AWB_A1 155552
#define OV4689_CALIBRATE_AWB_B1 128
#define OV4689_CALIBRATE_AWB_C1 (-94868)

// Rgain and Bgain of the golden sample
#define OV4689_GOLDEN_RGAIN 0
#define OV4689_GOLDEN_BGAIN 0

#define OV4689_REG_ADDR_STANDBY 0x0100

#define OV4689_AGAIN_MAX_NUM 233
static const xmedia_u32 g_ov4689_again_index[OV4689_AGAIN_MAX_NUM] = {
    1024,  1088,  1152,  1216,  1280,  1344,  1408,  1472,  1536,  1600,  1664,  1728,  1792,  1856,  1920,  1984,
    2048,  2112,  2176,  2240,  2304,  2368,  2432,  2496,  2560,  2624,  2688,  2752,  2816,  2880,  2944,  3008,
    3072,  3136,  3200,  3264,  3328,  3392,  3456,  3520,  3584,  3648,  3712,  3776,  3840,  3904,  3968,  4032,
    4096,  4160,  4224,  4288,  4352,  4416,  4480,  4544,  4608,  4672,  4736,  4800,  4864,  4928,  4992,  5056,
    5120,  5184,  5248,  5312,  5376,  5440,  5504,  5568,  5632,  5696,  5760,  5824,  5888,  5952,  6016,  6080,
    6144,  6208,  6272,  6336,  6400,  6464,  6528,  6592,  6656,  6720,  6784,  6848,  6912,  6976,  7040,  7104,
    7168,  7232,  7296,  7360,  7424,  7488,  7552,  7616,  7680,  7744,  7808,  7872,  7936,  8000,  8064,  8128,
    8192,  8256,  8320,  8384,  8448,  8512,  8576,  8640,  8704,  8768,  8832,  8896,  8960,  9024,  9088,  9152,
    9216,  9280,  9344,  9408,  9472,  9536,  9600,  9664,  9728,  9792,  9856,  9920,  9984,  10048, 10112, 10176,
    10240, 10304, 10368, 10432, 10496, 10560, 10624, 10688, 10752, 10816, 10880, 10944, 11008, 11072, 11136, 11200,
    11264, 11328, 11392, 11456, 11520, 11584, 11648, 11712, 11776, 11840, 11904, 11968, 12032, 12096, 12160, 12224,
    12288, 12352, 12416, 12480, 12544, 12608, 12672, 12736, 12800, 12864, 12928, 12992, 13056, 13120, 13184, 13248,
    13312, 13376, 13440, 13504, 13568, 13632, 13696, 13760, 13824, 13888, 13952, 14016, 14080, 14144, 14208, 14272,
    14336, 14400, 14464, 14528, 14592, 14656, 14720, 14784, 14848, 14912, 14976, 15040, 15104, 15168, 15232, 15296,
    15360, 15424, 15488, 15552, 15616, 15680, 15744, 15808, 15872,
};

static xmedia_u32 g_ov4689_again_table[OV4689_AGAIN_MAX_NUM] = {
    0x0080, 0x0088, 0x0090, 0x0098, 0x00A0, 0x00A8, 0x00B0, 0x00B8, 0x00C0, 0x00C8, 0x00D0, 0x00D8, 0x00E0, 0x00E8,
    0x00F0, 0x00F8, 0x0180, 0x0184, 0x0188, 0x018C, 0x0190, 0x0194, 0x0198, 0x019C, 0x01A0, 0x01A4, 0x01A8, 0x01AC,
    0x01B0, 0x01B4, 0x01B8, 0x01BC, 0x01C0, 0x01C4, 0x01C8, 0x01CC, 0x01D0, 0x01D4, 0x01D8, 0x01DC, 0x01E0, 0x01E4,
    0x01E8, 0x01EC, 0x01F0, 0x01F4, 0x01F8, 0x01FC, 0x0380, 0x0382, 0x0384, 0x0386, 0x0388, 0x038A, 0x038C, 0x038E,
    0x0390, 0x0392, 0x0394, 0x0396, 0x0398, 0x039A, 0x039C, 0x039E, 0x03A0, 0x03A2, 0x03A4, 0x03A6, 0x03A8, 0x03AA,
    0x03AC, 0x03AE, 0x03B0, 0x03B2, 0x03B4, 0x03B6, 0x03B8, 0x03BA, 0x03BC, 0x03BE, 0x03C0, 0x03C2, 0x03C4, 0x03C6,
    0x03C8, 0x03CA, 0x03CC, 0x03CE, 0x03D0, 0x03D2, 0x03D4, 0x03D6, 0x03D8, 0x03DA, 0x03DC, 0x03DE, 0x03E0, 0x03E2,
    0x03E4, 0x03E6, 0x03E8, 0x03EA, 0x03EC, 0x03EE, 0x03F0, 0x03F2, 0x03F4, 0x03F6, 0x03F8, 0x03FA, 0x03FC, 0x03FE,
    0x0780, 0x0781, 0x0782, 0x0783, 0x0784, 0x0785, 0x0786, 0x0787, 0x0788, 0x0789, 0x078A, 0x078B, 0x078C, 0x078D,
    0x078E, 0x078F, 0x0790, 0x0791, 0x0792, 0x0793, 0x0794, 0x0795, 0x0796, 0x0797, 0x0798, 0x0799, 0x079A, 0x079B,
    0x079C, 0x079D, 0x079E, 0x079F, 0x07A0, 0x07A1, 0x07A2, 0x07A3, 0x07A4, 0x07A5, 0x07A6, 0x07A7, 0x07A8, 0x07A9,
    0x07AA, 0x07AB, 0x07AC, 0x07AD, 0x07AE, 0x07AF, 0x07B0, 0x07B1, 0x07B2, 0x07B3, 0x07B4, 0x07B5, 0x07B6, 0x07B7,
    0x07B8, 0x07B9, 0x07BA, 0x07BB, 0x07BC, 0x07BD, 0x07BE, 0x07BF, 0x07C0, 0x07C1, 0x07C2, 0x07C3, 0x07C4, 0x07C5,
    0x07C6, 0x07C7, 0x07C8, 0x07C9, 0x07CA, 0x07CB, 0x07CC, 0x07CD, 0x07CE, 0x07CF, 0x07D0, 0x07D1, 0x07D2, 0x07D3,
    0x07D4, 0x07D5, 0x07D6, 0x07D7, 0x07D8, 0x07D9, 0x07DA, 0x07DB, 0x07DC, 0x07DD, 0x07DE, 0x07DF, 0x07E0, 0x07E1,
    0x07E2, 0x07E3, 0x07E4, 0x07E5, 0x07E6, 0x07E7, 0x07E8, 0x07E9, 0x07EA, 0x07EB, 0x07EC, 0x07ED, 0x07EE, 0x07EF,
    0x07F0, 0x07F1, 0x07F2, 0x07F3, 0x07F4, 0x07F5, 0x07F6, 0x07F7, 0x07F8,
};


static sensor_context *g_ov4689_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define OV4689_GET_CTX(dev, ctx) ctx = g_ov4689_ctx[dev]
#define OV4689_SET_CTX(dev, ctx) g_ov4689_ctx[dev] = ctx

static xmedia_s32 g_ov4689_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] =
                                                                       SENSOR_DEV_INVALID };

static xmedia_u32 g_ov4689_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_ov4689_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_ov4689_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_ov4689_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

static const xmedia_sensor_property g_ov4689_property[OV4689_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        2688, 1520, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OV4689_BIT_RATE,
    },
};

static const xmedia_sensor_capability g_ov4689_capability = {
    .max_width  = 2688,
    .max_height = 1520,
    .max_fps    = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_4L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = OV4689_ADDR_BYTE,
        .data_byte_num     = OV4689_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { OV4689_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { OV4689_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x01 },
    },
};

static xmedia_s32 ov4689_ctx_init(xmedia_u32 dev)
{
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    OV4689_GET_CTX(dev, ov4689_ctx);
    if (ov4689_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    ov4689_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (ov4689_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(ov4689_ctx, 0, sizeof(sensor_context));
    ov4689_ctx->i2c_addr             = OV4689_I2C_ADDR;
    ov4689_ctx->lanes                = XMEDIA_SENSOR_MIPI_LANES_4L;
    ov4689_ctx->work_mode            = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    ov4689_ctx->bus_info.i2c_dev     = SENSOR_I2C_INVALID;
    ov4689_ctx->size.width           = g_ov4689_property[0].width;
    ov4689_ctx->size.height          = g_ov4689_property[0].height;
    ov4689_ctx->fps                  = g_ov4689_property[0].max_fps;
    ov4689_ctx->wdr_mode             = g_ov4689_property[0].wdr_mode;
    ov4689_ctx->img_mode             = OV4689_4M_30FPS_10BIT_LINEAR_MODE;
    ov4689_ctx->fl_std               = OV4689_4L_VMAX_4M30_LINEAR;
    ov4689_ctx->fl[SENSOR_CUR_FRAME] = OV4689_4L_VMAX_4M30_LINEAR;
    ov4689_ctx->fl[SENSOR_PRE_FRAME] = OV4689_4L_VMAX_4M30_LINEAR;
    OV4689_SET_CTX(dev, ov4689_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void ov4689_ctx_exit(xmedia_u32 dev)
{
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_FREE(ov4689_ctx);
    OV4689_SET_CTX(dev, XMEDIA_NULL);

    return;
}

static xmedia_s32 ov4689_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    for (i = 0; i < OV4689_SPECS_MAX_NUM; i++) {
        if (g_ov4689_property[i].width == ov4689_ctx->size.width &&
            g_ov4689_property[i].height == ov4689_ctx->size.height &&
            g_ov4689_property[i].wdr_mode == ov4689_ctx->wdr_mode) {
            memcpy(property, &g_ov4689_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Please check input param!\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 ov4689_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ov4689_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Invalid mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ov4689_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        ov4689_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        ov4689_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        ov4689_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 ov4689_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    ov4689_ctx->i2c_addr = slave_addr;
    ret = ov4689_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    *slave_addr = ov4689_ctx->i2c_addr;

    return XMEDIA_SUCCESS;
}

// called by ISP
static xmedia_s32 ov4689_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    ov4689_ctx->init_mode = init_mode;
    ret = ov4689_i2c_init(dev, ov4689_ctx->bus_info.i2c_dev, ov4689_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    ov4689_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    ret = ov4689_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ov4689_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    ov4689_ctx->mirror_en = mirror_en;
    ret = ov4689_set_mirror_flip(dev, mirror_en, ov4689_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    ov4689_ctx->flip_en = flip_en;
    ret = ov4689_set_mirror_flip(dev, ov4689_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_set_image_mode(sensor_context *ov4689_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (ov4689_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (OV4689_RES_IS_1440P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OV4689_4M_30FPS_10BIT_LINEAR_MODE;
            ov4689_ctx->fl_std = OV4689_4L_VMAX_4M30_LINEAR;
        } else {
            OV4689_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        OV4689_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ov4689_ctx->size.width  = sns_attr->width;
    ov4689_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            ov4689_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(ov4689_ctx->wdr_int_time, 0, sizeof(ov4689_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

static xmedia_void ov4689_init_common_reg_info(sensor_context *ov4689_ctx)
{
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_EXP_H].delay_frame_num = 2;
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_EXP_H].reg_addr        = OV4689_REG_ADDR_EXP_H;
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_EXP_M].delay_frame_num = 2;
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_EXP_M].reg_addr        = OV4689_REG_ADDR_EXP_M;
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_EXP_L].delay_frame_num = 2;
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_EXP_L].reg_addr        = OV4689_REG_ADDR_EXP_L;

    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_AGAIN_H].delay_frame_num = 2;
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_AGAIN_H].reg_addr        = OV4689_REG_ADDR_AGC_H;
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_AGAIN_L].delay_frame_num = 2;
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_AGAIN_L].reg_addr        = OV4689_REG_ADDR_AGC_L;

    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_VMAX_H].delay_frame_num = 1;
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_VMAX_H].reg_addr        = OV4689_REG_ADDR_VMAX_H;
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_VMAX_L].delay_frame_num = 1;
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_VMAX_L].reg_addr        = OV4689_REG_ADDR_VMAX_L;

    return;
}

static xmedia_s32 ov4689_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = ov4689_ctx->bus_info.i2c_dev;
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2;
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = OV4689_REG_MAX_NUM;

    for (i = 0; i < ov4689_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = ov4689_ctx->i2c_addr;
        ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = OV4689_ADDR_BYTE;
        ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = OV4689_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    ov4689_init_common_reg_info(ov4689_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    // Set wdr mode
    ret = ov4689_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = ov4689_set_image_mode(ov4689_ctx, &ov4689_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    ov4689_ctx->fl[SENSOR_CUR_FRAME] = ov4689_ctx->fl_std;
    ov4689_ctx->fl[SENSOR_PRE_FRAME] = ov4689_ctx->fl[SENSOR_CUR_FRAME];

    ret = ov4689_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef OV4689_ISP_DEFAULT_SUPPORT
    // TO-DO: alg add param
    isp_default->blc         = XMEDIA_NULL;
    isp_default->bnr         = XMEDIA_NULL;
    isp_default->clut_attr   = XMEDIA_NULL;
    isp_default->crosstalk   = XMEDIA_NULL;
    isp_default->csc         = XMEDIA_NULL;
    isp_default->dehaze      = XMEDIA_NULL;
    isp_default->demosaic    = XMEDIA_NULL;
    isp_default->dpc_dynamic = XMEDIA_NULL;
    isp_default->dpc_static  = XMEDIA_NULL;
    isp_default->drc         = XMEDIA_NULL;
    isp_default->expander    = XMEDIA_NULL;
    isp_default->fpn         = XMEDIA_NULL;
    isp_default->gamma       = XMEDIA_NULL;
    isp_default->gcac        = XMEDIA_NULL;
    isp_default->hlc         = XMEDIA_NULL;
    isp_default->lce         = XMEDIA_NULL;
    isp_default->mlsc_attr   = XMEDIA_NULL;
    isp_default->mlsc_lut    = XMEDIA_NULL;
    isp_default->radial_crop = XMEDIA_NULL;
    isp_default->rgbir       = XMEDIA_NULL;
    isp_default->rlsc_attr   = XMEDIA_NULL;
    isp_default->rlsc_lut    = XMEDIA_NULL;
    isp_default->sharpen     = XMEDIA_NULL;
    isp_default->stnr        = XMEDIA_NULL;
    isp_default->wdr         = XMEDIA_NULL;
    return XMEDIA_SUCCESS;
#else
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from ov4689!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 ov4689_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (ov4689_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x80;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x80;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x80;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x80;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    if (ov4689_ctx->mirror_en && ov4689_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (ov4689_ctx->mirror_en && (!ov4689_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!ov4689_ctx->mirror_en) && ov4689_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < OV4689_SPECS_MAX_NUM; i++) {
        if (g_ov4689_property[i].width == ov4689_ctx->size.width &&
            g_ov4689_property[i].height == ov4689_ctx->size.height &&
            g_ov4689_property[i].wdr_mode == ov4689_ctx->wdr_mode) {
            *bayer_pattern = g_ov4689_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= OV4689_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);
    memcpy(capability, &g_ov4689_capability, sizeof(xmedia_sensor_capability));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ov4689_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 ov4689_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_ov4689_exposure[dev]      = init_param->exposure;
    g_ov4689_sample_r_gain[dev] = init_param->sample_rgain;
    g_ov4689_sample_b_gain[dev] = init_param->sample_bgain;

    g_ov4689_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_ov4689_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_ov4689_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

xmedia_s32 ov4689_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case OV4689_4M_30FPS_10BIT_LINEAR_MODE:
            *max_fps = 30;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = ov4689_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines     = sns_ctx->fl_std;
    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   =  50 * 256;

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1.0;
    ae_sns_dft->int_time_accu.offset    = 0.0;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1.0;
    ae_sns_dft->again_accu.offset    = 0;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy  = 0.0078125;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 8 << ae_sns_dft->ispdgain_shift;

    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_4;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_5_6;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    // lines per second: VTS * max_fps
    ae_sns_dft->lines_per_500ms  = (sns_ctx->fl_std * max_fps) / 2;
    ae_sns_dft->man_ratio_enable = XMEDIA_FALSE;

    return XMEDIA_SUCCESS;
}

static xmedia_void ov4689_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->max_again = 15872;
    ae_sns_dft->min_again = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = 0x80;
    ae_sns_dft->min_dgain = 0x80;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure   = g_ov4689_exposure[dev] ? g_ov4689_exposure[dev] : 76151;

    ae_sns_dft->max_int_time = sns_ctx->fl_std - OV4689_EXP_OFFSET;
    ae_sns_dft->min_int_time = 2;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    return;
}

static xmedia_s32 ov4689_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = ov4689_get_ae_common_default(dev, ov4689_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (ov4689_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            ov4689_get_ae_linear_default(dev, ov4689_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support wdr mode\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
            break;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    int_time = SENSOR_MAX(int_time, 2);

    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_EXP_H].data = SENSOR_HIGHER_4BITS(int_time);
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_EXP_M].data = SENSOR_MIDDLE_8BITS(int_time);
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_EXP_L].data = SENSOR_LOWER_4BITS(int_time);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);

    if (*again_lin >= g_ov4689_again_index[OV4689_AGAIN_MAX_NUM - 1]) {
        *again_lin = g_ov4689_again_index[OV4689_AGAIN_MAX_NUM - 1];
        *again_db  = g_ov4689_again_table[OV4689_AGAIN_MAX_NUM - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < OV4689_AGAIN_MAX_NUM; i++) {
        if (*again_lin < g_ov4689_again_index[i]) {
            *again_lin = g_ov4689_again_index[i - 1];
            *again_db  = g_ov4689_again_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    ov4689_ctx->regs_info[0].i2c_data[OV4689_REG_AGAIN_H].data = ((again & 0x3F00) >> 8);
    ov4689_ctx->regs_info[0].i2c_data[OV4689_REG_AGAIN_L].data = SENSOR_LOW_8BITS(again);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 ov4689_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = OV4689_CALIBRATE_STATIC_TEMP;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = OV4689_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = OV4689_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = OV4689_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = OV4689_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 5: point index on the awb curve param
    awb_sns_dft->wb_para[0] = OV4689_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1] = OV4689_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2] = OV4689_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3] = OV4689_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4] = OV4689_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5] = OV4689_CALIBRATE_AWB_C1;

    awb_sns_dft->golden_rgain = OV4689_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = OV4689_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (ov4689_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_ov4689_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_ov4689_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
    }

    awb_sns_dft->init_rgain   = g_ov4689_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_ov4689_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_ov4689_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_ov4689_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_ov4689_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_standby(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = ov4689_write_reg(dev, OV4689_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_resume(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = ov4689_write_reg(dev, OV4689_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *ov4689_ctx   = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;
    return XMEDIA_SUCCESS;

    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < ov4689_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < ov4689_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= ov4689_write_reg(dev, ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
        SENSOR_PRINT("reg_addr: 0x%x, reg_data: 0x%x\n", ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
            ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_start(xmedia_u32 dev)
{
    xmedia_s32               ret;
    xmedia_u8                img_mode;
    xmedia_sensor_mipi_lanes mipi_lanes;
    xmedia_sensor_work_mode  work_mode;
    sensor_context          *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    work_mode  = ov4689_ctx->work_mode;
    mipi_lanes = ov4689_ctx->lanes;
    img_mode   = ov4689_ctx->img_mode;

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL || mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check param!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (ov4689_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = ov4689_init_image(dev, img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = ov4689_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = ov4689_set_mirror_flip(dev, ov4689_ctx->mirror_en, ov4689_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = ov4689_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_ov4689_dev_map[index] == SENSOR_DEV_INVALID) {
            g_ov4689_dev_map[index] = pipe;
            *dev                     = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are binded to pipe!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 ov4689_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_ov4689_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 ov4689_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{
    switch (img_mode) {
        case OV4689_4M_30FPS_10BIT_LINEAR_MODE:
            *min_fps = 2;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case OV4689_4M_30FPS_10BIT_LINEAR_MODE:
            *vmax = OV4689_4L_VMAX_4M30_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = ov4689_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = ov4689_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = ov4689_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is [%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, OV4689_FULL_LINES_MAX);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    for (i = 0; i < ov4689_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            ov4689_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &ov4689_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&ov4689_ctx->regs_info[SENSOR_PRE_FRAME], &ov4689_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    ov4689_ctx->fl[SENSOR_PRE_FRAME] = ov4689_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    ret = ov4689_calc_fps(fps, ov4689_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    ov4689_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    ov4689_ctx->fl_std               = ov4689_ctx->fl[SENSOR_CUR_FRAME];
    ov4689_ctx->fps                  = fps;
    ae_sns_dft->fps                  = fps;
    ae_sns_dft->full_lines_std       = ov4689_ctx->fl_std;
    ae_sns_dft->full_lines           = ov4689_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time         = ov4689_ctx->fl[SENSOR_CUR_FRAME] - OV4689_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]- ov4689 set fps: %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 */
static xmedia_s32 ov4689_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *ov4689_ctx = XMEDIA_NULL;
    xmedia_u32      vblank;
    xmedia_float    min_fps, max_fps;
    xmedia_u32      min_fps_vamx, max_fps_vmax;
    xmedia_s32      ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    if (ov4689_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", ov4689_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = ov4689_get_min_fps(ov4689_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = ov4689_get_max_fps(ov4689_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = ov4689_get_vmax(ov4689_ctx->img_mode, &max_fps_vmax);
    SENSOR_CHECK_RET_RETURN(ret);
    min_fps_vamx = max_fps * max_fps_vmax / SENSOR_DIV_0_TO_1_FLOAT(min_fps);

    full_lines = SENSOR_MIN(full_lines, OV4689_FULL_LINES_MAX);
    full_lines = SENSOR_MIN(full_lines, min_fps_vamx);

    vblank = full_lines - OV4689_4L_VMAX_4M30_LINEAR;
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_VMAX_H].data = SENSOR_HIGH_8BITS(vblank);
    ov4689_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OV4689_REG_VMAX_L].data = SENSOR_LOW_8BITS(vblank);

    ov4689_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    ov4689_ctx->fps                  = min_fps * min_fps_vamx / full_lines;

    ae_sns_dft->full_lines   = ov4689_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = ov4689_ctx->fl[SENSOR_CUR_FRAME] - OV4689_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]- ov4689 set fps: %f\n", dev, ov4689_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    ret = ov4689_get_min_fps(ov4689_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = ov4689_get_max_fps(ov4689_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = ov4689_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ov4689_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, OV4689_NAME, sizeof(OV4689_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = ov4689_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = ov4689_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = ov4689_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = ov4689_mirror;
    info->isp_func.pfn_sensor_flip             = ov4689_flip;
    info->isp_func.pfn_sensor_set_bus_info     = ov4689_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = ov4689_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = ov4689_set_init_param;
    info->isp_func.pfn_sensor_start            = ov4689_start;
    info->isp_func.pfn_sensor_stop             = ov4689_stop;
    info->isp_func.pfn_sensor_standby          = ov4689_standby;
    info->isp_func.pfn_sensor_resume           = ov4689_resume;
    info->isp_func.pfn_sensor_write_reg        = ov4689_write_reg;
    info->isp_func.pfn_sensor_read_reg         = ov4689_read_reg;
    info->isp_func.pfn_sensor_init             = ov4689_init;
    info->isp_func.pfn_sensor_exit             = ov4689_exit;

    // called by ISP
    info->isp_func.pfn_sensor_set_attr            = ov4689_set_attr;
    info->isp_func.pfn_sensor_get_isp_default     = ov4689_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = ov4689_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = ov4689_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = ov4689_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = ov4689_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = ov4689_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = ov4689_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = ov4689_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = ov4689_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = ov4689_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = ov4689_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = ov4689_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = ov4689_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = ov4689_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = ov4689_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = ov4689_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = ov4689_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 ov4689_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = ov4689_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = ov4689_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = ov4689_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OV4689 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 ov4689_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *ov4689_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = ov4689_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    OV4689_GET_CTX(dev, ov4689_ctx);
    SENSOR_CHECK_PTR_RETURN(ov4689_ctx);

    if (ov4689_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OV4689 unregister function failed!\n");
        return ret;
    }

    ov4689_ctx_exit(dev);
    g_ov4689_dev_map[dev] = SENSOR_DEV_INVALID; // reset dev

    return XMEDIA_SUCCESS;
}
