#include <stdlib.h>
#include <string.h>
#include "gc2053.h"
#include "gc2053_ctrl.h"
#include "gc2053_ex.h"
#include "xmedia_isp.h"

#ifdef __linux__
//#define GC2053_ISP_DEFAULT_SUPPORT
#endif

#ifdef FPGA
#define GC2053_INPUT_CLOCK_LINEAR 6000000 // 6M
#define GC2053_MAX_FPS_LINEAR     6.6     // (30 * (GC2053_INPUT_CLOKC_LINEAR / XMEDIA_SENSOR_CLOCK_FREQ_27MHZ))
#define GC2053_BIT_RATE           600
#else
#define GC2053_INPUT_CLOCK_LINEAR XMEDIA_SENSOR_CLOCK_FREQ_24MHZ
#define GC2053_INPUT_CLOCK_WDR    XMEDIA_SENSOR_CLOCK_FREQ_27MHZ
#define GC2053_BIT_RATE           592
#ifdef GC2053_DVP
#define GC2053_MAX_FPS_LINEAR     25
#else
#define GC2053_MAX_FPS_LINEAR     30
#endif
#endif

#ifdef GC2053_DVP
#define XMEDIA_INTF_TYPE_INDEX  XMEDIA_INTF_TYPE_DC
#else
#define XMEDIA_INTF_TYPE_INDEX  XMEDIA_INTF_TYPE_MIPI_CSI
#endif

#define gc2053_NAME          "GC2053"
#define gc2053_SPECS_MAX_NUM 1
#define ISO_LUT_NUM          8

#define GC2053_AGAIN_MAX_NUM  29
#define GC2053_AGAIN_INDEX    6

#define GC2053_ERR_MODE_PRINT(sensor_image_mode)                                                                       \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sensor_image_mode->width,       \
                     sensor_image_mode->height, sensor_image_mode->wdr_mode);                                          \
    } while (0)

SENSOR_PRIORITY_DATA static sensor_context  *g_gc2053_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define GC2053_GET_CTX(dev, ctx) ctx = g_gc2053_ctx[dev]
#define GC2053_SET_CTX(dev, ctx) g_gc2053_ctx[dev] = ctx

SENSOR_PRIORITY_DATA static xmedia_s32 g_gc2053_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM]  =
                                            { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

static xmedia_u32 g_gc2053_linesper500ms[XMEDIA_SENSOR_DEV_MAX_NUM] = {0};
static xmedia_u32 g_gc2053_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]  = {0};
static xmedia_u16 g_gc2053_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = {{0}};
static xmedia_u16 g_gc2053_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = {0};
static xmedia_u16 g_gc2053_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = {0};
static xmedia_u16 g_gc2053_init_ccm[XMEDIA_SENSOR_DEV_MAX_NUM][CCM_MATRIX_SIZE] = {{0}};
static xmedia_bool g_gc2053_abisirmode[XMEDIA_SENSOR_DEV_MAX_NUM] = {XMEDIA_FALSE};

SENSOR_PRIORITY_RODATA static const xmedia_sensor_property g_gc2053_property[gc2053_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_NONE, GC2053_MAX_FPS_LINEAR, GC2053_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_INDEX, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, GC2053_BIT_RATE
    },
};

static const xmedia_u32 regvaltable[GC2053_AGAIN_MAX_NUM][GC2053_AGAIN_INDEX] = {
    {0x00, 0x00, 0x01, 0x00, 0x82, 0x90}, //0xb4 0xb3 0xb8 0xb9 [0x38 0x7c] for FSWDR
    {0x00, 0x10, 0x01, 0x0c, 0x82, 0x90},
    {0x00, 0x20, 0x01, 0x1b, 0x82, 0x90},
    {0x00, 0x30, 0x01, 0x2c, 0x82, 0x90},
    {0x00, 0x40, 0x01, 0x3f, 0x82, 0x90},
    {0x00, 0x50, 0x02, 0x16, 0x88, 0x93},
    {0x00, 0x60, 0x02, 0x35, 0x88, 0x93},
    {0x00, 0x70, 0x03, 0x16, 0x88, 0x93},
    {0x00, 0x80, 0x04, 0x02, 0x88, 0x93},
    {0x00, 0x90, 0x04, 0x31, 0x88, 0x93},
    {0x00, 0xa0, 0x05, 0x32, 0x88, 0x93},
    {0x00, 0xb0, 0x06, 0x35, 0x88, 0x93},
    {0x00, 0xc0, 0x08, 0x04, 0x88, 0x93},
    {0x00, 0x5a, 0x09, 0x19, 0x88, 0x93},
    {0x00, 0x83, 0x0b, 0x0f, 0x88, 0x93},
    {0x00, 0x93, 0x0d, 0x12, 0x88, 0x93},
    {0x00, 0x84, 0x10, 0x00, 0x88, 0x93},
    {0x00, 0x94, 0x12, 0x3a, 0x88, 0x93},
    {0x01, 0x2c, 0x1a, 0x02, 0x88, 0x93},
    {0x01, 0x3c, 0x1b, 0x20, 0x88, 0x93},
    {0x00, 0x8c, 0x20, 0x0f, 0x88, 0x93},
    {0x00, 0x9c, 0x26, 0x07, 0x88, 0x93},
    {0x02, 0x64, 0x36, 0x21, 0x88, 0x93},
    {0x02, 0x74, 0x37, 0x3a, 0x88, 0x93},
    {0x00, 0xc6, 0x3d, 0x02, 0x88, 0x93},
    {0x00, 0xdc, 0x3f, 0x3f, 0x88, 0x93},
    {0x02, 0x85, 0x3f, 0x3f, 0x88, 0x93},
    {0x02, 0x95, 0x3f, 0x3f, 0x88, 0x93},
    {0x00, 0xce, 0x3f, 0x3f, 0x88, 0x93},
};

static const xmedia_u32 analog_gain_table[GC2053_AGAIN_MAX_NUM] = {
    1024, 1230, 1440, 1730, 2032, 2380, 2880, 3460, 4080, 4800, 5776,
    6760, 8064, 9500, 11552, 13600, 16132, 18912, 22528, 27036, 32340,
    38256, 45600, 53912, 63768, 76880, 92300, 108904, 123568,
};

SENSOR_PRIORITY_RODATA static const xmedia_sensor_capability g_gc2053_capability = {
    .max_width  = 1920,
    .max_height = 1080,
    .max_fps    = GC2053_MAX_FPS_LINEAR,
    .init_mclk  = XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
#ifdef GC2053_DVP
    .wdr_mode   = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format = XMEDIA_VIDEO_WDR_FMT_NONE,
    .bit_width  = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time = 200, // TODO: 待后续确认复位所需时间
    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_FALSE,
#else
    .wdr_mode   = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format = XMEDIA_VIDEO_WDR_FMT_NONE,
    .bit_width  = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time = 200, // TODO: 待后续确认复位所需时间
    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_2L,
#endif
    .standby_info = {
        .standby_supported = XMEDIA_FALSE, // TODO: 目前standby后resume不能恢复，寄存器配置更新后还未测试
        .addr_byte_num     = GC2053_ADDR_BYTE,
        .data_byte_num     = GC2053_DATA_BYTE,
        .standby_reg_num   = 4,
        .standby_reg_addr  = { 0x3e, 0xf7, 0xfc, 0xf9 },
        .standby_reg_data  = { 0x00, 0x00, 0x01, 0x01 },
        .resume_reg_num    = 4,
        .resume_reg_addr   = { 0x3e, 0xf7, 0xfc, 0xf9 },
        .resume_reg_data   = { 0x91, 0x01, 0x8e, 0x42 },
    },
};

static const xmedia_sensor_trig_attr g_gc2053_trig_attr = {
    .trig_mode       = 0,
    .hs_enable       = XMEDIA_FALSE,
    .vs_enable       = XMEDIA_TRUE,
    .hs_invert       = 0,
    .vs_invert       = 0,
    .hs_time         = 0,
    .vs_time         = (1.0 / 30.0) * 1000000,
    .vs_active       = 1 * ((1.0 / (GC2053_VMAX_1080P30_LINEAR * 30.0)) * 1000000), // the time of 1 line
    .delay_frame_num = 3,
};

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2053_ctx_init(xmedia_u32 dev)
{
    sensor_context *gc2053_ctx = XMEDIA_NULL;

    GC2053_GET_CTX(dev, gc2053_ctx);
    if (gc2053_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    gc2053_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (gc2053_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(gc2053_ctx, 0, sizeof(sensor_context));
    gc2053_ctx->i2c_addr             = GC2053_I2C_ADDR;
    gc2053_ctx->lanes                = XMEDIA_SENSOR_MIPI_LANES_2L;
    gc2053_ctx->work_mode            = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    gc2053_ctx->bus_info.i2c_dev     = SENSOR_I2C_INVALID;
    gc2053_ctx->size.width           = g_gc2053_property[0].width;
    gc2053_ctx->size.height          = g_gc2053_property[0].height;
    gc2053_ctx->fps                  = g_gc2053_property[0].max_fps;
    gc2053_ctx->wdr_mode             = g_gc2053_property[0].wdr_mode;
    gc2053_ctx->img_mode             = GC2053_2M_10BIT_LINEAR_MODE;
    gc2053_ctx->fl_std               = GC2053_VMAX_1080P30_LINEAR;
    gc2053_ctx->fl[SENSOR_CUR_FRAME] = GC2053_VMAX_1080P30_LINEAR;
    gc2053_ctx->fl[SENSOR_PRE_FRAME] = GC2053_VMAX_1080P30_LINEAR;

    GC2053_SET_CTX(dev, gc2053_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void gc2053_ctx_exit(xmedia_u32 dev)
{
    sensor_context *gc2053_ctx = XMEDIA_NULL;

    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_FREE(gc2053_ctx);
    GC2053_SET_CTX(dev, XMEDIA_NULL);

    return;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2053_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    for (i = 0; i < gc2053_SPECS_MAX_NUM; i++) {
        if (g_gc2053_property[i].width == gc2053_ctx->size.width &&
            g_gc2053_property[i].height == gc2053_ctx->size.height &&
            g_gc2053_property[i].wdr_mode == gc2053_ctx->wdr_mode) {
            memcpy(property, &g_gc2053_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Please check input param!\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 gc2053_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL && work_mode != XMEDIA_SENSOR_WORK_MODE_SLAVE ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc2053_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
#ifdef GC2053_DVP
    SENSOR_TRACE(MODULE_DBG_ERR, "in dvp mode,there is no need to configure mipi !\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#else
    sensor_context *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Invalid mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc2053_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
#endif
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2053_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        gc2053_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        gc2053_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        gc2053_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 gc2053_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;
    sensor_context *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    gc2053_ctx->i2c_addr = slave_addr;
    ret = gc2053_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    *slave_addr = gc2053_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

// called by ISP
SENSOR_PRIORITY_FUNC static xmedia_s32 gc2053_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32     ret;
    sensor_context *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    gc2053_ctx->init_mode = init_mode;
    ret = gc2053_i2c_init(dev, gc2053_ctx->bus_info.i2c_dev, gc2053_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    gc2053_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2053_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;
    sensor_context *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    ret = gc2053_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    gc2053_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32 ret;
    sensor_context *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx );

    gc2053_ctx ->mirror_en = mirror_en;
    ret = gc2053_set_mirror_flip(dev, mirror_en, gc2053_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    sensor_context *gc2053_ctx  = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx );

    gc2053_ctx ->flip_en = flip_en;
    ret = gc2053_set_mirror_flip(dev, gc2053_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2053_set_image_mode(sensor_context *gc2053_ctx, xmedia_u8 *image_mode,
    const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (gc2053_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (GC2053_RES_IS_1080P(sns_attr->width, sns_attr->height)) {
            *image_mode        = GC2053_2M_10BIT_LINEAR_MODE;
            gc2053_ctx->fl_std = GC2053_VMAX_1080P30_LINEAR;
        }else {
            GC2053_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        GC2053_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc2053_ctx->size.width  = sns_attr->width;
    gc2053_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2053_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context  *gc2053_ctx = XMEDIA_NULL;

    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            gc2053_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(gc2053_ctx->wdr_int_time, 0, sizeof(gc2053_ctx->wdr_int_time));
    return XMEDIA_SUCCESS;
}

#define GC2053_LINEAR_REG_INFO_MAX_NUM   GC2053_REG_L_MAX_NUM
#define GC2053_2TO1_WDR_REG_INFO_MAX_NUM GC2053_REG_MAX_NUM
SENSOR_PRIORITY_FUNC static xmedia_void gc2053_init_common_reg_info(sensor_context *gc2053_ctx)
{
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_EXP_L].delay_frame_num     = 2;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_EXP_L].reg_addr            = GC2053_REG_EXP_ADDR_L;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_EXP_H].delay_frame_num     = 2;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_EXP_H].reg_addr            = GC2053_REG_EXP_ADDR_H;

    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_AGC_L].delay_frame_num     = 2;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_AGC_L].reg_addr            = GC2053_REG_AGAIN_ADDR_L;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_AGC_H].delay_frame_num     = 2;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_AGC_H].reg_addr            = GC2053_REG_AGAIN_ADDR_H;

    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_DGC_L].delay_frame_num     = 2;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_DGC_L].reg_addr            = GC2053_REG_DGAIN_ADDR_L;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_DGC_H].delay_frame_num     = 2;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_DGC_H].reg_addr            = GC2053_REG_DGAIN_ADDR_H;

    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_PREGAIN_L].delay_frame_num = 2;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_PREGAIN_L].reg_addr   = GC2053_REG_AUTO_PREGAIN_ADDR_L;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_PREGAIN_H].delay_frame_num = 2;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_PREGAIN_H].reg_addr   = GC2053_REG_AUTO_PREGAIN_ADDR_H;

    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_STATUS].delay_frame_num    = 3;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_STATUS].reg_addr           = GC2053_REG_STATUS_ADDR;

    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_VMAX_L].delay_frame_num    = 2;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_VMAX_L].reg_addr           = GC2053_REG_VMAX_ADDR_L;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_VMAX_H].delay_frame_num    = 2;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_VMAX_H].reg_addr           = GC2053_REG_VMAX_ADDR_H;

    return;
}

SENSOR_PRIORITY_FUNC static xmedia_void gc2053_init_2to1_wdr_frame_reg_info(sensor_context *gc2053_ctx)
{
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_S_REG_EXP_L].delay_frame_num  = 1;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_S_REG_EXP_L].reg_addr         = GC2053_REG_EXP_ADDR_L;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_S_REG_EXP_H].delay_frame_num  = 1;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_S_REG_EXP_H].reg_addr         = GC2053_REG_EXP_ADDR_H;

    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_PRBS_MODE].delay_frame_num = 2;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_PRBS_MODE].reg_addr        = GC2053_REG_PRBS_MODE_ADDR;

    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_OPTICAL_BLACK].delay_frame_num = 2;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_OPTICAL_BLACK].reg_addr = GC2053_REG_OPTICAL_BLACK_ADDR;

    return;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2053_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *gc2053_ctx  = XMEDIA_NULL;

    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev               = gc2053_ctx->bus_info.i2c_dev;
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max        = 3; // maximum delay valid frames
    gc2053_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                       = GC2053_LINEAR_REG_INFO_MAX_NUM;

    if (gc2053_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_FRAME) {
        gc2053_ctx->regs_info[SENSOR_CUR_FRAME].reg_num = GC2053_2TO1_WDR_REG_INFO_MAX_NUM;
    }

    for (i = 0; i < gc2053_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = gc2053_ctx->i2c_addr;
        gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = GC2053_ADDR_BYTE;
        gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = GC2053_DATA_BYTE;
    }
    // init general register - The registers should init both in linear and 2to1 wdr mode
    gc2053_init_common_reg_info(gc2053_ctx);

    // init 2to1 wdr mode Regs
    if(gc2053_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_FRAME){
        gc2053_init_2to1_wdr_frame_reg_info(gc2053_ctx);
    }
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2053_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32       ret;
    sensor_context  *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

#ifdef GC2053_DVP
    if(sns_attr->wdr_mode != XMEDIA_VIDEO_WDR_MODE_NONE){
        SENSOR_TRACE(MODULE_DBG_ERR, "in dvp mode,not support this mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }
#endif
    // set wdr mode
    ret = gc2053_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // set image mode
    ret = gc2053_set_image_mode(gc2053_ctx, &gc2053_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    gc2053_ctx->fl[SENSOR_CUR_FRAME] = gc2053_ctx->fl_std;
    gc2053_ctx->fl[SENSOR_PRE_FRAME] = gc2053_ctx->fl[SENSOR_CUR_FRAME];

    ret = gc2053_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

//与ISP算法相关的参数交互
static xmedia_s32 gc2053_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef GC2053_ISP_DEFAULT_SUPPORT
    //TO-DO: alg add param
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from gc2053!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 gc2053_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context          *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;
    black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R ]  = 256;
    black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR]  = 256;
    black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB]  = 256;
    black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B ]  = 256;

    if (gc2053_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_FRAME) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R ]  = 256;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR]  = 256;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB]  = 256;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B ]  = 256;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    for (i = 0; i < gc2053_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            gc2053_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    // 单独配置 vmax 高位寄存器不会生效，需要在配置低位寄存器后才会生效
    if (gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_VMAX_H].update == XMEDIA_TRUE) {
        gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC2053_REG_VMAX_L].update = XMEDIA_TRUE;
    }

    memcpy(sns_regs_info, &gc2053_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&gc2053_ctx->regs_info[SENSOR_PRE_FRAME], &gc2053_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    gc2053_ctx->fl[SENSOR_PRE_FRAME] = gc2053_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                       i;
    xmedia_sensor_mirror_flip_type  type;
    sensor_context                 *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    if (gc2053_ctx->mirror_en && gc2053_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (gc2053_ctx->mirror_en && (!gc2053_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!gc2053_ctx->mirror_en) && gc2053_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < gc2053_SPECS_MAX_NUM; i++) {
        if (g_gc2053_property[i].width == gc2053_ctx->size.width &&
            g_gc2053_property[i].height == gc2053_ctx->size.height &&
            g_gc2053_property[i].wdr_mode == gc2053_ctx->wdr_mode) {
            *bayer_pattern = g_gc2053_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= gc2053_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2053_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);
    memcpy(capability, &g_gc2053_capability, sizeof(xmedia_sensor_capability));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_get_trig_attr(xmedia_sensor_trig_attr *trig_attr)
{
    SENSOR_CHECK_PTR_RETURN(trig_attr);
    memcpy(trig_attr, &g_gc2053_trig_attr, sizeof(xmedia_sensor_trig_attr));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context          *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc2053_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_gc2053_exposure[dev]                       = init_param->exposure;
    g_gc2053_sample_r_gain[dev]                  = init_param->sample_rgain;
    g_gc2053_sample_b_gain[dev]                  = init_param->sample_bgain;
    g_gc2053_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_gc2053_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_gc2053_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case GC2053_2M_10BIT_LINEAR_MODE:
            *max_fps = GC2053_MAX_FPS_LINEAR;
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                      xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = gc2053_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines_std          = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq            = 50 * 256;
    ae_sns_dft->full_lines_max          = GC2053_FULL_LINES_MAX;
    ae_sns_dft->ae_run_interval         = 1;
    ae_sns_dft->init_exposure           = 10;

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->again_accu.accu_type    = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy     = 1;
    ae_sns_dft->dgain_accu.accu_type    = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy     = 0.015625;

    ae_sns_dft->ispdgain_shift          = 8;
    ae_sns_dft->min_ispdgain_target     = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target     = 2 << ae_sns_dft->ispdgain_shift;

    ae_sns_dft->max_iris_f_no           = XMEDIA_ISP_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_f_no           = XMEDIA_ISP_IRIS_F_NO_32_0;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_TRUE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    if (g_gc2053_linesper500ms[dev] == 0) {
        ae_sns_dft->lines_per_500ms = sns_ctx->fl_std * 30 / 2;
    } else {
        ae_sns_dft->lines_per_500ms = g_gc2053_linesper500ms[dev];
    }

    return XMEDIA_SUCCESS;
}

static xmedia_void gc2053_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                       xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->max_again        = 113168;
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 512;
    ae_sns_dft->min_dgain        = 64;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x38;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure       = g_gc2053_exposure[dev] ? g_gc2053_exposure[dev] : 148859;
    ae_sns_dft->max_int_time        = sns_ctx->fl_std - 2;
    ae_sns_dft->min_int_time        = 1;
    ae_sns_dft->max_int_time_target = 65535; // max 65535
    ae_sns_dft->min_int_time_target = 1;

    return;
}

static xmedia_s32 gc2053_get_ae_2to1_wdr_frame_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                             xmedia_isp_ae_sensor_default *ae_sns_dft)
{
     sensor_context *gc2053_ctx = XMEDIA_NULL;

     GC2053_GET_CTX(dev, gc2053_ctx);
     SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

     ae_sns_dft->ae_compensation = 0x18;
     ae_sns_dft->ae_exp_mode = XMEDIA_ISP_AE_STRATEGY_EXP_LOWLIGHT_PRIOR;
     ae_sns_dft->int_time_accu.offset    = -0.15417;

     ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
     ae_sns_dft->int_time_accu.accuracy  = 4;
     ae_sns_dft->int_time_accu.offset    = 0;

     ae_sns_dft->max_again        = 113168;
     ae_sns_dft->min_again        = 1024;
     ae_sns_dft->max_again_target = ae_sns_dft->max_again;
     ae_sns_dft->min_again_target = ae_sns_dft->min_again;

     ae_sns_dft->max_dgain        = 64;
     ae_sns_dft->min_dgain        = 64;
     ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
     ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

     ae_sns_dft->max_int_time        = sns_ctx->fl_std - 2;
     ae_sns_dft->min_int_time        = 1;
     ae_sns_dft->max_int_time_target = 65535;
     ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

     ae_sns_dft->ispdgain_shift      = 8;
     ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
     ae_sns_dft->max_ispdgain_target = 4 << ae_sns_dft->ispdgain_shift;
     ae_sns_dft->man_ratio_enable    = XMEDIA_TRUE;
     ae_sns_dft->ratio[0] = 0x400;
     return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32              ret;
    sensor_context          *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = gc2053_get_ae_common_default(dev, gc2053_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (gc2053_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            gc2053_get_ae_linear_default(dev, gc2053_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_FRAME:
            gc2053_get_ae_2to1_wdr_frame_default(dev, gc2053_ctx, ae_sns_dft);
            break;
    }
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{   // TO-DO：min fps is to be confirmed.
    switch (img_mode) {

        case GC2053_2M_10BIT_LINEAR_MODE:
            *min_fps = 2.06;
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {

        case GC2053_2M_10BIT_LINEAR_MODE:
            *vmax = GC2053_VMAX_1080P30_LINEAR;
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32 ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32 vmax;

    ret = gc2053_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc2053_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc2053_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is [%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    xmedia_u32      full_lines = GC2053_VMAX_1080P30_LINEAR;
    sensor_context* gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    ret = gc2053_calc_fps(fps, gc2053_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines = (full_lines > GC2053_FULL_LINES_MAX) ? GC2053_FULL_LINES_MAX : full_lines;

    gc2053_ctx->fps    = fps;
    gc2053_ctx->fl_std = full_lines;
    gc2053_ctx->regs_info[0].i2c_data[GC2053_REG_STATUS].data = 0x0;
    gc2053_ctx->regs_info[0].i2c_data[GC2053_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);
    gc2053_ctx->regs_info[0].i2c_data[GC2053_REG_VMAX_H].data = (full_lines & 0x3F00) >> 8;

    ae_sns_dft->fps                  = fps;
    ae_sns_dft->full_lines_std       = gc2053_ctx->fl_std;
    ae_sns_dft->max_int_time         = gc2053_ctx->fl_std - 2;
    gc2053_ctx->fl[SENSOR_CUR_FRAME] = gc2053_ctx->fl_std;
    ae_sns_dft->full_lines           = gc2053_ctx->fl[SENSOR_CUR_FRAME];

    SENSOR_PRINT("dev[%d] - gc2053 set fps = %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = gc2053_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc2053_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc2053_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = (vmax_max_fps * max_fps / min_fps);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                             xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context* gc2053_ctx = XMEDIA_NULL;
    xmedia_u32   vmax_min_fps;
    xmedia_s32   ret;
    xmedia_float min_fps;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    if (gc2053_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", gc2053_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = gc2053_get_min_fps_vmax(gc2053_ctx->img_mode, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = gc2053_get_min_fps(gc2053_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines = (full_lines > GC2053_FULL_LINES_MAX) ? GC2053_FULL_LINES_MAX : full_lines;
    gc2053_ctx->fl[0] = full_lines;

    gc2053_ctx->regs_info[0].i2c_data[GC2053_REG_STATUS].data = 0x0;
    gc2053_ctx->regs_info[0].i2c_data[GC2053_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);
    gc2053_ctx->regs_info[0].i2c_data[GC2053_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);

    ae_sns_dft->full_lines   = gc2053_ctx->fl[0];
    ae_sns_dft->max_int_time = gc2053_ctx->fl[0] - 2;

    gc2053_ctx->fps = min_fps * vmax_min_fps / full_lines;

    SENSOR_PRINT("dev[%d]-gc2053 set fps %f\n", dev, gc2053_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context       *gc2053_ctx                       = XMEDIA_NULL;
    static xmedia_bool first[XMEDIA_SENSOR_DEV_MAX_NUM]    = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 1 };
    xmedia_u32 exposure;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    exposure = (int_time > GC2053_FULL_LINES_MAX) ? GC2053_FULL_LINES_MAX : int_time;

    if (gc2053_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_FRAME) {
        if (first[dev]) { // short exposure
            gc2053_ctx->regs_info[0].i2c_data[GC2053_REG_EXP_L].data = SENSOR_LOW_8BITS(exposure);
            gc2053_ctx->regs_info[0].i2c_data[GC2053_REG_EXP_H].data = SENSOR_HIGH_8BITS(exposure);

            first[dev] = XMEDIA_FALSE;

        } else { // long exposure
            gc2053_ctx->regs_info[0].i2c_data[GC2053_S_REG_EXP_L].data = SENSOR_LOW_8BITS(exposure);
            gc2053_ctx->regs_info[0].i2c_data[GC2053_S_REG_EXP_H].data = SENSOR_HIGH_8BITS(exposure);
            first[dev] = XMEDIA_TRUE;
        }
    } else {
        gc2053_ctx->regs_info[0].i2c_data[GC2053_REG_EXP_L].data = SENSOR_LOW_8BITS(exposure);
        gc2053_ctx->regs_info[0].i2c_data[GC2053_REG_EXP_H].data = SENSOR_HIGH_8BITS(exposure);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_u32 again;
    xmedia_u32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    again = *again_lin;

    if (again >= analog_gain_table[GC2053_AGAIN_MAX_NUM - 1]) {
        *again_lin = analog_gain_table[GC2053_AGAIN_MAX_NUM -1];
        *again_db = GC2053_AGAIN_MAX_NUM -1;
    } else {
        for (i = 1; i < GC2053_AGAIN_MAX_NUM; i++) {
            if (again < analog_gain_table[i]) {
                *again_lin = analog_gain_table[i - 1];
                *again_db = i - 1;
                break;
            }
        }
    }
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(dgain_lin);
    SENSOR_CHECK_PTR_RETURN(dgain_db);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    xmedia_u8 dgain_high;
    xmedia_u8 dgain_low;
    sensor_context *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    dgain_high = (dgain >> 6) & 0x0f;
    dgain_low  = (dgain & 0x3f) << 2;

    gc2053_ctx->regs_info[0].i2c_data[GC2053_REG_AGC_L].data     = regvaltable[again][1];
    gc2053_ctx->regs_info[0].i2c_data[GC2053_REG_AGC_H].data     = regvaltable[again][0];
    gc2053_ctx->regs_info[0].i2c_data[GC2053_REG_DGC_L].data     = regvaltable[again][3];
    gc2053_ctx->regs_info[0].i2c_data[GC2053_REG_DGC_H].data     = regvaltable[again][2];
    gc2053_ctx->regs_info[0].i2c_data[GC2053_REG_PREGAIN_L].data = dgain_low;
    gc2053_ctx->regs_info[0].i2c_data[GC2053_REG_PREGAIN_H].data = dgain_high;

    if(gc2053_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_FRAME){
        gc2053_ctx->regs_info[0].i2c_data[GC2053_REG_PRBS_MODE].data  = regvaltable[again][4];
        gc2053_ctx->regs_info[0].i2c_data[GC2053_REG_OPTICAL_BLACK].data  = regvaltable[again][5];
    }
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc2053_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    xmedia_u32 timemaxtmp     = 0;
    sensor_context *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    memcpy(gc2053_ctx->ratio, inttime_attr->ratio, sizeof(inttime_attr->ratio));

    if (gc2053_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_FRAME) {
        timemaxtmp = ((gc2053_ctx->fl[0] - 2) << 6) / SENSOR_DIV_0_TO_1(inttime_attr->ratio[0]);
        inttime_attr->max_inttime[0] = timemaxtmp;
        inttime_attr->max_inttime[1] = (gc2053_ctx->fl[0] - 2);
        inttime_attr->min_inttime[0] = 2;
        inttime_attr->min_inttime[1] = inttime_attr->max_inttime[0] * inttime_attr->ratio[0] >> 6;
    }

    return XMEDIA_SUCCESS;
}

// AWB
// Rgain and Bgain of the golden sample
#define GOLDEN_RGAIN                          0
#define GOLDEN_BGAIN                          0

static xmedia_s32 gc2053_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context          *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = 5082;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = 446;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = 256;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = 256;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = 436;

    // 0 ~ 6: point index on the awb curve param
    awb_sns_dft->wb_para[0]   = 60;
    awb_sns_dft->wb_para[1]   = 96;
    awb_sns_dft->wb_para[2]   = -100;
    awb_sns_dft->wb_para[3]   = 206542;
    awb_sns_dft->wb_para[4]   = 128;
    awb_sns_dft->wb_para[5]   = -154259;
    awb_sns_dft->golden_rgain = GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = GOLDEN_BGAIN;

    switch (gc2053_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_gc2053_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            if (g_gc2053_abisirmode[dev] == XMEDIA_TRUE) {
                memcpy(&awb_sns_dft->agc_tbl, &g_gc2053_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            } else {
                memcpy(&awb_sns_dft->agc_tbl, &g_gc2053_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            }
            break;
    }
    awb_sns_dft->init_rgain   = g_gc2053_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_gc2053_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_gc2053_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_gc2053_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_gc2053_sample_b_gain[dev];
    awb_sns_dft->awb_runinterval = 4;

    for(xmedia_s32 i = 0;i < XMEDIA_ISP_CCM_MATRIX_SIZE;i++){
        awb_sns_dft->init_ccm[i] = g_gc2053_init_ccm[dev][i];
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 gc2053_standby(xmedia_u32 dev)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    gc2053_write_reg(dev, 0x3e, 0x00);
    gc2053_write_reg(dev, 0xf7, 0x00);
    gc2053_write_reg(dev, 0xfc, 0x01);
    gc2053_write_reg(dev, 0xf9, 0x01);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2053_resume(xmedia_u32 dev)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    gc2053_write_reg(dev, 0xf9, 0x42);
    gc2053_write_reg(dev, 0xf7, 0x01);
    gc2053_write_reg(dev, 0xfc, 0x8e);
    gc2053_write_reg(dev, 0x3e, 0x91);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2053_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context* gc2053_ctx   = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < gc2053_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < gc2053_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= gc2053_write_reg(dev, gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                gc2053_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_void gc2053_delay_ms(xmedia_s32 ms)
{
    sleep_us(ms * 1000);
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2053_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    if (gc2053_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = gc2053_init_image(dev, gc2053_ctx->img_mode, gc2053_ctx->work_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    gc2053_delay_ms(10);
    ret = gc2053_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = gc2053_set_mirror_flip(dev, gc2053_ctx->mirror_en, gc2053_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

xmedia_s32 gc2053_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = gc2053_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2053_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_gc2053_dev_map[index] == SENSOR_DEV_INVALID) {
            g_gc2053_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }
    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2053_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_gc2053_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }
    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 gc2053_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    ret = gc2053_get_min_fps(gc2053_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc2053_get_max_fps(gc2053_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = gc2053_ctx->fps;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 gc2053_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);

    memcpy(info->sensor_name, gc2053_NAME, sizeof(gc2053_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property      = gc2053_get_property;
    info->isp_func.pfn_sensor_set_work_mode     = gc2053_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes    = gc2053_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror            = gc2053_mirror;
    info->isp_func.pfn_sensor_flip              = gc2053_flip;
    info->isp_func.pfn_sensor_set_bus_info      = gc2053_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr      = gc2053_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param    = gc2053_set_init_param;
    info->isp_func.pfn_sensor_start             = gc2053_start;
    info->isp_func.pfn_sensor_stop              = gc2053_stop;
    info->isp_func.pfn_sensor_standby           = gc2053_standby;
    info->isp_func.pfn_sensor_resume            = gc2053_resume;
    info->isp_func.pfn_sensor_write_reg         = gc2053_write_reg;
    info->isp_func.pfn_sensor_read_reg          = gc2053_read_reg;

    // called by ISP
    info->isp_func.pfn_sensor_init                = gc2053_init;
    info->isp_func.pfn_sensor_exit                = gc2053_exit;
    info->isp_func.pfn_sensor_set_attr            = gc2053_set_attr;
    info->isp_func.pfn_sensor_get_isp_default     = gc2053_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = gc2053_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = gc2053_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = gc2053_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = gc2053_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = gc2053_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = gc2053_get_fps;
    info->isp_func.pfn_sensor_get_trig_attr       = gc2053_get_trig_attr;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = gc2053_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = gc2053_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = gc2053_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = gc2053_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = gc2053_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = gc2053_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = gc2053_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = gc2053_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = gc2053_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = gc2053_get_awb_default;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2053_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = gc2053_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = gc2053_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret  = gc2053_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc2053 register failed! error code = %d.\n", ret);
        return ret;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 gc2053_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *gc2053_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = gc2053_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    GC2053_GET_CTX(dev, gc2053_ctx);
    SENSOR_CHECK_PTR_RETURN(gc2053_ctx);

    if (gc2053_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "GC2053 unregister function failed!\n");
        return ret;
    }
    gc2053_ctx_exit(dev);
    g_gc2053_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;
}
