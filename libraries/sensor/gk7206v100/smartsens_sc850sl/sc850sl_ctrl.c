#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "sc850sl.h"
#include "sc850sl_ctrl.h"

static xmedia_s32 g_sc850sl_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

static xmedia_s32 g_sc850sl_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
#define SC850SL_REG_ADDR_MIRROR_FLIP 0x3221

static xmedia_s32 sc850sl_linear_comm_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc850sl_write_reg(dev, 0x36e9, 0x80);
    ret |= sc850sl_write_reg(dev, 0x36f9, 0x80);
    ret |= sc850sl_write_reg(dev, 0x36ea, 0x09);
    ret |= sc850sl_write_reg(dev, 0x36eb, 0x0c);
    ret |= sc850sl_write_reg(dev, 0x36ec, 0x4a);
    ret |= sc850sl_write_reg(dev, 0x36ed, 0x24);
    ret |= sc850sl_write_reg(dev, 0x36fa, 0x0b);
    ret |= sc850sl_write_reg(dev, 0x36fb, 0x33);
    ret |= sc850sl_write_reg(dev, 0x36fc, 0x10);
    ret |= sc850sl_write_reg(dev, 0x36fd, 0x37);
    ret |= sc850sl_write_reg(dev, 0x36e9, 0x24);
    ret |= sc850sl_write_reg(dev, 0x36f9, 0x53);
    ret |= sc850sl_write_reg(dev, 0x3018, 0x7a);
    ret |= sc850sl_write_reg(dev, 0x3019, 0xf0);
    ret |= sc850sl_write_reg(dev, 0x301a, 0x30);
    ret |= sc850sl_write_reg(dev, 0x301e, 0x3c);
    ret |= sc850sl_write_reg(dev, 0x301f, 0x0a);
    ret |= sc850sl_write_reg(dev, 0x302a, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3031, 0x0c);
    ret |= sc850sl_write_reg(dev, 0x3032, 0x20);
    ret |= sc850sl_write_reg(dev, 0x3033, 0x22);
    ret |= sc850sl_write_reg(dev, 0x3037, 0x00);
    ret |= sc850sl_write_reg(dev, 0x303e, 0xb4);
    ret |= sc850sl_write_reg(dev, 0x320c, 0x04);
    ret |= sc850sl_write_reg(dev, 0x320d, 0x4c);
    ret |= sc850sl_write_reg(dev, 0x3226, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3227, 0x03);
    ret |= sc850sl_write_reg(dev, 0x3250, 0x40);
    ret |= sc850sl_write_reg(dev, 0x3253, 0x08);
    ret |= sc850sl_write_reg(dev, 0x327e, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3280, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3281, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3301, 0x3c);
    ret |= sc850sl_write_reg(dev, 0x3304, 0x30);
    ret |= sc850sl_write_reg(dev, 0x3306, 0xe8);
    ret |= sc850sl_write_reg(dev, 0x3308, 0x10);
    ret |= sc850sl_write_reg(dev, 0x3309, 0x70);
    ret |= sc850sl_write_reg(dev, 0x330a, 0x01);
    ret |= sc850sl_write_reg(dev, 0x330b, 0xe0);
    ret |= sc850sl_write_reg(dev, 0x330d, 0x10);
    ret |= sc850sl_write_reg(dev, 0x3314, 0x92);
    ret |= sc850sl_write_reg(dev, 0x331e, 0x29);
    ret |= sc850sl_write_reg(dev, 0x331f, 0x69);
    ret |= sc850sl_write_reg(dev, 0x3333, 0x10);
    ret |= sc850sl_write_reg(dev, 0x3347, 0x05);
    ret |= sc850sl_write_reg(dev, 0x3348, 0xd0);
    ret |= sc850sl_write_reg(dev, 0x3352, 0x01);
    ret |= sc850sl_write_reg(dev, 0x3356, 0x38);
    ret |= sc850sl_write_reg(dev, 0x335d, 0x60);
    ret |= sc850sl_write_reg(dev, 0x3362, 0x70);
    ret |= sc850sl_write_reg(dev, 0x338f, 0x80);
    ret |= sc850sl_write_reg(dev, 0x33af, 0x48);
    ret |= sc850sl_write_reg(dev, 0x33fe, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3400, 0x12);
    ret |= sc850sl_write_reg(dev, 0x3406, 0x04);
    ret |= sc850sl_write_reg(dev, 0x3410, 0x12);
    ret |= sc850sl_write_reg(dev, 0x3416, 0x06);
    ret |= sc850sl_write_reg(dev, 0x3433, 0x01);
    ret |= sc850sl_write_reg(dev, 0x3440, 0x12);
    ret |= sc850sl_write_reg(dev, 0x3446, 0x08);
    ret |= sc850sl_write_reg(dev, 0x3478, 0x01);
    ret |= sc850sl_write_reg(dev, 0x3479, 0x01);
    ret |= sc850sl_write_reg(dev, 0x347a, 0x02);
    ret |= sc850sl_write_reg(dev, 0x347b, 0x01);
    ret |= sc850sl_write_reg(dev, 0x347c, 0x04);
    ret |= sc850sl_write_reg(dev, 0x347d, 0x01);
    ret |= sc850sl_write_reg(dev, 0x3616, 0x0c);
    ret |= sc850sl_write_reg(dev, 0x3620, 0x92);
    ret |= sc850sl_write_reg(dev, 0x3622, 0x74);
    ret |= sc850sl_write_reg(dev, 0x3629, 0x74);
    ret |= sc850sl_write_reg(dev, 0x362a, 0xf0);
    ret |= sc850sl_write_reg(dev, 0x362b, 0x0f);
    ret |= sc850sl_write_reg(dev, 0x362d, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3630, 0x68);
    ret |= sc850sl_write_reg(dev, 0x3633, 0x22);
    ret |= sc850sl_write_reg(dev, 0x3634, 0x22);
    ret |= sc850sl_write_reg(dev, 0x3635, 0x20);
    ret |= sc850sl_write_reg(dev, 0x3637, 0x06);
    ret |= sc850sl_write_reg(dev, 0x3638, 0x26);
    ret |= sc850sl_write_reg(dev, 0x363b, 0x06);
    ret |= sc850sl_write_reg(dev, 0x363c, 0x07);
    ret |= sc850sl_write_reg(dev, 0x363d, 0x05);
    ret |= sc850sl_write_reg(dev, 0x363e, 0x8f);
    ret |= sc850sl_write_reg(dev, 0x3648, 0xe0);
    ret |= sc850sl_write_reg(dev, 0x3649, 0x0a);
    ret |= sc850sl_write_reg(dev, 0x364a, 0x06);
    ret |= sc850sl_write_reg(dev, 0x364c, 0x6a);
    ret |= sc850sl_write_reg(dev, 0x3650, 0x3d);
    ret |= sc850sl_write_reg(dev, 0x3654, 0x40);
    ret |= sc850sl_write_reg(dev, 0x3656, 0x68);
    ret |= sc850sl_write_reg(dev, 0x3657, 0x0f);
    ret |= sc850sl_write_reg(dev, 0x3658, 0x3d);
    ret |= sc850sl_write_reg(dev, 0x365c, 0x40);
    ret |= sc850sl_write_reg(dev, 0x365e, 0x68);
    ret |= sc850sl_write_reg(dev, 0x3901, 0x04);
    ret |= sc850sl_write_reg(dev, 0x3904, 0x20);
    ret |= sc850sl_write_reg(dev, 0x3905, 0x91);
    ret |= sc850sl_write_reg(dev, 0x391e, 0x83);
    ret |= sc850sl_write_reg(dev, 0x3928, 0x04);
    ret |= sc850sl_write_reg(dev, 0x3933, 0xa0);
    ret |= sc850sl_write_reg(dev, 0x3934, 0x0a);
    ret |= sc850sl_write_reg(dev, 0x3935, 0x68);
    ret |= sc850sl_write_reg(dev, 0x3936, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3937, 0x20);
    ret |= sc850sl_write_reg(dev, 0x3938, 0x0a);
    ret |= sc850sl_write_reg(dev, 0x3946, 0x20);
    ret |= sc850sl_write_reg(dev, 0x3961, 0x40);
    ret |= sc850sl_write_reg(dev, 0x3962, 0x40);
    ret |= sc850sl_write_reg(dev, 0x3963, 0xc8);
    ret |= sc850sl_write_reg(dev, 0x3964, 0xc8);
    ret |= sc850sl_write_reg(dev, 0x3965, 0x40);
    ret |= sc850sl_write_reg(dev, 0x3966, 0x40);
    ret |= sc850sl_write_reg(dev, 0x3967, 0x00);
    ret |= sc850sl_write_reg(dev, 0x39cd, 0xc8);
    ret |= sc850sl_write_reg(dev, 0x39ce, 0xc8);
    ret |= sc850sl_write_reg(dev, 0x3e01, 0x82);
    ret |= sc850sl_write_reg(dev, 0x3e02, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3e0e, 0x02);
    ret |= sc850sl_write_reg(dev, 0x3e0f, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3e1c, 0x0f);
    ret |= sc850sl_write_reg(dev, 0x3e23, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3e24, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3e53, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3e54, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3e68, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3e69, 0x80);
    ret |= sc850sl_write_reg(dev, 0x3e73, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3e74, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3e86, 0x03);
    ret |= sc850sl_write_reg(dev, 0x3e87, 0x40);
    ret |= sc850sl_write_reg(dev, 0x3f02, 0x24);
    ret |= sc850sl_write_reg(dev, 0x4424, 0x02);
    ret |= sc850sl_write_reg(dev, 0x4501, 0xc4);
    ret |= sc850sl_write_reg(dev, 0x4509, 0x20);
    ret |= sc850sl_write_reg(dev, 0x4561, 0x12);
    ret |= sc850sl_write_reg(dev, 0x4800, 0x24);
    ret |= sc850sl_write_reg(dev, 0x4837, 0x0f);
    ret |= sc850sl_write_reg(dev, 0x4900, 0x24);
    ret |= sc850sl_write_reg(dev, 0x4937, 0x0f);
    ret |= sc850sl_write_reg(dev, 0x5000, 0x0e);
    ret |= sc850sl_write_reg(dev, 0x500f, 0x35);
    ret |= sc850sl_write_reg(dev, 0x5020, 0x00);
    ret |= sc850sl_write_reg(dev, 0x5787, 0x10);
    ret |= sc850sl_write_reg(dev, 0x5788, 0x06);
    ret |= sc850sl_write_reg(dev, 0x5789, 0x00);
    ret |= sc850sl_write_reg(dev, 0x578a, 0x18);
    ret |= sc850sl_write_reg(dev, 0x578b, 0x0c);
    ret |= sc850sl_write_reg(dev, 0x578c, 0x00);
    ret |= sc850sl_write_reg(dev, 0x5790, 0x10);
    ret |= sc850sl_write_reg(dev, 0x5791, 0x06);
    ret |= sc850sl_write_reg(dev, 0x5792, 0x01);
    ret |= sc850sl_write_reg(dev, 0x5793, 0x18);
    ret |= sc850sl_write_reg(dev, 0x5794, 0x0c);
    ret |= sc850sl_write_reg(dev, 0x5795, 0x01);
    ret |= sc850sl_write_reg(dev, 0x5799, 0x06);
    ret |= sc850sl_write_reg(dev, 0x57a2, 0x60);
    ret |= sc850sl_write_reg(dev, 0x59e0, 0xfe);
    ret |= sc850sl_write_reg(dev, 0x59e1, 0x40);
    ret |= sc850sl_write_reg(dev, 0x59e2, 0x38);
    ret |= sc850sl_write_reg(dev, 0x59e3, 0x30);
    ret |= sc850sl_write_reg(dev, 0x59e4, 0x20);
    ret |= sc850sl_write_reg(dev, 0x59e5, 0x38);
    ret |= sc850sl_write_reg(dev, 0x59e6, 0x30);
    ret |= sc850sl_write_reg(dev, 0x59e7, 0x20);
    ret |= sc850sl_write_reg(dev, 0x59e8, 0x3f);
    ret |= sc850sl_write_reg(dev, 0x59e9, 0x38);
    ret |= sc850sl_write_reg(dev, 0x59ea, 0x30);
    ret |= sc850sl_write_reg(dev, 0x59eb, 0x3f);
    ret |= sc850sl_write_reg(dev, 0x59ec, 0x38);
    ret |= sc850sl_write_reg(dev, 0x59ed, 0x30);
    ret |= sc850sl_write_reg(dev, 0x59ee, 0xfe);
    ret |= sc850sl_write_reg(dev, 0x59ef, 0x40);
    ret |= sc850sl_write_reg(dev, 0x59f4, 0x38);
    ret |= sc850sl_write_reg(dev, 0x59f5, 0x30);
    ret |= sc850sl_write_reg(dev, 0x59f6, 0x20);
    ret |= sc850sl_write_reg(dev, 0x59f7, 0x38);
    ret |= sc850sl_write_reg(dev, 0x59f8, 0x30);
    ret |= sc850sl_write_reg(dev, 0x59f9, 0x20);
    ret |= sc850sl_write_reg(dev, 0x59fa, 0x3f);
    ret |= sc850sl_write_reg(dev, 0x59fb, 0x38);
    ret |= sc850sl_write_reg(dev, 0x59fc, 0x30);
    ret |= sc850sl_write_reg(dev, 0x59fd, 0x3f);
    ret |= sc850sl_write_reg(dev, 0x59fe, 0x38);
    ret |= sc850sl_write_reg(dev, 0x59ff, 0x30);
    ret |= sc850sl_write_reg(dev, 0x5000, 0x00); // sensor dpc off
    ret |= sc850sl_write_reg(dev, 0x5002, 0x00);
    ret |= sc850sl_write_reg(dev, 0x33b0, 0x00);
    ret |= sc850sl_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc850sl_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc850sl_4lane_linear_3840x2160_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc850sl_write_reg(dev, 0x0103, 0x01);
    ret |= sc850sl_write_reg(dev, 0x0100, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3208, 0x0f);
    ret |= sc850sl_write_reg(dev, 0x3209, 0x00);
    ret |= sc850sl_write_reg(dev, 0x320a, 0x08);
    ret |= sc850sl_write_reg(dev, 0x320b, 0x70);
    ret |= sc850sl_linear_comm_init(dev);


    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc850sl_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("============================================================================\n");
        SENSOR_PRINT("====== SC850SL_24MInput_MIPI_4lane_12bit_3840x2160_linear init success! ====\n");
        SENSOR_PRINT("============================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

static xmedia_s32 sc850sl_4lane_linear_2560x1920_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc850sl_write_reg(dev, 0x0103, 0x01);
    ret |= sc850sl_write_reg(dev, 0x0100, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3208, 0x0a);
    ret |= sc850sl_write_reg(dev, 0x3209, 0x00);
    ret |= sc850sl_write_reg(dev, 0x320a, 0x07);
    ret |= sc850sl_write_reg(dev, 0x320b, 0x80);
    ret |= sc850sl_linear_comm_init(dev);


    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc850sl_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("============================================================================\n");
        SENSOR_PRINT("====== SC850SL_24MInput_MIPI_4lane_12bit_2560x1920_linear init success! ====\n");
        SENSOR_PRINT("============================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

static xmedia_s32 sc850sl_4lane_linear_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc850sl_write_reg(dev, 0x0103, 0x01);
    ret |= sc850sl_write_reg(dev, 0x0100, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3208, 0x0a);
    ret |= sc850sl_write_reg(dev, 0x3209, 0x80);
    ret |= sc850sl_write_reg(dev, 0x320a, 0x05);
    ret |= sc850sl_write_reg(dev, 0x320b, 0xf0);
    ret |= sc850sl_linear_comm_init(dev);


    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc850sl_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("============================================================================\n");
        SENSOR_PRINT("====== SC850SL_24MInput_MIPI_4lane_12bit_2688x1520_linear init success! ====\n");
        SENSOR_PRINT("============================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

static xmedia_s32 sc850sl_wdr_comm_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc850sl_write_reg(dev, 0x36e9,0x80);
    ret |= sc850sl_write_reg(dev, 0x36f9,0x80);
    ret |= sc850sl_write_reg(dev, 0x36ea,0x09);
    ret |= sc850sl_write_reg(dev, 0x36eb,0x0c);
    ret |= sc850sl_write_reg(dev, 0x36ec,0x4b);
    ret |= sc850sl_write_reg(dev, 0x36ed,0x34);
    ret |= sc850sl_write_reg(dev, 0x36fa,0x0b);
    ret |= sc850sl_write_reg(dev, 0x36fb,0x13);
    ret |= sc850sl_write_reg(dev, 0x36fc,0x00);
    ret |= sc850sl_write_reg(dev, 0x36fd,0x17);
    ret |= sc850sl_write_reg(dev, 0x36e9,0x24);
    ret |= sc850sl_write_reg(dev, 0x36f9,0x20);
    ret |= sc850sl_write_reg(dev, 0x3018,0x7a);
    ret |= sc850sl_write_reg(dev, 0x3019,0xf0);
    ret |= sc850sl_write_reg(dev, 0x301a,0x30);
    ret |= sc850sl_write_reg(dev, 0x301e,0x3c);
    ret |= sc850sl_write_reg(dev, 0x301f,0x06);
    ret |= sc850sl_write_reg(dev, 0x3028,0x77);
    ret |= sc850sl_write_reg(dev, 0x302a,0x00);
    ret |= sc850sl_write_reg(dev, 0x3031,0x0a);
    ret |= sc850sl_write_reg(dev, 0x3032,0x20);
    ret |= sc850sl_write_reg(dev, 0x3033,0x22);
    ret |= sc850sl_write_reg(dev, 0x3037,0x60);
    ret |= sc850sl_write_reg(dev, 0x303e,0xb4);
    ret |= sc850sl_write_reg(dev, 0x3201,0x98);
    ret |= sc850sl_write_reg(dev, 0x3203,0x0c);
    ret |= sc850sl_write_reg(dev, 0x3205,0xa7);
    ret |= sc850sl_write_reg(dev, 0x3207,0x83);
    ret |= sc850sl_write_reg(dev, 0x320c,0x03);
    ret |= sc850sl_write_reg(dev, 0x320d,0x26);
    ret |= sc850sl_write_reg(dev, 0x320e,0x11);
    ret |= sc850sl_write_reg(dev, 0x320f,0x94);
    ret |= sc850sl_write_reg(dev, 0x3211,0x08);
    ret |= sc850sl_write_reg(dev, 0x3213,0x2c);
    ret |= sc850sl_write_reg(dev, 0x3223,0xd0);
    ret |= sc850sl_write_reg(dev, 0x3226,0x10);
    ret |= sc850sl_write_reg(dev, 0x3227,0x03);
    ret |= sc850sl_write_reg(dev, 0x3230,0x11);
    ret |= sc850sl_write_reg(dev, 0x3231,0x93);
    ret |= sc850sl_write_reg(dev, 0x3250,0xff);
    ret |= sc850sl_write_reg(dev, 0x3253,0x08);
    ret |= sc850sl_write_reg(dev, 0x327e,0x55);
    ret |= sc850sl_write_reg(dev, 0x3280,0x00);
    ret |= sc850sl_write_reg(dev, 0x3281,0x01);
    ret |= sc850sl_write_reg(dev, 0x3301,0x24);
    ret |= sc850sl_write_reg(dev, 0x3304,0x30);
    ret |= sc850sl_write_reg(dev, 0x3306,0x54);
    ret |= sc850sl_write_reg(dev, 0x3308,0x10);
    ret |= sc850sl_write_reg(dev, 0x3309,0x60);
    ret |= sc850sl_write_reg(dev, 0x330a,0x00);
    ret |= sc850sl_write_reg(dev, 0x330b,0xa0);
    ret |= sc850sl_write_reg(dev, 0x330d,0x10);
    ret |= sc850sl_write_reg(dev, 0x3314,0x92);
    ret |= sc850sl_write_reg(dev, 0x331e,0x29);
    ret |= sc850sl_write_reg(dev, 0x331f,0x59);
    ret |= sc850sl_write_reg(dev, 0x3333,0x10);
    ret |= sc850sl_write_reg(dev, 0x3347,0x05);
    ret |= sc850sl_write_reg(dev, 0x3348,0x50);
    ret |= sc850sl_write_reg(dev, 0x3352,0x01);
    ret |= sc850sl_write_reg(dev, 0x3356,0x38);
    ret |= sc850sl_write_reg(dev, 0x335d,0x60);
    ret |= sc850sl_write_reg(dev, 0x3362,0x70);
    ret |= sc850sl_write_reg(dev, 0x338f,0x80);
    ret |= sc850sl_write_reg(dev, 0x33af,0x48);
    ret |= sc850sl_write_reg(dev, 0x33fe,0x02);
    ret |= sc850sl_write_reg(dev, 0x3400,0x12);
    ret |= sc850sl_write_reg(dev, 0x3406,0x04);
    ret |= sc850sl_write_reg(dev, 0x3410,0x12);
    ret |= sc850sl_write_reg(dev, 0x3416,0x06);
    ret |= sc850sl_write_reg(dev, 0x3433,0x01);
    ret |= sc850sl_write_reg(dev, 0x3440,0x12);
    ret |= sc850sl_write_reg(dev, 0x3446,0x08);
    ret |= sc850sl_write_reg(dev, 0x3478,0x01);
    ret |= sc850sl_write_reg(dev, 0x3479,0x01);
    ret |= sc850sl_write_reg(dev, 0x347a,0x02);
    ret |= sc850sl_write_reg(dev, 0x347b,0x01);
    ret |= sc850sl_write_reg(dev, 0x347c,0x02);
    ret |= sc850sl_write_reg(dev, 0x347d,0x01);
    ret |= sc850sl_write_reg(dev, 0x3616,0x0c);
    ret |= sc850sl_write_reg(dev, 0x3620,0x94);
    ret |= sc850sl_write_reg(dev, 0x3622,0x74);
    ret |= sc850sl_write_reg(dev, 0x3629,0x74);
    ret |= sc850sl_write_reg(dev, 0x362a,0xf0);
    ret |= sc850sl_write_reg(dev, 0x362b,0x0f);
    ret |= sc850sl_write_reg(dev, 0x362d,0x00);
    ret |= sc850sl_write_reg(dev, 0x3630,0x68);
    ret |= sc850sl_write_reg(dev, 0x3633,0x24);
    ret |= sc850sl_write_reg(dev, 0x3634,0x22);
    ret |= sc850sl_write_reg(dev, 0x3635,0x20);
    ret |= sc850sl_write_reg(dev, 0x3637,0x18);
    ret |= sc850sl_write_reg(dev, 0x3638,0x26);
    ret |= sc850sl_write_reg(dev, 0x363b,0x06);
    ret |= sc850sl_write_reg(dev, 0x363c,0x07);
    ret |= sc850sl_write_reg(dev, 0x363d,0x05);
    ret |= sc850sl_write_reg(dev, 0x363e,0x8f);
    ret |= sc850sl_write_reg(dev, 0x3648,0xe0);
    ret |= sc850sl_write_reg(dev, 0x3649,0x0a);
    ret |= sc850sl_write_reg(dev, 0x364a,0x06);
    ret |= sc850sl_write_reg(dev, 0x364c,0x6a);
    ret |= sc850sl_write_reg(dev, 0x3650,0x3d);
    ret |= sc850sl_write_reg(dev, 0x3654,0x70);
    ret |= sc850sl_write_reg(dev, 0x3656,0x68);
    ret |= sc850sl_write_reg(dev, 0x3657,0x0f);
    ret |= sc850sl_write_reg(dev, 0x3658,0x3d);
    ret |= sc850sl_write_reg(dev, 0x365c,0x40);
    ret |= sc850sl_write_reg(dev, 0x365e,0x68);
    ret |= sc850sl_write_reg(dev, 0x3901,0x04);
    ret |= sc850sl_write_reg(dev, 0x3902,0xf1);
    ret |= sc850sl_write_reg(dev, 0x3904,0x20);
    ret |= sc850sl_write_reg(dev, 0x3905,0x91);
    ret |= sc850sl_write_reg(dev, 0x391e,0x03);
    ret |= sc850sl_write_reg(dev, 0x3928,0x04);
    ret |= sc850sl_write_reg(dev, 0x3933,0xa0);
    ret |= sc850sl_write_reg(dev, 0x3934,0x0a);
    ret |= sc850sl_write_reg(dev, 0x3935,0x68);
    ret |= sc850sl_write_reg(dev, 0x3936,0x00);
    ret |= sc850sl_write_reg(dev, 0x3937,0x20);
    ret |= sc850sl_write_reg(dev, 0x3938,0x0a);
    ret |= sc850sl_write_reg(dev, 0x3946,0x20);
    ret |= sc850sl_write_reg(dev, 0x3961,0x40);
    ret |= sc850sl_write_reg(dev, 0x3962,0x40);
    ret |= sc850sl_write_reg(dev, 0x3963,0xc8);
    ret |= sc850sl_write_reg(dev, 0x3964,0xc8);
    ret |= sc850sl_write_reg(dev, 0x3965,0x40);
    ret |= sc850sl_write_reg(dev, 0x3966,0x40);
    ret |= sc850sl_write_reg(dev, 0x3967,0x00);
    ret |= sc850sl_write_reg(dev, 0x39cd,0xc8);
    ret |= sc850sl_write_reg(dev, 0x39ce,0xc8);
    ret |= sc850sl_write_reg(dev, 0x3e00,0x01);
    ret |= sc850sl_write_reg(dev, 0x3e01,0x00);
    ret |= sc850sl_write_reg(dev, 0x3e02,0x00);
    ret |= sc850sl_write_reg(dev, 0x3e04,0x10);
    ret |= sc850sl_write_reg(dev, 0x3e05,0x00);
    ret |= sc850sl_write_reg(dev, 0x3e0e,0x02);
    ret |= sc850sl_write_reg(dev, 0x3e0f,0x00);
    ret |= sc850sl_write_reg(dev, 0x3e1c,0x0f);
    ret |= sc850sl_write_reg(dev, 0x3e23,0x01);
    ret |= sc850sl_write_reg(dev, 0x3e24,0x0a);
    ret |= sc850sl_write_reg(dev, 0x3e53,0x00);
    ret |= sc850sl_write_reg(dev, 0x3e54,0x00);
    ret |= sc850sl_write_reg(dev, 0x3e68,0x00);
    ret |= sc850sl_write_reg(dev, 0x3e69,0x80);
    ret |= sc850sl_write_reg(dev, 0x3e73,0x00);
    ret |= sc850sl_write_reg(dev, 0x3e74,0x00);
    ret |= sc850sl_write_reg(dev, 0x3e86,0x03);
    ret |= sc850sl_write_reg(dev, 0x3e87,0x40);
    ret |= sc850sl_write_reg(dev, 0x3f02,0x24);
    ret |= sc850sl_write_reg(dev, 0x4424,0x02);
    ret |= sc850sl_write_reg(dev, 0x4501,0xb4);
    ret |= sc850sl_write_reg(dev, 0x4503,0x60);
    ret |= sc850sl_write_reg(dev, 0x4509,0x20);
    ret |= sc850sl_write_reg(dev, 0x4561,0x12);
    ret |= sc850sl_write_reg(dev, 0x4800,0x24);
    ret |= sc850sl_write_reg(dev, 0x4837,0x16);
    ret |= sc850sl_write_reg(dev, 0x4853,0xf8);
    ret |= sc850sl_write_reg(dev, 0x4900,0x24);
    ret |= sc850sl_write_reg(dev, 0x4937,0x16);
    ret |= sc850sl_write_reg(dev, 0x4953,0xf8);
    ret |= sc850sl_write_reg(dev, 0x5000,0x0e);
    ret |= sc850sl_write_reg(dev, 0x500f,0x35);
    ret |= sc850sl_write_reg(dev, 0x5020,0x00);
    ret |= sc850sl_write_reg(dev, 0x5787,0x10);
    ret |= sc850sl_write_reg(dev, 0x5788,0x06);
    ret |= sc850sl_write_reg(dev, 0x5789,0x00);
    ret |= sc850sl_write_reg(dev, 0x578a,0x18);
    ret |= sc850sl_write_reg(dev, 0x578b,0x0c);
    ret |= sc850sl_write_reg(dev, 0x578c,0x00);
    ret |= sc850sl_write_reg(dev, 0x5790,0x10);
    ret |= sc850sl_write_reg(dev, 0x5791,0x06);
    ret |= sc850sl_write_reg(dev, 0x5792,0x01);
    ret |= sc850sl_write_reg(dev, 0x5793,0x18);
    ret |= sc850sl_write_reg(dev, 0x5794,0x0c);
    ret |= sc850sl_write_reg(dev, 0x5795,0x01);
    ret |= sc850sl_write_reg(dev, 0x5799,0x06);
    ret |= sc850sl_write_reg(dev, 0x57a2,0x60);
    ret |= sc850sl_write_reg(dev, 0x59e0,0xfe);
    ret |= sc850sl_write_reg(dev, 0x59e1,0x40);
    ret |= sc850sl_write_reg(dev, 0x59e2,0x38);
    ret |= sc850sl_write_reg(dev, 0x59e3,0x30);
    ret |= sc850sl_write_reg(dev, 0x59e4,0x20);
    ret |= sc850sl_write_reg(dev, 0x59e5,0x38);
    ret |= sc850sl_write_reg(dev, 0x59e6,0x30);
    ret |= sc850sl_write_reg(dev, 0x59e7,0x20);
    ret |= sc850sl_write_reg(dev, 0x59e8,0x3f);
    ret |= sc850sl_write_reg(dev, 0x59e9,0x38);
    ret |= sc850sl_write_reg(dev, 0x59ea,0x30);
    ret |= sc850sl_write_reg(dev, 0x59eb,0x3f);
    ret |= sc850sl_write_reg(dev, 0x59ec,0x38);
    ret |= sc850sl_write_reg(dev, 0x59ed,0x30);
    ret |= sc850sl_write_reg(dev, 0x59ee,0xfe);
    ret |= sc850sl_write_reg(dev, 0x59ef,0x40);
    ret |= sc850sl_write_reg(dev, 0x59f4,0x38);
    ret |= sc850sl_write_reg(dev, 0x59f5,0x30);
    ret |= sc850sl_write_reg(dev, 0x59f6,0x20);
    ret |= sc850sl_write_reg(dev, 0x59f7,0x38);
    ret |= sc850sl_write_reg(dev, 0x59f8,0x30);
    ret |= sc850sl_write_reg(dev, 0x59f9,0x20);
    ret |= sc850sl_write_reg(dev, 0x59fa,0x3f);
    ret |= sc850sl_write_reg(dev, 0x59fb,0x38);
    ret |= sc850sl_write_reg(dev, 0x59fc,0x30);
    ret |= sc850sl_write_reg(dev, 0x59fd,0x3f);
    ret |= sc850sl_write_reg(dev, 0x59fe,0x38);
    ret |= sc850sl_write_reg(dev, 0x59ff,0x30);
    ret |= sc850sl_write_reg(dev, 0x0100,0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc850sl_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 sc850sl_4lane_wdr_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    ret |= sc850sl_write_reg(dev, 0x0103, 0x01);
    ret |= sc850sl_write_reg(dev, 0x0100, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3208, 0x0a);
    ret |= sc850sl_write_reg(dev, 0x3209, 0x80);
    ret |= sc850sl_write_reg(dev, 0x320a, 0x05);
    ret |= sc850sl_write_reg(dev, 0x320b, 0xf0);
    ret |= sc850sl_wdr_comm_init(dev);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc850sl_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("================================================================================\n");
        SENSOR_PRINT("=== SC850SL_MIPI_27MInput_raw10_4lane_2to1wdr_2688X1520_init success!===========\n");
        SENSOR_PRINT("================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

static xmedia_s32 sc850sl_4lane_wdr_2560x1920_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc850sl_write_reg(dev, 0x0103, 0x01);
    ret |= sc850sl_write_reg(dev, 0x0100, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3208, 0x0a);
    ret |= sc850sl_write_reg(dev, 0x3209, 0x00);
    ret |= sc850sl_write_reg(dev, 0x320a, 0x07);
    ret |= sc850sl_write_reg(dev, 0x320b, 0x80);
    ret |= sc850sl_write_reg(dev, 0x320c, 0x08);
    ret |= sc850sl_wdr_comm_init(dev);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc850sl_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("================================================================================\n");
        SENSOR_PRINT("====SC850SL_MIPI_27MInput_raw10_4lane_2to1wdr_2560X1920_init success!===========\n");
        SENSOR_PRINT("================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

static xmedia_s32 sc850sl_4lane_wdr_3840x2160_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc850sl_write_reg(dev, 0x0103, 0x01);
    ret |= sc850sl_write_reg(dev, 0x0100, 0x00);
    ret |= sc850sl_write_reg(dev, 0x36e9, 0x80);
    ret |= sc850sl_write_reg(dev, 0x36f9, 0x80);
    ret |= sc850sl_write_reg(dev, 0x36ea, 0x07);
    ret |= sc850sl_write_reg(dev, 0x36eb, 0x0c);
    ret |= sc850sl_write_reg(dev, 0x36ec, 0x4b);
    ret |= sc850sl_write_reg(dev, 0x36ed, 0x24);
    ret |= sc850sl_write_reg(dev, 0x36fa, 0x0b);
    ret |= sc850sl_write_reg(dev, 0x36fb, 0x13);
    ret |= sc850sl_write_reg(dev, 0x36fc, 0x00);
    ret |= sc850sl_write_reg(dev, 0x36fd, 0x17);
    ret |= sc850sl_write_reg(dev, 0x36e9, 0x24);
    ret |= sc850sl_write_reg(dev, 0x36f9, 0x20);
    ret |= sc850sl_write_reg(dev, 0x3000, 0x01);
    ret |= sc850sl_write_reg(dev, 0x3018, 0x7a);
    ret |= sc850sl_write_reg(dev, 0x3019, 0xf0);
    ret |= sc850sl_write_reg(dev, 0x301a, 0x30);
    ret |= sc850sl_write_reg(dev, 0x301e, 0x3c);
    ret |= sc850sl_write_reg(dev, 0x301f, 0x05);
    ret |= sc850sl_write_reg(dev, 0x3028, 0x77);
    ret |= sc850sl_write_reg(dev, 0x302a, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3031, 0x0a);
    ret |= sc850sl_write_reg(dev, 0x3032, 0x20);
    ret |= sc850sl_write_reg(dev, 0x3033, 0x22);
    ret |= sc850sl_write_reg(dev, 0x3037, 0x60);
    ret |= sc850sl_write_reg(dev, 0x303e, 0xb4);
    ret |= sc850sl_write_reg(dev, 0x3201, 0x98);
    ret |= sc850sl_write_reg(dev, 0x3203, 0x0c);
    ret |= sc850sl_write_reg(dev, 0x3205, 0xa7);
    ret |= sc850sl_write_reg(dev, 0x3207, 0x83);
    ret |= sc850sl_write_reg(dev, 0x320c, 0x03);
    ret |= sc850sl_write_reg(dev, 0x320d, 0x39);
    ret |= sc850sl_write_reg(dev, 0x320e, 0x11);
    ret |= sc850sl_write_reg(dev, 0x320f, 0x94);
    ret |= sc850sl_write_reg(dev, 0x3211, 0x08);
    ret |= sc850sl_write_reg(dev, 0x3213, 0x2c);
    ret |= sc850sl_write_reg(dev, 0x3223, 0xd0);
    ret |= sc850sl_write_reg(dev, 0x3226, 0x10);
    ret |= sc850sl_write_reg(dev, 0x3227, 0x03);
    ret |= sc850sl_write_reg(dev, 0x3230, 0x11);
    ret |= sc850sl_write_reg(dev, 0x3231, 0x93);
    ret |= sc850sl_write_reg(dev, 0x3250, 0xff);
    ret |= sc850sl_write_reg(dev, 0x3253, 0x08);
    ret |= sc850sl_write_reg(dev, 0x327e, 0x55);
    ret |= sc850sl_write_reg(dev, 0x3280, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3281, 0x01);
    ret |= sc850sl_write_reg(dev, 0x3301, 0x24);
    ret |= sc850sl_write_reg(dev, 0x3304, 0x30);
    ret |= sc850sl_write_reg(dev, 0x3306, 0x54);
    ret |= sc850sl_write_reg(dev, 0x3308, 0x10);
    ret |= sc850sl_write_reg(dev, 0x3309, 0x60);
    ret |= sc850sl_write_reg(dev, 0x330a, 0x00);
    ret |= sc850sl_write_reg(dev, 0x330b, 0xa0);
    ret |= sc850sl_write_reg(dev, 0x330d, 0x10);
    ret |= sc850sl_write_reg(dev, 0x3314, 0x92);
    ret |= sc850sl_write_reg(dev, 0x331e, 0x29);
    ret |= sc850sl_write_reg(dev, 0x331f, 0x59);
    ret |= sc850sl_write_reg(dev, 0x3333, 0x10);
    ret |= sc850sl_write_reg(dev, 0x3347, 0x05);
    ret |= sc850sl_write_reg(dev, 0x3348, 0x50);
    ret |= sc850sl_write_reg(dev, 0x3352, 0x01);
    ret |= sc850sl_write_reg(dev, 0x3356, 0x38);
    ret |= sc850sl_write_reg(dev, 0x335d, 0x60);
    ret |= sc850sl_write_reg(dev, 0x3362, 0x70);
    ret |= sc850sl_write_reg(dev, 0x338f, 0x80);
    ret |= sc850sl_write_reg(dev, 0x33af, 0x48);
    ret |= sc850sl_write_reg(dev, 0x33fe, 0x02);
    ret |= sc850sl_write_reg(dev, 0x3400, 0x12);
    ret |= sc850sl_write_reg(dev, 0x3406, 0x04);
    ret |= sc850sl_write_reg(dev, 0x3410, 0x12);
    ret |= sc850sl_write_reg(dev, 0x3416, 0x06);
    ret |= sc850sl_write_reg(dev, 0x3433, 0x01);
    ret |= sc850sl_write_reg(dev, 0x3440, 0x12);
    ret |= sc850sl_write_reg(dev, 0x3446, 0x08);
    ret |= sc850sl_write_reg(dev, 0x3478, 0x01);
    ret |= sc850sl_write_reg(dev, 0x3479, 0x01);
    ret |= sc850sl_write_reg(dev, 0x347a, 0x02);
    ret |= sc850sl_write_reg(dev, 0x347b, 0x01);
    ret |= sc850sl_write_reg(dev, 0x347c, 0x02);
    ret |= sc850sl_write_reg(dev, 0x347d, 0x01);
    ret |= sc850sl_write_reg(dev, 0x3616, 0x0c);
    ret |= sc850sl_write_reg(dev, 0x3620, 0x94);
    ret |= sc850sl_write_reg(dev, 0x3622, 0x74);
    ret |= sc850sl_write_reg(dev, 0x3629, 0x74);
    ret |= sc850sl_write_reg(dev, 0x362a, 0xf0);
    ret |= sc850sl_write_reg(dev, 0x362b, 0x0f);
    ret |= sc850sl_write_reg(dev, 0x362d, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3630, 0x68);
    ret |= sc850sl_write_reg(dev, 0x3633, 0x24);
    ret |= sc850sl_write_reg(dev, 0x3634, 0x22);
    ret |= sc850sl_write_reg(dev, 0x3635, 0x20);
    ret |= sc850sl_write_reg(dev, 0x3637, 0x18);
    ret |= sc850sl_write_reg(dev, 0x3638, 0x26);
    ret |= sc850sl_write_reg(dev, 0x363b, 0x06);
    ret |= sc850sl_write_reg(dev, 0x363c, 0x07);
    ret |= sc850sl_write_reg(dev, 0x363d, 0x05);
    ret |= sc850sl_write_reg(dev, 0x363e, 0x8f);
    ret |= sc850sl_write_reg(dev, 0x3648, 0xe0);
    ret |= sc850sl_write_reg(dev, 0x3649, 0x0a);
    ret |= sc850sl_write_reg(dev, 0x364a, 0x06);
    ret |= sc850sl_write_reg(dev, 0x364c, 0x6a);
    ret |= sc850sl_write_reg(dev, 0x3650, 0x3d);
    ret |= sc850sl_write_reg(dev, 0x3654, 0x70);
    ret |= sc850sl_write_reg(dev, 0x3656, 0x68);
    ret |= sc850sl_write_reg(dev, 0x3657, 0x0f);
    ret |= sc850sl_write_reg(dev, 0x3658, 0x3d);
    ret |= sc850sl_write_reg(dev, 0x365c, 0x40);
    ret |= sc850sl_write_reg(dev, 0x365e, 0x68);
    ret |= sc850sl_write_reg(dev, 0x3901, 0x04);
    ret |= sc850sl_write_reg(dev, 0x3902, 0xf1);
    ret |= sc850sl_write_reg(dev, 0x3904, 0x20);
    ret |= sc850sl_write_reg(dev, 0x3905, 0x91);
    ret |= sc850sl_write_reg(dev, 0x391e, 0x03);
    ret |= sc850sl_write_reg(dev, 0x3928, 0x04);
    ret |= sc850sl_write_reg(dev, 0x3933, 0xa0);
    ret |= sc850sl_write_reg(dev, 0x3934, 0x0a);
    ret |= sc850sl_write_reg(dev, 0x3935, 0x68);
    ret |= sc850sl_write_reg(dev, 0x3936, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3937, 0x20);
    ret |= sc850sl_write_reg(dev, 0x3938, 0x0a);
    ret |= sc850sl_write_reg(dev, 0x3946, 0x20);
    ret |= sc850sl_write_reg(dev, 0x3961, 0x40);
    ret |= sc850sl_write_reg(dev, 0x3962, 0x40);
    ret |= sc850sl_write_reg(dev, 0x3963, 0xc8);
    ret |= sc850sl_write_reg(dev, 0x3964, 0xc8);
    ret |= sc850sl_write_reg(dev, 0x3965, 0x40);
    ret |= sc850sl_write_reg(dev, 0x3966, 0x40);
    ret |= sc850sl_write_reg(dev, 0x3967, 0x00);
    ret |= sc850sl_write_reg(dev, 0x39cd, 0xc8);
    ret |= sc850sl_write_reg(dev, 0x39ce, 0xc8);
    ret |= sc850sl_write_reg(dev, 0x3e00, 0x01);
    ret |= sc850sl_write_reg(dev, 0x3e01, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3e02, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3e04, 0x10);
    ret |= sc850sl_write_reg(dev, 0x3e05, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3e0e, 0x02);
    ret |= sc850sl_write_reg(dev, 0x3e0f, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3e1c, 0x0f);
    ret |= sc850sl_write_reg(dev, 0x3e23, 0x01);
    ret |= sc850sl_write_reg(dev, 0x3e24, 0x0a);
    ret |= sc850sl_write_reg(dev, 0x3e53, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3e54, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3e68, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3e69, 0x80);
    ret |= sc850sl_write_reg(dev, 0x3e73, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3e74, 0x00);
    ret |= sc850sl_write_reg(dev, 0x3e86, 0x03);
    ret |= sc850sl_write_reg(dev, 0x3e87, 0x40);
    ret |= sc850sl_write_reg(dev, 0x3f02, 0x24);
    ret |= sc850sl_write_reg(dev, 0x4424, 0x02);
    ret |= sc850sl_write_reg(dev, 0x4501, 0xb4);
    ret |= sc850sl_write_reg(dev, 0x4503, 0x60);
    ret |= sc850sl_write_reg(dev, 0x4509, 0x20);
    ret |= sc850sl_write_reg(dev, 0x4561, 0x12);
    ret |= sc850sl_write_reg(dev, 0x4800, 0x24);
    ret |= sc850sl_write_reg(dev, 0x4837, 0x21);
    ret |= sc850sl_write_reg(dev, 0x4853, 0xf8);
    ret |= sc850sl_write_reg(dev, 0x4900, 0x24);
    ret |= sc850sl_write_reg(dev, 0x4937, 0x21);
    ret |= sc850sl_write_reg(dev, 0x4953, 0xf8);
    ret |= sc850sl_write_reg(dev, 0x5000, 0x0e);
    ret |= sc850sl_write_reg(dev, 0x500f, 0x35);
    ret |= sc850sl_write_reg(dev, 0x5020, 0x00);
    ret |= sc850sl_write_reg(dev, 0x5787, 0x10);
    ret |= sc850sl_write_reg(dev, 0x5788, 0x06);
    ret |= sc850sl_write_reg(dev, 0x5789, 0x00);
    ret |= sc850sl_write_reg(dev, 0x578a, 0x18);
    ret |= sc850sl_write_reg(dev, 0x578b, 0x0c);
    ret |= sc850sl_write_reg(dev, 0x578c, 0x00);
    ret |= sc850sl_write_reg(dev, 0x5790, 0x10);
    ret |= sc850sl_write_reg(dev, 0x5791, 0x06);
    ret |= sc850sl_write_reg(dev, 0x5792, 0x01);
    ret |= sc850sl_write_reg(dev, 0x5793, 0x18);
    ret |= sc850sl_write_reg(dev, 0x5794, 0x0c);
    ret |= sc850sl_write_reg(dev, 0x5795, 0x01);
    ret |= sc850sl_write_reg(dev, 0x5799, 0x06);
    ret |= sc850sl_write_reg(dev, 0x57a2, 0x60);
    ret |= sc850sl_write_reg(dev, 0x59e0, 0xfe);
    ret |= sc850sl_write_reg(dev, 0x59e1, 0x40);
    ret |= sc850sl_write_reg(dev, 0x59e2, 0x38);
    ret |= sc850sl_write_reg(dev, 0x59e3, 0x30);
    ret |= sc850sl_write_reg(dev, 0x59e4, 0x20);
    ret |= sc850sl_write_reg(dev, 0x59e5, 0x38);
    ret |= sc850sl_write_reg(dev, 0x59e6, 0x30);
    ret |= sc850sl_write_reg(dev, 0x59e7, 0x20);
    ret |= sc850sl_write_reg(dev, 0x59e8, 0x3f);
    ret |= sc850sl_write_reg(dev, 0x59e9, 0x38);
    ret |= sc850sl_write_reg(dev, 0x59ea, 0x30);
    ret |= sc850sl_write_reg(dev, 0x59eb, 0x3f);
    ret |= sc850sl_write_reg(dev, 0x59ec, 0x38);
    ret |= sc850sl_write_reg(dev, 0x59ed, 0x30);
    ret |= sc850sl_write_reg(dev, 0x59ee, 0xfe);
    ret |= sc850sl_write_reg(dev, 0x59ef, 0x40);
    ret |= sc850sl_write_reg(dev, 0x59f4, 0x38);
    ret |= sc850sl_write_reg(dev, 0x59f5, 0x30);
    ret |= sc850sl_write_reg(dev, 0x59f6, 0x20);
    ret |= sc850sl_write_reg(dev, 0x59f7, 0x38);
    ret |= sc850sl_write_reg(dev, 0x59f8, 0x30);
    ret |= sc850sl_write_reg(dev, 0x59f9, 0x20);
    ret |= sc850sl_write_reg(dev, 0x59fa, 0x3f);
    ret |= sc850sl_write_reg(dev, 0x59fb, 0x38);
    ret |= sc850sl_write_reg(dev, 0x59fc, 0x30);
    ret |= sc850sl_write_reg(dev, 0x59fd, 0x3f);
    ret |= sc850sl_write_reg(dev, 0x59fe, 0x38);
    ret |= sc850sl_write_reg(dev, 0x59ff, 0x30);
    ret |= sc850sl_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc850sl_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("================================================================================\n");
        SENSOR_PRINT("==== SC850SL_MIPI_27MInput_raw10_4lane_2to1wdr_3840X2160_init success!==========\n");
        SENSOR_PRINT("================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

xmedia_s32 sc850sl_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case SC850SL_8M_12BIT_LINEAR_MODE:
            ret = sc850sl_4lane_linear_3840x2160_init(dev);
            break;
        case SC850SL_5M_12BIT_LINEAR_MODE:
            ret = sc850sl_4lane_linear_2560x1920_init(dev);
            break;
        case SC850SL_4M_12BIT_LINEAR_MODE:
            ret = sc850sl_4lane_linear_2688x1520_init(dev);
            break;
        case SC850SL_8M_10BIT_WDR_MODE:
            ret = sc850sl_4lane_wdr_3840x2160_init(dev);
            break;
        case SC850SL_5M_10BIT_WDR_MODE:
            ret = sc850sl_4lane_wdr_2560x1920_init(dev);
            break;
        case SC850SL_4M_10BIT_WDR_MODE:
            ret = sc850sl_4lane_wdr_2688x1520_init(dev);
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc850sl_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;

    if (g_sc850sl_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_sc850sl_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_sc850sl_i2c_fd[dev]);
        g_sc850sl_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_sc850sl_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc850sl_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_sc850sl_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_sc850sl_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_sc850sl_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = sc850sl_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc850sl_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_sc850sl_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_sc850sl_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc850sl_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[SC850SL_DATA_BYTE];

    if (g_sc850sl_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_sc850sl_i2c_fd[dev], g_sc850sl_i2c_addr[dev], addr, SC850SL_ADDR_BYTE, buf, SC850SL_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //SC850SL_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc850sl_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_sc850sl_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_sc850sl_i2c_fd[dev], g_sc850sl_i2c_addr[dev], buf, SC850SL_ADDR_BYTE + SC850SL_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}
xmedia_s32 sc850sl_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    sc850sl_read_reg(dev, SC850SL_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

    if (mirror_en) {
        mirror_flip_type |= 0x06;
    } else {
        mirror_flip_type &= 0xf9;
    }

    if (flip_en) {
        mirror_flip_type |= 0x60;
    } else {
        mirror_flip_type &= 0x9f;
    }

    ret = sc850sl_write_reg(dev, SC850SL_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc850sl_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
