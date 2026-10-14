#include <stdlib.h>
#include <string.h>
#include "ox03f10.h"
#include "ox03f10_ctrl.h"
#include "ox03f10_ex.h"

#ifdef __linux__
#define OX03F10_ISP_DEFAULT_SUPPORT
#endif

#ifdef FPGA
#define OX03F10_INPUT_CLOCK_BUILTIN XMEDIA_SENSOR_CLOCK_FREQ_24MHZ
#define OX03F10_1M_MAX_FPS_BUILTIN  10.0
#define OX03F10_2M_MAX_FPS_BUILTIN  0.0 // Not support
#define OX03F10_3M_MAX_FPS_BUILTIN  0.0 // Not support
#define OX03F10_3M_MAX_FPS_LINEAR   0.0 // Not support
#define OX03F10_1M_BIT_RATE_BUILTIN 600
#define OX03F10_2M_BIT_RATE_BUILTIN 600
#define OX03F10_3M_BIT_RATE_BUILTIN 600
#define OX03F10_3M_BIT_RATE_LINEAR  600
#else
#define OX03F10_INPUT_CLOCK_BUILTIN XMEDIA_SENSOR_CLOCK_FREQ_24MHZ
#define OX03F10_1M_MAX_FPS_BUILTIN  60.0
#define OX03F10_2M_MAX_FPS_BUILTIN  60.0
#define OX03F10_3M_MAX_FPS_BUILTIN  60.0
#define OX03F10_3M_MAX_FPS_LINEAR   60.0
#define OX03F10_1M_BIT_RATE_BUILTIN 400
#define OX03F10_2M_BIT_RATE_BUILTIN 1400
#define OX03F10_3M_BIT_RATE_BUILTIN 1400
#define OX03F10_3M_BIT_RATE_LINEAR  1400
#endif

#ifdef OX03F10_BIT_WIDTH_14
#define OX03F10_DATA_BIT                  XMEDIA_VIDEO_DATA_WIDTH_14
#define OX03F10_BAYER_PATTERN_MIRROR_FLIP XMEDIA_VIDEO_BAYER_FMT_RGGB
#define OX03F10_BAYER_PATTERN_MIRROR      XMEDIA_VIDEO_BAYER_FMT_GBRG
#define OX03F10_BAYER_PATTERN_FLIP        XMEDIA_VIDEO_BAYER_FMT_RGGB
#define OX03F10_BAYER_PATTERN_NORMAL      XMEDIA_VIDEO_BAYER_FMT_GBRG
#else
#define OX03F10_DATA_BIT                  XMEDIA_VIDEO_DATA_WIDTH_12
#define OX03F10_BAYER_PATTERN_MIRROR_FLIP XMEDIA_VIDEO_BAYER_FMT_GRBG
#define OX03F10_BAYER_PATTERN_MIRROR      XMEDIA_VIDEO_BAYER_FMT_BGGR
#define OX03F10_BAYER_PATTERN_FLIP        XMEDIA_VIDEO_BAYER_FMT_GRBG
#define OX03F10_BAYER_PATTERN_NORMAL      XMEDIA_VIDEO_BAYER_FMT_BGGR
#endif

#define OX03F10_NAME          "OX03F10"
#define OX03F10_SPECS_MAX_NUM 4

#define OX03F10_REG_ADDR_STANDBY 0x0100

#define OX03F10_RES_IS_720P(w, h)  ((w) == 1280 && (h) == 720)
#define OX03F10_RES_IS_1280P(w, h) ((w) == 1920 && (h) == 1280)
#define OX03F10_RES_IS_1536P(w, h) ((w) == 1920 && (h) == 1536)

typedef struct {
    xmedia_u16 hcg_gain[XMEDIA_ISP_BAYER_PATTERN_NUM]; // r g b g
    xmedia_u16 lcg_gain[XMEDIA_ISP_BAYER_PATTERN_NUM];
    xmedia_u16 vs_gain[XMEDIA_ISP_BAYER_PATTERN_NUM];
    xmedia_u16 spd_gain[XMEDIA_ISP_BAYER_PATTERN_NUM];
} wb_gain;

#define OX03F10_ERR_MODE_PRINT(sns_attr, mipi_lanes)                                                                   \
    do {                                                                                                               \
        SENSOR_PRINT("Not support! Width:%d, Height:%d, WDRMode:%d, mipi_lanes:%d.\n", sns_attr->width,                \
                     sns_attr->height, sns_attr->wdr_mode, mipi_lanes);                                                \
    } while (0)

SENSOR_PRIORITY_DATA sensor_context *g_ox03f10_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define OX03F10_SET_CTX(dev, sns_ctx) g_ox03f10_ctx[dev] = sns_ctx
#define OX03F10_GET_CTX(dev, sns_ctx) sns_ctx = g_ox03f10_ctx[dev]

SENSOR_PRIORITY_DATA static xmedia_s32 g_ox03f10_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] =
    { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

static xmedia_u32 g_ox03f10_again[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_ox03f10_dgain[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_ox03f10_isp_dgain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_ox03f10_init_time[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u32 g_ox03f10_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_ox03f10_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_ox03f10_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u16 g_ox03f10_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

#define CT_MAX_NUM 6
static const xmedia_s32 g_ct_lpd_r_gain[CT_MAX_NUM] = { 2327, 2145, 1874, 1644, 1300, 1190 };
static const xmedia_s32 g_ct_lpd_b_gain[CT_MAX_NUM] = { 1316, 1399, 1599, 1907, 2386, 2597 };
static const xmedia_s32 g_ct_spd_r_gain[CT_MAX_NUM] = { 2194, 2039, 1803, 1602, 1285, 1184 };
static const xmedia_s32 g_ct_spd_b_gain[CT_MAX_NUM] = { 1485, 1580, 1784, 2087, 2510, 2692 };
static const xmedia_s32 g_ct_tab[CT_MAX_NUM]        = { 7500, 6500, 5100, 4100, 3100, 2800 }; // color temp

SENSOR_PRIORITY_RODATA static const xmedia_sensor_property g_ox03f10_property[OX03F10_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        1280, 720, XMEDIA_VIDEO_WDR_MODE_BUILT_IN,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_BUILTIN, OX03F10_1M_MAX_FPS_BUILTIN, OX03F10_INPUT_CLOCK_BUILTIN,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_14,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_GBRG,
          XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_GBRG },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OX03F10_1M_BIT_RATE_BUILTIN,
    },

    {
        // width, height, wdr_mode
        1920, 1280, XMEDIA_VIDEO_WDR_MODE_BUILT_IN,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_BUILTIN, OX03F10_2M_MAX_FPS_BUILTIN, OX03F10_INPUT_CLOCK_BUILTIN,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_14,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_GBRG,
          XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_GBRG },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OX03F10_2M_BIT_RATE_BUILTIN,
    },

    {
        // width, height, wdr_mode
        1920, 1536, XMEDIA_VIDEO_WDR_MODE_BUILT_IN,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_BUILTIN, OX03F10_3M_MAX_FPS_BUILTIN, OX03F10_INPUT_CLOCK_BUILTIN,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, OX03F10_DATA_BIT,
        // bayer_format - mirror&flip /mirror /flip /normal
        { OX03F10_BAYER_PATTERN_MIRROR_FLIP, OX03F10_BAYER_PATTERN_MIRROR,
          OX03F10_BAYER_PATTERN_FLIP, OX03F10_BAYER_PATTERN_NORMAL },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OX03F10_3M_BIT_RATE_BUILTIN,
    },

    {
        // width, height, wdr_mode
        1920, 1536, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_NONE, OX03F10_3M_MAX_FPS_LINEAR, OX03F10_INPUT_CLOCK_BUILTIN,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, OX03F10_DATA_BIT,
        // bayer_format - mirror&flip /mirror /flip /normal
        { OX03F10_BAYER_PATTERN_MIRROR_FLIP, OX03F10_BAYER_PATTERN_MIRROR,
          OX03F10_BAYER_PATTERN_FLIP, OX03F10_BAYER_PATTERN_NORMAL},
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OX03F10_3M_BIT_RATE_LINEAR,
    },
};


// awb static param for Fuji Lens New IR_Cut
#define OX03F10_CALIBRATE_STATIC_TEMP       5120
#define OX03F10_CALIBRATE_STATIC_WB_R_GAIN  447
#define OX03F10_CALIBRATE_STATIC_WB_GR_GAIN 256
#define OX03F10_CALIBRATE_STATIC_WB_GB_GAIN 256
#define OX03F10_CALIBRATE_STATIC_WB_B_GAIN  571

// Calibration results for Auto WB Planck
#define OX03F10_CALIBRATE_AWB_P1 159
#define OX03F10_CALIBRATE_AWB_P2 -71
#define OX03F10_CALIBRATE_AWB_Q1 -166
#define OX03F10_CALIBRATE_AWB_A1 183129
#define OX03F10_CALIBRATE_AWB_B1 128
#define OX03F10_CALIBRATE_AWB_C1 -122544

// Rgain and Bgain of the golden sample
#define OX03F10_GOLDEN_RGAIN 0
#define OX03F10_GOLDEN_BGAIN 0

SENSOR_PRIORITY_RODATA static const xmedia_sensor_capability g_ox03f10_capability = {
    .max_width  = 1920,
    .max_height = 1536,
    .max_fps    = 60.0,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_BUILT_IN,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_BUILTIN,
    .bit_width   = OX03F10_DATA_BIT,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_4L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = OX03F10_ADDR_BYTE,
        .data_byte_num     = OX03F10_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { OX03F10_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { OX03F10_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x01 },
    },
};

SENSOR_PRIORITY_FUNC static xmedia_s32 ox03f10_ctx_init(xmedia_u32 dev)
{
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    OX03F10_GET_CTX(dev, ox03f10_ctx);
    if (ox03f10_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    ox03f10_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (ox03f10_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(ox03f10_ctx, 0, sizeof(sensor_context));
    ox03f10_ctx->i2c_addr             = OX03F10_I2C_ADDR;
    ox03f10_ctx->lanes                = XMEDIA_SENSOR_MIPI_LANES_4L;
    ox03f10_ctx->work_mode            = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    ox03f10_ctx->bus_info.i2c_dev     = SENSOR_I2C_INVALID;
    ox03f10_ctx->size.width           = g_ox03f10_property[0].width;
    ox03f10_ctx->size.height          = g_ox03f10_property[0].height;
    ox03f10_ctx->fps                  = g_ox03f10_property[0].max_fps;
    ox03f10_ctx->wdr_mode             = g_ox03f10_property[0].wdr_mode;
    ox03f10_ctx->img_mode             = OX03F10_SENSOR_1M_BUILT_IN_MODE;
    ox03f10_ctx->fl_std               = OX03F10_VMAX_1M_BUILT_IN;
    ox03f10_ctx->fl[SENSOR_CUR_FRAME] = OX03F10_VMAX_1M_BUILT_IN;
    ox03f10_ctx->fl[SENSOR_PRE_FRAME] = OX03F10_VMAX_1M_BUILT_IN;

    OX03F10_SET_CTX(dev, ox03f10_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void ox03f10_ctx_exit(xmedia_u32 dev)
{
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_FREE(ox03f10_ctx);
    OX03F10_SET_CTX(dev, XMEDIA_NULL);

    return;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 ox03f10_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    for (i = 0; i < OX03F10_SPECS_MAX_NUM; i++) {
        if (g_ox03f10_property[i].width == ox03f10_ctx->size.width &&
            g_ox03f10_property[i].height == ox03f10_ctx->size.height &&
            g_ox03f10_property[i].wdr_mode == ox03f10_ctx->wdr_mode) {
            memcpy(property, &g_ox03f10_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n",
                ox03f10_ctx->size.width, ox03f10_ctx->size.height, ox03f10_ctx->wdr_mode);
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 ox03f10_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ox03f10_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "not supported mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ox03f10_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 ox03f10_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        ox03f10_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        ox03f10_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        ox03f10_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 ox03f10_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    ox03f10_ctx->i2c_addr = slave_addr;
    ret                   = ox03f10_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    *slave_addr = ox03f10_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}


// called by ISP
SENSOR_PRIORITY_FUNC static xmedia_s32 ox03f10_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    ox03f10_ctx->init_mode = init_mode;
    ret = ox03f10_i2c_init(dev, ox03f10_ctx->bus_info.i2c_dev, ox03f10_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    ox03f10_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 ox03f10_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    ret = ox03f10_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ox03f10_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    ox03f10_ctx->mirror_en = mirror_en;
    ret                    = ox03f10_set_mirror_flip(dev, mirror_en, ox03f10_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    ox03f10_ctx->flip_en = flip_en;
    ret                  = ox03f10_set_mirror_flip(dev, ox03f10_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 ox03f10_set_image_mode(sensor_context *ox03f10_ctx, xmedia_u8 *image_mode,
                                         const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (ox03f10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_BUILT_IN) {
        if (OX03F10_RES_IS_720P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OX03F10_SENSOR_1M_BUILT_IN_MODE;
            ox03f10_ctx->fl_std = OX03F10_VMAX_1M_BUILT_IN;
            return XMEDIA_SUCCESS;
        } else if (OX03F10_RES_IS_1280P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OX03F10_SENSOR_2M_BUILT_IN_MODE;
            ox03f10_ctx->fl_std = OX03F10_VMAX_2M_BUILT_IN;
            return XMEDIA_SUCCESS;
        } else if (OX03F10_RES_IS_1536P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OX03F10_SENSOR_3M_BUILT_IN_MODE;
            ox03f10_ctx->fl_std = OX03F10_VMAX_3M_BUILT_IN;
            return XMEDIA_SUCCESS;
        } else {
            goto ERR0;
        }
    } else if (ox03f10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (OX03F10_RES_IS_1536P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OX03F10_SENSOR_3M_LINEAR_MODE;
            ox03f10_ctx->fl_std = OX03F10_VMAX_3M_LINEAR;
            return XMEDIA_SUCCESS;
        } else {
            goto ERR0;
        }
    }

ERR0:
    OX03F10_ERR_MODE_PRINT(sns_attr, ox03f10_ctx->lanes);
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 ox03f10_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_BUILT_IN:
            ox03f10_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_BUILT_IN;
            SENSOR_PRINT("sensor built in WDR mode.\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_NONE:
            ox03f10_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("sensor linear mode.\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(ox03f10_ctx->wdr_int_time, 0, sizeof(ox03f10_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

#define OX03F10_BUILT_IN_REG_INFO_MAX_NUM OX03F10_REG_MAX_NUM
SENSOR_PRIORITY_FUNC xmedia_s32 ox03f10_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.type           = XMEDIA_SENSOR_BUS_TYPE_I2C;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = ox03f10_ctx->bus_info.i2c_dev;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = OX03F10_BUILT_IN_REG_INFO_MAX_NUM;

    for (i = 0; i < ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = ox03f10_ctx->i2c_addr;
        ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = OX03F10_ADDR_BYTE;
        ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = OX03F10_DATA_BYTE;
    }

    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VMAX_H].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VMAX_H].reg_addr        = OX03F10_VMAX_H_ADDR;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VMAX_L].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VMAX_L].reg_addr        = OX03F10_VMAX_L_ADDR;

    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_EXP_H].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_EXP_H].reg_addr = OX03F10_REG_ADDR_HCG_EXP_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_EXP_L].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_EXP_L].reg_addr = OX03F10_REG_ADDR_HCG_EXP_L;

    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_AGC_L].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_AGC_L].reg_addr = OX03F10_REG_ADDR_HCG_AGC_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_AGC_H].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_AGC_H].reg_addr = OX03F10_REG_ADDR_HCG_AGC_H;

    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_DGC_L].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_DGC_L].reg_addr = OX03F10_REG_ADDR_HCG_DGC_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_DGC_M].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_DGC_M].reg_addr = OX03F10_REG_ADDR_HCG_DGC_M;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_DGC_H].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_DGC_H].reg_addr = OX03F10_REG_ADDR_HCG_DGC_H;

    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_AGC_L].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_AGC_L].reg_addr = OX03F10_REG_ADDR_LCG_AGC_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_AGC_H].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_AGC_H].reg_addr = OX03F10_REG_ADDR_LCG_AGC_H;

    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_DGC_L].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_DGC_L].reg_addr = OX03F10_REG_ADDR_LCG_DGC_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_DGC_M].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_DGC_M].reg_addr = OX03F10_REG_ADDR_LCG_DGC_M;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_DGC_H].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_DGC_H].reg_addr = OX03F10_REG_ADDR_LCG_DGC_H;

    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_EXP_H].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_EXP_H].reg_addr = OX03F10_REG_ADDR_SPD_EXP_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_EXP_L].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_EXP_L].reg_addr = OX03F10_REG_ADDR_SPD_EXP_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_AGC_L].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_AGC_L].reg_addr = OX03F10_REG_ADDR_SPD_AGC_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_AGC_H].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_AGC_H].reg_addr = OX03F10_REG_ADDR_SPD_AGC_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_DGC_L].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_DGC_L].reg_addr = OX03F10_REG_ADDR_SPD_DGC_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_DGC_M].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_DGC_M].reg_addr = OX03F10_REG_ADDR_SPD_DGC_M;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_DGC_H].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_DGC_H].reg_addr = OX03F10_REG_ADDR_SPD_DGC_H;

    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_EXP_H].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_EXP_H].reg_addr        = OX03F10_REG_ADDR_VS_EXP_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_EXP_L].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_EXP_L].reg_addr        = OX03F10_REG_ADDR_VS_EXP_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_AGC_L].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_AGC_L].reg_addr        = OX03F10_REG_ADDR_VS_AGC_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_AGC_H].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_AGC_H].reg_addr        = OX03F10_REG_ADDR_VS_AGC_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_DGC_L].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_DGC_L].reg_addr        = OX03F10_REG_ADDR_VS_DGC_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_DGC_M].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_DGC_M].reg_addr        = OX03F10_REG_ADDR_VS_DGC_M;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_DGC_H].delay_frame_num = 2;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_DGC_H].reg_addr        = OX03F10_REG_ADDR_VS_DGC_H;

    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_R_H].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_R_H].reg_addr = OX03F10_REG_ADDR_HCG_WB_R_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_R_L].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_R_L].reg_addr = OX03F10_REG_ADDR_HCG_WB_R_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_GR_H].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_GR_H].reg_addr = OX03F10_REG_ADDR_HCG_WB_GR_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_GR_L].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_GR_L].reg_addr = OX03F10_REG_ADDR_HCG_WB_GR_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_B_H].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_B_H].reg_addr = OX03F10_REG_ADDR_HCG_WB_B_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_B_L].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_B_L].reg_addr = OX03F10_REG_ADDR_HCG_WB_B_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_GB_H].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_GB_H].reg_addr = OX03F10_REG_ADDR_HCG_WB_GB_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_GB_L].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_GB_L].reg_addr = OX03F10_REG_ADDR_HCG_WB_GB_L;

    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_R_H].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_R_H].reg_addr = OX03F10_REG_ADDR_LCG_WB_R_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_R_L].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_R_L].reg_addr = OX03F10_REG_ADDR_LCG_WB_R_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_GR_H].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_GR_H].reg_addr = OX03F10_REG_ADDR_LCG_WB_GR_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_GR_L].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_GR_L].reg_addr = OX03F10_REG_ADDR_LCG_WB_GR_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_B_H].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_B_H].reg_addr = OX03F10_REG_ADDR_LCG_WB_B_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_B_L].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_B_L].reg_addr = OX03F10_REG_ADDR_LCG_WB_B_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_GB_H].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_GB_H].reg_addr = OX03F10_REG_ADDR_LCG_WB_GB_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_GB_L].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_GB_L].reg_addr = OX03F10_REG_ADDR_LCG_WB_GB_L;

    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_R_H].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_R_H].reg_addr = OX03F10_REG_ADDR_SPD_WB_R_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_R_L].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_R_L].reg_addr = OX03F10_REG_ADDR_SPD_WB_R_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_GR_H].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_GR_H].reg_addr = OX03F10_REG_ADDR_SPD_WB_GR_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_GR_L].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_GR_L].reg_addr = OX03F10_REG_ADDR_SPD_WB_GR_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_B_H].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_B_H].reg_addr = OX03F10_REG_ADDR_SPD_WB_B_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_B_L].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_B_L].reg_addr = OX03F10_REG_ADDR_SPD_WB_B_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_GB_H].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_GB_H].reg_addr = OX03F10_REG_ADDR_SPD_WB_GB_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_GB_L].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_GB_L].reg_addr = OX03F10_REG_ADDR_SPD_WB_GB_L;

    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_R_H].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_R_H].reg_addr = OX03F10_REG_ADDR_VS_WB_R_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_R_L].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_R_L].reg_addr = OX03F10_REG_ADDR_VS_WB_R_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_GR_H].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_GR_H].reg_addr = OX03F10_REG_ADDR_VS_WB_GR_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_GR_L].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_GR_L].reg_addr = OX03F10_REG_ADDR_VS_WB_GR_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_B_H].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_B_H].reg_addr = OX03F10_REG_ADDR_VS_WB_B_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_B_L].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_B_L].reg_addr = OX03F10_REG_ADDR_VS_WB_B_L;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_GB_H].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_GB_H].reg_addr = OX03F10_REG_ADDR_VS_WB_GB_H;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_GB_L].delay_frame_num = 1;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_GB_L].reg_addr = OX03F10_REG_ADDR_VS_WB_GB_L;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 ox03f10_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    // Set wdr mode
    ret = ox03f10_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = ox03f10_set_image_mode(ox03f10_ctx, &ox03f10_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    ox03f10_ctx->size.width           = sns_attr->width;
    ox03f10_ctx->size.height          = sns_attr->height;
    ox03f10_ctx->fl[SENSOR_CUR_FRAME] = ox03f10_ctx->fl_std;
    ox03f10_ctx->fl[SENSOR_PRE_FRAME] = ox03f10_ctx->fl[SENSOR_CUR_FRAME];

    ret = ox03f10_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));

    // for quick start
    if (ox03f10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_BUILT_IN) {
        isp_default->expander = &g_ox03f10_expander_attr;
    }

#ifdef OX03F10_ISP_DEFAULT_SUPPORT
    // TO-DO: alg add param
    isp_default->bnr         = XMEDIA_NULL;
    isp_default->clut_attr   = XMEDIA_NULL;
    isp_default->crosstalk   = XMEDIA_NULL;
    isp_default->csc         = XMEDIA_NULL;
    isp_default->dehaze      = XMEDIA_NULL;
    isp_default->demosaic    = XMEDIA_NULL;
    isp_default->dpc_dynamic = XMEDIA_NULL;
    isp_default->dpc_static  = XMEDIA_NULL;
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

    switch(ox03f10_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            isp_default->blc = XMEDIA_NULL;
            isp_default->drc = XMEDIA_NULL;
            break;

        case XMEDIA_VIDEO_WDR_MODE_BUILT_IN:
            isp_default->blc = XMEDIA_NULL;
            isp_default->drc = XMEDIA_NULL;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support mode: %d\n", ox03f10_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
#else
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from ox03f10!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 ox03f10_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (ox03f10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x100;
    } else { // bulit-in
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x40;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x40;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x40;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x40;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    if (ox03f10_ctx->mirror_en && ox03f10_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (ox03f10_ctx->mirror_en && (!ox03f10_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!ox03f10_ctx->mirror_en) && ox03f10_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < OX03F10_SPECS_MAX_NUM; i++) {
        if (g_ox03f10_property[i].width == ox03f10_ctx->size.width && g_ox03f10_property[i].height == ox03f10_ctx->size.height &&
            g_ox03f10_property[i].wdr_mode == ox03f10_ctx->wdr_mode) {
            *bayer_pattern = g_ox03f10_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= OX03F10_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 ox03f10_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);
    memcpy(capability, &g_ox03f10_capability, sizeof(xmedia_sensor_capability));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ox03f10_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
SENSOR_PRIORITY_FUNC static xmedia_s32 ox03f10_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_ox03f10_again[dev]         = init_param->again;
    g_ox03f10_dgain[dev]         = init_param->dgain;
    g_ox03f10_isp_dgain[dev]     = init_param->ispdgain;
    g_ox03f10_init_time[dev]     = init_param->exp_time;
    g_ox03f10_exposure[dev]      = init_param->exposure;
    g_ox03f10_sample_r_gain[dev] = init_param->sample_rgain;
    g_ox03f10_sample_b_gain[dev] = init_param->sample_bgain;

    g_ox03f10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_ox03f10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_ox03f10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

xmedia_s32 ox03f10_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case OX03F10_SENSOR_1M_BUILT_IN_MODE:
            *max_fps = OX03F10_1M_MAX_FPS_BUILTIN;
            break;

        case OX03F10_SENSOR_2M_BUILT_IN_MODE:
            *max_fps = OX03F10_2M_MAX_FPS_BUILTIN;
            break;

        case OX03F10_SENSOR_3M_BUILT_IN_MODE:
            *max_fps = OX03F10_3M_MAX_FPS_BUILTIN;
            break;

        case OX03F10_SENSOR_3M_LINEAR_MODE:
            *max_fps = OX03F10_3M_MAX_FPS_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = ox03f10_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines     = sns_ctx->fl_std;
    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 0;
    ae_sns_dft->full_lines_max = OX03F10_FULL_LINES_MAX;

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 2.0;
    ae_sns_dft->int_time_accu.offset    = 0.0;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->again_accu.accuracy  = 0.0625;
    ae_sns_dft->again_accu.offset    = 0;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy  = 0.0009765625;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 16 << ae_sns_dft->ispdgain_shift;

    memcpy(&ae_sns_dft->piris_attr, &g_ox03f10_piris_attr, sizeof(xmedia_isp_piris_attr));
    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_32_0;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    // lines per second: VTS * max_fps
    ae_sns_dft->lines_per_500ms = (sns_ctx->fl_std * max_fps) / 2;

    return XMEDIA_SUCCESS;
}

static xmedia_void ox03f10_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                 xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->max_again        = 248; // 15.5x
    ae_sns_dft->min_again        = 16;  // 1x
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 1024 * 15.999; // 15x
    ae_sns_dft->min_dgain        = 1024;          // 1x
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x44;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = 30800000;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - OX03F10_EXP_OFFSET;
    ae_sns_dft->min_int_time        = 2;
    ae_sns_dft->max_int_time_target = 65535; // max 65535
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    return;
}

static xmedia_s32 ox03f10_get_ae_2to1_built_in_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                       xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    ae_sns_dft->max_again        = 248;
    ae_sns_dft->min_again        = 16;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;
    ae_sns_dft->ae_compensation  = 0x38;

    ae_sns_dft->max_dgain        = 1024;
    ae_sns_dft->min_dgain        = 1024;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - OX03F10_EXP_OFFSET;
    ae_sns_dft->min_int_time        = 2;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;
    ae_sns_dft->init_exposure       = 20276883;

    ae_sns_dft->man_ratio_enable = XMEDIA_TRUE;
    ae_sns_dft->ratio[0]         = 256 * 64;
    ae_sns_dft->exp_ratio_max    = ae_sns_dft->ratio[0];
    ae_sns_dft->exp_ratio_min    = ae_sns_dft->ratio[0];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = ox03f10_get_ae_common_default(dev, ox03f10_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (ox03f10_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            ox03f10_get_ae_linear_default(dev, ox03f10_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_BUILT_IN:
            ox03f10_get_ae_2to1_built_in_default(dev, ox03f10_ctx, ae_sns_dft);
            break;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context *ox03f10_ctx = XMEDIA_NULL;
    xmedia_u32      int_time_dcg;
    xmedia_u32      int_time_vs;
    xmedia_u32      int_time_spd;
    xmedia_float    inttime_ratio;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    inttime_ratio = (ox03f10_ctx->fl[0] - 13 - 3) / 3.0;
    int_time_vs   = SENSOR_CLIP3(int_time / (inttime_ratio + 1), 1, 3);
    int_time_dcg =
        SENSOR_CLIP3(int_time * inttime_ratio / (inttime_ratio + 1), 2, ox03f10_ctx->fl[0] - 13 - int_time_vs);

    if (ox03f10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_BUILT_IN) {
        int_time_dcg = SENSOR_CLIP3(int_time, 4, ox03f10_ctx->fl[0] - 13 - int_time_vs);
        int_time_spd = SENSOR_CLIP3(int_time, 4, ox03f10_ctx->fl[0] - 13);
    } else {
        int_time_dcg = int_time;
        int_time_spd = int_time;
        int_time_vs  = int_time;
    }

    if (ox03f10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_BUILT_IN) {
        ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_EXP_H].data = SENSOR_HIGH_8BITS(int_time_dcg);
        ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_EXP_L].data = SENSOR_LOW_8BITS(int_time_dcg);
        ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_EXP_H].data  = SENSOR_HIGH_8BITS(int_time_vs);
        ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_EXP_L].data  = SENSOR_LOW_8BITS(int_time_vs);
        ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_EXP_H].data = SENSOR_HIGH_8BITS(int_time_spd);
        ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_EXP_L].data = SENSOR_LOW_8BITS(int_time_spd);
    } else {
#if (OX03F10_HCG|OX03F10_LCG)
        ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_EXP_H].data = SENSOR_HIGH_8BITS(int_time_dcg);
        ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_EXP_L].data = SENSOR_LOW_8BITS(int_time_dcg);
#endif

#if OX03F10_SPD
        ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_EXP_H].data = SENSOR_HIGH_8BITS(int_time_spd);
        ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_EXP_L].data = SENSOR_LOW_8BITS(int_time_spd);
#endif
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *ox03f10_ctx = XMEDIA_NULL;
    xmedia_u32      again_hcg;
    xmedia_u32      dgain_hcg;
    xmedia_u32      again_lcg;
    xmedia_u32      dgain_lcg;
    xmedia_u32      again_vs;
    xmedia_u32      dgain_vs;
    xmedia_u32      again_spd;
    xmedia_u32      dgain_spd;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    if (ox03f10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_BUILT_IN) {
        again_hcg = again;
        again_lcg = again / 4;
        again_spd = again / 2;
        again_vs  = again / 8;
        dgain_hcg = dgain;
        dgain_lcg = dgain;
        dgain_spd = dgain;
        dgain_vs  = dgain;

        again_hcg = SENSOR_CLIP3(again_hcg, 16, 248);
        again_lcg = SENSOR_CLIP3(again_lcg, 16, 248);
        again_vs  = SENSOR_CLIP3(again_vs, 16, 248);
        again_spd = SENSOR_CLIP3(again_spd, 68, 248);
    } else {
        again_hcg = again;
        dgain_hcg = dgain;
        again_lcg = again;
        dgain_lcg = dgain;
        again_spd = again;
        dgain_spd = dgain;
        again_vs  = again;
        dgain_vs  = dgain;
    }

    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_AGC_L].data = ((again_hcg & 0xf) << 4);
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_AGC_H].data = ((again_hcg & 0xf0) >> 4);
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_DGC_L].data = ((dgain_hcg & 0x03) << 6);
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_DGC_M].data = ((dgain_hcg & 0x03fc) >> 2);
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_DGC_H].data = ((dgain_hcg & 0x3c00) >> 10);

    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_AGC_L].data = ((again_lcg & 0xf) << 4);
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_AGC_H].data = ((again_lcg & 0xf0) >> 4);
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_DGC_L].data = ((dgain_lcg & 0x03) << 6);
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_DGC_M].data = ((dgain_lcg & 0x03fc) >> 2);
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_DGC_H].data = ((dgain_lcg & 0x3c00) >> 10);

    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_AGC_L].data = ((again_spd & 0xf) << 4);  // 0x40;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_AGC_H].data = ((again_spd & 0xf0) >> 4); // 0x04;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_DGC_L].data = ((dgain_spd & 0x03) << 6); // 0x00;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_DGC_M].data =
        ((dgain_spd & 0x03fc) >> 2); // 0x00;
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_DGC_H].data =
        ((dgain_spd & 0x3c00) >> 10); // 0x01;

    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_AGC_L].data = ((again_vs & 0xf) << 4);
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_AGC_H].data = ((again_vs & 0xf0) >> 4);
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_DGC_L].data = ((dgain_vs & 0x03) << 6);
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_DGC_M].data = ((dgain_vs & 0x03fc) >> 2);
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_DGC_H].data = ((dgain_vs & 0x3c00) >> 10);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = OX03F10_CALIBRATE_STATIC_TEMP;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = OX03F10_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = OX03F10_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = OX03F10_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = OX03F10_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 6: point index on the awb curve param
    awb_sns_dft->wb_para[0]      = OX03F10_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1]      = OX03F10_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2]      = OX03F10_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3]      = OX03F10_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4]      = OX03F10_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5]      = OX03F10_CALIBRATE_AWB_C1;
    awb_sns_dft->golden_rgain    = OX03F10_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain    = OX03F10_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (ox03f10_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_ox03f10_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_ox03f10_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_BUILT_IN:
            memcpy(&awb_sns_dft->ccm, &g_ox03f10_awb_wdr_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_ox03f10_awb_wdr_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
    }

    awb_sns_dft->init_rgain   = 531;
    awb_sns_dft->init_ggain   = 256;
    awb_sns_dft->init_bgain   = 357;
    awb_sns_dft->sample_rgain = g_ox03f10_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_ox03f10_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_update_awb_gain(xmedia_u32 ct, wb_gain *gain)
{
    xmedia_s32 i          = 0;
    xmedia_s32 tmp_r_gain = 0;
    xmedia_s32 tmp_b_gain = 0;

    if (ct >= g_ct_tab[0]) {
        gain->hcg_gain[0] = g_ct_lpd_r_gain[0]; // rgain
        gain->hcg_gain[1] = 1024;
        gain->hcg_gain[2] = g_ct_lpd_b_gain[0]; // bgain
        gain->hcg_gain[3] = 1024;
        gain->spd_gain[0] = g_ct_spd_r_gain[0]; // rgain
        gain->spd_gain[1] = 1024;
        gain->spd_gain[2] = g_ct_spd_b_gain[0]; // bgain
        gain->spd_gain[3] = 1024;
    } else if (ct <= g_ct_tab[CT_MAX_NUM - 1]) {
        gain->hcg_gain[0] = g_ct_lpd_r_gain[CT_MAX_NUM - 1];
        gain->hcg_gain[1] = 1024;
        gain->hcg_gain[2] = g_ct_lpd_b_gain[CT_MAX_NUM - 1];
        gain->hcg_gain[3] = 1024;
        gain->spd_gain[0] = g_ct_spd_r_gain[CT_MAX_NUM - 1]; // rgain
        gain->spd_gain[1] = 1024;
        gain->spd_gain[2] = g_ct_spd_b_gain[CT_MAX_NUM - 1]; // bgain
        gain->spd_gain[3] = 1024;
    } else {
        for (i = 0; i < CT_MAX_NUM - 1; i++) {
            if (ct < g_ct_tab[i] && ct >= g_ct_tab[i + 1]) {
                tmp_r_gain = g_ct_lpd_r_gain[i] + (g_ct_tab[i] - (xmedia_s32)ct) *
                                                      (g_ct_lpd_r_gain[i + 1] - g_ct_lpd_r_gain[i]) /
                                                      (g_ct_tab[i] - g_ct_tab[i + 1]);
                tmp_b_gain = g_ct_lpd_b_gain[i] + (g_ct_tab[i] - (xmedia_s32)ct) *
                                                      (g_ct_lpd_b_gain[i + 1] - g_ct_lpd_b_gain[i]) /
                                                      (g_ct_tab[i] - g_ct_tab[i + 1]);
                gain->hcg_gain[0] = tmp_r_gain;
                gain->hcg_gain[1] = 1024;
                gain->hcg_gain[2] = tmp_b_gain;
                gain->hcg_gain[3] = 1024;
                tmp_r_gain        = g_ct_spd_r_gain[i] + (g_ct_tab[i] - (xmedia_s32)ct) *
                                                      (g_ct_spd_r_gain[i + 1] - g_ct_spd_r_gain[i]) /
                                                      (g_ct_tab[i] - g_ct_tab[i + 1]);
                tmp_b_gain = g_ct_spd_b_gain[i] + (g_ct_tab[i] - (xmedia_s32)ct) *
                                                      (g_ct_spd_b_gain[i + 1] - g_ct_spd_b_gain[i]) /
                                                      (g_ct_tab[i] - g_ct_tab[i + 1]);
                gain->spd_gain[0] = tmp_r_gain; // rgain
                gain->spd_gain[1] = 1024;
                gain->spd_gain[2] = tmp_b_gain; // bgain
                gain->spd_gain[3] = 1024;
                break;
            }
        }
    }
    for (i = 0; i < XMEDIA_ISP_BAYER_PATTERN_NUM; i++) {
        gain->lcg_gain[i] = gain->hcg_gain[i];
        gain->vs_gain[i]  = gain->hcg_gain[i];
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_set_color_temperature(xmedia_u32 dev, xmedia_u32 ct)
{
    xmedia_u32      i;
    wb_gain         gain = { 0 };
    sensor_context *ctx  = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);

    OX03F10_GET_CTX(dev, ctx);
    SENSOR_CHECK_PTR_RETURN(ctx);

    if (ctx->ct == ct) {
        return XMEDIA_SUCCESS;
    }

    if (ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        for (i = 0; i < XMEDIA_ISP_BAYER_PATTERN_NUM; i++) {
            gain.hcg_gain[i] = 0x400;
            gain.lcg_gain[i] = 0x400;
            gain.spd_gain[i] = 0x400;
            gain.vs_gain[i]  = 0x400;
        }
    } else {
        ox03f10_update_awb_gain(ct, &gain);
    }

    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_R_H].data  = SENSOR_HIGH_8BITS(gain.hcg_gain[0]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_R_L].data  = SENSOR_LOW_8BITS(gain.hcg_gain[0]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_GR_H].data = SENSOR_HIGH_8BITS(gain.hcg_gain[1]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_GR_L].data = SENSOR_LOW_8BITS(gain.hcg_gain[1]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_B_H].data  = SENSOR_HIGH_8BITS(gain.hcg_gain[2]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_B_L].data  = SENSOR_LOW_8BITS(gain.hcg_gain[2]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_GB_H].data = SENSOR_HIGH_8BITS(gain.hcg_gain[3]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_HCG_WB_GB_L].data = SENSOR_LOW_8BITS(gain.hcg_gain[3]);

    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_R_H].data  = SENSOR_HIGH_8BITS(gain.lcg_gain[0]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_R_L].data  = SENSOR_LOW_8BITS(gain.lcg_gain[0]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_GR_H].data = SENSOR_HIGH_8BITS(gain.lcg_gain[1]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_GR_L].data = SENSOR_LOW_8BITS(gain.lcg_gain[1]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_B_H].data  = SENSOR_HIGH_8BITS(gain.lcg_gain[2]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_B_L].data  = SENSOR_LOW_8BITS(gain.lcg_gain[2]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_GB_H].data = SENSOR_HIGH_8BITS(gain.lcg_gain[3]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_LCG_WB_GB_L].data = SENSOR_LOW_8BITS(gain.lcg_gain[3]);

    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_R_H].data  = SENSOR_HIGH_8BITS(gain.spd_gain[0]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_R_L].data  = SENSOR_LOW_8BITS(gain.spd_gain[0]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_GR_H].data = SENSOR_HIGH_8BITS(gain.spd_gain[1]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_GR_L].data = SENSOR_LOW_8BITS(gain.spd_gain[1]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_B_H].data  = SENSOR_HIGH_8BITS(gain.spd_gain[2]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_B_L].data  = SENSOR_LOW_8BITS(gain.spd_gain[2]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_GB_H].data = SENSOR_HIGH_8BITS(gain.spd_gain[3]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_SPD_WB_GB_L].data = SENSOR_LOW_8BITS(gain.spd_gain[3]);

    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_R_H].data  = SENSOR_HIGH_8BITS(gain.vs_gain[0]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_R_L].data  = SENSOR_LOW_8BITS(gain.vs_gain[0]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_GR_H].data = SENSOR_HIGH_8BITS(gain.vs_gain[1]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_GR_L].data = SENSOR_LOW_8BITS(gain.vs_gain[1]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_B_H].data  = SENSOR_HIGH_8BITS(gain.vs_gain[2]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_B_L].data  = SENSOR_LOW_8BITS(gain.vs_gain[2]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_GB_H].data = SENSOR_HIGH_8BITS(gain.vs_gain[3]);
    ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VS_WB_GB_L].data = SENSOR_LOW_8BITS(gain.vs_gain[3]);
    ctx->ct                                                                = ct;
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 ox03f10_standby(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = ox03f10_write_reg(dev, OX03F10_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_resume(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = ox03f10_write_reg(dev, OX03F10_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 ox03f10_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *ox03f10_ctx  = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= ox03f10_write_reg(dev, ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                 ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
        ox03f10_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data =
            ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 ox03f10_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    if (ox03f10_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = ox03f10_init_image(dev, ox03f10_ctx->img_mode, ox03f10_ctx->work_mode, ox03f10_ctx->mirror_en,
                                 ox03f10_ctx->flip_en);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = ox03f10_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 ox03f10_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = ox03f10_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 ox03f10_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_ox03f10_dev_map[index] == SENSOR_DEV_INVALID) {
            g_ox03f10_dev_map[index] = pipe;
            *dev                     = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 ox03f10_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_ox03f10_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 ox03f10_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    for (i = 0; i < ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            ox03f10_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &ox03f10_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&ox03f10_ctx->regs_info[SENSOR_PRE_FRAME],
           &ox03f10_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    ox03f10_ctx->fl[SENSOR_PRE_FRAME] = ox03f10_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{
    switch (img_mode) {
        case OX03F10_SENSOR_1M_BUILT_IN_MODE:
        case OX03F10_SENSOR_2M_BUILT_IN_MODE:
        case OX03F10_SENSOR_3M_BUILT_IN_MODE:
        case OX03F10_SENSOR_3M_LINEAR_MODE:
            *min_fps = 0.51;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case OX03F10_SENSOR_1M_BUILT_IN_MODE:
            *vmax = OX03F10_VMAX_1M_BUILT_IN;
            break;

        case OX03F10_SENSOR_2M_BUILT_IN_MODE:
            *vmax = OX03F10_VMAX_2M_BUILT_IN;
            break;

        case OX03F10_SENSOR_3M_BUILT_IN_MODE:
            *vmax = OX03F10_VMAX_3M_BUILT_IN;
            break;

        case OX03F10_SENSOR_3M_LINEAR_MODE:
            *vmax = OX03F10_VMAX_3M_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = ox03f10_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = ox03f10_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = ox03f10_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is [%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, OX03F10_FULL_LINES_MAX);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    ret = ox03f10_calc_fps(fps, ox03f10_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    if (ox03f10_ctx->img_mode == OX03F10_SENSOR_1M_BUILT_IN_MODE ||
        ox03f10_ctx->img_mode == OX03F10_SENSOR_2M_BUILT_IN_MODE ||
        ox03f10_ctx->img_mode == OX03F10_SENSOR_3M_BUILT_IN_MODE ||
        ox03f10_ctx->img_mode == OX03F10_SENSOR_3M_LINEAR_MODE) {
        ae_sns_dft->int_time_accu.offset = 0.2818;
    }
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    ox03f10_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    ox03f10_ctx->fl_std               = ox03f10_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->fps                   = fps;
    ox03f10_ctx->fps                  = fps;
    ae_sns_dft->full_lines_std        = ox03f10_ctx->fl_std;
    ae_sns_dft->full_lines            = ox03f10_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time          = ox03f10_ctx->fl[SENSOR_CUR_FRAME] - OX03F10_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]- ox03f10 set fps: %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines: AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines: AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 */
static xmedia_s32 ox03f10_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                             xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *ox03f10_ctx = XMEDIA_NULL;
    xmedia_float    min_fps, max_fps;
    xmedia_u32      min_fps_vamx, max_fps_vmax;
    xmedia_s32      ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    if (ox03f10_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", ox03f10_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = ox03f10_get_max_fps(ox03f10_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = ox03f10_get_min_fps(ox03f10_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = ox03f10_get_vmax(ox03f10_ctx->img_mode, &max_fps_vmax);
    SENSOR_CHECK_RET_RETURN(ret);
    min_fps_vamx = max_fps * max_fps_vmax / SENSOR_DIV_0_TO_1_FLOAT(min_fps);

    if (ox03f10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        full_lines = (full_lines > OX03F10_FULL_LINES_MAX) ? OX03F10_FULL_LINES_MAX : full_lines;
    } else if (ox03f10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_BUILT_IN) {
        full_lines = (full_lines + 20) / 12;
        full_lines = (full_lines < ae_sns_dft->full_lines_std) ? ae_sns_dft->full_lines_std : full_lines;
        full_lines = (full_lines > OX03F10_FULL_LINES_MAX) ? OX03F10_FULL_LINES_MAX : full_lines;
    } else {
        ae_sns_dft->full_lines = ox03f10_ctx->fl_std;
    }

    full_lines = SENSOR_MIN(full_lines, min_fps_vamx);

    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    ox03f10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX03F10_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    ox03f10_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    ox03f10_ctx->fps                  =  max_fps * max_fps_vmax / full_lines;
    ae_sns_dft->full_lines            = ox03f10_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time          = ox03f10_ctx->fl[SENSOR_CUR_FRAME] - OX03F10_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]- ox03f10 set fps: %f\n", dev, ox03f10_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox03f10_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    ret = ox03f10_get_min_fps(ox03f10_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = ox03f10_get_max_fps(ox03f10_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = ox03f10_ctx->fps;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 ox03f10_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, OX03F10_NAME, sizeof(OX03F10_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property    = ox03f10_get_property;
    info->isp_func.pfn_sensor_set_work_mode   = ox03f10_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes  = ox03f10_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror          = ox03f10_mirror;
    info->isp_func.pfn_sensor_flip            = ox03f10_flip;
    info->isp_func.pfn_sensor_set_bus_info    = ox03f10_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr    = ox03f10_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param  = ox03f10_set_init_param;
    info->isp_func.pfn_sensor_start           = ox03f10_start;
    info->isp_func.pfn_sensor_stop            = ox03f10_stop;
    info->isp_func.pfn_sensor_standby         = ox03f10_standby;
    info->isp_func.pfn_sensor_resume          = ox03f10_resume;
    info->isp_func.pfn_sensor_write_reg       = ox03f10_write_reg;
    info->isp_func.pfn_sensor_read_reg        = ox03f10_read_reg;
    info->isp_func.pfn_sensor_init            = ox03f10_init;
    info->isp_func.pfn_sensor_exit            = ox03f10_exit;
    info->isp_func.pfn_sensor_set_attr        = ox03f10_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = ox03f10_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = ox03f10_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = ox03f10_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = ox03f10_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = ox03f10_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = ox03f10_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = ox03f10_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = ox03f10_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = ox03f10_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = ox03f10_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = ox03f10_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = ox03f10_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = ox03f10_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = ox03f10_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = ox03f10_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = ox03f10_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default       = ox03f10_get_awb_default;
    info->awb_func.pfn_sensor_set_color_temperature = ox03f10_set_color_temperature;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 ox03f10_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = ox03f10_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = ox03f10_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = ox03f10_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OX03F10 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 ox03f10_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *ox03f10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = ox03f10_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    OX03F10_GET_CTX(dev, ox03f10_ctx);
    SENSOR_CHECK_PTR_RETURN(ox03f10_ctx);

    if (ox03f10_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OX03F10 unregister function failed!\n");
        return ret;
    }

    ox03f10_ctx_exit(dev);
    g_ox03f10_dev_map[dev] = SENSOR_DEV_INVALID; // reset dev

    return XMEDIA_SUCCESS;
}
