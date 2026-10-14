#include <stdlib.h>
#include <string.h>
#include "bf3a03.h"
#include "bf3a03_ctrl.h"
#include "bf3a03_ex.h"
#include "xmedia_isp.h"

#define BF3A03_NAME          "BF3A03"
#define BF3A03_SPECS_MAX_NUM 1

#ifdef __linux__
#define BF3A03_ISP_DEFAULT_SUPPORT
#endif

#define BF3A03_HMAX_640X480_LINEAR     0x500 // 未找到vmax对应的寄存器
#define BF3A03_640X480_BIT_RATE_LINEAR 432

#define BF3A03_REG_ADDR_STANDBY 0x09

#define BF3A03_RES_IS_640X480(w, h) ((w) == 640 && (h) == 480)

#define BF3A03_ERR_MODE_PRINT(sns_attr)                                                                                \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sns_attr->width,                \
                     sns_attr->height, sns_attr->wdr_mode);                                                            \
    } while (0)

sensor_context *g_bf3a03_ctx[XMEDIA_SENSOR_DEV_MAX_NUM]    = { XMEDIA_NULL };
#define BF3A03_SET_CTX(dev, sns_ctx) g_bf3a03_ctx[dev] = sns_ctx
#define BF3A03_GET_CTX(dev, sns_ctx) sns_ctx = g_bf3a03_ctx[dev]

static xmedia_u32 g_bf3a03_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_bf3a03_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_bf3a03_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_bf3a03_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };
static xmedia_s32 g_bf3a03_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

static const xmedia_sensor_property g_bf3a03_property[BF3A03_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        640, 480, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_NONE, 30, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_DC, XMEDIA_VIDEO_PIXEL_FMT_YUYV_PACKAGE_422, XMEDIA_VIDEO_DATA_WIDTH_8,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate
        { 0 }, BF3A03_640X480_BIT_RATE_LINEAR,
    },
};

static const xmedia_sensor_capability g_bf3a03_capability = {
    .max_width   = 640,
    .max_height  = 480,
    .max_fps     = 30,

    .output_intf = XMEDIA_INTF_TYPE_DC,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_NONE,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_8,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported  = XMEDIA_FALSE,
    .mipi_lanes_supported  = XMEDIA_FALSE,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = BF3A03_ADDR_BYTE,
        .data_byte_num     = BF3A03_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { BF3A03_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0xD5 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { BF3A03_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x55 },
    },
};

static xmedia_s32 bf3a03_ctx_init(xmedia_u32 dev)
{
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    BF3A03_GET_CTX(dev, bf3a03_ctx);
    if (bf3a03_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    bf3a03_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (bf3a03_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(bf3a03_ctx, 0, sizeof(sensor_context));
    bf3a03_ctx->i2c_addr         = BF3A03_I2C_ADDR;
    bf3a03_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_2L;
    bf3a03_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    bf3a03_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    bf3a03_ctx->size.width       = 640;
    bf3a03_ctx->size.height      = 480;
    bf3a03_ctx->fps              = g_bf3a03_property[0].max_fps;
    bf3a03_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    bf3a03_ctx->img_mode             = BF3A03_DVP_640X480_LINEAR_MODE;
    bf3a03_ctx->fl_std               = BF3A03_VMAX_640X480_LINEAR << BF3A03_EXP_SHIFT;
    bf3a03_ctx->fl[SENSOR_CUR_FRAME] = BF3A03_VMAX_640X480_LINEAR << BF3A03_EXP_SHIFT;
    bf3a03_ctx->fl[SENSOR_PRE_FRAME] = BF3A03_VMAX_640X480_LINEAR << BF3A03_EXP_SHIFT;

    BF3A03_SET_CTX(dev, bf3a03_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void bf3a03_ctx_exit(xmedia_u32 dev)
{
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_FREE(bf3a03_ctx);
    BF3A03_SET_CTX(dev, XMEDIA_NULL);
}

static xmedia_s32 bf3a03_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    for (i = 0; i < BF3A03_SPECS_MAX_NUM; i++) {
        if (g_bf3a03_property[i].width == bf3a03_ctx->size.width &&
            g_bf3a03_property[i].height == bf3a03_ctx->size.height &&
            g_bf3a03_property[i].wdr_mode == bf3a03_ctx->wdr_mode) {
            memcpy(property, &g_bf3a03_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n", bf3a03_ctx->size.width,
                 bf3a03_ctx->size.height, bf3a03_ctx->wdr_mode);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 bf3a03_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    bf3a03_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 bf3a03_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        bf3a03_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        bf3a03_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        bf3a03_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 bf3a03_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    bf3a03_ctx->i2c_addr = slave_addr;
    ret                  = bf3a03_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    *slave_addr = bf3a03_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    bf3a03_ctx->init_mode = init_mode;

    ret = bf3a03_i2c_init(dev, bf3a03_ctx->bus_info.i2c_dev, bf3a03_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    bf3a03_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    ret = bf3a03_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    bf3a03_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    bf3a03_ctx->mirror_en = mirror_en;
    ret                   = bf3a03_set_mirror_flip(dev, mirror_en, bf3a03_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    bf3a03_ctx->flip_en = flip_en;
    ret                 = bf3a03_set_mirror_flip(dev, bf3a03_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_set_image_mode(sensor_context *bf3a03_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (bf3a03_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (BF3A03_RES_IS_640X480(sns_attr->width, sns_attr->height)) {
            *image_mode = BF3A03_DVP_640X480_LINEAR_MODE;
            if (bf3a03_ctx->work_mode == XMEDIA_SENSOR_WORK_MODE_NORMAL) {
                bf3a03_ctx->fl_std = BF3A03_VMAX_640X480_LINEAR << BF3A03_EXP_SHIFT;
            } else {
                SENSOR_TRACE(MODULE_DBG_ERR, "Not support work mode: %d\n", bf3a03_ctx->work_mode);
                return XMEDIA_ERRCODE_NOT_SUPPORT;
            }
        } else {
            BF3A03_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        BF3A03_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    bf3a03_ctx->size.width  = sns_attr->width;
    bf3a03_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            bf3a03_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(bf3a03_ctx->wdr_int_time, 0, sizeof(bf3a03_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

static xmedia_void bf3a03_init_common_reg_info(sensor_context *bf3a03_ctx)
{
    return;
}

static xmedia_s32 bf3a03_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    bf3a03_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = bf3a03_ctx->bus_info.i2c_dev;
    bf3a03_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    bf3a03_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = BF3A03_REG_MAX_NUM;

    for (i = 0; i < bf3a03_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        bf3a03_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        bf3a03_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = bf3a03_ctx->i2c_addr;
        bf3a03_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = BF3A03_ADDR_BYTE;
        bf3a03_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = BF3A03_DATA_BYTE;
    }

    // init general register - The registers should init both in linear
    bf3a03_init_common_reg_info(bf3a03_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    // Set wdr mode
    ret = bf3a03_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = bf3a03_set_image_mode(bf3a03_ctx, &bf3a03_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    bf3a03_ctx->fl[SENSOR_CUR_FRAME] = bf3a03_ctx->fl_std;
    bf3a03_ctx->fl[SENSOR_PRE_FRAME] = bf3a03_ctx->fl[SENSOR_CUR_FRAME];

    ret = bf3a03_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef BF3A03_ISP_DEFAULT_SUPPORT
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from bf3a03!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 bf3a03_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (bf3a03_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 64;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 64;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 64;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 64;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *bf3a03_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    if (bf3a03_ctx->mirror_en && bf3a03_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (bf3a03_ctx->mirror_en && (!bf3a03_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!bf3a03_ctx->mirror_en) && bf3a03_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < BF3A03_SPECS_MAX_NUM; i++) {
        if (g_bf3a03_property[i].width == bf3a03_ctx->size.width &&
            g_bf3a03_property[i].height == bf3a03_ctx->size.height &&
            g_bf3a03_property[i].wdr_mode == bf3a03_ctx->wdr_mode) {
            *bayer_pattern = g_bf3a03_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= BF3A03_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_bf3a03_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_get_trig_attr(xmedia_sensor_trig_attr *trig_attr)
{
    SENSOR_CHECK_PTR_RETURN(trig_attr);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    bf3a03_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 bf3a03_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_bf3a03_exposure[dev]      = init_param->exposure;
    g_bf3a03_sample_r_gain[dev] = init_param->sample_rgain;
    g_bf3a03_sample_b_gain[dev] = init_param->sample_bgain;

    g_bf3a03_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_bf3a03_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_bf3a03_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 bf3a03_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 bf3a03_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 bf3a03_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 bf3a03_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

// AWB
static xmedia_s32 bf3a03_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 bf3a03_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case BF3A03_DVP_640X480_LINEAR_MODE:
            *max_fps = 30.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_standby(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = bf3a03_write_reg(dev, BF3A03_REG_ADDR_STANDBY, 0xD5);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_resume(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = bf3a03_write_reg(dev, BF3A03_REG_ADDR_STANDBY, 0x55);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *bf3a03_ctx   = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < bf3a03_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += bf3a03_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < bf3a03_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= bf3a03_write_reg(dev, bf3a03_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                bf3a03_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    if (bf3a03_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = bf3a03_init_image(dev, bf3a03_ctx->img_mode, bf3a03_ctx->work_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = bf3a03_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = bf3a03_set_mirror_flip(dev, bf3a03_ctx->mirror_en, bf3a03_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = bf3a03_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_bf3a03_dev_map[index] == SENSOR_DEV_INVALID) {
            g_bf3a03_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 bf3a03_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_bf3a03_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 bf3a03_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{
    switch (img_mode) {
        case BF3A03_DVP_640X480_LINEAR_MODE:
            *min_fps = 1.5;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    for (i = 0; i < bf3a03_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (bf3a03_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            bf3a03_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            bf3a03_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            bf3a03_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &bf3a03_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&bf3a03_ctx->regs_info[SENSOR_PRE_FRAME], &bf3a03_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    bf3a03_ctx->fl[SENSOR_PRE_FRAME] = bf3a03_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 bf3a03_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 * 注意：2to1 wdr模式下，曝光比和ae wdr mode 由AE传入，调用本接口前，AE需先调用bf3a03_get_wdr_max_inttime
 */
static xmedia_s32 bf3a03_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 bf3a03_get_hmax_vmax(xmedia_u32 dev, xmedia_u32 *hmax, xmedia_u32 *vmax)
{
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(hmax);
    SENSOR_CHECK_PTR_RETURN(vmax);

    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    *vmax = bf3a03_ctx->fl[SENSOR_CUR_FRAME] >> BF3A03_EXP_SHIFT;

    switch (bf3a03_ctx->img_mode) {
        case BF3A03_DVP_640X480_LINEAR_MODE:
            if (bf3a03_ctx->work_mode == XMEDIA_SENSOR_WORK_MODE_NORMAL) {
                *hmax = BF3A03_HMAX_640X480_LINEAR; // TODO: 待确认寄存器值是否就是真正的hmax
            } else {
                SENSOR_TRACE(MODULE_DBG_ERR, "Not support work mode: %d\n", bf3a03_ctx->work_mode);
                return XMEDIA_ERRCODE_NOT_SUPPORT;
            }
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    ret = bf3a03_get_min_fps(bf3a03_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = bf3a03_get_max_fps(bf3a03_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = bf3a03_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 bf3a03_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, BF3A03_NAME, sizeof(BF3A03_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = bf3a03_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = bf3a03_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = bf3a03_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = bf3a03_mirror;
    info->isp_func.pfn_sensor_flip             = bf3a03_flip;
    info->isp_func.pfn_sensor_set_bus_info     = bf3a03_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = bf3a03_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = bf3a03_set_init_param;
    info->isp_func.pfn_sensor_start            = bf3a03_start;
    info->isp_func.pfn_sensor_stop             = bf3a03_stop;
    info->isp_func.pfn_sensor_standby          = bf3a03_standby;
    info->isp_func.pfn_sensor_resume           = bf3a03_resume;
    info->isp_func.pfn_sensor_write_reg        = bf3a03_write_reg;
    info->isp_func.pfn_sensor_read_reg         = bf3a03_read_reg;
    info->isp_func.pfn_sensor_init             = bf3a03_init;
    info->isp_func.pfn_sensor_exit             = bf3a03_exit;
    info->isp_func.pfn_sensor_set_attr         = bf3a03_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = bf3a03_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = bf3a03_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = bf3a03_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = bf3a03_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = bf3a03_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = bf3a03_get_dev_addr;
    info->isp_func.pfn_sensor_get_trig_attr       = bf3a03_get_trig_attr;
    info->isp_func.pfn_sensor_get_hmax_vmax       = bf3a03_get_hmax_vmax;
    info->isp_func.pfn_sensor_get_fps             = bf3a03_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = bf3a03_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = bf3a03_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = bf3a03_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = bf3a03_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = bf3a03_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = bf3a03_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = bf3a03_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = bf3a03_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = bf3a03_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = bf3a03_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 bf3a03_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = bf3a03_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = bf3a03_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = bf3a03_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "BF3A03 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 bf3a03_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *bf3a03_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = bf3a03_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    BF3A03_GET_CTX(dev, bf3a03_ctx);
    SENSOR_CHECK_PTR_RETURN(bf3a03_ctx);

    if (bf3a03_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "BF3A03 unregister function failed!\n");
        return ret;
    }

    bf3a03_ctx_exit(dev);
    g_bf3a03_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;
}
