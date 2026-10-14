#include <stdlib.h>
#include <string.h>
#include "gc20c3.h"
#include "gc20c3_ctrl.h"
#include "gc20c3_ex.h"
#include "xmedia_isp.h"

#define GC20C3_NAME          "GC20C3"
#define GC20C3_SPECS_MAX_NUM 1

#ifdef __linux__
#define GC20C3_ISP_DEFAULT_SUPPORT
#endif

#if GC20C3_2LANE_LINEAR_1920X1080_30FPS
#define GC20C3_HMAX_2M_1920x1080_LINEAR       0x3E8
#define GC20C3_HMAX_2M_1920x1080_LINEAR_SLAVE 0x3E8
#define GC20C3_2M_1080P_BIT_RATE_LINEAR       540
#define GC20C3_2LANE_LINEAR_1920X1080_MAX_FPS 30
#else
#define GC20C3_HMAX_2M_1920x1080_LINEAR       3000
#define GC20C3_HMAX_2M_1920x1080_LINEAR_SLAVE 3000
#define GC20C3_2M_1080P_BIT_RATE_LINEAR       684
#define GC20C3_2LANE_LINEAR_1920X1080_MAX_FPS 40
#endif

#define GC20C3_REG_ADDR_STANDBY 0x0100 // TODO: 待确认

#define GC20C3_RES_IS_1920x1080(w, h) ((w) == 1920 && (h) == 1080)

#define GC20C3_ERR_MODE_PRINT(sns_attr)                                                                   \
    do {                                                                                                 \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sns_attr->width,  \
                     sns_attr->height, sns_attr->wdr_mode);                                              \
    } while (0)

// awb static param for Fuji Lens New IR_Cut
#define GC20C3_CALIBRATE_STATIC_TEMP       5000
#define GC20C3_CALIBRATE_STATIC_WB_R_GAIN  369
#define GC20C3_CALIBRATE_STATIC_WB_GR_GAIN 256
#define GC20C3_CALIBRATE_STATIC_WB_GB_GAIN 256
#define GC20C3_CALIBRATE_STATIC_WB_B_GAIN  660

// Calibration results for Auto WB Planck
#define GC20C3_CALIBRATE_AWB_P1 108
#define GC20C3_CALIBRATE_AWB_P2 8
#define GC20C3_CALIBRATE_AWB_Q1 (-140)
#define GC20C3_CALIBRATE_AWB_A1 219225
#define GC20C3_CALIBRATE_AWB_B1 128
#define GC20C3_CALIBRATE_AWB_C1 (-152450)

// Rgain and Bgain of the golden sample
#define GC20C3_GOLDEN_RGAIN 0
#define GC20C3_GOLDEN_BGAIN 0

sensor_context *g_gc20c3_ctx[XMEDIA_SENSOR_DEV_MAX_NUM]     = { XMEDIA_NULL };
#define GC20C3_SET_CTX(dev, sns_ctx) g_gc20c3_ctx[dev] = sns_ctx
#define GC20C3_GET_CTX(dev, sns_ctx) sns_ctx = g_gc20c3_ctx[dev]

static xmedia_u32 g_gc20c3_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_gc20c3_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_gc20c3_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_gc20c3_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };
static xmedia_s32 g_gc20c3_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

static const xmedia_sensor_property g_gc20c3_property[GC20C3_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, GC20C3_2LANE_LINEAR_1920X1080_MAX_FPS, XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_GRBG, XMEDIA_VIDEO_BAYER_FMT_GRBG,
          XMEDIA_VIDEO_BAYER_FMT_GRBG, XMEDIA_VIDEO_BAYER_FMT_GRBG },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, GC20C3_2M_1080P_BIT_RATE_LINEAR,
    },
};

static const xmedia_sensor_capability g_gc20c3_capability = {
    .max_width   = 1920,
    .max_height  = 1080,
    .max_fps     = GC20C3_2LANE_LINEAR_1920X1080_MAX_FPS,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported  = XMEDIA_FALSE,
    .mipi_lanes_supported  = XMEDIA_SENSOR_MIPI_LANES_2L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = GC20C3_ADDR_BYTE,
        .data_byte_num     = GC20C3_DATA_BYTE,

        .standby_reg_num   = 1,
        .standby_reg_addr  = { 0x03fe },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 12,
        .resume_reg_addr   = { 0x03fe, 0x0d40, 0x0b4d, 0x03be, 0x03bb, 0x0d10,
                               0x0d10, 0x0100, 0x031c, 0x0336, 0x0336, 0x03fe },
        .resume_reg_data   = { 0x30, 0x01, 0x02, 0x7f, 0xff, 0x06, 0x07, 0x03, 0x1f, 0x01, 0x00, 0x00 },
    },
};

static xmedia_sensor_trig_attr g_gc20c3_trig_attr = {
    .trig_mode       = 0,
    .hs_enable       = XMEDIA_FALSE,
    .vs_enable       = XMEDIA_TRUE,
    .hs_invert       = 0,
    .vs_invert       = 0,
    .hs_time         = 0,
    .vs_time         = (1.0 / 30.0) * 1000000,
    // the time of 1 line
    .vs_active  = 1 * ((1.0 / ( GC20C3_VMAX_1920X1080_LINEAR * GC20C3_2LANE_LINEAR_1920X1080_MAX_FPS )) * 1000000),
    .delay_frame_num = 2,
};

#define GC20C3_AGAIN_MAX_NUM   13
#define GC20C3_AGAIN_INDEX_MAX    7

static const xmedia_u32 g_gc20c3_reg_val_table[GC20C3_AGAIN_MAX_NUM][GC20C3_AGAIN_INDEX_MAX] = {
    // 0d04 0d05  0e36  0e39  04a8  04a9  0052      |  实际倍数   | Again dB|
    { 0x00, 0x01, 0x15, 0x15, 0x01, 0x00, 0x64 }, //|  X1        | 0.00    |
    { 0x00, 0x02, 0x15, 0x15, 0x01, 0x1b, 0x64 }, //|  X1.43     | 3.09    |
    { 0x00, 0x03, 0x16, 0x16, 0x02, 0x00, 0x64 }, //|  X2.01     | 6.05    |
    { 0x00, 0x04, 0x17, 0x17, 0x02, 0x37, 0x64 }, //|  X2.87     | 9.17    |
    { 0x00, 0x05, 0x17, 0x17, 0x04, 0x02, 0x84 }, //|  X4.03     | 12.11   |
    { 0x00, 0x06, 0x18, 0x18, 0x05, 0x32, 0x84 }, //|  X5.79     | 15.25   |
    { 0x00, 0x07, 0x19, 0x19, 0x08, 0x05, 0x84 }, //|  X8.09     | 18.16   |
    { 0x04, 0x97, 0x1a, 0x1a, 0x0b, 0x10, 0x84 }, //|  X11.25    | 21.03   |
    { 0x08, 0x07, 0x1b, 0x1b, 0x10, 0x04, 0x84 }, //|  X16.07    | 24.12   |
    { 0x0a, 0x4f, 0x1c, 0x1c, 0x16, 0x22, 0x88 }, //|  X22.54    | 27.06   |
    { 0x0c, 0x07, 0x1d, 0x1d, 0x20, 0x06, 0x88 }, //|  X32.10    | 30.13   |
    { 0x0d, 0x2f, 0x1e, 0x1e, 0x2d, 0x10, 0x88 }, //|  X45.26    | 33.11   |
    { 0x0e, 0x07, 0x20, 0x20, 0x3f, 0x23, 0x72 }, //|  X63.56    | 36.06   |
};

static const xmedia_u32 g_gc20c3_again_table[GC20C3_AGAIN_MAX_NUM] = {
   1024, 1464, 2058, 2938, 4126, 5928, 8284, 11520, 16455, 23080, 32870, 43346, 65085
};

static xmedia_s32 gc20c3_ctx_init(xmedia_u32 dev)
{
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    GC20C3_GET_CTX(dev, gc20c3_ctx);
    if (gc20c3_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    gc20c3_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (gc20c3_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(gc20c3_ctx, 0, sizeof(sensor_context));
    gc20c3_ctx->i2c_addr         = GC20C3_I2C_ADDR;
    gc20c3_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_2L;
    gc20c3_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    gc20c3_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    gc20c3_ctx->size.width       = 1920;
    gc20c3_ctx->size.height      = 1080;
    gc20c3_ctx->fps              = 30;
    gc20c3_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    gc20c3_ctx->img_mode             = GC20C3_2M_1920X1080_LINEAR_MODE;
    gc20c3_ctx->fl_std               = GC20C3_VMAX_1920X1080_LINEAR;
    gc20c3_ctx->fl[SENSOR_CUR_FRAME] = GC20C3_VMAX_1920X1080_LINEAR;
    gc20c3_ctx->fl[SENSOR_PRE_FRAME] = GC20C3_VMAX_1920X1080_LINEAR;

    GC20C3_SET_CTX(dev, gc20c3_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void gc20c3_ctx_exit(xmedia_u32 dev)
{
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_FREE(gc20c3_ctx);
    GC20C3_SET_CTX(dev, XMEDIA_NULL);
}

static xmedia_s32 gc20c3_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    for (i = 0; i < GC20C3_SPECS_MAX_NUM; i++) {
        if (g_gc20c3_property[i].width == gc20c3_ctx->size.width &&
            g_gc20c3_property[i].height == gc20c3_ctx->size.height &&
            g_gc20c3_property[i].wdr_mode == gc20c3_ctx->wdr_mode) {
            memcpy(property, &g_gc20c3_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n",
                    gc20c3_ctx->size.width, gc20c3_ctx->size.height, gc20c3_ctx->wdr_mode);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 gc20c3_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL && work_mode != XMEDIA_SENSOR_WORK_MODE_SLAVE ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc20c3_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "No support mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc20c3_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        gc20c3_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        gc20c3_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        gc20c3_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 gc20c3_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    gc20c3_ctx->i2c_addr = slave_addr;
    ret = gc20c3_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    *slave_addr = gc20c3_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    gc20c3_ctx->init_mode = init_mode;

    ret = gc20c3_i2c_init(dev, gc20c3_ctx->bus_info.i2c_dev, gc20c3_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    gc20c3_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    ret = gc20c3_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    gc20c3_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    gc20c3_ctx->mirror_en = mirror_en;
    ret = gc20c3_set_mirror_flip(dev, mirror_en, gc20c3_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    gc20c3_ctx->flip_en = flip_en;
    ret = gc20c3_set_mirror_flip(dev, gc20c3_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_set_image_mode(sensor_context *gc20c3_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (gc20c3_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (GC20C3_RES_IS_1920x1080(sns_attr->width, sns_attr->height)) {
            *image_mode        = GC20C3_2M_1920X1080_LINEAR_MODE;
            gc20c3_ctx->fl_std = GC20C3_VMAX_1920X1080_LINEAR;
        } else {
            GC20C3_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        GC20C3_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc20c3_ctx->size.width  = sns_attr->width;
    gc20c3_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            gc20c3_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(gc20c3_ctx->wdr_int_time, 0, sizeof(gc20c3_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

static xmedia_void gc20c3_init_common_reg_info(sensor_context *gc20c3_ctx)
{
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_EXP_H].delay_frame_num = 2;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_EXP_H].reg_addr        = GC20C3_REG_ADDR_EXP_H;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_EXP_L].delay_frame_num = 2;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_EXP_L].reg_addr        = GC20C3_REG_ADDR_EXP_L;

    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_1].delay_frame_num = 1;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_1].reg_addr        = GC20C3_REG_ADDR_AGAIN_1;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_2].delay_frame_num = 1;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_2].reg_addr        = GC20C3_REG_ADDR_AGAIN_2;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_3].delay_frame_num = 1;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_3].reg_addr        = GC20C3_REG_ADDR_AGAIN_3;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_4].delay_frame_num = 1;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_4].reg_addr        = GC20C3_REG_ADDR_AGAIN_4;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_5].delay_frame_num = 1;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_5].reg_addr        = GC20C3_REG_ADDR_AGAIN_5;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_6].delay_frame_num = 1;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_6].reg_addr        = GC20C3_REG_ADDR_AGAIN_6;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_7].delay_frame_num = 1;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_7].reg_addr        = GC20C3_REG_ADDR_AGAIN_7;

    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_DGAIN_H].delay_frame_num = 1;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_DGAIN_H].reg_addr        = GC20C3_REG_ADDR_DGAIN_H;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_DGAIN_L].delay_frame_num = 1;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_DGAIN_L].reg_addr        = GC20C3_REG_ADDR_DGAIN_L;

    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_VMAX_H].delay_frame_num = 1;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_VMAX_H].reg_addr        = GC20C3_REG_ADDR_VMAX_H;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_VMAX_L].delay_frame_num = 1;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_VMAX_L].reg_addr        = GC20C3_REG_ADDR_VMAX_L;

    return;
}

static xmedia_s32 gc20c3_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = gc20c3_ctx->bus_info.i2c_dev;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = GC20C3_REG_MAX_NUM;

    for (i = 0; i < gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = gc20c3_ctx->i2c_addr;
        gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = GC20C3_ADDR_BYTE;
        gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = GC20C3_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    gc20c3_init_common_reg_info(gc20c3_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    // Set wdr mode
    ret = gc20c3_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = gc20c3_set_image_mode(gc20c3_ctx, &gc20c3_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    gc20c3_ctx->fl[SENSOR_CUR_FRAME] = gc20c3_ctx->fl_std;
    gc20c3_ctx->fl[SENSOR_PRE_FRAME] = gc20c3_ctx->fl[SENSOR_CUR_FRAME];

    ret = gc20c3_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef GC20C3_ISP_DEFAULT_SUPPORT
    // TO-DO: alg add param
    isp_default->ainr             = XMEDIA_NULL;
    isp_default->anti_false_color = XMEDIA_NULL;
    isp_default->blc              = XMEDIA_NULL;
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from gc20c3!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 gc20c3_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (gc20c3_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x104; // TODO: 待确认blc
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x104;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    if (gc20c3_ctx->mirror_en && gc20c3_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (gc20c3_ctx->mirror_en && (!gc20c3_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!gc20c3_ctx->mirror_en) && gc20c3_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < GC20C3_SPECS_MAX_NUM; i++) {
        if (g_gc20c3_property[i].width == gc20c3_ctx->size.width &&
            g_gc20c3_property[i].height == gc20c3_ctx->size.height &&
            g_gc20c3_property[i].wdr_mode == gc20c3_ctx->wdr_mode) {
            *bayer_pattern = g_gc20c3_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= GC20C3_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_gc20c3_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_get_trig_attr(xmedia_sensor_trig_attr *trig_attr)
{
    SENSOR_CHECK_PTR_RETURN(trig_attr);
    memcpy(trig_attr, &g_gc20c3_trig_attr, sizeof(xmedia_sensor_trig_attr));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc20c3_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 gc20c3_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_gc20c3_exposure[dev]      = init_param->exposure;
    g_gc20c3_sample_r_gain[dev] = init_param->sample_rgain;
    g_gc20c3_sample_b_gain[dev] = init_param->sample_bgain;

    g_gc20c3_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_gc20c3_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_gc20c3_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256;
    ae_sns_dft->full_lines_max = GC20C3_FULL_LINES_MAX;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 0;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy  = 0.0009765625;

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

static xmedia_void gc20c3_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 65085;
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 16380;
    ae_sns_dft->min_dgain        = 1024;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_gc20c3_exposure[dev] ? g_gc20c3_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - GC20C3_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 1;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = 1;

    return;
}

static xmedia_s32 gc20c3_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = gc20c3_get_ae_common_default(dev, gc20c3_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (gc20c3_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            gc20c3_get_ae_linear_default(dev, gc20c3_ctx, ae_sns_dft);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", gc20c3_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_EXP_H].data = SENSOR_HIGH_8BITS(int_time);
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_EXP_L].data = SENSOR_LOW_8BITS(int_time);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_gc20c3_again_table[GC20C3_AGAIN_MAX_NUM - 1]) {
        *again_lin = g_gc20c3_again_table[GC20C3_AGAIN_MAX_NUM - 1];
        *again_db  = GC20C3_AGAIN_MAX_NUM - 1;
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < GC20C3_AGAIN_MAX_NUM; i++) {
        if (*again_lin < g_gc20c3_again_table[i]) {
            *again_lin = g_gc20c3_again_table[i-1];
            *again_db  = i - 1;
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_DGAIN_H].data = SENSOR_HIGH_8BITS(dgain);
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_DGAIN_L].data = SENSOR_LOW_8BITS (dgain);
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_1].data = g_gc20c3_reg_val_table[again][0];
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_2].data = g_gc20c3_reg_val_table[again][1];
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_3].data = g_gc20c3_reg_val_table[again][2];
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_4].data = g_gc20c3_reg_val_table[again][3];
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_5].data = g_gc20c3_reg_val_table[again][4];
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_6].data = g_gc20c3_reg_val_table[again][5];
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_AGAIN_7].data = g_gc20c3_reg_val_table[again][6];

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 gc20c3_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    // awb_sns_dft->wb_ref_temp = GC20C3_CALIBRATE_STATIC_TEMP;

    // awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = GC20C3_CALIBRATE_STATIC_WB_R_GAIN;
    // awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = GC20C3_CALIBRATE_STATIC_WB_GR_GAIN;
    // awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = GC20C3_CALIBRATE_STATIC_WB_GB_GAIN;
    // awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = GC20C3_CALIBRATE_STATIC_WB_B_GAIN;

    // // 0 ~ 5: point index on the awb curve param
    // awb_sns_dft->wb_para[0] = GC20C3_CALIBRATE_AWB_P1;
    // awb_sns_dft->wb_para[1] = GC20C3_CALIBRATE_AWB_P2;
    // awb_sns_dft->wb_para[2] = GC20C3_CALIBRATE_AWB_Q1;
    // awb_sns_dft->wb_para[3] = GC20C3_CALIBRATE_AWB_A1;
    // awb_sns_dft->wb_para[4] = GC20C3_CALIBRATE_AWB_B1;
    // awb_sns_dft->wb_para[5] = GC20C3_CALIBRATE_AWB_C1;

    awb_sns_dft->golden_rgain = GC20C3_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = GC20C3_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (gc20c3_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_gc20c3_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_gc20c3_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", gc20c3_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_gc20c3_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_gc20c3_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_gc20c3_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_gc20c3_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_gc20c3_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

xmedia_s32 gc20c3_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case GC20C3_2M_1920X1080_LINEAR_MODE:
            *max_fps = GC20C3_2LANE_LINEAR_1920X1080_MAX_FPS;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_standby(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret |= gc20c3_write_reg(dev, 0x031c, 0x18);
    ret |= gc20c3_write_reg(dev, 0x03bb, 0x00);
    ret |= gc20c3_write_reg(dev, 0x03be, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0d10, 0x04);
    ret |= gc20c3_write_reg(dev, 0x0b4d, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0100, 0x00);
    ret |= gc20c3_write_reg(dev, 0x0d40, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_resume(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret |= gc20c3_write_reg(dev, 0x03fe, 0x30);
    ret |= gc20c3_write_reg(dev, 0x0d40, 0x01);
    ret |= gc20c3_write_reg(dev, 0x0b4d, 0x02);
    ret |= gc20c3_write_reg(dev, 0x03be, 0x7f);
    ret |= gc20c3_write_reg(dev, 0x03bb, 0xff);
    ret |= gc20c3_write_reg(dev, 0x0d10, 0x06);
    usleep(100); // 100us
    ret |= gc20c3_write_reg(dev, 0x0d10, 0x07);
    ret |= gc20c3_write_reg(dev, 0x0100, 0x03);
    ret |= gc20c3_write_reg(dev, 0x031c, 0x1f);
    ret |= gc20c3_write_reg(dev, 0x0336, 0x01);
    ret |= gc20c3_write_reg(dev, 0x0336, 0x00);
    ret |= gc20c3_write_reg(dev, 0x03fe, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_low_power(xmedia_u32 dev, xmedia_bool enable)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *gc20c3_ctx  = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= gc20c3_write_reg(dev, gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    if (gc20c3_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = gc20c3_init_image(dev, gc20c3_ctx->img_mode, gc20c3_ctx->work_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = gc20c3_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = gc20c3_set_mirror_flip(dev, gc20c3_ctx->mirror_en, gc20c3_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = gc20c3_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_gc20c3_dev_map[index] == SENSOR_DEV_INVALID) {
            g_gc20c3_dev_map[index] = pipe;
            *dev                     = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 gc20c3_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_gc20c3_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 gc20c3_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{
    switch (img_mode) {
        case GC20C3_2M_1920X1080_LINEAR_MODE:
            *min_fps = 1.5;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case GC20C3_2M_1920X1080_LINEAR_MODE:
            *vmax = GC20C3_VMAX_1920X1080_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_u32   vmax;
    xmedia_float max_fps, min_fps;

    ret = gc20c3_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc20c3_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc20c3_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is (%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, GC20C3_FULL_LINES_MAX);

    SENSOR_PRINT("gc20c3 set fps = %f\n", fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    for (i = 0; i < gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            gc20c3_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &gc20c3_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&gc20c3_ctx->regs_info[SENSOR_PRE_FRAME], &gc20c3_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    gc20c3_ctx->fl[SENSOR_PRE_FRAME] = gc20c3_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    ret = gc20c3_calc_fps(fps, gc20c3_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    gc20c3_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    gc20c3_ctx->fl_std               = gc20c3_ctx->fl[SENSOR_CUR_FRAME];
    gc20c3_ctx->fps                  = fps;

    ae_sns_dft->fps            = fps;
    ae_sns_dft->full_lines_std = gc20c3_ctx->fl_std;
    ae_sns_dft->full_lines     = gc20c3_ctx->fl_std;
    ae_sns_dft->max_int_time   = gc20c3_ctx->fl[SENSOR_CUR_FRAME] - GC20C3_EXP_OFFSET_LINEAR;

    SENSOR_PRINT("dev[%d]- gc20c3 set fps: %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = gc20c3_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc20c3_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc20c3_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = (vmax_max_fps) * max_fps / min_fps;

    return XMEDIA_SUCCESS;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 gc20c3_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 * 注意：2to1 wdr模式下，曝光比和ae wdr mode 由AE传入，调用本接口前，AE需先调用gc20c3_get_max_inttime
 */
static xmedia_s32 gc20c3_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *gc20c3_ctx = XMEDIA_NULL;
    xmedia_u32     vmax_min_fps;
    xmedia_s32     ret;
    xmedia_float   min_fps;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    if (gc20c3_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", gc20c3_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = gc20c3_get_min_fps_vmax(gc20c3_ctx->img_mode, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = gc20c3_get_min_fps(gc20c3_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                        = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    gc20c3_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    gc20c3_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC20C3_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    ae_sns_dft->full_lines   = gc20c3_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = gc20c3_ctx->fl[SENSOR_CUR_FRAME] - GC20C3_EXP_OFFSET_LINEAR;

    gc20c3_ctx->fps = min_fps * vmax_min_fps / full_lines;
    SENSOR_PRINT("sensor fps: %f\n", gc20c3_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_get_hmax_vmax(xmedia_u32 dev, xmedia_u32 *hmax, xmedia_u32 *vmax)
{
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(hmax);
    SENSOR_CHECK_PTR_RETURN(vmax);

    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    *vmax = gc20c3_ctx->fl[SENSOR_CUR_FRAME];

    switch (gc20c3_ctx->img_mode) {
        case GC20C3_2M_1920X1080_LINEAR_MODE:
            if (gc20c3_ctx->work_mode == XMEDIA_SENSOR_WORK_MODE_NORMAL) {
                *hmax = GC20C3_HMAX_2M_1920x1080_LINEAR << 1; // TODO: 待确认寄存器值是否就是真正的hmax
            } else if (gc20c3_ctx->work_mode == XMEDIA_SENSOR_WORK_MODE_SLAVE) {
                *hmax = GC20C3_HMAX_2M_1920x1080_LINEAR_SLAVE << 1; // TODO: 待确认寄存器值是否就是真正的hmax
            } else {
                SENSOR_TRACE(MODULE_DBG_ERR, "Not support work mode: %d\n", gc20c3_ctx->work_mode);
                return XMEDIA_ERRCODE_NOT_SUPPORT;
            }
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    ret = gc20c3_get_min_fps(gc20c3_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc20c3_get_max_fps(gc20c3_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = gc20c3_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc20c3_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, GC20C3_NAME, sizeof(GC20C3_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = gc20c3_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = gc20c3_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = gc20c3_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = gc20c3_mirror;
    info->isp_func.pfn_sensor_flip             = gc20c3_flip;
    info->isp_func.pfn_sensor_set_bus_info     = gc20c3_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = gc20c3_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = gc20c3_set_init_param;
    info->isp_func.pfn_sensor_start            = gc20c3_start;
    info->isp_func.pfn_sensor_stop             = gc20c3_stop;
    info->isp_func.pfn_sensor_standby          = gc20c3_standby;
    info->isp_func.pfn_sensor_resume           = gc20c3_resume;
    info->isp_func.pfn_sensor_low_power        = gc20c3_low_power;
    info->isp_func.pfn_sensor_write_reg        = gc20c3_write_reg;
    info->isp_func.pfn_sensor_read_reg         = gc20c3_read_reg;
    info->isp_func.pfn_sensor_init             = gc20c3_init;
    info->isp_func.pfn_sensor_exit             = gc20c3_exit;
    info->isp_func.pfn_sensor_set_attr         = gc20c3_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = gc20c3_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = gc20c3_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = gc20c3_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = gc20c3_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = gc20c3_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = gc20c3_get_dev_addr;
    info->isp_func.pfn_sensor_get_trig_attr       = gc20c3_get_trig_attr;
    info->isp_func.pfn_sensor_get_hmax_vmax       = gc20c3_get_hmax_vmax;
    info->isp_func.pfn_sensor_get_fps             = gc20c3_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = gc20c3_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = gc20c3_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = gc20c3_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = gc20c3_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = gc20c3_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = gc20c3_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = gc20c3_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = gc20c3_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = gc20c3_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = gc20c3_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 gc20c3_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = gc20c3_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = gc20c3_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = gc20c3_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "GC20C3 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 gc20c3_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *gc20c3_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = gc20c3_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    GC20C3_GET_CTX(dev, gc20c3_ctx);
    SENSOR_CHECK_PTR_RETURN(gc20c3_ctx);

    if (gc20c3_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "GC20C3 unregister function failed!\n");
        return ret;
    }

    gc20c3_ctx_exit(dev);
    g_gc20c3_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;
}
