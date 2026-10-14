#include <stdlib.h>
#include <string.h>
#include "imx179.h"
#include "imx179_ctrl.h"
#include "imx179_ex.h"

#define IMX179_NAME "IMX179"
#define IMX179_SPECS_MAX_NUM 2

#define IMX179_8M_BIT_RATE_LINEAR       330
#define IMX179_640_480_BIT_RATE_LINEAR  309

#ifdef __linux__
#define IMX179_ISP_DEFAULT_SUPPORT
#endif

#define IMX179_RES_IS_2464P(w, h) ((w) == 3280 && (h) == 2464)
#define IMX179_RES_IS_480P(w, h)  ((w) == 640 && (h) == 480)

#define IMX179_ERR_MODE_PRINT(sensor_image_mode)                                                                       \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sensor_image_mode->width,       \
                     sensor_image_mode->height, sensor_image_mode->wdr_mode);                                          \
    } while (0)

xmedia_isp_ae_wdr_mode g_imx179_wdr_mode[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = 0 };

sensor_context*g_imx179_ctx[XMEDIA_SENSOR_DEV_MAX_NUM]  = { XMEDIA_NULL };
#define IMX179_GET_CTX(dev, ctx) ctx = g_imx179_ctx[dev]
#define IMX179_SET_CTX(dev, ctx) g_imx179_ctx[dev] = ctx

static xmedia_s32 g_imx179_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM]  =
                                                           {[0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] = SENSOR_DEV_INVALID};

static xmedia_u32 g_imx179_again[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_imx179_dgain[XMEDIA_SENSOR_DEV_MAX_NUM]     = { 0 };
static xmedia_u16 g_imx179_isp_dgain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_imx179_init_time[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };

static xmedia_u32 g_imx179_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]        = { 0 };
static xmedia_u16 g_imx179_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM]   = { 0 };
static xmedia_u16 g_imx179_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM]   = { 0 };

static xmedia_u16 g_imx179_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

/* Rgain and Bgain of the golden sample */
#define GOLDEN_RGAIN 0
#define GOLDEN_BGAIN 0

/* AWB default parameter and function */
#define CALIBRATE_STATIC_WB_R_GAIN  445
#define CALIBRATE_STATIC_WB_GR_GAIN 256
#define CALIBRATE_STATIC_WB_GB_GAIN 256
#define CALIBRATE_STATIC_WB_B_GAIN  669

/* Calibration results for Auto WB Planck */
#define CALIBRATE_AWB_P1 100
#define CALIBRATE_AWB_P2 37
#define CALIBRATE_AWB_Q1 (-118)
#define CALIBRATE_AWB_A1 204157
#define CALIBRATE_AWB_B1 128
#define CALIBRATE_AWB_C1 (-140493)

#define GAIN_NODE_NUM    97

static const xmedia_u32 g_imx179_gain_index[GAIN_NODE_NUM] = {
    1024, 1088, 1136, 1184, 1232, 1296, 1344, 1392, 1440, 1488,  1536,  1600,  1648, 1696, 1744, 1808, 1856,
    1920, 1952, 2000, 2048, 2112, 2160, 2208, 2256, 2304, 2368,  2416,  2448,  2512, 2560, 2624, 2688, 2704,
    2768, 2816, 2880, 2912, 2992, 3024, 3088, 3136, 3200, 3248,  3280,  3328,  3408, 3456, 3504, 3552, 3600,
    3648, 3712, 3760, 3808, 3856, 3920, 3984, 4048, 4096, 4160,  4240,  4304,  4384, 4448, 4528, 4608, 4688,
    4768, 4864, 4960, 5040, 5152, 5248, 5360, 5472, 5584, 5712,  5840,  5968,  6096, 6400, 6720, 6912, 7088,
    7488, 7712, 7952, 8192, 8464, 8736, 9056, 9360, 9712, 10096, 10496, 10928,
};

static const xmedia_u32 g_imx179_gain_table[GAIN_NODE_NUM] = {
    0,   12,  23,  33,  42,  52,  59,  66,  73,  79,  85,  91,  96,  101, 105, 110, 114, 118, 121, 125,
    128, 131, 134, 137, 139, 142, 145, 147, 149, 151, 153, 156, 158, 159, 161, 163, 165, 166, 168, 169,
    171, 172, 174, 175, 176, 177, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192,
    193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212,
    213, 215, 217, 218, 219, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232,
};

static const xmedia_sensor_property g_imx179_property[IMX179_SPECS_MAX_NUM] = {
    {
        // width, height, wdr_mode
        3280, 2464, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_NONE, 30, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_GRBG,
          XMEDIA_VIDEO_BAYER_FMT_GBRG, XMEDIA_VIDEO_BAYER_FMT_RGGB },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, IMX179_8M_BIT_RATE_LINEAR,
    },
    {
        // width, height, wdr_mode
        640, 480, XMEDIA_VIDEO_WDR_MODE_NONE,
        // wdr_format, max_fps, input_clock
        XMEDIA_VIDEO_WDR_FMT_NONE, 120, XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
        // output_intf, pixel_format, bit_width
        XMEDIA_INTF_TYPE_MIPI_CSI, XMEDIA_VIDEO_PIXEL_FMT_RAW, XMEDIA_VIDEO_DATA_WIDTH_10,
        // bayer_format - mirror&flip /mirror /flip /normal
        { XMEDIA_VIDEO_BAYER_FMT_BGGR, XMEDIA_VIDEO_BAYER_FMT_GRBG,
          XMEDIA_VIDEO_BAYER_FMT_GBRG, XMEDIA_VIDEO_BAYER_FMT_RGGB },
        // vcid, bit_rate (TODO: bit_rate to be confirmed)
        { 0 }, IMX179_640_480_BIT_RATE_LINEAR,
    },

};

static const xmedia_sensor_capability g_imx179_capability = {
    .max_width  = 3280,
    .max_height = 2464,
    .max_fps    = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_2TO1_LINE,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_4L,

    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = IMX179_ADDR_BYTE,
        .data_byte_num     = IMX179_DATA_BYTE,
        .standby_reg_num   = 1,
        .standby_reg_addr  = { 0x0100 },
        .standby_reg_data  = { 0x01 },
        .resume_reg_num    = 1,
        .resume_reg_addr   = { 0x0100 },
        .resume_reg_data   = { 0x00 },
    },
};

static xmedia_s32 imx179_ctx_init(xmedia_u32 dev)
{
    sensor_context *imx179_ctx = XMEDIA_NULL;

    IMX179_GET_CTX(dev, imx179_ctx);

    if (imx179_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    imx179_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (imx179_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(imx179_ctx, 0, sizeof(sensor_context));
    imx179_ctx->i2c_addr         = IMX179_I2C_ADDR;
    imx179_ctx->lanes            = XMEDIA_SENSOR_MIPI_LANES_2L;
    imx179_ctx->work_mode        = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    imx179_ctx->bus_info.i2c_dev = SENSOR_I2C_INVALID;
    imx179_ctx->size.width       = 3280;
    imx179_ctx->size.height      = 2464;
    imx179_ctx->fps              = g_imx179_property[0].max_fps;
    imx179_ctx->wdr_mode         = XMEDIA_VIDEO_WDR_MODE_NONE;

    imx179_ctx->img_mode             = IMX179_8M_10BIT_LINEAR_MODE;
    imx179_ctx->fl_std               = IMX179_VMAX_8M_LINEAR;
    imx179_ctx->fl[SENSOR_CUR_FRAME] = IMX179_VMAX_8M_LINEAR;
    imx179_ctx->fl[SENSOR_PRE_FRAME] = IMX179_VMAX_8M_LINEAR;
    IMX179_SET_CTX(dev, imx179_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_void imx179_ctx_exit(xmedia_u32 dev)
{
    sensor_context *imx179_ctx = XMEDIA_NULL;

    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_FREE(imx179_ctx);
    IMX179_SET_CTX(dev, XMEDIA_NULL);
}

static xmedia_s32 imx179_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    for (i = 0; i < IMX179_SPECS_MAX_NUM; i++) {
        if (g_imx179_property[i].width    == imx179_ctx->size.width &&
            g_imx179_property[i].height   == imx179_ctx->size.height &&
            g_imx179_property[i].wdr_mode == imx179_ctx->wdr_mode) {
            memcpy(property, &g_imx179_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width: %d, Height: %d, Wdr_mode: %d !\n",
                 imx179_ctx->size.width, imx179_ctx->size.height, imx179_ctx->wdr_mode);
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 imx179_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    imx179_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_4L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "not supported mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    imx179_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        imx179_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        imx179_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        imx179_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;
    sensor_context *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    imx179_ctx->i2c_addr = slave_addr;
    ret = imx179_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    *slave_addr = imx179_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}


static xmedia_s32 imx179_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32     ret;
    sensor_context *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    imx179_ctx->init_mode = init_mode;
    ret = imx179_i2c_init(dev, imx179_ctx->bus_info.i2c_dev, imx179_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;
    sensor_context *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    ret = imx179_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32 ret;
    sensor_context *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    imx179_ctx->mirror_en = mirror_en;
    ret = imx179_set_mirror_flip(dev, mirror_en, imx179_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    sensor_context *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    imx179_ctx->flip_en = flip_en;
    ret = imx179_set_mirror_flip(dev, imx179_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_set_image_mode(sensor_context *imx179_ctx, xmedia_u8 *image_mode,
                                                 const xmedia_sensor_attr *sns_attr)
{
     SENSOR_CHECK_PTR_RETURN(sns_attr);
     SENSOR_CHECK_PTR_RETURN(image_mode);

    if (imx179_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (IMX179_RES_IS_2464P(sns_attr->width, sns_attr->height)) {
            *image_mode           = IMX179_8M_10BIT_LINEAR_MODE;
             imx179_ctx->fl_std   = IMX179_VMAX_8M_LINEAR;
             return XMEDIA_SUCCESS;
        } else if(IMX179_RES_IS_480P(sns_attr->width, sns_attr->height)) {
            *image_mode           = IMX179_1M_10BIT_LINEAR_MODE;
             imx179_ctx->fl_std   = IMX179_VMAX_640_480_LINEAR;
            return XMEDIA_SUCCESS;
        }
    }

    IMX179_ERR_MODE_PRINT(sns_attr);
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 imx179_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context  *imx179_ctx = XMEDIA_NULL;

    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            imx179_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }
    memset(imx179_ctx->wdr_int_time, 0, sizeof(imx179_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

static xmedia_void imx179_init_common_reg_info(sensor_context *imx179_ctx)
{
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_GROUP_HOLD_START].delay_frame_num = 2;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_GROUP_HOLD_START].reg_addr = IMX179_REG_ADDR_GROUP_HOLD;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_GROUP_HOLD_START].data = 1;

    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_EXP_H].delay_frame_num   = 2;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_EXP_H].reg_addr          = IMX179_REG_ADDR_EXP_H;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_EXP_L].delay_frame_num   = 2;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_EXP_L].reg_addr          = IMX179_REG_ADDR_EXP_L;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_AGAIN_H].delay_frame_num = 2;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_AGAIN_H].reg_addr        = IMX179_REG_ADDR_AGAIN_H;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_AGAIN_L].delay_frame_num = 2;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_AGAIN_L].reg_addr        = IMX179_REG_ADDR_AGAIN_L;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_DGAIN_GR_H].delay_frame_num = 2;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_DGAIN_GR_H].reg_addr       = IMX179_REG_ADDR_DGAIN_GR_H;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_DGAIN_GR_L].delay_frame_num = 2;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_DGAIN_GR_L].reg_addr       = IMX179_REG_ADDR_DGAIN_GR_L;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_DGAIN_R_H].delay_frame_num = 2;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_DGAIN_R_H].reg_addr        = IMX179_REG_ADDR_DGAIN_R_H;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_DGAIN_R_L].delay_frame_num = 2;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_DGAIN_R_L].reg_addr        = IMX179_REG_ADDR_DGAIN_R_L;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_DGAIN_B_H].delay_frame_num = 2;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_DGAIN_B_H].reg_addr        = IMX179_REG_ADDR_DGAIN_B_H;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_DGAIN_B_L].delay_frame_num = 2;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_DGAIN_B_L].reg_addr        = IMX179_REG_ADDR_DGAIN_B_L;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_DGAIN_GB_H].delay_frame_num = 2;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_DGAIN_GB_H].reg_addr       = IMX179_REG_ADDR_DGAIN_GB_H;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_DGAIN_GB_L].delay_frame_num = 2;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_DGAIN_GB_L].reg_addr       = IMX179_REG_ADDR_DGAIN_GB_L;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_VMAX_H].delay_frame_num  = 2;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_VMAX_H].reg_addr         = IMX179_REG_ADDR_VMAX_H;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_VMAX_L].delay_frame_num  = 2;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_VMAX_L].reg_addr         = IMX179_REG_ADDR_VMAX_L;

    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_GROUP_HOLD_END].delay_frame_num = 2;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_GROUP_HOLD_END].reg_addr = IMX179_REG_ADDR_GROUP_HOLD;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_GROUP_HOLD_END].data = 0;
}

static xmedia_s32 imx179_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32     i;
    sensor_context *imx179_ctx  = XMEDIA_NULL;

    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    imx179_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = imx179_ctx->bus_info.i2c_dev;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2;
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = IMX179_REG_MAX_NUM;

    for (i = 0; i < imx179_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = imx179_ctx->i2c_addr;
        imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = IMX179_ADDR_BYTE;
        imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = IMX179_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    imx179_init_common_reg_info(imx179_ctx);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    // set wdr mode
    ret = imx179_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // set image mode
    ret = imx179_set_image_mode(imx179_ctx, &imx179_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    imx179_ctx->size.width  = sns_attr->width;
    imx179_ctx->size.height = sns_attr->height;
    imx179_ctx->fl[SENSOR_CUR_FRAME] = imx179_ctx->fl_std;
    imx179_ctx->fl[SENSOR_PRE_FRAME] = imx179_ctx->fl[SENSOR_CUR_FRAME];

    ret = imx179_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef IMX179_ISP_DEFAULT_SUPPORT
    //TO-DO: alg add param
    isp_default->blc         = &g_imx179_blc;;
    isp_default->bnr         = &g_imx179_bnr;
    isp_default->clut_attr   = XMEDIA_NULL;
    isp_default->clut_attr   = XMEDIA_NULL;
    isp_default->crosstalk   = &g_imx179_crosstalk;
    isp_default->csc         = &g_imx179_csc;
    isp_default->dehaze      = &g_imx179_dehaze;
    isp_default->demosaic    = &g_imx179_demosaic;
    isp_default->dpc_dynamic = XMEDIA_NULL;
    isp_default->dpc_static  = XMEDIA_NULL;
    isp_default->drc         = XMEDIA_NULL;
    isp_default->expander    = XMEDIA_NULL;
    isp_default->fpn         = XMEDIA_NULL;
    isp_default->gamma       = &g_imx179_gamma;
    isp_default->gcac        = XMEDIA_NULL;
    isp_default->hlc         = XMEDIA_NULL;
    isp_default->lce         = &g_imx179_lce;
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from imx179!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 imx179_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (imx179_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[0] = 200;
        black_level->black_level[1] = 200;
        black_level->black_level[2] = 200;
        black_level->black_level[3] = 200;

    } else { // black level of WDR mode
        black_level->black_level[0] = 200;
        black_level->black_level[1] = 200;
        black_level->black_level[2] = 200;
        black_level->black_level[3] = 200;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                 i = 0;
    xmedia_u8       mirror_flip = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    sensor_context  *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    if (imx179_ctx->mirror_en && imx179_ctx->flip_en) {
        mirror_flip = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (imx179_ctx->mirror_en && (!imx179_ctx->flip_en)) {
        mirror_flip = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!imx179_ctx->mirror_en) && imx179_ctx->flip_en) {
        mirror_flip = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        mirror_flip = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < IMX179_SPECS_MAX_NUM; i++) {
        if (g_imx179_property[i].width    == imx179_ctx->size.width &&
            g_imx179_property[i].height   == imx179_ctx->size.height &&
            g_imx179_property[i].wdr_mode == imx179_ctx->wdr_mode) {

            *bayer_pattern = g_imx179_property[i].bayer_format[mirror_flip];
            return XMEDIA_SUCCESS;;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 imx179_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);

    memcpy(capability, &g_imx179_capability, sizeof(xmedia_sensor_capability));

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    imx179_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_imx179_again[dev]         = init_param->again;
    g_imx179_dgain[dev]         = init_param->dgain;
    g_imx179_isp_dgain[dev]     = init_param->ispdgain;
    g_imx179_init_time[dev]     = init_param->exp_time;
    g_imx179_exposure[dev]      = init_param->exposure;
    g_imx179_sample_r_gain[dev] = init_param->sample_rgain;
    g_imx179_sample_b_gain[dev] = init_param->sample_bgain;
    g_imx179_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_imx179_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_imx179_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                           xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->flicker_freq   = 50 * 256;
    ae_sns_dft->full_lines     = sns_ctx->fl_std;
    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->full_lines_max = IMX179_FULL_LINES_MAX;
    ae_sns_dft->lines_per_500ms = sns_ctx->fl_std * 30 / 2;
    ae_sns_dft->hmax_times = (1000000000) / (sns_ctx->fl_std * 30);

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy  = 1;

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy  = 0.00390625;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 2 << ae_sns_dft->ispdgain_shift;

    memcpy(&ae_sns_dft->piris_attr, &g_imx179_piris_attr, sizeof(xmedia_isp_piris_attr));
    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_32_0;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    return XMEDIA_SUCCESS;
}

static xmedia_void imx179_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 2;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->max_int_time     = sns_ctx->fl_std - IMX179_EXP_OFFSET_LINEAR;
    ae_sns_dft->min_int_time     = 8;

    ae_sns_dft->max_again        = 10928;
    ae_sns_dft->min_again        = 1024;
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain        = 4057;
    ae_sns_dft->min_dgain        = 256;
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 255 << ae_sns_dft->ispdgain_shift;

    ae_sns_dft->ae_compensation = 0x38;
    ae_sns_dft->ae_exp_mode     = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure   = g_imx179_exposure[dev] ? g_imx179_exposure[dev] : 76151;
}

static xmedia_s32 imx179_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context  *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    memset(ae_sns_dft, 0, sizeof(xmedia_isp_ae_sensor_default));

    ret = imx179_get_ae_common_default(dev, imx179_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (imx179_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            imx179_get_ae_linear_default(dev, imx179_ctx, ae_sns_dft);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not supported wdr mode!\n");
            break;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_EXP_H].data = SENSOR_HIGH_8BITS(int_time);
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_EXP_L].data = SENSOR_LOW_8BITS(int_time);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_u32 i;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(again_lin);
    SENSOR_CHECK_PTR_RETURN(again_db);

    if (*again_lin >= g_imx179_gain_index[GAIN_NODE_NUM - 1]) {
        *again_lin = g_imx179_gain_index[GAIN_NODE_NUM - 1];
        *again_db  = g_imx179_gain_table[GAIN_NODE_NUM - 1];
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < GAIN_NODE_NUM; i++) {
        if (*again_lin < g_imx179_gain_index[i]) {
            *again_lin = g_imx179_gain_index[i - 1];
            *again_db  = g_imx179_gain_table[i - 1];
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(dgain_lin);
    SENSOR_CHECK_PTR_RETURN(dgain_db);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *imx179_ctx = XMEDIA_NULL;

    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    imx179_ctx->regs_info[0].i2c_data[IMX179_REG_AGAIN_H].data    = SENSOR_HIGH_8BITS(again);
    imx179_ctx->regs_info[0].i2c_data[IMX179_REG_AGAIN_L].data    = SENSOR_LOW_8BITS(again);
    imx179_ctx->regs_info[0].i2c_data[IMX179_REG_DGAIN_GR_H].data = SENSOR_HIGH_8BITS(dgain);
    imx179_ctx->regs_info[0].i2c_data[IMX179_REG_DGAIN_GR_L].data = SENSOR_LOW_8BITS(dgain);
    imx179_ctx->regs_info[0].i2c_data[IMX179_REG_DGAIN_R_H].data  = SENSOR_HIGH_8BITS(dgain);
    imx179_ctx->regs_info[0].i2c_data[IMX179_REG_DGAIN_R_L].data  = SENSOR_LOW_8BITS(dgain);
    imx179_ctx->regs_info[0].i2c_data[IMX179_REG_DGAIN_B_H].data  = SENSOR_HIGH_8BITS(dgain);
    imx179_ctx->regs_info[0].i2c_data[IMX179_REG_DGAIN_B_L].data  = SENSOR_LOW_8BITS(dgain);
    imx179_ctx->regs_info[0].i2c_data[IMX179_REG_DGAIN_GB_H].data = SENSOR_HIGH_8BITS(dgain);
    imx179_ctx->regs_info[0].i2c_data[IMX179_REG_DGAIN_GB_L].data = SENSOR_LOW_8BITS(dgain);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context  *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = 4950;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = CALIBRATE_STATIC_WB_B_GAIN;

    awb_sns_dft->wb_para[0]   = CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1]   = CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2]   = CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3]   = CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4]   = CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5]   = CALIBRATE_AWB_C1;
    awb_sns_dft->golden_rgain = GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = GOLDEN_BGAIN;

    switch (imx179_ctx->wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            memcpy(&awb_sns_dft->ccm, &g_awb_ccm_wdr, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_awb_agc_table_wdr, sizeof(xmedia_isp_awb_agc_table));
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support wdr mode: %d\n", imx179_ctx->wdr_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    awb_sns_dft->init_rgain   = g_imx179_wb_gain[dev][0];
    awb_sns_dft->init_ggain   = g_imx179_wb_gain[dev][1];
    awb_sns_dft->init_bgain   = g_imx179_wb_gain[dev][2];
    awb_sns_dft->sample_rgain = g_imx179_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_imx179_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx179_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case IMX179_8M_10BIT_LINEAR_MODE:
            *max_fps = 30;
            break;
        case IMX179_1M_10BIT_LINEAR_MODE:
            *max_fps = 120;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx179_standby(xmedia_u32 dev)
{
    imx179_write_reg(dev, 0x0100, 0x00); // STANDBY

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx179_resume(xmedia_u32 dev)
{
    imx179_write_reg(dev, 0x0100, 0x01); // standby

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *imx179_ctx   = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;

    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < imx179_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < imx179_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= imx179_write_reg(dev, imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx179_start(xmedia_u32 dev)
{
    xmedia_s32     ret;
    sensor_context *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    if (imx179_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check param!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (imx179_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = imx179_init_image(dev, imx179_ctx->img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    imx179_delay_ms(10);
    ret = imx179_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = imx179_set_mirror_flip(dev, imx179_ctx->mirror_en, imx179_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx179_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = imx179_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_imx179_dev_map[index] == SENSOR_DEV_INVALID) {
            g_imx179_dev_map[index] = pipe;
            *dev                    = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are registered!\n");
    return XMEDIA_FAILURE;
}

static xmedia_s32 imx179_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_imx179_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

#define IMX179_MARGIN 40
static xmedia_s32 imx179_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{   // TO-DO：min fps is to be confirmed.
    switch (img_mode) {
        case IMX179_8M_10BIT_LINEAR_MODE:
        case IMX179_1M_10BIT_LINEAR_MODE:
            *min_fps = 0.8;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case IMX179_8M_10BIT_LINEAR_MODE:
            *vmax = IMX179_VMAX_8M_LINEAR;
            break;

        case IMX179_1M_10BIT_LINEAR_MODE:
            *vmax = IMX179_VMAX_640_480_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32    i = 0, flag = 0;
    sensor_context* imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    for (i = 0; i < imx179_ctx->regs_info[0].reg_num; i++) {
        if (imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            imx179_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
            flag = 1;
        }
    }

    if (flag) {
        imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_GROUP_HOLD_START].update = XMEDIA_TRUE;
        imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_GROUP_HOLD_END].update   = XMEDIA_TRUE;
    }

    memcpy(sns_regs_info, &imx179_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&imx179_ctx->regs_info[SENSOR_PRE_FRAME], &imx179_ctx->regs_info[SENSOR_CUR_FRAME],
                                                                    sizeof(xmedia_sensor_regs_info));
    imx179_ctx->fl[SENSOR_PRE_FRAME] = imx179_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_s32      ret;
    xmedia_float    max_fps;
    xmedia_float    min_fps;
    xmedia_u32      vmax;
    sensor_context* imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    ret = imx179_get_max_fps(imx179_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = imx179_get_min_fps(imx179_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = imx179_get_vmax(imx179_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support fps: %f, range of fps is [%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    full_lines  = (full_lines > IMX179_FULL_LINES_MAX) ? IMX179_FULL_LINES_MAX : full_lines;

    imx179_ctx->fl_std               = full_lines;
    imx179_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    imx179_ctx->fps                  = fps;
    ae_sns_dft->full_lines           = full_lines;
    ae_sns_dft->full_lines_std       = full_lines;
    ae_sns_dft->fps                  = fps;
    ae_sns_dft->max_int_time         = full_lines - IMX179_EXP_OFFSET_LINEAR;

    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_VMAX_L].data =  SENSOR_LOW_8BITS(full_lines);
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_VMAX_H].data =  SENSOR_HIGH_8BITS(full_lines);

    SENSOR_PRINT("dev[%d]- imx179 set fps: %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                                        xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    xmedia_u32      vmax;
    xmedia_u32      vmax_max_fps;
    xmedia_float    max_fps;
    sensor_context *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    if (imx179_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", imx179_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = imx179_get_max_fps(imx179_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = imx179_get_vmax(imx179_ctx->img_mode, &vmax_max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    vmax  = full_lines;
    vmax  = (vmax > IMX179_FULL_LINES_MAX) ? IMX179_FULL_LINES_MAX : vmax;

    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_VMAX_H].data  = SENSOR_LOW_8BITS(vmax);
    imx179_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[IMX179_REG_VMAX_L].data  = SENSOR_HIGH_8BITS(vmax);

    imx179_ctx->fl[SENSOR_CUR_FRAME] = vmax;
    imx179_ctx->fps                  = max_fps * vmax_max_fps / vmax;

    ae_sns_dft->full_lines   = imx179_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = imx179_ctx->fl[SENSOR_CUR_FRAME] - 8;

    SENSOR_PRINT("dev[%d]- imx179 set fps: %f\n", dev, imx179_ctx->fps);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    xmedia_u32    short_max0   = 0;
    xmedia_u32    short_max    = 0;
    xmedia_u32    short_limit  = 2;
    sensor_context* imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    if (imx179_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_2TO1_LINE) {
        short_max0 = ((imx179_ctx->fl[1] - IMX179_MARGIN  - imx179_ctx->wdr_int_time[0]) * 0x40) /
                                                        SENSOR_DIV_0_TO_1(inttime_attr->ratio[0]);
        short_max = ((imx179_ctx->fl[0] - IMX179_MARGIN) * 0x40) / (inttime_attr->ratio[0] + 0x40);
        short_max = (short_max0 < short_max) ? short_max0 : short_max;
        short_max = (short_max == 0) ? 1 : short_max;

        if (short_max >= short_limit) {
            inttime_attr->max_inttime[0] = short_max;
            inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * inttime_attr->ratio[0]) >> 6;
            inttime_attr->min_inttime[0] = short_limit;
            inttime_attr->min_inttime[1] = (inttime_attr->min_inttime[0] * inttime_attr->ratio[0]) >> 6;
        } else {
            short_max = short_limit;
            inttime_attr->max_inttime[0] = short_max;
            inttime_attr->max_inttime[1] = (inttime_attr->max_inttime[0] * 0xFFF) >> 6;
            inttime_attr->min_inttime[0] = inttime_attr->max_inttime[0];
            inttime_attr->min_inttime[1] = inttime_attr->max_inttime[1];
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *imx179_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    IMX179_GET_CTX(dev, imx179_ctx);
    SENSOR_CHECK_PTR_RETURN(imx179_ctx);

    ret = imx179_get_min_fps(imx179_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = imx179_get_max_fps(imx179_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = imx179_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 imx179_init_register_func(xmedia_sensor_register_info* info)
{
    SENSOR_CHECK_PTR_RETURN(info);

    memcpy(info->sensor_name, IMX179_NAME, sizeof(IMX179_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property      = imx179_get_property;
    info->isp_func.pfn_sensor_set_work_mode     = imx179_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes    = imx179_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror            = imx179_mirror;
    info->isp_func.pfn_sensor_flip              = imx179_flip;
    info->isp_func.pfn_sensor_set_bus_info      = imx179_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr      = imx179_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param    = imx179_set_init_param;
    info->isp_func.pfn_sensor_start             = imx179_start;
    info->isp_func.pfn_sensor_stop              = imx179_stop;
    info->isp_func.pfn_sensor_standby           = imx179_standby;
    info->isp_func.pfn_sensor_resume            = imx179_resume;
    info->isp_func.pfn_sensor_write_reg         = imx179_write_reg;
    info->isp_func.pfn_sensor_read_reg          = imx179_read_reg;
    info->isp_func.pfn_sensor_init              = imx179_init;
    info->isp_func.pfn_sensor_exit              = imx179_exit;
    info->isp_func.pfn_sensor_set_attr          = imx179_set_attr;

    // called by ISP
    info->isp_func.pfn_sensor_get_isp_default     = imx179_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = imx179_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = imx179_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = imx179_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = imx179_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = imx179_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = imx179_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = imx179_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = imx179_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = imx179_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = imx179_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = imx179_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = imx179_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = imx179_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = imx179_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = imx179_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = imx179_get_awb_default;

    return XMEDIA_SUCCESS;

}

xmedia_s32 imx179_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = imx179_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = imx179_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret = imx179_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "IMX179 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx179_unregister(xmedia_u32 pipe)
{
    xmedia_s32 ret;
    xmedia_u32 dev;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = imx179_match_dev(pipe, &dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_PRINT("IMX179 need not unregister function!\n");
        return XMEDIA_SUCCESS;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "IMX179 unregister function failed!\n");
        return XMEDIA_FAILURE;
    }

    imx179_ctx_exit(dev);
    g_imx179_dev_map[dev] = SENSOR_DEV_INVALID;

    return XMEDIA_SUCCESS;
}
