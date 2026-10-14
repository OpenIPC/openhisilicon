#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "os08b10.h"
#include "os08b10_ctrl.h"

static xmedia_s32 g_os08b10_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                    { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
static xmedia_s32 g_os08b10_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                    { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

#define OS08B10_REG_ADDR_MIRROR_FLIP 0x3820

static void os08b10_delay_ms(int ms)
{
    usleep(ms * 1000);
    return;
}

/*
 * 函数功能: os08b10 MIPI 4lane 2160p线性模式初始化序列 - 3840x2160x30fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 os08b10_4lane_linear_3840x2160_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08b10_write_reg(dev, 0x0103, 0x01);
    ret |= os08b10_write_reg(dev, 0x0109, 0x01);
    ret |= os08b10_write_reg(dev, 0x0104, 0x03);
    ret |= os08b10_write_reg(dev, 0x0102, 0x00);
    ret |= os08b10_write_reg(dev, 0x0303, 0x00);
    ret |= os08b10_write_reg(dev, 0x0305, 0x3b);
    ret |= os08b10_write_reg(dev, 0x0306, 0x03);
    ret |= os08b10_write_reg(dev, 0x0307, 0x00);
    ret |= os08b10_write_reg(dev, 0x0308, 0x04);
    ret |= os08b10_write_reg(dev, 0x0309, 0x02);
    ret |= os08b10_write_reg(dev, 0x030a, 0x01);
    ret |= os08b10_write_reg(dev, 0x030c, 0x00);
    ret |= os08b10_write_reg(dev, 0x0317, 0x4a);
    ret |= os08b10_write_reg(dev, 0x0322, 0x01);
    ret |= os08b10_write_reg(dev, 0x0323, 0x05);
    ret |= os08b10_write_reg(dev, 0x0324, 0x01);
    ret |= os08b10_write_reg(dev, 0x0325, 0x80);
    ret |= os08b10_write_reg(dev, 0x0327, 0x02);
    ret |= os08b10_write_reg(dev, 0x0328, 0x05);
    ret |= os08b10_write_reg(dev, 0x0329, 0x01);
    ret |= os08b10_write_reg(dev, 0x032a, 0x02);
    ret |= os08b10_write_reg(dev, 0x032c, 0x01);
    ret |= os08b10_write_reg(dev, 0x032d, 0x00);
    ret |= os08b10_write_reg(dev, 0x032e, 0x03);
    ret |= os08b10_write_reg(dev, 0x032f, 0x01);
    ret |= os08b10_write_reg(dev, 0x3002, 0x00);
    ret |= os08b10_write_reg(dev, 0x300f, 0x11);
    ret |= os08b10_write_reg(dev, 0x3012, 0x41);
    ret |= os08b10_write_reg(dev, 0x301e, 0xb8);
    ret |= os08b10_write_reg(dev, 0x3026, 0x00);
    ret |= os08b10_write_reg(dev, 0x3027, 0x00);
    ret |= os08b10_write_reg(dev, 0x302c, 0x1a);
    ret |= os08b10_write_reg(dev, 0x3104, 0x00);
    ret |= os08b10_write_reg(dev, 0x3106, 0x10);
    ret |= os08b10_write_reg(dev, 0x3400, 0x08);
    ret |= os08b10_write_reg(dev, 0x3408, 0x05);
    ret |= os08b10_write_reg(dev, 0x3409, 0x22);
    ret |= os08b10_write_reg(dev, 0x340a, 0x02);
    ret |= os08b10_write_reg(dev, 0x340c, 0x0b);
    ret |= os08b10_write_reg(dev, 0x340d, 0x90);
    ret |= os08b10_write_reg(dev, 0x3420, 0x00);
    ret |= os08b10_write_reg(dev, 0x3421, 0x00);
    ret |= os08b10_write_reg(dev, 0x3422, 0x00);
    ret |= os08b10_write_reg(dev, 0x3423, 0x00);
    ret |= os08b10_write_reg(dev, 0x3424, 0x00);
    ret |= os08b10_write_reg(dev, 0x3425, 0x00);
    ret |= os08b10_write_reg(dev, 0x3426, 0x00);
    ret |= os08b10_write_reg(dev, 0x3427, 0x00);
    ret |= os08b10_write_reg(dev, 0x3428, 0x00);
    ret |= os08b10_write_reg(dev, 0x3429, 0x00);
    ret |= os08b10_write_reg(dev, 0x342a, 0x00);
    ret |= os08b10_write_reg(dev, 0x342b, 0x00);
    ret |= os08b10_write_reg(dev, 0x342c, 0x00);
    ret |= os08b10_write_reg(dev, 0x342d, 0x00);
    ret |= os08b10_write_reg(dev, 0x342e, 0x00);
    ret |= os08b10_write_reg(dev, 0x342f, 0x00);
    ret |= os08b10_write_reg(dev, 0x3501, 0x01);
    ret |= os08b10_write_reg(dev, 0x3502, 0x40);
    ret |= os08b10_write_reg(dev, 0x3504, 0x08);
    ret |= os08b10_write_reg(dev, 0x3541, 0x00);
    ret |= os08b10_write_reg(dev, 0x3542, 0x40);
    ret |= os08b10_write_reg(dev, 0x3581, 0x00);
    ret |= os08b10_write_reg(dev, 0x3582, 0x40);
    ret |= os08b10_write_reg(dev, 0x3603, 0x30);
    ret |= os08b10_write_reg(dev, 0x3612, 0x96);
    ret |= os08b10_write_reg(dev, 0x3613, 0xe0);
    ret |= os08b10_write_reg(dev, 0x3623, 0x00);
    ret |= os08b10_write_reg(dev, 0x3624, 0xff);
    ret |= os08b10_write_reg(dev, 0x3625, 0xff);
    ret |= os08b10_write_reg(dev, 0x362a, 0x0e);
    ret |= os08b10_write_reg(dev, 0x362b, 0x0e);
    ret |= os08b10_write_reg(dev, 0x362c, 0x0e);
    ret |= os08b10_write_reg(dev, 0x362d, 0x0e);
    ret |= os08b10_write_reg(dev, 0x362e, 0x17);
    ret |= os08b10_write_reg(dev, 0x362f, 0x2d);
    ret |= os08b10_write_reg(dev, 0x3630, 0x67);
    ret |= os08b10_write_reg(dev, 0x3631, 0x7f);
    ret |= os08b10_write_reg(dev, 0x3638, 0x00);
    ret |= os08b10_write_reg(dev, 0x3639, 0xff);
    ret |= os08b10_write_reg(dev, 0x363a, 0xff);
    ret |= os08b10_write_reg(dev, 0x3643, 0x0a);
    ret |= os08b10_write_reg(dev, 0x3644, 0x00);
    ret |= os08b10_write_reg(dev, 0x3645, 0x0a);
    ret |= os08b10_write_reg(dev, 0x3646, 0x0a);
    ret |= os08b10_write_reg(dev, 0x3647, 0x06);
    ret |= os08b10_write_reg(dev, 0x3648, 0x00);
    ret |= os08b10_write_reg(dev, 0x3649, 0x0a);
    ret |= os08b10_write_reg(dev, 0x364a, 0x0d);
    ret |= os08b10_write_reg(dev, 0x364b, 0x02);
    ret |= os08b10_write_reg(dev, 0x364c, 0x0e);
    ret |= os08b10_write_reg(dev, 0x364d, 0x0e);
    ret |= os08b10_write_reg(dev, 0x364e, 0x0e);
    ret |= os08b10_write_reg(dev, 0x364f, 0x0e);
    ret |= os08b10_write_reg(dev, 0x3650, 0xf7);
    ret |= os08b10_write_reg(dev, 0x3651, 0x77);
    ret |= os08b10_write_reg(dev, 0x365a, 0xbb);
    ret |= os08b10_write_reg(dev, 0x365b, 0x9d);
    ret |= os08b10_write_reg(dev, 0x365c, 0x8e);
    ret |= os08b10_write_reg(dev, 0x365d, 0x86);
    ret |= os08b10_write_reg(dev, 0x3661, 0x07);
    ret |= os08b10_write_reg(dev, 0x3662, 0x00);
    ret |= os08b10_write_reg(dev, 0x3667, 0xd4);
    ret |= os08b10_write_reg(dev, 0x366f, 0x00);
    ret |= os08b10_write_reg(dev, 0x3671, 0x88);
    ret |= os08b10_write_reg(dev, 0x3673, 0x2a);
    ret |= os08b10_write_reg(dev, 0x3678, 0x00);
    ret |= os08b10_write_reg(dev, 0x3679, 0x00);
    ret |= os08b10_write_reg(dev, 0x3682, 0xf1);
    ret |= os08b10_write_reg(dev, 0x3687, 0x02);
    ret |= os08b10_write_reg(dev, 0x3688, 0x81);
    ret |= os08b10_write_reg(dev, 0x3689, 0x02);
    ret |= os08b10_write_reg(dev, 0x3692, 0x0f);
    ret |= os08b10_write_reg(dev, 0x3701, 0x39);
    ret |= os08b10_write_reg(dev, 0x3703, 0x32);
    ret |= os08b10_write_reg(dev, 0x3705, 0x00);
    ret |= os08b10_write_reg(dev, 0x3706, 0x74);
    ret |= os08b10_write_reg(dev, 0x3708, 0x35);
    ret |= os08b10_write_reg(dev, 0x370a, 0x01);
    ret |= os08b10_write_reg(dev, 0x370b, 0x4b);
    ret |= os08b10_write_reg(dev, 0x3712, 0x13);
    ret |= os08b10_write_reg(dev, 0x3714, 0x02);
    ret |= os08b10_write_reg(dev, 0x3715, 0x00);
    ret |= os08b10_write_reg(dev, 0x3716, 0x00);
    ret |= os08b10_write_reg(dev, 0x3719, 0x23);
    ret |= os08b10_write_reg(dev, 0x371c, 0x00);
    ret |= os08b10_write_reg(dev, 0x371d, 0x08);
    ret |= os08b10_write_reg(dev, 0x373b, 0x50);
    ret |= os08b10_write_reg(dev, 0x3755, 0x01);
    ret |= os08b10_write_reg(dev, 0x3756, 0xea);
    ret |= os08b10_write_reg(dev, 0x3757, 0xea);
    ret |= os08b10_write_reg(dev, 0x376b, 0x02);
    ret |= os08b10_write_reg(dev, 0x376c, 0x02);
    ret |= os08b10_write_reg(dev, 0x376d, 0x04);
    ret |= os08b10_write_reg(dev, 0x376e, 0x04);
    ret |= os08b10_write_reg(dev, 0x376f, 0x22);
    ret |= os08b10_write_reg(dev, 0x377a, 0x02);
    ret |= os08b10_write_reg(dev, 0x377b, 0x01);
    ret |= os08b10_write_reg(dev, 0x3785, 0x08);
    ret |= os08b10_write_reg(dev, 0x3790, 0x10);
    ret |= os08b10_write_reg(dev, 0x3792, 0x00);
    ret |= os08b10_write_reg(dev, 0x3796, 0x00);
    ret |= os08b10_write_reg(dev, 0x3797, 0x00);
    ret |= os08b10_write_reg(dev, 0x3798, 0x00);
    ret |= os08b10_write_reg(dev, 0x3799, 0x00);
    ret |= os08b10_write_reg(dev, 0x37bb, 0x88);
    ret |= os08b10_write_reg(dev, 0x37be, 0x01);
    ret |= os08b10_write_reg(dev, 0x37bf, 0x01);
    ret |= os08b10_write_reg(dev, 0x37c0, 0x00);
    ret |= os08b10_write_reg(dev, 0x37c7, 0x51);
    ret |= os08b10_write_reg(dev, 0x37c8, 0x22);
    ret |= os08b10_write_reg(dev, 0x37c9, 0x00);
    ret |= os08b10_write_reg(dev, 0x37ca, 0xe4);
    ret |= os08b10_write_reg(dev, 0x37cc, 0x0e);
    ret |= os08b10_write_reg(dev, 0x37cf, 0x02);
    ret |= os08b10_write_reg(dev, 0x37d0, 0x00);
    ret |= os08b10_write_reg(dev, 0x37d1, 0x74);
    ret |= os08b10_write_reg(dev, 0x37d2, 0x01);
    ret |= os08b10_write_reg(dev, 0x37d3, 0x4b);
    ret |= os08b10_write_reg(dev, 0x37d4, 0x00);
    ret |= os08b10_write_reg(dev, 0x37d5, 0x74);
    ret |= os08b10_write_reg(dev, 0x37d6, 0x01);
    ret |= os08b10_write_reg(dev, 0x37d7, 0x4b);
    ret |= os08b10_write_reg(dev, 0x3800, 0x00);
    ret |= os08b10_write_reg(dev, 0x3801, 0x00);
    ret |= os08b10_write_reg(dev, 0x3802, 0x00);
    ret |= os08b10_write_reg(dev, 0x3803, 0x00);
    ret |= os08b10_write_reg(dev, 0x3804, 0x0f);
    ret |= os08b10_write_reg(dev, 0x3805, 0x1f);
    ret |= os08b10_write_reg(dev, 0x3806, 0x08);
    ret |= os08b10_write_reg(dev, 0x3807, 0x7f);
    ret |= os08b10_write_reg(dev, 0x3808, 0x0f);
    ret |= os08b10_write_reg(dev, 0x3809, 0x00);
    ret |= os08b10_write_reg(dev, 0x380a, 0x08);
    ret |= os08b10_write_reg(dev, 0x380b, 0x70);
    ret |= os08b10_write_reg(dev, 0x380c, 0x04);
    ret |= os08b10_write_reg(dev, 0x380d, 0x38);
    ret |= os08b10_write_reg(dev, 0x380e, 0x05);
    ret |= os08b10_write_reg(dev, 0x380f, 0xc8);
    ret |= os08b10_write_reg(dev, 0x3811, 0x0f);
    ret |= os08b10_write_reg(dev, 0x3813, 0x05); /* vertical offset 0x8 */
    ret |= os08b10_write_reg(dev, 0x3814, 0x01);
    ret |= os08b10_write_reg(dev, 0x3815, 0x01);
    ret |= os08b10_write_reg(dev, 0x3816, 0x01);
    ret |= os08b10_write_reg(dev, 0x3817, 0x01);
    ret |= os08b10_write_reg(dev, 0x381c, 0x00);
    ret |= os08b10_write_reg(dev, 0x3820, 0x02);
    ret |= os08b10_write_reg(dev, 0x3821, 0x00);
    ret |= os08b10_write_reg(dev, 0x3822, 0x14); /* vts add disable[4]: 1 dis; 0 enable */
    ret |= os08b10_write_reg(dev, 0x3823, 0x08);
    ret |= os08b10_write_reg(dev, 0x382b, 0x00);
    ret |= os08b10_write_reg(dev, 0x382c, 0x00);
    ret |= os08b10_write_reg(dev, 0x383e, 0x00);
    ret |= os08b10_write_reg(dev, 0x384c, 0x04);
    ret |= os08b10_write_reg(dev, 0x384d, 0x38);
    ret |= os08b10_write_reg(dev, 0x3858, 0x3c);
    ret |= os08b10_write_reg(dev, 0x3850, 0x00);
    ret |= os08b10_write_reg(dev, 0x3851, 0x00);
    ret |= os08b10_write_reg(dev, 0x3852, 0x00);
    ret |= os08b10_write_reg(dev, 0x3853, 0x00);
    ret |= os08b10_write_reg(dev, 0x3865, 0x02);
    ret |= os08b10_write_reg(dev, 0x3866, 0x00);
    ret |= os08b10_write_reg(dev, 0x3867, 0x02);
    ret |= os08b10_write_reg(dev, 0x3868, 0x00);
    ret |= os08b10_write_reg(dev, 0x386a, 0x05);
    ret |= os08b10_write_reg(dev, 0x386b, 0x00);
    ret |= os08b10_write_reg(dev, 0x386c, 0x05);
    ret |= os08b10_write_reg(dev, 0x386d, 0x61);
    ret |= os08b10_write_reg(dev, 0x3881, 0x02);
    ret |= os08b10_write_reg(dev, 0x3882, 0x00);
    ret |= os08b10_write_reg(dev, 0x3883, 0x08);
    ret |= os08b10_write_reg(dev, 0x3884, 0x01);
    ret |= os08b10_write_reg(dev, 0x3885, 0x00);
    ret |= os08b10_write_reg(dev, 0x3886, 0x00);
    ret |= os08b10_write_reg(dev, 0x3887, 0x20);
    ret |= os08b10_write_reg(dev, 0x3888, 0x40);
    ret |= os08b10_write_reg(dev, 0x3900, 0x00);
    ret |= os08b10_write_reg(dev, 0x3940, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c37, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c49, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c4a, 0x9a);
    ret |= os08b10_write_reg(dev, 0x3c4c, 0x01);
    ret |= os08b10_write_reg(dev, 0x3c4d, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c4e, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c50, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c51, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c52, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c67, 0x10);
    ret |= os08b10_write_reg(dev, 0x3c69, 0x10);
    ret |= os08b10_write_reg(dev, 0x3c6a, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c6b, 0x62);
    ret |= os08b10_write_reg(dev, 0x3c6c, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c6d, 0x62);
    ret |= os08b10_write_reg(dev, 0x3c70, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c71, 0x04);
    ret |= os08b10_write_reg(dev, 0x3c72, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c73, 0x62);
    ret |= os08b10_write_reg(dev, 0x3c74, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c75, 0x62);
    ret |= os08b10_write_reg(dev, 0x3c76, 0x12);
    ret |= os08b10_write_reg(dev, 0x3c77, 0x12);
    ret |= os08b10_write_reg(dev, 0x3c79, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c7a, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c7b, 0x00);
    ret |= os08b10_write_reg(dev, 0x3cb6, 0x41);
    ret |= os08b10_write_reg(dev, 0x3cb9, 0x00);
    ret |= os08b10_write_reg(dev, 0x3cc0, 0x90);
    ret |= os08b10_write_reg(dev, 0x3cc2, 0x90);
    ret |= os08b10_write_reg(dev, 0x3cc5, 0x00);
    ret |= os08b10_write_reg(dev, 0x3cc6, 0x98);
    ret |= os08b10_write_reg(dev, 0x3cc7, 0x00);
    ret |= os08b10_write_reg(dev, 0x3cc8, 0x98);
    ret |= os08b10_write_reg(dev, 0x3cc9, 0x00);
    ret |= os08b10_write_reg(dev, 0x3cca, 0x98);
    ret |= os08b10_write_reg(dev, 0x3ccc, 0x01);
    ret |= os08b10_write_reg(dev, 0x3cd9, 0x7c);
    ret |= os08b10_write_reg(dev, 0x3cda, 0x7c);
    ret |= os08b10_write_reg(dev, 0x3cdb, 0x7c);
    ret |= os08b10_write_reg(dev, 0x3cdc, 0x7c);
    ret |= os08b10_write_reg(dev, 0x3d8c, 0x70);
    ret |= os08b10_write_reg(dev, 0x3d8d, 0x10);
    ret |= os08b10_write_reg(dev, 0x4001, 0x2b); /* OB ouput 0x6b */
    ret |= os08b10_write_reg(dev, 0x4004, 0x01); /* blc target 0x40 */
    ret |= os08b10_write_reg(dev, 0x4005, 0x00);
    ret |= os08b10_write_reg(dev, 0x4008, 0x02);
    ret |= os08b10_write_reg(dev, 0x4009, 0x0d);
    ret |= os08b10_write_reg(dev, 0x400a, 0x04);
    ret |= os08b10_write_reg(dev, 0x400b, 0xa8);
    ret |= os08b10_write_reg(dev, 0x400e, 0x40);
    ret |= os08b10_write_reg(dev, 0x401a, 0x50);
    ret |= os08b10_write_reg(dev, 0x402e, 0x01); /* blc target 0x40 */
    ret |= os08b10_write_reg(dev, 0x402f, 0x00);
    ret |= os08b10_write_reg(dev, 0x4030, 0x00);
    ret |= os08b10_write_reg(dev, 0x4031, 0x40);
    ret |= os08b10_write_reg(dev, 0x4032, 0x9f);
    ret |= os08b10_write_reg(dev, 0x4050, 0x01);
    ret |= os08b10_write_reg(dev, 0x4051, 0x06);
    ret |= os08b10_write_reg(dev, 0x405d, 0x00);
    ret |= os08b10_write_reg(dev, 0x405e, 0x00);
    ret |= os08b10_write_reg(dev, 0x4288, 0xcf);
    ret |= os08b10_write_reg(dev, 0x4289, 0x00);
    ret |= os08b10_write_reg(dev, 0x428a, 0x40);
    ret |= os08b10_write_reg(dev, 0x430b, 0xff);
    ret |= os08b10_write_reg(dev, 0x430c, 0xff);
    ret |= os08b10_write_reg(dev, 0x430d, 0x00);
    ret |= os08b10_write_reg(dev, 0x430e, 0x00);
    ret |= os08b10_write_reg(dev, 0x4314, 0x04);
    ret |= os08b10_write_reg(dev, 0x4500, 0x1a);
    ret |= os08b10_write_reg(dev, 0x4501, 0x18);
    ret |= os08b10_write_reg(dev, 0x4504, 0x00);
    ret |= os08b10_write_reg(dev, 0x4505, 0x00);
    ret |= os08b10_write_reg(dev, 0x4506, 0x32);
    ret |= os08b10_write_reg(dev, 0x4507, 0x02);
    ret |= os08b10_write_reg(dev, 0x4508, 0x1a);
    ret |= os08b10_write_reg(dev, 0x450a, 0x12);
    ret |= os08b10_write_reg(dev, 0x450b, 0x28);
    ret |= os08b10_write_reg(dev, 0x450c, 0x00);
    ret |= os08b10_write_reg(dev, 0x450f, 0x80);
    ret |= os08b10_write_reg(dev, 0x4513, 0x00);
    ret |= os08b10_write_reg(dev, 0x4516, 0x28);
    ret |= os08b10_write_reg(dev, 0x4603, 0x00);
    ret |= os08b10_write_reg(dev, 0x460a, 0x50);
    ret |= os08b10_write_reg(dev, 0x4640, 0x62);
    ret |= os08b10_write_reg(dev, 0x464d, 0x00);
    ret |= os08b10_write_reg(dev, 0x4800, 0x04);
    ret |= os08b10_write_reg(dev, 0x480e, 0x00);
    ret |= os08b10_write_reg(dev, 0x4813, 0x00);
    ret |= os08b10_write_reg(dev, 0x4837, 0x0b);
    ret |= os08b10_write_reg(dev, 0x484b, 0x27);
    ret |= os08b10_write_reg(dev, 0x4850, 0x47);
    ret |= os08b10_write_reg(dev, 0x4851, 0xaa);
    ret |= os08b10_write_reg(dev, 0x4852, 0xff);
    ret |= os08b10_write_reg(dev, 0x4853, 0x20);
    ret |= os08b10_write_reg(dev, 0x4854, 0x08);
    ret |= os08b10_write_reg(dev, 0x4855, 0x30);
    ret |= os08b10_write_reg(dev, 0x4860, 0x00);
    ret |= os08b10_write_reg(dev, 0x4d00, 0x4d);
    ret |= os08b10_write_reg(dev, 0x4d01, 0x42);
    ret |= os08b10_write_reg(dev, 0x4d02, 0xb9);
    ret |= os08b10_write_reg(dev, 0x4d03, 0x94);
    ret |= os08b10_write_reg(dev, 0x4d04, 0x95);
    ret |= os08b10_write_reg(dev, 0x4d05, 0xce);
    ret |= os08b10_write_reg(dev, 0x4d09, 0x6f);
    ret |= os08b10_write_reg(dev, 0x5000, 0x03); /* sensor dpc 0xf */
    ret |= os08b10_write_reg(dev, 0x5001, 0x0d);
    ret |= os08b10_write_reg(dev, 0x5080, 0x00);
    ret |= os08b10_write_reg(dev, 0x50c0, 0x00);
    ret |= os08b10_write_reg(dev, 0x5100, 0x00);
    ret |= os08b10_write_reg(dev, 0x5200, 0x70);
    ret |= os08b10_write_reg(dev, 0x5201, 0x70);
    ret |= os08b10_write_reg(dev, 0x5681, 0x00);
    ret |= os08b10_write_reg(dev, 0x5780, 0x53);
    ret |= os08b10_write_reg(dev, 0x5781, 0x03);
    ret |= os08b10_write_reg(dev, 0x5782, 0x60);
    ret |= os08b10_write_reg(dev, 0x5783, 0xf0);
    ret |= os08b10_write_reg(dev, 0x5784, 0x00);
    ret |= os08b10_write_reg(dev, 0x5785, 0x40);
    ret |= os08b10_write_reg(dev, 0x5786, 0x01);
    ret |= os08b10_write_reg(dev, 0x5788, 0x60);
    ret |= os08b10_write_reg(dev, 0x5789, 0xf0);
    ret |= os08b10_write_reg(dev, 0x5791, 0x08);
    ret |= os08b10_write_reg(dev, 0x5792, 0x11);
    ret |= os08b10_write_reg(dev, 0x5793, 0x33);
    os08b10_delay_ms(1);   /* delay 1 ms */
    ret |= os08b10_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08b10_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("========================================================================\n");
        SENSOR_PRINT("=== OS08B10_24MInput_MIPI_4lane_12bit_3840x2160_30fps init success!=====\n");
        SENSOR_PRINT("========================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

static xmedia_s32 os08b10_4lane_wdr_3840x2160_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os08b10_write_reg(dev, 0x0103, 0x01);
    ret |= os08b10_write_reg(dev, 0x0109, 0x01);
    ret |= os08b10_write_reg(dev, 0x0104, 0x03);
    ret |= os08b10_write_reg(dev, 0x0102, 0x00);
    ret |= os08b10_write_reg(dev, 0x0303, 0x00);
    ret |= os08b10_write_reg(dev, 0x0305, 0x3e);
    ret |= os08b10_write_reg(dev, 0x0306, 0x03);
    ret |= os08b10_write_reg(dev, 0x0307, 0x00);
    ret |= os08b10_write_reg(dev, 0x0308, 0x04);
    ret |= os08b10_write_reg(dev, 0x0309, 0x02);
    ret |= os08b10_write_reg(dev, 0x030a, 0x01);
    ret |= os08b10_write_reg(dev, 0x030c, 0x00);
    ret |= os08b10_write_reg(dev, 0x0317, 0x49);
    ret |= os08b10_write_reg(dev, 0x0322, 0x01);
    ret |= os08b10_write_reg(dev, 0x0323, 0x05);
    ret |= os08b10_write_reg(dev, 0x0324, 0x01);
    ret |= os08b10_write_reg(dev, 0x0325, 0x90);
    ret |= os08b10_write_reg(dev, 0x0327, 0x03);
    ret |= os08b10_write_reg(dev, 0x0328, 0x05);
    ret |= os08b10_write_reg(dev, 0x0329, 0x02);
    ret |= os08b10_write_reg(dev, 0x032a, 0x02);
    ret |= os08b10_write_reg(dev, 0x032c, 0x01);
    ret |= os08b10_write_reg(dev, 0x032d, 0x00);
    ret |= os08b10_write_reg(dev, 0x032e, 0x03);
    ret |= os08b10_write_reg(dev, 0x032f, 0x01);
    ret |= os08b10_write_reg(dev, 0x3002, 0x00);
    ret |= os08b10_write_reg(dev, 0x300f, 0x11);
    ret |= os08b10_write_reg(dev, 0x3012, 0x41);
    ret |= os08b10_write_reg(dev, 0x301e, 0xb8);
    ret |= os08b10_write_reg(dev, 0x3026, 0x12);
    ret |= os08b10_write_reg(dev, 0x3027, 0x08);
    ret |= os08b10_write_reg(dev, 0x302c, 0x1a);
    ret |= os08b10_write_reg(dev, 0x3104, 0x00);
    ret |= os08b10_write_reg(dev, 0x3106, 0x10);
    ret |= os08b10_write_reg(dev, 0x3400, 0x00);
    ret |= os08b10_write_reg(dev, 0x3408, 0x05);
    ret |= os08b10_write_reg(dev, 0x3409, 0x22);
    ret |= os08b10_write_reg(dev, 0x340a, 0x02);
    ret |= os08b10_write_reg(dev, 0x340c, 0x08);
    ret |= os08b10_write_reg(dev, 0x340d, 0xa4);
    ret |= os08b10_write_reg(dev, 0x3420, 0x00);
    ret |= os08b10_write_reg(dev, 0x3421, 0x00);
    ret |= os08b10_write_reg(dev, 0x3422, 0x00);
    ret |= os08b10_write_reg(dev, 0x3423, 0x00);
    ret |= os08b10_write_reg(dev, 0x3424, 0x00);
    ret |= os08b10_write_reg(dev, 0x3425, 0x00);
    ret |= os08b10_write_reg(dev, 0x3426, 0x55);
    ret |= os08b10_write_reg(dev, 0x3427, 0x15);
    ret |= os08b10_write_reg(dev, 0x3428, 0x55);
    ret |= os08b10_write_reg(dev, 0x3429, 0x15);
    ret |= os08b10_write_reg(dev, 0x342a, 0x50);
    ret |= os08b10_write_reg(dev, 0x342b, 0x00);
    ret |= os08b10_write_reg(dev, 0x342c, 0x01);
    ret |= os08b10_write_reg(dev, 0x342d, 0x54);
    ret |= os08b10_write_reg(dev, 0x342e, 0x01);
    ret |= os08b10_write_reg(dev, 0x342f, 0x04);
    ret |= os08b10_write_reg(dev, 0x3501, 0x04);
    ret |= os08b10_write_reg(dev, 0x3502, 0x10);
    ret |= os08b10_write_reg(dev, 0x3504, 0x08);
    ret |= os08b10_write_reg(dev, 0x3541, 0x00);
    ret |= os08b10_write_reg(dev, 0x3542, 0x40);
    ret |= os08b10_write_reg(dev, 0x3581, 0x00);
    ret |= os08b10_write_reg(dev, 0x3582, 0x40);
    ret |= os08b10_write_reg(dev, 0x3603, 0x30);
    ret |= os08b10_write_reg(dev, 0x3612, 0x96);
    ret |= os08b10_write_reg(dev, 0x3613, 0xe0);
    ret |= os08b10_write_reg(dev, 0x3623, 0x00);
    ret |= os08b10_write_reg(dev, 0x3624, 0xff);
    ret |= os08b10_write_reg(dev, 0x3625, 0xff);
    ret |= os08b10_write_reg(dev, 0x362a, 0x12);
    ret |= os08b10_write_reg(dev, 0x362b, 0x12);
    ret |= os08b10_write_reg(dev, 0x362c, 0x12);
    ret |= os08b10_write_reg(dev, 0x362d, 0x12);
    ret |= os08b10_write_reg(dev, 0x362e, 0x17);
    ret |= os08b10_write_reg(dev, 0x362f, 0x2d);
    ret |= os08b10_write_reg(dev, 0x3630, 0x67);
    ret |= os08b10_write_reg(dev, 0x3631, 0x7f);
    ret |= os08b10_write_reg(dev, 0x3638, 0x00);
    ret |= os08b10_write_reg(dev, 0x3639, 0xff);
    ret |= os08b10_write_reg(dev, 0x363a, 0xff);
    ret |= os08b10_write_reg(dev, 0x3643, 0x0a);
    ret |= os08b10_write_reg(dev, 0x3644, 0x00);
    ret |= os08b10_write_reg(dev, 0x3645, 0x0a);
    ret |= os08b10_write_reg(dev, 0x3646, 0x0a);
    ret |= os08b10_write_reg(dev, 0x3647, 0x06);
    ret |= os08b10_write_reg(dev, 0x3648, 0x00);
    ret |= os08b10_write_reg(dev, 0x3649, 0x0a);
    ret |= os08b10_write_reg(dev, 0x364a, 0x0d);
    ret |= os08b10_write_reg(dev, 0x364b, 0x02);
    ret |= os08b10_write_reg(dev, 0x364c, 0x12);
    ret |= os08b10_write_reg(dev, 0x364d, 0x12);
    ret |= os08b10_write_reg(dev, 0x364e, 0x12);
    ret |= os08b10_write_reg(dev, 0x364f, 0x12);
    ret |= os08b10_write_reg(dev, 0x3650, 0xf7);
    ret |= os08b10_write_reg(dev, 0x3651, 0x77);
    ret |= os08b10_write_reg(dev, 0x365a, 0xbb);
    ret |= os08b10_write_reg(dev, 0x365b, 0x9d);
    ret |= os08b10_write_reg(dev, 0x365c, 0x8e);
    ret |= os08b10_write_reg(dev, 0x365d, 0x86);
    ret |= os08b10_write_reg(dev, 0x3661, 0x07);
    ret |= os08b10_write_reg(dev, 0x3662, 0x0a);
    ret |= os08b10_write_reg(dev, 0x3667, 0x54);
    ret |= os08b10_write_reg(dev, 0x366f, 0x00);
    ret |= os08b10_write_reg(dev, 0x3671, 0x89);
    ret |= os08b10_write_reg(dev, 0x3673, 0x2a);
    ret |= os08b10_write_reg(dev, 0x3678, 0x22);
    ret |= os08b10_write_reg(dev, 0x3679, 0x00);
    ret |= os08b10_write_reg(dev, 0x3682, 0xf1);
    ret |= os08b10_write_reg(dev, 0x3687, 0x02);
    ret |= os08b10_write_reg(dev, 0x3688, 0x81);
    ret |= os08b10_write_reg(dev, 0x3689, 0x02);
    ret |= os08b10_write_reg(dev, 0x3692, 0x0f);
    ret |= os08b10_write_reg(dev, 0x3701, 0x39);
    ret |= os08b10_write_reg(dev, 0x3703, 0x32);
    ret |= os08b10_write_reg(dev, 0x3705, 0x00);
    ret |= os08b10_write_reg(dev, 0x3706, 0x3a);
    ret |= os08b10_write_reg(dev, 0x3708, 0x27);
    ret |= os08b10_write_reg(dev, 0x370a, 0x00);
    ret |= os08b10_write_reg(dev, 0x370b, 0x8a);
    ret |= os08b10_write_reg(dev, 0x3712, 0x13);
    ret |= os08b10_write_reg(dev, 0x3714, 0x02);
    ret |= os08b10_write_reg(dev, 0x3715, 0x00);
    ret |= os08b10_write_reg(dev, 0x3716, 0x00);
    ret |= os08b10_write_reg(dev, 0x3719, 0x23);
    ret |= os08b10_write_reg(dev, 0x371c, 0x00);
    ret |= os08b10_write_reg(dev, 0x371d, 0x08);
    ret |= os08b10_write_reg(dev, 0x373b, 0x50);
    ret |= os08b10_write_reg(dev, 0x3755, 0x01);
    ret |= os08b10_write_reg(dev, 0x3756, 0xa9);
    ret |= os08b10_write_reg(dev, 0x3757, 0xa9);
    ret |= os08b10_write_reg(dev, 0x376b, 0x02);
    ret |= os08b10_write_reg(dev, 0x376c, 0x32);
    ret |= os08b10_write_reg(dev, 0x376d, 0x04);
    ret |= os08b10_write_reg(dev, 0x376e, 0x04);
    ret |= os08b10_write_reg(dev, 0x376f, 0x22);
    ret |= os08b10_write_reg(dev, 0x377a, 0x02);
    ret |= os08b10_write_reg(dev, 0x377b, 0x01);
    ret |= os08b10_write_reg(dev, 0x3785, 0x08);
    ret |= os08b10_write_reg(dev, 0x3790, 0x10);
    ret |= os08b10_write_reg(dev, 0x3792, 0x00);
    ret |= os08b10_write_reg(dev, 0x3796, 0x00);
    ret |= os08b10_write_reg(dev, 0x3797, 0x00);
    ret |= os08b10_write_reg(dev, 0x3798, 0x00);
    ret |= os08b10_write_reg(dev, 0x3799, 0x00);
    ret |= os08b10_write_reg(dev, 0x37bb, 0x88);
    ret |= os08b10_write_reg(dev, 0x37be, 0x01);
    ret |= os08b10_write_reg(dev, 0x37bf, 0x01);
    ret |= os08b10_write_reg(dev, 0x37c0, 0x00);
    ret |= os08b10_write_reg(dev, 0x37c7, 0x51);
    ret |= os08b10_write_reg(dev, 0x37c8, 0x22);
    ret |= os08b10_write_reg(dev, 0x37c9, 0x00);
    ret |= os08b10_write_reg(dev, 0x37ca, 0xb6);
    ret |= os08b10_write_reg(dev, 0x37cc, 0x0e);
    ret |= os08b10_write_reg(dev, 0x37cf, 0x02);
    ret |= os08b10_write_reg(dev, 0x37d0, 0x00);
    ret |= os08b10_write_reg(dev, 0x37d1, 0x3a);
    ret |= os08b10_write_reg(dev, 0x37d2, 0x00);
    ret |= os08b10_write_reg(dev, 0x37d3, 0x8a);
    ret |= os08b10_write_reg(dev, 0x37d4, 0x00);
    ret |= os08b10_write_reg(dev, 0x37d5, 0x3a);
    ret |= os08b10_write_reg(dev, 0x37d6, 0x00);
    ret |= os08b10_write_reg(dev, 0x37d7, 0x8a);
    ret |= os08b10_write_reg(dev, 0x3800, 0x00);
    ret |= os08b10_write_reg(dev, 0x3801, 0x00);
    ret |= os08b10_write_reg(dev, 0x3802, 0x00);
    ret |= os08b10_write_reg(dev, 0x3803, 0x00);
    ret |= os08b10_write_reg(dev, 0x3804, 0x0f);
    ret |= os08b10_write_reg(dev, 0x3805, 0x1f);
    ret |= os08b10_write_reg(dev, 0x3806, 0x08);
    ret |= os08b10_write_reg(dev, 0x3807, 0x7f);
    ret |= os08b10_write_reg(dev, 0x3808, 0x0f);
    ret |= os08b10_write_reg(dev, 0x3809, 0x00);
    ret |= os08b10_write_reg(dev, 0x380a, 0x08);
    ret |= os08b10_write_reg(dev, 0x380b, 0x70); /* vertical 0x70 */
    ret |= os08b10_write_reg(dev, 0x380c, 0x02);
    ret |= os08b10_write_reg(dev, 0x380d, 0xd0);
    ret |= os08b10_write_reg(dev, 0x380e, 0x04);
    ret |= os08b10_write_reg(dev, 0x380f, 0x86);
    ret |= os08b10_write_reg(dev, 0x3811, 0x11); /* horizontal offset 0x11 */
    ret |= os08b10_write_reg(dev, 0x3813, 0x01); /* vertical offset 0x1 */
    ret |= os08b10_write_reg(dev, 0x3814, 0x01);
    ret |= os08b10_write_reg(dev, 0x3815, 0x01);
    ret |= os08b10_write_reg(dev, 0x3816, 0x01);
    ret |= os08b10_write_reg(dev, 0x3817, 0x01);
    ret |= os08b10_write_reg(dev, 0x381c, 0x08);
    ret |= os08b10_write_reg(dev, 0x3820, 0x03);
    ret |= os08b10_write_reg(dev, 0x3821, 0x00);
    ret |= os08b10_write_reg(dev, 0x3822, 0x04);
    ret |= os08b10_write_reg(dev, 0x3823, 0x08);
    ret |= os08b10_write_reg(dev, 0x382b, 0x00);
    ret |= os08b10_write_reg(dev, 0x382c, 0x00);
    ret |= os08b10_write_reg(dev, 0x383e, 0x00);
    ret |= os08b10_write_reg(dev, 0x384c, 0x02);
    ret |= os08b10_write_reg(dev, 0x384d, 0xd0);
    ret |= os08b10_write_reg(dev, 0x3858, 0x7c);
    ret |= os08b10_write_reg(dev, 0x3850, 0xff);
    ret |= os08b10_write_reg(dev, 0x3851, 0xff);
    ret |= os08b10_write_reg(dev, 0x3852, 0xff);
    ret |= os08b10_write_reg(dev, 0x3853, 0xff);
    ret |= os08b10_write_reg(dev, 0x3865, 0x02);
    ret |= os08b10_write_reg(dev, 0x3866, 0x00);
    ret |= os08b10_write_reg(dev, 0x3867, 0x02);
    ret |= os08b10_write_reg(dev, 0x3868, 0x00);
    ret |= os08b10_write_reg(dev, 0x386a, 0x05);
    ret |= os08b10_write_reg(dev, 0x386b, 0x00);
    ret |= os08b10_write_reg(dev, 0x386c, 0x05);
    ret |= os08b10_write_reg(dev, 0x386d, 0x61);
    ret |= os08b10_write_reg(dev, 0x3881, 0x02);
    ret |= os08b10_write_reg(dev, 0x3882, 0x00);
    ret |= os08b10_write_reg(dev, 0x3883, 0x08);
    ret |= os08b10_write_reg(dev, 0x3884, 0x01);
    ret |= os08b10_write_reg(dev, 0x3885, 0x00);
    ret |= os08b10_write_reg(dev, 0x3886, 0x00);
    ret |= os08b10_write_reg(dev, 0x3887, 0x20);
    ret |= os08b10_write_reg(dev, 0x3888, 0x40);
    ret |= os08b10_write_reg(dev, 0x3c37, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c49, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c4a, 0x9a);
    ret |= os08b10_write_reg(dev, 0x3c4c, 0x01);
    ret |= os08b10_write_reg(dev, 0x3c4d, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c4e, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c50, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c51, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c52, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c67, 0x10);
    ret |= os08b10_write_reg(dev, 0x3c69, 0x10);
    ret |= os08b10_write_reg(dev, 0x3c6a, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c6b, 0x52);
    ret |= os08b10_write_reg(dev, 0x3c6c, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c6d, 0x52);
    ret |= os08b10_write_reg(dev, 0x3c70, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c71, 0x04);
    ret |= os08b10_write_reg(dev, 0x3c72, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c73, 0x52);
    ret |= os08b10_write_reg(dev, 0x3c74, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c75, 0x52);
    ret |= os08b10_write_reg(dev, 0x3c76, 0x12);
    ret |= os08b10_write_reg(dev, 0x3c77, 0x12);
    ret |= os08b10_write_reg(dev, 0x3c79, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c7a, 0x00);
    ret |= os08b10_write_reg(dev, 0x3c7b, 0x00);
    ret |= os08b10_write_reg(dev, 0x3cb6, 0x41);
    ret |= os08b10_write_reg(dev, 0x3cb9, 0x00);
    ret |= os08b10_write_reg(dev, 0x3cc0, 0x90);
    ret |= os08b10_write_reg(dev, 0x3cc2, 0x90);
    ret |= os08b10_write_reg(dev, 0x3cc5, 0x00);
    ret |= os08b10_write_reg(dev, 0x3cc6, 0x98);
    ret |= os08b10_write_reg(dev, 0x3cc7, 0x00);
    ret |= os08b10_write_reg(dev, 0x3cc8, 0x98);
    ret |= os08b10_write_reg(dev, 0x3cc9, 0x00);
    ret |= os08b10_write_reg(dev, 0x3cca, 0x98);
    ret |= os08b10_write_reg(dev, 0x3ccc, 0x01);
    ret |= os08b10_write_reg(dev, 0x3cd9, 0x7c);
    ret |= os08b10_write_reg(dev, 0x3cda, 0x7c);
    ret |= os08b10_write_reg(dev, 0x3cdb, 0x7c);
    ret |= os08b10_write_reg(dev, 0x3cdc, 0x7c);
    ret |= os08b10_write_reg(dev, 0x3d8c, 0x70);
    ret |= os08b10_write_reg(dev, 0x3d8d, 0x10);
    ret |= os08b10_write_reg(dev, 0x4001, 0xeb);
    ret |= os08b10_write_reg(dev, 0x4004, 0x00); /* blc target 0x4b */
    ret |= os08b10_write_reg(dev, 0x4005, 0x4b);
    ret |= os08b10_write_reg(dev, 0x4008, 0x02);
    ret |= os08b10_write_reg(dev, 0x4009, 0x0d);
    ret |= os08b10_write_reg(dev, 0x400a, 0x02);
    ret |= os08b10_write_reg(dev, 0x400b, 0x00);
    ret |= os08b10_write_reg(dev, 0x400e, 0x40);
    ret |= os08b10_write_reg(dev, 0x401a, 0x50);
    ret |= os08b10_write_reg(dev, 0x402e, 0x00); /* blc target 0x4b */
    ret |= os08b10_write_reg(dev, 0x402f, 0x4b);
    ret |= os08b10_write_reg(dev, 0x4030, 0x00);
    ret |= os08b10_write_reg(dev, 0x4031, 0x40);
    ret |= os08b10_write_reg(dev, 0x4032, 0x1f);
    ret |= os08b10_write_reg(dev, 0x4050, 0x01);
    ret |= os08b10_write_reg(dev, 0x4051, 0x06);
    ret |= os08b10_write_reg(dev, 0x405d, 0x00);
    ret |= os08b10_write_reg(dev, 0x405e, 0x00);
    ret |= os08b10_write_reg(dev, 0x4288, 0xce);
    ret |= os08b10_write_reg(dev, 0x4289, 0x04);
    ret |= os08b10_write_reg(dev, 0x428a, 0x40);
    ret |= os08b10_write_reg(dev, 0x430b, 0x0f);
    ret |= os08b10_write_reg(dev, 0x430c, 0xfc);
    ret |= os08b10_write_reg(dev, 0x430d, 0x00);
    ret |= os08b10_write_reg(dev, 0x430e, 0x00);
    ret |= os08b10_write_reg(dev, 0x4314, 0x04);
    ret |= os08b10_write_reg(dev, 0x4500, 0x1a);
    ret |= os08b10_write_reg(dev, 0x4501, 0x18);
    ret |= os08b10_write_reg(dev, 0x4504, 0x00);
    ret |= os08b10_write_reg(dev, 0x4505, 0x00);
    ret |= os08b10_write_reg(dev, 0x4506, 0x32);
    ret |= os08b10_write_reg(dev, 0x4507, 0x03);
    ret |= os08b10_write_reg(dev, 0x4508, 0x1a);
    ret |= os08b10_write_reg(dev, 0x450a, 0x12);
    ret |= os08b10_write_reg(dev, 0x450b, 0x28);
    ret |= os08b10_write_reg(dev, 0x450c, 0x00);
    ret |= os08b10_write_reg(dev, 0x450f, 0x80);
    ret |= os08b10_write_reg(dev, 0x4513, 0x00);
    ret |= os08b10_write_reg(dev, 0x4516, 0x28);
    ret |= os08b10_write_reg(dev, 0x4603, 0x00);
    ret |= os08b10_write_reg(dev, 0x460a, 0x50);
    ret |= os08b10_write_reg(dev, 0x4640, 0x62);
    ret |= os08b10_write_reg(dev, 0x464d, 0x00);
    ret |= os08b10_write_reg(dev, 0x4800, 0x04);
    ret |= os08b10_write_reg(dev, 0x480e, 0x04);
    ret |= os08b10_write_reg(dev, 0x4813, 0x84);
    ret |= os08b10_write_reg(dev, 0x4837, 0x0a);
    ret |= os08b10_write_reg(dev, 0x484b, 0x67);
    ret |= os08b10_write_reg(dev, 0x4850, 0x47);
    ret |= os08b10_write_reg(dev, 0x4851, 0xaa);
    ret |= os08b10_write_reg(dev, 0x4852, 0xff);
    ret |= os08b10_write_reg(dev, 0x4853, 0x20);
    ret |= os08b10_write_reg(dev, 0x4854, 0x08);
    ret |= os08b10_write_reg(dev, 0x4855, 0x30);
    ret |= os08b10_write_reg(dev, 0x4860, 0x00);
    ret |= os08b10_write_reg(dev, 0x4d00, 0x4d);
    ret |= os08b10_write_reg(dev, 0x4d01, 0x42);
    ret |= os08b10_write_reg(dev, 0x4d02, 0xb9);
    ret |= os08b10_write_reg(dev, 0x4d03, 0x94);
    ret |= os08b10_write_reg(dev, 0x4d04, 0x95);
    ret |= os08b10_write_reg(dev, 0x4d05, 0xce);
    ret |= os08b10_write_reg(dev, 0x4d09, 0x6f);
    ret |= os08b10_write_reg(dev, 0x5000, 0x03); /* sensor dpc 0x3 */
    ret |= os08b10_write_reg(dev, 0x5001, 0x0d);
    ret |= os08b10_write_reg(dev, 0x5080, 0x00);
    ret |= os08b10_write_reg(dev, 0x50c0, 0x00);
    ret |= os08b10_write_reg(dev, 0x5100, 0x00);
    ret |= os08b10_write_reg(dev, 0x5200, 0x70);
    ret |= os08b10_write_reg(dev, 0x5201, 0x70);
    ret |= os08b10_write_reg(dev, 0x5681, 0x02);
    ret |= os08b10_write_reg(dev, 0x5780, 0x53);
    ret |= os08b10_write_reg(dev, 0x5781, 0x03);
    ret |= os08b10_write_reg(dev, 0x5782, 0x18);
    ret |= os08b10_write_reg(dev, 0x5783, 0x3c);
    ret |= os08b10_write_reg(dev, 0x5784, 0x00);
    ret |= os08b10_write_reg(dev, 0x5785, 0x40);
    ret |= os08b10_write_reg(dev, 0x5786, 0x01);
    ret |= os08b10_write_reg(dev, 0x5788, 0x18);
    ret |= os08b10_write_reg(dev, 0x5789, 0x3c);
    ret |= os08b10_write_reg(dev, 0x5791, 0x08);
    ret |= os08b10_write_reg(dev, 0x5792, 0x11);
    ret |= os08b10_write_reg(dev, 0x5793, 0x33);
    ret |= os08b10_write_reg(dev, 0x3508, 0x01);
    ret |= os08b10_write_reg(dev, 0x3509, 0x80);
    ret |= os08b10_write_reg(dev, 0x350a, 0x01);
    ret |= os08b10_write_reg(dev, 0x350b, 0x01);
    ret |= os08b10_write_reg(dev, 0x350c, 0x00);
    ret |= os08b10_write_reg(dev, 0x3548, 0x01);
    ret |= os08b10_write_reg(dev, 0x3549, 0x80);
    ret |= os08b10_write_reg(dev, 0x354a, 0x01);
    ret |= os08b10_write_reg(dev, 0x354b, 0x01);
    ret |= os08b10_write_reg(dev, 0x354c, 0x00);
    os08b10_delay_ms(1);
    ret |= os08b10_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08b10_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("===  OS08B10_raw12_3840X2160_2to1wdr_30fps_4lane_24Mclk_init success!  ==========\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

xmedia_s32 os08b10_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case OS08B10_8M_30FPS_12BIT_LINEAR_MODE:
            ret = os08b10_4lane_linear_3840x2160_init(dev);
            break;

        case OS08B10_8M_30FPS_12BIT_WDR_MODE:
            ret = os08b10_4lane_wdr_3840x2160_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os08b10_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_os08b10_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_os08b10_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_os08b10_i2c_fd[dev]);
        g_os08b10_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_os08b10_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os08b10_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_os08b10_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_os08b10_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_os08b10_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = os08b10_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os08b10_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_os08b10_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_os08b10_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os08b10_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[OS08B10_DATA_BYTE];

    if (g_os08b10_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_os08b10_i2c_fd[dev], g_os08b10_i2c_addr[dev], addr, OS08B10_ADDR_BYTE, buf, OS08B10_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //OS08B10_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os08b10_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_os08b10_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_os08b10_i2c_fd[dev], g_os08b10_i2c_addr[dev], buf, OS08B10_ADDR_BYTE + OS08B10_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 os08b10_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    os08b10_read_reg(dev, OS08B10_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

    if (mirror_en) {
        mirror_flip_type |= 0x02;
    } else {
        mirror_flip_type &= 0xfd;
    }

    if (flip_en) {
        mirror_flip_type |= 0x04;
    } else {
        mirror_flip_type &= 0xfb;
    }

    ret = os08b10_write_reg(dev, OS08B10_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os08b10_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}
