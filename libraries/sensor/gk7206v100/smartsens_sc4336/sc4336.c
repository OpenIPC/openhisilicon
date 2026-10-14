#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sc4336.h"
#include "sc4336_ctrl.h"
#include "sc4336_ex.h"

#define SC4336_NAME          "SC4336"
#define SC4336_SPECS_MAX_NUM 1

#define SC4336_4M_BIT_RATE_LINEAR  600

#ifdef __linux__
//#define SC4336_ISP_DEFAULT_SUPPORT
#endif

#define SC4336_REG_ADDR_STANDBY 0x0100

#define SC4336_RES_IS_1440P(w, h) ((w) == 2560 && (h) == 1440)

#define SC4336_ISO_LUT_NUM     8

#define SC4336_ERR_MODE_PRINT(sns_attr)                                                                                \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sns_attr->width,                \
                     sns_attr->height, sns_attr->wdr_mode);                                                            \
    } while (0)

/* awb static param for Fuji Lens New IR_Cut */
#define SC4336_CALIBRATE_STATIC_TEMP       5000
#define SC4336_CALIBRATE_STATIC_WB_R_GAIN  329
#define SC4336_CALIBRATE_STATIC_WB_GR_GAIN 256
#define SC4336_CALIBRATE_STATIC_WB_GB_GAIN 256
#define SC4336_CALIBRATE_STATIC_WB_B_GAIN  542

/* Calibration results for Auto WB Planck */
#define SC4336_CALIBRATE_AWB_P1 169
#define SC4336_CALIBRATE_AWB_P2 (-72)
#define SC4336_CALIBRATE_AWB_Q1 (-159)
#define SC4336_CALIBRATE_AWB_A1 193427
#define SC4336_CALIBRATE_AWB_B1 128
#define SC4336_CALIBRATE_AWB_C1 (-130502)

/* Rgain and Bgain of the golden sample */
#define SC4336_GOLDEN_RGAIN 0
#define SC4336_GOLDEN_BGAIN 0

static sensor_context *g_sc4336_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define SC4336_GET_CTX(dev, ctx) ctx = g_sc4336_ctx[dev]
#define SC4336_SET_CTX(dev, ctx) g_sc4336_ctx[dev] = ctx

static xmedia_s32 g_sc4336_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] =
                                                                      SENSOR_DEV_INVALID };

#define SC4336_AGAIN_INDEX_MAX 6
static const xmedia_s32 g_sc4336_again_real_cal[SC4336_AGAIN_INDEX_MAX] = {
    1024,  2048,  4096,  8192, 16384, 32768
};

static const xmedia_s32 g_sc4336_again_reg_table[SC4336_AGAIN_INDEX_MAX] = { 0x00,  0x08, 0x09,  0x0b, 0x0f,  0x1f};

#define SC4336_DGAIN_INDEX_MAX 128
static const xmedia_s32 dgain_index[SC4336_DGAIN_INDEX_MAX] = {
    1024, 1056, 1088, 1120,  1152, 1184, 1216, 1248,  1280, 1312, 1344, 1376,
    1408, 1440, 1472, 1504,  1536, 1568, 1600, 1632,  1664, 1696, 1728, 1760,
    1792, 1824, 1856, 1888,  1920, 1952, 1984, 2016,  2048, 2112, 2176, 2240,
    2304, 2368, 2432, 2496,  2560, 2624, 2688, 2752,  2816, 2880, 2944, 3008,
    3072, 3136, 3200, 3264,  3328, 3392, 3456, 3520,  3584, 3648, 3712, 3776,
    3840, 3904, 3968, 4032,  4096, 4224, 4352, 4480,  4608, 4736, 4864, 4992,
    5120, 5248, 5376, 5504,  5632, 5760, 5888, 6016,  6144, 6272, 6400, 6528,
    6656, 6784, 6912, 7040,  7168, 7296, 7424, 7552,  7680, 7808, 7936, 8064,
    8192, 8448, 8704, 8960,  9216, 9472, 9728, 9984, 10240, 10496, 10752, 11008,
    11264, 11520, 11776, 12032, 12288, 12544, 12800, 13056, 13312, 13568, 13824, 14080,
    14336, 14592, 14848, 15104, 15360, 15616, 15872, 16128
};

static const xmedia_s32 dgain_table[SC4336_DGAIN_INDEX_MAX] = {
    0x0080, 0x0084, 0x0088, 0x008C, 0x0090, 0x0094, 0x0098, 0x009C, 0x00A0, 0x00A4, 0x00A8, 0x00AC,
    0x00B0, 0x00B4, 0x00B8, 0x00BC, 0x00C0, 0x00C4, 0x00C8, 0x00CC, 0x00D0, 0x00D4, 0x00D8, 0x00DC,
    0x00E0, 0x00E4, 0x00E8, 0x00EC, 0x00F0, 0x00F4, 0x00F8, 0x00FC, 0x0180, 0x0184, 0x0188, 0x018C,
    0x0190, 0x0194, 0x0198, 0x019C, 0x01A0, 0x01A4, 0x01A8, 0x01AC, 0x01B0, 0x01B4, 0x01B8, 0x01BC,
    0x01C0, 0x01C4, 0x01C8, 0x01CC, 0x01D0, 0x01D4, 0x01D8, 0x01DC, 0x01E0, 0x01E4, 0x01E8, 0x01EC,
    0x01F0, 0x01F4, 0x01F8, 0x01FC, 0x0380, 0x0384, 0x0388, 0x038C, 0x0390, 0x0394, 0x0398, 0x039C,
    0x03A0, 0x03A4, 0x03A8, 0x03AC, 0x03B0, 0x03B4, 0x03B8, 0x03BC, 0x03C0, 0x03C4, 0x03C8, 0x03CC,
    0x03D0, 0x03D4, 0x03D8, 0x03DC, 0x03E0, 0x03E4, 0x03E8, 0x03EC, 0x03F0, 0x03F4, 0x03F8, 0x03FC,
    0x0780, 0x0784, 0x0788, 0x078C, 0x0790, 0x0794, 0x0798, 0x079C, 0x07A0, 0x07A4, 0x07A8, 0x07AC,
    0x07B0, 0x07B4, 0x07B8, 0x07BC, 0x07C0, 0x07C4, 0x07C8, 0x07CC, 0x07D0, 0x07D4, 0x07D8, 0x07DC,
    0x07E0, 0x07E4, 0x07E8, 0x07EC, 0x07F0, 0x07F4, 0x07F8, 0x07FC,
};

static xmedia_u32 g_sc4336_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_sc4336_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_sc4336_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_sc4336_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

static const xmedia_sensor_property g_sc4336_property[SC4336_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        2560, 1440, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_VC, 30, XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR,
          XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_BGGR },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, SC4336_4M_BIT_RATE_LINEAR,
    },
};

static const xmedia_sensor_capability g_sc4336_capability = {
    .max_width  = 2560,
    .max_height = 1440,
    .max_fps    = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_27MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_TRUE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_2L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = SC4336_ADDR_BYTE,
        .data_byte_num     = SC4336_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { SC4336_REG_ADDR_STANDBY },
        .standby_reg_data  = { 0x00 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { SC4336_REG_ADDR_STANDBY },
        .resume_reg_data   = { 0x01 },
    },
};

static xmedia_s32 sc4336_ctx_init(xmedia_u32 dev)
{
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SC4336_GET_CTX(dev, sc4336_ctx);
    if (sc4336_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    sc4336_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (sc4336_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }
    memset(sc4336_ctx, 0, sizeof(sensor_context));
    sc4336_ctx->i2c_addr             = SC4336_I2C_ADDR;
    sc4336_ctx->lanes                = XMEDIA_SENSOR_MIPI_LANES_2L;
    sc4336_ctx->work_mode            = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    sc4336_ctx->bus_info.i2c_dev     = SENSOR_I2C_INVALID;
    sc4336_ctx->size.width           = g_sc4336_property[0].width;
    sc4336_ctx->size.height          = g_sc4336_property[0].height;
    sc4336_ctx->fps                  = g_sc4336_property[0].max_fps;
    sc4336_ctx->wdr_mode             = g_sc4336_property[0].wdr_mode;
    sc4336_ctx->img_mode             = SC4336_4M_30FPS_10BIT_LINEAR_MODE;
    sc4336_ctx->fl_std               = SC4336_VMAX_4M_LINEAR;
    sc4336_ctx->fl[SENSOR_CUR_FRAME] = SC4336_VMAX_4M_LINEAR;
    sc4336_ctx->fl[SENSOR_PRE_FRAME] = SC4336_VMAX_4M_LINEAR;

    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_EXP_H].data   = 0x00;
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_EXP_M].data   = 0xbb;
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_EXP_L].data   = 0x00;
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_AGAIN_H].data = 0x00; // no use
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_AGAIN_L].data = 0x00; // again
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_DGAIN_H].data = 0x00; // dgain
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_DGAIN_L].data = 0x80; // dfine gain
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_VMAX_H].data  = 0x05; // vmax
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_VMAX_L].data  = 0xdc;

    SC4336_SET_CTX(dev, sc4336_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void sc4336_ctx_exit(xmedia_u32 dev)
{
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_FREE(sc4336_ctx);
    SC4336_SET_CTX(dev, XMEDIA_NULL);

    return;
}

static xmedia_s32 sc4336_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    for (i = 0; i < SC4336_SPECS_MAX_NUM; i++) {
        if (g_sc4336_property[i].width == sc4336_ctx->size.width &&
            g_sc4336_property[i].height == sc4336_ctx->size.height &&
            g_sc4336_property[i].wdr_mode == sc4336_ctx->wdr_mode) {
            memcpy(property, &g_sc4336_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Please check input param!\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 sc4336_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc4336_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Invalid mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc4336_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        sc4336_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        sc4336_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        sc4336_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

/* one i2c dev shared by multiple sensors, slave_addr represent the real slave addr,
 * sensor driver will not do other operation.
 */
static xmedia_s32 sc4336_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    sc4336_ctx->i2c_addr = slave_addr;
    ret = sc4336_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    *slave_addr = sc4336_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

// called by ISP
static xmedia_s32 sc4336_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    sc4336_ctx->init_mode = init_mode;
    ret = sc4336_i2c_init(dev, sc4336_ctx->bus_info.i2c_dev, sc4336_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    sc4336_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    ret = sc4336_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    sc4336_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    sc4336_ctx->mirror_en = mirror_en;
    ret = sc4336_set_mirror_flip(dev, mirror_en, sc4336_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    sc4336_ctx->flip_en = flip_en;
    ret = sc4336_set_mirror_flip(dev, sc4336_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_set_image_mode(sensor_context *sc4336_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (sc4336_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (SC4336_RES_IS_1440P(sns_attr->width, sns_attr->height)) {
            *image_mode         = SC4336_4M_30FPS_10BIT_LINEAR_MODE;
            sc4336_ctx->fl_std = SC4336_VMAX_4M_LINEAR;
        } else {
            SC4336_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        SC4336_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc4336_ctx->size.width  = sns_attr->width;
    sc4336_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            sc4336_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(sc4336_ctx->wdr_int_time, 0, sizeof(sc4336_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

static xmedia_void sc4336_init_common_reg_info(sensor_context *sc4336_ctx)
{
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_EXP_H].delay_frame_num = 2;
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_EXP_H].reg_addr        = SC4336_REG_ADDR_EXP_H;
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_EXP_M].delay_frame_num = 2;
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_EXP_M].reg_addr        = SC4336_REG_ADDR_EXP_M;
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_EXP_L].delay_frame_num = 2;
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_EXP_L].reg_addr        = SC4336_REG_ADDR_EXP_L;

    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_AGAIN_H].delay_frame_num = 2;
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_AGAIN_H].reg_addr        = SC4336_REG_ADDR_AGAIN_H;
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_AGAIN_L].delay_frame_num = 2;
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_AGAIN_L].reg_addr        = SC4336_REG_ADDR_AGAIN_L;

    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_DGAIN_H].delay_frame_num = 2;
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_DGAIN_H].reg_addr        = SC4336_REG_ADDR_DGAIN_H;
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_DGAIN_L].delay_frame_num = 2;
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_DGAIN_L].reg_addr        = SC4336_REG_ADDR_DGAIN_L;

    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_VMAX_H].delay_frame_num = 2;
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_VMAX_H].reg_addr        = SC4336_REG_ADDR_VMAX_H;
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_VMAX_L].delay_frame_num = 2;
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_VMAX_L].reg_addr        = SC4336_REG_ADDR_VMAX_L;

    return;
}

static xmedia_s32 sc4336_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = sc4336_ctx->bus_info.i2c_dev;
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = SC4336_REG_L_MAX_NUM;

    for (i = 0; i < sc4336_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = sc4336_ctx->i2c_addr;
        sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = SC4336_ADDR_BYTE;
        sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = SC4336_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    sc4336_init_common_reg_info(sc4336_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    // Set wdr mode
    ret = sc4336_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = sc4336_set_image_mode(sc4336_ctx, &sc4336_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    sc4336_ctx->fl[SENSOR_CUR_FRAME] = sc4336_ctx->fl_std;
    sc4336_ctx->fl[SENSOR_PRE_FRAME] = sc4336_ctx->fl[SENSOR_CUR_FRAME];

    ret = sc4336_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef SC4336_ISP_DEFAULT_SUPPORT
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from sc4336!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 sc4336_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (sc4336_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
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

static xmedia_s32 sc4336_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    if (sc4336_ctx->mirror_en && sc4336_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (sc4336_ctx->mirror_en && (!sc4336_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!sc4336_ctx->mirror_en) && sc4336_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < SC4336_SPECS_MAX_NUM; i++) {
        if (g_sc4336_property[i].width == sc4336_ctx->size.width &&
            g_sc4336_property[i].height == sc4336_ctx->size.height &&
            g_sc4336_property[i].wdr_mode == sc4336_ctx->wdr_mode) {
            *bayer_pattern = g_sc4336_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= SC4336_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);
    memcpy(capability, &g_sc4336_capability, sizeof(xmedia_sensor_capability));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    sc4336_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 sc4336_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_sc4336_exposure[dev]      = init_param->exposure;
    g_sc4336_sample_r_gain[dev] = init_param->sample_rgain;
    g_sc4336_sample_b_gain[dev] = init_param->sample_bgain;

    g_sc4336_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_sc4336_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_sc4336_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc4336_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case SC4336_4M_30FPS_10BIT_LINEAR_MODE:
            *max_fps = 30.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = sc4336_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   = 50 * 256;//50 * 256;
    ae_sns_dft->full_lines_max = SC4336_FULL_LINES_MAX;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->dgain_accu.accuracy  = 1;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 16 << ae_sns_dft->ispdgain_shift;

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

static xmedia_void sc4336_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_again        = 32*1024; // max: 15.875 * 1024
    ae_sns_dft->min_again        = 1024;  // min: 1 * 1024
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 1024; // max: 31.5 * 1024
    ae_sns_dft->min_dgain        = 1024;  // min: 1 * 1024
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x40;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;

    ae_sns_dft->init_exposure = g_sc4336_exposure[dev] ? g_sc4336_exposure[dev] : 148859;

    ae_sns_dft->max_int_time        = sns_ctx->fl_std - SC4336_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time        = 1;     // min 3
    ae_sns_dft->max_int_time_target = 65535; // max 65535
    ae_sns_dft->min_int_time_target = 1;     // min 3

    return;
}

static xmedia_s32 sc4336_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = sc4336_get_ae_common_default(dev, sc4336_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (sc4336_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            sc4336_get_ae_linear_default(dev, sc4336_ctx, ae_sns_dft);
            break;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context    *sc4336_ctx = XMEDIA_NULL;
    xmedia_u32 int_time_val;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    int_time_val = SENSOR_MIN(int_time, sc4336_ctx->fl_std - SC4336_EXP_OFFSET_LINEAR);
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_EXP_H].data = SENSOR_HIGHER_4BITS(int_time_val);
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_EXP_M].data = SENSOR_MIDDLE_8BITS(int_time_val);
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_EXP_L].data = SENSOR_LOWER_4BITS(int_time_val);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_sc4336_again_real_cal[SC4336_AGAIN_INDEX_MAX - 1]) {
        *again_db = 5;
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < SC4336_AGAIN_INDEX_MAX; i++) {
        if (*again_lin < g_sc4336_again_real_cal[i]) {
            *again_db = i - 1;
            break;
        }
    }

    *again_lin = g_sc4336_again_real_cal[*again_db];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    xmedia_s32 i;
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(dgain_lin);
    SENSOR_CHECK_PTR_RETURN(dgain_db);

    if (*dgain_lin >= dgain_index[SC4336_DGAIN_INDEX_MAX - 1]) {
        *dgain_lin = dgain_index[SC4336_DGAIN_INDEX_MAX - 1];
        *dgain_db = dgain_table[SC4336_DGAIN_INDEX_MAX - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < SC4336_DGAIN_INDEX_MAX; i++) {
        if (*dgain_lin < dgain_index[i]) {
            *dgain_lin = dgain_index[i - 1];
            *dgain_db = dgain_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *sc4336_ctx = XMEDIA_NULL;
    xmedia_u8 reg_0x3e09;       // again
    xmedia_u8 reg_0x3e07;       // dgain
    xmedia_u8 reg_0x3e06;
    xmedia_u16 again_temp;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    again_temp = g_sc4336_again_reg_table[again];

    reg_0x3e06 = SENSOR_HIGH_8BITS(dgain);
    reg_0x3e07 = SENSOR_LOW_8BITS (dgain);
    reg_0x3e09 = SENSOR_LOW_8BITS (again_temp);

    sc4336_ctx->regs_info[0].i2c_data[SC4336_REG_DGAIN_H].data = reg_0x3e06;
    sc4336_ctx->regs_info[0].i2c_data[SC4336_REG_DGAIN_L].data = reg_0x3e07;

    sc4336_ctx->regs_info[0].i2c_data[SC4336_REG_AGAIN_L].data = reg_0x3e09;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = SC4336_CALIBRATE_STATIC_TEMP;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = SC4336_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = SC4336_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = SC4336_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = SC4336_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 5: point index on the awb curve param
    awb_sns_dft->wb_para[0] = SC4336_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1] = SC4336_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2] = SC4336_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3] = SC4336_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4] = SC4336_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5] = SC4336_CALIBRATE_AWB_C1;

    awb_sns_dft->golden_rgain = SC4336_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = SC4336_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (sc4336_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_sc4336_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_sc4336_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
    }

    awb_sns_dft->init_rgain   = g_sc4336_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_sc4336_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_sc4336_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_sc4336_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_sc4336_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_standby(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc4336_write_reg(dev, SC4336_REG_ADDR_STANDBY, 0x00);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_resume(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc4336_write_reg(dev, SC4336_REG_ADDR_STANDBY, 0x01);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *sc4336_ctx   = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < sc4336_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < sc4336_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= sc4336_write_reg(dev, sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_start(xmedia_u32 dev)
{
    xmedia_s32               ret;
    xmedia_u8                img_mode;
    xmedia_sensor_mipi_lanes mipi_lanes;
    xmedia_sensor_work_mode  work_mode;
    sensor_context          *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    work_mode  = sc4336_ctx->work_mode;
    mipi_lanes = sc4336_ctx->lanes;
    img_mode   = sc4336_ctx->img_mode;

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL || mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check param!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (sc4336_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = sc4336_init_image(dev, img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = sc4336_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc4336_set_mirror_flip(dev, sc4336_ctx->mirror_en, sc4336_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = sc4336_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_sc4336_dev_map[index] == SENSOR_DEV_INVALID) {
            g_sc4336_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 sc4336_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_sc4336_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 sc4336_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{ // TO-DO：min fps is to be confirmed.
    switch (img_mode) {
        case SC4336_4M_30FPS_10BIT_LINEAR_MODE:
            *min_fps = 4.0;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case SC4336_4M_30FPS_10BIT_LINEAR_MODE:
            *vmax = SC4336_VMAX_4M_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = sc4336_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc4336_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc4336_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is [%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    for (i = 0; i < sc4336_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            sc4336_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    memcpy(sns_regs_info, &sc4336_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&sc4336_ctx->regs_info[SENSOR_PRE_FRAME], &sc4336_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    sc4336_ctx->fl[SENSOR_PRE_FRAME] = sc4336_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    ret = sc4336_calc_fps(fps, sc4336_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    sc4336_ctx->regs_info[0].i2c_data[SC4336_REG_VMAX_H].data = SENSOR_HIGH_8BITS(full_lines);
    sc4336_ctx->regs_info[0].i2c_data[SC4336_REG_VMAX_L].data = SENSOR_LOW_8BITS(full_lines);

    sc4336_ctx->fl_std = SENSOR_MIN(full_lines, SC4336_FULL_LINES_MAX);
    sc4336_ctx->fl[SENSOR_CUR_FRAME] = sc4336_ctx->fl_std;
    sc4336_ctx->fps                  = fps;
    ae_sns_dft->fps                  = fps;
    ae_sns_dft->full_lines_std       = sc4336_ctx->fl_std;
    ae_sns_dft->full_lines           = sc4336_ctx->fl[SENSOR_CUR_FRAME];

    if (sc4336_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sc4336_ctx->fl[SENSOR_CUR_FRAME] - SC4336_EXP_OFFSET_LINEAR;
    } else {
        // not support
    }

    SENSOR_PRINT("dev[%d]- sc4336 set fps: %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_get_min_fps_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax_max_fps;
    xmedia_s32   ret;

    ret = sc4336_get_max_fps(img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc4336_get_min_fps(img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc4336_get_vmax(img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    *vmax = vmax_max_fps * max_fps / min_fps;        // SC4336_EXP_SHIFT移位有sensor差异
    return XMEDIA_SUCCESS;
}

// line wdr模式下，根据AE期望曝光比，返回不同模式下长短帧的最大/最小曝光时间
static xmedia_s32 sc4336_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 * 注意：2to1 wdr模式下，曝光比和ae wdr mode 由AE传入，调用本接口前，AE需先调用sc4336_get_max_inttime
 */
static xmedia_s32 sc4336_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *sc4336_ctx = XMEDIA_NULL;
    xmedia_u32      vmax_min_fps;
    xmedia_s32      ret;
    xmedia_float    min_fps;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    if (sc4336_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", sc4336_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (sc4336_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        full_lines = full_lines + SC4336_EXP_OFFSET_LINEAR;
    }

    ret = sc4336_get_min_fps(sc4336_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc4336_get_min_fps_vmax(sc4336_ctx->img_mode, &vmax_min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    full_lines                       = (full_lines > vmax_min_fps) ? vmax_min_fps : full_lines;
    sc4336_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    sc4336_ctx->fps                  = min_fps * vmax_min_fps / full_lines;

    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_VMAX_H].data =
        SENSOR_HIGH_8BITS(full_lines);
    sc4336_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[SC4336_REG_VMAX_L].data =
        SENSOR_LOW_8BITS(full_lines);

    ae_sns_dft->full_lines = sc4336_ctx->fl[SENSOR_CUR_FRAME];
    if (sc4336_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        ae_sns_dft->max_int_time = sc4336_ctx->fl[SENSOR_CUR_FRAME] - SC4336_EXP_OFFSET_LINEAR;
    }

    SENSOR_PRINT("dev[%d]- sc4336 set fps: %f\n", dev, sc4336_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    ret = sc4336_get_min_fps(sc4336_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = sc4336_get_max_fps(sc4336_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = sc4336_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc4336_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, SC4336_NAME, sizeof(SC4336_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = sc4336_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = sc4336_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = sc4336_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = sc4336_mirror;
    info->isp_func.pfn_sensor_flip             = sc4336_flip;
    info->isp_func.pfn_sensor_set_bus_info     = sc4336_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = sc4336_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = sc4336_set_init_param;
    info->isp_func.pfn_sensor_start            = sc4336_start;
    info->isp_func.pfn_sensor_stop             = sc4336_stop;
    info->isp_func.pfn_sensor_standby          = sc4336_standby;
    info->isp_func.pfn_sensor_resume           = sc4336_resume;
    info->isp_func.pfn_sensor_write_reg        = sc4336_write_reg;
    info->isp_func.pfn_sensor_read_reg         = sc4336_read_reg;
    info->isp_func.pfn_sensor_init             = sc4336_init;
    info->isp_func.pfn_sensor_exit             = sc4336_exit;

    // called by ISP
    info->isp_func.pfn_sensor_set_attr            = sc4336_set_attr;
    info->isp_func.pfn_sensor_get_isp_default     = sc4336_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = sc4336_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = sc4336_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = sc4336_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = sc4336_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = sc4336_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = sc4336_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = sc4336_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = sc4336_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = sc4336_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = sc4336_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = sc4336_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = sc4336_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = sc4336_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = sc4336_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = sc4336_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = sc4336_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc4336_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = sc4336_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = sc4336_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = sc4336_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "SC4336 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 sc4336_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *sc4336_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = sc4336_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    SC4336_GET_CTX(dev, sc4336_ctx);
    SENSOR_CHECK_PTR_RETURN(sc4336_ctx);

    if (sc4336_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "SC4336 unregister function failed!\n");
        return ret;
    }

    sc4336_ctx_exit(dev);
    g_sc4336_dev_map[dev] = SENSOR_DEV_INVALID; // reset dev

    return XMEDIA_SUCCESS;
}
