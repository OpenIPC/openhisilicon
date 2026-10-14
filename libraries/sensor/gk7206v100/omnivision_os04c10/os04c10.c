#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "os04c10.h"
#include "os04c10_ctrl.h"
#include "os04c10_ex.h"
#include "sns_comm.h"
#include "xmedia_isp.h"

#ifdef __linux__
// #define OS04C10_ISP_DEFAULT_SUPPORT
#endif

#ifdef FPGA
#define OS04C10_INPUT_CLOCK_LINEAR      18000000 // 12M
#define OS04C10_INPUT_CLOCK_WDR         0        // not support
#define OS04C10_4M_MAX_FPS_LINEAR       12.0
#define OS04C10_4M_MAX_FPS_WDR          0.0
#define OS04C10_4M_DATA_WIDTH_LINEAR_4L XMEDIA_VIDEO_DATA_WIDTH_10
#define OS04C10_4M_DATA_WIDTH_LINEAR_2L XMEDIA_VIDEO_DATA_WIDTH_MAX // not support
#define OS04C10_4M_DATA_WIDTH_WDR       XMEDIA_VIDEO_DATA_WIDTH_MAX // not support
#define OS04C10_4M_BIT_RATE_LINEAR_4L   600
#define OS04C10_4M_BIT_RATE_LINEAR_2L   0 // not support
#define OS04C10_4M_BIT_RATE_WDR         0 // not support
#else
#define OS04C10_INPUT_CLOCK_LINEAR      XMEDIA_SENSOR_CLOCK_FREQ_24MHZ
#define OS04C10_INPUT_CLOCK_WDR         XMEDIA_SENSOR_CLOCK_FREQ_24MHZ
#define OS04C10_4M_MAX_FPS_LINEAR       30.0
#define OS04C10_4M_MAX_FPS_WDR          30.0
#define OS04C10_4M_DATA_WIDTH_LINEAR_4L XMEDIA_VIDEO_DATA_WIDTH_12
#define OS04C10_4M_DATA_WIDTH_LINEAR_2L XMEDIA_VIDEO_DATA_WIDTH_10
#define OS04C10_4M_DATA_WIDTH_WDR       XMEDIA_VIDEO_DATA_WIDTH_10
#define OS04C10_4M_BIT_RATE_LINEAR_4L   400
#define OS04C10_4M_BIT_RATE_LINEAR_2L   700
#define OS04C10_4M_BIT_RATE_WDR         1400
#endif

#define OS04C10_NAME          "OS04C10"
#define OS04C10_SPECS_MAX_NUM 2

#define OS04C10_REG_ADDR_STANDBY 0x0100

#define OS04C10_RES_IS_1520P(w, h) ((w) == 2688 && (h) == 1520)

#define OS04C10_ERR_MODE_PRINT(sns_attr, mipi_lanes)                                                                   \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d, mipi_lanes:%d.\n",                 \
                     sns_attr->width, sns_attr->height, sns_attr->wdr_mode, mipi_lanes);                               \
    } while (0)

static sensor_context *g_os04c10_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define OS04C10_GET_CTX(dev, sns_ctx) sns_ctx = g_os04c10_ctx[dev]
#define OS04C10_SET_CTX(dev, sns_ctx) g_os04c10_ctx[dev] = sns_ctx

static xmedia_s32 g_os04c10_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] =
                                                                       SENSOR_DEV_INVALID };

static xmedia_u32 g_os04c10_again[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_os04c10_dgain[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_os04c10_isp_dgain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_os04c10_init_time[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u32 g_os04c10_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_os04c10_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_os04c10_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u16 g_os04c10_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

static xmedia_sensor_property g_os04c10_2lane_property[OS04C10_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        2688, 1520, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_NONE, OS04C10_4M_MAX_FPS_LINEAR, OS04C10_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, OS04C10_4M_DATA_WIDTH_LINEAR_2L,
        // bayer_format - mirror&flip /mirror /flip /normal
        {XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
        XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OS04C10_4M_BIT_RATE_LINEAR_2L,
     },

    {
        // width, height, wdr_mode
        2688, 1520, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC,    OS04C10_4M_MAX_FPS_WDR, OS04C10_INPUT_CLOCK_WDR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, OS04C10_4M_DATA_WIDTH_WDR,
        // bayer_format - mirror&flip /mirror /flip /normal
        {XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
        XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, OS04C10_4M_BIT_RATE_WDR,
     },
};

static xmedia_sensor_property g_os04c10_4lane_property[OS04C10_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        2688, 1520, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_NONE, OS04C10_4M_MAX_FPS_LINEAR, OS04C10_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, OS04C10_4M_DATA_WIDTH_LINEAR_4L,
        // bayer_format - mirror&flip /mirror /flip /normal
        {XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
        XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OS04C10_4M_BIT_RATE_LINEAR_4L,
    },
};

#define OS04C10_AGAIN_MAX_NUM 233
static const xmedia_u32 g_os04c10_again_index[OS04C10_AGAIN_MAX_NUM] = {
    1024,  1088,  1152,  1216,  1280,  1344,  1408,  1472,  1536,  1600,  1664,  1728,  1792,  1856,  1920,  1984,
    2048,  2112,  2176,  2240,  2304,  2368,  2432,  2496,  2560,  2624,  2688,  2752,  2816,  2880,  2944,  3008,
    3072,  3136,  3200,  3264,  3328,  3392,  3456,  3520,  3584,  3648,  3712,  3776,  3840,  3904,  3968,  4032,
    4096,  4160,  4224,  4288,  4352,  4416,  4480,  4544,  4608,  4672,  4736,  4800,  4864,  4928,  4992,  5056,
    5120,  5184,  5248,  5312,  5376,  5440,  5504,  5568,  5632,  5696,  5760,  5824,  5888,  5952,  6016,  6080,
    6144,  6208,  6272,  6336,  6400,  6464,  6528,  6592,  6656,  6720,  6784,  6848,  6912,  6976,  7040,  7104,
    7168,  7232,  7296,  7360,  7424,  7488,  7552,  7616,  7680,  7744,  7808,  7872,  7936,  8000,  8064,  8128,
    8192,  8256,  8320,  8384,  8448,  8512,  8576,  8640,  8704,  8768,  8832,  8896,  8960,  9024,  9088,  9152,
    9216,  9280,  9344,  9408,  9472,  9536,  9600,  9664,  9728,  9792,  9856,  9920,  9984,  10048, 10112, 10176,
    10240, 10304, 10368, 10432, 10496, 10560, 10624, 10688, 10752, 10816, 10880, 10944, 11008, 11072, 11136, 11200,
    11264, 11328, 11392, 11456, 11520, 11584, 11648, 11712, 11776, 11840, 11904, 11968, 12032, 12096, 12160, 12224,
    12288, 12352, 12416, 12480, 12544, 12608, 12672, 12736, 12800, 12864, 12928, 12992, 13056, 13120, 13184, 13248,
    13312, 13376, 13440, 13504, 13568, 13632, 13696, 13760, 13824, 13888, 13952, 14016, 14080, 14144, 14208, 14272,
    14336, 14400, 14464, 14528, 14592, 14656, 14720, 14784, 14848, 14912, 14976, 15040, 15104, 15168, 15232, 15296,
    15360, 15424, 15488, 15552, 15616, 15680, 15744, 15808, 15872,
};

static const xmedia_u32 g_os04c10_again_table[OS04C10_AGAIN_MAX_NUM] = {
    0x0080, 0x0088, 0x0090, 0x0098, 0x00a0, 0x00a8, 0x00b0, 0x00b8, 0x00c0, 0x00c8, 0x00d0, 0x00d8, 0x00e0, 0x00e8,
    0x00f0, 0x00f8, 0x0100, 0x0108, 0x0110, 0x0118, 0x0120, 0x0128, 0x0130, 0x0138, 0x0140, 0x0148, 0x0150, 0x0158,
    0x0160, 0x0168, 0x0170, 0x0178, 0x0180, 0x0188, 0x0190, 0x0198, 0x01a0, 0x01a8, 0x01b0, 0x01b8, 0x01c0, 0x01c8,
    0x01d0, 0x01d8, 0x01e0, 0x01e8, 0x01f0, 0x01f8, 0x0200, 0x0208, 0x0210, 0x0218, 0x0220, 0x0228, 0x0230, 0x0238,
    0x0240, 0x0248, 0x0250, 0x0258, 0x0260, 0x0268, 0x0270, 0x0278, 0x0280, 0x0288, 0x0290, 0x0298, 0x02a0, 0x02a8,
    0x02b0, 0x02b8, 0x02c0, 0x02c8, 0x02d0, 0x02d8, 0x02e0, 0x02e8, 0x02f0, 0x02f8, 0x0300, 0x0308, 0x0310, 0x0318,
    0x0320, 0x0328, 0x0330, 0x0338, 0x0340, 0x0348, 0x0350, 0x0358, 0x0360, 0x0368, 0x0370, 0x0378, 0x0380, 0x0388,
    0x0390, 0x0398, 0x03a0, 0x03a8, 0x03b0, 0x03b8, 0x03c0, 0x03c8, 0x03d0, 0x03d8, 0x03e0, 0x03e8, 0x03f0, 0x03f8,
    0x0400, 0x0408, 0x0410, 0x0418, 0x0420, 0x0428, 0x0430, 0x0438, 0x0440, 0x0448, 0x0450, 0x0458, 0x0460, 0x0468,
    0x0470, 0x0478, 0x0480, 0x0488, 0x0490, 0x0498, 0x04a0, 0x04a8, 0x04b0, 0x04b8, 0x04c0, 0x04c8, 0x04d0, 0x04d8,
    0x04e0, 0x04e8, 0x04f0, 0x04f8, 0x0500, 0x0508, 0x0510, 0x0518, 0x0520, 0x0528, 0x0530, 0x0538, 0x0540, 0x0548,
    0x0550, 0x0558, 0x0560, 0x0568, 0x0570, 0x0578, 0x0580, 0x0588, 0x0590, 0x0598, 0x05a0, 0x05a8, 0x05b0, 0x05b8,
    0x05c0, 0x05c8, 0x05d0, 0x05d8, 0x05e0, 0x05e8, 0x05f0, 0x05f8, 0x0600, 0x0608, 0x0610, 0x0618, 0x0620, 0x0628,
    0x0630, 0x0638, 0x0640, 0x0648, 0x0650, 0x0658, 0x0660, 0x0668, 0x0670, 0x0678, 0x0680, 0x0688, 0x0690, 0x0698,
    0x06a0, 0x06a8, 0x06b0, 0x06b8, 0x06c0, 0x06c8, 0x06d0, 0x06d8, 0x06e0, 0x06e8, 0x06f0, 0x06f8, 0x0700, 0x0708,
    0x0710, 0x0718, 0x0720, 0x0728, 0x0730, 0x0738, 0x0740, 0x0748, 0x0750, 0x0758, 0x0760, 0x0768, 0x0770, 0x0778,
    0x0780, 0x0788, 0x0790, 0x0798, 0x07a0, 0x07a8, 0x07b0, 0x07b8, 0x07c0,
};

// awb static param for Fuji Lens New IR_Cut
#define OS04C10_CALIBRATE_STATIC_TEMP       5000
#define OS04C10_CALIBRATE_STATIC_WB_R_GAIN  366
#define OS04C10_CALIBRATE_STATIC_WB_GR_GAIN 256
#define OS04C10_CALIBRATE_STATIC_WB_GB_GAIN 256
#define OS04C10_CALIBRATE_STATIC_WB_B_GAIN  533

// Calibration results for Auto WB Planck
#define OS04C10_CALIBRATE_AWB_P1 137
#define OS04C10_CALIBRATE_AWB_P2 (-13)
#define OS04C10_CALIBRATE_AWB_Q1 (-132)
#define OS04C10_CALIBRATE_AWB_A1 155552
#define OS04C10_CALIBRATE_AWB_B1 128
#define OS04C10_CALIBRATE_AWB_C1 (-94868)

// Rgain and Bgain of the golden sample
#define OS04C10_GOLDEN_RGAIN 0
#define OS04C10_GOLDEN_BGAIN 0

#define OS04C10_FULL_LINES_MAX 0xFFFF

static const xmedia_sensor_capability g_os04c10_capability = {
    .max_width  = 2688,
    .max_height = 1520,
    .max_fps    = OS04C10_4M_MAX_FPS_WDR,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_TRUE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_2L|XMEDIA_SENSOR_MIPI_LANES_4L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = OS04C10_ADDR_BYTE,
        .data_byte_num     = OS04C10_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { OS04C10_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { OS04C10_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x01 },
    },
};

static xmedia_s32 os04c10_ctx_init(xmedia_u32 dev)
{
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    OS04C10_GET_CTX(dev, os04c10_ctx);

    if (os04c10_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    os04c10_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (os04c10_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(os04c10_ctx, 0, sizeof(sensor_context));
    os04c10_ctx->i2c_addr             = OS04C10_I2C_ADDR;
    os04c10_ctx->lanes                = XMEDIA_SENSOR_MIPI_LANES_2L;
    os04c10_ctx->work_mode            = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    os04c10_ctx->bus_info.i2c_dev     = SENSOR_I2C_INVALID;
    os04c10_ctx->size.width           = g_os04c10_2lane_property[0].width;
    os04c10_ctx->size.height          = g_os04c10_2lane_property[0].height;
    os04c10_ctx->fps                  = g_os04c10_2lane_property[0].max_fps;
    os04c10_ctx->wdr_mode             = g_os04c10_2lane_property[0].wdr_mode;
    os04c10_ctx->img_mode             = OS04C10_4L_4M_12BIT_LINEAR_MODE;
    os04c10_ctx->fl_std               = OS04C10_VMAX_2L_4M_LINEAR;
    os04c10_ctx->fl[SENSOR_CUR_FRAME] = OS04C10_VMAX_2L_4M_LINEAR;
    os04c10_ctx->fl[SENSOR_PRE_FRAME] = OS04C10_VMAX_2L_4M_LINEAR;

    OS04C10_SET_CTX(dev, os04c10_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_void os04c10_ctx_exit(xmedia_u32 dev)
{
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_FREE(os04c10_ctx);
    OS04C10_SET_CTX(dev, XMEDIA_NULL);

    return;
}

static xmedia_s32 os04c10_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8               i;
    sensor_context         *os04c10_ctx      = XMEDIA_NULL;
    xmedia_sensor_property *os04c10_property = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    if (os04c10_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_2L) {
        os04c10_property = g_os04c10_2lane_property;
    } else if (os04c10_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_4L) {
        os04c10_property = g_os04c10_4lane_property;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR, "mipi lanes err.\n");
        goto ERR;
    }

    for (i = 0; i < OS04C10_SPECS_MAX_NUM; i++) {
        if (os04c10_property[i].width == os04c10_ctx->size.width &&
            os04c10_property[i].height == os04c10_ctx->size.height &&
            os04c10_property[i].wdr_mode == os04c10_ctx->wdr_mode) {
            memcpy(property, &os04c10_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

ERR:
    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Please check input param!\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 os04c10_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL && work_mode != XMEDIA_SENSOR_WORK_MODE_MASTER &&
        work_mode != XMEDIA_SENSOR_WORK_MODE_SLAVE) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os04c10_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L && mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Invalid mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os04c10_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        os04c10_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        os04c10_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        os04c10_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 os04c10_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    os04c10_ctx->i2c_addr = slave_addr;
    ret                   = os04c10_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    *slave_addr = os04c10_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

// called by ISP
static xmedia_s32 os04c10_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    os04c10_ctx->init_mode = init_mode;
    ret                    = os04c10_i2c_init(dev, os04c10_ctx->bus_info.i2c_dev, os04c10_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    os04c10_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    ret = os04c10_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    os04c10_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    os04c10_ctx->mirror_en = mirror_en;
    ret                    = os04c10_set_mirror_flip(dev, mirror_en, os04c10_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    os04c10_ctx->flip_en = flip_en;
    ret                  = os04c10_set_mirror_flip(dev, os04c10_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

#define OS04C10_LINEAR_REG_INFO_MAX_NUM   OS04C10_REG_L_MAX_NUM
#define OS04C10_2TO1_WDR_REG_INFO_MAX_NUM OS04C10_REG_MAX_NUM
static xmedia_void os04c10_init_common_reg_info(sensor_context *os04c10_ctx)
{
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_EXP_H].delay_frame_num = 2;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_EXP_H].reg_addr        = OS04C10_REG_ADDR_EXP_H;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_EXP_L].delay_frame_num = 2;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_EXP_L].reg_addr        = OS04C10_REG_ADDR_EXP_L;

    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_AGC_H].delay_frame_num = 2;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_AGC_H].reg_addr        = OS04C10_REG_ADDR_AGC_H;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_AGC_L].delay_frame_num = 2;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_AGC_L].reg_addr        = OS04C10_REG_ADDR_AGC_L;

    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_DGC_H].delay_frame_num = 2;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_DGC_H].reg_addr        = OS04C10_REG_ADDR_DGC_H;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_DGC_L].delay_frame_num = 2;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_DGC_L].reg_addr        = OS04C10_REG_ADDR_DGC_L;

    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_VMAX_H].delay_frame_num = 2;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_VMAX_H].reg_addr        = OS04C10_REG_ADDR_VMAX_H;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_VMAX_L].delay_frame_num = 2;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_VMAX_L].reg_addr        = OS04C10_REG_ADDR_VMAX_L;

    return;
}

static xmedia_void os04c10_init_2to1_wdr_reg_info(sensor_context *os04c10_ctx)
{
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_S_EXP_H].delay_frame_num = 2;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_S_EXP_H].reg_addr        = OS04C10_REG_ADDR_S_EXP_H;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_S_EXP_L].delay_frame_num = 2;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_S_EXP_L].reg_addr        = OS04C10_REG_ADDR_S_EXP_L;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_S_AGC_H].delay_frame_num = 2;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_S_AGC_H].reg_addr        = OS04C10_REG_ADDR_S_AGC_H;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_S_AGC_L].delay_frame_num = 2;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_S_AGC_L].reg_addr        = OS04C10_REG_ADDR_S_AGC_L;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_S_DGC_H].delay_frame_num = 2;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_S_DGC_H].reg_addr        = OS04C10_REG_ADDR_S_DGC_H;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_S_DGC_L].delay_frame_num = 2;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_S_DGC_L].reg_addr        = OS04C10_REG_ADDR_S_DGC_L;
    return;
}

static xmedia_s32 os04c10_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.type           = XMEDIA_SENSOR_BUS_TYPE_I2C;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = os04c10_ctx->bus_info.i2c_dev;
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = OS04C10_LINEAR_REG_INFO_MAX_NUM;

    if (os04c10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        os04c10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num = OS04C10_2TO1_WDR_REG_INFO_MAX_NUM;
    }

    for (i = 0; i < os04c10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = os04c10_ctx->i2c_addr;
        os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = OS04C10_ADDR_BYTE;
        os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = OS04C10_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    os04c10_init_common_reg_info(os04c10_ctx);

    // init 2to1 wdr mode Regs
    if (os04c10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        os04c10_init_2to1_wdr_reg_info(os04c10_ctx);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_set_image_mode(sensor_context *os04c10_ctx, xmedia_u8 *image_mode,
                                         const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (os04c10_ctx->lanes != XMEDIA_SENSOR_MIPI_LANES_4L && os04c10_ctx->lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        goto ERR0;
    }

    if (os04c10_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_4L) {
        if (os04c10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
            if (OS04C10_RES_IS_1520P(sns_attr->width, sns_attr->height)) {
                *image_mode         = OS04C10_4L_4M_12BIT_LINEAR_MODE;
                os04c10_ctx->fl_std = OS04C10_VMAX_4L_4M_LINEAR;
                goto FINISH;
            }
        }
    } else if (os04c10_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_2L) {
        if (os04c10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
            if (OS04C10_RES_IS_1520P(sns_attr->width, sns_attr->height)) {
                *image_mode         = OS04C10_2L_4M_10BIT_LINEAR_MODE;
                os04c10_ctx->fl_std = OS04C10_VMAX_2L_4M_LINEAR;
                goto FINISH;
            }
        } else if (os04c10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
            if (OS04C10_RES_IS_1520P(sns_attr->width, sns_attr->height)) {
                *image_mode         = OS04C10_2L_4M_10BIT_WDR_MODE;
                os04c10_ctx->fl_std = OS04C10_VMAX_2L_4M_WDR;
                goto FINISH;
            }
        }
    }

ERR0:
    OS04C10_ERR_MODE_PRINT(sns_attr, os04c10_ctx->lanes);
    return XMEDIA_ERRCODE_NOT_SUPPORT;

FINISH:
    os04c10_ctx->size.width  = sns_attr->width;
    os04c10_ctx->size.height = sns_attr->height;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            os04c10_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            os04c10_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
            SENSOR_PRINT("2to1 line WDR\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(os04c10_ctx->wdr_int_time, 0, sizeof(os04c10_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    // Set wdr mode
    ret = os04c10_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = os04c10_set_image_mode(os04c10_ctx, &os04c10_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    os04c10_ctx->fl[SENSOR_CUR_FRAME] = os04c10_ctx->fl_std;
    os04c10_ctx->fl[SENSOR_PRE_FRAME] = os04c10_ctx->fl[SENSOR_CUR_FRAME];

    ret = os04c10_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef OS04C10_ISP_DEFAULT_SUPPORT
    // TO-DO: alg add param
    isp_default->blc         = &g_os04c10_blc;
    isp_default->bnr         = &g_os04c10_bnr;
    isp_default->clut_attr   = &g_os04c10_clut_attr;
    isp_default->crosstalk   = &g_os04c10_crosstalk;
    isp_default->csc         = &g_os04c10_csc;
    isp_default->dehaze      = &g_os04c10_dehaze;
    isp_default->demosaic    = &g_os04c10_dms;
    isp_default->dpc_dynamic = &g_os04c10_dpc;
    isp_default->dpc_static  = XMEDIA_NULL;
    isp_default->drc         = &g_os04c10_drc;
    isp_default->expander    = XMEDIA_NULL;
    isp_default->fpn         = XMEDIA_NULL;
    isp_default->gamma       = &g_os04c10_gamma;
    isp_default->gcac        = &g_os04c10_gcac;
    isp_default->hlc         = XMEDIA_NULL;
    isp_default->lce         = &g_os04c10_lce;
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from os04c10!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 os04c10_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (os04c10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x80;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x80;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x80;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x80;
    } else {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x80;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x80;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x80;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x80;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);

    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    for (i = 0; i < os04c10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            os04c10_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &os04c10_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&os04c10_ctx->regs_info[SENSOR_PRE_FRAME], &os04c10_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    os04c10_ctx->fl[SENSOR_PRE_FRAME] = os04c10_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *os04c10_ctx      = XMEDIA_NULL;
    xmedia_sensor_property        *os04c10_property = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    if (os04c10_ctx->mirror_en && os04c10_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (os04c10_ctx->mirror_en && (!os04c10_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!os04c10_ctx->mirror_en) && os04c10_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    if (os04c10_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_2L) {
        os04c10_property = g_os04c10_2lane_property;
    } else if (os04c10_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_4L) {
        os04c10_property = g_os04c10_4lane_property;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR, "mipi lanes err.\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    for (i = 0; i < OS04C10_SPECS_MAX_NUM; i++) {
        if (os04c10_property[i].width == os04c10_ctx->size.width &&
            os04c10_property[i].height == os04c10_ctx->size.height &&
            os04c10_property[i].wdr_mode == os04c10_ctx->wdr_mode) {
            *bayer_pattern = os04c10_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= OS04C10_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);
    memcpy(capability, &g_os04c10_capability, sizeof(xmedia_sensor_capability));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os04c10_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 os04c10_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_os04c10_again[dev]         = init_param->again;
    g_os04c10_dgain[dev]         = init_param->dgain;
    g_os04c10_isp_dgain[dev]     = init_param->ispdgain;
    g_os04c10_init_time[dev]     = init_param->exp_time;
    g_os04c10_exposure[dev]      = init_param->exposure;
    g_os04c10_sample_r_gain[dev] = init_param->sample_rgain;
    g_os04c10_sample_b_gain[dev] = init_param->sample_bgain;

    g_os04c10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_os04c10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_os04c10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case OS04C10_4L_4M_12BIT_LINEAR_MODE:
        case OS04C10_2L_4M_10BIT_LINEAR_MODE:
            *max_fps = OS04C10_4M_MAX_FPS_LINEAR;
            break;

        case OS04C10_2L_4M_10BIT_WDR_MODE:
            *max_fps = OS04C10_4M_MAX_FPS_WDR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = os04c10_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines     = sns_ctx->fl_std;
    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256; // light flicker freq: 50Hz, accuracy: 256
    ae_sns_dft->full_lines_max = OS04C10_FULL_LINES_MAX;

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1.0;
    ae_sns_dft->int_time_accu.offset    = 0.0;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1.0 / 16;
    ae_sns_dft->again_accu.offset    = 0;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy  = 1.0 / 1024;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 8 << ae_sns_dft->ispdgain_shift;

    memcpy(&ae_sns_dft->piris_attr, &g_os04c10_piris_attr, sizeof(xmedia_isp_piris_attr));
    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_4;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_5_6;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    // lines per second: VTS * max_fps
    ae_sns_dft->lines_per_500ms = (sns_ctx->fl_std * max_fps) / 2;

    return XMEDIA_SUCCESS;
}

static xmedia_void os04c10_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                 xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->max_again        = 15872; // 15.5x
    ae_sns_dft->min_again        = 1024;  // 1x
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 1024 * 15; // 15x
    ae_sns_dft->min_dgain        = 1024;      // 1x
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_os04c10_exposure[dev] ? g_os04c10_exposure[dev] : 76151;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - OS04C10_EXP_OFFSET;
    ae_sns_dft->min_int_time        = 2;
    ae_sns_dft->max_int_time_target = 65535; // max 65535
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    ae_sns_dft->ae_route_ex_valid = XMEDIA_FALSE;

    return;
}

static xmedia_s32 os04c10_get_ae_2to1_wdr_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                  xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1.0;
    ae_sns_dft->int_time_accu.offset    = 0;
    ae_sns_dft->init_exposure           = g_os04c10_exposure[dev] ? g_os04c10_exposure[dev] : 52000;

    ae_sns_dft->max_again        = 15872; // 15.5x
    ae_sns_dft->min_again        = 1024;  // 1x
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 1024; // 1024 * 15; // max: 2047
    ae_sns_dft->min_dgain        = 1024; // min: 1 * 1024
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - OS04C10_EXP_OFFSET;
    ae_sns_dft->min_int_time        = 2;     // min 2
    ae_sns_dft->max_int_time_target = 65535; // max 65535
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;
    ae_sns_dft->max_ispdgain_target = 4 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->diff_gain_support   = XMEDIA_TRUE;

    if (os04c10_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
        ae_sns_dft->ae_compensation = 0x38;
        ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    } else {
        ae_sns_dft->ae_compensation  = 0x20;
        ae_sns_dft->ae_exp_mode      = XMEDIA_ISP_AE_STRATEGY_EXP_LOWLIGHT_PRIOR;
        ae_sns_dft->man_ratio_enable = XMEDIA_FALSE;

        /*
         * exp ratio array index( 4to1 wdr):
         * 0: short / very shourt;
         * 1: middle / short;
         * 2: long / middle
         * 2to1 line wdr：ratio[0] valid
         */
        ae_sns_dft->ratio[0] = 0x100;
        ae_sns_dft->ratio[1] = 0x40;
        ae_sns_dft->ratio[2] = 0x40;
    }

    ae_sns_dft->ae_route_ex_valid = XMEDIA_FALSE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = os04c10_get_ae_common_default(dev, os04c10_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (os04c10_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            os04c10_get_ae_linear_default(dev, os04c10_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            os04c10_get_ae_2to1_wdr_default(dev, os04c10_ctx, ae_sns_dft);
            break;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{
    switch (img_mode) {
        case OS04C10_4L_4M_12BIT_LINEAR_MODE:
        case OS04C10_2L_4M_10BIT_LINEAR_MODE:
            *min_fps = 1.06;
            break;

        case OS04C10_2L_4M_10BIT_WDR_MODE:
            *min_fps = 12.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case OS04C10_4L_4M_12BIT_LINEAR_MODE:
            *vmax = OS04C10_VMAX_4L_4M_LINEAR;
            break;

        case OS04C10_2L_4M_10BIT_LINEAR_MODE:
            *vmax = OS04C10_VMAX_2L_4M_LINEAR;
            break;

        case OS04C10_2L_4M_10BIT_WDR_MODE:
            *vmax = OS04C10_VMAX_2L_4M_WDR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines,
                                   xmedia_u32 *lines_per500ms)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_u32   vmax_min_fps;

    ret = os04c10_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04c10_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04c10_get_vmax(sns_ctx->img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    vmax_min_fps = vmax_max_fps * max_fps / min_fps;

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %.2f, range of fps is [%.2f, %.2f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax_max_fps * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, vmax_min_fps);
    *lines_per500ms = vmax_max_fps * max_fps / 2;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);

    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    ret = os04c10_calc_fps(fps, os04c10_ctx, &full_lines, &ae_sns_dft->lines_per_500ms);
    SENSOR_CHECK_RET_RETURN(ret);

    os04c10_ctx->fps = fps;

    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    os04c10_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    os04c10_ctx->fl_std               = os04c10_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->fps                   = fps;
    ae_sns_dft->full_lines_std        = os04c10_ctx->fl_std;
    ae_sns_dft->full_lines            = os04c10_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time          = os04c10_ctx->fl[SENSOR_CUR_FRAME] - OS04C10_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]-os04c10 set fps = %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 os04c10_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    xmedia_u32      short_max_pre_frame;
    xmedia_u32      short_max;
    xmedia_u32      short_time_min_limit;
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    memcpy(os04c10_ctx->ratio, inttime_attr->ratio, sizeof(inttime_attr->ratio));

    /*
     * 0x40 is EXP-1X
     * Ratio = L / S * 0x40;
     * VTS   = L + S + EXP_OFFSET;
     * --> S = (L * 0x40) / Ratio
     * --> S = [(VTS - EXP_OFFSET) * 0x40] / (Ratio + 0x40);
     */
    short_time_min_limit = 2;
    short_max            = 2;
    if (os04c10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (os04c10_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
            short_max_pre_frame = os04c10_ctx->fl[SENSOR_PRE_FRAME] - os04c10_ctx->wdr_int_time[0] - OS04C10_EXP_OFFSET;
            short_max           = os04c10_ctx->fl[SENSOR_CUR_FRAME] - OS04C10_EXP_OFFSET;
            short_max           = (short_max_pre_frame < short_max) ? short_max_pre_frame : short_max;
            inttime_attr->max_inttime[0] = short_time_min_limit; // 0：short frame
            inttime_attr->min_inttime[0] = short_time_min_limit;
            inttime_attr->max_inttime[1] = short_max; // 1：long frame
            inttime_attr->min_inttime[1] = short_time_min_limit;
            return XMEDIA_SUCCESS;
        } else {
            short_max_pre_frame =
                ((os04c10_ctx->fl[SENSOR_PRE_FRAME] - OS04C10_EXP_OFFSET - os04c10_ctx->wdr_int_time[0]) * 0x40) /
                SENSOR_DIV_0_TO_1(os04c10_ctx->ratio[0]);
            short_max =
                ((os04c10_ctx->fl[SENSOR_CUR_FRAME] - OS04C10_EXP_OFFSET) * 0x40) / (os04c10_ctx->ratio[0] + 0x40);
            short_max = (short_max_pre_frame < short_max) ? short_max_pre_frame : short_max;
            short_max = (short_max == 0) ? 1 : short_max;
        }
    }

    if (short_max >= short_time_min_limit) { // 0：short frame; 1：long frame
        inttime_attr->max_inttime[0] = short_max;
        inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * os04c10_ctx->ratio[0]) >> 6;
        inttime_attr->min_inttime[0] = short_time_min_limit;
        inttime_attr->min_inttime[1] = (inttime_attr->min_inttime[0] * os04c10_ctx->ratio[0]) >> 6;
    } else {
        short_max                    = short_time_min_limit;
        inttime_attr->max_inttime[0] = short_max;
        inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * 0xFFF) >> 6; // shift 6
        inttime_attr->min_inttime[0] = inttime_attr->max_inttime[0];
        inttime_attr->min_inttime[1] = inttime_attr->max_inttime[1];
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_float *fps, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = os04c10_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04c10_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04c10_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *fps  = min_fps;
    *vmax = (vmax_max_fps)*max_fps / min_fps;
    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines: AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines: AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 */
static xmedia_s32 os04c10_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                             xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *os04c10_ctx = XMEDIA_NULL;
    xmedia_u32      vmax_min_fps;
    xmedia_s32      ret;
    xmedia_float    fps, min_fps;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);

    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    if (os04c10_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    full_lines = full_lines + OS04C10_EXP_OFFSET;
    ret        = os04c10_get_min_fps_vmax(os04c10_ctx->img_mode, &min_fps, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                        = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    os04c10_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    fps                               = min_fps * vmax_min_fps / full_lines;
    os04c10_ctx->fps                  = fps;

    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    ae_sns_dft->full_lines   = os04c10_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = os04c10_ctx->fl[SENSOR_CUR_FRAME] - OS04C10_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]-os04c10 set fps = %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context    *os04c10_ctx                               = XMEDIA_NULL;
    static xmedia_bool first[XMEDIA_SENSOR_DEV_MAX_NUM]          = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 1 };
    static xmedia_u32  short_int_time[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
    static xmedia_u32  long_int_time[XMEDIA_SENSOR_DEV_MAX_NUM]  = { 0 };

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    if (os04c10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (first[dev]) { // 0: short exposure
            os04c10_ctx->wdr_int_time[0] = int_time;
            short_int_time[dev]          = int_time;
            os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_S_EXP_H].data =
                SENSOR_HIGH_8BITS(short_int_time[dev]);
            os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_S_EXP_L].data =
                SENSOR_LOW_8BITS(short_int_time[dev]);
            first[dev] = XMEDIA_FALSE;
        } else { // 1: long exposure
            os04c10_ctx->wdr_int_time[1] = int_time;
            long_int_time[dev]           = int_time;
            os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_EXP_H].data =
                SENSOR_HIGH_8BITS(long_int_time[dev]);
            os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_EXP_L].data =
                SENSOR_LOW_8BITS(long_int_time[dev]);
            first[dev] = XMEDIA_TRUE;
        }
    } else if (os04c10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_EXP_H].data = SENSOR_HIGH_8BITS(int_time);
        os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_EXP_L].data = SENSOR_LOW_8BITS(int_time);
        first[dev]                                                                = XMEDIA_TRUE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_os04c10_again_index[OS04C10_AGAIN_MAX_NUM - 1]) {
        *again_lin = g_os04c10_again_index[OS04C10_AGAIN_MAX_NUM - 1];
        *again_db  = g_os04c10_again_table[OS04C10_AGAIN_MAX_NUM - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < OS04C10_AGAIN_MAX_NUM; i++) {
        if (*again_lin < g_os04c10_again_index[i]) {
            *again_lin = g_os04c10_again_index[i - 1];
            *again_db  = g_os04c10_again_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_AGC_L].data = SENSOR_LOW_8BITS(again);
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_AGC_H].data = ((again & 0x3F00) >> 8);
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_DGC_L].data = SENSOR_LOW_8BITS(dgain);
    os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_DGC_H].data = ((dgain & 0x3F00) >> 8);

    if (os04c10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_S_AGC_L].data = SENSOR_LOW_8BITS(again);
        os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_S_AGC_H].data = ((again & 0x3F00) >> 8);
        os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_S_DGC_L].data = SENSOR_LOW_8BITS(dgain);
        os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04C10_REG_S_DGC_H].data = ((dgain & 0x3F00) >> 8);
    }

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 os04c10_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = OS04C10_CALIBRATE_STATIC_TEMP; // wb_ref_temp 5000

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = OS04C10_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = OS04C10_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = OS04C10_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = OS04C10_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 6: point index on the awb curve param
    awb_sns_dft->wb_para[0]      = OS04C10_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1]      = OS04C10_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2]      = OS04C10_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3]      = OS04C10_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4]      = OS04C10_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5]      = OS04C10_CALIBRATE_AWB_C1;
    awb_sns_dft->golden_rgain    = OS04C10_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain    = OS04C10_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (os04c10_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_os04c10_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_os04c10_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_os04c10_awb_wdr_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_os04c10_awb_wdr_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
    }

    awb_sns_dft->init_rgain   = g_os04c10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_os04c10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_os04c10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_os04c10_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_os04c10_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_standby(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os04c10_write_reg(dev, OS04C10_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_resume(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os04c10_write_reg(dev, OS04C10_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *os04c10_ctx  = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < os04c10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < os04c10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= os04c10_write_reg(dev, os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                 os04c10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_start(xmedia_u32 dev)
{
    xmedia_s32              ret;
    xmedia_u8               img_mode;
    xmedia_sensor_work_mode work_mode;
    sensor_context         *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    work_mode = os04c10_ctx->work_mode;
    img_mode  = os04c10_ctx->img_mode;

    if (os04c10_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = os04c10_init_image(dev, img_mode, work_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = os04c10_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = os04c10_set_mirror_flip(dev, os04c10_ctx->mirror_en, os04c10_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os04c10_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_os04c10_dev_map[index] == SENSOR_DEV_INVALID) {
            g_os04c10_dev_map[index] = pipe;
            *dev                     = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 os04c10_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_os04c10_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 os04c10_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32      ret;
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);

    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    *fps = os04c10_ctx->fps;

    ret = os04c10_get_min_fps(os04c10_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04c10_get_max_fps(os04c10_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, OS04C10_NAME, sizeof(OS04C10_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property   = os04c10_get_property;
    info->isp_func.pfn_sensor_set_work_mode  = os04c10_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes = os04c10_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror         = os04c10_mirror;
    info->isp_func.pfn_sensor_flip           = os04c10_flip;
    info->isp_func.pfn_sensor_set_bus_info   = os04c10_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr   = os04c10_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param = os04c10_set_init_param;
    info->isp_func.pfn_sensor_start          = os04c10_start;
    info->isp_func.pfn_sensor_stop           = os04c10_stop;
    info->isp_func.pfn_sensor_standby        = os04c10_standby;
    info->isp_func.pfn_sensor_resume         = os04c10_resume;
    info->isp_func.pfn_sensor_write_reg      = os04c10_write_reg;
    info->isp_func.pfn_sensor_read_reg       = os04c10_read_reg;
    info->isp_func.pfn_sensor_init           = os04c10_init;
    info->isp_func.pfn_sensor_exit           = os04c10_exit;

    // called by ISP
    info->isp_func.pfn_sensor_set_attr            = os04c10_set_attr;
    info->isp_func.pfn_sensor_get_isp_default     = os04c10_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = os04c10_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = os04c10_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = os04c10_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = os04c10_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = os04c10_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = os04c10_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = os04c10_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = os04c10_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = os04c10_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = os04c10_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = os04c10_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = os04c10_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = os04c10_calc_dgain; // not support
    info->ae_func.pfn_sensor_update_gains       = os04c10_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = os04c10_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = os04c10_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 os04c10_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = os04c10_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = os04c10_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = os04c10_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OS04C10 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 os04c10_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *os04c10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = os04c10_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    OS04C10_GET_CTX(dev, os04c10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04c10_ctx);

    if (os04c10_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OS04C10 unregister function failed!\n");
        return ret;
    }

    os04c10_ctx_exit(dev);
    g_os04c10_dev_map[dev] = SENSOR_DEV_INVALID; // reset dev

    return XMEDIA_SUCCESS;
}
