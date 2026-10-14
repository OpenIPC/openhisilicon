#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "imx307.h"
#include "imx307_ctrl.h"
#include "imx307_ex.h"

#ifdef FPGA
#define IMX307_INPUT_CLOCK_LINEAR 18000000
#define IMX307_MAX_FPS_LINEAR     14.5
#else
#define IMX307_INPUT_CLOCK_LINEAR XMEDIA_SENSOR_CLOCK_FREQ_37_125MHZ
#define IMX307_MAX_FPS_LINEAR     30
#endif

#define IMX307_NAME "IMX307"
#define IMX307_SPECS_MAX_NUM 2

#define IMX307_2M_BIT_RATE_LINEAR   400
#define IMX307_2M_BIT_RATE_WDR      800

#ifdef __linux__
//#define IMX307_ISP_DEFAULT_SUPPORT
#endif

#define IMX307_RES_IS_1080P(w, h) ((w) == 1920 && (h) == 1080)

#define IMX307_GAIN_TABLE 241

#define IMX307_ERR_MODE_PRINT(sensor_image_mode)                                                                       \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sensor_image_mode->width,       \
                     sensor_image_mode->height, sensor_image_mode->wdr_mode);                                          \
    } while (0)

#define IS_LINE_WDR_MODE(mode)                                                                                         \
    (((mode) == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) || ((mode) == XMEDIA_VIDEO_WDR_MODE_3TO1_LINE) ||                     \
     ((mode) == XMEDIA_VIDEO_WDR_MODE_4TO1_LINE))

IMX307_STATE_S g_imx307_state[XMEDIA_SENSOR_DEV_MAX_NUM] = {{0}};
xmedia_isp_ae_wdr_mode g_imx307_wdr_mode[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = 0 };

static xmedia_u32 maxtimegetcnt[XMEDIA_SENSOR_DEV_MAX_NUM] = {0};
sensor_context *g_imx307_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define IMX307_GET_CTX(dev, ctx) ctx = g_imx307_ctx[dev]
#define IMX307_SET_CTX(dev, ctx) g_imx307_ctx[dev] = ctx
static xmedia_s32 g_imx307_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM]  = { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] =
                                                                                  SENSOR_DEV_INVALID };
static xmedia_u32 g_lines_per_500ms[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u32 g_imx307_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]        = { 0 };
static xmedia_u16 g_imx307_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM]   = { 0 };
static xmedia_u16 g_imx307_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM]   = { 0 };
static xmedia_u16 g_imx307_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

static const xmedia_sensor_property g_imx307_property[IMX307_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, IMX307_MAX_FPS_LINEAR, IMX307_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB,
          XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB },
        // vcid,  bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, IMX307_2M_BIT_RATE_LINEAR,
    },
    {
        // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_DOL, 30, XMEDIA_SENSOR_CLOCK_FREQ_37_125MHZ,
        // output_intf, pixel_format,  bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB,
          XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB },
        // vcid,  bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1}, IMX307_2M_BIT_RATE_WDR,
    },
};

static const xmedia_u32 gain_table[IMX307_GAIN_TABLE] = {
    1024,    1059,    1097,    1135,    1175,    1217,    1259,
    1304,    1349,    1397,    1446,    1497,    1549,    1604, // 3.9dB
    1660,    1719,    1779,    1842,    1906,    1973,    2043,
    2114,    2189,    2266,    2345,    2428,    2513,    2601, // 8.1dB
    2693,    2788,    2886,    2987,    3092,    3201,    3313,
    3430,    3550,    3675,    3804,    3938,    4076,    4219, // 12.3dB
    4368,    4521,    4680,    4845,    5015,    5191,    5374,
    5562,    5758,    5960,    6170,    6387,    6611,    6843, // 16.5dB
    7084,    7333,    7591,    7857,    8133,    8419,    8715,
    9021,    9338,    9667,    10006,   10358,   10722,   11099, // 20.7dB
    11489,   11893,   12311,   12743,   13191,   13655,   14135,
    14631,   15146,   15678,   16229,   16799,   17390,   18001, // 24.9dB
    18633,   19288,   19966,   20668,   21394,   22146,   22924,
    23730,   24564,   25427,   26320,   27245,   28203,   29194, // 29.1dB
    30220,   31282,   32381,   33519,   34697,   35917,   37179,
    38485,   39838,   41238,   42687,   44187,   45740,   47347, // 33.3dB
    49011,   50734,   52517,   54362,   56273,   58250,   60297,
    62416,   64610,   66880,   69230,   71663,   74182,   76789, // 37.5dB
    79487,   82281,   85172,   88165,   91264,   94471,   97791,
    101228,  104785,  108468,  112279,  116225,  120310,  124537, // 41.7dB
    128914,  133444,  138134,  142988,  148013,  153215,  158599,
    164172,  169942,  175914,  182096,  188495,  195119,  201976, // 45.9dB
    209074,  216421,  224027,  231900,  240049,  248485,  257217,
    266256,  275613,  285299,  295325,  305703,  316446,  327567, // 50.1dB
    339078,  350994,  363329,  376097,  389314,  402995,  417157,
    431817,  446992,  462700,  478961,  495793,  513216,  531251, // 54.3dB
    549921,  569246,  589250,  609958,  631393,  653582,  676550,
    700326,  724936,  750412,  776783,  804081,  832338,  861589, // 58.5dB
    891867,  923209,  955652,  989236,  1024000, 1059985, 1097236,
    1135795, 1175709, 1217026, 1259795, 1304067, 1349895, 1397333, // 62.7dB
    1446438, 1497269, 1549887, 1604353, 1660734, 1719095, 1779508,
    1842044, 1906777, 1973786, 2043149, 2114949, 2189273, 2266209, // 66.9dB
    2345848, 2428287, 2513622, 2601956, 2693394, 2788046, 2886024,
    2987445, 3092431, 3201105, 3313599, 3430046, 3550585, 3675361, // 71.1dB
    3804521, 3938220, 4076617                                      // 72.0dB
};

static const xmedia_sensor_capability g_imx307_capability = {
    .max_width  = 1920,
    .max_height = 1080,
    .max_fps    = 30,

    .wdr_mode   = XMEDIA_VIDEO_WDR_MODE_NONE,
    .init_mclk  = XMEDIA_SENSOR_CLOCK_FREQ_37_125MHZ,
    .wdr_format = XMEDIA_VIDEO_WDR_FMT_DOL,
    .bit_width  = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_4L,

    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = IMX307_ADDR_BYTE,
        .data_byte_num     = IMX307_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { 0x3000 },
        .standby_reg_data  = { 0x01 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { 0x3000 },
        .resume_reg_data   = { 0x00 },
    },
};

static xmedia_s32 imx307_ctx_init(xmedia_u32 dev)
{
    sensor_context *imx307_ctx = XMEDIA_NULL;

    IMX307_GET_CTX(dev, imx307_ctx);
    if (imx307_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    imx307_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (imx307_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(imx307_ctx, 0, sizeof(sensor_context));
    imx307_ctx->i2c_addr             = IMX307_I2C_ADDR;
    imx307_ctx->lanes                = XMEDIA_SENSOR_MIPI_LANES_2L;
    imx307_ctx->work_mode            = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    imx307_ctx->bus_info.i2c_dev     = SENSOR_I2C_INVALID;
    imx307_ctx->size.width           = g_imx307_property[0].width;
    imx307_ctx->size.height          = g_imx307_property[0].height;
    imx307_ctx->fps                  = g_imx307_property[0].max_fps;
    imx307_ctx->wdr_mode             = g_imx307_property[0].wdr_mode;
    imx307_ctx->img_mode             = IMX307_2M_2L_10BIT_LINEAR_MODE;
    imx307_ctx->fl_std               = IMX307_VMAX_1080P30_LINEAR;
    imx307_ctx->fl[SENSOR_CUR_FRAME] = IMX307_VMAX_1080P30_LINEAR;
    imx307_ctx->fl[SENSOR_PRE_FRAME] = IMX307_VMAX_1080P30_LINEAR;
    IMX307_SET_CTX(dev, imx307_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void imx307_ctx_exit(xmedia_u32 dev)
{
    sensor_context *imx307_ctx = XMEDIA_NULL;

    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_FREE(imx307_ctx);
    IMX307_SET_CTX(dev, imx307_ctx);

    return;
}

static xmedia_s32 imx307_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *imx307_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    for (i = 0; i < IMX307_SPECS_MAX_NUM; i++) {
        if (g_imx307_property[i].width == imx307_ctx->size.width &&
            g_imx307_property[i].height == imx307_ctx->size.height &&
            g_imx307_property[i].wdr_mode == imx307_ctx->wdr_mode) {
            memcpy(property, &g_imx307_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }
    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Please check input param!\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 imx307_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *imx307_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    imx307_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *imx307_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        imx307_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        imx307_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        imx307_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 imx307_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;
    sensor_context *imx307_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    imx307_ctx->i2c_addr = slave_addr;
    ret = imx307_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *imx307_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    *slave_addr = imx307_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

// called by ISP
static xmedia_s32 imx307_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *imx307_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    imx307_ctx->init_mode = init_mode;
    ret                   = imx307_i2c_init(dev, imx307_ctx->bus_info.i2c_dev, imx307_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

// new added
static xmedia_s32 imx307_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *imx307_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    ret = imx307_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *imx307_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    imx307_ctx->mirror_en = mirror_en;
    ret = imx307_set_mirror_flip(dev, mirror_en, imx307_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    sensor_context *imx307_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    imx307_ctx->flip_en = flip_en;
    ret = imx307_set_mirror_flip(dev, imx307_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_set_image_mode(sensor_context *imx307_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr, xmedia_u32 dev)
{
     SENSOR_CHECK_PTR_RETURN(sns_attr);
     SENSOR_CHECK_PTR_RETURN(image_mode);

    if (imx307_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (IMX307_RES_IS_1080P(sns_attr->width, sns_attr->height)) {
            if(imx307_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_2L){
                *image_mode           = IMX307_2M_2L_10BIT_LINEAR_MODE;
                 imx307_ctx->fl_std   = IMX307_VMAX_1080P30_LINEAR;
                 g_imx307_state[dev].u8Hcg = 0x2;

            }else if(imx307_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_4L){
                *image_mode           = IMX307_2M_4L_10BIT_LINEAR_MODE;
                 imx307_ctx->fl_std   = IMX307_VMAX_1080P30_LINEAR;
                 g_imx307_state[dev].u8Hcg = 0x2;
            }
        } else {
            IMX307_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else if (imx307_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (IMX307_RES_IS_1080P(sns_attr->width, sns_attr->height)) {
            if(imx307_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_2L){
                *image_mode           = IMX307_2M_2L_10BIT_2TO1_WDR_MODE;
                 imx307_ctx->fl_std   = IMX307_VMAX_2L_1080P60TO30_WDR * 2;
                 g_imx307_state[dev].u8Hcg = 0x1;
                 g_imx307_state[dev].u32BRL = 1109;
            }else if(imx307_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_4L){
                *image_mode           = IMX307_2M_4L_10BIT_2TO1_WDR_MODE;
                 imx307_ctx->fl_std   = IMX307_VMAX_4L_1080P60TO30_WDR * 2;
                 g_imx307_state[dev].u8Hcg = 0x2;
                 g_imx307_state[dev].u32BRL = 1109;
            }
        } else {
            IMX307_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        IMX307_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    imx307_ctx->size.width  = sns_attr->width;
    imx307_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context  *imx307_ctx = XMEDIA_NULL;

    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            imx307_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            imx307_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
            SENSOR_PRINT("2to1 line WDR 1080p mode(60fps->30fps)\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }
    memset(imx307_ctx->wdr_int_time, 0, sizeof(imx307_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

static xmedia_void imx307_init_common_reg_info(sensor_context *imx307_ctx)
{
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS1_L].delay_frame_num = 2; // 寄存器延迟生效帧数
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS1_L].reg_addr        = IMX307_REG_ADDR_SHS1_L;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS1_M].delay_frame_num = 2;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS1_M].reg_addr        = IMX307_REG_ADDR_SHS1_M;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS1_H].delay_frame_num = 2;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS1_H].reg_addr        = IMX307_REG_ADDR_SHS1_H;

    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_GAIN].delay_frame_num =
        2; // make shutter and gain effective at the same time
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_GAIN].reg_addr       = IMX307_REG_ADDR_GAIN; // gain
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_HCG].delay_frame_num = 1;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_HCG].reg_addr        = IMX307_REG_ADDR_HCG;

    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_VMAX_L].delay_frame_num = 2;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_VMAX_L].reg_addr        = IMX307_REG_ADDR_VMAX_L;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_VMAX_M].delay_frame_num = 2;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_VMAX_M].reg_addr        = IMX307_REG_ADDR_VMAX_M;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_VMAX_H].delay_frame_num = 2;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_VMAX_H].reg_addr        = IMX307_REG_ADDR_VMAX_H;

    return;
}

static xmedia_void imx307_init_2to1_wdr_reg_info(sensor_context *imx307_ctx)
{
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS1_L].delay_frame_num = 2;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS1_M].delay_frame_num = 2;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS1_H].delay_frame_num = 2;

    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS2_L].delay_frame_num = 2;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS2_L].reg_addr        = IMX307_REG_ADDR_SHS2_L;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS2_M].delay_frame_num = 2;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS2_M].reg_addr        = IMX307_REG_ADDR_SHS2_M;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS2_H].delay_frame_num = 2;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS2_H].reg_addr        = IMX307_REG_ADDR_SHS2_H;

    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_RHS1_L].delay_frame_num = 1;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_RHS1_L].reg_addr        = IMX307_REG_ADDR_RHS1_L;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_RHS1_M].delay_frame_num = 1;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_RHS1_M].reg_addr        = IMX307_REG_ADDR_RHS1_M;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_RHS1_H].delay_frame_num = 1;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_RHS1_H].reg_addr        = IMX307_REG_ADDR_RHS1_H;

    imx307_ctx->regs_info[0].i2c_data[IMX307_REG__Y_OUT_SIZE].delay_frame_num   = 1;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG__Y_OUT_SIZE].reg_addr          = IMX307_REG_ADDR_Y_OUT_SIZE;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG__Y_OUT_SIZE_H].delay_frame_num = 1;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG__Y_OUT_SIZE_H].reg_addr        = IMX307_REG_ADDR_Y_OUT_SIZE_H;

    return;
}

static xmedia_s32 imx307_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *imx307_ctx = XMEDIA_NULL;

    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    imx307_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = imx307_ctx->bus_info.i2c_dev;
    imx307_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    imx307_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = 8;

    if (imx307_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        imx307_ctx->regs_info[0].reg_num                += 11;
        imx307_ctx->regs_info[0].cfg_to_valid_delay_max  = 2;
    }

    for (i = 0; i < imx307_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        imx307_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        imx307_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = imx307_ctx->i2c_addr;
        imx307_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = IMX307_ADDR_BYTE;
        imx307_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = IMX307_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    imx307_init_common_reg_info(imx307_ctx);

    // init 2to1 wdr mode Regs
    if (imx307_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        imx307_init_2to1_wdr_reg_info(imx307_ctx);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *imx307_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    // set wdr mode
    ret = imx307_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // set image mode
    ret = imx307_set_image_mode(imx307_ctx, &imx307_ctx->img_mode, sns_attr, dev);
    SENSOR_CHECK_RET_RETURN(ret);

    imx307_ctx->fl[SENSOR_CUR_FRAME] = imx307_ctx->fl_std;
    imx307_ctx->fl[SENSOR_PRE_FRAME] = imx307_ctx->fl[SENSOR_CUR_FRAME];

    ret = imx307_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context    *imx307_ctx = XMEDIA_NULL;
    xmedia_sensor_attr sns_attr;
    xmedia_s32         ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L && mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "not support mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    imx307_ctx->lanes = mipi_lanes;
    sns_attr.height    =  imx307_ctx->size.height;
    sns_attr.width     =  imx307_ctx->size.width;
    sns_attr.wdr_mode  =  imx307_ctx->wdr_mode;
    ret = imx307_set_image_mode(imx307_ctx, &imx307_ctx->img_mode, &sns_attr, dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
        SENSOR_CHECK_DEV_RETURN(dev);
        SENSOR_CHECK_PTR_RETURN(isp_default);

        memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef IMX307_ISP_DEFAULT_SUPPORT
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
        SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from imx307!\n ");

        return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 imx307_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    xmedia_u32  i;

    sensor_context *imx307_ctx = XMEDIA_NULL;
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (imx307_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) { // black level of linear mode
        for (i = 0; i < 4; i++) {
            black_level->black_level[i] = 0xF0; // 240
        }
    } else { // black level of DOL mode
        black_level->black_level[0] = 0xF0;
        black_level->black_level[1] = 0xF0;
        black_level->black_level[2] = 0xF0;
        black_level->black_level[3] = 0xF0;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *imx307_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    if (imx307_ctx->mirror_en && imx307_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (imx307_ctx->mirror_en && (!imx307_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!imx307_ctx->mirror_en) && imx307_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < IMX307_SPECS_MAX_NUM; i++) {
        if (g_imx307_property[i].width == imx307_ctx->size.width &&
            g_imx307_property[i].height == imx307_ctx->size.height &&
            g_imx307_property[i].wdr_mode == imx307_ctx->wdr_mode) {
            *bayer_pattern = g_imx307_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= IMX307_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);
    memcpy(capability, &g_imx307_capability, sizeof(xmedia_sensor_capability));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context          *imx307_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    imx307_ctx->ae_wdr_mode = ae_wdr_mode;
    maxtimegetcnt[dev] = 0;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 imx307_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_imx307_exposure[dev]        = init_param->exposure;
    g_imx307_sample_r_gain[dev]   = init_param->sample_rgain;
    g_imx307_sample_b_gain[dev]   = init_param->sample_bgain;

    g_imx307_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_imx307_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_imx307_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                      xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context*         imx307_ctx = XMEDIA_NULL;
    xmedia_sensor_mipi_lanes mipi_lanes;

    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);
    mipi_lanes = imx307_ctx->lanes;

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ae_sns_dft->full_lines_std = imx307_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256;
    ae_sns_dft->full_lines_max = IMX307_FULL_LINES_MAX;

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

    if (mipi_lanes == XMEDIA_SENSOR_MIPI_LANES_2L) {
        ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_0;
        ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_32_0;
    } else if (mipi_lanes == XMEDIA_SENSOR_MIPI_LANES_4L) {
        memcpy(&ae_sns_dft->piris_attr, &g_piris_attr, sizeof(xmedia_isp_piris_attr));
        ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_4;
        ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_5_6;
    }

    ae_sns_dft->ae_route_ex_valid          = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num    = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    if (g_lines_per_500ms[dev] == 0) {
        ae_sns_dft->lines_per_500ms = imx307_ctx->fl_std * 30 / 2;
    } else {
        ae_sns_dft->lines_per_500ms = g_lines_per_500ms[dev];
    }

    return XMEDIA_SUCCESS;

}

static xmedia_void imx307_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                         xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_sensor_mipi_lanes mipi_lanes;
    mipi_lanes = sns_ctx->lanes;

    ae_sns_dft->max_again        = 22924;
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 128914;
    ae_sns_dft->min_dgain        = 1024;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x38;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_imx307_exposure[dev] ? g_imx307_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - 2;
    ae_sns_dft->max_int_time_target = 65535;

    if (mipi_lanes == XMEDIA_SENSOR_MIPI_LANES_2L) {
        ae_sns_dft->min_int_time        = 3;
        ae_sns_dft->min_int_time_target = 3;
    } else if (mipi_lanes == XMEDIA_SENSOR_MIPI_LANES_4L) {
        ae_sns_dft->min_int_time        = 1;
        ae_sns_dft->min_int_time_target = 1;
    }

    return;
}

 static xmedia_s32 imx307_get_ae_2to1_wdr_line_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                                     xmedia_isp_ae_sensor_default *ae_sns_dft)
 {
    ae_sns_dft->max_int_time = sns_ctx->fl_std - 2;
    xmedia_sensor_mipi_lanes mipi_lanes;
    mipi_lanes = sns_ctx->lanes;

    if (mipi_lanes == XMEDIA_SENSOR_MIPI_LANES_2L) {
        ae_sns_dft->min_int_time = 4;
    } else if (mipi_lanes == XMEDIA_SENSOR_MIPI_LANES_4L) {
        ae_sns_dft->min_int_time = 2;
    }

    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    ae_sns_dft->max_again        = 22924;
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 128914;
    ae_sns_dft->min_dgain        = 1024;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;
    ae_sns_dft->ae_compensation  = 24;

    if (mipi_lanes == XMEDIA_SENSOR_MIPI_LANES_2L) {
        ae_sns_dft->init_exposure = g_imx307_exposure[dev] ? g_imx307_exposure[dev] : 16462;
    } else if (mipi_lanes == XMEDIA_SENSOR_MIPI_LANES_4L) {
        ae_sns_dft->init_exposure = g_imx307_exposure[dev] ? g_imx307_exposure[dev] : 270762;
    }

    if (g_imx307_wdr_mode[dev] == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
        ae_sns_dft->ae_compensation = 56;
        ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    } else {
        ae_sns_dft->max_ispdgain_target = 1024;
        ae_sns_dft->ae_exp_mode         = XMEDIA_ISP_AE_STRATEGY_EXP_LOWLIGHT_PRIOR;
        ae_sns_dft->man_ratio_enable    = XMEDIA_FALSE;
        ae_sns_dft->ratio[0]            = 0x400;
        ae_sns_dft->ratio[1]            = 0x40;
        ae_sns_dft->ratio[2]            = 0x40;
    }

    return XMEDIA_SUCCESS;
 }

static xmedia_s32 imx307_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32              ret;
    sensor_context          *imx307_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = imx307_get_ae_common_default(dev, imx307_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (imx307_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            imx307_get_ae_linear_default(dev, imx307_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            imx307_get_ae_2to1_wdr_line_default(dev, imx307_ctx, ae_sns_dft);
            break;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context    *imx307_ctx                                = XMEDIA_NULL;
    static xmedia_bool first[XMEDIA_SENSOR_DEV_MAX_NUM]          = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 1 };
    static xmedia_u32  short_int_time[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
    static xmedia_u32  long_int_time[XMEDIA_SENSOR_DEV_MAX_NUM]  = { 0 };

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);
    xmedia_s32 rhs1[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
    xmedia_s32 shs1[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
    xmedia_s32 shs2[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

    xmedia_s32 value = 0;
    xmedia_s32 youtsize;

    if (imx307_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (first[dev]) { // short exposure
            imx307_ctx->wdr_int_time[0] = int_time;
            short_int_time[dev]         = int_time;
            first[dev]                  = XMEDIA_FALSE;
        } else { // long exposure
            imx307_ctx->wdr_int_time[1] = int_time;
            long_int_time[dev]          = int_time;

            shs2[dev] = imx307_ctx->fl[1] - long_int_time[dev] - 1;

            // allocate the RHS1
            shs1[dev] = (short_int_time[dev] % 2) + 2;
            rhs1[dev] = short_int_time[dev] + shs1[dev] + 1;

            youtsize = (1097 + (rhs1[dev] - 1) / 2 + 7) * 2;
            youtsize = (youtsize >= 0x1FFF) ? 0x1FFF : youtsize;

            imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS1_L].data = SENSOR_LOW_8BITS(shs1[dev]);
            imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS1_M].data = SENSOR_HIGH_8BITS(shs1[dev]);
            imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS1_H].data = SENSOR_HIGHER_16BITS(shs1[dev]);

            imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS2_L].data = SENSOR_LOW_8BITS(shs2[dev]);
            imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS2_M].data = SENSOR_HIGH_8BITS(shs2[dev]);
            imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS2_H].data = SENSOR_HIGHER_16BITS(shs2[dev]);

            imx307_ctx->regs_info[0].i2c_data[IMX307_REG_RHS1_L].data = SENSOR_LOW_8BITS(rhs1[dev]);
            imx307_ctx->regs_info[0].i2c_data[IMX307_REG_RHS1_M].data = SENSOR_HIGH_8BITS(rhs1[dev]);
            imx307_ctx->regs_info[0].i2c_data[IMX307_REG_RHS1_H].data = SENSOR_HIGHER_16BITS(rhs1[dev]);

            imx307_ctx->regs_info[0].i2c_data[IMX307_REG__Y_OUT_SIZE].data = (youtsize & 0xFF);
            imx307_ctx->regs_info[0].i2c_data[IMX307_REG__Y_OUT_SIZE_H].data = ((youtsize & 0x1F00) >> 8);

            first[dev] = XMEDIA_TRUE;
        }
    } else {
        value = imx307_ctx->fl[0] - int_time - 1;

        imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS1_L].data = SENSOR_LOW_8BITS(value);
        imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS1_M].data = SENSOR_HIGH_8BITS(value);
        imx307_ctx->regs_info[0].i2c_data[IMX307_REG_SHS1_H].data = SENSOR_HIGHER_16BITS(value);
        first[dev] = XMEDIA_TRUE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_u32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= gain_table[90]) {
        *again_lin = gain_table[90];
        *again_db  = 90;
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < 91; i++) {
        if (*again_lin < gain_table[i]) {
            *again_lin = gain_table[i - 1];
            *again_db  = i - 1;
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    xmedia_s32 i;
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(dgain_lin);
    SENSOR_CHECK_PTR_RETURN(dgain_db);


    if (*dgain_lin >= gain_table[140]) {
        *dgain_lin = gain_table[140];
        *dgain_db = 140;
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < 141; i++) {
        if (*dgain_lin < gain_table[i]) {
            *dgain_lin = gain_table[i - 1];
            *dgain_db = i - 1;
            break;
        }
    }

    return XMEDIA_SUCCESS;
}
static xmedia_s32 imx307_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *imx307_ctx = XMEDIA_NULL;
    xmedia_u32 hcg = g_imx307_state[dev].u8Hcg;
    xmedia_u32 tmp;

    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    if (again >= 21) {
        hcg = hcg | 0x10;  // bit[4] HCG  .Reg0x3009[7:0]
        again = again - 21;
    }

    tmp = again + dgain;
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_GAIN].data = SENSOR_LOW_8BITS(tmp);
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_HCG].data  = SENSOR_LOW_8BITS(hcg);

    return XMEDIA_SUCCESS;
}

#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

static xmedia_s32 imx307_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *imx307_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = 4950;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x1C2;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x100;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x100;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x1BF;

    awb_sns_dft->wb_para[0]   = -53;
    awb_sns_dft->wb_para[1]   = 309;
    awb_sns_dft->wb_para[2]   = 0;
    awb_sns_dft->wb_para[3]   = 160279;
    awb_sns_dft->wb_para[4]   = 128;
    awb_sns_dft->wb_para[5]   = -110119;
    awb_sns_dft->golden_rgain = GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (imx307_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_awb_ccm_wdr, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_awb_agc_table_wdr, sizeof(xmedia_isp_awb_agc_table));
            break;
    }

    awb_sns_dft->init_rgain   = g_imx307_wb_gain[dev][0];
    awb_sns_dft->init_ggain   = g_imx307_wb_gain[dev][1];
    awb_sns_dft->init_bgain   = g_imx307_wb_gain[dev][2];
    awb_sns_dft->sample_rgain = g_imx307_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_imx307_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case IMX307_2M_2L_10BIT_LINEAR_MODE:
            *max_fps = 30.0;
            break;
        case IMX307_2M_2L_10BIT_2TO1_WDR_MODE:
            *max_fps = 30.0;
            break;
        case IMX307_2M_4L_10BIT_LINEAR_MODE:
            *max_fps = 30.0;
            break;
        case IMX307_2M_4L_10BIT_2TO1_WDR_MODE:
            *max_fps = 30.0;
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_standby(xmedia_u32 dev)
{
    imx307_write_reg(dev, 0x3000, 0x01); // STANDBY
    imx307_write_reg(dev, 0x3002, 0x01); // XTMSTA

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_resume(xmedia_u32 dev)
{
    imx307_write_reg(dev, 0x3000, 0x00); // standby
    imx307_delay_ms(20);
    imx307_write_reg(dev, 0x3002, 0x00); // master mode start
    imx307_write_reg(dev, 0x304b, 0x0a);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *imx307_ctx   = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < imx307_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += imx307_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < imx307_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= imx307_write_reg(dev, imx307_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                imx307_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_start(xmedia_u32 dev)
{
    xmedia_s32               ret;
    xmedia_u8                img_mode;
    xmedia_sensor_mipi_lanes mipi_lanes;
    xmedia_sensor_work_mode  work_mode;
    sensor_context          *imx307_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    work_mode  = imx307_ctx->work_mode;
    mipi_lanes = imx307_ctx->lanes;
    img_mode   = imx307_ctx->img_mode;

    printf("imx307_ctx->lanes = %d\n", mipi_lanes);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL ||
        (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L && mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check param!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (imx307_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = imx307_init_image(dev, img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    imx307_delay_ms(10);
    ret = imx307_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = imx307_set_mirror_flip(dev, imx307_ctx->mirror_en, imx307_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = imx307_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_imx307_dev_map[index] == SENSOR_DEV_INVALID) {
            g_imx307_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_FAILURE;
}

static xmedia_s32 imx307_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_imx307_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 imx307_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{ // TO-DO：min fps is to be confirmed.
    switch (img_mode) {
        case IMX307_2M_2L_10BIT_2TO1_WDR_MODE:
            *min_fps = 15.22;
            break;
        case IMX307_2M_2L_10BIT_LINEAR_MODE:
            *min_fps = 0.12;
            break;
        case IMX307_2M_4L_10BIT_2TO1_WDR_MODE:
            *min_fps = 16.5;
            break;
        case IMX307_2M_4L_10BIT_LINEAR_MODE:
            *min_fps = 0.12;
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }
    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case IMX307_2M_2L_10BIT_2TO1_WDR_MODE:
            *vmax = IMX307_VMAX_2L_1080P60TO30_WDR;
            break;
        case IMX307_2M_2L_10BIT_LINEAR_MODE:
            *vmax = IMX307_VMAX_1080P30_LINEAR;
            break;
        case IMX307_2M_4L_10BIT_2TO1_WDR_MODE:
            *vmax = IMX307_VMAX_4L_1080P60TO30_WDR;
            break;
        case IMX307_2M_4L_10BIT_LINEAR_MODE:
            *vmax = IMX307_VMAX_1080P30_LINEAR;
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }
    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *imx307_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    for (i = 0; i < imx307_ctx->regs_info[0].reg_num; i++) {
        if (imx307_ctx->regs_info[0].i2c_data[i].data == imx307_ctx->regs_info[1].i2c_data[i].data) {
            imx307_ctx->regs_info[0].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            imx307_ctx->regs_info[0].i2c_data[i].update = XMEDIA_TRUE;
        }
    }
    memcpy(sns_regs_info, &imx307_ctx->regs_info[0], sizeof(xmedia_sensor_regs_info));
    memcpy(&imx307_ctx->regs_info[1], &imx307_ctx->regs_info[0], sizeof(xmedia_sensor_regs_info));

    imx307_ctx->fl[1] = imx307_ctx->fl[0];
    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_calc_fps(xmedia_float fps, sensor_context *imx307_ctx, xmedia_u32 *full_lines,
                                  xmedia_u32 *lines_per500ms)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = imx307_get_max_fps(imx307_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = imx307_get_min_fps(imx307_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = imx307_get_vmax(imx307_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is [%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, IMX307_FULL_LINES_MAX);

    if (imx307_ctx->work_mode == IMX307_2M_2L_10BIT_2TO1_WDR_MODE || IMX307_2M_4L_10BIT_2TO1_WDR_MODE) {
        *lines_per500ms = vmax * 30;
    } else {
        *lines_per500ms = vmax * 15; // lines_per500ms = vmax* fps / 2;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *imx307_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    ret = imx307_calc_fps(fps, imx307_ctx, &full_lines, &ae_sns_dft->lines_per_500ms);
    SENSOR_CHECK_RET_RETURN(ret);

    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_VMAX_M].data = SENSOR_HIGH_8BITS(full_lines);
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_VMAX_H].data = SENSOR_HIGHER_16BITS(full_lines);

    if (imx307_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        imx307_ctx->fl_std = full_lines * 2;

        // RHS1 limitation:
        // 2n + 5
        // RHS1 <= FSC - BRL*2 -21
        //(2 * VMAX_IMX307_1080P30_WDR - 2 * gu32BRL - 21) - (((2 * VMAX_IMX307_1080P30_WDR - 2 * 1109 - 21) - 5) %2)
        g_imx307_state[dev].u32RHS1_MAX = (full_lines - g_imx307_state[dev].u32BRL) * 2 - 21;
    } else {
        imx307_ctx->fl_std = full_lines;
    }

    imx307_ctx->fps                  = fps;
    ae_sns_dft->fps                  = fps;
    ae_sns_dft->full_lines_std       = imx307_ctx->fl_std;
    ae_sns_dft->max_int_time         = imx307_ctx->fl_std - 2;
    imx307_ctx->fl[SENSOR_CUR_FRAME] = imx307_ctx->fl_std;
    ae_sns_dft->full_lines           = imx307_ctx->fl[SENSOR_CUR_FRAME];

    SENSOR_PRINT("dev[%d]- imx307 set fps: %f\n", dev, fps);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *imx307_ctx = XMEDIA_NULL;
    xmedia_float    max_fps;
    xmedia_u32      vmax;
    xmedia_s32      ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    if (imx307_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", imx307_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = imx307_get_max_fps(imx307_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = imx307_get_vmax(imx307_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if (imx307_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        full_lines =
            (full_lines > 2 * IMX307_FULL_LINES_MAX_2TO1_WDR) ? 2 * IMX307_FULL_LINES_MAX_2TO1_WDR : full_lines;
        imx307_ctx->fl[0]               = (full_lines >> 1) << 1;
        g_imx307_state[dev].u32RHS1_MAX = imx307_ctx->fl[0] - g_imx307_state[dev].u32BRL * 2 - 21;
    } else {
        full_lines        = (full_lines > IMX307_FULL_LINES_MAX) ? IMX307_FULL_LINES_MAX : full_lines;
        imx307_ctx->fl[0] = full_lines;
    }

    imx307_ctx->fps = max_fps * vmax / full_lines;

    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_VMAX_L].data = SENSOR_LOW_8BITS(imx307_ctx->fl[0]);
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_VMAX_M].data = SENSOR_HIGH_8BITS(imx307_ctx->fl[0]);
    imx307_ctx->regs_info[0].i2c_data[IMX307_REG_VMAX_H].data = SENSOR_HIGHER_16BITS(imx307_ctx->fl[0]);

    ae_sns_dft->full_lines   = imx307_ctx->fl[0];
    ae_sns_dft->max_int_time = imx307_ctx->fl[0] - 2;

    SENSOR_PRINT("dev[%d]- imx307 set fps: %f\n", dev, imx307_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr) // 遗留
{
    xmedia_s32      i                    = 0;
    xmedia_u32      int_time_max_tmp0    = 0;
    xmedia_u32      int_time_max_tmp     = 0;
    xmedia_u32      ratio_tmp            = 0x40;
    xmedia_u32      short_time_min_limit = 0;
    sensor_context *imx307_ctx           = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    short_time_min_limit = (imx307_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) ? 2 :
                               ((imx307_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_3TO1_LINE) ? 3 : 2);

    if (imx307_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        /*  limitation for DOL 2t1

            SHS1 limitation:
            2 or more
            RHS1 - 2 or less

            SHS2 limitation:
            RHS1 + 2 or more
            FSC - 2 or less

            RHS1 Limitation
            2n + 5 (n = 0,1,2...)
            RHS1 <= FSC - BRL * 2 - 21

            short exposure time = RHS1 - (SHS1 + 1) <= RHS1 - 3
            long exposure time = FSC - (SHS2 + 1) <= FSC - (RHS1 + 3)
            ExposureShort + ExposureLong <= FSC - 6
            short exposure time <= (FSC - 6) / (ratio + 1)
        */
        if (g_imx307_wdr_mode[dev] == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
            int_time_max_tmp0 = imx307_ctx->fl[1] - 6 - imx307_ctx->wdr_int_time[0];
            int_time_max_tmp  = imx307_ctx->fl[0] - 10;
            int_time_max_tmp  = (int_time_max_tmp0 < int_time_max_tmp) ? int_time_max_tmp0 : int_time_max_tmp;
            inttime_attr->max_inttime[0] = int_time_max_tmp;
            inttime_attr->min_inttime[0] = short_time_min_limit;
        } else {
            int_time_max_tmp0 = ((imx307_ctx->fl[1] - 6 - imx307_ctx->wdr_int_time[0]) * 0x40) /
                                SENSOR_DIV_0_TO_1(inttime_attr->ratio[0]);
            int_time_max_tmp = ((imx307_ctx->fl[0] - 6) * 0x40) / SENSOR_DIV_0_TO_1(inttime_attr->ratio[0] + 0x40);
            int_time_max_tmp = (int_time_max_tmp0 < int_time_max_tmp) ? int_time_max_tmp0 : int_time_max_tmp;
            int_time_max_tmp = (int_time_max_tmp > (g_imx307_state[dev].u32RHS1_MAX - 3)) ?
                                   (g_imx307_state[dev].u32RHS1_MAX - 3) :
                                   int_time_max_tmp;
            int_time_max_tmp = (int_time_max_tmp == 0) ? 1 : int_time_max_tmp;
        }
    } else {
    }

    if (int_time_max_tmp >= short_time_min_limit) {
        if (IS_LINE_WDR_MODE(imx307_ctx->wdr_mode)) {
            inttime_attr->max_inttime[0] = int_time_max_tmp;
            for (i = 1; i < XMEDIA_ISP_WDR_CHN_MAX_NUM; i++) {
                inttime_attr->max_inttime[i] = inttime_attr->max_inttime[i - 1] * inttime_attr->ratio[i - 1] >> 6;
            }

            inttime_attr->min_inttime[0] = short_time_min_limit;
            for (i = 1; i < XMEDIA_ISP_WDR_CHN_MAX_NUM; i++) {
                inttime_attr->min_inttime[i] = inttime_attr->min_inttime[i - 1] * inttime_attr->ratio[i - 1] >> 6;
            }
        } else {
        }
    } else {
        int_time_max_tmp = short_time_min_limit;
        if (imx307_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
            ratio_tmp                    = 0xFFF;
            inttime_attr->max_inttime[0] = int_time_max_tmp;
            inttime_attr->max_inttime[1] = inttime_attr->max_inttime[0] * ratio_tmp >> 6;
        } else {
        }
        for (i = 0; i < XMEDIA_ISP_WDR_CHN_MAX_NUM; i++) {
            inttime_attr->min_inttime[i] = inttime_attr->max_inttime[i];
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32      ret;
    sensor_context *imx307_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    IMX307_GET_CTX(dev, imx307_ctx);
    SENSOR_CHECK_PTR_RETURN(imx307_ctx);

    ret = imx307_get_min_fps(imx307_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = imx307_get_max_fps(imx307_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = imx307_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx307_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);

    memcpy(info->sensor_name, IMX307_NAME, sizeof(IMX307_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property   = imx307_get_property;
    info->isp_func.pfn_sensor_set_work_mode  = imx307_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes = imx307_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror         = imx307_mirror;
    info->isp_func.pfn_sensor_flip           = imx307_flip;
    info->isp_func.pfn_sensor_set_bus_info   = imx307_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr   = imx307_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param = imx307_set_init_param;
    info->isp_func.pfn_sensor_start          = imx307_start;
    info->isp_func.pfn_sensor_stop           = imx307_stop;
    info->isp_func.pfn_sensor_standby        = imx307_standby;
    info->isp_func.pfn_sensor_resume         = imx307_resume;
    info->isp_func.pfn_sensor_write_reg      = imx307_write_reg;
    info->isp_func.pfn_sensor_read_reg       = imx307_read_reg;
    info->isp_func.pfn_sensor_init           = imx307_init;
    info->isp_func.pfn_sensor_exit           = imx307_exit;

    // called by ISP
    info->isp_func.pfn_sensor_set_attr            = imx307_set_attr;
    info->isp_func.pfn_sensor_get_isp_default     = imx307_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = imx307_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = imx307_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = imx307_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = imx307_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = imx307_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = imx307_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = imx307_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = imx307_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = imx307_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = imx307_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = imx307_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = imx307_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = imx307_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = imx307_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = imx307_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = imx307_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx307_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = imx307_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = imx307_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));

    info.dev_id = dev;
    ret         = imx307_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "IMX307 register failed! error code = 0x%x.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx307_unregister(xmedia_u32 pipe)
{
    xmedia_s32 ret;
    xmedia_u32 dev;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = imx307_match_dev(pipe, &dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_PRINT("IMX307 need not unregister function!\n");
        return XMEDIA_SUCCESS;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "IMX307 unregister function failed!\n");
        return XMEDIA_FAILURE;
    }

    imx307_ctx_exit(dev);
    g_imx307_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;

}
