#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "os05a10.h"
#include "os05a10_ctrl.h"
#include "os05a10_ex.h"
#include "xmedia_isp.h"

#ifdef FPGA
#define OS05A10_5M_INPUT_CLOCK_LINEAR   9000000
#define OS05A10_4M_INPUT_CLOCK_LINEAR   9000000
#define OS05A10_5M_MAX_FPS_LINEAR       6.357 // 17fps * ( 9M / 24M )
#define OS05A10_4M_MAX_FPS_LINEAR       6.357 // 17fps * ( 9M / 24M )
#define OS05A10_5M_DATA_WIDTH_LINEAR    XMEDIA_VIDEO_DATA_WIDTH_10
#define OS05A10_4M_DATA_WIDTH_LINEAR    XMEDIA_VIDEO_DATA_WIDTH_10
#define OS05A10_5M_INPUT_CLOCK_WDR      0     // not support
#define OS05A10_4M_INPUT_CLOCK_WDR      0     // not support
#define OS05A10_5M_MAX_FPS_WDR          0.0   // not support
#define OS05A10_4M_MAX_FPS_WDR          0.0   // not support
#define OS05A10_5M_DATA_WIDTH_WDR       XMEDIA_VIDEO_DATA_WIDTH_MAX
#define OS05A10_4M_DATA_WIDTH_WDR       XMEDIA_VIDEO_DATA_WIDTH_MAX
#define OS05A10_5M_BIT_RATE_LINEAR      600
#define OS05A10_4M_BIT_RATE_LINEAR      600
#define OS05A10_5M_BIT_RATE_WDR         600
#define OS05A10_4M_BIT_RATE_WDR         600
#else
#define OS05A10_5M_INPUT_CLOCK_LINEAR   XMEDIA_SENSOR_CLOCK_FREQ_24MHZ
#define OS05A10_4M_INPUT_CLOCK_LINEAR   XMEDIA_SENSOR_CLOCK_FREQ_24MHZ
#define OS05A10_5M_MAX_FPS_LINEAR       30.0
#define OS05A10_4M_MAX_FPS_LINEAR       30.0
#define OS05A10_5M_DATA_WIDTH_LINEAR    XMEDIA_VIDEO_DATA_WIDTH_12
#define OS05A10_4M_DATA_WIDTH_LINEAR    XMEDIA_VIDEO_DATA_WIDTH_12
#define OS05A10_5M_INPUT_CLOCK_WDR      XMEDIA_SENSOR_CLOCK_FREQ_24MHZ
#define OS05A10_4M_INPUT_CLOCK_WDR      XMEDIA_SENSOR_CLOCK_FREQ_24MHZ
#define OS05A10_5M_MAX_FPS_WDR          30.0
#define OS05A10_4M_MAX_FPS_WDR          30.0
#define OS05A10_5M_DATA_WIDTH_WDR       XMEDIA_VIDEO_DATA_WIDTH_10
#define OS05A10_4M_DATA_WIDTH_WDR       XMEDIA_VIDEO_DATA_WIDTH_10
#define OS05A10_5M_BIT_RATE_LINEAR      600
#define OS05A10_4M_BIT_RATE_LINEAR      600
#define OS05A10_5M_BIT_RATE_WDR         1000
#define OS05A10_4M_BIT_RATE_WDR         1000
#endif

#define OS05A10_NAME          "OS05A10"
#define OS05A10_SPECS_MAX_NUM 4

#ifdef __linux__
#define OS05A10_ISP_DEFAULT_SUPPORT
#endif

#define OS05A10_REG_ADDR_STANDBY 0x0100 // BIT[0]

#define OS05A10_RES_IS_1944P(w, h) ((w) == 2688 && (h) == 1944)
#define OS05A10_RES_IS_1520P(w, h) ((w) == 2688 && (h) == 1520)

#define OS05A10_ERR_MODE_PRINT(sensor_image_mode)                                                                      \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sensor_image_mode->width,       \
                     sensor_image_mode->height, sensor_image_mode->wdr_mode);                                          \
    } while (0)

static sensor_context *g_os05a10_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define OS05A10_GET_CTX(dev, ctx) ctx = g_os05a10_ctx[dev]
#define OS05A10_SET_CTX(dev, ctx) g_os05a10_ctx[dev] = ctx

static xmedia_s32 g_os05a10_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] =
                                                                           SENSOR_DEV_INVALID };

static xmedia_u32 g_os05a10_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_os05a10_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_os05a10_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_os05a10_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

static const xmedia_sensor_property g_os05a10_property[OS05A10_SPECS_MAX_NUM] = {
     {
        // width, height, wdr_mode
        2688, 1944, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS05A10_5M_MAX_FPS_LINEAR, OS05A10_5M_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, OS05A10_5M_DATA_WIDTH_LINEAR,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OS05A10_5M_BIT_RATE_LINEAR,
     },

     {
        // width, height, wdr_mode
        2688, 1520, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS05A10_4M_MAX_FPS_LINEAR, OS05A10_4M_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, OS05A10_4M_DATA_WIDTH_LINEAR,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OS05A10_4M_BIT_RATE_LINEAR,
     },

     {
        // width, height, wdr_mode
        2688, 1944, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS05A10_5M_MAX_FPS_WDR, OS05A10_5M_INPUT_CLOCK_WDR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, OS05A10_5M_DATA_WIDTH_WDR,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, OS05A10_5M_BIT_RATE_WDR,
     },

     {
        // width, height, wdr_mode
        2688, 1520, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS05A10_4M_MAX_FPS_WDR, OS05A10_4M_INPUT_CLOCK_WDR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, OS05A10_4M_DATA_WIDTH_WDR,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, OS05A10_4M_BIT_RATE_WDR,
     },
};

static const xmedia_sensor_capability g_os05a10_capability = {
    .max_width  = 2688,
    .max_height = 1944,
    .max_fps    = OS05A10_5M_DATA_WIDTH_WDR,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = OS05A10_5M_DATA_WIDTH_LINEAR,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_TRUE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_4L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = OS05A10_ADDR_BYTE,
        .data_byte_num     = OS05A10_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { OS05A10_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { OS05A10_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x01 },
    },
};

static xmedia_sensor_trig_attr g_os05a10_trig_attr = {
    .trig_mode       = 0,
    .hs_enable       = XMEDIA_FALSE,
    .vs_enable       = XMEDIA_TRUE,
    .hs_invert       = 0,
    .vs_invert       = 0,
    .hs_time         = 0,
    .vs_time         = (1.0 / 30.0) * 1000000,
    .vs_active       = 1 * ((1.0 / (OS05A10_VMAX_5M_LINEAR * 30.0)) * 1000000), // the time of 1 line
    .delay_frame_num = 2,
};

static xmedia_s32 os05a10_ctx_init(xmedia_u32 dev)
{
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    OS05A10_GET_CTX(dev, os05a10_ctx);
    if (os05a10_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " dev-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    os05a10_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (os05a10_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " dev-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(os05a10_ctx, 0, sizeof(sensor_context));
    os05a10_ctx->i2c_addr             = OS05A10_I2C_ADDR;
    os05a10_ctx->lanes                = XMEDIA_SENSOR_MIPI_LANES_4L;
    os05a10_ctx->work_mode            = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    os05a10_ctx->bus_info.i2c_dev     = SENSOR_I2C_INVALID;
    os05a10_ctx->size.width           = g_os05a10_property[0].width;
    os05a10_ctx->size.height          = g_os05a10_property[0].height;
    os05a10_ctx->fps                  = g_os05a10_property[0].max_fps;
    os05a10_ctx->wdr_mode             = g_os05a10_property[0].wdr_mode;
    os05a10_ctx->img_mode             = OS05A10_5M_LINEAR_MODE;
    os05a10_ctx->fl_std               = OS05A10_VMAX_5M_LINEAR;
    os05a10_ctx->fl[SENSOR_CUR_FRAME] = OS05A10_VMAX_5M_LINEAR;
    os05a10_ctx->fl[SENSOR_PRE_FRAME] = OS05A10_VMAX_5M_LINEAR;
    OS05A10_SET_CTX(dev, os05a10_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_void os05a10_ctx_exit(xmedia_u32 dev)
{
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_FREE(os05a10_ctx);
    OS05A10_SET_CTX(dev, XMEDIA_NULL);

    return;
}

static xmedia_s32 os05a10_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    for (i = 0; i < OS05A10_SPECS_MAX_NUM; i++) {
        if (g_os05a10_property[i].width == os05a10_ctx->size.width &&
            g_os05a10_property[i].height == os05a10_ctx->size.height &&
            g_os05a10_property[i].wdr_mode == os05a10_ctx->wdr_mode) {
            memcpy(property, &g_os05a10_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Please check input param!\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 os05a10_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    if (work_mode < XMEDIA_SENSOR_WORK_MODE_NORMAL || work_mode >= XMEDIA_SENSOR_WORK_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode[%d] param invalid!\n", work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os05a10_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Invalid mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os05a10_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        os05a10_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        os05a10_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        os05a10_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 os05a10_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    os05a10_ctx->i2c_addr = slave_addr;
    ret = os05a10_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    *slave_addr = os05a10_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

// called by ISP
static xmedia_s32 os05a10_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    os05a10_ctx->init_mode = init_mode;
    ret = os05a10_i2c_init(dev, os05a10_ctx->bus_info.i2c_dev, os05a10_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    os05a10_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    ret = os05a10_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    os05a10_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    os05a10_ctx->mirror_en = mirror_en;
    ret = os05a10_set_mirror_flip(dev, mirror_en, os05a10_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    os05a10_ctx->flip_en = flip_en;
    ret = os05a10_set_mirror_flip(dev, os05a10_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_set_image_mode(sensor_context *os05a10_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (os05a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (OS05A10_RES_IS_1944P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OS05A10_5M_LINEAR_MODE;
            os05a10_ctx->fl_std = OS05A10_VMAX_5M_LINEAR;
        } else if (OS05A10_RES_IS_1520P(sns_attr->width, sns_attr->height)){
            *image_mode         = OS05A10_4M_LINEAR_MODE;
            os05a10_ctx->fl_std = OS05A10_VMAX_4M_LINEAR;
        } else {
            OS05A10_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else if (os05a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (OS05A10_RES_IS_1944P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OS05A10_5M_WDR_MODE;
            os05a10_ctx->fl_std = OS05A10_VMAX_5M_WDR;
        } else if (OS05A10_RES_IS_1520P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OS05A10_4M_WDR_MODE;
            os05a10_ctx->fl_std = OS05A10_VMAX_4M_WDR;
        } else {
            OS05A10_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        OS05A10_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os05a10_ctx->size.width  = sns_attr->width;
    os05a10_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            os05a10_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            os05a10_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
            SENSOR_PRINT("2to1 line WDR\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support this mode - %d!\n", wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(os05a10_ctx->wdr_int_time, 0, sizeof(os05a10_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

static xmedia_void os05a10_init_common_reg_info(sensor_context *os05a10_ctx)
{
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_EXP_H].delay_frame_num = 2;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_EXP_H].reg_addr        = OS05A10_REG_ADDR_EXP_H;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_EXP_L].delay_frame_num = 2;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_EXP_L].reg_addr        = OS05A10_REG_ADDR_EXP_L;

    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_AGC_H].delay_frame_num = 2;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_AGC_H].reg_addr        = OS05A10_REG_ADDR_AGC_H;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_AGC_L].delay_frame_num = 2;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_AGC_L].reg_addr        = OS05A10_REG_ADDR_AGC_L;

    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_DGC_H].delay_frame_num = 2;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_DGC_H].reg_addr        = OS05A10_REG_ADDR_DGC_H;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_DGC_L].delay_frame_num = 2;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_DGC_L].reg_addr        = OS05A10_REG_ADDR_DGC_L;

    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_VMAX_H].delay_frame_num = 2;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_VMAX_H].reg_addr        = OS05A10_REG_ADDR_VMAX_H;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_VMAX_L].delay_frame_num = 2;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_VMAX_L].reg_addr        = OS05A10_REG_ADDR_VMAX_L;

    return;
}

static xmedia_void os05a10_init_2to1_wdr_reg_info(sensor_context *os05a10_ctx)
{
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_S_EXP_H].delay_frame_num = 2;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_S_EXP_H].reg_addr        = OS05A10_REG_ADDR_S_EXP_H;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_S_EXP_L].delay_frame_num = 2;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_S_EXP_L].reg_addr        = OS05A10_REG_ADDR_S_EXP_L;

    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_S_AGC_H].delay_frame_num = 2;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_S_AGC_H].reg_addr        = OS05A10_REG_ADDR_S_AGC_H;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_S_AGC_L].delay_frame_num = 2;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_S_AGC_L].reg_addr        = OS05A10_REG_ADDR_S_AGC_L;

    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_S_DGC_H].delay_frame_num = 2;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_S_DGC_H].reg_addr        = OS05A10_REG_ADDR_S_DGC_H;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_S_DGC_L].delay_frame_num = 2;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_S_DGC_L].reg_addr        = OS05A10_REG_ADDR_S_DGC_L;

    return;
}

#define OS05A10_LINEAR_REG_INFO_MAX_NUM   OS05A10_REG_L_MAX_NUM
#define OS05A10_2TO1_WDR_REG_INFO_MAX_NUM OS05A10_REG_MAX_NUM
xmedia_s32 os05a10_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = os05a10_ctx->bus_info.i2c_dev;
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = OS05A10_REG_L_MAX_NUM;

    if (os05a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        os05a10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num = OS05A10_2TO1_WDR_REG_INFO_MAX_NUM;
    }

    for (i = 0; i < os05a10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = os05a10_ctx->i2c_addr;
        os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = OS05A10_ADDR_BYTE;
        os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = OS05A10_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    os05a10_init_common_reg_info(os05a10_ctx);

    // init 2to1 wdr mode Regs
    if (os05a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        os05a10_init_2to1_wdr_reg_info(os05a10_ctx);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    // Set wdr mode
    ret = os05a10_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = os05a10_set_image_mode(os05a10_ctx, &os05a10_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    os05a10_ctx->fl[SENSOR_CUR_FRAME] = os05a10_ctx->fl_std;
    os05a10_ctx->fl[SENSOR_PRE_FRAME] = os05a10_ctx->fl[SENSOR_CUR_FRAME];

    ret = os05a10_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef OS05A10_ISP_DEFAULT_SUPPORT
    // TO-DO: alg add param
    isp_default->bnr         = &g_os05a10_bnr;
    isp_default->clut_attr   = XMEDIA_NULL;
    isp_default->crosstalk   = &g_os05a10_crosstalk;
    isp_default->csc         = &g_os05a10_csc;
    isp_default->dehaze      = &g_os05a10_dehaze;
    isp_default->demosaic    = &g_os05a10_demosaic;
    isp_default->dpc_dynamic = &g_os05a10_dpc;
    isp_default->dpc_static  = XMEDIA_NULL;
    isp_default->expander    = XMEDIA_NULL;
    isp_default->fpn         = XMEDIA_NULL;
    isp_default->gamma       = &g_os05a10_gamma;
    isp_default->gcac        = &g_os05a10_gcac;
    isp_default->hlc         = XMEDIA_NULL;
    isp_default->lce         = &g_os05a10_lce;
    isp_default->mlsc_attr   = XMEDIA_NULL;
    isp_default->mlsc_lut    = &g_os05a10_shading_lut;
    isp_default->radial_crop = XMEDIA_NULL;
    isp_default->rgbir       = XMEDIA_NULL;
    isp_default->rlsc_attr   = XMEDIA_NULL;
    isp_default->rlsc_lut    = XMEDIA_NULL;
    isp_default->sharpen     = XMEDIA_NULL;
    isp_default->stnr        = XMEDIA_NULL;
    isp_default->wdr         = XMEDIA_NULL;

    switch(os05a10_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            isp_default->blc = &g_os05a10_blc;
            isp_default->drc = XMEDIA_NULL;
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            isp_default->blc = &g_os05a10_blc_wdr;
            isp_default->drc = &g_os05a10_drc;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support mode: %d\n", os05a10_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
#else
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from os05a10!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 os05a10_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (os05a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x40;//0x104,
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x40;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x40;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x40;
    } else {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x104;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    for (i = 0; i < os05a10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            os05a10_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &os05a10_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&os05a10_ctx->regs_info[SENSOR_PRE_FRAME], &os05a10_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    os05a10_ctx->fl[SENSOR_PRE_FRAME] = os05a10_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    if (os05a10_ctx->mirror_en && os05a10_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (os05a10_ctx->mirror_en && (!os05a10_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!os05a10_ctx->mirror_en) && os05a10_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < OS05A10_SPECS_MAX_NUM; i++) {
        if (g_os05a10_property[i].width == os05a10_ctx->size.width &&
            g_os05a10_property[i].height == os05a10_ctx->size.height &&
            g_os05a10_property[i].wdr_mode == os05a10_ctx->wdr_mode) {
            *bayer_pattern = g_os05a10_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= OS05A10_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);
    memcpy(capability, &g_os05a10_capability, sizeof(xmedia_sensor_capability));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_get_trig_attr(xmedia_sensor_trig_attr *trig_attr)
{
    SENSOR_CHECK_PTR_RETURN(trig_attr);
    memcpy(trig_attr, &g_os05a10_trig_attr, sizeof(xmedia_sensor_trig_attr));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os05a10_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 os05a10_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_os05a10_exposure[dev]      = init_param->exposure;
    g_os05a10_sample_r_gain[dev] = init_param->sample_rgain;
    g_os05a10_sample_b_gain[dev] = init_param->sample_bgain;

    g_os05a10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_os05a10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_os05a10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

xmedia_s32 os05a10_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case OS05A10_5M_LINEAR_MODE:
            *max_fps = OS05A10_5M_MAX_FPS_LINEAR;
            break;

        case OS05A10_4M_LINEAR_MODE:
            *max_fps = OS05A10_4M_MAX_FPS_LINEAR;
            break;

        case OS05A10_5M_WDR_MODE:
            *max_fps = OS05A10_5M_MAX_FPS_WDR;
            break;

        case OS05A10_4M_WDR_MODE:
            *max_fps = OS05A10_4M_MAX_FPS_WDR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = os05a10_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   =  50 * 256; // light flicker freq: 50Hz, accuracy: 256
    ae_sns_dft->full_lines_max = OS05A10_FULL_LINES_MAX;

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1.0 / 16;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy  =  0.0009765625; // accuracy: 0.0009765625 - 1/1024

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 32 << ae_sns_dft->ispdgain_shift;

    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_32_0;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    // lines per second: VTS * max_fps
    ae_sns_dft->lines_per_500ms  = (sns_ctx->fl_std * max_fps) / 2;
    ae_sns_dft->man_ratio_enable = XMEDIA_FALSE;
    SENSOR_PRINT("man_ratio_enable: %d \n", ae_sns_dft->man_ratio_enable);

    return XMEDIA_SUCCESS;
}

static xmedia_void os05a10_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->max_again = 15872; // max 15872 (15.5X)
    ae_sns_dft->min_again = 1024;  // min 1024
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = 1024; // max 1024
    ae_sns_dft->min_dgain = 1024; // min 1024
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure = g_os05a10_exposure[dev] ? g_os05a10_exposure[dev] : 76151; // init 76151

    ae_sns_dft->max_int_time = sns_ctx->fl_std - OS05A10_EXP_OFFSET;
    ae_sns_dft->min_int_time = 4; // min int 4
    ae_sns_dft->max_int_time_target = 65535; // max int 65535
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    return;
}

static xmedia_s32 os05a10_get_ae_2to1_wdr_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                              xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    ae_sns_dft->max_int_time = os05a10_ctx->fl_std - OS05A10_EXP_OFFSET;
    ae_sns_dft->min_int_time = 4; // min_int_time 4
    ae_sns_dft->int_time_accu.offset = 0; // 0.66 line for stagger

    ae_sns_dft->max_int_time_target = 65535; // max 65535
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    ae_sns_dft->max_again        = 15872;
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain         = 1024 * 15.99; // 15.99X
    ae_sns_dft->min_dgain         = 1024;  // 1X
    ae_sns_dft->max_dgain_target  = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target  = ae_sns_dft->min_dgain;
    ae_sns_dft->init_exposure     = g_os05a10_exposure[dev] ? g_os05a10_exposure[dev] : 52000;

    if (os05a10_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
        ae_sns_dft->ae_compensation = 56;
        ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    } else {
        ae_sns_dft->ae_compensation     = 32;
        ae_sns_dft->ae_exp_mode         = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
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

static xmedia_s32 os05a10_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = os05a10_get_ae_common_default(dev, os05a10_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (os05a10_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            os05a10_get_ae_linear_default(dev, os05a10_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            os05a10_get_ae_2to1_wdr_default(dev, os05a10_ctx, ae_sns_dft);
            break;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{ // TO-DO：min fps is to be confirmed.
    switch (img_mode) {
        case OS05A10_5M_LINEAR_MODE:
        case OS05A10_4M_LINEAR_MODE:
            *min_fps = 2.0;
            break;

        case OS05A10_5M_WDR_MODE:
        case OS05A10_4M_WDR_MODE:
            *min_fps = 2.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case OS05A10_5M_LINEAR_MODE:
            *vmax = OS05A10_VMAX_5M_LINEAR;
            break;

        case OS05A10_4M_LINEAR_MODE:
            *vmax = OS05A10_VMAX_4M_LINEAR;
            break;

        case OS05A10_5M_WDR_MODE:
            *vmax = OS05A10_VMAX_5M_WDR;
            break;

        case OS05A10_4M_WDR_MODE:
            *vmax = OS05A10_VMAX_4M_WDR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = os05a10_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os05a10_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os05a10_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is [%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    ret = os05a10_calc_fps(fps, os05a10_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    os05a10_ctx->fl[SENSOR_CUR_FRAME] = full_lines ;
    os05a10_ctx->fl_std               = os05a10_ctx->fl[SENSOR_CUR_FRAME];
    os05a10_ctx->fps                  = fps;
    ae_sns_dft->fps                   = fps;
    ae_sns_dft->full_lines_std        = os05a10_ctx->fl_std;
    ae_sns_dft->full_lines            = os05a10_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time          = os05a10_ctx->fl[SENSOR_CUR_FRAME] - OS05A10_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]-os05a10 set fps = %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = os05a10_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os05a10_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os05a10_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = (vmax_max_fps ) * max_fps / min_fps;
    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 */
static xmedia_s32 os05a10_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context               *os05a10_ctx = XMEDIA_NULL;
    xmedia_u32                    vmax_min_fps;
    xmedia_s32                    ret;
    xmedia_float                  min_fps;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    if (os05a10_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", os05a10_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    full_lines = full_lines + OS05A10_EXP_OFFSET;
    ret = os05a10_get_min_fps_vmax(os05a10_ctx->img_mode, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                        = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    os05a10_ctx->fl[SENSOR_CUR_FRAME] = full_lines;

    ret = os05a10_get_min_fps(os05a10_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    os05a10_ctx->fps = min_fps * vmax_min_fps / full_lines;

    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines );
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    ae_sns_dft->full_lines   = os05a10_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = os05a10_ctx->fl[SENSOR_CUR_FRAME] - OS05A10_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]-set fps: %f. \n", dev, os05a10_ctx->fps);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context    *os05a10_ctx                      = XMEDIA_NULL;
    static xmedia_bool first[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 1 };

    SENSOR_CHECK_DEV_RETURN(dev);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    if (os05a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (first[dev]) { // 0: short exposure
            os05a10_ctx->wdr_int_time[0] = int_time;
            os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_S_EXP_H].data = SENSOR_HIGH_8BITS(int_time);
            os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_S_EXP_L].data = SENSOR_LOW_8BITS(int_time);
            first[dev] = XMEDIA_FALSE;
        } else { // 1: long exposure
            os05a10_ctx->wdr_int_time[1] = int_time;
            os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_EXP_H].data = SENSOR_HIGH_8BITS(int_time);
            os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_EXP_L].data = SENSOR_LOW_8BITS(int_time);
            first[dev] = XMEDIA_TRUE;
        }
    } else if (os05a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_EXP_H].data = SENSOR_HIGH_8BITS(int_time);
        os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_EXP_L].data = SENSOR_LOW_8BITS(int_time);
        first[dev] = XMEDIA_TRUE;
    }

    return XMEDIA_SUCCESS;
}

#define OS05A10_AGAIN_INDEX_MAX 233
static const xmedia_u32 g_os05a10_again_index[OS05A10_AGAIN_INDEX_MAX] = {
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

static xmedia_u32 g_os05a10_again_table[OS05A10_AGAIN_INDEX_MAX] = {
    0x0080, 0x0088, 0x0090, 0x0098, 0x00a0, 0x00a8, 0x00b0, 0x00b8, 0x00c0, 0x00c8, 0x00d0, 0x00d8, 0x00e0, 0x00e8,
    0x00f0, 0x00f8, 0x0100, 0x0108, 0x0110, 0x0118, 0x0120, 0x0128, 0x0130, 0x0138, 0x0140, 0x0148, 0x0150, 0x0158,
    0x0160, 0x0168, 0x0170, 0x0178, 0x0180, 0x0188, 0x0190, 0x0198, 0x01a0, 0x01a8, 0x01b0, 0x01b8, 0x01c0, 0x01c8,
    0x01d0, 0x01d8, 0x01e0, 0x01e8, 0x01f0, 0x01f8, 0x0200, 0x0208, 0x0210, 0x0218, 0x0220, 0x0228, 0x0230, 0x0238,
    0x0240, 0x0248, 0x0250, 0x0258, 0x0260, 0x0268, 0x0270, 0x0278, 0x0280, 0x0288, 0x0290, 0x0298, 0x02a0, 0x02a8,
    0x02b0, 0x02b8, 0x02c0, 0x02c8, 0x02d0, 0x02d8, 0x02e0, 0x02e8, 0x02f0, 0x02f8, 0x0300, 0x0308, 0x0310, 0x0318,
    0x0320, 0x0328, 0x0330, 0x0338, 0x0340, 0x0348, 0x0350, 0x0358, 0x0360, 0x0368, 0x0370, 0x0378, 0x0380, 0x0388,
    0x0390, 0x0398, 0x03a0, 0x03a8, 0x03b0, 0x03b8, 0x03c0, 0x03c8, 0x03d0, 0x03d8, 0x03e0, 0x03e8, 0x03f0, 0x03f8,
    0x0400, 0x0408, 0x0410, 0x0418, 0x0420, 0x0428, 0x0430, 0x0438, 0x0440, 0x0448, 0x0450, 0x0458, 0x0460, 0x0468,
    0x0470, 0x0478, 0x0480, 0x0488, 0x0490, 0x0498, 0x04a0, 0x04a8, 0x04b0, 0x04b8, 0x04c0, 0x04c8, 0x04d0, 0x04d8,
    0x04e0, 0x04e8, 0x04f0, 0x04f8, 0x0500, 0x0508, 0x0510, 0x0518, 0x0520, 0x0528, 0x0530, 0x0538, 0x0540, 0x0548,
    0x0550, 0x0558, 0x0560, 0x0568, 0x0570, 0x0578, 0x0580, 0x0588, 0x0590, 0x0598, 0x05a0, 0x05a8, 0x05b0, 0x05b8,
    0x05c0, 0x05c8, 0x05d0, 0x05d8, 0x05e0, 0x05e8, 0x05f0, 0x05f8, 0x0600, 0x0608, 0x0610, 0x0618, 0x0620, 0x0628,
    0x0630, 0x0638, 0x0640, 0x0648, 0x0650, 0x0658, 0x0660, 0x0668, 0x0670, 0x0678, 0x0680, 0x0688, 0x0690, 0x0698,
    0x06a0, 0x06a8, 0x06b0, 0x06b8, 0x06c0, 0x06c8, 0x06d0, 0x06d8, 0x06e0, 0x06e8, 0x06f0, 0x06f8, 0x0700, 0x0708,
    0x0710, 0x0718, 0x0720, 0x0728, 0x0730, 0x0738, 0x0740, 0x0748, 0x0750, 0x0758, 0x0760, 0x0768, 0x0770, 0x0778,
    0x0780, 0x0788, 0x0790, 0x0798, 0x07a0, 0x07a8, 0x07b0, 0x07b8, 0x07c0,
};

static xmedia_s32 os05a10_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_os05a10_again_index[OS05A10_AGAIN_INDEX_MAX - 1]) {
        *again_lin = g_os05a10_again_index[OS05A10_AGAIN_INDEX_MAX - 1];
        *again_db  = g_os05a10_again_table[OS05A10_AGAIN_INDEX_MAX - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < OS05A10_AGAIN_INDEX_MAX; i++) {
        if (*again_lin < g_os05a10_again_index[i]) {
            *again_lin = g_os05a10_again_index[i - 1];
            *again_db  = g_os05a10_again_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_AGC_L].data = SENSOR_LOW_8BITS(again);
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_AGC_H].data = ((again & 0x3F00) >> 8);
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_DGC_L].data = SENSOR_LOW_8BITS(dgain);
    os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_DGC_H].data = ((dgain & 0x3F00) >> 8);

    if (os05a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_S_AGC_L].data = SENSOR_LOW_8BITS(again);
        os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_S_AGC_H].data = ((again & 0x3F00) >> 8);
        os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_S_DGC_L].data = SENSOR_LOW_8BITS(dgain);
        os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS05A10_REG_S_DGC_H].data = ((dgain & 0x3F00) >> 8);
    }

    return XMEDIA_SUCCESS;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 os05a10_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    xmedia_u32      short_max_pre_frame;
    xmedia_u32      short_max;
    xmedia_u32      short_time_min_limit;
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);
    memcpy(os05a10_ctx->ratio, inttime_attr->ratio, sizeof(inttime_attr->ratio));

    /*
     * 0x40 is EXP-1X
     * Ratio = L / S * 0x40;
     * VTS   = L + S + EXP_OFFSET;
     * --> S = [(VTS - EXP_OFFSET) * 0x40] / (Ratio + 0x40);
     */
    short_time_min_limit = 4;
    short_max            = 4;
    if (os05a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (os05a10_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
            short_max_pre_frame = os05a10_ctx->fl[SENSOR_PRE_FRAME] - os05a10_ctx->wdr_int_time[0] - OS05A10_EXP_OFFSET;
            short_max           = os05a10_ctx->fl[SENSOR_CUR_FRAME] - OS05A10_EXP_OFFSET;
            short_max           = (short_max_pre_frame < short_max) ? short_max_pre_frame : short_max;
            inttime_attr->max_inttime[0] = short_time_min_limit; // 0：short frame
            inttime_attr->min_inttime[0] = short_time_min_limit;
            inttime_attr->max_inttime[1] = short_max;            // 1：long frame
            inttime_attr->min_inttime[1] = short_time_min_limit;
            return XMEDIA_SUCCESS;
        } else {
            short_max_pre_frame =
                ((os05a10_ctx->fl[SENSOR_PRE_FRAME] - OS05A10_EXP_OFFSET - os05a10_ctx->wdr_int_time[0]) * 0x40) /
                SENSOR_DIV_0_TO_1(os05a10_ctx->ratio[0]);
            short_max =
                ((os05a10_ctx->fl[SENSOR_CUR_FRAME] - OS05A10_EXP_OFFSET) * 0x40) / (os05a10_ctx->ratio[0] + 0x40);
            short_max = (short_max_pre_frame < short_max) ? short_max_pre_frame : short_max;
            short_max = (short_max == 0) ? 1 : short_max;
        }
    }

    if (short_max >= short_time_min_limit) { // 0：short frame; 1：long frame
        inttime_attr->max_inttime[0] = short_max;
        inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * os05a10_ctx->ratio[0]) >> 6; // shift 6
        inttime_attr->min_inttime[0] = short_time_min_limit;
        inttime_attr->min_inttime[1] = (inttime_attr->min_inttime[0] * os05a10_ctx->ratio[0]) >> 6; // shift 6
    } else {
        short_max                    = short_time_min_limit;
        inttime_attr->max_inttime[0] = short_max;
        inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * 0xFFF) >> 6; // shift 6
        inttime_attr->min_inttime[0] = inttime_attr->max_inttime[0];
        inttime_attr->min_inttime[1] = inttime_attr->max_inttime[1];
    }

    return XMEDIA_SUCCESS;
}

// awb static param for Fuji Lens New IR_Cut
#define CALIBRATE_REF_TEMP   5000
#define CALIBRATE_WB_R_GAIN  405//492
#define CALIBRATE_WB_GR_GAIN 256
#define CALIBRATE_WB_GB_GAIN 256
#define CALIBRATE_WB_B_GAIN  608//443

// Calibration results for Auto WB Planck
#define CALIBRATE_AWB_P1 57//16
#define CALIBRATE_AWB_P2 131//240
#define CALIBRATE_AWB_Q1 (-66)//0
#define CALIBRATE_AWB_A1 165896//188580
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-96402)//(-139106)

// Rgain and Bgain of the golden sample
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

// AWB
static xmedia_s32 os05a10_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = CALIBRATE_REF_TEMP;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = CALIBRATE_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = CALIBRATE_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = CALIBRATE_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = CALIBRATE_WB_B_GAIN;

    // 0 ~ 5: point index on the awb curve param
    awb_sns_dft->wb_para[0] = CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1] = CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2] = CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3] = CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4] = CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5] = CALIBRATE_AWB_C1;

    awb_sns_dft->golden_rgain = GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (os05a10_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_os05a10_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_os05a10_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_os05a10_awb_ccm_wdr, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_os05a10_awb_agc_table_wdr, sizeof(xmedia_isp_awb_agc_table));
            break;
    }

    awb_sns_dft->init_rgain   = g_os05a10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_os05a10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_os05a10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_os05a10_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_os05a10_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_standby(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os05a10_write_reg(dev, OS05A10_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_resume(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os05a10_write_reg(dev, OS05A10_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_void os05a10_delay_ms(xmedia_s32 ms)
{
    usleep(ms * 1000);
}

static xmedia_s32 os05a10_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *os05a10_ctx  = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;
    xmedia_u32      reg_num;

    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < os05a10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    // the delay frame num of VTS and VSYNC is different
    if (os05a10_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        reg_num = os05a10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num - 4;
    } else {
        reg_num = os05a10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num;
    }

    for (i = 0; i < reg_num; i++) {
        ret |= os05a10_write_reg(dev, os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                os05a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    if (os05a10_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = os05a10_init_image(dev, os05a10_ctx->img_mode, os05a10_ctx->work_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    os05a10_delay_ms(10);
    ret = os05a10_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = os05a10_set_mirror_flip(dev, os05a10_ctx->mirror_en, os05a10_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os05a10_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_os05a10_dev_map[index] == SENSOR_DEV_INVALID) {
            g_os05a10_dev_map[index] = pipe;
            *dev                     = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are binded to pipe!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 os05a10_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_os05a10_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 os05a10_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    ret = os05a10_get_min_fps(os05a10_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os05a10_get_max_fps(os05a10_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = os05a10_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, OS05A10_NAME, sizeof(OS05A10_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = os05a10_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = os05a10_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = os05a10_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = os05a10_mirror;
    info->isp_func.pfn_sensor_flip             = os05a10_flip;
    info->isp_func.pfn_sensor_set_bus_info     = os05a10_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = os05a10_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = os05a10_set_init_param;
    info->isp_func.pfn_sensor_start            = os05a10_start;
    info->isp_func.pfn_sensor_stop             = os05a10_stop;
    info->isp_func.pfn_sensor_standby          = os05a10_standby;
    info->isp_func.pfn_sensor_resume           = os05a10_resume;
    info->isp_func.pfn_sensor_write_reg        = os05a10_write_reg;
    info->isp_func.pfn_sensor_read_reg         = os05a10_read_reg;
    info->isp_func.pfn_sensor_init             = os05a10_init;
    info->isp_func.pfn_sensor_exit             = os05a10_exit;

    // called by ISP
    info->isp_func.pfn_sensor_set_attr            = os05a10_set_attr;
    info->isp_func.pfn_sensor_get_isp_default     = os05a10_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = os05a10_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = os05a10_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = os05a10_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = os05a10_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = os05a10_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = os05a10_get_fps;
    info->isp_func.pfn_sensor_get_trig_attr       = os05a10_get_trig_attr;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = os05a10_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = os05a10_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = os05a10_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = os05a10_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = os05a10_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = os05a10_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = os05a10_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = os05a10_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = os05a10_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = os05a10_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 os05a10_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = os05a10_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = os05a10_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = os05a10_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OS05A10 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 os05a10_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *os05a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = os05a10_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    OS05A10_GET_CTX(dev, os05a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os05a10_ctx);

    if (os05a10_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OS05A10 unregister function failed!\n");
        return ret;
    }

    os05a10_ctx_exit(dev);
    g_os05a10_dev_map[dev] = SENSOR_DEV_INVALID; // reset dev

    return XMEDIA_SUCCESS;
}
