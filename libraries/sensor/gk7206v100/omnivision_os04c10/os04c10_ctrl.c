#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "os04c10.h"
#include "os04c10_ctrl.h"

#define OS04C10_REG_ADDR_MIRROR_FLIP 0x3820
#define OS04C10_REG_ADDR_ANALOG_FLIP 0x3716

static xmedia_s32 g_os04c10_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM]   = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] =
                                                                        SENSOR_I2C_INVALID };
static xmedia_s32 g_os04c10_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] =
                                                                        SENSOR_I2C_INVALID };

#ifdef FPGA
static xmedia_s32 os04c10_4lane_linear_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os04c10_write_reg(dev, 0x0103, 0x01);
    ret |= os04c10_write_reg(dev, 0x0301, 0x84);
    ret |= os04c10_write_reg(dev, 0x0303, 0x01);
    ret |= os04c10_write_reg(dev, 0x0305, 0x2b);
    ret |= os04c10_write_reg(dev, 0x0306, 0x01);
    ret |= os04c10_write_reg(dev, 0x0307, 0x17);
    ret |= os04c10_write_reg(dev, 0x0323, 0x04);
    ret |= os04c10_write_reg(dev, 0x0324, 0x01);
    ret |= os04c10_write_reg(dev, 0x0325, 0x62);
    ret |= os04c10_write_reg(dev, 0x3012, 0x06);
    ret |= os04c10_write_reg(dev, 0x3013, 0x02);
    ret |= os04c10_write_reg(dev, 0x3016, 0x72);
    ret |= os04c10_write_reg(dev, 0x3021, 0x03);
    ret |= os04c10_write_reg(dev, 0x3106, 0x21);
    ret |= os04c10_write_reg(dev, 0x3107, 0xa1);
    ret |= os04c10_write_reg(dev, 0x3500, 0x00);
    ret |= os04c10_write_reg(dev, 0x3501, 0x06);
    ret |= os04c10_write_reg(dev, 0x3502, 0x1e);
    ret |= os04c10_write_reg(dev, 0x3503, 0x88);
    ret |= os04c10_write_reg(dev, 0x3508, 0x00);
    ret |= os04c10_write_reg(dev, 0x3509, 0x80);
    ret |= os04c10_write_reg(dev, 0x350a, 0x04);
    ret |= os04c10_write_reg(dev, 0x350b, 0x00);
    ret |= os04c10_write_reg(dev, 0x350c, 0x00);
    ret |= os04c10_write_reg(dev, 0x350d, 0x80);
    ret |= os04c10_write_reg(dev, 0x350e, 0x04);
    ret |= os04c10_write_reg(dev, 0x350f, 0x00);
    ret |= os04c10_write_reg(dev, 0x3510, 0x00);
    ret |= os04c10_write_reg(dev, 0x3511, 0x00);
    ret |= os04c10_write_reg(dev, 0x3512, 0x20);
    ret |= os04c10_write_reg(dev, 0x3624, 0x02);
    ret |= os04c10_write_reg(dev, 0x3625, 0x4c);
    ret |= os04c10_write_reg(dev, 0x3660, 0x00);
    ret |= os04c10_write_reg(dev, 0x3666, 0xa5);
    ret |= os04c10_write_reg(dev, 0x3667, 0xa5);
    ret |= os04c10_write_reg(dev, 0x366a, 0x64);
    ret |= os04c10_write_reg(dev, 0x3673, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3672, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3671, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3670, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3685, 0x00);
    ret |= os04c10_write_reg(dev, 0x3694, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3693, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3692, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3691, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3696, 0x4c);
    ret |= os04c10_write_reg(dev, 0x3697, 0x4c);
    ret |= os04c10_write_reg(dev, 0x3698, 0x40);
    ret |= os04c10_write_reg(dev, 0x3699, 0x80);
    ret |= os04c10_write_reg(dev, 0x369a, 0x18);
    ret |= os04c10_write_reg(dev, 0x369b, 0x1f);
    ret |= os04c10_write_reg(dev, 0x369c, 0x14);
    ret |= os04c10_write_reg(dev, 0x369d, 0x80);
    ret |= os04c10_write_reg(dev, 0x369e, 0x40);
    ret |= os04c10_write_reg(dev, 0x369f, 0x21);
    ret |= os04c10_write_reg(dev, 0x36a0, 0x12);
    ret |= os04c10_write_reg(dev, 0x36a1, 0x5d);
    ret |= os04c10_write_reg(dev, 0x36a2, 0x66);
    ret |= os04c10_write_reg(dev, 0x370a, 0x00);
    ret |= os04c10_write_reg(dev, 0x370e, 0x0c);
    ret |= os04c10_write_reg(dev, 0x3710, 0x00);
    ret |= os04c10_write_reg(dev, 0x3713, 0x00);
    ret |= os04c10_write_reg(dev, 0x3725, 0x02);
    ret |= os04c10_write_reg(dev, 0x372a, 0x03);
    ret |= os04c10_write_reg(dev, 0x3738, 0xce);
    ret |= os04c10_write_reg(dev, 0x3739, 0x10);
    ret |= os04c10_write_reg(dev, 0x3748, 0x00);
    ret |= os04c10_write_reg(dev, 0x374a, 0x00);
    ret |= os04c10_write_reg(dev, 0x374c, 0x00);
    ret |= os04c10_write_reg(dev, 0x374e, 0x00);
    ret |= os04c10_write_reg(dev, 0x3756, 0x00);
    ret |= os04c10_write_reg(dev, 0x3757, 0x0e);
    ret |= os04c10_write_reg(dev, 0x3767, 0x00);
    ret |= os04c10_write_reg(dev, 0x3771, 0x00);
    ret |= os04c10_write_reg(dev, 0x377b, 0x20);
    ret |= os04c10_write_reg(dev, 0x377c, 0x00);
    ret |= os04c10_write_reg(dev, 0x377d, 0x0c);
    ret |= os04c10_write_reg(dev, 0x3781, 0x03);
    ret |= os04c10_write_reg(dev, 0x3782, 0x00);
    ret |= os04c10_write_reg(dev, 0x3789, 0x14);
    ret |= os04c10_write_reg(dev, 0x3795, 0x02);
    ret |= os04c10_write_reg(dev, 0x379c, 0x00);
    ret |= os04c10_write_reg(dev, 0x379d, 0x00);
    ret |= os04c10_write_reg(dev, 0x37b8, 0x04);
    ret |= os04c10_write_reg(dev, 0x37ba, 0x03);
    ret |= os04c10_write_reg(dev, 0x37bb, 0x00);
    ret |= os04c10_write_reg(dev, 0x37bc, 0x04);
    ret |= os04c10_write_reg(dev, 0x37be, 0x08);
    ret |= os04c10_write_reg(dev, 0x37c4, 0x11);
    ret |= os04c10_write_reg(dev, 0x37c5, 0x80);
    ret |= os04c10_write_reg(dev, 0x37c6, 0x14);
    ret |= os04c10_write_reg(dev, 0x37c7, 0x08);
    ret |= os04c10_write_reg(dev, 0x37da, 0x11);
    ret |= os04c10_write_reg(dev, 0x381f, 0x08);
    ret |= os04c10_write_reg(dev, 0x3829, 0x03);
    ret |= os04c10_write_reg(dev, 0x3832, 0x00);
    ret |= os04c10_write_reg(dev, 0x3881, 0x00);
    ret |= os04c10_write_reg(dev, 0x3888, 0x04);
    ret |= os04c10_write_reg(dev, 0x388b, 0x00);
    ret |= os04c10_write_reg(dev, 0x3c80, 0x10);
    ret |= os04c10_write_reg(dev, 0x3c86, 0x00);
    ret |= os04c10_write_reg(dev, 0x3c9f, 0x01);
    ret |= os04c10_write_reg(dev, 0x3d85, 0x1b);
    ret |= os04c10_write_reg(dev, 0x3d8c, 0x71);
    ret |= os04c10_write_reg(dev, 0x3d8d, 0xe2);
    ret |= os04c10_write_reg(dev, 0x3f00, 0x0b);
    ret |= os04c10_write_reg(dev, 0x3f06, 0x04);
    ret |= os04c10_write_reg(dev, 0x400a, 0x01);
    ret |= os04c10_write_reg(dev, 0x400b, 0x50);
    ret |= os04c10_write_reg(dev, 0x400e, 0x08);
    ret |= os04c10_write_reg(dev, 0x4040, 0x00);
    ret |= os04c10_write_reg(dev, 0x4041, 0x07);
    ret |= os04c10_write_reg(dev, 0x4043, 0x7e);
    ret |= os04c10_write_reg(dev, 0x4045, 0x7e);
    ret |= os04c10_write_reg(dev, 0x4047, 0x7e);
    ret |= os04c10_write_reg(dev, 0x4049, 0x7e);
    ret |= os04c10_write_reg(dev, 0x4090, 0x14);
    ret |= os04c10_write_reg(dev, 0x40b0, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b1, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b2, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b3, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b4, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b5, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b7, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b8, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b9, 0x00);
    ret |= os04c10_write_reg(dev, 0x40ba, 0x00);
    ret |= os04c10_write_reg(dev, 0x4301, 0x00);
    ret |= os04c10_write_reg(dev, 0x4303, 0x00);
    ret |= os04c10_write_reg(dev, 0x4502, 0x04);
    ret |= os04c10_write_reg(dev, 0x4503, 0x00);
    ret |= os04c10_write_reg(dev, 0x4504, 0x06);
    ret |= os04c10_write_reg(dev, 0x4506, 0x00);
    ret |= os04c10_write_reg(dev, 0x4507, 0x64);
    ret |= os04c10_write_reg(dev, 0x4803, 0x00);
    ret |= os04c10_write_reg(dev, 0x480c, 0x32);
    ret |= os04c10_write_reg(dev, 0x480e, 0x00);
    ret |= os04c10_write_reg(dev, 0x4813, 0x00);
    ret |= os04c10_write_reg(dev, 0x4819, 0x70);
    ret |= os04c10_write_reg(dev, 0x481f, 0x30);
    ret |= os04c10_write_reg(dev, 0x4823, 0x3f);
    ret |= os04c10_write_reg(dev, 0x4825, 0x30);
    ret |= os04c10_write_reg(dev, 0x4833, 0x10);
    ret |= os04c10_write_reg(dev, 0x484b, 0x07);
    ret |= os04c10_write_reg(dev, 0x488b, 0x00);
    ret |= os04c10_write_reg(dev, 0x4d00, 0x04);
    ret |= os04c10_write_reg(dev, 0x4d01, 0xad);
    ret |= os04c10_write_reg(dev, 0x4d02, 0xbc);
    ret |= os04c10_write_reg(dev, 0x4d03, 0xa1);
    ret |= os04c10_write_reg(dev, 0x4d04, 0x1f);
    ret |= os04c10_write_reg(dev, 0x4d05, 0x4c);
    ret |= os04c10_write_reg(dev, 0x4d0b, 0x01);
    ret |= os04c10_write_reg(dev, 0x4e00, 0x2a);
    ret |= os04c10_write_reg(dev, 0x4e0d, 0x00);
    ret |= os04c10_write_reg(dev, 0x5001, 0x09);
    ret |= os04c10_write_reg(dev, 0x5004, 0x00);
    ret |= os04c10_write_reg(dev, 0x5080, 0x04);
    ret |= os04c10_write_reg(dev, 0x5036, 0x00);
    ret |= os04c10_write_reg(dev, 0x5180, 0x70);
    ret |= os04c10_write_reg(dev, 0x5181, 0x10);
    ret |= os04c10_write_reg(dev, 0x520a, 0x03);
    ret |= os04c10_write_reg(dev, 0x520b, 0x06);
    ret |= os04c10_write_reg(dev, 0x520c, 0x0c);
    ret |= os04c10_write_reg(dev, 0x580b, 0x0f);
    ret |= os04c10_write_reg(dev, 0x580d, 0x00);
    ret |= os04c10_write_reg(dev, 0x580f, 0x00);
    ret |= os04c10_write_reg(dev, 0x5820, 0x00);
    ret |= os04c10_write_reg(dev, 0x5821, 0x00);
    ret |= os04c10_write_reg(dev, 0x301c, 0xf8);
    ret |= os04c10_write_reg(dev, 0x301e, 0xb4);
    ret |= os04c10_write_reg(dev, 0x301f, 0xd0);
    ret |= os04c10_write_reg(dev, 0x3022, 0x01);
    ret |= os04c10_write_reg(dev, 0x3109, 0xe7);
    ret |= os04c10_write_reg(dev, 0x3600, 0x00);
    ret |= os04c10_write_reg(dev, 0x3610, 0x65);
    ret |= os04c10_write_reg(dev, 0x3611, 0x85);
    ret |= os04c10_write_reg(dev, 0x3613, 0x3a);
    ret |= os04c10_write_reg(dev, 0x3615, 0x60);
    ret |= os04c10_write_reg(dev, 0x3621, 0x90);
    ret |= os04c10_write_reg(dev, 0x3620, 0x0c);
    ret |= os04c10_write_reg(dev, 0x3629, 0x00);
    ret |= os04c10_write_reg(dev, 0x3661, 0x04);
    ret |= os04c10_write_reg(dev, 0x3662, 0x10);
    ret |= os04c10_write_reg(dev, 0x3664, 0x70);
    ret |= os04c10_write_reg(dev, 0x3665, 0x00);
    ret |= os04c10_write_reg(dev, 0x3681, 0xa6);
    ret |= os04c10_write_reg(dev, 0x3682, 0x53);
    ret |= os04c10_write_reg(dev, 0x3683, 0x2a);
    ret |= os04c10_write_reg(dev, 0x3684, 0x15);
    ret |= os04c10_write_reg(dev, 0x3700, 0x2a);
    ret |= os04c10_write_reg(dev, 0x3701, 0x12);
    ret |= os04c10_write_reg(dev, 0x3703, 0x28);
    ret |= os04c10_write_reg(dev, 0x3704, 0x0e);
    ret |= os04c10_write_reg(dev, 0x3706, 0x4a);
    ret |= os04c10_write_reg(dev, 0x3709, 0x4a);
    ret |= os04c10_write_reg(dev, 0x370b, 0xa2);
    ret |= os04c10_write_reg(dev, 0x370c, 0x01);
    ret |= os04c10_write_reg(dev, 0x370f, 0x04);
    ret |= os04c10_write_reg(dev, 0x3714, 0x24);
    ret |= os04c10_write_reg(dev, 0x3716, 0x24);
    ret |= os04c10_write_reg(dev, 0x3719, 0x11);
    ret |= os04c10_write_reg(dev, 0x371a, 0x1e);
    ret |= os04c10_write_reg(dev, 0x3720, 0x00);
    ret |= os04c10_write_reg(dev, 0x3724, 0x13);
    ret |= os04c10_write_reg(dev, 0x373f, 0xb0);
    ret |= os04c10_write_reg(dev, 0x3741, 0x4a);
    ret |= os04c10_write_reg(dev, 0x3743, 0x4a);
    ret |= os04c10_write_reg(dev, 0x3745, 0x4a);
    ret |= os04c10_write_reg(dev, 0x3747, 0x4a);
    ret |= os04c10_write_reg(dev, 0x3749, 0xa2);
    ret |= os04c10_write_reg(dev, 0x374b, 0xa2);
    ret |= os04c10_write_reg(dev, 0x374d, 0xa2);
    ret |= os04c10_write_reg(dev, 0x374f, 0xa2);
    ret |= os04c10_write_reg(dev, 0x3755, 0x10);
    ret |= os04c10_write_reg(dev, 0x376c, 0x00);
    ret |= os04c10_write_reg(dev, 0x378d, 0x30);
    ret |= os04c10_write_reg(dev, 0x3790, 0x4a);
    ret |= os04c10_write_reg(dev, 0x3791, 0xa2);
    ret |= os04c10_write_reg(dev, 0x3798, 0x40);
    ret |= os04c10_write_reg(dev, 0x379e, 0x00);
    ret |= os04c10_write_reg(dev, 0x379f, 0x04);
    ret |= os04c10_write_reg(dev, 0x37a1, 0x10);
    ret |= os04c10_write_reg(dev, 0x37a2, 0x1e);
    ret |= os04c10_write_reg(dev, 0x37a8, 0x10);
    ret |= os04c10_write_reg(dev, 0x37a9, 0x1e);
    ret |= os04c10_write_reg(dev, 0x37ac, 0xa0);
    ret |= os04c10_write_reg(dev, 0x37b9, 0x01);
    ret |= os04c10_write_reg(dev, 0x37bd, 0x01);
    ret |= os04c10_write_reg(dev, 0x37bf, 0x26);
    ret |= os04c10_write_reg(dev, 0x37c0, 0x11);
    ret |= os04c10_write_reg(dev, 0x37c2, 0x04);
    ret |= os04c10_write_reg(dev, 0x37cd, 0x19);
    ret |= os04c10_write_reg(dev, 0x37d8, 0x02);
    ret |= os04c10_write_reg(dev, 0x37d9, 0x08);
    ret |= os04c10_write_reg(dev, 0x37e5, 0x02);
    ret |= os04c10_write_reg(dev, 0x3800, 0x00);
    ret |= os04c10_write_reg(dev, 0x3801, 0x00);
    ret |= os04c10_write_reg(dev, 0x3802, 0x00);
    ret |= os04c10_write_reg(dev, 0x3803, 0x00);
    ret |= os04c10_write_reg(dev, 0x3804, 0x0a);
    ret |= os04c10_write_reg(dev, 0x3805, 0x8f);
    ret |= os04c10_write_reg(dev, 0x3806, 0x05);
    ret |= os04c10_write_reg(dev, 0x3807, 0xff);
    ret |= os04c10_write_reg(dev, 0x3808, 0x0a);
    ret |= os04c10_write_reg(dev, 0x3809, 0x80);
    ret |= os04c10_write_reg(dev, 0x380a, 0x05);
    ret |= os04c10_write_reg(dev, 0x380b, 0xf0);

    // ret |= os04c10_write_reg(dev, 0x380c, 0x09);
    // ret |= os04c10_write_reg(dev, 0x380d, 0x2e);
    // ret |= os04c10_write_reg(dev, 0x380c, 0x0a); // xjh
    // ret |= os04c10_write_reg(dev, 0x380d, 0x80); // xjh
    ret |= os04c10_write_reg(dev, 0x380c, 0x11); // xjh MONO
    ret |= os04c10_write_reg(dev, 0x380d, 0x30); // xjh MONO
    // ret |= os04c10_write_reg(dev, 0x380c, 0x12); // xjh
    // ret |= os04c10_write_reg(dev, 0x380d, 0x5c); // xjh

    ret |= os04c10_write_reg(dev, 0x380e, 0x0a);
    ret |= os04c10_write_reg(dev, 0x380f, 0x26);
    // ret |= os04c10_write_reg(dev, 0x380e, 0x14); // xjh
    // ret |= os04c10_write_reg(dev, 0x380f, 0x4c); // xjh

    ret |= os04c10_write_reg(dev, 0x3811, 0x08);
    ret |= os04c10_write_reg(dev, 0x3813, 0x08);
    ret |= os04c10_write_reg(dev, 0x3814, 0x01);
    ret |= os04c10_write_reg(dev, 0x3815, 0x01);
    ret |= os04c10_write_reg(dev, 0x3816, 0x01);
    ret |= os04c10_write_reg(dev, 0x3817, 0x01);
    ret |= os04c10_write_reg(dev, 0x3820, 0x88);
    ret |= os04c10_write_reg(dev, 0x3821, 0x00);
    ret |= os04c10_write_reg(dev, 0x3880, 0x25);
    ret |= os04c10_write_reg(dev, 0x3882, 0x20);
    ret |= os04c10_write_reg(dev, 0x3c91, 0x0b);
    ret |= os04c10_write_reg(dev, 0x3c94, 0x45);
    ret |= os04c10_write_reg(dev, 0x4000, 0xf3);
    ret |= os04c10_write_reg(dev, 0x4001, 0x60);
    ret |= os04c10_write_reg(dev, 0x4003, 0x40);
    ret |= os04c10_write_reg(dev, 0x4008, 0x02);
    ret |= os04c10_write_reg(dev, 0x4009, 0x0d);
    ret |= os04c10_write_reg(dev, 0x4300, 0xff);
    ret |= os04c10_write_reg(dev, 0x4302, 0x0f);
    ret |= os04c10_write_reg(dev, 0x4305, 0x83);
    ret |= os04c10_write_reg(dev, 0x4505, 0x84);
    ret |= os04c10_write_reg(dev, 0x4809, 0x1e);
    ret |= os04c10_write_reg(dev, 0x480a, 0x04);
    ret |= os04c10_write_reg(dev, 0x4837, 0x15);
    ret |= os04c10_write_reg(dev, 0x4c00, 0x08);
    ret |= os04c10_write_reg(dev, 0x4c01, 0x08);
    ret |= os04c10_write_reg(dev, 0x4c04, 0x00);
    ret |= os04c10_write_reg(dev, 0x4c05, 0x00);
    ret |= os04c10_write_reg(dev, 0x5000, 0xe9);
    ret |= os04c10_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os04c10_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("==== OS04C10_24MInput_MIPI_4lane344Mbps_10bit_linear_2688x1520_16fps init success!====\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_2lane_linear_2688x1520_init(xmedia_u32 dev)
{
    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("========== OS04C10_24MInput_MIPI_2lane_10bit_linear_2688x1520 NOT support!=============\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 os04c10_2lane_wdr_2688x1520_init(xmedia_u32 dev)
{
    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("============= OS04C10_24MInput_MIPI_2lane_10bit_wdr_2688x1520 NOT support!=============\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}
#else

static void os04c10_delay_ms(xmedia_s32 ms)
{
    usleep(ms * 1000); // 1ms: 1000us
    return;
}

static xmedia_s32 os04c10_4lane_linear_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os04c10_write_reg(dev, 0x0103, 0x01);
    ret |= os04c10_write_reg(dev, 0x0301, 0xe4);
    ret |= os04c10_write_reg(dev, 0x0303, 0x01);
    ret |= os04c10_write_reg(dev, 0x0305, 0x6e);
    ret |= os04c10_write_reg(dev, 0x0306, 0x01);
    ret |= os04c10_write_reg(dev, 0x0307, 0x17);
    ret |= os04c10_write_reg(dev, 0x0323, 0x04);
    ret |= os04c10_write_reg(dev, 0x0324, 0x01);
    ret |= os04c10_write_reg(dev, 0x0325, 0x62);
    ret |= os04c10_write_reg(dev, 0x3012, 0x06);
    ret |= os04c10_write_reg(dev, 0x3013, 0x02);
    ret |= os04c10_write_reg(dev, 0x3016, 0x72);
    ret |= os04c10_write_reg(dev, 0x3021, 0x03);
    ret |= os04c10_write_reg(dev, 0x3106, 0x21);
    ret |= os04c10_write_reg(dev, 0x3107, 0xa1);
    ret |= os04c10_write_reg(dev, 0x3500, 0x00);
    ret |= os04c10_write_reg(dev, 0x3501, 0x06);
    ret |= os04c10_write_reg(dev, 0x3502, 0x1e);
    ret |= os04c10_write_reg(dev, 0x3503, 0x88);
    ret |= os04c10_write_reg(dev, 0x3508, 0x00);
    ret |= os04c10_write_reg(dev, 0x3509, 0x80);
    ret |= os04c10_write_reg(dev, 0x350a, 0x04);
    ret |= os04c10_write_reg(dev, 0x350b, 0x00);
    ret |= os04c10_write_reg(dev, 0x350c, 0x00);
    ret |= os04c10_write_reg(dev, 0x350d, 0x80);
    ret |= os04c10_write_reg(dev, 0x350e, 0x04);
    ret |= os04c10_write_reg(dev, 0x350f, 0x00);
    ret |= os04c10_write_reg(dev, 0x3510, 0x00);
    ret |= os04c10_write_reg(dev, 0x3511, 0x00);
    ret |= os04c10_write_reg(dev, 0x3512, 0x20);
    ret |= os04c10_write_reg(dev, 0x3624, 0x02);
    ret |= os04c10_write_reg(dev, 0x3625, 0x4c);
    ret |= os04c10_write_reg(dev, 0x3660, 0x00);
    ret |= os04c10_write_reg(dev, 0x3666, 0xa5);
    ret |= os04c10_write_reg(dev, 0x3667, 0xa5);
    ret |= os04c10_write_reg(dev, 0x366a, 0x60);
    ret |= os04c10_write_reg(dev, 0x3673, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3672, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3671, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3670, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3685, 0x00);
    ret |= os04c10_write_reg(dev, 0x3694, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3693, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3692, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3691, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3696, 0x4c);
    ret |= os04c10_write_reg(dev, 0x3697, 0x4c);
    ret |= os04c10_write_reg(dev, 0x3698, 0x40);
    ret |= os04c10_write_reg(dev, 0x3699, 0x80);
    ret |= os04c10_write_reg(dev, 0x369a, 0x18);
    ret |= os04c10_write_reg(dev, 0x369b, 0x1f);
    ret |= os04c10_write_reg(dev, 0x369c, 0x14);
    ret |= os04c10_write_reg(dev, 0x369d, 0x80);
    ret |= os04c10_write_reg(dev, 0x369e, 0x40);
    ret |= os04c10_write_reg(dev, 0x369f, 0x21);
    ret |= os04c10_write_reg(dev, 0x36a0, 0x12);
    ret |= os04c10_write_reg(dev, 0x36a1, 0x5d);
    ret |= os04c10_write_reg(dev, 0x36a2, 0x66);
    ret |= os04c10_write_reg(dev, 0x370a, 0x02);
    ret |= os04c10_write_reg(dev, 0x370e, 0x0c);
    ret |= os04c10_write_reg(dev, 0x3710, 0x00);
    ret |= os04c10_write_reg(dev, 0x3713, 0x00);
    ret |= os04c10_write_reg(dev, 0x3725, 0x02);
    ret |= os04c10_write_reg(dev, 0x372a, 0x03);
    ret |= os04c10_write_reg(dev, 0x3738, 0xce);
    ret |= os04c10_write_reg(dev, 0x3739, 0x10);
    ret |= os04c10_write_reg(dev, 0x3748, 0x02);
    ret |= os04c10_write_reg(dev, 0x374a, 0x02);
    ret |= os04c10_write_reg(dev, 0x374c, 0x02);
    ret |= os04c10_write_reg(dev, 0x374e, 0x02);
    ret |= os04c10_write_reg(dev, 0x3756, 0x00);
    ret |= os04c10_write_reg(dev, 0x3757, 0x0e);
    ret |= os04c10_write_reg(dev, 0x3767, 0x00);
    ret |= os04c10_write_reg(dev, 0x3771, 0x00);
    ret |= os04c10_write_reg(dev, 0x377b, 0x20);
    ret |= os04c10_write_reg(dev, 0x377c, 0x00);
    ret |= os04c10_write_reg(dev, 0x377d, 0x0c);
    ret |= os04c10_write_reg(dev, 0x3781, 0x03);
    ret |= os04c10_write_reg(dev, 0x3782, 0x00);
    ret |= os04c10_write_reg(dev, 0x3789, 0x14);
    ret |= os04c10_write_reg(dev, 0x3795, 0x02);
    ret |= os04c10_write_reg(dev, 0x379c, 0x00);
    ret |= os04c10_write_reg(dev, 0x379d, 0x00);
    ret |= os04c10_write_reg(dev, 0x37b8, 0x04);
    ret |= os04c10_write_reg(dev, 0x37ba, 0x03);
    ret |= os04c10_write_reg(dev, 0x37bb, 0x00);
    ret |= os04c10_write_reg(dev, 0x37bc, 0x04);
    ret |= os04c10_write_reg(dev, 0x37be, 0x08);
    ret |= os04c10_write_reg(dev, 0x37c4, 0x11);
    ret |= os04c10_write_reg(dev, 0x37c5, 0x80);
    ret |= os04c10_write_reg(dev, 0x37c6, 0x14);
    ret |= os04c10_write_reg(dev, 0x37c7, 0x08);
    ret |= os04c10_write_reg(dev, 0x37da, 0x11);
    ret |= os04c10_write_reg(dev, 0x381f, 0x08);
    ret |= os04c10_write_reg(dev, 0x3829, 0x03);
    ret |= os04c10_write_reg(dev, 0x3832, 0x00);
    ret |= os04c10_write_reg(dev, 0x3881, 0x00);
    ret |= os04c10_write_reg(dev, 0x3888, 0x04);
    ret |= os04c10_write_reg(dev, 0x388b, 0x00);
    ret |= os04c10_write_reg(dev, 0x3c80, 0x10);
    ret |= os04c10_write_reg(dev, 0x3c86, 0x00);
    ret |= os04c10_write_reg(dev, 0x3c9f, 0x01);
    ret |= os04c10_write_reg(dev, 0x3d85, 0x1b);
    ret |= os04c10_write_reg(dev, 0x3d8c, 0x71);
    ret |= os04c10_write_reg(dev, 0x3d8d, 0xe2);
    ret |= os04c10_write_reg(dev, 0x3f00, 0x0b);
    ret |= os04c10_write_reg(dev, 0x3f06, 0x04);
    ret |= os04c10_write_reg(dev, 0x400a, 0x01);
    ret |= os04c10_write_reg(dev, 0x400b, 0x50);
    ret |= os04c10_write_reg(dev, 0x400e, 0x08);
    ret |= os04c10_write_reg(dev, 0x4040, 0x00);
    ret |= os04c10_write_reg(dev, 0x4041, 0x07);
    ret |= os04c10_write_reg(dev, 0x4043, 0x7e);
    ret |= os04c10_write_reg(dev, 0x4045, 0x7e);
    ret |= os04c10_write_reg(dev, 0x4047, 0x7e);
    ret |= os04c10_write_reg(dev, 0x4049, 0x7e);
    ret |= os04c10_write_reg(dev, 0x4090, 0x04);
    ret |= os04c10_write_reg(dev, 0x40b0, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b1, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b2, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b3, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b4, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b5, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b7, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b8, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b9, 0x00);
    ret |= os04c10_write_reg(dev, 0x40ba, 0x00);
    ret |= os04c10_write_reg(dev, 0x4301, 0x00);
    ret |= os04c10_write_reg(dev, 0x4303, 0x00);
    ret |= os04c10_write_reg(dev, 0x4502, 0x04);
    ret |= os04c10_write_reg(dev, 0x4503, 0x00);
    ret |= os04c10_write_reg(dev, 0x4504, 0x06);
    ret |= os04c10_write_reg(dev, 0x4506, 0x00);
    ret |= os04c10_write_reg(dev, 0x4507, 0x64);
    ret |= os04c10_write_reg(dev, 0x4803, 0x00);
    ret |= os04c10_write_reg(dev, 0x480c, 0x32);
    ret |= os04c10_write_reg(dev, 0x480e, 0x00);
    ret |= os04c10_write_reg(dev, 0x4813, 0x00);
    ret |= os04c10_write_reg(dev, 0x4819, 0x70);
    ret |= os04c10_write_reg(dev, 0x481f, 0x30);
    ret |= os04c10_write_reg(dev, 0x4823, 0x3f);
    ret |= os04c10_write_reg(dev, 0x4825, 0x30);
    ret |= os04c10_write_reg(dev, 0x4833, 0x10);
    ret |= os04c10_write_reg(dev, 0x484b, 0x07);
    ret |= os04c10_write_reg(dev, 0x488b, 0x00);
    ret |= os04c10_write_reg(dev, 0x4d00, 0x04);
    ret |= os04c10_write_reg(dev, 0x4d01, 0xad);
    ret |= os04c10_write_reg(dev, 0x4d02, 0xbc);
    ret |= os04c10_write_reg(dev, 0x4d03, 0xa1);
    ret |= os04c10_write_reg(dev, 0x4d04, 0x1f);
    ret |= os04c10_write_reg(dev, 0x4d05, 0x4c);
    ret |= os04c10_write_reg(dev, 0x4d0b, 0x01);
    ret |= os04c10_write_reg(dev, 0x4e00, 0x2a);
    ret |= os04c10_write_reg(dev, 0x4e0d, 0x00);
    ret |= os04c10_write_reg(dev, 0x5001, 0x09);
    ret |= os04c10_write_reg(dev, 0x5004, 0x00);
    ret |= os04c10_write_reg(dev, 0x5080, 0x04);
    ret |= os04c10_write_reg(dev, 0x5036, 0x00);
    ret |= os04c10_write_reg(dev, 0x5180, 0x70);
    ret |= os04c10_write_reg(dev, 0x5181, 0x10);
    ret |= os04c10_write_reg(dev, 0x520a, 0x03);
    ret |= os04c10_write_reg(dev, 0x520b, 0x06);
    ret |= os04c10_write_reg(dev, 0x520c, 0x0c);
    ret |= os04c10_write_reg(dev, 0x580b, 0x0f);
    ret |= os04c10_write_reg(dev, 0x580d, 0x00);
    ret |= os04c10_write_reg(dev, 0x580f, 0x00);
    ret |= os04c10_write_reg(dev, 0x5820, 0x00);
    ret |= os04c10_write_reg(dev, 0x5821, 0x00);
    ret |= os04c10_write_reg(dev, 0x301c, 0xf8);
    ret |= os04c10_write_reg(dev, 0x301e, 0xb4);
    ret |= os04c10_write_reg(dev, 0x301f, 0xd0);
    ret |= os04c10_write_reg(dev, 0x3022, 0x61);
    ret |= os04c10_write_reg(dev, 0x3109, 0xe7);
    ret |= os04c10_write_reg(dev, 0x3600, 0x00);
    ret |= os04c10_write_reg(dev, 0x3610, 0x95);
    ret |= os04c10_write_reg(dev, 0x3611, 0x85);
    ret |= os04c10_write_reg(dev, 0x3613, 0x3a);
    ret |= os04c10_write_reg(dev, 0x3615, 0x60);
    ret |= os04c10_write_reg(dev, 0x3621, 0xb0);
    ret |= os04c10_write_reg(dev, 0x3620, 0x0c);
    ret |= os04c10_write_reg(dev, 0x3629, 0x00);
    ret |= os04c10_write_reg(dev, 0x3661, 0x04);
    ret |= os04c10_write_reg(dev, 0x3662, 0x10);
    ret |= os04c10_write_reg(dev, 0x3664, 0x70);
    ret |= os04c10_write_reg(dev, 0x3665, 0x00);
    ret |= os04c10_write_reg(dev, 0x3681, 0xa6);
    ret |= os04c10_write_reg(dev, 0x3682, 0x53);
    ret |= os04c10_write_reg(dev, 0x3683, 0x2a);
    ret |= os04c10_write_reg(dev, 0x3684, 0x15);
    ret |= os04c10_write_reg(dev, 0x3700, 0x2a);
    ret |= os04c10_write_reg(dev, 0x3701, 0x12);
    ret |= os04c10_write_reg(dev, 0x3703, 0x28);
    ret |= os04c10_write_reg(dev, 0x3704, 0x0e);
    ret |= os04c10_write_reg(dev, 0x3706, 0x9d);
    ret |= os04c10_write_reg(dev, 0x3709, 0x4a);
    ret |= os04c10_write_reg(dev, 0x370b, 0x48);
    ret |= os04c10_write_reg(dev, 0x370c, 0x01);
    ret |= os04c10_write_reg(dev, 0x370f, 0x04);
    ret |= os04c10_write_reg(dev, 0x3714, 0x24);
    ret |= os04c10_write_reg(dev, 0x3716, 0x24);
    ret |= os04c10_write_reg(dev, 0x3719, 0x11);
    ret |= os04c10_write_reg(dev, 0x371a, 0x1e);
    ret |= os04c10_write_reg(dev, 0x3720, 0x00);
    ret |= os04c10_write_reg(dev, 0x3724, 0x13);
    ret |= os04c10_write_reg(dev, 0x373f, 0xb0);
    ret |= os04c10_write_reg(dev, 0x3741, 0x9d);
    ret |= os04c10_write_reg(dev, 0x3743, 0x9d);
    ret |= os04c10_write_reg(dev, 0x3745, 0x9d);
    ret |= os04c10_write_reg(dev, 0x3747, 0x9d);
    ret |= os04c10_write_reg(dev, 0x3749, 0x48);
    ret |= os04c10_write_reg(dev, 0x374b, 0x48);
    ret |= os04c10_write_reg(dev, 0x374d, 0x48);
    ret |= os04c10_write_reg(dev, 0x374f, 0x48);
    ret |= os04c10_write_reg(dev, 0x3755, 0x10);
    ret |= os04c10_write_reg(dev, 0x376c, 0x00);
    ret |= os04c10_write_reg(dev, 0x378d, 0x3c);
    ret |= os04c10_write_reg(dev, 0x3790, 0x01);
    ret |= os04c10_write_reg(dev, 0x3791, 0x01);
    ret |= os04c10_write_reg(dev, 0x3798, 0x40);
    ret |= os04c10_write_reg(dev, 0x379e, 0x00);
    ret |= os04c10_write_reg(dev, 0x379f, 0x04);
    ret |= os04c10_write_reg(dev, 0x37a1, 0x10);
    ret |= os04c10_write_reg(dev, 0x37a2, 0x1e);
    ret |= os04c10_write_reg(dev, 0x37a8, 0x10);
    ret |= os04c10_write_reg(dev, 0x37a9, 0x1e);
    ret |= os04c10_write_reg(dev, 0x37ac, 0xa0);
    ret |= os04c10_write_reg(dev, 0x37b9, 0x01);
    ret |= os04c10_write_reg(dev, 0x37bd, 0x01);
    ret |= os04c10_write_reg(dev, 0x37bf, 0x26);
    ret |= os04c10_write_reg(dev, 0x37c0, 0x11);
    ret |= os04c10_write_reg(dev, 0x37c2, 0x04);
    ret |= os04c10_write_reg(dev, 0x37cd, 0x19);
    ret |= os04c10_write_reg(dev, 0x37d8, 0x02);
    ret |= os04c10_write_reg(dev, 0x37d9, 0x08);
    ret |= os04c10_write_reg(dev, 0x37e5, 0x02);
    ret |= os04c10_write_reg(dev, 0x3800, 0x00);
    ret |= os04c10_write_reg(dev, 0x3801, 0x00);
    ret |= os04c10_write_reg(dev, 0x3802, 0x00);
    ret |= os04c10_write_reg(dev, 0x3803, 0x00);
    ret |= os04c10_write_reg(dev, 0x3804, 0x0a);
    ret |= os04c10_write_reg(dev, 0x3805, 0x8f);
    ret |= os04c10_write_reg(dev, 0x3806, 0x05);
    ret |= os04c10_write_reg(dev, 0x3807, 0xff);
    ret |= os04c10_write_reg(dev, 0x3808, 0x0a);
    ret |= os04c10_write_reg(dev, 0x3809, 0x80);
    ret |= os04c10_write_reg(dev, 0x380a, 0x05);
    ret |= os04c10_write_reg(dev, 0x380b, 0xf0);
    ret |= os04c10_write_reg(dev, 0x380c, 0x08);
    ret |= os04c10_write_reg(dev, 0x380d, 0x5c);
    ret |= os04c10_write_reg(dev, 0x380e, 0x06);
    ret |= os04c10_write_reg(dev, 0x380f, 0x26);
    ret |= os04c10_write_reg(dev, 0x3811, 0x08);
    ret |= os04c10_write_reg(dev, 0x3813, 0x08);
    ret |= os04c10_write_reg(dev, 0x3814, 0x01);
    ret |= os04c10_write_reg(dev, 0x3815, 0x01);
    ret |= os04c10_write_reg(dev, 0x3816, 0x01);
    ret |= os04c10_write_reg(dev, 0x3817, 0x01);
    ret |= os04c10_write_reg(dev, 0x3820, 0x88);
    ret |= os04c10_write_reg(dev, 0x3821, 0x00);
    ret |= os04c10_write_reg(dev, 0x3880, 0x25);
    ret |= os04c10_write_reg(dev, 0x3882, 0x20);
    ret |= os04c10_write_reg(dev, 0x3c91, 0x0b);
    ret |= os04c10_write_reg(dev, 0x3c94, 0x45);
    ret |= os04c10_write_reg(dev, 0x4000, 0xf3);
    ret |= os04c10_write_reg(dev, 0x4001, 0x60);
    ret |= os04c10_write_reg(dev, 0x4003, 0x80);
    ret |= os04c10_write_reg(dev, 0x4008, 0x02);
    ret |= os04c10_write_reg(dev, 0x4009, 0x0d);
    ret |= os04c10_write_reg(dev, 0x4300, 0xff);
    ret |= os04c10_write_reg(dev, 0x4302, 0x0f);
    ret |= os04c10_write_reg(dev, 0x4305, 0x83);
    ret |= os04c10_write_reg(dev, 0x4505, 0x84);
    ret |= os04c10_write_reg(dev, 0x4809, 0x1e);
    ret |= os04c10_write_reg(dev, 0x480a, 0x04);
    ret |= os04c10_write_reg(dev, 0x4837, 0x23);
    ret |= os04c10_write_reg(dev, 0x4c00, 0x08);
    ret |= os04c10_write_reg(dev, 0x4c01, 0x08);
    ret |= os04c10_write_reg(dev, 0x4c04, 0x00);
    ret |= os04c10_write_reg(dev, 0x4c05, 0x00);
    ret |= os04c10_write_reg(dev, 0x5000, 0xe9);
    ret |= os04c10_write_reg(dev, 0x0100, 0x01);

    // 1000 PSRR_ON HCG
    ret |= os04c10_write_reg(dev, 0x3798, 0x40);

    // 1001 PSRR_ON LCG
    ret |= os04c10_write_reg(dev, 0x3798, 0xc0);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os04c10_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("==== OS04C10_24MInput_MIPI_4lane_12bit_linear_2688x1520_30fps init success!============\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_2lane_linear_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os04c10_write_reg(dev, 0x0100, 0x00);
    ret |= os04c10_write_reg(dev, 0x0103, 0x01);
    ret |= os04c10_write_reg(dev, 0x0301, 0x84);
    ret |= os04c10_write_reg(dev, 0x0303, 0x01);
    ret |= os04c10_write_reg(dev, 0x0305, 0x5b);
    ret |= os04c10_write_reg(dev, 0x0306, 0x01);
    ret |= os04c10_write_reg(dev, 0x0307, 0x17);
    ret |= os04c10_write_reg(dev, 0x0323, 0x04);
    ret |= os04c10_write_reg(dev, 0x0324, 0x01);
    ret |= os04c10_write_reg(dev, 0x0325, 0x62);
    ret |= os04c10_write_reg(dev, 0x3012, 0x06);
    ret |= os04c10_write_reg(dev, 0x3013, 0x02);
    ret |= os04c10_write_reg(dev, 0x3016, 0x32);
    ret |= os04c10_write_reg(dev, 0x3021, 0x03);
    ret |= os04c10_write_reg(dev, 0x3106, 0x25);
    ret |= os04c10_write_reg(dev, 0x3107, 0xa1);
    ret |= os04c10_write_reg(dev, 0x3500, 0x00);
    ret |= os04c10_write_reg(dev, 0x3501, 0x04);
    ret |= os04c10_write_reg(dev, 0x3502, 0x40);
    ret |= os04c10_write_reg(dev, 0x3503, 0x88);
    ret |= os04c10_write_reg(dev, 0x3508, 0x00);
    ret |= os04c10_write_reg(dev, 0x3509, 0x80);
    ret |= os04c10_write_reg(dev, 0x350a, 0x04);
    ret |= os04c10_write_reg(dev, 0x350b, 0x00);
    ret |= os04c10_write_reg(dev, 0x350c, 0x00);
    ret |= os04c10_write_reg(dev, 0x350d, 0x80);
    ret |= os04c10_write_reg(dev, 0x350e, 0x04);
    ret |= os04c10_write_reg(dev, 0x350f, 0x00);
    ret |= os04c10_write_reg(dev, 0x3510, 0x00);
    ret |= os04c10_write_reg(dev, 0x3511, 0x01);
    ret |= os04c10_write_reg(dev, 0x3512, 0x20);
    ret |= os04c10_write_reg(dev, 0x3624, 0x02);
    ret |= os04c10_write_reg(dev, 0x3625, 0x4c);
    ret |= os04c10_write_reg(dev, 0x3660, 0x00);
    ret |= os04c10_write_reg(dev, 0x3666, 0xa5);
    ret |= os04c10_write_reg(dev, 0x3667, 0xa5);
    ret |= os04c10_write_reg(dev, 0x366a, 0x64);
    ret |= os04c10_write_reg(dev, 0x3673, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3672, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3671, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3670, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3685, 0x00);
    ret |= os04c10_write_reg(dev, 0x3694, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3693, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3692, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3691, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3696, 0x4c);
    ret |= os04c10_write_reg(dev, 0x3697, 0x4c);
    ret |= os04c10_write_reg(dev, 0x3698, 0x40);
    ret |= os04c10_write_reg(dev, 0x3699, 0x80);
    ret |= os04c10_write_reg(dev, 0x369a, 0x18);
    ret |= os04c10_write_reg(dev, 0x369b, 0x1f);
    ret |= os04c10_write_reg(dev, 0x369c, 0x14);
    ret |= os04c10_write_reg(dev, 0x369d, 0x80);
    ret |= os04c10_write_reg(dev, 0x369e, 0x40);
    ret |= os04c10_write_reg(dev, 0x369f, 0x21);
    ret |= os04c10_write_reg(dev, 0x36a0, 0x12);
    ret |= os04c10_write_reg(dev, 0x36a1, 0x5d);
    ret |= os04c10_write_reg(dev, 0x36a2, 0x66);
    ret |= os04c10_write_reg(dev, 0x370a, 0x00);
    ret |= os04c10_write_reg(dev, 0x370e, 0x0c);
    ret |= os04c10_write_reg(dev, 0x3710, 0x00);
    ret |= os04c10_write_reg(dev, 0x3713, 0x00);
    ret |= os04c10_write_reg(dev, 0x3725, 0x02);
    ret |= os04c10_write_reg(dev, 0x372a, 0x03);
    ret |= os04c10_write_reg(dev, 0x3738, 0xce);
    ret |= os04c10_write_reg(dev, 0x3748, 0x00);
    ret |= os04c10_write_reg(dev, 0x374a, 0x00);
    ret |= os04c10_write_reg(dev, 0x374c, 0x00);
    ret |= os04c10_write_reg(dev, 0x374e, 0x00);
    ret |= os04c10_write_reg(dev, 0x3756, 0x00);
    ret |= os04c10_write_reg(dev, 0x3757, 0x0e);
    ret |= os04c10_write_reg(dev, 0x3767, 0x00);
    ret |= os04c10_write_reg(dev, 0x3771, 0x00);
    ret |= os04c10_write_reg(dev, 0x377b, 0x20);
    ret |= os04c10_write_reg(dev, 0x377c, 0x00);
    ret |= os04c10_write_reg(dev, 0x377d, 0x0c);
    ret |= os04c10_write_reg(dev, 0x3781, 0x03);
    ret |= os04c10_write_reg(dev, 0x3782, 0x00);
    ret |= os04c10_write_reg(dev, 0x3789, 0x14);
    ret |= os04c10_write_reg(dev, 0x3795, 0x02);
    ret |= os04c10_write_reg(dev, 0x379c, 0x00);
    ret |= os04c10_write_reg(dev, 0x379d, 0x00);
    ret |= os04c10_write_reg(dev, 0x37b8, 0x04);
    ret |= os04c10_write_reg(dev, 0x37ba, 0x03);
    ret |= os04c10_write_reg(dev, 0x37bb, 0x00);
    ret |= os04c10_write_reg(dev, 0x37bc, 0x04);
    ret |= os04c10_write_reg(dev, 0x37be, 0x08);
    ret |= os04c10_write_reg(dev, 0x37c4, 0x11);
    ret |= os04c10_write_reg(dev, 0x37c5, 0x80);
    ret |= os04c10_write_reg(dev, 0x37c6, 0x14);
    ret |= os04c10_write_reg(dev, 0x37c7, 0x08);
    ret |= os04c10_write_reg(dev, 0x37da, 0x11);
    ret |= os04c10_write_reg(dev, 0x381f, 0x08);
    ret |= os04c10_write_reg(dev, 0x3829, 0x03);
    ret |= os04c10_write_reg(dev, 0x3881, 0x00);
    ret |= os04c10_write_reg(dev, 0x3888, 0x04);
    ret |= os04c10_write_reg(dev, 0x388b, 0x00);
    ret |= os04c10_write_reg(dev, 0x3c80, 0x10);
    ret |= os04c10_write_reg(dev, 0x3c86, 0x00);
    ret |= os04c10_write_reg(dev, 0x3c8c, 0x20);
    ret |= os04c10_write_reg(dev, 0x3c9f, 0x01);
    ret |= os04c10_write_reg(dev, 0x3d85, 0x1b);
    ret |= os04c10_write_reg(dev, 0x3d8c, 0x71);
    ret |= os04c10_write_reg(dev, 0x3d8d, 0xe2);
    ret |= os04c10_write_reg(dev, 0x3f00, 0x0b);
    ret |= os04c10_write_reg(dev, 0x3f06, 0x04);
    ret |= os04c10_write_reg(dev, 0x400a, 0x01);
    ret |= os04c10_write_reg(dev, 0x400b, 0x50);
    ret |= os04c10_write_reg(dev, 0x400e, 0x08);
    ret |= os04c10_write_reg(dev, 0x4043, 0x7e);
    ret |= os04c10_write_reg(dev, 0x4045, 0x7e);
    ret |= os04c10_write_reg(dev, 0x4047, 0x7e);
    ret |= os04c10_write_reg(dev, 0x4049, 0x7e);
    ret |= os04c10_write_reg(dev, 0x4090, 0x14);
    ret |= os04c10_write_reg(dev, 0x40b0, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b1, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b2, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b3, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b4, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b5, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b7, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b8, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b9, 0x00);
    ret |= os04c10_write_reg(dev, 0x40ba, 0x00);
    ret |= os04c10_write_reg(dev, 0x4301, 0x00);
    ret |= os04c10_write_reg(dev, 0x4303, 0x00);
    ret |= os04c10_write_reg(dev, 0x4502, 0x04);
    ret |= os04c10_write_reg(dev, 0x4503, 0x00);
    ret |= os04c10_write_reg(dev, 0x4504, 0x06);
    ret |= os04c10_write_reg(dev, 0x4506, 0x00);
    ret |= os04c10_write_reg(dev, 0x4507, 0x64);
    ret |= os04c10_write_reg(dev, 0x4803, 0x10);
    ret |= os04c10_write_reg(dev, 0x480c, 0x32);
    ret |= os04c10_write_reg(dev, 0x480e, 0x00);
    ret |= os04c10_write_reg(dev, 0x4813, 0x00);
    ret |= os04c10_write_reg(dev, 0x4819, 0x70);
    ret |= os04c10_write_reg(dev, 0x481f, 0x30);
    ret |= os04c10_write_reg(dev, 0x4823, 0x3c);
    ret |= os04c10_write_reg(dev, 0x4825, 0x32);
    ret |= os04c10_write_reg(dev, 0x4833, 0x10);
    ret |= os04c10_write_reg(dev, 0x484b, 0x07);
    ret |= os04c10_write_reg(dev, 0x488b, 0x00);
    ret |= os04c10_write_reg(dev, 0x4d00, 0x04);
    ret |= os04c10_write_reg(dev, 0x4d01, 0xad);
    ret |= os04c10_write_reg(dev, 0x4d02, 0xbc);
    ret |= os04c10_write_reg(dev, 0x4d03, 0xa1);
    ret |= os04c10_write_reg(dev, 0x4d04, 0x1f);
    ret |= os04c10_write_reg(dev, 0x4d05, 0x4c);
    ret |= os04c10_write_reg(dev, 0x4d0b, 0x01);
    ret |= os04c10_write_reg(dev, 0x4e00, 0x2a);
    ret |= os04c10_write_reg(dev, 0x4e0d, 0x00);
    ret |= os04c10_write_reg(dev, 0x5001, 0x09);
    ret |= os04c10_write_reg(dev, 0x5004, 0x00);
    ret |= os04c10_write_reg(dev, 0x5080, 0x04);
    ret |= os04c10_write_reg(dev, 0x5036, 0x00);
    ret |= os04c10_write_reg(dev, 0x5180, 0x70);
    ret |= os04c10_write_reg(dev, 0x5181, 0x10);
    ret |= os04c10_write_reg(dev, 0x520a, 0x03);
    ret |= os04c10_write_reg(dev, 0x520b, 0x06);
    ret |= os04c10_write_reg(dev, 0x520c, 0x0c);
    ret |= os04c10_write_reg(dev, 0x580b, 0x0f);
    ret |= os04c10_write_reg(dev, 0x580d, 0x00);
    ret |= os04c10_write_reg(dev, 0x580f, 0x00);
    ret |= os04c10_write_reg(dev, 0x5820, 0x00);
    ret |= os04c10_write_reg(dev, 0x5821, 0x00);
    ret |= os04c10_write_reg(dev, 0x301c, 0xf0);
    ret |= os04c10_write_reg(dev, 0x301e, 0xb4);
    ret |= os04c10_write_reg(dev, 0x301f, 0xd0);
    ret |= os04c10_write_reg(dev, 0x3022, 0x01);
    ret |= os04c10_write_reg(dev, 0x3109, 0xe7);
    ret |= os04c10_write_reg(dev, 0x3600, 0x00);
    ret |= os04c10_write_reg(dev, 0x3610, 0x65);
    ret |= os04c10_write_reg(dev, 0x3611, 0x85);
    ret |= os04c10_write_reg(dev, 0x3613, 0x3a);
    ret |= os04c10_write_reg(dev, 0x3615, 0x60);
    ret |= os04c10_write_reg(dev, 0x3621, 0x90);
    ret |= os04c10_write_reg(dev, 0x3620, 0x0c);
    ret |= os04c10_write_reg(dev, 0x3629, 0x00);
    ret |= os04c10_write_reg(dev, 0x3661, 0x04);
    ret |= os04c10_write_reg(dev, 0x3664, 0x70);
    ret |= os04c10_write_reg(dev, 0x3665, 0x00);
    ret |= os04c10_write_reg(dev, 0x3681, 0xa6);
    ret |= os04c10_write_reg(dev, 0x3682, 0x53);
    ret |= os04c10_write_reg(dev, 0x3683, 0x2a);
    ret |= os04c10_write_reg(dev, 0x3684, 0x15);
    ret |= os04c10_write_reg(dev, 0x3700, 0x2a);
    ret |= os04c10_write_reg(dev, 0x3701, 0x12);
    ret |= os04c10_write_reg(dev, 0x3703, 0x28);
    ret |= os04c10_write_reg(dev, 0x3704, 0x0e);
    ret |= os04c10_write_reg(dev, 0x3706, 0x4a);
    ret |= os04c10_write_reg(dev, 0x3709, 0x4a);
    ret |= os04c10_write_reg(dev, 0x370b, 0xa2);
    ret |= os04c10_write_reg(dev, 0x370c, 0x01);
    ret |= os04c10_write_reg(dev, 0x370f, 0x04);
    ret |= os04c10_write_reg(dev, 0x3714, 0x24);
    ret |= os04c10_write_reg(dev, 0x3716, 0x24);
    ret |= os04c10_write_reg(dev, 0x3719, 0x11);
    ret |= os04c10_write_reg(dev, 0x371a, 0x1e);
    ret |= os04c10_write_reg(dev, 0x3720, 0x00);
    ret |= os04c10_write_reg(dev, 0x3724, 0x13);
    ret |= os04c10_write_reg(dev, 0x373f, 0xb0);
    ret |= os04c10_write_reg(dev, 0x3741, 0x4a);
    ret |= os04c10_write_reg(dev, 0x3743, 0x4a);
    ret |= os04c10_write_reg(dev, 0x3745, 0x4a);
    ret |= os04c10_write_reg(dev, 0x3747, 0x4a);
    ret |= os04c10_write_reg(dev, 0x3749, 0xa2);
    ret |= os04c10_write_reg(dev, 0x374b, 0xa2);
    ret |= os04c10_write_reg(dev, 0x374d, 0xa2);
    ret |= os04c10_write_reg(dev, 0x374f, 0xa2);
    ret |= os04c10_write_reg(dev, 0x3755, 0x10);
    ret |= os04c10_write_reg(dev, 0x376c, 0x00);
    ret |= os04c10_write_reg(dev, 0x378d, 0x30);
    ret |= os04c10_write_reg(dev, 0x3790, 0x4a);
    ret |= os04c10_write_reg(dev, 0x3791, 0xa2);
    ret |= os04c10_write_reg(dev, 0x3798, 0x40);
    ret |= os04c10_write_reg(dev, 0x379e, 0x00);
    ret |= os04c10_write_reg(dev, 0x379f, 0x04);
    ret |= os04c10_write_reg(dev, 0x37a1, 0x10);
    ret |= os04c10_write_reg(dev, 0x37a2, 0x1e);
    ret |= os04c10_write_reg(dev, 0x37a8, 0x10);
    ret |= os04c10_write_reg(dev, 0x37a9, 0x1e);
    ret |= os04c10_write_reg(dev, 0x37ac, 0xa0);
    ret |= os04c10_write_reg(dev, 0x37b9, 0x01);
    ret |= os04c10_write_reg(dev, 0x37bd, 0x01);
    ret |= os04c10_write_reg(dev, 0x37bf, 0x26);
    ret |= os04c10_write_reg(dev, 0x37c0, 0x11);
    ret |= os04c10_write_reg(dev, 0x37c2, 0x04);
    ret |= os04c10_write_reg(dev, 0x37cd, 0x19);
    ret |= os04c10_write_reg(dev, 0x37e0, 0x08);
    ret |= os04c10_write_reg(dev, 0x37e6, 0x04);
    ret |= os04c10_write_reg(dev, 0x37e5, 0x02);
    ret |= os04c10_write_reg(dev, 0x37e1, 0x0c);
    ret |= os04c10_write_reg(dev, 0x3737, 0x04);
    ret |= os04c10_write_reg(dev, 0x37d8, 0x02);
    ret |= os04c10_write_reg(dev, 0x37e2, 0x10);
    ret |= os04c10_write_reg(dev, 0x3739, 0x10);
    ret |= os04c10_write_reg(dev, 0x3662, 0x10);
    ret |= os04c10_write_reg(dev, 0x37e4, 0x20);
    ret |= os04c10_write_reg(dev, 0x37e3, 0x08);
    ret |= os04c10_write_reg(dev, 0x37d9, 0x08);
    ret |= os04c10_write_reg(dev, 0x4040, 0x00);
    ret |= os04c10_write_reg(dev, 0x4041, 0x07);
    ret |= os04c10_write_reg(dev, 0x4008, 0x02);
    ret |= os04c10_write_reg(dev, 0x4009, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3800, 0x00);
    ret |= os04c10_write_reg(dev, 0x3801, 0x00);
    ret |= os04c10_write_reg(dev, 0x3802, 0x00);
    ret |= os04c10_write_reg(dev, 0x3803, 0x00);
    ret |= os04c10_write_reg(dev, 0x3804, 0x0a);
    ret |= os04c10_write_reg(dev, 0x3805, 0x8f);
    ret |= os04c10_write_reg(dev, 0x3806, 0x05);
    ret |= os04c10_write_reg(dev, 0x3807, 0xff);
    ret |= os04c10_write_reg(dev, 0x3808, 0x0a);
    ret |= os04c10_write_reg(dev, 0x3809, 0x80);
    ret |= os04c10_write_reg(dev, 0x380a, 0x05);
    ret |= os04c10_write_reg(dev, 0x380b, 0xf0);
    ret |= os04c10_write_reg(dev, 0x380c, 0x08);
    ret |= os04c10_write_reg(dev, 0x380d, 0x5c);
    ret |= os04c10_write_reg(dev, 0x380e, 0x06);
    ret |= os04c10_write_reg(dev, 0x380f, 0x26);
    ret |= os04c10_write_reg(dev, 0x3811, 0x08);
    ret |= os04c10_write_reg(dev, 0x3813, 0x08);
    ret |= os04c10_write_reg(dev, 0x3814, 0x01);
    ret |= os04c10_write_reg(dev, 0x3815, 0x01);
    ret |= os04c10_write_reg(dev, 0x3816, 0x01);
    ret |= os04c10_write_reg(dev, 0x3817, 0x01);
    ret |= os04c10_write_reg(dev, 0x3820, 0x88);
    ret |= os04c10_write_reg(dev, 0x3821, 0x00);
    ret |= os04c10_write_reg(dev, 0x3880, 0x25);
    ret |= os04c10_write_reg(dev, 0x3882, 0x20);
    ret |= os04c10_write_reg(dev, 0x3c91, 0x0b);
    ret |= os04c10_write_reg(dev, 0x3c94, 0x45);
    ret |= os04c10_write_reg(dev, 0x4000, 0xf3);
    ret |= os04c10_write_reg(dev, 0x4001, 0x60);
    ret |= os04c10_write_reg(dev, 0x4003, 0x40);
    ret |= os04c10_write_reg(dev, 0x4300, 0xff);
    ret |= os04c10_write_reg(dev, 0x4302, 0x0f);
    ret |= os04c10_write_reg(dev, 0x4305, 0x83);
    ret |= os04c10_write_reg(dev, 0x4505, 0x84);
    ret |= os04c10_write_reg(dev, 0x4809, 0x1e);
    ret |= os04c10_write_reg(dev, 0x480a, 0x04);
    ret |= os04c10_write_reg(dev, 0x4837, 0x15);
    ret |= os04c10_write_reg(dev, 0x4c00, 0x08);
    ret |= os04c10_write_reg(dev, 0x4c01, 0x00);
    ret |= os04c10_write_reg(dev, 0x4c04, 0x00);
    ret |= os04c10_write_reg(dev, 0x4c05, 0x00);
    ret |= os04c10_write_reg(dev, 0x5000, 0xf9);
    ret |= os04c10_write_reg(dev, 0x3624, 0x00);
    ret |= os04c10_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os04c10_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("========== OS04C10_24MInput_MIPI_2lane_10bit_linear_2688x1520_30fps init success!======\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os04c10_2lane_wdr_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os04c10_write_reg(dev, 0x0103, 0x01);
    ret |= os04c10_write_reg(dev, 0x0301, 0x84);
    ret |= os04c10_write_reg(dev, 0x0303, 0x01);
    ret |= os04c10_write_reg(dev, 0x0305, 0x61);
    ret |= os04c10_write_reg(dev, 0x0306, 0x01);
    ret |= os04c10_write_reg(dev, 0x0307, 0x17);
    ret |= os04c10_write_reg(dev, 0x0323, 0x04);
    ret |= os04c10_write_reg(dev, 0x0324, 0x01);
    ret |= os04c10_write_reg(dev, 0x0325, 0x7a);
    ret |= os04c10_write_reg(dev, 0x3012, 0x06);
    ret |= os04c10_write_reg(dev, 0x3013, 0x02);
    ret |= os04c10_write_reg(dev, 0x3016, 0x72);
    ret |= os04c10_write_reg(dev, 0x3021, 0x03);
    ret |= os04c10_write_reg(dev, 0x3106, 0x21);
    ret |= os04c10_write_reg(dev, 0x3107, 0xa1);
    ret |= os04c10_write_reg(dev, 0x3500, 0x00);
    // ret |= os04c10_write_reg(dev, 0x3501, 0x03); // long exp_H
    // ret |= os04c10_write_reg(dev, 0x3502, 0x08); // long exp_L
    ret |= os04c10_write_reg(dev, 0x3501, 0x04);
    ret |= os04c10_write_reg(dev, 0x3502, 0x20);

    ret |= os04c10_write_reg(dev, 0x3503, 0x88);
    ret |= os04c10_write_reg(dev, 0x3508, 0x00);
    ret |= os04c10_write_reg(dev, 0x3509, 0x80);
    ret |= os04c10_write_reg(dev, 0x350a, 0x04);
    ret |= os04c10_write_reg(dev, 0x350b, 0x00);
    ret |= os04c10_write_reg(dev, 0x350c, 0x00);
    ret |= os04c10_write_reg(dev, 0x350d, 0x80);
    ret |= os04c10_write_reg(dev, 0x350e, 0x04);
    ret |= os04c10_write_reg(dev, 0x350f, 0x00);
    ret |= os04c10_write_reg(dev, 0x3510, 0x00);
    ret |= os04c10_write_reg(dev, 0x3511, 0x01); // short exp_H
    ret |= os04c10_write_reg(dev, 0x3512, 0x08); // short exp_L
    ret |= os04c10_write_reg(dev, 0x3624, 0x02);
    ret |= os04c10_write_reg(dev, 0x3625, 0x4c);
    ret |= os04c10_write_reg(dev, 0x3660, 0x04);
    ret |= os04c10_write_reg(dev, 0x3666, 0xa5);
    ret |= os04c10_write_reg(dev, 0x3667, 0xa5);
    ret |= os04c10_write_reg(dev, 0x366a, 0x54);
    ret |= os04c10_write_reg(dev, 0x3673, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3672, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3671, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3670, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3685, 0x00);
    ret |= os04c10_write_reg(dev, 0x3694, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3693, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3692, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3691, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3696, 0x4c);
    ret |= os04c10_write_reg(dev, 0x3697, 0x4c);
    ret |= os04c10_write_reg(dev, 0x3698, 0x40);
    ret |= os04c10_write_reg(dev, 0x3699, 0x80);
    ret |= os04c10_write_reg(dev, 0x369a, 0x18);
    ret |= os04c10_write_reg(dev, 0x369b, 0x1f);
    ret |= os04c10_write_reg(dev, 0x369c, 0x14);
    ret |= os04c10_write_reg(dev, 0x369d, 0x80);
    ret |= os04c10_write_reg(dev, 0x369e, 0x40);
    ret |= os04c10_write_reg(dev, 0x369f, 0x21);
    ret |= os04c10_write_reg(dev, 0x36a0, 0x12);
    ret |= os04c10_write_reg(dev, 0x36a1, 0x5d);
    ret |= os04c10_write_reg(dev, 0x36a2, 0x66);
    ret |= os04c10_write_reg(dev, 0x370a, 0x00);
    ret |= os04c10_write_reg(dev, 0x370e, 0x0c);
    ret |= os04c10_write_reg(dev, 0x3710, 0x00);
    ret |= os04c10_write_reg(dev, 0x3713, 0x00);
    ret |= os04c10_write_reg(dev, 0x3725, 0x02);
    ret |= os04c10_write_reg(dev, 0x372a, 0x03);
    ret |= os04c10_write_reg(dev, 0x3738, 0xce);
    ret |= os04c10_write_reg(dev, 0x3748, 0x00);
    ret |= os04c10_write_reg(dev, 0x374a, 0x00);
    ret |= os04c10_write_reg(dev, 0x374c, 0x00);
    ret |= os04c10_write_reg(dev, 0x374e, 0x00);
    ret |= os04c10_write_reg(dev, 0x3756, 0x00);
    ret |= os04c10_write_reg(dev, 0x3757, 0x00);
    ret |= os04c10_write_reg(dev, 0x3767, 0x00);
    ret |= os04c10_write_reg(dev, 0x3771, 0x00);
    ret |= os04c10_write_reg(dev, 0x377b, 0x28);
    ret |= os04c10_write_reg(dev, 0x377c, 0x00);
    ret |= os04c10_write_reg(dev, 0x377d, 0x0c);
    ret |= os04c10_write_reg(dev, 0x3781, 0x03);
    ret |= os04c10_write_reg(dev, 0x3782, 0x00);
    ret |= os04c10_write_reg(dev, 0x3789, 0x14);
    ret |= os04c10_write_reg(dev, 0x3795, 0x02);
    ret |= os04c10_write_reg(dev, 0x379c, 0x00);
    ret |= os04c10_write_reg(dev, 0x379d, 0x00);
    ret |= os04c10_write_reg(dev, 0x37b8, 0x04);
    ret |= os04c10_write_reg(dev, 0x37ba, 0x03);
    ret |= os04c10_write_reg(dev, 0x37bb, 0x00);
    ret |= os04c10_write_reg(dev, 0x37bc, 0x04);
    ret |= os04c10_write_reg(dev, 0x37be, 0x08);
    ret |= os04c10_write_reg(dev, 0x37c4, 0x11);
    ret |= os04c10_write_reg(dev, 0x37c5, 0x80);
    ret |= os04c10_write_reg(dev, 0x37c6, 0x14);
    ret |= os04c10_write_reg(dev, 0x37c7, 0x08);
    ret |= os04c10_write_reg(dev, 0x37da, 0x11);
    ret |= os04c10_write_reg(dev, 0x381f, 0x08);
    ret |= os04c10_write_reg(dev, 0x3829, 0x03);
    ret |= os04c10_write_reg(dev, 0x3881, 0x00);
    ret |= os04c10_write_reg(dev, 0x3888, 0x04);
    ret |= os04c10_write_reg(dev, 0x388b, 0x00);
    ret |= os04c10_write_reg(dev, 0x3c80, 0x10);
    ret |= os04c10_write_reg(dev, 0x3c86, 0x00);
    ret |= os04c10_write_reg(dev, 0x3c8c, 0x20);
    ret |= os04c10_write_reg(dev, 0x3c9f, 0x01);
    ret |= os04c10_write_reg(dev, 0x3d85, 0x1b);
    ret |= os04c10_write_reg(dev, 0x3d8c, 0x71);
    ret |= os04c10_write_reg(dev, 0x3d8d, 0xe2);
    ret |= os04c10_write_reg(dev, 0x3f00, 0x0b);
    ret |= os04c10_write_reg(dev, 0x3f06, 0x04);
    ret |= os04c10_write_reg(dev, 0x400a, 0x01);
    ret |= os04c10_write_reg(dev, 0x400b, 0x50);
    ret |= os04c10_write_reg(dev, 0x400e, 0x08);
    ret |= os04c10_write_reg(dev, 0x4043, 0x7e);
    ret |= os04c10_write_reg(dev, 0x4045, 0x7e);
    ret |= os04c10_write_reg(dev, 0x4047, 0x7e);
    ret |= os04c10_write_reg(dev, 0x4049, 0x7e);
    ret |= os04c10_write_reg(dev, 0x4090, 0x14);
    ret |= os04c10_write_reg(dev, 0x40b0, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b1, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b2, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b3, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b4, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b5, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b7, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b8, 0x00);
    ret |= os04c10_write_reg(dev, 0x40b9, 0x00);
    ret |= os04c10_write_reg(dev, 0x40ba, 0x01);
    ret |= os04c10_write_reg(dev, 0x4301, 0x00);
    ret |= os04c10_write_reg(dev, 0x4303, 0x00);
    ret |= os04c10_write_reg(dev, 0x4502, 0x04);
    ret |= os04c10_write_reg(dev, 0x4503, 0x00);
    ret |= os04c10_write_reg(dev, 0x4504, 0x06);
    ret |= os04c10_write_reg(dev, 0x4506, 0x00);
    ret |= os04c10_write_reg(dev, 0x4507, 0x47);
    ret |= os04c10_write_reg(dev, 0x4803, 0x00);
    ret |= os04c10_write_reg(dev, 0x480c, 0x32);
    ret |= os04c10_write_reg(dev, 0x480e, 0x04);
    ret |= os04c10_write_reg(dev, 0x4813, 0xe4);
    ret |= os04c10_write_reg(dev, 0x4819, 0x70);
    ret |= os04c10_write_reg(dev, 0x481f, 0x30);
    ret |= os04c10_write_reg(dev, 0x4823, 0x3f);
    ret |= os04c10_write_reg(dev, 0x4825, 0x30);
    ret |= os04c10_write_reg(dev, 0x4833, 0x10);
    ret |= os04c10_write_reg(dev, 0x484b, 0x27);
    ret |= os04c10_write_reg(dev, 0x488b, 0x00);
    ret |= os04c10_write_reg(dev, 0x4d00, 0x04);
    ret |= os04c10_write_reg(dev, 0x4d01, 0xad);
    ret |= os04c10_write_reg(dev, 0x4d02, 0xbc);
    ret |= os04c10_write_reg(dev, 0x4d03, 0xa1);
    ret |= os04c10_write_reg(dev, 0x4d04, 0x1f);
    ret |= os04c10_write_reg(dev, 0x4d05, 0x4c);
    ret |= os04c10_write_reg(dev, 0x4d0b, 0x01);
    ret |= os04c10_write_reg(dev, 0x4e00, 0x2a);
    ret |= os04c10_write_reg(dev, 0x4e0d, 0x00);
    ret |= os04c10_write_reg(dev, 0x5001, 0x09);
    ret |= os04c10_write_reg(dev, 0x5004, 0x00);
    ret |= os04c10_write_reg(dev, 0x5080, 0x04);
    ret |= os04c10_write_reg(dev, 0x5036, 0x80);
    ret |= os04c10_write_reg(dev, 0x5180, 0x70);
    ret |= os04c10_write_reg(dev, 0x5181, 0x10);
    ret |= os04c10_write_reg(dev, 0x520a, 0x03);
    ret |= os04c10_write_reg(dev, 0x520b, 0x06);
    ret |= os04c10_write_reg(dev, 0x520c, 0x0c);
    ret |= os04c10_write_reg(dev, 0x580b, 0x0f);
    ret |= os04c10_write_reg(dev, 0x580d, 0x00);
    ret |= os04c10_write_reg(dev, 0x580f, 0x00);
    ret |= os04c10_write_reg(dev, 0x5820, 0x00);
    ret |= os04c10_write_reg(dev, 0x5821, 0x00);
    ret |= os04c10_write_reg(dev, 0x301c, 0xf8);
    ret |= os04c10_write_reg(dev, 0x301e, 0xb4);
    ret |= os04c10_write_reg(dev, 0x301f, 0xf0);
    ret |= os04c10_write_reg(dev, 0x3022, 0x01);
    ret |= os04c10_write_reg(dev, 0x3109, 0xe7);
    ret |= os04c10_write_reg(dev, 0x3600, 0x00);
    ret |= os04c10_write_reg(dev, 0x3610, 0x75);
    ret |= os04c10_write_reg(dev, 0x3611, 0x85);
    ret |= os04c10_write_reg(dev, 0x3613, 0x3a);
    ret |= os04c10_write_reg(dev, 0x3615, 0x60);
    ret |= os04c10_write_reg(dev, 0x3621, 0x90);
    ret |= os04c10_write_reg(dev, 0x3620, 0x0c);
    ret |= os04c10_write_reg(dev, 0x3629, 0x00);
    ret |= os04c10_write_reg(dev, 0x3661, 0x04);
    ret |= os04c10_write_reg(dev, 0x3664, 0x70);
    ret |= os04c10_write_reg(dev, 0x3665, 0x00);
    ret |= os04c10_write_reg(dev, 0x3681, 0xa6);
    ret |= os04c10_write_reg(dev, 0x3682, 0x53);
    ret |= os04c10_write_reg(dev, 0x3683, 0x2a);
    ret |= os04c10_write_reg(dev, 0x3684, 0x15);
    ret |= os04c10_write_reg(dev, 0x3700, 0x2a);
    ret |= os04c10_write_reg(dev, 0x3701, 0x12);
    ret |= os04c10_write_reg(dev, 0x3703, 0x28);
    ret |= os04c10_write_reg(dev, 0x3704, 0x0e);
    ret |= os04c10_write_reg(dev, 0x3706, 0x4a);
    ret |= os04c10_write_reg(dev, 0x3709, 0x4a);
    ret |= os04c10_write_reg(dev, 0x370b, 0xa2);
    ret |= os04c10_write_reg(dev, 0x370c, 0x01);
    ret |= os04c10_write_reg(dev, 0x370f, 0x04);
    ret |= os04c10_write_reg(dev, 0x3714, 0x24);
    ret |= os04c10_write_reg(dev, 0x3716, 0x24);
    ret |= os04c10_write_reg(dev, 0x3719, 0x11);
    ret |= os04c10_write_reg(dev, 0x371a, 0x1e);
    ret |= os04c10_write_reg(dev, 0x3720, 0x00);
    ret |= os04c10_write_reg(dev, 0x3724, 0x13);
    ret |= os04c10_write_reg(dev, 0x373f, 0xb0);
    ret |= os04c10_write_reg(dev, 0x3741, 0x4a);
    ret |= os04c10_write_reg(dev, 0x3743, 0x4a);
    ret |= os04c10_write_reg(dev, 0x3745, 0x4a);
    ret |= os04c10_write_reg(dev, 0x3747, 0x4a);
    ret |= os04c10_write_reg(dev, 0x3749, 0xa2);
    ret |= os04c10_write_reg(dev, 0x374b, 0xa2);
    ret |= os04c10_write_reg(dev, 0x374d, 0xa2);
    ret |= os04c10_write_reg(dev, 0x374f, 0xa2);
    ret |= os04c10_write_reg(dev, 0x3755, 0x10);
    ret |= os04c10_write_reg(dev, 0x376c, 0x00);
    ret |= os04c10_write_reg(dev, 0x378d, 0x30);
    ret |= os04c10_write_reg(dev, 0x3790, 0x4a);
    ret |= os04c10_write_reg(dev, 0x3791, 0xa2);
    ret |= os04c10_write_reg(dev, 0x3798, 0x40);
    ret |= os04c10_write_reg(dev, 0x379e, 0x00);
    ret |= os04c10_write_reg(dev, 0x379f, 0x04);
    ret |= os04c10_write_reg(dev, 0x37a1, 0x10);
    ret |= os04c10_write_reg(dev, 0x37a2, 0x1e);
    ret |= os04c10_write_reg(dev, 0x37a8, 0x10);
    ret |= os04c10_write_reg(dev, 0x37a9, 0x1e);
    ret |= os04c10_write_reg(dev, 0x37ac, 0xa0);
    ret |= os04c10_write_reg(dev, 0x37b9, 0x01);
    ret |= os04c10_write_reg(dev, 0x37bd, 0x01);
    ret |= os04c10_write_reg(dev, 0x37bf, 0x26);
    ret |= os04c10_write_reg(dev, 0x37c0, 0x11);
    ret |= os04c10_write_reg(dev, 0x37c2, 0x04);
    ret |= os04c10_write_reg(dev, 0x37cd, 0x19);
    ret |= os04c10_write_reg(dev, 0x37e0, 0x08);
    ret |= os04c10_write_reg(dev, 0x37e6, 0x04);
    ret |= os04c10_write_reg(dev, 0x37e5, 0x02);
    ret |= os04c10_write_reg(dev, 0x37e1, 0x0c);
    ret |= os04c10_write_reg(dev, 0x3737, 0x04);
    ret |= os04c10_write_reg(dev, 0x37d8, 0x02);
    ret |= os04c10_write_reg(dev, 0x37e2, 0x10);
    ret |= os04c10_write_reg(dev, 0x3739, 0x10);
    ret |= os04c10_write_reg(dev, 0x3662, 0x10);
    ret |= os04c10_write_reg(dev, 0x37e4, 0x20);
    ret |= os04c10_write_reg(dev, 0x37e3, 0x08);
    ret |= os04c10_write_reg(dev, 0x37d9, 0x08);
    ret |= os04c10_write_reg(dev, 0x4040, 0x00);
    ret |= os04c10_write_reg(dev, 0x4041, 0x07);
    ret |= os04c10_write_reg(dev, 0x4008, 0x02);
    ret |= os04c10_write_reg(dev, 0x4009, 0x0d);
    ret |= os04c10_write_reg(dev, 0x3800, 0x00);
    ret |= os04c10_write_reg(dev, 0x3801, 0x00);
    ret |= os04c10_write_reg(dev, 0x3802, 0x00);
    ret |= os04c10_write_reg(dev, 0x3803, 0x00);
    ret |= os04c10_write_reg(dev, 0x3804, 0x0a);
    ret |= os04c10_write_reg(dev, 0x3805, 0x8f);
    ret |= os04c10_write_reg(dev, 0x3806, 0x05);
    ret |= os04c10_write_reg(dev, 0x3807, 0xff);
    ret |= os04c10_write_reg(dev, 0x3808, 0x0a);
    ret |= os04c10_write_reg(dev, 0x3809, 0x80);
    ret |= os04c10_write_reg(dev, 0x380a, 0x05);
    ret |= os04c10_write_reg(dev, 0x380b, 0xf0);
    ret |= os04c10_write_reg(dev, 0x380c, 0x04);
    ret |= os04c10_write_reg(dev, 0x380d, 0x4e);
    ret |= os04c10_write_reg(dev, 0x380e, 0x06);
    ret |= os04c10_write_reg(dev, 0x380f, 0x60);
    ret |= os04c10_write_reg(dev, 0x3811, 0x08);
    ret |= os04c10_write_reg(dev, 0x3813, 0x08);
    ret |= os04c10_write_reg(dev, 0x3814, 0x01);
    ret |= os04c10_write_reg(dev, 0x3815, 0x01);
    ret |= os04c10_write_reg(dev, 0x3816, 0x01);
    ret |= os04c10_write_reg(dev, 0x3817, 0x01);
    ret |= os04c10_write_reg(dev, 0x3820, 0x88);
    ret |= os04c10_write_reg(dev, 0x3821, 0x04);
    ret |= os04c10_write_reg(dev, 0x3880, 0x25);
    ret |= os04c10_write_reg(dev, 0x3882, 0x20);
    ret |= os04c10_write_reg(dev, 0x3c91, 0x0b);
    ret |= os04c10_write_reg(dev, 0x3c94, 0x45);
    ret |= os04c10_write_reg(dev, 0x4000, 0xf3);
    ret |= os04c10_write_reg(dev, 0x4001, 0x60);
    ret |= os04c10_write_reg(dev, 0x4003, 0x40);
    ret |= os04c10_write_reg(dev, 0x4300, 0xff);
    ret |= os04c10_write_reg(dev, 0x4302, 0x0f);
    ret |= os04c10_write_reg(dev, 0x4305, 0x83);
    ret |= os04c10_write_reg(dev, 0x4505, 0x84);
    ret |= os04c10_write_reg(dev, 0x4809, 0x0e);
    ret |= os04c10_write_reg(dev, 0x480a, 0x04);
    ret |= os04c10_write_reg(dev, 0x4837, 0x14);
    ret |= os04c10_write_reg(dev, 0x4c00, 0x08);
    ret |= os04c10_write_reg(dev, 0x4c01, 0x08);
    ret |= os04c10_write_reg(dev, 0x4c04, 0x00);
    ret |= os04c10_write_reg(dev, 0x4c05, 0x00);
    ret |= os04c10_write_reg(dev, 0x5000, 0xe9);
    ret |= os04c10_write_reg(dev, 0x3624, 0x00);
    ret |= os04c10_write_reg(dev, 0x3016, 0x32);
    ret |= os04c10_write_reg(dev, 0x0306, 0x00);
    ret |= os04c10_write_reg(dev, 0x4837, 0x0A);
    ret |= os04c10_write_reg(dev, 0x0305, 0x5D);
    ret |= os04c10_write_reg(dev, 0x0325, 0x7A);
    ret |= os04c10_write_reg(dev, 0x3106, 0x25);
    ret |= os04c10_write_reg(dev, 0x0100, 0x01);
    os04c10_delay_ms(1);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os04c10_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("========== OS04C10_24MInput_MIPI_2lane_10bit_wdr_2688x1520_30fps init success!=========\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_SUCCESS;
}
#endif

xmedia_void os04c10_master_init(xmedia_u32 dev)
{
    os04c10_write_reg(dev, 0x0100, 0x00);
    os04c10_write_reg(dev, 0x3002, 0x22); // [1] vsync_oen, [0]: fsin_oen
    os04c10_write_reg(dev, 0x3690, 0x00); // [4]: 1'b0, 1st set vsync pulse
    os04c10_write_reg(dev, 0x383e, 0x00); // vscyn_rise_rcnt_pt[23:16]
    os04c10_write_reg(dev, 0x3818, 0x00); // Slave vsync pulse position cs [15:8]
    os04c10_write_reg(dev, 0x3819, 0x00); // Slave vsync pulse position cs [7:0],max is HTS/4
    os04c10_write_reg(dev, 0x381a, 0x00); // vscyn_rise_rcnt_pt[15:8]
    os04c10_write_reg(dev, 0x381b, 0x18); // vscyn_rise_rcnt_pt[7:0], max: VTS-12 for AHBIN 720p, (VTS -12)*2 for other formats
    os04c10_write_reg(dev, 0x3832, 0xf8); // default, 8'h08, [7:4] vsync pulse width
    os04c10_write_reg(dev, 0x368a, 0x04); // GPIO enable
    os04c10_write_reg(dev, 0x0100, 0x01);

    SENSOR_PRINT("dev[%d]- OS04C10 master setting init success.\n", dev);

    return;
}

xmedia_void os04c10_slave_init(xmedia_u32 dev)
{
    os04c10_write_reg(dev, 0x0100, 0x00);
    os04c10_write_reg(dev, 0x3002, 0x22); // [1] vsync_oen, [0]: fsin_oen
    os04c10_write_reg(dev, 0x3663, 0x22); // [2] fsin pad disable
    os04c10_write_reg(dev, 0x3823, 0x00); // [3:0]r_ext_vsync_div, SYNC every frame
    os04c10_write_reg(dev, 0x3822, 0x44); // [6]: ext_vs_adj_vts mode enable [5]: fix_cnt_en
    os04c10_write_reg(dev, 0x3832, 0xf8); // [7:4] vsync pulse width
    os04c10_write_reg(dev, 0x368a, 0x04); // GPIO enable
    os04c10_write_reg(dev, 0x3829, 0x03); // [5:4]: vts_adj_threshold = 0, other bits follow base setting
    os04c10_write_reg(dev, 0x3844, 0x06); // threshold_vts_sub[7:0]
    os04c10_write_reg(dev, 0x3843, 0x00); // man_vts_adj_val[23:16]
    os04c10_write_reg(dev, 0x382a, 0x00); // man_vts_adj_val[15:8]
    os04c10_write_reg(dev, 0x382b, 0x0c); // man_vts_adj_val[7:0]
    os04c10_write_reg(dev, 0x0100, 0x01);

    SENSOR_PRINT("dev[%d]- OS04C10 slave setting init success.\n", dev);

    return;
}

xmedia_s32 os04c10_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch (image_mode) {
        case OS04C10_4L_4M_12BIT_LINEAR_MODE:
            ret = os04c10_4lane_linear_2688x1520_init(dev);
            break;

        case OS04C10_2L_4M_10BIT_LINEAR_MODE:
            ret = os04c10_2lane_linear_2688x1520_init(dev);
            break;

        case OS04C10_2L_4M_10BIT_WDR_MODE:
            ret = os04c10_2lane_wdr_2688x1520_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (work_mode == XMEDIA_SENSOR_WORK_MODE_MASTER) {
        os04c10_master_init(dev);
    } else if (work_mode == XMEDIA_SENSOR_WORK_MODE_SLAVE) {
        os04c10_slave_init(dev);
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os04c10_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_os04c10_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_os04c10_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_os04c10_i2c_fd[dev]);
        g_os04c10_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_os04c10_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os04c10_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_os04c10_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_os04c10_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_os04c10_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = os04c10_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os04c10_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_os04c10_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_os04c10_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os04c10_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[OS04C10_DATA_BYTE];

    if (g_os04c10_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_os04c10_i2c_fd[dev], g_os04c10_i2c_addr[dev], addr, OS04C10_ADDR_BYTE, buf, OS04C10_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //OS04C10_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os04c10_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_os04c10_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_os04c10_i2c_fd[dev], g_os04c10_i2c_addr[dev], buf, OS04C10_ADDR_BYTE + OS04C10_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 os04c10_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if (mirror_en && flip_en) {
        ret |= os04c10_write_reg(dev, OS04C10_REG_ADDR_MIRROR_FLIP, 0xB8);
        ret |= os04c10_write_reg(dev, OS04C10_REG_ADDR_ANALOG_FLIP, 0x04);
    } else if (mirror_en && (!flip_en)) {
        ret |= os04c10_write_reg(dev, OS04C10_REG_ADDR_MIRROR_FLIP, 0x88);
        ret |= os04c10_write_reg(dev, OS04C10_REG_ADDR_ANALOG_FLIP, 0x24);
    } else if ((!mirror_en) && flip_en) {
        ret |= os04c10_write_reg(dev, OS04C10_REG_ADDR_MIRROR_FLIP, 0xB0);
        ret |= os04c10_write_reg(dev, OS04C10_REG_ADDR_ANALOG_FLIP, 0x04);
    } else if ((!mirror_en) && (!flip_en)) {
        ret |= os04c10_write_reg(dev, OS04C10_REG_ADDR_MIRROR_FLIP, 0x80);
        ret |= os04c10_write_reg(dev, OS04C10_REG_ADDR_ANALOG_FLIP, 0x24);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os04c10_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}
