#include <stdlib.h>
#include <string.h>
#include "mis20s1.h"
#include "mis20s1_ctrl.h"
#include "mis20s1_ex.h"
#include "xmedia_isp.h"

#define MIS20S1_NAME          "MIS20S1"
#define MIS20S1_SPECS_MAX_NUM 1

#ifdef __linux__
#define MIS20S1_ISP_DEFAULT_SUPPORT
#endif

#define MIS20S1_2M_1080_BIT_RATE_LINEAR  372

#define MIS20S1_RES_IS_1080P(w, h) ((w) == 1920 && (h) == 1080)

#define MIS20S1_ERR_MODE_PRINT(sns_attr)                                                                         \
    do {                                                                                                         \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sns_attr->width,          \
                     sns_attr->height, sns_attr->wdr_mode);                                                      \
    } while (0)

// awb static param for Fuji Lens New IR_Cut
#define MIS20S1_CALIBRATE_STATIC_TEMP       5000
#define MIS20S1_CALIBRATE_STATIC_WB_R_GAIN  488
#define MIS20S1_CALIBRATE_STATIC_WB_GR_GAIN 256
#define MIS20S1_CALIBRATE_STATIC_WB_GB_GAIN 256
#define MIS20S1_CALIBRATE_STATIC_WB_B_GAIN  440

// Calibration results for Auto WB Planck
#define MIS20S1_CALIBRATE_AWB_P1 38
#define MIS20S1_CALIBRATE_AWB_P2 160
#define MIS20S1_CALIBRATE_AWB_Q1 -58
#define MIS20S1_CALIBRATE_AWB_A1 214888
#define MIS20S1_CALIBRATE_AWB_B1 128
#define MIS20S1_CALIBRATE_AWB_C1 (-164091)

// Rgain and Bgain of the golden sample
#define MIS20S1_GOLDEN_RGAIN 0
#define MIS20S1_GOLDEN_BGAIN 0

#define MIS20S1_AGAIN_MIN   (1024)
#define MIS20S1_AGAIN_MAX   (1024 * 32)     // the max again is 77660 mean 75.84 recommend

#define SENSOR_LCG_TO_HCG    85

#define SENSOR_LCG_TO_HCG1   80
#define SENSOR_LCG_TO_HCG_OFFSET 5
#define MIS20S1_MAX_GAIN         (MIS20S1_AGAIN_MAX * SENSOR_LCG_TO_HCG1)

static xmedia_u8 g_dcg_data = 2;

sensor_context *g_mis20s1_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define MIS20S1_SET_CTX(dev, ctx) g_mis20s1_ctx[dev] = ctx
#define MIS20S1_GET_CTX(dev, ctx) ctx = g_mis20s1_ctx[dev]

static xmedia_u32 g_mis20s1_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_mis20s1_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_mis20s1_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_mis20s1_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };
static xmedia_s32 g_mis20s1_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

static const xmedia_sensor_property g_mis20s1_property[MIS20S1_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_NONE, 30, XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_GBRG, XMEDIA_VIDEO_BAYER_FMT_RGGB,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_GRBG },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, MIS20S1_2M_1080_BIT_RATE_LINEAR,
    },
};

static const xmedia_sensor_capability g_mis20s1_capability = {
    .max_width   = 1920,
    .max_height  = 1080,
    .max_fps     = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .mipi_lanes_supported  = XMEDIA_SENSOR_MIPI_LANES_2L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = MIS20S1_ADDR_BYTE,
        .data_byte_num     = MIS20S1_DATA_BYTE,
        .standby_reg_num   = 0,
        .resume_reg_num    = 0,
    },
};

static xmedia_s32 mis20s1_ctx_init(xmedia_u32 dev)
{
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    if (mis20s1_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    mis20s1_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (mis20s1_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(mis20s1_ctx, 0, sizeof(sensor_context));
    mis20s1_ctx->i2c_addr         = MIS20S1_I2C_ADDR;
    mis20s1_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_2L;
    mis20s1_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    mis20s1_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    mis20s1_ctx->size.width       = 1920;
    mis20s1_ctx->size.height      = 1080;
    mis20s1_ctx->fps              = g_mis20s1_property[0].max_fps;
    mis20s1_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    mis20s1_ctx->img_mode             = MIS20S1_2M_1080_10BIT_LINEAR_MODE;
    mis20s1_ctx->fl_std               = MIS20S1_VMAX_2M_1080_LINEAR;
    mis20s1_ctx->fl[SENSOR_CUR_FRAME] = MIS20S1_VMAX_2M_1080_LINEAR;
    mis20s1_ctx->fl[SENSOR_PRE_FRAME] = MIS20S1_VMAX_2M_1080_LINEAR;

    MIS20S1_SET_CTX(dev, mis20s1_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void mis20s1_ctx_exit(xmedia_u32 dev)
{
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_FREE(mis20s1_ctx);
    MIS20S1_SET_CTX(dev, XMEDIA_NULL);
}

static xmedia_s32 mis20s1_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    for (i = 0; i < MIS20S1_SPECS_MAX_NUM; i++) {
        if (g_mis20s1_property[i].width == mis20s1_ctx->size.width &&
            g_mis20s1_property[i].height == mis20s1_ctx->size.height &&
            g_mis20s1_property[i].wdr_mode == mis20s1_ctx->wdr_mode) {
            memcpy(property, &g_mis20s1_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n",
                    mis20s1_ctx->size.width, mis20s1_ctx->size.height, mis20s1_ctx->wdr_mode);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 mis20s1_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    mis20s1_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "No support mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    mis20s1_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        mis20s1_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        mis20s1_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        mis20s1_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 mis20s1_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    mis20s1_ctx->i2c_addr = slave_addr;
    ret                   = mis20s1_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    *slave_addr = mis20s1_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 ctx_dcg_check(xmedia_u32 dev, xmedia_u8 *dcg_data)
{
    xmedia_u32 reg0x2003;

    mis20s1_read_reg(dev, 0x2003, &reg0x2003);

    if (reg0x2003 == 0 || reg0x2003 == 0xFF) {
        *dcg_data = SENSOR_LCG_TO_HCG;
        return XMEDIA_SUCCESS;
    }

    *dcg_data = (reg0x2003 & 0x0f) * 10;
    mis20s1_read_reg(dev, 0x2004, &reg0x2003);
    *dcg_data += ((reg0x2003 & 0xfc) >> 2);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    mis20s1_ctx->init_mode = init_mode;

    ret = mis20s1_i2c_init(dev, mis20s1_ctx->bus_info.i2c_dev, mis20s1_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    ctx_dcg_check(dev, &g_dcg_data);

    mis20s1_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    ret = mis20s1_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    mis20s1_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    mis20s1_ctx->mirror_en = mirror_en;
    ret                    = mis20s1_set_mirror_flip(dev, mirror_en, mis20s1_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    mis20s1_ctx->flip_en = flip_en;
    ret                  = mis20s1_set_mirror_flip(dev, mis20s1_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_set_image_mode(sensor_context *mis20s1_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (mis20s1_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (MIS20S1_RES_IS_1080P(sns_attr->width, sns_attr->height)) {
            *image_mode         = MIS20S1_2M_1080_10BIT_LINEAR_MODE;
            mis20s1_ctx->fl_std = MIS20S1_VMAX_2M_1080_LINEAR;
        } else {
            MIS20S1_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        MIS20S1_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    mis20s1_ctx->size.width  = sns_attr->width;
    mis20s1_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            mis20s1_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(mis20s1_ctx->wdr_int_time, 0, sizeof(mis20s1_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

static xmedia_void mis20s1_init_common_reg_info(sensor_context *mis20s1_ctx)
{
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_EXP_H].delay_frame_num = 2;
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_EXP_H].reg_addr        = MIS20S1_REG_ADDR_EXP_H;
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_EXP_L].delay_frame_num = 2;
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_EXP_L].reg_addr        = MIS20S1_REG_ADDR_EXP_L;

    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_AGAIN_H].delay_frame_num = 2;
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_AGAIN_H].reg_addr        = MIS20S1_REG_ADDR_AGAIN_H;
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_AGAIN_L].delay_frame_num = 2;
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_AGAIN_L].reg_addr        = MIS20S1_REG_ADDR_AGAIN_L;

    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_VMAX_H].delay_frame_num = 2;
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_VMAX_H].reg_addr        = MIS20S1_REG_ADDR_VMAX_H;
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_VMAX_L].delay_frame_num = 2;
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_VMAX_L].reg_addr        = MIS20S1_REG_ADDR_VMAX_L;

    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_DGAIN_H].delay_frame_num = 2;
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_DGAIN_H].reg_addr        = MIS20S1_REG_ADDR_DGAIN_H;
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_DGAIN_L].delay_frame_num = 2;
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_DGAIN_L].reg_addr        = MIS20S1_REG_ADDR_DGAIN_L;

    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_NUM1].delay_frame_num = 2;
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_NUM1].reg_addr        = MIS20S1_REG_ADDR_NUM1;

    return;
}

xmedia_s32 mis20s1_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = mis20s1_ctx->bus_info.i2c_dev;
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = MIS20S1_REG_MAX_NUM;

    for (i = 0; i < mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = mis20s1_ctx->i2c_addr;
        mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = MIS20S1_ADDR_BYTE;
        mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = MIS20S1_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    mis20s1_init_common_reg_info(mis20s1_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    // Set wdr mode
    ret = mis20s1_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = mis20s1_set_image_mode(mis20s1_ctx, &mis20s1_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    mis20s1_ctx->fl[SENSOR_CUR_FRAME] = mis20s1_ctx->fl_std;
    mis20s1_ctx->fl[SENSOR_PRE_FRAME] = mis20s1_ctx->fl[SENSOR_CUR_FRAME];

    ret = mis20s1_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef MIS20S1_ISP_DEFAULT_SUPPORT
    // TO-DO: alg add param
    isp_default->blc         = &g_mis20s1_blc_attr;
    isp_default->bnr         = &g_mis20s1_bnr;
    isp_default->clut_attr   = &g_mis20s1_clut_attr;
    isp_default->crosstalk   = XMEDIA_NULL;
    isp_default->csc         = XMEDIA_NULL;
    isp_default->dehaze      = &g_mis20s1_dehaze_attr;
    isp_default->demosaic    = &g_mis20s1_demosaic_attr;
    isp_default->dpc_dynamic = XMEDIA_NULL;
    isp_default->dpc_static  = XMEDIA_NULL;
    isp_default->drc         = XMEDIA_NULL;
    isp_default->expander    = XMEDIA_NULL;
    isp_default->fpn         = XMEDIA_NULL;
    isp_default->gamma       = &g_mis20s1_gamma_attr;
    isp_default->gcac        = XMEDIA_NULL;
    isp_default->hlc         = XMEDIA_NULL;
    isp_default->lce         = &g_mis20s1_lce_attr;
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from mis20s1!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 mis20s1_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (mis20s1_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 252;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 252;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 252;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 252;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    if (mis20s1_ctx->mirror_en && mis20s1_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (mis20s1_ctx->mirror_en && (!mis20s1_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!mis20s1_ctx->mirror_en) && mis20s1_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < MIS20S1_SPECS_MAX_NUM; i++) {
        if (g_mis20s1_property[i].width == mis20s1_ctx->size.width &&
            g_mis20s1_property[i].height == mis20s1_ctx->size.height &&
            g_mis20s1_property[i].wdr_mode == mis20s1_ctx->wdr_mode) {
            *bayer_pattern = g_mis20s1_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= MIS20S1_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_mis20s1_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    mis20s1_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 mis20s1_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_mis20s1_exposure[dev]      = init_param->exposure;
    g_mis20s1_sample_r_gain[dev] = init_param->sample_rgain;
    g_mis20s1_sample_b_gain[dev] = init_param->sample_bgain;

    g_mis20s1_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_mis20s1_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_mis20s1_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

xmedia_s32 mis20s1_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case MIS20S1_2M_1080_10BIT_LINEAR_MODE:
            *max_fps = 30.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                        xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = mis20s1_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256;
    ae_sns_dft->full_lines_max = MIS20S1_FULL_LINES_MAX;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->again_accu.accuracy  = 0.0009765625; // 1 / 1024

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy  = 0.015625;    // 1/64

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 32 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->ispdgain_sep_thd    = 16 << ae_sns_dft->ispdgain_shift;

    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_32_0;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    // lines per second: VTS * max_fps
    ae_sns_dft->lines_per_500ms  = (sns_ctx->fl_std * max_fps) / 2;
    ae_sns_dft->man_ratio_enable = XMEDIA_FALSE;

    SENSOR_PRINT("man_ratio_enable: %d,\n", ae_sns_dft->man_ratio_enable);

    return XMEDIA_SUCCESS;
}

static xmedia_void mis20s1_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = MIS20S1_AGAIN_MAX * 8.5;
    ae_sns_dft->min_again        = MIS20S1_AGAIN_MIN;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 64; /* if Dgain enable,please set ispdgain bigger than 1 */
    ae_sns_dft->min_dgain        = 64;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_mis20s1_exposure[dev] ? g_mis20s1_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - MIS20S1_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 1;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = 1;

    return;
}

static xmedia_s32 mis20s1_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = mis20s1_get_ae_common_default(dev, mis20s1_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (mis20s1_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            mis20s1_get_ae_linear_default(dev, mis20s1_ctx, ae_sns_dft);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", mis20s1_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);
    int_time = (int_time > MIS20S1_FULL_LINES_MAX) ? MIS20S1_FULL_LINES_MAX : int_time;

    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_EXP_H].data = SENSOR_HIGH_8BITS(int_time);
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_EXP_L].data = SENSOR_LOW_8BITS(int_time);
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_NUM1].data  = 1;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    *again_db = *again_lin;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    static xmedia_u32  again1 = 0,dgain1 = 0x100;

    if (again < 1024) {
        again = 1024;
    } else if (again > MIS20S1_MAX_GAIN ) {
        again = MIS20S1_MAX_GAIN;
    }

    if (again < (g_dcg_data) * 1024 / 10) {
        mis20s1_write_reg(dev, 0x310c, 0x0);
        again1 = 1024 - (1024 << 10) / again;
    } else if(again > (g_dcg_data ) * 1024 / 10) {
        if (again > (g_dcg_data * MIS20S1_AGAIN_MAX  / 10)) {
            dgain1 = again * 256 / (g_dcg_data * MIS20S1_AGAIN_MAX / 10);
            again1 = 992;//32倍对应寄存器的值
        } else {
            again1 = 1024 - ((1024  << 10) * g_dcg_data ) / again / 10;
            dgain1 = 0x100;
        }
        mis20s1_write_reg(dev, 0x310c, 0x1);
    }

    dgain1 = 0x100; //默认配置为1倍

    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_DGAIN_H].data = ((dgain1 >> 8) & 0x3);
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_DGAIN_L].data = SENSOR_LOW_8BITS(dgain1);
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_AGAIN_H].data = ((again1 >> 8) & 0x3);
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_AGAIN_L].data = SENSOR_LOW_8BITS(again1);

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 mis20s1_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = MIS20S1_CALIBRATE_STATIC_TEMP;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = MIS20S1_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = MIS20S1_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = MIS20S1_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = MIS20S1_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 5: point index on the awb curve param
    awb_sns_dft->wb_para[0] = MIS20S1_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1] = MIS20S1_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2] = MIS20S1_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3] = MIS20S1_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4] = MIS20S1_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5] = MIS20S1_CALIBRATE_AWB_C1;

    awb_sns_dft->golden_rgain    = MIS20S1_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain    = MIS20S1_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (mis20s1_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_mis20s1_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_mis20s1_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", mis20s1_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_mis20s1_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_mis20s1_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_mis20s1_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_mis20s1_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_mis20s1_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_standby(xmedia_u32 dev)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_resume(xmedia_u32 dev)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *mis20s1_ctx  = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= mis20s1_write_reg(dev, mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                 mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    if (mis20s1_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = mis20s1_init_image(dev, mis20s1_ctx->img_mode, mis20s1_ctx->work_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = mis20s1_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = mis20s1_set_mirror_flip(dev, mis20s1_ctx->mirror_en, mis20s1_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = mis20s1_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_mis20s1_dev_map[index] == SENSOR_DEV_INVALID) {
            g_mis20s1_dev_map[index] = pipe;
            *dev                     = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 mis20s1_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_mis20s1_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 mis20s1_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{ // TO-DO：min fps is to be confirmed.
    switch (img_mode) {
        case MIS20S1_2M_1080_10BIT_LINEAR_MODE:
            *min_fps = 2.75;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case MIS20S1_2M_1080_10BIT_LINEAR_MODE:
            *vmax = MIS20S1_VMAX_2M_1080_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 mis20s1_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    for (i = 0; i < mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            mis20s1_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_NUM1].update = XMEDIA_TRUE;

    memcpy(sns_regs_info, &mis20s1_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&mis20s1_ctx->regs_info[SENSOR_PRE_FRAME], &mis20s1_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    mis20s1_ctx->fl[SENSOR_PRE_FRAME] = mis20s1_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = mis20s1_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = mis20s1_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = mis20s1_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is (%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, MIS20S1_FULL_LINES_MAX);

    SENSOR_PRINT("mis20s1 set fps = %f\n", fps);

    return XMEDIA_SUCCESS;
}

xmedia_s32 mis20s1_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    ret = mis20s1_calc_fps(fps, mis20s1_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_NUM1].data   = 1;

    mis20s1_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    mis20s1_ctx->fl_std               = mis20s1_ctx->fl[SENSOR_CUR_FRAME];
    mis20s1_ctx->fps                  = fps;

    ae_sns_dft->fps            = fps;
    ae_sns_dft->full_lines_std = mis20s1_ctx->fl_std;
    ae_sns_dft->full_lines     = mis20s1_ctx->fl_std;

    if (mis20s1_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = mis20s1_ctx->fl[SENSOR_CUR_FRAME] - MIS20S1_EXP_OFFSET_LINEAR;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = mis20s1_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = mis20s1_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = mis20s1_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = vmax_max_fps * max_fps / min_fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);

    return XMEDIA_SUCCESS;
}

xmedia_s32 mis20s1_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *mis20s1_ctx = XMEDIA_NULL;
    xmedia_u32      vmax_min_fps;
    xmedia_s32      ret;
    xmedia_float    min_fps;

    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    if (mis20s1_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", mis20s1_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (mis20s1_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        full_lines = full_lines + MIS20S1_EXP_OFFSET_LINEAR;
    }

    ret = mis20s1_get_min_fps_vmax(mis20s1_ctx->img_mode, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = mis20s1_get_min_fps(mis20s1_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                        = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    mis20s1_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    mis20s1_ctx->fps                  = min_fps * vmax_min_fps / full_lines;

    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_VMAX_H].data =
                                                                    SENSOR_HIGH_8BITS(full_lines);
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_VMAX_L].data =
                                                                    SENSOR_LOW_8BITS(full_lines);
    mis20s1_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[MIS20S1_REG_NUM1].data   = 1;

    ae_sns_dft->full_lines   = mis20s1_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = mis20s1_ctx->fl[SENSOR_CUR_FRAME] - MIS20S1_EXP_OFFSET_LINEAR;

    SENSOR_PRINT("dev[%d]- mis40h1 set fps: %f\n", dev, mis20s1_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32      ret;
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    ret = mis20s1_get_min_fps(mis20s1_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = mis20s1_get_max_fps(mis20s1_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = mis20s1_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 mis20s1_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, MIS20S1_NAME, sizeof(MIS20S1_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = mis20s1_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = mis20s1_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = mis20s1_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = mis20s1_mirror;
    info->isp_func.pfn_sensor_flip             = mis20s1_flip;
    info->isp_func.pfn_sensor_set_bus_info     = mis20s1_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = mis20s1_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = mis20s1_set_init_param;
    info->isp_func.pfn_sensor_start            = mis20s1_start;
    info->isp_func.pfn_sensor_stop             = mis20s1_stop;
    info->isp_func.pfn_sensor_standby          = mis20s1_standby;
    info->isp_func.pfn_sensor_resume           = mis20s1_resume;
    info->isp_func.pfn_sensor_write_reg        = mis20s1_write_reg;
    info->isp_func.pfn_sensor_read_reg         = mis20s1_read_reg;
    info->isp_func.pfn_sensor_init             = mis20s1_init;
    info->isp_func.pfn_sensor_exit             = mis20s1_exit;
    info->isp_func.pfn_sensor_set_attr         = mis20s1_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = mis20s1_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = mis20s1_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = mis20s1_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = mis20s1_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = mis20s1_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = mis20s1_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = mis20s1_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = mis20s1_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = mis20s1_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = mis20s1_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = mis20s1_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = mis20s1_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = mis20s1_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = mis20s1_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = mis20s1_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = mis20s1_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = mis20s1_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 mis20s1_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = mis20s1_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = mis20s1_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = mis20s1_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "MIS20S1 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 mis20s1_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *mis20s1_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = mis20s1_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    MIS20S1_GET_CTX(dev, mis20s1_ctx);
    SENSOR_CHECK_PTR_RETURN(mis20s1_ctx);

    if (mis20s1_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "MIS20S1 unregister function failed!\n");
        return ret;
    }

    mis20s1_ctx_exit(dev);
    g_mis20s1_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;
}
