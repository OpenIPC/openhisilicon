#include <stdlib.h>
#include <string.h>
#include "cv4003.h"
#include "cv4003_ctrl.h"
#include "cv4003_ex.h"
#include "xmedia_isp.h"

#define CV4003_NAME          "CV4003"
#define CV4003_SPECS_MAX_NUM 2

#ifdef __linux__
#define CV4003_ISP_DEFAULT_SUPPORT
#endif

#define CV4003_4M_BIT_RATE_LINEAR 1000
#define CV4003_1M_BIT_RATE_LINEAR 840

#define CV4003_REG_ADDR_STANDBY 0x3000

#define CV4003_RES_IS_1440P(w, h) ((w) == 2560 && (h) == 1440)
#define CV4003_RES_IS_720P(w, h)  ((w) == 1280 && (h) == 720)

#define CV4003_ERR_MODE_PRINT(sns_attr)                                                                              \
    do {                                                                                                             \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sns_attr->width,              \
                     sns_attr->height, sns_attr->wdr_mode);                                                          \
    } while (0)

SENSOR_PRIORITY_DATA sensor_context *g_cv4003_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define CV4003_SET_CONTEXT(dev, sns_ctx) g_cv4003_ctx[dev] = sns_ctx
#define CV4003_GET_CONTEXT(dev, sns_ctx) sns_ctx = g_cv4003_ctx[dev]

SENSOR_PRIORITY_DATA static xmedia_s32 g_cv4003_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

static xmedia_u32 g_cv4003_again[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_cv4003_dgain[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_cv4003_isp_dgain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_cv4003_init_time[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u32 g_cv4003_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_cv4003_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_cv4003_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u16 g_cv4003_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

// awb static param for Fuji Lens New IR_Cut
#define CV4003_CALIBRATE_STATIC_TEMP       5000
#define CV4003_CALIBRATE_STATIC_WB_R_GAIN  509
#define CV4003_CALIBRATE_STATIC_WB_GR_GAIN 256
#define CV4003_CALIBRATE_STATIC_WB_GB_GAIN 256
#define CV4003_CALIBRATE_STATIC_WB_B_GAIN  532

// Calibration results for Auto WB Planck
#define CV4003_CALIBRATE_AWB_P1 66
#define CV4003_CALIBRATE_AWB_P2 44
#define CV4003_CALIBRATE_AWB_Q1 -146
#define CV4003_CALIBRATE_AWB_A1 221006
#define CV4003_CALIBRATE_AWB_B1 128
#define CV4003_CALIBRATE_AWB_C1 -168531

// Rgain and Bgain of the golden sample
#define CV4003_GOLDEN_RGAIN 0
#define CV4003_GOLDEN_BGAIN 0

#define CV4003_FULL_LINES_MAX 0xFFFFF

#define CV4003_GAIN_NODE_NUM  249

static xmedia_u32 g_cv4003_again_table[CV4003_GAIN_NODE_NUM] = {
    1024, 1024, 1024, 1024, 1040, 1040, 1040, 1040,
    1056, 1056, 1056, 1056, 1072, 1072, 1072, 1072,
    1088, 1088, 1088, 1104, 1104, 1104, 1120, 1120,
    1120, 1120, 1136, 1136, 1136, 1152, 1152, 1152,
    1168, 1168, 1168, 1184, 1184, 1184, 1200, 1200,
    1200, 1216, 1216, 1216, 1232, 1232, 1248, 1248,
    1248, 1264, 1264, 1264, 1280, 1280, 1296, 1296,
    1296, 1312, 1312, 1328, 1328, 1344, 1344, 1344,
    1360, 1360, 1376, 1376, 1392, 1392, 1408, 1408,
    1424, 1424, 1440, 1440, 1456, 1456, 1472, 1472,
    1488, 1488, 1504, 1504, 1520, 1520, 1536, 1536,
    1552, 1568, 1568, 1584, 1584, 1600, 1616, 1616,
    1632, 1648, 1648, 1664, 1680, 1680, 1696, 1712,
    1712, 1728, 1744, 1744, 1760, 1776, 1792, 1808,
    1808, 1824, 1840, 1856, 1872, 1872, 1888, 1904,
    1920, 1936, 1952, 1968, 1984, 2000, 2016, 2016,
    2048, 2064, 2080, 2096, 2112, 2128, 2144, 2160,
    2176, 2192, 2208, 2240, 2256, 2272, 2288, 2304,
    2336, 2352, 2368, 2400, 2416, 2448, 2464, 2496,
    2512, 2544, 2560, 2592, 2608, 2640, 2672, 2688,
    2720, 2752, 2784, 2816, 2848, 2880, 2912, 2944,
    2976, 3008, 3040, 3072, 3120, 3152, 3184, 3232,
    3264, 3312, 3360, 3392, 3440, 3488, 3536, 3584,
    3632, 3680, 3744, 3792, 3840, 3904, 3968, 4032,
    4096, 4160, 4224, 4288, 4368, 4432, 4512, 4592,
    4672, 4752, 4848, 4944, 5040, 5136, 5232, 5344,
    5456, 5568, 5696, 5824, 5952, 6080, 6240, 6384,
    6544, 6720, 6896, 7072, 7280, 7488, 7696, 7936,
    8192, 8448, 8736, 9024, 9360, 9696, 10080, 10480,
    10912, 11392, 11904, 12480, 13104, 13792, 14560, 15408,
    16384, 17472, 18720, 20160, 21840, 23824, 26208, 29120,
    32768
};

SENSOR_PRIORITY_RODATA static const xmedia_sensor_property g_cv4003_property[CV4003_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        2560, 1440, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_GRBG,
          XMEDIA_VIDEO_BAYER_FMT_GBRG, XMEDIA_VIDEO_BAYER_FMT_RGGB },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, CV4003_4M_BIT_RATE_LINEAR,
    },
    {
        // width, height, wdr_mode
        1280, 720, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 25, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_GRBG,
          XMEDIA_VIDEO_BAYER_FMT_GBRG, XMEDIA_VIDEO_BAYER_FMT_RGGB },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, CV4003_1M_BIT_RATE_LINEAR,
    },
};

SENSOR_PRIORITY_RODATA static const xmedia_sensor_capability g_cv4003_capability = {
    .max_width  = 2560,
    .max_height = 1440,
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
        .addr_byte_num     = CV4003_ADDR_BYTE,
        .data_byte_num     = CV4003_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { CV4003_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x01 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { CV4003_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x00 },
    },
};

SENSOR_PRIORITY_FUNC static xmedia_s32 cv4003_ctx_init(xmedia_u32 dev)
{
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    CV4003_GET_CONTEXT(dev, cv4003_ctx);

    if (cv4003_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    cv4003_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (cv4003_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(cv4003_ctx, 0, sizeof(sensor_context));
    cv4003_ctx->i2c_addr         = CV4003_I2C_ADDR;
    cv4003_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_2L;
    cv4003_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    cv4003_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    cv4003_ctx->size.width       = 2560;
    cv4003_ctx->size.height      = 1440;
    cv4003_ctx->fps              = g_cv4003_property[0].max_fps;
    cv4003_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    cv4003_ctx->img_mode             = CV4003_4M_LINEAR_MODE;
    cv4003_ctx->fl_std               = CV4003_VMAX_4M_LINEAR;
    cv4003_ctx->fl[SENSOR_CUR_FRAME] = CV4003_VMAX_4M_LINEAR;
    cv4003_ctx->fl[SENSOR_PRE_FRAME] = CV4003_VMAX_4M_LINEAR;

    CV4003_SET_CONTEXT(dev, cv4003_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_void cv4003_ctx_exit(xmedia_u32 dev)
{
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_FREE(cv4003_ctx);
    CV4003_SET_CONTEXT(dev, XMEDIA_NULL);
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv4003_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    for (i = 0; i < CV4003_SPECS_MAX_NUM; i++) {
        if (g_cv4003_property[i].width == cv4003_ctx->size.width &&
            g_cv4003_property[i].height == cv4003_ctx->size.height &&
            g_cv4003_property[i].wdr_mode == cv4003_ctx->wdr_mode) {
            memcpy(property, &g_cv4003_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n",
                    cv4003_ctx->size.width, cv4003_ctx->size.height, cv4003_ctx->wdr_mode);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 cv4003_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    cv4003_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "No support mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    cv4003_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv4003_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        cv4003_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        cv4003_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        cv4003_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 cv4003_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    cv4003_ctx->i2c_addr = slave_addr;
    ret = cv4003_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    *slave_addr = cv4003_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv4003_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    cv4003_ctx->init_mode = init_mode;

    ret = cv4003_i2c_init(dev, cv4003_ctx->bus_info.i2c_dev, cv4003_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    cv4003_ctx->init = XMEDIA_TRUE;

#ifdef USE_PREROLL_INFO
    ret = cv4003_preroll_720p_write_cis2psram_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);
#endif

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv4003_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    ret = cv4003_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    cv4003_ctx->init = XMEDIA_FALSE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    cv4003_ctx->mirror_en = mirror_en;
    ret = cv4003_set_mirror_flip(dev, mirror_en, cv4003_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    cv4003_ctx->flip_en = flip_en;

    ret = cv4003_set_mirror_flip(dev, cv4003_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv4003_set_image_mode(sensor_context *cv4003_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);
    if (cv4003_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (CV4003_RES_IS_1440P(sns_attr->width, sns_attr->height)) {
            *image_mode         = CV4003_4M_LINEAR_MODE;
            cv4003_ctx->fl_std = CV4003_VMAX_4M_LINEAR;
        } else if(CV4003_RES_IS_720P(sns_attr->width, sns_attr->height)){
            *image_mode         = CV4003_1M_LINEAR_MODE;
            cv4003_ctx->fl_std = CV4003_VMAX_1M_LINEAR;
            cv4003_ctx->regs_info[SENSOR_CUR_FRAME].reg_num = 0;

        }else {
            CV4003_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    }  else {
        CV4003_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    cv4003_ctx->size.width  = sns_attr->width;
    cv4003_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv4003_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            cv4003_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(cv4003_ctx->wdr_int_time, 0, sizeof(cv4003_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_void cv4003_init_common_reg_info(sensor_context *cv4003_ctx)
{
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_EXP_L].delay_frame_num = 2;
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_EXP_L].reg_addr = CV4003_REG_ADDR_EXP_L;
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_EXP_M].delay_frame_num = 2;
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_EXP_M].reg_addr = CV4003_REG_ADDR_EXP_M;
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_EXP_H].delay_frame_num = 2;
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_EXP_H].reg_addr = CV4003_REG_ADDR_EXP_H;

    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_AGAIN].delay_frame_num = 2;
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_AGAIN].reg_addr = CV4003_REG_ADDR_AGAIN;
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_DGAIN_L].delay_frame_num = 2;
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_DGAIN_L].reg_addr = CV4003_REG_ADDR_DGAIN_L;
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_DGAIN_H].delay_frame_num = 2;
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_DGAIN_H].reg_addr = CV4003_REG_ADDR_DGAIN_H;
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_VMAX_L].delay_frame_num  = 2;
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_VMAX_L].reg_addr = CV4003_REG_ADDR_VMAX_L;
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_VMAX_M].delay_frame_num  = 2;
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_VMAX_M].reg_addr = CV4003_REG_ADDR_VMAX_M;
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_VMAX_H].delay_frame_num  = 2;
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_VMAX_H].reg_addr = CV4003_REG_ADDR_VMAX_H;

    return;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv4003_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = cv4003_ctx->bus_info.i2c_dev;
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = CV4003_REG_MAX_NUM;

    for (i = 0; i < cv4003_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = cv4003_ctx->i2c_addr;
        cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = CV4003_ADDR_BYTE;
        cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = CV4003_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    cv4003_init_common_reg_info(cv4003_ctx);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv4003_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    // Set wdr mode
    ret = cv4003_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = cv4003_set_image_mode(cv4003_ctx, &cv4003_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    cv4003_ctx->fl[SENSOR_CUR_FRAME] = cv4003_ctx->fl_std;
    cv4003_ctx->fl[SENSOR_PRE_FRAME] = cv4003_ctx->fl[SENSOR_CUR_FRAME];

    ret = cv4003_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef CV4003_ISP_DEFAULT_SUPPORT
    isp_default->blc         = XMEDIA_NULL;
    isp_default->bnr         = &g_cv4003_bnr;
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

    switch(cv4003_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            isp_default->drc = XMEDIA_NULL;
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            isp_default->drc = XMEDIA_NULL;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support mode: %d\n", cv4003_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
#else
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from cv4003!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 cv4003_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (cv4003_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 256;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 256;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 256;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 256;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    for (i = 0; i < cv4003_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            cv4003_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &cv4003_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&cv4003_ctx->regs_info[SENSOR_PRE_FRAME], &cv4003_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    cv4003_ctx->fl[SENSOR_PRE_FRAME] = cv4003_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;

}

static xmedia_s32 cv4003_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    if (cv4003_ctx->mirror_en && cv4003_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (cv4003_ctx->mirror_en && (!cv4003_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!cv4003_ctx->mirror_en) && cv4003_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < CV4003_SPECS_MAX_NUM; i++) {
        if (g_cv4003_property[i].width == cv4003_ctx->size.width &&
            g_cv4003_property[i].height == cv4003_ctx->size.height &&
            g_cv4003_property[i].wdr_mode == cv4003_ctx->wdr_mode) {
            *bayer_pattern = g_cv4003_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= CV4003_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv4003_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_cv4003_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    cv4003_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 cv4003_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_cv4003_again[dev]         = init_param->again;
    g_cv4003_dgain[dev]         = init_param->dgain;
    g_cv4003_isp_dgain[dev]     = init_param->ispdgain;
    g_cv4003_init_time[dev]     = init_param->exp_time;
    g_cv4003_exposure[dev]      = init_param->exposure;
    g_cv4003_sample_r_gain[dev] = init_param->sample_rgain;
    g_cv4003_sample_b_gain[dev] = init_param->sample_bgain;

    g_cv4003_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_cv4003_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_cv4003_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

xmedia_s32 cv4003_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case CV4003_4M_LINEAR_MODE:
            *max_fps = 30;
            break;
        case CV4003_1M_LINEAR_MODE:
             *max_fps = 120;
             break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->flicker_freq    = 50 * 256;
    ae_sns_dft->full_lines      = sns_ctx->fl_std;
    ae_sns_dft->full_lines_std  = sns_ctx->fl_std;
    ae_sns_dft->full_lines_max  = CV4003_FULL_LINES_MAX;
    ae_sns_dft->lines_per_500ms = (sns_ctx->fl_std * 30) / 2;
    ae_sns_dft->hmax_times      = (1000000000) / (sns_ctx->fl_std * 30);

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy  = 0.015625;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 100 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->ispdgain_sep_thd    = 16 << ae_sns_dft->ispdgain_shift;

    memcpy(&ae_sns_dft->piris_attr, &g_cv4003_piris_attr, sizeof(xmedia_isp_piris_attr));
    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_32_0;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    return XMEDIA_SUCCESS;
}

static xmedia_void cv4003_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 2;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 32768;
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 512;
    ae_sns_dft->min_dgain        = 64;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x38;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure   = g_cv4003_exposure[dev] ? g_cv4003_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - CV4003_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 2;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = 2;
    return;
}

static xmedia_s32 cv4003_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    memset(ae_sns_dft, 0, sizeof(xmedia_isp_ae_sensor_default));

    ret = cv4003_get_ae_common_default(dev, cv4003_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (cv4003_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            cv4003_get_ae_linear_default(dev, cv4003_ctx, ae_sns_dft);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", cv4003_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{
    switch (img_mode) {
        case CV4003_4M_LINEAR_MODE:
        case CV4003_1M_LINEAR_MODE:
            *min_fps = 2.75;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case CV4003_4M_LINEAR_MODE:
            *vmax = CV4003_VMAX_4M_LINEAR;
            break;
        case CV4003_1M_LINEAR_MODE:
            *vmax = CV4003_VMAX_1M_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = cv4003_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = cv4003_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = cv4003_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = vmax_max_fps * max_fps / min_fps;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    xmedia_u32   lines;
    xmedia_float max_fps, min_fps;
    xmedia_u32   vmax;
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);

    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    ret = cv4003_get_vmax(cv4003_ctx->img_mode, &lines);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = cv4003_get_max_fps(cv4003_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = cv4003_get_min_fps(cv4003_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "not support fps: %f\n", fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    vmax = lines * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);

    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_VMAX_L].data = SENSOR_LOW_8BITS(vmax);
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_VMAX_M].data = SENSOR_HIGH_8BITS(vmax);
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_VMAX_H].data = ((vmax)&0xFF0000)>>16;

    cv4003_ctx->fl_std               = vmax;
    cv4003_ctx->fl[SENSOR_CUR_FRAME] = cv4003_ctx->fl_std;
    cv4003_ctx->fps                  = fps;

    ae_sns_dft->fps            = fps;
    ae_sns_dft->full_lines_std = cv4003_ctx->fl_std;
    ae_sns_dft->max_int_time   = cv4003_ctx->fl_std - CV4003_EXP_OFFSET_LINEAR;
    ae_sns_dft->full_lines     = cv4003_ctx->fl[SENSOR_CUR_FRAME];

    SENSOR_PRINT("dev[%d]-cv4003 set fps = %f\n", dev, ae_sns_dft->fps);

    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 * 注意：2to1 wdr模式下，曝光比和ae wdr mode 由AE传入，调用本接口前，AE需先调用cv4003_get_max_inttime
 */
static xmedia_s32 cv4003_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *cv4003_ctx = XMEDIA_NULL;
    xmedia_u32 lines_max;
    xmedia_float min_fps;
    xmedia_u32 vmax;
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    if (cv4003_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", cv4003_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = cv4003_get_min_fps(cv4003_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = cv4003_get_min_fps_vmax(cv4003_ctx->img_mode, &lines_max);
    SENSOR_CHECK_RET_RETURN(ret);

    vmax = full_lines;
    vmax = SENSOR_MIN(vmax, lines_max);
    cv4003_ctx->fl[SENSOR_CUR_FRAME] = vmax;
    cv4003_ctx->fps                  = min_fps * lines_max / vmax;

    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_VMAX_L].data = SENSOR_LOW_8BITS(vmax);
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_VMAX_M].data = SENSOR_HIGH_8BITS(vmax);
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_VMAX_H].data = ((vmax & 0xFF0000) >> 16);

    ae_sns_dft->full_lines   = cv4003_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = cv4003_ctx->fl[SENSOR_CUR_FRAME] - CV4003_EXP_OFFSET_LINEAR;

    SENSOR_PRINT("dev[%d]-cv4003 set fps = %f\n", dev, cv4003_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context *cv4003_ctx = XMEDIA_NULL;
    xmedia_u32 value;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    value = (int_time > cv4003_ctx->fl_std)? cv4003_ctx->fl_std : (cv4003_ctx->fl_std - int_time);
    value = (value > (cv4003_ctx->fl_std - 2)) ? (cv4003_ctx->fl_std - 2) : value;
    value = value / 2 * 2;
    value = (value < 6)?  6: value;

    if (cv4003_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_EXP_L].data = SENSOR_LOW_8BITS(value);
        cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_EXP_M].data = SENSOR_HIGH_8BITS(value);
        cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_EXP_H].data = ((value)&0xFF0000)>>16;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_cv4003_again_table[CV4003_GAIN_NODE_NUM - 1]) {
        *again_lin = g_cv4003_again_table[CV4003_GAIN_NODE_NUM - 1];
        *again_db = CV4003_GAIN_NODE_NUM - 1;
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < CV4003_GAIN_NODE_NUM; i++) {
        if (*again_lin < g_cv4003_again_table[i]) {
            *again_lin = g_cv4003_again_table[i - 1];
            *again_db = i - 1;
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    xmedia_u32 dgain, dgain_reg;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(dgain_lin);
    SENSOR_CHECK_PTR_RETURN(dgain_db);

    dgain = *dgain_lin;
    dgain_reg = dgain / 16;

    *dgain_lin = dgain_reg * 16;
    *dgain_db = dgain_reg ;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_AGAIN].data   = SENSOR_LOW_8BITS(again);
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_DGAIN_L].data = SENSOR_LOW_8BITS(dgain);
    cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[CV4003_REG_DGAIN_H].data = SENSOR_HIGH_8BITS(dgain);

    return XMEDIA_SUCCESS;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 cv4003_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 cv4003_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = CV4003_CALIBRATE_STATIC_TEMP;    // wb_ref_temp 5000

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = CV4003_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = CV4003_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = CV4003_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = CV4003_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 6: point index on the awb curve param
    awb_sns_dft->wb_para[0]   = CV4003_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1]   = CV4003_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2]   = CV4003_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3]   = CV4003_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4]   = CV4003_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5]   = CV4003_CALIBRATE_AWB_C1;
    awb_sns_dft->golden_rgain = CV4003_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = CV4003_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (cv4003_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_cv4003_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_cv4003_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", cv4003_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_cv4003_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_cv4003_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_cv4003_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_cv4003_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_cv4003_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_standby(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = cv4003_write_reg(dev, CV4003_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_resume(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = cv4003_write_reg(dev, CV4003_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv4003_config_init_param(xmedia_u32 dev)
{
    xmedia_u32 i;
    sensor_context *cv4003_ctx  = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < cv4003_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < cv4003_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= cv4003_write_reg(dev, cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                cv4003_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv4003_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    if (cv4003_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = cv4003_init_image(dev, cv4003_ctx->img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = cv4003_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = cv4003_set_mirror_flip(dev, cv4003_ctx->mirror_en, cv4003_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 cv4003_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = cv4003_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv4003_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_cv4003_dev_map[index] == SENSOR_DEV_INVALID) {
            g_cv4003_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv4003_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_cv4003_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 cv4003_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    ret = cv4003_get_min_fps(cv4003_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = cv4003_get_max_fps(cv4003_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = cv4003_ctx->fps;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv4003_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, CV4003_NAME, sizeof(CV4003_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = cv4003_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = cv4003_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = cv4003_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = cv4003_mirror;
    info->isp_func.pfn_sensor_flip             = cv4003_flip;
    info->isp_func.pfn_sensor_set_bus_info     = cv4003_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = cv4003_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = cv4003_set_init_param;
    info->isp_func.pfn_sensor_start            = cv4003_start;
    info->isp_func.pfn_sensor_stop             = cv4003_stop;
    info->isp_func.pfn_sensor_standby          = cv4003_standby;
    info->isp_func.pfn_sensor_resume           = cv4003_resume;
    info->isp_func.pfn_sensor_write_reg        = cv4003_write_reg;
    info->isp_func.pfn_sensor_read_reg         = cv4003_read_reg;
    info->isp_func.pfn_sensor_init             = cv4003_init;
    info->isp_func.pfn_sensor_exit             = cv4003_exit;
    info->isp_func.pfn_sensor_set_attr         = cv4003_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = cv4003_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = cv4003_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = cv4003_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = cv4003_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = cv4003_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = cv4003_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = cv4003_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = cv4003_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = cv4003_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = cv4003_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = cv4003_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = cv4003_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = cv4003_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = cv4003_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = cv4003_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = cv4003_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = cv4003_get_awb_default;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv4003_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = cv4003_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = cv4003_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = cv4003_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "CV4003 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 cv4003_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *cv4003_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = cv4003_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    CV4003_GET_CONTEXT(dev, cv4003_ctx);
    SENSOR_CHECK_PTR_RETURN(cv4003_ctx);

    if (cv4003_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "CV4003 unregister function failed!\n");
        return ret;
    }

    cv4003_ctx_exit(dev);
    g_cv4003_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;
}
