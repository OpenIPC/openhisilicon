#include <stdlib.h>
#include <string.h>
#include "sc8238.h"
#include "sc8238_ctrl.h"
#include "sc8238_ex.h"

#ifdef FPGA
#define SC8238_8M_MAX_FPS_LINEAR        3.386
#define SC8238_4M_MAX_FPS_LINEAR        3.386
#define SC8238_1M_MAX_FPS_LINEAR        20.013
#define SC8238_5M_1728P_MAX_FPS_LINEAR  0.0 // not support
#define SC8238_5M_MAX_FPS_LINEAR        SC8238_8M_MAX_FPS_LINEAR    // crop from 8M
#define SC8238_3M_MAX_FPS_LINEAR        SC8238_8M_MAX_FPS_LINEAR
#define SC8238_2M_MAX_FPS_LINEAR        SC8238_8M_MAX_FPS_LINEAR
#define SC8238_8M_MAX_FPS_WDR           1.693
#define SC8238_1M_MAX_FPS_WDR           10.006
#define SC8238_5M_MAX_FPS_WDR           SC8238_8M_MAX_FPS_WDR       // crop from 8M
#define SC8238_4M_MAX_FPS_WDR           SC8238_8M_MAX_FPS_WDR
#define SC8238_3M_MAX_FPS_WDR           SC8238_8M_MAX_FPS_WDR
#define SC8238_2M_MAX_FPS_WDR           SC8238_8M_MAX_FPS_WDR
#define SC8238_8M_BIT_RATE_LINEAR       600
#define SC8238_5M_BIT_RATE_LINEAR       600
#define SC8238_4M_BIT_RATE_LINEAR       600
#define SC8238_3M_BIT_RATE_LINEAR       600
#define SC8238_2M_BIT_RATE_LINEAR       600
#define SC8238_1M_BIT_RATE_LINEAR       600
#define SC8238_5M_1728P_BIT_RATE_LINEAR 600
#define SC8238_8M_BIT_RATE_WDR          600
#define SC8238_5M_BIT_RATE_WDR          600
#define SC8238_4M_BIT_RATE_WDR          600
#define SC8238_3M_BIT_RATE_WDR          600
#define SC8238_2M_BIT_RATE_WDR          600
#define SC8238_1M_BIT_RATE_WDR          600
#else
#define SC8238_8M_MAX_FPS_LINEAR        30.0
#define SC8238_5M_1728P_MAX_FPS_LINEAR  30.0
#define SC8238_5M_MAX_FPS_LINEAR        SC8238_8M_MAX_FPS_LINEAR    // crop from 8M
#define SC8238_4M_MAX_FPS_LINEAR        SC8238_8M_MAX_FPS_LINEAR
#define SC8238_3M_MAX_FPS_LINEAR        SC8238_8M_MAX_FPS_LINEAR
#define SC8238_2M_MAX_FPS_LINEAR        SC8238_8M_MAX_FPS_LINEAR
#define SC8238_1M_MAX_FPS_LINEAR        SC8238_8M_MAX_FPS_LINEAR
#define SC8238_8M_MAX_FPS_WDR           15.0
#define SC8238_5M_MAX_FPS_WDR           SC8238_8M_MAX_FPS_WDR       // crop from 8M
#define SC8238_4M_MAX_FPS_WDR           SC8238_8M_MAX_FPS_WDR
#define SC8238_3M_MAX_FPS_WDR           SC8238_8M_MAX_FPS_WDR
#define SC8238_2M_MAX_FPS_WDR           SC8238_8M_MAX_FPS_WDR
#define SC8238_1M_MAX_FPS_WDR           SC8238_8M_MAX_FPS_WDR
#define SC8238_8M_BIT_RATE_LINEAR       700
#define SC8238_5M_BIT_RATE_LINEAR       700
#define SC8238_4M_BIT_RATE_LINEAR       700
#define SC8238_3M_BIT_RATE_LINEAR       700
#define SC8238_2M_BIT_RATE_LINEAR       700
#define SC8238_1M_BIT_RATE_LINEAR       700
#define SC8238_5M_1728P_BIT_RATE_LINEAR 700
#define SC8238_8M_BIT_RATE_WDR          700
#define SC8238_5M_BIT_RATE_WDR          700
#define SC8238_4M_BIT_RATE_WDR          700
#define SC8238_3M_BIT_RATE_WDR          700
#define SC8238_2M_BIT_RATE_WDR          700
#define SC8238_1M_BIT_RATE_WDR          700
#endif

#define SC8238_NAME          "SC8238"
#define SC8238_SPECS_MAX_NUM 13

#ifdef __linux__
//#define SC8238_ISP_DEFAULT_SUPPORT
#endif

#define SC8238_REG_ADDR_STANDBY 0x0100

#define SC8238_RES_IS_720P(w, h)  ((w) == 1280 && (h) == 720)
#define SC8238_RES_IS_1080P(w, h) ((w) == 1920 && (h) == 1080)
#define SC8238_RES_IS_1536P(w, h) ((w) == 2048 && (h) == 1536)
#define SC8238_RES_IS_1520P(w, h) ((w) == 2688 && (h) == 1520)
#define SC8238_RES_IS_1920P(w, h) ((w) == 2560 && (h) == 1920)
#define SC8238_RES_IS_1728P(w, h) ((w) == 3072 && (h) == 1728)
#define SC8238_RES_IS_2160P(w, h) ((w) == 3840 && (h) == 2160)

#define SC8238_ERR_MODE_PRINT(sensor_image_mode)                                                                       \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sensor_image_mode->width,       \
                     sensor_image_mode->height, sensor_image_mode->wdr_mode);                                          \
    } while (0)

#define SC8238_AGAIN_INDEX_MAX 256
#define SC8238_DGAIN_INDEX_MAX 160

sensor_context *g_sc8238_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define SC8238_GET_CTX(dev, ctx) ctx = g_sc8238_ctx[dev]
#define SC8238_SET_CTX(dev, ctx) g_sc8238_ctx[dev] = ctx

static xmedia_s32 g_sc8238_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

static xmedia_u32 g_sc8238_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_sc8238_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_sc8238_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u16 g_sc8238_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

static const xmedia_sensor_property g_sc8238_property[SC8238_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        3840, 2160, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, SC8238_8M_MAX_FPS_LINEAR, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC8238_8M_BIT_RATE_LINEAR,
     },

    {
        // width, height, wdr_mode
        2560, 1920, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, SC8238_5M_MAX_FPS_LINEAR, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC8238_5M_BIT_RATE_LINEAR,
     },

    {
        // width, height, wdr_mode
        2688, 1520, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, SC8238_4M_MAX_FPS_LINEAR, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format,  bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC8238_4M_BIT_RATE_LINEAR,
     },

    {
        // width, height, wdr_mode
        2048, 1536, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, SC8238_3M_MAX_FPS_LINEAR, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format,  bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC8238_3M_BIT_RATE_LINEAR,
     },

    {
        // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, SC8238_2M_MAX_FPS_LINEAR, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC8238_2M_BIT_RATE_LINEAR,
     },

    {
        // width, height, wdr_mode
        1280,  720, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, SC8238_1M_MAX_FPS_LINEAR, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC8238_1M_BIT_RATE_LINEAR,
     },

    {
        // width, height, wdr_mode
        3072, 1728, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, SC8238_5M_1728P_MAX_FPS_LINEAR, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC8238_5M_1728P_BIT_RATE_LINEAR,
     },

    {
        // width, height, wdr_mode
        1280,  720, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, SC8238_1M_MAX_FPS_WDR, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, SC8238_1M_BIT_RATE_WDR,
     },

    {
        // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC,SC8238_2M_MAX_FPS_WDR, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, SC8238_2M_BIT_RATE_WDR,
     },

    {
        // width, height, wdr_mode
        2048, 1536, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, SC8238_3M_MAX_FPS_WDR, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, SC8238_3M_BIT_RATE_WDR,
     },

    {
        // width, height, wdr_mode
        2688, 1520, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, SC8238_4M_MAX_FPS_WDR, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, SC8238_4M_BIT_RATE_WDR,
     },

    {
        // width, height, wdr_mode
        2560, 1920, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, SC8238_5M_MAX_FPS_WDR, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, SC8238_5M_BIT_RATE_WDR,
     },

    {
        // width, height, wdr_mode
        3840, 2160, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, SC8238_8M_MAX_FPS_WDR, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, SC8238_8M_BIT_RATE_WDR,
     },
};

static const xmedia_u32 g_sc8238_again_index[SC8238_AGAIN_INDEX_MAX] = {
    1024,  1040,  1056,  1072,  1088,  1104,  1120,  1136,  1152,  1168,  1184,  1200,  1216,  1232,  1248,  1264,
    1280,  1296,  1312,  1328,  1344,  1360,  1376,  1392,  1408,  1424,  1440,  1456,  1472,  1488,  1504,  1520,
    1536,  1552,  1568,  1584,  1600,  1616,  1632,  1648,  1664,  1680,  1696,  1712,  1728,  1744,  1760,  1776,
    1792,  1808,  1824,  1840,  1856,  1872,  1888,  1904,  1920,  1936,  1952,  1968,  1984,  2000,  2016,  2032,
    2048,  2080,  2112,  2144,  2176,  2208,  2240,  2272,  2304,  2336,  2368,  2400,  2432,  2464,  2496,  2528,
    2560,  2592,  2624,  2656,  2688,  2720,  2752,  2784,  2816,  2848,  2880,  2912,  2944,  2976,  3008,  3040,
    3072,  3104,  3136,  3168,  3200,  3232,  3264,  3296,  3328,  3360,  3392,  3424,  3456,  3488,  3520,  3552,
    3584,  3616,  3648,  3680,  3712,  3744,  3776,  3808,  3840,  3872,  3904,  3936,  3968,  4000,  4032,  4064,
    4096,  4160,  4224,  4288,  4352,  4416,  4480,  4544,  4608,  4672,  4736,  4800,  4864,  4928,  4992,  5056,
    5120,  5184,  5248,  5312,  5376,  5440,  5504,  5568,  5632,  5696,  5760,  5824,  5888,  5952,  6016,  6080,
    6144,  6208,  6272,  6336,  6400,  6464,  6528,  6592,  6656,  6720,  6784,  6848,  6912,  6976,  7040,  7104,
    7168,  7232,  7296,  7360,  7424,  7488,  7552,  7616,  7680,  7744,  7808,  7872,  7936,  8000,  8064,  8128,
    8192,  8320,  8448,  8576,  8704,  8832,  8960,  9088,  9216,  9344,  9472,  9600,  9728,  9856,  9984,  10112,
    10240, 10368, 10496, 10624, 10752, 10880, 11008, 11136, 11264, 11392, 11520, 11648, 11776, 11904, 12032, 12160,
    12288, 12416, 12544, 12672, 12800, 12928, 13056, 13184, 13312, 13440, 13568, 13696, 13824, 13952, 14080, 14208,
    14336, 14464, 14592, 14720, 14848, 14976, 15104, 15232, 15360, 15488, 15616, 15744, 15872, 16000, 16128, 16256
};

static const xmedia_u32 g_sc8238_again_table[SC8238_AGAIN_INDEX_MAX] = {
    0x0340, 0x0341, 0x0342, 0x0343, 0x0344, 0x0345, 0x0346, 0x0347, 0x0348, 0x0349, 0x034A, 0x034B, 0x034C,
    0x034D, 0x034E, 0x034F, 0x0350, 0x0351, 0x0352, 0x0353, 0x0354, 0x0355, 0x0356, 0x0357, 0x0358, 0x0359,
    0x035A, 0x035B, 0x035C, 0x035D, 0x035E, 0x035F, 0x0360, 0x0361, 0x0362, 0x0363, 0x0364, 0x0365, 0x0366,
    0x0367, 0x0368, 0x0369, 0x036A, 0x036B, 0x036C, 0x036D, 0x036E, 0x036F, 0x0370, 0x0371, 0x0372, 0x0373,
    0x0374, 0x0375, 0x0376, 0x0377, 0x0378, 0x0379, 0x037A, 0x037B, 0x037C, 0x037D, 0x037E, 0x037F,

    0x0740, 0x0741, 0x0742, 0x0743, 0x0744, 0x0745, 0x0746, 0x0747, 0x0748, 0x0749, 0x074A, 0x074B, 0x074C,
    0x074D, 0x074E, 0x074F, 0x0750, 0x0751, 0x0752, 0x0753, 0x0754, 0x0755, 0x0756, 0x0757, 0x0758, 0x0759,
    0x075A, 0x075B, 0x075C, 0x075D, 0x075E, 0x075F, 0x0760, 0x0761, 0x0762, 0x0763, 0x0764, 0x0765, 0x0766,
    0x0767, 0x0768, 0x0769, 0x076A, 0x076B, 0x076C, 0x076D, 0x076E, 0x076F, 0x0770, 0x0771, 0x0772, 0x0773,
    0x0774, 0x0775, 0x0776, 0x0777, 0x0778, 0x0779, 0x077A, 0x077B, 0x077C, 0x077D, 0x077E, 0x077F,

    0x0F40, 0x0F41, 0x0F42, 0x0F43, 0x0F44, 0x0F45, 0x0F46, 0x0F47, 0x0F48, 0x0F49, 0x0F4A, 0x0F4B, 0x0F4C,
    0x0F4D, 0x0F4E, 0x0F4F, 0x0F50, 0x0F51, 0x0F52, 0x0F53, 0x0F54, 0x0F55, 0x0F56, 0x0F57, 0x0F58, 0x0F59,
    0x0F5A, 0x0F5B, 0x0F5C, 0x0F5D, 0x0F5E, 0x0F5F, 0x0F60, 0x0F61, 0x0F62, 0x0F63, 0x0F64, 0x0F65, 0x0F66,
    0x0F67, 0x0F68, 0x0F69, 0x0F6A, 0x0F6B, 0x0F6C, 0x0F6D, 0x0F6E, 0x0F6F, 0x0F70, 0x0F71, 0x0F72, 0x0F73,
    0x0F74, 0x0F75, 0x0F76, 0x0F77, 0x0F78, 0x0F79, 0x0F7A, 0x0F7B, 0x0F7C, 0x0F7D, 0x0F7E, 0x0F7F,

    0x1F40, 0x1F41, 0x1F42, 0x1F43, 0x1F44, 0x1F45, 0x1F46, 0x1F47, 0x1F48, 0x1F49, 0x1F4A, 0x1F4B, 0x1F4C,
    0x1F4D, 0x1F4E, 0x1F4F, 0x1F50, 0x1F51, 0x1F52, 0x1F53, 0x1F54, 0x1F55, 0x1F56, 0x1F57, 0x1F58, 0x1F59,
    0x1F5A, 0x1F5B, 0x1F5C, 0x1F5D, 0x1F5E, 0x1F5F, 0x1F60, 0x1F61, 0x1F62, 0x1F63, 0x1F64, 0x1F65, 0x1F66,
    0x1F67, 0x1F68, 0x1F69, 0x1F6A, 0x1F6B, 0x1F6C, 0x1F6D, 0x1F6E, 0x1F6F, 0x1F70, 0x1F71, 0x1F72, 0x1F73,
    0x1F74, 0x1F75, 0x1F76, 0x1F77, 0x1F78, 0x1F79, 0x1F7A, 0x1F7B, 0x1F7C, 0x1F7D, 0x1F7E, 0x1F7F
};

static const xmedia_u32 g_sc8238_dgain_index[SC8238_DGAIN_INDEX_MAX] = {
    1024,  1056,  1089,  1120,  1152,  1184,  1217,  1248,  1280,  1312,  1345,  1376,  1408,  1440,  1473,  1504,
    1536,  1568,  1601,  1632,  1664,  1696,  1729,  1760,  1792,  1824,  1857,  1888,  1920,  1952,  1985,  2016,
    2048,  2113,  2176,  2241,  2304,  2369,  2432,  2497,  2560,  2625,  2688,  2753,  2816,  2881,  2944,  3009,
    3072,  3137,  3200,  3265,  3328,  3393,  3456,  3521,  3584,  3649,  3712,  3777,  3840,  3905,  3968,  4033,
    4096,  4224,  4352,  4480,  4608,  4736,  4864,  4992,  5120,  5248,  5376,  5504,  5632,  5760,  5888,  6016,
    6144,  6272,  6400,  6528,  6656,  6784,  6912,  7040,  7168,  7296,  7424,  7552,  7680,  7808,  7936,  8064,
    8192,  8448,  8704,  8960,  9216,  9472,  9728,  9984,  10240, 10496, 10752, 11008, 11264, 11520, 11776, 12032,
    12288, 12544, 12800, 13056, 13312, 13568, 13824, 14080, 14336, 14592, 14848, 15104, 15360, 15616, 15872, 16128,
    16384, 16896, 17408, 17920, 18432, 18944, 19456, 19968, 20480, 20992, 21504, 22016, 22528, 23040, 23552, 24064,
    24576, 25088, 25600, 26112, 26624, 27136, 27648, 28160, 28672, 29184, 29696, 30208, 30720, 31232, 31744, 32256,
};

static const xmedia_u32 g_sc8238_dgain_table[SC8238_DGAIN_INDEX_MAX] = {
    0x0080, 0x0084, 0x0088, 0x008C, 0x0090, 0x0094, 0x0098, 0x009C, 0x00A0, 0x00A4, 0x00A8, 0x00AC, 0x00B0, 0x00B4,
    0x00B8, 0x00BC, 0x00C0, 0x00C4, 0x00C8, 0x00CC, 0x00D0, 0x00D4, 0x00D8, 0x00DC, 0x00E0, 0x00E4, 0x00E8, 0x00EC,
    0x00F0, 0x00F4, 0x00F8, 0x00FC, 0x0180, 0x0184, 0x0188, 0x018C, 0x0190, 0x0194, 0x0198, 0x019C, 0x01A0, 0x01A4,
    0x01A8, 0x01AC, 0x01B0, 0x01B4, 0x01B8, 0x01BC, 0x01C0, 0x01C4, 0x01C8, 0x01CC, 0x01D0, 0x01D4, 0x01D8, 0x01DC,
    0x01E0, 0x01E4, 0x01E8, 0x01EC, 0x01F0, 0x01F4, 0x01F8, 0x01FC, 0x0380, 0x0384, 0x0388, 0x038C, 0x0390, 0x0394,
    0x0398, 0x039C, 0x03A0, 0x03A4, 0x03A8, 0x03AC, 0x03B0, 0x03B4, 0x03B8, 0x03BC, 0x03C0, 0x03C4, 0x03C8, 0x03CC,
    0x03D0, 0x03D4, 0x03D8, 0x03DC, 0x03E0, 0x03E4, 0x03E8, 0x03EC, 0x03F0, 0x03F4, 0x03F8, 0x03FC, 0x0780, 0x0784,
    0x0788, 0x078C, 0x0790, 0x0794, 0x0798, 0x079C, 0x07A0, 0x07A4, 0x07A8, 0x07AC, 0x07B0, 0x07B4, 0x07B8, 0x07BC,
    0x07C0, 0x07C4, 0x07C8, 0x07CC, 0x07D0, 0x07D4, 0x07D8, 0x07DC, 0x07E0, 0x07E4, 0x07E8, 0x07EC, 0x07F0, 0x07F4,
    0x07F8, 0x07FC, 0x0F80, 0x0F84, 0x0F88, 0x0F8C, 0x0F90, 0x0F94, 0x0F98, 0x0F9C, 0x0FA0, 0x0FA4, 0x0FA8, 0x0FAC,
    0x0FB0, 0x0FB4, 0x0FB8, 0x0FBC, 0x0FC0, 0x0FC4, 0x0FC8, 0x0FCC, 0x0FD0, 0x0FD4, 0x0FD8, 0x0FDC, 0x0FE0, 0x0FE4,
    0x0FE8, 0x0FEC, 0x0FF0, 0x0FF4, 0x0FF8, 0x0FFC,
};

static const xmedia_sensor_capability g_sc8238_capability = {
    .max_width  = 3840,
    .max_height = 2160,
    .max_fps    = SC8238_8M_MAX_FPS_LINEAR,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_4L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = SC8238_ADDR_BYTE,
        .data_byte_num     = SC8238_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { SC8238_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { SC8238_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x01 },
    },
};

static xmedia_s32 sc8238_ctx_init(xmedia_u32 dev)
{
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SC8238_GET_CTX(dev, sc8238_ctx);

    if (sc8238_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    sc8238_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (sc8238_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(sc8238_ctx, 0, sizeof(sensor_context));
    sc8238_ctx->i2c_addr         = SC8238_I2C_ADDR;
    sc8238_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_4L;
    sc8238_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    sc8238_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    sc8238_ctx->size.width       = 3840;
    sc8238_ctx->size.height      = 2160;
    sc8238_ctx->fps              = g_sc8238_property[0].max_fps;
    sc8238_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    sc8238_ctx->img_mode             = SC8238_8M_10BIT_LINEAR_MODE;
    sc8238_ctx->fl_std               = SC8238_VMAX_8M_LINEAR << SC8238_EXP_SHIFT;
    sc8238_ctx->fl[SENSOR_CUR_FRAME] = SC8238_VMAX_8M_LINEAR << SC8238_EXP_SHIFT;
    sc8238_ctx->fl[SENSOR_PRE_FRAME] = SC8238_VMAX_8M_LINEAR << SC8238_EXP_SHIFT;

    SC8238_SET_CTX(dev, sc8238_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void sc8238_ctx_exit(xmedia_u32 dev)
{
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_FREE(sc8238_ctx);
    SC8238_SET_CTX(dev, XMEDIA_NULL);
}

static xmedia_s32 sc8238_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    for (i = 0; i < SC8238_SPECS_MAX_NUM; i++) {
        if (g_sc8238_property[i].width == sc8238_ctx->size.width &&
            g_sc8238_property[i].height == sc8238_ctx->size.height &&
            g_sc8238_property[i].wdr_mode == sc8238_ctx->wdr_mode) {
            memcpy(property, &g_sc8238_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n",
                    sc8238_ctx->size.width, sc8238_ctx->size.height, sc8238_ctx->wdr_mode);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 sc8238_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc8238_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "No support mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc8238_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        sc8238_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        sc8238_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        sc8238_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 sc8238_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    sc8238_ctx->i2c_addr = slave_addr;
    ret = sc8238_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    *slave_addr = sc8238_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}


static xmedia_s32 sc8238_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    sc8238_ctx->init_mode = init_mode;
    ret = sc8238_i2c_init(dev, sc8238_ctx->bus_info.i2c_dev, sc8238_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    sc8238_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    ret = sc8238_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    sc8238_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    sc8238_ctx->mirror_en = mirror_en;
    ret = sc8238_set_mirror_flip(dev, mirror_en, sc8238_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    sc8238_ctx->flip_en = flip_en;
    ret = sc8238_set_mirror_flip(dev, sc8238_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_set_image_mode(sensor_context *sc8238_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (sc8238_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (SC8238_RES_IS_720P(sns_attr->width, sns_attr->height)) {
            *image_mode        = SC8238_1M_10BIT_LINEAR_MODE;
            sc8238_ctx->fl_std = SC8238_VMAX_1M_LINEAR << SC8238_EXP_SHIFT;
        } else if (SC8238_RES_IS_1080P(sns_attr->width, sns_attr->height)) {
            *image_mode        = SC8238_2M_10BIT_LINEAR_MODE;
            sc8238_ctx->fl_std = SC8238_VMAX_2M_LINEAR << SC8238_EXP_SHIFT;
        } else if (SC8238_RES_IS_1536P(sns_attr->width, sns_attr->height)) {
            *image_mode        = SC8238_3M_10BIT_LINEAR_MODE;
            sc8238_ctx->fl_std = SC8238_VMAX_3M_LINEAR << SC8238_EXP_SHIFT;
        }else if (SC8238_RES_IS_1520P(sns_attr->width, sns_attr->height)) {
            *image_mode        = SC8238_4M_10BIT_LINEAR_MODE;
            sc8238_ctx->fl_std = SC8238_VMAX_4M_LINEAR << SC8238_EXP_SHIFT;
        }else if (SC8238_RES_IS_1920P(sns_attr->width, sns_attr->height)) {
            *image_mode        = SC8238_5M_10BIT_LINEAR_MODE;
            sc8238_ctx->fl_std = SC8238_VMAX_5M_LINEAR << SC8238_EXP_SHIFT;
        }else if (SC8238_RES_IS_1728P(sns_attr->width, sns_attr->height)) {
            *image_mode        = SC8238_5M_10BIT_1728P_LINEAR_MODE;
            sc8238_ctx->fl_std = SC8238_VMAX_5M_1728P_LINEAR << SC8238_EXP_SHIFT;
        }else if (SC8238_RES_IS_2160P(sns_attr->width, sns_attr->height)) {
            *image_mode        = SC8238_8M_10BIT_LINEAR_MODE;
            sc8238_ctx->fl_std = SC8238_VMAX_8M_LINEAR << SC8238_EXP_SHIFT;
        } else {
            SC8238_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else if (sc8238_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (SC8238_RES_IS_720P(sns_attr->width, sns_attr->height)) {
            *image_mode               = SC8238_1M_10BIT_WDR_MODE;
            sc8238_ctx->fl_std        = SC8238_VMAX_1M_WDR << SC8238_EXP_SHIFT;
            sc8238_ctx->max_short_exp = SC8238_S_EXP_MAX_DEFAULT_720P << SC8238_EXP_SHIFT;
        } else if (SC8238_RES_IS_1080P(sns_attr->width, sns_attr->height)) {
            *image_mode               = SC8238_2M_10BIT_WDR_MODE;
            sc8238_ctx->fl_std        = SC8238_VMAX_2M_WDR << SC8238_EXP_SHIFT;
            sc8238_ctx->max_short_exp = SC8238_S_EXP_MAX_DEFAULT_2160P << SC8238_EXP_SHIFT;
        } else if (SC8238_RES_IS_1536P(sns_attr->width, sns_attr->height)) {
            *image_mode               = SC8238_3M_10BIT_WDR_MODE;
            sc8238_ctx->fl_std        = SC8238_VMAX_3M_WDR << SC8238_EXP_SHIFT;
            sc8238_ctx->max_short_exp = SC8238_S_EXP_MAX_DEFAULT_2160P << SC8238_EXP_SHIFT;
        } else if (SC8238_RES_IS_1520P(sns_attr->width, sns_attr->height)) {
            *image_mode               = SC8238_4M_10BIT_WDR_MODE;
            sc8238_ctx->fl_std        = SC8238_VMAX_4M_WDR << SC8238_EXP_SHIFT;
            sc8238_ctx->max_short_exp = SC8238_S_EXP_MAX_DEFAULT_2160P << SC8238_EXP_SHIFT;
        } else if (SC8238_RES_IS_1920P(sns_attr->width, sns_attr->height)) {
            *image_mode               = SC8238_5M_10BIT_WDR_MODE;
            sc8238_ctx->fl_std        = SC8238_VMAX_5M_WDR << SC8238_EXP_SHIFT;
            sc8238_ctx->max_short_exp = SC8238_S_EXP_MAX_DEFAULT_2160P << SC8238_EXP_SHIFT;
        } else if (SC8238_RES_IS_2160P(sns_attr->width, sns_attr->height)) {
            *image_mode               = SC8238_8M_10BIT_WDR_MODE;
            sc8238_ctx->fl_std        = SC8238_VMAX_8M_WDR << SC8238_EXP_SHIFT;
            sc8238_ctx->max_short_exp = SC8238_S_EXP_MAX_DEFAULT_2160P << SC8238_EXP_SHIFT;
        } else {
            SC8238_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }

    } else {
        SC8238_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc8238_ctx->size.width  = sns_attr->width;
    sc8238_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            sc8238_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            sc8238_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
            SENSOR_PRINT("2to1 line WDR\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(sc8238_ctx->wdr_int_time, 0, sizeof(sc8238_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

static xmedia_void sc8238_init_common_reg_info(sensor_context *sc8238_ctx)
{
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_EXP_H].delay_frame_num = 2;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_EXP_H].reg_addr        = SC8238_REG_ADDR_EXP_H;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_EXP_M].delay_frame_num = 2;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_EXP_M].reg_addr        = SC8238_REG_ADDR_EXP_M;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_EXP_L].delay_frame_num = 2;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_EXP_L].reg_addr        = SC8238_REG_ADDR_EXP_L;

    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_DGC_H].delay_frame_num = 2;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_DGC_H].reg_addr        = SC8238_REG_ADDR_DGAIN_H;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_DGC_L].delay_frame_num = 2;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_DGC_L].reg_addr        = SC8238_REG_ADDR_DGAIN_L;

    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_AGC_H].delay_frame_num = 2;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_AGC_H].reg_addr        = SC8238_REG_ADDR_AGAIN_H;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_AGC_L].delay_frame_num = 2;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_AGC_L].reg_addr        = SC8238_REG_ADDR_AGAIN_L;

    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_VMAX_H].delay_frame_num = 2;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_VMAX_H].reg_addr        = SC8238_REG_ADDR_VMAX_H;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_VMAX_L].delay_frame_num = 2;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_VMAX_L].reg_addr        = SC8238_REG_ADDR_VMAX_L;
}

static xmedia_void sc8238_init_2to1_wdr_reg_info(sensor_context *sc8238_ctx)
{
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_EXP_H].delay_frame_num   = 2;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_EXP_H].reg_addr          = SC8238_REG_ADDR_S_EXP_H;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_EXP_L].delay_frame_num   = 2;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_EXP_L].reg_addr          = SC8238_REG_ADDR_S_EXP_L;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_DGAIN_H].delay_frame_num = 2;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_DGAIN_H].reg_addr        = SC8238_REG_ADDR_S_DGAIN_H;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_DGAIN_L].delay_frame_num = 2;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_DGAIN_L].reg_addr        = SC8238_REG_ADDR_S_DGAIN_L;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_AGAIN_H].delay_frame_num = 2;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_AGAIN_H].reg_addr        = SC8238_REG_ADDR_S_AGAIN_H;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_AGAIN_L].delay_frame_num = 2;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_AGAIN_L].reg_addr        = SC8238_REG_ADDR_S_AGAIN_L;

    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_EXP_MAX_H].delay_frame_num = 0;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_EXP_MAX_H].reg_addr = SC8238_REG_ADDR_S_EXP_MAX_H;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_EXP_MAX_L].delay_frame_num = 0;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_EXP_MAX_L].reg_addr = SC8238_REG_ADDR_S_EXP_MAX_L;
}

static xmedia_s32 sc8238_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = sc8238_ctx->bus_info.i2c_dev;
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = SC8238_REG_L_MAX_NUM;

    if (sc8238_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc8238_ctx->regs_info[SENSOR_CUR_FRAME].reg_num = SC8238_REG_MAX_NUM;
    }

    for (i = 0; i < sc8238_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = sc8238_ctx->i2c_addr;
        sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = SC8238_ADDR_BYTE;
        sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = SC8238_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    sc8238_init_common_reg_info(sc8238_ctx);

    // init 2to1 wdr mode Regs
    if (sc8238_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc8238_init_2to1_wdr_reg_info(sc8238_ctx);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    // Set wdr mode
    ret = sc8238_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = sc8238_set_image_mode(sc8238_ctx, &sc8238_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    sc8238_ctx->fl[SENSOR_CUR_FRAME] = sc8238_ctx->fl_std;
    sc8238_ctx->fl[SENSOR_PRE_FRAME] = sc8238_ctx->fl[SENSOR_CUR_FRAME];

    ret = sc8238_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));

#ifdef SC8238_ISP_DEFAULT_SUPPORT
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from sc8238!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 sc8238_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (sc8238_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x104;
    } else {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x104;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x104;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    if (sc8238_ctx->mirror_en && sc8238_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (sc8238_ctx->mirror_en && (!sc8238_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!sc8238_ctx->mirror_en) && sc8238_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < SC8238_SPECS_MAX_NUM; i++) {
        if (g_sc8238_property[i].width == sc8238_ctx->size.width &&
            g_sc8238_property[i].height == sc8238_ctx->size.height &&
            g_sc8238_property[i].wdr_mode == sc8238_ctx->wdr_mode) {
            *bayer_pattern = g_sc8238_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= SC8238_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_sc8238_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc8238_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 sc8238_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_sc8238_exposure[dev]      = init_param->exposure;
    g_sc8238_sample_r_gain[dev] = init_param->sample_rgain;
    g_sc8238_sample_b_gain[dev] = init_param->sample_bgain;

    g_sc8238_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_sc8238_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_sc8238_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc8238_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case SC8238_1M_10BIT_LINEAR_MODE:
            *max_fps = SC8238_1M_MAX_FPS_LINEAR;
            break;
        case SC8238_2M_10BIT_LINEAR_MODE:
            *max_fps = SC8238_2M_MAX_FPS_LINEAR;
            break;
        case SC8238_3M_10BIT_LINEAR_MODE:
            *max_fps = SC8238_3M_MAX_FPS_LINEAR;
            break;
        case SC8238_4M_10BIT_LINEAR_MODE:
            *max_fps = SC8238_4M_MAX_FPS_LINEAR;
            break;
        case SC8238_5M_10BIT_LINEAR_MODE:
            *max_fps = SC8238_5M_MAX_FPS_LINEAR;
            break;
        case SC8238_5M_10BIT_1728P_LINEAR_MODE:
            *max_fps = SC8238_5M_1728P_MAX_FPS_LINEAR;
            break;
        case SC8238_8M_10BIT_LINEAR_MODE:
            *max_fps = SC8238_8M_MAX_FPS_LINEAR;
            break;

        case SC8238_1M_10BIT_WDR_MODE:
            *max_fps = SC8238_1M_MAX_FPS_WDR;
            break;
        case SC8238_2M_10BIT_WDR_MODE:
            *max_fps = SC8238_2M_MAX_FPS_WDR;
            break;
        case SC8238_3M_10BIT_WDR_MODE:
            *max_fps = SC8238_3M_MAX_FPS_WDR;
            break;
        case SC8238_4M_10BIT_WDR_MODE:
            *max_fps = SC8238_4M_MAX_FPS_WDR;
            break;
        case SC8238_5M_10BIT_WDR_MODE:
            *max_fps = SC8238_5M_MAX_FPS_WDR;
            break;
        case SC8238_8M_10BIT_WDR_MODE:
            *max_fps = SC8238_8M_MAX_FPS_WDR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8283_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = sc8238_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 0;
    ae_sns_dft->full_lines_max = SC8238_FULL_LINES_MAX;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->dgain_accu.accuracy  = 1;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 4 << ae_sns_dft->ispdgain_shift;

    memcpy(&ae_sns_dft->piris_attr, &g_sc8238_piris_attr, sizeof(xmedia_isp_piris_attr));
    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_32_0;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    // lines per second: VTS * max_fps
    ae_sns_dft->lines_per_500ms  = (sns_ctx->fl_std * max_fps) / 2;
    ae_sns_dft->man_ratio_enable = XMEDIA_FALSE;
    SENSOR_PRINT("man_ratio_enable: %d \n", ae_sns_dft->man_ratio_enable);

    return XMEDIA_SUCCESS;
}

static xmedia_void sc8238_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 2;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 16256; // max: 15.875 * 1024
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 32256; // max: 31.5 * 1024
    ae_sns_dft->min_dgain        = 1024;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x38;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_sc8238_exposure[dev] ? g_sc8238_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - SC8238_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 3;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = 3;

    return;
}

static xmedia_s32 sc8238_get_ae_2to1_wdr_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                 xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 4;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 16256; // 15.875 * 1024
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 32256; // 31.5 * 1024
    ae_sns_dft->min_dgain        = 1024;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;
    ae_sns_dft->init_exposure    = g_sc8238_exposure[dev] ? g_sc8238_exposure[dev] : 16462;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - sns_ctx->max_short_exp - SC8238_EXP_OFFSET_WDR;
    ae_sns_dft->min_int_time        = 5;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = 5;

    if (sc8238_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
        ae_sns_dft->ae_compensation = 0x30;
        ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    } else {
        ae_sns_dft->max_dgain_target    = 1024;
        ae_sns_dft->max_ispdgain_target = 4096;
        ae_sns_dft->ae_compensation     = 0x40;
        ae_sns_dft->ae_exp_mode         = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
        ae_sns_dft->man_ratio_enable    = XMEDIA_FALSE;

        ae_sns_dft->ratio[0] = 0x400;
        ae_sns_dft->ratio[1] = 0x40;
        ae_sns_dft->ratio[2] = 0x40;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    memset(ae_sns_dft, 0, sizeof(xmedia_isp_ae_sensor_default));

    ret = sc8283_get_ae_common_default(dev, sc8238_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (sc8238_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            sc8238_get_ae_linear_default(dev, sc8238_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            sc8238_get_ae_2to1_wdr_default(dev, sc8238_ctx, ae_sns_dft);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", sc8238_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context    *sc8238_ctx                       = XMEDIA_NULL;
    static xmedia_bool first[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 1 };

    SENSOR_CHECK_DEV_RETURN(dev);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    if (sc8238_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (first[dev]) { // 0: short exposure
            sc8238_ctx->wdr_int_time[0] = int_time;
            sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_EXP_H].data = SENSOR_MIDDLE_8BITS(int_time);
            sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_EXP_L].data = SENSOR_LOWER_4BITS(int_time);
            first[dev] = XMEDIA_FALSE;
        } else { // 1: long exposure
            sc8238_ctx->wdr_int_time[1] = int_time;
            sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_EXP_H].data = SENSOR_HIGHER_4BITS(int_time);
            sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_EXP_M].data = SENSOR_MIDDLE_8BITS(int_time);
            sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_EXP_L].data = SENSOR_LOWER_4BITS(int_time);
            first[dev] = XMEDIA_TRUE;
        }
    } else if (sc8238_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_EXP_H].data = SENSOR_HIGHER_4BITS(int_time);
        sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_EXP_M].data = SENSOR_MIDDLE_8BITS(int_time);
        sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_EXP_L].data = SENSOR_LOWER_4BITS(int_time);
        first[dev] = XMEDIA_TRUE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_sc8238_again_index[SC8238_AGAIN_INDEX_MAX - 1]) {
        *again_lin = g_sc8238_again_index[SC8238_AGAIN_INDEX_MAX - 1];
        *again_db  = g_sc8238_again_table[SC8238_AGAIN_INDEX_MAX - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < SC8238_AGAIN_INDEX_MAX; i++) {
        if (*again_lin < g_sc8238_again_index[i]) {
            *again_lin = g_sc8238_again_index[i - 1];
            *again_db  = g_sc8238_again_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(dgain_lin);
    SENSOR_CHECK_PTR_RETURN(dgain_db);

    if (*dgain_lin >= g_sc8238_dgain_index[SC8238_DGAIN_INDEX_MAX - 1]) {
        *dgain_lin = g_sc8238_dgain_index[SC8238_DGAIN_INDEX_MAX - 1];
        *dgain_db  = g_sc8238_dgain_table[SC8238_DGAIN_INDEX_MAX - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < SC8238_DGAIN_INDEX_MAX; i++) {
        if (*dgain_lin < g_sc8238_dgain_index[i]) {
            *dgain_lin = g_sc8238_dgain_index[i - 1];
            *dgain_db  = g_sc8238_dgain_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    sc8238_ctx->regs_info[0].i2c_data[SC8238_REG_DGC_H].data = SENSOR_HIGH_8BITS(dgain);
    sc8238_ctx->regs_info[0].i2c_data[SC8238_REG_DGC_L].data = SENSOR_LOW_8BITS(dgain);
    sc8238_ctx->regs_info[0].i2c_data[SC8238_REG_AGC_H].data = SENSOR_HIGH_8BITS(again);
    sc8238_ctx->regs_info[0].i2c_data[SC8238_REG_AGC_L].data = SENSOR_LOW_8BITS(again);

    if (sc8238_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc8238_ctx->regs_info[0].i2c_data[SC8238_REG_S_DGAIN_H].data = SENSOR_HIGH_8BITS(dgain);
        sc8238_ctx->regs_info[0].i2c_data[SC8238_REG_S_DGAIN_L].data = SENSOR_LOW_8BITS(dgain);
        sc8238_ctx->regs_info[0].i2c_data[SC8238_REG_S_AGAIN_H].data = SENSOR_HIGH_8BITS(again);
        sc8238_ctx->regs_info[0].i2c_data[SC8238_REG_S_AGAIN_L].data = SENSOR_LOW_8BITS(again);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = 5000;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = 366;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = 256;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = 256;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = 533;

    // 0 ~ 6: point index on the awb curve param
    awb_sns_dft->wb_para[0]   = 137;
    awb_sns_dft->wb_para[1]   = -13;
    awb_sns_dft->wb_para[2]   = -132;
    awb_sns_dft->wb_para[3]   = 155552;
    awb_sns_dft->wb_para[4]   = 128;
    awb_sns_dft->wb_para[5]   = -94868;
    awb_sns_dft->golden_rgain = 0;
    awb_sns_dft->golden_bgain = 0;
    awb_sns_dft->awb_runinterval = 2;
    switch (sc8238_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_sc8238_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_sc8238_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_sc8238_awb_wdr_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_sc8238_awb_wdr_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", sc8238_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_sc8238_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_sc8238_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_sc8238_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_sc8238_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_sc8238_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_standby(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc8238_write_reg(dev, SC8238_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_resume(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc8238_write_reg(dev, SC8238_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_void sc8238_delay_ms(xmedia_s32 ms)
{
    sleep_us(ms * 1000);
}

static xmedia_s32 sc8238_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *sc8238_ctx   = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < sc8238_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < sc8238_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= sc8238_write_reg(dev, sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    if (sc8238_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = sc8238_init_image(dev, sc8238_ctx->img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    sc8238_delay_ms(10);
    ret = sc8238_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc8238_set_mirror_flip(dev, sc8238_ctx->mirror_en, sc8238_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc8238_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_sc8238_dev_map[index] == SENSOR_DEV_INVALID) {
            g_sc8238_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 sc8238_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_sc8238_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 sc8238_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{ // TO-DO：min fps is to be confirmed.
    switch (img_mode) {
        case SC8238_1M_10BIT_LINEAR_MODE:
            *min_fps = 2.5;
            break;
        case SC8238_2M_10BIT_LINEAR_MODE:
            *min_fps = 1.5;
            break;
        case SC8238_3M_10BIT_LINEAR_MODE:
            *min_fps = 1.5;
            break;
        case SC8238_4M_10BIT_LINEAR_MODE:
            *min_fps = 1.5;
            break;
        case SC8238_5M_10BIT_LINEAR_MODE:
        case SC8238_5M_10BIT_1728P_LINEAR_MODE:
            *min_fps = 1.5;
            break;
        case SC8238_8M_10BIT_LINEAR_MODE:
            *min_fps = 1.5;
            break;
        case SC8238_1M_10BIT_WDR_MODE:
            *min_fps = 2;
            break;
        case SC8238_2M_10BIT_WDR_MODE:
            *min_fps = 0;
            break;
        case SC8238_3M_10BIT_WDR_MODE:
            *min_fps = 0;
            break;
        case SC8238_4M_10BIT_WDR_MODE:
            *min_fps = 0;
            break;
        case SC8238_5M_10BIT_WDR_MODE:
            *min_fps = 0;
            break;
        case SC8238_8M_10BIT_WDR_MODE:
            *min_fps = 0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case SC8238_1M_10BIT_LINEAR_MODE:
            *vmax = SC8238_VMAX_1M_LINEAR;
            break;
        case SC8238_2M_10BIT_LINEAR_MODE:
            *vmax = SC8238_VMAX_2M_LINEAR;
            break;
        case SC8238_3M_10BIT_LINEAR_MODE:
            *vmax = SC8238_VMAX_3M_LINEAR;
            break;
        case SC8238_4M_10BIT_LINEAR_MODE:
            *vmax = SC8238_VMAX_4M_LINEAR;
            break;
        case SC8238_5M_10BIT_LINEAR_MODE:
            *vmax = SC8238_VMAX_5M_LINEAR;
            break;
        case SC8238_5M_10BIT_1728P_LINEAR_MODE:
            *vmax = SC8238_VMAX_5M_1728P_LINEAR;
            break;
        case SC8238_8M_10BIT_LINEAR_MODE:
            *vmax = SC8238_VMAX_8M_LINEAR;
            break;

        case SC8238_1M_10BIT_WDR_MODE:
            *vmax = SC8238_VMAX_1M_WDR;
            break;
        case SC8238_2M_10BIT_WDR_MODE:
            *vmax = SC8238_VMAX_2M_WDR;
            break;
        case SC8238_3M_10BIT_WDR_MODE:
            *vmax = SC8238_VMAX_3M_WDR;
            break;
        case SC8238_4M_10BIT_WDR_MODE:
            *vmax = SC8238_VMAX_4M_WDR;
            break;
        case SC8238_5M_10BIT_WDR_MODE:
            *vmax = SC8238_VMAX_5M_WDR;
            break;
        case SC8238_8M_10BIT_WDR_MODE:
            *vmax = SC8238_VMAX_8M_WDR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = sc8238_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc8238_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc8238_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is (%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, SC8238_FULL_LINES_MAX);

    if (sns_ctx->img_mode == SC8238_1M_10BIT_WDR_MODE) {
        sns_ctx->max_short_exp = SC8238_S_EXP_MAX_DEFAULT_720P * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    } else if (sns_ctx->img_mode == SC8238_2M_10BIT_WDR_MODE ||
               sns_ctx->img_mode == SC8238_3M_10BIT_WDR_MODE ||
               sns_ctx->img_mode == SC8238_4M_10BIT_WDR_MODE ||
               sns_ctx->img_mode == SC8238_5M_10BIT_WDR_MODE ||
               sns_ctx->img_mode == SC8238_8M_10BIT_WDR_MODE) {
        sns_ctx->max_short_exp = SC8238_S_EXP_MAX_DEFAULT_2160P * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    for (i = 0; i < sc8238_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            sc8238_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &sc8238_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&sc8238_ctx->regs_info[SENSOR_PRE_FRAME], &sc8238_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    sc8238_ctx->fl[SENSOR_PRE_FRAME] = sc8238_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    ret = sc8238_calc_fps(fps, sc8238_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    sc8238_ctx->fl[SENSOR_CUR_FRAME] = full_lines << SC8238_EXP_SHIFT;
    sc8238_ctx->fl_std               = sc8238_ctx->fl[SENSOR_CUR_FRAME];
    sc8238_ctx->fps                  = fps;
    ae_sns_dft->fps                  = fps;
    ae_sns_dft->full_lines_std       = sc8238_ctx->fl_std;
    ae_sns_dft->full_lines           = sc8238_ctx->fl_std;

    if (sc8238_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sc8238_ctx->fl[SENSOR_CUR_FRAME] - SC8238_EXP_OFFSET_LINEAR;
    } else if (sc8238_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_EXP_MAX_H].data =
                                                                        SENSOR_HIGH_8BITS(sc8238_ctx->max_short_exp);
        sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_EXP_MAX_L].data =
                                                                        SENSOR_LOW_8BITS(sc8238_ctx->max_short_exp);
        sc8238_ctx->max_short_exp = sc8238_ctx->max_short_exp << SC8238_EXP_SHIFT;
        ae_sns_dft->max_int_time  =
                            sc8238_ctx->fl[SENSOR_CUR_FRAME] - sc8238_ctx->max_short_exp - SC8238_EXP_OFFSET_WDR;
    }

    SENSOR_PRINT("dev[%d]- sc8238 set fps: %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = sc8238_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc8238_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc8238_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = (vmax_max_fps << SC8238_EXP_SHIFT) * max_fps / min_fps;
    return XMEDIA_SUCCESS;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 sc8238_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    xmedia_u32      ratio;
    xmedia_u32      vmax;
    xmedia_u32      short_frame_min_exp;
    xmedia_u32      short_frame_max_exp;
    xmedia_u32      long_frame_min_exp;
    xmedia_u32      long_frame_max_exp;
    xmedia_u32      reg_short_frame_max_exp, diff;
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);
    memcpy(sc8238_ctx->ratio, inttime_attr->ratio, sizeof(inttime_attr->ratio));

    short_frame_min_exp = 5;
    long_frame_min_exp  = 5;
    if (sc8238_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (sc8238_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
            inttime_attr->min_inttime[0] = short_frame_min_exp; // 0:short frame; 1: long frame
            inttime_attr->max_inttime[0] = short_frame_min_exp;

            inttime_attr->min_inttime[1] = long_frame_min_exp;

            // short_frame_max_exp = 2 * ( reg_short_frame_max_exp - 8)
            reg_short_frame_max_exp = inttime_attr->min_inttime[0] / 2 + 8;

            // long_frame_max_exp = 2 * ( vmax - reg_short_frame_exp - 9)
            inttime_attr->max_inttime[1] = sc8238_ctx->fl[SENSOR_CUR_FRAME] - 2 * reg_short_frame_max_exp - 18;
        } else {
            /*
           * 已知：曝光比ratio, vmax
           * 求解：长帧最大曝光时间long_frame_max_exp， 短帧最大曝光时间short_frame_max_exp
           *       短帧最大曝光时间寄存器取值 reg_short_frame_max_exp
           * ratio = 64 * long_frame_max_exp / short_frame_max_exp; //AE所给的曝光比, 64为1倍
           * long_frame_max_exp = 2 * ( vmax - reg_short_frame_exp - 9); //9: long_exp_offset
           * short_frame_max_exp = 2 * ( reg_short_frame_max_exp - 8); // 8: s_exp_offset
           * 解得：
           * reg_short_frame_max_exp = (64 * vmax - 64 * 9 + 8 * ratio) / (64 + ratio)
           */
            ratio                   = SENSOR_DIV_0_TO_1(inttime_attr->ratio[0]);
            vmax                    = sc8238_ctx->fl[SENSOR_CUR_FRAME] >> SC8238_EXP_SHIFT;
            reg_short_frame_max_exp = (64 * vmax - 64 * 9 + 8 * ratio) / (64 + ratio);

            if(reg_short_frame_max_exp < (sc8238_ctx->max_short_exp >> SC8238_EXP_SHIFT)) {
                diff = (sc8238_ctx->max_short_exp >> SC8238_EXP_SHIFT) - reg_short_frame_max_exp;
                if(diff > 50) {
                    diff = 50;
                }

                reg_short_frame_max_exp = (xmedia_u32)(sc8238_ctx->max_short_exp >> SC8238_EXP_SHIFT) - diff;

                if(reg_short_frame_max_exp % 2 != 0) {
                    reg_short_frame_max_exp += 1;
                }
            }

            short_frame_max_exp     = 2 * (reg_short_frame_max_exp - 8);
            long_frame_max_exp      = short_frame_max_exp * ratio / 64;
            long_frame_min_exp      = short_frame_min_exp * ratio / 64;

            // 0:short frame; 1: long frame
            inttime_attr->min_inttime[0] = short_frame_min_exp;
            inttime_attr->min_inttime[1] = long_frame_min_exp;
            inttime_attr->max_inttime[0] = short_frame_max_exp;
            inttime_attr->max_inttime[1] = long_frame_max_exp;
        }

        sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_EXP_MAX_H].data =
                                                                        SENSOR_HIGH_8BITS(reg_short_frame_max_exp);
        sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_S_EXP_MAX_L].data =
                                                                        SENSOR_LOW_8BITS(reg_short_frame_max_exp);
        sc8238_ctx->max_short_exp = reg_short_frame_max_exp << SC8238_EXP_SHIFT;
    }

    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 * 注意：2to1 wdr模式下，曝光比和ae wdr mode 由AE传入，调用本接口前，AE需先调用sc8238_get_max_inttime
 */
static xmedia_s32 sc8238_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context               *sc8238_ctx = XMEDIA_NULL;
    xmedia_float                  min_fps;
    xmedia_u32                    vmax_min_fps;
    xmedia_s32                    ret;
    xmedia_sensor_ae_inttime_attr inttime_attr;
    return XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    if (sc8238_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", sc8238_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (sc8238_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        full_lines = full_lines + SC8238_EXP_OFFSET_LINEAR;
    } else if (sc8238_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        full_lines = full_lines + SC8238_EXP_OFFSET_WDR;
    }

    ret = sc8238_get_min_fps(sc8238_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc8238_get_min_fps_vmax(sc8238_ctx->img_mode, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                       = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    sc8238_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    sc8238_ctx->fps                  = min_fps * vmax_min_fps / full_lines;

    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_VMAX_H].data =
                                                                    SENSOR_HIGH_8BITS(full_lines >> SC8238_EXP_SHIFT);
    sc8238_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC8238_REG_VMAX_L].data =
                                                                    SENSOR_LOW_8BITS(full_lines >> SC8238_EXP_SHIFT);

    ae_sns_dft->full_lines = sc8238_ctx->fl[SENSOR_CUR_FRAME];
    if (sc8238_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sc8238_ctx->fl[SENSOR_CUR_FRAME] - SC8238_EXP_OFFSET_LINEAR;
    } else if (sc8238_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        inttime_attr.ratio[0] = sc8238_ctx->ratio[0]; // index 0: 长短帧曝光比
        sc8238_get_max_inttime(dev, &inttime_attr);
        ae_sns_dft->max_int_time = sc8238_ctx->fl[SENSOR_CUR_FRAME] - sc8238_ctx->max_short_exp - SC8238_EXP_OFFSET_WDR;
    }

    SENSOR_PRINT("dev[%d]- sc8238 set fps: %f\n", dev, sc8238_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    ret = sc8238_get_min_fps(sc8238_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc8238_get_max_fps(sc8238_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = sc8238_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc8238_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, SC8238_NAME, sizeof(SC8238_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = sc8238_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = sc8238_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = sc8238_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = sc8238_mirror;
    info->isp_func.pfn_sensor_flip             = sc8238_flip;
    info->isp_func.pfn_sensor_set_bus_info     = sc8238_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = sc8238_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = sc8238_set_init_param;
    info->isp_func.pfn_sensor_start            = sc8238_start;
    info->isp_func.pfn_sensor_stop             = sc8238_stop;
    info->isp_func.pfn_sensor_standby          = sc8238_standby;
    info->isp_func.pfn_sensor_resume           = sc8238_resume;
    info->isp_func.pfn_sensor_write_reg        = sc8238_write_reg;
    info->isp_func.pfn_sensor_read_reg         = sc8238_read_reg;
    info->isp_func.pfn_sensor_init             = sc8238_init;
    info->isp_func.pfn_sensor_exit             = sc8238_exit;
    info->isp_func.pfn_sensor_set_attr         = sc8238_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = sc8238_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = sc8238_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = sc8238_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = sc8238_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = sc8238_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = sc8238_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = sc8238_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = sc8238_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = sc8238_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = sc8238_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = sc8238_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = sc8238_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = sc8238_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = sc8238_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = sc8238_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = sc8238_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = sc8238_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc8238_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = sc8238_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc8238_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = sc8238_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "SC8238 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc8238_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *sc8238_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = sc8238_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    SC8238_GET_CTX(dev, sc8238_ctx);
    SENSOR_CHECK_PTR_RETURN(sc8238_ctx);

    if (sc8238_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "SC8238 unregister function failed!\n");
        return ret;
    }

    sc8238_ctx_exit(dev);
    g_sc8238_dev_map[dev] = SENSOR_DEV_INVALID; // reset dev

    return XMEDIA_SUCCESS;
}
