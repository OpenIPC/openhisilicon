#include <stdlib.h>
#include <string.h>
#include "jxf37p.h"
#include "jxf37p_ctrl.h"
#include "jxf37p_ex.h"
#include "xmedia_isp.h"

#define JXF37P_NAME          "JXF37P"
#define JXF37P_SPECS_MAX_NUM 1

#ifdef __linux__
#define JXF37P_ISP_DEFAULT_SUPPORT
#endif

#define JXF37P_HMAX_2M_1920x1080_LINEAR 0x500
#define JXF37P_2M_1080P_BIT_RATE_LINEAR 432

#define JXF37P_REG_ADDR_STANDBY 0x12 // 数据手册里没找到standby，只看到 0x12[6] sleep mode

#define JXF37P_RES_IS_1080P(w, h) ((w) == 1920 && (h) == 1080)

#define JXF37P_ERR_MODE_PRINT(sns_attr)                                                                                \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sns_attr->width,                \
                     sns_attr->height, sns_attr->wdr_mode);                                                            \
    } while (0)

// awb static param for Fuji Lens New IR_Cut
#define JXF37P_CALIBRATE_STATIC_TEMP       5000
#define JXF37P_CALIBRATE_STATIC_WB_R_GAIN  369
#define JXF37P_CALIBRATE_STATIC_WB_GR_GAIN 256
#define JXF37P_CALIBRATE_STATIC_WB_GB_GAIN 256
#define JXF37P_CALIBRATE_STATIC_WB_B_GAIN  660

// Calibration results for Auto WB Planck
#define JXF37P_CALIBRATE_AWB_P1 108
#define JXF37P_CALIBRATE_AWB_P2 8
#define JXF37P_CALIBRATE_AWB_Q1 (-140)
#define JXF37P_CALIBRATE_AWB_A1 219225
#define JXF37P_CALIBRATE_AWB_B1 128
#define JXF37P_CALIBRATE_AWB_C1 (-152450)

// Rgain and Bgain of the golden sample
#define JXF37P_GOLDEN_RGAIN 0
#define JXF37P_GOLDEN_BGAIN 0

#define JXF37P_GAIN_INDEX_MAX 64

sensor_context *g_jxf37p_ctx[XMEDIA_SENSOR_DEV_MAX_NUM]    = { XMEDIA_NULL };
#define JXF37P_SET_CTX(dev, sns_ctx) g_jxf37p_ctx[dev] = sns_ctx
#define JXF37P_GET_CTX(dev, sns_ctx) sns_ctx = g_jxf37p_ctx[dev]

static xmedia_u32 g_jxf37p_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_jxf37p_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_jxf37p_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_jxf37p_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };
static xmedia_s32 g_jxf37p_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

static const xmedia_sensor_property g_jxf37p_property[JXF37P_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate
        { 0 }, JXF37P_2M_1080P_BIT_RATE_LINEAR,
    },
};

static const xmedia_u32 g_jxf37p_gain_table[JXF37P_GAIN_INDEX_MAX] = {
    1024, 1088, 1152, 1216, 1280,  1344,  1408,  1472,  1536,  1600,  1664,  1728,  1792,  1856,  1920,  1984,
    2048, 2176, 2304, 2432, 2560,  2688,  2816,  2944,  3072,  3200,  3328,  3456,  3584,  3712,  3840,  3968,
    4096, 4352, 4608, 4864, 5120,  5376,  5632,  5888,  6144,  6400,  6656,  6912,  7168,  7424,  7680,  7936,
    8192, 8704, 9216, 9728, 10240, 10752, 11264, 11776, 12288, 12800, 13312, 13824, 14336, 14848, 15360, 15872
};

static const xmedia_sensor_capability g_jxf37p_capability = {
    .max_width   = 1920,
    .max_height  = 1080,
    .max_fps     = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_NONE,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported  = XMEDIA_FALSE,
    .mipi_lanes_supported  = XMEDIA_SENSOR_MIPI_LANES_2L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = JXF37P_ADDR_BYTE,
        .data_byte_num     = JXF37P_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { JXF37P_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x40 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { JXF37P_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x00 },
    },
};

static xmedia_s32 jxf37p_ctx_init(xmedia_u32 dev)
{
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    JXF37P_GET_CTX(dev, jxf37p_ctx);
    if (jxf37p_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    jxf37p_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (jxf37p_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(jxf37p_ctx, 0, sizeof(sensor_context));
    jxf37p_ctx->i2c_addr         = JXF37P_I2C_ADDR;
    jxf37p_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_2L;
    jxf37p_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    jxf37p_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    jxf37p_ctx->size.width       = 1920;
    jxf37p_ctx->size.height      = 1080;
    jxf37p_ctx->fps              = 30;
    jxf37p_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    jxf37p_ctx->img_mode             = JXF37P_2LANE_1920X1080_LINEAR_MODE;
    jxf37p_ctx->fl_std               = JXF37P_VMAX_2M_1920x1080_LINEAR << JXF37P_EXP_SHIFT;
    jxf37p_ctx->fl[SENSOR_CUR_FRAME] = JXF37P_VMAX_2M_1920x1080_LINEAR << JXF37P_EXP_SHIFT;
    jxf37p_ctx->fl[SENSOR_PRE_FRAME] = JXF37P_VMAX_2M_1920x1080_LINEAR << JXF37P_EXP_SHIFT;

    JXF37P_SET_CTX(dev, jxf37p_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void jxf37p_ctx_exit(xmedia_u32 dev)
{
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_FREE(jxf37p_ctx);
    JXF37P_SET_CTX(dev, XMEDIA_NULL);
}

static xmedia_s32 jxf37p_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    for (i = 0; i < JXF37P_SPECS_MAX_NUM; i++) {
        if (g_jxf37p_property[i].width == jxf37p_ctx->size.width &&
            g_jxf37p_property[i].height == jxf37p_ctx->size.height &&
            g_jxf37p_property[i].wdr_mode == jxf37p_ctx->wdr_mode) {
            memcpy(property, &g_jxf37p_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n", jxf37p_ctx->size.width,
                 jxf37p_ctx->size.height, jxf37p_ctx->wdr_mode);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 jxf37p_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL && work_mode != XMEDIA_SENSOR_WORK_MODE_SLAVE) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    jxf37p_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "No support mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    jxf37p_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        jxf37p_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        jxf37p_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        jxf37p_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 jxf37p_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    jxf37p_ctx->i2c_addr = slave_addr;
    ret                  = jxf37p_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    *slave_addr = jxf37p_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    jxf37p_ctx->init_mode = init_mode;

    ret = jxf37p_i2c_init(dev, jxf37p_ctx->bus_info.i2c_dev, jxf37p_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    jxf37p_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    ret = jxf37p_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    jxf37p_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    jxf37p_ctx->mirror_en = mirror_en;
    ret                   = jxf37p_set_mirror_flip(dev, mirror_en, jxf37p_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    jxf37p_ctx->flip_en = flip_en;
    ret                 = jxf37p_set_mirror_flip(dev, jxf37p_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_set_image_mode(sensor_context *jxf37p_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (jxf37p_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (JXF37P_RES_IS_1080P(sns_attr->width, sns_attr->height)) {
            *image_mode = JXF37P_2LANE_1920X1080_LINEAR_MODE;
            if (jxf37p_ctx->work_mode == XMEDIA_SENSOR_WORK_MODE_NORMAL) {
                jxf37p_ctx->fl_std = JXF37P_VMAX_2M_1920x1080_LINEAR << JXF37P_EXP_SHIFT;
            } else {
                SENSOR_TRACE(MODULE_DBG_ERR, "Not support work mode: %d\n", jxf37p_ctx->work_mode);
                return XMEDIA_ERRCODE_NOT_SUPPORT;
            }
        } else {
            JXF37P_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        JXF37P_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    jxf37p_ctx->size.width  = sns_attr->width;
    jxf37p_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            jxf37p_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(jxf37p_ctx->wdr_int_time, 0, sizeof(jxf37p_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

static xmedia_void jxf37p_init_common_reg_info(sensor_context *jxf37p_ctx)
{
    jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_EXP_H].delay_frame_num = 2;
    jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_EXP_H].reg_addr        = JXF37P_REG_ADDR_EXP_H;
    jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_EXP_L].delay_frame_num = 2;
    jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_EXP_L].reg_addr        = JXF37P_REG_ADDR_EXP_L;

    jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_GAIN].delay_frame_num = 2;
    jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_GAIN].reg_addr        = JXF37P_REG_ADDR_GAIN;

    jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_VMAX_H].delay_frame_num = 1;
    jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_VMAX_H].reg_addr        = JXF37P_REG_ADDR_VMAX_H;
    jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_VMAX_L].delay_frame_num = 1;
    jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_VMAX_L].reg_addr        = JXF37P_REG_ADDR_VMAX_L;

    return;
}

static xmedia_s32 jxf37p_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = jxf37p_ctx->bus_info.i2c_dev;
    jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = JXF37P_REG_MAX_NUM;

    for (i = 0; i < jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = jxf37p_ctx->i2c_addr;
        jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = JXF37P_ADDR_BYTE;
        jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = JXF37P_DATA_BYTE;
    }

    // init general register - The registers should init both in linear
    jxf37p_init_common_reg_info(jxf37p_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    // Set wdr mode
    ret = jxf37p_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = jxf37p_set_image_mode(jxf37p_ctx, &jxf37p_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    jxf37p_ctx->fl[SENSOR_CUR_FRAME] = jxf37p_ctx->fl_std;
    jxf37p_ctx->fl[SENSOR_PRE_FRAME] = jxf37p_ctx->fl[SENSOR_CUR_FRAME];

    ret = jxf37p_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef JXF37P_ISP_DEFAULT_SUPPORT
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from jxf37p!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 jxf37p_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (jxf37p_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 64;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 64;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 64;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 64;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    if (jxf37p_ctx->mirror_en && jxf37p_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (jxf37p_ctx->mirror_en && (!jxf37p_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!jxf37p_ctx->mirror_en) && jxf37p_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < JXF37P_SPECS_MAX_NUM; i++) {
        if (g_jxf37p_property[i].width == jxf37p_ctx->size.width &&
            g_jxf37p_property[i].height == jxf37p_ctx->size.height &&
            g_jxf37p_property[i].wdr_mode == jxf37p_ctx->wdr_mode) {
            *bayer_pattern = g_jxf37p_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= JXF37P_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_jxf37p_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_get_trig_attr(xmedia_sensor_trig_attr *trig_attr)
{
    SENSOR_CHECK_PTR_RETURN(trig_attr);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    jxf37p_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 jxf37p_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_jxf37p_exposure[dev]      = init_param->exposure;
    g_jxf37p_sample_r_gain[dev] = init_param->sample_rgain;
    g_jxf37p_sample_b_gain[dev] = init_param->sample_bgain;

    g_jxf37p_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_jxf37p_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_jxf37p_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256;
    ae_sns_dft->full_lines_max = JXF37P_FULL_LINES_MAX;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy  = 1;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 4 << ae_sns_dft->ispdgain_shift;

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

static xmedia_void jxf37p_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 15872;
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 1;
    ae_sns_dft->min_dgain        = 1;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_jxf37p_exposure[dev] ? g_jxf37p_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - JXF37P_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 1;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = 1;

    return;
}

static xmedia_s32 jxf37p_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = jxf37p_get_ae_common_default(dev, jxf37p_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (jxf37p_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            jxf37p_get_ae_linear_default(dev, jxf37p_ctx, ae_sns_dft);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", jxf37p_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_EXP_H].data = SENSOR_HIGH_8BITS(int_time);
    jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_EXP_L].data = SENSOR_LOW_8BITS(int_time);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32       i;
    static xmedia_u8 againmax = JXF37P_GAIN_INDEX_MAX - 1;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_jxf37p_gain_table[againmax]) {
        *again_db = againmax;
    } else {
        for (i = 1; i < JXF37P_GAIN_INDEX_MAX; i++) {
            if (*again_lin < g_jxf37p_gain_table[i]) {
                *again_db = i - 1;
                break;
            }
        }
    }

    *again_lin = g_jxf37p_gain_table[*again_db];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_GAIN].data = SENSOR_LOW_8BITS(again);

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 jxf37p_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    // awb_sns_dft->wb_ref_temp = JXF37P_CALIBRATE_STATIC_TEMP;

    // awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = JXF37P_CALIBRATE_STATIC_WB_R_GAIN;
    // awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = JXF37P_CALIBRATE_STATIC_WB_GR_GAIN;
    // awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = JXF37P_CALIBRATE_STATIC_WB_GB_GAIN;
    // awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = JXF37P_CALIBRATE_STATIC_WB_B_GAIN;

    // // 0 ~ 5: point index on the awb curve param
    // awb_sns_dft->wb_para[0] = JXF37P_CALIBRATE_AWB_P1;
    // awb_sns_dft->wb_para[1] = JXF37P_CALIBRATE_AWB_P2;
    // awb_sns_dft->wb_para[2] = JXF37P_CALIBRATE_AWB_Q1;
    // awb_sns_dft->wb_para[3] = JXF37P_CALIBRATE_AWB_A1;
    // awb_sns_dft->wb_para[4] = JXF37P_CALIBRATE_AWB_B1;
    // awb_sns_dft->wb_para[5] = JXF37P_CALIBRATE_AWB_C1;

    awb_sns_dft->golden_rgain    = JXF37P_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain    = JXF37P_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (jxf37p_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_jxf37p_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_jxf37p_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", jxf37p_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_jxf37p_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_jxf37p_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_jxf37p_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_jxf37p_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_jxf37p_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

xmedia_s32 jxf37p_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case JXF37P_2LANE_1920X1080_LINEAR_MODE:
            *max_fps = 30.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_standby(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = jxf37p_write_reg(dev, JXF37P_REG_ADDR_STANDBY, 0x40);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_resume(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = jxf37p_write_reg(dev, JXF37P_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *jxf37p_ctx   = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= jxf37p_write_reg(dev, jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    if (jxf37p_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = jxf37p_init_image(dev, jxf37p_ctx->img_mode, jxf37p_ctx->work_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = jxf37p_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = jxf37p_set_mirror_flip(dev, jxf37p_ctx->mirror_en, jxf37p_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = jxf37p_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_jxf37p_dev_map[index] == SENSOR_DEV_INVALID) {
            g_jxf37p_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 jxf37p_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_jxf37p_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 jxf37p_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{
    switch (img_mode) {
        case JXF37P_2LANE_1920X1080_LINEAR_MODE:
            *min_fps = 0.515;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax, xmedia_sensor_work_mode work_mode)
{
    switch (img_mode) {
        case JXF37P_2LANE_1920X1080_LINEAR_MODE:
            if (work_mode == XMEDIA_SENSOR_WORK_MODE_NORMAL) {
                *vmax = JXF37P_VMAX_2M_1920x1080_LINEAR;
            } else {
                SENSOR_TRACE(MODULE_DBG_ERR, "Not support work mode: %d\n", work_mode);
                return XMEDIA_ERRCODE_NOT_SUPPORT;
            }
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = jxf37p_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = jxf37p_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = jxf37p_get_vmax(sns_ctx->img_mode, &vmax, sns_ctx->work_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is (%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, JXF37P_FULL_LINES_MAX);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    for (i = 0; i < jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            jxf37p_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &jxf37p_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&jxf37p_ctx->regs_info[SENSOR_PRE_FRAME], &jxf37p_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    jxf37p_ctx->fl[SENSOR_PRE_FRAME] = jxf37p_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    ret = jxf37p_calc_fps(fps, jxf37p_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    if (jxf37p_ctx->work_mode == XMEDIA_SENSOR_WORK_MODE_SLAVE) {
        full_lines -= 1; // 从sensor的vmax必须是主sensor的vmax-1，这个是switch场景的要求
        jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
        jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);
    } else {
        jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
        jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);
    }
    jxf37p_ctx->fl[SENSOR_CUR_FRAME] = full_lines << JXF37P_EXP_SHIFT;
    jxf37p_ctx->fl_std               = jxf37p_ctx->fl[SENSOR_CUR_FRAME];
    jxf37p_ctx->fps                  = fps;

    ae_sns_dft->fps            = fps;
    ae_sns_dft->full_lines_std = jxf37p_ctx->fl_std;
    ae_sns_dft->full_lines     = jxf37p_ctx->fl_std;

    if (jxf37p_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = jxf37p_ctx->fl[SENSOR_CUR_FRAME] - JXF37P_EXP_OFFSET_LINEAR;
    }

    SENSOR_PRINT("dev[%d]- sc200ai set fps: %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax,
                                                 xmedia_sensor_work_mode work_mode)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = jxf37p_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = jxf37p_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = jxf37p_get_vmax(img_mode, &vmax_max_fps, work_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = (vmax_max_fps << JXF37P_EXP_SHIFT) * max_fps / min_fps;

    return XMEDIA_SUCCESS;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 jxf37p_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 * 注意：2to1 wdr模式下，曝光比和ae wdr mode 由AE传入，调用本接口前，AE需先调用jxf37p_get_max_inttime
 */
static xmedia_s32 jxf37p_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *jxf37p_ctx = XMEDIA_NULL;
    xmedia_u32      vmax_min_fps;
    xmedia_s32      ret;
    xmedia_float    min_fps;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    if (jxf37p_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", jxf37p_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    full_lines = full_lines + JXF37P_EXP_OFFSET_LINEAR;

    ret = jxf37p_get_min_fps_vmax(jxf37p_ctx->img_mode, &vmax_min_fps, jxf37p_ctx->work_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = jxf37p_get_min_fps(jxf37p_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                       = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    jxf37p_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_VMAX_H].data =
                                                                    SENSOR_HIGH_8BITS(full_lines >> JXF37P_EXP_SHIFT);
    jxf37p_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[JXF37P_REG_VMAX_L].data =
                                                                    SENSOR_LOW_8BITS(full_lines >> JXF37P_EXP_SHIFT);

    ae_sns_dft->full_lines   = jxf37p_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = jxf37p_ctx->fl[SENSOR_CUR_FRAME] - JXF37P_EXP_OFFSET_LINEAR;

    jxf37p_ctx->fps = min_fps * vmax_min_fps / full_lines;
    SENSOR_PRINT("sensor fps: %f\n", jxf37p_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_get_hmax_vmax(xmedia_u32 dev, xmedia_u32 *hmax, xmedia_u32 *vmax)
{
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(hmax);
    SENSOR_CHECK_PTR_RETURN(vmax);

    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    *vmax = jxf37p_ctx->fl[SENSOR_CUR_FRAME] >> JXF37P_EXP_SHIFT;

    switch (jxf37p_ctx->img_mode) {
        case JXF37P_2LANE_1920X1080_LINEAR_MODE:
            if (jxf37p_ctx->work_mode == XMEDIA_SENSOR_WORK_MODE_NORMAL) {
                *hmax = JXF37P_HMAX_2M_1920x1080_LINEAR; // TODO: 待确认寄存器值是否就是真正的hmax
            } else {
                SENSOR_TRACE(MODULE_DBG_ERR, "Not support work mode: %d\n", jxf37p_ctx->work_mode);
                return XMEDIA_ERRCODE_NOT_SUPPORT;
            }
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    ret = jxf37p_get_min_fps(jxf37p_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = jxf37p_get_max_fps(jxf37p_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = jxf37p_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 jxf37p_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, JXF37P_NAME, sizeof(JXF37P_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = jxf37p_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = jxf37p_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = jxf37p_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = jxf37p_mirror;
    info->isp_func.pfn_sensor_flip             = jxf37p_flip;
    info->isp_func.pfn_sensor_set_bus_info     = jxf37p_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = jxf37p_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = jxf37p_set_init_param;
    info->isp_func.pfn_sensor_start            = jxf37p_start;
    info->isp_func.pfn_sensor_stop             = jxf37p_stop;
    info->isp_func.pfn_sensor_standby          = jxf37p_standby;
    info->isp_func.pfn_sensor_resume           = jxf37p_resume;
    info->isp_func.pfn_sensor_write_reg        = jxf37p_write_reg;
    info->isp_func.pfn_sensor_read_reg         = jxf37p_read_reg;
    info->isp_func.pfn_sensor_init             = jxf37p_init;
    info->isp_func.pfn_sensor_exit             = jxf37p_exit;
    info->isp_func.pfn_sensor_set_attr         = jxf37p_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = jxf37p_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = jxf37p_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = jxf37p_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = jxf37p_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = jxf37p_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = jxf37p_get_dev_addr;
    info->isp_func.pfn_sensor_get_trig_attr       = jxf37p_get_trig_attr;
    info->isp_func.pfn_sensor_get_hmax_vmax       = jxf37p_get_hmax_vmax;
    info->isp_func.pfn_sensor_get_fps             = jxf37p_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = jxf37p_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = jxf37p_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = jxf37p_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = jxf37p_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = jxf37p_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = jxf37p_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = jxf37p_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = jxf37p_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = jxf37p_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = jxf37p_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 jxf37p_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = jxf37p_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = jxf37p_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = jxf37p_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "JXF37P register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 jxf37p_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *jxf37p_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = jxf37p_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    JXF37P_GET_CTX(dev, jxf37p_ctx);
    SENSOR_CHECK_PTR_RETURN(jxf37p_ctx);

    if (jxf37p_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "JXF37P unregister function failed!\n");
        return ret;
    }

    jxf37p_ctx_exit(dev);
    g_jxf37p_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;
}
