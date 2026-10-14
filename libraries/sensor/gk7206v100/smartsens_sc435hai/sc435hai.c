#include <stdlib.h>
#include <string.h>
#include "sc435hai.h"
#include "sc435hai_ctrl.h"
#include "sc435hai_ex.h"
#include "xmedia_isp.h"

#define SC435HAI_NAME          "SC435HAI"
#define SC435HAI_SPECS_MAX_NUM 1

#ifdef __linux__
#define SC435HAI_ISP_DEFAULT_SUPPORT
#endif

#define SC435HAI_4M_1440P_BIT_RATE_LINEAR 450

#define SC435HAI_RES_IS_1440P(w, h) ((w) == 2560 && (h) == 1440)

#define SC435HAI_ERR_MODE_PRINT(sns_attr)                                                                            \
    do {                                                                                                             \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sns_attr->width,              \
                     sns_attr->height, sns_attr->wdr_mode);                                                          \
    } while (0)

// awb static param for Fuji Lens New IR_Cut
#define SC435HAI_CALIBRATE_STATIC_TEMP       5000
#define SC435HAI_CALIBRATE_STATIC_WB_R_GAIN  433
#define SC435HAI_CALIBRATE_STATIC_WB_GR_GAIN 256
#define SC435HAI_CALIBRATE_STATIC_WB_GB_GAIN 256
#define SC435HAI_CALIBRATE_STATIC_WB_B_GAIN  653

// Calibration results for Auto WB Planck
#define SC435HAI_CALIBRATE_AWB_P1 165
#define SC435HAI_CALIBRATE_AWB_P2 (-89)
#define SC435HAI_CALIBRATE_AWB_Q1 (-177)
#define SC435HAI_CALIBRATE_AWB_A1 200851
#define SC435HAI_CALIBRATE_AWB_B1 128
#define SC435HAI_CALIBRATE_AWB_C1 (-139601)

// Rgain and Bgain of the golden sample
#define SC435HAI_GOLDEN_RGAIN 0
#define SC435HAI_GOLDEN_BGAIN 0

#define SC435HAI_AGAIN_INDEX_MAX 178
#define SC435HAI_DGAIN_INDEX_MAX 128

sensor_context *g_sc435hai_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define SC435HAI_SET_CONTEXT(dev, sns_ctx) g_sc435hai_ctx[dev] = sns_ctx
#define SC435HAI_GET_CONTEXT(dev, sns_ctx) sns_ctx = g_sc435hai_ctx[dev]

static xmedia_u32 g_sc435hai_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]                             = { 0 };
static xmedia_u16 g_sc435hai_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM]                        = { 0 };
static xmedia_u16 g_sc435hai_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM]                        = { 0 };
static xmedia_u16 g_sc435hai_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };
static xmedia_s32 g_sc435hai_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                    { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID };

static const xmedia_sensor_property g_sc435hai_property[SC435HAI_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        2560, 1440, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_NONE, 30, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC435HAI_4M_1440P_BIT_RATE_LINEAR },
};

static const xmedia_u32 g_sc435hai_again_index[SC435HAI_AGAIN_INDEX_MAX] = {
    1024,  1056,  1089,  1120,  1152,  1184,  1217,  1248,  1280,  1312,  1345,  1376,  1408,  1440,  1473,
    1504,  1536,  1568,  1577,  1626,  1675,  1724,  1775,  1824,  1873,  1922,  1971,  2020,  2070,  2119,
    2169,  2218,  2267,  2316,  2365,  2415,  2464,  2513,  2563,  2612,  2661,  2711,  2760,  2809,  2858,
    2907,  2957,  3006,  3056,  3105,  3154,  3252,  3352,  3450,  3548,  3646,  3746,  3844,  3942,  4041,
    4140,  4238,  4337,  4435,  4534,  4633,  4731,  4829,  4929,  5027,  5125,  5223,  5323,  5421,  5519,
    5618,  5717,  5815,  5914,  6012,  6111,  6210,  6308,  6505,  6702,  6900,  7096,  7294,  7491,  7688,
    7885,  8082,  8279,  8477,  8673,  8871,  9068,  9265,  9462,  9659,  9856,  10054, 10250, 10448, 10644,
    10842, 11039, 11236, 11433, 11631, 11827, 12025, 12221, 12419, 12616, 13010, 13404, 13798, 14193, 14587,
    14981, 15375, 15770, 16164, 16558, 16952, 17347, 17741, 18135, 18529, 18924, 19318, 19712, 20106, 20500,
    20895, 21289, 21683, 22077, 22472, 22866, 23260, 23654, 24049, 24443, 24837, 25231, 26020, 26808, 27597,
    28385, 29174, 29962, 30751, 31539, 32328, 33116, 33905, 34693, 35482, 36270, 37059, 37847, 38636, 39424,
    40212, 41001, 41789, 42578, 43366, 44155, 44943, 45732, 46520, 47309, 48097, 48886, 49674,
};

static const xmedia_u32 g_sc435hai_again_table[SC435HAI_AGAIN_INDEX_MAX] = {
    0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D,
    0x002E, 0x002F, 0x0030, 0x0031, 0x8020, 0x8021, 0x8022, 0x8023, 0x8024, 0x8025, 0x8026, 0x8027, 0x8028, 0x8029,
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

static const xmedia_u32 g_sc435hai_dgain_index[SC435HAI_DGAIN_INDEX_MAX] = {
    1024,  1056,  1089,  1120,  1152,  1184,  1217,  1248,  1280,  1312,  1345,  1376,  1408,  1440,  1473,  1504,
    1536,  1568,  1601,  1632,  1664,  1696,  1729,  1760,  1792,  1824,  1857,  1888,  1920,  1952,  1985,  2016,
    2048,  2113,  2176,  2241,  2304,  2369,  2432,  2497,  2560,  2625,  2688,  2753,  2816,  2881,  2944,  3009,
    3072,  3137,  3200,  3265,  3328,  3393,  3456,  3521,  3584,  3649,  3712,  3777,  3840,  3905,  3968,  4033,
    4096,  4224,  4352,  4480,  4608,  4736,  4864,  4992,  5120,  5248,  5376,  5504,  5632,  5760,  5888,  6016,
    6144,  6272,  6400,  6528,  6656,  6784,  6912,  7040,  7168,  7296,  7424,  7552,  7680,  7808,  7936,  8064,
    8192,  8448,  8704,  8960,  9216,  9472,  9728,  9984,  10240, 10496, 10752, 11008, 11264, 11520, 11776, 12032,
    12288, 12544, 12800, 13056, 13312, 13568, 13824, 14080, 14336, 14592, 14848, 15104, 15360, 15616, 15872, 16128,
};

static const xmedia_u32 g_sc435hai_dgain_table[SC435HAI_DGAIN_INDEX_MAX] = {
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

static const xmedia_sensor_capability g_sc435hai_capability = {
    .max_width   = 2560,
    .max_height  = 1440,
    .max_fps     = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .mipi_lanes_supported  = XMEDIA_SENSOR_MIPI_LANES_2L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = SC435HAI_ADDR_BYTE,
        .data_byte_num     = SC435HAI_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { SC435HAI_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { SC435HAI_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x01 },
    },
};

static xmedia_s32 sc435hai_ctx_init(xmedia_u32 dev)
{
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    if (sc435hai_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    sc435hai_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (sc435hai_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(sc435hai_ctx, 0, sizeof(sensor_context));
    sc435hai_ctx->i2c_addr         = SC435HAI_I2C_ADDR;
    sc435hai_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_2L;
    sc435hai_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    sc435hai_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    sc435hai_ctx->size.width       = 2560;
    sc435hai_ctx->size.height      = 1440;
    sc435hai_ctx->fps              = g_sc435hai_property[0].max_fps;
    sc435hai_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    sc435hai_ctx->img_mode             = SC435HAI_4M_1440_10BIT_LINEAR_MODE;
    sc435hai_ctx->fl_std               = SC435HAI_VMAX_4M_1440_LINEAR << SC435HAI_EXP_SHIFT;
    sc435hai_ctx->fl[SENSOR_CUR_FRAME] = SC435HAI_VMAX_4M_1440_LINEAR << SC435HAI_EXP_SHIFT;
    sc435hai_ctx->fl[SENSOR_PRE_FRAME] = SC435HAI_VMAX_4M_1440_LINEAR << SC435HAI_EXP_SHIFT;

    SC435HAI_SET_CONTEXT(dev, sc435hai_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void sc435hai_ctx_exit(xmedia_u32 dev)
{
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_FREE(sc435hai_ctx);
    SC435HAI_SET_CONTEXT(dev, XMEDIA_NULL);
}

static xmedia_s32 sc435hai_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    for (i = 0; i < SC435HAI_SPECS_MAX_NUM; i++) {
        if (g_sc435hai_property[i].width == sc435hai_ctx->size.width &&
            g_sc435hai_property[i].height == sc435hai_ctx->size.height &&
            g_sc435hai_property[i].wdr_mode == sc435hai_ctx->wdr_mode) {
            memcpy(property, &g_sc435hai_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n", sc435hai_ctx->size.width,
                 sc435hai_ctx->size.height, sc435hai_ctx->wdr_mode);

    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 sc435hai_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    sc435hai_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "No support mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc435hai_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        sc435hai_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        sc435hai_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        sc435hai_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 sc435hai_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    sc435hai_ctx->i2c_addr = slave_addr;
    ret                    = sc435hai_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    *slave_addr = sc435hai_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    sc435hai_ctx->init_mode = init_mode;

    ret = sc435hai_i2c_init(dev, sc435hai_ctx->bus_info.i2c_dev, sc435hai_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    sc435hai_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    ret = sc435hai_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    sc435hai_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_void sc435hai_init_common_reg_info(sensor_context *sc435hai_ctx)
{
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_EXP_H].delay_frame_num = 2;
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_EXP_H].reg_addr        = SC435HAI_REG_ADDR_EXP_H;
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_EXP_M].delay_frame_num = 2;
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_EXP_M].reg_addr        = SC435HAI_REG_ADDR_EXP_M;
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_EXP_L].delay_frame_num = 2;
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_EXP_L].reg_addr        = SC435HAI_REG_ADDR_EXP_L;

    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_AGAIN_H].delay_frame_num = 2;
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_AGAIN_H].reg_addr      = SC435HAI_REG_ADDR_AGAIN_H;
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_AGAIN_L].delay_frame_num = 2;
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_AGAIN_L].reg_addr      = SC435HAI_REG_ADDR_AGAIN_L;

    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_DGAIN_H].delay_frame_num = 2;
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_DGAIN_H].reg_addr      = SC435HAI_REG_ADDR_DGAIN_H;
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_DGAIN_L].delay_frame_num = 2;
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_DGAIN_L].reg_addr      = SC435HAI_REG_ADDR_DGAIN_L;

    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_VMAX_H].delay_frame_num = 1;
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_VMAX_H].reg_addr       = SC435HAI_REG_ADDR_VMAX_H;
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_VMAX_L].delay_frame_num = 1;
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_VMAX_L].reg_addr        = SC435HAI_REG_ADDR_VMAX_L;

    return;
}

xmedia_s32 sc435hai_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = sc435hai_ctx->bus_info.i2c_dev;
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = SC435HAI_REG_MAX_NUM;

    for (i = 0; i < sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = sc435hai_ctx->i2c_addr;
        sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = SC435HAI_ADDR_BYTE;
        sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = SC435HAI_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    sc435hai_init_common_reg_info(sc435hai_ctx);

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc435hai_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case SC435HAI_4M_1440_10BIT_LINEAR_MODE:
            *max_fps = 30.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{ // TO-DO：min fps is to be confirmed.
    switch (img_mode) {
        case SC435HAI_4M_1440_10BIT_LINEAR_MODE:
            *min_fps = 1;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case SC435HAI_4M_1440_10BIT_LINEAR_MODE:
            *vmax = SC435HAI_VMAX_4M_1440_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = sc435hai_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc435hai_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc435hai_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is (%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, SC435HAI_FULL_LINES_MAX);

    SENSOR_PRINT("sc435hai set fps = %f\n", fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    sc435hai_ctx->mirror_en = mirror_en;
    ret                     = sc435hai_set_mirror_flip(dev, mirror_en, sc435hai_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    sc435hai_ctx->flip_en = flip_en;
    ret                   = sc435hai_set_mirror_flip(dev, sc435hai_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_set_image_mode(sensor_context *sc435hai_ctx, xmedia_u8 *image_mode,
                                         const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (sc435hai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (SC435HAI_RES_IS_1440P(sns_attr->width, sns_attr->height)) {
            *image_mode          = SC435HAI_4M_1440_10BIT_LINEAR_MODE;
            sc435hai_ctx->fl_std = SC435HAI_VMAX_4M_1440_LINEAR << SC435HAI_EXP_SHIFT;
        } else {
            SC435HAI_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        SC435HAI_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc435hai_ctx->size.width  = sns_attr->width;
    sc435hai_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            sc435hai_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            sc435hai_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE;
            SENSOR_PRINT("2to1 line WDR\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(sc435hai_ctx->wdr_int_time, 0, sizeof(sc435hai_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    // Set wdr mode
    ret = sc435hai_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = sc435hai_set_image_mode(sc435hai_ctx, &sc435hai_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    sc435hai_ctx->fl[SENSOR_CUR_FRAME] = sc435hai_ctx->fl_std;
    sc435hai_ctx->fl[SENSOR_PRE_FRAME] = sc435hai_ctx->fl[SENSOR_CUR_FRAME];

    ret = sc435hai_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef SC435HAI_ISP_DEFAULT_SUPPORT
    // TO-DO: alg add param
    isp_default->blc         = &g_sc435hai_blc_attr;
    isp_default->bnr         = &g_sc435hai_bnr;
    isp_default->clut_attr   = &g_sc435hai_clut_attr;
    isp_default->crosstalk   = XMEDIA_NULL;
    isp_default->csc         = XMEDIA_NULL;
    isp_default->dehaze      = &g_sc435hai_dehaze_attr;
    isp_default->demosaic    = &g_sc435hai_demosaic_attr;
    isp_default->dpc_dynamic = XMEDIA_NULL;
    isp_default->dpc_static  = XMEDIA_NULL;
    isp_default->drc         = XMEDIA_NULL;
    isp_default->expander    = XMEDIA_NULL;
    isp_default->fpn         = XMEDIA_NULL;
    isp_default->gamma       = &g_sc435hai_gamma_attr;
    isp_default->gcac        = XMEDIA_NULL;
    isp_default->hlc         = XMEDIA_NULL;
    isp_default->lce         = &g_sc435hai_lce_attr;
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from sc435hai!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 sc435hai_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (sc435hai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x100;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc435hai_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    for (i = 0; i < sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            sc435hai_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &sc435hai_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&sc435hai_ctx->regs_info[SENSOR_PRE_FRAME], &sc435hai_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    sc435hai_ctx->fl[SENSOR_PRE_FRAME] = sc435hai_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    if (sc435hai_ctx->mirror_en && sc435hai_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (sc435hai_ctx->mirror_en && (!sc435hai_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!sc435hai_ctx->mirror_en) && sc435hai_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < SC435HAI_SPECS_MAX_NUM; i++) {
        if (g_sc435hai_property[i].width == sc435hai_ctx->size.width &&
            g_sc435hai_property[i].height == sc435hai_ctx->size.height &&
            g_sc435hai_property[i].wdr_mode == sc435hai_ctx->wdr_mode) {
            *bayer_pattern = g_sc435hai_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= SC435HAI_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_sc435hai_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc435hai_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 sc435hai_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_sc435hai_exposure[dev]      = init_param->exposure;
    g_sc435hai_sample_r_gain[dev] = init_param->sample_rgain;
    g_sc435hai_sample_b_gain[dev] = init_param->sample_bgain;

    g_sc435hai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_sc435hai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_sc435hai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = sc435hai_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256;
    ae_sns_dft->full_lines_max = SC435HAI_FULL_LINES_MAX;

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

static xmedia_void sc435hai_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                 xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 49674;
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 16128;
    ae_sns_dft->min_dgain        = 1024;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x38;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_sc435hai_exposure[dev] ? g_sc435hai_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - SC435HAI_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 1;
    ae_sns_dft->max_int_time_target = 65535;
    ae_sns_dft->min_int_time_target = 1;

    return;
}

static xmedia_s32 sc435hai_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = sc435hai_get_ae_common_default(dev, sc435hai_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (sc435hai_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            sc435hai_get_ae_linear_default(dev, sc435hai_ctx, ae_sns_dft);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", sc435hai_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc435hai_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    ret = sc435hai_calc_fps(fps, sc435hai_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_VMAX_H].data = (full_lines & 0x7F00) >> 8;
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    sc435hai_ctx->fps                  = fps;
    sc435hai_ctx->fl[SENSOR_CUR_FRAME] = full_lines << SC435HAI_EXP_SHIFT;
    sc435hai_ctx->fl_std               = sc435hai_ctx->fl[SENSOR_CUR_FRAME];

    ae_sns_dft->fps            = fps;
    ae_sns_dft->full_lines_std = sc435hai_ctx->fl_std;
    ae_sns_dft->full_lines     = sc435hai_ctx->fl_std;

    if (sc435hai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sc435hai_ctx->fl[SENSOR_CUR_FRAME] - SC435HAI_EXP_OFFSET_LINEAR;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32      ret;
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    *fps = sc435hai_ctx->fps;
    ret  = sc435hai_get_min_fps(sc435hai_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret  = sc435hai_get_max_fps(sc435hai_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = sc435hai_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc435hai_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc435hai_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = (vmax_max_fps << SC435HAI_EXP_SHIFT) * max_fps / min_fps;

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc435hai_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *sc435hai_ctx = XMEDIA_NULL;
    xmedia_u32      vmax_min_fps;
    xmedia_s32      ret;
    xmedia_float    fps;
    xmedia_float    min_fps;

    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    if (sc435hai_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        full_lines = full_lines + SC435HAI_EXP_OFFSET_LINEAR;
    }

    ret = sc435hai_get_min_fps_vmax(sc435hai_ctx->img_mode, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc435hai_get_min_fps(sc435hai_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                         = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    sc435hai_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_VMAX_H].data =
                                                                    ((full_lines >> SC435HAI_EXP_SHIFT)&0x7F00) >> 8;
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_VMAX_L].data =
                                                                    SENSOR_LOW_8BITS(full_lines >> SC435HAI_EXP_SHIFT);

    ae_sns_dft->full_lines   = sc435hai_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = sc435hai_ctx->fl[SENSOR_CUR_FRAME] - SC435HAI_EXP_OFFSET_LINEAR;

    fps = min_fps * vmax_min_fps / full_lines;
    SENSOR_PRINT("sensor fps: %f\n", fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_EXP_H].data = SENSOR_HIGHER_4BITS(int_time);
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_EXP_M].data = SENSOR_MIDDLE_8BITS(int_time);
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_EXP_L].data = SENSOR_LOWER_4BITS(int_time);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_sc435hai_again_index[SC435HAI_AGAIN_INDEX_MAX - 1]) {
        *again_lin = g_sc435hai_again_index[SC435HAI_AGAIN_INDEX_MAX - 1];
        *again_db  = g_sc435hai_again_table[SC435HAI_AGAIN_INDEX_MAX - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < SC435HAI_AGAIN_INDEX_MAX; i++) {
        if (*again_lin < g_sc435hai_again_index[i]) {
            *again_lin = g_sc435hai_again_index[i - 1];
            *again_db  = g_sc435hai_again_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(dgain_lin);
    SENSOR_CHECK_PTR_RETURN(dgain_db);

    if (*dgain_lin >= g_sc435hai_dgain_index[SC435HAI_DGAIN_INDEX_MAX - 1]) {
        *dgain_lin = g_sc435hai_dgain_index[SC435HAI_DGAIN_INDEX_MAX - 1];
        *dgain_db  = g_sc435hai_dgain_table[SC435HAI_DGAIN_INDEX_MAX - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; SC435HAI_DGAIN_INDEX_MAX; i++) {
        if (*dgain_lin < g_sc435hai_dgain_index[i]) {
            *dgain_lin = g_sc435hai_dgain_index[i - 1];
            *dgain_db  = g_sc435hai_dgain_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_DGAIN_H].data = SENSOR_HIGH_8BITS(dgain);
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_DGAIN_L].data = SENSOR_LOW_8BITS(dgain);
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_AGAIN_H].data = SENSOR_HIGH_8BITS(again);
    sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC435HAI_REG_AGAIN_L].data = SENSOR_LOW_8BITS(again);

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc435hai_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 sc435hai_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = SC435HAI_CALIBRATE_STATIC_TEMP;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = SC435HAI_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = SC435HAI_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = SC435HAI_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = SC435HAI_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 5: point index on the awb curve param
    awb_sns_dft->wb_para[0] = SC435HAI_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1] = SC435HAI_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2] = SC435HAI_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3] = SC435HAI_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4] = SC435HAI_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5] = SC435HAI_CALIBRATE_AWB_C1;

    awb_sns_dft->golden_rgain    = SC435HAI_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain    = SC435HAI_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (sc435hai_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_sc435hai_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_sc435hai_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_sc435hai_awb_wdr_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_sc435hai_awb_wdr_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "No support %d ! \n", sc435hai_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_sc435hai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_sc435hai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_sc435hai_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_sc435hai_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_sc435hai_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_standby(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc435hai_write_reg(dev, SC435HAI_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_resume(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc435hai_write_reg(dev, SC435HAI_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *sc435hai_ctx = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= sc435hai_write_reg(dev, sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                 sc435hai_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_start(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    if (sc435hai_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = sc435hai_init_image(dev, sc435hai_ctx->img_mode, sc435hai_ctx->work_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = sc435hai_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc435hai_set_mirror_flip(dev, sc435hai_ctx->mirror_en, sc435hai_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc435hai_set_stream_mode(dev, XMEDIA_TRUE);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc435hai_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc435hai_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;
    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_sc435hai_dev_map[index] == SENSOR_DEV_INVALID) {
            g_sc435hai_dev_map[index] = pipe;
            *dev                      = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 sc435hai_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_sc435hai_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 sc435hai_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, SC435HAI_NAME, sizeof(SC435HAI_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property    = sc435hai_get_property;
    info->isp_func.pfn_sensor_set_work_mode   = sc435hai_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes  = sc435hai_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror          = sc435hai_mirror;
    info->isp_func.pfn_sensor_flip            = sc435hai_flip;
    info->isp_func.pfn_sensor_set_bus_info    = sc435hai_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr    = sc435hai_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param  = sc435hai_set_init_param;
    info->isp_func.pfn_sensor_start           = sc435hai_start;
    info->isp_func.pfn_sensor_stop            = sc435hai_stop;
    info->isp_func.pfn_sensor_standby         = sc435hai_standby;
    info->isp_func.pfn_sensor_resume          = sc435hai_resume;
    info->isp_func.pfn_sensor_write_reg       = sc435hai_write_reg;
    info->isp_func.pfn_sensor_read_reg        = sc435hai_read_reg;
    info->isp_func.pfn_sensor_init            = sc435hai_init;
    info->isp_func.pfn_sensor_exit            = sc435hai_exit;
    info->isp_func.pfn_sensor_set_attr        = sc435hai_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = sc435hai_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = sc435hai_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = sc435hai_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = sc435hai_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = sc435hai_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = sc435hai_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = sc435hai_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = sc435hai_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = sc435hai_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = sc435hai_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = sc435hai_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = sc435hai_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = sc435hai_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = sc435hai_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = sc435hai_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = sc435hai_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = sc435hai_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc435hai_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = sc435hai_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc435hai_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = sc435hai_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "SC435HAI register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc435hai_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *sc435hai_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = sc435hai_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    SC435HAI_GET_CONTEXT(dev, sc435hai_ctx);
    SENSOR_CHECK_PTR_RETURN(sc435hai_ctx);

    if (sc435hai_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "SC435HAI unregister function failed!\n");
        return ret;
    }

    sc435hai_ctx_exit(dev);
    g_sc435hai_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;
}
