#include <stdlib.h>
#include <string.h>
#include "cv8001.h"
#include "cv8001_ctrl.h"
#include "cv8001_ex.h"
#include "xmedia_isp.h"

#define CV8001_NAME          "CV8001"
#define CV8001_SPECS_MAX_NUM 1

#ifdef __linux__
#define CV8001_ISP_DEFAULT_SUPPORT
#endif

#define CV8001_REG_ADDR_STANDBY 0x3000

#define CV8001_RES_IS_2160P(w, h) ((w) == 3840 && (h) == 2160)

#define CV8001_ERR_MODE_PRINT(sns_attr)                                                                               \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sns_attr->width,                \
                     sns_attr->height, sns_attr->wdr_mode);                                                            \
    } while (0)

SENSOR_PRIORITY_DATA sensor_context *g_cv8001_ctx[XMEDIA_SENSOR_DEV_MAX_NUM]     = { XMEDIA_NULL };
#define CV8001_SET_CONTEXT(dev, sns_ctx) g_cv8001_ctx[dev] = sns_ctx

SENSOR_PRIORITY_DATA static xmedia_s32 g_cv8001_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

static xmedia_u32 g_cv8001_again[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_cv8001_dgain[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_cv8001_isp_dgain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_cv8001_init_time[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u32 g_cv8001_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_cv8001_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_cv8001_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u16 g_cv8001_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

// awb static param for Fuji Lens New IR_Cut
#define CV8001_CALIBRATE_STATIC_TEMP       5000
#define CV8001_CALIBRATE_STATIC_WB_R_GAIN  492
#define CV8001_CALIBRATE_STATIC_WB_GR_GAIN 256
#define CV8001_CALIBRATE_STATIC_WB_GB_GAIN 256
#define CV8001_CALIBRATE_STATIC_WB_B_GAIN  443

// Calibration results for Auto WB Planck
#define CV8001_CALIBRATE_AWB_P1 16
#define CV8001_CALIBRATE_AWB_P2 240
#define CV8001_CALIBRATE_AWB_Q1 0
#define CV8001_CALIBRATE_AWB_A1 188580
#define CV8001_CALIBRATE_AWB_B1 128
#define CV8001_CALIBRATE_AWB_C1 (-139106)

// Rgain and Bgain of the golden sample
#define CV8001_GOLDEN_RGAIN 0
#define CV8001_GOLDEN_BGAIN 0

#define CV8001_FULL_LINES_MAX 0xFFFFF

#define CV8001_GAIN_NODE_NUM  241
#define CV8001_AGAIN_NODE_NUM 101
#define CV8001_DGAIN_NODE_NUM 141

static xmedia_u32 g_cv8001_again_table[CV8001_GAIN_NODE_NUM] = {
    1024, 1059, 1097, 1135, 1175, 1217, 1259, 1304, 1349, 1397, 1446, 1497, 1549, 1604, // 3.9dB
    1660, 1719, 1779, 1842, 1906, 1973, 2043, 2114, 2189, 2266, 2345, 2428, 2513, 2602, // 8.1dB
    2693, 2788, 2886, 2987, 3092, 3201, 3313, 3430, 3550, 3675, 3804, 3938, 4076, 4219, // 12.3dB
    4368, 4521, 4680, 4845, 5015, 5191, 5374, 5562, 5758, 5960, 6170, 6387, 6611, 6843, // 16.5dB

    7084,  7333,  7591, 7857,  8134, 8419,  8715,  9022, 9339,  9667, 10007, 10358,     // 20.1dB
    10722, 11099, 11489, 11893, 12311, 12743, 13191, 13655, 14135, 14631, 15146, 15678, // 23.7dB
    16229, 16799, 17390, 18001, 18633, 19288, 19966, 20668, 21394, 22146, 22924, 23730, // 27.3dB
    24564, 25427, 26320, 27245, 28203, 29194, 30220, 31282, 32381, 33519, 34697, 35917, // 30.9dB
    37179, 38485, 39838, 41238, 42687, 44187, 45740, 47347, 49011, 50734, 52517, 54362, // 34.5dB
    56272, 58250, 60297, 62416, 64610, 66880, 69231, 71663, 74182, 76789, 79487, 82281, // 38.1dB

    85172,  88165,  91264,  94471, 97791, 101227, 104785, 108467, 112279, 116225, 120309, 124537,  // 41.7dB
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

SENSOR_PRIORITY_RODATA static const xmedia_sensor_property g_cv8001_property[CV8001_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        3840, 2160, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_GRBG,
          XMEDIA_VIDEO_BAYER_FMT_GBRG, XMEDIA_VIDEO_BAYER_FMT_RGGB },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, XMEDIA_VIDEO_DATA_WIDTH_10,
    },
};

SENSOR_PRIORITY_RODATA static const xmedia_sensor_capability g_cv8001_capability = {
    .max_width  = 3840,
    .max_height = 2160,
    .max_fps    = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_2L,
    .standby_info = {
        .standby_supported = XMEDIA_TRUE,
        .addr_byte_num     = CV8001_ADDR_BYTE,
        .data_byte_num     = CV8001_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { CV8001_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { CV8001_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x01 },
    },
};

SENSOR_PRIORITY_FUNC static xmedia_s32 cv8001_ctx_init(xmedia_u32 dev)
{
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    CV8001_GET_CONTEXT(dev, cv8001_ctx);

    if (cv8001_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    cv8001_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (cv8001_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(cv8001_ctx, 0, sizeof(sensor_context));
    cv8001_ctx->i2c_addr         = CV8001_I2C_ADDR;
    cv8001_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_2L;
    cv8001_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    cv8001_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    cv8001_ctx->size.width       = 3840;
    cv8001_ctx->size.height      = 2160;
    cv8001_ctx->fps              = g_cv8001_property[0].max_fps;
    cv8001_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    cv8001_ctx->img_mode             = CV8001_8M_LINEAR_MODE;
    cv8001_ctx->fl_std               = CV8001_VMAX_8M_LINEAR;
    cv8001_ctx->fl[SENSOR_CUR_FRAME] = CV8001_VMAX_8M_LINEAR;
    cv8001_ctx->fl[SENSOR_PRE_FRAME] = CV8001_VMAX_8M_LINEAR;

    CV8001_SET_CONTEXT(dev, cv8001_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_void cv8001_ctx_exit(xmedia_u32 dev)
{
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_FREE(cv8001_ctx);
    CV8001_SET_CONTEXT(dev, XMEDIA_NULL);
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv8001_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    for (i = 0; i < CV8001_SPECS_MAX_NUM; i++) {
        if (g_cv8001_property[i].width == cv8001_ctx->size.width &&
            g_cv8001_property[i].height == cv8001_ctx->size.height &&
            g_cv8001_property[i].wdr_mode == cv8001_ctx->wdr_mode) {
            memcpy(property, &g_cv8001_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n",
                    cv8001_ctx->size.width, cv8001_ctx->size.height, cv8001_ctx->wdr_mode);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 cv8001_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    cv8001_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv8001_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "No support mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    cv8001_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv8001_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        cv8001_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        cv8001_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        cv8001_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 cv8001_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    cv8001_ctx->i2c_addr = slave_addr;
    ret = cv8001_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv8001_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    *slave_addr = cv8001_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv8001_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    cv8001_ctx->init_mode = init_mode;

    ret = cv8001_i2c_init(dev, cv8001_ctx->bus_info.i2c_dev, cv8001_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    cv8001_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv8001_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    ret = cv8001_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    cv8001_ctx->init = XMEDIA_FALSE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv8001_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    cv8001_ctx->mirror_en = mirror_en;
    ret = cv8001_set_mirror_flip(dev, mirror_en, cv8001_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv8001_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    cv8001_ctx->flip_en = flip_en;

    ret = cv8001_set_mirror_flip(dev, cv8001_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv8001_set_image_mode(sensor_context *cv8001_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);
    if (cv8001_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (CV8001_RES_IS_2160P(sns_attr->width, sns_attr->height)) {
            *image_mode         = CV8001_8M_LINEAR_MODE;
            cv8001_ctx->fl_std = CV8001_VMAX_8M_LINEAR;
        } else{
            CV8001_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    }  else {
        CV8001_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    cv8001_ctx->size.width  = sns_attr->width;
    cv8001_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv8001_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            cv8001_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            cv8001_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
            SENSOR_PRINT("2to1 line WDR\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(cv8001_ctx->wdr_int_time, 0, sizeof(cv8001_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_void cv8001_init_common_reg_info(sensor_context *cv8001_ctx)
{
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_EXP_H_IDX].delay_frame_num = 2;
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_EXP_H_IDX].reg_addr = CV8001_REG_ADDR_EXP_H;
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_EXP_M_IDX].delay_frame_num = 2;
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_EXP_M_IDX].reg_addr = CV8001_REG_ADDR_EXP_M;
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_EXP_L_IDX].delay_frame_num = 2;
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_EXP_L_IDX].reg_addr = CV8001_REG_ADDR_EXP_L;

    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_AGAIN_1_IDX].delay_frame_num = 2;
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_AGAIN_1_IDX].reg_addr = CV8001_REG_ADDR_GAIN_1;
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_VMAX_H_IDX].delay_frame_num  = 1;
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_VMAX_H_IDX].reg_addr = CV8001_REG_ADDR_VMAX_H;
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_VMAX_M_IDX].delay_frame_num  = 1;
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_VMAX_M_IDX].reg_addr = CV8001_REG_ADDR_VMAX_M;
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_VMAX_L_IDX].delay_frame_num  = 1;
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_VMAX_L_IDX].reg_addr = CV8001_REG_ADDR_VMAX_L;

    return;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv8001_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = cv8001_ctx->bus_info.i2c_dev;
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = CV8001_REG_MAX_NUM;

    for (i = 0; i < cv8001_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = cv8001_ctx->i2c_addr;
        cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = CV8001_ADDR_BYTE;
        cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = CV8001_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    cv8001_init_common_reg_info(cv8001_ctx);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv8001_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    // Set wdr mode
    ret = cv8001_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = cv8001_set_image_mode(cv8001_ctx, &cv8001_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    cv8001_ctx->fl[SENSOR_CUR_FRAME] = cv8001_ctx->fl_std;
    cv8001_ctx->fl[SENSOR_PRE_FRAME] = cv8001_ctx->fl[SENSOR_CUR_FRAME];

    ret = cv8001_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv8001_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef CV8001_ISP_DEFAULT_SUPPORT
    isp_default->blc         = XMEDIA_NULL;
    isp_default->bnr         = XMEDIA_NULL;
    isp_default->clut_attr   = XMEDIA_NULL;
    isp_default->crosstalk   = XMEDIA_NULL;
    isp_default->csc         = XMEDIA_NULL;
    isp_default->dehaze      = XMEDIA_NULL;
    isp_default->demosaic    = XMEDIA_NULL;
    isp_default->dpc_dynamic = XMEDIA_NULL;
    isp_default->dpc_static  = XMEDIA_NULL;
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

    switch(cv8001_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            isp_default->drc = XMEDIA_NULL;
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            isp_default->drc = XMEDIA_NULL;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support mode: %d\n", cv8001_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
#else
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from cv8001!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 cv8001_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (cv8001_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 200;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 200;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 200;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 200;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv8001_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    for (i = 0; i < cv8001_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            cv8001_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &cv8001_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&cv8001_ctx->regs_info[SENSOR_PRE_FRAME], &cv8001_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    cv8001_ctx->fl[SENSOR_PRE_FRAME] = cv8001_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;

}

static xmedia_s32 cv8001_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    if (cv8001_ctx->mirror_en && cv8001_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (cv8001_ctx->mirror_en && (!cv8001_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!cv8001_ctx->mirror_en) && cv8001_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < CV8001_SPECS_MAX_NUM; i++) {
        if (g_cv8001_property[i].width == cv8001_ctx->size.width &&
            g_cv8001_property[i].height == cv8001_ctx->size.height &&
            g_cv8001_property[i].wdr_mode == cv8001_ctx->wdr_mode) {
            *bayer_pattern = g_cv8001_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= CV8001_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv8001_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_cv8001_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv8001_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    cv8001_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 cv8001_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_cv8001_again[dev]         = init_param->again;
    g_cv8001_dgain[dev]         = init_param->dgain;
    g_cv8001_isp_dgain[dev]     = init_param->ispdgain;
    g_cv8001_init_time[dev]     = init_param->exp_time;
    g_cv8001_exposure[dev]      = init_param->exposure;
    g_cv8001_sample_r_gain[dev] = init_param->sample_rgain;
    g_cv8001_sample_b_gain[dev] = init_param->sample_bgain;

    g_cv8001_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_cv8001_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_cv8001_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

xmedia_s32 cv8001_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case CV8001_8M_LINEAR_MODE:
            *max_fps = 30;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv8001_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->flicker_freq    = 50 * 256;
    ae_sns_dft->full_lines      = sns_ctx->fl_std;
    ae_sns_dft->full_lines_std  = sns_ctx->fl_std;
    ae_sns_dft->full_lines_max  = CV8001_FULL_LINES_MAX;
    ae_sns_dft->lines_per_500ms = (sns_ctx->fl_std * 30) / 2;
    ae_sns_dft->hmax_times      = (1000000000) / (sns_ctx->fl_std * 30);

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->dgain_accu.accuracy  = 1;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 2 << ae_sns_dft->ispdgain_shift;

    memcpy(&ae_sns_dft->piris_attr, &g_cv8001_piris_attr, sizeof(xmedia_isp_piris_attr));
    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_32_0;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    return XMEDIA_SUCCESS;
}

static xmedia_void cv8001_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
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
    ae_sns_dft->init_exposure   = g_cv8001_exposure[dev] ? g_cv8001_exposure[dev] : 76151;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - CV8001_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 8;

    return;
}

static xmedia_s32 cv8001_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    memset(ae_sns_dft, 0, sizeof(xmedia_isp_ae_sensor_default));

    ret = cv8001_get_ae_common_default(dev, cv8001_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (cv8001_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            cv8001_get_ae_linear_default(dev, cv8001_ctx, ae_sns_dft);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", cv8001_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv8001_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{
    switch (img_mode) {
        case CV8001_8M_LINEAR_MODE:
            *min_fps = 1.06;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv8001_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case CV8001_8M_LINEAR_MODE:
            *vmax = CV8001_VMAX_8M_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv8001_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = cv8001_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = cv8001_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = cv8001_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = vmax_max_fps * max_fps / min_fps;
    return XMEDIA_SUCCESS;
}
static xmedia_s32 cv8001_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    xmedia_u32   lines;
    xmedia_float max_fps, min_fps;
    xmedia_u32   vmax;
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);

    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    ret = cv8001_get_vmax(cv8001_ctx->img_mode, &lines);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = cv8001_get_max_fps(cv8001_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = cv8001_get_min_fps(cv8001_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "not support fps: %f\n", fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    vmax = lines * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);

    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_VMAX_L_IDX].data = ((vmax)&0xF0000)>>16;
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_VMAX_M_IDX].data = ((vmax)&0x0FF00)>>8;
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_VMAX_H_IDX].data = ((vmax)&0x000FF);

    cv8001_ctx->fl_std               = vmax;
    cv8001_ctx->fl[SENSOR_CUR_FRAME] = cv8001_ctx->fl_std;
    cv8001_ctx->fps                  = fps;

    ae_sns_dft->fps            = fps;
    ae_sns_dft->full_lines_std = cv8001_ctx->fl_std;
    ae_sns_dft->max_int_time   = cv8001_ctx->fl_std - 4;
    ae_sns_dft->full_lines     = cv8001_ctx->fl[SENSOR_CUR_FRAME];

    SENSOR_PRINT("dev[%d]-cv8001 set fps = %f\n", dev, ae_sns_dft->fps);

    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 * 注意：2to1 wdr模式下，曝光比和ae wdr mode 由AE传入，调用本接口前，AE需先调用cv8001_get_max_inttime
 */
static xmedia_s32 cv8001_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *cv8001_ctx = XMEDIA_NULL;
    xmedia_u32 lines_max;
    xmedia_float min_fps;
    xmedia_u32 vmax;
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    if (cv8001_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", cv8001_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = cv8001_get_min_fps(cv8001_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = cv8001_get_min_fps_vmax(cv8001_ctx->img_mode, &lines_max);
    SENSOR_CHECK_RET_RETURN(ret);

    vmax = full_lines;
    vmax = SENSOR_MIN(vmax, lines_max);
    cv8001_ctx->fl[SENSOR_CUR_FRAME] = vmax;
    cv8001_ctx->fps                  = min_fps * lines_max / vmax;

    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_VMAX_L_IDX].data = SENSOR_LOW_8BITS(vmax);
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_VMAX_M_IDX].data = SENSOR_LOW_8BITS(vmax);
    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_VMAX_H_IDX].data = SENSOR_HIGH_8BITS(vmax);

    ae_sns_dft->full_lines   = cv8001_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = cv8001_ctx->fl[SENSOR_CUR_FRAME] - CV8001_EXP_OFFSET_LINEAR;

    SENSOR_PRINT("dev[%d]-cv8001 set fps = %f\n", dev, cv8001_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv8001_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context *cv8001_ctx = XMEDIA_NULL;
    xmedia_u32 value;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    value = cv8001_ctx->fl[SENSOR_CUR_FRAME] - int_time;
    value = SENSOR_MIN(SENSOR_MAX(value, 8), cv8001_ctx->fl[SENSOR_CUR_FRAME] - 4);

    if (cv8001_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_EXP_H_IDX].data = ((value)&0x000FF);
        cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_EXP_M_IDX].data = ((value)&0x0FF00)>>8;
        cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_EXP_L_IDX].data = ((value)&0xF0000)>>16;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv8001_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_cv8001_again_table[CV8001_AGAIN_NODE_NUM - 1]) {
        *again_lin = g_cv8001_again_table[CV8001_AGAIN_NODE_NUM - 1];
        *again_db = CV8001_AGAIN_NODE_NUM - 1;
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < CV8001_AGAIN_NODE_NUM; i++) {
        if (*again_lin < g_cv8001_again_table[i]) {
            *again_lin = g_cv8001_again_table[i - 1];
            *again_db = i - 1;
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv8001_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    xmedia_s32 i;
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(dgain_lin);
    SENSOR_CHECK_PTR_RETURN(dgain_db);

    if (*dgain_lin >= g_cv8001_again_table[CV8001_DGAIN_NODE_NUM - 1]) {
        *dgain_lin = g_cv8001_again_table[CV8001_DGAIN_NODE_NUM - 1];
        *dgain_db  = CV8001_DGAIN_NODE_NUM - 1;
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < CV8001_DGAIN_NODE_NUM; i++) {
        if (*dgain_lin < g_cv8001_again_table[i]) {
            *dgain_lin = g_cv8001_again_table[i - 1];
            *dgain_db  = i - 1;
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv8001_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    xmedia_u32 gain = again + dgain;
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    if(gain > 240){  //max 72dB, 0.3dB per step.
        gain = 240;
    }

    cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV8001_REG_AGAIN_1_IDX].data = gain;

    return XMEDIA_SUCCESS;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 cv8001_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 cv8001_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = CV8001_CALIBRATE_STATIC_TEMP;    // wb_ref_temp 5000

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = CV8001_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = CV8001_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = CV8001_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = CV8001_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 6: point index on the awb curve param
    awb_sns_dft->wb_para[0]   = CV8001_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1]   = CV8001_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2]   = CV8001_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3]   = CV8001_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4]   = CV8001_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5]   = CV8001_CALIBRATE_AWB_C1;
    awb_sns_dft->golden_rgain = CV8001_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = CV8001_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (cv8001_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_cv8001_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_cv8001_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_cv8001_awb_wdr_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_cv8001_awb_wdr_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", cv8001_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_cv8001_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_cv8001_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_cv8001_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_cv8001_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_cv8001_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv8001_standby(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = cv8001_write_reg(dev, CV8001_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv8001_resume(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = cv8001_write_reg(dev, CV8001_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv8001_config_init_param(xmedia_u32 dev)
{
    xmedia_u32 i;
    sensor_context *cv8001_ctx  = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < cv8001_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < cv8001_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= cv8001_write_reg(dev, cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                cv8001_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv8001_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    if (cv8001_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = cv8001_init_image(dev, cv8001_ctx->img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = cv8001_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = cv8001_set_mirror_flip(dev, cv8001_ctx->mirror_en, cv8001_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv8001_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = cv8001_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv8001_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_cv8001_dev_map[index] == SENSOR_DEV_INVALID) {
            g_cv8001_dev_map[index] = pipe;
            *dev                     = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv8001_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_cv8001_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 cv8001_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    ret = cv8001_get_min_fps(cv8001_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = cv8001_get_max_fps(cv8001_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = cv8001_ctx->fps;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv8001_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, CV8001_NAME, sizeof(CV8001_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = cv8001_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = cv8001_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = cv8001_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = cv8001_mirror;
    info->isp_func.pfn_sensor_flip             = cv8001_flip;
    info->isp_func.pfn_sensor_set_bus_info     = cv8001_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = cv8001_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = cv8001_set_init_param;
    info->isp_func.pfn_sensor_start            = cv8001_start;
    info->isp_func.pfn_sensor_stop             = cv8001_stop;
    info->isp_func.pfn_sensor_standby          = cv8001_standby;
    info->isp_func.pfn_sensor_resume           = cv8001_resume;
    info->isp_func.pfn_sensor_write_reg        = cv8001_write_reg;
    info->isp_func.pfn_sensor_read_reg         = cv8001_read_reg;
    info->isp_func.pfn_sensor_init             = cv8001_init;
    info->isp_func.pfn_sensor_exit             = cv8001_exit;
    info->isp_func.pfn_sensor_set_attr         = cv8001_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = cv8001_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = cv8001_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = cv8001_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = cv8001_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = cv8001_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = cv8001_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = cv8001_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = cv8001_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = cv8001_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = cv8001_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = cv8001_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = cv8001_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = cv8001_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = cv8001_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = cv8001_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = cv8001_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = cv8001_get_awb_default;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv8001_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = cv8001_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = cv8001_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = cv8001_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "CV8001 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 cv8001_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *cv8001_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = cv8001_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    CV8001_GET_CONTEXT(dev, cv8001_ctx);
    SENSOR_CHECK_PTR_RETURN(cv8001_ctx);

    if (cv8001_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "CV8001 unregister function failed!\n");
        return ret;
    }

    cv8001_ctx_exit(dev);
    g_cv8001_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;
}
