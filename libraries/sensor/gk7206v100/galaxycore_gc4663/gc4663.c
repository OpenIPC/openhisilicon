#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gc4663.h"
#include "gc4663_ctrl.h"
#include "gc4663_ex.h"
#include "xmedia_isp.h"

#define GC4663_NAME          "GC4663"
#define GC4663_SPECS_MAX_NUM 2

#ifdef __linux__
//#define GC4663_ISP_DEFAULT_SUPPORT
#endif

#define GC4663_REG_ADDR_STANDBY 0x0100

#define GC4663_RES_IS_1440P(w, h) ((w) == 2560 && (h) == 1440)

#define GC4663_AGAIN_MAX_INDEX 26

#define GC4663_BIT_RATE_4M_LINEAR 700
#define GC4663_BIT_RATE_4M_WDR    1075

#define GC4663_ERR_MODE_PRINT(sns_attr)                                                                                \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sns_attr->width,                \
                     sns_attr->height, sns_attr->wdr_mode);                                                            \
    } while (0)

static sensor_context *g_gc4663_ctx[XMEDIA_SENSOR_DEV_MAX_NUM]     = { XMEDIA_NULL };
#define GC4663_GET_CTX(dev, ctx) ctx = g_gc4663_ctx[dev]
#define GC4663_SET_CTX(dev, ctx) g_gc4663_ctx[dev] = ctx

static xmedia_s32      g_gc4663_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] =
                                                                           SENSOR_DEV_INVALID };

static xmedia_u32 g_gc4663_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_gc4663_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_gc4663_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u16 g_gc4663_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

static const xmedia_sensor_property g_gc4663_property[GC4663_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        2560, 1440, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_GRBG, XMEDIA_VIDEO_BAYER_FMT_GRBG,
          XMEDIA_VIDEO_BAYER_FMT_GRBG, XMEDIA_VIDEO_BAYER_FMT_GRBG },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, GC4663_BIT_RATE_4M_LINEAR,
    },

    {
        // width, height, wdr_mode
        2560,  1440, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 25, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_GRBG, XMEDIA_VIDEO_BAYER_FMT_GRBG,
          XMEDIA_VIDEO_BAYER_FMT_GRBG, XMEDIA_VIDEO_BAYER_FMT_GRBG },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, GC4663_BIT_RATE_4M_WDR,
    },
};

static const xmedia_u32 analog_gain_table[GC4663_AGAIN_MAX_INDEX] =
{
    1024,
    1213,
    1433,
    1698,
    2048,
    2426,
    2867,
    3397,
    4096,
    4853,
    5734,
    6795,
    8192,
    9707,
    11468,
    13590,
    16384,
    19415,
    22937,
    27181,
    32768,
    38830,
    45875,
    54362,
    65536,
    77660,
};

static const xmedia_u32 analog_gain_table_liner[GC4663_AGAIN_MAX_INDEX] =
{
    1024,
    1213,
    1434,
    1699,
    2048,
    2427,
    2867,
    3398,
    4096,
    4854,
    5734,
    6795,
    8192,
    9708,
    11469,
    13591,
    16384,
    19415,
    22938,
    27181,
    32768,
    38830,
    45875,
    54362,
    65536,
    77660,
};

static const xmedia_u32 reg_val_table_wdr[GC4663_AGAIN_MAX_INDEX][7] =
{
    //2b3      2b4       2b8         2b9        515      519      2d9
    {0x00,  0x00,  0x01,  0x00,  0x30,  0x28,  0x66},
    {0x20,  0x00,  0x01,  0x0B,  0x30,  0x2a,  0x68},
    {0x01,  0x00,  0x01,  0x19,  0x30,  0x27,  0x65},
    {0x21,  0x00,  0x01,  0x2A,  0x30,  0x29,  0x67},
    {0x02,  0x00,  0x02,  0x00,  0x30,  0x27,  0x65},
    {0x22,  0x00,  0x02,  0x17,  0x30,  0x29,  0x67},
    {0x03,  0x00,  0x02,  0x33,  0x30,  0x28,  0x66},
    {0x23,  0x00,  0x03,  0x14,  0x30,  0x2a,  0x68},
    {0x04,  0x00,  0x04,  0x00,  0x30,  0x2a,  0x68},
    {0x24,  0x00,  0x04,  0x2F,  0x30,  0x2b,  0x69},
    {0x05,  0x00,  0x05,  0x26,  0x30,  0x2c,  0x6A},
    {0x25,  0x00,  0x06,  0x28,  0x30,  0x2e,  0x6C},
    {0x06,  0x00,  0x08,  0x00,  0x30,  0x2f,  0x6D},
    {0x26,  0x00,  0x09,  0x1E,  0x30,  0x31,  0x6F},
    {0x46,  0x00,  0x0B,  0x0C,  0x30,  0x34,  0x72},
    {0x66,  0x00,  0x0D,  0x11,  0x30,  0x37,  0x75},
    {0x0e,  0x00,  0x10,  0x00,  0x30,  0x3a,  0x78},
    {0x2e,  0x00,  0x12,  0x3D,  0x30,  0x3e,  0x7C},
    {0x4e,  0x00,  0x16,  0x19,  0x30,  0x41,  0x7F},
    {0x6e,  0x00,  0x1A,  0x22,  0x30,  0x45,  0x83},
    {0x1e,  0x00,  0x20,  0x00,  0x30,  0x49,  0x87},
    {0x3e,  0x00,  0x25,  0x3A,  0x30,  0x4d,  0x8B},
    {0x5e,  0x00,  0x2C,  0x33,  0x30,  0x53,  0x91},
    {0x7e,  0x00,  0x35,  0x05,  0x30,  0x5a,  0x98},
    {0x9e,  0x00,  0x40,  0x00,  0x30,  0x60,  0x9E},
    {0xbe,  0x00,  0x4B,  0x35,  0x30,  0x67,  0xA5},
};

static const xmedia_u32 reg_val_table_linear[GC4663_AGAIN_MAX_INDEX][7] =
{
   //2b3      2b4       2b8         2b9        515      519      2d9
    {0x00,  0x00,  0x01,  0x00,  0x30,  0x1e,  0x5C},
    {0x20,  0x00,  0x01,  0x0B,  0x30,  0x1e,  0x5C},
    {0x01,  0x00,  0x01,  0x19,  0x30,  0x1d,  0x5B},
    {0x21,  0x00,  0x01,  0x2A,  0x30,  0x1e,  0x5C},
    {0x02,  0x00,  0x02,  0x00,  0x30,  0x1e,  0x5C},
    {0x22,  0x00,  0x02,  0x17,  0x30,  0x1d,  0x5B},
    {0x03,  0x00,  0x02,  0x33,  0x20,  0x16,  0x54},
    {0x23,  0x00,  0x03,  0x14,  0x20,  0x17,  0x55},
    {0x04,  0x00,  0x04,  0x00,  0x20,  0x17,  0x55},
    {0x24,  0x00,  0x04,  0x2F,  0x20,  0x19,  0x57},
    {0x05,  0x00,  0x05,  0x26,  0x20,  0x19,  0x57},
    {0x25,  0x00,  0x06,  0x28,  0x20,  0x1b,  0x59},
    {0x0c,  0x00,  0x08,  0x00,  0x20,  0x1d,  0x5B},
    {0x2C,  0x00,  0x09,  0x1E,  0x20,  0x1f,  0x5D},
    {0x0D,  0x00,  0x0B,  0x0C,  0x20,  0x21,  0x5F},
    {0x2D,  0x00,  0x0D,  0x11,  0x20,  0x24,  0x62},
    {0x1C,  0x00,  0x10,  0x00,  0x20,  0x26,  0x64},
    {0x3C,  0x00,  0x12,  0x3D,  0x18,  0x2a,  0x68},
    {0x5C,  0x00,  0x16,  0x19,  0x18,  0x2c,  0x6A},
    {0x7C,  0x00,  0x1A,  0x22,  0x18,  0x2e,  0x6C},
    {0x9C,  0x00,  0x20,  0x00,  0x18,  0x32,  0x70},
    {0xBC,  0x00,  0x25,  0x3A,  0x18,  0x35,  0x73},
    {0xDC,  0x00,  0x2C,  0x33,  0x10,  0x36,  0x74},
    {0xFC,  0x00,  0x35,  0x05,  0x10,  0x38,  0x76},
    {0x1C,  0x01,  0x40,  0x00,  0x10,  0x3c,  0x7A},
    {0x3C,  0x01,  0x4B,  0x35,  0x10,  0x42,  0x80},
};

/* Rgain and Bgain of the golden sample */
#define GC4663_GOLDEN_RGAIN 0
#define GC4663_GOLDEN_BGAIN 0

/* AWB default parameter and function */
#define GC4663_CALIBRATE_STATIC_WB_R_GAIN 449
#define GC4663_CALIBRATE_STATIC_WB_GR_GAIN 256
#define GC4663_CALIBRATE_STATIC_WB_GB_GAIN 256
#define GC4663_CALIBRATE_STATIC_WB_B_GAIN 512

/* Calibration results for Auto WB Planck */
#define GC4663_CALIBRATE_AWB_P1 -6
#define GC4663_CALIBRATE_AWB_P2 185
#define GC4663_CALIBRATE_AWB_Q1 -76
#define GC4663_CALIBRATE_AWB_A1 159908
#define GC4663_CALIBRATE_AWB_B1 128
#define GC4663_CALIBRATE_AWB_C1 -106649

static const xmedia_sensor_capability g_gc4663_capability = {
    .max_width  = 2560,
    .max_height = 1440,
    .max_fps    = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_2L,
    .standby_info = {
        .standby_supported = XMEDIA_TRUE,
        .addr_byte_num     = GC4663_ADDR_BYTE,
        .data_byte_num     = GC4663_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { GC4663_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { GC4663_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x09 },
    },
};

static xmedia_s32 gc4663_ctx_init(xmedia_u32 dev)
{
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    GC4663_GET_CTX(dev, gc4663_ctx);
    if (gc4663_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    gc4663_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (gc4663_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(gc4663_ctx, 0, sizeof(sensor_context));
    gc4663_ctx->i2c_addr             = GC4663_I2C_ADDR;
    gc4663_ctx->lanes                = XMEDIA_SENSOR_MIPI_LANES_2L;
    gc4663_ctx->work_mode            = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    gc4663_ctx->bus_info.i2c_dev     = SENSOR_I2C_INVALID;
    gc4663_ctx->size.width           = g_gc4663_property[0].width;
    gc4663_ctx->size.height          = g_gc4663_property[0].height;
    gc4663_ctx->wdr_mode             = g_gc4663_property[0].wdr_mode;
    gc4663_ctx->fps                  = g_gc4663_property[0].max_fps;
    gc4663_ctx->img_mode             = GC4663_4M_10BIT_LINEAR_MODE;
    gc4663_ctx->fl_std               = GC4663_VMAX_4M_LINEAR;
    gc4663_ctx->fl[SENSOR_CUR_FRAME] = GC4663_VMAX_4M_LINEAR;
    gc4663_ctx->fl[SENSOR_PRE_FRAME] = GC4663_VMAX_4M_LINEAR;
    GC4663_SET_CTX(dev, gc4663_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void gc4663_ctx_exit(xmedia_u32 dev)
{
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_FREE(gc4663_ctx);
    GC4663_SET_CTX(dev, XMEDIA_NULL);

    return;
}

static xmedia_s32 gc4663_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    for (i = 0; i < GC4663_SPECS_MAX_NUM; i++) {
        if (g_gc4663_property[i].width == gc4663_ctx->size.width &&
            g_gc4663_property[i].height == gc4663_ctx->size.height &&
            g_gc4663_property[i].wdr_mode == gc4663_ctx->wdr_mode) {
            memcpy(property, &g_gc4663_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Please check input param!\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 gc4663_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc4663_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Invalid mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc4663_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        gc4663_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        gc4663_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        gc4663_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 gc4663_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    gc4663_ctx->i2c_addr = slave_addr;
    ret = gc4663_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    *slave_addr = gc4663_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

// called by ISP
static xmedia_s32 gc4663_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    gc4663_ctx->init_mode = init_mode;
    ret = gc4663_i2c_init(dev, gc4663_ctx->bus_info.i2c_dev, gc4663_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    gc4663_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    ret = gc4663_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    gc4663_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    gc4663_ctx->mirror_en = mirror_en;
    ret = gc4663_set_mirror_flip(dev, mirror_en, gc4663_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    gc4663_ctx->flip_en = flip_en;
    ret = gc4663_set_mirror_flip(dev, gc4663_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_set_image_mode(sensor_context *gc4663_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (gc4663_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (GC4663_RES_IS_1440P(sns_attr->width, sns_attr->height)) {
            *image_mode        = GC4663_4M_10BIT_LINEAR_MODE;
            gc4663_ctx->fl_std = GC4663_VMAX_4M_LINEAR;
        } else {
            GC4663_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else if (gc4663_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (GC4663_RES_IS_1440P(sns_attr->width, sns_attr->height)) {
            *image_mode               = GC4663_4M_10BIT_WDR_MODE;
            gc4663_ctx->fl_std        = GC4663_VMAX_4M_WDR;
        } else {
            GC4663_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        GC4663_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc4663_ctx->size.width  = sns_attr->width;
    gc4663_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            gc4663_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            gc4663_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
            SENSOR_PRINT("2to1 line WDR\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(gc4663_ctx->wdr_int_time, 0, sizeof(gc4663_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

#define GC4663_LINEAR_REG_INFO_MAX_NUM   GC4663_REG_L_MAX_NUM
#define GC4663_2TO1_WDR_REG_INFO_MAX_NUM GC4663_REG_MAX_NUM
static xmedia_void gc4663_init_common_reg_info(sensor_context *gc4663_ctx)
{
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_EXP_H].delay_frame_num =
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - GC4663_REG_DELAY_CONFIG_NUM;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_EXP_H].reg_addr        = GC4663_REG_ADDR_EXP_H;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_EXP_L].delay_frame_num =
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - GC4663_REG_DELAY_CONFIG_NUM;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_EXP_L].reg_addr        = GC4663_REG_ADDR_EXP_L;

    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_AGAIN_0_H].delay_frame_num =
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - GC4663_REG_DELAY_CONFIG_NUM;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_AGAIN_0_H].reg_addr        = GC4663_REG_ADDR_AGAIN_0_H;

    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_AGAIN_0_L].delay_frame_num =
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - GC4663_REG_DELAY_CONFIG_NUM;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_AGAIN_0_L].reg_addr        = GC4663_REG_ADDR_AGAIN_0_L;

    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_AGAIN_1_H].delay_frame_num =
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - GC4663_REG_DELAY_CONFIG_NUM;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_AGAIN_1_H].reg_addr        = GC4663_REG_ADDR_AGAIN_1_H;

    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_AGAIN_1_L].delay_frame_num =
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - GC4663_REG_DELAY_CONFIG_NUM;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_AGAIN_1_L].reg_addr        = GC4663_REG_ADDR_AGAIN_1_L;

    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_AGAIN_2].delay_frame_num =
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - GC4663_REG_DELAY_CONFIG_NUM;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_AGAIN_2].reg_addr        = GC4663_REG_ADDR_AGAIN_2;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_AGAIN_3].delay_frame_num =
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - GC4663_REG_DELAY_CONFIG_NUM;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_AGAIN_3].reg_addr        = GC4663_REG_ADDR_AGAIN_3;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_AGAIN_4].delay_frame_num =
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - GC4663_REG_DELAY_CONFIG_NUM;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_AGAIN_4].reg_addr        = GC4663_REG_ADDR_AGAIN_4;

    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_DGAIN_H].delay_frame_num =
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - GC4663_REG_DELAY_CONFIG_NUM;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_DGAIN_H].reg_addr        = GC4663_REG_ADDR_DGAIN_H;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_DGAIN_L].delay_frame_num =
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - GC4663_REG_DELAY_CONFIG_NUM;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_DGAIN_L].reg_addr        = GC4663_REG_ADDR_DGAIN_L;

    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_VMAX_H].delay_frame_num =
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - GC4663_REG_DELAY_CONFIG_NUM;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_VMAX_H].reg_addr        = GC4663_REG_ADDR_VMAX_H;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_VMAX_L].delay_frame_num =
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - GC4663_REG_DELAY_CONFIG_NUM;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_VMAX_L].reg_addr        = GC4663_REG_ADDR_VMAX_L;

    return;
}

static xmedia_void gc4663_init_2to1_wdr_reg_info(sensor_context *gc4663_ctx)
{
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_S_EXP_MAX_H].delay_frame_num =
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_S_EXP_MAX_H].reg_addr = GC4663_REG_ADDR_S_EXP_H;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_S_EXP_MAX_L].delay_frame_num =
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_S_EXP_MAX_L].reg_addr = GC4663_REG_ADDR_S_EXP_L;

    return;
}

static xmedia_s32 gc4663_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = gc4663_ctx->bus_info.i2c_dev;
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = GC4663_LINEAR_REG_INFO_MAX_NUM;

    if (gc4663_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].reg_num = GC4663_2TO1_WDR_REG_INFO_MAX_NUM;
    }

    for (i = 0; i < gc4663_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = gc4663_ctx->i2c_addr;
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = GC4663_ADDR_BYTE;
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = GC4663_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    gc4663_init_common_reg_info(gc4663_ctx);

    // init 2to1 wdr mode Regs
    if (gc4663_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        gc4663_init_2to1_wdr_reg_info(gc4663_ctx);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    // Set wdr mode
    ret = gc4663_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = gc4663_set_image_mode(gc4663_ctx, &gc4663_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    gc4663_ctx->fl[SENSOR_CUR_FRAME] = gc4663_ctx->fl_std;
    gc4663_ctx->fl[SENSOR_PRE_FRAME] = gc4663_ctx->fl[SENSOR_CUR_FRAME];

    ret = gc4663_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef GC4663_ISP_DEFAULT_SUPPORT
    // TO-DO: alg add param
    isp_default->blc         = &g_gc4663_blc;
    isp_default->bnr         = &g_gc4663_bnr;
    isp_default->clut_attr   = &g_gc4663_clut_attr;
    isp_default->crosstalk   = &g_gc4663_crosstalk;
    isp_default->csc         = &g_gc4663_csc;
    isp_default->dehaze      = &g_gc4663_dehaze;
    isp_default->demosaic    = &g_gc4663_dms;
    isp_default->dpc_dynamic = &g_gc4663_dpc;
    isp_default->dpc_static  = XMEDIA_NULL;
    isp_default->drc         = &g_gc4663_drc;
    isp_default->expander    = XMEDIA_NULL;
    isp_default->fpn         = XMEDIA_NULL;
    isp_default->gamma       = &g_gc4663_gamma;
    isp_default->gcac        = &g_gc4663_gcac;
    isp_default->hlc         = XMEDIA_NULL;
    isp_default->lce         = &g_gc4663_lce;
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from gc4663!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 gc4663_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x100;
    black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x100;
    black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x100;
    black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x100;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    if (gc4663_ctx->mirror_en && gc4663_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (gc4663_ctx->mirror_en && (!gc4663_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!gc4663_ctx->mirror_en) && gc4663_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < GC4663_SPECS_MAX_NUM; i++) {
        if (g_gc4663_property[i].width == gc4663_ctx->size.width &&
            g_gc4663_property[i].height == gc4663_ctx->size.height &&
            g_gc4663_property[i].wdr_mode == gc4663_ctx->wdr_mode) {
            *bayer_pattern = g_gc4663_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= GC4663_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);
    memcpy(capability, &g_gc4663_capability, sizeof(xmedia_sensor_capability));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc4663_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，是AE/AWB头几帧计算的基础
static xmedia_s32 gc4663_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_gc4663_exposure[dev]      = init_param->exposure;
    g_gc4663_sample_r_gain[dev] = init_param->sample_rgain;
    g_gc4663_sample_b_gain[dev] = init_param->sample_bgain;

    g_gc4663_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_gc4663_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_gc4663_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

xmedia_s32 gc4663_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case GC4663_4M_10BIT_LINEAR_MODE:
            *max_fps = 30.0;
            break;

        case GC4663_4M_10BIT_WDR_MODE:
            *max_fps = 25.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = gc4663_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256;
    ae_sns_dft->full_lines_max = GC4663_FULL_LINES_MAX;
    ae_sns_dft->ae_run_interval = 1;
    ae_sns_dft->init_exposure = 10;

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy  = 0.015625;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 2 << ae_sns_dft->ispdgain_shift;

    memcpy(&ae_sns_dft->piris_attr, &g_gc4663_piris_attr, sizeof(xmedia_isp_piris_attr));
    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_32_0;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    // lines per second: VTS * max_fps
    ae_sns_dft->lines_per_500ms  = (sns_ctx->fl_std * max_fps) / 2;
    ae_sns_dft->man_ratio_enable = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_void gc4663_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->max_again        = GC4663_AGAIN_MAX;
    ae_sns_dft->min_again        = GC4663_AGAIN_MIN;  // min: 1 * 1024
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 512;
    ae_sns_dft->min_dgain        = 64;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x38;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_gc4663_exposure[dev] ? g_gc4663_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - GC4663_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 1;     // min 1
    ae_sns_dft->max_int_time_target = GC4663_SHUTTER_MAX; // max 0x3fdb
    ae_sns_dft->min_int_time_target = 1;     // min 1

    return;
}

static xmedia_s32 gc4663_get_ae_2to1_wdr_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                 xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    ae_sns_dft->max_again        = GC4663_WDR_AGAIN_MAX;
    ae_sns_dft->min_again        = GC4663_WDR_AGAIN_MIN;  // 1 * 1024
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 512; // 31.5 * 1024
    ae_sns_dft->min_dgain        = 64;  // 1 * 1024
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;
    ae_sns_dft->ae_compensation     = 0x18;
    ae_sns_dft->init_exposure    = g_gc4663_exposure[dev] ? g_gc4663_exposure[dev] : 270762;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - sns_ctx->max_short_exp - GC4663_EXP_OFFSET_WDR;
    ae_sns_dft->min_int_time        = 2;     // min 2
    ae_sns_dft->max_int_time_target = 65535; // max 65535
    ae_sns_dft->min_int_time_target = 2;     // min 2

    if (gc4663_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
        ae_sns_dft->ae_compensation = 0x38;
        ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    } else {
        ae_sns_dft->max_ispdgain_target = 1024;
        ae_sns_dft->ae_compensation     = 0x18;
        ae_sns_dft->ae_exp_mode         = XMEDIA_ISP_AE_STRATEGY_EXP_LOWLIGHT_PRIOR;
        ae_sns_dft->man_ratio_enable    = XMEDIA_FALSE;

        /*
         * exp ratio array index( 4to1 wdr):
         * 0: short / very shourt;
         * 1: middle / short;
         * 2: long / middle
         * 2to1 line wdr：ratio[0] valid
         */
        ae_sns_dft->ratio[0] = 0x400;
        ae_sns_dft->ratio[1] = 0x40;
        ae_sns_dft->ratio[2] = 0x40;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = gc4663_get_ae_common_default(dev, gc4663_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (gc4663_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            gc4663_get_ae_linear_default(dev, gc4663_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            gc4663_get_ae_2to1_wdr_default(dev, gc4663_ctx, ae_sns_dft);
            break;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context    *gc4663_ctx                       = XMEDIA_NULL;
    xmedia_u32         val;
    static xmedia_bool first[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 1 };
    static xmedia_u32  short_int_time                   = 37;
    static xmedia_u32  long_int_time                    = 128;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    val = SENSOR_MIN(int_time, GC4663_FULL_LINES_MAX);

    if (gc4663_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (first[dev]) { // 0: short exposure
            gc4663_ctx->wdr_int_time[0] = int_time;
            short_int_time = val;
            first[dev] = XMEDIA_FALSE;
        } else { // 1: long exposure
            gc4663_ctx->wdr_int_time[1] = int_time;
            long_int_time               = val;

            gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_EXP_H].data =
                SENSOR_LOW_8BITS(long_int_time);
            gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_EXP_L].data =
                SENSOR_HIGH_8BITS(long_int_time);
            gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_S_EXP_MAX_H].data =
                SENSOR_LOW_8BITS(short_int_time);
            gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_S_EXP_MAX_L].data =
                SENSOR_HIGH_8BITS(short_int_time);
            first[dev] = XMEDIA_TRUE;
        }
    } else {
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_EXP_H].data = SENSOR_LOW_8BITS(val);
        gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_EXP_L].data = SENSOR_HIGH_8BITS(val);
        first[dev] = XMEDIA_TRUE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (gc4663_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        for (i = 0; i < GC4663_AGAIN_MAX_INDEX - 1 ; i++) {
            if (*again_lin < analog_gain_table[i + 1]) {
                break;
            }
        }
        *again_lin = analog_gain_table[i];
        *again_db = i ;
    } else {
          for (i = 0; i < GC4663_AGAIN_MAX_INDEX - 1 ; i++) {
                if (*again_lin < analog_gain_table_liner[i + 1]) {
                break;
            }
        }
        *again_lin = analog_gain_table[i];
        *again_db = i ;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *gc4663_ctx = XMEDIA_NULL;
    xmedia_u8 dgain_h, dgain_l;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    dgain_h = (dgain >> 6) & 0x0F;
    dgain_l = (dgain & 0x3F) << 2;
    if (gc4663_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        gc4663_ctx->regs_info[0].i2c_data[GC4663_REG_AGAIN_0_H].data = reg_val_table_wdr[again][0];
        gc4663_ctx->regs_info[0].i2c_data[GC4663_REG_AGAIN_0_L].data = reg_val_table_wdr[again][1];
        gc4663_ctx->regs_info[0].i2c_data[GC4663_REG_AGAIN_1_H].data = reg_val_table_wdr[again][2];
        gc4663_ctx->regs_info[0].i2c_data[GC4663_REG_AGAIN_1_L].data = reg_val_table_wdr[again][3];
        gc4663_ctx->regs_info[0].i2c_data[GC4663_REG_AGAIN_2].data   = reg_val_table_wdr[again][4];
        gc4663_ctx->regs_info[0].i2c_data[GC4663_REG_AGAIN_3].data   = reg_val_table_wdr[again][5];
        gc4663_ctx->regs_info[0].i2c_data[GC4663_REG_AGAIN_4].data   = reg_val_table_wdr[again][6];
    } else {
        gc4663_ctx->regs_info[0].i2c_data[GC4663_REG_AGAIN_0_H].data = reg_val_table_linear[again][0];
        gc4663_ctx->regs_info[0].i2c_data[GC4663_REG_AGAIN_0_L].data = reg_val_table_linear[again][1];
        gc4663_ctx->regs_info[0].i2c_data[GC4663_REG_AGAIN_1_H].data = reg_val_table_linear[again][2];
        gc4663_ctx->regs_info[0].i2c_data[GC4663_REG_AGAIN_1_L].data = reg_val_table_linear[again][3];
        gc4663_ctx->regs_info[0].i2c_data[GC4663_REG_AGAIN_2].data   = reg_val_table_linear[again][4];
        gc4663_ctx->regs_info[0].i2c_data[GC4663_REG_AGAIN_3].data   = reg_val_table_linear[again][5];
        gc4663_ctx->regs_info[0].i2c_data[GC4663_REG_AGAIN_4].data   = reg_val_table_linear[again][6];
    }

    gc4663_ctx->regs_info[0].i2c_data[GC4663_REG_DGAIN_H].data   = dgain_h;
    gc4663_ctx->regs_info[0].i2c_data[GC4663_REG_DGAIN_L].data   = dgain_l;

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 gc4663_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = 5082;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = GC4663_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = GC4663_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = GC4663_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = GC4663_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 6: point index on the awb curve param
    awb_sns_dft->wb_para[0]   = GC4663_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1]   = GC4663_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2]   = GC4663_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3]   = GC4663_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4]   = GC4663_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5]   = GC4663_CALIBRATE_AWB_C1;
    awb_sns_dft->golden_rgain = GC4663_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = GC4663_GOLDEN_BGAIN;

    switch (gc4663_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_gc4663_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_gc4663_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_gc4663_awb_wdr_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_gc4663_awb_wdr_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
    }

    awb_sns_dft->init_rgain   = g_gc4663_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_gc4663_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_gc4663_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_gc4663_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_gc4663_sample_b_gain[dev];
    awb_sns_dft->awb_runinterval = 4;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_standby(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = gc4663_write_reg(dev, GC4663_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_resume(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = gc4663_write_reg(dev, GC4663_REG_ADDR_STANDBY, 0x09);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *gc4663_ctx   = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < gc4663_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < gc4663_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= gc4663_write_reg(dev, gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_start(xmedia_u32 dev)
{
    xmedia_s32               ret;
    xmedia_u8                img_mode;
    xmedia_sensor_mipi_lanes mipi_lanes;
    xmedia_sensor_work_mode  work_mode;
    sensor_context          *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    work_mode  = gc4663_ctx->work_mode;
    mipi_lanes = gc4663_ctx->lanes;
    img_mode   = gc4663_ctx->img_mode;

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL || mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check param!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (gc4663_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = gc4663_init_image(dev, img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = gc4663_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = gc4663_set_mirror_flip(dev, gc4663_ctx->mirror_en, gc4663_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = gc4663_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_gc4663_dev_map[index] == SENSOR_DEV_INVALID) {
            g_gc4663_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 gc4663_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_gc4663_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 gc4663_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{ // TO-DO：min fps is to be confirmed.
    switch (img_mode) {
        case GC4663_4M_10BIT_LINEAR_MODE:
            *min_fps = 2;
            break;
        case GC4663_4M_10BIT_WDR_MODE:
            *min_fps = 2;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case GC4663_4M_10BIT_LINEAR_MODE:
            *vmax = GC4663_VMAX_4M_LINEAR;
            break;

        case GC4663_4M_10BIT_WDR_MODE:
            *vmax = GC4663_VMAX_4M_WDR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = gc4663_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc4663_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc4663_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is (%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, GC4663_FULL_LINES_MAX);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    for (i = 0; i < gc4663_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            gc4663_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &gc4663_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&gc4663_ctx->regs_info[SENSOR_PRE_FRAME], &gc4663_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    gc4663_ctx->fl[SENSOR_PRE_FRAME] = gc4663_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    ret = gc4663_calc_fps(fps, gc4663_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    gc4663_ctx->regs_info[0].i2c_data[GC4663_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    gc4663_ctx->regs_info[0].i2c_data[GC4663_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    gc4663_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    gc4663_ctx->fl_std               = gc4663_ctx->fl[SENSOR_CUR_FRAME];
    gc4663_ctx->fps                  = fps;
    ae_sns_dft->fps                  = fps;
    ae_sns_dft->full_lines_std       = gc4663_ctx->fl_std;
    ae_sns_dft->full_lines           = gc4663_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time         = gc4663_ctx->fl_std - 8;

    SENSOR_PRINT("dev[%d]- gc4663 set fps = %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = gc4663_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc4663_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc4663_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = vmax_max_fps * max_fps / min_fps;
    return XMEDIA_SUCCESS;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 gc4663_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    xmedia_u32      ratio;
    xmedia_u32      short_frame_min_exp;
    xmedia_u32      max_inttime_temp0 = 0;
    xmedia_u32      max_inttime_temp1 = 2;
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);
    memcpy(gc4663_ctx->ratio, inttime_attr->ratio, sizeof(inttime_attr->ratio));

    ratio = gc4663_ctx->ratio[0];
    short_frame_min_exp = 2;

    if (gc4663_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (gc4663_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
            max_inttime_temp0 = gc4663_ctx->fl[1] - 6 -gc4663_ctx->wdr_int_time[0];
            max_inttime_temp1 = gc4663_ctx->fl[0] - 8;
            max_inttime_temp1 = SENSOR_MIN(max_inttime_temp0, max_inttime_temp1);
        } else {
            max_inttime_temp0 = (gc4663_ctx->fl[1] - 16 -gc4663_ctx->wdr_int_time[0]) * 0x40 /
                SENSOR_DIV_0_TO_1(ratio);
            // exp_short <= vb
            // vb = frame_length - window_height - 32
            // window_height = 1456
            // max_exp_short = frame_length - 1456 - 32
            max_inttime_temp1 = gc4663_ctx->fl[0] - 1456 - 16 - 20 - 6;
            max_inttime_temp1 = SENSOR_MIN(max_inttime_temp0, max_inttime_temp1);
            max_inttime_temp1 = SENSOR_DIV_0_TO_1(max_inttime_temp1);

        }
    }

    inttime_attr->min_inttime[0] = short_frame_min_exp;
    inttime_attr->max_inttime[0] = max_inttime_temp1;

    inttime_attr->min_inttime[1] = inttime_attr->min_inttime[0] * ratio >> 6;
    inttime_attr->max_inttime[1] = inttime_attr->max_inttime[0] * ratio >> 6;

    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 * 注意：2to1 wdr模式下，曝光比和ae wdr mode 由AE传入，调用本接口前，AE需先调用gc4663_get_wdr_max_inttime
 */
static xmedia_s32 gc4663_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *gc4663_ctx = XMEDIA_NULL;
    xmedia_u32      full_line_max;
    xmedia_float    min_fps;
    xmedia_u32      min_fps_vmax;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    if (gc4663_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", gc4663_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = gc4663_get_min_fps(gc4663_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc4663_get_min_fps_vmax(gc4663_ctx->img_mode, &min_fps_vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if (gc4663_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        full_line_max = GC4663_FULL_LINES_MAX;
        full_lines                       = SENSOR_MIN(full_lines, full_line_max);
        gc4663_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    } else if (gc4663_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        full_line_max = 2 * GC4663_FULL_LINES_MAX;
        full_lines    = SENSOR_MIN(full_lines, full_line_max);
        gc4663_ctx->fl[SENSOR_CUR_FRAME] = (full_lines >> 1) << 1;
    }

    full_lines = SENSOR_MIN(full_lines, min_fps_vmax);
    gc4663_ctx->fps = min_fps * min_fps_vmax / full_lines;

    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_VMAX_H].data =
        SENSOR_HIGH_8BITS(full_lines);
    gc4663_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4663_REG_VMAX_L].data =
        SENSOR_LOW_8BITS(full_lines);

    ae_sns_dft->full_lines   = gc4663_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = gc4663_ctx->fl[SENSOR_CUR_FRAME] - 2;

    SENSOR_PRINT("dev[%d]- gc4663 set fps: %f", dev, gc4663_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    ret = gc4663_get_min_fps(gc4663_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc4663_get_max_fps(gc4663_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = gc4663_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4663_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, GC4663_NAME, sizeof(GC4663_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = gc4663_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = gc4663_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = gc4663_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = gc4663_mirror;
    info->isp_func.pfn_sensor_flip             = gc4663_flip;
    info->isp_func.pfn_sensor_set_bus_info     = gc4663_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = gc4663_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = gc4663_set_init_param;
    info->isp_func.pfn_sensor_start            = gc4663_start;
    info->isp_func.pfn_sensor_stop             = gc4663_stop;
    info->isp_func.pfn_sensor_standby          = gc4663_standby;
    info->isp_func.pfn_sensor_resume           = gc4663_resume;
    info->isp_func.pfn_sensor_write_reg        = gc4663_write_reg;
    info->isp_func.pfn_sensor_read_reg         = gc4663_read_reg;
    info->isp_func.pfn_sensor_init             = gc4663_init;
    info->isp_func.pfn_sensor_exit             = gc4663_exit;

    // called by ISP
    info->isp_func.pfn_sensor_set_attr            = gc4663_set_attr;
    info->isp_func.pfn_sensor_get_isp_default     = gc4663_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = gc4663_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = gc4663_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = gc4663_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = gc4663_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = gc4663_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = gc4663_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = gc4663_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = gc4663_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = gc4663_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = gc4663_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = gc4663_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = gc4663_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = gc4663_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = gc4663_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = gc4663_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = gc4663_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 gc4663_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = gc4663_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = gc4663_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = gc4663_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "GC4663 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 gc4663_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *gc4663_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = gc4663_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    GC4663_GET_CTX(dev, gc4663_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4663_ctx);

    if (gc4663_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "GC4663 unregister function failed!\n");
        return ret;
    }

    gc4663_ctx_exit(dev);
    g_gc4663_dev_map[dev] = SENSOR_DEV_INVALID; // reset dev

    return XMEDIA_SUCCESS;
}
