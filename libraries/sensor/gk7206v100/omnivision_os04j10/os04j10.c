#include <stdlib.h>
#include <string.h>
#include "os04j10.h"
#include "os04j10_ctrl.h"
#include "os04j10_ex.h"

#ifdef __linux__
#define OS04J10_ISP_DEFAULT_SUPPORT
#endif

#define OS04J10_INPUT_CLOCK_LINEAR XMEDIA_SENSOR_CLOCK_FREQ_24MHZ
#define OS04J10_MAX_FPS_LINEAR     30.0
#define OS04J10_DATA_WIDTH_LINEAR  XMEDIA_VIDEO_DATA_WIDTH_12
#define OS04J10_INPUT_CLOCK_WDR    XMEDIA_SENSOR_CLOCK_FREQ_24MHZ
#define OS04J10_MAX_FPS_WDR        30.0
#define OS04J10_DATA_WIDTH_WDR     XMEDIA_VIDEO_DATA_WIDTH_10
#define OS04J10_BIT_RATE_LINEAR    720
#define OS04J10_BIT_RATE_WDR       1440

#define OS04J10_NAME          "OS04J10"
#define OS04J10_SPECS_MAX_NUM 1

#define OS04J10_RES_IS_1440P(w, h) ((w) == 2560 && (h) == 1440)

#define OS04J10_ERR_MODE_PRINT(sensor_image_mode)                                                                      \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sensor_image_mode->width,       \
                     sensor_image_mode->height, sensor_image_mode->wdr_mode);                                          \
    } while (0)

sensor_context *g_os04j10_ctx[XMEDIA_SENSOR_DEV_MAX_NUM]     = { XMEDIA_NULL };
#define OS04J10_SET_CTX(dev, sns_ctx) g_os04j10_ctx[dev] = sns_ctx
#define OS04J10_GET_CTX(dev, sns_ctx) sns_ctx = g_os04j10_ctx[dev]

// awb static param for Fuji Lens New IR_Cut
#define CALIBRATE_STATIC_WB_R_GAIN  487
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  495

// Calibration results for Auto WB Planck
#define CALIBRATE_AWB_P1 34
#define CALIBRATE_AWB_P2 187
#define CALIBRATE_AWB_Q1 (-35)
#define CALIBRATE_AWB_A1 183933
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-131952)

// Rgain and Bgain of the golden sample
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

static xmedia_u32 g_os04j10_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_os04j10_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_os04j10_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_os04j10_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };
static xmedia_s32 g_os04j10_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

static const xmedia_sensor_property g_os04j10_property[OS04J10_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        2560, 1440, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, OS04J10_MAX_FPS_LINEAR, OS04J10_INPUT_CLOCK_LINEAR,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, OS04J10_DATA_WIDTH_LINEAR,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, OS04J10_BIT_RATE_LINEAR,
     },
};

#define OS04J10_AGAIN_MAX_NUM 96
static const xmedia_u32 g_os04j10_again_table[OS04J10_AGAIN_MAX_NUM] = {
    1024,  1088,  1152,  1216,  1280,  1344,  1408,  1472,  1536,  1600,  1664,  1728,  1792,  1856,  1920,  1984,
    2048,  2176,  2304,  2432,  2560,  2688,  2816,  2944,  3072,  3200,  3328,  3456,  3584,  3712,  3840,  3968,
    4096,  4352,  4608,  4864,  5120,  5376,  5632,  5888,  6144,  6400,  6656,  6912,  7168,  7424,  7680,  7936,
    8192,  8704,  9216,  9728,  10240, 10752, 11264, 11776, 12288, 12800, 13312, 13824, 14336, 14848, 15360, 15872,
    16384, 17408, 18432, 19456, 20480, 21504, 22528, 23552, 24576, 25600, 26624, 27648, 28672, 29696, 30720, 31744,
    32768, 34816, 36864, 38912, 40960, 43008, 45056, 47104, 49152, 51200, 53248, 55296, 57344, 59392, 61440, 63488,
};

#define OS04J10_DGAIN_MAX_NUM 992
static const xmedia_u32 g_os04j10_dgain_table[OS04J10_DGAIN_MAX_NUM] = {
    1024,  1056,  1088,  1120,  1152,  1184,  1216,  1248,  1280,  1312,  1344,  1376,  1408,  1440,  1472,  1504,
    1536,  1568,  1600,  1632,  1664,  1696,  1728,  1760,  1792,  1824,  1856,  1888,  1920,  1952,  1984,  2016,
    2048,  2080,  2112,  2144,  2176,  2208,  2240,  2272,  2304,  2336,  2368,  2400,  2432,  2464,  2496,  2528,
    2560,  2592,  2624,  2656,  2688,  2720,  2752,  2784,  2816,  2848,  2880,  2912,  2944,  2976,  3008,  3040,
    3072,  3104,  3136,  3168,  3200,  3232,  3264,  3296,  3328,  3360,  3392,  3424,  3456,  3488,  3520,  3552,
    3584,  3616,  3648,  3680,  3712,  3744,  3776,  3808,  3840,  3872,  3904,  3936,  3968,  4000,  4032,  4064,
    4096,  4128,  4160,  4192,  4224,  4256,  4288,  4320,  4352,  4384,  4416,  4448,  4480,  4512,  4544,  4576,
    4608,  4640,  4672,  4704,  4736,  4768,  4800,  4832,  4864,  4896,  4928,  4960,  4992,  5024,  5056,  5088,
    5120,  5152,  5184,  5216,  5248,  5280,  5312,  5344,  5376,  5408,  5440,  5472,  5504,  5536,  5568,  5600,
    5632,  5664,  5696,  5728,  5760,  5792,  5824,  5856,  5888,  5920,  5952,  5984,  6016,  6048,  6080,  6112,
    6144,  6176,  6208,  6240,  6272,  6304,  6336,  6368,  6400,  6432,  6464,  6496,  6528,  6560,  6592,  6624,
    6656,  6688,  6720,  6752,  6784,  6816,  6848,  6880,  6912,  6944,  6976,  7008,  7040,  7072,  7104,  7136,
    7168,  7200,  7232,  7264,  7296,  7328,  7360,  7392,  7424,  7456,  7488,  7520,  7552,  7584,  7616,  7648,
    7680,  7712,  7744,  7776,  7808,  7840,  7872,  7904,  7936,  7968,  8000,  8032,  8064,  8096,  8128,  8160,
    8192,  8224,  8256,  8288,  8320,  8352,  8384,  8416,  8448,  8480,  8512,  8544,  8576,  8608,  8640,  8672,
    8704,  8736,  8768,  8800,  8832,  8864,  8896,  8928,  8960,  8992,  9024,  9056,  9088,  9120,  9152,  9184,
    9216,  9248,  9280,  9312,  9344,  9376,  9408,  9440,  9472,  9504,  9536,  9568,  9600,  9632,  9664,  9696,
    9728,  9760,  9792,  9824,  9856,  9888,  9920,  9952,  9984,  10016, 10048, 10080, 10112, 10144, 10176, 10208,
    10240, 10272, 10304, 10336, 10368, 10400, 10432, 10464, 10496, 10528, 10560, 10592, 10624, 10656, 10688, 10720,
    10752, 10784, 10816, 10848, 10880, 10912, 10944, 10976, 11008, 11040, 11072, 11104, 11136, 11168, 11200, 11232,
    11264, 11296, 11328, 11360, 11392, 11424, 11456, 11488, 11520, 11552, 11584, 11616, 11648, 11680, 11712, 11744,
    11776, 11808, 11840, 11872, 11904, 11936, 11968, 12000, 12032, 12064, 12096, 12128, 12160, 12192, 12224, 12256,
    12288, 12320, 12352, 12384, 12416, 12448, 12480, 12512, 12544, 12576, 12608, 12640, 12672, 12704, 12736, 12768,
    12800, 12832, 12864, 12896, 12928, 12960, 12992, 13024, 13056, 13088, 13120, 13152, 13184, 13216, 13248, 13280,
    13312, 13344, 13376, 13408, 13440, 13472, 13504, 13536, 13568, 13600, 13632, 13664, 13696, 13728, 13760, 13792,
    13824, 13856, 13888, 13920, 13952, 13984, 14016, 14048, 14080, 14112, 14144, 14176, 14208, 14240, 14272, 14304,
    14336, 14368, 14400, 14432, 14464, 14496, 14528, 14560, 14592, 14624, 14656, 14688, 14720, 14752, 14784, 14816,
    14848, 14880, 14912, 14944, 14976, 15008, 15040, 15072, 15104, 15136, 15168, 15200, 15232, 15264, 15296, 15328,
    15360, 15392, 15424, 15456, 15488, 15520, 15552, 15584, 15616, 15648, 15680, 15712, 15744, 15776, 15808, 15840,
    15872, 15904, 15936, 15968, 16000, 16032, 16064, 16096, 16128, 16160, 16192, 16224, 16256, 16288, 16320, 16352,
    16384, 16416, 16448, 16480, 16512, 16544, 16576, 16608, 16640, 16672, 16704, 16736, 16768, 16800, 16832, 16864,
    16896, 16928, 16960, 16992, 17024, 17056, 17088, 17120, 17152, 17184, 17216, 17248, 17280, 17312, 17344, 17376,
    17408, 17440, 17472, 17504, 17536, 17568, 17600, 17632, 17664, 17696, 17728, 17760, 17792, 17824, 17856, 17888,
    17920, 17952, 17984, 18016, 18048, 18080, 18112, 18144, 18176, 18208, 18240, 18272, 18304, 18336, 18368, 18400,
    18432, 18464, 18496, 18528, 18560, 18592, 18624, 18656, 18688, 18720, 18752, 18784, 18816, 18848, 18880, 18912,
    18944, 18976, 19008, 19040, 19072, 19104, 19136, 19168, 19200, 19232, 19264, 19296, 19328, 19360, 19392, 19424,
    19456, 19488, 19520, 19552, 19584, 19616, 19648, 19680, 19712, 19744, 19776, 19808, 19840, 19872, 19904, 19936,
    19968, 20000, 20032, 20064, 20096, 20128, 20160, 20192, 20224, 20256, 20288, 20320, 20352, 20384, 20416, 20448,
    20480, 20512, 20544, 20576, 20608, 20640, 20672, 20704, 20736, 20768, 20800, 20832, 20864, 20896, 20928, 20960,
    20992, 21024, 21056, 21088, 21120, 21152, 21184, 21216, 21248, 21280, 21312, 21344, 21376, 21408, 21440, 21472,
    21504, 21536, 21568, 21600, 21632, 21664, 21696, 21728, 21760, 21792, 21824, 21856, 21888, 21920, 21952, 21984,
    22016, 22048, 22080, 22112, 22144, 22176, 22208, 22240, 22272, 22304, 22336, 22368, 22400, 22432, 22464, 22496,
    22528, 22560, 22592, 22624, 22656, 22688, 22720, 22752, 22784, 22816, 22848, 22880, 22912, 22944, 22976, 23008,
    23040, 23072, 23104, 23136, 23168, 23200, 23232, 23264, 23296, 23328, 23360, 23392, 23424, 23456, 23488, 23520,
    23552, 23584, 23616, 23648, 23680, 23712, 23744, 23776, 23808, 23840, 23872, 23904, 23936, 23968, 24000, 24032,
    24064, 24096, 24128, 24160, 24192, 24224, 24256, 24288, 24320, 24352, 24384, 24416, 24448, 24480, 24512, 24544,
    24576, 24608, 24640, 24672, 24704, 24736, 24768, 24800, 24832, 24864, 24896, 24928, 24960, 24992, 25024, 25056,
    25088, 25120, 25152, 25184, 25216, 25248, 25280, 25312, 25344, 25376, 25408, 25440, 25472, 25504, 25536, 25568,
    25600, 25632, 25664, 25696, 25728, 25760, 25792, 25824, 25856, 25888, 25920, 25952, 25984, 26016, 26048, 26080,
    26112, 26144, 26176, 26208, 26240, 26272, 26304, 26336, 26368, 26400, 26432, 26464, 26496, 26528, 26560, 26592,
    26624, 26656, 26688, 26720, 26752, 26784, 26816, 26848, 26880, 26912, 26944, 26976, 27008, 27040, 27072, 27104,
    27136, 27168, 27200, 27232, 27264, 27296, 27328, 27360, 27392, 27424, 27456, 27488, 27520, 27552, 27584, 27616,
    27648, 27680, 27712, 27744, 27776, 27808, 27840, 27872, 27904, 27936, 27968, 28000, 28032, 28064, 28096, 28128,
    28160, 28192, 28224, 28256, 28288, 28320, 28352, 28384, 28416, 28448, 28480, 28512, 28544, 28576, 28608, 28640,
    28672, 28704, 28736, 28768, 28800, 28832, 28864, 28896, 28928, 28960, 28992, 29024, 29056, 29088, 29120, 29152,
    29184, 29216, 29248, 29280, 29312, 29344, 29376, 29408, 29440, 29472, 29504, 29536, 29568, 29600, 29632, 29664,
    29696, 29728, 29760, 29792, 29824, 29856, 29888, 29920, 29952, 29984, 30016, 30048, 30080, 30112, 30144, 30176,
    30208, 30240, 30272, 30304, 30336, 30368, 30400, 30432, 30464, 30496, 30528, 30560, 30592, 30624, 30656, 30688,
    30720, 30752, 30784, 30816, 30848, 30880, 30912, 30944, 30976, 31008, 31040, 31072, 31104, 31136, 31168, 31200,
    31232, 31264, 31296, 31328, 31360, 31392, 31424, 31456, 31488, 31520, 31552, 31584, 31616, 31648, 31680, 31712,
    31744, 31776, 31808, 31840, 31872, 31904, 31936, 31968, 32000, 32032, 32064, 32096, 32128, 32160, 32192, 32224,
    32256, 32288, 32320, 32352, 32384, 32416, 32448, 32480, 32512, 32544, 32576, 32608, 32640, 32672, 32704, 32736
};

static const xmedia_sensor_capability g_os04j10_capability = {
    .max_width  = 2560,
    .max_height = 1440,
    .max_fps    = OS04J10_MAX_FPS_LINEAR,

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
        .addr_byte_num     = OS04J10_ADDR_BYTE,
        .data_byte_num     = OS04J10_DATA_BYTE,
        .standby_reg_num   = 2,
        .standby_reg_addr  = { 0xfd, 0x09 },
        .standby_reg_data  = { 0x00, 0x07 },
        .resume_reg_num    = 14,
        .resume_reg_addr   = { 0xfd, 0x09, XMEDIA_SENSOR_DELAY_FLAG, 0x59, 0xfe, XMEDIA_SENSOR_DELAY_FLAG, 0xfb,
                               0xfd, 0x08, 0x09, 0xfd, 0x28, 0x2c, 0xfb },
        .resume_reg_data   = { 0x00, 0x00, 0x2, 0x00, 0x01, 0x1, 0x00, 0x01, 0x05, 0xa8, 0x00, 0xfe, 0x01, 0x03 },
    },
};

static xmedia_s32 os04j10_ctx_init(xmedia_u32 dev)
{
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    OS04J10_GET_CTX(dev, os04j10_ctx);
    if (os04j10_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    os04j10_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (os04j10_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(os04j10_ctx, 0, sizeof(sensor_context));
    os04j10_ctx->i2c_addr         = OS04J10_I2C_ADDR;
    os04j10_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_4L;
    os04j10_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    os04j10_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    os04j10_ctx->size.width       = 2688;
    os04j10_ctx->size.height      = 1520;
    os04j10_ctx->fps              = g_os04j10_property[0].max_fps;
    os04j10_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    os04j10_ctx->img_mode             = OS04J10_4M_LINEAR_MODE;
    os04j10_ctx->fl_std               = OS04J10_VMAX_4M_LINEAR;
    os04j10_ctx->fl[SENSOR_CUR_FRAME] = OS04J10_VMAX_4M_LINEAR;
    os04j10_ctx->fl[SENSOR_PRE_FRAME] = OS04J10_VMAX_4M_LINEAR;

    OS04J10_SET_CTX(dev, os04j10_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void os04j10_ctx_exit(xmedia_u32 dev)
{
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_FREE(os04j10_ctx);
    OS04J10_SET_CTX(dev, XMEDIA_NULL);
}

static xmedia_s32 os04j10_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    for (i = 0; i < OS04J10_SPECS_MAX_NUM; i++) {
        if (g_os04j10_property[i].width == os04j10_ctx->size.width &&
            g_os04j10_property[i].height == os04j10_ctx->size.height &&
            g_os04j10_property[i].wdr_mode == os04j10_ctx->wdr_mode) {
            memcpy(property, &g_os04j10_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n",
                    os04j10_ctx->size.width, os04j10_ctx->size.height, os04j10_ctx->wdr_mode);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 os04j10_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os04j10_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "No support mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os04j10_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        os04j10_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        os04j10_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        os04j10_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    os04j10_ctx->i2c_addr = slave_addr;
    ret = os04j10_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    *slave_addr = os04j10_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}


static xmedia_s32 os04j10_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    os04j10_ctx->init_mode = init_mode;
    ret = os04j10_i2c_init(dev, os04j10_ctx->bus_info.i2c_dev, os04j10_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    os04j10_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    ret = os04j10_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    os04j10_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    os04j10_ctx->mirror_en = mirror_en;
    ret = os04j10_set_mirror_flip(dev, mirror_en, os04j10_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    os04j10_ctx->flip_en = flip_en;
    ret = os04j10_set_mirror_flip(dev, os04j10_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_set_image_mode(sensor_context *os04j10_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (os04j10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (OS04J10_RES_IS_1440P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OS04J10_4M_LINEAR_MODE;
            os04j10_ctx->fl_std = OS04J10_VMAX_4M_LINEAR ;
        } else {
            OS04J10_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else if (os04j10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        OS04J10_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os04j10_ctx->size.width  = sns_attr->width;
    os04j10_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            os04j10_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

//        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
//            os04j10_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
//            SENSOR_PRINT("2to1 line WDR\n");
//            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(os04j10_ctx->wdr_int_time, 0, sizeof(os04j10_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

#define OS04J10_LINEAR_REG_INFO_MAX_NUM   OS04J10_REG_L_MAX_NUM
#define OS04J10_2TO1_WDR_REG_INFO_MAX_NUM OS04J10_REG_MAX_NUM
static xmedia_void os04j10_init_common_reg_info(sensor_context *os04j10_ctx)
{
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_PAGE].delay_frame_num  = 2;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_PAGE].reg_addr         = OS04J10_REG_ADDR_PAGE;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_PAGE].data             = 1;

    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_EXP_H].delay_frame_num = 2;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_EXP_H].reg_addr        = OS04J10_REG_ADDR_EXP_H;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_EXP_L].delay_frame_num = 2;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_EXP_L].reg_addr        = OS04J10_REG_ADDR_EXP_L;

    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_DGAIN_H].delay_frame_num = 2;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_DGAIN_H].reg_addr        = OS04J10_REG_ADDR_DGAIN_H;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_DGAIN_L].delay_frame_num = 2;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_DGAIN_L].reg_addr        = OS04J10_REG_ADDR_DGAIN_L;

    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_AGAIN].delay_frame_num = 2;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_AGAIN].reg_addr        = OS04J10_REG_ADDR_AGAIN;

    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_SCG_EN].delay_frame_num = 2;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_SCG_EN].reg_addr        = OS04J10_REG_ADDR_SCG_EN;

    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_VBLANK_H].delay_frame_num = 2;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_VBLANK_H].reg_addr        = OS04J10_REG_ADDR_VBLANK_H;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_VBLANK_L].delay_frame_num = 2;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_VBLANK_L].reg_addr        = OS04J10_REG_ADDR_VBLANK_L;

    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_TRIG].delay_frame_num   = 2;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_TRIG].reg_addr          = OS04J10_REG_ADDR_TRIG;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_TRIG].data              = 2;
    return;
}

static xmedia_void os04j10_init_2to1_wdr_reg_info(sensor_context *os04j10_ctx)
{
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_S_EXP_H].delay_frame_num = 2;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_S_EXP_H].reg_addr        = OS04J10_REG_ADDR_S_EXP_H;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_S_EXP_L].delay_frame_num = 2;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_S_EXP_L].reg_addr        = OS04J10_REG_ADDR_S_EXP_L;

    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_S_DGAIN_H].delay_frame_num = 2;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_S_DGAIN_H].reg_addr        = OS04J10_REG_ADDR_S_DGAIN_H;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_S_DGAIN_L].delay_frame_num = 2;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_S_DGAIN_L].reg_addr        = OS04J10_REG_ADDR_S_DGAIN_L;

    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_S_AGAIN].delay_frame_num = 2;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_S_AGAIN].reg_addr        = OS04J10_REG_ADDR_S_AGAIN;

    return;
}

static xmedia_s32 os04j10_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = os04j10_ctx->bus_info.i2c_dev;
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = OS04J10_LINEAR_REG_INFO_MAX_NUM;

     if (os04j10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
         os04j10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num = OS04J10_2TO1_WDR_REG_INFO_MAX_NUM;
     }

    for (i = 0; i < os04j10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = os04j10_ctx->i2c_addr;
        os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = OS04J10_ADDR_BYTE;
        os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = OS04J10_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    os04j10_init_common_reg_info(os04j10_ctx);

    // init 2to1 wdr mode Regs
    if (os04j10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        os04j10_init_2to1_wdr_reg_info(os04j10_ctx);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    // Set wdr mode
    ret = os04j10_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = os04j10_set_image_mode(os04j10_ctx, &os04j10_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    os04j10_ctx->fl[SENSOR_CUR_FRAME] = os04j10_ctx->fl_std;
    os04j10_ctx->fl[SENSOR_PRE_FRAME] = os04j10_ctx->fl[SENSOR_CUR_FRAME];

    ret = os04j10_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef OS04J10_ISP_DEFAULT_SUPPORT
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from os04j10!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 os04j10_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (os04j10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
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

static xmedia_s32 os04j10_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    if (os04j10_ctx->mirror_en && os04j10_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (os04j10_ctx->mirror_en && (!os04j10_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!os04j10_ctx->mirror_en) && os04j10_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < OS04J10_SPECS_MAX_NUM; i++) {
        if (g_os04j10_property[i].width == os04j10_ctx->size.width &&
            g_os04j10_property[i].height == os04j10_ctx->size.height &&
            g_os04j10_property[i].wdr_mode == os04j10_ctx->wdr_mode) {
            *bayer_pattern = g_os04j10_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= OS04J10_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_os04j10_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os04j10_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 os04j10_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_os04j10_exposure[dev]      = init_param->exposure;
    g_os04j10_sample_r_gain[dev] = init_param->sample_rgain;
    g_os04j10_sample_b_gain[dev] = init_param->sample_bgain;

    g_os04j10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_os04j10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_os04j10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

xmedia_s32 os04j10_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case OS04J10_4M_LINEAR_MODE:
            *max_fps = OS04J10_MAX_FPS_LINEAR;
            break;
        case OS04J10_4M_WDR_MODE:
            *max_fps = OS04J10_MAX_FPS_WDR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = os04j10_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256; // light flicker freq: 50Hz, accuracy: 256
    ae_sns_dft->full_lines_max = OS04J10_FULL_LINES_MAX;

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 0.0625; // accuracy: 0.0625 - 1/16

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->dgain_accu.accuracy  = 0.03125; //1/32

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 32 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->ispdgain_sep_thd    = 32 << ae_sns_dft->ispdgain_shift;

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

static xmedia_void os04j10_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->max_again        = 63488; // max 1024 * 15.5 * g_dcg_ratio
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = 32736;
    ae_sns_dft->min_dgain = 1024;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure   = g_os04j10_exposure[dev] ? g_os04j10_exposure[dev] : 52000;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - OS04J10_EXP_OFFSET;
    ae_sns_dft->min_int_time        = 2;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    return;
}

static xmedia_s32 os04j10_get_ae_2to1_wdr_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                              xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    ae_sns_dft->max_int_time         = os04j10_ctx->fl_std - OS04J10_EXP_OFFSET;
    ae_sns_dft->min_int_time         = 2;
    ae_sns_dft->int_time_accu.offset = 0;

    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    ae_sns_dft->max_again        = 16 * 16 - 1;
    ae_sns_dft->min_again        = 16;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain         = 32 * 32 - 1;
    ae_sns_dft->min_dgain         = 32;
    ae_sns_dft->max_dgain_target  = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target  = ae_sns_dft->min_dgain;
    ae_sns_dft->diff_gain_support = XMEDIA_TRUE;
    ae_sns_dft->init_exposure     = g_os04j10_exposure[dev] ? g_os04j10_exposure[dev] : 52000;

    if (os04j10_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
        ae_sns_dft->ae_compensation = 56;
        ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    } else {
        ae_sns_dft->ae_compensation  = 32;
        ae_sns_dft->ae_exp_mode      = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
        ae_sns_dft->man_ratio_enable = XMEDIA_FALSE;

        ae_sns_dft->ratio[0] = 0x400;
        ae_sns_dft->ratio[1] = 0x40;
        ae_sns_dft->ratio[2] = 0x40;
    }

    return XMEDIA_SUCCESS;

}

static xmedia_s32 os04j10_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    memset(ae_sns_dft, 0, sizeof(xmedia_isp_ae_sensor_default));

    ret = os04j10_get_ae_common_default(dev, os04j10_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (os04j10_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            os04j10_get_ae_linear_default(dev, os04j10_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            ret = os04j10_get_ae_2to1_wdr_default(dev, os04j10_ctx, ae_sns_dft);
            SENSOR_CHECK_RET_RETURN(ret);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", os04j10_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context    *os04j10_ctx                      = XMEDIA_NULL;
    static xmedia_bool first[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 1 };

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    if (os04j10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (first[dev]) { // 0: short exposure
            os04j10_ctx->wdr_int_time[0] = int_time;
            os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_S_EXP_H].data = SENSOR_HIGH_8BITS(int_time);
            os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_S_EXP_L].data = SENSOR_LOW_8BITS(int_time);
            first[dev] = XMEDIA_FALSE;
        } else { // 1: long exposure
            os04j10_ctx->wdr_int_time[1] = int_time;
            os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_EXP_H].data = SENSOR_HIGH_8BITS(int_time);
            os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_EXP_L].data = SENSOR_LOW_8BITS(int_time);
            first[dev] = XMEDIA_TRUE;
        }
    } else if (os04j10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_EXP_H].data = SENSOR_HIGH_8BITS(int_time);
        os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_EXP_L].data = SENSOR_LOW_8BITS(int_time);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_u32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_os04j10_again_table[OS04J10_AGAIN_MAX_NUM - 1]) {
        *again_lin = g_os04j10_again_table[OS04J10_AGAIN_MAX_NUM - 1];
        *again_db  = g_os04j10_again_table[OS04J10_AGAIN_MAX_NUM - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < OS04J10_AGAIN_MAX_NUM; i++) {
        if (*again_lin < g_os04j10_again_table[i]) {
            *again_lin = g_os04j10_again_table[i - 1];
            *again_db  = g_os04j10_again_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    xmedia_u32 i = OS04J10_DGAIN_MAX_NUM;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(dgain_lin);
    SENSOR_CHECK_PTR_RETURN(dgain_db);

    if (*dgain_lin >= g_os04j10_dgain_table[OS04J10_DGAIN_MAX_NUM - 1]) {
        *dgain_lin = g_os04j10_dgain_table[OS04J10_DGAIN_MAX_NUM - 1];
        *dgain_db = g_os04j10_dgain_table[OS04J10_DGAIN_MAX_NUM - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < OS04J10_DGAIN_MAX_NUM; i++) {
        if (*dgain_lin < g_os04j10_dgain_table[i]) {
            *dgain_lin = g_os04j10_dgain_table[i - 1];
            *dgain_db = g_os04j10_dgain_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

xmedia_u32 g_dcg_ratio[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
xmedia_void os04j10_set_dcg_ratio(xmedia_u32 dev, xmedia_u32 dcg_ratio)
{
    g_dcg_ratio[dev] = dcg_ratio;
}

static xmedia_s32 os04j10_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *os04j10_ctx = XMEDIA_NULL;
    xmedia_u32      dgain_reg   = 0x00;
    xmedia_bool    dcg_en       = 0;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    if(again < 1024) { //1x
        again = 1024;
        dcg_en = 0;
    } else if (again >= 1024 && again < g_dcg_ratio[dev] * 1024) { // 小于4.5x
        again = again;
        dcg_en = 0;
    } else if (again >= g_dcg_ratio[dev] * 1024 && again < 71424) { // 大于4.5x 小于 15.5x  切换到hcg
        again = again / g_dcg_ratio[dev];
        dcg_en = 1;
    } else if (again >= 71424 ) { // 大于15.5x
        again = 15872; // 15.5x
        dcg_en = 1;
    }

    again = again / 64;

    dgain_reg = dgain / 1024 * 64 + dgain % 1024 / 32 * 2; // div 1024.0 x32.0

    dgain_reg = dgain_reg & 0x7FE;

    os04j10_ctx->regs_info[0].i2c_data[OS04J10_REG_AGAIN].data    = again;
    os04j10_ctx->regs_info[0].i2c_data[OS04J10_REG_SCG_EN].data   = dcg_en;
    os04j10_ctx->regs_info[0].i2c_data[OS04J10_REG_DGAIN_H].data  = SENSOR_HIGH_8BITS(dgain_reg);
    os04j10_ctx->regs_info[0].i2c_data[OS04J10_REG_DGAIN_L].data  = dgain_reg & 0xFE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = 5000;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 5: point index on the awb curve param
    awb_sns_dft->wb_para[0] = CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1] = CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2] = CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3] = CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4] = CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5] = CALIBRATE_AWB_C1;

    awb_sns_dft->golden_rgain = GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (os04j10_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_os04j10_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_os04j10_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_os04j10_awb_ccm_wdr, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_os04j10_awb_agc_table_wdr, sizeof(xmedia_isp_awb_agc_table));
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", os04j10_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_os04j10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_os04j10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_os04j10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_os04j10_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_os04j10_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_standby(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os04j10_write_reg(dev, 0xfd, 0x00);
    ret = os04j10_write_reg(dev, 0x28, 0xfc);
    ret = os04j10_write_reg(dev, 0x2c, 0x00);
    ret = os04j10_write_reg(dev, 0xea, 0x01);
    ret = os04j10_write_reg(dev, 0xea, 0x00);
    ret = os04j10_write_reg(dev, 0x59, 0x03);
    ret = os04j10_write_reg(dev, 0xfe, 0x01);
    ret = os04j10_write_reg(dev, 0xfb, 0x00);
    ret = os04j10_write_reg(dev, 0xfd, 0x01);
    ret = os04j10_write_reg(dev, 0x08, 0x00);
    ret = os04j10_write_reg(dev, 0x09, 0x0a);
    delay_ms(2);
    ret = os04j10_write_reg(dev, 0xfd, 0x00);
    ret = os04j10_write_reg(dev, 0xfd, 0x00);
    ret = os04j10_write_reg(dev, 0x09, 0x07);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_resume(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os04j10_write_reg(dev, 0xfd, 0x00);
    ret = os04j10_write_reg(dev, 0x09, 0x00);
    delay_ms(2);
    ret = os04j10_write_reg(dev, 0x59, 0x00);
    ret = os04j10_write_reg(dev, 0xfe, 0x01);
    delay_ms(1);
    ret = os04j10_write_reg(dev, 0xfb, 0x00);
    ret = os04j10_write_reg(dev, 0xfd, 0x01);
    ret = os04j10_write_reg(dev, 0x08, 0x05);
    ret = os04j10_write_reg(dev, 0x09, 0xa8);
    ret = os04j10_write_reg(dev, 0xfd, 0x00);
    ret = os04j10_write_reg(dev, 0x28, 0xfe);
    ret = os04j10_write_reg(dev, 0x2c, 0x01);
    ret = os04j10_write_reg(dev, 0xfb, 0x03);

    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_void os04j10_delay_ms(xmedia_s32 ms)
{
    sleep_us(ms * 1000);
}

static xmedia_s32 os04j10_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *os04j10_ctx  = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < os04j10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < os04j10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= os04j10_write_reg(dev, os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    if (os04j10_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = os04j10_init_image(dev, os04j10_ctx->img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    os04j10_delay_ms(10);
    ret = os04j10_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = os04j10_set_mirror_flip(dev, os04j10_ctx->mirror_en, os04j10_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os04j10_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_os04j10_dev_map[index] == SENSOR_DEV_INVALID) {
            g_os04j10_dev_map[index] = pipe;
            *dev                     = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are binded to pipe!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 os04j10_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_os04j10_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 os04j10_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{ // TO-DO：min fps is to be confirmed.
    switch (img_mode) {
        case OS04J10_4M_LINEAR_MODE:
        case OS04J10_4M_WDR_MODE:
            *min_fps = 1.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case OS04J10_4M_LINEAR_MODE:
            *vmax = OS04J10_VMAX_4M_LINEAR;
            break;

        case OS04J10_4M_WDR_MODE:
            *vmax = OS04J10_VMAX_4M_WDR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = os04j10_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04j10_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04j10_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is (%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, OS04J10_FULL_LINES_MAX);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    xmedia_bool     update = XMEDIA_FALSE;
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    for (i = 0; i < os04j10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            os04j10_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
            update = XMEDIA_TRUE;
        }
    }

    if(update == XMEDIA_TRUE){
        os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_PAGE].update = XMEDIA_TRUE;
        os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_TRIG].update = XMEDIA_TRUE;
    }

    memcpy(sns_regs_info, &os04j10_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&os04j10_ctx->regs_info[SENSOR_PRE_FRAME], &os04j10_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    os04j10_ctx->fl[SENSOR_PRE_FRAME] = os04j10_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_u32      vblank;
    xmedia_s32      ret;
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    ret = os04j10_calc_fps(fps, os04j10_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    vblank = full_lines - OS04J10_VMAX_4M_LINEAR + 16;

    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_VBLANK_H].data = SENSOR_HIGH_8BITS(vblank);
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_VBLANK_L].data = SENSOR_LOW_8BITS(vblank);

    os04j10_ctx->fl[SENSOR_CUR_FRAME] = full_lines ;
    os04j10_ctx->fl_std               = os04j10_ctx->fl[SENSOR_CUR_FRAME];
    os04j10_ctx->fps                  = fps;
    ae_sns_dft->fps                   = fps;
    ae_sns_dft->full_lines_std        = os04j10_ctx->fl_std;
    ae_sns_dft->full_lines            = os04j10_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time          = os04j10_ctx->fl[SENSOR_CUR_FRAME] - OS04J10_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]- os04j10 set fps: %f\n", dev, fps);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = os04j10_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04j10_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04j10_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = vmax_max_fps * max_fps / min_fps;
    return XMEDIA_SUCCESS;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 os04j10_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    xmedia_u32 short_max_pre_frame;
    xmedia_u32 short_max;
    xmedia_u32 short_time_min_limit;
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);
    memcpy(os04j10_ctx->ratio, inttime_attr->ratio, sizeof(inttime_attr->ratio));

    short_time_min_limit = 2;
    short_max            = 2;
    if (os04j10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (os04j10_ctx->ae_wdr_mode == XMEDIA_ISP_AE_WDR_MODE_LONG_FRAME) {
            short_max_pre_frame = os04j10_ctx->fl[SENSOR_PRE_FRAME] - 20 - os04j10_ctx->wdr_int_time[0];
            short_max = os04j10_ctx->fl[SENSOR_CUR_FRAME] - 20; // sensor limit: sub 20
            short_max = (short_max_pre_frame < short_max) ? short_max_pre_frame : short_max;
            inttime_attr->max_inttime[0] = short_time_min_limit;  // index 0：short frame
            inttime_attr->min_inttime[0] = short_time_min_limit;
            inttime_attr->max_inttime[1] = short_max;  // index 1：long frame
            inttime_attr->min_inttime[1] = short_time_min_limit;
            return XMEDIA_SUCCESS;
        } else {
            short_max_pre_frame = ((os04j10_ctx->fl[SENSOR_PRE_FRAME] - 20 - os04j10_ctx->wdr_int_time[0]) * 0x40) /
                                  SENSOR_DIV_0_TO_1(os04j10_ctx->ratio[0]);
            short_max = ((os04j10_ctx->fl[SENSOR_CUR_FRAME] - 20) * 0x40) / (os04j10_ctx->ratio[0] + 0x40);
            short_max = (short_max_pre_frame < short_max) ? short_max_pre_frame : short_max;
            short_max = (short_max == 0) ? 1 : short_max;
        }
    }

    if (short_max >= short_time_min_limit) { // 0：short frame; 1：long frame
        inttime_attr->max_inttime[0] = short_max;
        inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * os04j10_ctx->ratio[0]) >> 6;
        inttime_attr->min_inttime[0] = short_time_min_limit;
        inttime_attr->min_inttime[1] = (inttime_attr->min_inttime[0] * os04j10_ctx->ratio[0]) >> 6;
    } else {
        short_max = short_time_min_limit;
        inttime_attr->max_inttime[0] = short_max;
        inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * 0xFFF) >> 6;
        inttime_attr->min_inttime[0] = inttime_attr->max_inttime[0];
        inttime_attr->min_inttime[1] = inttime_attr->max_inttime[1];
    }
    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 */
static xmedia_s32 os04j10_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *os04j10_ctx = XMEDIA_NULL;
    xmedia_float    min_fps;
    xmedia_u32      vmax_min_fps;
    xmedia_s32      ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    if (os04j10_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", os04j10_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = os04j10_get_min_fps(os04j10_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04j10_get_min_fps_vmax(os04j10_ctx->img_mode, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                        = full_lines + OS04J10_EXP_OFFSET;
    full_lines                        = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    os04j10_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    os04j10_ctx->fps                  = min_fps * vmax_min_fps / full_lines;

    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_VBLANK_H].data = SENSOR_HIGH_8BITS(full_lines);
    os04j10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04J10_REG_VBLANK_L].data = SENSOR_LOW_8BITS(full_lines);

    ae_sns_dft->full_lines   = os04j10_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = os04j10_ctx->fl[SENSOR_CUR_FRAME] - OS04J10_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]- os04j10 set fps: %f\n", dev, os04j10_ctx->fps);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    ret = os04j10_get_min_fps(os04j10_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04j10_get_max_fps(os04j10_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = os04j10_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04j10_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, OS04J10_NAME, sizeof(OS04J10_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = os04j10_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = os04j10_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = os04j10_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = os04j10_mirror;
    info->isp_func.pfn_sensor_flip             = os04j10_flip;
    info->isp_func.pfn_sensor_set_bus_info     = os04j10_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = os04j10_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = os04j10_set_init_param;
    info->isp_func.pfn_sensor_start            = os04j10_start;
    info->isp_func.pfn_sensor_stop             = os04j10_stop;
    info->isp_func.pfn_sensor_standby          = os04j10_standby;
    info->isp_func.pfn_sensor_resume           = os04j10_resume;
    info->isp_func.pfn_sensor_write_reg        = os04j10_write_reg;
    info->isp_func.pfn_sensor_read_reg         = os04j10_read_reg;
    info->isp_func.pfn_sensor_init             = os04j10_init;
    info->isp_func.pfn_sensor_exit             = os04j10_exit;
    info->isp_func.pfn_sensor_set_attr         = os04j10_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = os04j10_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = os04j10_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = os04j10_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = os04j10_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = os04j10_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = os04j10_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = os04j10_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = os04j10_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = os04j10_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = os04j10_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = os04j10_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = os04j10_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = os04j10_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = os04j10_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = os04j10_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = os04j10_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = os04j10_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 os04j10_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = os04j10_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = os04j10_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = os04j10_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OS04J10 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 os04j10_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *os04j10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = os04j10_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    OS04J10_GET_CTX(dev, os04j10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04j10_ctx);

    if (os04j10_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OS04J10 unregister function failed!\n");
        return ret;
    }

    os04j10_ctx_exit(dev);
    g_os04j10_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;
}
