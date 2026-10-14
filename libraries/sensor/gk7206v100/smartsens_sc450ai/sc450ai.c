#include <stdlib.h>
#include <string.h>
#include "sc450ai.h"
#include "sc450ai_ctrl.h"
#include "sc450ai_ex.h"

#define SC450AI_NAME          "SC450AI"
#define SC450AI_SPECS_MAX_NUM 4

#ifdef __linux__
#define SC450AI_ISP_DEFAULT_SUPPORT
#endif

#define SC450AI_4M_1440P_BIT_RATE_LINEAR  700
#define SC450AI_4M_1440P_BIT_RATE_WDR     720
#define SC450AI_4M_1520P_BIT_RATE_LINEAR  700
#define SC450AI_1M_760P_BIT_RATE_LINEAR   360

#define SC450AI_REG_ADDR_STANDBY 0x0100

#define SC450AI_RES_IS_1440P(w, h) ((w) == 2560 && (h) == 1440)
#define SC450AI_RES_IS_1520P(w, h) ((w) == 2688 && (h) == 1520)
#define SC450AI_RES_IS_760P(w, h)  ((w) == 1344 && (h) == 760)

#define SC450AI_ERR_MODE_PRINT(sns_attr)                                                                               \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sns_attr->width,                \
                     sns_attr->height, sns_attr->wdr_mode);                                                            \
    } while (0)

// awb static param for Fuji Lens New IR_Cut
#define SC450AI_CALIBRATE_STATIC_TEMP       5000
#define SC450AI_CALIBRATE_STATIC_WB_R_GAIN  433
#define SC450AI_CALIBRATE_STATIC_WB_GR_GAIN 256
#define SC450AI_CALIBRATE_STATIC_WB_GB_GAIN 256
#define SC450AI_CALIBRATE_STATIC_WB_B_GAIN  653

// Calibration results for Auto WB Planck
#define SC450AI_CALIBRATE_AWB_P1 165
#define SC450AI_CALIBRATE_AWB_P2 (-89)
#define SC450AI_CALIBRATE_AWB_Q1 (-177)
#define SC450AI_CALIBRATE_AWB_A1 200851
#define SC450AI_CALIBRATE_AWB_B1 128
#define SC450AI_CALIBRATE_AWB_C1 (-139601)

// Rgain and Bgain of the golden sample
#define SC450AI_GOLDEN_RGAIN 0
#define SC450AI_GOLDEN_BGAIN 0

#define SC450AI_AGAIN_INDEX_MAX 378
#define SC450AI_DGAIN_INDEX_MAX 256

SENSOR_PRIORITY_DATA sensor_context *g_sc450ai_ctx[XMEDIA_SENSOR_DEV_MAX_NUM]     = { XMEDIA_NULL };
#define SC450AI_SET_CTX(dev, sns_ctx) g_sc450ai_ctx[dev] = sns_ctx
#define SC450AI_GET_CTX(dev, sns_ctx) sns_ctx = g_sc450ai_ctx[dev]

static xmedia_u32 g_sc450ai_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_sc450ai_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_sc450ai_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_sc450ai_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };
SENSOR_PRIORITY_DATA static xmedia_s32 g_sc450ai_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

SENSOR_PRIORITY_RODATA static const xmedia_sensor_property g_sc450ai_property[SC450AI_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        2560, 1440, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC450AI_4M_1440P_BIT_RATE_LINEAR,
    },

    {
        // width, height, wdr_mode
        2560, 1440, XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0, 1 }, SC450AI_4M_1440P_BIT_RATE_WDR,
    },

    {
        // width, height, wdr_mode
        2688, 1520, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC450AI_4M_1520P_BIT_RATE_LINEAR,
    },

    {
        // width, height, wdr_mode
        1344, 760, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 120, XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC450AI_1M_760P_BIT_RATE_LINEAR,
    },
};

static const xmedia_u32 g_sc450ai_again_index[SC450AI_AGAIN_INDEX_MAX] = { // 1 2 3 6 12
    1024, 1040, 1056, 1072, 1088, 1104, 1120, 1136, 1152, 1168, 1184, 1200, 1216, 1232, 1248, 1264,
    1280, 1296, 1312, 1328, 1344, 1360, 1376, 1392, 1408, 1424, 1440, 1456, 1472, 1488, 1504, 1520,
    1536, 1552, 1568, 1584, 1600, 1616, 1632, 1648, 1664, 1680, 1696, 1712, 1728, 1744, 1760, 1776,
    1792, 1808, 1823, 1840, 1856, 1872, 1888, 1904, 1920, 1936, 1952, 1968, 1985, 2000, 2016, 2032,

    2048, 2080, 2113, 2144, 2176, 2208, 2241, 2272, 2304, 2336, 2369, 2400, 2432, 2464, 2497, 2528,
    2560, 2592, 2625, 2656, 2688, 2720, 2753, 2784, 2816, 2848, 2881, 2912, 2944, 2976, 3009, 3040,
    3072, 3104, 3137, 3168, 3200, 3232, 3265, 3296, 3328, 3360, 3393, 3424, 3456, 3488, 3521, 3552,
    3584, 3616, 3649, 3680, 3712, 3744, 3777, 3808, 3840, 3872,

    3905, 3965, 4026, 4087, 4148, 4209, 4270, 4330, 4392, 4453, 4514, 4575, 4636, 4697, 4758, 4819,
    4880, 4941, 5002, 5063, 5124, 5185, 5246, 5307, 5368, 5429, 5490, 5551, 5612, 5673, 5734, 5795,
    5856, 5917, 5978, 6039, 6100, 6161, 6222, 6283, 6344, 6405, 6466, 6527, 6588, 6649, 6710, 6771,
    6832, 6893, 6954, 7015, 7076, 7137, 7198, 7259, 7320, 7381, 7442, 7503, 7564, 7625, 7686, 7747,

    7808, 7930, 8052, 8174, 8296, 8418, 8540, 8662, 8784, 8906, 9028, 9150, 9272, 9394, 9516, 9638,
    9760, 9882, 10004, 10126, 10248, 10370, 10492, 10614, 10736, 10858, 10980, 11102, 11224, 11346, 11468, 11590,
    11713, 11834, 11956, 12078, 12200, 12322, 12444, 12566, 12688, 12810, 12932, 13054, 13176, 13298, 13420, 13542,
    13664, 13786, 13908, 14030, 14152, 14274, 14396, 14518, 14640, 14762, 14884, 15006, 15128, 15250, 15372, 15494,

    15616, 15860, 16104, 16348, 16592, 16836, 17080, 17324, 17568, 17812, 18056, 18300, 18544, 18788, 19032, 19276,
    19521, 19764, 20008, 20252, 20496, 20740, 20984, 21228, 21472, 21716, 21960, 22204, 22448, 22692, 22936, 23180,
    23424, 23668, 23912, 24156, 24400, 24644, 24888, 25132, 25376, 25620, 25864, 26108, 26352, 26596, 26840, 27084,
    27329, 27572, 27816, 28060, 28306, 28548, 28792, 29036, 29280, 29524, 29768, 30012, 30256, 30500, 30744, 30988,

    31232, 31720, 32208, 32696, 33184, 33672, 34160, 34648, 35137, 35624, 36112, 36600, 37088, 37576, 38064, 38552,
    39040, 39528, 40016, 40505, 40992, 41480, 41968, 42456, 43945, 43432, 43920, 44408, 44896, 45384, 45872, 46360,
    46848, 47336, 47824, 48312, 48800, 49288, 49776, 50264, 50753, 51240, 51728, 52216, 52704, 53192, 53680, 54168,
    54656, 55144, 55632, 56120, 56608, 57096, 57584, 58072, 58561, 59048, 59536, 60024, 60512, 61000, 61488, 61976
};

static const xmedia_u32 g_sc450ai_again_table[SC450AI_AGAIN_INDEX_MAX] = {
    0x0340, 0x0341, 0x0342, 0x0343, 0x0344, 0x0345, 0x0346, 0x0347,
    0x0348, 0x0349, 0x034A, 0x034B, 0x034C, 0x034D, 0x034E, 0x034F,
    0x0350, 0x0351, 0x0352, 0x0353, 0x0354, 0x0355, 0x0356, 0x0357,
    0x0358, 0x0359, 0x035A, 0x035B, 0x035C, 0x035D, 0x035E, 0x035F,
    0x0360, 0x0361, 0x0362, 0x0363, 0x0364, 0x0365, 0x0366, 0x0367,
    0x0368, 0x0369, 0x036A, 0x036B, 0x036C, 0x036D, 0x036E, 0x036F,
    0x0370, 0x0371, 0x0372, 0x0373, 0x0374, 0x0375, 0x0376, 0x0377,
    0x0378, 0x0379, 0x037A, 0x037B, 0x037C, 0x037D, 0x037E, 0x037F,

    0x0740, 0x0741, 0x0742, 0x0743, 0x0744, 0x0745, 0x0746, 0x0747,
    0x0748, 0x0749, 0x074A, 0x074B, 0x074C, 0x074D, 0x074E, 0x074F,
    0x0750, 0x0751, 0x0752, 0x0753, 0x0754, 0x0755, 0x0756, 0x0757,
    0x0758, 0x0759, 0x075A, 0x075B, 0x075C, 0x075D, 0x075E, 0x075F,
    0x0760, 0x0761, 0x0762, 0x0763, 0x0764, 0x0765, 0x0766, 0x0767,
    0x0768, 0x0769, 0x076A, 0x076B, 0x076C, 0x076D, 0x076E, 0x076F,
    0x0770, 0x0771, 0x0772, 0x0773, 0x0774, 0x0775, 0x0776, 0x0777, 0x0778, 0x0779,

    0x2340, 0x2341, 0x2342, 0x2343, 0x2344, 0x2345, 0x2346, 0x2347,
    0x2348, 0x2349, 0x234A, 0x234B, 0x234C, 0x234D, 0x234E, 0x234F,
    0x2350, 0x2351, 0x2352, 0x2353, 0x2354, 0x2355, 0x2356, 0x2357,
    0x2358, 0x2359, 0x235A, 0x235B, 0x235C, 0x235D, 0x235E, 0x235F,
    0x2360, 0x2361, 0x2362, 0x2363, 0x2364, 0x2365, 0x2366, 0x2367,
    0x2368, 0x2369, 0x236A, 0x236B, 0x236C, 0x236D, 0x236E, 0x236F,
    0x2370, 0x2371, 0x2372, 0x2373, 0x2374, 0x2375, 0x2376, 0x2377,
    0x2378, 0x2379, 0x237A, 0x237B, 0x237C, 0x237D, 0x237E, 0x237F,

    0x2740, 0x2741, 0x2742, 0x2743, 0x2744, 0x2745, 0x2746, 0x2747,
    0x2748, 0x2749, 0x274A, 0x274B, 0x274C, 0x274D, 0x274E, 0x274F,
    0x2750, 0x2751, 0x2752, 0x2753, 0x2754, 0x2755, 0x2756, 0x2757,
    0x2758, 0x2759, 0x275A, 0x275B, 0x275C, 0x275D, 0x275E, 0x275F,
    0x2760, 0x2761, 0x2762, 0x2763, 0x2764, 0x2765, 0x2766, 0x2767,
    0x2768, 0x2769, 0x276A, 0x276B, 0x276C, 0x276D, 0x276E, 0x276F,
    0x2770, 0x2771, 0x2772, 0x2773, 0x2774, 0x2775, 0x2776, 0x2777,
    0x2778, 0x2779, 0x277A, 0x277B, 0x277C, 0x277D, 0x277E, 0x277F,

    0x2F40, 0x2F41, 0x2F42, 0x2F43, 0x2F44, 0x2F45, 0x2F46, 0x2F47,
    0x2F48, 0x2F49, 0x2F4A, 0x2F4B, 0x2F4C, 0x2F4D, 0x2F4E, 0x2F4F,
    0x2F50, 0x2F51, 0x2F52, 0x2F53, 0x2F54, 0x2F55, 0x2F56, 0x2F57,
    0x2F58, 0x2F59, 0x2F5A, 0x2F5B, 0x2F5C, 0x2F5D, 0x2F5E, 0x2F5F,
    0x2F60, 0x2F61, 0x2F62, 0x2F63, 0x2F64, 0x2F65, 0x2F66, 0x2F67,
    0x2F68, 0x2F69, 0x2F6A, 0x2F6B, 0x2F6C, 0x2F6D, 0x2F6E, 0x2F6F,
    0x2F70, 0x2F71, 0x2F72, 0x2F73, 0x2F74, 0x2F75, 0x2F76, 0x2F77,
    0x2F78, 0x2F79, 0x2F7A, 0x2F7B, 0x2F7C, 0x2F7D, 0x2F7E, 0x2F7F,

    0x3F40, 0x3F41, 0x3F42, 0x3F43, 0x3F44, 0x3F45, 0x3F46, 0x3F47,
    0x3F48, 0x3F49, 0x3F4A, 0x3F4B, 0x3F4C, 0x3F4D, 0x3F4E, 0x3F4F,
    0x3F50, 0x3F51, 0x3F52, 0x3F53, 0x3F54, 0x3F55, 0x3F56, 0x3F57,
    0x3F58, 0x3F59, 0x3F5A, 0x3F5B, 0x3F5C, 0x3F5D, 0x3F5E, 0x3F5F,
    0x3F60, 0x3F61, 0x3F62, 0x3F63, 0x3F64, 0x3F65, 0x3F66, 0x3F67,
    0x3F68, 0x3F69, 0x3F6A, 0x3F6B, 0x3F6C, 0x3F6D, 0x3F6E, 0x3F6F,
    0x3F70, 0x3F71, 0x3F72, 0x3F73, 0x3F74, 0x3F75, 0x3F76, 0x3F77,
    0x3F78, 0x3F79, 0x3F7A, 0x3F7B, 0x3F7C, 0x3F7D, 0x3F7E, 0x3F7F
};

static const xmedia_u32 g_sc450ai_dgain_index[SC450AI_DGAIN_INDEX_MAX] = { // 1 2 4 8 16
    1024, 1040, 1056, 1072, 1088, 1104, 1120, 1136, 1152, 1168, 1184, 1200, 1216, 1232, 1248, 1264,
    1280, 1296, 1312, 1328, 1344, 1360, 1376, 1392, 1408, 1424, 1440, 1456, 1472, 1488, 1504, 1520,
    1536, 1552, 1568, 1584, 1600, 1616, 1632, 1648, 1664, 1680, 1696, 1712, 1728, 1744, 1760, 1776,
    1792, 1808, 1824, 1840, 1856, 1872, 1888, 1904, 1920, 1936, 1952, 1968, 1984, 2000, 2016, 2032,

    2048, 2080, 2112, 2144, 2176, 2208, 2240, 2272, 2304, 2336, 2368, 2400, 2432, 2464, 2496, 2528,
    2560, 2592, 2624, 2656, 2688, 2720, 2752, 2784, 2816, 2848, 2880, 2912, 2944, 2976, 3008, 3040,
    3072, 3104, 3136, 3168, 3200, 3232, 3264, 3296, 3328, 3360, 3392, 3424, 3456, 3488, 3520, 3552,
    3584, 3616, 3648, 3680, 3712, 3744, 3776, 3808, 3840, 3872, 3904, 3936, 3968, 4000, 4032, 4064,

    4096, 4160, 4224, 4288, 4352, 4416, 4480, 4544, 4608, 4672, 4736, 4800, 4864, 4928, 4992, 5056,
    5120, 5184, 5248, 5312, 5376, 5440, 5504, 5568, 5632, 5696, 5760, 5824, 5888, 5952, 6016, 6080,
    6144, 6208, 6272, 6336, 6400, 6464, 6528, 6592, 6656, 6720, 6784, 6848, 6912, 6976, 7040, 7104,
    7168, 7232, 7296, 7360, 7424, 7488, 7552, 7616, 7680, 7744, 7808, 7872, 7936, 8000, 8064, 8128,

     8192,  8320,  8448,  8576,  8704,  8832,  8960,  9088,  9216,  9344,  9472,  9600,  9728, 9856,   9984, 10112,
    10240, 10368, 10496, 10624, 10752, 10880, 11008, 11136, 11264, 11392, 11520, 11648, 11776, 11904, 12032, 12160,
    12288, 12416, 12544, 12672, 12800, 12928, 13056, 13184, 13312, 13440, 13568, 13696, 13824, 13952, 14080, 14208,
    14336, 14464, 14592, 14720, 14848, 14976, 15104, 15232, 15360, 15488, 15616, 15744, 15872, 16000, 16128, 16256
};

static const xmedia_u32 g_sc450ai_dgain_table[SC450AI_DGAIN_INDEX_MAX] = { // 1 2 4 8 16
    0x0080, 0x0082, 0x0084, 0x0086, 0x0088, 0x008A, 0x008C, 0x008E,
    0x0090, 0x0092, 0x0094, 0x0096, 0x0098, 0x009A, 0x009C, 0x009E,
    0x00A0, 0x00A2, 0x00A4, 0x00A6, 0x00A8, 0x00AA, 0x00AC, 0x00AE,
    0x00B0, 0x00B2, 0x00B4, 0x00B6, 0x00B8, 0x00BA, 0x00BC, 0x00BE,
    0x00C0, 0x00C2, 0x00C4, 0x00C6, 0x00C8, 0x00CA, 0x00CC, 0x00CE,
    0x00D0, 0x00D2, 0x00D4, 0x00D6, 0x00D8, 0x00DA, 0x00DC, 0x00DE,
    0x00E0, 0x00E2, 0x00E4, 0x00E6, 0x00E8, 0x00EA, 0x00EC, 0x00EE,
    0x00F0, 0x00F2, 0x00F4, 0x00F6, 0x00F8, 0x00FA, 0x00FC, 0x00FE,

    0x0180, 0x0182, 0x0184, 0x0186, 0x0188, 0x018A, 0x018C, 0x018E,
    0x0190, 0x0192, 0x0194, 0x0196, 0x0198, 0x019A, 0x019C, 0x019E,
    0x01A0, 0x01A2, 0x01A4, 0x01A6, 0x01A8, 0x01AA, 0x01AC, 0x01AE,
    0x01B0, 0x01B2, 0x01B4, 0x01B6, 0x01B8, 0x01BA, 0x01BC, 0x01BE,
    0x01C0, 0x01C2, 0x01C4, 0x01C6, 0x01C8, 0x01CA, 0x01CC, 0x01CE,
    0x01D0, 0x01D2, 0x01D4, 0x01D6, 0x01D8, 0x01DA, 0x01DC, 0x01DE,
    0x01E0, 0x01E2, 0x01E4, 0x01E6, 0x01E8, 0x01EA, 0x01EC, 0x01EE,
    0x01F0, 0x01F2, 0x01F4, 0x01F6, 0x01F8, 0x01FA, 0x01FC, 0x01FE,

    0x0380, 0x0382, 0x0384, 0x0386, 0x0388, 0x038A, 0x038C, 0x038E,
    0x0390, 0x0392, 0x0394, 0x0396, 0x0398, 0x039A, 0x039C, 0x039E,
    0x03A0, 0x03A2, 0x03A4, 0x03A6, 0x03A8, 0x03AA, 0x03AC, 0x03AE,
    0x03B0, 0x03B2, 0x03B4, 0x03B6, 0x03B8, 0x03BA, 0x03BC, 0x03BE,
    0x03C0, 0x03C2, 0x03C4, 0x03C6, 0x03C8, 0x03CA, 0x03CC, 0x03CE,
    0x03D0, 0x03D2, 0x03D4, 0x03D6, 0x03D8, 0x03DA, 0x03DC, 0x03DE,
    0x03E0, 0x03E2, 0x03E4, 0x03E6, 0x03E8, 0x03EA, 0x03EC, 0x03EE,
    0x03F0, 0x03F2, 0x03F4, 0x03F6, 0x03F8, 0x03FA, 0x03FC, 0x03FE,

    0x0780, 0x0782, 0x0784, 0x0786, 0x0788, 0x078A, 0x078C, 0x078E,
    0x0790, 0x0792, 0x0794, 0x0796, 0x0798, 0x079A, 0x079C, 0x079E,
    0x07A0, 0x07A2, 0x07A4, 0x07A6, 0x07A8, 0x07AA, 0x07AC, 0x07AE,
    0x07B0, 0x07B2, 0x07B4, 0x07B6, 0x07B8, 0x07BA, 0x07BC, 0x07BE,
    0x07C0, 0x07C2, 0x07C4, 0x07C6, 0x07C8, 0x07CA, 0x07CC, 0x07CE,
    0x07D0, 0x07D2, 0x07D4, 0x07D6, 0x07D8, 0x07DA, 0x07DC, 0x07DE,
    0x07E0, 0x07E2, 0x07E4, 0x07E6, 0x07E8, 0x07EA, 0x07EC, 0x07EE,
    0x07F0, 0x07F2, 0x07F4, 0x07F6, 0x07F8, 0x07FA, 0x07FC, 0x07FE
};

SENSOR_PRIORITY_RODATA static const xmedia_sensor_capability g_sc450ai_capability = {
    .max_width   = 2688,
    .max_height  = 1520,
    .max_fps     = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported  = XMEDIA_FALSE,
    .mipi_lanes_supported  = XMEDIA_SENSOR_MIPI_LANES_2L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = SC450AI_ADDR_BYTE,
        .data_byte_num     = SC450AI_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { SC450AI_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { SC450AI_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x01 },
    },
};

SENSOR_PRIORITY_FUNC static xmedia_s32 sc450ai_ctx_init(xmedia_u32 dev)
{
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SC450AI_GET_CTX(dev, sc450ai_ctx);
    if (sc450ai_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    sc450ai_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (sc450ai_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(sc450ai_ctx, 0, sizeof(sensor_context));
    sc450ai_ctx->i2c_addr         = SC450AI_I2C_ADDR;
    sc450ai_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_2L;
    sc450ai_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    sc450ai_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    sc450ai_ctx->size.width       = 2560;
    sc450ai_ctx->size.height      = 1440;
    sc450ai_ctx->fps              = g_sc450ai_property[0].max_fps;
    sc450ai_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    sc450ai_ctx->img_mode             = SC450AI_4M_1440_10BIT_LINEAR_MODE;
    sc450ai_ctx->fl_std               = SC450AI_VMAX_4M_1440_LINEAR << SC450AI_EXP_SHIFT;
    sc450ai_ctx->fl[SENSOR_CUR_FRAME] = SC450AI_VMAX_4M_1440_LINEAR << SC450AI_EXP_SHIFT;
    sc450ai_ctx->fl[SENSOR_PRE_FRAME] = SC450AI_VMAX_4M_1440_LINEAR << SC450AI_EXP_SHIFT;

    SC450AI_SET_CTX(dev, sc450ai_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void sc450ai_ctx_exit(xmedia_u32 dev)
{
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_FREE(sc450ai_ctx);
    SC450AI_SET_CTX(dev, XMEDIA_NULL);
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc450ai_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    for (i = 0; i < SC450AI_SPECS_MAX_NUM; i++) {
        if (g_sc450ai_property[i].width == sc450ai_ctx->size.width &&
            g_sc450ai_property[i].height == sc450ai_ctx->size.height &&
            g_sc450ai_property[i].wdr_mode == sc450ai_ctx->wdr_mode) {
            memcpy(property, &g_sc450ai_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n",
                    sc450ai_ctx->size.width, sc450ai_ctx->size.height, sc450ai_ctx->wdr_mode);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 sc450ai_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc450ai_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    if (mipi_lanes == XMEDIA_SENSOR_MIPI_LANES_2L || mipi_lanes == XMEDIA_SENSOR_MIPI_LANES_4L) {
        sc450ai_ctx->lanes = mipi_lanes;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR, "No support mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc450ai_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        sc450ai_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        sc450ai_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        sc450ai_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 sc450ai_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    sc450ai_ctx->i2c_addr = slave_addr;
    ret = sc450ai_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    *slave_addr = sc450ai_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}


SENSOR_PRIORITY_FUNC static xmedia_s32 sc450ai_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    sc450ai_ctx->init_mode = init_mode;

    ret = sc450ai_i2c_init(dev, sc450ai_ctx->bus_info.i2c_dev, sc450ai_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    sc450ai_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc450ai_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    ret = sc450ai_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    sc450ai_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    sc450ai_ctx->mirror_en = mirror_en;
    ret = sc450ai_set_mirror_flip(dev, mirror_en, sc450ai_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    sc450ai_ctx->flip_en = flip_en;
    ret = sc450ai_set_mirror_flip(dev, sc450ai_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc450ai_set_image_mode(sensor_context *sc450ai_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (sc450ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (SC450AI_RES_IS_1440P(sns_attr->width, sns_attr->height)) {
            *image_mode         = SC450AI_4M_1440_10BIT_LINEAR_MODE;
            sc450ai_ctx->fl_std = SC450AI_VMAX_4M_1440_LINEAR << SC450AI_EXP_SHIFT;
        } else if (SC450AI_RES_IS_1520P(sns_attr->width, sns_attr->height)) {
            *image_mode         = SC450AI_4M_1520_10BIT_LINEAR_MODE;
            sc450ai_ctx->fl_std = SC450AI_VMAX_4M_1520_LINEAR << SC450AI_EXP_SHIFT;
        }  else if (SC450AI_RES_IS_760P(sns_attr->width, sns_attr->height)) {
            *image_mode         = SC450AI_1M_760_10BIT_LINEAR_MODE;
            sc450ai_ctx->fl_std = SC450AI_VMAX_1M_760_LINEAR << SC450AI_EXP_SHIFT;
        } else {
            SC450AI_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else if (sc450ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (SC450AI_RES_IS_1440P(sns_attr->width, sns_attr->height)) {
            *image_mode         = SC450AI_4M_1440_10BIT_WDR_MODE;
            sc450ai_ctx->fl_std = SC450AI_VMAX_4M_1440_WDR << SC450AI_EXP_SHIFT;
        } else {
            SC450AI_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        SC450AI_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc450ai_ctx->size.width  = sns_attr->width;
    sc450ai_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc450ai_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            sc450ai_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            sc450ai_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
            SENSOR_PRINT("2to1 line WDR\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(sc450ai_ctx->wdr_int_time, 0, sizeof(sc450ai_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

#define SC450AI_LINEAR_REG_INFO_MAX_NUM   SC450AI_REG_L_MAX_NUM
#define SC450AI_2TO1_WDR_REG_INFO_MAX_NUM SC450AI_REG_MAX_NUM
SENSOR_PRIORITY_FUNC static xmedia_void sc450ai_init_common_reg_info(sensor_context *sc450ai_ctx)
{
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_EXP_H].delay_frame_num = 2;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_EXP_H].reg_addr        = SC450AI_REG_ADDR_EXP_H;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_EXP_M].delay_frame_num = 2;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_EXP_M].reg_addr        = SC450AI_REG_ADDR_EXP_M;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_EXP_L].delay_frame_num = 2;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_EXP_L].reg_addr        = SC450AI_REG_ADDR_EXP_L;

    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_AGAIN_H].delay_frame_num = 2;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_AGAIN_H].reg_addr        = SC450AI_REG_ADDR_AGAIN_H;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_AGAIN_L].delay_frame_num = 2;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_AGAIN_L].reg_addr        = SC450AI_REG_ADDR_AGAIN_L;

    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_DGAIN_H].delay_frame_num = 2;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_DGAIN_H].reg_addr        = SC450AI_REG_ADDR_DGAIN_H;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_DGAIN_L].delay_frame_num = 2;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_DGAIN_L].reg_addr        = SC450AI_REG_ADDR_DGAIN_L;

    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_VMAX_H].delay_frame_num = 0;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_VMAX_H].reg_addr        = SC450AI_REG_ADDR_VMAX_H;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_VMAX_L].delay_frame_num = 0;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_VMAX_L].reg_addr        = SC450AI_REG_ADDR_VMAX_L;

    return;
}

SENSOR_PRIORITY_FUNC static xmedia_void sc450ai_init_2to1_wdr_reg_info(sensor_context *sc450ai_ctx)
{
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_EXP_H].delay_frame_num = 2;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_EXP_H].reg_addr        = SC450AI_REG_ADDR_S_EXP_H;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_EXP_L].delay_frame_num = 2;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_EXP_L].reg_addr        = SC450AI_REG_ADDR_S_EXP_L;

    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_AGAIN_H].delay_frame_num = 2;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_AGAIN_H].reg_addr = SC450AI_REG_ADDR_S_AGAIN_H;

    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_AGAIN_L].delay_frame_num = 2;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_AGAIN_L].reg_addr = SC450AI_REG_ADDR_S_AGAIN_L;

    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_DGAIN_H].delay_frame_num = 2;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_DGAIN_H].reg_addr = SC450AI_REG_ADDR_S_DGAIN_H;

    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_DGAIN_L].delay_frame_num = 2;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_DGAIN_L].reg_addr = SC450AI_REG_ADDR_S_DGAIN_L;

    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_EXP_MAX_H].delay_frame_num = 0;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_EXP_MAX_H].reg_addr = SC450AI_REG_ADDR_S_EXP_MAX_H;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_EXP_MAX_L].delay_frame_num = 0;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_EXP_MAX_L].reg_addr = SC450AI_REG_ADDR_S_EXP_MAX_L;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc450ai_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = sc450ai_ctx->bus_info.i2c_dev;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = SC450AI_LINEAR_REG_INFO_MAX_NUM;

    if (sc450ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num = SC450AI_2TO1_WDR_REG_INFO_MAX_NUM;
    }
    for (i = 0; i < sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = sc450ai_ctx->i2c_addr;
        sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = SC450AI_ADDR_BYTE;
        sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = SC450AI_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    sc450ai_init_common_reg_info(sc450ai_ctx);

     // init 2to1 wdr mode Regs
    if (sc450ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc450ai_init_2to1_wdr_reg_info(sc450ai_ctx);
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc450ai_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    // Set wdr mode
    ret = sc450ai_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = sc450ai_set_image_mode(sc450ai_ctx, &sc450ai_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    sc450ai_ctx->fl[SENSOR_CUR_FRAME] = sc450ai_ctx->fl_std;
    sc450ai_ctx->fl[SENSOR_PRE_FRAME] = sc450ai_ctx->fl[SENSOR_CUR_FRAME];

    ret = sc450ai_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef SC450AI_ISP_DEFAULT_SUPPORT
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from sc450ai!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 sc450ai_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (sc450ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x100;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    if (sc450ai_ctx->mirror_en && sc450ai_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (sc450ai_ctx->mirror_en && (!sc450ai_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!sc450ai_ctx->mirror_en) && sc450ai_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < SC450AI_SPECS_MAX_NUM; i++) {
        if (g_sc450ai_property[i].width == sc450ai_ctx->size.width &&
            g_sc450ai_property[i].height == sc450ai_ctx->size.height &&
            g_sc450ai_property[i].wdr_mode == sc450ai_ctx->wdr_mode) {
            *bayer_pattern = g_sc450ai_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= SC450AI_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc450ai_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_sc450ai_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc450ai_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 sc450ai_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_sc450ai_exposure[dev]      = init_param->exposure;
    g_sc450ai_sample_r_gain[dev] = init_param->sample_rgain;
    g_sc450ai_sample_b_gain[dev] = init_param->sample_bgain;

    g_sc450ai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_sc450ai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_sc450ai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc450ai_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case SC450AI_1M_760_10BIT_LINEAR_MODE:
            *max_fps = 120.0;
            break;
        case SC450AI_4M_1440_10BIT_LINEAR_MODE:
            *max_fps = 30.0;
            break;
        case SC450AI_4M_1440_10BIT_WDR_MODE:
            *max_fps = 30.0;
            break;
        case SC450AI_4M_1520_10BIT_LINEAR_MODE:
            *max_fps = 30.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = sc450ai_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256;
    ae_sns_dft->full_lines_max = SC450AI_FULL_LINES_MAX;

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

static xmedia_void sc450ai_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 61976; // max: 61976
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 1024;
    ae_sns_dft->min_dgain        = 1024;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x38;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_sc450ai_exposure[dev] ? g_sc450ai_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - 8;
    ae_sns_dft->min_int_time        = 1;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = 1;

    return;
}

static xmedia_s32 sc450ai_get_ae_2to1_wdr_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                 xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 4;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 7746; // 32*2.55*128
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 2032;//3.969x128
    ae_sns_dft->min_dgain        = 1024;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;
    ae_sns_dft->init_exposure    = g_sc450ai_exposure[dev] ? g_sc450ai_exposure[dev] : 16462;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - SC450AI_EXP_OFFSET_WDR;
    ae_sns_dft->min_int_time        = 5;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = 5;

    if (sc450ai_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
        ae_sns_dft->ae_compensation = 0x30;
        ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    } else {
        ae_sns_dft->max_dgain_target    = 1024;
        ae_sns_dft->max_ispdgain_target = 4096;
        ae_sns_dft->ae_compensation     = 0x40;
        ae_sns_dft->ae_exp_mode         = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
        ae_sns_dft->man_ratio_enable    = XMEDIA_TRUE;

        ae_sns_dft->ratio[0] = 0x400;
        ae_sns_dft->ratio[1] = 0x40;
        ae_sns_dft->ratio[2] = 0x40;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = sc450ai_get_ae_common_default(dev, sc450ai_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (sc450ai_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            sc450ai_get_ae_linear_default(dev, sc450ai_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            sc450ai_get_ae_2to1_wdr_default(dev, sc450ai_ctx, ae_sns_dft);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", sc450ai_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context    *sc450ai_ctx                      = XMEDIA_NULL;
    static xmedia_bool first[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 1 };

    SENSOR_CHECK_DEV_RETURN(dev);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    if (sc450ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (first[dev]) { // 0: short exposure
            sc450ai_ctx->wdr_int_time[0] = int_time;
            sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_EXP_H].data =
                                                                                    SENSOR_MIDDLE_8BITS(int_time);
            sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_EXP_L].data =
                                                                                    SENSOR_LOWER_4BITS(int_time);
            first[dev] = XMEDIA_FALSE;
        } else { // 1: long exposure
            sc450ai_ctx->wdr_int_time[1] = int_time;
            sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_EXP_H].data = SENSOR_HIGHER_4BITS(int_time);
            sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_EXP_M].data = SENSOR_MIDDLE_8BITS(int_time);
            sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_EXP_L].data = SENSOR_LOWER_4BITS(int_time);
            first[dev] = XMEDIA_TRUE;
        }
    } else if (sc450ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_EXP_H].data = SENSOR_HIGHER_4BITS(int_time);
        sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_EXP_M].data = SENSOR_MIDDLE_8BITS(int_time);
        sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_EXP_L].data = SENSOR_LOWER_4BITS(int_time);
        first[dev]                                                                = XMEDIA_TRUE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_sc450ai_again_index[SC450AI_AGAIN_INDEX_MAX - 1]) {
        *again_lin = g_sc450ai_again_index[SC450AI_AGAIN_INDEX_MAX - 1];
        *again_db = g_sc450ai_again_table[SC450AI_AGAIN_INDEX_MAX - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < SC450AI_AGAIN_INDEX_MAX; i++) {
        if (*again_lin < g_sc450ai_again_index[i]) {
            *again_lin = g_sc450ai_again_index[i - 1];
            *again_db = g_sc450ai_again_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(dgain_lin);
    SENSOR_CHECK_PTR_RETURN(dgain_db);

    if (*dgain_lin >= g_sc450ai_dgain_index[SC450AI_DGAIN_INDEX_MAX - 1]) {
        *dgain_lin = g_sc450ai_dgain_index[SC450AI_DGAIN_INDEX_MAX - 1];
        *dgain_db  = g_sc450ai_dgain_table[SC450AI_DGAIN_INDEX_MAX - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; SC450AI_DGAIN_INDEX_MAX; i++) {
        if (*dgain_lin < g_sc450ai_dgain_index[i]) {
            *dgain_lin = g_sc450ai_dgain_index[i - 1];
            *dgain_db  = g_sc450ai_dgain_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *sc450ai_ctx = XMEDIA_NULL;
    xmedia_u8 reg_0x3e09; // again
    xmedia_u8 reg_0x3e08;
    xmedia_u8 reg_0x3e07; // dgain
    xmedia_u8 reg_0x3e06;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    reg_0x3e06 = SENSOR_HIGH_8BITS(dgain);
    reg_0x3e07 = SENSOR_LOW_8BITS(dgain);
    reg_0x3e08 = SENSOR_HIGH_8BITS(again);
    reg_0x3e09 = SENSOR_LOW_8BITS(again);

    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_DGAIN_H].data = reg_0x3e06;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_DGAIN_L].data = reg_0x3e07;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_AGAIN_H].data = reg_0x3e08;
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_AGAIN_L].data = reg_0x3e09;

    if (sc450ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_DGAIN_H].data = reg_0x3e06;
        sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_DGAIN_L].data = reg_0x3e07;
        sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_AGAIN_H].data = reg_0x3e08;
        sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_AGAIN_L].data = reg_0x3e09;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = SC450AI_CALIBRATE_STATIC_TEMP;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = SC450AI_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = SC450AI_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = SC450AI_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = SC450AI_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 5: point index on the awb curve param
    awb_sns_dft->wb_para[0] = SC450AI_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1] = SC450AI_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2] = SC450AI_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3] = SC450AI_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4] = SC450AI_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5] = SC450AI_CALIBRATE_AWB_C1;

    awb_sns_dft->golden_rgain = SC450AI_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = SC450AI_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (sc450ai_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_sc450ai_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_sc450ai_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_sc450ai_awb_wdr_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_sc450ai_awb_wdr_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", sc450ai_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_sc450ai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_sc450ai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_sc450ai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_sc450ai_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_sc450ai_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_standby(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc450ai_write_reg(dev, SC450AI_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc450ai_resume(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc450ai_write_reg(dev, SC450AI_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc450ai_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *sc450ai_ctx  = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= sc450ai_write_reg(dev, sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc450ai_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    if (sc450ai_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = sc450ai_init_image(dev, sc450ai_ctx->img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = sc450ai_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc450ai_set_mirror_flip(dev, sc450ai_ctx->mirror_en, sc450ai_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc450ai_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc450ai_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_sc450ai_dev_map[index] == SENSOR_DEV_INVALID) {
            g_sc450ai_dev_map[index] = pipe;
            *dev                     = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc450ai_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_sc450ai_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 sc450ai_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{ // TO-DO：min fps is to be confirmed.
    switch (img_mode) {
        case SC450AI_1M_760_10BIT_LINEAR_MODE:
            *min_fps = 5;
            break;
        case SC450AI_4M_1440_10BIT_LINEAR_MODE:
            *min_fps = 5;
            break;
        case SC450AI_4M_1440_10BIT_WDR_MODE:
            *min_fps = 5;
            break;
        case SC450AI_4M_1520_10BIT_LINEAR_MODE:
            *min_fps = 5;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case SC450AI_1M_760_10BIT_LINEAR_MODE:
            *vmax = SC450AI_VMAX_1M_760_LINEAR;
            break;
        case SC450AI_4M_1440_10BIT_LINEAR_MODE:
            *vmax = SC450AI_VMAX_4M_1440_LINEAR;
            break;
        case SC450AI_4M_1440_10BIT_WDR_MODE:
            *vmax = SC450AI_VMAX_4M_1440_WDR;
            break;
        case SC450AI_4M_1520_10BIT_LINEAR_MODE:
            *vmax = SC450AI_VMAX_4M_1520_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = sc450ai_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc450ai_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc450ai_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is (%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, SC450AI_FULL_LINES_MAX);

    if (sns_ctx->img_mode == SC450AI_4M_1440_10BIT_WDR_MODE) {
        sns_ctx->max_short_exp = SC450AI_S_EXP_MAX_DEFAULT * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    for (i = 0; i < sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            sc450ai_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &sc450ai_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&sc450ai_ctx->regs_info[SENSOR_PRE_FRAME], &sc450ai_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    sc450ai_ctx->fl[SENSOR_PRE_FRAME] = sc450ai_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    ret = sc450ai_calc_fps(fps, sc450ai_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    sc450ai_ctx->fl[SENSOR_CUR_FRAME] = full_lines << SC450AI_EXP_SHIFT;
    sc450ai_ctx->fl_std               = sc450ai_ctx->fl[SENSOR_CUR_FRAME];
    sc450ai_ctx->fps                  = fps;
    ae_sns_dft->fps                   = fps;
    ae_sns_dft->full_lines_std        = sc450ai_ctx->fl_std;
    ae_sns_dft->full_lines            = sc450ai_ctx->fl_std;

    if (sc450ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sc450ai_ctx->fl[SENSOR_CUR_FRAME] - SC450AI_EXP_OFFSET_LINEAR;
    } else if (sc450ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_EXP_MAX_H].data =
                                                                        SENSOR_HIGH_8BITS(sc450ai_ctx->max_short_exp);
        sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_EXP_MAX_L].data =
                                                                        SENSOR_LOW_8BITS(sc450ai_ctx->max_short_exp);

        sc450ai_ctx->max_short_exp = sc450ai_ctx->max_short_exp << SC450AI_EXP_SHIFT;
        ae_sns_dft->max_int_time   = sc450ai_ctx->fl[SENSOR_CUR_FRAME] - sc450ai_ctx->max_short_exp - \
            SC450AI_EXP_OFFSET_WDR;
    }

    SENSOR_PRINT("dev[%d]- sc450ai set fps: %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = sc450ai_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc450ai_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc450ai_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = (vmax_max_fps << SC450AI_EXP_SHIFT) * max_fps / min_fps;

    return XMEDIA_SUCCESS;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 sc450ai_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    xmedia_u32      ratio;
    xmedia_u32      short_frame_min_exp;
    xmedia_u32      short_frame_max_exp;
    xmedia_u32      max_short_exp;
    xmedia_u32      long_frame_min_exp;
    xmedia_u32      long_frame_max_exp;
    xmedia_u32      reg_short_frame_max_exp;
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);
    memcpy(sc450ai_ctx->ratio, inttime_attr->ratio, sizeof(inttime_attr->ratio));

    max_short_exp = sc450ai_ctx->max_short_exp;
    ratio = inttime_attr->ratio[0];
    short_frame_min_exp = 5;
    long_frame_min_exp  = 5;
    short_frame_max_exp = 1;

    if (sc450ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (sc450ai_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
            inttime_attr->min_inttime[0] = short_frame_min_exp; // 0:short frame; 1: long frame
            inttime_attr->max_inttime[0] = short_frame_max_exp;
            inttime_attr->min_inttime[1] = long_frame_min_exp;
            reg_short_frame_max_exp      = inttime_attr->min_inttime[0] + 18;
            inttime_attr->max_inttime[1] = sc450ai_ctx->fl[SENSOR_CUR_FRAME] - reg_short_frame_max_exp - 18;
        } else {
            short_frame_max_exp = ((((sc450ai_ctx->fl[0]) * 0x40)  / SENSOR_DIV_0_TO_1(ratio + 0x40) + 1) * 2) / 2;
            short_frame_max_exp = SENSOR_MIN(short_frame_max_exp, (2 * max_short_exp - 11));
            short_frame_max_exp = SENSOR_MIN(short_frame_max_exp,
                    ((sc450ai_ctx->fl[0] - max_short_exp - SC450AI_EXP_OFFSET_WDR) * 0x40 / SENSOR_DIV_0_TO_1(ratio)));

            long_frame_max_exp           = short_frame_max_exp * ratio / 64;
            long_frame_min_exp           = short_frame_min_exp * ratio / 64;
            reg_short_frame_max_exp      = (short_frame_max_exp + 11) / 2;
            inttime_attr->min_inttime[0] = short_frame_min_exp; // 0:short frame; 1: long frame
            inttime_attr->max_inttime[0] = short_frame_max_exp;
            inttime_attr->min_inttime[1] = long_frame_min_exp;
            inttime_attr->max_inttime[1] = long_frame_max_exp;
        }
        sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_EXP_MAX_H].data =
            SENSOR_HIGH_8BITS(reg_short_frame_max_exp);
        sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_S_EXP_MAX_L].data =
            SENSOR_LOW_8BITS(reg_short_frame_max_exp);
        sc450ai_ctx->max_short_exp = reg_short_frame_max_exp << SC450AI_EXP_SHIFT;
    }

    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 * 注意：2to1 wdr模式下，曝光比和ae wdr mode 由AE传入，调用本接口前，AE需先调用sc450ai_get_max_inttime
 */
static xmedia_s32 sc450ai_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context               *sc450ai_ctx = XMEDIA_NULL;
    xmedia_u32                    vmax_min_fps;
    xmedia_s32                    ret;
    xmedia_sensor_ae_inttime_attr inttime_attr;
    xmedia_float                  min_fps;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    if (sc450ai_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", sc450ai_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (sc450ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        full_lines = full_lines + SC450AI_EXP_OFFSET_LINEAR;
    } else if (sc450ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        full_lines = full_lines + SC450AI_EXP_OFFSET_WDR;
    }

    ret = sc450ai_get_min_fps(sc450ai_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc450ai_get_min_fps_vmax(sc450ai_ctx->img_mode, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                        = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    sc450ai_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    sc450ai_ctx->fps                  = min_fps * vmax_min_fps / full_lines;

    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_VMAX_H].data =
                                                                    SENSOR_HIGH_8BITS(full_lines >> SC450AI_EXP_SHIFT);
    sc450ai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC450AI_REG_VMAX_L].data =
                                                                    SENSOR_LOW_8BITS(full_lines >> SC450AI_EXP_SHIFT);

    ae_sns_dft->full_lines = sc450ai_ctx->fl[SENSOR_CUR_FRAME];
    if (sc450ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sc450ai_ctx->fl[SENSOR_CUR_FRAME] - SC450AI_EXP_OFFSET_LINEAR;
    } else if (sc450ai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        inttime_attr.ratio[0] = sc450ai_ctx->ratio[0]; // index 0: 长短帧曝光比
        sc450ai_get_max_inttime(dev, &inttime_attr);
        ae_sns_dft->max_int_time =
                            sc450ai_ctx->fl[SENSOR_CUR_FRAME] - sc450ai_ctx->max_short_exp - SC450AI_EXP_OFFSET_WDR;
    }

    SENSOR_PRINT("dev[%d]- sc450ai set fps: %f\n", dev, sc450ai_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc450ai_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    ret = sc450ai_get_min_fps(sc450ai_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc450ai_get_max_fps(sc450ai_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = sc450ai_ctx->fps;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc450ai_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, SC450AI_NAME, sizeof(SC450AI_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = sc450ai_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = sc450ai_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = sc450ai_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = sc450ai_mirror;
    info->isp_func.pfn_sensor_flip             = sc450ai_flip;
    info->isp_func.pfn_sensor_set_bus_info     = sc450ai_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = sc450ai_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = sc450ai_set_init_param;
    info->isp_func.pfn_sensor_start            = sc450ai_start;
    info->isp_func.pfn_sensor_stop             = sc450ai_stop;
    info->isp_func.pfn_sensor_standby          = sc450ai_standby;
    info->isp_func.pfn_sensor_resume           = sc450ai_resume;
    info->isp_func.pfn_sensor_write_reg        = sc450ai_write_reg;
    info->isp_func.pfn_sensor_read_reg         = sc450ai_read_reg;
    info->isp_func.pfn_sensor_init             = sc450ai_init;
    info->isp_func.pfn_sensor_exit             = sc450ai_exit;
    info->isp_func.pfn_sensor_set_attr         = sc450ai_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = sc450ai_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = sc450ai_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = sc450ai_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = sc450ai_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = sc450ai_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = sc450ai_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = sc450ai_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = sc450ai_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = sc450ai_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = sc450ai_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = sc450ai_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = sc450ai_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = sc450ai_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = sc450ai_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = sc450ai_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = sc450ai_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = sc450ai_get_awb_default;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc450ai_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = sc450ai_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc450ai_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = sc450ai_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "SC450AI register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc450ai_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *sc450ai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = sc450ai_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    SC450AI_GET_CTX(dev, sc450ai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc450ai_ctx);

    if (sc450ai_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "SC450AI unregister function failed!\n");
        return ret;
    }

    sc450ai_ctx_exit(dev);
    g_sc450ai_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;
}
