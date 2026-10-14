#include <stdlib.h>
#include <string.h>
#include "sc285sl.h"
#include "sc285sl_ctrl.h"
#include "sc285sl_ex.h"
#include "xmedia_isp.h"

#define SC285SL_NAME          "SC285SL"
#define SC285SL_4LANE_SPECS_MAX_NUM 1
#define SC285SL_2LANE_SPECS_MAX_NUM 1

#ifdef __linux__
#define SC285SL_ISP_DEFAULT_SUPPORT
#endif

#define SC285SL_2M_4LANE_1080P_BIT_RATE_LINEAR 668
#define SC285SL_2M_2LANE_1080P_BIT_RATE_LINEAR 1296

#define SC285SL_RES_IS_1080P(w, h) ((w) == 1920 && (h) == 1080)

#define SC285SL_ERR_MODE_PRINT(sns_attr)                                                                               \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sns_attr->width,                \
                     sns_attr->height, sns_attr->wdr_mode);                                                            \
    } while (0)

// awb static param for Fuji Lens New IR_Cut
#define SC285SL_CALIBRATE_STATIC_TEMP       5000
#define SC285SL_CALIBRATE_STATIC_WB_R_GAIN  433
#define SC285SL_CALIBRATE_STATIC_WB_GR_GAIN 256
#define SC285SL_CALIBRATE_STATIC_WB_GB_GAIN 256
#define SC285SL_CALIBRATE_STATIC_WB_B_GAIN  653

// Calibration results for Auto WB Planck
#define SC285SL_CALIBRATE_AWB_P1 165
#define SC285SL_CALIBRATE_AWB_P2 (-89)
#define SC285SL_CALIBRATE_AWB_Q1 (-177)
#define SC285SL_CALIBRATE_AWB_A1 200851
#define SC285SL_CALIBRATE_AWB_B1 128
#define SC285SL_CALIBRATE_AWB_C1 (-139601)

// Rgain and Bgain of the golden sample
#define SC285SL_GOLDEN_RGAIN 0
#define SC285SL_GOLDEN_BGAIN 0

#define SC285SL_AGAIN_INDEX_MAX 236
#define SC285SL_DGAIN_INDEX_MAX 256

sensor_context *g_sc285sl_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define SC285SL_SET_CONTEXT(dev, sns_ctx) g_sc285sl_ctx[dev] = sns_ctx
#define SC285SL_GET_CONTEXT(dev, sns_ctx) sns_ctx = g_sc285sl_ctx[dev]

static xmedia_u32 g_sc285sl_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]                             = { 0 };
static xmedia_u16 g_sc285sl_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM]                        = { 0 };
static xmedia_u16 g_sc285sl_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM]                        = { 0 };
static xmedia_u16 g_sc285sl_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };
static xmedia_s32 g_sc285sl_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

static const xmedia_sensor_property g_sc285sl_4lane_property[SC285SL_4LANE_SPECS_MAX_NUM] = {
    {
     // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_NONE,
     // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
     // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_12,
     // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
     // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC285SL_2M_4LANE_1080P_BIT_RATE_LINEAR, },
};

static const xmedia_sensor_property g_sc285sl_2lane_property[SC285SL_2LANE_SPECS_MAX_NUM] = {
    {
     // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_NONE,
     // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
     // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_12,
     // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
     // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC285SL_2M_2LANE_1080P_BIT_RATE_LINEAR, },
};

static const xmedia_u32 g_sc285sl_again_index[SC285SL_AGAIN_INDEX_MAX] = {
    1024,   1056,   1089,   1120,   1152,   1184,   1217,   1248,   1280,   1312,   1345,   1376,   1408,   1440,
    1473,   1504,   1536,   1568,   1601,   1632,   1664,   1696,   1729,   1760,   1792,   1824,   1857,   1888,
    1920,   1952,   1985,   2016,   2048,   2113,   2176,   2241,   2304,   2369,   2432,   2497,   2560,   2625,
    2688,   2753,   2816,   2881,   2944,   3009,   3072,   3137,   3200,   3265,   3328,   3393,   3456,   3521,
    3584,   3649,   3712,   3777,   3840,   3905,   3968,   4033,   4096,   4224,   4352,   4480,   4608,   4736,
    4864,   4992,   5120,   5248,   5376,   5504,   5530,   5703,   5876,   6048,   6221,   6394,   6567,   6739,
    6912,   7085,   7258,   7430,   7603,   7776,   7949,   8121,   8294,   8467,   8641,   8813,   8986,   9159,
    9332,   9504,   9677,   9850,   10023,  10195,  10368,  10541,  10714,  10886,  11059,  11405,  11750,  12097,
    12442,  12788,  13133,  13479,  13824,  14170,  14515,  14861,  15206,  15553,  15898,  16244,  16589,  16935,
    17280,  17626,  17971,  18317,  18662,  19009,  19354,  19700,  20045,  20391,  20736,  21082,  21427,  21773,
    22118,  22810,  23501,  24192,  24883,  25574,  26266,  26957,  27648,  28339,  29030,  29722,  30413,  31104,
    31795,  32486,  33178,  33869,  34560,  35251,  35942,  36634,  37325,  38016,  38707,  39398,  40090,  40781,
    41472,  42163,  42854,  43546,  44237,  45619,  47002,  48384,  49766,  51149,  52531,  53914,  55296,  56678,
    58061,  59443,  60826,  62208,  63590,  64973,  66355,  67738,  69120,  70502,  71885,  73267,  74650,  76032,
    77414,  78797,  80179,  81562,  82944,  84326,  85709,  87091,  88474,  91238,  94003,  96768,  99533,  102298,
    105062, 107827, 110592, 113357, 116122, 118886, 121651, 124416, 127181, 129946, 132710, 135475, 138240, 141005,
    143770, 146534, 149299, 152064, 154829, 157594, 160358, 163123, 165888, 168653, 171418, 174182,
};

static const xmedia_u32 g_sc285sl_again_table[SC285SL_AGAIN_INDEX_MAX] = {
    0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D,
    0x002E, 0x002F, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B,
    0x003C, 0x003D, 0x003E, 0x003F, 0x0120, 0x0121, 0x0122, 0x0123, 0x0124, 0x0125, 0x0126, 0x0127, 0x0128, 0x0129,
    0x012A, 0x012B, 0x012C, 0x012D, 0x012E, 0x012F, 0x0130, 0x0131, 0x0132, 0x0133, 0x0134, 0x0135, 0x0136, 0x0137,
    0x0138, 0x0139, 0x013A, 0x013B, 0x013C, 0x013D, 0x013E, 0x013F, 0x0320, 0x0321, 0x0322, 0x0323, 0x0324, 0x0325,
    0x0326, 0x0327, 0x0328, 0x0329, 0x032A, 0x032B, 0x8020, 0x8021, 0x8022, 0x8023, 0x8024, 0x8025, 0x8026, 0x8027,
    0x8028, 0x8029, 0x802A, 0x802B, 0x802C, 0x802D, 0x802E, 0x802F, 0x8030, 0x8031, 0x8032, 0x8033, 0x8034, 0x8035,
    0x8036, 0x8037, 0x8038, 0x8039, 0x803A, 0x803B, 0x803C, 0x803D, 0x803E, 0x803F, 0x8120, 0x8121, 0x8122, 0x8123,
    0x8124, 0x8125, 0x8126, 0x8127, 0x8128, 0x8129, 0x812A, 0x812B, 0x812C, 0x812D, 0x812E, 0x812F, 0x8130, 0x8131,
    0x8132, 0x8133, 0x8134, 0x8135, 0x8136, 0x8137, 0x8138, 0x8139, 0x813A, 0x813B, 0x813C, 0x813D, 0x813E, 0x813F,
    0x8320, 0x8321, 0x8322, 0x8323, 0x8324, 0x8325, 0x8326, 0x8327, 0x8328, 0x8329, 0x832A, 0x832B, 0x832C, 0x832D,
    0x832E, 0x832F, 0x8330, 0x8331, 0x8332, 0x8333, 0x8334, 0x8335, 0x8336, 0x8337, 0x8338, 0x8339, 0x833A, 0x833B,
    0x833C, 0x833D, 0x833E, 0x833F, 0x8720, 0x8721, 0x8722, 0x8723, 0x8724, 0x8725, 0x8726, 0x8727, 0x8728, 0x8729,
    0x872A, 0x872B, 0x872C, 0x872D, 0x872E, 0x872F, 0x8730, 0x8731, 0x8732, 0x8733, 0x8734, 0x8735, 0x8736, 0x8737,
    0x8738, 0x8739, 0x873A, 0x873B, 0x873C, 0x873D, 0x873E, 0x873F, 0x8f20, 0x8f21, 0x8f22, 0x8f23, 0x8f24, 0x8f25,
    0x8f26, 0x8f27, 0x8f28, 0x8f29, 0x8f2A, 0x8f2B, 0x8f2C, 0x8f2D, 0x8f2E, 0x8f2F, 0x8f30, 0x8f31, 0x8f32, 0x8f33,
    0x8f34, 0x8f35, 0x8f36, 0x8f37, 0x8f38, 0x8f39, 0x8f3A, 0x8f3B, 0x8f3C, 0x8f3D, 0x8f3E, 0x8f3F,
};

static const xmedia_u32 g_sc285sl_dgain_index[SC285SL_DGAIN_INDEX_MAX] = {
    1024,  1040,  1056,  1072,  1089,  1104,  1120,  1136,  1152,  1168,  1184,  1200,  1217,  1232,  1248,  1264,
    1280,  1296,  1312,  1328,  1345,  1360,  1376,  1392,  1408,  1424,  1440,  1456,  1473,  1488,  1504,  1520,
    1536,  1552,  1568,  1584,  1601,  1616,  1632,  1648,  1664,  1680,  1696,  1712,  1729,  1744,  1760,  1776,
    1792,  1808,  1824,  1840,  1857,  1872,  1888,  1904,  1920,  1936,  1952,  1968,  1985,  2000,  2016,  2032,
    2048,  2080,  2113,  2144,  2176,  2208,  2241,  2272,  2304,  2336,  2369,  2400,  2432,  2464,  2497,  2528,
    2560,  2592,  2625,  2656,  2688,  2720,  2753,  2784,  2816,  2848,  2881,  2912,  2944,  2976,  3009,  3040,
    3072,  3104,  3137,  3168,  3200,  3232,  3265,  3296,  3328,  3360,  3393,  3424,  3456,  3488,  3521,  3552,
    3584,  3616,  3649,  3680,  3712,  3744,  3777,  3808,  3840,  3872,  3905,  3936,  3968,  4000,  4033,  4064,
    4096,  4161,  4224,  4289,  4352,  4417,  4480,  4545,  4608,  4673,  4736,  4801,  4864,  4929,  4992,  5057,
    5120,  5185,  5248,  5313,  5376,  5441,  5504,  5569,  5632,  5697,  5760,  5825,  5888,  5953,  6016,  6081,
    6144,  6209,  6272,  6337,  6400,  6465,  6528,  6593,  6656,  6721,  6784,  6849,  6912,  6977,  7040,  7105,
    7168,  7233,  7296,  7361,  7424,  7489,  7552,  7617,  7680,  7745,  7808,  7873,  7936,  8001,  8064,  8129,
    8192,  8320,  8448,  8576,  8704,  8832,  8960,  9088,  9216,  9344,  9472,  9600,  9728,  9856,  9984,  10112,
    10240, 10368, 10496, 10624, 10752, 10880, 11008, 11136, 11264, 11392, 11520, 11648, 11776, 11904, 12032, 12160,
    12288, 12416, 12544, 12672, 12800, 12928, 13056, 13184, 13312, 13440, 13568, 13696, 13824, 13952, 14080, 14208,
    14336, 14464, 14592, 14720, 14848, 14976, 15104, 15232, 15360, 15488, 15616, 15744, 15872, 16000, 16128, 16256,
};

static const xmedia_u32 g_sc285sl_dgain_table[SC285SL_DGAIN_INDEX_MAX] = {
    0x0080, 0x0082, 0x0084, 0x0086, 0x0088, 0x008a, 0x008c, 0x008e, 0x0090, 0x0092, 0x0094, 0x0096, 0x0098, 0x009a,
    0x009c, 0x009e, 0x00a0, 0x00a2, 0x00a4, 0x00a6, 0x00a8, 0x00aa, 0x00ac, 0x00ae, 0x00b0, 0x00b2, 0x00b4, 0x00b6,
    0x00b8, 0x00ba, 0x00bc, 0x00be, 0x00c0, 0x00c2, 0x00c4, 0x00c6, 0x00c8, 0x00ca, 0x00cc, 0x00ce, 0x00d0, 0x00d2,
    0x00d4, 0x00d6, 0x00d8, 0x00da, 0x00dc, 0x00de, 0x00e0, 0x00e2, 0x00e4, 0x00e6, 0x00e8, 0x00ea, 0x00ec, 0x00ee,
    0x00f0, 0x00f2, 0x00f4, 0x00f6, 0x00f8, 0x00fa, 0x00fc, 0x00fe, 0x0180, 0x0182, 0x0184, 0x0186, 0x0188, 0x018a,
    0x018c, 0x018e, 0x0190, 0x0192, 0x0194, 0x0196, 0x0198, 0x019a, 0x019c, 0x019e, 0x01a0, 0x01a2, 0x01a4, 0x01a6,
    0x01a8, 0x01aa, 0x01ac, 0x01ae, 0x01b0, 0x01b2, 0x01b4, 0x01b6, 0x01b8, 0x01ba, 0x01bc, 0x01be, 0x01c0, 0x01c2,
    0x01c4, 0x01c6, 0x01c8, 0x01ca, 0x01cc, 0x01ce, 0x01d0, 0x01d2, 0x01d4, 0x01d6, 0x01d8, 0x01da, 0x01dc, 0x01de,
    0x01e0, 0x01e2, 0x01e4, 0x01e6, 0x01e8, 0x01ea, 0x01ec, 0x01ee, 0x01f0, 0x01f2, 0x01f4, 0x01f6, 0x01f8, 0x01fa,
    0x01fc, 0x01fe, 0x0380, 0x0382, 0x0384, 0x0386, 0x0388, 0x038a, 0x038c, 0x038e, 0x0390, 0x0392, 0x0394, 0x0396,
    0x0398, 0x039a, 0x039c, 0x039e, 0x03a0, 0x03a2, 0x03a4, 0x03a6, 0x03a8, 0x03aa, 0x03ac, 0x03ae, 0x03b0, 0x03b2,
    0x03b4, 0x03b6, 0x03b8, 0x03ba, 0x03bc, 0x03be, 0x03c0, 0x03c2, 0x03c4, 0x03c6, 0x03c8, 0x03ca, 0x03cc, 0x03ce,
    0x03d0, 0x03d2, 0x03d4, 0x03d6, 0x03d8, 0x03da, 0x03dc, 0x03de, 0x03e0, 0x03e2, 0x03e4, 0x03e6, 0x03e8, 0x03ea,
    0x03ec, 0x03ee, 0x03f0, 0x03f2, 0x03f4, 0x03f6, 0x03f8, 0x03fa, 0x03fc, 0x03fe, 0x0780, 0x0782, 0x0784, 0x0786,
    0x0788, 0x078a, 0x078c, 0x078e, 0x0790, 0x0792, 0x0794, 0x0796, 0x0798, 0x079a, 0x079c, 0x079e, 0x07a0, 0x07a2,
    0x07a4, 0x07a6, 0x07a8, 0x07aa, 0x07ac, 0x07ae, 0x07b0, 0x07b2, 0x07b4, 0x07b6, 0x07b8, 0x07ba, 0x07bc, 0x07be,
    0x07c0, 0x07c2, 0x07c4, 0x07c6, 0x07c8, 0x07ca, 0x07cc, 0x07ce, 0x07d0, 0x07d2, 0x07d4, 0x07d6, 0x07d8, 0x07da,
    0x07dc, 0x07de, 0x07e0, 0x07e2, 0x07e4, 0x07e6, 0x07e8, 0x07ea, 0x07ec, 0x07ee, 0x07f0, 0x07f2, 0x07f4, 0x07f6,
    0x07f8, 0x07fa, 0x07fc, 0x07fe,
};

static const xmedia_sensor_capability g_sc285sl_capability = {
    .max_width   = 1920,
    .max_height  = 1080,
    .max_fps     = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_12,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .mipi_lanes_supported  = XMEDIA_SENSOR_MIPI_LANES_4L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = SC285SL_ADDR_BYTE,
        .data_byte_num     = SC285SL_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { SC285SL_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { SC285SL_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x01 },
    },
};

static xmedia_s32 sc285sl_ctx_init(xmedia_u32 dev)
{
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    if (sc285sl_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    sc285sl_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (sc285sl_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(sc285sl_ctx, 0, sizeof(sensor_context));
    sc285sl_ctx->i2c_addr         = SC285SL_I2C_ADDR;
    sc285sl_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_2L;
    sc285sl_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    sc285sl_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    sc285sl_ctx->size.width       = 1920;
    sc285sl_ctx->size.height      = 1080;
    sc285sl_ctx->fps              = g_sc285sl_4lane_property[0].max_fps;
    sc285sl_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    sc285sl_ctx->img_mode             = SC285SL_2M_4LANE_1080_12BIT_LINEAR_MODE;
    sc285sl_ctx->fl_std               = SC285SL_VMAX_2M_4LANE_1080_LINEAR;
    sc285sl_ctx->fl[SENSOR_CUR_FRAME] = SC285SL_VMAX_2M_4LANE_1080_LINEAR;
    sc285sl_ctx->fl[SENSOR_PRE_FRAME] = SC285SL_VMAX_2M_4LANE_1080_LINEAR;

    SC285SL_SET_CONTEXT(dev, sc285sl_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void sc285sl_ctx_exit(xmedia_u32 dev)
{
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_FREE(sc285sl_ctx);
    SC285SL_SET_CONTEXT(dev, XMEDIA_NULL);
}

static xmedia_s32 sc285sl_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    if (sc285sl_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_4L) {
        for (i = 0; i < SC285SL_4LANE_SPECS_MAX_NUM; i++) {
            if (g_sc285sl_4lane_property[i].width == sc285sl_ctx->size.width &&
                g_sc285sl_4lane_property[i].height == sc285sl_ctx->size.height &&
                g_sc285sl_4lane_property[i].wdr_mode == sc285sl_ctx->wdr_mode) {
                memcpy(property, &g_sc285sl_4lane_property[i], sizeof(xmedia_sensor_property));
                return XMEDIA_SUCCESS;
            }
        }
    } else if (sc285sl_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_2L) {
        for (i = 0; i < SC285SL_2LANE_SPECS_MAX_NUM; i++) {
            if (g_sc285sl_2lane_property[i].width == sc285sl_ctx->size.width &&
                g_sc285sl_2lane_property[i].height == sc285sl_ctx->size.height &&
                g_sc285sl_2lane_property[i].wdr_mode == sc285sl_ctx->wdr_mode) {
                memcpy(property, &g_sc285sl_2lane_property[i], sizeof(xmedia_sensor_property));
                return XMEDIA_SUCCESS;
            }
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n", sc285sl_ctx->size.width,
                 sc285sl_ctx->size.height, sc285sl_ctx->wdr_mode);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 sc285sl_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    sc285sl_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L && mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "No support mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc285sl_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        sc285sl_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        sc285sl_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        sc285sl_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 sc285sl_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    sc285sl_ctx->i2c_addr = slave_addr;
    ret                   = sc285sl_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    *slave_addr = sc285sl_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    sc285sl_ctx->init_mode = init_mode;

    ret = sc285sl_i2c_init(dev, sc285sl_ctx->bus_info.i2c_dev, sc285sl_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    sc285sl_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    ret = sc285sl_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    sc285sl_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_void sc285sl_init_common_reg_info(sensor_context *sc285sl_ctx)
{
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_EXP_HIGHER].delay_frame_num = 2;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_EXP_HIGHER].reg_addr   = SC285SL_REG_ADDR_EXP_HIGHER;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_EXP_H].delay_frame_num = 2;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_EXP_H].reg_addr        = SC285SL_REG_ADDR_EXP_H;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_EXP_M].delay_frame_num = 2;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_EXP_M].reg_addr        = SC285SL_REG_ADDR_EXP_M;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_EXP_L].delay_frame_num = 2;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_EXP_L].reg_addr        = SC285SL_REG_ADDR_EXP_L;

    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_AGAIN_H].delay_frame_num = 2;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_AGAIN_H].reg_addr        = SC285SL_REG_ADDR_AGAIN_H;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_AGAIN_L].delay_frame_num = 2;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_AGAIN_L].reg_addr        = SC285SL_REG_ADDR_AGAIN_L;

    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_DGAIN_H].delay_frame_num = 2;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_DGAIN_H].reg_addr        = SC285SL_REG_ADDR_DGAIN_H;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_DGAIN_L].delay_frame_num = 2;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_DGAIN_L].reg_addr        = SC285SL_REG_ADDR_DGAIN_L;

    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_VMAX_H].delay_frame_num = 1;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_VMAX_H].reg_addr        = SC285SL_REG_ADDR_VMAX_H;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_VMAX_M].delay_frame_num = 1;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_VMAX_M].reg_addr        = SC285SL_REG_ADDR_VMAX_M;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_VMAX_L].delay_frame_num = 1;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_VMAX_L].reg_addr        = SC285SL_REG_ADDR_VMAX_L;

    return;
}

xmedia_s32 sc285sl_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = sc285sl_ctx->bus_info.i2c_dev;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = SC285SL_REG_MAX_NUM;

    for (i = 0; i < sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = sc285sl_ctx->i2c_addr;
        sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = SC285SL_ADDR_BYTE;
        sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = SC285SL_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    sc285sl_init_common_reg_info(sc285sl_ctx);

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc285sl_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case SC285SL_2M_4LANE_1080_12BIT_LINEAR_MODE:
        case SC285SL_2M_2LANE_1080_12BIT_LINEAR_MODE:
            *max_fps = 30.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{ // TO-DO：min fps is to be confirmed.
    switch (img_mode) {
        case SC285SL_2M_4LANE_1080_12BIT_LINEAR_MODE:
        case SC285SL_2M_2LANE_1080_12BIT_LINEAR_MODE:
            *min_fps = 1;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case SC285SL_2M_4LANE_1080_12BIT_LINEAR_MODE:
            *vmax = SC285SL_VMAX_2M_4LANE_1080_LINEAR;
            break;
        case SC285SL_2M_2LANE_1080_12BIT_LINEAR_MODE:
            *vmax = SC285SL_VMAX_2M_2LANE_1080_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = sc285sl_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc285sl_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc285sl_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is (%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, SC285SL_FULL_LINES_MAX);

    SENSOR_PRINT("sc285sl set fps = %f\n", fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    sc285sl_ctx->mirror_en = mirror_en;
    ret                    = sc285sl_set_mirror_flip(dev, mirror_en, sc285sl_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    sc285sl_ctx->flip_en = flip_en;
    ret                  = sc285sl_set_mirror_flip(dev, sc285sl_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_set_image_mode(sensor_context *sc285sl_ctx, xmedia_u8 *image_mode,
                                         const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (sc285sl_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (SC285SL_RES_IS_1080P(sns_attr->width, sns_attr->height) &&
                                (sc285sl_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_4L)) {
            *image_mode         = SC285SL_2M_4LANE_1080_12BIT_LINEAR_MODE;
            sc285sl_ctx->fl_std = SC285SL_VMAX_2M_4LANE_1080_LINEAR;
        } else if (SC285SL_RES_IS_1080P(sns_attr->width, sns_attr->height) &&
                                (sc285sl_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_2L)) {
            *image_mode         = SC285SL_2M_2LANE_1080_12BIT_LINEAR_MODE;
            sc285sl_ctx->fl_std = SC285SL_VMAX_2M_2LANE_1080_LINEAR;
        } else {
            SC285SL_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        SC285SL_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc285sl_ctx->size.width  = sns_attr->width;
    sc285sl_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            sc285sl_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            sc285sl_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
            SENSOR_PRINT("2to1 line WDR\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(sc285sl_ctx->wdr_int_time, 0, sizeof(sc285sl_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    // Set wdr mode
    ret = sc285sl_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = sc285sl_set_image_mode(sc285sl_ctx, &sc285sl_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    sc285sl_ctx->fl[SENSOR_CUR_FRAME] = sc285sl_ctx->fl_std;
    sc285sl_ctx->fl[SENSOR_PRE_FRAME] = sc285sl_ctx->fl[SENSOR_CUR_FRAME];

    ret = sc285sl_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef SC285SL_ISP_DEFAULT_SUPPORT
    // TO-DO: alg add param
    isp_default->blc         = &g_sc285sl_blc_attr;
    isp_default->bnr         = &g_sc285sl_bnr;
    isp_default->clut_attr   = &g_sc285sl_clut_attr;
    isp_default->crosstalk   = XMEDIA_NULL;
    isp_default->csc         = XMEDIA_NULL;
    isp_default->dehaze      = &g_sc285sl_dehaze_attr;
    isp_default->demosaic    = &g_sc285sl_demosaic_attr;
    isp_default->dpc_dynamic = XMEDIA_NULL;
    isp_default->dpc_static  = XMEDIA_NULL;
    isp_default->drc         = XMEDIA_NULL;
    isp_default->expander    = XMEDIA_NULL;
    isp_default->fpn         = XMEDIA_NULL;
    isp_default->gamma       = &g_sc285sl_gamma_attr;
    isp_default->gcac        = XMEDIA_NULL;
    isp_default->hlc         = XMEDIA_NULL;
    isp_default->lce         = &g_sc285sl_lce_attr;
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from sc285sl!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 sc285sl_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (sc285sl_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x100;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc285sl_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    for (i = 0; i < sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            sc285sl_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &sc285sl_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&sc285sl_ctx->regs_info[SENSOR_PRE_FRAME], &sc285sl_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    sc285sl_ctx->fl[SENSOR_PRE_FRAME] = sc285sl_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    if (sc285sl_ctx->mirror_en && sc285sl_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (sc285sl_ctx->mirror_en && (!sc285sl_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!sc285sl_ctx->mirror_en) && sc285sl_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    if (sc285sl_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_4L) {
        for (i = 0; i < SC285SL_4LANE_SPECS_MAX_NUM; i++) {
            if (g_sc285sl_4lane_property[i].width == sc285sl_ctx->size.width &&
                g_sc285sl_4lane_property[i].height == sc285sl_ctx->size.height &&
                g_sc285sl_4lane_property[i].wdr_mode == sc285sl_ctx->wdr_mode) {
                *bayer_pattern = g_sc285sl_4lane_property[i].bayer_format[type];
                break;
            }
        }

        if (i >= SC285SL_4LANE_SPECS_MAX_NUM) {
            *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
            SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else if (sc285sl_ctx->lanes == XMEDIA_SENSOR_MIPI_LANES_2L) {
        for (i = 0; i < SC285SL_2LANE_SPECS_MAX_NUM; i++) {
            if (g_sc285sl_2lane_property[i].width == sc285sl_ctx->size.width &&
                g_sc285sl_2lane_property[i].height == sc285sl_ctx->size.height &&
                g_sc285sl_2lane_property[i].wdr_mode == sc285sl_ctx->wdr_mode) {
                *bayer_pattern = g_sc285sl_2lane_property[i].bayer_format[type];
                break;
            }
        }

        if (i >= SC285SL_2LANE_SPECS_MAX_NUM) {
            *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
            SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_sc285sl_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc285sl_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 sc285sl_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_sc285sl_exposure[dev]      = init_param->exposure;
    g_sc285sl_sample_r_gain[dev] = init_param->sample_rgain;
    g_sc285sl_sample_b_gain[dev] = init_param->sample_bgain;

    g_sc285sl_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_sc285sl_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_sc285sl_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = sc285sl_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256;
    ae_sns_dft->full_lines_max = SC285SL_FULL_LINES_MAX;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->dgain_accu.accuracy  = 1;

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
    SENSOR_PRINT("man_ratio_enable: %d \n", ae_sns_dft->man_ratio_enable);

    return XMEDIA_SUCCESS;
}

static xmedia_void sc285sl_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                 xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 174182;
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 16256;
    ae_sns_dft->min_dgain        = 1024;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x38;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_sc285sl_exposure[dev] ? g_sc285sl_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - SC285SL_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 1;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = 1;

    return;
}

static xmedia_s32 sc285sl_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = sc285sl_get_ae_common_default(dev, sc285sl_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (sc285sl_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            sc285sl_get_ae_linear_default(dev, sc285sl_ctx, ae_sns_dft);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", sc285sl_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc285sl_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    ret = sc285sl_calc_fps(fps, sc285sl_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_VMAX_H].data = (full_lines & 0x3F0000) >> 16;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_VMAX_M].data = SENSOR_HIGH_8BITS(full_lines);
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    sc285sl_ctx->fps                  = fps;
    sc285sl_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    sc285sl_ctx->fl_std               = sc285sl_ctx->fl[SENSOR_CUR_FRAME];

    ae_sns_dft->fps            = fps;
    ae_sns_dft->full_lines_std = sc285sl_ctx->fl_std;
    ae_sns_dft->full_lines     = sc285sl_ctx->fl_std;

    if (sc285sl_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sc285sl_ctx->fl[SENSOR_CUR_FRAME] - SC285SL_EXP_OFFSET_LINEAR;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32      ret;
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    *fps = sc285sl_ctx->fps;
    ret  = sc285sl_get_min_fps(sc285sl_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc285sl_get_max_fps(sc285sl_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = sc285sl_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc285sl_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc285sl_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = vmax_max_fps * max_fps / min_fps;

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc285sl_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *sc285sl_ctx = XMEDIA_NULL;
    xmedia_u32      vmax_min_fps;
    xmedia_s32      ret;
    xmedia_float    fps;
    xmedia_float    min_fps;

    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    if (sc285sl_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        full_lines = full_lines + SC285SL_EXP_OFFSET_LINEAR;
    }

    ret = sc285sl_get_min_fps_vmax(sc285sl_ctx->img_mode, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc285sl_get_min_fps(sc285sl_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                        = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    sc285sl_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_VMAX_H].data = (full_lines & 0x3F0000) >> 16;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_VMAX_M].data = SENSOR_HIGH_8BITS(full_lines);
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    ae_sns_dft->full_lines   = sc285sl_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = sc285sl_ctx->fl[SENSOR_CUR_FRAME] - SC285SL_EXP_OFFSET_LINEAR;

    fps = min_fps * vmax_min_fps / full_lines;
    SENSOR_PRINT("sensor fps: %f\n", fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_EXP_HIGHER].data = (int_time & 0x300000) >> 20;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_EXP_H].data = (int_time & 0xFF000) >> 12;
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_EXP_M].data = SENSOR_MIDDLE_8BITS(int_time);
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_EXP_L].data = SENSOR_LOWER_4BITS(int_time);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_sc285sl_again_index[SC285SL_AGAIN_INDEX_MAX - 1]) {
        *again_lin = g_sc285sl_again_index[SC285SL_AGAIN_INDEX_MAX - 1];
        *again_db  = g_sc285sl_again_table[SC285SL_AGAIN_INDEX_MAX - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < SC285SL_AGAIN_INDEX_MAX; i++) {
        if (*again_lin < g_sc285sl_again_index[i]) {
            *again_lin = g_sc285sl_again_index[i - 1];
            *again_db  = g_sc285sl_again_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(dgain_lin);
    SENSOR_CHECK_PTR_RETURN(dgain_db);

    if (*dgain_lin >= g_sc285sl_dgain_index[SC285SL_DGAIN_INDEX_MAX - 1]) {
        *dgain_lin = g_sc285sl_dgain_index[SC285SL_DGAIN_INDEX_MAX - 1];
        *dgain_db  = g_sc285sl_dgain_table[SC285SL_DGAIN_INDEX_MAX - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; SC285SL_DGAIN_INDEX_MAX; i++) {
        if (*dgain_lin < g_sc285sl_dgain_index[i]) {
            *dgain_lin = g_sc285sl_dgain_index[i - 1];
            *dgain_db  = g_sc285sl_dgain_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_DGAIN_H].data = SENSOR_HIGH_8BITS(dgain);
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_DGAIN_L].data = SENSOR_LOW_8BITS(dgain);
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_AGAIN_H].data = SENSOR_HIGH_8BITS(again);
    sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC285SL_REG_AGAIN_L].data = SENSOR_LOW_8BITS(again);

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc285sl_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 sc285sl_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = SC285SL_CALIBRATE_STATIC_TEMP;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = SC285SL_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = SC285SL_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = SC285SL_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = SC285SL_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 5: point index on the awb curve param
    awb_sns_dft->wb_para[0] = SC285SL_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1] = SC285SL_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2] = SC285SL_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3] = SC285SL_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4] = SC285SL_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5] = SC285SL_CALIBRATE_AWB_C1;

    awb_sns_dft->golden_rgain    = SC285SL_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain    = SC285SL_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (sc285sl_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_sc285sl_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_sc285sl_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_sc285sl_awb_wdr_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_sc285sl_awb_wdr_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", sc285sl_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_sc285sl_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_sc285sl_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_sc285sl_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_sc285sl_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_sc285sl_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_standby(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc285sl_write_reg(dev, SC285SL_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_resume(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc285sl_write_reg(dev, SC285SL_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *sc285sl_ctx  = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= sc285sl_write_reg(dev, sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                 sc285sl_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    if (sc285sl_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = sc285sl_init_image(dev, sc285sl_ctx->img_mode, sc285sl_ctx->work_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = sc285sl_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc285sl_set_mirror_flip(dev, sc285sl_ctx->mirror_en, sc285sl_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc285sl_set_stream_mode(dev, XMEDIA_TRUE);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc285sl_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc285sl_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;
    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_sc285sl_dev_map[index] == SENSOR_DEV_INVALID) {
            g_sc285sl_dev_map[index] = pipe;
            *dev                     = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 sc285sl_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_sc285sl_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 sc285sl_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, SC285SL_NAME, sizeof(SC285SL_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property   = sc285sl_get_property;
    info->isp_func.pfn_sensor_set_work_mode  = sc285sl_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes = sc285sl_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror         = sc285sl_mirror;
    info->isp_func.pfn_sensor_flip           = sc285sl_flip;
    info->isp_func.pfn_sensor_set_bus_info   = sc285sl_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr   = sc285sl_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param = sc285sl_set_init_param;
    info->isp_func.pfn_sensor_start          = sc285sl_start;
    info->isp_func.pfn_sensor_stop           = sc285sl_stop;
    info->isp_func.pfn_sensor_standby        = sc285sl_standby;
    info->isp_func.pfn_sensor_resume         = sc285sl_resume;
    info->isp_func.pfn_sensor_write_reg      = sc285sl_write_reg;
    info->isp_func.pfn_sensor_read_reg       = sc285sl_read_reg;
    info->isp_func.pfn_sensor_init           = sc285sl_init;
    info->isp_func.pfn_sensor_exit           = sc285sl_exit;
    info->isp_func.pfn_sensor_set_attr       = sc285sl_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = sc285sl_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = sc285sl_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = sc285sl_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = sc285sl_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = sc285sl_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = sc285sl_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = sc285sl_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = sc285sl_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = sc285sl_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = sc285sl_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = sc285sl_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = sc285sl_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = sc285sl_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = sc285sl_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = sc285sl_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = sc285sl_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = sc285sl_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc285sl_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = sc285sl_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc285sl_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = sc285sl_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "SC285SL register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc285sl_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *sc285sl_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = sc285sl_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    SC285SL_GET_CONTEXT(dev, sc285sl_ctx);
    SENSOR_CHECK_PTR_RETURN(sc285sl_ctx);

    if (sc285sl_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "SC285SL unregister function failed!\n");
        return ret;
    }

    sc285sl_ctx_exit(dev);
    g_sc285sl_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;
}
