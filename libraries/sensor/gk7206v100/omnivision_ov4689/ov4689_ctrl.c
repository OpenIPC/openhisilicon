#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "ov4689.h"
#include "ov4689_ctrl.h"

#define OV4689_REG_ADDR_FLIP   0x3820
#define OV4689_REG_ADDR_MIRROR 0x3821

static xmedia_s32 g_ov4689_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
static xmedia_s32 g_ov4689_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

static void ov4689_delay_ms(xmedia_s32 ms)
{
    usleep(ms * 1000); // 1ms: 1000us
    return;
}

/*
 * 函数功能: ov4689 MIPI 2lane 1520p 10bit线性模式初始化序列 - 2688x1520x30fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 ov4689_4lane_linear_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= ov4689_write_reg(dev, 0x0103, 0x01);
    ov4689_delay_ms(10);
    ret |= ov4689_write_reg(dev, 0x3638, 0x00);
    ret |= ov4689_write_reg(dev, 0x0300, 0x00);
    ret |= ov4689_write_reg(dev, 0x0302, 0x1e);
    ret |= ov4689_write_reg(dev, 0x0303, 0x00);
    ret |= ov4689_write_reg(dev, 0x0304, 0x03);
    ret |= ov4689_write_reg(dev, 0x030b, 0x00);
    ret |= ov4689_write_reg(dev, 0x030d, 0x1e);
    ret |= ov4689_write_reg(dev, 0x030e, 0x04);
    ret |= ov4689_write_reg(dev, 0x030f, 0x01);
    ret |= ov4689_write_reg(dev, 0x0312, 0x01);
    ret |= ov4689_write_reg(dev, 0x031e, 0x00);
    ret |= ov4689_write_reg(dev, 0x3000, 0x20);
    ret |= ov4689_write_reg(dev, 0x3002, 0x00);
    ret |= ov4689_write_reg(dev, 0x3018, 0x72);
    ret |= ov4689_write_reg(dev, 0x3020, 0x93);
    ret |= ov4689_write_reg(dev, 0x3021, 0x03);
    ret |= ov4689_write_reg(dev, 0x3022, 0x01);
    ret |= ov4689_write_reg(dev, 0x3031, 0x0a);
    ret |= ov4689_write_reg(dev, 0x3305, 0xf1);
    ret |= ov4689_write_reg(dev, 0x3307, 0x04);
    ret |= ov4689_write_reg(dev, 0x3309, 0x29);
    ret |= ov4689_write_reg(dev, 0x3500, 0x00);
    ret |= ov4689_write_reg(dev, 0x3501, 0x60);
    ret |= ov4689_write_reg(dev, 0x3502, 0x00);
    ret |= ov4689_write_reg(dev, 0x3503, 0x04);
    ret |= ov4689_write_reg(dev, 0x3504, 0x00);
    ret |= ov4689_write_reg(dev, 0x3505, 0x00);
    ret |= ov4689_write_reg(dev, 0x3506, 0x00);
    ret |= ov4689_write_reg(dev, 0x3507, 0x00);
    ret |= ov4689_write_reg(dev, 0x3508, 0x00);
    ret |= ov4689_write_reg(dev, 0x3509, 0x80);
    ret |= ov4689_write_reg(dev, 0x350a, 0x00);
    ret |= ov4689_write_reg(dev, 0x350b, 0x00);
    ret |= ov4689_write_reg(dev, 0x350c, 0x00);
    ret |= ov4689_write_reg(dev, 0x350d, 0x00);
    ret |= ov4689_write_reg(dev, 0x350e, 0x00);
    ret |= ov4689_write_reg(dev, 0x350f, 0x80);
    ret |= ov4689_write_reg(dev, 0x3510, 0x00);
    ret |= ov4689_write_reg(dev, 0x3511, 0x00);
    ret |= ov4689_write_reg(dev, 0x3512, 0x00);
    ret |= ov4689_write_reg(dev, 0x3513, 0x00);
    ret |= ov4689_write_reg(dev, 0x3514, 0x00);
    ret |= ov4689_write_reg(dev, 0x3515, 0x80);
    ret |= ov4689_write_reg(dev, 0x3516, 0x00);
    ret |= ov4689_write_reg(dev, 0x3517, 0x00);
    ret |= ov4689_write_reg(dev, 0x3518, 0x00);
    ret |= ov4689_write_reg(dev, 0x3519, 0x00);
    ret |= ov4689_write_reg(dev, 0x351a, 0x00);
    ret |= ov4689_write_reg(dev, 0x351b, 0x80);
    ret |= ov4689_write_reg(dev, 0x351c, 0x00);
    ret |= ov4689_write_reg(dev, 0x351d, 0x00);
    ret |= ov4689_write_reg(dev, 0x351e, 0x00);
    ret |= ov4689_write_reg(dev, 0x351f, 0x00);
    ret |= ov4689_write_reg(dev, 0x3520, 0x00);
    ret |= ov4689_write_reg(dev, 0x3521, 0x80);
    ret |= ov4689_write_reg(dev, 0x3522, 0x08);
    ret |= ov4689_write_reg(dev, 0x3524, 0x08);
    ret |= ov4689_write_reg(dev, 0x3526, 0x08);
    ret |= ov4689_write_reg(dev, 0x3528, 0x08);
    ret |= ov4689_write_reg(dev, 0x352a, 0x08);
    ret |= ov4689_write_reg(dev, 0x3602, 0x00);
    ret |= ov4689_write_reg(dev, 0x3604, 0x02);
    ret |= ov4689_write_reg(dev, 0x3605, 0x00);
    ret |= ov4689_write_reg(dev, 0x3606, 0x00);
    ret |= ov4689_write_reg(dev, 0x3607, 0x00);
    ret |= ov4689_write_reg(dev, 0x3609, 0x12);
    ret |= ov4689_write_reg(dev, 0x360a, 0x40);
    ret |= ov4689_write_reg(dev, 0x360c, 0x08);
    ret |= ov4689_write_reg(dev, 0x360f, 0xe5);
    ret |= ov4689_write_reg(dev, 0x3608, 0x8f);
    ret |= ov4689_write_reg(dev, 0x3611, 0x00);
    ret |= ov4689_write_reg(dev, 0x3613, 0xf7);
    ret |= ov4689_write_reg(dev, 0x3616, 0x58);
    ret |= ov4689_write_reg(dev, 0x3619, 0x99);
    ret |= ov4689_write_reg(dev, 0x361b, 0x60);
    ret |= ov4689_write_reg(dev, 0x361c, 0x7a);
    ret |= ov4689_write_reg(dev, 0x361e, 0x79);
    ret |= ov4689_write_reg(dev, 0x361f, 0x02);
    ret |= ov4689_write_reg(dev, 0x3632, 0x00);
    ret |= ov4689_write_reg(dev, 0x3633, 0x10);
    ret |= ov4689_write_reg(dev, 0x3634, 0x10);
    ret |= ov4689_write_reg(dev, 0x3635, 0x10);
    ret |= ov4689_write_reg(dev, 0x3636, 0x15);
    ret |= ov4689_write_reg(dev, 0x3646, 0x86);
    ret |= ov4689_write_reg(dev, 0x364a, 0x0b);
    ret |= ov4689_write_reg(dev, 0x3700, 0x17);
    ret |= ov4689_write_reg(dev, 0x3701, 0x22);
    ret |= ov4689_write_reg(dev, 0x3703, 0x10);
    ret |= ov4689_write_reg(dev, 0x370a, 0x37);
    ret |= ov4689_write_reg(dev, 0x3705, 0x00);
    ret |= ov4689_write_reg(dev, 0x3706, 0x63);
    ret |= ov4689_write_reg(dev, 0x3709, 0x3c);
    ret |= ov4689_write_reg(dev, 0x370b, 0x01);
    ret |= ov4689_write_reg(dev, 0x370c, 0x30);
    ret |= ov4689_write_reg(dev, 0x3710, 0x24);
    ret |= ov4689_write_reg(dev, 0x3711, 0x0c);
    ret |= ov4689_write_reg(dev, 0x3716, 0x00);
    ret |= ov4689_write_reg(dev, 0x3720, 0x28);
    ret |= ov4689_write_reg(dev, 0x3729, 0x7b);
    ret |= ov4689_write_reg(dev, 0x372a, 0x84);
    ret |= ov4689_write_reg(dev, 0x372b, 0xbd);
    ret |= ov4689_write_reg(dev, 0x372c, 0xbc);
    ret |= ov4689_write_reg(dev, 0x372e, 0x52);
    ret |= ov4689_write_reg(dev, 0x373c, 0x0e);
    ret |= ov4689_write_reg(dev, 0x373e, 0x33);
    ret |= ov4689_write_reg(dev, 0x3743, 0x10);
    ret |= ov4689_write_reg(dev, 0x3744, 0x88);
    ret |= ov4689_write_reg(dev, 0x374a, 0x43);
    ret |= ov4689_write_reg(dev, 0x374c, 0x00);
    ret |= ov4689_write_reg(dev, 0x374e, 0x23);
    ret |= ov4689_write_reg(dev, 0x3751, 0x7b);
    ret |= ov4689_write_reg(dev, 0x3752, 0x84);
    ret |= ov4689_write_reg(dev, 0x3753, 0xbd);
    ret |= ov4689_write_reg(dev, 0x3754, 0xbc);
    ret |= ov4689_write_reg(dev, 0x3756, 0x52);
    ret |= ov4689_write_reg(dev, 0x375c, 0x00);
    ret |= ov4689_write_reg(dev, 0x3760, 0x00);
    ret |= ov4689_write_reg(dev, 0x3761, 0x00);
    ret |= ov4689_write_reg(dev, 0x3762, 0x00);
    ret |= ov4689_write_reg(dev, 0x3763, 0x00);
    ret |= ov4689_write_reg(dev, 0x3764, 0x00);
    ret |= ov4689_write_reg(dev, 0x3767, 0x04);
    ret |= ov4689_write_reg(dev, 0x3768, 0x04);
    ret |= ov4689_write_reg(dev, 0x3769, 0x08);
    ret |= ov4689_write_reg(dev, 0x376a, 0x08);
    ret |= ov4689_write_reg(dev, 0x376b, 0x20);
    ret |= ov4689_write_reg(dev, 0x376c, 0x00);
    ret |= ov4689_write_reg(dev, 0x376d, 0x00);
    ret |= ov4689_write_reg(dev, 0x376e, 0x00);
    ret |= ov4689_write_reg(dev, 0x3773, 0x00);
    ret |= ov4689_write_reg(dev, 0x3774, 0x51);
    ret |= ov4689_write_reg(dev, 0x3776, 0xbd);
    ret |= ov4689_write_reg(dev, 0x3777, 0xbd);
    ret |= ov4689_write_reg(dev, 0x3781, 0x18);
    ret |= ov4689_write_reg(dev, 0x3783, 0x25);
    ret |= ov4689_write_reg(dev, 0x3800, 0x00);
    ret |= ov4689_write_reg(dev, 0x3801, 0x08);
    ret |= ov4689_write_reg(dev, 0x3802, 0x00);
    ret |= ov4689_write_reg(dev, 0x3803, 0x04);
    ret |= ov4689_write_reg(dev, 0x3804, 0x0a);
    ret |= ov4689_write_reg(dev, 0x3805, 0x97);
    ret |= ov4689_write_reg(dev, 0x3806, 0x05);
    ret |= ov4689_write_reg(dev, 0x3807, 0xfb);
    ret |= ov4689_write_reg(dev, 0x3808, 0x0a);
    ret |= ov4689_write_reg(dev, 0x3809, 0x80);
    ret |= ov4689_write_reg(dev, 0x380a, 0x05);
    ret |= ov4689_write_reg(dev, 0x380b, 0xf0);
    ret |= ov4689_write_reg(dev, 0x380c, 0x09);
    ret |= ov4689_write_reg(dev, 0x380d, 0xc4);
    ret |= ov4689_write_reg(dev, 0x380e, 0x06);
    ret |= ov4689_write_reg(dev, 0x380f, 0x40);
    ret |= ov4689_write_reg(dev, 0x3810, 0x00);
    ret |= ov4689_write_reg(dev, 0x3811, 0x08);
    ret |= ov4689_write_reg(dev, 0x3812, 0x00);
    ret |= ov4689_write_reg(dev, 0x3813, 0x04);
    ret |= ov4689_write_reg(dev, 0x3814, 0x01);
    ret |= ov4689_write_reg(dev, 0x3815, 0x01);
    ret |= ov4689_write_reg(dev, 0x3819, 0x01);
    ret |= ov4689_write_reg(dev, 0x3820, 0x00);
    ret |= ov4689_write_reg(dev, 0x3821, 0x06);
    ret |= ov4689_write_reg(dev, 0x3829, 0x00);
    ret |= ov4689_write_reg(dev, 0x382a, 0x01);
    ret |= ov4689_write_reg(dev, 0x382b, 0x01);
    ret |= ov4689_write_reg(dev, 0x382d, 0x7f);
    ret |= ov4689_write_reg(dev, 0x3830, 0x04);
    ret |= ov4689_write_reg(dev, 0x3836, 0x01);
    ret |= ov4689_write_reg(dev, 0x3841, 0x02);
    ret |= ov4689_write_reg(dev, 0x3846, 0x08);
    ret |= ov4689_write_reg(dev, 0x3847, 0x07);
    ret |= ov4689_write_reg(dev, 0x3d85, 0x36);
    ret |= ov4689_write_reg(dev, 0x3d8c, 0x71);
    ret |= ov4689_write_reg(dev, 0x3d8d, 0xcb);
    ret |= ov4689_write_reg(dev, 0x3f0a, 0x00);
    ret |= ov4689_write_reg(dev, 0x4000, 0x71);
    ret |= ov4689_write_reg(dev, 0x4001, 0x40);
    ret |= ov4689_write_reg(dev, 0x4002, 0x04);
    ret |= ov4689_write_reg(dev, 0x4003, 0x14);
    ret |= ov4689_write_reg(dev, 0x400e, 0x00);
    ret |= ov4689_write_reg(dev, 0x4011, 0x00);
    ret |= ov4689_write_reg(dev, 0x401a, 0x00);
    ret |= ov4689_write_reg(dev, 0x401b, 0x00);
    ret |= ov4689_write_reg(dev, 0x401c, 0x00);
    ret |= ov4689_write_reg(dev, 0x401d, 0x00);
    ret |= ov4689_write_reg(dev, 0x401f, 0x00);
    ret |= ov4689_write_reg(dev, 0x4020, 0x00);
    ret |= ov4689_write_reg(dev, 0x4021, 0x10);
    ret |= ov4689_write_reg(dev, 0x4022, 0x07);
    ret |= ov4689_write_reg(dev, 0x4023, 0xcf);
    ret |= ov4689_write_reg(dev, 0x4024, 0x09);
    ret |= ov4689_write_reg(dev, 0x4025, 0x60);
    ret |= ov4689_write_reg(dev, 0x4026, 0x09);
    ret |= ov4689_write_reg(dev, 0x4027, 0x6f);
    ret |= ov4689_write_reg(dev, 0x4028, 0x00);
    ret |= ov4689_write_reg(dev, 0x4029, 0x02);
    ret |= ov4689_write_reg(dev, 0x402a, 0x06);
    ret |= ov4689_write_reg(dev, 0x402b, 0x04);
    ret |= ov4689_write_reg(dev, 0x402c, 0x02);
    ret |= ov4689_write_reg(dev, 0x402d, 0x02);
    ret |= ov4689_write_reg(dev, 0x402e, 0x0e);
    ret |= ov4689_write_reg(dev, 0x402f, 0x04);
    ret |= ov4689_write_reg(dev, 0x4302, 0xff);
    ret |= ov4689_write_reg(dev, 0x4303, 0xff);
    ret |= ov4689_write_reg(dev, 0x4304, 0x00);
    ret |= ov4689_write_reg(dev, 0x4305, 0x00);
    ret |= ov4689_write_reg(dev, 0x4306, 0x00);
    ret |= ov4689_write_reg(dev, 0x4308, 0x02);
    ret |= ov4689_write_reg(dev, 0x4500, 0x6c);
    ret |= ov4689_write_reg(dev, 0x4501, 0xc4);
    ret |= ov4689_write_reg(dev, 0x4502, 0x40);
    ret |= ov4689_write_reg(dev, 0x4503, 0x02);
    ret |= ov4689_write_reg(dev, 0x4601, 0xA7);
    ret |= ov4689_write_reg(dev, 0x4800, 0x04);
    ret |= ov4689_write_reg(dev, 0x4813, 0x08);
    ret |= ov4689_write_reg(dev, 0x481f, 0x40);
    ret |= ov4689_write_reg(dev, 0x4829, 0x78);
    ret |= ov4689_write_reg(dev, 0x4837, 0x10);
    ret |= ov4689_write_reg(dev, 0x4b00, 0x2a);
    ret |= ov4689_write_reg(dev, 0x4b0d, 0x00);
    ret |= ov4689_write_reg(dev, 0x4d00, 0x04);
    ret |= ov4689_write_reg(dev, 0x4d01, 0x42);
    ret |= ov4689_write_reg(dev, 0x4d02, 0xd1);
    ret |= ov4689_write_reg(dev, 0x4d03, 0x93);
    ret |= ov4689_write_reg(dev, 0x4d04, 0xf5);
    ret |= ov4689_write_reg(dev, 0x4d05, 0xc1);
    ret |= ov4689_write_reg(dev, 0x5000, 0xf3);
    ret |= ov4689_write_reg(dev, 0x5001, 0x11);
    ret |= ov4689_write_reg(dev, 0x5004, 0x00);
    ret |= ov4689_write_reg(dev, 0x500a, 0x00);
    ret |= ov4689_write_reg(dev, 0x500b, 0x00);
    ret |= ov4689_write_reg(dev, 0x5032, 0x00);
    ret |= ov4689_write_reg(dev, 0x5040, 0x00);
    ret |= ov4689_write_reg(dev, 0x5050, 0x0c);
    ret |= ov4689_write_reg(dev, 0x5500, 0x00);
    ret |= ov4689_write_reg(dev, 0x5501, 0x10);
    ret |= ov4689_write_reg(dev, 0x5502, 0x01);
    ret |= ov4689_write_reg(dev, 0x5503, 0x0f);
    ret |= ov4689_write_reg(dev, 0x8000, 0x00);
    ret |= ov4689_write_reg(dev, 0x8001, 0x00);
    ret |= ov4689_write_reg(dev, 0x8002, 0x00);
    ret |= ov4689_write_reg(dev, 0x8003, 0x00);
    ret |= ov4689_write_reg(dev, 0x8004, 0x00);
    ret |= ov4689_write_reg(dev, 0x8005, 0x00);
    ret |= ov4689_write_reg(dev, 0x8006, 0x00);
    ret |= ov4689_write_reg(dev, 0x8007, 0x00);
    ret |= ov4689_write_reg(dev, 0x8008, 0x00);
    ret |= ov4689_write_reg(dev, 0x3638, 0x00);
    ret |= ov4689_write_reg(dev, 0x3105, 0x31);
    ret |= ov4689_write_reg(dev, 0x301a, 0xf9);
    ret |= ov4689_write_reg(dev, 0x3508, 0x07);
    ret |= ov4689_write_reg(dev, 0x484b, 0x05);
    ret |= ov4689_write_reg(dev, 0x4805, 0x03);
    ret |= ov4689_write_reg(dev, 0x3601, 0x01);
    ret |= ov4689_write_reg(dev, 0x3745, 0xc0);
    ret |= ov4689_write_reg(dev, 0x3798, 0x1b);
    ret |= ov4689_write_reg(dev, 0x0100, 0x01);
    ov4689_delay_ms(10);
    ret |= ov4689_write_reg(dev, 0x3105, 0x11);
    ret |= ov4689_write_reg(dev, 0x301a, 0xf1);
    ret |= ov4689_write_reg(dev, 0x4805, 0x00);
    ret |= ov4689_write_reg(dev, 0x301a, 0xf0);
    ret |= ov4689_write_reg(dev, 0x3208, 0x00);
    ret |= ov4689_write_reg(dev, 0x302a, 0x00);
    ret |= ov4689_write_reg(dev, 0x302a, 0x00);
    ret |= ov4689_write_reg(dev, 0x302a, 0x00);
    ret |= ov4689_write_reg(dev, 0x302a, 0x00);
    ret |= ov4689_write_reg(dev, 0x302a, 0x00);
    ret |= ov4689_write_reg(dev, 0x3601, 0x00);
    ret |= ov4689_write_reg(dev, 0x3638, 0x00);
    ret |= ov4689_write_reg(dev, 0x3208, 0x10);
    ret |= ov4689_write_reg(dev, 0x3208, 0xa0);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "ov4689_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("==== OV4689_24MInput_MIPI_4lane_10bit_linear_2688x1520_30fps init success!============\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_SUCCESS;
}


xmedia_s32 ov4689_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case OV4689_4M_30FPS_10BIT_LINEAR_MODE:
            ret = ov4689_4lane_linear_2688x1520_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 ov4689_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_ov4689_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_ov4689_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_ov4689_i2c_fd[dev]);
        g_ov4689_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_ov4689_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 ov4689_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_ov4689_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_ov4689_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_ov4689_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = ov4689_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 ov4689_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_ov4689_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_ov4689_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 ov4689_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[OV4689_DATA_BYTE];

    if (g_ov4689_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_ov4689_i2c_fd[dev], g_ov4689_i2c_addr[dev], addr, OV4689_ADDR_BYTE, buf, OV4689_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //OV4689_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 ov4689_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_ov4689_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_ov4689_i2c_fd[dev], g_ov4689_i2c_addr[dev], buf, OV4689_ADDR_BYTE + OV4689_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 ov4689_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u32 mirror_type;
    xmedia_u32 flip_type;

    if (mirror_en){
        mirror_type = 0x6;
    } else {
        mirror_type = 0x0;
    }

    if (flip_en){
        flip_type = 0x6;
    } else {
        flip_type = 0x0;
    }

    ret |= ov4689_write_reg(dev, OV4689_REG_ADDR_MIRROR, mirror_type);
    ret |= ov4689_write_reg(dev, OV4689_REG_ADDR_FLIP, flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "ov4689_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}

