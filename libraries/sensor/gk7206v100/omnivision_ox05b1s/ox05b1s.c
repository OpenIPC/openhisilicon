#include <stdlib.h>
#include <string.h>
#include "ox05b1s.h"
#include "ox05b1s_ctrl.h"
#include "ox05b1s_ex.h"

#define OX05B1S_INPUT_CLOCK_LINEAR  XMEDIA_SENSOR_CLOCK_FREQ_24MHZ
#define OX05B1S_1M_MAX_FPS_LINEAR   30.0
#define OX05B1S_5M_MAX_FPS_LINEAR   60.0
#define OX05B1S_BIT_RATE_LINEAR     1500

#define OX05B1S_NAME          "OX05B1S"
#define OX05B1S_SPECS_MAX_NUM 2

#ifdef __linux__
#define OX05B1S_ISP_DEFAULT_SUPPORT
#endif

#define OX05B1S_REG_ADDR_STANDBY 0x0100

#define OX05B1S_RES_IS_720P(w, h)  ((w) == 1280 && (h) == 720)
#define OX05B1S_RES_IS_1944P(w, h) ((w) == 2592 && (h) == 1944)

#define OX05B1S_ERR_MODE_PRINT(sns_attr, mipi_lanes)                                                                   \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d, mipi_lanes:%d.\n",                 \
                     sns_attr->width, sns_attr->height, sns_attr->wdr_mode, mipi_lanes);                               \
    } while (0)

sensor_context *g_ox05b1s_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define OX05B1S_SET_CTX(dev, ctx) g_ox05b1s_ctx[dev] = ctx
#define OX05B1S_GET_CTX(dev, ctx) ctx = g_ox05b1s_ctx[dev]

static xmedia_s32 g_ox05b1s_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] =
    { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

static xmedia_u32 g_ox05b1s_again[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_ox05b1s_dgain[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_ox05b1s_isp_dgain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_ox05b1s_init_time[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u32 g_ox05b1s_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_ox05b1s_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_ox05b1s_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u16 g_ox05b1s_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

static const xmedia_sensor_property g_ox05b1s_property[OX05B1S_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        1280, 720, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_NONE, OX05B1S_1M_MAX_FPS_LINEAR, OX05B1S_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_IRGBG, XMEDIA_VIDEO_BAYER_FMT_BGRGI,
          XMEDIA_VIDEO_BAYER_FMT_IGBGR, XMEDIA_VIDEO_BAYER_FMT_GRGBI },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OX05B1S_BIT_RATE_LINEAR,
    },

    {
        // width, height, wdr_mode
        2592, 1944, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_NONE, OX05B1S_5M_MAX_FPS_LINEAR, OX05B1S_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_IRGBG, XMEDIA_VIDEO_BAYER_FMT_BGRGI,
          XMEDIA_VIDEO_BAYER_FMT_IGBGR, XMEDIA_VIDEO_BAYER_FMT_GRGBI },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OX05B1S_BIT_RATE_LINEAR,
    },

};

// awb static param for Fuji Lens New IR_Cut
#define OX05B1S_CALIBRATE_STATIC_TEMP       5120
#define OX05B1S_CALIBRATE_STATIC_WB_R_GAIN  584
#define OX05B1S_CALIBRATE_STATIC_WB_GR_GAIN 256
#define OX05B1S_CALIBRATE_STATIC_WB_GB_GAIN 256
#define OX05B1S_CALIBRATE_STATIC_WB_B_GAIN  486

// Calibration results for Auto WB Planck
#define OX05B1S_CALIBRATE_AWB_P1 (-217)
#define OX05B1S_CALIBRATE_AWB_P2 683
#define OX05B1S_CALIBRATE_AWB_Q1 209
#define OX05B1S_CALIBRATE_AWB_A1 129335
#define OX05B1S_CALIBRATE_AWB_B1 128
#define OX05B1S_CALIBRATE_AWB_C1 (-78613)

// Rgain and Bgain of the golden sample
#define OX05B1S_GOLDEN_RGAIN 0
#define OX05B1S_GOLDEN_BGAIN 0

static const xmedia_sensor_capability g_ox05b1s_capability = {
    .max_width  = 2592,
    .max_height = 1944,
    .max_fps    = 60.0,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_NONE,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,

    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_4L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = OX05B1S_ADDR_BYTE,
        .data_byte_num     = OX05B1S_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { OX05B1S_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { OX05B1S_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x01 },
    },
};

static xmedia_s32 ox05b1s_ctx_init(xmedia_u32 dev)
{
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    if (ox05b1s_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    ox05b1s_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (ox05b1s_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(ox05b1s_ctx, 0, sizeof(sensor_context));
    ox05b1s_ctx->i2c_addr             = OX05B1S_I2C_ADDR;
    ox05b1s_ctx->lanes                = XMEDIA_SENSOR_MIPI_LANES_4L;
    ox05b1s_ctx->work_mode            = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    ox05b1s_ctx->bus_info.i2c_dev     = SENSOR_I2C_INVALID;
    ox05b1s_ctx->img_mode             = OX05B1S_1M_LINEAR_MODE;
    ox05b1s_ctx->size.width           = 1280;
    ox05b1s_ctx->size.height          = 720;
    ox05b1s_ctx->fps                  = g_ox05b1s_property[0].max_fps;
    ox05b1s_ctx->wdr_mode             = XMEDIA_VIDEO_WDR_MODE_NONE;
    ox05b1s_ctx->fl_std               = OX05B1S_VMAX_1M_LINEAR;
    ox05b1s_ctx->fl[SENSOR_CUR_FRAME] = OX05B1S_VMAX_1M_LINEAR;
    ox05b1s_ctx->fl[SENSOR_PRE_FRAME] = OX05B1S_VMAX_1M_LINEAR;

    OX05B1S_SET_CTX(dev, ox05b1s_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void ox05b1s_ctx_exit(xmedia_u32 dev)
{
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_FREE(ox05b1s_ctx);
    OX05B1S_SET_CTX(dev, XMEDIA_NULL);

    return;
}

static xmedia_s32 ox05b1s_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8 i;
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    for (i = 0; i < OX05B1S_SPECS_MAX_NUM; i++) {
        if (g_ox05b1s_property[i].width == ox05b1s_ctx->size.width &&
            g_ox05b1s_property[i].height == ox05b1s_ctx->size.height &&
            g_ox05b1s_property[i].wdr_mode == ox05b1s_ctx->wdr_mode) {
            memcpy(property, &g_ox05b1s_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Please check input param! width:%d, height: %d, mode: %d\n",
            ox05b1s_ctx->size.width, ox05b1s_ctx->size.height, ox05b1s_ctx->wdr_mode);
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 ox05b1s_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ox05b1s_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    if (ox05b1s_ctx->lanes != XMEDIA_SENSOR_MIPI_LANES_4L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "not support mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ox05b1s_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        ox05b1s_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        ox05b1s_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        ox05b1s_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c or %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 ox05b1s_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    ox05b1s_ctx->i2c_addr = slave_addr;
    ret = ox05b1s_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    *slave_addr = ox05b1s_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}



// called by ISP
static xmedia_s32 ox05b1s_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    ox05b1s_ctx->init_mode = init_mode;
    ret = ox05b1s_i2c_init(dev, ox05b1s_ctx->bus_info.i2c_dev, ox05b1s_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    ox05b1s_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    ret = ox05b1s_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ox05b1s_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    ox05b1s_ctx->mirror_en = mirror_en;
    ret = ox05b1s_set_mirror_flip(dev, mirror_en, ox05b1s_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    ox05b1s_ctx->flip_en = flip_en;
    ret = ox05b1s_set_mirror_flip(dev, ox05b1s_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_set_image_mode(sensor_context *ox05b1s_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (ox05b1s_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (OX05B1S_RES_IS_720P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OX05B1S_1M_LINEAR_MODE;
            ox05b1s_ctx->fl_std = OX05B1S_VMAX_1M_LINEAR;
            return XMEDIA_SUCCESS;
        } else if (OX05B1S_RES_IS_1944P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OX05B1S_5M_LINEAR_MODE;
            ox05b1s_ctx->fl_std = OX05B1S_VMAX_5M_LINEAR;
            return XMEDIA_SUCCESS;
        }
    }

    OX05B1S_ERR_MODE_PRINT(sns_attr, ox05b1s_ctx->lanes);
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 ox05b1s_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            ox05b1s_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode.\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support mode %d!\n", wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(ox05b1s_ctx->wdr_int_time, 0, sizeof(ox05b1s_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

#define OX05B1S_LINEAR_REG_INFO_MAX_NUM OX05B1S_REG_MAX_NUM
static xmedia_void ox05b1s_init_common_reg_info(sensor_context *ox05b1s_ctx)
{
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_EXP_H].delay_frame_num = 2;
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_EXP_H].reg_addr = OX05B1S_REG_ADDR_EXP_H;
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_EXP_L].delay_frame_num = 2;
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_EXP_L].reg_addr = OX05B1S_REG_ADDR_EXP_L;

    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_AGC_L].delay_frame_num = 2;
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_AGC_L].reg_addr = OX05B1S_REG_ADDR_AGC_L;
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_AGC_H].delay_frame_num = 2;
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_AGC_H].reg_addr = OX05B1S_REG_ADDR_AGC_H;

    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_DGC_L].delay_frame_num = 2;
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_DGC_L].reg_addr = OX05B1S_REG_ADDR_DGC_L;
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_DGC_M].delay_frame_num = 2;
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_DGC_M].reg_addr = OX05B1S_REG_ADDR_DGC_M;
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_DGC_H].delay_frame_num = 2;
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_DGC_H].reg_addr = OX05B1S_REG_ADDR_DGC_H;

    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_VMAX_H].delay_frame_num = 2;
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_VMAX_H].reg_addr = OX05B1S_VMAX_H_ADDR;
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_VMAX_L].delay_frame_num = 2;
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_VMAX_L].reg_addr = OX05B1S_VMAX_L_ADDR;

    return;
}

static xmedia_s32 ox05b1s_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32 i;
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.type           = XMEDIA_SENSOR_BUS_TYPE_I2C;
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = ox05b1s_ctx->bus_info.i2c_dev;
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = OX05B1S_LINEAR_REG_INFO_MAX_NUM;

    for (i = 0; i < ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = ox05b1s_ctx->i2c_addr;
        ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = OX05B1S_ADDR_BYTE;
        ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = OX05B1S_DATA_BYTE;
    }

    ox05b1s_init_common_reg_info(ox05b1s_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    // Set wdr mode
    ret = ox05b1s_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = ox05b1s_set_image_mode(ox05b1s_ctx, &ox05b1s_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    ox05b1s_ctx->size.width           = sns_attr->width;
    ox05b1s_ctx->size.height          = sns_attr->height;
    ox05b1s_ctx->wdr_mode             = sns_attr->wdr_mode;
    ox05b1s_ctx->fl[SENSOR_CUR_FRAME] = ox05b1s_ctx->fl_std;
    ox05b1s_ctx->fl[SENSOR_PRE_FRAME] = ox05b1s_ctx->fl[SENSOR_CUR_FRAME];

    ret = ox05b1s_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef OX05B1S_ISP_DEFAULT_SUPPORT
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
    return XMEDIA_SUCCESS;
#else
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from ox05b1s!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 ox05b1s_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (ox05b1s_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x100;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    if (ox05b1s_ctx->mirror_en && ox05b1s_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (ox05b1s_ctx->mirror_en && (!ox05b1s_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!ox05b1s_ctx->mirror_en) && ox05b1s_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < OX05B1S_SPECS_MAX_NUM; i++) {
        if (g_ox05b1s_property[i].width == ox05b1s_ctx->size.width   &&
            g_ox05b1s_property[i].height == ox05b1s_ctx->size.height &&
            g_ox05b1s_property[i].wdr_mode == ox05b1s_ctx->wdr_mode) {
            *bayer_pattern = g_ox05b1s_property[i].bayer_format[type];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);
    memcpy(capability, &g_ox05b1s_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ox05b1s_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 ox05b1s_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_ox05b1s_again[dev]         = init_param->again;
    g_ox05b1s_dgain[dev]         = init_param->dgain;
    g_ox05b1s_isp_dgain[dev]     = init_param->ispdgain;
    g_ox05b1s_init_time[dev]     = init_param->exp_time;
    g_ox05b1s_exposure[dev]      = init_param->exposure;
    g_ox05b1s_sample_r_gain[dev] = init_param->sample_rgain;
    g_ox05b1s_sample_b_gain[dev] = init_param->sample_bgain;

    g_ox05b1s_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_ox05b1s_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_ox05b1s_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

xmedia_s32 ox05b1s_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case OX05B1S_1M_LINEAR_MODE:
            *max_fps = OX05B1S_1M_MAX_FPS_LINEAR;
            break;

        case OX05B1S_5M_LINEAR_MODE:
            *max_fps = OX05B1S_5M_MAX_FPS_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = ox05b1s_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines     = sns_ctx->fl_std;
    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 0;
    ae_sns_dft->full_lines_max = OX05B1S_FULL_LINES_MAX;

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
    ae_sns_dft->max_ispdgain_target = 32 << ae_sns_dft->ispdgain_shift;

    memcpy(&ae_sns_dft->piris_attr, &g_ox05b1s_piris_attr, sizeof(xmedia_isp_piris_attr));
    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_32_0;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    // lines per second: VTS * max_fps
    ae_sns_dft->lines_per_500ms  = (sns_ctx->fl_std * max_fps) / 2;

    return XMEDIA_SUCCESS;
}

static xmedia_void ox05b1s_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
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

    ae_sns_dft->init_exposure = g_ox05b1s_exposure[dev] ? g_ox05b1s_exposure[dev] : 76151;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - OX05B1S_EXP_OFFSET;
    ae_sns_dft->min_int_time        = 2;
    ae_sns_dft->max_int_time_target = 65535; // max 65535
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    return;
}

static xmedia_s32 ox05b1s_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    memset(ae_sns_dft, 0, sizeof(xmedia_isp_ae_sensor_default));

    ret = ox05b1s_get_ae_common_default(dev, ox05b1s_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (ox05b1s_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            ox05b1s_get_ae_linear_default(dev, ox05b1s_ctx, ae_sns_dft);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check wdr mode %d!\n", ox05b1s_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_EXP_H].data = SENSOR_HIGH_8BITS(int_time);
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_EXP_L].data = SENSOR_LOW_8BITS(int_time);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    if (ox05b1s_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_AGC_L].data = ((again & 0Xf) << 4);
        ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_AGC_H].data = ((again & 0xf0) >> 4);

        ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_DGC_L].data = ((dgain & 0x03) << 6);
        ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_DGC_M].data = ((dgain & 0x03fc) >> 2);
        ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_DGC_H].data = ((dgain & 0x3c00) >> 10);
    }

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 ox05b1s_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = OX05B1S_CALIBRATE_STATIC_TEMP;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = OX05B1S_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = OX05B1S_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = OX05B1S_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = OX05B1S_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 6: point index on the awb curve param
    awb_sns_dft->wb_para[0]   = OX05B1S_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1]   = OX05B1S_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2]   = OX05B1S_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3]   = OX05B1S_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4]   = OX05B1S_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5]   = OX05B1S_CALIBRATE_AWB_C1;
    awb_sns_dft->golden_rgain = OX05B1S_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = OX05B1S_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (ox05b1s_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_ox05b1s_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_ox05b1s_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support mode %d!\n", ox05b1s_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_ox05b1s_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_ox05b1s_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_ox05b1s_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_ox05b1s_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_ox05b1s_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_standby(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = ox05b1s_write_reg(dev, OX05B1S_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_resume(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = ox05b1s_write_reg(dev, OX05B1S_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_config_init_param(xmedia_u32 dev)
{
    xmedia_u32 i;
    sensor_context *ox05b1s_ctx  = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= ox05b1s_write_reg(dev, ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    if (ox05b1s_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = ox05b1s_init_image(dev, ox05b1s_ctx->img_mode, ox05b1s_ctx->work_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = ox05b1s_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = ox05b1s_set_mirror_flip(dev, ox05b1s_ctx->mirror_en, ox05b1s_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = ox05b1s_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_ox05b1s_dev_map[index] == SENSOR_DEV_INVALID) {
            g_ox05b1s_dev_map[index] = pipe;
            *dev                     = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 ox05b1s_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_ox05b1s_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 ox05b1s_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    for (i = 0; i < ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            ox05b1s_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&ox05b1s_ctx->regs_info[SENSOR_PRE_FRAME], &ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));
    ox05b1s_ctx->fl[SENSOR_PRE_FRAME] = ox05b1s_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{
    switch (img_mode) {
        case OX05B1S_1M_LINEAR_MODE:
        case OX05B1S_5M_LINEAR_MODE:
            *min_fps = 0.51;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case OX05B1S_1M_LINEAR_MODE:
            *vmax = OX05B1S_VMAX_1M_LINEAR;
            break;

        case OX05B1S_5M_LINEAR_MODE:
            *vmax = OX05B1S_VMAX_5M_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;

    ret = ox05b1s_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = ox05b1s_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = ox05b1s_get_vmax(sns_ctx->img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %.2f, range of fps is [%.2f, %.2f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax_max_fps * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, OX05B1S_FULL_LINES_MAX);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    ret = ox05b1s_calc_fps(fps, ox05b1s_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    ox05b1s_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    ox05b1s_ctx->fps                  = fps;

    ae_sns_dft->full_lines_std = full_lines;
    ae_sns_dft->full_lines     = full_lines;
    ae_sns_dft->max_int_time   = full_lines - OX05B1S_EXP_OFFSET;
    ae_sns_dft->fps            = fps;

    SENSOR_PRINT("dev[%d]- cv8001 set fps: %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);

    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines: AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines: AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 */
static xmedia_s32 ox05b1s_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;
    xmedia_float    min_fps, max_fps;
    xmedia_u32      min_fps_vmax, max_fps_vmax;
    xmedia_s32      ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    if (ox05b1s_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", ox05b1s_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = ox05b1s_get_max_fps(ox05b1s_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = ox05b1s_get_min_fps(ox05b1s_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = ox05b1s_get_vmax(ox05b1s_ctx->img_mode, &max_fps_vmax);
    SENSOR_CHECK_RET_RETURN(ret);
    min_fps_vmax = max_fps * max_fps_vmax / SENSOR_DIV_0_TO_1_FLOAT(min_fps);

    full_lines =  full_lines + OX05B1S_EXP_OFFSET;
    full_lines = (full_lines > min_fps_vmax) ? min_fps_vmax : full_lines;
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    ox05b1s_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OX05B1S_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    ox05b1s_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    ox05b1s_ctx->fps                  = min_fps * min_fps_vmax / full_lines;

    ae_sns_dft->full_lines   = ox05b1s_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = ox05b1s_ctx->fl[SENSOR_CUR_FRAME] - OX05B1S_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]- cv8001 set fps: %f\n", dev, ox05b1s_ctx->fps);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    ret = ox05b1s_get_min_fps(ox05b1s_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = ox05b1s_get_max_fps(ox05b1s_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = ox05b1s_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 ox05b1s_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, OX05B1S_NAME, sizeof(OX05B1S_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = ox05b1s_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = ox05b1s_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = ox05b1s_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = ox05b1s_mirror;
    info->isp_func.pfn_sensor_flip             = ox05b1s_flip;
    info->isp_func.pfn_sensor_set_bus_info     = ox05b1s_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = ox05b1s_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = ox05b1s_set_init_param;
    info->isp_func.pfn_sensor_start            = ox05b1s_start;
    info->isp_func.pfn_sensor_stop             = ox05b1s_stop;
    info->isp_func.pfn_sensor_standby          = ox05b1s_standby;
    info->isp_func.pfn_sensor_resume           = ox05b1s_resume;
    info->isp_func.pfn_sensor_write_reg        = ox05b1s_write_reg;
    info->isp_func.pfn_sensor_read_reg         = ox05b1s_read_reg;
    info->isp_func.pfn_sensor_init             = ox05b1s_init;
    info->isp_func.pfn_sensor_exit             = ox05b1s_exit;
    info->isp_func.pfn_sensor_set_attr         = ox05b1s_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = ox05b1s_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = ox05b1s_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = ox05b1s_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = ox05b1s_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = ox05b1s_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = ox05b1s_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = ox05b1s_get_fps;
    // AE
    info->ae_func.pfn_sensor_get_ae_default     = ox05b1s_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = ox05b1s_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = ox05b1s_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = ox05b1s_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = ox05b1s_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = ox05b1s_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = ox05b1s_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = ox05b1s_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = ox05b1s_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = ox05b1s_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 ox05b1s_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = ox05b1s_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = ox05b1s_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = ox05b1s_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OX05B1S register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 ox05b1s_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *ox05b1s_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = ox05b1s_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    OX05B1S_GET_CTX(dev, ox05b1s_ctx);
    SENSOR_CHECK_PTR_RETURN(ox05b1s_ctx);

    if (ox05b1s_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OX05B1S unregister function failed!\n");
        return ret;
    }

    ox05b1s_ctx_exit(dev);
    g_ox05b1s_dev_map[dev] = SENSOR_DEV_INVALID; // reset dev

    return XMEDIA_SUCCESS;
}
