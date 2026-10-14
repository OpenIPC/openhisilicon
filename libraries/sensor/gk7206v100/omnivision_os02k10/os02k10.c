#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "os02k10.h"
#include "os02k10_ctrl.h"
#include "os02k10_ex.h"
#include "xmedia_isp.h"

#define OS02K10_NAME          "OS02K10"
#define OS02K10_SPECS_MAX_NUM 2

#ifdef __linux__
#define OS02K10_ISP_DEFAULT_SUPPORT
#endif

#define OS02K10_REG_ADDR_STANDBY 0x0100
#define OS02K10_BIT_RATE_LINEAR  600
#define OS02K10_BIT_RATE_BUILTIN 1000

#define OS02K10_RES_IS_1080P(w, h) ((w) == 1920 && (h) == 1080)

#define OS02K10_ERR_MODE_PRINT(sns_attr, mipi_lanes)                                                                   \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d, mipi_lanes:%d.\n",                 \
                     sns_attr->width, sns_attr->height, sns_attr->wdr_mode, mipi_lanes);                               \
    } while (0)

static sensor_context *g_os02k10_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define OS02K10_GET_CTX(dev, ctx) ctx = g_os02k10_ctx[dev]
#define OS02K10_SET_CTX(dev, ctx) g_os02k10_ctx[dev] = ctx

static xmedia_s32 g_os02k10_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] =
                                                                       SENSOR_DEV_INVALID };

static xmedia_u32 g_os02k10_again[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_os02k10_dgain[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_os02k10_isp_dgain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_os02k10_init_time[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u32 g_os02k10_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_os02k10_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_os02k10_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u16 g_os02k10_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

static const xmedia_sensor_property g_os02k10_property[OS02K10_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_NONE, 30, XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OS02K10_BIT_RATE_LINEAR,
    },

    {
        // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_BUILT_IN,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_BUILTIN, 30, XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_12,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OS02K10_BIT_RATE_BUILTIN,
    },
};

// awb static param for Fuji Lens New IR_Cut
#define OS02K10_CALIBRATE_STATIC_TEMP       5120
#define OS02K10_CALIBRATE_STATIC_WB_R_GAIN  584
#define OS02K10_CALIBRATE_STATIC_WB_GR_GAIN 256
#define OS02K10_CALIBRATE_STATIC_WB_GB_GAIN 256
#define OS02K10_CALIBRATE_STATIC_WB_B_GAIN  486

// Calibration results for Auto WB Planck
#define OS02K10_CALIBRATE_AWB_P1    -217
#define OS02K10_CALIBRATE_AWB_P2    683
#define OS02K10_CALIBRATE_AWB_Q1    209
#define OS02K10_CALIBRATE_AWB_A1    129335
#define OS02K10_CALIBRATE_AWB_B1    128
#define OS02K10_CALIBRATE_AWB_C1    -78613

// Rgain and Bgain of the golden sample
#define OS02K10_GOLDEN_RGAIN 0
#define OS02K10_GOLDEN_BGAIN 0

static const xmedia_sensor_capability g_os02k10_capability = {
    .max_width  = 1920,
    .max_height = 1080,
    .max_fps    = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_NONE,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_12,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_TRUE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_2L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = OS02K10_ADDR_BYTE,
        .data_byte_num     = OS02K10_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { OS02K10_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { OS02K10_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x01 },
    },
};

static xmedia_s32 os02k10_ctx_init(xmedia_u32 dev)
{
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    OS02K10_GET_CTX(dev, os02k10_ctx);
    if (os02k10_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    os02k10_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (os02k10_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(os02k10_ctx, 0, sizeof(sensor_context));
    os02k10_ctx->i2c_addr             = OS02K10_I2C_ADDR;
    os02k10_ctx->lanes                = XMEDIA_SENSOR_MIPI_LANES_2L;
    os02k10_ctx->work_mode            = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    os02k10_ctx->bus_info.i2c_dev     = SENSOR_I2C_INVALID;
    os02k10_ctx->size.width           = g_os02k10_property[0].width;
    os02k10_ctx->size.height          = g_os02k10_property[0].height;
    os02k10_ctx->fps                  = g_os02k10_property[0].max_fps;
    os02k10_ctx->wdr_mode             = g_os02k10_property[0].wdr_mode;
    os02k10_ctx->img_mode             = OS02K10_SENSOR_2M_LINEAR_MODE;
    os02k10_ctx->fl_std               = OS02K10_VMAX_2M_LINEAR;
    os02k10_ctx->fl[SENSOR_CUR_FRAME] = OS02K10_VMAX_2M_LINEAR;
    os02k10_ctx->fl[SENSOR_PRE_FRAME] = OS02K10_VMAX_2M_LINEAR;
    OS02K10_SET_CTX(dev, os02k10_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void os02k10_ctx_exit(xmedia_u32 dev)
{
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_FREE(os02k10_ctx);
    OS02K10_SET_CTX(dev, XMEDIA_NULL);

    return;
}

static xmedia_s32 os02k10_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    for (i = 0; i < OS02K10_SPECS_MAX_NUM; i++) {
        if (g_os02k10_property[i].width == os02k10_ctx->size.width &&
            g_os02k10_property[i].height == os02k10_ctx->size.height &&
            g_os02k10_property[i].wdr_mode == os02k10_ctx->wdr_mode) {
            memcpy(property, &g_os02k10_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Please check input param!\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 os02k10_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os02k10_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        os02k10_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        os02k10_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        os02k10_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 os02k10_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    os02k10_ctx->i2c_addr = slave_addr;
    ret = os02k10_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    *slave_addr = os02k10_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}



// called by ISP
static xmedia_s32 os02k10_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    os02k10_ctx->init_mode = init_mode;
    ret = os02k10_i2c_init(dev, os02k10_ctx->bus_info.i2c_dev, os02k10_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    os02k10_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    ret = os02k10_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    os02k10_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    os02k10_ctx->mirror_en = mirror_en;
    ret = os02k10_set_mirror_flip(dev, mirror_en, os02k10_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    os02k10_ctx->flip_en = flip_en;
    ret = os02k10_set_mirror_flip(dev, os02k10_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_set_image_mode(sensor_context *os02k10_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (os02k10_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_4L ) {
        if (os02k10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_BUILT_IN) {
            if (OS02K10_RES_IS_1080P(sns_attr->width, sns_attr->height)) {
                *image_mode         = OS02K10_SENSOR_2M_BUILT_IN_MODE;
                os02k10_ctx->fl_std = OS02K10_VMAX_2M_BUILT_IN;
                return XMEDIA_SUCCESS;
           }
        }
    } else if (os02k10_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_2L ){
        if (os02k10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
            if (OS02K10_RES_IS_1080P(sns_attr->width, sns_attr->height)) {
                *image_mode         = OS02K10_SENSOR_2M_LINEAR_MODE;
                os02k10_ctx->fl_std = OS02K10_VMAX_2M_LINEAR;
                return XMEDIA_SUCCESS;
            }
        }
    }

    OS02K10_ERR_MODE_PRINT(sns_attr, os02k10_ctx->lanes);
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 os02k10_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            os02k10_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode.\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_BUILT_IN:
            os02k10_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_BUILT_IN;
            SENSOR_PRINT("sensor built in WDR mode.\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(os02k10_ctx->wdr_int_time, 0, sizeof(os02k10_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

#define OS02K10_LINEAR_REG_INFO_MAX_NUM   OS02K10_REG_L_MAX_NUM
#define OS02K10_2TO1_WDR_REG_INFO_MAX_NUM OS02K10_REG_MAX_NUM
static xmedia_void os02k10_init_common_reg_info(sensor_context *os02k10_ctx)
{
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_EXP_H].delay_frame_num = 2;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_EXP_H].reg_addr = OS02K10_REG_ADDR_EXP_H;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_EXP_L].delay_frame_num = 2;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_EXP_L].reg_addr = OS02K10_REG_ADDR_EXP_L;

    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_AGC_L].delay_frame_num = 2;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_AGC_L].reg_addr = OS02K10_REG_ADDR_AGC_L;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_AGC_H].delay_frame_num = 2;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_AGC_H].reg_addr = OS02K10_REG_ADDR_AGC_H;

    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_DGC_L].delay_frame_num = 2;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_DGC_L].reg_addr = OS02K10_REG_ADDR_DGC_L;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_DGC_M].delay_frame_num = 2;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_DGC_M].reg_addr = OS02K10_REG_ADDR_DGC_M;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_DGC_H].delay_frame_num = 2;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_DGC_H].reg_addr = OS02K10_REG_ADDR_DGC_H;

    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_VMAX_H].delay_frame_num = 2;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_VMAX_H].reg_addr = OS02K10_VMAX_H_ADDR;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_VMAX_L].delay_frame_num = 2;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_VMAX_L].reg_addr = OS02K10_VMAX_L_ADDR;

    return;
}

static xmedia_void os02k10_init_2to1_built_in_reg_info(sensor_context *os02k10_ctx)
{
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_BUILT_IN_AGC_L].delay_frame_num = 2;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_BUILT_IN_AGC_L].reg_addr
        = OS02K10_REG_ADDR_BUILT_IN_AGC_L;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_BUILT_IN_AGC_H].delay_frame_num = 2;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_BUILT_IN_AGC_H].reg_addr
        = OS02K10_REG_ADDR_BUILT_IN_AGC_H;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_BUILT_IN_DGC_L].delay_frame_num = 2;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_BUILT_IN_DGC_L].reg_addr
        = OS02K10_REG_ADDR_BUILT_IN_DGC_L;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_BUILT_IN_DGC_M].delay_frame_num = 2;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_BUILT_IN_DGC_M].reg_addr
        = OS02K10_REG_ADDR_BUILT_IN_DGC_M;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_BUILT_IN_DGC_H].delay_frame_num = 2;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_BUILT_IN_DGC_H].reg_addr
        = OS02K10_REG_ADDR_BUILT_IN_DGC_H;

    return;
}

static xmedia_s32 os02k10_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32 i;
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.type           = XMEDIA_SENSOR_BUS_TYPE_I2C;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = os02k10_ctx->bus_info.i2c_dev;
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = OS02K10_LINEAR_REG_INFO_MAX_NUM;

    if (os02k10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_BUILT_IN) {
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num = OS02K10_2TO1_WDR_REG_INFO_MAX_NUM;
    }

    for (i = 0; i < os02k10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = os02k10_ctx->i2c_addr;
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = OS02K10_ADDR_BYTE;
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = OS02K10_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    os02k10_init_common_reg_info(os02k10_ctx);

    // init 2to1 wdr mode Regs
    if (os02k10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_BUILT_IN) {
        os02k10_init_2to1_built_in_reg_info(os02k10_ctx);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    // Set wdr mode
    ret = os02k10_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = os02k10_set_image_mode(os02k10_ctx, &os02k10_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    os02k10_ctx->size.width  = sns_attr->width;
    os02k10_ctx->size.height = sns_attr->height;
    os02k10_ctx->fl[SENSOR_CUR_FRAME] = os02k10_ctx->fl_std;
    os02k10_ctx->fl[SENSOR_PRE_FRAME] = os02k10_ctx->fl[SENSOR_CUR_FRAME];

    ret = os02k10_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    if (os02k10_ctx->lanes != XMEDIA_SENSOR_MIPI_LANES_4L && os02k10_ctx->lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "please check  params mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os02k10_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef OS02K10_ISP_DEFAULT_SUPPORT
    // TO-DO: alg add param
    isp_default->clut_attr   = XMEDIA_NULL;
    isp_default->crosstalk   = XMEDIA_NULL;
    isp_default->csc         = XMEDIA_NULL;
    isp_default->dpc_static  = XMEDIA_NULL;
    isp_default->expander    = XMEDIA_NULL;
    isp_default->fpn         = XMEDIA_NULL;
    isp_default->gcac        = XMEDIA_NULL;
    isp_default->hlc         = XMEDIA_NULL;
    isp_default->mlsc_attr   = XMEDIA_NULL;
    isp_default->mlsc_lut    = XMEDIA_NULL;
    isp_default->radial_crop = XMEDIA_NULL;
    isp_default->rgbir       = XMEDIA_NULL;
    isp_default->rlsc_attr   = XMEDIA_NULL;
    isp_default->rlsc_lut    = XMEDIA_NULL;
    isp_default->sharpen     = XMEDIA_NULL;
    isp_default->stnr        = XMEDIA_NULL;
    isp_default->wdr         = XMEDIA_NULL;
    switch(os02k10_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            isp_default->blc         = XMEDIA_NULL;
            isp_default->expander    = XMEDIA_NULL;
            break;
        case XMEDIA_VIDEO_WDR_MODE_BUILT_IN:
            isp_default->blc         = &g_os02k10_bulitin_blc;
            isp_default->bnr         = XMEDIA_NULL;
            isp_default->dehaze      = XMEDIA_NULL;
            isp_default->demosaic    = XMEDIA_NULL;
            isp_default->dpc_dynamic = XMEDIA_NULL;
            isp_default->drc         = XMEDIA_NULL;
            isp_default->gamma       = XMEDIA_NULL;
            isp_default->lce         = XMEDIA_NULL;
            isp_default->expander    = &g_os02k10_expader;
           break;
           default:
      }
    return XMEDIA_SUCCESS;
#else
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from os02k10!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 os02k10_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (os02k10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x80;//0x80
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x80;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x80;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x80;
    } else {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x400;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x400;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x400;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x400;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    if (os02k10_ctx->mirror_en && os02k10_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (os02k10_ctx->mirror_en && (!os02k10_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!os02k10_ctx->mirror_en) && os02k10_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < OS02K10_SPECS_MAX_NUM; i++) {
        if (g_os02k10_property[i].width == os02k10_ctx->size.width &&
             g_os02k10_property[i].height ==  os02k10_ctx->size.height &&
            g_os02k10_property[i].wdr_mode == os02k10_ctx->wdr_mode) {
            *bayer_pattern = g_os02k10_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= OS02K10_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);
    memcpy(capability, &g_os02k10_capability, sizeof(xmedia_sensor_capability));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os02k10_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 os02k10_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_os02k10_again[dev]         = init_param->again;
    g_os02k10_dgain[dev]         = init_param->dgain;
    g_os02k10_isp_dgain[dev]     = init_param->ispdgain;
    g_os02k10_init_time[dev]     = init_param->exp_time;
    g_os02k10_exposure[dev]      = init_param->exposure;
    g_os02k10_sample_r_gain[dev] = init_param->sample_rgain;
    g_os02k10_sample_b_gain[dev] = init_param->sample_bgain;

    g_os02k10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_os02k10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_os02k10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

xmedia_s32 os02k10_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case OS02K10_SENSOR_2M_LINEAR_MODE:
            *max_fps = 30;
            break;

        case OS02K10_SENSOR_2M_BUILT_IN_MODE:
            *max_fps = 30;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = os02k10_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines     = sns_ctx->fl_std;
    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 0;
    ae_sns_dft->full_lines_max = OS02K10_FULL_LINES_MAX;

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1.0;
    ae_sns_dft->int_time_accu.offset    = 0.0;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->again_accu.accuracy  = 0.0625;
    ae_sns_dft->again_accu.offset    = 0;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy  = 0.0009765625;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1  << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 16 << ae_sns_dft->ispdgain_shift;

    memcpy(&ae_sns_dft->piris_attr, &g_os02k10_piris_attr, sizeof(xmedia_isp_piris_attr));
    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_32_0;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    // lines per second: VTS * max_fps
    ae_sns_dft->lines_per_500ms  = (sns_ctx->fl_std * max_fps) / 2;

    return XMEDIA_SUCCESS;
}

static xmedia_void os02k10_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->max_again  = 248; // 15.5x
    ae_sns_dft->min_again  = 16;  // 1x
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 1024 * 15.999; // 15x
    ae_sns_dft->min_dgain        = 1024;      // 1x
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x30;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_os02k10_exposure[dev] ? g_os02k10_exposure[dev] : 76151;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - OS02K10_EXP_OFFSET;
    ae_sns_dft->min_int_time        = 2;
    ae_sns_dft->max_int_time_target = 65535; // max 65535
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    return;
}

static xmedia_s32 os02k10_get_ae_2to1_built_in_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                 xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    ae_sns_dft->max_again        = 248;
    ae_sns_dft->min_again        = 16;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;
    ae_sns_dft->ae_compensation = 0x38;

    ae_sns_dft->max_dgain        = 1024 * 15.999;
    ae_sns_dft->min_dgain        = 1024;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - OS02K10_EXP_OFFSET;
    ae_sns_dft->min_int_time        = 2;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    ae_sns_dft->man_ratio_enable = XMEDIA_TRUE;
    ae_sns_dft->ratio[0]         = 16 * 64;
    ae_sns_dft->exp_ratio_max    = ae_sns_dft->ratio[0];
    ae_sns_dft->exp_ratio_min    = ae_sns_dft->ratio[0];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = os02k10_get_ae_common_default(dev, os02k10_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (os02k10_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            os02k10_get_ae_linear_default(dev, os02k10_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_BUILT_IN:
            os02k10_get_ae_2to1_built_in_default(dev, os02k10_ctx, ae_sns_dft);
            break;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_EXP_H].data = SENSOR_HIGH_8BITS(int_time);
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_EXP_L].data = SENSOR_LOW_8BITS(int_time);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *os02k10_ctx = XMEDIA_NULL;
    xmedia_u32  again_agin_hcg;
    xmedia_u32  dgain_agin_hcg;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    if (os02k10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_AGC_L].data = ((again & 0Xf) << 4);
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_AGC_H].data = ((again & 0xf0) >> 4);
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_DGC_L].data = ((dgain & 0x03) << 6);
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_DGC_M].data = ((dgain & 0x03fc) >> 2);
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_DGC_H].data = ((dgain & 0x3c00) >> 10);

    } else if(os02k10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_BUILT_IN){
        again_agin_hcg = again / 2;
        dgain_agin_hcg = dgain / 2;

        if(again_agin_hcg < 16){
            again_agin_hcg = 16;
        }
        if(dgain_agin_hcg < 1024){
            dgain_agin_hcg = 1024;
        }
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_AGC_L].data = ((again_agin_hcg & 0Xf) << 4);
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_AGC_H].data = ((again_agin_hcg & 0xf0) >> 4);
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_DGC_L].data = ((dgain_agin_hcg & 0x03) << 6);
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_DGC_M].data = ((dgain_agin_hcg & 0x03fc) >> 2);
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_DGC_H].data = ((dgain_agin_hcg & 0x3c00) >> 10);

        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_BUILT_IN_AGC_L].data = ((again & 0Xf) << 4);
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_BUILT_IN_AGC_H].data = ((again & 0xf0) >> 4);
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_BUILT_IN_DGC_L].data = ((dgain & 0x03) << 6);
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_BUILT_IN_DGC_M].data = ((dgain & 0x03fc) >> 2);
        os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_BUILT_IN_DGC_H].data = ((dgain & 0x3c00) >> 10);

    }

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 os02k10_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = OS02K10_CALIBRATE_STATIC_TEMP;    // wb_ref_temp 5000

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = OS02K10_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = OS02K10_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = OS02K10_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = OS02K10_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 6: point index on the awb curve param
    awb_sns_dft->wb_para[0]   = OS02K10_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1]   = OS02K10_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2]   = OS02K10_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3]   = OS02K10_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4]   = OS02K10_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5]   = OS02K10_CALIBRATE_AWB_C1;
    awb_sns_dft->golden_rgain = OS02K10_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = OS02K10_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (os02k10_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_os02k10_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_os02k10_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_BUILT_IN:
            memcpy(&awb_sns_dft->ccm, &g_os02k10_awb_wdr_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_os02k10_awb_wdr_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
    }

    awb_sns_dft->init_rgain   = g_os02k10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_os02k10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_os02k10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_os02k10_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_os02k10_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_standby(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os02k10_write_reg(dev, OS02K10_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_resume(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os02k10_write_reg(dev, OS02K10_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_config_init_param(xmedia_u32 dev)
{
    xmedia_u32 i;
    sensor_context *os02k10_ctx   = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < os02k10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < os02k10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= os02k10_write_reg(dev, os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_start(xmedia_u32 dev)
{
    xmedia_s32               ret;
    xmedia_u8                img_mode;
    xmedia_sensor_work_mode  work_mode;
    sensor_context          *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    work_mode  = os02k10_ctx->work_mode;
    img_mode   = os02k10_ctx->img_mode;

    if (os02k10_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = os02k10_init_image(dev, img_mode, work_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = os02k10_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = os02k10_set_mirror_flip(dev, os02k10_ctx->mirror_en, os02k10_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os02k10_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_os02k10_dev_map[index] == SENSOR_DEV_INVALID) {
            g_os02k10_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 os02k10_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_os02k10_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 os02k10_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    for (i = 0; i < os02k10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            os02k10_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &os02k10_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&os02k10_ctx->regs_info[SENSOR_PRE_FRAME], &os02k10_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    os02k10_ctx->fl[SENSOR_PRE_FRAME] = os02k10_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{
    switch (img_mode) {
        case OS02K10_SENSOR_2M_LINEAR_MODE:
            *min_fps = 0.51;
            break;

        case OS02K10_SENSOR_2M_BUILT_IN_MODE:
            *min_fps = 0.51;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case OS02K10_SENSOR_2M_LINEAR_MODE:
            *vmax = OS02K10_VMAX_2M_LINEAR;
            break;

        case OS02K10_SENSOR_2M_BUILT_IN_MODE:
            *vmax = OS02K10_VMAX_2M_BUILT_IN;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = os02k10_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os02k10_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os02k10_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = (vmax_max_fps ) * max_fps / min_fps;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;

    ret = os02k10_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os02k10_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os02k10_get_vmax(sns_ctx->img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %.2f, range of fps is [%.2f, %.2f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax_max_fps * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, OS02K10_FULL_LINES_MAX);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    ret = os02k10_calc_fps(fps, os02k10_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    if(os02k10_ctx->img_mode == OS02K10_SENSOR_2M_BUILT_IN_MODE){
        ae_sns_dft->int_time_accu.offset = 0.2818;
    }
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    os02k10_ctx->fl[SENSOR_CUR_FRAME] = full_lines ;
    os02k10_ctx->fl_std               = os02k10_ctx->fl[SENSOR_CUR_FRAME];
    os02k10_ctx->fps                  = fps;
    ae_sns_dft->fps                   = fps;
    ae_sns_dft->full_lines_std        = os02k10_ctx->fl_std;
    ae_sns_dft->full_lines            = os02k10_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time          = os02k10_ctx->fl[SENSOR_CUR_FRAME] - OS02K10_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]- os02k10 set fps: %f\n", dev, fps);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines: AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines: AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 */
static xmedia_s32 os02k10_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float min_fps;
    xmedia_u32   min_fps_vmax;
    sensor_context  *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    if (os02k10_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", os02k10_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = os02k10_get_min_fps(os02k10_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os02k10_get_min_fps_vmax(os02k10_ctx->img_mode, &min_fps_vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if (os02k10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE){
        full_lines = (full_lines > OS02K10_FULL_LINES_MAX) ? OS02K10_FULL_LINES_MAX : full_lines;
    } else if (os02k10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_BUILT_IN){
        full_lines = (full_lines + 20) /12;
        full_lines = (full_lines < ae_sns_dft->full_lines_std) ? ae_sns_dft->full_lines_std : full_lines;
        full_lines = (full_lines > OS02K10_FULL_LINES_MAX) ? OS02K10_FULL_LINES_MAX : full_lines;
    } else {
        ae_sns_dft->full_lines = os02k10_ctx->fl_std;
    }

    full_lines = (full_lines > min_fps_vmax) ? min_fps_vmax : full_lines;
    os02k10_ctx->fps = min_fps * min_fps_vmax / full_lines;

    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    os02k10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS02K10_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    os02k10_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    ae_sns_dft->full_lines   = os02k10_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = os02k10_ctx->fl[SENSOR_CUR_FRAME] - OS02K10_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]- os02k10 set fps: %f\n", dev, os02k10_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    ret = os02k10_get_min_fps(os02k10_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os02k10_get_max_fps(os02k10_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = os02k10_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, OS02K10_NAME, sizeof(OS02K10_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = os02k10_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = os02k10_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = os02k10_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = os02k10_mirror;
    info->isp_func.pfn_sensor_flip             = os02k10_flip;
    info->isp_func.pfn_sensor_set_bus_info     = os02k10_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = os02k10_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = os02k10_set_init_param;
    info->isp_func.pfn_sensor_start            = os02k10_start;
    info->isp_func.pfn_sensor_stop             = os02k10_stop;
    info->isp_func.pfn_sensor_standby          = os02k10_standby;
    info->isp_func.pfn_sensor_resume           = os02k10_resume;
    info->isp_func.pfn_sensor_write_reg        = os02k10_write_reg;
    info->isp_func.pfn_sensor_read_reg         = os02k10_read_reg;
    info->isp_func.pfn_sensor_init             = os02k10_init;
    info->isp_func.pfn_sensor_exit             = os02k10_exit;

    // called by ISP
    info->isp_func.pfn_sensor_set_attr            = os02k10_set_attr;
    info->isp_func.pfn_sensor_get_isp_default     = os02k10_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = os02k10_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = os02k10_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = os02k10_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = os02k10_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = os02k10_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = os02k10_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = os02k10_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = os02k10_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = os02k10_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = os02k10_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = os02k10_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = os02k10_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = os02k10_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = os02k10_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = os02k10_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = os02k10_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 os02k10_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = os02k10_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = os02k10_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = os02k10_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OS02K10 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 os02k10_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *os02k10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = os02k10_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    OS02K10_GET_CTX(dev, os02k10_ctx);
    SENSOR_CHECK_PTR_RETURN(os02k10_ctx);

    if (os02k10_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OS02K10 unregister function failed!\n");
        return ret;
    }

    os02k10_ctx_exit(dev);
    g_os02k10_dev_map[dev] = SENSOR_DEV_INVALID; // reset dev

    return XMEDIA_SUCCESS;
}
