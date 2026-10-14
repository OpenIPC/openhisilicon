#include <stdlib.h>
#include <string.h>
#include "gc2083.h"
#include "gc2083_ctrl.h"
#include "gc2083_ex.h"
#include "xmedia_isp.h"

#define GC2083_NAME          "GC2083"
#define GC2083_SPECS_MAX_NUM 1

#ifdef __linux__
#define GC2083_ISP_DEFAULT_SUPPORT
#endif

#define GC2083_RES_IS_1620P(w, h) ((w) == 1920 && (h) == 1080)

#define GC2083_ERR_MODE_PRINT(sns_attr)                                                                                \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sns_attr->width,                \
                     sns_attr->height, sns_attr->wdr_mode);                                                            \
    } while (0)

// awb static param for Fuji Lens New IR_Cut
#define GC2083_CALIBRATE_STATIC_TEMP       5082
#define GC2083_CALIBRATE_STATIC_WB_R_GAIN  446
#define GC2083_CALIBRATE_STATIC_WB_GR_GAIN 256
#define GC2083_CALIBRATE_STATIC_WB_GB_GAIN 256
#define GC2083_CALIBRATE_STATIC_WB_B_GAIN  436

// Calibration results for Auto WB Planck
#define GC2083_CALIBRATE_AWB_P1 60
#define GC2083_CALIBRATE_AWB_P2 96
#define GC2083_CALIBRATE_AWB_Q1 (-100)
#define GC2083_CALIBRATE_AWB_A1 206542
#define GC2083_CALIBRATE_AWB_B1 128
#define GC2083_CALIBRATE_AWB_C1 (-154259)

// Rgain and Bgain of the golden sample
#define GC2083_GOLDEN_RGAIN 0
#define GC2083_GOLDEN_BGAIN 0

#define GC2083_AGAIN_INDEX_MAX  29
#define GC2083_AGAIN_REG_INDEX  13

sensor_context *g_gc2083_ctx[XMEDIA_SENSOR_DEV_MAX_NUM]     = { XMEDIA_NULL };
#define GC2083_SET_CTX(dev, sns_ctx) g_gc2083_ctx[dev] = sns_ctx
#define GC2083_GET_CTX(dev, sns_ctx) sns_ctx = g_gc2083_ctx[dev]

static xmedia_s32 g_gc2083_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] =
                                                                           SENSOR_DEV_INVALID };

static xmedia_u32 g_gc2083_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_gc2083_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_gc2083_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_bool g_is_ir_mode[XMEDIA_SENSOR_DEV_MAX_NUM]          = { 0 };
static xmedia_u16 g_gc2083_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };
static xmedia_u32 g_init_iso[XMEDIA_SENSOR_DEV_MAX_NUM] = {[0 ... (XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 100};
static xmedia_u16 g_init_ccm[XMEDIA_SENSOR_DEV_MAX_NUM][CCM_MATRIX_SIZE] = {{0}};

#define GC2083_2M_BIT_RATE_LINEAR 600
#define GC2083_REG_ADDR_STANDBY1 0x03f2
#define GC2083_REG_ADDR_STANDBY2 0x03fc

SENSOR_PRIORITY_RODATA static const xmedia_sensor_property g_gc2083_property[GC2083_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_NONE, 30, XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB,
          XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, GC2083_2M_BIT_RATE_LINEAR,
     },
};

static const xmedia_u32 gc2083_reg_val_table[GC2083_AGAIN_INDEX_MAX][GC2083_AGAIN_REG_INDEX] = {
   //0x00d0 0x0155 0x0410 0x0411 0x0412 0x0413 0x0414 0x0415 0x0416 0x0417 0x00b8 0x00b9 0x0dc1
    {0x00,  0x03,  0x11,  0x11,  0x11,  0x11,  0x6f,  0x6f,  0x6f,  0x6f,  0x01,  0x00, 0x00},
    {0x10,  0x03,  0x11,  0x11,  0x11,  0x11,  0x6f,  0x6f,  0x6f,  0x6f,  0x01,  0x0c, 0x00},
    {0x01,  0x03,  0x11,  0x11,  0x11,  0x11,  0x6f,  0x6f,  0x6f,  0x6f,  0x01,  0x1a, 0x00},
    {0x11,  0x03,  0x11,  0x11,  0x11,  0x11,  0x6f,  0x6f,  0x6f,  0x6f,  0x01,  0x2b, 0x00},
    {0x02,  0x03,  0x11,  0x11,  0x11,  0x11,  0x6f,  0x6f,  0x6f,  0x6f,  0x02,  0x00, 0x00},
    {0x12,  0x03,  0x11,  0x11,  0x11,  0x11,  0x6f,  0x6f,  0x6f,  0x6f,  0x02,  0x18, 0x00},
    {0x03,  0x03,  0x11,  0x11,  0x11,  0x11,  0x6f,  0x6f,  0x6f,  0x6f,  0x02,  0x33, 0x00},
    {0x13,  0x03,  0x11,  0x11,  0x11,  0x11,  0x6f,  0x6f,  0x6f,  0x6f,  0x03,  0x15, 0x00},
    {0x04,  0x03,  0x11,  0x11,  0x11,  0x11,  0x6f,  0x6f,  0x6f,  0x6f,  0x04,  0x00, 0x00},
    {0x14,  0x03,  0x11,  0x11,  0x11,  0x11,  0x6f,  0x6f,  0x6f,  0x6f,  0x04,  0xe0, 0x00},
    {0x05,  0x03,  0x11,  0x11,  0x11,  0x11,  0x6f,  0x6f,  0x6f,  0x6f,  0x05,  0x26, 0x00},
    {0x15,  0x03,  0x11,  0x11,  0x11,  0x11,  0x6f,  0x6f,  0x6f,  0x6f,  0x06,  0x2b, 0x00},
    {0x44,  0x03,  0x11,  0x11,  0x11,  0x11,  0x6f,  0x6f,  0x6f,  0x6f,  0x08,  0x00, 0x00},
    {0x54,  0x03,  0x11,  0x11,  0x11,  0x11,  0x6f,  0x6f,  0x6f,  0x6f,  0x09,  0x22, 0x00},
    {0x45,  0x03,  0x11,  0x11,  0x11,  0x11,  0x6f,  0x6f,  0x6f,  0x6f,  0x0b,  0x0d, 0x00},
    {0x55,  0x03,  0x11,  0x11,  0x11,  0x11,  0x6f,  0x6f,  0x6f,  0x6f,  0x0d,  0x16, 0x00},
    {0x04,  0x19,  0x16,  0x16,  0x16,  0x16,  0x6f,  0x6f,  0x6f,  0x6f,  0x10,  0x00, 0x01},
    {0x14,  0x19,  0x16,  0x16,  0x16,  0x16,  0x6f,  0x6f,  0x6f,  0x6f,  0x13,  0x04, 0x01},
    {0x24,  0x19,  0x16,  0x16,  0x16,  0x16,  0x6f,  0x6f,  0x6f,  0x6f,  0x16,  0x1a, 0x01},
    {0x34,  0x19,  0x16,  0x16,  0x16,  0x16,  0x6f,  0x6f,  0x6f,  0x6f,  0x1a,  0x2b, 0x01},
    {0x44,  0x36,  0x18,  0x18,  0x18,  0x18,  0x6f,  0x6f,  0x6f,  0x6f,  0x20,  0x00, 0x01},
    {0x54,  0x36,  0x18,  0x18,  0x18,  0x18,  0x6f,  0x6f,  0x6f,  0x6f,  0x26,  0x07, 0x01},
    {0x64,  0x36,  0x18,  0x18,  0x18,  0x18,  0x6f,  0x6f,  0x6f,  0x6f,  0x2c,  0x33, 0x01},
    {0x74,  0x36,  0x18,  0x18,  0x18,  0x18,  0x6f,  0x6f,  0x6f,  0x6f,  0x35,  0x17, 0x01},
    {0x84,  0x64,  0x16,  0x16,  0x16,  0x16,  0x72,  0x72,  0x72,  0x72,  0x35,  0x17, 0x01},
    {0x94,  0x64,  0x16,  0x16,  0x16,  0x16,  0x72,  0x72,  0x72,  0x72,  0x35,  0x17, 0x01},
    {0x85,  0x64,  0x16,  0x16,  0x16,  0x16,  0x72,  0x72,  0x72,  0x72,  0x35,  0x17, 0x01},
    {0x95,  0x64,  0x16,  0x16,  0x16,  0x16,  0x72,  0x72,  0x72,  0x72,  0x35,  0x17, 0x01},
    {0xa5,  0x64,  0x16,  0x16,  0x16,  0x16,  0x72,  0x72,  0x72,  0x72,  0x35,  0x17, 0x01},
};

static const xmedia_u32 g_gc2083_again_table[GC2083_AGAIN_INDEX_MAX] = {
    1024, 1216, 1440, 1712, 2048, 2432, 2864, 3408, 4096, 4880, 5728,
    6832, 8192, 9760, 11472, 13664, 16384, 19520, 22944, 27312, 32768,
    39024, 45872, 54640, 65536, 78048, 91744, 109280,128448,
};

SENSOR_PRIORITY_RODATA static const xmedia_sensor_capability g_gc2083_capability = {
    .max_width  = 1920,
    .max_height = 1080,
    .max_fps    = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
    .wdr_mode   = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width  = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time = 200, // TODO: 待后续确认复位所需时间
    .slave_mode_supported = XMEDIA_TRUE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_2L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = GC2083_ADDR_BYTE,
        .data_byte_num     = GC2083_DATA_BYTE,
        .standby_reg_num   = 2,
        .standby_reg_addr  = { GC2083_REG_ADDR_STANDBY1, GC2083_REG_ADDR_STANDBY2},
        .standby_reg_data  = { 0x00, 0x8F },
        .resume_reg_num    = 2,
        .resume_reg_addr   = { GC2083_REG_ADDR_STANDBY1, GC2083_REG_ADDR_STANDBY2},
        .resume_reg_data   = { 0x01, 0x8E },
    },
};

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2083_ctx_init(xmedia_u32 dev)
{
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    GC2083_GET_CTX(dev, gc2083_ctx);

    if (gc2083_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    gc2083_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (gc2083_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(gc2083_ctx, 0, sizeof(sensor_context));
    gc2083_ctx->i2c_addr         = GC2083_I2C_ADDR;
    gc2083_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_2L;
    gc2083_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    gc2083_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    gc2083_ctx->size.width       = 1920;
    gc2083_ctx->size.height      = 1080;
    gc2083_ctx->fps              = g_gc2083_property[0].max_fps;
    gc2083_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    gc2083_ctx->img_mode             = GC2083_2M_10BIT_LINEAR_MODE;
    gc2083_ctx->fl_std               = GC2083_VMAX_2M_LINEAR;
    gc2083_ctx->fl[SENSOR_CUR_FRAME] = GC2083_VMAX_2M_LINEAR;
    gc2083_ctx->fl[SENSOR_PRE_FRAME] = GC2083_VMAX_2M_LINEAR;

    GC2083_SET_CTX(dev, gc2083_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void gc2083_ctx_exit(xmedia_u32 dev)
{
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_FREE(gc2083_ctx);
    GC2083_SET_CTX(dev, XMEDIA_NULL);;

    return;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2083_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    for (i = 0; i < GC2083_SPECS_MAX_NUM; i++) {
        if (g_gc2083_property[i].width == gc2083_ctx->size.width &&
            g_gc2083_property[i].height == gc2083_ctx->size.height &&
            g_gc2083_property[i].wdr_mode == gc2083_ctx->wdr_mode) {
            memcpy(property, &g_gc2083_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n",
                    gc2083_ctx->size.width, gc2083_ctx->size.height, gc2083_ctx->wdr_mode);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 gc2083_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc2083_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "No support mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc2083_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2083_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        gc2083_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        gc2083_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        gc2083_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 gc2083_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    gc2083_ctx->i2c_addr = slave_addr;
    ret = gc2083_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    *slave_addr = gc2083_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2083_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    gc2083_ctx->init_mode = init_mode;

    ret = gc2083_i2c_init(dev, gc2083_ctx->bus_info.i2c_dev, gc2083_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    gc2083_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2083_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    ret = gc2083_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    gc2083_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    gc2083_ctx->mirror_en = mirror_en;

    ret = gc2083_set_mirror_flip(dev, mirror_en, gc2083_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    gc2083_ctx->flip_en = flip_en;

    ret = gc2083_set_mirror_flip(dev, gc2083_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2083_set_image_mode(sensor_context *gc2083_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (gc2083_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (GC2083_RES_IS_1620P(sns_attr->width, sns_attr->height)) {
            *image_mode         = GC2083_2M_10BIT_LINEAR_MODE;
            gc2083_ctx->fl_std  = GC2083_VMAX_2M_LINEAR;
        } else {
            GC2083_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        GC2083_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc2083_ctx->size.width  = sns_attr->width;
    gc2083_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2083_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            gc2083_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(gc2083_ctx->wdr_int_time, 0, sizeof(gc2083_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_void gc2083_init_common_reg_info(sensor_context *gc2083_ctx)
{
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_EXP_H].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_EXP_H].reg_addr        = GC2083_REG_ADDR_EXP_H;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_EXP_L].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_EXP_L].reg_addr        = GC2083_REG_ADDR_EXP_L;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_1].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_1].reg_addr        = GC2083_REG_ADDR_AGAIN_1;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_2].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_2].reg_addr        = GC2083_REG_ADDR_AGAIN_2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_3].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_3].reg_addr        = GC2083_REG_ADDR_AGAIN_3;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_4].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_4].reg_addr        = GC2083_REG_ADDR_AGAIN_4;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_5].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_5].reg_addr        = GC2083_REG_ADDR_AGAIN_5;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_6].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_6].reg_addr        = GC2083_REG_ADDR_AGAIN_6;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_7].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_7].reg_addr        = GC2083_REG_ADDR_AGAIN_7;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_8].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_8].reg_addr        = GC2083_REG_ADDR_AGAIN_8;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_9].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_9].reg_addr        = GC2083_REG_ADDR_AGAIN_9;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_10].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_10].reg_addr        = GC2083_REG_ADDR_AGAIN_10;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_11].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_11].reg_addr        = GC2083_REG_ADDR_AGAIN_11;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_12].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_12].reg_addr        = GC2083_REG_ADDR_AGAIN_12;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_13].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_13].reg_addr        = GC2083_REG_ADDR_AGAIN_13;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_14].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_14].reg_addr        = GC2083_REG_ADDR_AGAIN_14;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_15].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_AGAIN_15].reg_addr        = GC2083_REG_ADDR_AGAIN_15;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_DGAIN_H].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_DGAIN_H].reg_addr        = GC2083_REG_ADDR_DGAIN_H;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_DGAIN_L].delay_frame_num = 2;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_DGAIN_L].reg_addr        = GC2083_REG_ADDR_DGAIN_L;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_VMAX_L].delay_frame_num = 3;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_VMAX_L].reg_addr        = GC2083_REG_ADDR_VMAX_L;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_VMAX_H].delay_frame_num = 3;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_VMAX_H].reg_addr        = GC2083_REG_ADDR_VMAX_H;

    return;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2083_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = gc2083_ctx->bus_info.i2c_dev;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 3;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = GC2083_REG_MAX_NUM;

    for (i = 0; i < gc2083_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = gc2083_ctx->i2c_addr;
        gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = GC2083_ADDR_BYTE;
        gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = GC2083_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    gc2083_init_common_reg_info(gc2083_ctx);

    return XMEDIA_SUCCESS;
}
SENSOR_PRIORITY_FUNC xmedia_s32 gc2083_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    // Set wdr mode
    ret = gc2083_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = gc2083_set_image_mode(gc2083_ctx, &gc2083_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    gc2083_ctx->fl[SENSOR_CUR_FRAME] = gc2083_ctx->fl_std;
    gc2083_ctx->fl[SENSOR_PRE_FRAME] = gc2083_ctx->fl[SENSOR_CUR_FRAME];

    ret = gc2083_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));

#ifdef GC2083_ISP_DEFAULT_SUPPORT
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from gc2083!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 gc2083_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (gc2083_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 256;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 256;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 256;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 256;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    if (gc2083_ctx->mirror_en && gc2083_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (gc2083_ctx->mirror_en && (!gc2083_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!gc2083_ctx->mirror_en) && gc2083_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < GC2083_SPECS_MAX_NUM; i++) {
        if (g_gc2083_property[i].width == gc2083_ctx->size.width &&
            g_gc2083_property[i].height == gc2083_ctx->size.height &&
            g_gc2083_property[i].wdr_mode == gc2083_ctx->wdr_mode) {
            *bayer_pattern = g_gc2083_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= GC2083_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2083_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_gc2083_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_get_trig_attr(xmedia_sensor_trig_attr *trig_attr)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc2083_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 gc2083_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    if (init_param->init_iso) {
        g_init_iso[dev]    = init_param->init_iso;
    }

    g_gc2083_exposure[dev]      = init_param->exposure;
    g_gc2083_sample_r_gain[dev] = init_param->sample_rgain;
    g_gc2083_sample_b_gain[dev] = init_param->sample_bgain;

    g_gc2083_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_gc2083_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_gc2083_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    g_is_ir_mode[dev] = init_param->ir_mode;

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        g_init_ccm[dev][i] = init_param->ccm[i];
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->full_lines_std  = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq    = 50 * 256;
    ae_sns_dft->full_lines_max  = GC2083_FULL_LINES_MAX;
    ae_sns_dft->ae_run_interval = 1;
    ae_sns_dft->init_exposure   = 10;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy  = 0.015625;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 2 << ae_sns_dft->ispdgain_shift;

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

static xmedia_void gc2083_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 128448;
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 512;
    ae_sns_dft->min_dgain        = 64;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_gc2083_exposure[dev] ? g_gc2083_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - GC2083_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 1;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = 1;

    return;
}

static xmedia_s32 gc2083_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = gc2083_get_ae_common_default(dev, gc2083_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (gc2083_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            gc2083_get_ae_linear_default(dev, gc2083_ctx, ae_sns_dft);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", gc2083_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

     if (gc2083_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_EXP_H].data = SENSOR_LOW_8BITS(int_time);
        gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_EXP_L].data = SENSOR_HIGH_8BITS(int_time);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_gc2083_again_table[GC2083_AGAIN_INDEX_MAX - 1]) {
        *again_lin = g_gc2083_again_table[GC2083_AGAIN_INDEX_MAX - 1];
        *again_db  = GC2083_AGAIN_INDEX_MAX - 1;
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < GC2083_AGAIN_INDEX_MAX; i++) {
        if (*again_lin < g_gc2083_again_table[i]) {
            *again_lin = g_gc2083_again_table[i-1];
            *again_db  = i - 1;
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *gc2083_ctx = XMEDIA_NULL;
    xmedia_u8 high, low;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    high = (dgain >> 6) & 0x0f;
    low  = (dgain & 0x3f) << 2;

    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_AGAIN_1].data  = gc2083_reg_val_table[again][0];
    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_AGAIN_2].data  = gc2083_reg_val_table[again][1];
    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_AGAIN_3].data  = gc2083_reg_val_table[again][2];
    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_AGAIN_4].data  = gc2083_reg_val_table[again][3];
    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_AGAIN_5].data  = gc2083_reg_val_table[again][4];
    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_AGAIN_6].data  = gc2083_reg_val_table[again][5];
    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_AGAIN_7].data  = gc2083_reg_val_table[again][6];
    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_AGAIN_8].data  = gc2083_reg_val_table[again][7];
    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_AGAIN_9].data  = gc2083_reg_val_table[again][8];
    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_AGAIN_10].data = gc2083_reg_val_table[again][9];
    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_AGAIN_11].data = gc2083_reg_val_table[again][10];
    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_AGAIN_12].data = gc2083_reg_val_table[again][11];
    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_AGAIN_13].data = 0x2e;
    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_AGAIN_14].data = gc2083_reg_val_table[again][12];
    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_AGAIN_15].data = 0x28;

    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_DGAIN_H].data = high;
    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_DGAIN_L].data = low;

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 gc2083_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    xmedia_s32      i;
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = GC2083_CALIBRATE_STATIC_TEMP;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = GC2083_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = GC2083_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = GC2083_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = GC2083_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 5: point index on the awb curve param
    awb_sns_dft->wb_para[0] = GC2083_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1] = GC2083_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2] = GC2083_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3] = GC2083_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4] = GC2083_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5] = GC2083_CALIBRATE_AWB_C1;

    awb_sns_dft->golden_rgain = GC2083_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = GC2083_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 4;
    switch (gc2083_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_gc2083_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            if (g_is_ir_mode[dev] == XMEDIA_TRUE) {
                memcpy(&awb_sns_dft->agc_tbl, &g_gc2083_awb_agc_ir_table, sizeof(xmedia_isp_awb_agc_table));
            } else {
                memcpy(&awb_sns_dft->agc_tbl, &g_gc2083_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            }
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", gc2083_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_gc2083_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_gc2083_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_gc2083_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_gc2083_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_gc2083_sample_b_gain[dev];

    for (i = 0; i < CCM_MATRIX_SIZE; i++) {
        awb_sns_dft->init_ccm[i] = g_init_ccm[dev][i];
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 gc2083_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case GC2083_2M_10BIT_LINEAR_MODE:
            *max_fps = 30.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_standby(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = gc2083_write_reg(dev, GC2083_REG_ADDR_STANDBY1, 0x00);
    ret = gc2083_write_reg(dev, GC2083_REG_ADDR_STANDBY2, 0x8f);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_resume(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = gc2083_write_reg(dev, GC2083_REG_ADDR_STANDBY1, 0x01);
    ret = gc2083_write_reg(dev, GC2083_REG_ADDR_STANDBY2, 0x8e);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2083_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *gc2083_ctx   = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < gc2083_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < gc2083_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= gc2083_write_reg(dev, gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2083_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);


    if (gc2083_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = gc2083_init_image(dev, gc2083_ctx->img_mode, gc2083_ctx->work_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = gc2083_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = gc2083_set_mirror_flip(dev, gc2083_ctx->mirror_en, gc2083_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = gc2083_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2083_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_gc2083_dev_map[index] == SENSOR_DEV_INVALID) {
            g_gc2083_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2083_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_gc2083_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 gc2083_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{ // TO-DO：min fps is to be confirmed.
    switch (img_mode) {
        case GC2083_2M_10BIT_LINEAR_MODE:
            *min_fps = 2.06;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case GC2083_2M_10BIT_LINEAR_MODE:
            *vmax = GC2083_VMAX_2M_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines,
                                         xmedia_u32 *lines_per500ms)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = gc2083_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc2083_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc2083_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is [%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, GC2083_FULL_LINES_MAX);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    for (i = 0; i < gc2083_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            gc2083_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    // vmax 高位寄存不能单独更新，需与低位寄存器同时写, 且高/低位寄存器同时更新时，需先写高位寄存器, 高位寄存器才生效
    if (gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_VMAX_H].update == XMEDIA_TRUE) {
        gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_VMAX_L].update = XMEDIA_TRUE;
    }

    memcpy(sns_regs_info, &gc2083_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&gc2083_ctx->regs_info[SENSOR_PRE_FRAME], &gc2083_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    gc2083_ctx->fl[SENSOR_PRE_FRAME] = gc2083_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}


static xmedia_s32 gc2083_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    ret = gc2083_calc_fps(fps, gc2083_ctx, &full_lines, &ae_sns_dft->lines_per_500ms);
    SENSOR_CHECK_RET_RETURN(ret);

    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_VMAX_H].data = ((full_lines & 0x3F00) >> 8);
    gc2083_ctx->regs_info[0].i2c_data[GC2083_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    gc2083_ctx->fl_std               = full_lines;
    gc2083_ctx->fl[SENSOR_CUR_FRAME] = gc2083_ctx->fl_std;
    gc2083_ctx->fps                  = fps;

    ae_sns_dft->fps                  = fps;
    ae_sns_dft->full_lines_std       = gc2083_ctx->fl_std;
    ae_sns_dft->full_lines           = gc2083_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = gc2083_ctx->fl_std - GC2083_EXP_OFFSET_LINEAR;

    SENSOR_PRINT("dev[%d]- gc2083 set fps: %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = gc2083_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc2083_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc2083_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = (vmax_max_fps << GC2083_EXP_SHIFT) * max_fps / min_fps;
    return XMEDIA_SUCCESS;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 gc2083_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);

    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 * 注意：2to1 wdr模式下，曝光比和ae wdr mode 由AE传入，调用本接口前，AE需先调用gc2083_get_wdr_max_inttime
 */
static xmedia_s32 gc2083_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *gc2083_ctx = XMEDIA_NULL;
    xmedia_u32      vmax_min_fps;
    xmedia_s32      ret;
    xmedia_float    min_fps;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    if (gc2083_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", gc2083_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (gc2083_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        full_lines = full_lines + GC2083_EXP_OFFSET_LINEAR;
    }

    ret = gc2083_get_min_fps_vmax(gc2083_ctx->img_mode, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = gc2083_get_min_fps(gc2083_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                        = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    gc2083_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_VMAX_H].data = ((full_lines & 0x3F00) >> 8);
    gc2083_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2083_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    ae_sns_dft->full_lines   = gc2083_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = gc2083_ctx->fl[SENSOR_CUR_FRAME] - GC2083_EXP_OFFSET_LINEAR;

    gc2083_ctx->fps = min_fps * vmax_min_fps / full_lines;
    SENSOR_PRINT("dev[%d]- gc2083 set fps: %f\n", dev, gc2083_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2083_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    ret = gc2083_get_min_fps(gc2083_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc2083_get_max_fps(gc2083_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = gc2083_ctx->fps;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2083_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, GC2083_NAME, sizeof(GC2083_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = gc2083_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = gc2083_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = gc2083_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = gc2083_mirror;
    info->isp_func.pfn_sensor_flip             = gc2083_flip;
    info->isp_func.pfn_sensor_set_bus_info     = gc2083_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = gc2083_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = gc2083_set_init_param;
    info->isp_func.pfn_sensor_start            = gc2083_start;
    info->isp_func.pfn_sensor_stop             = gc2083_stop;
    info->isp_func.pfn_sensor_standby          = gc2083_standby;
    info->isp_func.pfn_sensor_resume           = gc2083_resume;
    info->isp_func.pfn_sensor_write_reg        = gc2083_write_reg;
    info->isp_func.pfn_sensor_read_reg         = gc2083_read_reg;
    info->isp_func.pfn_sensor_init             = gc2083_init;
    info->isp_func.pfn_sensor_exit             = gc2083_exit;

    // called by ISP
    info->isp_func.pfn_sensor_set_attr            = gc2083_set_attr;
    info->isp_func.pfn_sensor_get_isp_default     = gc2083_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = gc2083_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = gc2083_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = gc2083_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = gc2083_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = gc2083_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = gc2083_get_fps;
    info->isp_func.pfn_sensor_get_trig_attr       = gc2083_get_trig_attr;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = gc2083_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = gc2083_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = gc2083_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = gc2083_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = gc2083_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = gc2083_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = gc2083_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = gc2083_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = gc2083_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = gc2083_get_awb_default;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2083_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = gc2083_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = gc2083_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = gc2083_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "GC2083 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 gc2083_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *gc2083_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = gc2083_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    GC2083_GET_CTX(dev, gc2083_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2083_ctx);

    if (gc2083_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "GC2083 unregister function failed!\n");
        return ret;
    }

    gc2083_ctx_exit(dev);
    g_gc2083_dev_map[dev] = SENSOR_DEV_INVALID; // reset dev

    return XMEDIA_SUCCESS;
}
