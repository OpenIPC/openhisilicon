#include <stdlib.h>
#include <string.h>
#include "imx415.h"
#include "imx415_ctrl.h"
#include "imx415_ex.h"

#define IMX415_NAME "IMX415"
#define IMX415_SPECS_MAX_NUM 1

#define IMX415_8M_BIT_RATE_LINEAR 891
#define IMX415_8M_BIT_RATE_WDR    1440

#ifdef __linux__
#define IMX415_ISP_DEFAULT_SUPPORT
#endif

#define IMX415_RES_IS_2160P(w, h) ((w) == 3840 && (h) == 2160)

#define IMX415_ERR_MODE_PRINT(sensor_image_mode)                                                                       \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sensor_image_mode->width,       \
                     sensor_image_mode->height, sensor_image_mode->wdr_mode);                                          \
    } while (0)

xmedia_isp_ae_wdr_mode g_imx415_wdr_mode[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = 0 };

sensor_context*g_imx415_ctx[XMEDIA_SENSOR_DEV_MAX_NUM]  = { XMEDIA_NULL };
#define IMX415_GET_CTX(dev, ctx) ctx = g_imx415_ctx[dev]
#define IMX415_SET_CTX(dev, ctx) g_imx415_ctx[dev] = ctx

static xmedia_s32 g_imx415_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM]  =
                                                           {[0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID};

static xmedia_u32 g_imx415_again[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_imx415_dgain[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_imx415_isp_dgain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_imx415_init_time[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u32 g_imx415_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]        = { 0 };
static xmedia_u16 g_imx415_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM]   = { 0 };
static xmedia_u16 g_imx415_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM]   = { 0 };

static xmedia_u16 g_imx415_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

/* AWB default parameter and function */
#define CALIBRATE_STATIC_WB_R_GAIN  445
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  669

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1 100
#define CALIBRATE_AWB_P2 37
#define CALIBRATE_AWB_Q1 (-118)
#define CALIBRATE_AWB_A1 204157
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-140493)

#define GAIN_NODE_NUM    241
#define AGAIN_NODE_NUM   101
#define DGAIN_NODE_NUM   141

static const xmedia_u32 g_gain_table[GAIN_NODE_NUM] = {
    1024, 1059, 1097, 1135, 1175, 1217, 1259, 1304, 1349, 1397, 1446, 1497, 1549, 1604, // 3.9dB
    1660, 1719, 1779, 1842, 1906, 1973, 2043, 2114, 2189, 2266, 2345, 2428, 2513, 2602, // 8.1dB
    2693, 2788, 2886, 2987, 3092, 3201, 3313, 3430, 3550, 3675, 3804, 3938, 4076, 4219, // 12.3dB
    4368, 4521, 4680, 4845, 5015, 5191, 5374, 5562, 5758, 5960, 6170, 6387, 6611, 6843, // 16.5dB

    7084,  7333,  7591, 7857,  8134, 8419,  8715,  9022, 9339,  9667, 10007, 10358, // 20.1dB
    10722, 11099, 11489, 11893, 12311, 12743, 13191, 13655, 14135, 14631, 15146, 15678, // 23.7dB
    16229, 16799, 17390, 18001, 18633, 19288, 19966, 20668, 21394, 22146, 22924, 23730, // 27.3dB
    24564, 25427, 26320, 27245, 28203, 29194, 30220, 31282, 32381, 33519, 34697, 35917, // 30.9dB
    37179, 38485, 39838, 41238, 42687, 44187, 45740, 47347, 49011, 50734, 52517, 54362, // 34.5dB
    56272, 58250, 60297, 62416, 64610, 66880, 69231, 71663, 74182, 76789, 79487, 82281, // 38.1dB

    85172,  88165,  91264,  94471, 97791, 101227, 104785, 108467, 112279, 116225, 120309, 124537,// 41.7dB
    128913, 133444, 138133, 142988, 148013, 153214, 158599, 164172, 169941, 175913, 182095, 188495,// 45.3dB
    195119, 201976, 209073, 216421, 224026, 231899, 240049, 248485, 257217, 266256, 275613, 285298,// 48.9dB
    295324, 305703, 320110, 327567, 339078, 350994, 363329, 376097, 389314, 402995, 417157, 431817,// 52.5dB
    446992, 462700, 478960, 495792, 513215, 531251, 549920, 569246, 589250, 609958, 631393, 653581,// 56.1dB
    676550, 700325, 724936, 750412, 776783, 804081, 832338, 861588, 891866, 923208, 955652, 989236,// 59.7dB

    1024000, 1059985, 1097235, 1135795, 1175709, 1217026, 1259795, 1304067, 1349894, 1397333, // 62.7dB
    1446438, 1497269, 1549886, 1604353, 1660733, 1719095, 1779508, 1842043, 1906777, 1913785, // 65.7dB
    2043148, 2114949, 2189273, 2266208, 2345848, 2428287, 2513622, 2601956, 2693394, 2788046, // 68.7dB
    2886024, 2987445, 3092431, 3201105, 3313599, 3430046, 3550585, 3675361, 3804521, 3938220, 4076617// 72.0dB
};

static const xmedia_sensor_property g_imx415_property[IMX415_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        3840, 2160, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_NONE, 30, XMEDIA_SENSOR_CLOCK_FREQ_37_125MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_GBRG, XMEDIA_VIDEO_BAYER_FMT_GBRG,
          XMEDIA_VIDEO_BAYER_FMT_GBRG, XMEDIA_VIDEO_BAYER_FMT_GBRG },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, IMX415_8M_BIT_RATE_LINEAR,
    },
/* not support wdr yet.
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
        { 0, 1 }, IMX415_8M_BIT_RATE_WDR,
    },
*/
};

static const xmedia_sensor_capability g_imx415_capability = {
    .max_width  = 3840,
    .max_height = 2160,
    .max_fps    = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_37_125MHZ,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_4L,

    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = IMX415_ADDR_BYTE,
        .data_byte_num     = IMX415_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { 0x3000 },
        .standby_reg_data  = { 0x01 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { 0x3000 },
        .resume_reg_data   = { 0x00 },
    },
};

static xmedia_s32 imx415_ctx_init(xmedia_u32 dev)
{
    sensor_context *imx415_ctx = XMEDIA_NULL;

    IMX415_GET_CTX(dev, imx415_ctx);

    if (imx415_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    imx415_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (imx415_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(imx415_ctx, 0, sizeof(sensor_context));
    imx415_ctx->i2c_addr         = IMX415_I2C_ADDR;
    imx415_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_2L;
    imx415_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    imx415_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    imx415_ctx->size.width       = 3840;
    imx415_ctx->size.height      = 2160;
    imx415_ctx->fps              = g_imx415_property[0].max_fps;
    imx415_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    imx415_ctx->img_mode             = IMX415_8M_10BIT_LINEAR_MODE;
    imx415_ctx->fl_std               = IMX415_VMAX_8M_LINEAR;
    imx415_ctx->fl[SENSOR_CUR_FRAME] = IMX415_VMAX_8M_LINEAR;
    imx415_ctx->fl[SENSOR_PRE_FRAME] = IMX415_VMAX_8M_LINEAR;
    IMX415_SET_CTX(dev, imx415_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_void imx415_ctx_exit(xmedia_u32 dev)
{
    sensor_context *imx415_ctx = XMEDIA_NULL;

    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_FREE(imx415_ctx);
    IMX415_SET_CTX(dev, XMEDIA_NULL);
}

static xmedia_s32 imx415_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    for (i = 0; i < IMX415_SPECS_MAX_NUM; i++) {
        if (g_imx415_property[i].width    == imx415_ctx->size.width &&
            g_imx415_property[i].height   == imx415_ctx->size.height &&
            g_imx415_property[i].wdr_mode == imx415_ctx->wdr_mode) {
            memcpy(property, &g_imx415_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n",
                 imx415_ctx->size.width, imx415_ctx->size.height, imx415_ctx->wdr_mode);
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 imx415_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    imx415_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "not supported mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    imx415_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        imx415_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        imx415_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        imx415_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;
    sensor_context *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    imx415_ctx->i2c_addr = slave_addr;
    ret = imx415_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    *slave_addr = imx415_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}


static xmedia_s32 imx415_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32     ret;
    sensor_context *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    imx415_ctx->init_mode = init_mode;
    ret = imx415_i2c_init(dev, imx415_ctx->bus_info.i2c_dev, imx415_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;
    sensor_context *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    ret = imx415_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32 ret;
    sensor_context *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    imx415_ctx->mirror_en = mirror_en;
    ret = imx415_set_mirror_flip(dev, mirror_en, imx415_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    sensor_context *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    imx415_ctx->flip_en = flip_en;
    ret = imx415_set_mirror_flip(dev, imx415_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_set_image_mode(sensor_context *imx415_ctx, xmedia_u8 *image_mode,
                                                 const xmedia_sensor_attr *sns_attr)
{
     SENSOR_CHECK_PTR_RETURN(sns_attr);
     SENSOR_CHECK_PTR_RETURN(image_mode);

    if (imx415_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (IMX415_RES_IS_2160P(sns_attr->width, sns_attr->height)) {
            *image_mode           = IMX415_8M_10BIT_LINEAR_MODE;
             imx415_ctx->fl_std   = IMX415_VMAX_8M_LINEAR;

             return XMEDIA_SUCCESS;
        }
    }
//    else if (imx415_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
//        if (IMX415_RES_IS_2160P(sns_attr->width, sns_attr->height)) {
//            *image_mode           = IMX415_8M_10BIT_WDR_MODE;
//             imx415_ctx->fl_std   = IMX415_VMAX_8M_WDR;
//
//             return XMEDIA_SUCCESS;
//        }
//    }

    IMX415_ERR_MODE_PRINT(sns_attr);
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 imx415_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context  *imx415_ctx = XMEDIA_NULL;

    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            imx415_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
//            imx415_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
//            SENSOR_PRINT("2to1 line WDR 1080p mode(60fps->30fps)\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }
    memset(imx415_ctx->wdr_int_time, 0, sizeof(imx415_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

static xmedia_void imx415_init_common_reg_info(sensor_context *imx415_ctx)
{
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR0_H].delay_frame_num = 2;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR0_H].reg_addr        = IMX415_REG_ADDR_SHR0_H;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR0_M].delay_frame_num = 2;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR0_M].reg_addr        = IMX415_REG_ADDR_SHR0_M;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR0_L].delay_frame_num = 2;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR0_L].reg_addr        = IMX415_REG_ADDR_SHR0_L;

    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_GAIN].delay_frame_num   = 2;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_GAIN].reg_addr          = IMX415_REG_ADDR_GAIN;

    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_VMAX_H].delay_frame_num = 2;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_VMAX_H].reg_addr        = IMX415_REG_ADDR_VMAX_H;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_VMAX_M].delay_frame_num = 2;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_VMAX_M].reg_addr        = IMX415_REG_ADDR_VMAX_M;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_VMAX_L].delay_frame_num = 2;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_VMAX_L].reg_addr        = IMX415_REG_ADDR_VMAX_L;
}

static xmedia_void imx415_init_2to1_wdr_reg_info(sensor_context *imx415_ctx)
{
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR1_H].delay_frame_num = 2;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR1_H].reg_addr        = IMX415_REG_ADDR_SHR1_H;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR1_M].delay_frame_num = 2;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR1_M].reg_addr        = IMX415_REG_ADDR_SHR1_M;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR1_L].delay_frame_num = 2;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR1_L].reg_addr        = IMX415_REG_ADDR_SHR1_L;

    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_RHS1_H].delay_frame_num = 2;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_RHS1_H].reg_addr        = IMX415_REG_ADDR_RHS1_H;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_RHS1_M].delay_frame_num = 2;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_RHS1_M].reg_addr        = IMX415_REG_ADDR_RHS1_M;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_RHS1_L].delay_frame_num = 2;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_RHS1_L].reg_addr        = IMX415_REG_ADDR_RHS1_L;
}

static xmedia_s32 imx415_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32     i;
    sensor_context *imx415_ctx  = XMEDIA_NULL;

    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    imx415_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = imx415_ctx->bus_info.i2c_dev;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2;
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = IMX415_LINEAR_REG_INFO_MAX_NUM;

    if (imx415_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        imx415_ctx->regs_info[SENSOR_CUR_FRAME].reg_num = IMX415_2TO1_WDR_REG_INFO_MAX_NUM;
        imx415_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2;
    }

    for (i = 0; i < imx415_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = imx415_ctx->i2c_addr;
        imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = IMX415_ADDR_BYTE;
        imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = IMX415_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    imx415_init_common_reg_info(imx415_ctx);

    // init 2to1 wdr mode Regs
    if (imx415_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        imx415_init_2to1_wdr_reg_info(imx415_ctx);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    // set wdr mode
    ret = imx415_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // set image mode
    ret = imx415_set_image_mode(imx415_ctx, &imx415_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    imx415_ctx->size.width  = sns_attr->width;
    imx415_ctx->size.height = sns_attr->height;
    imx415_ctx->fl[SENSOR_CUR_FRAME] = imx415_ctx->fl_std;
    imx415_ctx->fl[SENSOR_PRE_FRAME] = imx415_ctx->fl[SENSOR_CUR_FRAME];

    ret = imx415_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef IMX415_ISP_DEFAULT_SUPPORT
    //TO-DO: alg add param
    isp_default->blc         = &g_imx415_blc;;
    isp_default->bnr         = &g_imx415_bnr;
    isp_default->clut_attr   = XMEDIA_NULL;
    isp_default->clut_attr   = XMEDIA_NULL;
    isp_default->crosstalk   = &g_imx415_crosstalk;
    isp_default->csc         = &g_imx415_csc;
    isp_default->dehaze      = &g_imx415_dehaze;
    isp_default->demosaic    = &g_imx415_demosaic;
    isp_default->dpc_dynamic = XMEDIA_NULL;
    isp_default->dpc_static  = XMEDIA_NULL;
    isp_default->drc         = XMEDIA_NULL;
    isp_default->expander    = XMEDIA_NULL;
    isp_default->fpn         = XMEDIA_NULL;
    isp_default->gamma       = &g_imx415_gamma;
    isp_default->gcac        = XMEDIA_NULL;
    isp_default->hlc         = XMEDIA_NULL;
    isp_default->lce         = &g_imx415_lce;
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from imx415!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 imx415_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (imx415_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[0] = 200;
        black_level->black_level[1] = 200;
        black_level->black_level[2] = 200;
        black_level->black_level[3] = 200;

    } else { // black level of WDR mode
        black_level->black_level[0] = 200;
        black_level->black_level[1] = 200;
        black_level->black_level[2] = 200;
        black_level->black_level[3] = 200;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                 i = 0;
    xmedia_u8       mirror_flip = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    sensor_context  *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    if (imx415_ctx->mirror_en && imx415_ctx->flip_en) {
        mirror_flip = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (imx415_ctx->mirror_en && (!imx415_ctx->flip_en)) {
        mirror_flip = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!imx415_ctx->mirror_en) && imx415_ctx->flip_en) {
        mirror_flip = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        mirror_flip = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < IMX415_SPECS_MAX_NUM; i++) {
        if (g_imx415_property[i].width    == imx415_ctx->size.width &&
            g_imx415_property[i].height   == imx415_ctx->size.height &&
            g_imx415_property[i].wdr_mode == imx415_ctx->wdr_mode) {

            *bayer_pattern = g_imx415_property[i].bayer_format[mirror_flip];
            return XMEDIA_SUCCESS;;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 imx415_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_imx415_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context          *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    imx415_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_imx415_again[dev]         = init_param->again;
    g_imx415_dgain[dev]         = init_param->dgain;
    g_imx415_isp_dgain[dev]     = init_param->ispdgain;
    g_imx415_init_time[dev]     = init_param->exp_time;
    g_imx415_exposure[dev]      = init_param->exposure;
    g_imx415_sample_r_gain[dev] = init_param->sample_rgain;
    g_imx415_sample_b_gain[dev] = init_param->sample_bgain;
    g_imx415_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_imx415_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_imx415_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                           xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->flicker_freq   = 50 * 256;
    ae_sns_dft->full_lines     = sns_ctx->fl_std;
    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->full_lines_max = IMX415_FULL_LINES_MAX;
    ae_sns_dft->lines_per_500ms = sns_ctx->fl_std * 30 / 2;
    ae_sns_dft->hmax_times = (1000000000) / (sns_ctx->fl_std * 30);

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

    memcpy(&ae_sns_dft->piris_attr, &g_imx415_piris_attr, sizeof(xmedia_isp_piris_attr));
    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_32_0;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    return XMEDIA_SUCCESS;
}

static xmedia_void imx415_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 2;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_int_time     = sns_ctx->fl_std - 4;
    ae_sns_dft->min_int_time     = 8;

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
    ae_sns_dft->init_exposure   = g_imx415_exposure[dev] ? g_imx415_exposure[dev] : 76151;
}

 static xmedia_void imx415_get_ae_2to1_wdr_line_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                                       xmedia_isp_ae_sensor_default *ae_sns_dft)
 {
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 6;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_int_time = sns_ctx->fl_std - 8;
    ae_sns_dft->min_int_time = 4;

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

    ae_sns_dft->init_exposure = g_imx415_exposure[dev] ? g_imx415_exposure[dev] : 52000;

    if (g_imx415_wdr_mode[dev] == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
        ae_sns_dft->ae_compensation = 64;
        ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    } else {
        ae_sns_dft->ae_compensation  = 32;
        ae_sns_dft->ae_exp_mode      = XMEDIA_ISP_AE_STRATEGY_EXP_LOWLIGHT_PRIOR;
        ae_sns_dft->man_ratio_enable = XMEDIA_FALSE;
        ae_sns_dft->ratio[0]         = 0x200;
        ae_sns_dft->ratio[1]         = 0x400;
    }
 }

static xmedia_s32 imx415_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context  *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    memset(ae_sns_dft, 0, sizeof(xmedia_isp_ae_sensor_default));

    ret = imx415_get_ae_common_default(dev, imx415_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (imx415_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            imx415_get_ae_linear_default(dev, imx415_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            imx415_get_ae_2to1_wdr_line_default(dev, imx415_ctx, ae_sns_dft);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not supported wdr mode!\n");
            break;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context  *imx415_ctx = XMEDIA_NULL;
    xmedia_s32       value = 0;
    xmedia_u32       shr0, shr1, rhs1;
    static xmedia_u8 count[XMEDIA_SENSOR_DEV_MAX_NUM] = {[0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 0};

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    if (imx415_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (count[dev] == 0) { /* short exposure */
            imx415_ctx->wdr_int_time[0] = int_time;
            count[dev] = 1;
        } else if (count[dev] == 1) { /* long exposure */
            imx415_ctx->wdr_int_time[1] = int_time;

            shr1 = 9;                                  //shr1 = 2n + 1 (n: 0,1,2...) && shr1 >= 9
            rhs1 = shr1 + imx415_ctx->wdr_int_time[0]; //short inttime = rhs1 - shr1
            rhs1 = (rhs1 % 4 == 1) ? rhs1 : rhs1 + 1 - (rhs1 % 4);//rsh1 = 4n + 1 (n: 0,1,2...)
            if(rhs1 < (shr1 + 8)){                     //rhs1 >= shr1 + 8
                rhs1 = shr1 + 8;
            }
            /* TODO: rhs1 < BRL x 2 */

            shr0 = imx415_ctx->fl[SENSOR_CUR_FRAME] - imx415_ctx->wdr_int_time[1];//long inttime = vmax(fsc) - shr0
            shr0 =(shr0 % 2 == 0) ? shr0 : (shr0 + 1); //shr0  = 2n (n: 0,1,2...)
            if(shr0 < (rhs1 + 9)){                     //shr0 >= rhs1 + 9
                shr0 = rhs1 + 9;
            }
            if(shr0 > imx415_ctx->fl[SENSOR_CUR_FRAME] - 8 ){         //shr0 <= fsc -8 && shr0  = 2n (n: 0,1,2...)
                shr0 = imx415_ctx->fl[SENSOR_CUR_FRAME] - (imx415_ctx->fl[SENSOR_CUR_FRAME] % 2) - 8;
            }

            imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR0_H].data = SENSOR_LOW_8BITS(shr0);
            imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR0_M].data = SENSOR_HIGH_8BITS(shr0);
            imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR0_L].data = SENSOR_HIGHER_16BITS(shr0);
            imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR1_H].data = SENSOR_LOW_8BITS(shr1);
            imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR1_M].data = SENSOR_HIGH_8BITS(shr1);
            imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR1_L].data = SENSOR_HIGHER_16BITS(shr1);
            imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_RHS1_H].data = SENSOR_LOW_8BITS(rhs1);
            imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_RHS1_M].data = SENSOR_HIGH_8BITS(rhs1);
            imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_RHS1_L].data = SENSOR_HIGHER_16BITS(rhs1);

            count[dev] = 0;
        }
    } else {
        int_time = SENSOR_MIN(SENSOR_MAX(int_time, 4), imx415_ctx->fl[SENSOR_CUR_FRAME] - 8);
        value = imx415_ctx->fl[SENSOR_CUR_FRAME] - int_time;  //the hardware value should be [8, fl-4].

        imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR0_L].data = SENSOR_LOW_8BITS(value);
        imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR0_M].data = SENSOR_HIGH_8BITS(value);
        imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_SHR0_H].data = SENSOR_HIGHER_16BITS(value);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
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

static xmedia_s32 imx415_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
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

static xmedia_s32 imx415_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    xmedia_u32 gain = again + dgain;
    sensor_context *imx415_ctx = XMEDIA_NULL;

    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    if(gain > 240){  //max 72dB, 0.3dB per step.
        gain = 240;
    }

    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_GAIN].data = gain;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context  *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = 4950;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = CALIBRATE_STATIC_WB_B_GAIN;

    awb_sns_dft->wb_para[0]   = CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1]   = CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2]   = CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3]   = CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4]   = CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5]   = CALIBRATE_AWB_C1;
    awb_sns_dft->golden_rgain = GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = GOLDEN_BGAIN;

    switch (imx415_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_awb_ccm_wdr, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_awb_agc_table_wdr, sizeof(xmedia_isp_awb_agc_table));
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support wdr mode: %d\n", imx415_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_imx415_wb_gain[dev][0];
    awb_sns_dft->init_ggain   = g_imx415_wb_gain[dev][1];
    awb_sns_dft->init_bgain   = g_imx415_wb_gain[dev][2];
    awb_sns_dft->sample_rgain = g_imx415_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_imx415_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx415_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case IMX415_8M_10BIT_LINEAR_MODE:
            *max_fps = 30;
            break;
        case IMX415_8M_10BIT_WDR_MODE:
            *max_fps = 30;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx415_standby(xmedia_u32 dev)
{
    imx415_write_reg(dev, 0x3000, 0x01); // STANDBY
    imx415_write_reg(dev, 0x3002, 0x01); // XTMSTA

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx415_resume(xmedia_u32 dev)
{
    imx415_write_reg(dev, 0x3000, 0x00); // standby
    imx415_delay_ms(20);
    imx415_write_reg(dev, 0x3002, 0x00); // master mode start

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *imx415_ctx   = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < imx415_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < imx415_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= imx415_write_reg(dev, imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx415_start(xmedia_u32 dev)
{
    xmedia_s32     ret;
    sensor_context *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    if (imx415_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check param!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (imx415_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = imx415_init_image(dev, imx415_ctx->img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    imx415_delay_ms(10);
    ret = imx415_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = imx415_set_mirror_flip(dev, imx415_ctx->mirror_en, imx415_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx415_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = imx415_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_imx415_dev_map[index] == SENSOR_DEV_INVALID) {
            g_imx415_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_FAILURE;
}

static xmedia_s32 imx415_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_imx415_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

#define IMX415_MARGIN 40
static xmedia_s32 imx415_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{   // TO-DO：min fps is to be confirmed.
    switch (img_mode) {

        case IMX415_8M_10BIT_LINEAR_MODE:
            *min_fps = 0.8;
            break;

        case IMX415_8M_10BIT_WDR_MODE:
            *min_fps = 10;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {

        case IMX415_8M_10BIT_LINEAR_MODE:
            *vmax = IMX415_VMAX_8M_LINEAR;
            break;

        case IMX415_8M_10BIT_WDR_MODE:
            *vmax = IMX415_VMAX_8M_WDR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32    i;
    sensor_context* imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    for (i = 0; i < imx415_ctx->regs_info[0].reg_num; i++) {
        if (imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            imx415_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }
    memcpy(sns_regs_info, &imx415_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&imx415_ctx->regs_info[SENSOR_PRE_FRAME],
           &imx415_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    imx415_ctx->fl[SENSOR_PRE_FRAME] = imx415_ctx->fl[SENSOR_CUR_FRAME];
    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    xmedia_float    max_fps;
    xmedia_float    min_fps;
    xmedia_u32      vmax;
    sensor_context* imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    ret = imx415_get_max_fps(imx415_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = imx415_get_min_fps(imx415_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = imx415_get_vmax(imx415_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support fps: %f, range of fps is [%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);

    if (imx415_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE){
        full_lines  = (full_lines > IMX415_FULL_LINES_MAX) ? IMX415_FULL_LINES_MAX : full_lines;

    } else if (imx415_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE){
        full_lines  = (full_lines > IMX415_FULL_LINES_MAX_2TO1_WDR) ? IMX415_FULL_LINES_MAX_2TO1_WDR : full_lines;
    }

    imx415_ctx->fl_std               = full_lines;
    imx415_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    imx415_ctx->fps                  = fps;
    ae_sns_dft->full_lines           = full_lines;
    ae_sns_dft->full_lines_std       = full_lines;
    ae_sns_dft->fps                  = fps;
    ae_sns_dft->max_int_time         = full_lines - 8;

    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_VMAX_L].data =  SENSOR_LOW_8BITS(full_lines);
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_VMAX_M].data =  SENSOR_HIGH_8BITS(full_lines);
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_VMAX_H].data =  SENSOR_HIGHER_16BITS(full_lines);

    SENSOR_PRINT("dev[%d]- imx415 set fps: %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_void imx415_vmax_limit(xmedia_u32 *vmax, xmedia_u32 full_line, xmedia_u32 step, xmedia_bool fps_up)
{
    if (fps_up) {
        if ((*vmax) + step < full_line) {
            (*vmax) = (full_line - step);
        }
    } else {
        if ((*vmax) > full_line + step) {
            (*vmax) = (full_line + step);
        }
    }
}

static xmedia_s32 imx415_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                                        xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    xmedia_u32      vmax;
    xmedia_u32      vmax_max_fps;
    xmedia_float    max_fps;
    sensor_context *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    if (imx415_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", imx415_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = imx415_get_max_fps(imx415_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = imx415_get_vmax(imx415_ctx->img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    if (imx415_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        vmax  = full_lines / 2;
        imx415_vmax_limit(&vmax, imx415_ctx->fl[SENSOR_CUR_FRAME], 20,
                              full_lines < imx415_ctx->fl[SENSOR_CUR_FRAME]);
        vmax  = (vmax > IMX415_FULL_LINES_MAX_2TO1_WDR) ? IMX415_FULL_LINES_MAX_2TO1_WDR : vmax;
        vmax *= 2;
    } else {
        vmax  = full_lines;
        vmax  = (vmax > IMX415_FULL_LINES_MAX) ? IMX415_FULL_LINES_MAX : vmax;
    }

    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_VMAX_H].data  = SENSOR_LOW_8BITS(vmax);
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_VMAX_M].data  = SENSOR_HIGH_8BITS(vmax);
    imx415_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX415_REG_VMAX_L].data  = SENSOR_HIGHER_16BITS(vmax);

    imx415_ctx->fl[SENSOR_CUR_FRAME] = vmax;
    imx415_ctx->fps                  = max_fps * vmax_max_fps / vmax;

    ae_sns_dft->full_lines   = imx415_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = imx415_ctx->fl[SENSOR_CUR_FRAME] - 8;

    SENSOR_PRINT("dev[%d]- imx415 set fps: %f\n", dev, imx415_ctx->fps);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    xmedia_u32    short_max0   = 0;
    xmedia_u32    short_max    = 0;
    xmedia_u32    short_limit  = 2;
    sensor_context* imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    if (imx415_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        short_max0 = ((imx415_ctx->fl[1] - IMX415_MARGIN  - imx415_ctx->wdr_int_time[0]) * 0x40) /
                                                        SENSOR_DIV_0_TO_1(inttime_attr->ratio[0]);
        short_max = ((imx415_ctx->fl[0] - IMX415_MARGIN) * 0x40) / (inttime_attr->ratio[0] + 0x40);
        short_max = (short_max0 < short_max) ? short_max0 : short_max;
        short_max = (short_max == 0) ? 1 : short_max;

        if (short_max >= short_limit) {
            inttime_attr->max_inttime[0] = short_max;
            inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * inttime_attr->ratio[0]) >> 6;
            inttime_attr->min_inttime[0] = short_limit;
            inttime_attr->min_inttime[1] = (inttime_attr->min_inttime[0] * inttime_attr->ratio[0]) >> 6;
        } else {
            short_max = short_limit;
            inttime_attr->max_inttime[0] = short_max;
            inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * 0xFFF) >> 6;
            inttime_attr->min_inttime[0] = inttime_attr->max_inttime[0];
            inttime_attr->min_inttime[1] = inttime_attr->max_inttime[1];
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *imx415_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    IMX415_GET_CTX(dev, imx415_ctx);
    SENSOR_CHECK_PTR_RETURN(imx415_ctx);

    ret = imx415_get_min_fps(imx415_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = imx415_get_max_fps(imx415_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = imx415_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx415_init_register_func(xmedia_sensor_register_info* info)
{
    SENSOR_CHECK_PTR_RETURN(info);

    memcpy(info->sensor_name, IMX415_NAME, sizeof(IMX415_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property      = imx415_get_property;
    info->isp_func.pfn_sensor_set_work_mode     = imx415_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes    = imx415_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror            = imx415_mirror;
    info->isp_func.pfn_sensor_flip              = imx415_flip;
    info->isp_func.pfn_sensor_set_bus_info      = imx415_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr      = imx415_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param    = imx415_set_init_param;
    info->isp_func.pfn_sensor_start             = imx415_start;
    info->isp_func.pfn_sensor_stop              = imx415_stop;
    info->isp_func.pfn_sensor_standby           = imx415_standby;
    info->isp_func.pfn_sensor_resume            = imx415_resume;
    info->isp_func.pfn_sensor_write_reg         = imx415_write_reg;
    info->isp_func.pfn_sensor_read_reg          = imx415_read_reg;
    info->isp_func.pfn_sensor_init              = imx415_init;
    info->isp_func.pfn_sensor_exit              = imx415_exit;
    info->isp_func.pfn_sensor_set_attr          = imx415_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = imx415_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = imx415_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = imx415_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = imx415_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = imx415_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = imx415_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = imx415_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = imx415_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = imx415_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = imx415_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = imx415_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = imx415_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = imx415_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = imx415_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = imx415_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = imx415_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = imx415_get_awb_default;

    return XMEDIA_SUCCESS;

}

xmedia_s32 imx415_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = imx415_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = imx415_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret = imx415_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "IMX415 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx415_unregister(xmedia_u32 pipe)
{
    xmedia_s32 ret;
    xmedia_u32 dev;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = imx415_match_dev(pipe, &dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_PRINT("IMX415 need not unregister function!\n");
        return XMEDIA_SUCCESS;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "IMX415 unregister function failed!\n");
        return XMEDIA_FAILURE;
    }

    imx415_ctx_exit(dev);
    g_imx415_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;
}
