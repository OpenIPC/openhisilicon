#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sc530ai.h"
#include "sc530ai_ctrl.h"
#include "sc530ai_ex.h"
#include "xmedia_isp.h"

#define SC530AI_NAME          "SC530AI"
#define SC530AI_4LANE_SPECS_MAX_NUM 2
#define SC530AI_2LANE_SPECS_MAX_NUM 1

#define SC530AI_5M_4LANE_BIT_RATE_LINEAR  500
#define SC530AI_5M_4LANE_BIT_RATE_WDR     800
#define SC530AI_5M_2LANE_BIT_RATE_LINEAR  792

//#define SC530AI_ISP_DEFAULT_SUPPORT

#define SC530AI_REG_ADDR_STANDBY 0x0100

#define SC530AI_RES_IS_1620P(w, h) ((w) == 2880 && (h) == 1620)

#define SC530AI_ISO_LUT_NUM     8

#define SC530AI_ERR_MODE_PRINT(sns_attr)                                                                               \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sns_attr->width,                \
                     sns_attr->height, sns_attr->wdr_mode);                                                            \
    } while (0)

// awb static param for Fuji Lens New IR_Cut
#define SC530AI_CALIBRATE_STATIC_TEMP       5000
#define SC530AI_CALIBRATE_STATIC_WB_R_GAIN  331
#define SC530AI_CALIBRATE_STATIC_WB_GR_GAIN 256
#define SC530AI_CALIBRATE_STATIC_WB_GB_GAIN 256
#define SC530AI_CALIBRATE_STATIC_WB_B_GAIN  541

// Calibration results for Auto WB Planck
#define SC530AI_CALIBRATE_AWB_P1 10
#define SC530AI_CALIBRATE_AWB_P2 (228)
#define SC530AI_CALIBRATE_AWB_Q1 (-17)
#define SC530AI_CALIBRATE_AWB_A1 140796
#define SC530AI_CALIBRATE_AWB_B1 128
#define SC530AI_CALIBRATE_AWB_C1 (-81748)

// Rgain and Bgain of the golden sample
#define SC530AI_GOLDEN_RGAIN 0
#define SC530AI_GOLDEN_BGAIN 0

static sensor_context *g_sc530ai_ctx[XMEDIA_SENSOR_DEV_MAX_NUM]     = { XMEDIA_NULL };
#define SC530AI_SET_CTX(dev, sns_ctx) g_sc530ai_ctx[dev] = sns_ctx
#define SC530AI_GET_CTX(dev, sns_ctx) sns_ctx = g_sc530ai_ctx[dev]
static xmedia_s32      g_sc530ai_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] =
                                                                           SENSOR_DEV_INVALID };
static xmedia_u32 g_sc530ai_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_sc530ai_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_sc530ai_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_sc530ai_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

static const xmedia_sensor_property g_sc530ai_4lane_property[SC530AI_4LANE_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        2880, 1620, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC530AI_5M_4LANE_BIT_RATE_LINEAR,
    },

    {
        // width, height, wdr_mode
        2880, 1620, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, SC530AI_5M_4LANE_BIT_RATE_WDR,
    },
};

static xmedia_sensor_property g_sc530ai_2lane_property[SC530AI_2LANE_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        2880, 1620, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC530AI_5M_2LANE_BIT_RATE_LINEAR,
    },
};


#define SC530AI_AGAIN_INDEX_MAX  64
static const xmedia_u32 g_sc530ai_again_table[SC530AI_AGAIN_INDEX_MAX] = {
    1024, 1088, 1152, 1216, 1280, 1344, 1408, 1472, 1536, 1600, 1664, 1728, 1792, 1856,
    1920, 1984, 2048, 2176, 2304, 2432, 2560, 2688, 2816, 2944, 3072, 3200, 3328, 3456,
    3584, 3712, 3840, 3968, 4096, 4352, 4608, 4864, 5120, 5376, 5632, 5888, 6144, 6400,
    6656, 6912, 7168, 7424, 7680, 7936, 8192, 8704, 9216, 9728, 10240, 10752, 11264,
    11776, 12288, 12800, 13312, 13824, 14336, 14848, 15360, 15872
};

static xmedia_float again_real_cal[]= {
    128, 1, 256, 2, 326.4, 2.55, 326.4*2, 2.55*2, 326.4*4, 2.55*4, 326.4*8,
    2.55*8, 326.4*16, 2.55*16, 326.4*32, 2.55*32
};
static xmedia_float again_reg_table[]={
    128, 0x00, 256,0x01, 326.4, 0x40, 326.4*2, 0x48, 326.4*4, 0x49, 326.4*8,
    0x4b, 326.4*16, 0x4f, 326.4*32, 0x5f
};

static const xmedia_sensor_capability g_sc530ai_capability = {
    .max_width  = 2880,
    .max_height = 1620,
    .max_fps    = SC530AI_SENSOR_FPS_MAX,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_TRUE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_4L | XMEDIA_SENSOR_MIPI_LANES_2L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = SC530AI_ADDR_BYTE,
        .data_byte_num     = SC530AI_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { SC530AI_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { SC530AI_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x01 },
    },
};

static xmedia_sensor_trig_attr g_sc530ai_trig_attr = {
    .trig_mode  = 0,
    .hs_enable  = XMEDIA_FALSE,
    .vs_enable  = XMEDIA_TRUE,
    .hs_invert  = 0,
    .vs_invert  = 0,
    .hs_time    = 0,
    .vs_time    = (1.0 / 30.0) * 1000000,
    .vs_active  = 1 * ((1.0 / ( SC530AI_VMAX_5M_LINEAR * 30.0 )) * 1000000), // the time of 1 line
    .delay_frame_num = 2,
};

static xmedia_s32 sc530ai_ctx_init(xmedia_u32 dev)
{
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SC530AI_GET_CTX(dev, sc530ai_ctx);
    if (sc530ai_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

        sc530ai_ctx = (sensor_context *)malloc(sizeof(sensor_context));
        if (sc530ai_ctx == XMEDIA_NULL) {
            SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
            return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(sc530ai_ctx, 0, sizeof(sensor_context));
    sc530ai_ctx->i2c_addr         = SC530AI_I2C_ADDR;
    sc530ai_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_4L;
    sc530ai_ctx->work_mode            = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    sc530ai_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    sc530ai_ctx->size.width       = g_sc530ai_4lane_property[0].width;
    sc530ai_ctx->size.height          = g_sc530ai_4lane_property[0].height;
    sc530ai_ctx->fps                  = g_sc530ai_4lane_property[0].max_fps;
    sc530ai_ctx->wdr_mode             = g_sc530ai_4lane_property[0].wdr_mode;
    sc530ai_ctx->img_mode             = SC530AI_5M_30FPS_10BIT_LINEAR_MODE;
    sc530ai_ctx->wdr_mode             = XMEDIA_VIDEO_WDR_MODE_NONE;
    sc530ai_ctx->fl_std               = SC530AI_VMAX_5M_LINEAR << SC530AI_EXP_SHIFT;
    sc530ai_ctx->fl[SENSOR_CUR_FRAME] = SC530AI_VMAX_5M_LINEAR << SC530AI_EXP_SHIFT;
    sc530ai_ctx->fl[SENSOR_PRE_FRAME] = SC530AI_VMAX_5M_LINEAR << SC530AI_EXP_SHIFT;

    SC530AI_SET_CTX(dev, sc530ai_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void sc530ai_ctx_exit(xmedia_u32 dev)
{
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_FREE(sc530ai_ctx);
    SC530AI_SET_CTX(dev, XMEDIA_NULL);

    return;
}

static xmedia_s32 sc530ai_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);
    
    if (sc530ai_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_4L) {
        for (i = 0; i < SC530AI_4LANE_SPECS_MAX_NUM; i++) {
            if (g_sc530ai_4lane_property[i].width == sc530ai_ctx->size.width &&
                g_sc530ai_4lane_property[i].height == sc530ai_ctx->size.height &&
                g_sc530ai_4lane_property[i].wdr_mode == sc530ai_ctx->wdr_mode) {
                memcpy(property, &g_sc530ai_4lane_property[i], sizeof(xmedia_sensor_property));
                return XMEDIA_SUCCESS;
            }
        }
    } else if (sc530ai_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_2L) {
        for (i = 0; i < SC530AI_2LANE_SPECS_MAX_NUM; i++) {
            if (g_sc530ai_2lane_property[i].width == sc530ai_ctx->size.width &&
                g_sc530ai_2lane_property[i].height == sc530ai_ctx->size.height &&
                g_sc530ai_2lane_property[i].wdr_mode == sc530ai_ctx->wdr_mode) {
                memcpy(property, &g_sc530ai_2lane_property[i], sizeof(xmedia_sensor_property));
                return XMEDIA_SUCCESS;
            }
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Please check input param!\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 sc530ai_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL && work_mode != XMEDIA_SENSOR_WORK_MODE_SLAVE) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc530ai_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        sc530ai_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        sc530ai_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        sc530ai_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 sc530ai_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    sc530ai_ctx->i2c_addr = slave_addr;
    ret = sc530ai_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    *slave_addr = sc530ai_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

// called by ISP
static xmedia_s32 sc530ai_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    sc530ai_ctx->init_mode = init_mode;
    ret = sc530ai_i2c_init(dev, sc530ai_ctx->bus_info.i2c_dev, sc530ai_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    sc530ai_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    ret = sc530ai_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    sc530ai_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    sc530ai_ctx->mirror_en = mirror_en;
    ret = sc530ai_set_mirror_flip(dev, mirror_en, sc530ai_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    sc530ai_ctx->flip_en = flip_en;
    ret = sc530ai_set_mirror_flip(dev, sc530ai_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_set_image_mode(sensor_context *sc530ai_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (sc530ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (SC530AI_RES_IS_1620P(sns_attr->width, sns_attr->height)) {
            if (sc530ai_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_2L) {
                *image_mode         = SC530AI_2L_5M_25FPS_10BIT_LINEAR_MODE;
                sc530ai_ctx->fl_std = SC530AI_2L_VMAX_5M_LINEAR << SC530AI_EXP_SHIFT;
            } else {
                *image_mode         = SC530AI_5M_30FPS_10BIT_LINEAR_MODE;
                sc530ai_ctx->fl_std = SC530AI_VMAX_5M_LINEAR << SC530AI_EXP_SHIFT;
            }
        } else {
            SC530AI_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else if (sc530ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (SC530AI_RES_IS_1620P(sns_attr->width, sns_attr->height)) {
            if (sc530ai_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_4L) {
                *image_mode         = SC530AI_5M_30FPS_10BIT_WDR_MODE;
                sc530ai_ctx->fl_std = SC530AI_VMAX_5M_WDR << SC530AI_EXP_SHIFT;
                sc530ai_ctx->max_short_exp = SC530AI_S_EXP_MAX_DEFAULT_1620P << SC530AI_EXP_SHIFT;
            } else {
                SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
                return XMEDIA_ERRCODE_NOT_SUPPORT;
            }
        } else {
            SC530AI_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        SC530AI_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc530ai_ctx->size.width  = sns_attr->width;
    sc530ai_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            sc530ai_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            sc530ai_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
            SENSOR_PRINT("2to1 line WDR\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(sc530ai_ctx->wdr_int_time, 0, sizeof(sc530ai_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

#define SC530AI_LINEAR_REG_INFO_MAX_NUM   SC530AI_REG_L_MAX_NUM
#define SC530AI_2TO1_WDR_REG_INFO_MAX_NUM SC530AI_REG_MAX_NUM

static xmedia_void sc530ai_init_common_reg_info(sensor_context *sc530ai_ctx)
{
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_EXP_H].delay_frame_num = 2;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_EXP_H].reg_addr        = SC530AI_REG_ADDR_EXP_H;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_EXP_M].delay_frame_num = 2;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_EXP_M].reg_addr        = SC530AI_REG_ADDR_EXP_M;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_EXP_L].delay_frame_num = 2;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_EXP_L].reg_addr        = SC530AI_REG_ADDR_EXP_L;

    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_AGAIN_H].delay_frame_num = 2;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_AGAIN_H].reg_addr        = SC530AI_REG_ADDR_AGAIN_H;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_AGAIN_L].delay_frame_num = 2;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_AGAIN_L].reg_addr        = SC530AI_REG_ADDR_AGAIN_L;

    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_DGAIN_H].delay_frame_num = 2;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_DGAIN_H].reg_addr        = SC530AI_REG_ADDR_DGAIN_H;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_DGAIN_L].delay_frame_num = 2;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_DGAIN_L].reg_addr        = SC530AI_REG_ADDR_DGAIN_L;

    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_VMAX_H].delay_frame_num = 2;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_VMAX_H].reg_addr        = SC530AI_REG_ADDR_VMAX_H;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_VMAX_L].delay_frame_num = 2;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_VMAX_L].reg_addr        = SC530AI_REG_ADDR_VMAX_L;

    return;
}

static xmedia_void sc530ai_init_2to1_wdr_reg_info(sensor_context *sc530ai_ctx)
{
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_EXP_H].delay_frame_num   = 2;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_EXP_H].reg_addr          = SC530AI_REG_ADDR_S_EXP_H;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_EXP_L].delay_frame_num   = 2;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_EXP_L].reg_addr          = SC530AI_REG_ADDR_S_EXP_L;

    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_AGAIN_H].delay_frame_num = 2;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_AGAIN_H].reg_addr        = SC530AI_REG_ADDR_S_AGAIN_H;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_AGAIN_L].delay_frame_num = 2;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_AGAIN_L].reg_addr        = SC530AI_REG_ADDR_S_AGAIN_L;

    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_DGAIN_H].delay_frame_num = 2;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_DGAIN_H].reg_addr        = SC530AI_REG_ADDR_S_DGAIN_H;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_DGAIN_L].delay_frame_num = 2;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_DGAIN_L].reg_addr        = SC530AI_REG_ADDR_S_DGAIN_L;

    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_EXP_MAX_H].delay_frame_num = 0;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_EXP_MAX_H].reg_addr = SC530AI_REG_ADDR_S_EXP_MAX_H;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_EXP_MAX_L].delay_frame_num = 0;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_EXP_MAX_L].reg_addr = SC530AI_REG_ADDR_S_EXP_MAX_L;

    return;
}

xmedia_s32 sc530ai_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = sc530ai_ctx->bus_info.i2c_dev;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = SC530AI_LINEAR_REG_INFO_MAX_NUM;

    if (sc530ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num = SC530AI_2TO1_WDR_REG_INFO_MAX_NUM;
    }

    for (i = 0; i < sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = sc530ai_ctx->i2c_addr;
        sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = SC530AI_ADDR_BYTE;
        sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = SC530AI_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    sc530ai_init_common_reg_info(sc530ai_ctx);

    // init 2to1 wdr mode Regs
    if (sc530ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc530ai_init_2to1_wdr_reg_info(sc530ai_ctx);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    // Set wdr mode
    ret = sc530ai_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = sc530ai_set_image_mode(sc530ai_ctx, &sc530ai_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    sc530ai_ctx->fl[SENSOR_CUR_FRAME] = sc530ai_ctx->fl_std;
    sc530ai_ctx->fl[SENSOR_PRE_FRAME] = sc530ai_ctx->fl[SENSOR_CUR_FRAME];

    ret = sc530ai_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *sc530ai_ctx = XMEDIA_NULL;
    xmedia_sensor_attr sns_attr;
    xmedia_s32         ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L && mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check param!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc530ai_ctx->lanes = mipi_lanes;
    sns_attr.height    = sc530ai_ctx->size.height;
    sns_attr.width     = sc530ai_ctx->size.width;
    sns_attr.wdr_mode  = sc530ai_ctx->wdr_mode;
    ret = sc530ai_set_image_mode(sc530ai_ctx, &sc530ai_ctx->img_mode, &sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef SC530AI_ISP_DEFAULT_SUPPORT
    // TO-DO: alg add param
    isp_default->blc         = &g_sc530ai_blc;
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
    switch(sc530ai_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
        isp_default->bnr         = &g_sc530ai_bnr;
        isp_default->dehaze      = &g_sc530ai_dehaze;
        isp_default->demosaic    = &g_sc530ai_dms;
        isp_default->dpc_dynamic = &g_sc530ai_dpc;
        isp_default->drc         = &g_sc530ai_drc;
        isp_default->gamma       = &g_sc530ai_gamma;
        isp_default->lce         = &g_sc530ai_lce;
           break;
        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
        isp_default->bnr         = &g_sc530ai_bnr_wdr;
        isp_default->dehaze      = &g_sc530ai_dehaze_wdr;
        isp_default->demosaic    = &g_sc530ai_dms_wdr;
        isp_default->dpc_dynamic = &g_sc530ai_dpc_wdr;
        isp_default->drc         = &g_sc530ai_drc_wdr;
        isp_default->gamma       = &g_sc530ai_gamma_wdr;
        isp_default->lce         = &g_sc530ai_lce_wdr;
        isp_default->wdr         = &g_sc530ai_wdr;
           break;
           default:
      }
    return XMEDIA_SUCCESS;
#else
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from sc530ai!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 sc530ai_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (sc530ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x104;
    } else {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x100;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);

    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    for (i = 0; i < sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            sc530ai_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &sc530ai_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&sc530ai_ctx->regs_info[SENSOR_PRE_FRAME], &sc530ai_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    sc530ai_ctx->fl[SENSOR_PRE_FRAME] = sc530ai_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    if (sc530ai_ctx->mirror_en && sc530ai_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (sc530ai_ctx->mirror_en && (!sc530ai_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!sc530ai_ctx->mirror_en) && sc530ai_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    if (sc530ai_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_4L) {
        for (i = 0; i < SC530AI_4LANE_SPECS_MAX_NUM; i++) {
            if (g_sc530ai_4lane_property[i].width == sc530ai_ctx->size.width &&
                g_sc530ai_4lane_property[i].height == sc530ai_ctx->size.height &&
                g_sc530ai_4lane_property[i].wdr_mode == sc530ai_ctx->wdr_mode) {
                *bayer_pattern = g_sc530ai_4lane_property[i].bayer_format[type];
                break;
            }
        }
        if (i >= SC530AI_4LANE_SPECS_MAX_NUM) {
            *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
            SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else if (sc530ai_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_2L) {
        for (i = 0; i < SC530AI_2LANE_SPECS_MAX_NUM; i++) {
            if (g_sc530ai_2lane_property[i].width == sc530ai_ctx->size.width &&
                g_sc530ai_2lane_property[i].height == sc530ai_ctx->size.height &&
                g_sc530ai_2lane_property[i].wdr_mode == sc530ai_ctx->wdr_mode) {
                *bayer_pattern = g_sc530ai_2lane_property[i].bayer_format[type];
                break;
            }
        }
        if (i >= SC530AI_2LANE_SPECS_MAX_NUM) {
            *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
            SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);
    memcpy(capability, &g_sc530ai_capability, sizeof(xmedia_sensor_capability));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_get_trig_attr(xmedia_sensor_trig_attr *trig_attr)
{
    SENSOR_CHECK_PTR_RETURN(trig_attr);
    memcpy(trig_attr, &g_sc530ai_trig_attr, sizeof(xmedia_sensor_trig_attr));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc530ai_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 sc530ai_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_sc530ai_exposure[dev]      = init_param->exposure;
    g_sc530ai_sample_r_gain[dev] = init_param->sample_rgain;
    g_sc530ai_sample_b_gain[dev] = init_param->sample_bgain;

    g_sc530ai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_sc530ai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_sc530ai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc530ai_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case SC530AI_5M_30FPS_10BIT_LINEAR_MODE:
            *max_fps = 30.0;
            break;

        case SC530AI_5M_30FPS_10BIT_WDR_MODE:
            *max_fps = 30.0;
            break;

        case SC530AI_2L_5M_25FPS_10BIT_LINEAR_MODE:
            *max_fps = 30.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = sc530ai_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 0;
    ae_sns_dft->full_lines_max = SC530AI_FULL_LINES_MAX;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->again_accu.accuracy  = 0.0078125;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy  = 0.0078125;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 2 << ae_sns_dft->ispdgain_shift;

    memcpy(&ae_sns_dft->piris_attr, &g_sc530ai_piris_attr, sizeof(xmedia_isp_piris_attr));

    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_32_0;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    if(g_sc530ai_exposure[dev] == 0){
        ae_sns_dft->init_exposure = 148599;
    }

    // lines per second: VTS * max_fps
    ae_sns_dft->lines_per_500ms  = (sns_ctx->fl_std * max_fps) / 2;
    ae_sns_dft->man_ratio_enable = XMEDIA_FALSE;
    SENSOR_PRINT("man_ratio_enable: %d \n", ae_sns_dft->man_ratio_enable);

    return XMEDIA_SUCCESS;
}

static xmedia_void sc530ai_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 10444; // 32*2.55*128
    ae_sns_dft->min_again        = 0x80;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 508; // 3.969x128
    ae_sns_dft->min_dgain        = 128;  // min: 128
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x38;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_sc530ai_exposure[dev] ? g_sc530ai_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - SC530AI_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 3;     // min 3
    ae_sns_dft->max_int_time_target = 65535; // max 65535
    ae_sns_dft->min_int_time_target = 3;     // min 3

    return;
}

static xmedia_s32 sc530ai_get_ae_2to1_wdr_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                 xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 4;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 10444; // 32*2.55*128
    ae_sns_dft->min_again        = 0x80;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 508;//3.969x128
    ae_sns_dft->min_dgain        = 128;  // min: 128
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;
    ae_sns_dft->init_exposure    = g_sc530ai_exposure[dev] ? g_sc530ai_exposure[dev] : 16462;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - sns_ctx->max_short_exp - SC530AI_EXP_OFFSET_WDR;
    ae_sns_dft->min_int_time        = 8;     // min 5
    ae_sns_dft->max_int_time_target = 65535; // max 65535
    ae_sns_dft->min_int_time_target = 8;     // min 5

    if (sc530ai_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
        ae_sns_dft->ae_compensation = 0x30;
        ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    } else {
        ae_sns_dft->max_dgain_target    = 1024;
        ae_sns_dft->max_ispdgain_target = 4096;
        ae_sns_dft->ae_compensation     = 0x40;
        ae_sns_dft->ae_exp_mode         = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
        ae_sns_dft->man_ratio_enable    = XMEDIA_TRUE;

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

static xmedia_s32 sc530ai_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = sc530ai_get_ae_common_default(dev, sc530ai_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (sc530ai_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            sc530ai_get_ae_linear_default(dev, sc530ai_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            sc530ai_get_ae_2to1_wdr_default(dev, sc530ai_ctx, ae_sns_dft);
            break;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{
    switch (img_mode) {
        case SC530AI_5M_30FPS_10BIT_LINEAR_MODE:
        case SC530AI_2L_5M_25FPS_10BIT_LINEAR_MODE:
            *min_fps = 2.2;     // 30 * 1500 / 0x6978
            break;
        case SC530AI_5M_30FPS_10BIT_WDR_MODE:
            *min_fps = 3.67;        // 30 * 3300 / 0x6978
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case SC530AI_5M_30FPS_10BIT_LINEAR_MODE:
            *vmax = SC530AI_VMAX_5M_LINEAR;
            break;
        case SC530AI_5M_30FPS_10BIT_WDR_MODE:
            *vmax = SC530AI_VMAX_5M_WDR;
            break;
        case SC530AI_2L_5M_25FPS_10BIT_LINEAR_MODE:
            *vmax = SC530AI_2L_VMAX_5M_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines,
                                         xmedia_u32 *lines_per500ms)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = sc530ai_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc530ai_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc530ai_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is [%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);

    if (sns_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE){
        *full_lines = (*full_lines > SC530AI_FULL_LINES_MAX) ? SC530AI_FULL_LINES_MAX : *full_lines;
    } else if(sns_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE){
        *full_lines = (*full_lines > SC530AI_FULL_LINES_MAX_2TO1_WDR) ? SC530AI_FULL_LINES_MAX_2TO1_WDR : *full_lines;
    }

    *lines_per500ms = (vmax << SC530AI_EXP_SHIFT) * max_fps / 2;

    if (sns_ctx->img_mode == SC530AI_5M_30FPS_10BIT_WDR_MODE) {
        sns_ctx->max_short_exp = SC530AI_S_EXP_MAX_DEFAULT_1620P * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);

    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    ret = sc530ai_calc_fps(fps, sc530ai_ctx, &full_lines, &ae_sns_dft->lines_per_500ms);
    SENSOR_CHECK_RET_RETURN(ret);

    if (sc530ai_ctx->work_mode == XMEDIA_SENSOR_WORK_MODE_SLAVE) {
        full_lines -= 3;
    }

    sc530ai_ctx->regs_info[0].i2c_data[SC530AI_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    sc530ai_ctx->regs_info[0].i2c_data[SC530AI_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    sc530ai_ctx->fl[SENSOR_CUR_FRAME] = full_lines << SC530AI_EXP_SHIFT;
    sc530ai_ctx->fl_std               = sc530ai_ctx->fl[SENSOR_CUR_FRAME];
    sc530ai_ctx->fps                  = fps;
    ae_sns_dft->fps                   = fps;
    ae_sns_dft->full_lines_std        = sc530ai_ctx->fl_std;
    ae_sns_dft->full_lines            = sc530ai_ctx->fl[SENSOR_CUR_FRAME];

    if (sc530ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sc530ai_ctx->fl[SENSOR_CUR_FRAME] - SC530AI_EXP_OFFSET_LINEAR;
    } else if (sc530ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_EXP_MAX_H].data =
            SENSOR_HIGH_8BITS(sc530ai_ctx->max_short_exp);
        sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_EXP_MAX_L].data =
            SENSOR_LOW_8BITS(sc530ai_ctx->max_short_exp);
        sc530ai_ctx->max_short_exp = sc530ai_ctx->max_short_exp << SC530AI_EXP_SHIFT;
        ae_sns_dft->max_int_time = sc530ai_ctx->fl[SENSOR_CUR_FRAME] - sc530ai_ctx->max_short_exp -
            SC530AI_EXP_OFFSET_WDR;
    }

    SENSOR_PRINT("dev[%d]-sc530ai set fps = %f.\n", dev, fps);

    return XMEDIA_SUCCESS;

}

static xmedia_s32 sc530ai_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32      ret;
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    *fps = sc530ai_ctx->fps;
    ret  = sc530ai_get_min_fps(sc530ai_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret  = sc530ai_get_max_fps(sc530ai_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = sc530ai_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc530ai_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc530ai_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = (vmax_max_fps << SC530AI_EXP_SHIFT) * max_fps / min_fps; // SC530AI_EXP_SHIFT移位有sensor差异
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    xmedia_u32      ratio;
    xmedia_u32      short_frame_min_exp;
    xmedia_u32      short_frame_max_exp;
    xmedia_u32      max_short_exp;
    xmedia_u32      long_frame_min_exp;
    xmedia_u32      long_frame_max_exp;
    xmedia_u32      reg_short_frame_max_exp;
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);
    memcpy(sc530ai_ctx->ratio, inttime_attr->ratio, sizeof(inttime_attr->ratio));

    max_short_exp = sc530ai_ctx->max_short_exp;
    ratio = inttime_attr->ratio[0];
    short_frame_min_exp = 8;
    long_frame_min_exp  = 8;
    if (sc530ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (sc530ai_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
            inttime_attr->min_inttime[0] = short_frame_min_exp; // 0:short frame; 1: long frame
            inttime_attr->max_inttime[0] = short_frame_min_exp;
            inttime_attr->min_inttime[1] = long_frame_min_exp;
            reg_short_frame_max_exp = inttime_attr->min_inttime[0] + 18;
            inttime_attr->min_inttime[1] = sc530ai_ctx->fl[SENSOR_CUR_FRAME] - reg_short_frame_max_exp - 18;
        } else {
            short_frame_max_exp = ((((sc530ai_ctx->fl[0]) * 0x40)  / SENSOR_DIV_0_TO_1(ratio + 0x40) + 1) * 2) / 2;
            short_frame_max_exp = (short_frame_max_exp > (max_short_exp - 6)) ? (max_short_exp - 6) : short_frame_max_exp;
            short_frame_max_exp = ((short_frame_max_exp * SENSOR_DIV_0_TO_1(ratio) / 0x40 ) >
                (sc530ai_ctx->fl[0] - max_short_exp - SC530AI_EXP_OFFSET_WDR)) ?
                ((sc530ai_ctx->fl[0] - max_short_exp - SC530AI_EXP_OFFSET_WDR) * 0x40 / SENSOR_DIV_0_TO_1(ratio)):
                short_frame_max_exp;
            long_frame_max_exp           = short_frame_max_exp * ratio / 64;
            long_frame_min_exp           = short_frame_min_exp * ratio / 64;
            reg_short_frame_max_exp      = (short_frame_max_exp + 18) / 2;
            inttime_attr->min_inttime[0] = short_frame_min_exp; // 0:short frame; 1: long frame
            inttime_attr->max_inttime[0] = short_frame_max_exp;
            inttime_attr->min_inttime[1] = long_frame_min_exp;
            inttime_attr->max_inttime[1] = long_frame_max_exp;
        }
        sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_EXP_MAX_H].data =
            SENSOR_HIGH_8BITS(reg_short_frame_max_exp);
        sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_EXP_MAX_L].data =
            SENSOR_LOW_8BITS(reg_short_frame_max_exp);
        sc530ai_ctx->max_short_exp = reg_short_frame_max_exp << SC530AI_EXP_SHIFT;
    }

    return XMEDIA_SUCCESS;

}

static xmedia_s32 sc530ai_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context               *sc530ai_ctx = XMEDIA_NULL;
    xmedia_u32                    vmax_min_fps;
    xmedia_s32                    ret;
    xmedia_sensor_ae_inttime_attr inttime_attr;
    xmedia_float                  min_fps;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    if (sc530ai_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (sc530ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        full_lines = full_lines + SC530AI_EXP_OFFSET_LINEAR;
    } else if (sc530ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        full_lines = full_lines + SC530AI_EXP_OFFSET_WDR;
    }

    ret = sc530ai_get_min_fps_vmax(sc530ai_ctx->img_mode, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc530ai_get_min_fps(sc530ai_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                        = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    sc530ai_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_VMAX_H].data =
        SENSOR_HIGH_8BITS(full_lines >> SC530AI_EXP_SHIFT);
    sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_VMAX_L].data =
        SENSOR_LOW_8BITS(full_lines >> SC530AI_EXP_SHIFT);

    ae_sns_dft->full_lines = sc530ai_ctx->fl[SENSOR_CUR_FRAME];
    if (sc530ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sc530ai_ctx->fl[SENSOR_CUR_FRAME] - SC530AI_EXP_OFFSET_LINEAR;
    } else if (sc530ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        inttime_attr.ratio[0] = sc530ai_ctx->ratio[0]; // index 0: 长短帧曝光比
        sc530ai_get_max_inttime(dev, &inttime_attr);
        ae_sns_dft->max_int_time = sc530ai_ctx->fl[SENSOR_CUR_FRAME] - sc530ai_ctx->max_short_exp - SC530AI_EXP_OFFSET_WDR;
    }

    sc530ai_ctx->fps = min_fps * vmax_min_fps / full_lines;

    SENSOR_PRINT("dev[%d] - sc530ai set fps: %f\n", dev, sc530ai_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context    *sc530ai_ctx                                = XMEDIA_NULL;
    static xmedia_bool first[XMEDIA_SENSOR_DEV_MAX_NUM]          = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 1 };
    static xmedia_u32  short_int_time[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
    static xmedia_u32  long_int_time[XMEDIA_SENSOR_DEV_MAX_NUM]  = { 0 };

    SENSOR_CHECK_DEV_RETURN(dev);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    if (sc530ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (first[dev]) { // 0: short exposure
            sc530ai_ctx->wdr_int_time[0] = int_time;
            short_int_time[dev]          = int_time;
            sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_EXP_H].data =
                SENSOR_MIDDLE_8BITS(short_int_time[dev]);
            sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_S_EXP_L].data =
                SENSOR_LOWER_4BITS(short_int_time[dev]);
            first[dev] = XMEDIA_FALSE;
        } else { // 1: long exposure
            sc530ai_ctx->wdr_int_time[1] = int_time;
            long_int_time[dev]           = int_time;
            sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_EXP_H].data =
                SENSOR_HIGHER_4BITS(long_int_time[dev]);
            sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_EXP_M].data =
                SENSOR_MIDDLE_8BITS(long_int_time[dev]);
            sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_EXP_L].data =
                SENSOR_LOWER_4BITS(long_int_time[dev]);
            first[dev] = XMEDIA_TRUE;
        }
    } else if (sc530ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_EXP_H].data = SENSOR_HIGHER_4BITS(int_time);
        sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_EXP_M].data = SENSOR_MIDDLE_8BITS(int_time);
        sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC530AI_REG_EXP_L].data = SENSOR_LOWER_4BITS(int_time);
        first[dev]                                                                = XMEDIA_TRUE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;
    static xmedia_u8 againmax = SC530AI_AGAIN_INDEX_MAX - 1;
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_sc530ai_again_table[againmax]) {
        *again_db = againmax;
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < SC530AI_AGAIN_INDEX_MAX; i++) {
        if (*again_lin < g_sc530ai_again_table[i]) {
            *again_db = i - 1;
            break;
        }
    }

    *again_lin = g_sc530ai_again_table[*again_db];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(dgain_lin);
    SENSOR_CHECK_PTR_RETURN(dgain_db);

    *dgain_db = *dgain_lin / 1024; // precison 1024

    if (*dgain_db == 3) { // 3
        *dgain_db = 2; // 2x
    } else if (*dgain_db >= 4 && *dgain_db < 8) { // 4 <= gain < 8
        *dgain_db = 4; // 4x
    } else if (*dgain_db >= 8) { // 8 <= gain
        *dgain_db = 8; // 8x
    }

    *dgain_lin = *dgain_db * 1024; // precison 1024

    return XMEDIA_SUCCESS;
}

static xmedia_u16 sc530ai_mapping_realgain_to_sensorgain(xmedia_float gain_real)
{
    xmedia_u16 sensor_gain;
    xmedia_u32 dgain = gain_real * 128;
    xmedia_u8 coarse_gain = 0;
    xmedia_u8 dfine_gain = 0;
    xmedia_u8 reg_0x3e06 = 0;

    for(coarse_gain = 1; coarse_gain <= 16; coarse_gain = coarse_gain * 2) {
        if(dgain < (128 * 2 * coarse_gain)) {
            break;
        }
    }
    dfine_gain = dgain / coarse_gain;

    for ( ; coarse_gain >= 2; coarse_gain = coarse_gain / 2) {
        reg_0x3e06 = (reg_0x3e06 << 1) | 0x01;
    }
    sensor_gain = ((xmedia_u16)reg_0x3e06) << 8;
    sensor_gain += dfine_gain;

    return sensor_gain;
}

static xmedia_s32 sc530ai_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *sc530ai_ctx = XMEDIA_NULL;
    xmedia_u16 again_temp, dgain_temp;
    xmedia_float f_dgain;
    xmedia_u8 again_index;
    xmedia_u8 reg_0x3e09;       // again
    xmedia_u8 reg_0x3e08;
    xmedia_u8 reg_0x3e07;       // dgain
    xmedia_u8 reg_0x3e06;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    for(again_index = 14; again_index >= 0 ;again_index -= 2)
    {
        if(again >= again_real_cal[again_index])
            break;
    }

    again_temp = again_reg_table[again_index + 1];

    f_dgain = (again / again_real_cal[again_index] ) * dgain / 128.0;

    dgain_temp = sc530ai_mapping_realgain_to_sensorgain(f_dgain);

    reg_0x3e06 = SENSOR_HIGH_8BITS(dgain_temp);
    reg_0x3e07 = SENSOR_LOW_8BITS (dgain_temp);
    reg_0x3e08 = SENSOR_HIGH_8BITS(again_temp);
    reg_0x3e09 = SENSOR_LOW_8BITS (again_temp);

    sc530ai_ctx->regs_info[0].i2c_data[SC530AI_REG_DGAIN_H].data = reg_0x3e06;
    sc530ai_ctx->regs_info[0].i2c_data[SC530AI_REG_DGAIN_L].data = reg_0x3e07;
    sc530ai_ctx->regs_info[0].i2c_data[SC530AI_REG_AGAIN_H].data = reg_0x3e08;
    sc530ai_ctx->regs_info[0].i2c_data[SC530AI_REG_AGAIN_L].data = reg_0x3e09;

    if (sc530ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc530ai_ctx->regs_info[0].i2c_data[SC530AI_REG_S_DGAIN_H].data = reg_0x3e06;
        sc530ai_ctx->regs_info[0].i2c_data[SC530AI_REG_S_DGAIN_L].data = reg_0x3e07;
        sc530ai_ctx->regs_info[0].i2c_data[SC530AI_REG_S_AGAIN_H].data = reg_0x3e08;
        sc530ai_ctx->regs_info[0].i2c_data[SC530AI_REG_S_AGAIN_L].data = reg_0x3e09;
    }
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = SC530AI_CALIBRATE_STATIC_TEMP;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = SC530AI_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = SC530AI_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = SC530AI_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = SC530AI_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 5: point index on the awb curve param
    awb_sns_dft->wb_para[0] = SC530AI_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1] = SC530AI_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2] = SC530AI_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3] = SC530AI_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4] = SC530AI_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5] = SC530AI_CALIBRATE_AWB_C1;

    awb_sns_dft->golden_rgain = SC530AI_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = SC530AI_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (sc530ai_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_sc530ai_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_sc530ai_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_sc530ai_awb_wdr_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_sc530ai_awb_wdr_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
    }

    awb_sns_dft->init_rgain   = g_sc530ai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_sc530ai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_sc530ai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_sc530ai_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_sc530ai_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_standby(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc530ai_write_reg(dev, SC530AI_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_resume(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc530ai_write_reg(dev, SC530AI_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *sc530ai_ctx   = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= sc530ai_write_reg(dev, sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                sc530ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_start(xmedia_u32 dev)
{
    xmedia_s32       ret;
    sensor_context  *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    if (sc530ai_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = sc530ai_init_image(dev, sc530ai_ctx->img_mode, sc530ai_ctx->work_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = sc530ai_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc530ai_set_mirror_flip(dev, sc530ai_ctx->mirror_en, sc530ai_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc530ai_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc530ai_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_sc530ai_dev_map[index] == SENSOR_DEV_INVALID) {
            g_sc530ai_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 sc530ai_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_sc530ai_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 sc530ai_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, SC530AI_NAME, sizeof(SC530AI_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = sc530ai_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = sc530ai_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = sc530ai_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = sc530ai_mirror;
    info->isp_func.pfn_sensor_flip             = sc530ai_flip;
    info->isp_func.pfn_sensor_set_bus_info     = sc530ai_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = sc530ai_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = sc530ai_set_init_param;
    info->isp_func.pfn_sensor_start            = sc530ai_start;
    info->isp_func.pfn_sensor_stop             = sc530ai_stop;
    info->isp_func.pfn_sensor_standby          = sc530ai_standby;
    info->isp_func.pfn_sensor_resume           = sc530ai_resume;
    info->isp_func.pfn_sensor_write_reg        = sc530ai_write_reg;
    info->isp_func.pfn_sensor_read_reg         = sc530ai_read_reg;
    info->isp_func.pfn_sensor_init             = sc530ai_init;
    info->isp_func.pfn_sensor_exit             = sc530ai_exit;

    // called by ISP
    info->isp_func.pfn_sensor_set_attr            = sc530ai_set_attr;
    info->isp_func.pfn_sensor_get_isp_default     = sc530ai_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = sc530ai_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = sc530ai_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = sc530ai_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = sc530ai_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = sc530ai_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = sc530ai_get_fps;
    info->isp_func.pfn_sensor_get_trig_attr       = sc530ai_get_trig_attr;
    info->isp_func.pfn_sensor_get_fps             = sc530ai_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = sc530ai_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = sc530ai_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = sc530ai_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = sc530ai_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = sc530ai_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = sc530ai_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = sc530ai_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = sc530ai_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = sc530ai_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = sc530ai_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc530ai_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = sc530ai_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc530ai_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = sc530ai_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "SC530AI register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc530ai_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *sc530ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = sc530ai_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    SC530AI_GET_CTX(dev, sc530ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc530ai_ctx);

    if (sc530ai_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "SC530AI unregister function failed!\n");
        return ret;
    }

    sc530ai_ctx_exit(dev);
    g_sc530ai_dev_map[dev] = SENSOR_DEV_INVALID; // reset dev

    return XMEDIA_SUCCESS;
}
