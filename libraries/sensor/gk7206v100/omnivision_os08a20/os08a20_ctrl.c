#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "os08a20.h"
#include "os08a20_ctrl.h"

#ifdef FPGA
#define OS08A20_HTS_LINEAR 0x1200 //2.22x
#define OS08A20_HTS_WDR    0xc24  //3x
#define OS08A20_VTS        0xfa0  //1.73x
#else
#define OS08A20_HTS_LINEAR 0x818
#define OS08A20_HTS_WDR    0x40c
#define OS08A20_VTS        0x90a
#endif

#define OS08A20_REG_ADDR_MIRROR 0x3821
#define OS08A20_REG_ADDR_FLIP   0x3820

SENSOR_PRIORITY_DATA static xmedia_s32 g_os08a20_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
SENSOR_PRIORITY_DATA static xmedia_s32 g_os08a20_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

SENSOR_PRIORITY_FUNC static void os08a20_delay_ms(int ms)
{
    sleep_us(ms * 1000);
}

#ifdef OS08A20_BIT_WIDTH_10
SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_linear_comm_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_write_reg(dev, 0x0103, 0x01);
    ret |= os08a20_write_reg(dev, 0x0303, 0x01);
    ret |= os08a20_write_reg(dev, 0x0305, 0x5a);
    ret |= os08a20_write_reg(dev, 0x0306, 0x00);
    ret |= os08a20_write_reg(dev, 0x0308, 0x03);
    ret |= os08a20_write_reg(dev, 0x0309, 0x04);
    ret |= os08a20_write_reg(dev, 0x032a, 0x00);
    ret |= os08a20_write_reg(dev, 0x300f, 0x11);
    ret |= os08a20_write_reg(dev, 0x3010, 0x01);
    ret |= os08a20_write_reg(dev, 0x3011, 0x04);
    ret |= os08a20_write_reg(dev, 0x3012, 0x41);
    ret |= os08a20_write_reg(dev, 0x3016, 0xf0);
    ret |= os08a20_write_reg(dev, 0x301e, 0x98);
    ret |= os08a20_write_reg(dev, 0x3031, 0xa9);
    ret |= os08a20_write_reg(dev, 0x3103, 0x92);
    ret |= os08a20_write_reg(dev, 0x3104, 0x01);
    ret |= os08a20_write_reg(dev, 0x3106, 0x10);
    ret |= os08a20_write_reg(dev, 0x3400, 0x04);
    ret |= os08a20_write_reg(dev, 0x3025, 0x03);
    ret |= os08a20_write_reg(dev, 0x3425, 0x01);
    ret |= os08a20_write_reg(dev, 0x3428, 0x01);
    ret |= os08a20_write_reg(dev, 0x3406, 0x08);
    ret |= os08a20_write_reg(dev, 0x3408, 0x03);
    ret |= os08a20_write_reg(dev, 0x340c, 0xff);
    ret |= os08a20_write_reg(dev, 0x340d, 0xff);
    ret |= os08a20_write_reg(dev, 0x031e, 0x09);
    ret |= os08a20_write_reg(dev, 0x3501, 0x08);
    ret |= os08a20_write_reg(dev, 0x3502, 0xe5);
    ret |= os08a20_write_reg(dev, 0x3505, 0x83);
    ret |= os08a20_write_reg(dev, 0x3508, 0x00);
    ret |= os08a20_write_reg(dev, 0x3509, 0x80);
    ret |= os08a20_write_reg(dev, 0x350a, 0x04);
    ret |= os08a20_write_reg(dev, 0x350b, 0x00);
    ret |= os08a20_write_reg(dev, 0x350c, 0x00);
    ret |= os08a20_write_reg(dev, 0x350d, 0x80);
    ret |= os08a20_write_reg(dev, 0x350e, 0x04);
    ret |= os08a20_write_reg(dev, 0x350f, 0x00);
    ret |= os08a20_write_reg(dev, 0x3600, 0x00);
    ret |= os08a20_write_reg(dev, 0x3603, 0x2c);
    ret |= os08a20_write_reg(dev, 0x3605, 0x50);
    ret |= os08a20_write_reg(dev, 0x3609, 0xb5);
    ret |= os08a20_write_reg(dev, 0x3610, 0x39);
    ret |= os08a20_write_reg(dev, 0x360c, 0x01);
    ret |= os08a20_write_reg(dev, 0x3628, 0xa4);
    ret |= os08a20_write_reg(dev, 0x362d, 0x10);
    ret |= os08a20_write_reg(dev, 0x3660, 0x43);
    ret |= os08a20_write_reg(dev, 0x3661, 0x06);
    ret |= os08a20_write_reg(dev, 0x3662, 0x00);
    ret |= os08a20_write_reg(dev, 0x3663, 0x28);
    ret |= os08a20_write_reg(dev, 0x3664, 0x0d);
    ret |= os08a20_write_reg(dev, 0x366a, 0x38);
    ret |= os08a20_write_reg(dev, 0x366b, 0xa0);
    ret |= os08a20_write_reg(dev, 0x366d, 0x00);
    ret |= os08a20_write_reg(dev, 0x366e, 0x00);
    ret |= os08a20_write_reg(dev, 0x3680, 0x00);
    ret |= os08a20_write_reg(dev, 0x36c0, 0x00);
    ret |= os08a20_write_reg(dev, 0x3701, 0x02);
    ret |= os08a20_write_reg(dev, 0x373b, 0x02);
    ret |= os08a20_write_reg(dev, 0x373c, 0x02);
    ret |= os08a20_write_reg(dev, 0x3736, 0x02);
    ret |= os08a20_write_reg(dev, 0x3737, 0x02);
    ret |= os08a20_write_reg(dev, 0x3705, 0x00);
    ret |= os08a20_write_reg(dev, 0x3706, 0x39);
    ret |= os08a20_write_reg(dev, 0x370a, 0x00);
    ret |= os08a20_write_reg(dev, 0x370b, 0x98);
    ret |= os08a20_write_reg(dev, 0x3709, 0x49);
    ret |= os08a20_write_reg(dev, 0x3714, 0x21);
    ret |= os08a20_write_reg(dev, 0x371c, 0x00);
    ret |= os08a20_write_reg(dev, 0x371d, 0x08);
    ret |= os08a20_write_reg(dev, 0x3740, 0x1b);
    ret |= os08a20_write_reg(dev, 0x3741, 0x04);
    ret |= os08a20_write_reg(dev, 0x375e, 0x0b);
    ret |= os08a20_write_reg(dev, 0x3760, 0x10);
    ret |= os08a20_write_reg(dev, 0x3776, 0x10);
    ret |= os08a20_write_reg(dev, 0x3781, 0x02);
    ret |= os08a20_write_reg(dev, 0x3782, 0x04);
    ret |= os08a20_write_reg(dev, 0x3783, 0x02);
    ret |= os08a20_write_reg(dev, 0x3784, 0x08);
    ret |= os08a20_write_reg(dev, 0x3785, 0x08);
    ret |= os08a20_write_reg(dev, 0x3788, 0x01);
    ret |= os08a20_write_reg(dev, 0x3789, 0x01);
    ret |= os08a20_write_reg(dev, 0x3797, 0x04);
    ret |= os08a20_write_reg(dev, 0x3762, 0x11);
    ret |= os08a20_write_reg(dev, 0x380c, 0x08);
    ret |= os08a20_write_reg(dev, 0x380d, 0x18);
    ret |= os08a20_write_reg(dev, 0x380e, 0x09);//VTS
    ret |= os08a20_write_reg(dev, 0x380f, 0x0a);
    ret |= os08a20_write_reg(dev, 0x3813, 0x10);
    ret |= os08a20_write_reg(dev, 0x3814, 0x01);
    ret |= os08a20_write_reg(dev, 0x3815, 0x01);
    ret |= os08a20_write_reg(dev, 0x3816, 0x01);
    ret |= os08a20_write_reg(dev, 0x3817, 0x01);
    ret |= os08a20_write_reg(dev, 0x381c, 0x00);
    ret |= os08a20_write_reg(dev, 0x3820, 0x00);
    ret |= os08a20_write_reg(dev, 0x3821, 0x04);
    ret |= os08a20_write_reg(dev, 0x3823, 0x08);
    ret |= os08a20_write_reg(dev, 0x3826, 0x00);
    ret |= os08a20_write_reg(dev, 0x3827, 0x08);
    ret |= os08a20_write_reg(dev, 0x382d, 0x08);
    ret |= os08a20_write_reg(dev, 0x3832, 0x02);
    ret |= os08a20_write_reg(dev, 0x3833, 0x00);
    ret |= os08a20_write_reg(dev, 0x383c, 0x48);
    ret |= os08a20_write_reg(dev, 0x383d, 0xff);
    ret |= os08a20_write_reg(dev, 0x3d85, 0x0b);
    ret |= os08a20_write_reg(dev, 0x3d84, 0x40);
    ret |= os08a20_write_reg(dev, 0x3d8c, 0x63);
    ret |= os08a20_write_reg(dev, 0x3d8d, 0xd7);
    ret |= os08a20_write_reg(dev, 0x4000, 0xf8);
    ret |= os08a20_write_reg(dev, 0x4001, 0x2b);
    ret |= os08a20_write_reg(dev, 0x4004, 0x00);
    ret |= os08a20_write_reg(dev, 0x4005, 0x40);
    ret |= os08a20_write_reg(dev, 0x400a, 0x01);
    ret |= os08a20_write_reg(dev, 0x400f, 0xa0);
    ret |= os08a20_write_reg(dev, 0x4010, 0x12);
    ret |= os08a20_write_reg(dev, 0x4018, 0x00);
    ret |= os08a20_write_reg(dev, 0x4008, 0x02);
    ret |= os08a20_write_reg(dev, 0x4009, 0x0d);
    ret |= os08a20_write_reg(dev, 0x401a, 0x58);
    ret |= os08a20_write_reg(dev, 0x4050, 0x00);
    ret |= os08a20_write_reg(dev, 0x4051, 0x01);
    ret |= os08a20_write_reg(dev, 0x4028, 0x2f);
    ret |= os08a20_write_reg(dev, 0x4052, 0x00);
    ret |= os08a20_write_reg(dev, 0x4053, 0x80);
    ret |= os08a20_write_reg(dev, 0x4054, 0x00);
    ret |= os08a20_write_reg(dev, 0x4055, 0x80);
    ret |= os08a20_write_reg(dev, 0x4056, 0x00);
    ret |= os08a20_write_reg(dev, 0x4057, 0x80);
    ret |= os08a20_write_reg(dev, 0x4058, 0x00);
    ret |= os08a20_write_reg(dev, 0x4059, 0x80);
    ret |= os08a20_write_reg(dev, 0x430b, 0xff);
    ret |= os08a20_write_reg(dev, 0x430c, 0xff);
    ret |= os08a20_write_reg(dev, 0x430d, 0x00);
    ret |= os08a20_write_reg(dev, 0x430e, 0x00);
    ret |= os08a20_write_reg(dev, 0x4501, 0x18);
    ret |= os08a20_write_reg(dev, 0x4502, 0x00);
    ret |= os08a20_write_reg(dev, 0x4643, 0x00);
    ret |= os08a20_write_reg(dev, 0x4640, 0x01);
    ret |= os08a20_write_reg(dev, 0x4641, 0x04);
    ret |= os08a20_write_reg(dev, 0x4800, 0x64);
    ret |= os08a20_write_reg(dev, 0x4809, 0x2b);
    ret |= os08a20_write_reg(dev, 0x4813, 0x90);
    ret |= os08a20_write_reg(dev, 0x4817, 0x04);
    ret |= os08a20_write_reg(dev, 0x4833, 0x18);
    ret |= os08a20_write_reg(dev, 0x4837, 0x0b);
    ret |= os08a20_write_reg(dev, 0x483b, 0x00);
    ret |= os08a20_write_reg(dev, 0x484b, 0x03);
    ret |= os08a20_write_reg(dev, 0x4850, 0x7c);
    ret |= os08a20_write_reg(dev, 0x4852, 0x06);
    ret |= os08a20_write_reg(dev, 0x4856, 0x58);
    ret |= os08a20_write_reg(dev, 0x4857, 0xaa);
    ret |= os08a20_write_reg(dev, 0x4862, 0x0a);
    ret |= os08a20_write_reg(dev, 0x4869, 0x18);
    ret |= os08a20_write_reg(dev, 0x486a, 0xaa);
    ret |= os08a20_write_reg(dev, 0x486e, 0x03);
    ret |= os08a20_write_reg(dev, 0x486f, 0x55);
    ret |= os08a20_write_reg(dev, 0x4875, 0xf0);
    ret |= os08a20_write_reg(dev, 0x5000, 0x89);
    ret |= os08a20_write_reg(dev, 0x5001, 0x42);
    ret |= os08a20_write_reg(dev, 0x5004, 0x40);
    ret |= os08a20_write_reg(dev, 0x5005, 0x00);
    ret |= os08a20_write_reg(dev, 0x5180, 0x00);
    ret |= os08a20_write_reg(dev, 0x5181, 0x10);
    ret |= os08a20_write_reg(dev, 0x580b, 0x03);
    ret |= os08a20_write_reg(dev, 0x4d00, 0x03);
    ret |= os08a20_write_reg(dev, 0x4d01, 0xc9);
    ret |= os08a20_write_reg(dev, 0x4d02, 0xbc);
    ret |= os08a20_write_reg(dev, 0x4d03, 0xc6);
    ret |= os08a20_write_reg(dev, 0x4d04, 0x4a);
    ret |= os08a20_write_reg(dev, 0x4d05, 0x25);
    ret |= os08a20_write_reg(dev, 0x4700, 0x2b);
    ret |= os08a20_write_reg(dev, 0x4e00, 0x2b);
    ret |= os08a20_write_reg(dev, 0x3501, 0x09);
    ret |= os08a20_write_reg(dev, 0x3502, 0x01);
    ret |= os08a20_write_reg(dev, 0x4028, 0x4f);
    ret |= os08a20_write_reg(dev, 0x4029, 0x1f);
    ret |= os08a20_write_reg(dev, 0x402a, 0x7f);
    ret |= os08a20_write_reg(dev, 0x402b, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_4lane_linear_3840x2160_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_linear_comm_init(dev);
    ret |= os08a20_write_reg(dev, 0x3800, 0x00);//X_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3801, 0x00);
    ret |= os08a20_write_reg(dev, 0x3802, 0x00);//Y_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3803, 0x0c);
    ret |= os08a20_write_reg(dev, 0x3804, 0x0e);//X_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3805, 0xff);
    ret |= os08a20_write_reg(dev, 0x3806, 0x08);//Y_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3807, 0x7b);
    ret |= os08a20_write_reg(dev, 0x3808, 0x0f);//X_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x3809, 0x00);
    ret |= os08a20_write_reg(dev, 0x380a, 0x08);//Y_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x380b, 0x70);

    os08a20_delay_ms(5);  // delay 5 ms
    ret |= os08a20_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("=== OS08A20_24MInput_MIPI_4lane_10bit_3840x2160_30fps init success!==============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_4lane_linear_2560x1920_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_linear_comm_init(dev);
    ret |= os08a20_write_reg(dev, 0x3800, 0x00);//X_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3801, 0x00);
    ret |= os08a20_write_reg(dev, 0x3802, 0x00);//Y_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3803, 0x0c);
    ret |= os08a20_write_reg(dev, 0x3804, 0x09);//X_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3805, 0xff);
    ret |= os08a20_write_reg(dev, 0x3806, 0x07);//Y_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3807, 0x8b);
    ret |= os08a20_write_reg(dev, 0x3808, 0x0a);//X_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x3809, 0x00);
    ret |= os08a20_write_reg(dev, 0x380a, 0x07);//Y_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x380b, 0x80);

    os08a20_delay_ms(5);  // delay 5 ms
    ret |= os08a20_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("=== OS08A20_24MInput_MIPI_4lane_10bit_2560x1920_30fps init success!==============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_4lane_linear_2560x1440_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_linear_comm_init(dev);
    ret |= os08a20_write_reg(dev, 0x3800, 0x00); //X_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3801, 0x00);
    ret |= os08a20_write_reg(dev, 0x3804, 0x09); //X_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3805, 0xff);

    ret |= os08a20_write_reg(dev, 0x3802, 0x00); //Y_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3803, 0x0c);
    ret |= os08a20_write_reg(dev, 0x3806, 0x05); //Y_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3807, 0xab);

    ret |= os08a20_write_reg(dev, 0x3808, 0x0a); //X_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x3809, 0x00);
    ret |= os08a20_write_reg(dev, 0x380a, 0x05); //Y_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x380b, 0xa0);

    os08a20_delay_ms(5);  // delay 5 ms
    ret |= os08a20_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("=== OS08A20_24MInput_MIPI_4lane_10bit_2560x1440_30fps init success!==============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_4lane_linear_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_linear_comm_init(dev);
    ret |= os08a20_write_reg(dev, 0x3800, 0x00); //X_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3801, 0x00);
    ret |= os08a20_write_reg(dev, 0x3804, 0x0a); //X_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3805, 0x7f);

    ret |= os08a20_write_reg(dev, 0x3802, 0x00); //Y_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3803, 0x0c);
    ret |= os08a20_write_reg(dev, 0x3806, 0x05); //Y_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3807, 0xfb);

    ret |= os08a20_write_reg(dev, 0x3808, 0x0a); //X_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x3809, 0x80);
    ret |= os08a20_write_reg(dev, 0x380a, 0x05); //Y_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x380b, 0xf0);

    os08a20_delay_ms(5);  // delay 5 ms
    ret |= os08a20_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("=== OS08A20_24MInput_MIPI_4lane_10bit_2688x1520_30fps init success!==============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_4lane_linear_2048x1536_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_linear_comm_init(dev);
    ret |= os08a20_write_reg(dev, 0x3800, 0x00); //X_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3801, 0x00);
    ret |= os08a20_write_reg(dev, 0x3804, 0x07); //X_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3805, 0xff);

    ret |= os08a20_write_reg(dev, 0x3802, 0x00); //Y_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3803, 0x0c);
    ret |= os08a20_write_reg(dev, 0x3806, 0x06); //Y_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3807, 0x0B);

    ret |= os08a20_write_reg(dev, 0x3808, 0x08); //X_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x3809, 0x00);
    ret |= os08a20_write_reg(dev, 0x380a, 0x06); //Y_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x380b, 0x00);

    os08a20_delay_ms(5);  // delay 5 ms
    ret |= os08a20_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("=== OS08A20_24MInput_MIPI_4lane_10bit_2048x1536_30fps init success!==============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_4lane_linear_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_linear_comm_init(dev);

    //binning mode
    ret |= os08a20_write_reg(dev, 0x0305, 0x32);
    ret |= os08a20_write_reg(dev, 0x3501, 0x04);
    ret |= os08a20_write_reg(dev, 0x3502, 0x62);
    ret |= os08a20_write_reg(dev, 0x3600, 0x09);
    ret |= os08a20_write_reg(dev, 0x3714, 0x22);
    ret |= os08a20_write_reg(dev, 0x3800, 0x00);
    ret |= os08a20_write_reg(dev, 0x3801, 0x00);
    ret |= os08a20_write_reg(dev, 0x3802, 0x00);
    ret |= os08a20_write_reg(dev, 0x3803, 0x0c);
    ret |= os08a20_write_reg(dev, 0x3804, 0x0e);
    ret |= os08a20_write_reg(dev, 0x3805, 0xff);
    ret |= os08a20_write_reg(dev, 0x3806, 0x08);
    ret |= os08a20_write_reg(dev, 0x3807, 0x6f);
    ret |= os08a20_write_reg(dev, 0x3808, 0x07);
    ret |= os08a20_write_reg(dev, 0x3809, 0x80);
    ret |= os08a20_write_reg(dev, 0x380a, 0x04);
    ret |= os08a20_write_reg(dev, 0x380b, 0x38);
    ret |= os08a20_write_reg(dev, 0x380c, 0x04);
    ret |= os08a20_write_reg(dev, 0x380d, 0x0c);
    ret |= os08a20_write_reg(dev, 0x380e, 0x12);
    ret |= os08a20_write_reg(dev, 0x380f, 0x14);
    ret |= os08a20_write_reg(dev, 0x3813, 0x08);
    ret |= os08a20_write_reg(dev, 0x3814, 0x03);
    ret |= os08a20_write_reg(dev, 0x3815, 0x01);
    ret |= os08a20_write_reg(dev, 0x3816, 0x03);
    ret |= os08a20_write_reg(dev, 0x3817, 0x01);
    ret |= os08a20_write_reg(dev, 0x381c, 0x00);
    ret |= os08a20_write_reg(dev, 0x3820, 0x01);
    ret |= os08a20_write_reg(dev, 0x3821, 0x05);
    ret |= os08a20_write_reg(dev, 0x4009, 0x05);
    ret |= os08a20_write_reg(dev, 0x4501, 0x98);
    ret |= os08a20_write_reg(dev, 0x4837, 0x14);
    ret |= os08a20_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("=== OS08A20_24MInput_MIPI_4lane_10bit_1920x1080_30fps init success!==============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}
#else
SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_linear_comm_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_write_reg(dev, 0x0100, 0x00);
    ret |= os08a20_write_reg(dev, 0x0103, 0x01);
    ret |= os08a20_write_reg(dev, 0x0303, 0x01);
    ret |= os08a20_write_reg(dev, 0x0305, 0x5a);
    ret |= os08a20_write_reg(dev, 0x0306, 0x00);
    ret |= os08a20_write_reg(dev, 0x0308, 0x03);//X_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x0309, 0x04);
    ret |= os08a20_write_reg(dev, 0x032a, 0x00);
    ret |= os08a20_write_reg(dev, 0x300f, 0x11);
    ret |= os08a20_write_reg(dev, 0x3010, 0x01);
    ret |= os08a20_write_reg(dev, 0x3011, 0x04);
    ret |= os08a20_write_reg(dev, 0x3012, 0x41);
    ret |= os08a20_write_reg(dev, 0x3016, 0xf0);
    ret |= os08a20_write_reg(dev, 0x301e, 0x98);
    ret |= os08a20_write_reg(dev, 0x3031, 0xa9);
    ret |= os08a20_write_reg(dev, 0x3103, 0x92);
    ret |= os08a20_write_reg(dev, 0x3104, 0x01);
    ret |= os08a20_write_reg(dev, 0x3106, 0x10);
    ret |= os08a20_write_reg(dev, 0x3400, 0x04);
    ret |= os08a20_write_reg(dev, 0x3025, 0x03);
    ret |= os08a20_write_reg(dev, 0x3425, 0x01);
    ret |= os08a20_write_reg(dev, 0x3428, 0x01);
    ret |= os08a20_write_reg(dev, 0x3406, 0x08);
    ret |= os08a20_write_reg(dev, 0x3408, 0x03);
    ret |= os08a20_write_reg(dev, 0x340c, 0xff);
    ret |= os08a20_write_reg(dev, 0x340d, 0xff);
    ret |= os08a20_write_reg(dev, 0x031e, 0x0a);
    ret |= os08a20_write_reg(dev, 0x3501, 0x08);
    ret |= os08a20_write_reg(dev, 0x3502, 0xe5);
    ret |= os08a20_write_reg(dev, 0x3505, 0x83);
    ret |= os08a20_write_reg(dev, 0x3508, 0x00);
    ret |= os08a20_write_reg(dev, 0x3509, 0x80);
    ret |= os08a20_write_reg(dev, 0x350a, 0x04);
    ret |= os08a20_write_reg(dev, 0x350b, 0x00);
    ret |= os08a20_write_reg(dev, 0x350c, 0x00);
    ret |= os08a20_write_reg(dev, 0x350d, 0x80);
    ret |= os08a20_write_reg(dev, 0x350e, 0x04);
    ret |= os08a20_write_reg(dev, 0x350f, 0x00);
    ret |= os08a20_write_reg(dev, 0x3600, 0x00);
    ret |= os08a20_write_reg(dev, 0x3603, 0x2c);
    ret |= os08a20_write_reg(dev, 0x3605, 0x50);
    ret |= os08a20_write_reg(dev, 0x3609, 0xdb);
    ret |= os08a20_write_reg(dev, 0x3610, 0x39);
    ret |= os08a20_write_reg(dev, 0x360c, 0x01);
    ret |= os08a20_write_reg(dev, 0x3628, 0xa4);
    ret |= os08a20_write_reg(dev, 0x362d, 0x10);
    ret |= os08a20_write_reg(dev, 0x3660, 0xd3);
    ret |= os08a20_write_reg(dev, 0x3661, 0x06);
    ret |= os08a20_write_reg(dev, 0x3662, 0x00);
    ret |= os08a20_write_reg(dev, 0x3663, 0x28);
    ret |= os08a20_write_reg(dev, 0x3664, 0x0d);
    ret |= os08a20_write_reg(dev, 0x366a, 0x38);
    ret |= os08a20_write_reg(dev, 0x366b, 0xa0);
    ret |= os08a20_write_reg(dev, 0x366d, 0x00);
    ret |= os08a20_write_reg(dev, 0x366e, 0x00);
    ret |= os08a20_write_reg(dev, 0x3680, 0x00);
    ret |= os08a20_write_reg(dev, 0x36c0, 0x00);
    ret |= os08a20_write_reg(dev, 0x3701, 0x02);
    ret |= os08a20_write_reg(dev, 0x373b, 0x02);
    ret |= os08a20_write_reg(dev, 0x373c, 0x02);
    ret |= os08a20_write_reg(dev, 0x3736, 0x02);
    ret |= os08a20_write_reg(dev, 0x3737, 0x02);
    ret |= os08a20_write_reg(dev, 0x3705, 0x00);
    ret |= os08a20_write_reg(dev, 0x3706, 0x72);
    ret |= os08a20_write_reg(dev, 0x370a, 0x01);
    ret |= os08a20_write_reg(dev, 0x370b, 0x30);
    ret |= os08a20_write_reg(dev, 0x3709, 0x48);
    ret |= os08a20_write_reg(dev, 0x3714, 0x21);
    ret |= os08a20_write_reg(dev, 0x371c, 0x00);
    ret |= os08a20_write_reg(dev, 0x371d, 0x08);
    ret |= os08a20_write_reg(dev, 0x3740, 0x1b);
    ret |= os08a20_write_reg(dev, 0x3741, 0x04);
    ret |= os08a20_write_reg(dev, 0x375e, 0x0b);
    ret |= os08a20_write_reg(dev, 0x3760, 0x10);
    ret |= os08a20_write_reg(dev, 0x3776, 0x10);
    ret |= os08a20_write_reg(dev, 0x3781, 0x02);
    ret |= os08a20_write_reg(dev, 0x3782, 0x04);
    ret |= os08a20_write_reg(dev, 0x3783, 0x02);
    ret |= os08a20_write_reg(dev, 0x3784, 0x08);
    ret |= os08a20_write_reg(dev, 0x3785, 0x08);
    ret |= os08a20_write_reg(dev, 0x3788, 0x01);
    ret |= os08a20_write_reg(dev, 0x3789, 0x01);
    ret |= os08a20_write_reg(dev, 0x3797, 0x04);
    ret |= os08a20_write_reg(dev, 0x3762, 0x11);
    ret |= os08a20_write_reg(dev, 0x380c, SENSOR_HIGH_8BITS(OS08A20_HTS_LINEAR));   // HTS 0x0818->0x1194->0xc24
    ret |= os08a20_write_reg(dev, 0x380d, SENSOR_LOW_8BITS(OS08A20_HTS_LINEAR));
    ret |= os08a20_write_reg(dev, 0x380e, SENSOR_HIGH_8BITS(OS08A20_VTS));          // VTS 0x090a->0xfa0
    ret |= os08a20_write_reg(dev, 0x380f, SENSOR_LOW_8BITS(OS08A20_VTS));
    ret |= os08a20_write_reg(dev, 0x3813, 0x16); // y windows offset 0x0-->0x16
    ret |= os08a20_write_reg(dev, 0x3814, 0x01);
    ret |= os08a20_write_reg(dev, 0x3815, 0x01);
    ret |= os08a20_write_reg(dev, 0x3816, 0x01);
    ret |= os08a20_write_reg(dev, 0x3817, 0x01);
    ret |= os08a20_write_reg(dev, 0x381c, 0x00);
    ret |= os08a20_write_reg(dev, 0x3820, 0x00);
    ret |= os08a20_write_reg(dev, 0x3821, 0x04);
    ret |= os08a20_write_reg(dev, 0x3823, 0x08);
    ret |= os08a20_write_reg(dev, 0x3826, 0x00);
    ret |= os08a20_write_reg(dev, 0x3827, 0x08);
    ret |= os08a20_write_reg(dev, 0x382d, 0x08);
    ret |= os08a20_write_reg(dev, 0x3832, 0x02);
    ret |= os08a20_write_reg(dev, 0x3833, 0x00);
    ret |= os08a20_write_reg(dev, 0x383c, 0x48);
    ret |= os08a20_write_reg(dev, 0x383d, 0xff);
    ret |= os08a20_write_reg(dev, 0x3d85, 0x0b);
    ret |= os08a20_write_reg(dev, 0x3d84, 0x40);
    ret |= os08a20_write_reg(dev, 0x3d8c, 0x63);
    ret |= os08a20_write_reg(dev, 0x3d8d, 0xd7);
    ret |= os08a20_write_reg(dev, 0x4000, 0xf8);
    ret |= os08a20_write_reg(dev, 0x4001, 0xeb);
    ret |= os08a20_write_reg(dev, 0x4004, 0x01);
    ret |= os08a20_write_reg(dev, 0x4005, 0x00);
    ret |= os08a20_write_reg(dev, 0x400a, 0x01);
    ret |= os08a20_write_reg(dev, 0x400f, 0xa0);
    ret |= os08a20_write_reg(dev, 0x4010, 0x12);
    ret |= os08a20_write_reg(dev, 0x4018, 0x00);
    ret |= os08a20_write_reg(dev, 0x4008, 0x02);
    ret |= os08a20_write_reg(dev, 0x4009, 0x0d);
    ret |= os08a20_write_reg(dev, 0x401a, 0x58);
    ret |= os08a20_write_reg(dev, 0x4050, 0x00);
    ret |= os08a20_write_reg(dev, 0x4051, 0x01);
    ret |= os08a20_write_reg(dev, 0x4028, 0x2f);
    ret |= os08a20_write_reg(dev, 0x4052, 0x00);
    ret |= os08a20_write_reg(dev, 0x4053, 0x80);
    ret |= os08a20_write_reg(dev, 0x4054, 0x00);
    ret |= os08a20_write_reg(dev, 0x4055, 0x80);
    ret |= os08a20_write_reg(dev, 0x4056, 0x00);
    ret |= os08a20_write_reg(dev, 0x4057, 0x80);
    ret |= os08a20_write_reg(dev, 0x4058, 0x00);
    ret |= os08a20_write_reg(dev, 0x4059, 0x80);
    ret |= os08a20_write_reg(dev, 0x430b, 0xff);
    ret |= os08a20_write_reg(dev, 0x430c, 0xff);
    ret |= os08a20_write_reg(dev, 0x430d, 0x00);
    ret |= os08a20_write_reg(dev, 0x430e, 0x00);
    ret |= os08a20_write_reg(dev, 0x4501, 0x18);
    ret |= os08a20_write_reg(dev, 0x4502, 0x00);
    ret |= os08a20_write_reg(dev, 0x4600, 0x00);
    ret |= os08a20_write_reg(dev, 0x4601, 0x20);
    ret |= os08a20_write_reg(dev, 0x4603, 0x01);
    ret |= os08a20_write_reg(dev, 0x4643, 0x00);
    ret |= os08a20_write_reg(dev, 0x4640, 0x01);
    ret |= os08a20_write_reg(dev, 0x4641, 0x04);
    ret |= os08a20_write_reg(dev, 0x4800, 0x44);    // 非连续模式：0x64->0x44
    ret |= os08a20_write_reg(dev, 0x4809, 0x2b);
    ret |= os08a20_write_reg(dev, 0x4813, 0x90);
    ret |= os08a20_write_reg(dev, 0x4817, 0x04);
    ret |= os08a20_write_reg(dev, 0x4833, 0x18);
    ret |= os08a20_write_reg(dev, 0x4837, 0x0b);
    ret |= os08a20_write_reg(dev, 0x483b, 0x00);
    ret |= os08a20_write_reg(dev, 0x484b, 0x03);
    ret |= os08a20_write_reg(dev, 0x4850, 0x7c);
    ret |= os08a20_write_reg(dev, 0x4852, 0x06);
    ret |= os08a20_write_reg(dev, 0x4856, 0x58);
    ret |= os08a20_write_reg(dev, 0x4857, 0xaa);
    ret |= os08a20_write_reg(dev, 0x4862, 0x0a);
    ret |= os08a20_write_reg(dev, 0x4869, 0x18);
    ret |= os08a20_write_reg(dev, 0x486a, 0xaa);
    ret |= os08a20_write_reg(dev, 0x486e, 0x03);
    ret |= os08a20_write_reg(dev, 0x486f, 0x55);
    ret |= os08a20_write_reg(dev, 0x4875, 0xf0);
    ret |= os08a20_write_reg(dev, 0x5000, 0x89);
    ret |= os08a20_write_reg(dev, 0x5001, 0x40);
    ret |= os08a20_write_reg(dev, 0x5004, 0x40);
    ret |= os08a20_write_reg(dev, 0x5005, 0x00);
    ret |= os08a20_write_reg(dev, 0x5180, 0x00);
    ret |= os08a20_write_reg(dev, 0x5181, 0x10);
    ret |= os08a20_write_reg(dev, 0x580b, 0x03);
    ret |= os08a20_write_reg(dev, 0x4d00, 0x03);
    ret |= os08a20_write_reg(dev, 0x4d01, 0xc9);
    ret |= os08a20_write_reg(dev, 0x4d02, 0xbc);
    ret |= os08a20_write_reg(dev, 0x4d03, 0xc6);
    ret |= os08a20_write_reg(dev, 0x4d04, 0x4a);
    ret |= os08a20_write_reg(dev, 0x4d05, 0x25);
    ret |= os08a20_write_reg(dev, 0x4700, 0x2b);
    ret |= os08a20_write_reg(dev, 0x4e00, 0x2b);
    ret |= os08a20_write_reg(dev, 0x3501, 0x09);
    ret |= os08a20_write_reg(dev, 0x3502, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

/*
 * 函数功能: os08a20 MIPI 4lane 2160p线性模式初始化序列 - 3840x2160x30fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_4lane_linear_3840x2160_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_linear_comm_init(dev);
    ret |= os08a20_write_reg(dev, 0x3800, 0x00);//X_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3801, 0x00);
    ret |= os08a20_write_reg(dev, 0x3802, 0x00);//Y_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3803, 0x0c);
    ret |= os08a20_write_reg(dev, 0x3804, 0x0e);//X_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3805, 0xff);
    ret |= os08a20_write_reg(dev, 0x3806, 0x08);//Y_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3807, 0x7b);
    ret |= os08a20_write_reg(dev, 0x3808, 0x0f);//X_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x3809, 0x00);
    ret |= os08a20_write_reg(dev, 0x380a, 0x08);//Y_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x380b, 0x70);

    os08a20_delay_ms(5);  // delay 5 ms
    ret |= os08a20_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("=== OS08A20_24MInput_MIPI_4lane_12bit_3840x2160_30fps init success!==============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_4lane_linear_2560x1920_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_linear_comm_init(dev);
    ret |= os08a20_write_reg(dev, 0x3800, 0x00);//X_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3801, 0x00);
    ret |= os08a20_write_reg(dev, 0x3802, 0x00);//Y_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3803, 0x0c);
    ret |= os08a20_write_reg(dev, 0x3804, 0x09);//X_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3805, 0xff);
    ret |= os08a20_write_reg(dev, 0x3806, 0x07);//Y_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3807, 0x8b);
    ret |= os08a20_write_reg(dev, 0x3808, 0x0a);//X_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x3809, 0x00);
    ret |= os08a20_write_reg(dev, 0x380a, 0x07);//Y_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x380b, 0x80);

    os08a20_delay_ms(5);  // delay 5 ms
    ret |= os08a20_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("=== OS08A20_24MInput_MIPI_4lane_12bit_2560x1920_30fps init success!==============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_4lane_linear_2560x1440_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_linear_comm_init(dev);
    ret |= os08a20_write_reg(dev, 0x3800, 0x00); //X_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3801, 0x00);
    ret |= os08a20_write_reg(dev, 0x3804, 0x09); //X_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3805, 0xff);

    ret |= os08a20_write_reg(dev, 0x3802, 0x00); //Y_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3803, 0x0c);
    ret |= os08a20_write_reg(dev, 0x3806, 0x05); //Y_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3807, 0xab);

    ret |= os08a20_write_reg(dev, 0x3808, 0x0a); //X_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x3809, 0x00);
    ret |= os08a20_write_reg(dev, 0x380a, 0x05); //Y_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x380b, 0xa0);

    os08a20_delay_ms(5);  // delay 5 ms
    ret |= os08a20_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("=== OS08A20_24MInput_MIPI_4lane_12bit_2560x1440_30fps init success!==============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_4lane_linear_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_linear_comm_init(dev);
    ret |= os08a20_write_reg(dev, 0x3800, 0x00); //X_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3801, 0x00);
    ret |= os08a20_write_reg(dev, 0x3804, 0x0a); //X_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3805, 0x7f);

    ret |= os08a20_write_reg(dev, 0x3802, 0x00); //Y_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3803, 0x0c);
    ret |= os08a20_write_reg(dev, 0x3806, 0x05); //Y_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3807, 0xfb);

    ret |= os08a20_write_reg(dev, 0x3808, 0x0a); //X_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x3809, 0x80);
    ret |= os08a20_write_reg(dev, 0x380a, 0x05); //Y_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x380b, 0xf0);

    os08a20_delay_ms(5);  // delay 5 ms
    ret |= os08a20_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("=== OS08A20_24MInput_MIPI_4lane_12bit_2688x1520_30fps init success!==============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_4lane_linear_2048x1536_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_linear_comm_init(dev);
    ret |= os08a20_write_reg(dev, 0x3800, 0x00); //X_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3801, 0x00);
    ret |= os08a20_write_reg(dev, 0x3804, 0x07); //X_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3805, 0xff);

    ret |= os08a20_write_reg(dev, 0x3802, 0x00); //Y_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3803, 0x0c);
    ret |= os08a20_write_reg(dev, 0x3806, 0x06); //Y_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3807, 0x0B);

    ret |= os08a20_write_reg(dev, 0x3808, 0x08); //X_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x3809, 0x00);
    ret |= os08a20_write_reg(dev, 0x380a, 0x06); //Y_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x380b, 0x00);

    os08a20_delay_ms(5);  // delay 5 ms
    ret |= os08a20_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("=== OS08A20_24MInput_MIPI_4lane_12bit_2048x1536_30fps init success!==============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_4lane_linear_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_linear_comm_init(dev);
    ret |= os08a20_write_reg(dev, 0x3800, 0x00);//X_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3801, 0x00);
    ret |= os08a20_write_reg(dev, 0x3802, 0x00);//Y_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3803, 0x0c);
    ret |= os08a20_write_reg(dev, 0x3804, 0x07);//X_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3805, 0x7f);
    ret |= os08a20_write_reg(dev, 0x3806, 0x04);//Y_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3807, 0x43);
    ret |= os08a20_write_reg(dev, 0x3808, 0x07);//X_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x3809, 0x80);
    ret |= os08a20_write_reg(dev, 0x380a, 0x04);//Y_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x380b, 0x38);

    os08a20_delay_ms(5);  // delay 5 ms
    ret |= os08a20_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("=== OS08A20_24MInput_MIPI_4lane_12bit_1920x1080_30fps init success!==============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}
#endif

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_wdr_comm_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_write_reg(dev, 0x0100, 0x00);
    ret |= os08a20_write_reg(dev, 0x0103, 0x01);
    ret |= os08a20_write_reg(dev, 0x0303, 0x01);
    ret |= os08a20_write_reg(dev, 0x0305, 0x5a);
    ret |= os08a20_write_reg(dev, 0x0306, 0x00);
    ret |= os08a20_write_reg(dev, 0x0308, 0x03);
    ret |= os08a20_write_reg(dev, 0x0309, 0x04);
    ret |= os08a20_write_reg(dev, 0x032a, 0x00);
    ret |= os08a20_write_reg(dev, 0x300f, 0x11);
    ret |= os08a20_write_reg(dev, 0x3010, 0x01);
    ret |= os08a20_write_reg(dev, 0x3011, 0x04);
    ret |= os08a20_write_reg(dev, 0x3012, 0x41);
    ret |= os08a20_write_reg(dev, 0x3016, 0xf0);
    ret |= os08a20_write_reg(dev, 0x301e, 0x98);
    ret |= os08a20_write_reg(dev, 0x3031, 0xa9);
    ret |= os08a20_write_reg(dev, 0x3103, 0x92);
    ret |= os08a20_write_reg(dev, 0x3104, 0x01);
    ret |= os08a20_write_reg(dev, 0x3106, 0x10);
    ret |= os08a20_write_reg(dev, 0x340c, 0xff);
    ret |= os08a20_write_reg(dev, 0x340d, 0xff);
    ret |= os08a20_write_reg(dev, 0x031e, 0x09);
    ret |= os08a20_write_reg(dev, 0x3501, 0x08);
    ret |= os08a20_write_reg(dev, 0x3502, 0xe5);
    ret |= os08a20_write_reg(dev, 0x3505, 0x83);
    ret |= os08a20_write_reg(dev, 0x3508, 0x00);
    ret |= os08a20_write_reg(dev, 0x3509, 0x80);
    ret |= os08a20_write_reg(dev, 0x350a, 0x04);
    ret |= os08a20_write_reg(dev, 0x350b, 0x00);
    ret |= os08a20_write_reg(dev, 0x350c, 0x00);
    ret |= os08a20_write_reg(dev, 0x350d, 0x80);
    ret |= os08a20_write_reg(dev, 0x350e, 0x04);
    ret |= os08a20_write_reg(dev, 0x350f, 0x00);
    ret |= os08a20_write_reg(dev, 0x3600, 0x00);
    ret |= os08a20_write_reg(dev, 0x3603, 0x2c);
    ret |= os08a20_write_reg(dev, 0x3605, 0x50);
    ret |= os08a20_write_reg(dev, 0x3609, 0xb5);
    ret |= os08a20_write_reg(dev, 0x3610, 0x39);
    ret |= os08a20_write_reg(dev, 0x360c, 0x01);
    ret |= os08a20_write_reg(dev, 0x3628, 0xa4);
    ret |= os08a20_write_reg(dev, 0x362d, 0x10);
    ret |= os08a20_write_reg(dev, 0x3660, 0x42);
    ret |= os08a20_write_reg(dev, 0x3661, 0x07);
    ret |= os08a20_write_reg(dev, 0x3662, 0x00);
    ret |= os08a20_write_reg(dev, 0x3663, 0x28);
    ret |= os08a20_write_reg(dev, 0x3664, 0x0d);
    ret |= os08a20_write_reg(dev, 0x366a, 0x38);
    ret |= os08a20_write_reg(dev, 0x366b, 0xa0);
    ret |= os08a20_write_reg(dev, 0x366d, 0x00);
    ret |= os08a20_write_reg(dev, 0x366e, 0x00);
    ret |= os08a20_write_reg(dev, 0x3680, 0x00);
    ret |= os08a20_write_reg(dev, 0x36c0, 0x00);
    ret |= os08a20_write_reg(dev, 0x3701, 0x02);
    ret |= os08a20_write_reg(dev, 0x373b, 0x02);
    ret |= os08a20_write_reg(dev, 0x373c, 0x02);
    ret |= os08a20_write_reg(dev, 0x3736, 0x02);
    ret |= os08a20_write_reg(dev, 0x3737, 0x02);
    ret |= os08a20_write_reg(dev, 0x3705, 0x00);
    ret |= os08a20_write_reg(dev, 0x3706, 0x39);
    ret |= os08a20_write_reg(dev, 0x370a, 0x00);
    ret |= os08a20_write_reg(dev, 0x370b, 0x98);
    ret |= os08a20_write_reg(dev, 0x3709, 0x49);
    ret |= os08a20_write_reg(dev, 0x3714, 0x21);
    ret |= os08a20_write_reg(dev, 0x371c, 0x00);
    ret |= os08a20_write_reg(dev, 0x371d, 0x08);
    ret |= os08a20_write_reg(dev, 0x3740, 0x1b);
    ret |= os08a20_write_reg(dev, 0x3741, 0x04);
    ret |= os08a20_write_reg(dev, 0x375e, 0x0b);
    ret |= os08a20_write_reg(dev, 0x3760, 0x10);
    ret |= os08a20_write_reg(dev, 0x3776, 0x10);
    ret |= os08a20_write_reg(dev, 0x3781, 0x02);
    ret |= os08a20_write_reg(dev, 0x3782, 0x04);
    ret |= os08a20_write_reg(dev, 0x3783, 0x02);
    ret |= os08a20_write_reg(dev, 0x3784, 0x08);
    ret |= os08a20_write_reg(dev, 0x3785, 0x08);
    ret |= os08a20_write_reg(dev, 0x3788, 0x01);
    ret |= os08a20_write_reg(dev, 0x3789, 0x01);
    ret |= os08a20_write_reg(dev, 0x3797, 0x04);
    ret |= os08a20_write_reg(dev, 0x3762, 0x11);
    ret |= os08a20_write_reg(dev, 0x380c, SENSOR_HIGH_8BITS(OS08A20_HTS_WDR));    // HTS 0x40c->0x1194->0xc24
    ret |= os08a20_write_reg(dev, 0x380d, SENSOR_LOW_8BITS(OS08A20_HTS_WDR));
    ret |= os08a20_write_reg(dev, 0x380e, SENSOR_HIGH_8BITS(OS08A20_VTS));    //vts 0x90a->0xfa0
    ret |= os08a20_write_reg(dev, 0x380f, SENSOR_LOW_8BITS(OS08A20_VTS));
    ret |= os08a20_write_reg(dev, 0x3813, 0x14);
    ret |= os08a20_write_reg(dev, 0x3814, 0x01);
    ret |= os08a20_write_reg(dev, 0x3815, 0x01);
    ret |= os08a20_write_reg(dev, 0x3816, 0x01);
    ret |= os08a20_write_reg(dev, 0x3817, 0x01);
    ret |= os08a20_write_reg(dev, 0x381c, 0x08);
    ret |= os08a20_write_reg(dev, 0x3820, 0x00);
    ret |= os08a20_write_reg(dev, 0x3821, 0x24);
    ret |= os08a20_write_reg(dev, 0x3823, 0x08);
    ret |= os08a20_write_reg(dev, 0x3826, 0x00);
    ret |= os08a20_write_reg(dev, 0x3827, 0x08);
    ret |= os08a20_write_reg(dev, 0x382d, 0x08);
    ret |= os08a20_write_reg(dev, 0x3832, 0x02);
    ret |= os08a20_write_reg(dev, 0x3833, 0x01);
    ret |= os08a20_write_reg(dev, 0x383c, 0x48);
    ret |= os08a20_write_reg(dev, 0x383d, 0xff);
    ret |= os08a20_write_reg(dev, 0x3d85, 0x0b);
    ret |= os08a20_write_reg(dev, 0x3d84, 0x40);
    ret |= os08a20_write_reg(dev, 0x3d8c, 0x63);
    ret |= os08a20_write_reg(dev, 0x3d8d, 0xd7);
    ret |= os08a20_write_reg(dev, 0x4000, 0xf8);
    ret |= os08a20_write_reg(dev, 0x4001, 0xeb);
    ret |= os08a20_write_reg(dev, 0x4004, 0x00);
    ret |= os08a20_write_reg(dev, 0x4005, 0x40);
    ret |= os08a20_write_reg(dev, 0x400a, 0x01);
    ret |= os08a20_write_reg(dev, 0x400f, 0xa0);
    ret |= os08a20_write_reg(dev, 0x4010, 0x12);
    ret |= os08a20_write_reg(dev, 0x4018, 0x00);
    ret |= os08a20_write_reg(dev, 0x4008, 0x02);
    ret |= os08a20_write_reg(dev, 0x4009, 0x0d);
    ret |= os08a20_write_reg(dev, 0x401a, 0x58);
    ret |= os08a20_write_reg(dev, 0x4050, 0x00);
    ret |= os08a20_write_reg(dev, 0x4051, 0x01);
    ret |= os08a20_write_reg(dev, 0x4028, 0x2f);
    ret |= os08a20_write_reg(dev, 0x4052, 0x00);
    ret |= os08a20_write_reg(dev, 0x4053, 0x80);
    ret |= os08a20_write_reg(dev, 0x4054, 0x00);
    ret |= os08a20_write_reg(dev, 0x4055, 0x80);
    ret |= os08a20_write_reg(dev, 0x4056, 0x00);
    ret |= os08a20_write_reg(dev, 0x4057, 0x80);
    ret |= os08a20_write_reg(dev, 0x4058, 0x00);
    ret |= os08a20_write_reg(dev, 0x4059, 0x80);
    ret |= os08a20_write_reg(dev, 0x430b, 0xff);
    ret |= os08a20_write_reg(dev, 0x430c, 0xff);
    ret |= os08a20_write_reg(dev, 0x430d, 0x00);
    ret |= os08a20_write_reg(dev, 0x430e, 0x00);
    ret |= os08a20_write_reg(dev, 0x4501, 0x18);
    ret |= os08a20_write_reg(dev, 0x4502, 0x00);
    ret |= os08a20_write_reg(dev, 0x4643, 0x00);
    ret |= os08a20_write_reg(dev, 0x4640, 0x01);
    ret |= os08a20_write_reg(dev, 0x4641, 0x04);
    ret |= os08a20_write_reg(dev, 0x4800, 0x44);    // 非连续模式：0x64->0x44
    ret |= os08a20_write_reg(dev, 0x4809, 0x2b);
    ret |= os08a20_write_reg(dev, 0x4813, 0x98);
    ret |= os08a20_write_reg(dev, 0x4817, 0x04);
    ret |= os08a20_write_reg(dev, 0x4833, 0x18);
    ret |= os08a20_write_reg(dev, 0x4837, 0x0b);
    ret |= os08a20_write_reg(dev, 0x483b, 0x00);
    ret |= os08a20_write_reg(dev, 0x484b, 0x03);
    ret |= os08a20_write_reg(dev, 0x4850, 0x7c);
    ret |= os08a20_write_reg(dev, 0x4852, 0x06);
    ret |= os08a20_write_reg(dev, 0x4856, 0x58);
    ret |= os08a20_write_reg(dev, 0x4857, 0xaa);
    ret |= os08a20_write_reg(dev, 0x4862, 0x0a);
    ret |= os08a20_write_reg(dev, 0x4869, 0x18);
    ret |= os08a20_write_reg(dev, 0x486a, 0xaa);
    ret |= os08a20_write_reg(dev, 0x486e, 0x07);
    ret |= os08a20_write_reg(dev, 0x486f, 0x55);
    ret |= os08a20_write_reg(dev, 0x4875, 0xf0);
    ret |= os08a20_write_reg(dev, 0x5000, 0x89);
    ret |= os08a20_write_reg(dev, 0x5001, 0x40);
    ret |= os08a20_write_reg(dev, 0x5004, 0x40);
    ret |= os08a20_write_reg(dev, 0x5005, 0x00);
    ret |= os08a20_write_reg(dev, 0x5180, 0x00);
    ret |= os08a20_write_reg(dev, 0x5181, 0x10);
    ret |= os08a20_write_reg(dev, 0x580b, 0x03);
    ret |= os08a20_write_reg(dev, 0x4d00, 0x03);
    ret |= os08a20_write_reg(dev, 0x4d01, 0xc9);
    ret |= os08a20_write_reg(dev, 0x4d02, 0xbc);
    ret |= os08a20_write_reg(dev, 0x4d03, 0xc6);
    ret |= os08a20_write_reg(dev, 0x4d04, 0x4a);
    ret |= os08a20_write_reg(dev, 0x4d05, 0x25);
    ret |= os08a20_write_reg(dev, 0x4700, 0x2b);
    ret |= os08a20_write_reg(dev, 0x4e00, 0x2b);
    ret |= os08a20_write_reg(dev, 0x3501, 0x08);
    ret |= os08a20_write_reg(dev, 0x3502, 0xe1);
    ret |= os08a20_write_reg(dev, 0x3511, 0x00);
    ret |= os08a20_write_reg(dev, 0x3512, 0x20);
    ret |= os08a20_write_reg(dev, 0x3833, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_4lane_wdr_3840x2160_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_wdr_comm_init(dev);
    ret |= os08a20_write_reg(dev, 0x3800, 0x00);//X_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3801, 0x00);
    ret |= os08a20_write_reg(dev, 0x3802, 0x00);//Y_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3803, 0x0c);
    ret |= os08a20_write_reg(dev, 0x3804, 0x0e);//X_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3805, 0xff);
    ret |= os08a20_write_reg(dev, 0x3806, 0x08);//Y_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3807, 0x7b);
    ret |= os08a20_write_reg(dev, 0x3808, 0x0f);//X_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x3809, 0x00);
    ret |= os08a20_write_reg(dev, 0x380a, 0x08);//Y_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x380b, 0x70);

    os08a20_delay_ms(20);
    ret |= os08a20_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("===  OS08A20_raw10_3840X2160_2to1wdr_30fps_4lane_10bit_24Mclk_init success!======\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_4lane_wdr_2560x1920_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_wdr_comm_init(dev);
    ret |= os08a20_write_reg(dev, 0x3800, 0x00);//X_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3801, 0x00);
    ret |= os08a20_write_reg(dev, 0x3802, 0x00);//Y_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3803, 0x0c);
    ret |= os08a20_write_reg(dev, 0x3804, 0x09);//X_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3805, 0xff);
    ret |= os08a20_write_reg(dev, 0x3806, 0x07);//Y_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3807, 0x8b);
    ret |= os08a20_write_reg(dev, 0x3808, 0x0a);//X_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x3809, 0x00);
    ret |= os08a20_write_reg(dev, 0x380a, 0x07);//Y_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x380b, 0x80);

    os08a20_delay_ms(1);
    ret |= os08a20_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("===  OS08A20_raw10_2560X1920_2to1wdr_30fps_4lane_24Mclk_init success!============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_4lane_wdr_2560x1440_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_wdr_comm_init(dev);
    ret |= os08a20_write_reg(dev, 0x3800, 0x00); //X_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3801, 0x00);
    ret |= os08a20_write_reg(dev, 0x3804, 0x09); //X_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3805, 0xff);

    ret |= os08a20_write_reg(dev, 0x3802, 0x00); //Y_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3803, 0x0c);
    ret |= os08a20_write_reg(dev, 0x3806, 0x05); //Y_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3807, 0xab);

    ret |= os08a20_write_reg(dev, 0x3808, 0x0a); //X_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x3809, 0x00);
    ret |= os08a20_write_reg(dev, 0x380a, 0x05); //Y_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x380b, 0xa0);

    os08a20_delay_ms(5);  // delay 5 ms
    ret |= os08a20_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("===  OS08A20_raw10_2560X1440_2to1wdr_30fps_4lane_24Mclk_init success!============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_4lane_wdr_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_wdr_comm_init(dev);
    ret |= os08a20_write_reg(dev, 0x3800, 0x00); //X_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3801, 0x00);
    ret |= os08a20_write_reg(dev, 0x3804, 0x0a); //X_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3805, 0x7f);

    ret |= os08a20_write_reg(dev, 0x3802, 0x00); //Y_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3803, 0x0c);
    ret |= os08a20_write_reg(dev, 0x3806, 0x05); //Y_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3807, 0xfb);

    ret |= os08a20_write_reg(dev, 0x3808, 0x0a); //X_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x3809, 0x80);
    ret |= os08a20_write_reg(dev, 0x380a, 0x05); //Y_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x380b, 0xf0);

    os08a20_delay_ms(5);  // delay 5 ms
    ret |= os08a20_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("===  OS08A20_raw10_2688X1520_2to1wdr_30fps_4lane_24Mclk_init success!============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_4lane_wdr_2048x1536_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_wdr_comm_init(dev);
    ret |= os08a20_write_reg(dev, 0x3800, 0x00); //X_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3801, 0x00);
    ret |= os08a20_write_reg(dev, 0x3804, 0x07); //X_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3805, 0xff);

    ret |= os08a20_write_reg(dev, 0x3802, 0x00); //Y_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3803, 0x0c);
    ret |= os08a20_write_reg(dev, 0x3806, 0x06); //Y_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3807, 0x0B);

    ret |= os08a20_write_reg(dev, 0x3808, 0x08); //X_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x3809, 0x00);
    ret |= os08a20_write_reg(dev, 0x380a, 0x06); //Y_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x380b, 0x00);

    os08a20_delay_ms(5);  // delay 5 ms
    ret |= os08a20_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("===  OS08A20_raw10_2048X1536_2to1wdr_30fps_4lane_24Mclk_init success!=========== \n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 os08a20_4lane_wdr_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08a20_wdr_comm_init(dev);
    ret |= os08a20_write_reg(dev, 0x3800, 0x00);//X_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3801, 0x00);
    ret |= os08a20_write_reg(dev, 0x3802, 0x00);//Y_ADDR_START
    ret |= os08a20_write_reg(dev, 0x3803, 0x0c);
    ret |= os08a20_write_reg(dev, 0x3804, 0x07);//X_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3805, 0x7f);
    ret |= os08a20_write_reg(dev, 0x3806, 0x04);//Y_ADDR_END
    ret |= os08a20_write_reg(dev, 0x3807, 0x43);
    ret |= os08a20_write_reg(dev, 0x3808, 0x07);//X_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x3809, 0x80);
    ret |= os08a20_write_reg(dev, 0x380a, 0x04);//Y_OUTSIZE
    ret |= os08a20_write_reg(dev, 0x380b, 0x38);

    os08a20_delay_ms(1);
    ret |= os08a20_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("===  OS08A20_raw10_1920X1080_2to1wdr_30fps_4lane_24Mclk_init success!============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC xmedia_s32 os08a20_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case OS08A20_8M_LINEAR_MODE:
            ret = os08a20_4lane_linear_3840x2160_init(dev);
            break;
        case OS08A20_5M_LINEAR_MODE:
            ret = os08a20_4lane_linear_2560x1920_init(dev);
            break;
        case OS08A20_4M_LINEAR_MODE:
            ret = os08a20_4lane_linear_2688x1520_init(dev);
            break;
        case OS08A20_4M_1440P_LINEAR_MODE:
            ret = os08a20_4lane_linear_2560x1440_init(dev);
            break;
        case OS08A20_3M_LINEAR_MODE:
            ret = os08a20_4lane_linear_2048x1536_init(dev);
            break;
        case OS08A20_2M_LINEAR_MODE:
            ret = os08a20_4lane_linear_1920x1080_init(dev);
            break;

        case OS08A20_8M_WDR_MODE:
            ret = os08a20_4lane_wdr_3840x2160_init(dev);
            break;
        case OS08A20_5M_WDR_MODE:
            ret = os08a20_4lane_wdr_2560x1920_init(dev);
            break;
        case OS08A20_4M_WDR_MODE:
            ret = os08a20_4lane_wdr_2688x1520_init(dev);
            break;
        case OS08A20_4M_1440P_WDR_MODE:
            ret = os08a20_4lane_wdr_2560x1440_init(dev);
            break;
        case OS08A20_3M_WDR_MODE:
            ret = os08a20_4lane_wdr_2048x1536_init(dev);
            break;
        case OS08A20_2M_WDR_MODE:
            ret = os08a20_4lane_wdr_1920x1080_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }
    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os08a20_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;

    if (g_os08a20_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_os08a20_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_os08a20_i2c_fd[dev]);
        g_os08a20_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_os08a20_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os08a20_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_os08a20_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_os08a20_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_os08a20_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = os08a20_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os08a20_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_os08a20_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_os08a20_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os08a20_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[OS08A20_DATA_BYTE];

    if (g_os08a20_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_os08a20_i2c_fd[dev], g_os08a20_i2c_addr[dev], addr, OS08A20_ADDR_BYTE, buf, OS08A20_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //OS08A20_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os08a20_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_os08a20_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_os08a20_i2c_fd[dev], g_os08a20_i2c_addr[dev], buf, OS08A20_ADDR_BYTE + OS08A20_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os08a20_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u32 mirror_type;
    xmedia_u32 flip_type;

    os08a20_read_reg(dev, OS08A20_REG_ADDR_MIRROR, &mirror_type);
    os08a20_read_reg(dev, OS08A20_REG_ADDR_FLIP, &flip_type);

    if (mirror_en){
        mirror_type |= 0x04;
    } else {
        mirror_type &= 0xfb;
    }

    if (flip_en){
        flip_type |= 0x04;
    } else {
        flip_type &= 0xfb;
    }

    ret  = os08a20_write_reg(dev, OS08A20_REG_ADDR_MIRROR, mirror_type);
    ret |= os08a20_write_reg(dev, OS08A20_REG_ADDR_FLIP, flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08a20_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}
