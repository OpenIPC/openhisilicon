#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sc200ai.h"
#include "sc200ai_ctrl.h"
#include "sc200ai_ex.h"

#ifdef FPGA
#define SC200AI_INPUT_CLOCK_LINEAR   XMEDIA_SENSOR_CLOCK_FREQ_24MHZ
#define SC200AI_INPUT_CLOCK_WDR      XMEDIA_SENSOR_CLOCK_FREQ_24MHZ
#define SC200AI_1M_MAX_FPS_LINEAR    21.548
#define SC200AI_2M_MAX_FPS_LINEAR    12.929
#define SC200AI_1M_MAX_FPS_WDR       6.459
#define SC200AI_2M_MAX_FPS_WDR       6.459
#define SC200AI_1M_BIT_RATE_LINEAR   600
#define SC200AI_2M_BIT_RATE_LINEAR   600
#define SC200AI_1M_BIT_RATE_WDR      600
#define SC200AI_2M_BIT_RATE_WDR      600
#else
#define SC200AI_INPUT_CLOCK_LINEAR   XMEDIA_SENSOR_CLOCK_FREQ_27MHZ
#define SC200AI_INPUT_CLOCK_WDR      XMEDIA_SENSOR_CLOCK_FREQ_27MHZ
#define SC200AI_1M_MAX_FPS_LINEAR    0.0
#define SC200AI_2M_MAX_FPS_LINEAR    30
#define SC200AI_1M_MAX_FPS_WDR       0.0 //not support
#define SC200AI_2M_MAX_FPS_WDR       30
#define SC200AI_1M_BIT_RATE_LINEAR   400
#define SC200AI_2M_BIT_RATE_LINEAR   400
#define SC200AI_1M_BIT_RATE_WDR      800
#define SC200AI_2M_BIT_RATE_WDR      800
#endif

#define SC200AI_NAME          "sc200ai"
#define SC200AI_SPECS_MAX_NUM 4

#ifdef __linux__
//#define SC200AI_ISP_DEFAULT_SUPPORT
#endif

#define SC200AI_REG_ADDR_STANDBY 0x0100

#define SC200AI_RES_IS_720P(w, h)  ((w) == 1280 && (h) == 720)
#define SC200AI_RES_IS_1080P(w, h) ((w) == 1920 && (h) == 1080)

#define SC200AI_AGAIN_INDEX_MAX 365

#define SC200AI_ERR_MODE_PRINT(sensor_image_mode)                                                                      \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sensor_image_mode->width,       \
                     sensor_image_mode->height, sensor_image_mode->wdr_mode);                                          \
    } while (0)

static sensor_context *g_sc200ai_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define SC200AI_GET_CTX(dev, ctx) ctx = g_sc200ai_ctx[dev]
#define SC200AI_SET_CTX(dev, ctx) g_sc200ai_ctx[dev] = ctx

static xmedia_s32 g_sc200ai_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] =
                                                                       SENSOR_DEV_INVALID };

static xmedia_u32 g_sc200ai_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_sc200ai_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_sc200ai_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_sc200ai_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

static const xmedia_sensor_property g_sc200ai_property[SC200AI_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        1280,  720, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, SC200AI_1M_MAX_FPS_LINEAR, SC200AI_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC200AI_1M_BIT_RATE_LINEAR,
    },

    {
        // width, height, wdr_mode
        1280,  720, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, SC200AI_1M_MAX_FPS_WDR, SC200AI_INPUT_CLOCK_WDR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, SC200AI_1M_BIT_RATE_WDR,
    },

    {
        // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, SC200AI_2M_MAX_FPS_LINEAR, SC200AI_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC200AI_2M_BIT_RATE_LINEAR,
    },

    {   // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, SC200AI_2M_MAX_FPS_WDR, SC200AI_INPUT_CLOCK_WDR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, SC200AI_2M_BIT_RATE_WDR,
    },
};

static const xmedia_u32 g_sc200ai_again_index[SC200AI_AGAIN_INDEX_MAX] = {
    1024,  1040,  1056,  1072,  1089,  1104,  1120,  1136,  1152,  1168,  1184,  1200,  1217,  1232,  1248,  1264,
    1280,  1296,  1312,  1328,  1345,  1360,  1376,  1392,  1408,  1424,  1440,  1456,  1473,  1488,  1504,  1520,
    1536,  1552,  1568,  1584,  1601,  1616,  1632,  1648,  1664,  1680,  1696,  1712,  1729,  1744,  1760,  1776,
    1792,  1808,  1824,  1840,  1857,  1872,  1888,  1904,  1920,  1936,  1952,  1968,  1985,  2000,  2016,  2032,
    2048,  2080,  2113,  2144,  2176,  2208,  2241,  2272,  2304,  2336,  2369,  2400,  2432,  2464,  2497,  2528,
    2560,  2592,  2625,  2656,  2688,  2720,  2753,  2784,  2816,  2848,  2881,  2912,  2944,  2976,  3009,  3040,
    3072,  3104,  3137,  3168,  3200,  3232,  3265,  3296,  3328,  3360,  3393,  3424,  3456,  3482,  3536,  3590,
    3644,  3700,  3754,  3808,  3863,  3917,  3971,  4025,  4080,  4135,  4189,  4243,  4298,  4352,  4406,  4461,
    4515,  4570,  4624,  4679,  4733,  4787,  4841,  4896,  4950,  5005,  5060,  5114,  5168,  5222,  5277,  5331,
    5385,  5441,  5495,  5549,  5603,  5658,  5712,  5766,  5820,  5876,  5930,  5984,  6039,  6093,  6147,  6201,
    6256,  6311,  6365,  6419,  6474,  6528,  6582,  6637,  6691,  6746,  6800,  6855,  6909,  6963,  7072,  7181,
    7290,  7398,  7507,  7617,  7725,  7834,  7942,  8052,  8160,  8269,  8377,  8487,  8595,  8704,  8813,  8922,
    9031,  9139,  9248,  9357,  9466,  9574,  9683,  9793,  9901,  10010, 10118, 10228, 10336, 10445, 10553, 10663,
    10771, 10880, 10989, 11098, 11207, 11315, 11424, 11533, 11642, 11750, 11859, 11969, 12077, 12186, 12294, 12404,
    12512, 12621, 12729, 12839, 12947, 13056, 13165, 13274, 13383, 13491, 13600, 13709, 13818, 13926, 14145, 14362,
    14580, 14797, 15015, 15232, 15450, 15667, 15885, 16102, 16321, 16538, 16756, 16973, 17191, 17408, 17626, 17843,
    18061, 18278, 18497, 18714, 18932, 19149, 19367, 19584, 19802, 20019, 20237, 20454, 20673, 20890, 21108, 21325,
    21543, 21760, 21978, 22195, 22413, 22630, 22849, 23066, 23284, 23501, 23719, 23936, 24154, 24371, 24589, 24806,
    25025, 25242, 25460, 25677, 25895, 26112, 26330, 26547, 26765, 26982, 27201, 27418, 27636, 27853, 28288, 28723,
    29158, 29594, 30029, 30464, 30899, 31334, 31770, 32205, 32640, 33075, 33510, 33946, 34381, 34816, 35251, 35686,
    36122, 36557, 36992, 37427, 37862, 38298, 38733, 39168, 39603, 40038, 40474, 40909, 41344, 41779, 42214, 42650,
    43085, 43520, 43955, 44390, 44826, 45261, 45696, 46131, 46566, 47002, 47437, 47872, 48307, 48742, 49178, 49613,
    50048, 50483, 50918, 51354, 51789, 52224, 52659, 53094, 53530, 53965, 54400, 54835, 55270,
};

static const xmedia_u32 g_sc200ai_again_table[SC200AI_AGAIN_INDEX_MAX] = {
    0x0340, 0x0341, 0x0342, 0x0343, 0x0344, 0x0345, 0x0346, 0x0347, 0x0348, 0x0349, 0x034A, 0x034B, 0x034C, 0x034D,
    0x034E, 0x034F, 0x0350, 0x0351, 0x0352, 0x0353, 0x0354, 0x0355, 0x0356, 0x0357, 0x0358, 0x0359, 0x035A, 0x035B,
    0x035C, 0x035D, 0x035E, 0x035F, 0x0360, 0x0361, 0x0362, 0x0363, 0x0364, 0x0365, 0x0366, 0x0367, 0x0368, 0x0369,
    0x036A, 0x036B, 0x036C, 0x036D, 0x036E, 0x036F, 0x0370, 0x0371, 0x0372, 0x0373, 0x0374, 0x0375, 0x0376, 0x0377,
    0x0378, 0x0379, 0x037A, 0x037B, 0x037C, 0x037D, 0x037E, 0x037F, 0x0740, 0x0741, 0x0742, 0x0743, 0x0744, 0x0745,
    0x0746, 0x0747, 0x0748, 0x0749, 0x074A, 0x074B, 0x074C, 0x074D, 0x074E, 0x074F, 0x0750, 0x0751, 0x0752, 0x0753,
    0x0754, 0x0755, 0x0756, 0x0757, 0x0758, 0x0759, 0x075A, 0x075B, 0x075C, 0x075D, 0x075E, 0x075F, 0x0760, 0x0761,
    0x0762, 0x0763, 0x0764, 0x0765, 0x0766, 0x0767, 0x0768, 0x0769, 0x076A, 0x076B, 0x076C, 0x2340, 0x2341, 0x2342,
    0x2343, 0x2344, 0x2345, 0x2346, 0x2347, 0x2348, 0x2349, 0x234A, 0x234B, 0x234C, 0x234D, 0x234E, 0x234F, 0x2350,
    0x2351, 0x2352, 0x2353, 0x2354, 0x2355, 0x2356, 0x2357, 0x2358, 0x2359, 0x235A, 0x235B, 0x235C, 0x235D, 0x235E,
    0x235F, 0x2360, 0x2361, 0x2362, 0x2363, 0x2364, 0x2365, 0x2366, 0x2367, 0x2368, 0x2369, 0x236A, 0x236B, 0x236C,
    0x236D, 0x236E, 0x236F, 0x2370, 0x2371, 0x2372, 0x2373, 0x2374, 0x2375, 0x2376, 0x2377, 0x2378, 0x2379, 0x237A,
    0x237B, 0x237C, 0x237D, 0x237E, 0x237F, 0x2740, 0x2741, 0x2742, 0x2743, 0x2744, 0x2745, 0x2746, 0x2747, 0x2748,
    0x2749, 0x274A, 0x274B, 0x274C, 0x274D, 0x274E, 0x274F, 0x2750, 0x2751, 0x2752, 0x2753, 0x2754, 0x2755, 0x2756,
    0x2757, 0x2758, 0x2759, 0x275A, 0x275B, 0x275C, 0x275D, 0x275E, 0x275F, 0x2760, 0x2761, 0x2762, 0x2763, 0x2764,
    0x2765, 0x2766, 0x2767, 0x2768, 0x2769, 0x276A, 0x276B, 0x276C, 0x276D, 0x276E, 0x276F, 0x2770, 0x2771, 0x2772,
    0x2773, 0x2774, 0x2775, 0x2776, 0x2777, 0x2778, 0x2779, 0x277A, 0x277B, 0x277C, 0x277D, 0x277E, 0x277F, 0x2F40,
    0x2F41, 0x2F42, 0x2F43, 0x2F44, 0x2F45, 0x2F46, 0x2F47, 0x2F48, 0x2F49, 0x2F4A, 0x2F4B, 0x2F4C, 0x2F4D, 0x2F4E,
    0x2F4F, 0x2F50, 0x2F51, 0x2F52, 0x2F53, 0x2F54, 0x2F55, 0x2F56, 0x2F57, 0x2F58, 0x2F59, 0x2F5A, 0x2F5B, 0x2F5C,
    0x2F5D, 0x2F5E, 0x2F5F, 0x2F60, 0x2F61, 0x2F62, 0x2F63, 0x2F64, 0x2F65, 0x2F66, 0x2F67, 0x2F68, 0x2F69, 0x2F6A,
    0x2F6B, 0x2F6C, 0x2F6D, 0x2F6E, 0x2F6F, 0x2F70, 0x2F71, 0x2F72, 0x2F73, 0x2F74, 0x2F75, 0x2F76, 0x2F77, 0x2F78,
    0x2F79, 0x2F7A, 0x2F7B, 0x2F7C, 0x2F7D, 0x2F7E, 0x2F7F, 0x3F40, 0x3F41, 0x3F42, 0x3F43, 0x3F44, 0x3F45, 0x3F46,
    0x3F47, 0x3F48, 0x3F49, 0x3F4A, 0x3F4B, 0x3F4C, 0x3F4D, 0x3F4E, 0x3F4F, 0x3F50, 0x3F51, 0x3F52, 0x3F53, 0x3F54,
    0x3F55, 0x3F56, 0x3F57, 0x3F58, 0x3F59, 0x3F5A, 0x3F5B, 0x3F5C, 0x3F5D, 0x3F5E, 0x3F5F, 0x3F60, 0x3F61, 0x3F62,
    0x3F63, 0x3F64, 0x3F65, 0x3F66, 0x3F67, 0x3F68, 0x3F69, 0x3F6A, 0x3F6B, 0x3F6C, 0x3F6D, 0x3F6E, 0x3F6F, 0x3F70,
    0x3F71, 0x3F72, 0x3F73, 0x3F74, 0x3F75, 0x3F76, 0x3F77, 0x3F78, 0x3F79, 0x3F7A, 0x3F7B, 0x3F7C, 0x3F7D, 0x3F7E,
    0x3F7F,
};

#define SC200AI_DGAIN_INDEX_MAX 320
static const xmedia_u32 g_sc200ai_dgain_index[SC200AI_DGAIN_INDEX_MAX] = {
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
    16384, 16640, 16896, 17152, 17408, 17664, 17920, 18176, 18432, 18688, 18944, 19200, 19456, 19712, 19968, 20224,
    20480, 20736, 20992, 21248, 21504, 21760, 22016, 22272, 22528, 22784, 23040, 23296, 23552, 23808, 24064, 24320,
    24576, 24832, 25088, 25344, 25600, 25856, 26112, 26368, 26624, 26880, 27136, 27392, 27648, 27904, 28160, 28416,
    28672, 28928, 29184, 29440, 29696, 29952, 30208, 30464, 30720, 30976, 31232, 31488, 31744, 32000, 32256, 32512,
};

static const xmedia_u32 g_sc200ai_dgain_table[SC200AI_DGAIN_INDEX_MAX] = {
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
    0x07f8, 0x07fa, 0x07fc, 0x07fe, 0x0f80, 0x0f82, 0x0f84, 0x0f86, 0x0f88, 0x0f8a, 0x0f8c, 0x0f8e, 0x0f90, 0x0f92,
    0x0f94, 0x0f96, 0x0f98, 0x0f9a, 0x0f9c, 0x0f9e, 0x0fa0, 0x0fa2, 0x0fa4, 0x0fa6, 0x0fa8, 0x0faa, 0x0fac, 0x0fae,
    0x0fb0, 0x0fb2, 0x0fb4, 0x0fb6, 0x0fb8, 0x0fba, 0x0fbc, 0x0fbe, 0x0fc0, 0x0fc2, 0x0fc4, 0x0fc6, 0x0fc8, 0x0fca,
    0x0fcc, 0x0fce, 0x0fd0, 0x0fd2, 0x0fd4, 0x0fd6, 0x0fd8, 0x0fda, 0x0fdc, 0x0fde, 0x0fe0, 0x0fe2, 0x0fe4, 0x0fe6,
    0x0fe8, 0x0fea, 0x0fec, 0x0fee, 0x0ff0, 0x0ff2, 0x0ff4, 0x0ff6, 0x0ff8, 0x0ffa, 0x0ffc, 0x0ffe,
};

static const xmedia_sensor_capability g_sc200ai_capability = {
    .max_width  = 1920,
    .max_height = 1080,
    .max_fps    = 21.548,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_2L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = SC200AI_ADDR_BYTE,
        .data_byte_num     = SC200AI_DATA_BYTE,
        .standby_reg_num   = 1,// TODO: 待后续确认
        .standby_reg_addr  = { SC200AI_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,// TODO: 待后续确认
        .resume_reg_addr   = { SC200AI_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x01 },
    },
};

static xmedia_s32 sc200ai_ctx_init(xmedia_u32 dev)
{
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SC200AI_GET_CTX(dev, sc200ai_ctx);
    if (sc200ai_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    sc200ai_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (sc200ai_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(sc200ai_ctx, 0, sizeof(sensor_context));
    sc200ai_ctx->i2c_addr             = SC200AI_I2C_ADDR;
    sc200ai_ctx->lanes                = XMEDIA_SENSOR_MIPI_LANES_2L;
    sc200ai_ctx->work_mode            = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    sc200ai_ctx->bus_info.i2c_dev     = SENSOR_I2C_INVALID;
    sc200ai_ctx->size.width           = g_sc200ai_property[0].width;
    sc200ai_ctx->size.height          = g_sc200ai_property[0].height;
    sc200ai_ctx->fps                  = g_sc200ai_property[0].max_fps;
    sc200ai_ctx->wdr_mode             = g_sc200ai_property[0].wdr_mode;
    sc200ai_ctx->img_mode             = SC200AI_1M_20FPS_10BIT_LINEAR_MODE;
    sc200ai_ctx->fl_std               = SC200AI_VMAX_1M_LINEAR << SC200AI_EXP_SHIFT;
    sc200ai_ctx->fl[SENSOR_CUR_FRAME] = SC200AI_VMAX_1M_LINEAR << SC200AI_EXP_SHIFT;
    sc200ai_ctx->fl[SENSOR_PRE_FRAME] = SC200AI_VMAX_1M_LINEAR << SC200AI_EXP_SHIFT;
    SC200AI_SET_CTX(dev, sc200ai_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void sc200ai_ctx_exit(xmedia_u32 dev)
{
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_FREE(sc200ai_ctx);
    SC200AI_SET_CTX(dev, XMEDIA_NULL);

    return;
}

static xmedia_s32 sc200ai_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    for (i = 0; i < SC200AI_SPECS_MAX_NUM; i++) {
        if (g_sc200ai_property[i].width == sc200ai_ctx->size.width &&
            g_sc200ai_property[i].height == sc200ai_ctx->size.height &&
            g_sc200ai_property[i].wdr_mode == sc200ai_ctx->wdr_mode) {
            memcpy(property, &g_sc200ai_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Please check input param!\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 sc200ai_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc200ai_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Invalid mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc200ai_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        sc200ai_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        sc200ai_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        sc200ai_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 sc200ai_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    sc200ai_ctx->i2c_addr = slave_addr;
    ret = sc200ai_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    *slave_addr = sc200ai_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

// called by ISP
static xmedia_s32 sc200ai_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    sc200ai_ctx->init_mode = init_mode;
    ret = sc200ai_i2c_init(dev, sc200ai_ctx->bus_info.i2c_dev, sc200ai_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    sc200ai_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    ret = sc200ai_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    sc200ai_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    sc200ai_ctx->mirror_en = mirror_en;
    ret = sc200ai_set_mirror_flip(dev, mirror_en, sc200ai_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    sc200ai_ctx->flip_en = flip_en;
    ret = sc200ai_set_mirror_flip(dev, sc200ai_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_set_image_mode(sensor_context *sc200ai_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (sc200ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (SC200AI_RES_IS_720P(sns_attr->width, sns_attr->height)) {
            *image_mode         = SC200AI_1M_20FPS_10BIT_LINEAR_MODE;
            sc200ai_ctx->fl_std = SC200AI_VMAX_1M_LINEAR << SC200AI_EXP_SHIFT;
        } else if (SC200AI_RES_IS_1080P(sns_attr->width, sns_attr->height)) {
            *image_mode         = SC200AI_2M_12FPS_10BIT_LINEAR_MODE;
            sc200ai_ctx->fl_std = SC200AI_VMAX_2M_LINEAR << SC200AI_EXP_SHIFT;
        } else {
            SC200AI_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else if (sc200ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (SC200AI_RES_IS_720P(sns_attr->width, sns_attr->height)) {
            *image_mode                = SC200AI_1M_6FPS_10BIT_WDR_MODE;
            sc200ai_ctx->fl_std        = SC200AI_VMAX_1M_WDR << SC200AI_EXP_SHIFT;
            sc200ai_ctx->max_short_exp = SC200AI_S_EXP_MAX_DEFAULT_720P << SC200AI_EXP_SHIFT;
        } else if(SC200AI_RES_IS_1080P(sns_attr->width, sns_attr->height)) {
            *image_mode                = SC200AI_2M_6FPS_10BIT_WDR_MODE;
            sc200ai_ctx->fl_std        = SC200AI_VMAX_2M_WDR << SC200AI_EXP_SHIFT;
            sc200ai_ctx->max_short_exp = SC200AI_S_EXP_MAX_DEFAULT_1080P << SC200AI_EXP_SHIFT;
        }
        else {
            SC200AI_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        SC200AI_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc200ai_ctx->size.width  = sns_attr->width;
    sc200ai_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            sc200ai_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            sc200ai_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
            SENSOR_PRINT("2to1 line WDR\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(sc200ai_ctx->wdr_int_time, 0, sizeof(sc200ai_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

#define SC200AI_LINEAR_REG_INFO_MAX_NUM   SC200AI_REG_L_MAX_NUM
#define SC200AI_2TO1_WDR_REG_INFO_MAX_NUM SC200AI_REG_MAX_NUM
static xmedia_void sc200ai_init_common_reg_info(sensor_context *sc200ai_ctx)
{
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_EXP_H].delay_frame_num = 2;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_EXP_H].reg_addr        = SC200AI_REG_ADDR_EXP_H;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_EXP_M].delay_frame_num = 2;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_EXP_M].reg_addr        = SC200AI_REG_ADDR_EXP_M;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_EXP_L].delay_frame_num = 2;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_EXP_L].reg_addr        = SC200AI_REG_ADDR_EXP_L;

    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_DGC_H].delay_frame_num = 2;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_DGC_H].reg_addr        = SC200AI_REG_ADDR_DGAIN_H;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_DGC_L].delay_frame_num = 2;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_DGC_L].reg_addr        = SC200AI_REG_ADDR_DGAIN_L;

    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_AGC_H].delay_frame_num = 2;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_AGC_H].reg_addr        = SC200AI_REG_ADDR_AGAIN_H;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_AGC_L].delay_frame_num = 2;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_AGC_L].reg_addr        = SC200AI_REG_ADDR_AGAIN_L;

    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_VMAX_H].delay_frame_num = 2;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_VMAX_H].reg_addr        = SC200AI_REG_ADDR_VMAX_H;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_VMAX_L].delay_frame_num = 2;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_VMAX_L].reg_addr        = SC200AI_REG_ADDR_VMAX_L;

    return;
}

static xmedia_void sc200ai_init_2to1_wdr_reg_info(sensor_context *sc200ai_ctx)
{
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_EXP_H].delay_frame_num = 2;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_EXP_H].reg_addr        = SC200AI_REG_ADDR_S_EXP_H;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_EXP_M].delay_frame_num = 2;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_EXP_M].reg_addr        = SC200AI_REG_ADDR_S_EXP_M;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_EXP_L].delay_frame_num = 2;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_EXP_L].reg_addr        = SC200AI_REG_ADDR_S_EXP_L;

    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_DGC_H].delay_frame_num = 2;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_DGC_H].reg_addr        = SC200AI_REG_ADDR_S_DGAIN_H;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_DGC_L].delay_frame_num = 2;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_DGC_L].reg_addr        = SC200AI_REG_ADDR_S_DGAIN_L;

    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_AGC_H].delay_frame_num = 2;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_AGC_H].reg_addr        = SC200AI_REG_ADDR_S_AGAIN_H;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_AGC_L].delay_frame_num = 2;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_AGC_L].reg_addr        = SC200AI_REG_ADDR_S_AGAIN_L;

    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_EXP_MAX_H].delay_frame_num = 0;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_EXP_MAX_H].reg_addr = SC200AI_REG_ADDR_S_EXP_MAX_H;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_EXP_MAX_L].delay_frame_num = 0;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_EXP_MAX_L].reg_addr = SC200AI_REG_ADDR_S_EXP_MAX_L;

    return;
}

static xmedia_s32 sc200ai_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = sc200ai_ctx->bus_info.i2c_dev;
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = SC200AI_LINEAR_REG_INFO_MAX_NUM;

    if (sc200ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num = SC200AI_2TO1_WDR_REG_INFO_MAX_NUM;
    }

    for (i = 0; i < sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = sc200ai_ctx->i2c_addr;
        sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = SC200AI_ADDR_BYTE;
        sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = SC200AI_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    sc200ai_init_common_reg_info(sc200ai_ctx);

    // init 2to1 wdr mode Regs
    if (sc200ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc200ai_init_2to1_wdr_reg_info(sc200ai_ctx);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    // Set wdr mode
    ret = sc200ai_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = sc200ai_set_image_mode(sc200ai_ctx, &sc200ai_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    sc200ai_ctx->fl[SENSOR_CUR_FRAME] = sc200ai_ctx->fl_std;
    sc200ai_ctx->fl[SENSOR_PRE_FRAME] = sc200ai_ctx->fl[SENSOR_CUR_FRAME];

    ret = sc200ai_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));

#ifdef SC200AI_ISP_DEFAULT_SUPPORT
    // TO-DO: alg add param
    isp_default->blc         = &g_sc200ai_blc;
    isp_default->bnr         = &g_sc200ai_bnr;
    isp_default->clut_attr   = XMEDIA_NULL;
    isp_default->crosstalk   = &g_sc200ai_crosstalk;
    isp_default->csc         = &g_sc200ai_csc;
    isp_default->dehaze      = &g_sc200ai_dehaze;
    isp_default->demosaic    = &g_sc200ai_dms;
    isp_default->dpc_dynamic = &g_sc200ai_dpc;
    isp_default->dpc_static  = XMEDIA_NULL;
    isp_default->drc         = &g_sc200ai_drc;
    isp_default->drc         = XMEDIA_NULL;
    isp_default->expander    = XMEDIA_NULL;
    isp_default->fpn         = XMEDIA_NULL;
    isp_default->gamma       = &g_sc200ai_gamma;
    isp_default->gcac        = &g_sc200ai_gcac;
    isp_default->hlc         = XMEDIA_NULL;
    isp_default->lce         = &g_sc200ai_lce;
    isp_default->mlsc_attr   = XMEDIA_NULL;
    isp_default->mlsc_lut    = XMEDIA_NULL;
    isp_default->radial_crop = XMEDIA_NULL;
    isp_default->rgbir       = XMEDIA_NULL;
    isp_default->rlsc_attr   = XMEDIA_NULL;
    isp_default->rlsc_lut    = XMEDIA_NULL;
    // isp_default->sharpen     = XMEDIA_NULL;
    // isp_default->stnr        = XMEDIA_NULL;
    isp_default->wdr         = XMEDIA_NULL;
    return XMEDIA_SUCCESS;
#else
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from sc200ai!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 sc200ai_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (sc200ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
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

static xmedia_s32 sc200ai_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    if (sc200ai_ctx->mirror_en && sc200ai_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (sc200ai_ctx->mirror_en && (!sc200ai_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!sc200ai_ctx->mirror_en) && sc200ai_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < SC200AI_SPECS_MAX_NUM; i++) {
        if (g_sc200ai_property[i].width == sc200ai_ctx->size.width &&
            g_sc200ai_property[i].height == sc200ai_ctx->size.height &&
            g_sc200ai_property[i].wdr_mode == sc200ai_ctx->wdr_mode) {
            *bayer_pattern = g_sc200ai_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= SC200AI_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);
    memcpy(capability, &g_sc200ai_capability, sizeof(xmedia_sensor_capability));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc200ai_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 sc200ai_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_sc200ai_exposure[dev]      = init_param->exposure;
    g_sc200ai_sample_r_gain[dev] = init_param->sample_rgain;
    g_sc200ai_sample_b_gain[dev] = init_param->sample_bgain;

    g_sc200ai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_sc200ai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_sc200ai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case SC200AI_1M_20FPS_10BIT_LINEAR_MODE:
            *max_fps = SC200AI_1M_MAX_FPS_LINEAR;
            break;
        case SC200AI_2M_12FPS_10BIT_LINEAR_MODE:
            *max_fps = SC200AI_2M_MAX_FPS_LINEAR;
            break;

        case SC200AI_1M_6FPS_10BIT_WDR_MODE:
            *max_fps = SC200AI_1M_MAX_FPS_WDR;
            break;

        case SC200AI_2M_6FPS_10BIT_WDR_MODE:
            *max_fps = SC200AI_2M_MAX_FPS_WDR;
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = sc200ai_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50*256;
    ae_sns_dft->full_lines_max = SC200AI_FULL_LINES_MAX;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->dgain_accu.accuracy  = 1;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 4 << ae_sns_dft->ispdgain_shift;

    memcpy(&ae_sns_dft->piris_attr, &g_sc200ai_piris_attr, sizeof(xmedia_isp_piris_attr));
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

static xmedia_void sc200ai_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 55270; // max:53.975 * 1024
    ae_sns_dft->min_again        = 1024;  // min: 1 * 1024
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 32512; // max: 31.750 * 1024
    ae_sns_dft->min_dgain        = 1024;  // min: 1 * 1024
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x38;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_sc200ai_exposure[dev] ? g_sc200ai_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - SC200AI_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 1;     // min 1
    ae_sns_dft->max_int_time_target = 65535; // max 65535
    ae_sns_dft->min_int_time_target = 1;     // min 1

    return;
}

static xmedia_s32 sc200ai_get_ae_2to1_wdr_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                 xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 4;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 55270; // max:53.975 * 1024
    ae_sns_dft->min_again        = 1024;  // 1 * 1024
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 32512; // max: 31.750 * 1024
    ae_sns_dft->min_dgain        = 1024;  // 1 * 1024
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;
    ae_sns_dft->init_exposure    = g_sc200ai_exposure[dev] ? g_sc200ai_exposure[dev] : 16462;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - sns_ctx->max_short_exp - SC200AI_EXP_OFFSET_WDR;
    ae_sns_dft->min_int_time        = 8;     // min 1
    ae_sns_dft->max_int_time_target = 65535; // max 65535
    ae_sns_dft->min_int_time_target = 8;     // min 1

    if (sc200ai_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
        ae_sns_dft->ae_compensation = 0x30;
        ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    } else {
        //ae_sns_dft->max_dgain_target    = 1024;
        ae_sns_dft->max_ispdgain_target = 4096;
        ae_sns_dft->ae_compensation     = 0x40;
        ae_sns_dft->ae_exp_mode         = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
        ae_sns_dft->man_ratio_enable    = XMEDIA_TRUE;

        /*
         * exp ratio array index( 4to1 wdr):
         * 0: short / very shourt;
         * 1: middle / short;
         * 2: long / middle
         * 2to1 line wdr：ratio[0] valid
         */
        ae_sns_dft->ratio[0] = 0x400;
        ae_sns_dft->ratio[1] = 0x40;
        ae_sns_dft->ratio[2] = 0x40;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = sc200ai_get_ae_common_default(dev, sc200ai_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (sc200ai_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            sc200ai_get_ae_linear_default(dev, sc200ai_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            sc200ai_get_ae_2to1_wdr_default(dev, sc200ai_ctx, ae_sns_dft);
            break;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context    *sc200ai_ctx                                = XMEDIA_NULL;
    static xmedia_bool first[XMEDIA_SENSOR_DEV_MAX_NUM]          = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 1 };
    static xmedia_u32  short_int_time[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
    static xmedia_u32  long_int_time[XMEDIA_SENSOR_DEV_MAX_NUM]  = { 0 };

    SENSOR_CHECK_DEV_RETURN(dev);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    if (sc200ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (first[dev]) { // 0: short exposure
            sc200ai_ctx->wdr_int_time[0] = int_time;
            short_int_time[dev]          = int_time;
            sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_EXP_H].data =
                SENSOR_HIGHER_4BITS(short_int_time[dev]);
            sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_EXP_M].data =
                SENSOR_MIDDLE_8BITS(short_int_time[dev]);
            sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_EXP_L].data =
                SENSOR_LOWER_4BITS(short_int_time[dev]);
            first[dev] = XMEDIA_FALSE;
        } else { // 1: long exposure
            sc200ai_ctx->wdr_int_time[1] = int_time;
            long_int_time[dev]           = int_time;
            sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_EXP_H].data =
                SENSOR_HIGHER_4BITS(long_int_time[dev]);
            sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_EXP_M].data =
                SENSOR_MIDDLE_8BITS(long_int_time[dev]);
            sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_EXP_L].data =
                SENSOR_LOWER_4BITS(long_int_time[dev]);
            first[dev] = XMEDIA_TRUE;
        }
    } else if (sc200ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_EXP_H].data = SENSOR_HIGHER_4BITS(int_time);
        sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_EXP_M].data = SENSOR_MIDDLE_8BITS(int_time);
        sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_EXP_L].data = SENSOR_LOWER_4BITS(int_time);
        first[dev]                                                                = XMEDIA_TRUE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_sc200ai_again_index[SC200AI_AGAIN_INDEX_MAX - 1]) {
        *again_lin  = g_sc200ai_again_index[SC200AI_AGAIN_INDEX_MAX - 1];
        *again_db   = g_sc200ai_again_table[SC200AI_AGAIN_INDEX_MAX - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < SC200AI_AGAIN_INDEX_MAX; i++) {
        if (*again_lin < g_sc200ai_again_index[i]) {
            *again_lin = g_sc200ai_again_index[i - 1];
            *again_db  = g_sc200ai_again_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(dgain_lin);
    SENSOR_CHECK_PTR_RETURN(dgain_db);

    if (*dgain_lin >= g_sc200ai_dgain_index[SC200AI_DGAIN_INDEX_MAX - 1]) {
        *dgain_lin  = g_sc200ai_dgain_index[SC200AI_DGAIN_INDEX_MAX - 1];
        *dgain_db   = g_sc200ai_dgain_table[SC200AI_DGAIN_INDEX_MAX - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < SC200AI_DGAIN_INDEX_MAX; i++) {
        if (*dgain_lin < g_sc200ai_dgain_index[i]) {
            *dgain_lin = g_sc200ai_dgain_index[i - 1];
            *dgain_db  = g_sc200ai_dgain_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_DGC_H].data = SENSOR_HIGH_8BITS(dgain);
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_DGC_L].data = SENSOR_LOW_8BITS(dgain);
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_AGC_H].data = SENSOR_HIGH_8BITS(again);
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_AGC_L].data = SENSOR_LOW_8BITS(again);

    if (sc200ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_DGC_H].data = SENSOR_HIGH_8BITS(dgain);
        sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_DGC_L].data = SENSOR_LOW_8BITS(dgain);
        sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_AGC_H].data = SENSOR_HIGH_8BITS(again);
        sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_AGC_L].data = SENSOR_LOW_8BITS(again);
    }

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 sc200ai_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = 4670;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = 441;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = 256;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = 256;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = 649;

    // 0 ~ 6: point index on the awb curve param
    awb_sns_dft->wb_para[0]   = -15;
    awb_sns_dft->wb_para[1]   = 229;
    awb_sns_dft->wb_para[2]   = -42;
    awb_sns_dft->wb_para[3]   = 138189;
    awb_sns_dft->wb_para[4]   = 128;
    awb_sns_dft->wb_para[5]   = -84042;
    awb_sns_dft->golden_rgain = 0;
    awb_sns_dft->golden_bgain = 0;
    awb_sns_dft->awb_runinterval = 2;
    switch (sc200ai_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_sc200ai_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_sc200ai_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_sc200ai_awb_wdr_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_sc200ai_awb_wdr_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
    }

    awb_sns_dft->init_rgain   = g_sc200ai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_sc200ai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_sc200ai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_sc200ai_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_sc200ai_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_standby(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc200ai_write_reg(dev, SC200AI_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_resume(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc200ai_write_reg(dev, SC200AI_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_void sc200ai_delay_ms(xmedia_s32 ms)
{
    usleep(ms * 1000);
}

static xmedia_s32 sc200ai_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *sc200ai_ctx  = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= sc200ai_write_reg(dev, sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_start(xmedia_u32 dev)
{
    xmedia_s32               ret;
    xmedia_u8                img_mode;
    xmedia_sensor_mipi_lanes mipi_lanes;
    xmedia_sensor_work_mode  work_mode;
    sensor_context          *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    work_mode  = sc200ai_ctx->work_mode;
    mipi_lanes = sc200ai_ctx->lanes;
    img_mode   = sc200ai_ctx->img_mode;

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL || mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check param!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (sc200ai_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = sc200ai_init_image(dev, img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    sc200ai_delay_ms(10);
    ret = sc200ai_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc200ai_set_mirror_flip(dev, sc200ai_ctx->mirror_en, sc200ai_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc200ai_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_sc200ai_dev_map[index] == SENSOR_DEV_INVALID) {
            g_sc200ai_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 sc200ai_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_sc200ai_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 sc200ai_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{ // TO-DO：min fps is to be confirmed.
    switch (img_mode) {
        case SC200AI_1M_20FPS_10BIT_LINEAR_MODE:
            *min_fps = 2;
            break;
        case SC200AI_2M_12FPS_10BIT_LINEAR_MODE:
            *min_fps = 2;
            break;

        case SC200AI_1M_6FPS_10BIT_WDR_MODE:
            *min_fps = 2;
            break;

        case SC200AI_2M_6FPS_10BIT_WDR_MODE:
            *min_fps = 2;
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case SC200AI_1M_20FPS_10BIT_LINEAR_MODE:
            *vmax = SC200AI_VMAX_1M_LINEAR;
            break;
        case SC200AI_2M_12FPS_10BIT_LINEAR_MODE:
            *vmax = SC200AI_VMAX_2M_LINEAR;
            break;

        case SC200AI_1M_6FPS_10BIT_WDR_MODE:
            *vmax = SC200AI_VMAX_1M_WDR;
            break;
        case SC200AI_2M_6FPS_10BIT_WDR_MODE:
            *vmax = SC200AI_VMAX_2M_WDR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = sc200ai_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc200ai_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc200ai_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is (%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, SC200AI_FULL_LINES_MAX);

    if (sns_ctx->img_mode == SC200AI_1M_6FPS_10BIT_WDR_MODE) {
        sns_ctx->max_short_exp = SC200AI_S_EXP_MAX_DEFAULT_720P * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    } else if (sns_ctx->img_mode == SC200AI_2M_6FPS_10BIT_WDR_MODE) {
        sns_ctx->max_short_exp = SC200AI_S_EXP_MAX_DEFAULT_1080P * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    for (i = 0; i < sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            sc200ai_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &sc200ai_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&sc200ai_ctx->regs_info[SENSOR_PRE_FRAME], &sc200ai_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    sc200ai_ctx->fl[SENSOR_PRE_FRAME] = sc200ai_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    ret = sc200ai_calc_fps(fps, sc200ai_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    sc200ai_ctx->regs_info[0].i2c_data[SC200AI_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    sc200ai_ctx->regs_info[0].i2c_data[SC200AI_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    sc200ai_ctx->fl[SENSOR_CUR_FRAME] = full_lines << SC200AI_EXP_SHIFT;
    sc200ai_ctx->fl_std               = sc200ai_ctx->fl[SENSOR_CUR_FRAME];
    sc200ai_ctx->fps                  = fps;
    ae_sns_dft->fps                   = fps;
    ae_sns_dft->full_lines_std        = sc200ai_ctx->fl_std;
    ae_sns_dft->full_lines            = sc200ai_ctx->fl[SENSOR_CUR_FRAME];

    if (sc200ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sc200ai_ctx->fl[SENSOR_CUR_FRAME] - SC200AI_EXP_OFFSET_LINEAR;
    } else if (sc200ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_EXP_MAX_H].data =
            SENSOR_HIGH_8BITS(sc200ai_ctx->max_short_exp);
        sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_EXP_MAX_L].data =
            SENSOR_LOW_8BITS(sc200ai_ctx->max_short_exp);
        sc200ai_ctx->max_short_exp = sc200ai_ctx->max_short_exp << SC200AI_EXP_SHIFT;
        ae_sns_dft->max_int_time = sc200ai_ctx->fl[SENSOR_CUR_FRAME] - sc200ai_ctx->max_short_exp - SC200AI_EXP_OFFSET_WDR;
    }

    SENSOR_PRINT("dev[%d]- sc200ai set fps: %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = sc200ai_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc200ai_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc200ai_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = (vmax_max_fps << SC200AI_EXP_SHIFT) * max_fps / min_fps;
    return XMEDIA_SUCCESS;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 sc200ai_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    xmedia_u32      ratio;
    xmedia_u32      vmax;
    xmedia_u32      short_frame_min_exp;
    xmedia_u32      short_frame_max_exp;
    xmedia_u32      long_frame_min_exp;
    xmedia_u32      long_frame_max_exp;
    xmedia_u32      reg_short_frame_max_exp;
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);
    memcpy(sc200ai_ctx->ratio, inttime_attr->ratio, sizeof(inttime_attr->ratio));

    short_frame_min_exp = 1;
    long_frame_min_exp  = 1;
    if (sc200ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (sc200ai_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
            inttime_attr->min_inttime[0] = short_frame_min_exp; // 0:short frame; 1: long frame
            inttime_attr->max_inttime[0] = short_frame_min_exp;

            inttime_attr->min_inttime[1] = long_frame_min_exp;

            // short_frame_max_exp = 2 * reg_short_frame_max_exp - 10
            // 2 * reg_short_frame_max_exp = short_frame_max_exp + 10
            reg_short_frame_max_exp = inttime_attr->min_inttime[0] + 10;

            // long_frame_max_exp = 2 * vmax -2 * reg_short_frame_exp - 10
            inttime_attr->max_inttime[1] = sc200ai_ctx->fl[SENSOR_CUR_FRAME] - reg_short_frame_max_exp - 10;
        } else {
            /*
             * 已知：曝光比ratio, vmax
             * 求解: 长帧最大曝光时间long_frame_max_exp， 短帧最大曝光时间short_frame_max_exp
             *       短帧最大曝光时间寄存器取值 reg_short_frame_max_exp
             * ratio = long_frame_max_exp / short_frame_max_exp * 64; // AE所给的曝光比, 64为1倍
             * long_frame_max_exp = 2 * vmax -2 * reg_short_frame_exp - 10; //10: long_exp_offset
             * short_frame_max_exp = 2 * reg_short_frame_max_exp - 10;     //10: s_exp_offset
             * 解得：
             * short_frame_max_exp = 64 * (2 * vmax - 20 ) / (64 + ratio )
             */
            ratio                   = SENSOR_DIV_0_TO_1(inttime_attr->ratio[0]);
            vmax                    = sc200ai_ctx->fl[SENSOR_CUR_FRAME]; // fl = 2 * vamx
            short_frame_max_exp     = 64 * (vmax - 20 ) / (64 + ratio);
            long_frame_max_exp      = short_frame_max_exp * ratio / 64;
            long_frame_min_exp      = short_frame_min_exp * ratio / 64;
            reg_short_frame_max_exp = (short_frame_max_exp + 10 ) / 2;

            // 0:short frame; 1: long frame
            inttime_attr->min_inttime[0] = short_frame_min_exp;
            inttime_attr->min_inttime[1] = long_frame_min_exp;
            inttime_attr->max_inttime[0] = short_frame_max_exp;
            inttime_attr->max_inttime[1] = long_frame_max_exp;
        }

        sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_EXP_MAX_H].data =
            SENSOR_HIGH_8BITS(reg_short_frame_max_exp);
        sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_S_EXP_MAX_L].data =
            SENSOR_LOW_8BITS(reg_short_frame_max_exp);
        sc200ai_ctx->max_short_exp = reg_short_frame_max_exp << SC200AI_EXP_SHIFT;
    }

    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 * 注意：2to1 wdr模式下，曝光比和ae wdr mode 由AE传入，调用本接口前，AE需先调用sc200ai_get_max_inttime
 */
static xmedia_s32 sc200ai_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context               *sc200ai_ctx = XMEDIA_NULL;
    xmedia_float                  min_fps;
    xmedia_u32                    vmax_min_fps;
    xmedia_s32                    ret;
    xmedia_sensor_ae_inttime_attr inttime_attr;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    if (sc200ai_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", sc200ai_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (sc200ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        full_lines = full_lines + SC200AI_EXP_OFFSET_LINEAR;
    } else if (sc200ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        full_lines = full_lines + SC200AI_EXP_OFFSET_WDR;
    }

    ret = sc200ai_get_min_fps(sc200ai_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc200ai_get_min_fps_vmax(sc200ai_ctx->img_mode, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                        = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    sc200ai_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    sc200ai_ctx->fps                  = min_fps * vmax_min_fps / full_lines;

    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_VMAX_H].data =
        SENSOR_HIGH_8BITS(full_lines >> SC200AI_EXP_SHIFT);
    sc200ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC200AI_REG_VMAX_L].data =
        SENSOR_LOW_8BITS(full_lines >> SC200AI_EXP_SHIFT);

    ae_sns_dft->full_lines = sc200ai_ctx->fl[SENSOR_CUR_FRAME];
    if (sc200ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sc200ai_ctx->fl[SENSOR_CUR_FRAME] - SC200AI_EXP_OFFSET_LINEAR;
    } else if (sc200ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        inttime_attr.ratio[0] = sc200ai_ctx->ratio[0]; // index 0: 长短帧曝光比
        sc200ai_get_max_inttime(dev, &inttime_attr);
        ae_sns_dft->max_int_time =
            sc200ai_ctx->fl[SENSOR_CUR_FRAME] - sc200ai_ctx->max_short_exp - SC200AI_EXP_OFFSET_WDR;
    }

    SENSOR_PRINT("dev[%d]- sc200ai set fps: %f\n", dev, sc200ai_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    ret = sc200ai_get_min_fps(sc200ai_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc200ai_get_max_fps(sc200ai_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = sc200ai_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc200ai_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, SC200AI_NAME, sizeof(SC200AI_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = sc200ai_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = sc200ai_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = sc200ai_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = sc200ai_mirror;
    info->isp_func.pfn_sensor_flip             = sc200ai_flip;
    info->isp_func.pfn_sensor_set_bus_info     = sc200ai_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = sc200ai_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = sc200ai_set_init_param;
    info->isp_func.pfn_sensor_start            = sc200ai_start;
    info->isp_func.pfn_sensor_stop             = sc200ai_stop;
    info->isp_func.pfn_sensor_standby          = sc200ai_standby;
    info->isp_func.pfn_sensor_resume           = sc200ai_resume;
    info->isp_func.pfn_sensor_write_reg        = sc200ai_write_reg;
    info->isp_func.pfn_sensor_read_reg         = sc200ai_read_reg;
    info->isp_func.pfn_sensor_init             = sc200ai_init;
    info->isp_func.pfn_sensor_exit             = sc200ai_exit;

    // called by ISP
    info->isp_func.pfn_sensor_set_attr            = sc200ai_set_attr;
    info->isp_func.pfn_sensor_get_isp_default     = sc200ai_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = sc200ai_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = sc200ai_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = sc200ai_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = sc200ai_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = sc200ai_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = sc200ai_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = sc200ai_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = sc200ai_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = sc200ai_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = sc200ai_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = sc200ai_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = sc200ai_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = sc200ai_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = sc200ai_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = sc200ai_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = sc200ai_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc200ai_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = sc200ai_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc200ai_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = sc200ai_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc200ai register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc200ai_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *sc200ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = sc200ai_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    SC200AI_GET_CTX(dev, sc200ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc200ai_ctx);

    if (sc200ai_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc200ai unregister function failed!\n");
        return ret;
    }

    sc200ai_ctx_exit(dev);
    g_sc200ai_dev_map[dev] = SENSOR_DEV_INVALID; // reset dev

    return XMEDIA_SUCCESS;
}
