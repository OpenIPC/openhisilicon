#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gc8613.h"
#include "gc8613_ctrl.h"
#include "gc8613_ex.h"
#include "xmedia_isp.h"

#ifdef FPGA
#define GC8613_INPUT_CLOCK_LINEAR       13500000     // 27/2 = 13.5M
#define GC8613_8M_MAX_FPS_LINEAR        (30.0 / 2 / 8)
#define GC8613_8M_BIT_RATE_2L_LINEAR     600
#define GC8613_8M_BIT_RATE_4L_LINEAR     0 // not support
#else
#define GC8613_INPUT_CLOCK_LINEAR        XMEDIA_SENSOR_CLOCK_FREQ_27MHZ
#define GC8613_8M_MAX_FPS_LINEAR         30.0
#define GC8613_8M_BIT_RATE_2L_LINEAR     1000
#define GC8613_8M_BIT_RATE_4L_LINEAR     1500
#endif

#define GC8613_NAME             "GC8613"
#define GC8613_SPECS_MAX_NUM    1

#ifdef __linux__
//#define GC8613_ISP_DEFAULT_SUPPORT
#endif

#define GC8613_RES_IS_2160P(w, h) ((w) == 3840 && (h) == 2160)

#define GC8613_ERR_MODE_PRINT(sns_attr, mipi_lanes)                                                                    \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d, mipi_lanes:%d.\n",                 \
                     sns_attr->width, sns_attr->height, sns_attr->wdr_mode, mipi_lanes);                               \
    } while (0)

static sensor_context *g_gc8613_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define GC8613_GET_CTX(dev, ctx) ctx = g_gc8613_ctx[dev]
#define GC8613_SET_CTX(dev, ctx) g_gc8613_ctx[dev] = ctx

static xmedia_s32 g_gc8613_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] =
                                                                      SENSOR_DEV_INVALID };

static xmedia_u32 g_gc8613_again[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_gc8613_dgain[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_gc8613_isp_dgain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_gc8613_init_time[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u32 g_gc8613_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_gc8613_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_gc8613_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u16 g_gc8613_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

static xmedia_sensor_property g_gc8613_2lane_property[GC8613_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        3840, 2160, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_NONE, GC8613_8M_MAX_FPS_LINEAR, GC8613_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10, // mipi 2lanes - 10bit
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_RGGB,
          XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, GC8613_8M_BIT_RATE_2L_LINEAR,
    },
};

static xmedia_sensor_property g_gc8613_4lane_property[GC8613_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        3840, 2160, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_NONE, GC8613_8M_MAX_FPS_LINEAR, GC8613_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_14, // mipi 4lanes - 14bit
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_RGGB,
          XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, GC8613_8M_BIT_RATE_4L_LINEAR,
    },

};
// awb static param for Fuji Lens New IR_Cut
#define GC8613_CALIBRATE_STATIC_TEMP       5000
#define GC8613_CALIBRATE_STATIC_WB_R_GAIN  492
#define GC8613_CALIBRATE_STATIC_WB_GR_GAIN 256
#define GC8613_CALIBRATE_STATIC_WB_GB_GAIN 256
#define GC8613_CALIBRATE_STATIC_WB_B_GAIN  443

// Calibration results for Auto WB Planck
#define GC8613_CALIBRATE_AWB_P1    16
#define GC8613_CALIBRATE_AWB_P2    240
#define GC8613_CALIBRATE_AWB_Q1    0
#define GC8613_CALIBRATE_AWB_A1    188580
#define GC8613_CALIBRATE_AWB_B1    128
#define GC8613_CALIBRATE_AWB_C1    (-139106)

// Rgain and Bgain of the golden sample
#define GC8613_GOLDEN_RGAIN 0
#define GC8613_GOLDEN_BGAIN 0

#define GC8613_FULL_LINES_MAX 0xFFFF

static const xmedia_sensor_capability g_gc8613_capability = {
    .max_width  = 3840,
    .max_height = 2160,
    .max_fps    = GC8613_8M_MAX_FPS_LINEAR,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_14,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_FALSE,

#ifdef FPGA
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_4L,
#else
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_2L|XMEDIA_SENSOR_MIPI_LANES_4L,
#endif
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = GC8613_ADDR_BYTE,
        .data_byte_num     = GC8613_DATA_BYTE,
        // The standby register is missing in datasheet.
    },
};

#define GC8613_REG_VAL_TAB_ROW    20
#define GC8613_REG_VAL_TAB_COL    10
static const xmedia_u32 g_gc8613_again_table[GC8613_REG_VAL_TAB_ROW][GC8613_REG_VAL_TAB_COL] = {
    {0x00, 0x00, 0x00, 0x46, 0x46, 0x74, 0x02, 0x01, 0x00, 0x75},
    {0x90, 0x02, 0x00, 0x47, 0x47, 0x74, 0x02, 0x01, 0x1D, 0x75},
    {0x01, 0x00, 0x00, 0x47, 0x47, 0x77, 0x02, 0x01, 0x3F, 0x75},
    {0x91, 0x02, 0x00, 0x48, 0x48, 0x79, 0x02, 0x01, 0x68, 0x75},
    {0x02, 0x00, 0x00, 0x49, 0x49, 0x7a, 0x02, 0x02, 0x00, 0x75},
    {0x92, 0x02, 0x00, 0x4b, 0x4b, 0x7b, 0x02, 0x02, 0x4A, 0x75},
    {0x03, 0x00, 0x00, 0x4c, 0x4c, 0x7c, 0x02, 0x02, 0x92, 0x75},
    {0x93, 0x02, 0x00, 0x4d, 0x4d, 0x7d, 0x02, 0x03, 0x48, 0x75},
    {0x00, 0x00, 0x01, 0x4f, 0x4f, 0x7e, 0x02, 0x04, 0x0F, 0x75},
    {0x90, 0x02, 0x01, 0x50, 0x50, 0x7f, 0x02, 0x04, 0x94, 0x75},
    {0x01, 0x00, 0x01, 0x51, 0x51, 0x7f, 0x02, 0x06, 0x0F, 0x75},
    {0x91, 0x02, 0x01, 0x53, 0x53, 0x7f, 0x02, 0x07, 0x24, 0x75},
    {0x02, 0x00, 0x01, 0x54, 0x54, 0x7f, 0x02, 0x08, 0x55, 0x75},
    {0x92, 0x02, 0x01, 0x56, 0x56, 0x7f, 0x02, 0x0A, 0x13, 0x75},
    {0x03, 0x00, 0x01, 0x58, 0x58, 0x7f, 0x02, 0x0C, 0x1F, 0x75},
    {0x93, 0x02, 0x01, 0x5a, 0x5a, 0x7f, 0x01, 0x0F, 0x0A, 0x75},
    {0x04, 0x00, 0x01, 0x5c, 0x5c, 0x7f, 0x01, 0x11, 0x87, 0x75},
    {0x94, 0x02, 0x01, 0x5e, 0x5e, 0x7f, 0x01, 0x15, 0x0F, 0x75},
    {0x05, 0x00, 0x01, 0x61, 0x61, 0x7f, 0x00, 0x18, 0x9E, 0x75},
    {0x95, 0x02, 0x01, 0x63, 0x63, 0x7f, 0x00, 0x1E, 0x15, 0x75}
};

static const xmedia_u32 g_gc8613_again_index[GC8613_REG_VAL_TAB_ROW + 1] = {
    1024,
    1265,
    1499,
    1770,
    2098,
    2529,
    2997,
    3541,
    4196,
    5059,
    5994,
    7082,
    8392,
    10117,
    11989,
    14164,
    16784,
    20234,
    23978,
    28328,
    33569,
};

static xmedia_s32 gc8613_ctx_init(xmedia_u32 dev)
{
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    GC8613_GET_CTX(dev, gc8613_ctx);
    if (gc8613_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    gc8613_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (gc8613_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(gc8613_ctx, 0, sizeof(sensor_context));
    gc8613_ctx->i2c_addr             = GC8613_I2C_ADDR;
    gc8613_ctx->lanes                = XMEDIA_SENSOR_MIPI_LANES_4L;
    gc8613_ctx->work_mode            = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    gc8613_ctx->bus_info.i2c_dev     = SENSOR_I2C_INVALID;
    gc8613_ctx->size.width           = g_gc8613_4lane_property[0].width;
    gc8613_ctx->size.height          = g_gc8613_4lane_property[0].height;
    gc8613_ctx->wdr_mode             = g_gc8613_4lane_property[0].wdr_mode;
    gc8613_ctx->fps                  = g_gc8613_4lane_property[0].max_fps;
    gc8613_ctx->img_mode             = GC8613_4L_8M_LINEAR_MODE;
    gc8613_ctx->fl_std               = GC8613_VMAX_4L_8M_LINEAR;
    gc8613_ctx->fl[SENSOR_CUR_FRAME] = GC8613_VMAX_4L_8M_LINEAR;
    gc8613_ctx->fl[SENSOR_PRE_FRAME] = GC8613_VMAX_4L_8M_LINEAR;
    GC8613_SET_CTX(dev, gc8613_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void gc8613_ctx_exit(xmedia_u32 dev)
{
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_FREE(gc8613_ctx);
    GC8613_SET_CTX(dev, XMEDIA_NULL);

    return;
}

static xmedia_s32 gc8613_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *gc8613_ctx              = XMEDIA_NULL;
    xmedia_sensor_property *gc8613_property = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    if (gc8613_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_2L) {
        gc8613_property = g_gc8613_2lane_property;
    } else if(gc8613_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_4L) {
        gc8613_property = g_gc8613_4lane_property;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR, "mipi lanes err.\n");
        goto ERR;
    }

    for (i = 0; i < GC8613_SPECS_MAX_NUM; i++) {
        if (gc8613_property[i].width == gc8613_ctx->size.width &&
            gc8613_property[i].height == gc8613_ctx->size.height &&
            gc8613_property[i].wdr_mode == gc8613_ctx->wdr_mode) {
            memcpy(property, &gc8613_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

ERR:
    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Please check input param!\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 gc8613_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc8613_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L && mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Invalid mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc8613_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        gc8613_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        gc8613_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        gc8613_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 gc8613_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    gc8613_ctx->i2c_addr = slave_addr;
    ret = gc8613_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    *slave_addr = gc8613_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

// called by ISP
static xmedia_s32 gc8613_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    gc8613_ctx->init_mode = init_mode;
    ret = gc8613_i2c_init(dev, gc8613_ctx->bus_info.i2c_dev, gc8613_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    gc8613_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    ret = gc8613_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    gc8613_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    gc8613_ctx->mirror_en = mirror_en;
    ret = gc8613_set_mirror_flip(dev, mirror_en, gc8613_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    gc8613_ctx->flip_en = flip_en;
    ret = gc8613_set_mirror_flip(dev, gc8613_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_set_image_mode(sensor_context *gc8613_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (gc8613_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_4L) {
        if (GC8613_RES_IS_2160P(sns_attr->width, sns_attr->height)) {
            *image_mode        = GC8613_4L_8M_LINEAR_MODE;
            gc8613_ctx->fl_std = GC8613_VMAX_4L_8M_LINEAR;
        } else {
            goto ERR;
        }
    } else if (gc8613_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_2L) {
        if (GC8613_RES_IS_2160P(sns_attr->width, sns_attr->height)) {
            *image_mode        = GC8613_2L_8M_LINEAR_MODE;
            gc8613_ctx->fl_std = GC8613_VMAX_2L_8M_LINEAR;
        } else {
            goto ERR;
        }
    } else {
        goto ERR;
    }

    gc8613_ctx->size.width  = sns_attr->width;
    gc8613_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;

ERR:
    GC8613_ERR_MODE_PRINT(sns_attr, gc8613_ctx->lanes);
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 gc8613_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            gc8613_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            gc8613_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
            SENSOR_PRINT("2to1 line WDR\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(gc8613_ctx->wdr_int_time, 0, sizeof(gc8613_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

#define GC8613_LINEAR_REG_INFO_MAX_NUM   GC8613_REG_L_MAX_NUM
static xmedia_void gc8613_init_common_reg_info(sensor_context *gc8613_ctx)
{
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_EXP_H].delay_frame_num  = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_EXP_H].reg_addr         = GC8613_REG_ADDR_EXP_H;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_EXP_L].delay_frame_num  = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_EXP_L].reg_addr         = GC8613_REG_ADDR_EXP_L;

    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_L].delay_frame_num  = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_L].reg_addr         = GC8613_REG_ADDR_AGC_L;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_M].delay_frame_num  = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_M].reg_addr         = GC8613_REG_ADDR_AGC_M;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_H].delay_frame_num  = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_H].reg_addr         = GC8613_REG_ADDR_AGC_H;

    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC1].delay_frame_num   = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC1].reg_addr          = GC8613_REG_ADDR_AGC1;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC2].delay_frame_num   = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC2].reg_addr          = GC8613_REG_ADDR_AGC2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC3].delay_frame_num   = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC3].reg_addr          = GC8613_REG_ADDR_AGC3;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC4].delay_frame_num   = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC4].reg_addr          = GC8613_REG_ADDR_AGC4;

    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_COL_GC_H].delay_frame_num   = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_COL_GC_H].reg_addr          = GC8613_REG_ADDR_COL_GC_H;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_COL_GC_L].delay_frame_num   = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_COL_GC_L].reg_addr          = GC8613_REG_ADDR_COL_GC_L;

    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC5].delay_frame_num   = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC5].reg_addr          = GC8613_REG_ADDR_AGC5;

    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_DGC_H].delay_frame_num  = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_DGC_H].reg_addr         = GC8613_REG_ADDR_DGC_H;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_DGC_L].delay_frame_num  = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_DGC_L].reg_addr         = GC8613_REG_ADDR_DGC_L;

    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_VMAX_H].delay_frame_num = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_VMAX_H].reg_addr        = GC8613_REG_ADDR_VMAX_H;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_VMAX_L].delay_frame_num = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_VMAX_L].reg_addr        = GC8613_REG_ADDR_VMAX_L;

    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_CTL1].delay_frame_num  = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_CTL1].reg_addr         = GC8613_REG_ADDR_AGC_CTL;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_CTL2].delay_frame_num  = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_CTL2].reg_addr         = GC8613_REG_ADDR_AGC_CTL;

    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_CTL3].delay_frame_num = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_CTL3].reg_addr        = GC8613_REG_ADDR_AGC_CTL;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_CTL4].delay_frame_num = 2;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_CTL4].reg_addr        = GC8613_REG_ADDR_AGC_CTL;

    return;
}

static xmedia_s32 gc8613_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32 i;
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.type           = XMEDIA_SENSOR_BUS_TYPE_I2C;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = gc8613_ctx->bus_info.i2c_dev;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = GC8613_LINEAR_REG_INFO_MAX_NUM;

    for (i = 0; i < gc8613_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = gc8613_ctx->i2c_addr;
        gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = GC8613_ADDR_BYTE;
        gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = GC8613_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    gc8613_init_common_reg_info(gc8613_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    // Set wdr mode
    ret = gc8613_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = gc8613_set_image_mode(gc8613_ctx, &gc8613_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    gc8613_ctx->fl[SENSOR_CUR_FRAME] = gc8613_ctx->fl_std;
    gc8613_ctx->fl[SENSOR_PRE_FRAME] = gc8613_ctx->fl[SENSOR_CUR_FRAME];

    ret = gc8613_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef GC8613_ISP_DEFAULT_SUPPORT
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from gc8613!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 gc8613_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (gc8613_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x40;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x40;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x40;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x40;
    } else {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x40;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x40;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x40;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x40;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *gc8613_ctx = XMEDIA_NULL;
    xmedia_sensor_property        *gc8613_property = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    if (gc8613_ctx->mirror_en && gc8613_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (gc8613_ctx->mirror_en && (!gc8613_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!gc8613_ctx->mirror_en) && gc8613_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    if (gc8613_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_2L) {
        gc8613_property = g_gc8613_2lane_property;
    } else if (gc8613_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_4L) {
        gc8613_property = g_gc8613_4lane_property;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR, "mipi lanes err.\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    for (i = 0; i < GC8613_SPECS_MAX_NUM; i++) {
        if (gc8613_property[i].width == gc8613_ctx->size.width &&
            gc8613_property[i].height == gc8613_ctx->size.height &&
            gc8613_property[i].wdr_mode == gc8613_ctx->wdr_mode) {
            *bayer_pattern = gc8613_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= GC8613_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);
    memcpy(capability, &g_gc8613_capability, sizeof(xmedia_sensor_capability));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    gc8613_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 gc8613_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_gc8613_again[dev]         = init_param->again;
    g_gc8613_dgain[dev]         = init_param->dgain;
    g_gc8613_isp_dgain[dev]     = init_param->ispdgain;
    g_gc8613_init_time[dev]     = init_param->exp_time;
    g_gc8613_exposure[dev]      = init_param->exposure;
    g_gc8613_sample_r_gain[dev] = init_param->sample_rgain;
    g_gc8613_sample_b_gain[dev] = init_param->sample_bgain;

    g_gc8613_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_gc8613_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_gc8613_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

xmedia_s32 gc8613_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case GC8613_4L_8M_LINEAR_MODE:
        case GC8613_2L_8M_LINEAR_MODE:
            *max_fps = GC8613_8M_MAX_FPS_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = gc8613_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines     = sns_ctx->fl_std;
    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256; // light flicker freq: 50Hz, accuracy: 256
    ae_sns_dft->full_lines_max = GC8613_FULL_LINES_MAX;

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1.0;
    ae_sns_dft->int_time_accu.offset    = 0.0;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1.0 / 4;
    ae_sns_dft->again_accu.offset    = 0;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy  = 1.0 / 64;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 32 << ae_sns_dft->ispdgain_shift;

    memcpy(&ae_sns_dft->piris_attr, &g_gc8613_piris_attr, sizeof(xmedia_isp_piris_attr));
    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_4;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_5_6;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    // lines per second: VTS * max_fps
    ae_sns_dft->lines_per_500ms  = (sns_ctx->fl_std * max_fps) / 2;

    return XMEDIA_SUCCESS;
}

static xmedia_void gc8613_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->max_again  = 28328;
    ae_sns_dft->min_again  = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 512;
    ae_sns_dft->min_dgain        = 64;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_gc8613_exposure[dev] ? g_gc8613_exposure[dev] : 76151;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - GC8613_EXP_OFFSET;
    ae_sns_dft->min_int_time        = 2;
    ae_sns_dft->max_int_time_target = 65535; // max 65535
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    ae_sns_dft->ae_route_ex_valid = XMEDIA_FALSE;

    return;
}

static xmedia_s32 gc8613_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = gc8613_get_ae_common_default(dev, gc8613_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (gc8613_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            gc8613_get_ae_linear_default(dev, gc8613_ctx, ae_sns_dft);
            break;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context    *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    if (gc8613_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        SENSOR_TRACE(MODULE_DBG_ERR, "not support mode[%d]!\n", gc8613_ctx->wdr_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    } else if (gc8613_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_EXP_H].data = ((int_time&0x3f00) >> 8);;
        gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_EXP_L].data = SENSOR_LOW_8BITS(int_time);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_gc8613_again_index[GC8613_REG_VAL_TAB_ROW - 1]) {
        *again_lin = g_gc8613_again_index[GC8613_REG_VAL_TAB_ROW - 1];
        *again_db  = GC8613_REG_VAL_TAB_ROW -1;
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < GC8613_REG_VAL_TAB_ROW; i++) {
        if (*again_lin < g_gc8613_again_index[i]) {
            *again_lin = g_gc8613_again_index[i - 1];
            *again_db  = i - 1;
            break;
        }
    }

    if(*again_lin >= 4196 && *again_lin <= 10117){
        *again_lin = 4196;
        *again_db  =8;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    //again
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_CTL1].data = 0x2d;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_L].data    = g_gc8613_again_table[again][0];
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_M].data    = g_gc8613_again_table[again][1];
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_CTL2].data = 0x28;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_H].data    = g_gc8613_again_table[again][2];
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC1].data     = g_gc8613_again_table[again][3];
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC2].data     = g_gc8613_again_table[again][4];
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_CTL3].data = 0x2d;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC3].data     = g_gc8613_again_table[again][5];
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC4].data     = g_gc8613_again_table[again][6];
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_CTL4].data = 0x28;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_COL_GC_H].data = g_gc8613_again_table[again][7];
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_COL_GC_L].data = g_gc8613_again_table[again][8];
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC5].data     = g_gc8613_again_table[again][9];

    //dgain
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_DGC_L].data = dgain & 0x3f;
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_DGC_H].data = ((dgain >> 6) & 0XFF);

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 gc8613_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = GC8613_CALIBRATE_STATIC_TEMP;    // wb_ref_temp 5000

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = GC8613_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = GC8613_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = GC8613_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = GC8613_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 6: point index on the awb curve param
    awb_sns_dft->wb_para[0]   = GC8613_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1]   = GC8613_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2]   = GC8613_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3]   = GC8613_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4]   = GC8613_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5]   = GC8613_CALIBRATE_AWB_C1;
    awb_sns_dft->golden_rgain = GC8613_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = GC8613_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;

    switch (gc8613_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_gc8613_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_gc8613_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
    }

    awb_sns_dft->init_rgain   = g_gc8613_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_gc8613_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_gc8613_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_gc8613_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_gc8613_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_standby(xmedia_u32 dev)
{
    //standby regsiter to be confirmed.
    SENSOR_CHECK_DEV_RETURN(dev);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_resume(xmedia_u32 dev)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_config_init_param(xmedia_u32 dev)
{
    xmedia_u32 i;
    sensor_context *gc8613_ctx   = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < gc8613_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < gc8613_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= gc8613_write_reg(dev, gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_start(xmedia_u32 dev)
{
    xmedia_s32               ret;
    xmedia_u8                img_mode;
    xmedia_sensor_work_mode  work_mode;
    sensor_context          *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    work_mode  = gc8613_ctx->work_mode;
    img_mode   = gc8613_ctx->img_mode;

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check param!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (gc8613_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = gc8613_init_image(dev, img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = gc8613_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = gc8613_set_mirror_flip(dev, gc8613_ctx->mirror_en, gc8613_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = gc8613_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_gc8613_dev_map[index] == SENSOR_DEV_INVALID) {
            g_gc8613_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 gc8613_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_gc8613_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 gc8613_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    //此处无差别更新寄存器配置，是因为在每帧配置gain的时候必须要先配置0x031d，配置此寄存器的目的是为gain寄存器
    //加buffer,保证寄存器n+2帧生效。
    for (i = 0; i < gc8613_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            gc8613_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            if (i >= GC8613_REG_AGC_L && i <= GC8613_REG_AGC_M) {
                gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_CTL1].update = XMEDIA_TRUE;
            }

            if (i >= GC8613_REG_AGC_H && i <= GC8613_REG_AGC2) {
                gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_CTL2].update = XMEDIA_TRUE;
            }

            if (i >= GC8613_REG_AGC3 && i <= GC8613_REG_AGC4){
                gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_CTL3].update = XMEDIA_TRUE;
            }

            if (i >= GC8613_REG_COL_GC_H && i <= GC8613_REG_AGC5) {
                gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_AGC_CTL4].update = XMEDIA_TRUE;
            }

            gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &gc8613_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&gc8613_ctx->regs_info[SENSOR_PRE_FRAME], &gc8613_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    gc8613_ctx->fl[SENSOR_PRE_FRAME] = gc8613_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{
    switch (img_mode) {
        case GC8613_4L_8M_LINEAR_MODE:
        case GC8613_2L_8M_LINEAR_MODE:
            *min_fps = 1.06;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case GC8613_4L_8M_LINEAR_MODE:
            *vmax = GC8613_VMAX_4L_8M_LINEAR;
            break;

        case GC8613_2L_8M_LINEAR_MODE:
            *vmax = GC8613_VMAX_2L_8M_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = gc8613_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc8613_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc8613_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = (vmax_max_fps ) * max_fps / min_fps;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;

    ret = gc8613_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc8613_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc8613_get_vmax(sns_ctx->img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %.2f, range of fps is [%.2f, %.2f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax_max_fps * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    ret = gc8613_calc_fps(fps, gc8613_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_VMAX_H].data = ((full_lines&0x3f00) >> 8);
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    gc8613_ctx->fl[SENSOR_CUR_FRAME]  = full_lines ;
    gc8613_ctx->fl_std                = gc8613_ctx->fl[SENSOR_CUR_FRAME];
    gc8613_ctx->fps                   = fps;
    ae_sns_dft->fps                   = fps;
    ae_sns_dft->full_lines_std        = gc8613_ctx->fl_std;
    ae_sns_dft->full_lines            = gc8613_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time          = gc8613_ctx->fl[SENSOR_CUR_FRAME] - GC8613_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]- gc8613 set fps:%f.\n", dev, fps);

    return XMEDIA_SUCCESS;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 gc8613_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    xmedia_u32      short_max_pre_frame;
    xmedia_u32      short_max;
    xmedia_u32      short_time_min_limit;
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);
    memcpy(gc8613_ctx->ratio, inttime_attr->ratio, sizeof(inttime_attr->ratio));

    /*
     * 0x40 is EXP-1X
     * Ratio = L / S * 0x40;
     * VTS   = L + S + EXP_OFFSET;
     * --> S = (L * 0x40) / Ratio
     * --> S = [(VTS - EXP_OFFSET) * 0x40] / (Ratio + 0x40);
     */
    short_time_min_limit = 2;
    short_max            = 2;
    if (gc8613_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (gc8613_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
            short_max_pre_frame = gc8613_ctx->fl[SENSOR_PRE_FRAME] - gc8613_ctx->wdr_int_time[0] - GC8613_EXP_OFFSET;
            short_max           = gc8613_ctx->fl[SENSOR_CUR_FRAME] - GC8613_EXP_OFFSET;
            short_max           = (short_max_pre_frame < short_max) ? short_max_pre_frame : short_max;
            inttime_attr->max_inttime[0] = short_time_min_limit; // 0：short frame
            inttime_attr->min_inttime[0] = short_time_min_limit;
            inttime_attr->max_inttime[1] = short_max;            // 1：long frame
            inttime_attr->min_inttime[1] = short_time_min_limit;
            return XMEDIA_SUCCESS;
        } else {
            short_max_pre_frame =
                ((gc8613_ctx->fl[SENSOR_PRE_FRAME] - GC8613_EXP_OFFSET - gc8613_ctx->wdr_int_time[0]) * 0x40) /
                SENSOR_DIV_0_TO_1(gc8613_ctx->ratio[0]);
            short_max =
                ((gc8613_ctx->fl[SENSOR_CUR_FRAME] - GC8613_EXP_OFFSET) * 0x40) / (gc8613_ctx->ratio[0] + 0x40);
            short_max = (short_max_pre_frame < short_max) ? short_max_pre_frame : short_max;
            short_max = (short_max == 0) ? 1 : short_max;
        }
    }

    if (short_max >= short_time_min_limit) { // 0：short frame; 1：long frame
        inttime_attr->max_inttime[0] = short_max;
        inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * gc8613_ctx->ratio[0]) >> 6;
        inttime_attr->min_inttime[0] = short_time_min_limit;
        inttime_attr->min_inttime[1] = (inttime_attr->min_inttime[0] * gc8613_ctx->ratio[0]) >> 6;
    } else {
        short_max                    = short_time_min_limit;
        inttime_attr->max_inttime[0] = short_max;
        inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * 0xFFF) >> 6; // shift 6
        inttime_attr->min_inttime[0] = inttime_attr->max_inttime[0];
        inttime_attr->min_inttime[1] = inttime_attr->max_inttime[1];
    }

    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines: AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines: AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 */
static xmedia_s32 gc8613_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context  *gc8613_ctx = XMEDIA_NULL;
    xmedia_u32       vmax_min_fps;
    xmedia_s32       ret;
    xmedia_float     min_fps;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    if (gc8613_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", gc8613_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = gc8613_get_min_fps(gc8613_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc8613_get_min_fps_vmax(gc8613_ctx->img_mode, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                        = full_lines + GC8613_EXP_OFFSET;
    full_lines                        = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    gc8613_ctx->fl[SENSOR_CUR_FRAME]  = full_lines;
    gc8613_ctx->fps                   = min_fps * vmax_min_fps / full_lines;

    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_VMAX_H].data = ((full_lines&0x3f00) >> 8);
    gc8613_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[GC8613_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    ae_sns_dft->full_lines   = gc8613_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = gc8613_ctx->fl[SENSOR_CUR_FRAME] - GC8613_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]- gc8613 set fps:%f.\n", dev, gc8613_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    ret = gc8613_get_min_fps(gc8613_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = gc8613_get_max_fps(gc8613_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = gc8613_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 gc8613_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, GC8613_NAME, sizeof(GC8613_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = gc8613_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = gc8613_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = gc8613_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = gc8613_mirror;
    info->isp_func.pfn_sensor_flip             = gc8613_flip;
    info->isp_func.pfn_sensor_set_bus_info     = gc8613_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = gc8613_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = gc8613_set_init_param;
    info->isp_func.pfn_sensor_start            = gc8613_start;
    info->isp_func.pfn_sensor_stop             = gc8613_stop;
    info->isp_func.pfn_sensor_standby          = gc8613_standby;
    info->isp_func.pfn_sensor_resume           = gc8613_resume;
    info->isp_func.pfn_sensor_write_reg        = gc8613_write_reg;
    info->isp_func.pfn_sensor_read_reg         = gc8613_read_reg;
    info->isp_func.pfn_sensor_init             = gc8613_init;
    info->isp_func.pfn_sensor_exit             = gc8613_exit;

    // called by ISP
    info->isp_func.pfn_sensor_set_attr            = gc8613_set_attr;
    info->isp_func.pfn_sensor_get_isp_default     = gc8613_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = gc8613_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = gc8613_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = gc8613_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = gc8613_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = gc8613_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = gc8613_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = gc8613_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = gc8613_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = gc8613_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = gc8613_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = gc8613_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = gc8613_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = gc8613_calc_dgain;   // not support
    info->ae_func.pfn_sensor_update_gains       = gc8613_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = gc8613_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = gc8613_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 gc8613_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = gc8613_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = gc8613_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = gc8613_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "GC8613 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 gc8613_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *gc8613_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = gc8613_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    GC8613_GET_CTX(dev, gc8613_ctx);
    SENSOR_CHECK_PTR_RETURN(gc8613_ctx);

    if (gc8613_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "GC8613 unregister function failed!\n");
        return ret;
    }

    gc8613_ctx_exit(dev);
    g_gc8613_dev_map[dev] = SENSOR_DEV_INVALID; // reset dev

    return XMEDIA_SUCCESS;
}
