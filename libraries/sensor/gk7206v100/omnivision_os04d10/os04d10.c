#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "os04d10.h"
#include "os04d10_ctrl.h"
#include "os04d10_ex.h"

#define OS04D10_NAME          "OS04D10"
#define OS04D10_SPECS_MAX_NUM 1
#define OS04D10_BIT_RATE      750

#ifdef __linux__
//#define OS04D10_ISP_DEFAULT_SUPPORT
#endif

#define OS04D10_REG_ADDR_STANDBY_0 0x36
#define OS04D10_REG_ADDR_STANDBY_1 0x20
#define OS04D10_REG_ADDR_STANDBY_2  0x33
#define OS04D10_REG_ADDR_STANDBY_3  0x01

#define OS04D10_RES_IS_1440P(w, h) ((w) == 2560 && (h) == 1440)

#define OS04D10_ERR_MODE_PRINT(sensor_attr)                                                                            \
    do {                                                                                                               \
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Width:%d, Height:%d, WDRMode:%d\n", sensor_attr->width,             \
                     sensor_attr->height, sensor_attr->wdr_mode);                                                      \
    } while (0)

#define OS04D10_CALIBRATE_STATIC_TEMP 4850

// awb static param for Fuji Lens New IR_Cut
#define OS04D10_CALIBRATE_STATIC_WB_R_GAIN  441
#define OS04D10_CALIBRATE_STATIC_WB_GR_GAIN 256
#define OS04D10_CALIBRATE_STATIC_WB_GB_GAIN 256
#define OS04D10_CALIBRATE_STATIC_WB_B_GAIN  498

// Calibration results for Auto WB Planck
#define OS04D10_CALIBRATE_AWB_P1 27
#define OS04D10_CALIBRATE_AWB_P2 257
#define OS04D10_CALIBRATE_AWB_Q1 (-26)
#define OS04D10_CALIBRATE_AWB_A1 181555
#define OS04D10_CALIBRATE_AWB_B1 128
#define OS04D10_CALIBRATE_AWB_C1 (-131126)

// Rgain and Bgain of the golden sample
#define OS04D10_GOLDEN_RGAIN 0
#define OS04D10_GOLDEN_BGAIN 0

#define OS04D10_AGAIN_INDEX_MAX  64
static const xmedia_u32 g_os04d10_again_table[OS04D10_AGAIN_INDEX_MAX] = {
    1024, 1088, 1152, 1216, 1280, 1344, 1408, 1472, 1536, 1600, 1664, 1728, 1792, 1856, 1920, 1984,
    2048, 2176, 2304, 2432, 2560, 2688, 2816, 2944, 3072, 3200, 3328, 3456, 3584, 3712, 3840, 3968,
    4096, 4352, 4608, 4864, 5120, 5376, 5632, 5888, 6144, 6400, 6656, 6912, 7168, 7424, 7680, 7936,
    8192, 8704, 9216, 9728, 10240, 10752, 11264, 11776, 12288, 12800, 13312, 13824, 14336, 14848,
    15360, 15872
};

static sensor_context *g_os04d10_ctx[XMEDIA_SENSOR_DEV_MAX_NUM] = { XMEDIA_NULL };
#define OS04D10_GET_CTX(dev, ctx) ctx = g_os04d10_ctx[dev]
#define OS04D10_SET_CTX(dev, ctx) g_os04d10_ctx[dev] = ctx

static xmedia_s32 g_os04d10_dev_map[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ... XMEDIA_SENSOR_DEV_MAX_NUM - 1] =
                                                                       SENSOR_DEV_INVALID };

static xmedia_u32 g_os04d10_exposure[XMEDIA_SENSOR_DEV_MAX_NUM]      = { 0 };
static xmedia_u16 g_os04d10_sample_r_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_os04d10_sample_b_gain[XMEDIA_SENSOR_DEV_MAX_NUM] = { 0 };
static xmedia_u16 g_os04d10_wb_gain[XMEDIA_SENSOR_DEV_MAX_NUM][SENSOR_AWB_CHN_GIAN_MAX_NUM] = { { 0 } };

static const xmedia_sensor_property g_os04d10_property[OS04D10_SPECS_MAX_NUM] = {
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
        { 0 }, OS04D10_BIT_RATE,
    },
};

static const xmedia_sensor_capability g_os04d10_capability = {
    .max_width  = 2560,
    .max_height = 1440,
    .max_fps    = 30,

    .output_intf = XMEDIA_INTF_TYPE_MIPI_CSI,
    .init_mclk   = XMEDIA_SENSOR_CLOCK_FREQ_24MHZ,
    .wdr_mode    = XMEDIA_VIDEO_WDR_MODE_NONE,
    .wdr_format  = XMEDIA_VIDEO_WDR_FMT_VC,
    .bit_width   = XMEDIA_VIDEO_DATA_WIDTH_10,
    .reset_time  = 200, // TODO: 待后续确认复位所需时间

    .slave_mode_supported = XMEDIA_FALSE,
    .mipi_lanes_supported = XMEDIA_SENSOR_MIPI_LANES_2L,
    .standby_info = {
        .standby_supported = XMEDIA_FALSE,
        .addr_byte_num     = OS04D10_ADDR_BYTE,
        .data_byte_num     = OS04D10_DATA_BYTE,
        .standby_reg_num   = 3,//todo should same as standby sensor api
        .standby_reg_addr  = { OS04D10_REG_ADDR_PAGE_SELECT, OS04D10_REG_ADDR_STANDBY_0, XMEDIA_SENSOR_DELAY_FLAG,
                               OS04D10_REG_ADDR_PAGE_SELECT, OS04D10_REG_ADDR_STANDBY_2, OS04D10_REG_ADDR_STANDBY_3,
                               OS04D10_REG_ADDR_PAGE_SELECT, OS04D10_REG_ADDR_STANDBY_1 },
        .standby_reg_data  = { 0x00, 0x07, 0x01 },
        .resume_reg_num    = 8,
        .resume_reg_addr   = { OS04D10_REG_ADDR_PAGE_SELECT, OS04D10_REG_ADDR_STANDBY_0, XMEDIA_SENSOR_DELAY_FLAG,
                               OS04D10_REG_ADDR_PAGE_SELECT, OS04D10_REG_ADDR_STANDBY_2, OS04D10_REG_ADDR_STANDBY_3,
                               OS04D10_REG_ADDR_PAGE_SELECT, OS04D10_REG_ADDR_STANDBY_1 },
        .resume_reg_data   = { 0x00, 0x00, 0x5, 0x01, 0x03, 0x02, 0x00, 0x03 },
    },
};

static xmedia_s32 os04d10_ctx_init(xmedia_u32 dev)
{
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    OS04D10_GET_CTX(dev, os04d10_ctx);
    if (os04d10_ctx != XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx already malloc!\n", dev);
        return XMEDIA_ERRCODE_EXIST;
    }

    os04d10_ctx = (sensor_context *)malloc(sizeof(sensor_context));
    if (os04d10_ctx == XMEDIA_NULL) {
        SENSOR_TRACE(MODULE_DBG_ERR, " sensor-[%d] ctx malloc memory failed!\n", dev);
        return XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY;
    }

    memset(os04d10_ctx, 0, sizeof(sensor_context));
    os04d10_ctx->i2c_addr             = OS04D10_I2C_ADDR;
    os04d10_ctx->lanes                = XMEDIA_SENSOR_MIPI_LANES_2L;
    os04d10_ctx->work_mode            = XMEDIA_SENSOR_WORK_MODE_NORMAL;
    os04d10_ctx->bus_info.i2c_dev     = SENSOR_I2C_INVALID;
    os04d10_ctx->size.width           = g_os04d10_property[0].width;
    os04d10_ctx->size.height          = g_os04d10_property[0].height;
    os04d10_ctx->fps                  = g_os04d10_property[0].max_fps;
    os04d10_ctx->wdr_mode             = g_os04d10_property[0].wdr_mode;
    os04d10_ctx->img_mode             = OS04D10_4M_30FPS_10BIT_LINEAR_MODE;
    os04d10_ctx->fl_std               = OS04D10_2L_VMAX_4M30_LINEAR;
    os04d10_ctx->fl[SENSOR_CUR_FRAME] = OS04D10_2L_VMAX_4M30_LINEAR;
    os04d10_ctx->fl[SENSOR_PRE_FRAME] = OS04D10_2L_VMAX_4M30_LINEAR;
    OS04D10_SET_CTX(dev, os04d10_ctx);
    return XMEDIA_SUCCESS;
}

static xmedia_void os04d10_ctx_exit(xmedia_u32 dev)
{
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_FREE(os04d10_ctx);
    OS04D10_SET_CTX(dev, XMEDIA_NULL);

    return;
}

static xmedia_s32 os04d10_get_property(xmedia_u32 dev, xmedia_sensor_property *property)
{
    xmedia_u8       i;
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(property);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    for (i = 0; i < OS04D10_SPECS_MAX_NUM; i++) {
        if (g_os04d10_property[i].width == os04d10_ctx->size.width &&
            g_os04d10_property[i].height == os04d10_ctx->size.height &&
            g_os04d10_property[i].wdr_mode == os04d10_ctx->wdr_mode) {
            memcpy(property, &g_os04d10_property[i], sizeof(xmedia_sensor_property));
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "Not support! Please check input param!\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 os04d10_set_work_mode(xmedia_u32 dev, xmedia_sensor_work_mode work_mode)
{
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor work mode param invalid!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os04d10_ctx->work_mode = work_mode;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_set_mipi_lanes(xmedia_u32 dev, xmedia_sensor_mipi_lanes mipi_lanes)
{
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    if (mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Invalid mipi lanes [%d]! \n", mipi_lanes);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os04d10_ctx->lanes = mipi_lanes;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_set_bus_info(xmedia_u32 dev, const xmedia_sensor_commbus *sns_bus_info)
{
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_bus_info);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_I2C) {
        os04d10_ctx->bus_info.i2c_dev = sns_bus_info->i2c_dev;
    } else if (sns_bus_info->type == XMEDIA_SENSOR_BUS_TYPE_SPI) {
        os04d10_ctx->bus_info.spi_info.spi_dev = sns_bus_info->spi_info.spi_dev;
        os04d10_ctx->bus_info.spi_info.spi_cs  = sns_bus_info->spi_info.spi_cs;
    } else {
        SENSOR_TRACE(MODULE_DBG_ERR,
                     "Not support [%d], the value of bus type is {%d-i2c, %d-spi}, please check input type!\n",
                     sns_bus_info->type, XMEDIA_SENSOR_BUS_TYPE_I2C, XMEDIA_SENSOR_BUS_TYPE_SPI);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

// one i2c dev shared by multiple sensors
static xmedia_s32 os04d10_set_dev_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32      ret;
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    os04d10_ctx->i2c_addr = slave_addr;
    ret = os04d10_set_slave_addr(dev, slave_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_get_dev_addr(xmedia_u32 dev, xmedia_u32 *slave_addr)
{
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(slave_addr);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    *slave_addr = os04d10_ctx->i2c_addr;
    return XMEDIA_SUCCESS;
}

// called by ISP
static xmedia_s32 os04d10_init(xmedia_u32 dev, xmedia_sensor_init_mode init_mode)
{
    xmedia_s32      ret;
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    os04d10_ctx->init_mode = init_mode;
    ret = os04d10_i2c_init(dev, os04d10_ctx->bus_info.i2c_dev, os04d10_ctx->i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    os04d10_ctx->init = XMEDIA_TRUE;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_exit(xmedia_u32 dev)
{
    xmedia_s32      ret;
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    ret = os04d10_i2c_exit(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    os04d10_ctx->init = XMEDIA_FALSE;
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_mirror(xmedia_u32 dev, xmedia_bool mirror_en)
{
    xmedia_s32      ret;
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    os04d10_ctx->mirror_en = mirror_en;
    ret = os04d10_set_mirror_flip(dev, mirror_en, os04d10_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_flip(xmedia_u32 dev, xmedia_bool flip_en)
{
    xmedia_s32      ret;
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    os04d10_ctx->flip_en = flip_en;
    ret = os04d10_set_mirror_flip(dev, os04d10_ctx->mirror_en, flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_set_image_mode(sensor_context *os04d10_ctx, xmedia_u8 *image_mode,
                                        const xmedia_sensor_attr *sns_attr)
{
    SENSOR_CHECK_PTR_RETURN(image_mode);

    if (os04d10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        if (OS04D10_RES_IS_1440P(sns_attr->width, sns_attr->height)) {
            *image_mode         = OS04D10_4M_30FPS_10BIT_LINEAR_MODE;
            os04d10_ctx->fl_std = OS04D10_2L_VMAX_4M30_LINEAR ;
        } else {
            OS04D10_ERR_MODE_PRINT(sns_attr);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
        }
    } else {
        OS04D10_ERR_MODE_PRINT(sns_attr);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os04d10_ctx->size.width  = sns_attr->width;
    os04d10_ctx->size.height = sns_attr->height;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_set_wdr_mode(xmedia_u32 dev, xmedia_u8 wdr_mode)
{
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    switch (wdr_mode) {
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            os04d10_ctx->wdr_mode = XMEDIA_VIDEO_WDR_MODE_NONE;
            SENSOR_PRINT("linear mode\n");
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "NOT support this mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    memset(os04d10_ctx->wdr_int_time, 0, sizeof(os04d10_ctx->wdr_int_time));

    return XMEDIA_SUCCESS;
}

#define OS04D10_LINEAR_REG_INFO_MAX_NUM   OS04D10_REG_L_MAX_NUM
static xmedia_void os04d10_init_common_reg_info(sensor_context *os04d10_ctx)
{
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_EXP_H].delay_frame_num = 2;
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_EXP_H].reg_addr        = OS04D10_REG_ADDR_EXP_H;
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_EXP_L].delay_frame_num = 2;
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_EXP_L].reg_addr        = OS04D10_REG_ADDR_EXP_L;

    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_AGAIN].delay_frame_num = 2;
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_AGAIN].reg_addr        = OS04D10_REG_ADDR_AGAIN;
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_DGAIN].delay_frame_num = 2;
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_DGAIN].reg_addr        = OS04D10_REG_ADDR_DGAIN;

    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_VBLANK_H].delay_frame_num = 2;
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_VBLANK_H].reg_addr        = OS04D10_REG_ADDR_VBLANK_H;
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_VBLANK_L].delay_frame_num = 2;
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_VBLANK_L].reg_addr        = OS04D10_REG_ADDR_VBLANK_L;

    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_TRIGGER].delay_frame_num = 2;
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_TRIGGER].reg_addr        = OS04D10_REG_ADDR_TRIGGER;

    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_PAGE_SELECT].delay_frame_num = 2;
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_PAGE_SELECT].reg_addr = OS04D10_REG_ADDR_PAGE_SELECT;

    return;
}

static xmedia_s32 os04d10_init_reg_info(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].com_bus.i2c_dev        = os04d10_ctx->bus_info.i2c_dev;
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].cfg_to_valid_delay_max = 2; // maximum delay valid frames
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num                = OS04D10_LINEAR_REG_INFO_MAX_NUM;

    for (i = 0; i < os04d10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update        = XMEDIA_TRUE;
        os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].dev_addr      = os04d10_ctx->i2c_addr;
        os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].addr_byte_num = OS04D10_ADDR_BYTE;
        os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data_byte_num = OS04D10_DATA_BYTE;
    }

    // init general register - The registers should init both in linear and 2to1 wdr mode
    os04d10_init_common_reg_info(os04d10_ctx);

    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_TRIGGER].data     = 0x01;
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_PAGE_SELECT].data = 0x01;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_set_attr(xmedia_u32 dev, const xmedia_sensor_attr *sns_attr)
{
    xmedia_s32      ret;
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_attr);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    // Set wdr mode
    ret = os04d10_set_wdr_mode(dev, sns_attr->wdr_mode);
    SENSOR_CHECK_RET_RETURN(ret);

    // Set image mode
    ret = os04d10_set_image_mode(os04d10_ctx, &os04d10_ctx->img_mode, sns_attr);
    SENSOR_CHECK_RET_RETURN(ret);

    os04d10_ctx->fl[SENSOR_CUR_FRAME] = os04d10_ctx->fl_std;
    os04d10_ctx->fl[SENSOR_PRE_FRAME] = os04d10_ctx->fl[SENSOR_CUR_FRAME];

    ret = os04d10_init_reg_info(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_get_isp_default(xmedia_u32 dev, xmedia_sensor_isp_default *isp_default)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(isp_default);

    memset(isp_default, 0, sizeof(xmedia_sensor_isp_default));
#ifdef OS04D10_ISP_DEFAULT_SUPPORT
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
    SENSOR_TRACE(MODULE_DBG_WARN, "Not support isp getting default param from os04d10!\n ");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
#endif
}

static xmedia_s32 os04d10_get_isp_black_level(xmedia_u32 dev, xmedia_sensor_black_level *black_level)
{
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(black_level);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    // Don't need to update black level when iso change
    black_level->update = XMEDIA_FALSE;

    if (os04d10_ctx->wdr_mode == XMEDIA_VIDEO_WDR_MODE_NONE) {
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_R]  = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GR] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_GB] = 0x100;
        black_level->black_level[XMEDIA_SENSOR_BAYER_CHN_B]  = 0x100;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_get_bayer_pattern(xmedia_u32 dev, xmedia_video_bayer_format *bayer_pattern)
{
    xmedia_u8                      i;
    xmedia_sensor_mirror_flip_type type;
    sensor_context                *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(bayer_pattern);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    if (os04d10_ctx->mirror_en && os04d10_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR_FLIP;
    } else if (os04d10_ctx->mirror_en && (!os04d10_ctx->flip_en)) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_MIRROR;
    } else if ((!os04d10_ctx->mirror_en) && os04d10_ctx->flip_en) {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_FLIP;
    } else {
        type = XMEDIA_SENSOR_MIRROR_FLIP_TYPE_NORMAL;
    }

    for (i = 0; i < OS04D10_SPECS_MAX_NUM; i++) {
        if (g_os04d10_property[i].width == os04d10_ctx->size.width &&
            g_os04d10_property[i].height == os04d10_ctx->size.height &&
            g_os04d10_property[i].wdr_mode == os04d10_ctx->wdr_mode) {
            *bayer_pattern = g_os04d10_property[i].bayer_format[type];
            break;
        }
    }

    if (i >= OS04D10_SPECS_MAX_NUM) {
        *bayer_pattern = XMEDIA_VIDEO_BAYER_MAX;
        SENSOR_TRACE(MODULE_DBG_ERR, "ERR! Get bayer pattern failed!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_get_capability(xmedia_sensor_capability *capability)
{
    SENSOR_CHECK_PTR_RETURN(capability);
    memcpy(capability, &g_os04d10_capability, sizeof(xmedia_sensor_capability));
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_set_ae_wdr_attr(xmedia_u32 dev, xmedia_isp_ae_wdr_mode ae_wdr_mode)
{
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    if (ae_wdr_mode >= XMEDIA_ISP_AE_WDR_MODE_MAX) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not supported AE wdr mode!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    os04d10_ctx->ae_wdr_mode = ae_wdr_mode;

    return XMEDIA_SUCCESS;
}

// 用户期望第一帧生效的参数，且AE/AWB头几帧计算的基础
static xmedia_s32 os04d10_set_init_param(xmedia_u32 dev, const xmedia_sensor_init_param *init_param)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(init_param);

    g_os04d10_exposure[dev]      = init_param->exposure;
    g_os04d10_sample_r_gain[dev] = init_param->sample_rgain;
    g_os04d10_sample_b_gain[dev] = init_param->sample_bgain;

    g_os04d10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R] = init_param->wb_rgain;
    g_os04d10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G] = init_param->wb_ggain;
    g_os04d10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B] = init_param->wb_bgain;

    return XMEDIA_SUCCESS;
}

xmedia_s32 os04d10_get_max_fps(xmedia_u8 img_mode, xmedia_float *max_fps)
{
    switch (img_mode) {
        case OS04D10_4M_30FPS_10BIT_LINEAR_MODE:
            *max_fps = 30;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_get_ae_common_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                               xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32   ret;
    xmedia_float max_fps;

    ret = os04d10_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);

    ae_sns_dft->full_lines_std = sns_ctx->fl_std;
    ae_sns_dft->flicker_freq   =  0;

    ae_sns_dft->int_time_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->int_time_accu.accuracy  = 1;
    ae_sns_dft->int_time_accu.offset    = 0;

    ae_sns_dft->again_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_TABLE;
    ae_sns_dft->again_accu.accuracy = 0.0625; // accuracy: 0.0625 - 1/16

    ae_sns_dft->dgain_accu.accu_type = XMEDIA_ISP_AE_ACCURACY_TYPE_LINEAR;
    ae_sns_dft->dgain_accu.accuracy =  0.125;

    ae_sns_dft->ispdgain_shift      = 8;
    ae_sns_dft->min_ispdgain_target = 1 << ae_sns_dft->ispdgain_shift;
    ae_sns_dft->max_ispdgain_target = 8 << ae_sns_dft->ispdgain_shift;

    ae_sns_dft->max_iris_f_no = XMEDIA_ISP_IRIS_F_NO_1_0;
    ae_sns_dft->min_iris_f_no = XMEDIA_ISP_IRIS_F_NO_32_0;

    ae_sns_dft->ae_route_ex_valid           = XMEDIA_FALSE;
    ae_sns_dft->ae_route_attr.total_num     = 0;
    ae_sns_dft->ae_ext_route_attr.total_num = 0;

    // lines per second: VTS * max_fps
    ae_sns_dft->lines_per_500ms  = (sns_ctx->fl_std * max_fps) / 2;
    ae_sns_dft->man_ratio_enable = XMEDIA_FALSE;

    return XMEDIA_SUCCESS;
}

static xmedia_void os04d10_get_ae_linear_default(xmedia_u32 dev, sensor_context *sns_ctx,
                                                xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    ae_sns_dft->max_again = 15872; // max 992
    ae_sns_dft->min_again = 1024;  // min 16
    ae_sns_dft->max_again_target = ae_sns_dft->max_again;
    ae_sns_dft->min_again_target = ae_sns_dft->min_again;

    ae_sns_dft->max_dgain = 8; // max 1024
    ae_sns_dft->min_dgain = 8; // min 1024
    ae_sns_dft->max_dgain_target = ae_sns_dft->max_dgain;
    ae_sns_dft->min_dgain_target = ae_sns_dft->min_dgain;

    ae_sns_dft->ae_compensation = 0x38;
    ae_sns_dft->ae_exp_mode = XMEDIA_ISP_AE_STRATEGY_EXP_HIGHLIGHT_PRIOR;
    ae_sns_dft->init_exposure = g_os04d10_exposure[dev] ? g_os04d10_exposure[dev] : 130000; // init 130000

    ae_sns_dft->max_int_time = sns_ctx->fl_std - OS04D10_EXP_OFFSET;
    ae_sns_dft->min_int_time = 1; // min int 1
    ae_sns_dft->max_int_time_target = 65535; // max int 65535
    ae_sns_dft->min_int_time_target = ae_sns_dft->min_int_time;

    return;
}

static xmedia_s32 os04d10_get_ae_default(xmedia_u32 dev, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_s32      ret;
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    memset(&ae_sns_dft->ae_route_attr, 0, sizeof(xmedia_isp_ae_route));

    ret = os04d10_get_ae_common_default(dev, os04d10_ctx, ae_sns_dft);
    SENSOR_CHECK_RET_RETURN(ret);

    switch (os04d10_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            os04d10_get_ae_linear_default(dev, os04d10_ctx, ae_sns_dft);
            break;

        case XMEDIA_VIDEO_WDR_MODE_2TO1_LINE:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support wdr mode\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
            break;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_update_inttime(xmedia_u32 dev, xmedia_u32 int_time)
{
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    int_time = SENSOR_MAX(int_time, 2);

    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_EXP_H].data = SENSOR_HIGH_8BITS(int_time);
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_EXP_L].data = SENSOR_LOW_8BITS(int_time);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_calc_again(xmedia_u32 dev, xmedia_u32 *again_lin, xmedia_u32 *again_db)
{
    xmedia_s32 i;

    SENSOR_CHECK_DEV_RETURN(dev);

    if (*again_lin >= g_os04d10_again_table[OS04D10_AGAIN_INDEX_MAX - 1]) {
        *again_lin = g_os04d10_again_table[OS04D10_AGAIN_INDEX_MAX - 1];
        *again_db = OS04D10_AGAIN_INDEX_MAX - 1;
        return XMEDIA_SUCCESS;
    }

    for (i = 1; i < OS04D10_AGAIN_INDEX_MAX; i++) {
        if (*again_lin < g_os04d10_again_table[i]) {
            *again_lin = g_os04d10_again_table[i - 1];
            *again_db = i - 1;
            break;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_calc_dgain(xmedia_u32 dev, xmedia_u32 *dgain_lin, xmedia_u32 *dgain_db)
{
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_update_gains(xmedia_u32 dev, xmedia_u32 again, xmedia_u32 dgain)
{
    sensor_context *os04d10_ctx = XMEDIA_NULL;
    xmedia_u32 again_reg = 0x00;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    again_reg = (xmedia_u32)(g_os04d10_again_table[again] / 1024.0 * 16.0);

    os04d10_ctx->regs_info[0].i2c_data[OS04D10_REG_AGAIN].data = again_reg;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_get_max_inttime(xmedia_u32 dev, xmedia_sensor_ae_inttime_attr *inttime_attr)
{
    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(inttime_attr);

    return XMEDIA_SUCCESS;
}

// AWB
static xmedia_s32 os04d10_get_awb_default(xmedia_u32 dev, xmedia_isp_awb_sensor_default *awb_sns_dft)
{
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(awb_sns_dft);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    memset(awb_sns_dft, 0, sizeof(xmedia_isp_awb_sensor_default));
    awb_sns_dft->wb_ref_temp = OS04D10_CALIBRATE_STATIC_TEMP;

    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_R]  = OS04D10_CALIBRATE_STATIC_WB_R_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GR] = OS04D10_CALIBRATE_STATIC_WB_GR_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_GB] = OS04D10_CALIBRATE_STATIC_WB_GB_GAIN;
    awb_sns_dft->gain_offset[XMEDIA_SENSOR_BAYER_CHN_B]  = OS04D10_CALIBRATE_STATIC_WB_B_GAIN;

    // 0 ~ 5: point index on the awb curve param
    awb_sns_dft->wb_para[0] = OS04D10_CALIBRATE_AWB_P1;
    awb_sns_dft->wb_para[1] = OS04D10_CALIBRATE_AWB_P2;
    awb_sns_dft->wb_para[2] = OS04D10_CALIBRATE_AWB_Q1;
    awb_sns_dft->wb_para[3] = OS04D10_CALIBRATE_AWB_A1;
    awb_sns_dft->wb_para[4] = OS04D10_CALIBRATE_AWB_B1;
    awb_sns_dft->wb_para[5] = OS04D10_CALIBRATE_AWB_C1;

    awb_sns_dft->golden_rgain = OS04D10_GOLDEN_RGAIN;
    awb_sns_dft->golden_bgain = OS04D10_GOLDEN_BGAIN;
    awb_sns_dft->awb_runinterval = 2;
    switch (os04d10_ctx->wdr_mode) {
        default:
        case XMEDIA_VIDEO_WDR_MODE_NONE:
            memcpy(&awb_sns_dft->ccm, &g_os04d10_awb_ccm, sizeof(xmedia_isp_awb_ccm));
            memcpy(&awb_sns_dft->agc_tbl, &g_os04d10_awb_agc_table, sizeof(xmedia_isp_awb_agc_table));
            break;
    }

    awb_sns_dft->init_rgain   = g_os04d10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_R];
    awb_sns_dft->init_ggain   = g_os04d10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_G];
    awb_sns_dft->init_bgain   = g_os04d10_wb_gain[dev][SENSOR_AWB_CHN_GIAN_B];
    awb_sns_dft->sample_rgain = g_os04d10_sample_r_gain[dev];
    awb_sns_dft->sample_bgain = g_os04d10_sample_b_gain[dev];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_standby(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);

    ret |= os04d10_write_reg(dev, 0xfd, 0x00);
    ret |= os04d10_write_reg(dev, 0x36, 0x07);
    ret |= os04d10_write_reg(dev, 0x20, 0x01);

    return ret;
}

static xmedia_s32 os04d10_resume(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    SENSOR_CHECK_DEV_RETURN(dev);

    ret |= os04d10_write_reg(dev, 0xfd, 0x00);
    ret |= os04d10_write_reg(dev, 0x36, 0x00);
    ret |= os04d10_write_reg(dev, 0x20, 0x03);

    return ret;
}

static xmedia_s32 os04d10_config_init_param(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *os04d10_ctx  = XMEDIA_NULL;
    xmedia_s32      ret          = XMEDIA_SUCCESS;
    xmedia_u32      reg_data_sum = 0;
    return XMEDIA_SUCCESS;

    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    // exceptional process for AE unregister
    for (i = 0; i < os04d10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        reg_data_sum += os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data;
    }

    if (reg_data_sum == 0) {
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < os04d10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        ret |= os04d10_write_reg(dev, os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
        SENSOR_PRINT("reg_addr: 0x%x, reg_data: 0x%x\n", os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
            os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] config init param failed!\n", dev);
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_start(xmedia_u32 dev)
{
    xmedia_s32               ret;
    xmedia_u8                img_mode;
    xmedia_sensor_mipi_lanes mipi_lanes;
    xmedia_sensor_work_mode  work_mode;
    sensor_context          *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    work_mode  = os04d10_ctx->work_mode;
    mipi_lanes = os04d10_ctx->lanes;
    img_mode   = os04d10_ctx->img_mode;

    if (work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL || mipi_lanes != XMEDIA_SENSOR_MIPI_LANES_2L) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check param!\n");
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (os04d10_ctx->init_mode == XMEDIA_SENSOR_INIT_MODE_NORMAL) {
        ret = os04d10_init_image(dev, img_mode);
        SENSOR_CHECK_RET_RETURN(ret);
    }

    ret = os04d10_config_init_param(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = os04d10_set_mirror_flip(dev, os04d10_ctx->mirror_en, os04d10_ctx->flip_en);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_stop(xmedia_u32 dev)
{
    xmedia_s32 ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    ret = os04d10_standby(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_search_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_os04d10_dev_map[index] == SENSOR_DEV_INVALID) {
            g_os04d10_dev_map[index] = pipe;
            *dev                     = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "all sensor dev are binded to pipe!\n");
    return XMEDIA_ERRCODE_BINDED;
}

static xmedia_s32 os04d10_match_dev(xmedia_u32 pipe, xmedia_u32 *dev)
{
    xmedia_u32 index;

    SENSOR_CHECK_PTR_RETURN(dev);

    for (index = 0; index < XMEDIA_SENSOR_DEV_MAX_NUM; index++) {
        if (g_os04d10_dev_map[index] == pipe) {
            *dev = index;
            return XMEDIA_SUCCESS;
        }
    }

    SENSOR_TRACE(MODULE_DBG_ERR, "There is no sensor dev to match pipe[%d]!\n", pipe);
    return XMEDIA_ERRCODE_NOT_BIND;
}

static xmedia_s32 os04d10_get_min_fps(xmedia_u8 img_mode, xmedia_float *min_fps)
{ // TO-DO：min fps is to be confirmed.
    switch (img_mode) {
        case OS04D10_4M_30FPS_10BIT_LINEAR_MODE:
            *min_fps = 0.68;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_get_vmax(xmedia_u8 img_mode, xmedia_u32 *vmax)
{
    switch (img_mode) {
        case OS04D10_4M_30FPS_10BIT_LINEAR_MODE:
            *vmax = OS04D10_2L_VMAX_4M30_LINEAR;
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support img mode: %d\n", img_mode);
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_calc_fps(xmedia_float fps, sensor_context *sns_ctx, xmedia_u32 *full_lines)
{
    xmedia_s32   ret;
    xmedia_float max_fps;
    xmedia_float min_fps;
    xmedia_u32   vmax;

    ret = os04d10_get_max_fps(sns_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04d10_get_min_fps(sns_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04d10_get_vmax(sns_ctx->img_mode, &vmax);
    SENSOR_CHECK_RET_RETURN(ret);

    if ((fps > max_fps) || (fps < min_fps)) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Not support Fps: %f, range of fps is [%f, %f]\n", fps, min_fps, max_fps);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    *full_lines = vmax * max_fps / SENSOR_DIV_0_TO_1_FLOAT(fps);
    *full_lines = SENSOR_MIN(*full_lines, OS04D10_FULL_LINES_MAX);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_get_reg_info(xmedia_u32 dev, xmedia_sensor_regs_info *sns_regs_info)
{
    xmedia_s32      i;
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(sns_regs_info);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    for (i = 0; i < os04d10_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        if (os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data ==
            os04d10_ctx->regs_info[SENSOR_PRE_FRAME].i2c_data[i].data) {
            os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_FALSE;
        } else {
            os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].update = XMEDIA_TRUE;
        }
    }

    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_PAGE_SELECT].update = XMEDIA_TRUE;
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_TRIGGER].update     = XMEDIA_TRUE;

    memcpy(sns_regs_info, &os04d10_ctx->regs_info[SENSOR_CUR_FRAME], sizeof(xmedia_sensor_regs_info));
    memcpy(&os04d10_ctx->regs_info[SENSOR_PRE_FRAME], &os04d10_ctx->regs_info[SENSOR_CUR_FRAME],
           sizeof(xmedia_sensor_regs_info));

    os04d10_ctx->fl[SENSOR_PRE_FRAME] = os04d10_ctx->fl[SENSOR_CUR_FRAME];

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_set_fps(xmedia_u32 dev, xmedia_float fps, xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    xmedia_u32      full_lines;
    xmedia_u32      vblank;
    xmedia_s32      ret;
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    ret = os04d10_calc_fps(fps, os04d10_ctx, &full_lines);
    SENSOR_CHECK_RET_RETURN(ret);

    vblank = full_lines - OS04D10_2L_VMAX_4M30_LINEAR + 297;//182(27Mhz); /* +1->29.998fps */

    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_VBLANK_H].data = SENSOR_HIGH_8BITS(vblank);
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_VBLANK_L].data = SENSOR_LOW_8BITS(vblank);

    os04d10_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    os04d10_ctx->fl_std               = os04d10_ctx->fl[SENSOR_CUR_FRAME];
    os04d10_ctx->fps                  = fps;
    ae_sns_dft->fps                   = fps;
    ae_sns_dft->full_lines_std        = os04d10_ctx->fl_std;
    ae_sns_dft->full_lines            = os04d10_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time          = os04d10_ctx->fl[SENSOR_CUR_FRAME] - OS04D10_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]- os04d10 set fps: %f\n", dev, fps);

    return XMEDIA_SUCCESS;
}

/*
 * 线性：full_lines, AE期望的曝光时间, VMAX = full_lines + EXP_OFFSET_LINEAR
 * 2TO1 WDR: full_lines, AE期望的长帧+短帧的曝光时间, VMAX = full_lines + EXP_OFFSET_WDR
 */
static xmedia_s32 os04d10_set_slow_framerate(xmedia_u32 dev, xmedia_u32 full_lines,
                                            xmedia_isp_ae_sensor_default *ae_sns_dft)
{
    sensor_context *os04d10_ctx = XMEDIA_NULL;
    xmedia_u32      vblank;
    xmedia_float    min_fps, max_fps;
    xmedia_u32      min_fps_vamx, max_fps_vmax;
    xmedia_s32      ret;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(ae_sns_dft);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    if (os04d10_ctx->work_mode != XMEDIA_SENSOR_WORK_MODE_NORMAL) {
        SENSOR_TRACE(MODULE_DBG_ERR, "work mode[%d] param invalid!\n", os04d10_ctx->work_mode);
        return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    ret = os04d10_get_min_fps(os04d10_ctx->img_mode, &min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04d10_get_max_fps(os04d10_ctx->img_mode, &max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04d10_get_vmax(os04d10_ctx->img_mode, &max_fps_vmax);
    SENSOR_CHECK_RET_RETURN(ret);
    min_fps_vamx = max_fps * max_fps_vmax / SENSOR_DIV_0_TO_1_FLOAT(min_fps);

    full_lines = SENSOR_MIN(full_lines, OS04D10_FULL_LINES_MAX);
    full_lines = SENSOR_MIN(full_lines, min_fps_vamx);

    vblank = full_lines - OS04D10_2L_VMAX_4M30_LINEAR;
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_VBLANK_H].data = SENSOR_HIGH_8BITS(vblank);
    os04d10_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[OS04D10_REG_VBLANK_L].data = SENSOR_LOW_8BITS(vblank);

    os04d10_ctx->fl[SENSOR_CUR_FRAME] = full_lines;
    os04d10_ctx->fps                  = min_fps * min_fps_vamx / full_lines;

    ae_sns_dft->full_lines   = os04d10_ctx->fl[SENSOR_CUR_FRAME];
    ae_sns_dft->max_int_time = os04d10_ctx->fl[SENSOR_CUR_FRAME] - OS04D10_EXP_OFFSET;

    SENSOR_PRINT("dev[%d]- os04d10 set fps: %f\n", dev, os04d10_ctx->fps);

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_get_fps(xmedia_u32 dev, xmedia_float *fps, xmedia_float *min_fps, xmedia_float *max_fps)
{
    xmedia_s32 ret;
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_DEV_RETURN(dev);
    SENSOR_CHECK_PTR_RETURN(fps);
    SENSOR_CHECK_PTR_RETURN(min_fps);
    SENSOR_CHECK_PTR_RETURN(max_fps);
    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    ret = os04d10_get_min_fps(os04d10_ctx->img_mode, min_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    ret = os04d10_get_max_fps(os04d10_ctx->img_mode, max_fps);
    SENSOR_CHECK_RET_RETURN(ret);
    *fps = os04d10_ctx->fps;

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04d10_init_register_func(xmedia_sensor_register_info *info)
{
    SENSOR_CHECK_PTR_RETURN(info);
    memcpy(info->sensor_name, OS04D10_NAME, sizeof(OS04D10_NAME));

    // Called by user
    info->isp_func.pfn_sensor_get_property     = os04d10_get_property;
    info->isp_func.pfn_sensor_set_work_mode    = os04d10_set_work_mode;
    info->isp_func.pfn_sensor_set_mipi_lanes   = os04d10_set_mipi_lanes;
    info->isp_func.pfn_sensor_mirror           = os04d10_mirror;
    info->isp_func.pfn_sensor_flip             = os04d10_flip;
    info->isp_func.pfn_sensor_set_bus_info     = os04d10_set_bus_info;
    info->isp_func.pfn_sensor_set_dev_addr     = os04d10_set_dev_addr;
    info->isp_func.pfn_sensor_set_init_param   = os04d10_set_init_param;
    info->isp_func.pfn_sensor_start            = os04d10_start;
    info->isp_func.pfn_sensor_stop             = os04d10_stop;
    info->isp_func.pfn_sensor_standby          = os04d10_standby;
    info->isp_func.pfn_sensor_resume           = os04d10_resume;
    info->isp_func.pfn_sensor_write_reg        = os04d10_write_reg;
    info->isp_func.pfn_sensor_read_reg         = os04d10_read_reg;
    info->isp_func.pfn_sensor_init             = os04d10_init;
    info->isp_func.pfn_sensor_exit             = os04d10_exit;

    // called by ISP
    info->isp_func.pfn_sensor_set_attr            = os04d10_set_attr;
    info->isp_func.pfn_sensor_get_isp_default     = os04d10_get_isp_default;
    info->isp_func.pfn_sensor_get_isp_black_level = os04d10_get_isp_black_level;
    info->isp_func.pfn_sensor_get_reg_info        = os04d10_get_reg_info;
    info->isp_func.pfn_sensor_get_bayer_pattern   = os04d10_get_bayer_pattern;
    info->isp_func.pfn_sensor_get_capability      = os04d10_get_capability;
    info->isp_func.pfn_sensor_get_dev_addr        = os04d10_get_dev_addr;
    info->isp_func.pfn_sensor_get_fps             = os04d10_get_fps;

    // AE
    info->ae_func.pfn_sensor_get_ae_default     = os04d10_get_ae_default;
    info->ae_func.pfn_sensor_set_fps            = os04d10_set_fps;
    info->ae_func.pfn_sensor_set_slow_framerate = os04d10_set_slow_framerate;
    info->ae_func.pfn_sensor_get_max_inttime    = os04d10_get_max_inttime;
    info->ae_func.pfn_sensor_update_inttime     = os04d10_update_inttime;
    info->ae_func.pfn_sensor_calc_again         = os04d10_calc_again;
    info->ae_func.pfn_sensor_calc_dgain         = os04d10_calc_dgain;
    info->ae_func.pfn_sensor_update_gains       = os04d10_update_gains;
    info->ae_func.pfn_sensor_set_ae_wdr_attr    = os04d10_set_ae_wdr_attr;

    // AWB
    info->awb_func.pfn_sensor_get_awb_default = os04d10_get_awb_default;

    return XMEDIA_SUCCESS;
}

xmedia_s32 os04d10_register(xmedia_u32 pipe)
{
    xmedia_s32                  ret;
    xmedia_u32                  dev;
    xmedia_sensor_register_info info;

    SENSOR_CHECK_PIPE_RETURN(pipe);

    ret = os04d10_search_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = os04d10_ctx_init(dev);
    SENSOR_CHECK_RET_RETURN(ret);

    memset(&info, 0, sizeof(xmedia_sensor_register_info));
    info.dev_id = dev;
    ret         = os04d10_init_register_func(&info);
    SENSOR_CHECK_RET_RETURN(ret);

    ret = xmedia_isp_register_sensor(pipe, &info);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OS04D10 register failed! error code = %d.\n", ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 os04d10_unregister(xmedia_u32 pipe)
{
    xmedia_s32      ret;
    xmedia_u32      dev;
    sensor_context *os04d10_ctx = XMEDIA_NULL;

    SENSOR_CHECK_PIPE_RETURN(pipe);
    ret = os04d10_match_dev(pipe, &dev);
    SENSOR_CHECK_RET_RETURN(ret);

    OS04D10_GET_CTX(dev, os04d10_ctx);
    SENSOR_CHECK_PTR_RETURN(os04d10_ctx);

    if (os04d10_ctx->init == XMEDIA_TRUE) {
        SENSOR_PRINT("Operation not allowed, please EXIT at first!\n");
        return XMEDIA_ERRCODE_NOT_PERMITTED;
    }

    ret = xmedia_isp_unregister_sensor(pipe, dev);
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "OS04D10 unregister function failed!\n");
        return ret;
    }

    os04d10_ctx_exit(dev);
    g_os04d10_dev_map[dev] = SENSOR_DEV_INVALID; // reset dev

    return XMEDIA_SUCCESS;
}
