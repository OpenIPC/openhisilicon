#include <stdlib.h>
#include <string.h>
#include "imx678.h"
#include "imx678_ctrl.h"
#include "imx678_ex.h"

#define IMX678_NAME "IMX678"
#define IMX678_SPECS_MAX_NUM 2

#define IMX678_8M_BIT_RATE_LINEAR 1440
#define IMX678_8M_BIT_RATE_WDR    1440

#ifdef __linux__
//#define IMX678_ISP_DEFAULT_SUPPORT
#endif

#define IMX678_RES_IS_1080P(w, h) ((w) == 3840 && (h) == 2160)

#define IMX678_ERR_MODE_PRINT(sensor_image_mode)                                                                       \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sensor_image_mode->width,       \
                     sensor_image_mode->height, sensor_image_mode->wdr_mode);                                          \
    } while (0)

#define IS_LINE_WDR_MODE(mode)      (((mode) == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) ||  \
        ((mode) == XMEDIA_VIDEO_WDR_MODE_3TO1_LINE) || ((mode) == XMEDIA_VIDEO_WDR_MODE_4TO1_LINE))

imx678_state_s g_imx678_state[XMEDIA_SENSOR_DEV_MAX_NUM]     = {{0}};
xmedia_isp_ae_wdr_mode g_wdr_mode[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = 0 };

sensor_context *g_imx678_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define IMX678_SET_CTX(dev, ctx) g_imx678_ctx[dev] = ctx

static xmedia_s32 g_imx678_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM]  =
                                                        { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };
static xmedia_u32 g_lines_per_500ms[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u32 g_imx678_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_imx678_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_imx678_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_imx678_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

// Rgain and Bgain of the golden sample
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

// AWB default parameter and function
#define CALIBRATE_STATIC_WB_R_GAIN  430
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  530

// Calibration results for Auto WB Planck
#define CALIBRATE_AWB_P1 (-27)
#define CALIBRATE_AWB_P2 283
#define CALIBRATE_AWB_Q1 0
#define CALIBRATE_AWB_A1 223452
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-169558)

#define GAIN_NODE_NUM  241
#define AGAIN_NODE_NUM 101
#define DGAIN_NODE_NUM 141

static const xmedia_u32 g_gain_table[GAIN_NODE_NUM] = {
    1024,  1059, 1097, 1135, 1175, 1217, 1259, 1304, 1349, 1397, 1446,  1497,  1549,  1604,
    1660,  1719, 1779, 1842, 1906, 1973, 2043, 2114, 2189, 2266, 2345,  2428,  2513,  2602,
    2693,  2788, 2886, 2987, 3092, 3201, 3313, 3430, 3550, 3675, 3804,  3938,  4076,  4219,
    4368,  4521, 4680, 4845, 5015, 5191, 5374, 5562, 5758, 5960, 6170,  6387,  6611,  6843,
    7084,  7333, 7591, 7857, 8134, 8419, 8715, 9022, 9339, 9667, 10007, 10358, 10722, 11099,
    11489, 11893, 12311, 12743, 13191, 13655, 14135, 14631, 15146, 15678, 16229, 16799, 17390, 18001,
    18633, 19288, 19966, 20668, 21394, 22146, 22924, 23730, 24564, 25427, 26320, 27245, 28203, 29194,
    30220, 31282, 32381, 33519, 34697, 35917, 37179, 38485, 39838, 41238, 42687, 44187, 45740, 47347,
    49011, 50734, 52517, 54362, 56272, 58250, 60297, 62416, 64610, 66880, 69231, 71663, 74182, 76789,
    79487, 82281, 85172, 88165, 91264, 94471, 97791, 101227, 104785, 108467, 112279, 116225, 120309,
    124537, 128913, 133444, 138133, 142988, 148013, 153214, 158599, 164172, 169941, 175913, 182095,
    188495, 195119, 201976, 209073, 216421, 224026, 231899, 240049, 248485, 257217, 266256, 275613,
    285298, 295324, 305703, 320110, 327567, 339078, 350994, 363329, 376097, 389314, 402995, 417157,
    431817, 446992, 462700, 478960, 495792, 513215, 531251, 549920, 569246, 589250, 609958, 631393,
    653581, 676550, 700325, 724936, 750412, 776783, 804081, 832338, 861588, 891866,  923208,  955652,
    989236, 1024000, 1059985, 1097235, 1135795, 1175709, 1217026, 1259795, 1304067, 1349894, 1397333,
    1446438, 1497269, 1549886, 1604353, 1660733, 1719095, 1779508, 1842043, 1906777, 1913785, 2043148,
    2114949, 2189273, 2266208, 2345848, 2428287, 2513622, 2601956, 2693394, 2788046, 2886024, 2987445,
    3092431, 3201105, 3313599, 3430046, 3550585, 3675361, 3804521, 3938220, 4076617
};

static const xmedia_sensor_property g_imx678_property[IMX678_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        3840, 2160, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_37_125MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_12,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB,
          XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, IMX678_8M_BIT_RATE_LINEAR,
    },
    {
        // width, height, wdr_mode
        3840, 2160, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_37_125MHZ,
        // output_intf, pixel_format,  bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB,
          XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, IMX678_8M_BIT_RATE_WDR,
    },
};

static const xmedia_sensor_capability g_imx678_capability = {
    .max_width  = 3840,
    .max_height = 2160,
    .max_fps    = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_37_125MHZ,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_DOL,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_4L,

    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = IMX678_ADDR_BYTE,
        .data_byte_num     = IMX678_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { 0x3000 },
        .standby_reg_data  = { 0x01 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { 0x3000 },
        .resume_reg_data   = { 0x00 },

    },
};

static xmedia_s32 imx678_ctx_init(xmedia_u32 dev)
{
    sensor_context *imx678_ctx = XMEDIA_NULL;

    IMX678_GET_CTX(dev, imx678_ctx);
    if (imx678_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    imx678_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (imx678_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(imx678_ctx, 0, sizeof(sensor_context));

    imx678_ctx->i2c_addr         = IMX678_I2C_ADDR;
    imx678_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_2L;
    imx678_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    imx678_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    imx678_ctx->size.width       = 3840;
    imx678_ctx->size.height      = 2160;
    imx678_ctx->fps = g_imx678_property[0].max_fps;
    imx678_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    imx678_ctx->img_mode             = IMX678_8M_12BIT_LINEAR_MODE;
    imx678_ctx->fl_std               = IMX678_VMAX_8M_LINEAR;
    imx678_ctx->fl[SENSOR_CUR_FRAME] = IMX678_VMAX_8M_LINEAR;
    imx678_ctx->fl[SENSOR_PRE_FRAME] = IMX678_VMAX_8M_LINEAR;
    IMX678_SET_CTX(dev, imx678_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_void imx678_ctx_exit(xmedia_u32 dev)
{
    sensor_context *imx678_ctx = XMEDIA_NULL;

    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_FREE(imx678_ctx);
    IMX678_SET_CTX(dev, XMEDIA_NULL);
}

static xmedia_s32 imx678_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    for (i = 0; i < IMX678_SPECS_MAX_NUM; i++) {
        if (g_imx678_property[i].width == imx678_ctx->size.width &&
            g_imx678_property[i].height == imx678_ctx->size.height &&
            g_imx678_property[i].wdr_mode == imx678_ctx->wdr_mode) {
            memcpy(property, &g_imx678_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }
    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n",
                    imx678_ctx->size.width, imx678_ctx->size.height, imx678_ctx->wdr_mode);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 imx678_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    imx678_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "No support mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    imx678_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *imx678_ctx      = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        imx678_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        imx678_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        imx678_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 imx678_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    imx678_ctx->i2c_addr = slave_addr;
    ret = imx678_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    *slave_addr = imx678_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}


static xmedia_s32 imx678_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    imx678_ctx->init_mode = init_mode;

    ret = imx678_i2c_init(dev,imx678_ctx->bus_info.i2c_dev, imx678_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

// new added
static xmedia_s32 imx678_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    ret = imx678_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;

}

static xmedia_s32 imx678_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    imx678_ctx->mirror_en = mirror_en;
    ret = imx678_set_mirror_flip(dev, mirror_en, imx678_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    imx678_ctx->flip_en = flip_en;
    ret = imx678_set_mirror_flip(dev, imx678_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_set_image_mode(sensor_context *imx678_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr,xmedia_u32 dev)
{
     SENSOR_CHECK_PTR_RETURN(sns_attr);
     SENSOR_CHECK_PTR_RETURN(image_mode);

    if (imx678_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (IMX678_RES_IS_1080P(sns_attr->width, sns_attr->height)) {
            *image_mode           = IMX678_8M_12BIT_LINEAR_MODE;
             imx678_ctx->fl_std   = IMX678_VMAX_8M_LINEAR;
             g_imx678_state[dev].hcg = 0x1;

        } else {
            IMX678_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else if (imx678_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (IMX678_RES_IS_1080P(sns_attr->width, sns_attr->height)) {
            *image_mode           = IMX678_8M_10BIT_WDR_MODE;
             imx678_ctx->fl_std   = IMX678_VMAX_8M_WDR * 2;
             g_imx678_state[dev].hcg = 0x1;
             g_imx678_state[dev].brl = 2210;

        } else {
            IMX678_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        IMX678_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    imx678_ctx->size.width  = sns_attr->width;
    imx678_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context  *imx678_ctx = XMEDIA_NULL;

    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            imx678_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            imx678_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
            SENSOR_PRINT("2to1 line WDR 1080p mode(60fps->30fps)\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }
    memset(imx678_ctx->wdr_int_time, 0, sizeof(imx678_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

static xmedia_void imx678_init_common_reg_info(sensor_context *imx678_ctx)
{
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_SHR0_H].delay_frame_num = 2;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_SHR0_H].reg_addr        = IMX678_REG_ADDR_SHR0_H;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_SHR0_M].delay_frame_num = 2;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_SHR0_M].reg_addr        = IMX678_REG_ADDR_SHR0_M;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_SHR0_L].delay_frame_num = 2;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_SHR0_L].reg_addr        = IMX678_REG_ADDR_SHR0_L;

    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_GAIN].delay_frame_num = 2;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_GAIN].reg_addr        = IMX678_REG_ADDR_GAIN;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_HCG].delay_frame_num  = 1;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_HCG].reg_addr         = IMX678_REG_ADDR_HCG;

    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_VMAX_H].delay_frame_num = 2;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_VMAX_H].reg_addr        = IMX678_REG_ADDR_VMAX_H;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_VMAX_M].delay_frame_num = 2;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_VMAX_M].reg_addr        = IMX678_REG_ADDR_VMAX_M;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_VMAX_L].delay_frame_num = 2;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_VMAX_L].reg_addr        = IMX678_REG_ADDR_VMAX_L;

    return;
}

static xmedia_void imx678_init_2to1_wdr_reg_info(sensor_context *imx678_ctx)
{
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_SHR1_H].delay_frame_num = 2;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_SHR1_M].delay_frame_num = 2;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_SHR1_L].delay_frame_num = 2;

    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_SHR1_H].delay_frame_num = 2;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_SHR1_H].reg_addr        = IMX678_REG_ADDR_SHR1_H;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_SHR1_M].delay_frame_num = 2;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_SHR1_M].reg_addr        = IMX678_REG_ADDR_SHR1_M;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_SHR1_L].delay_frame_num = 2;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_SHR1_L].reg_addr        = IMX678_REG_ADDR_SHR1_L;

    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_RHS1_H].delay_frame_num = 2;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_RHS1_H].reg_addr        = IMX678_REG_ADDR_RHS1_H;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_RHS1_M].delay_frame_num = 2;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_RHS1_M].reg_addr        = IMX678_REG_ADDR_RHS1_M;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_RHS1_L].delay_frame_num = 2;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_RHS1_L].reg_addr        = IMX678_REG_ADDR_RHS1_L;

    imx678_ctx->regs_info[0].i2c_data[IMX678_REG__HCG_SEL1].delay_frame_num = 1;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG__HCG_SEL1].reg_addr        = IMX678_REG_ADDR_HCG_SEL1;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG__HCG_SEL2].delay_frame_num = 1;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG__HCG_SEL2].reg_addr        = IMX678_REG_ADDR_HCG_SEL2;

    return;
}

static xmedia_s32 imx678_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *imx678_ctx  = XMEDIA_NULL;

    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    imx678_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = imx678_ctx->bus_info.i2c_dev;
    imx678_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    imx678_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = IMX678_LINEAR_REG_INFO_MAX_NUM;

    if (imx678_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        imx678_ctx->regs_info[0].reg_num = IMX678_2TO1_WDR_REG_INFO_MAX_NUM;
        imx678_ctx->regs_info[0].cfg_to_valid_delay_max = 2;
    }

    for (i = 0; i < imx678_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = imx678_ctx->i2c_addr;
        imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = IMX678_ADDR_BYTE;
        imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = IMX678_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    imx678_init_common_reg_info(imx678_ctx);

    // init 2to1 wdr mode Regs
    if (imx678_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        imx678_init_2to1_wdr_reg_info(imx678_ctx);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    // set wdr mode
    ret = imx678_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // set image mode
    ret = imx678_set_image_mode(imx678_ctx, &imx678_ctx->img_mode, sns_attr,dev);
    SENSOR_CHECK_RET_RETURN(ret);

    imx678_ctx->fl[SENSOR_CUR_FRAME] = imx678_ctx->fl_std;
    imx678_ctx->fl[SENSOR_PRE_FRAME] = imx678_ctx->fl[SENSOR_CUR_FRAME];

    ret = imx678_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
        SENSOR_CHECK_DEV_RETURN(dev);
        SENSOR_CHECK_PTR_RETURN(isp_default);

        memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef IMX678_ISP_DEFAULT_SUPPORT
        //TO-DO: alg add param
        isp_default->blc         = XMEDIA_NULL;
        isp_default->bnr         = XMEDIA_NULL;
        isp_default->clut_attr   = XMEDIA_NULL;
        isp_default->clut_lut    = XMEDIA_NULL;
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
        SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from imx678!\n ");

        return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 imx678_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    xmedia_u32      i;
    sensor_context *imx678_ctx = XMEDIA_NULL;
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (imx678_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) { // black level of linear mode
        for (i = 0; i < 4; i++) {
            black_level->black_level[i] = 200;
        }
    } else { // black level of DOL mode
        black_level->black_level[0] = 200;
        black_level->black_level[1] = 200;
        black_level->black_level[2] = 200;
        black_level->black_level[3] = 200;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                 *imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    if (imx678_ctx->mirror_en && imx678_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (imx678_ctx->mirror_en && (!imx678_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!imx678_ctx->mirror_en) && imx678_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < IMX678_SPECS_MAX_NUM; i++) {
        if (g_imx678_property[i].width == imx678_ctx->size.width &&
            g_imx678_property[i].height == imx678_ctx->size.height &&
            g_imx678_property[i].wdr_mode == imx678_ctx->wdr_mode) {
            *bayer_pattern = g_imx678_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= IMX678_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_imx678_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    imx678_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 imx678_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_imx678_exposure[dev]        = init_param->exposure;
    g_imx678_sample_r_gain[dev]   = init_param->sample_rgain;
    g_imx678_sample_b_gain[dev]   = init_param->sample_bgain;

    g_imx678_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_imx678_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_imx678_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                      xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *imx678_ctx = XMEDIA_NULL;
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ae_sns_dft->full_lines_std = imx678_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256;
    ae_sns_dft->full_lines_max = IMX678_FULL_LINES_MAX;

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->dgain_accu.accuracy  = 1;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 2 << ae_sns_dft->ispdgain_shift;


    memcpy(&ae_sns_dft->piris_attr, &g_piris_attr, sizeof(xmedia_isp_piris_attr));
    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_32_0;

    ae_sns_dft->hmax_times = (1000000000) / (imx678_ctx->fl_std * 30);

    ae_sns_dft->ae_route_ex_valid          = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num    = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    if (g_lines_per_500ms[dev] == 0) {
        ae_sns_dft->lines_per_500ms = imx678_ctx->fl_std * 30 / 2;
    } else {
        ae_sns_dft->lines_per_500ms = g_lines_per_500ms[dev];
    }

    return XMEDIA_SUCCESS;

}

static xmedia_void imx678_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                         xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 2;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 32381;
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 128914;
    ae_sns_dft->min_dgain        = 1024;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 255 << ae_sns_dft->ispdgain_shift;

    ae_sns_dft->ae_compensation = 0x38;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure = g_imx678_exposure[dev] ? g_imx678_exposure[dev] : 76151;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - 6;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time        = 2;
    ae_sns_dft->min_int_time_target = 1;

    ae_sns_dft->ae_route_ex_valid   = XMEDIA_FALSE;

    return;
}

 static xmedia_s32 imx678_get_ae_2to1_wdr_line_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                                     xmedia_isp_ae_sensor_default *ae_sns_dft)
 {
 ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 2;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_int_time = sns_ctx->fl_std - 2;
    ae_sns_dft->min_int_time = 2;

    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    ae_sns_dft->max_again        = 32381;
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 128914;
    ae_sns_dft->min_dgain        = 1024;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;
    ae_sns_dft->max_ispdgain_target = 16 << ae_sns_dft->ispdgain_shift;

    ae_sns_dft->init_exposure = g_imx678_exposure[dev] ? g_imx678_exposure[dev] : 52000;

    if (g_wdr_mode[dev] == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
        ae_sns_dft->ae_compensation = 64;
        ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    } else {
        ae_sns_dft->ae_compensation = 32;
        ae_sns_dft->ae_exp_mode         = XMEDIA_ISP_AE_STRATEGY_EXP_LOWLIGHT_PRIOR;
        ae_sns_dft->man_ratio_enable    = XMEDIA_FALSE;
        ae_sns_dft->ratio[0]            = 0x200;
        ae_sns_dft->ratio[1]            = 0x400;
    }

    return XMEDIA_SUCCESS;
 }

static xmedia_s32 imx678_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);
    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = imx678_get_ae_common_default(dev, imx678_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (imx678_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            imx678_get_ae_linear_default(dev, imx678_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            imx678_get_ae_2to1_wdr_line_default(dev, imx678_ctx, ae_sns_dft);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", imx678_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context *imx678_ctx = XMEDIA_NULL;
    xmedia_s32 value = 0;
    xmedia_u32 shr0, shr1, rhs1;
    static xmedia_u32 count[XMEDIA_SENSOR_DEV_MAX_NUM] = {[0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 0};

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    if (imx678_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (count[dev] == 0) { // long exposure
            imx678_ctx->wdr_int_time[0] = int_time;
            count[dev]++;
        } else if (count[dev] == 1) { // short exposure
            imx678_ctx->wdr_int_time[1] = int_time;

            shr0 = imx678_ctx->fl[1] - int_time;
            shr0=(shr0 % 2 == 0) ? shr0 : shr0 + 2 - (shr0) % 2;
            shr1 = 5;
            rhs1 = shr1 + imx678_ctx->wdr_int_time[0];
            rhs1 = ((rhs1 - 1) % 4 == 0) ? rhs1 : rhs1 + 4 - (rhs1 - 1) % 4;
            count[dev] = 0;

            imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX678_REG_SHR0_H].data = SENSOR_LOW_8BITS(shr0);
            imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX678_REG_SHR0_M].data = SENSOR_HIGH_8BITS(shr0);
            imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX678_REG_SHR0_L].data = SENSOR_HIGHER_16BITS(shr0);
            imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX678_REG_SHR1_H].data = SENSOR_LOW_8BITS(shr1);
            imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX678_REG_SHR1_M].data = SENSOR_HIGH_8BITS(shr1);
            imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX678_REG_SHR1_L].data = SENSOR_HIGHER_16BITS(shr1);
            imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX678_REG_RHS1_H].data = SENSOR_LOW_8BITS(rhs1);
            imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX678_REG_RHS1_M].data = SENSOR_HIGH_8BITS(rhs1);
            imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX678_REG_RHS1_L].data = SENSOR_HIGHER_16BITS(rhs1);
        }
    } else {
        value = imx678_ctx->fl[0] - int_time + int_time % 2;
        value = SENSOR_MIN(value, 0xFFFFF);
        value = SENSOR_MIN(SENSOR_MAX(value, 6), imx678_ctx->fl[0] - 2);
        imx678_ctx->regs_info[0].i2c_data[IMX678_REG_SHR0_H].data = SENSOR_LOW_8BITS(value);
        imx678_ctx->regs_info[0].i2c_data[IMX678_REG_SHR0_M].data = SENSOR_HIGH_8BITS(value);
        imx678_ctx->regs_info[0].i2c_data[IMX678_REG_SHR0_L].data = SENSOR_HIGHER_16BITS(value);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_u32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_gain_table[AGAIN_NODE_NUM - 1]) {
        *again_lin = g_gain_table[AGAIN_NODE_NUM - 1];
        *again_db  = AGAIN_NODE_NUM - 1;
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < AGAIN_NODE_NUM; i++) {
        if (*again_lin < g_gain_table[i]) {
            *again_lin = g_gain_table[i - 1];
            *again_db  = i - 1;
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    xmedia_s32 i;
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(dgain_lin);
    SENSOR_CHECK_PTR_RETURN(dgain_db);

    if (*dgain_lin >= g_gain_table[DGAIN_NODE_NUM - 1]) {
        *dgain_lin = g_gain_table[DGAIN_NODE_NUM - 1];
        *dgain_db  = DGAIN_NODE_NUM - 1;
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < DGAIN_NODE_NUM; i++) {
        if (*dgain_lin < g_gain_table[i]) {
            *dgain_lin = g_gain_table[i - 1];
            *dgain_db  = i - 1;
            break;
        }
    }

    return XMEDIA_SUCCESS;
}
static xmedia_s32 imx678_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *imx678_ctx = XMEDIA_NULL;
    xmedia_u32 hcg = g_imx678_state[dev].hcg;
    xmedia_u32 tmp;

    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    if (again >= 30) { // HCG 27
        hcg = 0x1;
        again = again - 30; // again sub 30
    } else {
        hcg = 0x0;
    }

    tmp = again + dgain;

    if (imx678_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        imx678_ctx->regs_info[0].i2c_data[IMX678_REG_GAIN].data = (tmp & 0xFF);
        imx678_ctx->regs_info[0].i2c_data[IMX678_REG_HCG].data = (hcg & 0xFF);
    } else if (imx678_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        imx678_ctx->regs_info[0].i2c_data[IMX678_REG_GAIN].data = (tmp & 0xFF);
        imx678_ctx->regs_info[0].i2c_data[IMX678_REG_HCG].data = (hcg & 0xFF);
        imx678_ctx->regs_info[0].i2c_data[IMX678_REG__HCG_SEL1].data = (hcg & 0xFF);
        imx678_ctx->regs_info[0].i2c_data[IMX678_REG__HCG_SEL2].data = (hcg & 0xFF);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context  *imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = 4850;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = CALIBRATE_STATIC_WB_B_GAIN;

    awb_sns_dft->wb_para[0]   = CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1]   = CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2]   = CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3]   = CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4]   = CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5]   = -CALIBRATE_AWB_C1;
    awb_sns_dft->golden_rgain = GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (imx678_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_awb_ccm_wdr, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_awb_agc_table_wdr, sizeof(xmedia_isp_awb_agc_table));
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", imx678_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_imx678_wb_gain[dev][0];
    awb_sns_dft->init_ggain   = g_imx678_wb_gain[dev][1];
    awb_sns_dft->init_bgain   = g_imx678_wb_gain[dev][2];
    awb_sns_dft->sample_rgain = g_imx678_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_imx678_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx678_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case IMX678_8M_12BIT_LINEAR_MODE:
            *max_fps = 30;
            break;
        case IMX678_8M_10BIT_WDR_MODE:
            *max_fps = 30;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx678_standby(xmedia_u32 dev)
{
    imx678_write_reg(dev, 0x3000, 0x01); // STANDBY
    imx678_write_reg(dev, 0x3002, 0x01); // XTMSTA

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx678_resume(xmedia_u32 dev)
{
    imx678_write_reg(dev, 0x3000, 0x00); // standby
    imx678_delay_ms(20);
    imx678_write_reg(dev, 0x3002, 0x00); // master mode start

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *imx678_ctx   = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < imx678_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < imx678_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= imx678_write_reg(dev, imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx678_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    if (imx678_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = imx678_init_image(dev, imx678_ctx->img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    imx678_delay_ms(10);
    ret = imx678_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = imx678_set_mirror_flip(dev, imx678_ctx->mirror_en, imx678_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx678_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = imx678_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_imx678_dev_map[index] == SENSOR_DEV_INVALID) {
            g_imx678_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_FAILURE;
}

static xmedia_s32 imx678_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_imx678_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

#define IMX678_MARGIN 40
static xmedia_s32 imx678_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{   // TO-DO：min fps is to be confirmed.
    switch (img_mode) {

        case IMX678_8M_12BIT_LINEAR_MODE:
            *min_fps = 0.8;
            break;
        case IMX678_8M_10BIT_WDR_MODE:
            *min_fps = 10;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }
    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {

        case IMX678_8M_12BIT_LINEAR_MODE:
            *vmax = IMX678_VMAX_8M_LINEAR;
            break;
        case IMX678_8M_10BIT_WDR_MODE:
            *vmax = IMX678_VMAX_8M_WDR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context* imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    for (i = 0; i < imx678_ctx->regs_info[0].reg_num; i++) {
        if (imx678_ctx->regs_info[0].i2c_data[i].data == imx678_ctx->regs_info[1].i2c_data[i].data) {
            imx678_ctx->regs_info[0].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            imx678_ctx->regs_info[0].i2c_data[i].update = XMEDIA_TRUE;
        }
    }
    memcpy(sns_regs_info, &imx678_ctx->regs_info[0], sizeof(xmedia_sensor_regs_info));
    memcpy(&imx678_ctx->regs_info[1], &imx678_ctx->regs_info[0], sizeof(xmedia_sensor_regs_info));

    imx678_ctx->fl[1] = imx678_ctx->fl[0];
    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_calc_fps(xmedia_float fps, sensor_context *imx678_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = imx678_get_max_fps(imx678_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = imx678_get_min_fps(imx678_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = imx678_get_vmax(imx678_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is (%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);

    if (imx678_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE){
        *full_lines          = (*full_lines > IMX678_FULL_LINES_MAX) ? IMX678_FULL_LINES_MAX : *full_lines;
        *full_lines          = *full_lines + (*full_lines % 2);
        imx678_ctx->fl_std   = *full_lines;
    } else if (imx678_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE){
        *full_lines = (*full_lines > IMX678_FULL_LINES_MAX_2TO1_WDR) ? IMX678_FULL_LINES_MAX_2TO1_WDR : *full_lines;
        *full_lines        = *full_lines + (*full_lines % 2);
        imx678_ctx->fl_std = *full_lines * 2;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    ret = imx678_calc_fps(fps, imx678_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX678_REG_VMAX_H].data =  SENSOR_LOW_8BITS(full_lines);
    imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX678_REG_VMAX_M].data =  SENSOR_HIGH_8BITS(full_lines);
    imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX678_REG_VMAX_L].data =  SENSOR_HIGHER_16BITS(full_lines);

    imx678_ctx->fl[SENSOR_CUR_FRAME]  = imx678_ctx->fl_std;
    imx678_ctx->fps                   = fps;
    ae_sns_dft->fps                   = fps;
    ae_sns_dft->full_lines_std        = imx678_ctx->fl_std;
    ae_sns_dft->max_int_time          = imx678_ctx->fl_std - 2;
    ae_sns_dft->full_lines            = imx678_ctx->fl[SENSOR_CUR_FRAME];

    SENSOR_PRINT("dev[%d]- imx678 set fps: %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_void cmos_2to1_vmax_limit(xmedia_u32 *vmax, xmedia_u32 full_line, xmedia_u32 step, xmedia_bool fps_up)
{
    if (fps_up) {
        if ((*vmax) + step < full_line) {
            (*vmax) = (full_line - step);
            return;
        }
    } else {
        if ((*vmax) > full_line + step) {
            (*vmax) = (full_line + step);
            return;
        }
    }
}

static xmedia_s32 imx678_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                                        xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    xmedia_u32      vmax;
    xmedia_float    max_fps;
    xmedia_u32      vmax_max_fps;
    sensor_context *imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    if (imx678_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", imx678_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = imx678_get_max_fps(imx678_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = imx678_get_vmax(imx678_ctx->img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    if (imx678_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        vmax              = full_lines / 2;
        cmos_2to1_vmax_limit(&vmax, imx678_ctx->fl[0], 20, full_lines < imx678_ctx->fl[0]);
        vmax              = vmax + vmax % 2;
        vmax              = (vmax > IMX678_FULL_LINES_MAX_2TO1_WDR) ? IMX678_FULL_LINES_MAX_2TO1_WDR : vmax;
        imx678_ctx->fl[0] = vmax * 2;
        g_imx678_state[dev].rhs2_max = imx678_ctx->fl[0] - g_imx678_state[dev].brl * 2;
    } else {
        vmax              = full_lines;
        vmax              = (vmax > IMX678_FULL_LINES_MAX) ? IMX678_FULL_LINES_MAX : vmax;
        vmax              = vmax + vmax % 2;
        imx678_ctx->fl[0] = vmax;
    }

    imx678_ctx->fps                                           = max_fps * vmax_max_fps / vmax;
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_VMAX_H].data = SENSOR_LOW_8BITS(vmax);
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_VMAX_M].data = SENSOR_HIGH_8BITS(vmax);
    imx678_ctx->regs_info[0].i2c_data[IMX678_REG_VMAX_L].data = SENSOR_HIGHER_16BITS(vmax);

    ae_sns_dft->full_lines   = imx678_ctx->fl[0];
    ae_sns_dft->max_int_time = imx678_ctx->fl[0] - 2;

    SENSOR_PRINT("dev[%d]- imx678 set fps: %f\n", dev, imx678_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    xmedia_u32 short_max0           = 0;
    xmedia_u32 short_max            = 0;
    xmedia_u32 short_time_min_limit = 2;
    sensor_context* imx678_ctx      = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    if (imx678_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (imx678_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
            short_max0 = imx678_ctx->fl[1] - IMX678_MARGIN - imx678_ctx->wdr_int_time[0];
            short_max = imx678_ctx->fl[0] - IMX678_MARGIN;
            short_max = (short_max0 < short_max) ? short_max0 : short_max;
            inttime_attr->max_inttime[0] = short_time_min_limit;
            inttime_attr->min_inttime[0] = short_time_min_limit;
            inttime_attr->max_inttime[1] = short_max;
            inttime_attr->min_inttime[1] = short_time_min_limit;
        } else {
            short_max0 = ((imx678_ctx->fl[1] - IMX678_MARGIN  - imx678_ctx->wdr_int_time[0]) * 0x40) /
                    SENSOR_DIV_0_TO_1(inttime_attr->ratio[0]);
            short_max  = ((imx678_ctx->fl[0] - IMX678_MARGIN) * 0x40) / (inttime_attr->ratio[0] + 0x40);
            short_max  = (short_max0 < short_max) ? short_max0 : short_max;
            short_max  = (short_max == 0) ? 1 : short_max;

            if (short_max >= short_time_min_limit) {
                inttime_attr->max_inttime[0] = short_max;
                inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * inttime_attr->ratio[0]) >> 6;
                inttime_attr->min_inttime[0] = short_time_min_limit;
                inttime_attr->min_inttime[1] = (inttime_attr->min_inttime[0] * inttime_attr->ratio[0]) >> 6;
            } else {
                short_max = short_time_min_limit;
                inttime_attr->max_inttime[0] = short_max;
                inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * 0xFFF) >> 6;
                inttime_attr->min_inttime[0] = inttime_attr->max_inttime[0];
                inttime_attr->min_inttime[1] = inttime_attr->max_inttime[1];
            }
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *imx678_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    ret = imx678_get_min_fps(imx678_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = imx678_get_max_fps(imx678_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = imx678_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx678_init_register_func(xmedia_sensor_register_info* info)
{
    SENSOR_CHECK_PTR_RETURN(info);

    memcpy(info->sensor_name, IMX678_NAME, sizeof(IMX678_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property      = imx678_get_property;
    info->isp_func.pfn_sensor_set_work_mode     = imx678_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes    = imx678_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror            = imx678_mirror;
    info->isp_func.pfn_sensor_flip              = imx678_flip;
    info->isp_func.pfn_sensor_set_bus_info      = imx678_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr      = imx678_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param    = imx678_set_init_param;
    info->isp_func.pfn_sensor_start             = imx678_start;
    info->isp_func.pfn_sensor_stop              = imx678_stop;
    info->isp_func.pfn_sensor_standby           = imx678_standby;
    info->isp_func.pfn_sensor_resume            = imx678_resume;
    info->isp_func.pfn_sensor_write_reg         = imx678_write_reg;
    info->isp_func.pfn_sensor_read_reg          = imx678_read_reg;
    info->isp_func.pfn_sensor_init              = imx678_init;
    info->isp_func.pfn_sensor_exit              = imx678_exit;
    info->isp_func.pfn_sensor_set_attr          = imx678_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = imx678_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = imx678_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = imx678_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = imx678_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = imx678_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = imx678_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = imx678_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = imx678_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = imx678_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = imx678_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = imx678_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = imx678_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = imx678_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = imx678_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = imx678_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = imx678_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = imx678_get_awb_default;

    return XMEDIA_SUCCESS;

}

xmedia_s32 imx678_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = imx678_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = imx678_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret = imx678_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "IMX678 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx678_unregister(xmedia_u32 pipe)
{
    xmedia_s32 ret;
    xmedia_u32 dev;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = imx678_match_dev(pipe, &dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_PRINT("IMX678 need not unregister function!\n");
        return XMEDIA_SUCCESS;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "IMX678 unregister function failed!\n");
        return XMEDIA_FAILURE;
    }

    imx678_ctx_exit(dev);
    g_imx678_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;
}
