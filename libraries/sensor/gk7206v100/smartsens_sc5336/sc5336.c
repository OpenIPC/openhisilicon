#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sc5336.h"
#include "sc5336_ctrl.h"
#include "sc5336_ex.h"

#define SC5336_NAME          "SC5336"
#define SC5336_SPECS_MAX_NUM 1

#define SC5336_5M_BIT_RATE_LINEAR  1500

#ifdef __linux__
//#define SC5336_ISP_DEFAULT_SUPPORT
#endif

#define SC5336_REG_ADDR_STANDBY 0x0100

#define SC5336_RES_IS_1620P(w, h) ((w) == 2880 && (h) == 1620)

#define SC5336_ERR_MODE_PRINT(sns_attr)                                                                                \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sns_attr->width,                \
                     sns_attr->height, sns_attr->wdr_mode);                                                            \
    } while (0)

// awb static param for Fuji Lens New IR_Cut
#define SC5336_CALIBRATE_STATIC_TEMP       5000
#define SC5336_CALIBRATE_STATIC_WB_R_GAIN  360
#define SC5336_CALIBRATE_STATIC_WB_GR_GAIN 256
#define SC5336_CALIBRATE_STATIC_WB_GB_GAIN 256
#define SC5336_CALIBRATE_STATIC_WB_B_GAIN  578

// Calibration results for Auto WB Planck
#define SC5336_CALIBRATE_AWB_P1 130
#define SC5336_CALIBRATE_AWB_P2 -10
#define SC5336_CALIBRATE_AWB_Q1 -136
#define SC5336_CALIBRATE_AWB_A1 184237
#define SC5336_CALIBRATE_AWB_B1 128
#define SC5336_CALIBRATE_AWB_C1 -122059

// Rgain and Bgain of the golden sample
#define SC5336_GOLDEN_RGAIN 0
#define SC5336_GOLDEN_BGAIN 0

static sensor_context *g_sc5336_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define SC5336_GET_CTX(dev, ctx) ctx = g_sc5336_ctx[dev]
#define SC5336_SET_CTX(dev, ctx) g_sc5336_ctx[dev] = ctx

static xmedia_s32 g_sc5336_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] =
                                                                      SENSOR_DEV_INVALID };

static xmedia_u32 g_sc5336_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_sc5336_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_sc5336_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_sc5336_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

static xmedia_float again_real_cal[] = {128, 1, 256, 2, 512, 4, 1024, 8, 2048, 16, 4096, 32};
static xmedia_float again_reg_table[] = {128, 0x00, 256, 0x08, 512, 0x09, 1024, 0x0b, 2048, 0x0f, 4096, 0x1f};

static const xmedia_sensor_property g_sc5336_property[SC5336_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        2880, 1620, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB,
          XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC5336_5M_BIT_RATE_LINEAR,
     },
};

#define SC5336_AGAIN_INDEX_MAX  64
static const xmedia_u32 g_sc5336_again_table[SC5336_AGAIN_INDEX_MAX] = {
    1024, 1088, 1152, 1216, 1280, 1344, 1408, 1472, 1536, 1600, 1664, 1728, 1792, 1856,
    1920, 1984, 2048, 2176, 2304, 2432, 2560, 2688, 2816, 2944, 3072, 3200, 3328, 3456,
    3584, 3712, 3840, 3968, 4096, 4352, 4608, 4864, 5120, 5376, 5632, 5888, 6144, 6400,
    6656, 6912, 7168, 7424, 7680, 7936, 8192, 8704, 9216, 9728, 10240, 10752, 11264,
    11776, 12288, 12800, 13312, 13824, 14336, 14848, 15360, 15872
};

static const xmedia_sensor_capability g_sc5336_capability = {
    .max_width   = 2880,
    .max_height  = 1620,
    .max_fps     = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_2L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = SC5336_ADDR_BYTE,
        .data_byte_num     = SC5336_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { SC5336_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { SC5336_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x01 },
    },
};

static xmedia_s32 sc5336_ctx_init(xmedia_u32 dev)
{
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SC5336_GET_CTX(dev, sc5336_ctx);
    if (sc5336_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    sc5336_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (sc5336_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(sc5336_ctx, 0, sizeof(sensor_context));
    sc5336_ctx->i2c_addr             = SC5336_I2C_ADDR;
    sc5336_ctx->lanes                = XMEDIA_SENSOR_MIPI_LANES_2L;
    sc5336_ctx->work_mode            = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    sc5336_ctx->bus_info.i2c_dev     = SENSOR_I2C_INVALID;
    sc5336_ctx->size.width           = g_sc5336_property[0].width;
    sc5336_ctx->size.height          = g_sc5336_property[0].height;
    sc5336_ctx->fps                  = g_sc5336_property[0].max_fps;
    sc5336_ctx->wdr_mode             = g_sc5336_property[0].wdr_mode;
    sc5336_ctx->img_mode             = SC5336_5M_10BIT_LINEAR_MODE;
    sc5336_ctx->fl_std               = SC5336_VMAX_5M_LINEAR;
    sc5336_ctx->fl[SENSOR_CUR_FRAME] = SC5336_VMAX_5M_LINEAR;
    sc5336_ctx->fl[SENSOR_PRE_FRAME] = SC5336_VMAX_5M_LINEAR;
    SC5336_SET_CTX(dev, sc5336_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void sc5336_ctx_exit(xmedia_u32 dev)
{
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_FREE(sc5336_ctx);
    SC5336_SET_CTX(dev, XMEDIA_NULL);

    return;
}

static xmedia_s32 sc5336_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    for (i = 0; i < SC5336_SPECS_MAX_NUM; i++) {
        if (g_sc5336_property[i].width == sc5336_ctx->size.width &&
            g_sc5336_property[i].height == sc5336_ctx->size.height &&
            g_sc5336_property[i].wdr_mode == sc5336_ctx->wdr_mode) {
            memcpy(property, &g_sc5336_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Please check input param!\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 sc5336_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc5336_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Invalid mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc5336_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        sc5336_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        sc5336_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        sc5336_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 sc5336_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    sc5336_ctx->i2c_addr = slave_addr;
    ret = sc5336_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    *slave_addr = sc5336_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

// called by ISP
static xmedia_s32 sc5336_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    sc5336_ctx->init_mode = init_mode;
    ret = sc5336_i2c_init(dev, sc5336_ctx->bus_info.i2c_dev, sc5336_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    sc5336_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    ret = sc5336_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    sc5336_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    sc5336_ctx->mirror_en = mirror_en;
    ret = sc5336_set_mirror_flip(dev, mirror_en, sc5336_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    sc5336_ctx->flip_en = flip_en;
    ret = sc5336_set_mirror_flip(dev, sc5336_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_set_image_mode(sensor_context *sc5336_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (sc5336_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (SC5336_RES_IS_1620P(sns_attr->width, sns_attr->height)) {
            *image_mode        = SC5336_5M_10BIT_LINEAR_MODE;
            sc5336_ctx->fl_std = SC5336_VMAX_5M_LINEAR;
        } else {
            SC5336_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        SC5336_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc5336_ctx->size.width  = sns_attr->width;
    sc5336_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            sc5336_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(sc5336_ctx->wdr_int_time, 0, sizeof(sc5336_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

static xmedia_void sc5336_init_common_reg_info(sensor_context *sc5336_ctx)
{
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_EXP_H].delay_frame_num = 2;
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_EXP_H].reg_addr        = SC5336_REG_ADDR_EXP_H;
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_EXP_M].delay_frame_num = 2;
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_EXP_M].reg_addr        = SC5336_REG_ADDR_EXP_M;
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_EXP_L].delay_frame_num = 2;
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_EXP_L].reg_addr        = SC5336_REG_ADDR_EXP_L;

    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_AGAIN_H].delay_frame_num = 2;
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_AGAIN_H].reg_addr        = SC5336_REG_ADDR_AGAIN_H;
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_AGAIN_L].delay_frame_num = 2;
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_AGAIN_L].reg_addr        = SC5336_REG_ADDR_AGAIN_L;

    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_DGAIN_H].delay_frame_num = 2;
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_DGAIN_H].reg_addr        = SC5336_REG_ADDR_DGAIN_H;
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_DGAIN_L].delay_frame_num = 2;
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_DGAIN_L].reg_addr        = SC5336_REG_ADDR_DGAIN_L;

    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_VMAX_H].delay_frame_num = 2;
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_VMAX_H].reg_addr        = SC5336_REG_ADDR_VMAX_H;
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_VMAX_L].delay_frame_num = 2;
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_VMAX_L].reg_addr        = SC5336_REG_ADDR_VMAX_L;

    return;
}

static xmedia_s32 sc5336_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = sc5336_ctx->bus_info.i2c_dev;
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = SC5336_REG_L_MAX_NUM;

    for (i = 0; i < sc5336_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = sc5336_ctx->i2c_addr;
        sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = SC5336_ADDR_BYTE;
        sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = SC5336_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    sc5336_init_common_reg_info(sc5336_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    // Set wdr mode
    ret = sc5336_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = sc5336_set_image_mode(sc5336_ctx, &sc5336_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    sc5336_ctx->fl[SENSOR_CUR_FRAME] = sc5336_ctx->fl_std;
    sc5336_ctx->fl[SENSOR_PRE_FRAME] = sc5336_ctx->fl[SENSOR_CUR_FRAME];

    ret = sc5336_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef SC5336_ISP_DEFAULT_SUPPORT
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from sc5336!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 sc5336_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x100;
    black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x100;
    black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x100;
    black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x100;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    if (sc5336_ctx->mirror_en && sc5336_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (sc5336_ctx->mirror_en && (!sc5336_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!sc5336_ctx->mirror_en) && sc5336_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < SC5336_SPECS_MAX_NUM; i++) {
        if (g_sc5336_property[i].width == sc5336_ctx->size.width &&
            g_sc5336_property[i].height == sc5336_ctx->size.height &&
            g_sc5336_property[i].wdr_mode == sc5336_ctx->wdr_mode) {
            *bayer_pattern = g_sc5336_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= SC5336_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);
    memcpy(capability, &g_sc5336_capability, sizeof(xmedia_sensor_capability));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc5336_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 sc5336_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_sc5336_exposure[dev]      = init_param->exposure;
    g_sc5336_sample_r_gain[dev] = init_param->sample_rgain;
    g_sc5336_sample_b_gain[dev] = init_param->sample_bgain;

    g_sc5336_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_sc5336_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_sc5336_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case SC5336_5M_10BIT_LINEAR_MODE:
            *max_fps = 30.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = sc5336_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256;
    ae_sns_dft->full_lines_max = SC5336_FULL_LINES_MAX;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->again_accu.accuracy  = 0.0078125; // 1.0 / 128

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy  = 0.0078125; // 1.0 / 128

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 4 << ae_sns_dft->ispdgain_shift;

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

static xmedia_void sc5336_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 4096;
    ae_sns_dft->min_again        = 128;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 128 * 3.969;
    ae_sns_dft->min_dgain        = 128;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_sc5336_exposure[dev] ? g_sc5336_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - SC5336_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 1;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = 1;

    return;
}

static xmedia_s32 sc5336_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = sc5336_get_ae_common_default(dev, sc5336_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (sc5336_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            sc5336_get_ae_linear_default(dev, sc5336_ctx, ae_sns_dft);
            break;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context *sc5336_ctx = XMEDIA_NULL;
    xmedia_u32 reg_val;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    reg_val = SENSOR_MIN((SC5336_FULL_LINES_MAX - SC5336_EXP_OFFSET_LINEAR), int_time);

    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_EXP_H].data = SENSOR_HIGHER_4BITS(reg_val);
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_EXP_M].data = SENSOR_MIDDLE_8BITS(reg_val);
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_EXP_L].data = SENSOR_LOWER_4BITS(reg_val);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;
    xmedia_u32 again;
    static xmedia_u8 againmax = SC5336_AGAIN_INDEX_MAX - 1;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    again = *again_lin;

    if (again >= g_sc5336_again_table[againmax]) {
        *again_db = againmax;
    } else {
        for (i = 1; i < againmax; i++) {
            if (again < g_sc5336_again_table[i]) {
                *again_db = i - 1;
                break;
            }
        }
    }
    *again_lin = g_sc5336_again_table[*again_db];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(dgain_lin);
    SENSOR_CHECK_PTR_RETURN(dgain_db);

    *dgain_db = *dgain_lin / 1024; // precison 1024

    if (*dgain_db == 3) { // 3
        *dgain_db = 2; // 2x
    } else if (*dgain_db >= 4 && *dgain_db < 8) { // 4 < gain < 8
        *dgain_db = 4; // 4x
    } else if (*dgain_db >= 8) { // 8 < gain
        *dgain_db = 8; // 8x
    }

    *dgain_lin = *dgain_db * 1024; // precison 1024

    return XMEDIA_SUCCESS;
}

static xmedia_u16 sc5336_dgain_mapping_realgain_to_sensorgain(xmedia_float gain_real)
{
    xmedia_u16 sensor_gain;
    xmedia_u32 dgain = gain_real * 128;
    xmedia_u8 dcoarse_gain = 0;
    xmedia_u8 dfine_gain = 0;
    xmedia_u8 reg_0x3e06 = 0;

    for(dcoarse_gain = 1; dcoarse_gain <= 16; dcoarse_gain = dcoarse_gain * 2) {
        if(dgain < (128 * 2 * dcoarse_gain)) {
            break;
        }
    }

    dfine_gain = dgain / dcoarse_gain;
    for ( ; dcoarse_gain >= 2; dcoarse_gain=dcoarse_gain / 2) {
        reg_0x3e06 = (reg_0x3e06 << 1) | 0x01;
    }
    sensor_gain = ((xmedia_u16)reg_0x3e06) << 8;
    sensor_gain += dfine_gain;

    return sensor_gain;
}

static xmedia_s32 sc5336_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *sc5336_ctx = XMEDIA_NULL;
    xmedia_u16 again_temp,dgain_temp;
    xmedia_float f_dgain;
    xmedia_u8 again_index;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    for(again_index = 10; again_index >= 0 ; again_index -= 2)
    {
        if(again >= again_real_cal[again_index])
        break;
    }

    again_temp = again_reg_table[again_index + 1];

    f_dgain = (again / again_real_cal[again_index] ) * dgain / 128.0;

    dgain_temp = sc5336_dgain_mapping_realgain_to_sensorgain(f_dgain);

    sc5336_ctx->regs_info[0].i2c_data[SC5336_REG_AGAIN_H].data = 0x00;
    sc5336_ctx->regs_info[0].i2c_data[SC5336_REG_AGAIN_L].data = SENSOR_LOW_8BITS (again_temp);
    sc5336_ctx->regs_info[0].i2c_data[SC5336_REG_DGAIN_H].data = SENSOR_HIGH_8BITS(dgain_temp);
    sc5336_ctx->regs_info[0].i2c_data[SC5336_REG_DGAIN_L].data = SENSOR_LOW_8BITS (dgain_temp);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = SC5336_CALIBRATE_STATIC_TEMP;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = SC5336_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = SC5336_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = SC5336_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = SC5336_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 5: point index on the awb curve param
    awb_sns_dft->wb_para[0] = SC5336_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1] = SC5336_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2] = SC5336_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3] = SC5336_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4] = SC5336_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5] = SC5336_CALIBRATE_AWB_C1;

    awb_sns_dft->golden_rgain = SC5336_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = SC5336_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (sc5336_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_sc5336_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_sc5336_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
    }

    awb_sns_dft->init_rgain   = g_sc5336_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_sc5336_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_sc5336_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_sc5336_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_sc5336_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_standby(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc5336_write_reg(dev, SC5336_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_resume(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc5336_write_reg(dev, SC5336_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *sc5336_ctx   = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < sc5336_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < sc5336_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= sc5336_write_reg(dev, sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    if (sc5336_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = sc5336_init_image(dev, sc5336_ctx->img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = sc5336_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc5336_set_mirror_flip(dev, sc5336_ctx->mirror_en, sc5336_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc5336_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_sc5336_dev_map[index] == SENSOR_DEV_INVALID) {
            g_sc5336_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 sc5336_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_sc5336_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 sc5336_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{ // TO-DO：min fps is to be confirmed.
    switch (img_mode) {
        case SC5336_5M_10BIT_LINEAR_MODE:
            *min_fps = 4;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case SC5336_5M_10BIT_LINEAR_MODE:
            *vmax = SC5336_VMAX_5M_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = sc5336_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc5336_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc5336_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is [%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    for (i = 0; i < sc5336_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            sc5336_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &sc5336_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&sc5336_ctx->regs_info[SENSOR_PRE_FRAME], &sc5336_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    sc5336_ctx->fl[SENSOR_PRE_FRAME] = sc5336_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    ret = sc5336_calc_fps(fps, sc5336_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    sc5336_ctx->regs_info[0].i2c_data[SC5336_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    sc5336_ctx->regs_info[0].i2c_data[SC5336_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    sc5336_ctx->fl_std = SENSOR_MIN(full_lines, SC5336_FULL_LINES_MAX);
    sc5336_ctx->fl[SENSOR_CUR_FRAME] = sc5336_ctx->fl_std;
    sc5336_ctx->fps                  = fps;
    ae_sns_dft->fps                  = fps;
    ae_sns_dft->full_lines_std       = sc5336_ctx->fl_std;
    ae_sns_dft->full_lines           = sc5336_ctx->fl[SENSOR_CUR_FRAME];

    if (sc5336_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sc5336_ctx->fl[SENSOR_CUR_FRAME] - SC5336_EXP_OFFSET_LINEAR;
    }

    SENSOR_PRINT("dev[%d]- cv8001 set fps: %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = sc5336_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc5336_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc5336_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = vmax_max_fps * max_fps / min_fps;
    return XMEDIA_SUCCESS;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 sc5336_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 * 注意：2to1 wdr模式下，曝光比和ae wdr mode 由AE传入，调用本接口前，AE需先调用sc5336_get_max_inttime
 */
static xmedia_s32 sc5336_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *sc5336_ctx = XMEDIA_NULL;
    xmedia_float   min_fps;
    xmedia_u32     vmax_min_fps;
    xmedia_s32     ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    if (sc5336_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", sc5336_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (sc5336_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        full_lines = full_lines + SC5336_EXP_OFFSET_LINEAR;
    }

    ret = sc5336_get_min_fps(sc5336_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc5336_get_min_fps_vmax(sc5336_ctx->img_mode, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                       = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    sc5336_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    sc5336_ctx->fps                  = min_fps * vmax_min_fps / full_lines;

    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_VMAX_H].data =
        SENSOR_HIGH_8BITS(full_lines);
    sc5336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC5336_REG_VMAX_L].data =
        SENSOR_LOW_8BITS(full_lines);

    ae_sns_dft->full_lines = sc5336_ctx->fl[SENSOR_CUR_FRAME];
    if (sc5336_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sc5336_ctx->fl[SENSOR_CUR_FRAME] - SC5336_EXP_OFFSET_LINEAR;
    }

    SENSOR_PRINT("dev[%d]- sc5336 set fps: %f\n", dev, sc5336_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    ret = sc5336_get_min_fps(sc5336_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc5336_get_max_fps(sc5336_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = sc5336_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc5336_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, SC5336_NAME, sizeof(SC5336_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = sc5336_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = sc5336_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = sc5336_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = sc5336_mirror;
    info->isp_func.pfn_sensor_flip             = sc5336_flip;
    info->isp_func.pfn_sensor_set_bus_info     = sc5336_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = sc5336_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = sc5336_set_init_param;
    info->isp_func.pfn_sensor_start            = sc5336_start;
    info->isp_func.pfn_sensor_stop             = sc5336_stop;
    info->isp_func.pfn_sensor_standby          = sc5336_standby;
    info->isp_func.pfn_sensor_resume           = sc5336_resume;
    info->isp_func.pfn_sensor_write_reg        = sc5336_write_reg;
    info->isp_func.pfn_sensor_read_reg         = sc5336_read_reg;
    info->isp_func.pfn_sensor_init             = sc5336_init;
    info->isp_func.pfn_sensor_exit             = sc5336_exit;

    // called by ISP
    info->isp_func.pfn_sensor_set_attr            = sc5336_set_attr;
    info->isp_func.pfn_sensor_get_isp_default     = sc5336_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = sc5336_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = sc5336_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = sc5336_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = sc5336_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = sc5336_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = sc5336_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = sc5336_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = sc5336_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = sc5336_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = sc5336_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = sc5336_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = sc5336_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = sc5336_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = sc5336_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = sc5336_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = sc5336_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc5336_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = sc5336_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc5336_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = sc5336_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "SC5336 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc5336_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *sc5336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = sc5336_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    SC5336_GET_CTX(dev, sc5336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc5336_ctx);

    if (sc5336_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "SC5336 unregister function failed!\n");
        return ret;
    }

    sc5336_ctx_exit(dev);
    g_sc5336_dev_map[dev] = SENSOR_DEV_INVALID; // reset dev

    return XMEDIA_SUCCESS;
}
