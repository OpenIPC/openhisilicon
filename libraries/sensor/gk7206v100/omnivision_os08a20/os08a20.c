#include <stdlib.h>
#include <string.h>
#include "os08a20.h"
#include "os08a20_ctrl.h"
#include "os08a20_ex.h"

#ifdef FPGA
#define OS08A20_INPUT_CLOCK_LINEAR 9000000
#define OS08A20_INPUT_CLOCK_WDR    9000000
#define OS08A20_8M_MAX_FPS_LINEAR  3.0
#define OS08A20_5M_MAX_FPS_LINEAR  OS08A20_8M_MAX_FPS_LINEAR
#define OS08A20_4M_MAX_FPS_LINEAR  OS08A20_8M_MAX_FPS_LINEAR
#define OS08A20_3M_MAX_FPS_LINEAR  OS08A20_8M_MAX_FPS_LINEAR
#define OS08A20_2M_MAX_FPS_LINEAR  OS08A20_8M_MAX_FPS_LINEAR
#define OS08A20_8M_MAX_FPS_WDR     3
#define OS08A20_5M_MAX_FPS_WDR     OS08A20_8M_MAX_FPS_WDR
#define OS08A20_4M_MAX_FPS_WDR     OS08A20_8M_MAX_FPS_WDR
#define OS08A20_3M_MAX_FPS_WDR     OS08A20_8M_MAX_FPS_WDR
#define OS08A20_2M_MAX_FPS_WDR     OS08A20_8M_MAX_FPS_WDR
#define OS08A20_8M_BIT_RATE_LINEAR 600
#define OS08A20_5M_BIT_RATE_LINEAR OS08A20_8M_BIT_RATE_LINEAR
#define OS08A20_4M_BIT_RATE_LINEAR OS08A20_8M_BIT_RATE_LINEAR
#define OS08A20_3M_BIT_RATE_LINEAR OS08A20_8M_BIT_RATE_LINEAR
#define OS08A20_2M_BIT_RATE_LINEAR OS08A20_8M_BIT_RATE_LINEAR
#define OS08A20_8M_BIT_RATE_WDR    600
#define OS08A20_5M_BIT_RATE_WDR    OS08A20_8M_BIT_RATE_WDR
#define OS08A20_4M_BIT_RATE_WDR    OS08A20_8M_BIT_RATE_WDR
#define OS08A20_3M_BIT_RATE_WDR    OS08A20_8M_BIT_RATE_WDR
#define OS08A20_2M_BIT_RATE_WDR    OS08A20_8M_BIT_RATE_WDR
#else
#define OS08A20_INPUT_CLOCK_LINEAR XMEDIA_SENSOR_CLOCK_FREQ_24MHZ
#define OS08A20_INPUT_CLOCK_WDR    XMEDIA_SENSOR_CLOCK_FREQ_24MHZ
#define OS08A20_8M_MAX_FPS_LINEAR  30
#define OS08A20_5M_MAX_FPS_LINEAR  OS08A20_8M_MAX_FPS_LINEAR
#define OS08A20_4M_MAX_FPS_LINEAR  OS08A20_8M_MAX_FPS_LINEAR
#define OS08A20_3M_MAX_FPS_LINEAR  OS08A20_8M_MAX_FPS_LINEAR
#define OS08A20_2M_MAX_FPS_LINEAR  OS08A20_8M_MAX_FPS_LINEAR
#define OS08A20_8M_MAX_FPS_WDR     30
#define OS08A20_5M_MAX_FPS_WDR     OS08A20_8M_MAX_FPS_WDR
#define OS08A20_4M_MAX_FPS_WDR     OS08A20_8M_MAX_FPS_WDR
#define OS08A20_3M_MAX_FPS_WDR     OS08A20_8M_MAX_FPS_WDR
#define OS08A20_2M_MAX_FPS_WDR     OS08A20_8M_MAX_FPS_WDR
#define OS08A20_8M_BIT_RATE_LINEAR 1440
#define OS08A20_5M_BIT_RATE_LINEAR OS08A20_8M_BIT_RATE_LINEAR
#define OS08A20_4M_BIT_RATE_LINEAR OS08A20_8M_BIT_RATE_LINEAR
#define OS08A20_3M_BIT_RATE_LINEAR OS08A20_8M_BIT_RATE_LINEAR
#define OS08A20_2M_BIT_RATE_LINEAR 800
#define OS08A20_8M_BIT_RATE_WDR    1500
#define OS08A20_5M_BIT_RATE_WDR    OS08A20_8M_BIT_RATE_WDR
#define OS08A20_4M_BIT_RATE_WDR    OS08A20_8M_BIT_RATE_WDR
#define OS08A20_3M_BIT_RATE_WDR    OS08A20_8M_BIT_RATE_WDR
#define OS08A20_2M_BIT_RATE_WDR    OS08A20_8M_BIT_RATE_WDR
#endif

#ifdef OS08A20_BIT_WIDTH_10
#define OS08A20_LINEAR_BIT_WIDTH XMEDIA_VIDEO_DATA_WIDTH_10
#else
#define OS08A20_LINEAR_BIT_WIDTH XMEDIA_VIDEO_DATA_WIDTH_12
#endif

#define OS08A20_NAME          "OS08A20"
#define OS08A20_SPECS_MAX_NUM 12

#ifdef __linux__
#define OS08A20_ISP_DEFAULT_SUPPORT
#endif

#define OS08A20_REG_ADDR_STANDBY 0x0100

#define OS08A20_RES_IS_2160P(w, h) ((w) == 3840 && (h) == 2160)
#define OS08A20_RES_IS_1920P(w, h) ((w) == 2560 && (h) == 1920)
#define OS08A20_RES_IS_1440P(w, h) ((w) == 2560 && (h) == 1440)
#define OS08A20_RES_IS_1520P(w, h) ((w) == 2688 && (h) == 1520)
#define OS08A20_RES_IS_1536P(w, h) ((w) == 2048 && (h) == 1536)
#define OS08A20_RES_IS_1080P(w, h) ((w) == 1920 && (h) == 1080)

#define OS08A20_ERR_MODE_PRINT(sns_attr)                                                                               \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sns_attr->width,                \
                     sns_attr->height, sns_attr->wdr_mode);                                                            \
    } while (0)

SENSOR_PRIORITY_DATA sensor_context *g_os08a20_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define OS08A20_SET_CTX(dev, sns_ctx) g_os08a20_ctx[dev] = sns_ctx
#define OS08A20_GET_CTX(dev, sns_ctx) sns_ctx = g_os08a20_ctx[dev]

SENSOR_PRIORITY_DATA static xmedia_s32 g_os08a20_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

static xmedia_u32 g_os08a20_again[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_os08a20_dgain[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_os08a20_isp_dgain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_os08a20_init_time[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u32 g_os08a20_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_os08a20_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_os08a20_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u16 g_os08a20_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

#define OS08A20_AGAIN_INDEX_MAX 64

// awb static param for Fuji Lens New IR_Cut
#define OS08A20_CALIBRATE_STATIC_TEMP       5000
#define OS08A20_CALIBRATE_STATIC_WB_R_GAIN  492
#define OS08A20_CALIBRATE_STATIC_WB_GR_GAIN 256
#define OS08A20_CALIBRATE_STATIC_WB_GB_GAIN 256
#define OS08A20_CALIBRATE_STATIC_WB_B_GAIN  443

// Calibration results for Auto WB Planck
#define OS08A20_CALIBRATE_AWB_P1 16
#define OS08A20_CALIBRATE_AWB_P2 240
#define OS08A20_CALIBRATE_AWB_Q1 0
#define OS08A20_CALIBRATE_AWB_A1 188580
#define OS08A20_CALIBRATE_AWB_B1 128
#define OS08A20_CALIBRATE_AWB_C1 (-139106)

// Rgain and Bgain of the golden sample
#define OS08A20_GOLDEN_RGAIN 0
#define OS08A20_GOLDEN_BGAIN 0

#define OS08A20_FULL_LINES_MAX 0xFFFF

SENSOR_PRIORITY_RODATA static const xmedia_sensor_property g_os08a20_property[OS08A20_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        3840, 2160, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS08A20_8M_MAX_FPS_LINEAR, OS08A20_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, OS08A20_LINEAR_BIT_WIDTH,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OS08A20_8M_BIT_RATE_LINEAR,
    },
    {
        // width, height, wdr_mode
        2560, 1920, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS08A20_5M_MAX_FPS_LINEAR, OS08A20_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, OS08A20_LINEAR_BIT_WIDTH,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OS08A20_5M_BIT_RATE_LINEAR,
    },

    {
        // width, height, wdr_mode
        2560, 1440, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS08A20_5M_MAX_FPS_LINEAR, OS08A20_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, OS08A20_LINEAR_BIT_WIDTH,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OS08A20_5M_BIT_RATE_LINEAR,
    },

    {
        // width, height, wdr_mode
        2688, 1520, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS08A20_4M_MAX_FPS_LINEAR, OS08A20_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, OS08A20_LINEAR_BIT_WIDTH,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OS08A20_4M_BIT_RATE_LINEAR,
    },

    {
        // width, height, wdr_mode
        2048, 1536, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS08A20_3M_MAX_FPS_LINEAR, OS08A20_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, OS08A20_LINEAR_BIT_WIDTH,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OS08A20_3M_BIT_RATE_LINEAR,
    },

    {
        // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS08A20_2M_MAX_FPS_LINEAR, OS08A20_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, OS08A20_LINEAR_BIT_WIDTH,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OS08A20_2M_BIT_RATE_LINEAR,
    },

    {
        // width, height, wdr_mode
        3840, 2160, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS08A20_8M_MAX_FPS_WDR, OS08A20_INPUT_CLOCK_WDR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, OS08A20_8M_BIT_RATE_WDR,
    },

    {
        // width, height, wdr_mode
        2560, 1920, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS08A20_5M_MAX_FPS_WDR, OS08A20_INPUT_CLOCK_WDR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, OS08A20_5M_BIT_RATE_WDR,
    },

    {
        // width, height, wdr_mode
        2560, 1440, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS08A20_5M_MAX_FPS_WDR, OS08A20_INPUT_CLOCK_WDR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, OS08A20_5M_BIT_RATE_WDR,
    },

    {
        // width, height, wdr_mode
        2688, 1520, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS08A20_4M_MAX_FPS_WDR, OS08A20_INPUT_CLOCK_WDR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, OS08A20_4M_BIT_RATE_WDR,
    },

    {
        // width, height, wdr_mode
        2048, 1536, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS08A20_3M_MAX_FPS_WDR, OS08A20_INPUT_CLOCK_WDR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, OS08A20_3M_BIT_RATE_WDR,
    },

    {
        // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS08A20_2M_MAX_FPS_WDR, OS08A20_INPUT_CLOCK_WDR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, OS08A20_2M_BIT_RATE_WDR,
    },
};

static const xmedia_u32 g_os08a20_again_table[OS08A20_AGAIN_INDEX_MAX] = {
    1024, 1088, 1152, 1216, 1280, 1344, 1408, 1472, 1536, 1600, 1664, 1728, 1792, 1856, 1920, 1984, 2048, 2176,
    2304, 2432, 2560, 2688, 2816, 2944, 3072, 3200, 3328, 3456, 3584, 3712, 3840, 3968, 4096, 4352, 4608, 4864,
    5120, 5376, 5632, 5888, 6144, 6400, 6656, 6912, 7168, 7424, 7680, 7936, 8192, 8704, 9216, 9728, 10240, 10752,
    11264, 11776, 12288, 12800, 13312, 13824, 14336, 14848, 15360, 15872
};

SENSOR_PRIORITY_RODATA static const xmedia_sensor_capability g_os08a20_capability = {
    .max_width  = 3840,
    .max_height = 2160,
    .max_fps    = OS08A20_8M_MAX_FPS_WDR,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_12,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_4L,
    .standby_info = {
        .standby_supported = XMEDIA_TRUE,
        .addr_byte_num     = OS08A20_ADDR_BYTE,
        .data_byte_num     = OS08A20_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { OS08A20_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { OS08A20_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x01 },
    },
};

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_ctx_init(xmedia_u32 dev)
{
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    OS08A20_GET_CTX(dev, os08a20_ctx);

    if (os08a20_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    os08a20_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (os08a20_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(os08a20_ctx, 0, sizeof(sensor_context));
    os08a20_ctx->i2c_addr         = OS08A20_I2C_ADDR;
    os08a20_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_4L;
    os08a20_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    os08a20_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    os08a20_ctx->size.width       = 3840;
    os08a20_ctx->size.height      = 2160;
    os08a20_ctx->fps              = g_os08a20_property[0].max_fps;
    os08a20_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    os08a20_ctx->img_mode             = OS08A20_8M_LINEAR_MODE;
    os08a20_ctx->fl_std               = OS08A20_VMAX_8M_LINEAR;
    os08a20_ctx->fl[SENSOR_CUR_FRAME] = OS08A20_VMAX_8M_LINEAR;
    os08a20_ctx->fl[SENSOR_PRE_FRAME] = OS08A20_VMAX_8M_LINEAR;

    OS08A20_SET_CTX(dev, os08a20_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_void os08a20_ctx_exit(xmedia_u32 dev)
{
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_FREE(os08a20_ctx);
    OS08A20_SET_CTX(dev, XMEDIA_NULL);
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    for (i = 0; i < OS08A20_SPECS_MAX_NUM; i++) {
        if (g_os08a20_property[i].width == os08a20_ctx->size.width &&
            g_os08a20_property[i].height == os08a20_ctx->size.height &&
            g_os08a20_property[i].wdr_mode == os08a20_ctx->wdr_mode) {
            memcpy(property, &g_os08a20_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n",
                    os08a20_ctx->size.width, os08a20_ctx->size.height, os08a20_ctx->wdr_mode);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 os08a20_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os08a20_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "No support mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os08a20_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        os08a20_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        os08a20_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        os08a20_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 os08a20_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    os08a20_ctx->i2c_addr = slave_addr;
    ret = os08a20_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    *slave_addr = os08a20_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}


SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    os08a20_ctx->init_mode = init_mode;

    ret = os08a20_i2c_init(dev, os08a20_ctx->bus_info.i2c_dev, os08a20_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    os08a20_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    ret = os08a20_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    os08a20_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    os08a20_ctx->mirror_en = mirror_en;
    ret = os08a20_set_mirror_flip(dev, mirror_en, os08a20_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    os08a20_ctx->flip_en = flip_en;

    ret = os08a20_set_mirror_flip(dev, os08a20_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_set_image_mode(sensor_context *os08a20_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);
    if (os08a20_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (OS08A20_RES_IS_2160P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OS08A20_8M_LINEAR_MODE;
            os08a20_ctx->fl_std = OS08A20_VMAX_8M_LINEAR;
        } else if (OS08A20_RES_IS_1920P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OS08A20_5M_LINEAR_MODE;
            os08a20_ctx->fl_std = OS08A20_VMAX_5M_LINEAR;
        } else if (OS08A20_RES_IS_1440P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OS08A20_4M_1440P_LINEAR_MODE;
            os08a20_ctx->fl_std = OS08A20_VMAX_4M_1440P_LINEAR;
        } else if (OS08A20_RES_IS_1520P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OS08A20_4M_LINEAR_MODE;
            os08a20_ctx->fl_std = OS08A20_VMAX_4M_LINEAR;
        } else if (OS08A20_RES_IS_1536P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OS08A20_3M_LINEAR_MODE;
            os08a20_ctx->fl_std = OS08A20_VMAX_3M_LINEAR;
        } else if (OS08A20_RES_IS_1080P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OS08A20_2M_LINEAR_MODE;
            os08a20_ctx->fl_std = OS08A20_VMAX_2M_LINEAR;
        } else{
            OS08A20_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else if (os08a20_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (OS08A20_RES_IS_2160P(sns_attr->width, sns_attr->height)) {
            *image_mode                = OS08A20_8M_WDR_MODE;
            os08a20_ctx->fl_std        = OS08A20_VMAX_8M_WDR;
        } else if (OS08A20_RES_IS_1920P(sns_attr->width, sns_attr->height)) {
            *image_mode                = OS08A20_5M_WDR_MODE;
            os08a20_ctx->fl_std        = OS08A20_VMAX_5M_WDR;
        } else if (OS08A20_RES_IS_1440P(sns_attr->width, sns_attr->height)) {
            *image_mode                = OS08A20_4M_1440P_WDR_MODE;
            os08a20_ctx->fl_std        = OS08A20_VMAX_4M_1440P_WDR;
        } else if (OS08A20_RES_IS_1520P(sns_attr->width, sns_attr->height)) {
            *image_mode                = OS08A20_4M_WDR_MODE;
            os08a20_ctx->fl_std        = OS08A20_VMAX_4M_WDR;
        } else if (OS08A20_RES_IS_1536P(sns_attr->width, sns_attr->height)) {
            *image_mode                = OS08A20_3M_WDR_MODE;
            os08a20_ctx->fl_std        = OS08A20_VMAX_3M_WDR;
        } else if (OS08A20_RES_IS_1080P(sns_attr->width, sns_attr->height)) {
            *image_mode                = OS08A20_2M_WDR_MODE;
            os08a20_ctx->fl_std        = OS08A20_VMAX_2M_WDR;
        } else {
            OS08A20_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        OS08A20_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os08a20_ctx->size.width  = sns_attr->width;
    os08a20_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            os08a20_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            os08a20_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
            SENSOR_PRINT("2to1 line WDR\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(os08a20_ctx->wdr_int_time, 0, sizeof(os08a20_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

#define OS08A20_LINEAR_REG_INFO_MAX_NUM   OS08A20_REG_L_MAX_NUM
#define OS08A20_2TO1_WDR_REG_INFO_MAX_NUM OS08A20_REG_MAX_NUM
SENSOR_PRIORITY_FUNC static xmedia_void os08a20_init_common_reg_info(sensor_context *os08a20_ctx)
{
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_EXP_H_IDX].delay_frame_num = 2;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_EXP_H_IDX].reg_addr = OS08A20_REG_ADDR_EXP_H;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_EXP_L_IDX].delay_frame_num = 2;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_EXP_L_IDX].reg_addr = OS08A20_REG_ADDR_EXP_L;

    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_AGAIN_H_IDX].delay_frame_num = 2;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_AGAIN_H_IDX].reg_addr = OS08A20_REG_ADDR_AGAIN_H;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_AGAIN_L_IDX].delay_frame_num = 2;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_AGAIN_L_IDX].reg_addr = OS08A20_REG_ADDR_AGAIN_L;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_DGAIN_H_IDX].delay_frame_num = 2;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_DGAIN_H_IDX].reg_addr = OS08A20_REG_ADDR_DGAIN_H;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_DGAIN_L_IDX].delay_frame_num = 2;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_DGAIN_L_IDX].reg_addr = OS08A20_REG_ADDR_DGAIN_L;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_VMAX_H_IDX].delay_frame_num  = 1;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_VMAX_H_IDX].reg_addr = OS08A20_REG_ADDR_VMAX_H;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_VMAX_L_IDX].delay_frame_num  = 1;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_VMAX_L_IDX].reg_addr = OS08A20_REG_ADDR_VMAX_L;

    return;
}

SENSOR_PRIORITY_FUNC static xmedia_void os08a20_init_2to1_wdr_reg_info(sensor_context *os08a20_ctx)
{
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_S_EXP_H_IDX].delay_frame_num   = 2;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_S_EXP_H_IDX].reg_addr = OS08A20_REG_ADDR_S_EXP_H;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_S_EXP_L_IDX].delay_frame_num   = 2;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_S_EXP_L_IDX].reg_addr = OS08A20_REG_ADDR_S_EXP_L;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_S_AGAIN_H_IDX].delay_frame_num = 2;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_S_AGAIN_H_IDX].reg_addr = OS08A20_REG_ADDR_S_AGAIN_H;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_S_AGAIN_L_IDX].delay_frame_num = 2;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_S_AGAIN_L_IDX].reg_addr = OS08A20_REG_ADDR_S_AGAIN_L;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_S_DGAIN_H_IDX].delay_frame_num = 2;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_S_DGAIN_H_IDX].reg_addr = OS08A20_REG_ADDR_S_DGAIN_H;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_S_DGAIN_L_IDX].delay_frame_num = 2;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_S_DGAIN_L_IDX].reg_addr = OS08A20_REG_ADDR_S_DGAIN_L;
    return;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = os08a20_ctx->bus_info.i2c_dev;
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = OS08A20_LINEAR_REG_INFO_MAX_NUM;

    if (os08a20_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        os08a20_ctx->regs_info[SENSOR_CUR_FRAME].reg_num = OS08A20_2TO1_WDR_REG_INFO_MAX_NUM;
    }

    for (i = 0; i < os08a20_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = os08a20_ctx->i2c_addr;
        os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = OS08A20_ADDR_BYTE;
        os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = OS08A20_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    os08a20_init_common_reg_info(os08a20_ctx);

    // init 2to1 wdr mode Regs
    if (os08a20_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        os08a20_init_2to1_wdr_reg_info(os08a20_ctx);
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    // Set wdr mode
    ret = os08a20_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = os08a20_set_image_mode(os08a20_ctx, &os08a20_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    os08a20_ctx->fl[SENSOR_CUR_FRAME] = os08a20_ctx->fl_std;
    os08a20_ctx->fl[SENSOR_PRE_FRAME] = os08a20_ctx->fl[SENSOR_CUR_FRAME];

    ret = os08a20_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef OS08A20_ISP_DEFAULT_SUPPORT
    isp_default->blc         = &os08a20_blc;
    isp_default->bnr         = &os08a20_bnr;
    isp_default->clut_attr   = XMEDIA_NULL;
    isp_default->crosstalk   = &os08a20_crosstalk;
    isp_default->csc         = &os08a20_csc;
    isp_default->dehaze      = &os08a20_dehaze;
    isp_default->demosaic    = &os08a20_demosaic;
    isp_default->dpc_dynamic = XMEDIA_NULL;
    isp_default->dpc_static  = XMEDIA_NULL;
    isp_default->expander    = XMEDIA_NULL;
    isp_default->fpn         = XMEDIA_NULL;
    isp_default->gamma       = &os08a20_gamma;
    isp_default->gcac        = XMEDIA_NULL;
    isp_default->hlc         = XMEDIA_NULL;
    isp_default->lce         = &os08a20_lce;
    isp_default->mlsc_attr   = XMEDIA_NULL;
    isp_default->mlsc_lut    = XMEDIA_NULL;
    isp_default->radial_crop = XMEDIA_NULL;
    isp_default->rgbir       = XMEDIA_NULL;
    isp_default->rlsc_attr   = XMEDIA_NULL;
    isp_default->rlsc_lut    = XMEDIA_NULL;
    isp_default->sharpen     = XMEDIA_NULL;
    isp_default->stnr        = XMEDIA_NULL;
    isp_default->wdr         = XMEDIA_NULL;

    switch(os08a20_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            isp_default->drc = XMEDIA_NULL;
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            isp_default->drc = &os08a20_drc;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support mode: %d\n", os08a20_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
#else
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from os08a20!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 os08a20_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (os08a20_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x100;
    } else {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x100;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    if (os08a20_ctx->mirror_en && os08a20_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (os08a20_ctx->mirror_en && (!os08a20_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!os08a20_ctx->mirror_en) && os08a20_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < OS08A20_SPECS_MAX_NUM; i++) {
        if (g_os08a20_property[i].width == os08a20_ctx->size.width &&
            g_os08a20_property[i].height == os08a20_ctx->size.height &&
            g_os08a20_property[i].wdr_mode == os08a20_ctx->wdr_mode) {
            *bayer_pattern = g_os08a20_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= OS08A20_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_os08a20_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os08a20_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 os08a20_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_os08a20_again[dev]         = init_param->again;
    g_os08a20_dgain[dev]         = init_param->dgain;
    g_os08a20_isp_dgain[dev]     = init_param->ispdgain;
    g_os08a20_init_time[dev]     = init_param->exp_time;
    g_os08a20_exposure[dev]      = init_param->exposure;
    g_os08a20_sample_r_gain[dev] = init_param->sample_rgain;
    g_os08a20_sample_b_gain[dev] = init_param->sample_bgain;

    g_os08a20_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_os08a20_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_os08a20_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case OS08A20_8M_LINEAR_MODE:
            *max_fps = OS08A20_8M_MAX_FPS_LINEAR;
            break;
        case OS08A20_5M_LINEAR_MODE:
            *max_fps = OS08A20_5M_MAX_FPS_LINEAR;
            break;
        case OS08A20_4M_LINEAR_MODE:
        case OS08A20_4M_1440P_LINEAR_MODE:
            *max_fps = OS08A20_4M_MAX_FPS_LINEAR;
            break;
        case OS08A20_3M_LINEAR_MODE:
            *max_fps = OS08A20_3M_MAX_FPS_LINEAR;
            break;
        case OS08A20_2M_LINEAR_MODE:
            *max_fps = OS08A20_2M_MAX_FPS_LINEAR;
            break;

        case OS08A20_8M_WDR_MODE:
            *max_fps = OS08A20_8M_MAX_FPS_WDR;
            break;
        case OS08A20_5M_WDR_MODE:
            *max_fps = OS08A20_5M_MAX_FPS_WDR;
            break;
        case OS08A20_4M_WDR_MODE:
        case OS08A20_4M_1440P_WDR_MODE:
            *max_fps = OS08A20_4M_MAX_FPS_WDR;
            break;
        case OS08A20_3M_WDR_MODE:
            *max_fps = OS08A20_3M_MAX_FPS_WDR;
            break;
        case OS08A20_2M_WDR_MODE:
            *max_fps = OS08A20_2M_MAX_FPS_WDR;
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = os08a20_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256; // light flicker freq: 50Hz, accuracy: 256
    ae_sns_dft->full_lines_max = OS08A20_FULL_LINES_MAX;
    ae_sns_dft->hmax_times     = (1000000000) / (sns_ctx->fl_std * 30); // 1000000000ns, 30fps

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy = 0.0078125;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 128 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->ispdgain_sep_thd    = 32 << ae_sns_dft->ispdgain_shift;

    memcpy(&ae_sns_dft->piris_attr, &g_os08a20_piris_attr, sizeof(xmedia_isp_piris_attr));
    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_4;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_5_6;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    // lines per second: VTS * max_fps
    ae_sns_dft->lines_per_500ms  = (sns_ctx->fl_std * max_fps) / 2;

    return XMEDIA_SUCCESS;
}

static xmedia_void os08a20_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 15872; // max: 15872     15.5x
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 128;
    ae_sns_dft->min_dgain        = 128;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_os08a20_exposure[dev] ? g_os08a20_exposure[dev] : 76151;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - OS08A20_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 8;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    return;
}

static xmedia_s32 os08a20_get_ae_2to1_wdr_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                 xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = -0.115;

    ae_sns_dft->max_again        = 15872; // max: 15872     15.5x
    ae_sns_dft->min_again        = 1024;

    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 2047;
    ae_sns_dft->min_dgain        = 128;

    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;
    ae_sns_dft->init_exposure    = g_os08a20_exposure[dev] ? g_os08a20_exposure[dev] : 52000;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - OS08A20_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 2;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;
    ae_sns_dft->max_ispdgain_target = 4 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->diff_gain_support   = XMEDIA_TRUE;

    if (os08a20_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
        ae_sns_dft->ae_compensation = 0x38;
        ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    } else {
        ae_sns_dft->ae_compensation  = 0x20;
        ae_sns_dft->ae_exp_mode      = XMEDIA_ISP_AE_STRATEGY_EXP_LOWLIGHT_PRIOR;
        ae_sns_dft->man_ratio_enable = XMEDIA_FALSE;

        ae_sns_dft->ratio[0] = 0x400;
        ae_sns_dft->ratio[1] = 0x40;
        ae_sns_dft->ratio[2] = 0x40;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    memset(ae_sns_dft, 0, sizeof(xmedia_isp_ae_sensor_default));

    ret = os08a20_get_ae_common_default(dev, os08a20_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (os08a20_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            os08a20_get_ae_linear_default(dev, os08a20_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            os08a20_get_ae_2to1_wdr_default(dev, os08a20_ctx, ae_sns_dft);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", os08a20_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context    *os08a20_ctx                               = XMEDIA_NULL;
    static xmedia_bool first[XMEDIA_SENSOR_DEV_MAX_NUM]          = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 1 };
    static xmedia_u32  short_int_time[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
    static xmedia_u32  long_int_time[XMEDIA_SENSOR_DEV_MAX_NUM]  = { 0 };

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    if (os08a20_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (first[dev]) { // 0: short exposure
            os08a20_ctx->wdr_int_time[0] = int_time;
            short_int_time[dev]          = int_time;
            os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_S_EXP_H_IDX].data =
                                                                                SENSOR_HIGH_8BITS(short_int_time[dev]);
            os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_S_EXP_L_IDX].data =
                                                                                SENSOR_LOW_8BITS(short_int_time[dev]);
            first[dev] = XMEDIA_FALSE;
        } else { // 1: long exposure
            os08a20_ctx->wdr_int_time[1] = int_time;
            long_int_time[dev]           = int_time;
            os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_EXP_H_IDX].data   =
                SENSOR_HIGH_8BITS(long_int_time[dev]);
            os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_EXP_L_IDX].data   =
                SENSOR_LOW_8BITS(long_int_time[dev]);
            first[dev] = XMEDIA_TRUE;
        }
    } else if (os08a20_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_EXP_H_IDX].data = SENSOR_HIGH_8BITS(int_time);
        os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_EXP_L_IDX].data = SENSOR_LOW_8BITS(int_time);
        first[dev]                                                                    = XMEDIA_TRUE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_os08a20_again_table[OS08A20_AGAIN_INDEX_MAX - 1]) {
        *again_lin = g_os08a20_again_table[OS08A20_AGAIN_INDEX_MAX - 1];
        *again_db = OS08A20_AGAIN_INDEX_MAX - 1;
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < OS08A20_AGAIN_INDEX_MAX; i++) {
        if (*again_lin < g_os08a20_again_table[i]) {
            *again_lin = g_os08a20_again_table[i - 1];
            *again_db = i - 1;
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context    *os08a20_ctx = XMEDIA_NULL;
    static xmedia_bool first_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 1 };
    xmedia_u32         again_reg, dgain_reg;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    again_reg = g_os08a20_again_table[again];
    again_reg = SENSOR_CLIP3((again_reg >> 3), 0x80, 0x7C0);

    if (dgain < 0x80) {
        dgain_reg = 0x400;
    } else if (dgain < 0x800) {
        dgain_reg = dgain * 0x8;
    } else {
        dgain_reg = 0x3FFF;
    }

    if (os08a20_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (first_gain[dev] == XMEDIA_TRUE) {
            os08a20_ctx->regs_info[0].i2c_data[OS08A20_REG_AGAIN_H_IDX].data = ((again_reg & 0x3f00) >> 8);
            os08a20_ctx->regs_info[0].i2c_data[OS08A20_REG_AGAIN_L_IDX].data = (again_reg & 0xff);
            os08a20_ctx->regs_info[0].i2c_data[OS08A20_REG_DGAIN_H_IDX].data = ((dgain_reg & 0x3f00) >> 8);
            os08a20_ctx->regs_info[0].i2c_data[OS08A20_REG_DGAIN_L_IDX].data = (dgain_reg & 0xff);
            first_gain[dev] = XMEDIA_FALSE;
        } else {
            os08a20_ctx->regs_info[0].i2c_data[OS08A20_REG_S_AGAIN_H_IDX].data = ((again_reg & 0x3f00) >> 8);
            os08a20_ctx->regs_info[0].i2c_data[OS08A20_REG_S_AGAIN_L_IDX].data = (again_reg & 0xff);
            os08a20_ctx->regs_info[0].i2c_data[OS08A20_REG_S_DGAIN_H_IDX].data = ((dgain_reg & 0x3f00) >> 8);
            os08a20_ctx->regs_info[0].i2c_data[OS08A20_REG_S_DGAIN_L_IDX].data = (dgain_reg & 0xff);
            first_gain[dev] = XMEDIA_TRUE;
        }
    } else if (os08a20_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        os08a20_ctx->regs_info[0].i2c_data[OS08A20_REG_AGAIN_H_IDX].data = ((again_reg & 0x3f00) >> 8);
        os08a20_ctx->regs_info[0].i2c_data[OS08A20_REG_AGAIN_L_IDX].data = (again_reg & 0xff);
        os08a20_ctx->regs_info[0].i2c_data[OS08A20_REG_DGAIN_H_IDX].data = ((dgain_reg & 0x3f00) >> 8);
        os08a20_ctx->regs_info[0].i2c_data[OS08A20_REG_DGAIN_L_IDX].data = (dgain_reg & 0xff);
    }



    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = OS08A20_CALIBRATE_STATIC_TEMP;    // wb_ref_temp 5000

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = OS08A20_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = OS08A20_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = OS08A20_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = OS08A20_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 6: point index on the awb curve param
    awb_sns_dft->wb_para[0]   = OS08A20_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1]   = OS08A20_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2]   = OS08A20_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3]   = OS08A20_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4]   = OS08A20_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5]   = OS08A20_CALIBRATE_AWB_C1;
    awb_sns_dft->golden_rgain = OS08A20_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = OS08A20_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (os08a20_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_os08a20_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_os08a20_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_os08a20_awb_wdr_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_os08a20_awb_wdr_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", os08a20_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_os08a20_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_os08a20_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_os08a20_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_os08a20_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_os08a20_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_standby(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os08a20_write_reg(dev, OS08A20_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_resume(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os08a20_write_reg(dev, OS08A20_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_config_init_param(xmedia_u32 dev)
{
    xmedia_u32 i;
    sensor_context *os08a20_ctx  = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < os08a20_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < os08a20_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= os08a20_write_reg(dev, os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    if (os08a20_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = os08a20_init_image(dev, os08a20_ctx->img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = os08a20_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = os08a20_set_mirror_flip(dev, os08a20_ctx->mirror_en, os08a20_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os08a20_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_os08a20_dev_map[index] == SENSOR_DEV_INVALID) {
            g_os08a20_dev_map[index] = pipe;
            *dev                     = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_os08a20_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 os08a20_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    for (i = 0; i < os08a20_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            os08a20_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &os08a20_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&os08a20_ctx->regs_info[SENSOR_PRE_FRAME], &os08a20_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    os08a20_ctx->fl[SENSOR_PRE_FRAME] = os08a20_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_bool os08a20_get_2to1_vmax_limit(xmedia_u32 *vmax, xmedia_u32 full_line,
    xmedia_u32 step, xmedia_bool fps_up)
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

static xmedia_s32 os08a20_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{
    switch (img_mode) {
        case OS08A20_8M_LINEAR_MODE:
        case OS08A20_5M_LINEAR_MODE:
        case OS08A20_4M_LINEAR_MODE:
        case OS08A20_4M_1440P_LINEAR_MODE:
        case OS08A20_3M_LINEAR_MODE:
        case OS08A20_2M_LINEAR_MODE:
            *min_fps = 1.06;
            break;
        case OS08A20_8M_WDR_MODE:
        case OS08A20_5M_WDR_MODE:
        case OS08A20_4M_WDR_MODE:
        case OS08A20_4M_1440P_WDR_MODE:
        case OS08A20_3M_WDR_MODE:
        case OS08A20_2M_WDR_MODE:
            *min_fps = 5.0;
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case OS08A20_8M_LINEAR_MODE:
            *vmax = OS08A20_VMAX_8M_LINEAR;
            break;

        case OS08A20_5M_LINEAR_MODE:
            *vmax = OS08A20_VMAX_5M_LINEAR;
            break;

        case OS08A20_4M_LINEAR_MODE:
            *vmax = OS08A20_VMAX_4M_LINEAR;
            break;

        case OS08A20_4M_1440P_LINEAR_MODE:
            *vmax = OS08A20_VMAX_4M_1440P_LINEAR;
            break;

        case OS08A20_3M_LINEAR_MODE:
            *vmax = OS08A20_VMAX_3M_LINEAR;
            break;

        case OS08A20_2M_LINEAR_MODE:
            *vmax = OS08A20_VMAX_2M_LINEAR;
            break;

        case OS08A20_8M_WDR_MODE:
            *vmax = OS08A20_VMAX_8M_WDR;
            break;

        case OS08A20_5M_WDR_MODE:
            *vmax = OS08A20_VMAX_5M_WDR;
            break;

        case OS08A20_4M_WDR_MODE:
            *vmax = OS08A20_VMAX_4M_WDR;
            break;

        case OS08A20_4M_1440P_WDR_MODE:
            *vmax = OS08A20_VMAX_4M_1440P_WDR;
            break;

        case OS08A20_3M_WDR_MODE:
            *vmax = OS08A20_VMAX_3M_WDR;
            break;

        case OS08A20_2M_WDR_MODE:
            *vmax = OS08A20_VMAX_2M_WDR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = os08a20_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os08a20_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os08a20_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = vmax_max_fps * max_fps / min_fps;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_u32   lines, lines_max;
    xmedia_float max_fps, min_fps;
    xmedia_u32   vmax;
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    ret = os08a20_get_max_fps(os08a20_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os08a20_get_min_fps(os08a20_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os08a20_get_vmax(os08a20_ctx->img_mode, &lines);
    SENSOR_CHECK_RET_RETURN(ret);
    lines_max = lines * max_fps / min_fps;

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "not support fps: %f\n", fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    vmax = lines * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    vmax = (vmax > lines_max) ? lines_max : vmax;

    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_VMAX_L_IDX].data = SENSOR_LOW_8BITS(vmax);
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_VMAX_H_IDX].data = SENSOR_HIGH_8BITS(vmax);

    os08a20_ctx->fl_std               = vmax;
    os08a20_ctx->fl[SENSOR_CUR_FRAME] = os08a20_ctx->fl_std;
    os08a20_ctx->fps                  = fps;
    ae_sns_dft->fps                   = fps;
    ae_sns_dft->full_lines_std        = os08a20_ctx->fl_std;
    ae_sns_dft->max_int_time          = os08a20_ctx->fl_std - 10;
    ae_sns_dft->full_lines            = os08a20_ctx->fl[SENSOR_CUR_FRAME];

    SENSOR_PRINT("dev[%d]- os08a20 set fps: %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

typedef struct {
    xmedia_u32 dec[XMEDIA_ISP_WDR_CHN_MAX_NUM];
    xmedia_u32 inc[XMEDIA_ISP_WDR_CHN_MAX_NUM];
} time_step;

#define OS08A20_MARGIN 4
// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 os08a20_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    xmedia_u32 short_max0, i, short_max, short_time_min_limit;
    xmedia_u32 height;
    time_step step = {{0}};
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);
    memcpy(os08a20_ctx->ratio, inttime_attr->ratio, sizeof(inttime_attr->ratio));

    short_time_min_limit = 2; // short_time_min_limit 2

    if (os08a20_ctx->img_mode == OS08A20_8M_WDR_MODE || os08a20_ctx->img_mode == OS08A20_5M_WDR_MODE ||
        os08a20_ctx->img_mode == OS08A20_4M_WDR_MODE || os08a20_ctx->img_mode == OS08A20_3M_WDR_MODE ||
        os08a20_ctx->img_mode == OS08A20_2M_WDR_MODE || OS08A20_4M_1440P_WDR_MODE) {
         height = 2160;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR, "not supported image mode.\n");
    }

    for (i = 0; i < 2; ++i) { // frame number is 2
        step.inc[i] = (os08a20_ctx->fl[SENSOR_PRE_FRAME] - height) >> 1;
        step.dec[i] = (os08a20_ctx->fl[SENSOR_PRE_FRAME] - height) >> 1;
    }

    if (os08a20_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
        short_max0 = os08a20_ctx->fl[1] - OS08A20_MARGIN - step.dec[0] - os08a20_ctx->wdr_int_time[0];
        short_max = os08a20_ctx->fl[0] - OS08A20_MARGIN - step.dec[0];
        short_max = (short_max0 < short_max) ? short_max0 : short_max;

        inttime_attr->max_inttime[0] = short_time_min_limit;
        inttime_attr->min_inttime[0] = short_time_min_limit;
        inttime_attr->max_inttime[1] = short_max;
        inttime_attr->min_inttime[1] = short_time_min_limit;
    } else {
        short_max0 = ((os08a20_ctx->fl[1] - OS08A20_MARGIN - step.dec[0] - os08a20_ctx->wdr_int_time[0]) * 0x40) /
            SENSOR_DIV_0_TO_1(os08a20_ctx->ratio[0]);
        short_max = ((os08a20_ctx->fl[0] - OS08A20_MARGIN - step.inc[0]) * 0x40) / (os08a20_ctx->ratio[0] + 0x40);
        short_max = (short_max0 < short_max) ? short_max0 : short_max;
        short_max = (short_max == 0) ? 1 : short_max;

        if (short_max >= short_time_min_limit) {
            inttime_attr->max_inttime[0] = short_max;
            inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * os08a20_ctx->ratio[0]) >> 6;
            inttime_attr->min_inttime[0] = short_time_min_limit;
            inttime_attr->min_inttime[1] = (inttime_attr->min_inttime[0] * os08a20_ctx->ratio[0]) >> 6;
        } else {
            short_max = short_time_min_limit;
            inttime_attr->max_inttime[0] = short_max;
            inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * 0xFFF) >> 6; // shift 6
            inttime_attr->min_inttime[0] = inttime_attr->max_inttime[0];
            inttime_attr->min_inttime[1] = inttime_attr->max_inttime[1];
        }
    }

    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 * 注意：2to1 wdr模式下，曝光比和ae wdr mode 由AE传入，调用本接口前，AE需先调用os08a20_get_wdr_max_inttime
 */
static xmedia_s32 os08a20_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *os08a20_ctx = XMEDIA_NULL;
    xmedia_u32      min_fps_vamx;
    xmedia_u32      step;
    xmedia_u32      vmax;
    xmedia_s32      ret;
    xmedia_float    min_fps;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    if (os08a20_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", os08a20_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = os08a20_get_min_fps(os08a20_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os08a20_get_min_fps_vmax(os08a20_ctx->img_mode, &min_fps_vamx);
    SENSOR_CHECK_RET_RETURN(ret);

    step = 154;
    if (os08a20_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        vmax = full_lines;
        os08a20_get_2to1_vmax_limit(&vmax, os08a20_ctx->fl[SENSOR_CUR_FRAME],
            step, full_lines < os08a20_ctx->fl[SENSOR_CUR_FRAME]);
    } else {
        vmax = full_lines;
    }

    vmax = SENSOR_MIN(vmax, min_fps_vamx);
    os08a20_ctx->fl[SENSOR_CUR_FRAME] = vmax;
    os08a20_ctx->fps                  = min_fps * min_fps_vamx / vmax;

    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_VMAX_L_IDX].data = SENSOR_LOW_8BITS(vmax);
    os08a20_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS08A20_REG_VMAX_H_IDX].data = SENSOR_HIGH_8BITS(vmax);

    ae_sns_dft->full_lines   = os08a20_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = os08a20_ctx->fl[SENSOR_CUR_FRAME] - OS08A20_EXP_OFFSET_LINEAR;

    SENSOR_PRINT("dev[%d]- os08a20 set fps: %f\n", dev, os08a20_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os08a20_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    ret = os08a20_get_min_fps(os08a20_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os08a20_get_max_fps(os08a20_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = os08a20_ctx->fps;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, OS08A20_NAME, sizeof(OS08A20_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = os08a20_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = os08a20_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = os08a20_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = os08a20_mirror;
    info->isp_func.pfn_sensor_flip             = os08a20_flip;
    info->isp_func.pfn_sensor_set_bus_info     = os08a20_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = os08a20_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = os08a20_set_init_param;
    info->isp_func.pfn_sensor_start            = os08a20_start;
    info->isp_func.pfn_sensor_stop             = os08a20_stop;
    info->isp_func.pfn_sensor_standby          = os08a20_standby;
    info->isp_func.pfn_sensor_resume           = os08a20_resume;
    info->isp_func.pfn_sensor_write_reg        = os08a20_write_reg;
    info->isp_func.pfn_sensor_read_reg         = os08a20_read_reg;
    info->isp_func.pfn_sensor_init             = os08a20_init;
    info->isp_func.pfn_sensor_exit             = os08a20_exit;
    info->isp_func.pfn_sensor_set_attr         = os08a20_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = os08a20_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = os08a20_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = os08a20_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = os08a20_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = os08a20_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = os08a20_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = os08a20_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = os08a20_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = os08a20_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = os08a20_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = os08a20_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = os08a20_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = os08a20_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = os08a20_calc_dgain;   // not support
    info->ae_func.pfn_sensor_update_gains       = os08a20_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = os08a20_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = os08a20_get_awb_default;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os08a20_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = os08a20_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = os08a20_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = os08a20_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OS08A20 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 os08a20_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *os08a20_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = os08a20_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    OS08A20_GET_CTX(dev, os08a20_ctx);
    SENSOR_CHECK_PTR_RETURN(os08a20_ctx);

    if (os08a20_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OS08A20 unregister function failed!\n");
        return ret;
    }

    os08a20_ctx_exit(dev);
    g_os08a20_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;
}
