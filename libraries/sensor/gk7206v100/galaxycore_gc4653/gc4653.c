#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gc4653.h"
#include "gc4653_ctrl.h"
#include "gc4653_ex.h"
#include "xmedia_isp.h"

#define GC4653_NAME          "GC4653"
#define GC4653_SPECS_MAX_NUM 1

#define GC4653_BIT_RATE 700

#ifdef __linux__
//#define GC4653_ISP_DEFAULT_SUPPORT
#endif

#define GC4653_REG_ADDR_STANDBY 0x0100

#define GC4653_RES_IS_2160P(w, h) ((w) == 2560 && (h) == 1440)

#define GC4653_ERR_MODE_PRINT(sns_attr, mipi_lanes)                                                                    \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d, mipi_lanes:%d.\n",                 \
                     sns_attr->width, sns_attr->height, sns_attr->wdr_mode, mipi_lanes);                               \
    } while (0)

#define GC4653_REG_VAL_TAB_ROW          26
#define GC4653_REG_VAL_TAB_COL          7
#define GC4653_ANALOG_GAIN_LUT_SIZE     26
#define GC4653_ANALOG_GAIN_TAB_RANGE    26

static sensor_context *g_gc4653_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define GC4653_GET_CTX(dev, ctx) ctx = g_gc4653_ctx[dev]
#define GC4653_SET_CTX(dev, ctx) g_gc4653_ctx[dev] = ctx

static xmedia_s32 g_gc4653_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

static xmedia_u32 g_gc4653_again[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_gc4653_dgain[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_gc4653_isp_dgain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_gc4653_init_time[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u32 g_gc4653_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_gc4653_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_gc4653_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u16 g_gc4653_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

static const xmedia_sensor_property g_gc4653_property[GC4653_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        2560, 1440, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_NONE, 30, XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_GRBG, XMEDIA_VIDEO_BAYER_FMT_GRBG,
          XMEDIA_VIDEO_BAYER_FMT_GRBG, XMEDIA_VIDEO_BAYER_FMT_GRBG },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, GC4653_BIT_RATE,
    },
};

// awb static param for Fuji Lens New IR_Cut
#define GC4653_CALIBRATE_STATIC_TEMP       5082
#define GC4653_CALIBRATE_STATIC_WB_R_GAIN  404
#define GC4653_CALIBRATE_STATIC_WB_GR_GAIN 256
#define GC4653_CALIBRATE_STATIC_WB_GB_GAIN 256
#define GC4653_CALIBRATE_STATIC_WB_B_GAIN  509

// Calibration results for Auto WB Planck
#define GC4653_CALIBRATE_AWB_P1  12
#define GC4653_CALIBRATE_AWB_P2  213
#define GC4653_CALIBRATE_AWB_Q1  -30
#define GC4653_CALIBRATE_AWB_A1  186383
#define GC4653_CALIBRATE_AWB_B1  128
#define GC4653_CALIBRATE_AWB_C1  -131654

// Rgain and Bgain of the golden sample
#define GC4653_GOLDEN_RGAIN 0
#define GC4653_GOLDEN_BGAIN 0

static const xmedia_sensor_capability g_gc4653_capability = {
    .max_width  = 2560,
    .max_height = 1440,
    .max_fps    = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_NONE,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_2L,
    .standby_info = {
        .standby_supported = XMEDIA_TRUE,
        .addr_byte_num     = GC4653_ADDR_BYTE,
        .data_byte_num     = GC4653_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { GC4653_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { GC4653_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x09 },
    },
};

static const xmedia_u32 g_regvaltable[GC4653_REG_VAL_TAB_ROW][GC4653_REG_VAL_TAB_COL] = {
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

static const xmedia_u32 g_analog_gain_table[GC4653_ANALOG_GAIN_TAB_RANGE] = {
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
    77660
};

static xmedia_s32 gc4653_ctx_init(xmedia_u32 dev)
{
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    GC4653_GET_CTX(dev, gc4653_ctx);
    if (gc4653_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    gc4653_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (gc4653_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(gc4653_ctx, 0, sizeof(sensor_context));
    gc4653_ctx->i2c_addr             = GC4653_I2C_ADDR;
    gc4653_ctx->lanes                = XMEDIA_SENSOR_MIPI_LANES_2L;
    gc4653_ctx->work_mode            = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    gc4653_ctx->bus_info.i2c_dev     = SENSOR_I2C_INVALID;
    gc4653_ctx->size.width           = g_gc4653_property[0].width;
    gc4653_ctx->size.height          = g_gc4653_property[0].height;
    gc4653_ctx->wdr_mode             = g_gc4653_property[0].wdr_mode;
    gc4653_ctx->fps                  = g_gc4653_property[0].max_fps;
    gc4653_ctx->img_mode             = GC4653_4M_10BIT_LINEAR_MODE;
    gc4653_ctx->fl_std               = GC4653_VMAX_4M_LINEAR;
    gc4653_ctx->fl[SENSOR_CUR_FRAME] = GC4653_VMAX_4M_LINEAR;
    gc4653_ctx->fl[SENSOR_PRE_FRAME] = GC4653_VMAX_4M_LINEAR;
    GC4653_SET_CTX(dev, gc4653_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void gc4653_ctx_exit(xmedia_u32 dev)
{
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_FREE(gc4653_ctx);
    GC4653_SET_CTX(dev, XMEDIA_NULL);

    return;
}

static xmedia_s32 gc4653_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    for (i = 0; i < GC4653_SPECS_MAX_NUM; i++) {
        if (g_gc4653_property[i].width == gc4653_ctx->size.width &&
            g_gc4653_property[i].height == gc4653_ctx->size.height &&
            g_gc4653_property[i].wdr_mode == gc4653_ctx->wdr_mode) {
            memcpy(property, &g_gc4653_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Please check input param!\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 gc4653_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc4653_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Invalid mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc4653_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        gc4653_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        gc4653_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        gc4653_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 gc4653_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    gc4653_ctx->i2c_addr = slave_addr;
    ret = gc4653_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    *slave_addr = gc4653_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

// called by ISP
static xmedia_s32 gc4653_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    gc4653_ctx->init_mode = init_mode;
    ret = gc4653_i2c_init(dev, gc4653_ctx->bus_info.i2c_dev, gc4653_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    gc4653_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    ret = gc4653_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    gc4653_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    gc4653_ctx->mirror_en = mirror_en;
    ret = gc4653_set_mirror_flip(dev, mirror_en, gc4653_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    gc4653_ctx->flip_en = flip_en;
    ret = gc4653_set_mirror_flip(dev, gc4653_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_set_image_mode(sensor_context *gc4653_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    if (GC4653_RES_IS_2160P(sns_attr->width, sns_attr->height)) {
        *image_mode        = GC4653_4M_10BIT_LINEAR_MODE;
        gc4653_ctx->fl_std = GC4653_VMAX_4M_LINEAR;
    } else {
        GC4653_ERR_MODE_PRINT(sns_attr, gc4653_ctx->lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc4653_ctx->size.width  = sns_attr->width;
    gc4653_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            gc4653_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            gc4653_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
            SENSOR_PRINT("2to1 line WDR\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(gc4653_ctx->wdr_int_time, 0, sizeof(gc4653_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

#define GC4653_LINEAR_REG_INFO_MAX_NUM   GC4653_REG_L_MAX_NUM
static xmedia_void gc4653_init_common_reg_info(sensor_context *gc4653_ctx)
{
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_EXP_H].delay_frame_num    = 2;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_EXP_H].reg_addr           = GC4653_REG_ADDR_EXP_H;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_EXP_L].delay_frame_num    = 2;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_EXP_L].reg_addr           = GC4653_REG_ADDR_EXP_L;

    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT0].delay_frame_num  = 2;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT0].reg_addr         = GC4653_REG_ADDR_AGC_EXT0;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT1].delay_frame_num  = 2;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT1].reg_addr         = GC4653_REG_ADDR_AGC_EXT1;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT2].delay_frame_num  = 2;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT2].reg_addr         = GC4653_REG_ADDR_AGC_EXT2;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT3].delay_frame_num  = 2;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT3].reg_addr         = GC4653_REG_ADDR_AGC_EXT3;

    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT4].delay_frame_num  = 2;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT4].reg_addr         = GC4653_REG_ADDR_AGC_EXT4;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT5].delay_frame_num  = 2;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT5].reg_addr         = GC4653_REG_ADDR_AGC_EXT5;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT6].delay_frame_num  = 2;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT6].reg_addr         = GC4653_REG_ADDR_AGC_EXT6;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_DGC_H].delay_frame_num     = 2;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_DGC_H].reg_addr            = GC4653_REG_ADDR_DGC_H;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_DGC_L].delay_frame_num     = 2;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_DGC_L].reg_addr            = GC4653_REG_ADDR_DGC_L;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_VMAX_H].delay_frame_num    = 2;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_VMAX_H].reg_addr           = GC4653_REG_ADDR_VMAX_H;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_VMAX_L].delay_frame_num    = 2;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_VMAX_L].reg_addr           = GC4653_REG_ADDR_VMAX_L;

    return;
}

static xmedia_s32 gc4653_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32 i;
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.type           = XMEDIA_SENSOR_BUS_TYPE_I2C;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = gc4653_ctx->bus_info.i2c_dev;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = GC4653_LINEAR_REG_INFO_MAX_NUM;

    for (i = 0; i < gc4653_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = gc4653_ctx->i2c_addr;
        gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = GC4653_ADDR_BYTE;
        gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = GC4653_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    gc4653_init_common_reg_info(gc4653_ctx);

    return XMEDIA_SUCCESS;
}
static xmedia_s32 gc4653_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    // Set wdr mode
    ret = gc4653_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = gc4653_set_image_mode(gc4653_ctx, &gc4653_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    gc4653_ctx->fl[SENSOR_CUR_FRAME] = gc4653_ctx->fl_std;
    gc4653_ctx->fl[SENSOR_PRE_FRAME] = gc4653_ctx->fl[SENSOR_CUR_FRAME];

    ret = gc4653_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef GC4653_ISP_DEFAULT_SUPPORT
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from gc4653!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 gc4653_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (gc4653_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x100;
    } else {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x100;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    if (gc4653_ctx->mirror_en && gc4653_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (gc4653_ctx->mirror_en && (!gc4653_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!gc4653_ctx->mirror_en) && gc4653_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < GC4653_SPECS_MAX_NUM; i++) {
        if (g_gc4653_property[i].width == gc4653_ctx->size.width &&
            g_gc4653_property[i].height == gc4653_ctx->size.height &&
            g_gc4653_property[i].wdr_mode == gc4653_ctx->wdr_mode) {
            *bayer_pattern = g_gc4653_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= GC4653_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);
    memcpy(capability, &g_gc4653_capability, sizeof(xmedia_sensor_capability));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc4653_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 gc4653_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_gc4653_again[dev]         = init_param->again;
    g_gc4653_dgain[dev]         = init_param->dgain;
    g_gc4653_isp_dgain[dev]     = init_param->ispdgain;
    g_gc4653_init_time[dev]     = init_param->exp_time;
    g_gc4653_exposure[dev]      = init_param->exposure;
    g_gc4653_sample_r_gain[dev] = init_param->sample_rgain;
    g_gc4653_sample_b_gain[dev] = init_param->sample_bgain;

    g_gc4653_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_gc4653_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_gc4653_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

xmedia_s32 gc4653_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case GC4653_4M_10BIT_LINEAR_MODE:
            *max_fps = 30;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = gc4653_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines     = sns_ctx->fl_std;
    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256; // light flicker freq: 50Hz, accuracy: 256
    ae_sns_dft->full_lines_max = GC4653_FULL_LINES_MAX;

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1.0;
    ae_sns_dft->int_time_accu.offset    = 0.0;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1.0;
    ae_sns_dft->again_accu.offset    = 0;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy  = 0.015625;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 2 << ae_sns_dft->ispdgain_shift;

    memcpy(&ae_sns_dft->piris_attr, &g_gc4653_piris_attr, sizeof(xmedia_isp_piris_attr));
    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_32_0;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    // lines per second: VTS * max_fps
    ae_sns_dft->lines_per_500ms  = (sns_ctx->fl_std * max_fps) / 2;

    return XMEDIA_SUCCESS;
}

static xmedia_void gc4653_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->max_again  = 77660;
    ae_sns_dft->min_again  = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 512;
    ae_sns_dft->min_dgain        = 64;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_gc4653_exposure[dev] ? g_gc4653_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - GC4653_EXP_OFFSET;
    ae_sns_dft->min_int_time        = 1;
    ae_sns_dft->max_int_time_target = 0x3fdb; // max 65535
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    ae_sns_dft->ae_route_ex_valid = XMEDIA_FALSE;

    return;
}

static xmedia_s32 gc4653_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = gc4653_get_ae_common_default(dev, gc4653_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (gc4653_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            gc4653_get_ae_linear_default(dev, gc4653_ctx, ae_sns_dft);
            break;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context    *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    if (gc4653_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        SENSOR_TRACE(MODULE_DBG_ERR, "not support mode[%d]!\n", gc4653_ctx->wdr_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    } else if (gc4653_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        int_time = (int_time > GC4653_FULL_LINES_MAX) ? GC4653_FULL_LINES_MAX : int_time;
        gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_EXP_H].data = SENSOR_HIGH_8BITS(int_time);
        gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_EXP_L].data = SENSOR_LOW_8BITS(int_time);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_u32 again;
    xmedia_u32 i;
    static xmedia_u8 againmax = 25;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    again = *again_lin;

    if (again >= g_analog_gain_table[againmax]) {
        *again_lin = g_analog_gain_table[GC4653_ANALOG_GAIN_LUT_SIZE - 1];
        *again_db = againmax;
    } else {
        for (i = 0; i < GC4653_ANALOG_GAIN_LUT_SIZE - 1; i++) {
            if (again < g_analog_gain_table[i + 1]) {
                *again_lin = g_analog_gain_table[i];
                *again_db  = i;
                break;
            }
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    xmedia_u8 dgainhigh, dgainlow;
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);
    dgainhigh = (dgain >> 6) & 0x0f;
    dgainlow = (dgain & 0x3f) << 2;

    //again
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT0].data = g_regvaltable[again][0];
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT1].data = g_regvaltable[again][1];
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT2].data = g_regvaltable[again][2];
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT3].data = g_regvaltable[again][3];
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT4].data = g_regvaltable[again][4];
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT5].data = g_regvaltable[again][5];
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT6].data = g_regvaltable[again][6];

    //dgain
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_DGC_H].data    = dgainhigh;
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_DGC_L].data    = dgainlow;

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 gc4653_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = GC4653_CALIBRATE_STATIC_TEMP;    // wb_ref_temp 5000

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = GC4653_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = GC4653_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = GC4653_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = GC4653_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 6: point index on the awb curve param
    awb_sns_dft->wb_para[0]   = GC4653_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1]   = GC4653_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2]   = GC4653_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3]   = GC4653_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4]   = GC4653_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5]   = GC4653_CALIBRATE_AWB_C1;
    awb_sns_dft->golden_rgain = GC4653_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = GC4653_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;

    switch (gc4653_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_gc4653_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_gc4653_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
    }

    awb_sns_dft->init_rgain   = g_gc4653_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_gc4653_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_gc4653_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_gc4653_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_gc4653_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_standby(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = gc4653_write_reg(dev, GC4653_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_resume(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = gc4653_write_reg(dev, GC4653_REG_ADDR_STANDBY, 0x09);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_config_init_param(xmedia_u32 dev)
{
    xmedia_u32 i;
    sensor_context *gc4653_ctx   = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < gc4653_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < gc4653_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= gc4653_write_reg(dev, gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_start(xmedia_u32 dev)
{
    xmedia_s32               ret;
    xmedia_u8                img_mode;
    xmedia_sensor_work_mode  work_mode;
    sensor_context          *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    work_mode  = gc4653_ctx->work_mode;
    img_mode   = gc4653_ctx->img_mode;

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check param!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (gc4653_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = gc4653_init_image(dev, img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = gc4653_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = gc4653_set_mirror_flip(dev, gc4653_ctx->mirror_en, gc4653_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = gc4653_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_gc4653_dev_map[index] == SENSOR_DEV_INVALID) {
            g_gc4653_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 gc4653_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_gc4653_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 gc4653_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    for (i = 0; i < gc4653_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            gc4653_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    if ((gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT3].update == XMEDIA_TRUE) ||
        (gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT4].update == XMEDIA_TRUE)) {
            gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT3].update  = XMEDIA_TRUE;
            gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT4].update  = XMEDIA_TRUE;
    }

    if((gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_EXP_H].update == XMEDIA_TRUE) ||
        ( gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT4].update == XMEDIA_TRUE)) {
        gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_EXP_H].update = XMEDIA_TRUE;
        gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_EXP_L].update = XMEDIA_TRUE;
    }

    if((gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_DGC_H].update == XMEDIA_TRUE) ||
        ( gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_DGC_L].update == XMEDIA_TRUE)) {
        gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_DGC_H].update = XMEDIA_TRUE;
        gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_DGC_L].update = XMEDIA_TRUE;
    }

    if((gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_VMAX_H].update == XMEDIA_TRUE) ||
        ( gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_VMAX_L].update == XMEDIA_TRUE)) {
        gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_VMAX_H].update = XMEDIA_TRUE;
        gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_VMAX_L].update = XMEDIA_TRUE;
    }

    if((gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT0].update == XMEDIA_TRUE) ||
       (gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT1].update == XMEDIA_TRUE) ||
       (gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT2].update == XMEDIA_TRUE) ||
       (gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT3].update == XMEDIA_TRUE) ||
       (gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT4].update == XMEDIA_TRUE) ||
       (gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT5].update == XMEDIA_TRUE) ||
       (gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT6].update == XMEDIA_TRUE)) {
            gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT0].update = XMEDIA_TRUE;
            gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT1].update = XMEDIA_TRUE;
            gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT2].update = XMEDIA_TRUE;
            gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT3].update = XMEDIA_TRUE;
            gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT4].update = XMEDIA_TRUE;
            gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT5].update = XMEDIA_TRUE;
            gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_AGC_EXT6].update = XMEDIA_TRUE;
    }

    memcpy(sns_regs_info, &gc4653_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&gc4653_ctx->regs_info[SENSOR_PRE_FRAME], &gc4653_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    gc4653_ctx->fl[SENSOR_PRE_FRAME] = gc4653_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{
    switch (img_mode) {
        case GC4653_4M_10BIT_LINEAR_MODE:
            *min_fps = 2.75;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case GC4653_4M_10BIT_LINEAR_MODE:
            *vmax = GC4653_VMAX_4M_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = gc4653_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc4653_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc4653_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = (vmax_max_fps ) * max_fps / min_fps;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;

    ret = gc4653_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc4653_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc4653_get_vmax(sns_ctx->img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %.2f, range of fps is [%.2f, %.2f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax_max_fps * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    ret = gc4653_calc_fps(fps, gc4653_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    gc4653_ctx->fl[SENSOR_CUR_FRAME]  = full_lines;
    gc4653_ctx->fl_std                = gc4653_ctx->fl[SENSOR_CUR_FRAME];
    gc4653_ctx->fps                   = fps;

    ae_sns_dft->fps                   = fps;
    ae_sns_dft->full_lines_std        = gc4653_ctx->fl_std;
    ae_sns_dft->full_lines            = gc4653_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time          = gc4653_ctx->fl[SENSOR_CUR_FRAME] - GC4653_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]-gc4653 set fps:%f\n", dev, fps);
    return XMEDIA_SUCCESS;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 gc4653_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
     return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines: AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines: AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 */
static xmedia_s32 gc4653_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32       ret;
    sensor_context  *gc4653_ctx = XMEDIA_NULL;
    xmedia_float     min_fps;
    xmedia_u32       min_fps_vmax;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    if (gc4653_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", gc4653_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = gc4653_get_min_fps(gc4653_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc4653_get_min_fps_vmax(gc4653_ctx->img_mode, &min_fps_vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                        = (full_lines > min_fps_vmax) ? min_fps_vmax : full_lines;
    full_lines                        = (full_lines > GC4653_FULL_LINES_MAX) ? GC4653_FULL_LINES_MAX : full_lines;
    gc4653_ctx->fl[SENSOR_CUR_FRAME]  = full_lines;
    gc4653_ctx->fps                   = min_fps * min_fps_vmax / full_lines;

    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    gc4653_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC4653_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    ae_sns_dft->full_lines   = gc4653_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = gc4653_ctx->fl[SENSOR_CUR_FRAME] - GC4653_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]-gc4653 set fps:%f\n", dev, gc4653_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    ret = gc4653_get_min_fps(gc4653_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc4653_get_max_fps(gc4653_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = gc4653_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc4653_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, GC4653_NAME, sizeof(GC4653_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = gc4653_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = gc4653_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = gc4653_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = gc4653_mirror;
    info->isp_func.pfn_sensor_flip             = gc4653_flip;
    info->isp_func.pfn_sensor_set_bus_info     = gc4653_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = gc4653_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = gc4653_set_init_param;
    info->isp_func.pfn_sensor_start            = gc4653_start;
    info->isp_func.pfn_sensor_stop             = gc4653_stop;
    info->isp_func.pfn_sensor_standby          = gc4653_standby;
    info->isp_func.pfn_sensor_resume           = gc4653_resume;
    info->isp_func.pfn_sensor_write_reg        = gc4653_write_reg;
    info->isp_func.pfn_sensor_read_reg         = gc4653_read_reg;
    info->isp_func.pfn_sensor_init             = gc4653_init;
    info->isp_func.pfn_sensor_exit             = gc4653_exit;

    // called by ISP
    info->isp_func.pfn_sensor_set_attr            = gc4653_set_attr;
    info->isp_func.pfn_sensor_get_isp_default     = gc4653_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = gc4653_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = gc4653_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = gc4653_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = gc4653_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = gc4653_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = gc4653_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = gc4653_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = gc4653_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = gc4653_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = gc4653_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = gc4653_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = gc4653_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = gc4653_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = gc4653_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = gc4653_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = gc4653_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 gc4653_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = gc4653_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = gc4653_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = gc4653_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "GC4653 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 gc4653_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *gc4653_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = gc4653_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    GC4653_GET_CTX(dev, gc4653_ctx);
    SENSOR_CHECK_PTR_RETURN(gc4653_ctx);

    if (gc4653_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "GC4653 unregister function failed!\n");
        return ret;
    }

    gc4653_ctx_exit(dev);
    g_gc4653_dev_map[dev] = SENSOR_DEV_INVALID; // reset dev

    return XMEDIA_SUCCESS;
}
