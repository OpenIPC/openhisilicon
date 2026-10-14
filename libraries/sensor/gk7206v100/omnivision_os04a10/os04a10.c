#include <stdlib.h>
#include <string.h>
#include "os04a10.h"
#include "os04a10_ex.h"
#include "os04a10_ctrl.h"


#include "xmedia_isp.h"

#ifdef __linux__
#define OS04A10_ISP_DEFAULT_SUPPORT
#endif

#ifdef FPGA
#define OS04A10_INPUT_CLOCK_LINEAR 10000000 // 10M
#define OS04A10_MAX_FPS_LINEAR     25.0     // (60 * (OS04A10_INPUT_CLOKC_LINEAR / XMEDIA_SENSOR_CLOCK_FREQ_24MHZ))
#define OS04A10_DATA_WIDTH_LINEAR  XMEDIA_VIDEO_DATA_WIDTH_10
#define OS04A10_INPUT_CLOCK_WDR    7500000  // 7.5M
#define OS04A10_MAX_FPS_WDR        1.875    // (6 * (OS04A10_INPUT_CLOKC_WDR / XMEDIA_SENSOR_CLOCK_FREQ_24MHZ)
#define OS04A10_DATA_WIDTH_WDR     XMEDIA_VIDEO_DATA_WIDTH_10
#define OS04A10_BIT_RATE_LINEAR    420
#define OS04A10_BIT_RATE_WDR       420
#else
#define OS04A10_INPUT_CLOCK_LINEAR XMEDIA_SENSOR_CLOCK_FREQ_24MHZ
#define OS04A10_MAX_FPS_LINEAR     30.0
#define OS04A10_DATA_WIDTH_LINEAR  XMEDIA_VIDEO_DATA_WIDTH_12
#define OS04A10_INPUT_CLOCK_WDR    XMEDIA_SENSOR_CLOCK_FREQ_24MHZ
#define OS04A10_MAX_FPS_WDR        30.0
#define OS04A10_DATA_WIDTH_WDR     XMEDIA_VIDEO_DATA_WIDTH_10
#define OS04A10_BIT_RATE_LINEAR    800
#define OS04A10_BIT_RATE_WDR       1000
#endif

#define OS04A10_NAME          "OS04A10"
#define OS04A10_SPECS_MAX_NUM 2

#define OS04A10_REG_ADDR_STANDBY 0x0100 // BIT[0]

#define OS04A10_RES_IS_1520P(w, h) ((w) == 2688 && (h) == 1520)

#define OS04A10_ERR_MODE_PRINT(sensor_image_mode)                                                                      \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sensor_image_mode->width,       \
                     sensor_image_mode->height, sensor_image_mode->wdr_mode);                                          \
    } while (0)

sensor_context *g_os04a10_ctx[XMEDIA_SENSOR_DEV_MAX_NUM]     = { XMEDIA_NULL };
#define OS04A10_SET_CTX(dev, sns_ctx) g_os04a10_ctx[dev] = sns_ctx
#define OS04A10_GET_CTX(dev, sns_ctx) sns_ctx = g_os04a10_ctx[dev]

// awb static param for Fuji Lens New IR_Cut
#define CALIBRATE_STATIC_WB_R_GAIN  527
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  526

// Calibration results for Auto WB Planck
#define CALIBRATE_AWB_P1 1
#define CALIBRATE_AWB_P2 241
#define CALIBRATE_AWB_Q1 (-15)
#define CALIBRATE_AWB_A1 159211
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-104902)

// Rgain and Bgain of the golden sample
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

static xmedia_u32 g_os04a10_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_os04a10_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_os04a10_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_os04a10_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };
static xmedia_s32 g_os04a10_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

static const xmedia_sensor_property g_os04a10_property[OS04A10_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        2688, 1520, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS04A10_MAX_FPS_LINEAR, OS04A10_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, OS04A10_DATA_WIDTH_LINEAR,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB,
          XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OS04A10_BIT_RATE_LINEAR,
     },

    {
        // width, height, wdr_mode
        2688, 1520, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS04A10_MAX_FPS_WDR, OS04A10_INPUT_CLOCK_WDR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, OS04A10_DATA_WIDTH_WDR,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, OS04A10_BIT_RATE_WDR,
    },
};

static const xmedia_sensor_capability g_os04a10_capability = {
    .max_width  = 2688,
    .max_height = 1520,
    .max_fps    = OS04A10_MAX_FPS_LINEAR,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_4L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = OS04A10_ADDR_BYTE,
        .data_byte_num     = OS04A10_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { OS04A10_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { OS04A10_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x01 },
    },
};

static xmedia_s32 os04a10_ctx_init(xmedia_u32 dev)
{
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    OS04A10_GET_CTX(dev, os04a10_ctx);
    if (os04a10_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    os04a10_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (os04a10_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(os04a10_ctx, 0, sizeof(sensor_context));
    os04a10_ctx->i2c_addr         = OS04A10_I2C_ADDR;
    os04a10_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_4L;
    os04a10_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    os04a10_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    os04a10_ctx->size.width       = 2688;
    os04a10_ctx->size.height      = 1520;
    os04a10_ctx->fps              = g_os04a10_property[0].max_fps;
    os04a10_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    os04a10_ctx->img_mode             = OS04A10_4M_LINEAR_MODE;
    os04a10_ctx->fl_std               = OS04A10_VMAX_4M_LINEAR;
    os04a10_ctx->fl[SENSOR_CUR_FRAME] = OS04A10_VMAX_4M_LINEAR;
    os04a10_ctx->fl[SENSOR_PRE_FRAME] = OS04A10_VMAX_4M_LINEAR;

    OS04A10_SET_CTX(dev, os04a10_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void os04a10_ctx_exit(xmedia_u32 dev)
{
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_FREE(os04a10_ctx);
    OS04A10_SET_CTX(dev, XMEDIA_NULL);
}

static xmedia_s32 os04a10_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    for (i = 0; i < OS04A10_SPECS_MAX_NUM; i++) {
        if (g_os04a10_property[i].width == os04a10_ctx->size.width &&
            g_os04a10_property[i].height == os04a10_ctx->size.height &&
            g_os04a10_property[i].wdr_mode == os04a10_ctx->wdr_mode) {
            memcpy(property, &g_os04a10_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n",
                    os04a10_ctx->size.width, os04a10_ctx->size.height, os04a10_ctx->wdr_mode);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 os04a10_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os04a10_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "No support mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os04a10_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        os04a10_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        os04a10_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        os04a10_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 os04a10_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    os04a10_ctx->i2c_addr = slave_addr;
    ret = os04a10_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    *slave_addr = os04a10_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}


static xmedia_s32 os04a10_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    os04a10_ctx->init_mode = init_mode;
    ret = os04a10_i2c_init(dev, os04a10_ctx->bus_info.i2c_dev, os04a10_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    os04a10_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    ret = os04a10_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    os04a10_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    os04a10_ctx->mirror_en = mirror_en;
    ret = os04a10_set_mirror_flip(dev, mirror_en, os04a10_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    os04a10_ctx->flip_en = flip_en;
    ret = os04a10_set_mirror_flip(dev, os04a10_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_set_image_mode(sensor_context *os04a10_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (os04a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (OS04A10_RES_IS_1520P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OS04A10_4M_LINEAR_MODE;
            os04a10_ctx->fl_std = OS04A10_VMAX_4M_LINEAR ;
        } else {
            OS04A10_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else if (os04a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (OS04A10_RES_IS_1520P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OS04A10_4M_WDR_MODE;
            os04a10_ctx->fl_std = OS04A10_VMAX_4M_WDR;
        } else {
            OS04A10_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    }

    os04a10_ctx->size.width  = sns_attr->width;
    os04a10_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            os04a10_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            os04a10_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
            SENSOR_PRINT("2to1 line WDR\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(os04a10_ctx->wdr_int_time, 0, sizeof(os04a10_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

#define OS04A10_LINEAR_REG_INFO_MAX_NUM   OS04A10_REG_L_MAX_NUM
#define OS04A10_2TO1_WDR_REG_INFO_MAX_NUM OS04A10_REG_MAX_NUM
static xmedia_void os04a10_init_common_reg_info(sensor_context *os04a10_ctx)
{
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_EXP_H].delay_frame_num = 2;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_EXP_H].reg_addr        = OS04A10_REG_ADDR_EXP_H;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_EXP_L].delay_frame_num = 2;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_EXP_L].reg_addr        = OS04A10_REG_ADDR_EXP_L;

    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_DGC_H].delay_frame_num = 2;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_DGC_H].reg_addr        = OS04A10_REG_ADDR_DGAIN_H;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_DGC_M].delay_frame_num = 2;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_DGC_M].reg_addr        = OS04A10_REG_ADDR_DGAIN_M;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_DGC_L].delay_frame_num = 2;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_DGC_L].reg_addr        = OS04A10_REG_ADDR_DGAIN_L;

    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_AGC_H].delay_frame_num = 2;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_AGC_H].reg_addr        = OS04A10_REG_ADDR_AGAIN_H;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_AGC_L].delay_frame_num = 2;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_AGC_L].reg_addr        = OS04A10_REG_ADDR_AGAIN_L;

    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_VMAX_H].delay_frame_num = 2;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_VMAX_H].reg_addr        = OS04A10_REG_ADDR_VMAX_H;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_VMAX_L].delay_frame_num = 2;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_VMAX_L].reg_addr        = OS04A10_REG_ADDR_VMAX_L;

    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_DCG_SWITCH].delay_frame_num = 1;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_DCG_SWITCH].reg_addr = OS04A10_REG_ADDR_DCG_SWITCH;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_DCG_SWITCH].cfg_pos  = XMEDIA_SENSOR_CFG_POS_VBLANK;
    return;
}

static xmedia_void os04a10_init_2to1_wdr_reg_info(sensor_context *os04a10_ctx)
{
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_S_EXP_H].delay_frame_num = 2;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_S_EXP_H].reg_addr        = OS04A10_REG_ADDR_S_EXP_H;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_S_EXP_L].delay_frame_num = 2;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_S_EXP_L].reg_addr        = OS04A10_REG_ADDR_S_EXP_L;

    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_S_DGC_H].delay_frame_num = 2;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_S_DGC_H].reg_addr        = OS04A10_REG_ADDR_S_DGC_H;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_S_DGC_M].delay_frame_num = 2;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_S_DGC_M].reg_addr        = OS04A10_REG_ADDR_S_DGC_M;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_S_DGC_L].delay_frame_num = 2;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_S_DGC_L].reg_addr        = OS04A10_REG_ADDR_S_DGC_L;

    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_S_AGC_H].delay_frame_num = 2;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_S_AGC_H].reg_addr        = OS04A10_REG_ADDR_S_AGC_H;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_S_AGC_L].delay_frame_num = 2;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_S_AGC_L].reg_addr        = OS04A10_REG_ADDR_S_AGC_L;

    return;
}

xmedia_s32 os04a10_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = os04a10_ctx->bus_info.i2c_dev;
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = OS04A10_LINEAR_REG_INFO_MAX_NUM;

     if (os04a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
         os04a10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num = OS04A10_2TO1_WDR_REG_INFO_MAX_NUM;
     }

    for (i = 0; i < os04a10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = os04a10_ctx->i2c_addr;
        os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = OS04A10_ADDR_BYTE;
        os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = OS04A10_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    os04a10_init_common_reg_info(os04a10_ctx);

    // init 2to1 wdr mode Regs
    if (os04a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        os04a10_init_2to1_wdr_reg_info(os04a10_ctx);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    // Set wdr mode
    ret = os04a10_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = os04a10_set_image_mode(os04a10_ctx, &os04a10_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    os04a10_ctx->fl[SENSOR_CUR_FRAME] = os04a10_ctx->fl_std;
    os04a10_ctx->fl[SENSOR_PRE_FRAME] = os04a10_ctx->fl[SENSOR_CUR_FRAME];

    ret = os04a10_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef OS04A10_ISP_DEFAULT_SUPPORT
    // TO-DO: alg add param
    isp_default->blc         = &g_os04a10_blc;
    isp_default->bnr         = &g_os04a10_bnr;
    isp_default->clut_attr   = XMEDIA_NULL;
    isp_default->crosstalk   = &g_os04a10_crosstalk;
    isp_default->csc         = &g_os04a10_csc;
    isp_default->dehaze      = &g_os04a10_dehaze;
    isp_default->demosaic    = &g_os04a10_demosaic;
    isp_default->dpc_dynamic = XMEDIA_NULL;
    isp_default->dpc_static  = XMEDIA_NULL;
    isp_default->drc         = &g_os04a10_drc;
    isp_default->expander    = XMEDIA_NULL;
    isp_default->fpn         = XMEDIA_NULL;
    isp_default->gamma       = &g_os04a10_gamma;
    isp_default->gcac        = &g_os04a10_gcac;
    isp_default->hlc         = XMEDIA_NULL;
    isp_default->lce         = &g_os04a10_lce;
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from os04a10!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 os04a10_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (os04a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x104;
    } else {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x104;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    if (os04a10_ctx->mirror_en && os04a10_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (os04a10_ctx->mirror_en && (!os04a10_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!os04a10_ctx->mirror_en) && os04a10_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < OS04A10_SPECS_MAX_NUM; i++) {
        if (g_os04a10_property[i].width == os04a10_ctx->size.width &&
            g_os04a10_property[i].height == os04a10_ctx->size.height &&
            g_os04a10_property[i].wdr_mode == os04a10_ctx->wdr_mode) {
            *bayer_pattern = g_os04a10_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= OS04A10_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_os04a10_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os04a10_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 os04a10_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_os04a10_exposure[dev]      = init_param->exposure;
    g_os04a10_sample_r_gain[dev] = init_param->sample_rgain;
    g_os04a10_sample_b_gain[dev] = init_param->sample_bgain;

    g_os04a10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_os04a10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_os04a10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

xmedia_s32 os04a10_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case OS04A10_4M_LINEAR_MODE:
            *max_fps = OS04A10_MAX_FPS_LINEAR;
            break;
        case OS04A10_4M_WDR_MODE:
            *max_fps = OS04A10_MAX_FPS_WDR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = os04a10_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256; // light flicker freq: 50Hz, accuracy: 256
    ae_sns_dft->full_lines_max = OS04A10_FULL_LINES_MAX;

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->again_accu.accuracy  = 0.0625; // accuracy: 0.0625 - 1/16

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy  = 0.0009765625; // accuracy: 0.0009765625 - 1/1024

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 32 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->ispdgain_sep_thd    = 32 << ae_sns_dft->ispdgain_shift;

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

static xmedia_void os04a10_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->max_again        = 992;
    ae_sns_dft->min_again        = 16;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = 1024;
    ae_sns_dft->min_dgain = 1024;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure   = g_os04a10_exposure[dev] ? g_os04a10_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - OS04A10_EXP_OFFSET;
    ae_sns_dft->min_int_time        = 1;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    return;
}

static xmedia_s32 os04a10_get_ae_2to1_wdr_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                              xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    ae_sns_dft->max_int_time         = os04a10_ctx->fl_std - OS04A10_EXP_OFFSET;
    ae_sns_dft->min_int_time         = 2;
    ae_sns_dft->int_time_accu.offset = 0;

    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    ae_sns_dft->max_again        = 992;
    ae_sns_dft->min_again        = 16;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain         = 1024 * 15.99;
    ae_sns_dft->min_dgain         = 1024;
    ae_sns_dft->max_dgain_target  = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target  = ae_sns_dft->min_dgain;
    ae_sns_dft->diff_gain_support = XMEDIA_TRUE;
    ae_sns_dft->init_exposure     = g_os04a10_exposure[dev] ? g_os04a10_exposure[dev] : 52000;

    if (os04a10_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
        ae_sns_dft->ae_compensation = 56;
        ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    } else {
        ae_sns_dft->ae_compensation  = 32;
        ae_sns_dft->ae_exp_mode      = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
        ae_sns_dft->man_ratio_enable = XMEDIA_FALSE;

        ae_sns_dft->ratio[0] = 0x400;
        ae_sns_dft->ratio[1] = 0x40;
        ae_sns_dft->ratio[2] = 0x40;
    }

    return XMEDIA_SUCCESS;

}

static xmedia_s32 os04a10_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    memset(ae_sns_dft, 0, sizeof(xmedia_isp_ae_sensor_default));

    ret = os04a10_get_ae_common_default(dev, os04a10_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (os04a10_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            os04a10_get_ae_linear_default(dev, os04a10_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            ret = os04a10_get_ae_2to1_wdr_default(dev, os04a10_ctx, ae_sns_dft);
            SENSOR_CHECK_RET_RETURN(ret);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", os04a10_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context    *os04a10_ctx                      = XMEDIA_NULL;
    static xmedia_bool first[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 1 };

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    if (os04a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (first[dev]) { // 0: short exposure
            os04a10_ctx->wdr_int_time[0] = int_time;
            os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_S_EXP_H].data = SENSOR_HIGH_8BITS(int_time);
            os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_S_EXP_L].data = SENSOR_LOW_8BITS(int_time);
            first[dev] = XMEDIA_FALSE;
        } else { // 1: long exposure
            os04a10_ctx->wdr_int_time[1] = int_time;
            os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_EXP_H].data = SENSOR_HIGH_8BITS(int_time);
            os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_EXP_L].data = SENSOR_LOW_8BITS(int_time);
            first[dev] = XMEDIA_TRUE;
        }
    } else if (os04a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_EXP_H].data = SENSOR_HIGH_8BITS(int_time);
        os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_EXP_L].data = SENSOR_LOW_8BITS(int_time);
        first[dev]                                                                = XMEDIA_TRUE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *os04a10_ctx = XMEDIA_NULL;
    xmedia_u32      again_reg   = 0x00;
    xmedia_u32      dgain_reg   = 0x00;
    xmedia_u32      hcg_l_reg   = 0x00;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    if (again < 0x44) {
        if (os04a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
            hcg_l_reg = 0x12;
        } else {
            hcg_l_reg = 0x32;
        }
        again_reg = again;
    } else {
        hcg_l_reg = 0x02;
        again_reg = (xmedia_u32)((xmedia_float)again/4.25 + 0.5);
    }

    again_reg = SENSOR_CLIP3(again_reg, 0x10, 0xF8);
    dgain_reg = SENSOR_CLIP3(dgain, 0x400, 0x3FFF);

    os04a10_ctx->regs_info[0].i2c_data[OS04A10_REG_AGC_H].data      = ((again_reg & 0X1F0) >> 4);
    os04a10_ctx->regs_info[0].i2c_data[OS04A10_REG_AGC_L].data      = ((again_reg & 0XF) << 4);
    os04a10_ctx->regs_info[0].i2c_data[OS04A10_REG_DGC_H].data      = ((dgain_reg & 0x3C00) >> 10);
    os04a10_ctx->regs_info[0].i2c_data[OS04A10_REG_DGC_M].data      = ((dgain_reg & 0x03FC) >> 2);
    os04a10_ctx->regs_info[0].i2c_data[OS04A10_REG_DGC_L].data      = ((dgain_reg & 0x03) << 6);
    os04a10_ctx->regs_info[0].i2c_data[OS04A10_REG_DCG_SWITCH].data = (hcg_l_reg & 0xFF);

    // AE库暂不支持长短帧配置不同的gain, 这里暂时处理为长短帧gain无差别配置
    if (os04a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        os04a10_ctx->regs_info[0].i2c_data[OS04A10_REG_S_AGC_H].data = ((again_reg & 0X1F0) >> 4);
        os04a10_ctx->regs_info[0].i2c_data[OS04A10_REG_S_AGC_L].data = ((again_reg & 0XF) << 4);
        os04a10_ctx->regs_info[0].i2c_data[OS04A10_REG_S_DGC_H].data = ((dgain_reg & 0x3C00) >> 10);
        os04a10_ctx->regs_info[0].i2c_data[OS04A10_REG_S_DGC_M].data = ((dgain_reg & 0x03FC) >> 2);
        os04a10_ctx->regs_info[0].i2c_data[OS04A10_REG_S_DGC_L].data = ((dgain_reg & 0x03) << 6);
    }

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 os04a10_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = 5000;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = CALIBRATE_STATIC_WB_B_GAIN;

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
    switch (os04a10_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_os04a10_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_os04a10_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_os04a10_awb_ccm_wdr, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_os04a10_awb_agc_table_wdr, sizeof(xmedia_isp_awb_agc_table));
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", os04a10_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_os04a10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_os04a10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_os04a10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_os04a10_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_os04a10_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_standby(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os04a10_write_reg(dev, OS04A10_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_resume(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os04a10_write_reg(dev, OS04A10_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_void os04a10_delay_ms(xmedia_s32 ms)
{
    sleep_us(ms * 1000);
}

static xmedia_s32 os04a10_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *os04a10_ctx  = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < os04a10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < os04a10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= os04a10_write_reg(dev, os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    if (os04a10_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = os04a10_init_image(dev, os04a10_ctx->img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    os04a10_delay_ms(10);
    ret = os04a10_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = os04a10_set_mirror_flip(dev, os04a10_ctx->mirror_en, os04a10_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os04a10_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_os04a10_dev_map[index] == SENSOR_DEV_INVALID) {
            g_os04a10_dev_map[index] = pipe;
            *dev                     = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are binded to pipe!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 os04a10_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_os04a10_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 os04a10_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{ // TO-DO：min fps is to be confirmed.
    switch (img_mode) {
        case OS04A10_4M_LINEAR_MODE:
        case OS04A10_4M_WDR_MODE:
            *min_fps = 1.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case OS04A10_4M_LINEAR_MODE:
            *vmax = OS04A10_VMAX_4M_LINEAR;
            break;

        case OS04A10_4M_WDR_MODE:
            *vmax = OS04A10_VMAX_4M_WDR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = os04a10_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04a10_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04a10_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is (%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, OS04A10_FULL_LINES_MAX);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    for (i = 0; i < os04a10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            os04a10_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &os04a10_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&os04a10_ctx->regs_info[SENSOR_PRE_FRAME], &os04a10_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    os04a10_ctx->fl[SENSOR_PRE_FRAME] = os04a10_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    ret = os04a10_calc_fps(fps, os04a10_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    os04a10_ctx->fl[SENSOR_CUR_FRAME] = full_lines ;
    os04a10_ctx->fl_std               = os04a10_ctx->fl[SENSOR_CUR_FRAME];
    os04a10_ctx->fps                  = fps;
    ae_sns_dft->fps                   = fps;
    ae_sns_dft->full_lines_std        = os04a10_ctx->fl_std;
    ae_sns_dft->full_lines            = os04a10_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time          = os04a10_ctx->fl[SENSOR_CUR_FRAME] - OS04A10_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]- os04a10 set fps: %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = os04a10_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04a10_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04a10_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = vmax_max_fps * max_fps / min_fps;
    return XMEDIA_SUCCESS;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 os04a10_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    xmedia_u32 short_max_pre_frame;
    xmedia_u32 short_max;
    xmedia_u32 short_time_min_limit;
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);
    memcpy(os04a10_ctx->ratio, inttime_attr->ratio, sizeof(inttime_attr->ratio));

    short_time_min_limit = 2;
    short_max            = 2;
    if (os04a10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (os04a10_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
            short_max_pre_frame = os04a10_ctx->fl[SENSOR_PRE_FRAME] - 20 - os04a10_ctx->wdr_int_time[0];
            short_max = os04a10_ctx->fl[SENSOR_CUR_FRAME] - 20; // sensor limit: sub 20
            short_max = (short_max_pre_frame < short_max) ? short_max_pre_frame : short_max;
            inttime_attr->max_inttime[0] = short_time_min_limit;  // index 0：short frame
            inttime_attr->min_inttime[0] = short_time_min_limit;
            inttime_attr->max_inttime[1] = short_max;  // index 1：long frame
            inttime_attr->min_inttime[1] = short_time_min_limit;
            return XMEDIA_SUCCESS;
        } else {
            short_max_pre_frame = ((os04a10_ctx->fl[SENSOR_PRE_FRAME] - 20 - os04a10_ctx->wdr_int_time[0]) * 0x40) /
                                  SENSOR_DIV_0_TO_1(os04a10_ctx->ratio[0]);
            short_max = ((os04a10_ctx->fl[SENSOR_CUR_FRAME] - 20) * 0x40) / (os04a10_ctx->ratio[0] + 0x40);
            short_max = (short_max_pre_frame < short_max) ? short_max_pre_frame : short_max;
            short_max = (short_max == 0) ? 1 : short_max;
        }
    }

    if (short_max >= short_time_min_limit) { // 0：short frame; 1：long frame
        inttime_attr->max_inttime[0] = short_max;
        inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * os04a10_ctx->ratio[0]) >> 6;
        inttime_attr->min_inttime[0] = short_time_min_limit;
        inttime_attr->min_inttime[1] = (inttime_attr->min_inttime[0] * os04a10_ctx->ratio[0]) >> 6;
    } else {
        short_max = short_time_min_limit;
        inttime_attr->max_inttime[0] = short_max;
        inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * 0xFFF) >> 6;
        inttime_attr->min_inttime[0] = inttime_attr->max_inttime[0];
        inttime_attr->min_inttime[1] = inttime_attr->max_inttime[1];
    }
    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 */
static xmedia_s32 os04a10_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *os04a10_ctx = XMEDIA_NULL;
    xmedia_float    min_fps;
    xmedia_u32      vmax_min_fps;
    xmedia_s32      ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    if (os04a10_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", os04a10_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = os04a10_get_min_fps(os04a10_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04a10_get_min_fps_vmax(os04a10_ctx->img_mode, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                        = full_lines + OS04A10_EXP_OFFSET;
    full_lines                        = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    os04a10_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    os04a10_ctx->fps                  = min_fps * vmax_min_fps / full_lines;

    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    os04a10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04A10_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    ae_sns_dft->full_lines   = os04a10_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = os04a10_ctx->fl[SENSOR_CUR_FRAME] - OS04A10_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]- os04a10 set fps: %f\n", dev, os04a10_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    ret = os04a10_get_min_fps(os04a10_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04a10_get_max_fps(os04a10_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = os04a10_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04a10_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, OS04A10_NAME, sizeof(OS04A10_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = os04a10_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = os04a10_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = os04a10_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = os04a10_mirror;
    info->isp_func.pfn_sensor_flip             = os04a10_flip;
    info->isp_func.pfn_sensor_set_bus_info     = os04a10_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = os04a10_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = os04a10_set_init_param;
    info->isp_func.pfn_sensor_start            = os04a10_start;
    info->isp_func.pfn_sensor_stop             = os04a10_stop;
    info->isp_func.pfn_sensor_standby          = os04a10_standby;
    info->isp_func.pfn_sensor_resume           = os04a10_resume;
    info->isp_func.pfn_sensor_write_reg        = os04a10_write_reg;
    info->isp_func.pfn_sensor_read_reg         = os04a10_read_reg;
    info->isp_func.pfn_sensor_init             = os04a10_init;
    info->isp_func.pfn_sensor_exit             = os04a10_exit;
    info->isp_func.pfn_sensor_set_attr         = os04a10_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = os04a10_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = os04a10_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = os04a10_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = os04a10_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = os04a10_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = os04a10_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = os04a10_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = os04a10_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = os04a10_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = os04a10_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = os04a10_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = os04a10_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = os04a10_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = os04a10_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = os04a10_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = os04a10_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = os04a10_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 os04a10_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = os04a10_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = os04a10_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = os04a10_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OS04A10 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 os04a10_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *os04a10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = os04a10_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    OS04A10_GET_CTX(dev, os04a10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04a10_ctx);

    if (os04a10_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OS04A10 unregister function failed!\n");
        return ret;
    }

    os04a10_ctx_exit(dev);
    g_os04a10_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;
}
