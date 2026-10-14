#include <stdlib.h>
#include <string.h>
#include "sc235hai.h"
#include "sc235hai_ctrl.h"
#include "sc235hai_ex.h"
#include "xmedia_isp.h"

#define SC235HAI_NAME          "SC235HAI"
#define SC235HAI_SPECS_MAX_NUM 1

#ifdef __linux__
#define SC235HAI_ISP_DEFAULT_SUPPORT
#endif

#define SC235HAI_2M_1080P_BIT_RATE_LINEAR  756 // 371

#define SC235HAI_REG_ADDR_LOW_POWER_STANDBY 0x302c
#define SC235HAI_REG_ADDR_STANDBY 0x0100

#define SC235HAI_RES_IS_1080P(w, h) ((w) == 1920 && (h) == 1080)

#define SC235HAI_ERR_MODE_PRINT(sns_attr)                                                                           \
    do {                                                                                                            \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sns_attr->width,             \
                     sns_attr->height, sns_attr->wdr_mode);                                                         \
    } while (0)

// awb static param for Fuji Lens New IR_Cut
#define SC235HAI_CALIBRATE_STATIC_TEMP       5000
#define SC235HAI_CALIBRATE_STATIC_WB_R_GAIN  433
#define SC235HAI_CALIBRATE_STATIC_WB_GR_GAIN 256
#define SC235HAI_CALIBRATE_STATIC_WB_GB_GAIN 256
#define SC235HAI_CALIBRATE_STATIC_WB_B_GAIN  653

// Calibration results for Auto WB Planck
#define SC235HAI_CALIBRATE_AWB_P1 165
#define SC235HAI_CALIBRATE_AWB_P2 (-89)
#define SC235HAI_CALIBRATE_AWB_Q1 (-177)
#define SC235HAI_CALIBRATE_AWB_A1 200851
#define SC235HAI_CALIBRATE_AWB_B1 128
#define SC235HAI_CALIBRATE_AWB_C1 (-139601)

// Rgain and Bgain of the golden sample
#define SC235HAI_GOLDEN_RGAIN 0
#define SC235HAI_GOLDEN_BGAIN 0

#define SC235HAI_AGAIN_INDEX_MAX 220
#define SC235HAI_DGAIN_INDEX_MAX 128

SENSOR_PRIORITY_DATA sensor_context *g_sc235hai_ctx[XMEDIA_SENSOR_DEV_MAX_NUM]     = { XMEDIA_NULL };
#define SC235HAI_SET_CONTEXT(dev, sns_ctx) g_sc235hai_ctx[dev] = sns_ctx
#define SC235HAI_GET_CONTEXT(dev, sns_ctx) sns_ctx = g_sc235hai_ctx[dev]

static xmedia_u32 g_sc235hai_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_sc235hai_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_sc235hai_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_sc235hai_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };
static xmedia_u16 g_sc235hai_init_ccm[XMEDIA_SENSOR_DEV_MAX_NUM][CCM_MATRIX_SIZE]             = { { 0 } };

SENSOR_PRIORITY_DATA static xmedia_s32 g_sc235hai_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

SENSOR_PRIORITY_RODATA static const xmedia_sensor_property g_sc235hai_property[SC235HAI_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        1920, 1080, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_NONE, 60, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC235HAI_2M_1080P_BIT_RATE_LINEAR,
    },
};

static const xmedia_u32 g_sc235hai_again_index[SC235HAI_AGAIN_INDEX_MAX] = {
    1024,   1056,   1089,   1120,   1152,   1184,   1217,   1248,   1280,   1312,   1345,  1376,  1408,  1440,  1473,
    1504,   1536,   1568,   1601,   1632,   1664,   1696,   1729,   1760,   1792,   1824,  1857,  1888,  1920,  1952,
    1985,   2016,   2048,   2113,   2176,   2241,   2304,   2369,   2432,   2497,   2560,  2625,  2688,  2753,  2816,
    2881,   2944,   3009,   3072,   3137,   3200,   3265,   3328,   3393,   3456,   3521,  3584,  3649,  3712,  3777,
    3789,   3908,   4025,   4144,   4263,   4381,   4499,   4617,   4736,   4855,   4973,  5091,  5210,  5328,  5447,
    5564,   5683,   5802,   5920,   6039,   6157,   6275,   6394,   6512,   6630,   6749,  6867,  6986,  7105,  7222,
    7341,   7459,   7578,   7814,   8052,   8288,   8525,   8761,   8999,   9235,   9472,  9709,  9946,  10183, 10419,
    10656,  10893,  11130,  11366,  11603,  11841,  12077,  12314,  12550,  12788,  13024, 13261, 13497, 13735, 13971,
    14208,  14445,  14682,  14919,  15155,  15629,  16102,  16577,  17050,  17524,  17997, 18471, 18944, 19418, 19891,
    20365,  20838,  21313,  21786,  22260,  22733,  23207,  23680,  24154,  24627,  25101, 25574, 26049, 26522, 26996,
    27469,  27943,  28416,  28890,  29363,  29837,  30310,  31258,  32205,  33152,  34099, 35046, 35994, 36941, 37888,
    38835,  39782,  40730,  41677,  42624,  43571,  44518,  45466,  46413,  47360,  48307, 49254, 50202, 51149, 52096,
    53043,  53990,  54938,  55885,  56832,  57779,  58726,  59674,  60621,  62515,  64410, 66304, 68198, 70093, 71987,
    73882,  75776,  77670,  79565,  81459,  83354,  85248,  87142,  89037,  90931,  92826, 94720, 96614, 98509, 100403,
    102298, 104192, 106086, 107981, 109875, 111770, 113664, 115558, 117453, 119347,
};

static const xmedia_u32 g_sc235hai_again_table[SC235HAI_AGAIN_INDEX_MAX] = {
    0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D,
    0x002E, 0x002F, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B,
    0x003C, 0x003D, 0x003E, 0x003F, 0x0120, 0x0121, 0x0122, 0x0123, 0x0124, 0x0125, 0x0126, 0x0127, 0x0128, 0x0129,
    0x012A, 0x012B, 0x012C, 0x012D, 0x012E, 0x012F, 0x0130, 0x0131, 0x0132, 0x0133, 0x0134, 0x0135, 0x0136, 0x0137,
    0x0138, 0x0139, 0x013A, 0x013B, 0x8020, 0x8021, 0x8022, 0x8023, 0x8024, 0x8025, 0x8026, 0x8027, 0x8028, 0x8029,
    0x802A, 0x802B, 0x802C, 0x802D, 0x802E, 0x802F, 0x8030, 0x8031, 0x8032, 0x8033, 0x8034, 0x8035, 0x8036, 0x8037,
    0x8038, 0x8039, 0x803A, 0x803B, 0x803C, 0x803D, 0x803E, 0x803F, 0x8120, 0x8121, 0x8122, 0x8123, 0x8124, 0x8125,
    0x8126, 0x8127, 0x8128, 0x8129, 0x812A, 0x812B, 0x812C, 0x812D, 0x812E, 0x812F, 0x8130, 0x8131, 0x8132, 0x8133,
    0x8134, 0x8135, 0x8136, 0x8137, 0x8138, 0x8139, 0x813A, 0x813B, 0x813C, 0x813D, 0x813E, 0x813F, 0x8320, 0x8321,
    0x8322, 0x8323, 0x8324, 0x8325, 0x8326, 0x8327, 0x8328, 0x8329, 0x832A, 0x832B, 0x832C, 0x832D, 0x832E, 0x832F,
    0x8330, 0x8331, 0x8332, 0x8333, 0x8334, 0x8335, 0x8336, 0x8337, 0x8338, 0x8339, 0x833A, 0x833B, 0x833C, 0x833D,
    0x833E, 0x833F, 0x8720, 0x8721, 0x8722, 0x8723, 0x8724, 0x8725, 0x8726, 0x8727, 0x8728, 0x8729, 0x872A, 0x872B,
    0x872C, 0x872D, 0x872E, 0x872F, 0x8730, 0x8731, 0x8732, 0x8733, 0x8734, 0x8735, 0x8736, 0x8737, 0x8738, 0x8739,
    0x873A, 0x873B, 0x873C, 0x873D, 0x873E, 0x873F, 0x8F20, 0x8F21, 0x8F22, 0x8F23, 0x8F24, 0x8F25, 0x8F26, 0x8F27,
    0x8F28, 0x8F29, 0x8F2A, 0x8F2B, 0x8F2C, 0x8F2D, 0x8F2E, 0x8F2F, 0x8F30, 0x8F31, 0x8F32, 0x8F33, 0x8F34, 0x8F35,
    0x8F36, 0x8F37, 0x8F38, 0x8F39, 0x8F3A, 0x8F3B, 0x8F3C, 0x8F3D, 0x8F3E, 0x8F3F,
};

static const xmedia_u32 g_sc235hai_dgain_index[SC235HAI_DGAIN_INDEX_MAX] = {
    1024,  1056,  1089,  1120,  1152,  1184,  1217,  1248,  1280,  1312,  1345,  1376,  1408,  1440,  1473,  1504,
    1536,  1568,  1601,  1632,  1664,  1696,  1729,  1760,  1792,  1824,  1857,  1888,  1920,  1952,  1985,  2016,
    2048,  2113,  2176,  2241,  2304,  2369,  2432,  2497,  2560,  2625,  2688,  2753,  2816,  2881,  2944,  3009,
    3072,  3137,  3200,  3265,  3328,  3393,  3456,  3521,  3584,  3649,  3712,  3777,  3840,  3905,  3968,  4033,
    4096,  4224,  4352,  4480,  4608,  4736,  4864,  4992,  5120,  5248,  5376,  5504,  5632,  5760,  5888,  6016,
    6144,  6272,  6400,  6528,  6656,  6784,  6912,  7040,  7168,  7296,  7424,  7552,  7680,  7808,  7936,  8064,
    8192,  8448,  8704,  8960,  9216,  9472,  9728,  9984,  10240, 10496, 10752, 11008, 11264, 11520, 11776, 12032,
    12288, 12544, 12800, 13056, 13312, 13568, 13824, 14080, 14336, 14592, 14848, 15104, 15360, 15616, 15872, 16128,
};

static const xmedia_u32 g_sc235hai_dgain_table[SC235HAI_DGAIN_INDEX_MAX] = {
    0x0080, 0x0084, 0x0088, 0x008C, 0x0090, 0x0094, 0x0098, 0x009C, 0x00A0, 0x00A4, 0x00A8, 0x00AC, 0x00B0,
    0x00B4, 0x00B8, 0x00BC, 0x00C0, 0x00C4, 0x00C8, 0x00CC, 0x00D0, 0x00D4, 0x00D8, 0x00DC, 0x00E0, 0x00E4,
    0x00E8, 0x00EC, 0x00F0, 0x00F4, 0x00F8, 0x00FC, 0x0180, 0x0184, 0x0188, 0x018C, 0x0190, 0x0194, 0x0198,
    0x019C, 0x01A0, 0x01A4, 0x01A8, 0x01AC, 0x01B0, 0x01B4, 0x01B8, 0x01BC, 0x01C0, 0x01C4, 0x01C8, 0x01CC,
    0x01D0, 0x01D4, 0x01D8, 0x01DC, 0x01E0, 0x01E4, 0x01E8, 0x01EC, 0x01F0, 0x01F4, 0x01F8, 0x01FC, 0x0380,
    0x0384, 0x0388, 0x038C, 0x0390, 0x0394, 0x0398, 0x039C, 0x03A0, 0x03A4, 0x03A8, 0x03AC, 0x03B0, 0x03B4,
    0x03B8, 0x03BC, 0x03C0, 0x03C4, 0x03C8, 0x03CC, 0x03D0, 0x03D4, 0x03D8, 0x03DC, 0x03E0, 0x03E4, 0x03E8,
    0x03EC, 0x03F0, 0x03F4, 0x03F8, 0x03FC, 0x0780, 0x0784, 0x0788, 0x078C, 0x0790, 0x0794, 0x0798, 0x079C,
    0x07A0, 0x07A4, 0x07A8, 0x07AC, 0x07B0, 0x07B4, 0x07B8, 0x07BC, 0x07C0, 0x07C4, 0x07C8, 0x07CC, 0x07D0,
    0x07D4, 0x07D8, 0x07DC, 0x07E0, 0x07E4, 0x07E8, 0x07EC, 0x07F0, 0x07F4, 0x07F8, 0x07FC,
};

SENSOR_PRIORITY_RODATA static const xmedia_sensor_capability g_sc235hai_capability = {
    .max_width   = 1920,
    .max_height  = 1080,
    .max_fps     = 60,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported  = XMEDIA_TRUE,
    .mipi_lanes_supported  = XMEDIA_SENSOR_MIPI_LANES_2L,
    .standby_info = {
        .standby_supported = XMEDIA_TRUE,
        .addr_byte_num     = SC235HAI_ADDR_BYTE,
        .data_byte_num     = SC235HAI_DATA_BYTE,
        .standby_reg_num    = 4,
        .standby_reg_addr  = { SC235HAI_REG_ADDR_STANDBY, 0x3019, 0x3022, SC235HAI_REG_ADDR_LOW_POWER_STANDBY},
        .standby_reg_data  = { 0x0, 0xff, 0x03, 0x0f},
        .resume_reg_num    = 5,
        .resume_reg_addr   = { XMEDIA_SENSOR_DELAY_FLAG, SC235HAI_REG_ADDR_LOW_POWER_STANDBY, 0x3019, 0x3022, SC235HAI_REG_ADDR_STANDBY},
        .resume_reg_data   = { 0x5, 0x0, 0x0, 0x01, 0x01},
    },
};

static xmedia_sensor_trig_attr g_sc235hai_trig_attr = {
    .trig_mode  = 0,
    .hs_enable  = XMEDIA_FALSE,
    .vs_enable  = XMEDIA_TRUE,
    .hs_invert  = 0,
    .vs_invert  = 0,

    .hs_time    = 0,
    .vs_time    = (1.0 / 30.0) * 1000000,
    .vs_active  = 1 * ((1.0 / ( SC235HAI_VMAX_2M_1080_LINEAR * 30.0 )) * 1000000), // the time of 1 line
    .delay_frame_num = 2,
};

SENSOR_PRIORITY_FUNC static xmedia_s32 sc235hai_ctx_init(xmedia_u32 dev)
{
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    if (sc235hai_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    sc235hai_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (sc235hai_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(sc235hai_ctx, 0, sizeof(sensor_context));
    sc235hai_ctx->i2c_addr         = SC235HAI_I2C_ADDR;
    sc235hai_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_2L;
    sc235hai_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    sc235hai_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    sc235hai_ctx->size.width       = 1920;
    sc235hai_ctx->size.height      = 1080;
    sc235hai_ctx->fps              = g_sc235hai_property[0].max_fps;
    sc235hai_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    sc235hai_ctx->img_mode             = SC235HAI_2M_1080_10BIT_LINEAR_MODE;
    sc235hai_ctx->fl_std               = SC235HAI_VMAX_2M_1080_LINEAR << SC235HAI_EXP_SHIFT;
    sc235hai_ctx->fl[SENSOR_CUR_FRAME] = SC235HAI_VMAX_2M_1080_LINEAR << SC235HAI_EXP_SHIFT;
    sc235hai_ctx->fl[SENSOR_PRE_FRAME] = SC235HAI_VMAX_2M_1080_LINEAR << SC235HAI_EXP_SHIFT;

    SC235HAI_SET_CONTEXT(dev, sc235hai_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void sc235hai_ctx_exit(xmedia_u32 dev)
{
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_FREE(sc235hai_ctx);
    SC235HAI_SET_CONTEXT(dev, XMEDIA_NULL);
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc235hai_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    for (i = 0; i < SC235HAI_SPECS_MAX_NUM; i++) {
        if (g_sc235hai_property[i].width == sc235hai_ctx->size.width &&
            g_sc235hai_property[i].height == sc235hai_ctx->size.height &&
            g_sc235hai_property[i].wdr_mode == sc235hai_ctx->wdr_mode) {
            memcpy(property, &g_sc235hai_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n",
                    sc235hai_ctx->size.width, sc235hai_ctx->size.height, sc235hai_ctx->wdr_mode);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 sc235hai_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    sc235hai_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc235hai_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "No support mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc235hai_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc235hai_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        sc235hai_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        sc235hai_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        sc235hai_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 sc235hai_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    sc235hai_ctx->i2c_addr = slave_addr;
    ret = sc235hai_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    *slave_addr = sc235hai_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc235hai_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    sc235hai_ctx->init_mode = init_mode;

    ret = sc235hai_i2c_init(dev, sc235hai_ctx->bus_info.i2c_dev, sc235hai_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    sc235hai_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc235hai_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    ret = sc235hai_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    sc235hai_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_void sc235hai_init_common_reg_info(sensor_context *sc235hai_ctx)
{
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_EXP_H].delay_frame_num = 2;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_EXP_H].reg_addr        = SC235HAI_REG_ADDR_EXP_H;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_EXP_M].delay_frame_num = 2;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_EXP_M].reg_addr        = SC235HAI_REG_ADDR_EXP_M;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_EXP_L].delay_frame_num = 2;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_EXP_L].reg_addr        = SC235HAI_REG_ADDR_EXP_L;

    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_AGAIN_H].delay_frame_num = 2;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_AGAIN_H].reg_addr        = SC235HAI_REG_ADDR_AGAIN_H;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_AGAIN_L].delay_frame_num = 2;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_AGAIN_L].reg_addr        = SC235HAI_REG_ADDR_AGAIN_L;

    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_DGAIN_H].delay_frame_num = 2;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_DGAIN_H].reg_addr        = SC235HAI_REG_ADDR_DGAIN_H;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_DGAIN_L].delay_frame_num = 2;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_DGAIN_L].reg_addr        = SC235HAI_REG_ADDR_DGAIN_L;

    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_VMAX_H].delay_frame_num = 0;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_VMAX_H].reg_addr        = SC235HAI_REG_ADDR_VMAX_H;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_VMAX_L].delay_frame_num = 0;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_VMAX_L].reg_addr        = SC235HAI_REG_ADDR_VMAX_L;

    return;
}

static xmedia_void sc235hai_init_2to1_wdr_reg_info(sensor_context *sc235hai_ctx)
{
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_EXP_H].delay_frame_num = 2;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_EXP_H].reg_addr        = SC235HAI_REG_ADDR_S_EXP_H;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_EXP_L].delay_frame_num = 2;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_EXP_L].reg_addr        = SC235HAI_REG_ADDR_S_EXP_L;

    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_AGAIN_H].delay_frame_num = 2;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_AGAIN_H].reg_addr = SC235HAI_REG_ADDR_S_AGAIN_H;

    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_AGAIN_L].delay_frame_num = 2;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_AGAIN_L].reg_addr = SC235HAI_REG_ADDR_S_AGAIN_L;

    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_DGAIN_H].delay_frame_num = 2;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_DGAIN_H].reg_addr = SC235HAI_REG_ADDR_S_DGAIN_H;

    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_DGAIN_L].delay_frame_num = 2;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_DGAIN_L].reg_addr = SC235HAI_REG_ADDR_S_DGAIN_L;

    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_EXP_MAX_H].delay_frame_num = 0;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_EXP_MAX_H].reg_addr = SC235HAI_REG_ADDR_S_EXP_MAX_H;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_EXP_MAX_L].delay_frame_num = 0;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_EXP_MAX_L].reg_addr = SC235HAI_REG_ADDR_S_EXP_MAX_L;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc235hai_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = sc235hai_ctx->bus_info.i2c_dev;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = SC235HAI_REG_L_MAX_NUM;

    if (sc235hai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num = SC235HAI_REG_MAX_NUM;
    }

    for (i = 0; i < sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = sc235hai_ctx->i2c_addr;
        sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = SC235HAI_ADDR_BYTE;
        sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = SC235HAI_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    sc235hai_init_common_reg_info(sc235hai_ctx);

     // init 2to1 wdr mode Regs
    if (sc235hai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc235hai_init_2to1_wdr_reg_info(sc235hai_ctx);
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc235hai_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case SC235HAI_2M_1080_10BIT_LINEAR_MODE:
            *max_fps = 60.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{
    switch (img_mode) {
        case SC235HAI_2M_1080_10BIT_LINEAR_MODE:
            *min_fps = 2;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case SC235HAI_2M_1080_10BIT_LINEAR_MODE:
            *vmax = SC235HAI_VMAX_2M_1080_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = sc235hai_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc235hai_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc235hai_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is (%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, SC235HAI_FULL_LINES_MAX);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    sc235hai_ctx->mirror_en = mirror_en;
    ret = sc235hai_set_mirror_flip(dev, mirror_en, sc235hai_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    sc235hai_ctx->flip_en = flip_en;
    ret = sc235hai_set_mirror_flip(dev, sc235hai_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc235hai_set_image_mode(sensor_context *sc235hai_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (sc235hai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (SC235HAI_RES_IS_1080P(sns_attr->width, sns_attr->height)) {
            *image_mode         = SC235HAI_2M_1080_10BIT_LINEAR_MODE;
            sc235hai_ctx->fl_std = SC235HAI_VMAX_2M_1080_LINEAR << SC235HAI_EXP_SHIFT;
        } else {
            SC235HAI_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        SC235HAI_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc235hai_ctx->size.width  = sns_attr->width;
    sc235hai_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc235hai_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            sc235hai_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            sc235hai_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
            SENSOR_PRINT("2to1 line WDR\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(sc235hai_ctx->wdr_int_time, 0, sizeof(sc235hai_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc235hai_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    // Set wdr mode
    ret = sc235hai_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = sc235hai_set_image_mode(sc235hai_ctx, &sc235hai_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    sc235hai_ctx->fl[SENSOR_CUR_FRAME] = sc235hai_ctx->fl_std;
    sc235hai_ctx->fl[SENSOR_PRE_FRAME] = sc235hai_ctx->fl[SENSOR_CUR_FRAME];

    ret = sc235hai_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef SC235HAI_ISP_DEFAULT_SUPPORT
    // TO-DO: alg add param
    isp_default->blc         = &g_sc235hai_blc_attr;
    isp_default->bnr         = &g_sc235hai_bnr;
    isp_default->clut_attr   = &g_sc235hai_clut_attr;
    isp_default->crosstalk   = XMEDIA_NULL;
    isp_default->csc         = XMEDIA_NULL;
    isp_default->dehaze      = &g_sc235hai_dehaze_attr;
    isp_default->demosaic    = &g_sc235hai_demosaic_attr;
    isp_default->dpc_dynamic = XMEDIA_NULL;
    isp_default->dpc_static  = XMEDIA_NULL;
    isp_default->drc         = XMEDIA_NULL;
    isp_default->expander    = XMEDIA_NULL;
    isp_default->fpn         = XMEDIA_NULL;
    isp_default->gamma       = &g_sc235hai_gamma_attr;
    isp_default->gcac        = XMEDIA_NULL;
    isp_default->hlc         = XMEDIA_NULL;
    isp_default->lce         = &g_sc235hai_lce_attr;
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from sc235hai!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 sc235hai_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (sc235hai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 255;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 259;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 259;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 255;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    for (i = 0; i < sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            sc235hai_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &sc235hai_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&sc235hai_ctx->regs_info[SENSOR_PRE_FRAME], &sc235hai_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    sc235hai_ctx->fl[SENSOR_PRE_FRAME] = sc235hai_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;

}

static xmedia_s32 sc235hai_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    if (sc235hai_ctx->mirror_en && sc235hai_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (sc235hai_ctx->mirror_en && (!sc235hai_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!sc235hai_ctx->mirror_en) && sc235hai_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < SC235HAI_SPECS_MAX_NUM; i++) {
        if (g_sc235hai_property[i].width == sc235hai_ctx->size.width &&
            g_sc235hai_property[i].height == sc235hai_ctx->size.height &&
            g_sc235hai_property[i].wdr_mode == sc235hai_ctx->wdr_mode) {
            *bayer_pattern = g_sc235hai_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= SC235HAI_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc235hai_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_sc235hai_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_get_trig_attr(xmedia_sensor_trig_attr *trig_attr)
{
    SENSOR_CHECK_PTR_RETURN(trig_attr);
    memcpy(trig_attr, &g_sc235hai_trig_attr, sizeof(xmedia_sensor_trig_attr));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc235hai_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 sc235hai_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    xmedia_u32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_sc235hai_exposure[dev]      = init_param->exposure;
    g_sc235hai_sample_r_gain[dev] = init_param->sample_rgain;
    g_sc235hai_sample_b_gain[dev] = init_param->sample_bgain;

    g_sc235hai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_sc235hai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_sc235hai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    for (i = 0; i < XMEDIA_ISP_CCM_MATRIX_SIZE; i++) {
        g_sc235hai_init_ccm[dev][i] = init_param->ccm[i];
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = sc235hai_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256;
    ae_sns_dft->full_lines_max = SC235HAI_FULL_LINES_MAX;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->dgain_accu.accuracy  = 1;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 32 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->ispdgain_sep_thd    = 8 << ae_sns_dft->ispdgain_shift;

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

static xmedia_void sc235hai_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 119347;
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 16128;
    ae_sns_dft->min_dgain        = 1024;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x38;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_sc235hai_exposure[dev] ? g_sc235hai_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - SC235HAI_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 1;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = 1;

    return;
}

static xmedia_s32 sc235hai_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = sc235hai_get_ae_common_default(dev, sc235hai_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (sc235hai_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            sc235hai_get_ae_linear_default(dev, sc235hai_ctx, ae_sns_dft);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", sc235hai_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    ret = sc235hai_calc_fps(fps, sc235hai_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_VMAX_H].data = (full_lines&0x7F00) >> 8;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    sc235hai_ctx->fl[SENSOR_CUR_FRAME] = full_lines << SC235HAI_EXP_SHIFT;
    sc235hai_ctx->fl_std               = sc235hai_ctx->fl[SENSOR_CUR_FRAME];
    sc235hai_ctx->fps                  = fps;

    ae_sns_dft->fps            = fps;
    ae_sns_dft->full_lines_std = sc235hai_ctx->fl_std;
    ae_sns_dft->full_lines     = sc235hai_ctx->fl_std;

    if (sc235hai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sc235hai_ctx->fl[SENSOR_CUR_FRAME] - SC235HAI_EXP_OFFSET_LINEAR;
    }

    SENSOR_PRINT("dev[%d] - sc235hai set fps = %f\n", dev, fps);

    return XMEDIA_SUCCESS;

}

static xmedia_s32 sc235hai_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32      ret;
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    *fps = sc235hai_ctx->fps;
    ret  = sc235hai_get_min_fps(sc235hai_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret  = sc235hai_get_max_fps(sc235hai_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = sc235hai_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc235hai_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc235hai_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = (vmax_max_fps << SC235HAI_EXP_SHIFT) * max_fps / min_fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      vmax_min_fps;
    xmedia_s32      ret;
    xmedia_float    min_fps;
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    if (sc235hai_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", sc235hai_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (sc235hai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        full_lines = full_lines + SC235HAI_EXP_OFFSET_LINEAR;
    }

    ret = sc235hai_get_min_fps_vmax(sc235hai_ctx->img_mode, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc235hai_get_min_fps(sc235hai_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                         = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    sc235hai_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_VMAX_H].data =
                                                                    ((full_lines >> SC235HAI_EXP_SHIFT)&0x7F00) >> 8;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_VMAX_L].data =
                                                                    SENSOR_LOW_8BITS(full_lines >> SC235HAI_EXP_SHIFT);

    ae_sns_dft->full_lines   = sc235hai_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = sc235hai_ctx->fl[SENSOR_CUR_FRAME] - SC235HAI_EXP_OFFSET_LINEAR;

    sc235hai_ctx->fps = min_fps * vmax_min_fps / full_lines;

    SENSOR_PRINT("dev[%d] - sc235hai set fps: %f\n", dev, sc235hai_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context    *sc235hai_ctx                     = XMEDIA_NULL;
    static xmedia_bool first[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = 1 };

    SENSOR_CHECK_DEV_RETURN(dev);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    if (sc235hai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        if (first[dev]) { // 0: short exposure
            sc235hai_ctx->wdr_int_time[0] = int_time;
            sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_EXP_H].data =
                                                                                    SENSOR_MIDDLE_8BITS(int_time);
            sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_EXP_L].data =
                                                                                    SENSOR_LOWER_4BITS(int_time);
            first[dev] = XMEDIA_FALSE;
        } else { // 1: long exposure
            sc235hai_ctx->wdr_int_time[1] = int_time;
            sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_EXP_H].data = SENSOR_HIGHER_4BITS(int_time);
            sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_EXP_M].data = SENSOR_MIDDLE_8BITS(int_time);
            sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_EXP_L].data = SENSOR_LOWER_4BITS(int_time);
            first[dev] = XMEDIA_TRUE;
        }
    } else if (sc235hai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_EXP_H].data = SENSOR_HIGHER_4BITS(int_time);
        sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_EXP_M].data = SENSOR_MIDDLE_8BITS(int_time);
        sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_EXP_L].data = SENSOR_LOWER_4BITS(int_time);
        first[dev]                                                                = XMEDIA_TRUE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_sc235hai_again_index[SC235HAI_AGAIN_INDEX_MAX - 1]) {
        *again_lin = g_sc235hai_again_index[SC235HAI_AGAIN_INDEX_MAX - 1];
        *again_db  = g_sc235hai_again_table[SC235HAI_AGAIN_INDEX_MAX - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < SC235HAI_AGAIN_INDEX_MAX; i++) {
        if (*again_lin < g_sc235hai_again_index[i]) {
            *again_lin = g_sc235hai_again_index[i - 1];
            *again_db  = g_sc235hai_again_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(dgain_lin);
    SENSOR_CHECK_PTR_RETURN(dgain_db);

    if (*dgain_lin >= g_sc235hai_dgain_index[SC235HAI_DGAIN_INDEX_MAX - 1]) {
        *dgain_lin = g_sc235hai_dgain_index[SC235HAI_DGAIN_INDEX_MAX - 1];
        *dgain_db  = g_sc235hai_dgain_table[SC235HAI_DGAIN_INDEX_MAX - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; SC235HAI_DGAIN_INDEX_MAX; i++) {
        if (*dgain_lin < g_sc235hai_dgain_index[i]) {
            *dgain_lin = g_sc235hai_dgain_index[i - 1];
            *dgain_db  = g_sc235hai_dgain_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *sc235hai_ctx = XMEDIA_NULL;
    xmedia_u8 reg_0x3e09; // again
    xmedia_u8 reg_0x3e08;
    xmedia_u8 reg_0x3e07; // dgain
    xmedia_u8 reg_0x3e06;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    reg_0x3e06 = SENSOR_HIGH_8BITS(dgain);
    reg_0x3e07 = SENSOR_LOW_8BITS(dgain);
    reg_0x3e08 = SENSOR_HIGH_8BITS(again);
    reg_0x3e09 = SENSOR_LOW_8BITS(again);

    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_DGAIN_H].data = reg_0x3e06;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_DGAIN_L].data = reg_0x3e07;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_AGAIN_H].data = reg_0x3e08;
    sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_AGAIN_L].data = reg_0x3e09;

    if (sc235hai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_DGAIN_H].data = reg_0x3e06;
        sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_DGAIN_L].data = reg_0x3e07;
        sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_AGAIN_H].data = reg_0x3e08;
        sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC235HAI_REG_S_AGAIN_L].data = reg_0x3e09;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 sc235hai_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *sc235hai_ctx = XMEDIA_NULL;
    xmedia_u32      i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = SC235HAI_CALIBRATE_STATIC_TEMP;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = SC235HAI_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = SC235HAI_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = SC235HAI_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = SC235HAI_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 5: point index on the awb curve param
    awb_sns_dft->wb_para[0] = SC235HAI_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1] = SC235HAI_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2] = SC235HAI_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3] = SC235HAI_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4] = SC235HAI_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5] = SC235HAI_CALIBRATE_AWB_C1;

    awb_sns_dft->golden_rgain = SC235HAI_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = SC235HAI_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (sc235hai_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_sc235hai_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_sc235hai_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_sc235hai_awb_wdr_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_sc235hai_awb_wdr_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", sc235hai_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_sc235hai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_sc235hai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_sc235hai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_sc235hai_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_sc235hai_sample_b_gain[dev];
    for (i = 0; i < XMEDIA_ISP_CCM_MATRIX_SIZE; i++) {
        awb_sns_dft->init_ccm[i] = g_sc235hai_init_ccm[dev][i];
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_standby(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc235hai_write_reg(dev, SC235HAI_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_resume(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc235hai_write_reg(dev, SC235HAI_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_low_power(xmedia_u32 dev, xmedia_bool enable)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);

    if (enable == XMEDIA_TRUE) {
        ret = sc235hai_write_reg(dev, SC235HAI_REG_ADDR_LOW_POWER_STANDBY, 0x0F);
        ret |= sc235hai_write_reg(dev, 0x3019, 0xff);
        ret |= sc235hai_write_reg(dev, 0x3022, 0x03);
    } else {
        ret = sc235hai_write_reg(dev, SC235HAI_REG_ADDR_LOW_POWER_STANDBY, 0x0);
        ret |= sc235hai_write_reg(dev, 0x3019, 0x00);
        ret |= sc235hai_write_reg(dev, 0x3022, 0x01);
    }

    return ret;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc235hai_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *sc235hai_ctx = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= sc235hai_write_reg(dev, sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                sc235hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc235hai_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    if (sc235hai_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = sc235hai_init_image(dev, sc235hai_ctx->img_mode, sc235hai_ctx->work_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = sc235hai_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc235hai_set_mirror_flip(dev, sc235hai_ctx->mirror_en, sc235hai_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc235hai_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc235hai_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc235hai_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_sc235hai_dev_map[index] == SENSOR_DEV_INVALID) {
            g_sc235hai_dev_map[index] = pipe;
            *dev                     = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc235hai_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_sc235hai_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 sc235hai_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, SC235HAI_NAME, sizeof(SC235HAI_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = sc235hai_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = sc235hai_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = sc235hai_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = sc235hai_mirror;
    info->isp_func.pfn_sensor_flip             = sc235hai_flip;
    info->isp_func.pfn_sensor_set_bus_info     = sc235hai_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = sc235hai_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = sc235hai_set_init_param;
    info->isp_func.pfn_sensor_start            = sc235hai_start;
    info->isp_func.pfn_sensor_stop             = sc235hai_stop;
    info->isp_func.pfn_sensor_standby          = sc235hai_standby;
    info->isp_func.pfn_sensor_resume           = sc235hai_resume;
    info->isp_func.pfn_sensor_write_reg        = sc235hai_write_reg;
    info->isp_func.pfn_sensor_read_reg         = sc235hai_read_reg;
    info->isp_func.pfn_sensor_init             = sc235hai_init;
    info->isp_func.pfn_sensor_exit             = sc235hai_exit;
    info->isp_func.pfn_sensor_set_attr         = sc235hai_set_attr;
    info->isp_func.pfn_sensor_low_power        = sc235hai_low_power;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = sc235hai_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = sc235hai_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = sc235hai_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = sc235hai_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = sc235hai_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = sc235hai_get_dev_addr;
    info->isp_func.pfn_sensor_get_trig_attr       = sc235hai_get_trig_attr;
    info->isp_func.pfn_sensor_get_fps             = sc235hai_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = sc235hai_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = sc235hai_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = sc235hai_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = sc235hai_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = sc235hai_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = sc235hai_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = sc235hai_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = sc235hai_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = sc235hai_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = sc235hai_get_awb_default;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc235hai_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = sc235hai_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc235hai_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = sc235hai_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "SC235HAI register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc235hai_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *sc235hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = sc235hai_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    SC235HAI_GET_CONTEXT(dev, sc235hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc235hai_ctx);

    if (sc235hai_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "SC235HAI unregister function failed!\n");
        return ret;
    }

    sc235hai_ctx_exit(dev);
    g_sc235hai_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;
}
