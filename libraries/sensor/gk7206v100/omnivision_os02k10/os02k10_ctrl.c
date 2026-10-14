#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "os02k10.h"
#include "os02k10_ctrl.h"

#define OS02K10_REG_ADDR_MIRROR_FLIP    0x3820

static xmedia_s32 g_os02k10_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] =
                                                                     SENSOR_I2C_INVALID };
static xmedia_s32 g_os02k10_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] =
                                                                     SENSOR_I2C_INVALID };

static void os02k10_delay_ms(xmedia_s32 ms)
{
    usleep(ms * 1000); // 1ms: 1000us
    return;
}

static xmedia_s32 os02k10_2lane_linear_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os02k10_write_reg(dev, 0x0100,0x00);
    ret |= os02k10_write_reg(dev, 0x0103,0x01);
    os02k10_delay_ms(10);
    ret |= os02k10_write_reg(dev, 0x0103,0x00);
    os02k10_delay_ms(10);
    //27M 30fps 2lane 480Mbps/lane
    ret |= os02k10_write_reg(dev, 0x0109,0x01);
    ret |= os02k10_write_reg(dev, 0x0104,0x02);
    ret |= os02k10_write_reg(dev, 0x0102,0x00);
    ret |= os02k10_write_reg(dev, 0x0305,0x5c);
    ret |= os02k10_write_reg(dev, 0x0306,0x00);
    ret |= os02k10_write_reg(dev, 0x0307,0x01);
    ret |= os02k10_write_reg(dev, 0x030a,0x01);
    ret |= os02k10_write_reg(dev, 0x0317,0x09);
    ret |= os02k10_write_reg(dev, 0x0323,0x07);
    ret |= os02k10_write_reg(dev, 0x0324,0x01);
    ret |= os02k10_write_reg(dev, 0x0325,0xb0);
    ret |= os02k10_write_reg(dev, 0x0327,0x07);
    ret |= os02k10_write_reg(dev, 0x032c,0x02);
    ret |= os02k10_write_reg(dev, 0x032d,0x02);
    ret |= os02k10_write_reg(dev, 0x032e,0x05);
    ret |= os02k10_write_reg(dev, 0x300f,0x11);
    ret |= os02k10_write_reg(dev, 0x3012,0x21);
    ret |= os02k10_write_reg(dev, 0x3026,0x10);
    ret |= os02k10_write_reg(dev, 0x3027,0x08);
    ret |= os02k10_write_reg(dev, 0x302d,0x24);
    ret |= os02k10_write_reg(dev, 0x3103,0x29);
    ret |= os02k10_write_reg(dev, 0x3106,0x10);
    ret |= os02k10_write_reg(dev, 0x3400,0x00);
    ret |= os02k10_write_reg(dev, 0x3406,0x08);
    ret |= os02k10_write_reg(dev, 0x3408,0x05);
    ret |= os02k10_write_reg(dev, 0x340c,0x05);
    ret |= os02k10_write_reg(dev, 0x3425,0x51);
    ret |= os02k10_write_reg(dev, 0x3426,0x10);
    ret |= os02k10_write_reg(dev, 0x3427,0x14);
    ret |= os02k10_write_reg(dev, 0x3428,0x10);
    ret |= os02k10_write_reg(dev, 0x3429,0x10);
    ret |= os02k10_write_reg(dev, 0x342a,0x10);
    ret |= os02k10_write_reg(dev, 0x342b,0x04);
    ret |= os02k10_write_reg(dev, 0x3504,0x08);
    ret |= os02k10_write_reg(dev, 0x3508,0x01);
    ret |= os02k10_write_reg(dev, 0x3509,0x00);
    ret |= os02k10_write_reg(dev, 0x3544,0x08);
    ret |= os02k10_write_reg(dev, 0x3548,0x01);
    ret |= os02k10_write_reg(dev, 0x3549,0x00);
    ret |= os02k10_write_reg(dev, 0x3584,0x08);
    ret |= os02k10_write_reg(dev, 0x3588,0x01);
    ret |= os02k10_write_reg(dev, 0x3589,0x00);
    ret |= os02k10_write_reg(dev, 0x3601,0x70);
    ret |= os02k10_write_reg(dev, 0x3604,0xe3);
    ret |= os02k10_write_reg(dev, 0x3605,0x7f);
    ret |= os02k10_write_reg(dev, 0x3606,0x00);
    ret |= os02k10_write_reg(dev, 0x3608,0xa8);
    ret |= os02k10_write_reg(dev, 0x360a,0xd0);
    ret |= os02k10_write_reg(dev, 0x360b,0x08);
    ret |= os02k10_write_reg(dev, 0x360e,0xc8);
    ret |= os02k10_write_reg(dev, 0x360f,0x66);
    ret |= os02k10_write_reg(dev, 0x3610,0x81);
    ret |= os02k10_write_reg(dev, 0x3611,0x89);
    ret |= os02k10_write_reg(dev, 0x3612,0x4e);
    ret |= os02k10_write_reg(dev, 0x3613,0xbd);
    ret |= os02k10_write_reg(dev, 0x362a,0x0e);
    ret |= os02k10_write_reg(dev, 0x362b,0x0e);
    ret |= os02k10_write_reg(dev, 0x362c,0x0e);
    ret |= os02k10_write_reg(dev, 0x362d,0x0e);
    ret |= os02k10_write_reg(dev, 0x362e,0x0c);
    ret |= os02k10_write_reg(dev, 0x362f,0x1a);
    ret |= os02k10_write_reg(dev, 0x3630,0x32);
    ret |= os02k10_write_reg(dev, 0x3631,0x64);
    ret |= os02k10_write_reg(dev, 0x3638,0x00);
    ret |= os02k10_write_reg(dev, 0x3643,0x00);
    ret |= os02k10_write_reg(dev, 0x3644,0x00);
    ret |= os02k10_write_reg(dev, 0x3645,0x00);
    ret |= os02k10_write_reg(dev, 0x3646,0x00);
    ret |= os02k10_write_reg(dev, 0x3647,0x00);
    ret |= os02k10_write_reg(dev, 0x3648,0x00);
    ret |= os02k10_write_reg(dev, 0x3649,0x00);
    ret |= os02k10_write_reg(dev, 0x364a,0x04);
    ret |= os02k10_write_reg(dev, 0x364c,0x0e);
    ret |= os02k10_write_reg(dev, 0x364d,0x0e);
    ret |= os02k10_write_reg(dev, 0x364e,0x0e);
    ret |= os02k10_write_reg(dev, 0x364f,0x0e);
    ret |= os02k10_write_reg(dev, 0x3650,0xff);
    ret |= os02k10_write_reg(dev, 0x3651,0xff);
    ret |= os02k10_write_reg(dev, 0x3661,0x07);
    ret |= os02k10_write_reg(dev, 0x3662,0x02);
    ret |= os02k10_write_reg(dev, 0x3663,0x20);
    ret |= os02k10_write_reg(dev, 0x3665,0x12);
    ret |= os02k10_write_reg(dev, 0x3667,0xd4);
    ret |= os02k10_write_reg(dev, 0x3668,0x80);
    ret |= os02k10_write_reg(dev, 0x366f,0x00);
    ret |= os02k10_write_reg(dev, 0x3670,0xc7);
    ret |= os02k10_write_reg(dev, 0x3671,0x08);
    ret |= os02k10_write_reg(dev, 0x3673,0x2a);
    ret |= os02k10_write_reg(dev, 0x3681,0x80);
    ret |= os02k10_write_reg(dev, 0x3700,0x26);
    ret |= os02k10_write_reg(dev, 0x3701,0x1e);
    ret |= os02k10_write_reg(dev, 0x3702,0x25);
    ret |= os02k10_write_reg(dev, 0x3703,0x28);
    ret |= os02k10_write_reg(dev, 0x3706,0x3e);
    ret |= os02k10_write_reg(dev, 0x3707,0x0a);
    ret |= os02k10_write_reg(dev, 0x3708,0x36);
    ret |= os02k10_write_reg(dev, 0x3709,0x55);
    ret |= os02k10_write_reg(dev, 0x370a,0x00);
    ret |= os02k10_write_reg(dev, 0x370b,0xa3);
    ret |= os02k10_write_reg(dev, 0x3714,0x01);
    ret |= os02k10_write_reg(dev, 0x371b,0x16);
    ret |= os02k10_write_reg(dev, 0x371c,0x00);
    ret |= os02k10_write_reg(dev, 0x371d,0x08);
    ret |= os02k10_write_reg(dev, 0x3756,0x9b);
    ret |= os02k10_write_reg(dev, 0x3757,0x9b);
    ret |= os02k10_write_reg(dev, 0x3762,0x1d);
    ret |= os02k10_write_reg(dev, 0x376c,0x00);
    ret |= os02k10_write_reg(dev, 0x3776,0x05);
    ret |= os02k10_write_reg(dev, 0x3777,0x22);
    ret |= os02k10_write_reg(dev, 0x3779,0x60);
    ret |= os02k10_write_reg(dev, 0x377c,0x48);
    ret |= os02k10_write_reg(dev, 0x3783,0x02);
    ret |= os02k10_write_reg(dev, 0x3784,0x06);
    ret |= os02k10_write_reg(dev, 0x3785,0x0a);
    ret |= os02k10_write_reg(dev, 0x3790,0x10);
    ret |= os02k10_write_reg(dev, 0x3793,0x04);
    ret |= os02k10_write_reg(dev, 0x3794,0x07);
    ret |= os02k10_write_reg(dev, 0x3796,0x00);
    ret |= os02k10_write_reg(dev, 0x3797,0x02);
    ret |= os02k10_write_reg(dev, 0x379c,0x4d);
    ret |= os02k10_write_reg(dev, 0x37a1,0x80);
    ret |= os02k10_write_reg(dev, 0x37bb,0x88);
    ret |= os02k10_write_reg(dev, 0x37bd,0x01);
    ret |= os02k10_write_reg(dev, 0x37be,0x01);
    ret |= os02k10_write_reg(dev, 0x37bf,0x00);
    ret |= os02k10_write_reg(dev, 0x37c0,0x01);
    ret |= os02k10_write_reg(dev, 0x37c7,0x56);
    ret |= os02k10_write_reg(dev, 0x37ca,0x21);
    ret |= os02k10_write_reg(dev, 0x37cc,0x13);
    ret |= os02k10_write_reg(dev, 0x37cd,0x90);
    ret |= os02k10_write_reg(dev, 0x37cf,0x02);
    ret |= os02k10_write_reg(dev, 0x37d1,0x3e);
    ret |= os02k10_write_reg(dev, 0x37d2,0x00);
    ret |= os02k10_write_reg(dev, 0x37d3,0xa3);
    ret |= os02k10_write_reg(dev, 0x37d5,0x3e);
    ret |= os02k10_write_reg(dev, 0x37d6,0x00);
    ret |= os02k10_write_reg(dev, 0x37d7,0xa3);
    ret |= os02k10_write_reg(dev, 0x37d8,0x01);
    ret |= os02k10_write_reg(dev, 0x37da,0x00);
    ret |= os02k10_write_reg(dev, 0x37db,0x00);
    ret |= os02k10_write_reg(dev, 0x37dc,0x00);
    ret |= os02k10_write_reg(dev, 0x37dd,0x00);
    ret |= os02k10_write_reg(dev, 0x3800,0x00);
    ret |= os02k10_write_reg(dev, 0x3801,0x00);
    ret |= os02k10_write_reg(dev, 0x3802,0x00);
    ret |= os02k10_write_reg(dev, 0x3803,0x04);
    ret |= os02k10_write_reg(dev, 0x3804,0x07);
    ret |= os02k10_write_reg(dev, 0x3805,0x8f);
    ret |= os02k10_write_reg(dev, 0x3806,0x04);
    ret |= os02k10_write_reg(dev, 0x3807,0x43);
    ret |= os02k10_write_reg(dev, 0x3808,0x07);
    ret |= os02k10_write_reg(dev, 0x3809,0x80);
    ret |= os02k10_write_reg(dev, 0x380a,0x04);
    ret |= os02k10_write_reg(dev, 0x380b,0x38);
    ret |= os02k10_write_reg(dev, 0x380c,0x04);
    ret |= os02k10_write_reg(dev, 0x380d,0xd0);
    ret |= os02k10_write_reg(dev, 0x380e,0x04);
    ret |= os02k10_write_reg(dev, 0x380f,0xe2);
    ret |= os02k10_write_reg(dev, 0x3811,0x08);
    ret |= os02k10_write_reg(dev, 0x3813,0x04);
    ret |= os02k10_write_reg(dev, 0x3814,0x01);
    ret |= os02k10_write_reg(dev, 0x3815,0x01);
    ret |= os02k10_write_reg(dev, 0x3816,0x01);
    ret |= os02k10_write_reg(dev, 0x3817,0x01);
    ret |= os02k10_write_reg(dev, 0x381c,0x00);
    ret |= os02k10_write_reg(dev, 0x3820,0x02);
    ret |= os02k10_write_reg(dev, 0x3821,0x00);
    ret |= os02k10_write_reg(dev, 0x3822,0x14);
    ret |= os02k10_write_reg(dev, 0x3833,0x41);
    ret |= os02k10_write_reg(dev, 0x384c,0x02);
    ret |= os02k10_write_reg(dev, 0x384d,0xd0);
    ret |= os02k10_write_reg(dev, 0x3858,0x0d);
    ret |= os02k10_write_reg(dev, 0x3865,0x00);
    ret |= os02k10_write_reg(dev, 0x3866,0xc0);
    ret |= os02k10_write_reg(dev, 0x3867,0x00);
    ret |= os02k10_write_reg(dev, 0x3868,0xc0);
    ret |= os02k10_write_reg(dev, 0x3900,0x13);
    ret |= os02k10_write_reg(dev, 0x3940,0x13);
    ret |= os02k10_write_reg(dev, 0x3980,0x13);
    ret |= os02k10_write_reg(dev, 0x390c,0x03);
    ret |= os02k10_write_reg(dev, 0x390d,0x02);
    ret |= os02k10_write_reg(dev, 0x390e,0x01);
    ret |= os02k10_write_reg(dev, 0x390f,0x03);
    ret |= os02k10_write_reg(dev, 0x3910,0x02);
    ret |= os02k10_write_reg(dev, 0x3911,0x01);
    ret |= os02k10_write_reg(dev, 0x394c,0x02);
    ret |= os02k10_write_reg(dev, 0x394d,0x02);
    ret |= os02k10_write_reg(dev, 0x394e,0x01);
    ret |= os02k10_write_reg(dev, 0x394f,0x02);
    ret |= os02k10_write_reg(dev, 0x3950,0x02);
    ret |= os02k10_write_reg(dev, 0x3951,0x01);
    ret |= os02k10_write_reg(dev, 0x398c,0x02);
    ret |= os02k10_write_reg(dev, 0x398d,0x01);
    ret |= os02k10_write_reg(dev, 0x398e,0x01);
    ret |= os02k10_write_reg(dev, 0x398f,0x02);
    ret |= os02k10_write_reg(dev, 0x3990,0x01);
    ret |= os02k10_write_reg(dev, 0x3991,0x01);
    ret |= os02k10_write_reg(dev, 0x5395,0x38);
    ret |= os02k10_write_reg(dev, 0x5392,0x14);
    ret |= os02k10_write_reg(dev, 0x5396,0x02);
    ret |= os02k10_write_reg(dev, 0x5397,0x01);
    ret |= os02k10_write_reg(dev, 0x5398,0x01);
    ret |= os02k10_write_reg(dev, 0x5399,0x02);
    ret |= os02k10_write_reg(dev, 0x539a,0x01);
    ret |= os02k10_write_reg(dev, 0x539b,0x01);
    ret |= os02k10_write_reg(dev, 0x5415,0x38);
    ret |= os02k10_write_reg(dev, 0x5412,0x14);
    ret |= os02k10_write_reg(dev, 0x5416,0x01);
    ret |= os02k10_write_reg(dev, 0x5417,0x01);
    ret |= os02k10_write_reg(dev, 0x5418,0x01);
    ret |= os02k10_write_reg(dev, 0x5419,0x01);
    ret |= os02k10_write_reg(dev, 0x541a,0x01);
    ret |= os02k10_write_reg(dev, 0x541b,0x01);
    ret |= os02k10_write_reg(dev, 0x5495,0x38);
    ret |= os02k10_write_reg(dev, 0x5492,0x14);
    ret |= os02k10_write_reg(dev, 0x5496,0x01);
    ret |= os02k10_write_reg(dev, 0x5497,0x01);
    ret |= os02k10_write_reg(dev, 0x5498,0x01);
    ret |= os02k10_write_reg(dev, 0x5499,0x01);
    ret |= os02k10_write_reg(dev, 0x549a,0x01);
    ret |= os02k10_write_reg(dev, 0x549b,0x01);
    ret |= os02k10_write_reg(dev, 0x3c01,0x11);
    ret |= os02k10_write_reg(dev, 0x3c05,0x00);
    ret |= os02k10_write_reg(dev, 0x3c0f,0x1c);
    ret |= os02k10_write_reg(dev, 0x3c12,0x0d);
    ret |= os02k10_write_reg(dev, 0x3c14,0x21);
    ret |= os02k10_write_reg(dev, 0x3c19,0x01);
    ret |= os02k10_write_reg(dev, 0x3c21,0x40);
    ret |= os02k10_write_reg(dev, 0x3c3b,0x18);
    ret |= os02k10_write_reg(dev, 0x3c3d,0xc9);
    ret |= os02k10_write_reg(dev, 0x3c55,0xcb);
    ret |= os02k10_write_reg(dev, 0x3c5d,0xcf);
    ret |= os02k10_write_reg(dev, 0x3c5e,0xcf);
    ret |= os02k10_write_reg(dev, 0x3ce0,0x00);
    ret |= os02k10_write_reg(dev, 0x3ce1,0x00);
    ret |= os02k10_write_reg(dev, 0x3ce2,0x00);
    ret |= os02k10_write_reg(dev, 0x3ce3,0x00);
    ret |= os02k10_write_reg(dev, 0x3d8c,0x70);
    ret |= os02k10_write_reg(dev, 0x3d8d,0x10);
    ret |= os02k10_write_reg(dev, 0x4001,0x2f);
    ret |= os02k10_write_reg(dev, 0x4033,0x80);
    ret |= os02k10_write_reg(dev, 0x4008,0x02);
    ret |= os02k10_write_reg(dev, 0x4009,0x11);
    ret |= os02k10_write_reg(dev, 0x4004,0x00);
    ret |= os02k10_write_reg(dev, 0x4005,0x40);
    ret |= os02k10_write_reg(dev, 0x400a,0x01);
    ret |= os02k10_write_reg(dev, 0x400b,0x3c);
    ret |= os02k10_write_reg(dev, 0x400e,0x40);
    ret |= os02k10_write_reg(dev, 0x4011,0xbb);
    ret |= os02k10_write_reg(dev, 0x410f,0x01);
    ret |= os02k10_write_reg(dev, 0x4028,0x6f);
    ret |= os02k10_write_reg(dev, 0x4029,0x0f);
    ret |= os02k10_write_reg(dev, 0x402a,0x3f);
    ret |= os02k10_write_reg(dev, 0x402b,0x01);
    ret |= os02k10_write_reg(dev, 0x402e,0x00);
    ret |= os02k10_write_reg(dev, 0x402f,0x40);
    ret |= os02k10_write_reg(dev, 0x4030,0x00);
    ret |= os02k10_write_reg(dev, 0x4031,0x40);
    ret |= os02k10_write_reg(dev, 0x4032,0x2f);
    ret |= os02k10_write_reg(dev, 0x4050,0x00);
    ret |= os02k10_write_reg(dev, 0x4051,0x07);
    ret |= os02k10_write_reg(dev, 0x4288,0xcf);
    ret |= os02k10_write_reg(dev, 0x4289,0x03);
    ret |= os02k10_write_reg(dev, 0x428a,0x46);
    ret |= os02k10_write_reg(dev, 0x430b,0x0f);
    ret |= os02k10_write_reg(dev, 0x430c,0xfc);
    ret |= os02k10_write_reg(dev, 0x430d,0x00);
    ret |= os02k10_write_reg(dev, 0x430e,0x00);
    ret |= os02k10_write_reg(dev, 0x4314,0x04);
    ret |= os02k10_write_reg(dev, 0x4500,0x18);
    ret |= os02k10_write_reg(dev, 0x4501,0x18);
    ret |= os02k10_write_reg(dev, 0x4504,0x00);
    ret |= os02k10_write_reg(dev, 0x4507,0x02);
    ret |= os02k10_write_reg(dev, 0x4508,0x1a);
    ret |= os02k10_write_reg(dev, 0x4603,0x00);
    ret |= os02k10_write_reg(dev, 0x4640,0x62);
    ret |= os02k10_write_reg(dev, 0x4646,0xaa);
    ret |= os02k10_write_reg(dev, 0x4647,0x55);
    ret |= os02k10_write_reg(dev, 0x4648,0x99);
    ret |= os02k10_write_reg(dev, 0x4649,0x66);
    ret |= os02k10_write_reg(dev, 0x464d,0x00);
    ret |= os02k10_write_reg(dev, 0x4654,0x11);
    ret |= os02k10_write_reg(dev, 0x4655,0x22);
    ret |= os02k10_write_reg(dev, 0x4800,0x04);
    ret |= os02k10_write_reg(dev, 0x4810,0xff);
    ret |= os02k10_write_reg(dev, 0x4811,0xff);
    ret |= os02k10_write_reg(dev, 0x480e,0x00);
    ret |= os02k10_write_reg(dev, 0x4813,0x00);
    ret |= os02k10_write_reg(dev, 0x4837,0x0e);
    ret |= os02k10_write_reg(dev, 0x484b,0x27);
    ret |= os02k10_write_reg(dev, 0x4d00,0x4e);
    ret |= os02k10_write_reg(dev, 0x4d01,0x0c);
    ret |= os02k10_write_reg(dev, 0x4d02,0xb8);
    ret |= os02k10_write_reg(dev, 0x4d03,0xea);
    ret |= os02k10_write_reg(dev, 0x4d04,0x74);
    ret |= os02k10_write_reg(dev, 0x4d05,0xb7);
    ret |= os02k10_write_reg(dev, 0x4d09,0x4f);
    ret |= os02k10_write_reg(dev, 0x5000,0x1f);
    ret |= os02k10_write_reg(dev, 0x5080,0x00);
    ret |= os02k10_write_reg(dev, 0x50c0,0x00);
    ret |= os02k10_write_reg(dev, 0x5100,0x00);
    ret |= os02k10_write_reg(dev, 0x5200,0x00);
    ret |= os02k10_write_reg(dev, 0x5201,0x70);
    ret |= os02k10_write_reg(dev, 0x5202,0x03);
    ret |= os02k10_write_reg(dev, 0x5203,0x7f);
    ret |= os02k10_write_reg(dev, 0x5780,0x53);
    ret |= os02k10_write_reg(dev, 0x5786,0x01);
    ret |= os02k10_write_reg(dev, 0x3501,0x02);
    ret |= os02k10_write_reg(dev, 0x0305,0x50);
    ret |= os02k10_write_reg(dev, 0x4837,0x10);
    ret |= os02k10_write_reg(dev, 0x380c,0x0b);
    ret |= os02k10_write_reg(dev, 0x380d,0x40);
    ret |= os02k10_write_reg(dev, 0x380e,0x05);
    ret |= os02k10_write_reg(dev, 0x380f,0x7e);
    ret |= os02k10_write_reg(dev, 0x3501,0x04);
    ret |= os02k10_write_reg(dev, 0x3502,0xda);
    ret |= os02k10_write_reg(dev, 0x0100,0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os02k10_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("==== OS02K10_27MInput_MIPI_2lane_10bit_linear_1920x1080_30fps init success!============\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os02k10_4lane_builtin_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os02k10_write_reg(dev, 0x302a, 0x00);
    ret |= os02k10_write_reg(dev, 0x0103, 0x01);
    ret |= os02k10_write_reg(dev, 0x0109, 0x01);
    ret |= os02k10_write_reg(dev, 0x0104, 0x02);
    ret |= os02k10_write_reg(dev, 0x0102, 0x00);
    ret |= os02k10_write_reg(dev, 0x0305, 0x6c);
    ret |= os02k10_write_reg(dev, 0x0306, 0x00);
    ret |= os02k10_write_reg(dev, 0x0307, 0x00);
    ret |= os02k10_write_reg(dev, 0x030a, 0x01);
    ret |= os02k10_write_reg(dev, 0x0317, 0x0a);
    ret |= os02k10_write_reg(dev, 0x0323, 0x07);
    ret |= os02k10_write_reg(dev, 0x0324, 0x01);
    ret |= os02k10_write_reg(dev, 0x0325, 0x80);
    ret |= os02k10_write_reg(dev, 0x0327, 0x07);
    ret |= os02k10_write_reg(dev, 0x032c, 0x02);
    ret |= os02k10_write_reg(dev, 0x032d, 0x02);
    ret |= os02k10_write_reg(dev, 0x032e, 0x05);
    ret |= os02k10_write_reg(dev, 0x300f, 0x11);
    ret |= os02k10_write_reg(dev, 0x3012, 0x41);
    ret |= os02k10_write_reg(dev, 0x3026, 0x10);
    ret |= os02k10_write_reg(dev, 0x3027, 0x08);
    ret |= os02k10_write_reg(dev, 0x302d, 0x24);
    ret |= os02k10_write_reg(dev, 0x3103, 0x29);
    ret |= os02k10_write_reg(dev, 0x3106, 0x10);
    ret |= os02k10_write_reg(dev, 0x3400, 0x00);
    ret |= os02k10_write_reg(dev, 0x3406, 0x08);
    ret |= os02k10_write_reg(dev, 0x3408, 0x05);
    ret |= os02k10_write_reg(dev, 0x3409, 0x28);
    ret |= os02k10_write_reg(dev, 0x340c, 0x05);
    ret |= os02k10_write_reg(dev, 0x3425, 0x51);
    ret |= os02k10_write_reg(dev, 0x3426, 0x10);
    ret |= os02k10_write_reg(dev, 0x3427, 0x14);
    ret |= os02k10_write_reg(dev, 0x3428, 0x10);
    ret |= os02k10_write_reg(dev, 0x3429, 0x10);
    ret |= os02k10_write_reg(dev, 0x342a, 0x10);
    ret |= os02k10_write_reg(dev, 0x342b, 0x04);
    ret |= os02k10_write_reg(dev, 0x3504, 0x08);
    ret |= os02k10_write_reg(dev, 0x3508, 0x01);
    ret |= os02k10_write_reg(dev, 0x3509, 0x00);
    ret |= os02k10_write_reg(dev, 0x3544, 0x08);
    ret |= os02k10_write_reg(dev, 0x3548, 0x01);
    ret |= os02k10_write_reg(dev, 0x3549, 0x00);
    ret |= os02k10_write_reg(dev, 0x3584, 0x08);
    ret |= os02k10_write_reg(dev, 0x3588, 0x01);
    ret |= os02k10_write_reg(dev, 0x3589, 0x00);
    ret |= os02k10_write_reg(dev, 0x3601, 0x70);
    ret |= os02k10_write_reg(dev, 0x3604, 0xe3);
    ret |= os02k10_write_reg(dev, 0x3605, 0xff);
    ret |= os02k10_write_reg(dev, 0x3606, 0x01);
    ret |= os02k10_write_reg(dev, 0x3608, 0xa8);
    ret |= os02k10_write_reg(dev, 0x360a, 0xd0);
    ret |= os02k10_write_reg(dev, 0x360b, 0x08);
    ret |= os02k10_write_reg(dev, 0x360e, 0xc8);
    ret |= os02k10_write_reg(dev, 0x360f, 0x66);
    ret |= os02k10_write_reg(dev, 0x3610, 0x81);
    ret |= os02k10_write_reg(dev, 0x3611, 0x89);
    ret |= os02k10_write_reg(dev, 0x3612, 0x4e);
    ret |= os02k10_write_reg(dev, 0x3613, 0xbd);
    ret |= os02k10_write_reg(dev, 0x362a, 0x0e);
    ret |= os02k10_write_reg(dev, 0x362b, 0x0e);
    ret |= os02k10_write_reg(dev, 0x362c, 0x0e);
    ret |= os02k10_write_reg(dev, 0x362d, 0x0e);
    ret |= os02k10_write_reg(dev, 0x362e, 0x0c);
    ret |= os02k10_write_reg(dev, 0x362f, 0x1a);
    ret |= os02k10_write_reg(dev, 0x3630, 0x32);
    ret |= os02k10_write_reg(dev, 0x3631, 0x64);
    ret |= os02k10_write_reg(dev, 0x3638, 0x00);
    ret |= os02k10_write_reg(dev, 0x3643, 0x00);
    ret |= os02k10_write_reg(dev, 0x3644, 0x00);
    ret |= os02k10_write_reg(dev, 0x3645, 0x00);
    ret |= os02k10_write_reg(dev, 0x3646, 0x00);
    ret |= os02k10_write_reg(dev, 0x3647, 0x00);
    ret |= os02k10_write_reg(dev, 0x3648, 0x00);
    ret |= os02k10_write_reg(dev, 0x3649, 0x00);
    ret |= os02k10_write_reg(dev, 0x364a, 0x04);
    ret |= os02k10_write_reg(dev, 0x364c, 0x0e);
    ret |= os02k10_write_reg(dev, 0x364d, 0x0e);
    ret |= os02k10_write_reg(dev, 0x364e, 0x0e);
    ret |= os02k10_write_reg(dev, 0x364f, 0x0e);
    ret |= os02k10_write_reg(dev, 0x3650, 0xff);
    ret |= os02k10_write_reg(dev, 0x3651, 0xff);
    ret |= os02k10_write_reg(dev, 0x3661, 0x07);
    ret |= os02k10_write_reg(dev, 0x3662, 0x00);
    ret |= os02k10_write_reg(dev, 0x3663, 0x20);
    ret |= os02k10_write_reg(dev, 0x3665, 0x12);
    ret |= os02k10_write_reg(dev, 0x3667, 0xd4);
    ret |= os02k10_write_reg(dev, 0x3668, 0x80);
    ret |= os02k10_write_reg(dev, 0x366f, 0x00);
    ret |= os02k10_write_reg(dev, 0x3670, 0x07);
    ret |= os02k10_write_reg(dev, 0x3671, 0x34);
    ret |= os02k10_write_reg(dev, 0x3673, 0x2a);
    ret |= os02k10_write_reg(dev, 0x3681, 0x80);
    ret |= os02k10_write_reg(dev, 0x3700, 0x26);
    ret |= os02k10_write_reg(dev, 0x3701, 0x1e);
    ret |= os02k10_write_reg(dev, 0x3702, 0x25);
    ret |= os02k10_write_reg(dev, 0x3703, 0x28);
    ret |= os02k10_write_reg(dev, 0x3706, 0xb1);
    ret |= os02k10_write_reg(dev, 0x3707, 0x0a);
    ret |= os02k10_write_reg(dev, 0x3708, 0x34);
    ret |= os02k10_write_reg(dev, 0x3709, 0x50);
    ret |= os02k10_write_reg(dev, 0x370a, 0x02);
    ret |= os02k10_write_reg(dev, 0x370b, 0x21);
    ret |= os02k10_write_reg(dev, 0x3714, 0x01);
    ret |= os02k10_write_reg(dev, 0x371b, 0x16);
    ret |= os02k10_write_reg(dev, 0x371c, 0x00);
    ret |= os02k10_write_reg(dev, 0x371d, 0x08);
    ret |= os02k10_write_reg(dev, 0x3756, 0xe7);
    ret |= os02k10_write_reg(dev, 0x3757, 0xe7);
    ret |= os02k10_write_reg(dev, 0x3762, 0x1d);
    ret |= os02k10_write_reg(dev, 0x376c, 0x01);
    ret |= os02k10_write_reg(dev, 0x3776, 0x05);
    ret |= os02k10_write_reg(dev, 0x3777, 0x22);
    ret |= os02k10_write_reg(dev, 0x3779, 0x60);
    ret |= os02k10_write_reg(dev, 0x377c, 0x48);
    ret |= os02k10_write_reg(dev, 0x3783, 0x02);
    ret |= os02k10_write_reg(dev, 0x3784, 0x06);
    ret |= os02k10_write_reg(dev, 0x3785, 0x0a);
    ret |= os02k10_write_reg(dev, 0x3790, 0x10);
    ret |= os02k10_write_reg(dev, 0x3793, 0x04);
    ret |= os02k10_write_reg(dev, 0x3794, 0x07);
    ret |= os02k10_write_reg(dev, 0x3796, 0x00);
    ret |= os02k10_write_reg(dev, 0x3797, 0x02);
    ret |= os02k10_write_reg(dev, 0x379c, 0x4d);
    ret |= os02k10_write_reg(dev, 0x37a1, 0x80);
    ret |= os02k10_write_reg(dev, 0x37bb, 0x88);
    ret |= os02k10_write_reg(dev, 0x37be, 0xb8);
    ret |= os02k10_write_reg(dev, 0x37bf, 0x00);
    ret |= os02k10_write_reg(dev, 0x37c0, 0xcb);
    ret |= os02k10_write_reg(dev, 0x37c7, 0x56);
    ret |= os02k10_write_reg(dev, 0x37ca, 0x21);
    ret |= os02k10_write_reg(dev, 0x37cc, 0x13);
    ret |= os02k10_write_reg(dev, 0x37cd, 0x90);
    ret |= os02k10_write_reg(dev, 0x37cf, 0x02);
    ret |= os02k10_write_reg(dev, 0x37d1, 0xb1);
    ret |= os02k10_write_reg(dev, 0x37d2, 0x02);
    ret |= os02k10_write_reg(dev, 0x37d3, 0x21);
    ret |= os02k10_write_reg(dev, 0x37d5, 0xb1);
    ret |= os02k10_write_reg(dev, 0x37d6, 0x02);
    ret |= os02k10_write_reg(dev, 0x37d7, 0x21);
    ret |= os02k10_write_reg(dev, 0x37d8, 0x01);
    ret |= os02k10_write_reg(dev, 0x37da, 0x00);
    ret |= os02k10_write_reg(dev, 0x37db, 0x00);
    ret |= os02k10_write_reg(dev, 0x37dc, 0x00);
    ret |= os02k10_write_reg(dev, 0x37dd, 0x00);
    ret |= os02k10_write_reg(dev, 0x3800, 0x00);
    ret |= os02k10_write_reg(dev, 0x3801, 0x00);
    ret |= os02k10_write_reg(dev, 0x3802, 0x00);
    ret |= os02k10_write_reg(dev, 0x3803, 0x04);
    ret |= os02k10_write_reg(dev, 0x3804, 0x07);
    ret |= os02k10_write_reg(dev, 0x3805, 0x8f);
    ret |= os02k10_write_reg(dev, 0x3806, 0x04);
    ret |= os02k10_write_reg(dev, 0x3807, 0x43);
    ret |= os02k10_write_reg(dev, 0x3808, 0x07);
    ret |= os02k10_write_reg(dev, 0x3809, 0x80);
    ret |= os02k10_write_reg(dev, 0x380a, 0x04);
    ret |= os02k10_write_reg(dev, 0x380b, 0x38);
    ret |= os02k10_write_reg(dev, 0x380c, 0x09);
    ret |= os02k10_write_reg(dev, 0x380d, 0x90);
    ret |= os02k10_write_reg(dev, 0x380e, 0x05);
    ret |= os02k10_write_reg(dev, 0x380f, 0xb4);
    ret |= os02k10_write_reg(dev, 0x3811, 0x08);
    ret |= os02k10_write_reg(dev, 0x3813, 0x04);
    ret |= os02k10_write_reg(dev, 0x3814, 0x01);
    ret |= os02k10_write_reg(dev, 0x3815, 0x01);
    ret |= os02k10_write_reg(dev, 0x3816, 0x01);
    ret |= os02k10_write_reg(dev, 0x3817, 0x01);
    ret |= os02k10_write_reg(dev, 0x381c, 0x00);
    ret |= os02k10_write_reg(dev, 0x3820, 0x02);
    ret |= os02k10_write_reg(dev, 0x3821, 0x00);
    ret |= os02k10_write_reg(dev, 0x3822, 0x14);
    ret |= os02k10_write_reg(dev, 0x384c, 0x04);
    ret |= os02k10_write_reg(dev, 0x384d, 0xc8);
    ret |= os02k10_write_reg(dev, 0x3858, 0x0d);
    ret |= os02k10_write_reg(dev, 0x3865, 0x01);
    ret |= os02k10_write_reg(dev, 0x3866, 0xa0);
    ret |= os02k10_write_reg(dev, 0x3867, 0x00);
    ret |= os02k10_write_reg(dev, 0x3868, 0x20);
    ret |= os02k10_write_reg(dev, 0x3900, 0x13);
    ret |= os02k10_write_reg(dev, 0x3940, 0x13);
    ret |= os02k10_write_reg(dev, 0x3980, 0x13);
    ret |= os02k10_write_reg(dev, 0x3c01, 0x10);
    ret |= os02k10_write_reg(dev, 0x3c05, 0x00);
    ret |= os02k10_write_reg(dev, 0x3c0f, 0x1c);
    ret |= os02k10_write_reg(dev, 0x3c12, 0x0d);
    ret |= os02k10_write_reg(dev, 0x3c14, 0x21);
    ret |= os02k10_write_reg(dev, 0x3c19, 0x00);
    ret |= os02k10_write_reg(dev, 0x3c21, 0x00);
    ret |= os02k10_write_reg(dev, 0x3c3b, 0x18);
    ret |= os02k10_write_reg(dev, 0x3c3d, 0xc9);
    ret |= os02k10_write_reg(dev, 0x3c55, 0xcb);
    ret |= os02k10_write_reg(dev, 0x3c5d, 0xec);
    ret |= os02k10_write_reg(dev, 0x3c5e, 0xec);
    ret |= os02k10_write_reg(dev, 0x3ce0, 0x00);
    ret |= os02k10_write_reg(dev, 0x3ce1, 0x00);
    ret |= os02k10_write_reg(dev, 0x3ce2, 0x00);
    ret |= os02k10_write_reg(dev, 0x3ce3, 0x00);
    ret |= os02k10_write_reg(dev, 0x3d8c, 0x70);
    ret |= os02k10_write_reg(dev, 0x3d8d, 0x10);
    ret |= os02k10_write_reg(dev, 0x4001, 0x2f);
    ret |= os02k10_write_reg(dev, 0x4033, 0x80);
    ret |= os02k10_write_reg(dev, 0x4008, 0x02);
    ret |= os02k10_write_reg(dev, 0x4009, 0x11);
    ret |= os02k10_write_reg(dev, 0x4004, 0x01);
    ret |= os02k10_write_reg(dev, 0x4005, 0x00);
    ret |= os02k10_write_reg(dev, 0x400a, 0x03);
    ret |= os02k10_write_reg(dev, 0x400b, 0x3f);
    ret |= os02k10_write_reg(dev, 0x400e, 0x40);
    ret |= os02k10_write_reg(dev, 0x4011, 0xbb);
    ret |= os02k10_write_reg(dev, 0x410f, 0x01);
    ret |= os02k10_write_reg(dev, 0x402e, 0x01);
    ret |= os02k10_write_reg(dev, 0x402f, 0x00);
    ret |= os02k10_write_reg(dev, 0x4030, 0x01);
    ret |= os02k10_write_reg(dev, 0x4031, 0x00);
    ret |= os02k10_write_reg(dev, 0x4032, 0x9f);
    ret |= os02k10_write_reg(dev, 0x4050, 0x00);
    ret |= os02k10_write_reg(dev, 0x4051, 0x07);
    ret |= os02k10_write_reg(dev, 0x4288, 0xcf);
    ret |= os02k10_write_reg(dev, 0x4289, 0x03);
    ret |= os02k10_write_reg(dev, 0x428a, 0x56);
    ret |= os02k10_write_reg(dev, 0x430b, 0xff);
    ret |= os02k10_write_reg(dev, 0x430c, 0xff);
    ret |= os02k10_write_reg(dev, 0x430d, 0x00);
    ret |= os02k10_write_reg(dev, 0x430e, 0x00);
    ret |= os02k10_write_reg(dev, 0x4314, 0x00);
    ret |= os02k10_write_reg(dev, 0x4500, 0x18);
    ret |= os02k10_write_reg(dev, 0x4501, 0x18);
    ret |= os02k10_write_reg(dev, 0x4504, 0x00);
    ret |= os02k10_write_reg(dev, 0x4507, 0x02);
    ret |= os02k10_write_reg(dev, 0x4508, 0x80);
    ret |= os02k10_write_reg(dev, 0x4603, 0x00);
    ret |= os02k10_write_reg(dev, 0x4640, 0x62);
    ret |= os02k10_write_reg(dev, 0x4646, 0xaa);
    ret |= os02k10_write_reg(dev, 0x4647, 0x55);
    ret |= os02k10_write_reg(dev, 0x4648, 0x99);
    ret |= os02k10_write_reg(dev, 0x4649, 0x66);
    ret |= os02k10_write_reg(dev, 0x464d, 0x00);
    ret |= os02k10_write_reg(dev, 0x4654, 0x11);
    ret |= os02k10_write_reg(dev, 0x4655, 0x22);
    ret |= os02k10_write_reg(dev, 0x4800, 0x04);
    ret |= os02k10_write_reg(dev, 0x4810, 0xff);
    ret |= os02k10_write_reg(dev, 0x4811, 0xff);
    ret |= os02k10_write_reg(dev, 0x480e, 0x00);
    ret |= os02k10_write_reg(dev, 0x4813, 0x00);
    ret |= os02k10_write_reg(dev, 0x4837, 0x0c);
    ret |= os02k10_write_reg(dev, 0x484b, 0x27);
    ret |= os02k10_write_reg(dev, 0x4d00, 0x4e);
    ret |= os02k10_write_reg(dev, 0x4d01, 0x0c);
    ret |= os02k10_write_reg(dev, 0x4d09, 0x4f);
    ret |= os02k10_write_reg(dev, 0x5000, 0x1f);
    ret |= os02k10_write_reg(dev, 0x5080, 0x00);
    ret |= os02k10_write_reg(dev, 0x50c0, 0x00);
    ret |= os02k10_write_reg(dev, 0x5100, 0x00);
    ret |= os02k10_write_reg(dev, 0x5200, 0x00);
    ret |= os02k10_write_reg(dev, 0x5201, 0x70);
    ret |= os02k10_write_reg(dev, 0x5202, 0x03);
    ret |= os02k10_write_reg(dev, 0x5203, 0x7f);
    ret |= os02k10_write_reg(dev, 0x5780, 0x53);
    ret |= os02k10_write_reg(dev, 0x5786, 0x01);
    ret |= os02k10_write_reg(dev, 0x0305, 0x48);
    ret |= os02k10_write_reg(dev, 0x4837, 0x10);
    ret |= os02k10_write_reg(dev, 0x380e, 0x05);
    ret |= os02k10_write_reg(dev, 0x380f, 0xbe);
    ret |= os02k10_write_reg(dev, 0x3501, 0x05);
    ret |= os02k10_write_reg(dev, 0x3502, 0xb0);
    ret |= os02k10_write_reg(dev, 0x3541, 0x00);
    ret |= os02k10_write_reg(dev, 0x3542, 0x02);
    ret |= os02k10_write_reg(dev, 0x5780, 0x7b);
    ret |= os02k10_write_reg(dev, 0x5782, 0xc0);
    ret |= os02k10_write_reg(dev, 0x5783, 0xe0);
    ret |= os02k10_write_reg(dev, 0x5788, 0x60);
    ret |= os02k10_write_reg(dev, 0x5789, 0xe0);
    ret |= os02k10_write_reg(dev, 0x5784, 0x01);
    ret |= os02k10_write_reg(dev, 0x5785, 0x00);
    ret |= os02k10_write_reg(dev, 0x5792, 0x11);
    ret |= os02k10_write_reg(dev, 0x5793, 0x33);
    ret |= os02k10_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os02k10_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("========== OS02K10_27MInput_MIPI_4lane_12bit_builtin_1920x1080_30fps init success!=====\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_SUCCESS;
}

xmedia_void os02k10_master_init(xmedia_u32 dev)
{
    return;
}

xmedia_void os02k10_slave_init(xmedia_u32 dev)
{
    return;
}

xmedia_s32 os02k10_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case OS02K10_SENSOR_2M_LINEAR_MODE:
            ret = os02k10_2lane_linear_1920x1080_init(dev);
            break;

        case OS02K10_SENSOR_2M_BUILT_IN_MODE:
            ret = os02k10_4lane_builtin_1920x1080_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (work_mode == XMEDIA_SENSOR_WORK_MODE_MASTER) {
        os02k10_master_init(dev);
    } else if (work_mode == XMEDIA_SENSOR_WORK_MODE_SLAVE) {
        os02k10_slave_init(dev);
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os02k10_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_os02k10_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_os02k10_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_os02k10_i2c_fd[dev]);
        g_os02k10_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_os02k10_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os02k10_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_os02k10_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_os02k10_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_os02k10_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = os02k10_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os02k10_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_os02k10_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_os02k10_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os02k10_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[OS02K10_DATA_BYTE];

    if (g_os02k10_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_os02k10_i2c_fd[dev], g_os02k10_i2c_addr[dev], addr, OS02K10_ADDR_BYTE, buf, OS02K10_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //OS02K10_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os02k10_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_os02k10_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_os02k10_i2c_fd[dev], g_os02k10_i2c_addr[dev], buf, OS02K10_ADDR_BYTE + OS02K10_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 os02k10_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    os02k10_read_reg(dev, OS02K10_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

    if (mirror_en) {
        mirror_flip_type |= 0x02;
    } else {
        mirror_flip_type &= 0xfd;
    }

    if (flip_en) {
        mirror_flip_type |= 0x0c;
    } else {
        mirror_flip_type &= 0xf3;
    }

    ret = os02k10_write_reg(dev, OS02K10_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os02k10_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}
