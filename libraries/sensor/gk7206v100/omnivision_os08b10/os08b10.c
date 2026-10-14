#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "os08b10.h"
#include "os08b10_ctrl.h"
#include "os08b10_ex.h"

#define OS08B10_NAME          "OS08B10"
#define OS08B10_SPECS_MAX_NUM 2

#define OS08B10_8M_BIT_RATE_LINEAR 1500
#define OS08B10_8M_BIT_RATE_WDR    1500

#ifdef __linux__
// #define OS08B10_ISP_DEFAULT_SUPPORT
#endif

#define OS08B10_REG_ADDR_STANDBY 0x0100

#define OS08B10_RES_IS_2160P(w, h) ((w) == 3840 && (h) == 2160)

#define OS08B10_ERR_MODE_PRINT(sns_attr)                                                                               \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sns_attr->width,                \
                     sns_attr->height, sns_attr->wdr_mode);                                                            \
    } while (0)

static sensor_context *g_os08b10_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define OS08B10_GET_CTX(dev, ctx) ctx = g_os08b10_ctx[dev]
#define OS08B10_SET_CTX(dev, ctx) g_os08b10_ctx[dev] = ctx

static xmedia_s32 g_os08b10_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] =
                                                                       SENSOR_DEV_INVALID };

static xmedia_u32 g_os08b10_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_os08b10_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_os08b10_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u16 g_os08b10_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

static xmedia_bool             g_ae_route_ex_valid[XMEDIA_SENSOR_DEV_MAX_NUM]   = { 0 };
static xmedia_isp_ae_route     g_init_ae_route[XMEDIA_SENSOR_DEV_MAX_NUM]       = { { 0 } };
static xmedia_isp_ae_ext_route g_init_ae_route_ex[XMEDIA_SENSOR_DEV_MAX_NUM]    = { { 0 } };
static xmedia_isp_ae_route     g_init_ae_route_sf[XMEDIA_SENSOR_DEV_MAX_NUM]    = { { 0 } };
static xmedia_isp_ae_ext_route g_init_ae_route_sf_ex[XMEDIA_SENSOR_DEV_MAX_NUM] = { { 0 } };

static const xmedia_sensor_property g_os08b10_property[OS08B10_SPECS_MAX_NUM] = {
    {
     // width, height, wdr_mode
     3840, 2160, XMEDIA_VIDEO_WDR_MODE_NONE,
     // wdr_format, max_fps, input_clock
     XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
     // output_intf, pixel_format, bit_width
     XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_12,
     // bayer_format - mirror&flip /mirror /flip /normal
    { XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB,
      XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB },
     // vcid, bit_rate (TODO: bit_rate to be confirmed)
     { 0 }, OS08B10_8M_BIT_RATE_LINEAR,
    },

    {
     // width, height, wdr_mode
     3840, 2160, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
     // wdr_format, max_fps, input_clock
     XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
     // output_intf, pixel_format, bit_width
     XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_12,
     // bayer_format - mirror&flip /mirror /flip /normal
    { XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB,
      XMEDIA_VIDEO_BAYER_FMT_RGGB, XMEDIA_VIDEO_BAYER_FMT_RGGB },
     // vcid, bit_rate (TODO: bit_rate to be confirmed)
     { 0, 1 }, OS08B10_8M_BIT_RATE_WDR, },
};

#define OS08B10_AGAIN_INDEX_MAX 157
static const xmedia_u32 g_os08b10_again_table[OS08B10_AGAIN_INDEX_MAX] = {
    1024,  1088,  1152,  1216,  1280,  1344,  1408,  1472,  1536,  1600,  1664,  1728,  1792,  1856,  1920,  1984,
    2048,  2176,  2304,  2432,  2560,  2688,  2816,  2944,  3072,  3200,  3328,  3456,  3584,  3712,  3840,  3968,
    4096,  4352,  4608,  4864,  5120,  5376,  5632,  5888,  6144,  6400,  6656,  6912,  7168,  7424,  7680,  7936,
    8192,  8704,  9216,  9728,  10240, 10752, 11264, 11776, 12288, 12800, 13312, 13824, 14336, 14848, 15360, 15872,
    16384, 16896, 17408, 17920, 18432, 18944, 19456, 19968, 20480, 20992, 21504, 22016, 22528, 23040, 23552, 24064,
    24576, 25088, 25600, 26112, 26624, 27136, 27648, 28160, 28672, 29184, 29696, 30208, 30720, 31232, 31744, 32256,
    32768, 33280, 33792, 34304, 34816, 35328, 35840, 36352, 36864, 37376, 37888, 38400, 38912, 39424, 39936, 40448,
    40960, 41472, 41984, 42496, 43008, 43520, 44032, 44544, 45056, 45568, 46080, 46592, 47104, 47616, 48128, 48640,
    49152, 49664, 50176, 50688, 51200, 51712, 52224, 52736, 53248, 53760, 54272, 54784, 55296, 55808, 56320, 56832,
    57344, 57856, 58368, 58880, 59392, 59904, 60416, 60928, 61440, 61952, 62464, 62976, 63488
};

// awb static param for Fuji Lens New IR_Cut
#define OS08B10_CALIBRATE_STATIC_TEMP       5000
#define OS08B10_CALIBRATE_STATIC_WB_R_GAIN  492
#define OS08B10_CALIBRATE_STATIC_WB_GR_GAIN 256
#define OS08B10_CALIBRATE_STATIC_WB_GB_GAIN 256
#define OS08B10_CALIBRATE_STATIC_WB_B_GAIN  443

// Calibration results for Auto WB Planck
#define OS08B10_CALIBRATE_AWB_P1 16
#define OS08B10_CALIBRATE_AWB_P2 240
#define OS08B10_CALIBRATE_AWB_Q1 0
#define OS08B10_CALIBRATE_AWB_A1 188580
#define OS08B10_CALIBRATE_AWB_B1 128
#define OS08B10_CALIBRATE_AWB_C1 -139106

// Rgain and Bgain of the golden sample
#define OS08B10_GOLDEN_RGAIN 0
#define OS08B10_GOLDEN_BGAIN 0

static const xmedia_sensor_capability g_os08b10_capability = {
    .max_width   = 3840,
    .max_height  = 2160,
    .max_fps     = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_12,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_4L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = OS08B10_ADDR_BYTE,
        .data_byte_num     = OS08B10_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { OS08B10_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { OS08B10_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x01 },
    },
};

static xmedia_s32 os08b10_ctx_init(xmedia_u32 dev)
{
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    OS08B10_GET_CTX(dev, os08b10_ctx);
    if (os08b10_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    os08b10_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (os08b10_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(os08b10_ctx, 0, sizeof(sensor_context));
    os08b10_ctx->i2c_addr             = OS08B10_I2C_ADDR;
    os08b10_ctx->lanes                = XMEDIA_SENSOR_MIPI_LANES_4L;
    os08b10_ctx->work_mode            = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    os08b10_ctx->bus_info.i2c_dev     = SENSOR_I2C_INVALID;
    os08b10_ctx->size.width           = g_os08b10_property[0].width;
    os08b10_ctx->size.height          = g_os08b10_property[0].height;
    os08b10_ctx->fps                  = g_os08b10_property[0].max_fps;
    os08b10_ctx->wdr_mode             = g_os08b10_property[0].wdr_mode;
    os08b10_ctx->img_mode             = OS08B10_8M_30FPS_12BIT_LINEAR_MODE;
    os08b10_ctx->fl_std               = OS08B10_VMAX_8M_LINEAR;
    os08b10_ctx->fl[SENSOR_CUR_FRAME] = OS08B10_VMAX_8M_LINEAR;
    os08b10_ctx->fl[SENSOR_PRE_FRAME] = OS08B10_VMAX_8M_LINEAR;
    OS08B10_SET_CTX(dev, os08b10_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_void os08b10_ctx_exit(xmedia_u32 dev)
{
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_FREE(os08b10_ctx);
    OS08B10_SET_CTX(dev, XMEDIA_NULL);

    return;
}

static xmedia_s32 os08b10_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    for (i = 0; i < OS08B10_SPECS_MAX_NUM; i++) {
        if (g_os08b10_property[i].width == os08b10_ctx->size.width &&
            g_os08b10_property[i].height == os08b10_ctx->size.height &&
            g_os08b10_property[i].wdr_mode == os08b10_ctx->wdr_mode) {
            memcpy(property, &g_os08b10_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Please check input param!\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 os08b10_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os08b10_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Invalid mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os08b10_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        os08b10_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        os08b10_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        os08b10_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 os08b10_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    os08b10_ctx->i2c_addr = slave_addr;
    ret = os08b10_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    *slave_addr = os08b10_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

// called by ISP
static xmedia_s32 os08b10_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    os08b10_ctx->init_mode = init_mode;
    ret = os08b10_i2c_init(dev, os08b10_ctx->bus_info.i2c_dev, os08b10_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    os08b10_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    ret = os08b10_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    os08b10_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    os08b10_ctx->mirror_en = mirror_en;
    ret                    = os08b10_set_mirror_flip(dev, mirror_en, os08b10_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    os08b10_ctx->flip_en = flip_en;
    ret                  = os08b10_set_mirror_flip(dev, os08b10_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_set_image_mode(sensor_context *os08b10_ctx, xmedia_u8 *image_mode,
                                         const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (os08b10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (OS08B10_RES_IS_2160P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OS08B10_8M_30FPS_12BIT_LINEAR_MODE;
            os08b10_ctx->fl_std = OS08B10_VMAX_8M_LINEAR;
        } else {
            OS08B10_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else if (os08b10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (OS08B10_RES_IS_2160P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OS08B10_8M_30FPS_12BIT_WDR_MODE;
            os08b10_ctx->fl_std = OS08B10_VMAX_8M_WDR;
        } else {
            OS08B10_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        OS08B10_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os08b10_ctx->size.width  = sns_attr->width;
    os08b10_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            os08b10_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            os08b10_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
            SENSOR_PRINT("2to1 line WDR\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(os08b10_ctx->wdr_int_time, 0, sizeof(os08b10_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

#define OS08B10_LINEAR_REG_INFO_MAX_NUM   OS08B10_REG_L_MAX_NUM
#define OS08B10_2TO1_WDR_REG_INFO_MAX_NUM OS08B10_REG_MAX_NUM
static xmedia_void os08b10_init_common_reg_info(sensor_context *os08b10_ctx)
{
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_EXP_H_IDX].delay_frame_num =
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - OS08B10_REG_DELAY_CONFIG_NUM;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_EXP_H_IDX].reg_addr = OS08B10_REG_ADDR_EXP_H;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_EXP_L_IDX].delay_frame_num =
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - OS08B10_REG_DELAY_CONFIG_NUM;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_EXP_L_IDX].reg_addr = OS08B10_REG_ADDR_EXP_L;

    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_AGAIN_H_IDX].delay_frame_num =
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - OS08B10_REG_DELAY_CONFIG_NUM;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_AGAIN_H_IDX].reg_addr = OS08B10_REG_ADDR_AGAIN_H;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_AGAIN_L_IDX].delay_frame_num =
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - OS08B10_REG_DELAY_CONFIG_NUM;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_AGAIN_L_IDX].reg_addr = OS08B10_REG_ADDR_AGAIN_L;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_DGAIN_H_IDX].delay_frame_num =
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - OS08B10_REG_DELAY_CONFIG_NUM;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_DGAIN_H_IDX].reg_addr = OS08B10_REG_ADDR_DGAIN_H;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_DGAIN_L_IDX].delay_frame_num =
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - OS08B10_REG_DELAY_CONFIG_NUM;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_DGAIN_L_IDX].reg_addr = OS08B10_REG_ADDR_DGAIN_L;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_DGAIN_LL_IDX].delay_frame_num =
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - OS08B10_REG_DELAY_CONFIG_NUM;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_DGAIN_LL_IDX].reg_addr = OS08B10_REG_ADDR_DGAIN_LL;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_HCG_IDX].delay_frame_num =
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - OS08B10_REG_DELAY_CONFIG_NUM - 1;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_HCG_IDX].reg_addr = OS08B10_REG_ADDR_HCG;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_VMAX_H_IDX].delay_frame_num =
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - OS08B10_REG_DELAY_CONFIG_NUM - 1;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_VMAX_H_IDX].reg_addr = OS08B10_REG_ADDR_VMAX_H;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_VMAX_L_IDX].delay_frame_num =
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - OS08B10_REG_DELAY_CONFIG_NUM - 1;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_VMAX_L_IDX].reg_addr = OS08B10_REG_ADDR_VMAX_L;

    return;
}

static xmedia_void os08b10_init_2to1_wdr_reg_info(sensor_context *os08b10_ctx)
{
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_S_EXP_H_IDX].delay_frame_num =
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - OS08B10_REG_DELAY_CONFIG_NUM;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_S_EXP_H_IDX].reg_addr = OS08B10_REG_ADDR_S_EXP_H;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_S_EXP_L_IDX].delay_frame_num =
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - OS08B10_REG_DELAY_CONFIG_NUM;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_S_EXP_L_IDX].reg_addr = OS08B10_REG_ADDR_S_EXP_L;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_S_AGAIN_H_IDX].delay_frame_num =
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - OS08B10_REG_DELAY_CONFIG_NUM;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_S_AGAIN_H_IDX].reg_addr = OS08B10_REG_ADDR_S_AGAIN_H;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_S_AGAIN_L_IDX].delay_frame_num =
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - OS08B10_REG_DELAY_CONFIG_NUM;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_S_AGAIN_L_IDX].reg_addr = OS08B10_REG_ADDR_S_AGAIN_L;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_S_DGAIN_H_IDX].delay_frame_num =
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - OS08B10_REG_DELAY_CONFIG_NUM;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_S_DGAIN_H_IDX].reg_addr = OS08B10_REG_ADDR_S_DGAIN_H;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_S_DGAIN_L_IDX].delay_frame_num =
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - OS08B10_REG_DELAY_CONFIG_NUM;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_S_DGAIN_L_IDX].reg_addr = OS08B10_REG_ADDR_S_DGAIN_L;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_S_DGAIN_LL_IDX].delay_frame_num =
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max - OS08B10_REG_DELAY_CONFIG_NUM;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_S_DGAIN_LL_IDX].reg_addr =
        OS08B10_REG_ADDR_S_DGAIN_LL;
    return;
}

static xmedia_s32 os08b10_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = os08b10_ctx->bus_info.i2c_dev;
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = OS08B10_LINEAR_REG_INFO_MAX_NUM;

    if (os08b10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num = OS08B10_2TO1_WDR_REG_INFO_MAX_NUM;
    }

    for (i = 0; i < os08b10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = os08b10_ctx->i2c_addr;
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = OS08B10_ADDR_BYTE;
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = OS08B10_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    os08b10_init_common_reg_info(os08b10_ctx);

    // init 2to1 wdr mode Regs
    if (os08b10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        os08b10_init_2to1_wdr_reg_info(os08b10_ctx);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    // Set wdr mode
    ret = os08b10_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = os08b10_set_image_mode(os08b10_ctx, &os08b10_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    os08b10_ctx->fl[SENSOR_CUR_FRAME] = os08b10_ctx->fl_std;
    os08b10_ctx->fl[SENSOR_PRE_FRAME] = os08b10_ctx->fl[SENSOR_CUR_FRAME];

    ret = os08b10_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef OS08B10_ISP_DEFAULT_SUPPORT
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from os08b10!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 os08b10_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (os08b10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x400;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x400;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x400;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x400;
    } else {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x4B0;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x4B0;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x4B0;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x4B0;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    if (os08b10_ctx->mirror_en && os08b10_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (os08b10_ctx->mirror_en && (!os08b10_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!os08b10_ctx->mirror_en) && os08b10_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < OS08B10_SPECS_MAX_NUM; i++) {
        if (g_os08b10_property[i].width == os08b10_ctx->size.width &&
            g_os08b10_property[i].height == os08b10_ctx->size.height &&
            g_os08b10_property[i].wdr_mode == os08b10_ctx->wdr_mode) {
            *bayer_pattern = g_os08b10_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= OS08B10_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);
    memcpy(capability, &g_os08b10_capability, sizeof(xmedia_sensor_capability));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os08b10_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 os08b10_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_os08b10_exposure[dev]      = init_param->exposure;
    g_os08b10_sample_r_gain[dev] = init_param->sample_rgain;
    g_os08b10_sample_b_gain[dev] = init_param->sample_bgain;

    g_os08b10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_os08b10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_os08b10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    // ae快启的部分暂时不加

    return XMEDIA_SUCCESS;
}

xmedia_s32 os08b10_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case OS08B10_8M_30FPS_12BIT_LINEAR_MODE:
            *max_fps = 30.0;
            break;
        case OS08B10_8M_30FPS_12BIT_WDR_MODE:
            *max_fps = 30.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 os08b10_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{
    switch (img_mode) {
        case OS08B10_8M_30FPS_12BIT_LINEAR_MODE:
            *min_fps = 1.06;
            break;
        case OS08B10_8M_30FPS_12BIT_WDR_MODE:
            *min_fps = 5.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}
static xmedia_s32 os08b10_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = os08b10_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256; // light flicker freq: 50Hz, accuracy: 256
    ae_sns_dft->full_lines_max = OS08B10_FULL_LINES_MAX;
    ae_sns_dft->hmax_times     = (1000000000) / (sns_ctx->fl_std * 30);

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;
    ae_sns_dft->again_accu.offset    = 0;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->dgain_accu.accuracy  = 1;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 32 << ae_sns_dft->ispdgain_shift;

    memcpy(&ae_sns_dft->piris_attr, &g_os08b10_piris_attr, sizeof(xmedia_isp_piris_attr));
    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_4;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_5_6;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    // lines per second: VTS * max_fps
    ae_sns_dft->lines_per_500ms  = (sns_ctx->fl_std * max_fps) / 2;
    ae_sns_dft->man_ratio_enable = XMEDIA_FALSE;

    return XMEDIA_SUCCESS;
}

static xmedia_void os08b10_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                 xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 63488;
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 1024;
    ae_sns_dft->min_dgain        = 1024;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_os08b10_exposure[dev] ? g_os08b10_exposure[dev] : 76151;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - OS08B10_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 8;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    ae_sns_dft->ae_route_ex_valid = g_ae_route_ex_valid[dev];
    memcpy(&ae_sns_dft->ae_route_attr, &g_init_ae_route[dev], sizeof(xmedia_isp_ae_route));
    memcpy(&ae_sns_dft->ae_ext_route_attr, &g_init_ae_route_ex[dev], sizeof(xmedia_isp_ae_ext_route));

    return;
}

static xmedia_s32 os08b10_get_ae_2to1_wdr_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                  xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = -0.66;

    ae_sns_dft->max_again        = 63488;
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 16383;
    ae_sns_dft->min_dgain        = 1024;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;
    ae_sns_dft->init_exposure    = g_os08b10_exposure[dev] ? g_os08b10_exposure[dev] : 52000;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - OS08B10_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 2;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;
    ae_sns_dft->max_ispdgain_target = 4 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->diff_gain_support   = XMEDIA_TRUE;

    if (os08b10_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
        ae_sns_dft->ae_compensation = 0x38;
        ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    } else {
        ae_sns_dft->ae_compensation  = 0x20;
        ae_sns_dft->ae_exp_mode      = XMEDIA_ISP_AE_STRATEGY_EXP_LOWLIGHT_PRIOR;
        ae_sns_dft->man_ratio_enable = XMEDIA_TRUE;

        ae_sns_dft->ratio[0] = 0x400;
        ae_sns_dft->ratio[1] = 0x40;
        ae_sns_dft->ratio[2] = 0x40;
    }

    ae_sns_dft->ae_route_ex_valid = g_ae_route_ex_valid[dev];
    memcpy(&ae_sns_dft->ae_route_attr, &g_init_ae_route[dev], sizeof(xmedia_isp_ae_route));
    memcpy(&ae_sns_dft->ae_ext_route_attr, &g_init_ae_route_ex[dev], sizeof(xmedia_isp_ae_ext_route));
    memcpy(&ae_sns_dft->ae_route_sf_attr, &g_init_ae_route_sf[dev], sizeof(xmedia_isp_ae_route));
    memcpy(&ae_sns_dft->ae_ext_route_sf_attr, &g_init_ae_route_sf_ex[dev], sizeof(xmedia_isp_ae_ext_route));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = os08b10_get_ae_common_default(dev, os08b10_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (os08b10_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            os08b10_get_ae_linear_default(dev, os08b10_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            os08b10_get_ae_2to1_wdr_default(dev, os08b10_ctx, ae_sns_dft);
            break;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context    *os08b10_ctx                               = XMEDIA_NULL;
    static xmedia_bool first[XMEDIA_SENSOR_DEV_MAX_NUM]          = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 1 };
    static xmedia_u32  short_int_time[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
    static xmedia_u32  long_int_time[XMEDIA_SENSOR_DEV_MAX_NUM]  = { 0 };

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    if (os08b10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (first[dev]) { // 0: short exposure
            os08b10_ctx->wdr_int_time[0] = int_time;
            short_int_time[dev]          = int_time;
            first[dev]                   = XMEDIA_FALSE;
        } else { // 1: long exposure
            os08b10_ctx->wdr_int_time[1] = int_time;
            long_int_time[dev]           = int_time;
            os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_EXP_H_IDX].data =
                SENSOR_HIGH_8BITS(long_int_time[dev]);
            os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_EXP_L_IDX].data =
                SENSOR_LOW_8BITS(long_int_time[dev]);
            os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_S_EXP_H_IDX].data =
                SENSOR_HIGH_8BITS(short_int_time[dev]);
            os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_S_EXP_L_IDX].data =
                SENSOR_LOW_8BITS(short_int_time[dev]);
            first[dev] = XMEDIA_TRUE;
        }
    } else if (os08b10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_EXP_H_IDX].data = SENSOR_HIGH_8BITS(int_time);
        os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_EXP_L_IDX].data = SENSOR_LOW_8BITS(int_time);
        first[dev]                                                                    = XMEDIA_TRUE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_os08b10_again_table[OS08B10_AGAIN_INDEX_MAX - 1]) {
        *again_lin = g_os08b10_again_table[OS08B10_AGAIN_INDEX_MAX - 1];
        *again_db  = OS08B10_AGAIN_INDEX_MAX - 1;
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < OS08B10_AGAIN_INDEX_MAX; i++) {
        if (*again_lin < g_os08b10_again_table[i]) {
            *again_lin = g_os08b10_again_table[i - 1];
            *again_db  = i - 1;
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_void os08b10_gains_regs_set(xmedia_u32 dev, xmedia_u32 again_reg, xmedia_u32 dgain_reg,
                                          xmedia_u32 hcg_reg)
{
    static xmedia_bool first_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 1 };
    sensor_context    *os08b10_ctx                           = XMEDIA_NULL;

    OS08B10_GET_CTX(dev, os08b10_ctx);

    if (os08b10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        os08b10_ctx->regs_info[0].i2c_data[OS08B10_REG_AGAIN_L_IDX].data = (again_reg & 0xf0);
        os08b10_ctx->regs_info[0].i2c_data[OS08B10_REG_AGAIN_H_IDX].data = ((again_reg & 0x1f00) >> 8);

        os08b10_ctx->regs_info[0].i2c_data[OS08B10_REG_DGAIN_LL_IDX].data = ((dgain_reg << 6) & 0xff);
        os08b10_ctx->regs_info[0].i2c_data[OS08B10_REG_DGAIN_L_IDX].data  = ((dgain_reg >> 2) & 0xff);
        os08b10_ctx->regs_info[0].i2c_data[OS08B10_REG_DGAIN_H_IDX].data  = ((dgain_reg >> 10) & 0xff);
        os08b10_ctx->regs_info[0].i2c_data[OS08B10_REG_HCG_IDX].data      = (hcg_reg & 0xff);
    } else if (os08b10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (first_gain[dev] == XMEDIA_TRUE) { // long frame
            os08b10_ctx->regs_info[0].i2c_data[OS08B10_REG_AGAIN_L_IDX].data = (again_reg & 0xf0);
            os08b10_ctx->regs_info[0].i2c_data[OS08B10_REG_AGAIN_H_IDX].data = ((again_reg & 0x1f00) >> 8);

            os08b10_ctx->regs_info[0].i2c_data[OS08B10_REG_DGAIN_LL_IDX].data = ((dgain_reg << 6) & 0xff);
            os08b10_ctx->regs_info[0].i2c_data[OS08B10_REG_DGAIN_L_IDX].data  = ((dgain_reg >> 2) & 0xff);
            os08b10_ctx->regs_info[0].i2c_data[OS08B10_REG_DGAIN_H_IDX].data  = ((dgain_reg >> 10) & 0xff);

            first_gain[dev] = XMEDIA_FALSE;
        } else { // short frame
            os08b10_ctx->regs_info[0].i2c_data[OS08B10_REG_S_AGAIN_L_IDX].data = (again_reg & 0xf0);
            os08b10_ctx->regs_info[0].i2c_data[OS08B10_REG_S_AGAIN_H_IDX].data = ((again_reg & 0x1f00) >> 8);

            os08b10_ctx->regs_info[0].i2c_data[OS08B10_REG_S_DGAIN_LL_IDX].data = ((dgain_reg << 6) & 0xff);
            os08b10_ctx->regs_info[0].i2c_data[OS08B10_REG_S_DGAIN_L_IDX].data  = ((dgain_reg >> 2) & 0xff);
            os08b10_ctx->regs_info[0].i2c_data[OS08B10_REG_S_DGAIN_H_IDX].data  = ((dgain_reg >> 10) & 0xff);

            first_gain[dev] = XMEDIA_TRUE;
        }
        os08b10_ctx->regs_info[0].i2c_data[OS08B10_REG_HCG_IDX].data = (hcg_reg & 0xff);
    }
}

static xmedia_s32 os08b10_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *os08b10_ctx = XMEDIA_NULL;
    xmedia_u32      again_reg, dgain_reg;
    xmedia_u32      hcg_reg = 0x02;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    if (g_os08b10_again_table[again] < 0x1100) {
        if (os08b10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
            hcg_reg = 0x12;
        } else if (os08b10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
            hcg_reg = 0x32;
        } else {
            hcg_reg = 0x12;
        }

        again_reg = g_os08b10_again_table[again] >> 2;
    } else {
        hcg_reg   = 0x02;
        again_reg = (xmedia_u32)(((xmedia_float)g_os08b10_again_table[again] + 2) / 4.1);
        again_reg = again_reg >> 2;
    }

    again_reg = SENSOR_CLIP3(again_reg, 0x100, 0xF80);
    dgain_reg = SENSOR_CLIP3(dgain, 0x400, 0x3FFF);

    os08b10_gains_regs_set(dev, again_reg, dgain_reg, hcg_reg);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = OS08B10_CALIBRATE_STATIC_TEMP;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = OS08B10_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = OS08B10_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = OS08B10_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = OS08B10_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 6: point index on the awb curve param
    awb_sns_dft->wb_para[0]      = OS08B10_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1]      = OS08B10_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2]      = OS08B10_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3]      = OS08B10_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4]      = OS08B10_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5]      = OS08B10_CALIBRATE_AWB_C1;
    awb_sns_dft->golden_rgain    = OS08B10_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain    = OS08B10_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (os08b10_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_os08b10_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_os08b10_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_os08b10_awb_wdr_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_os08b10_awb_wdr_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
    }

    awb_sns_dft->init_rgain   = g_os08b10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_os08b10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_os08b10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_os08b10_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_os08b10_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_standby(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os08b10_write_reg(dev, OS08B10_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_resume(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os08b10_write_reg(dev, OS08B10_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *os08b10_ctx  = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < os08b10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < os08b10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= os08b10_write_reg(dev, os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                 os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_start(xmedia_u32 dev)
{
    xmedia_s32               ret;
    xmedia_u8                img_mode;
    xmedia_sensor_mipi_lanes mipi_lanes;
    xmedia_sensor_work_mode  work_mode;
    sensor_context          *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    work_mode  = os08b10_ctx->work_mode;
    mipi_lanes = os08b10_ctx->lanes;
    img_mode   = os08b10_ctx->img_mode;

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL || mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check param!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (os08b10_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = os08b10_init_image(dev, img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = os08b10_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = os08b10_set_mirror_flip(dev, os08b10_ctx->mirror_en, os08b10_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os08b10_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_os08b10_dev_map[index] == SENSOR_DEV_INVALID) {
            g_os08b10_dev_map[index] = pipe;
            *dev                     = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 os08b10_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_os08b10_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

typedef struct {
    xmedia_u32 dec[XMEDIA_ISP_WDR_CHN_MAX_NUM];
    xmedia_u32 inc[XMEDIA_ISP_WDR_CHN_MAX_NUM];
} time_step;

typedef struct {
    xmedia_u32            ver_lines;
    xmedia_u32            max_ver_lines;
    xmedia_float          max_fps;
    xmedia_float          min_fps;
    xmedia_u32            width;
    xmedia_u32            height;
    xmedia_u8             sns_mode;
    xmedia_video_wdr_mode wdr_mode;
} os08b10_video_mode_tbl;

const static os08b10_video_mode_tbl g_os08b10_mode_tbl[2] = {
    { OS08B10_VMAX_8M_LINEAR,     OS08B10_FULL_LINES_MAX, 30, 1.06, 3840, 2180, 0,      XMEDIA_VIDEO_WDR_MODE_NONE },
    {    OS08B10_VMAX_8M_WDR, OS08B10_FULL_LINES_MAX_WDR, 30,  5.0, 3840, 2180, 0, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE },
};

static xmedia_u32 g_os08b10_pre_int_time[XMEDIA_ISP_WDR_CHN_MAX_NUM] = { 0 };

static xmedia_s32 os08b10_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    for (i = 0; i < os08b10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            os08b10_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &os08b10_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&os08b10_ctx->regs_info[SENSOR_PRE_FRAME], &os08b10_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    os08b10_ctx->fl[SENSOR_PRE_FRAME] = os08b10_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_bool os08b10_get_2to1_vmax_limit(xmedia_u32 *vmax, xmedia_u32 full_line, xmedia_u32 step,
                                                      xmedia_bool fps_up)
{
    if (fps_up) {
        if ((*vmax) < full_line - step) {
            (*vmax) = (full_line - step);
            return XMEDIA_FALSE;
        }
    } else {
        if ((*vmax) > full_line + step) {
            (*vmax) = (full_line + step);
            return XMEDIA_FALSE;
        }
    }
    return XMEDIA_TRUE;
}

static xmedia_s32 os08b10_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      lines, lines_max, vmax, step;
    xmedia_float    max_fps, min_fps;
    xmedia_bool     achieve_fps_ok;
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    lines     = g_os08b10_mode_tbl[os08b10_ctx->img_mode].ver_lines;
    lines_max = g_os08b10_mode_tbl[os08b10_ctx->img_mode].max_ver_lines;
    max_fps   = g_os08b10_mode_tbl[os08b10_ctx->img_mode].max_fps;
    min_fps   = g_os08b10_mode_tbl[os08b10_ctx->img_mode].min_fps;

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "not support fps: %f\n", fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    achieve_fps_ok = XMEDIA_TRUE;
    vmax           = lines * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    if (os08b10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        step           = 20;
        achieve_fps_ok = os08b10_get_2to1_vmax_limit(&vmax, os08b10_ctx->fl[0], step, fps > ae_sns_dft->fps);
        vmax           = SENSOR_MIN(vmax, lines_max);
        vmax           = SENSOR_MIN(vmax, 0x486);
    } else {
        vmax = SENSOR_MIN(vmax, lines_max);
    }

    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_VMAX_L_IDX].data = SENSOR_LOW_8BITS(vmax);
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_VMAX_H_IDX].data = SENSOR_HIGH_8BITS(vmax);

    os08b10_ctx->fl_std               = vmax;
    os08b10_ctx->fps                  = fps;
    ae_sns_dft->lines_per_500ms       = lines * max_fps / 2;
    ae_sns_dft->fps                   = (achieve_fps_ok == XMEDIA_TRUE) ? fps : (lines * max_fps * 0x40 / vmax / 0x40);
    ae_sns_dft->full_lines_std        = os08b10_ctx->fl_std;
    ae_sns_dft->max_int_time          = os08b10_ctx->fl_std - OS08B10_EXP_OFFSET_LINEAR;
    os08b10_ctx->fl[SENSOR_CUR_FRAME] = os08b10_ctx->fl_std;
    ae_sns_dft->full_lines            = os08b10_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->hmax_times            = (1000000000) / (os08b10_ctx->fl_std * SENSOR_DIV_0_TO_1_FLOAT(fps));

    SENSOR_PRINT("dev[%d]- os08b10 set fps: %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_void os08b10_clip_ratio_range(xmedia_u32                          *a_exp_ratio,
                                                   const xmedia_sensor_ae_inttime_attr *int_time,
                                                   xmedia_u32                           wdr_frame_num)
{
    xmedia_u32 i, ratio_min, ratio_max;
    for (i = 0; i < wdr_frame_num - 1; ++i) {
        ratio_max      = int_time->max_inttime[i + 1] * 0x40 / int_time->min_inttime[i];
        ratio_min      = int_time->min_inttime[i + 1] * 0x40 / int_time->max_inttime[i];
        a_exp_ratio[i] = SENSOR_MIN(SENSOR_MAX(a_exp_ratio[i], ratio_min), ratio_max);
        a_exp_ratio[i] = SENSOR_MIN(SENSOR_MAX(a_exp_ratio[i], 0x40), 0x4000);
    }
}

static xmedia_void os08b10_step_limit(xmedia_sensor_ae_inttime_attr *int_time, const time_step *step,
                                             xmedia_u32 full_lines, xmedia_u32 wdr_frame_num)
{
    xmedia_u32 i;
    xmedia_u32 max_int_time = 0;
    for (i = 0; i < wdr_frame_num; ++i) {
        if (g_os08b10_pre_int_time[i] == 0) {
            return;
        }
    }
    for (i = 0; i < wdr_frame_num; ++i) {
        if (step->inc[i] > 0) {
            int_time->max_inttime[i] = SENSOR_MIN(g_os08b10_pre_int_time[i] + step->inc[i], int_time->max_inttime[i]);
        }
        if (step->dec[i] > 0) {
            if (g_os08b10_pre_int_time[i] > step->dec[i]) {
                int_time->min_inttime[i] = SENSOR_MAX(g_os08b10_pre_int_time[i] - step->dec[i], int_time->min_inttime[i]);
            } else {
                int_time->min_inttime[i] = SENSOR_MAX(int_time->min_inttime[i], 0);
            }
        }
        if (int_time->min_inttime[i] > int_time->max_inttime[i]) {
            int_time->max_inttime[i] = int_time->min_inttime[i];
        }
        // make sure LEF > SEF1 and SEF1 > SEF2
        if (i > 0) {
            int_time->max_inttime[i] = SENSOR_MAX(int_time->max_inttime[i], int_time->max_inttime[i - 1]);
            int_time->min_inttime[i] = SENSOR_MAX(int_time->min_inttime[i], int_time->min_inttime[i - 1]);
        }
    }
    // make sure max_int_time < full_lines
    for (i = 0; i < wdr_frame_num; ++i) {
        max_int_time += int_time->max_inttime[i];
    }
    if (max_int_time > full_lines) {
        max_int_time = max_int_time - full_lines;
        for (i = 0; i < wdr_frame_num; ++i) {
            if (int_time->max_inttime[i] - int_time->min_inttime[i] > max_int_time) {
                int_time->max_inttime[i] = int_time->max_inttime[i] - max_int_time;
                return;
            }
        }
    }

    return;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 os08b10_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    xmedia_u32      short_max0, i, short_max, short_time_min_limit;
    time_step       step        = { { 0 } };
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);
    memcpy(os08b10_ctx->ratio, inttime_attr->ratio, sizeof(inttime_attr->ratio));

    short_time_min_limit = 2;

    for (i = 0; i < 2; ++i) {
        step.inc[i] = (os08b10_ctx->fl[SENSOR_PRE_FRAME] - OS08B10_STEP_OFFSET) >> 2;
        step.dec[i] = (os08b10_ctx->fl[SENSOR_PRE_FRAME] - OS08B10_STEP_OFFSET) >> 2;
    }

    if (os08b10_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
        short_max0 = os08b10_ctx->fl[1] - OS08B10_MARGIN - step.dec[0] - os08b10_ctx->wdr_int_time[0];
        short_max  = os08b10_ctx->fl[0] - OS08B10_MARGIN - step.dec[0];
        short_max  = (short_max0 < short_max) ? short_max0 : short_max;

        inttime_attr->max_inttime[0] = short_time_min_limit;
        inttime_attr->min_inttime[0] = short_time_min_limit;
        inttime_attr->max_inttime[1] = short_max;
        inttime_attr->min_inttime[1] = short_time_min_limit;
    } else {
        short_max0 = ((os08b10_ctx->fl[1] - OS08B10_MARGIN - step.dec[0] - os08b10_ctx->wdr_int_time[0]) * 0x40) /
                     SENSOR_DIV_0_TO_1(os08b10_ctx->ratio[0]);
        short_max = ((os08b10_ctx->fl[0] - OS08B10_MARGIN - step.inc[0]) * 0x40) / (os08b10_ctx->ratio[0] + 0x40);
        short_max = (short_max0 < short_max) ? short_max0 : short_max;
        short_max = (short_max == 0) ? 1 : short_max;

        if (short_max >= short_time_min_limit) {
            inttime_attr->max_inttime[0] = short_max;
            inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * os08b10_ctx->ratio[0]) >> 6;
            inttime_attr->min_inttime[0] = short_time_min_limit;
            inttime_attr->min_inttime[1] = (inttime_attr->min_inttime[0] * os08b10_ctx->ratio[0]) >> 6;
        } else {
            short_max                    = short_time_min_limit;
            inttime_attr->max_inttime[0] = short_max;
            inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * 0xFFF) >> 6;
            inttime_attr->min_inttime[0] = inttime_attr->max_inttime[0];
            inttime_attr->min_inttime[1] = inttime_attr->max_inttime[1];
        }
    }

    os08b10_step_limit(inttime_attr, &step, os08b10_ctx->fl[1], 2);
    os08b10_clip_ratio_range(os08b10_ctx->ratio, (const xmedia_sensor_ae_inttime_attr *)inttime_attr, 2);

    g_os08b10_pre_int_time[0] = inttime_attr->max_inttime[0];
    g_os08b10_pre_int_time[1] = inttime_attr->min_inttime[0];

    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 * 注意：2to1 wdr模式下，曝光比和ae wdr mode 由AE传入，调用本接口前，AE需先调用os08b10_get_wdr_max_inttime
 */
static xmedia_s32 os08b10_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                                    xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *os08b10_ctx = XMEDIA_NULL;
    xmedia_u32      lines_max;
    xmedia_u32      step;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    if (os08b10_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", os08b10_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    lines_max = g_os08b10_mode_tbl[os08b10_ctx->img_mode].max_ver_lines;

    step = 20;
    if (os08b10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        os08b10_get_2to1_vmax_limit(&full_lines, os08b10_ctx->fl[SENSOR_CUR_FRAME], step,
                                           full_lines < os08b10_ctx->fl[SENSOR_CUR_FRAME]);
        full_lines = SENSOR_MIN(full_lines, lines_max);
        full_lines = SENSOR_MIN(full_lines, 0x486);
    } else {
        full_lines = SENSOR_MIN(full_lines, lines_max);
    }

    os08b10_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    os08b10_ctx->fps                  = g_os08b10_mode_tbl[os08b10_ctx->img_mode].ver_lines *
                                        g_os08b10_mode_tbl[os08b10_ctx->img_mode].max_fps / full_lines;

    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_VMAX_L_IDX].data = SENSOR_LOW_8BITS(full_lines);
    os08b10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08B10_REG_VMAX_H_IDX].data = SENSOR_HIGH_8BITS(full_lines);

    ae_sns_dft->full_lines   = os08b10_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = os08b10_ctx->fl[SENSOR_CUR_FRAME] - OS08B10_EXP_OFFSET_LINEAR;

    SENSOR_PRINT("dev[%d]- os08b10 set fps: %f\n", dev, os08b10_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32      ret;
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    ret = os08b10_get_min_fps(os08b10_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os08b10_get_max_fps(os08b10_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = os08b10_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08b10_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, OS08B10_NAME, sizeof(OS08B10_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property   = os08b10_get_property;
    info->isp_func.pfn_sensor_set_work_mode  = os08b10_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes = os08b10_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror         = os08b10_mirror;
    info->isp_func.pfn_sensor_flip           = os08b10_flip;
    info->isp_func.pfn_sensor_set_bus_info   = os08b10_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr   = os08b10_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param = os08b10_set_init_param;
    info->isp_func.pfn_sensor_start          = os08b10_start;
    info->isp_func.pfn_sensor_stop           = os08b10_stop;
    info->isp_func.pfn_sensor_standby        = os08b10_standby;
    info->isp_func.pfn_sensor_resume         = os08b10_resume;
    info->isp_func.pfn_sensor_write_reg      = os08b10_write_reg;
    info->isp_func.pfn_sensor_read_reg       = os08b10_read_reg;
    info->isp_func.pfn_sensor_init           = os08b10_init;
    info->isp_func.pfn_sensor_exit           = os08b10_exit;

    // called by ISP
    info->isp_func.pfn_sensor_set_attr            = os08b10_set_attr;
    info->isp_func.pfn_sensor_get_isp_default     = os08b10_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = os08b10_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = os08b10_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = os08b10_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = os08b10_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = os08b10_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = os08b10_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = os08b10_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = os08b10_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = os08b10_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = os08b10_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = os08b10_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = os08b10_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = os08b10_calc_dgain; // not support
    info->ae_func.pfn_sensor_update_gains       = os08b10_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = os08b10_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = os08b10_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 os08b10_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = os08b10_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = os08b10_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = os08b10_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OS08B10 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 os08b10_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *os08b10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = os08b10_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    OS08B10_GET_CTX(dev, os08b10_ctx);
    SENSOR_CHECK_PTR_RETURN(os08b10_ctx);

    if (os08b10_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please exit at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OS08B10 unregister function failed!\n");
        return ret;
    }

    os08b10_ctx_exit(dev);
    g_os08b10_dev_map[dev] = SENSOR_DEV_INVALID; // reset dev

    return XMEDIA_SUCCESS;
}
