#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "sc8238.h"
#include "sc8238_ctrl.h"


static xmedia_s32 g_sc8238_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                    { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

static xmedia_s32 g_sc8238_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                    { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
#define SC8238_REG_ADDR_MIRROR_FLIP 0x3221

#ifdef FPGA
#define MAX_FPS_LINEAR 3.386
static xmedia_s32 sc8238_linear_comm_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc8238_write_reg(dev, 0x36e9, 0x80);
    ret |= sc8238_write_reg(dev, 0x36f9, 0x80);
    ret |= sc8238_write_reg(dev, 0x3018, 0x72);
    ret |= sc8238_write_reg(dev, 0x3019, 0x00);
    ret |= sc8238_write_reg(dev, 0x301f, 0xf3);
    ret |= sc8238_write_reg(dev, 0x3031, 0x0a);
    ret |= sc8238_write_reg(dev, 0x3037, 0x20);
    ret |= sc8238_write_reg(dev, 0x3038, 0x44);
    ret |= sc8238_write_reg(dev, 0x3200, 0x00);
    ret |= sc8238_write_reg(dev, 0x3201, 0x00);
    ret |= sc8238_write_reg(dev, 0x3202, 0x00);
    ret |= sc8238_write_reg(dev, 0x3203, 0x00);
    ret |= sc8238_write_reg(dev, 0x3204, 0x0f);
    ret |= sc8238_write_reg(dev, 0x3205, 0x1f);
    ret |= sc8238_write_reg(dev, 0x3206, 0x08);
    ret |= sc8238_write_reg(dev, 0x3207, 0x8f);
    ret |= sc8238_write_reg(dev, 0x320c, 0x08); // HTS
    ret |= sc8238_write_reg(dev, 0x320d, 0x34);
    ret |= sc8238_write_reg(dev, 0x320e, 0x08); // VTS
    ret |= sc8238_write_reg(dev, 0x320f, 0xca);
    ret |= sc8238_write_reg(dev, 0x3210, 0x00);
    ret |= sc8238_write_reg(dev, 0x3211, 0x10);
    ret |= sc8238_write_reg(dev, 0x3212, 0x00);
    ret |= sc8238_write_reg(dev, 0x3213, 0x10);
    ret |= sc8238_write_reg(dev, 0x3241, 0x00);
    ret |= sc8238_write_reg(dev, 0x3243, 0x03);
    ret |= sc8238_write_reg(dev, 0x3248, 0x04);
    ret |= sc8238_write_reg(dev, 0x3271, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3273, 0x1f);
    ret |= sc8238_write_reg(dev, 0x3301, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3306, 0xa8);
    ret |= sc8238_write_reg(dev, 0x3308, 0x20);
    ret |= sc8238_write_reg(dev, 0x3309, 0x68);
    ret |= sc8238_write_reg(dev, 0x330b, 0x48);
    ret |= sc8238_write_reg(dev, 0x330d, 0x28);
    ret |= sc8238_write_reg(dev, 0x330e, 0x58);
    ret |= sc8238_write_reg(dev, 0x3314, 0x94);
    ret |= sc8238_write_reg(dev, 0x331f, 0x59);
    ret |= sc8238_write_reg(dev, 0x3332, 0x24);
    ret |= sc8238_write_reg(dev, 0x334c, 0x10);
    ret |= sc8238_write_reg(dev, 0x3350, 0x24);
    ret |= sc8238_write_reg(dev, 0x3358, 0x24);
    ret |= sc8238_write_reg(dev, 0x335c, 0x24);
    ret |= sc8238_write_reg(dev, 0x335d, 0x60);
    ret |= sc8238_write_reg(dev, 0x3364, 0x16);
    ret |= sc8238_write_reg(dev, 0x3366, 0x92);
    ret |= sc8238_write_reg(dev, 0x3367, 0x08);
    ret |= sc8238_write_reg(dev, 0x3368, 0x07);
    ret |= sc8238_write_reg(dev, 0x3369, 0x00);
    ret |= sc8238_write_reg(dev, 0x336a, 0x00);
    ret |= sc8238_write_reg(dev, 0x336b, 0x00);
    ret |= sc8238_write_reg(dev, 0x336c, 0xc2);
    ret |= sc8238_write_reg(dev, 0x337f, 0x33);
    ret |= sc8238_write_reg(dev, 0x3390, 0x08);
    ret |= sc8238_write_reg(dev, 0x3391, 0x18);
    ret |= sc8238_write_reg(dev, 0x3392, 0x38);
    ret |= sc8238_write_reg(dev, 0x3393, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3394, 0x28);
    ret |= sc8238_write_reg(dev, 0x3395, 0x60);
    ret |= sc8238_write_reg(dev, 0x3396, 0x08);
    ret |= sc8238_write_reg(dev, 0x3397, 0x18);
    ret |= sc8238_write_reg(dev, 0x3398, 0x38);
    ret |= sc8238_write_reg(dev, 0x3399, 0x1c);
    ret |= sc8238_write_reg(dev, 0x339a, 0x1c);
    ret |= sc8238_write_reg(dev, 0x339b, 0x28);
    ret |= sc8238_write_reg(dev, 0x339c, 0x60);
    ret |= sc8238_write_reg(dev, 0x339e, 0x24);
    ret |= sc8238_write_reg(dev, 0x33aa, 0x24);
    ret |= sc8238_write_reg(dev, 0x33af, 0x48);
    ret |= sc8238_write_reg(dev, 0x33e1, 0x08);
    ret |= sc8238_write_reg(dev, 0x33e2, 0x18);
    ret |= sc8238_write_reg(dev, 0x33e3, 0x10);
    ret |= sc8238_write_reg(dev, 0x33e4, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33e5, 0x10);
    ret |= sc8238_write_reg(dev, 0x33e6, 0x06);
    ret |= sc8238_write_reg(dev, 0x33e7, 0x02);
    ret |= sc8238_write_reg(dev, 0x33e8, 0x18);
    ret |= sc8238_write_reg(dev, 0x33e9, 0x10);
    ret |= sc8238_write_reg(dev, 0x33ea, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33eb, 0x10);
    ret |= sc8238_write_reg(dev, 0x33ec, 0x04);
    ret |= sc8238_write_reg(dev, 0x33ed, 0x02);
    ret |= sc8238_write_reg(dev, 0x33ee, 0xa0);
    ret |= sc8238_write_reg(dev, 0x33ef, 0x08);
    ret |= sc8238_write_reg(dev, 0x33f4, 0x18);
    ret |= sc8238_write_reg(dev, 0x33f5, 0x10);
    ret |= sc8238_write_reg(dev, 0x33f6, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33f7, 0x10);
    ret |= sc8238_write_reg(dev, 0x33f8, 0x06);
    ret |= sc8238_write_reg(dev, 0x33f9, 0x02);
    ret |= sc8238_write_reg(dev, 0x33fa, 0x18);
    ret |= sc8238_write_reg(dev, 0x33fb, 0x10);
    ret |= sc8238_write_reg(dev, 0x33fc, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33fd, 0x10);
    ret |= sc8238_write_reg(dev, 0x33fe, 0x04);
    ret |= sc8238_write_reg(dev, 0x33ff, 0x02);
    ret |= sc8238_write_reg(dev, 0x360f, 0x01);
    ret |= sc8238_write_reg(dev, 0x3622, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3624, 0x45);
    ret |= sc8238_write_reg(dev, 0x3628, 0x83);
    ret |= sc8238_write_reg(dev, 0x3630, 0x80);
    ret |= sc8238_write_reg(dev, 0x3631, 0x80);
    ret |= sc8238_write_reg(dev, 0x3632, 0xa8);
    ret |= sc8238_write_reg(dev, 0x3633, 0x53);
    ret |= sc8238_write_reg(dev, 0x3635, 0x02);
    ret |= sc8238_write_reg(dev, 0x3637, 0x52);
    ret |= sc8238_write_reg(dev, 0x3638, 0x0a);
    ret |= sc8238_write_reg(dev, 0x363a, 0x88);
    ret |= sc8238_write_reg(dev, 0x363b, 0x06);
    ret |= sc8238_write_reg(dev, 0x363d, 0x01);
    ret |= sc8238_write_reg(dev, 0x363e, 0x00);
    ret |= sc8238_write_reg(dev, 0x3641, 0x00);
    ret |= sc8238_write_reg(dev, 0x3670, 0x4a);
    ret |= sc8238_write_reg(dev, 0x3671, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3672, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3673, 0x17);
    ret |= sc8238_write_reg(dev, 0x3674, 0x80);
    ret |= sc8238_write_reg(dev, 0x3675, 0x85);
    ret |= sc8238_write_reg(dev, 0x3676, 0xa5);
    ret |= sc8238_write_reg(dev, 0x367a, 0x48);
    ret |= sc8238_write_reg(dev, 0x367b, 0x78);
    ret |= sc8238_write_reg(dev, 0x367c, 0x48);
    ret |= sc8238_write_reg(dev, 0x367d, 0x78);
    ret |= sc8238_write_reg(dev, 0x3690, 0x53);
    ret |= sc8238_write_reg(dev, 0x3691, 0x63);
    ret |= sc8238_write_reg(dev, 0x3692, 0x54);
    ret |= sc8238_write_reg(dev, 0x3699, 0x88);
    ret |= sc8238_write_reg(dev, 0x369a, 0x9f);
    ret |= sc8238_write_reg(dev, 0x369b, 0x9f);
    ret |= sc8238_write_reg(dev, 0x369c, 0x48);
    ret |= sc8238_write_reg(dev, 0x369d, 0x78);
    ret |= sc8238_write_reg(dev, 0x36a2, 0x48);
    ret |= sc8238_write_reg(dev, 0x36a3, 0x78);
    ret |= sc8238_write_reg(dev, 0x36bb, 0x48);
    ret |= sc8238_write_reg(dev, 0x36bc, 0x78);
    ret |= sc8238_write_reg(dev, 0x36c9, 0x05);
    ret |= sc8238_write_reg(dev, 0x36ca, 0x05);
    ret |= sc8238_write_reg(dev, 0x36cb, 0x05);
    ret |= sc8238_write_reg(dev, 0x36cc, 0x00);
    ret |= sc8238_write_reg(dev, 0x36cd, 0x10);
    ret |= sc8238_write_reg(dev, 0x36ce, 0x1a);
    ret |= sc8238_write_reg(dev, 0x36d0, 0x30);
    ret |= sc8238_write_reg(dev, 0x36d1, 0x48);
    ret |= sc8238_write_reg(dev, 0x36d2, 0x78);
    ret |= sc8238_write_reg(dev, 0x36ea, 0x36);
    ret |= sc8238_write_reg(dev, 0x36eb, 0x34);
    ret |= sc8238_write_reg(dev, 0x36ec, 0x15);
    ret |= sc8238_write_reg(dev, 0x36ed, 0x14);
    ret |= sc8238_write_reg(dev, 0x36fa, 0x36);
    ret |= sc8238_write_reg(dev, 0x36fb, 0x19);
    ret |= sc8238_write_reg(dev, 0x36fc, 0x11);
    ret |= sc8238_write_reg(dev, 0x36fd, 0x14);
    ret |= sc8238_write_reg(dev, 0x3901, 0x00);
    ret |= sc8238_write_reg(dev, 0x3902, 0xc5);
    ret |= sc8238_write_reg(dev, 0x3904, 0x18);
    ret |= sc8238_write_reg(dev, 0x3905, 0xd8);
    ret |= sc8238_write_reg(dev, 0x394c, 0x0f);
    ret |= sc8238_write_reg(dev, 0x394d, 0x20);
    ret |= sc8238_write_reg(dev, 0x394e, 0x08);
    ret |= sc8238_write_reg(dev, 0x394f, 0x90);
    ret |= sc8238_write_reg(dev, 0x3980, 0x71);
    ret |= sc8238_write_reg(dev, 0x3981, 0x70);
    ret |= sc8238_write_reg(dev, 0x3982, 0x00);
    ret |= sc8238_write_reg(dev, 0x3983, 0x00);
    ret |= sc8238_write_reg(dev, 0x3984, 0x20);
    ret |= sc8238_write_reg(dev, 0x3987, 0x0b);
    ret |= sc8238_write_reg(dev, 0x3990, 0x03);
    ret |= sc8238_write_reg(dev, 0x3991, 0xfd);
    ret |= sc8238_write_reg(dev, 0x3992, 0x03);
    ret |= sc8238_write_reg(dev, 0x3993, 0xfc);
    ret |= sc8238_write_reg(dev, 0x3994, 0x00);
    ret |= sc8238_write_reg(dev, 0x3995, 0x00);
    ret |= sc8238_write_reg(dev, 0x3996, 0x00);
    ret |= sc8238_write_reg(dev, 0x3997, 0x05);
    ret |= sc8238_write_reg(dev, 0x3998, 0x00);
    ret |= sc8238_write_reg(dev, 0x3999, 0x09);
    ret |= sc8238_write_reg(dev, 0x399a, 0x00);
    ret |= sc8238_write_reg(dev, 0x399b, 0x12);
    ret |= sc8238_write_reg(dev, 0x399c, 0x00);
    ret |= sc8238_write_reg(dev, 0x399d, 0x12);
    ret |= sc8238_write_reg(dev, 0x399e, 0x00);
    ret |= sc8238_write_reg(dev, 0x399f, 0x18);
    ret |= sc8238_write_reg(dev, 0x39a0, 0x00);
    ret |= sc8238_write_reg(dev, 0x39a1, 0x14);
    ret |= sc8238_write_reg(dev, 0x39a2, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a3, 0xe3);
    ret |= sc8238_write_reg(dev, 0x39a4, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a5, 0xf2);
    ret |= sc8238_write_reg(dev, 0x39a6, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a7, 0xf6);
    ret |= sc8238_write_reg(dev, 0x39a8, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a9, 0xfa);
    ret |= sc8238_write_reg(dev, 0x39aa, 0x03);
    ret |= sc8238_write_reg(dev, 0x39ab, 0xff);
    ret |= sc8238_write_reg(dev, 0x39ac, 0x00);
    ret |= sc8238_write_reg(dev, 0x39ad, 0x06);
    ret |= sc8238_write_reg(dev, 0x39ae, 0x00);
    ret |= sc8238_write_reg(dev, 0x39af, 0x09);
    ret |= sc8238_write_reg(dev, 0x39b0, 0x00);
    ret |= sc8238_write_reg(dev, 0x39b1, 0x12);
    ret |= sc8238_write_reg(dev, 0x39b2, 0x00);
    ret |= sc8238_write_reg(dev, 0x39b3, 0x22);
    ret |= sc8238_write_reg(dev, 0x39b4, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39b5, 0x1c);
    ret |= sc8238_write_reg(dev, 0x39b6, 0x38);
    ret |= sc8238_write_reg(dev, 0x39b7, 0x5b);
    ret |= sc8238_write_reg(dev, 0x39b8, 0x50);
    ret |= sc8238_write_reg(dev, 0x39b9, 0x38);
    ret |= sc8238_write_reg(dev, 0x39ba, 0x20);
    ret |= sc8238_write_reg(dev, 0x39bb, 0x10);
    ret |= sc8238_write_reg(dev, 0x39bc, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39bd, 0x16);
    ret |= sc8238_write_reg(dev, 0x39be, 0x21);
    ret |= sc8238_write_reg(dev, 0x39bf, 0x36);
    ret |= sc8238_write_reg(dev, 0x39c0, 0x3b);
    ret |= sc8238_write_reg(dev, 0x39c1, 0x2a);
    ret |= sc8238_write_reg(dev, 0x39c2, 0x16);
    ret |= sc8238_write_reg(dev, 0x39c3, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39c5, 0x30);
    ret |= sc8238_write_reg(dev, 0x39c6, 0x07);
    ret |= sc8238_write_reg(dev, 0x39c7, 0xf8);
    ret |= sc8238_write_reg(dev, 0x39c9, 0x07);
    ret |= sc8238_write_reg(dev, 0x39ca, 0xf8);
    ret |= sc8238_write_reg(dev, 0x39cc, 0x00);
    ret |= sc8238_write_reg(dev, 0x39cd, 0x1b);
    ret |= sc8238_write_reg(dev, 0x39ce, 0x00);
    ret |= sc8238_write_reg(dev, 0x39cf, 0x00);
    ret |= sc8238_write_reg(dev, 0x39d0, 0x1b);
    ret |= sc8238_write_reg(dev, 0x39d1, 0x00);
    ret |= sc8238_write_reg(dev, 0x39e2, 0x15);
    ret |= sc8238_write_reg(dev, 0x39e3, 0x87);
    ret |= sc8238_write_reg(dev, 0x39e4, 0x12);
    ret |= sc8238_write_reg(dev, 0x39e5, 0xb7);
    ret |= sc8238_write_reg(dev, 0x39e6, 0x00);
    ret |= sc8238_write_reg(dev, 0x39e7, 0x8c);
    ret |= sc8238_write_reg(dev, 0x39e8, 0x01);
    ret |= sc8238_write_reg(dev, 0x39e9, 0x31);
    ret |= sc8238_write_reg(dev, 0x39ea, 0x01);
    ret |= sc8238_write_reg(dev, 0x39eb, 0xd7);
    ret |= sc8238_write_reg(dev, 0x39ec, 0x08);
    ret |= sc8238_write_reg(dev, 0x39ed, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e00, 0x01);
    ret |= sc8238_write_reg(dev, 0x3e01, 0x18);
    ret |= sc8238_write_reg(dev, 0x3e02, 0xa0);
    ret |= sc8238_write_reg(dev, 0x3e08, 0x03);
    ret |= sc8238_write_reg(dev, 0x3e09, 0x40);
    ret |= sc8238_write_reg(dev, 0x3e0e, 0x09);
    ret |= sc8238_write_reg(dev, 0x3e14, 0x31);
    ret |= sc8238_write_reg(dev, 0x3e16, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e17, 0xac);
    ret |= sc8238_write_reg(dev, 0x3e18, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e19, 0xac);
    ret |= sc8238_write_reg(dev, 0x3e1b, 0x3a);
    ret |= sc8238_write_reg(dev, 0x3e1e, 0x76);
    ret |= sc8238_write_reg(dev, 0x3e25, 0x23);
    ret |= sc8238_write_reg(dev, 0x3e26, 0x40);
    ret |= sc8238_write_reg(dev, 0x4501, 0xa4);
    ret |= sc8238_write_reg(dev, 0x4509, 0x10);
    ret |= sc8238_write_reg(dev, 0x4837, 0xe5);
    ret |= sc8238_write_reg(dev, 0x5799, 0x06);
    ret |= sc8238_write_reg(dev, 0x57aa, 0x2f);
    ret |= sc8238_write_reg(dev, 0x57ab, 0xff);
    ret |= sc8238_write_reg(dev, 0x36e9, 0x44);
    ret |= sc8238_write_reg(dev, 0x36f9, 0x40);
    ret |= sc8238_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

/*
 * 函数功能: sc8238 MIPI 4lanes 1520p线性初始化序列 - 2688x1520
 * 函数参数：
 *      dev -  senor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc8238_4lane_linear_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc8238_write_reg(dev, 0x0103, 0x01);
    ret |= sc8238_write_reg(dev, 0x0100, 0x00);
    ret |= sc8238_write_reg(dev, 0x36e9, 0x80);
    ret |= sc8238_write_reg(dev, 0x36f9, 0x80);
    ret |= sc8238_write_reg(dev, 0x3018, 0x72);
    ret |= sc8238_write_reg(dev, 0x3019, 0x00);
    ret |= sc8238_write_reg(dev, 0x301f, 0xf3);
    ret |= sc8238_write_reg(dev, 0x3031, 0x0a);
    ret |= sc8238_write_reg(dev, 0x3037, 0x20);
    ret |= sc8238_write_reg(dev, 0x3038, 0x44);

    ret |= sc8238_write_reg(dev, 0x3200, 0x02);
    ret |= sc8238_write_reg(dev, 0x3201, 0x40);
    ret |= sc8238_write_reg(dev, 0x3202, 0x01);
    ret |= sc8238_write_reg(dev, 0x3203, 0x40);
    ret |= sc8238_write_reg(dev, 0x3204, 0x0c);
    ret |= sc8238_write_reg(dev, 0x3205, 0xdf);
    ret |= sc8238_write_reg(dev, 0x3206, 0x07);
    ret |= sc8238_write_reg(dev, 0x3207, 0x4f);
    ret |= sc8238_write_reg(dev, 0x3208, 0x0a); // widht
    ret |= sc8238_write_reg(dev, 0x3209, 0x80);
    ret |= sc8238_write_reg(dev, 0x320a, 0x05); // height
    ret |= sc8238_write_reg(dev, 0x320b, 0xf0);
    ret |= sc8238_write_reg(dev, 0x320c, 0x08); // HTS
    ret |= sc8238_write_reg(dev, 0x320d, 0x34);
    ret |= sc8238_write_reg(dev, 0x320e, 0x08); // VTS
    ret |= sc8238_write_reg(dev, 0x320f, 0xca);
    ret |= sc8238_write_reg(dev, 0x3210, 0x00);
    ret |= sc8238_write_reg(dev, 0x3211, 0x10);
    ret |= sc8238_write_reg(dev, 0x3212, 0x00);
    ret |= sc8238_write_reg(dev, 0x3213, 0x10);

    ret |= sc8238_write_reg(dev, 0x3241, 0x00);
    ret |= sc8238_write_reg(dev, 0x3243, 0x03);
    ret |= sc8238_write_reg(dev, 0x3248, 0x04);
    ret |= sc8238_write_reg(dev, 0x3271, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3273, 0x1f);
    ret |= sc8238_write_reg(dev, 0x3301, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3306, 0xa8);
    ret |= sc8238_write_reg(dev, 0x3308, 0x20);
    ret |= sc8238_write_reg(dev, 0x3309, 0x68);
    ret |= sc8238_write_reg(dev, 0x330b, 0x48);
    ret |= sc8238_write_reg(dev, 0x330d, 0x28);
    ret |= sc8238_write_reg(dev, 0x330e, 0x58);
    ret |= sc8238_write_reg(dev, 0x3314, 0x94);
    ret |= sc8238_write_reg(dev, 0x331f, 0x59);
    ret |= sc8238_write_reg(dev, 0x3332, 0x24);
    ret |= sc8238_write_reg(dev, 0x334c, 0x10);
    ret |= sc8238_write_reg(dev, 0x3350, 0x24);
    ret |= sc8238_write_reg(dev, 0x3358, 0x24);
    ret |= sc8238_write_reg(dev, 0x335c, 0x24);
    ret |= sc8238_write_reg(dev, 0x335d, 0x60);
    ret |= sc8238_write_reg(dev, 0x3364, 0x16);
    ret |= sc8238_write_reg(dev, 0x3366, 0x92);
    ret |= sc8238_write_reg(dev, 0x3367, 0x08);
    ret |= sc8238_write_reg(dev, 0x3368, 0x07);
    ret |= sc8238_write_reg(dev, 0x3369, 0x00);
    ret |= sc8238_write_reg(dev, 0x336a, 0x00);
    ret |= sc8238_write_reg(dev, 0x336b, 0x00);
    ret |= sc8238_write_reg(dev, 0x336c, 0xc2);
    ret |= sc8238_write_reg(dev, 0x337f, 0x33);
    ret |= sc8238_write_reg(dev, 0x3390, 0x08);
    ret |= sc8238_write_reg(dev, 0x3391, 0x18);
    ret |= sc8238_write_reg(dev, 0x3392, 0x38);
    ret |= sc8238_write_reg(dev, 0x3393, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3394, 0x28);
    ret |= sc8238_write_reg(dev, 0x3395, 0x60);
    ret |= sc8238_write_reg(dev, 0x3396, 0x08);
    ret |= sc8238_write_reg(dev, 0x3397, 0x18);
    ret |= sc8238_write_reg(dev, 0x3398, 0x38);
    ret |= sc8238_write_reg(dev, 0x3399, 0x1c);
    ret |= sc8238_write_reg(dev, 0x339a, 0x1c);
    ret |= sc8238_write_reg(dev, 0x339b, 0x28);
    ret |= sc8238_write_reg(dev, 0x339c, 0x60);
    ret |= sc8238_write_reg(dev, 0x339e, 0x24);
    ret |= sc8238_write_reg(dev, 0x33aa, 0x24);
    ret |= sc8238_write_reg(dev, 0x33af, 0x48);
    ret |= sc8238_write_reg(dev, 0x33e1, 0x08);
    ret |= sc8238_write_reg(dev, 0x33e2, 0x18);
    ret |= sc8238_write_reg(dev, 0x33e3, 0x10);
    ret |= sc8238_write_reg(dev, 0x33e4, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33e5, 0x10);
    ret |= sc8238_write_reg(dev, 0x33e6, 0x06);
    ret |= sc8238_write_reg(dev, 0x33e7, 0x02);
    ret |= sc8238_write_reg(dev, 0x33e8, 0x18);
    ret |= sc8238_write_reg(dev, 0x33e9, 0x10);
    ret |= sc8238_write_reg(dev, 0x33ea, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33eb, 0x10);
    ret |= sc8238_write_reg(dev, 0x33ec, 0x04);
    ret |= sc8238_write_reg(dev, 0x33ed, 0x02);
    ret |= sc8238_write_reg(dev, 0x33ee, 0xa0);
    ret |= sc8238_write_reg(dev, 0x33ef, 0x08);
    ret |= sc8238_write_reg(dev, 0x33f4, 0x18);
    ret |= sc8238_write_reg(dev, 0x33f5, 0x10);
    ret |= sc8238_write_reg(dev, 0x33f6, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33f7, 0x10);
    ret |= sc8238_write_reg(dev, 0x33f8, 0x06);
    ret |= sc8238_write_reg(dev, 0x33f9, 0x02);
    ret |= sc8238_write_reg(dev, 0x33fa, 0x18);
    ret |= sc8238_write_reg(dev, 0x33fb, 0x10);
    ret |= sc8238_write_reg(dev, 0x33fc, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33fd, 0x10);
    ret |= sc8238_write_reg(dev, 0x33fe, 0x04);
    ret |= sc8238_write_reg(dev, 0x33ff, 0x02);
    ret |= sc8238_write_reg(dev, 0x360f, 0x01);
    ret |= sc8238_write_reg(dev, 0x3622, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3624, 0x45);
    ret |= sc8238_write_reg(dev, 0x3628, 0x83);
    ret |= sc8238_write_reg(dev, 0x3630, 0x80);
    ret |= sc8238_write_reg(dev, 0x3631, 0x80);
    ret |= sc8238_write_reg(dev, 0x3632, 0xa8);
    ret |= sc8238_write_reg(dev, 0x3633, 0x53);
    ret |= sc8238_write_reg(dev, 0x3635, 0x02);
    ret |= sc8238_write_reg(dev, 0x3637, 0x52);
    ret |= sc8238_write_reg(dev, 0x3638, 0x0a);
    ret |= sc8238_write_reg(dev, 0x363a, 0x88);
    ret |= sc8238_write_reg(dev, 0x363b, 0x06);
    ret |= sc8238_write_reg(dev, 0x363d, 0x01);
    ret |= sc8238_write_reg(dev, 0x363e, 0x00);
    ret |= sc8238_write_reg(dev, 0x3641, 0x00);
    ret |= sc8238_write_reg(dev, 0x3670, 0x4a);
    ret |= sc8238_write_reg(dev, 0x3671, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3672, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3673, 0x17);
    ret |= sc8238_write_reg(dev, 0x3674, 0x80);
    ret |= sc8238_write_reg(dev, 0x3675, 0x85);
    ret |= sc8238_write_reg(dev, 0x3676, 0xa5);
    ret |= sc8238_write_reg(dev, 0x367a, 0x48);
    ret |= sc8238_write_reg(dev, 0x367b, 0x78);
    ret |= sc8238_write_reg(dev, 0x367c, 0x48);
    ret |= sc8238_write_reg(dev, 0x367d, 0x78);
    ret |= sc8238_write_reg(dev, 0x3690, 0x53);
    ret |= sc8238_write_reg(dev, 0x3691, 0x63);
    ret |= sc8238_write_reg(dev, 0x3692, 0x54);
    ret |= sc8238_write_reg(dev, 0x3699, 0x88);
    ret |= sc8238_write_reg(dev, 0x369a, 0x9f);
    ret |= sc8238_write_reg(dev, 0x369b, 0x9f);
    ret |= sc8238_write_reg(dev, 0x369c, 0x48);
    ret |= sc8238_write_reg(dev, 0x369d, 0x78);
    ret |= sc8238_write_reg(dev, 0x36a2, 0x48);
    ret |= sc8238_write_reg(dev, 0x36a3, 0x78);
    ret |= sc8238_write_reg(dev, 0x36bb, 0x48);
    ret |= sc8238_write_reg(dev, 0x36bc, 0x78);
    ret |= sc8238_write_reg(dev, 0x36c9, 0x05);
    ret |= sc8238_write_reg(dev, 0x36ca, 0x05);
    ret |= sc8238_write_reg(dev, 0x36cb, 0x05);
    ret |= sc8238_write_reg(dev, 0x36cc, 0x00);
    ret |= sc8238_write_reg(dev, 0x36cd, 0x10);
    ret |= sc8238_write_reg(dev, 0x36ce, 0x1a);
    ret |= sc8238_write_reg(dev, 0x36d0, 0x30);
    ret |= sc8238_write_reg(dev, 0x36d1, 0x48);
    ret |= sc8238_write_reg(dev, 0x36d2, 0x78);
    ret |= sc8238_write_reg(dev, 0x36ea, 0x36);
    ret |= sc8238_write_reg(dev, 0x36eb, 0x34);
    ret |= sc8238_write_reg(dev, 0x36ec, 0x15);
    ret |= sc8238_write_reg(dev, 0x36ed, 0x14);
    ret |= sc8238_write_reg(dev, 0x36fa, 0x36);
    ret |= sc8238_write_reg(dev, 0x36fb, 0x19);
    ret |= sc8238_write_reg(dev, 0x36fc, 0x11);
    ret |= sc8238_write_reg(dev, 0x36fd, 0x14);
    ret |= sc8238_write_reg(dev, 0x3901, 0x00);
    ret |= sc8238_write_reg(dev, 0x3902, 0xc5);
    ret |= sc8238_write_reg(dev, 0x3904, 0x18);
    ret |= sc8238_write_reg(dev, 0x3905, 0xd8);
    ret |= sc8238_write_reg(dev, 0x394c, 0x0f);
    ret |= sc8238_write_reg(dev, 0x394d, 0x20);
    ret |= sc8238_write_reg(dev, 0x394e, 0x08);
    ret |= sc8238_write_reg(dev, 0x394f, 0x90);
    ret |= sc8238_write_reg(dev, 0x3980, 0x71);
    ret |= sc8238_write_reg(dev, 0x3981, 0x70);
    ret |= sc8238_write_reg(dev, 0x3982, 0x00);
    ret |= sc8238_write_reg(dev, 0x3983, 0x00);
    ret |= sc8238_write_reg(dev, 0x3984, 0x20);
    ret |= sc8238_write_reg(dev, 0x3987, 0x0b);
    ret |= sc8238_write_reg(dev, 0x3990, 0x03);
    ret |= sc8238_write_reg(dev, 0x3991, 0xfd);
    ret |= sc8238_write_reg(dev, 0x3992, 0x03);
    ret |= sc8238_write_reg(dev, 0x3993, 0xfc);
    ret |= sc8238_write_reg(dev, 0x3994, 0x00);
    ret |= sc8238_write_reg(dev, 0x3995, 0x00);
    ret |= sc8238_write_reg(dev, 0x3996, 0x00);
    ret |= sc8238_write_reg(dev, 0x3997, 0x05);
    ret |= sc8238_write_reg(dev, 0x3998, 0x00);
    ret |= sc8238_write_reg(dev, 0x3999, 0x09);
    ret |= sc8238_write_reg(dev, 0x399a, 0x00);
    ret |= sc8238_write_reg(dev, 0x399b, 0x12);
    ret |= sc8238_write_reg(dev, 0x399c, 0x00);
    ret |= sc8238_write_reg(dev, 0x399d, 0x12);
    ret |= sc8238_write_reg(dev, 0x399e, 0x00);
    ret |= sc8238_write_reg(dev, 0x399f, 0x18);
    ret |= sc8238_write_reg(dev, 0x39a0, 0x00);
    ret |= sc8238_write_reg(dev, 0x39a1, 0x14);
    ret |= sc8238_write_reg(dev, 0x39a2, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a3, 0xe3);
    ret |= sc8238_write_reg(dev, 0x39a4, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a5, 0xf2);
    ret |= sc8238_write_reg(dev, 0x39a6, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a7, 0xf6);
    ret |= sc8238_write_reg(dev, 0x39a8, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a9, 0xfa);
    ret |= sc8238_write_reg(dev, 0x39aa, 0x03);
    ret |= sc8238_write_reg(dev, 0x39ab, 0xff);
    ret |= sc8238_write_reg(dev, 0x39ac, 0x00);
    ret |= sc8238_write_reg(dev, 0x39ad, 0x06);
    ret |= sc8238_write_reg(dev, 0x39ae, 0x00);
    ret |= sc8238_write_reg(dev, 0x39af, 0x09);
    ret |= sc8238_write_reg(dev, 0x39b0, 0x00);
    ret |= sc8238_write_reg(dev, 0x39b1, 0x12);
    ret |= sc8238_write_reg(dev, 0x39b2, 0x00);
    ret |= sc8238_write_reg(dev, 0x39b3, 0x22);
    ret |= sc8238_write_reg(dev, 0x39b4, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39b5, 0x1c);
    ret |= sc8238_write_reg(dev, 0x39b6, 0x38);
    ret |= sc8238_write_reg(dev, 0x39b7, 0x5b);
    ret |= sc8238_write_reg(dev, 0x39b8, 0x50);
    ret |= sc8238_write_reg(dev, 0x39b9, 0x38);
    ret |= sc8238_write_reg(dev, 0x39ba, 0x20);
    ret |= sc8238_write_reg(dev, 0x39bb, 0x10);
    ret |= sc8238_write_reg(dev, 0x39bc, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39bd, 0x16);
    ret |= sc8238_write_reg(dev, 0x39be, 0x21);
    ret |= sc8238_write_reg(dev, 0x39bf, 0x36);
    ret |= sc8238_write_reg(dev, 0x39c0, 0x3b);
    ret |= sc8238_write_reg(dev, 0x39c1, 0x2a);
    ret |= sc8238_write_reg(dev, 0x39c2, 0x16);
    ret |= sc8238_write_reg(dev, 0x39c3, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39c5, 0x30);
    ret |= sc8238_write_reg(dev, 0x39c6, 0x07);
    ret |= sc8238_write_reg(dev, 0x39c7, 0xf8);
    ret |= sc8238_write_reg(dev, 0x39c9, 0x07);
    ret |= sc8238_write_reg(dev, 0x39ca, 0xf8);
    ret |= sc8238_write_reg(dev, 0x39cc, 0x00);
    ret |= sc8238_write_reg(dev, 0x39cd, 0x1b);
    ret |= sc8238_write_reg(dev, 0x39ce, 0x00);
    ret |= sc8238_write_reg(dev, 0x39cf, 0x00);
    ret |= sc8238_write_reg(dev, 0x39d0, 0x1b);
    ret |= sc8238_write_reg(dev, 0x39d1, 0x00);
    ret |= sc8238_write_reg(dev, 0x39e2, 0x15);
    ret |= sc8238_write_reg(dev, 0x39e3, 0x87);
    ret |= sc8238_write_reg(dev, 0x39e4, 0x12);
    ret |= sc8238_write_reg(dev, 0x39e5, 0xb7);
    ret |= sc8238_write_reg(dev, 0x39e6, 0x00);
    ret |= sc8238_write_reg(dev, 0x39e7, 0x8c);
    ret |= sc8238_write_reg(dev, 0x39e8, 0x01);
    ret |= sc8238_write_reg(dev, 0x39e9, 0x31);
    ret |= sc8238_write_reg(dev, 0x39ea, 0x01);
    ret |= sc8238_write_reg(dev, 0x39eb, 0xd7);
    ret |= sc8238_write_reg(dev, 0x39ec, 0x08);
    ret |= sc8238_write_reg(dev, 0x39ed, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e00, 0x01);
    ret |= sc8238_write_reg(dev, 0x3e01, 0x18);
    ret |= sc8238_write_reg(dev, 0x3e02, 0xa0);
    ret |= sc8238_write_reg(dev, 0x3e08, 0x03);
    ret |= sc8238_write_reg(dev, 0x3e09, 0x40);
    ret |= sc8238_write_reg(dev, 0x3e0e, 0x09);
    ret |= sc8238_write_reg(dev, 0x3e14, 0x31);
    ret |= sc8238_write_reg(dev, 0x3e16, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e17, 0xac);
    ret |= sc8238_write_reg(dev, 0x3e18, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e19, 0xac);
    ret |= sc8238_write_reg(dev, 0x3e1b, 0x3a);
    ret |= sc8238_write_reg(dev, 0x3e1e, 0x76);
    ret |= sc8238_write_reg(dev, 0x3e25, 0x23);
    ret |= sc8238_write_reg(dev, 0x3e26, 0x40);
    ret |= sc8238_write_reg(dev, 0x4501, 0xa4);
    ret |= sc8238_write_reg(dev, 0x4509, 0x10);
    ret |= sc8238_write_reg(dev, 0x4837, 0xe5);
    ret |= sc8238_write_reg(dev, 0x5799, 0x06);
    ret |= sc8238_write_reg(dev, 0x57aa, 0x2f);
    ret |= sc8238_write_reg(dev, 0x57ab, 0xff);
    ret |= sc8238_write_reg(dev, 0x36e9, 0x44);
    ret |= sc8238_write_reg(dev, 0x36f9, 0x40);
    ret |= sc8238_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=================================================================================\n");
    SENSOR_PRINT("=== SC8238_24MInput_MIPI_4lane_10bit_2688x1520_3.386fps init success!============\n");
    SENSOR_PRINT("=================================================================================\n");
    return XMEDIA_SUCCESS;
}

/*
 * 函数功能: sc8238 MIPI 4lanes 720p线性初始化序列 - 1280x720x20fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc8238_4lane_linear_1280x720_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc8238_write_reg(dev, 0x0103, 0x01);
    ret |= sc8238_write_reg(dev, 0x0100, 0x00);
    ret |= sc8238_write_reg(dev, 0x36e9, 0x80);
    ret |= sc8238_write_reg(dev, 0x36f9, 0x80);
    ret |= sc8238_write_reg(dev, 0x3018, 0x72);
    ret |= sc8238_write_reg(dev, 0x3019, 0x00);
    ret |= sc8238_write_reg(dev, 0x301f, 0xf0);
    ret |= sc8238_write_reg(dev, 0x3031, 0x0a);
    ret |= sc8238_write_reg(dev, 0x3037, 0x20);
    ret |= sc8238_write_reg(dev, 0x3038, 0x44);
    ret |= sc8238_write_reg(dev, 0x3200, 0x05);
    ret |= sc8238_write_reg(dev, 0x3201, 0x00);
    ret |= sc8238_write_reg(dev, 0x3202, 0x02);
    ret |= sc8238_write_reg(dev, 0x3203, 0xde);
    ret |= sc8238_write_reg(dev, 0x3204, 0x0a);
    ret |= sc8238_write_reg(dev, 0x3205, 0x1f);
    ret |= sc8238_write_reg(dev, 0x3206, 0x05);
    ret |= sc8238_write_reg(dev, 0x3207, 0xb1);
    ret |= sc8238_write_reg(dev, 0x3208, 0x05);
    ret |= sc8238_write_reg(dev, 0x3209, 0x00);
    ret |= sc8238_write_reg(dev, 0x320a, 0x02);
    ret |= sc8238_write_reg(dev, 0x320b, 0xd0);
    ret |= sc8238_write_reg(dev, 0x320c, 0x04); // HTS
    ret |= sc8238_write_reg(dev, 0x320d, 0x2a);
    ret |= sc8238_write_reg(dev, 0x320e, 0x02); // VTS
    ret |= sc8238_write_reg(dev, 0x320f, 0xee);
    ret |= sc8238_write_reg(dev, 0x3210, 0x00);
    ret |= sc8238_write_reg(dev, 0x3211, 0x10);
    ret |= sc8238_write_reg(dev, 0x3212, 0x00);
    ret |= sc8238_write_reg(dev, 0x3213, 0x02);
    ret |= sc8238_write_reg(dev, 0x3241, 0x00);
    ret |= sc8238_write_reg(dev, 0x3243, 0x03);
    ret |= sc8238_write_reg(dev, 0x3248, 0x04);
    ret |= sc8238_write_reg(dev, 0x3271, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3273, 0x1f);
    ret |= sc8238_write_reg(dev, 0x3301, 0x17);
    ret |= sc8238_write_reg(dev, 0x3302, 0x10);
    ret |= sc8238_write_reg(dev, 0x3304, 0x20);
    ret |= sc8238_write_reg(dev, 0x3306, 0x48);
    ret |= sc8238_write_reg(dev, 0x3308, 0x14);
    ret |= sc8238_write_reg(dev, 0x3309, 0x50);
    ret |= sc8238_write_reg(dev, 0x330a, 0x00);
    ret |= sc8238_write_reg(dev, 0x330b, 0xb0);
    ret |= sc8238_write_reg(dev, 0x330c, 0x0a);
    ret |= sc8238_write_reg(dev, 0x330d, 0x20);
    ret |= sc8238_write_reg(dev, 0x330e, 0x58);
    ret |= sc8238_write_reg(dev, 0x3314, 0x90);
    ret |= sc8238_write_reg(dev, 0x331e, 0x11);
    ret |= sc8238_write_reg(dev, 0x331f, 0x41);
    ret |= sc8238_write_reg(dev, 0x3332, 0x1e);
    ret |= sc8238_write_reg(dev, 0x334c, 0x10);
    ret |= sc8238_write_reg(dev, 0x3350, 0x1e);
    ret |= sc8238_write_reg(dev, 0x3358, 0x1e);
    ret |= sc8238_write_reg(dev, 0x335c, 0x1e);
    ret |= sc8238_write_reg(dev, 0x335d, 0x60);
    ret |= sc8238_write_reg(dev, 0x3366, 0x4a);
    ret |= sc8238_write_reg(dev, 0x3367, 0x08);
    ret |= sc8238_write_reg(dev, 0x3368, 0x07);
    ret |= sc8238_write_reg(dev, 0x3369, 0x00);
    ret |= sc8238_write_reg(dev, 0x336a, 0x00);
    ret |= sc8238_write_reg(dev, 0x336b, 0x00);
    ret |= sc8238_write_reg(dev, 0x336c, 0xc2);
    ret |= sc8238_write_reg(dev, 0x337f, 0x33);
    ret |= sc8238_write_reg(dev, 0x339e, 0x1e);
    ret |= sc8238_write_reg(dev, 0x33a0, 0x1e);
    ret |= sc8238_write_reg(dev, 0x33a4, 0x1e);
    ret |= sc8238_write_reg(dev, 0x33aa, 0x1e);
    ret |= sc8238_write_reg(dev, 0x33af, 0x48);
    ret |= sc8238_write_reg(dev, 0x33b9, 0x0e);
    ret |= sc8238_write_reg(dev, 0x33e1, 0x08);
    ret |= sc8238_write_reg(dev, 0x33e2, 0x18);
    ret |= sc8238_write_reg(dev, 0x33e3, 0x10);
    ret |= sc8238_write_reg(dev, 0x33e4, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33e5, 0x10);
    ret |= sc8238_write_reg(dev, 0x33e6, 0x06);
    ret |= sc8238_write_reg(dev, 0x33e7, 0x02);
    ret |= sc8238_write_reg(dev, 0x33e8, 0x18);
    ret |= sc8238_write_reg(dev, 0x33e9, 0x10);
    ret |= sc8238_write_reg(dev, 0x33ea, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33eb, 0x10);
    ret |= sc8238_write_reg(dev, 0x33ec, 0x04);
    ret |= sc8238_write_reg(dev, 0x33ed, 0x02);
    ret |= sc8238_write_reg(dev, 0x33ee, 0xa0);
    ret |= sc8238_write_reg(dev, 0x33ef, 0x08);
    ret |= sc8238_write_reg(dev, 0x33f4, 0x18);
    ret |= sc8238_write_reg(dev, 0x33f5, 0x10);
    ret |= sc8238_write_reg(dev, 0x33f6, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33f7, 0x10);
    ret |= sc8238_write_reg(dev, 0x33f8, 0x06);
    ret |= sc8238_write_reg(dev, 0x33f9, 0x02);
    ret |= sc8238_write_reg(dev, 0x33fa, 0x18);
    ret |= sc8238_write_reg(dev, 0x33fb, 0x10);
    ret |= sc8238_write_reg(dev, 0x33fc, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33fd, 0x10);
    ret |= sc8238_write_reg(dev, 0x33fe, 0x04);
    ret |= sc8238_write_reg(dev, 0x33ff, 0x02);
    ret |= sc8238_write_reg(dev, 0x360f, 0x01);
    ret |= sc8238_write_reg(dev, 0x3622, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3624, 0x45);
    ret |= sc8238_write_reg(dev, 0x3625, 0x0a);
    ret |= sc8238_write_reg(dev, 0x3628, 0x83);
    ret |= sc8238_write_reg(dev, 0x3630, 0x80);
    ret |= sc8238_write_reg(dev, 0x3631, 0x80);
    ret |= sc8238_write_reg(dev, 0x3632, 0xa8);
    ret |= sc8238_write_reg(dev, 0x3633, 0x53);
    ret |= sc8238_write_reg(dev, 0x3635, 0x02);
    ret |= sc8238_write_reg(dev, 0x3637, 0x52);
    ret |= sc8238_write_reg(dev, 0x3638, 0x0a);
    ret |= sc8238_write_reg(dev, 0x363a, 0x88);
    ret |= sc8238_write_reg(dev, 0x363b, 0x06);
    ret |= sc8238_write_reg(dev, 0x363d, 0x01);
    ret |= sc8238_write_reg(dev, 0x363e, 0x00);
    ret |= sc8238_write_reg(dev, 0x3641, 0x00);
    ret |= sc8238_write_reg(dev, 0x3670, 0x4a);
    ret |= sc8238_write_reg(dev, 0x3671, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3672, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3673, 0x17);
    ret |= sc8238_write_reg(dev, 0x3674, 0x80);
    ret |= sc8238_write_reg(dev, 0x3675, 0xa5);
    ret |= sc8238_write_reg(dev, 0x3676, 0xd5);
    ret |= sc8238_write_reg(dev, 0x367a, 0x48);
    ret |= sc8238_write_reg(dev, 0x367b, 0x78);
    ret |= sc8238_write_reg(dev, 0x367c, 0x48);
    ret |= sc8238_write_reg(dev, 0x367d, 0x78);
    ret |= sc8238_write_reg(dev, 0x3690, 0x53);
    ret |= sc8238_write_reg(dev, 0x3691, 0x63);
    ret |= sc8238_write_reg(dev, 0x3692, 0x54);
    ret |= sc8238_write_reg(dev, 0x3699, 0x88);
    ret |= sc8238_write_reg(dev, 0x369a, 0x9f);
    ret |= sc8238_write_reg(dev, 0x369b, 0x9f);
    ret |= sc8238_write_reg(dev, 0x369c, 0x48);
    ret |= sc8238_write_reg(dev, 0x369d, 0x78);
    ret |= sc8238_write_reg(dev, 0x36a2, 0x48);
    ret |= sc8238_write_reg(dev, 0x36a3, 0x78);
    ret |= sc8238_write_reg(dev, 0x36bb, 0x48);
    ret |= sc8238_write_reg(dev, 0x36bc, 0x78);
    ret |= sc8238_write_reg(dev, 0x36c9, 0x01);
    ret |= sc8238_write_reg(dev, 0x36ca, 0x20);
    ret |= sc8238_write_reg(dev, 0x36cb, 0x20);
    ret |= sc8238_write_reg(dev, 0x36cc, 0x00);
    ret |= sc8238_write_reg(dev, 0x36cd, 0x10);
    ret |= sc8238_write_reg(dev, 0x36ce, 0x1a);
    ret |= sc8238_write_reg(dev, 0x36d0, 0x30);
    ret |= sc8238_write_reg(dev, 0x36d1, 0x48);
    ret |= sc8238_write_reg(dev, 0x36d2, 0x78);
    ret |= sc8238_write_reg(dev, 0x36ea, 0x36);
    ret |= sc8238_write_reg(dev, 0x36eb, 0x34);
    ret |= sc8238_write_reg(dev, 0x36ec, 0x15);
    ret |= sc8238_write_reg(dev, 0x36ed, 0x14);
    ret |= sc8238_write_reg(dev, 0x36fa, 0x36);
    ret |= sc8238_write_reg(dev, 0x36fb, 0x19);
    ret |= sc8238_write_reg(dev, 0x36fc, 0x11);
    ret |= sc8238_write_reg(dev, 0x36fd, 0x14);
    ret |= sc8238_write_reg(dev, 0x3901, 0x00);
    ret |= sc8238_write_reg(dev, 0x3902, 0xc5);
    ret |= sc8238_write_reg(dev, 0x3904, 0x18);
    ret |= sc8238_write_reg(dev, 0x3905, 0xd8);
    ret |= sc8238_write_reg(dev, 0x394c, 0x0f);
    ret |= sc8238_write_reg(dev, 0x394d, 0x20);
    ret |= sc8238_write_reg(dev, 0x394e, 0x08);
    ret |= sc8238_write_reg(dev, 0x394f, 0x90);
    ret |= sc8238_write_reg(dev, 0x3980, 0x71);
    ret |= sc8238_write_reg(dev, 0x3981, 0x70);
    ret |= sc8238_write_reg(dev, 0x3982, 0x00);
    ret |= sc8238_write_reg(dev, 0x3983, 0x00);
    ret |= sc8238_write_reg(dev, 0x3984, 0x20);
    ret |= sc8238_write_reg(dev, 0x3987, 0x0b);
    ret |= sc8238_write_reg(dev, 0x3990, 0x03);
    ret |= sc8238_write_reg(dev, 0x3991, 0xfd);
    ret |= sc8238_write_reg(dev, 0x3992, 0x03);
    ret |= sc8238_write_reg(dev, 0x3993, 0xfc);
    ret |= sc8238_write_reg(dev, 0x3994, 0x00);
    ret |= sc8238_write_reg(dev, 0x3995, 0x00);
    ret |= sc8238_write_reg(dev, 0x3996, 0x00);
    ret |= sc8238_write_reg(dev, 0x3997, 0x05);
    ret |= sc8238_write_reg(dev, 0x3998, 0x00);
    ret |= sc8238_write_reg(dev, 0x3999, 0x09);
    ret |= sc8238_write_reg(dev, 0x399a, 0x00);
    ret |= sc8238_write_reg(dev, 0x399b, 0x12);
    ret |= sc8238_write_reg(dev, 0x399c, 0x00);
    ret |= sc8238_write_reg(dev, 0x399d, 0x12);
    ret |= sc8238_write_reg(dev, 0x399e, 0x00);
    ret |= sc8238_write_reg(dev, 0x399f, 0x18);
    ret |= sc8238_write_reg(dev, 0x39a0, 0x00);
    ret |= sc8238_write_reg(dev, 0x39a1, 0x14);
    ret |= sc8238_write_reg(dev, 0x39a2, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a3, 0xe3);
    ret |= sc8238_write_reg(dev, 0x39a4, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a5, 0xf2);
    ret |= sc8238_write_reg(dev, 0x39a6, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a7, 0xf6);
    ret |= sc8238_write_reg(dev, 0x39a8, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a9, 0xfa);
    ret |= sc8238_write_reg(dev, 0x39aa, 0x03);
    ret |= sc8238_write_reg(dev, 0x39ab, 0xff);
    ret |= sc8238_write_reg(dev, 0x39ac, 0x00);
    ret |= sc8238_write_reg(dev, 0x39ad, 0x06);
    ret |= sc8238_write_reg(dev, 0x39ae, 0x00);
    ret |= sc8238_write_reg(dev, 0x39af, 0x09);
    ret |= sc8238_write_reg(dev, 0x39b0, 0x00);
    ret |= sc8238_write_reg(dev, 0x39b1, 0x12);
    ret |= sc8238_write_reg(dev, 0x39b2, 0x00);
    ret |= sc8238_write_reg(dev, 0x39b3, 0x22);
    ret |= sc8238_write_reg(dev, 0x39b4, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39b5, 0x1c);
    ret |= sc8238_write_reg(dev, 0x39b6, 0x38);
    ret |= sc8238_write_reg(dev, 0x39b7, 0x5b);
    ret |= sc8238_write_reg(dev, 0x39b8, 0x50);
    ret |= sc8238_write_reg(dev, 0x39b9, 0x38);
    ret |= sc8238_write_reg(dev, 0x39ba, 0x20);
    ret |= sc8238_write_reg(dev, 0x39bb, 0x10);
    ret |= sc8238_write_reg(dev, 0x39bc, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39bd, 0x16);
    ret |= sc8238_write_reg(dev, 0x39be, 0x21);
    ret |= sc8238_write_reg(dev, 0x39bf, 0x36);
    ret |= sc8238_write_reg(dev, 0x39c0, 0x3b);
    ret |= sc8238_write_reg(dev, 0x39c1, 0x2a);
    ret |= sc8238_write_reg(dev, 0x39c2, 0x16);
    ret |= sc8238_write_reg(dev, 0x39c3, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39c5, 0x30);
    ret |= sc8238_write_reg(dev, 0x39c6, 0x07);
    ret |= sc8238_write_reg(dev, 0x39c7, 0xf8);
    ret |= sc8238_write_reg(dev, 0x39c9, 0x07);
    ret |= sc8238_write_reg(dev, 0x39ca, 0xf8);
    ret |= sc8238_write_reg(dev, 0x39cc, 0x00);
    ret |= sc8238_write_reg(dev, 0x39cd, 0x1b);
    ret |= sc8238_write_reg(dev, 0x39ce, 0x00);
    ret |= sc8238_write_reg(dev, 0x39cf, 0x00);
    ret |= sc8238_write_reg(dev, 0x39d0, 0x1b);
    ret |= sc8238_write_reg(dev, 0x39d1, 0x00);
    ret |= sc8238_write_reg(dev, 0x39e2, 0x15);
    ret |= sc8238_write_reg(dev, 0x39e3, 0x87);
    ret |= sc8238_write_reg(dev, 0x39e4, 0x12);
    ret |= sc8238_write_reg(dev, 0x39e5, 0xb7);
    ret |= sc8238_write_reg(dev, 0x39e6, 0x00);
    ret |= sc8238_write_reg(dev, 0x39e7, 0x8c);
    ret |= sc8238_write_reg(dev, 0x39e8, 0x01);
    ret |= sc8238_write_reg(dev, 0x39e9, 0x31);
    ret |= sc8238_write_reg(dev, 0x39ea, 0x01);
    ret |= sc8238_write_reg(dev, 0x39eb, 0xd7);
    ret |= sc8238_write_reg(dev, 0x39ec, 0x08);
    ret |= sc8238_write_reg(dev, 0x39ed, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e00, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e01, 0x5d);
    ret |= sc8238_write_reg(dev, 0x3e02, 0x20);
    // ret |= sc8238_write_reg(dev, 0x3e08, 0x03);
    // ret |= sc8238_write_reg(dev, 0x3e09, 0x40);
    ret |= sc8238_write_reg(dev, 0x3e08, 0x06);
    ret |= sc8238_write_reg(dev, 0x3e09, 0x80);
    ret |= sc8238_write_reg(dev, 0x3e0e, 0x09);
    ret |= sc8238_write_reg(dev, 0x3e14, 0x31);
    ret |= sc8238_write_reg(dev, 0x3e16, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e17, 0xac);
    ret |= sc8238_write_reg(dev, 0x3e18, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e19, 0xac);
    ret |= sc8238_write_reg(dev, 0x3e1b, 0x3a);
    ret |= sc8238_write_reg(dev, 0x3e1e, 0x76);
    ret |= sc8238_write_reg(dev, 0x3e25, 0x23);
    ret |= sc8238_write_reg(dev, 0x3e26, 0x40);
    ret |= sc8238_write_reg(dev, 0x4501, 0xa4);
    ret |= sc8238_write_reg(dev, 0x4509, 0x10);
    ret |= sc8238_write_reg(dev, 0x4837, 0xe5);
    ret |= sc8238_write_reg(dev, 0x5799, 0x06);
    ret |= sc8238_write_reg(dev, 0x57aa, 0x2f);
    ret |= sc8238_write_reg(dev, 0x57ab, 0xff);
    ret |= sc8238_write_reg(dev, 0x36e9, 0x44);
    ret |= sc8238_write_reg(dev, 0x36f9, 0x40);
    ret |= sc8238_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("====================================================================================\n");
    SENSOR_PRINT("===== SC8238_24MInput_MIPI_4lane_10bit_1280x720_20.013fps init success! ============\n");
    SENSOR_PRINT("====================================================================================\n");
    return XMEDIA_SUCCESS;
}

/*
 * 函数功能: sc8238 MIPI 4lanes 1728p线性初始化序列 - 3072x1728
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc8238_4lane_linear_3072x1728_init(xmedia_u32 dev)
{
    SENSOR_PRINT("========================================================================================\n");
    SENSOR_PRINT("======================SC8238_5M_10BIT_1728P_LINEAR_MODE NOT SUPPORT! ============== ====\n");
    SENSOR_PRINT("========================================================================================\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;

}
#else
#define MAX_FPS_LINEAR 30.0
static xmedia_s32 sc8238_linear_comm_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc8238_write_reg(dev, 0x36e9, 0x80);
    ret |= sc8238_write_reg(dev, 0x36f9, 0x80);
    ret |= sc8238_write_reg(dev, 0x3018, 0x72);
    ret |= sc8238_write_reg(dev, 0x3019, 0x00);
    ret |= sc8238_write_reg(dev, 0x301f, 0x5a);
    ret |= sc8238_write_reg(dev, 0x3031, 0x0a);
    ret |= sc8238_write_reg(dev, 0x3037, 0x20);
    ret |= sc8238_write_reg(dev, 0x3038, 0x44);
    ret |= sc8238_write_reg(dev, 0x320c, 0x08);
    ret |= sc8238_write_reg(dev, 0x320d, 0x56);
    ret |= sc8238_write_reg(dev, 0x3241, 0x00);
    ret |= sc8238_write_reg(dev, 0x3243, 0x03);
    ret |= sc8238_write_reg(dev, 0x3248, 0x04);
    ret |= sc8238_write_reg(dev, 0x3271, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3273, 0x1f);
    ret |= sc8238_write_reg(dev, 0x3301, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3306, 0xa8);
    ret |= sc8238_write_reg(dev, 0x3308, 0x20);
    ret |= sc8238_write_reg(dev, 0x3309, 0x68);
    ret |= sc8238_write_reg(dev, 0x330b, 0x48);
    ret |= sc8238_write_reg(dev, 0x330d, 0x28);
    ret |= sc8238_write_reg(dev, 0x330e, 0x58);
    ret |= sc8238_write_reg(dev, 0x3314, 0x94);
    ret |= sc8238_write_reg(dev, 0x331f, 0x59);
    ret |= sc8238_write_reg(dev, 0x3332, 0x24);
    ret |= sc8238_write_reg(dev, 0x334c, 0x10);
    ret |= sc8238_write_reg(dev, 0x3350, 0x24);
    ret |= sc8238_write_reg(dev, 0x3358, 0x24);
    ret |= sc8238_write_reg(dev, 0x335c, 0x24);
    ret |= sc8238_write_reg(dev, 0x335d, 0x60);
    ret |= sc8238_write_reg(dev, 0x3364, 0x16);
    ret |= sc8238_write_reg(dev, 0x3366, 0x92);
    ret |= sc8238_write_reg(dev, 0x3367, 0x08);
    ret |= sc8238_write_reg(dev, 0x3368, 0x07);
    ret |= sc8238_write_reg(dev, 0x3369, 0x00);
    ret |= sc8238_write_reg(dev, 0x336a, 0x00);
    ret |= sc8238_write_reg(dev, 0x336b, 0x00);
    ret |= sc8238_write_reg(dev, 0x336c, 0xc2);
    ret |= sc8238_write_reg(dev, 0x337f, 0x33);
    ret |= sc8238_write_reg(dev, 0x3390, 0x08);
    ret |= sc8238_write_reg(dev, 0x3391, 0x18);
    ret |= sc8238_write_reg(dev, 0x3392, 0x38);
    ret |= sc8238_write_reg(dev, 0x3393, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3394, 0x28);
    ret |= sc8238_write_reg(dev, 0x3395, 0x60);
    ret |= sc8238_write_reg(dev, 0x3396, 0x08);
    ret |= sc8238_write_reg(dev, 0x3397, 0x18);
    ret |= sc8238_write_reg(dev, 0x3398, 0x38);
    ret |= sc8238_write_reg(dev, 0x3399, 0x1c);
    ret |= sc8238_write_reg(dev, 0x339a, 0x1c);
    ret |= sc8238_write_reg(dev, 0x339b, 0x28);
    ret |= sc8238_write_reg(dev, 0x339c, 0x60);
    ret |= sc8238_write_reg(dev, 0x339e, 0x24);
    ret |= sc8238_write_reg(dev, 0x33aa, 0x24);
    ret |= sc8238_write_reg(dev, 0x33af, 0x48);
    ret |= sc8238_write_reg(dev, 0x33e1, 0x08);
    ret |= sc8238_write_reg(dev, 0x33e2, 0x18);
    ret |= sc8238_write_reg(dev, 0x33e3, 0x10);
    ret |= sc8238_write_reg(dev, 0x33e4, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33e5, 0x10);
    ret |= sc8238_write_reg(dev, 0x33e6, 0x06);
    ret |= sc8238_write_reg(dev, 0x33e7, 0x02);
    ret |= sc8238_write_reg(dev, 0x33e8, 0x18);
    ret |= sc8238_write_reg(dev, 0x33e9, 0x10);
    ret |= sc8238_write_reg(dev, 0x33ea, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33eb, 0x10);
    ret |= sc8238_write_reg(dev, 0x33ec, 0x04);
    ret |= sc8238_write_reg(dev, 0x33ed, 0x02);
    ret |= sc8238_write_reg(dev, 0x33ee, 0xa0);
    ret |= sc8238_write_reg(dev, 0x33ef, 0x08);
    ret |= sc8238_write_reg(dev, 0x33f4, 0x18);
    ret |= sc8238_write_reg(dev, 0x33f5, 0x10);
    ret |= sc8238_write_reg(dev, 0x33f6, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33f7, 0x10);
    ret |= sc8238_write_reg(dev, 0x33f8, 0x06);
    ret |= sc8238_write_reg(dev, 0x33f9, 0x02);
    ret |= sc8238_write_reg(dev, 0x33fa, 0x18);
    ret |= sc8238_write_reg(dev, 0x33fb, 0x10);
    ret |= sc8238_write_reg(dev, 0x33fc, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33fd, 0x10);
    ret |= sc8238_write_reg(dev, 0x33fe, 0x04);
    ret |= sc8238_write_reg(dev, 0x33ff, 0x02);
    ret |= sc8238_write_reg(dev, 0x360f, 0x01);
    ret |= sc8238_write_reg(dev, 0x3622, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3624, 0x45);
    ret |= sc8238_write_reg(dev, 0x3625, 0x0a);
    ret |= sc8238_write_reg(dev, 0x3628, 0x83);
    ret |= sc8238_write_reg(dev, 0x3630, 0x80);
    ret |= sc8238_write_reg(dev, 0x3631, 0x80);
    ret |= sc8238_write_reg(dev, 0x3632, 0xa8);
    ret |= sc8238_write_reg(dev, 0x3633, 0x53);
    ret |= sc8238_write_reg(dev, 0x3635, 0x02);
    ret |= sc8238_write_reg(dev, 0x3637, 0x52);
    ret |= sc8238_write_reg(dev, 0x3638, 0x0a);
    ret |= sc8238_write_reg(dev, 0x363a, 0x88);
    ret |= sc8238_write_reg(dev, 0x363b, 0x06);
    ret |= sc8238_write_reg(dev, 0x363d, 0x01);
    ret |= sc8238_write_reg(dev, 0x363e, 0x00);
    ret |= sc8238_write_reg(dev, 0x3641, 0x00);
    ret |= sc8238_write_reg(dev, 0x3670, 0x4a);
    ret |= sc8238_write_reg(dev, 0x3671, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3672, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3673, 0x17);
    ret |= sc8238_write_reg(dev, 0x3674, 0x80);
    ret |= sc8238_write_reg(dev, 0x3675, 0x85);
    ret |= sc8238_write_reg(dev, 0x3676, 0xa5);
    ret |= sc8238_write_reg(dev, 0x367a, 0x48);
    ret |= sc8238_write_reg(dev, 0x367b, 0x78);
    ret |= sc8238_write_reg(dev, 0x367c, 0x48);
    ret |= sc8238_write_reg(dev, 0x367d, 0x78);
    ret |= sc8238_write_reg(dev, 0x3690, 0x53);
    ret |= sc8238_write_reg(dev, 0x3691, 0x63);
    ret |= sc8238_write_reg(dev, 0x3692, 0x54);
    ret |= sc8238_write_reg(dev, 0x3699, 0x88);
    ret |= sc8238_write_reg(dev, 0x369a, 0x9f);
    ret |= sc8238_write_reg(dev, 0x369b, 0x9f);
    ret |= sc8238_write_reg(dev, 0x369c, 0x48);
    ret |= sc8238_write_reg(dev, 0x369d, 0x78);
    ret |= sc8238_write_reg(dev, 0x36a2, 0x48);
    ret |= sc8238_write_reg(dev, 0x36a3, 0x78);
    ret |= sc8238_write_reg(dev, 0x36bb, 0x48);
    ret |= sc8238_write_reg(dev, 0x36bc, 0x78);
    ret |= sc8238_write_reg(dev, 0x36c9, 0x05);
    ret |= sc8238_write_reg(dev, 0x36ca, 0x05);
    ret |= sc8238_write_reg(dev, 0x36cb, 0x05);
    ret |= sc8238_write_reg(dev, 0x36cc, 0x00);
    ret |= sc8238_write_reg(dev, 0x36cd, 0x10);
    ret |= sc8238_write_reg(dev, 0x36ce, 0x1a);
    ret |= sc8238_write_reg(dev, 0x36d0, 0x30);
    ret |= sc8238_write_reg(dev, 0x36d1, 0x48);
    ret |= sc8238_write_reg(dev, 0x36d2, 0x78);
    ret |= sc8238_write_reg(dev, 0x36ea, 0x3b);
    ret |= sc8238_write_reg(dev, 0x36eb, 0x06);
    ret |= sc8238_write_reg(dev, 0x36ec, 0x05);
    ret |= sc8238_write_reg(dev, 0x36ed, 0x14);
    ret |= sc8238_write_reg(dev, 0x36fa, 0x30);
    ret |= sc8238_write_reg(dev, 0x36fb, 0x13);
    ret |= sc8238_write_reg(dev, 0x36fc, 0x10);
    ret |= sc8238_write_reg(dev, 0x36fd, 0x34);
    ret |= sc8238_write_reg(dev, 0x3901, 0x00);
    ret |= sc8238_write_reg(dev, 0x3902, 0xc5);
    ret |= sc8238_write_reg(dev, 0x3904, 0x18);
    ret |= sc8238_write_reg(dev, 0x3905, 0xd8);
    ret |= sc8238_write_reg(dev, 0x394c, 0x0f);
    ret |= sc8238_write_reg(dev, 0x394d, 0x20);
    ret |= sc8238_write_reg(dev, 0x394e, 0x08);
    ret |= sc8238_write_reg(dev, 0x394f, 0x90);
    ret |= sc8238_write_reg(dev, 0x3980, 0x71);
    ret |= sc8238_write_reg(dev, 0x3981, 0x70);
    ret |= sc8238_write_reg(dev, 0x3982, 0x00);
    ret |= sc8238_write_reg(dev, 0x3983, 0x00);
    ret |= sc8238_write_reg(dev, 0x3984, 0x20);
    ret |= sc8238_write_reg(dev, 0x3987, 0x0b);
    ret |= sc8238_write_reg(dev, 0x3990, 0x03);
    ret |= sc8238_write_reg(dev, 0x3991, 0xfd);
    ret |= sc8238_write_reg(dev, 0x3992, 0x03);
    ret |= sc8238_write_reg(dev, 0x3993, 0xfc);
    ret |= sc8238_write_reg(dev, 0x3994, 0x00);
    ret |= sc8238_write_reg(dev, 0x3995, 0x00);
    ret |= sc8238_write_reg(dev, 0x3996, 0x00);
    ret |= sc8238_write_reg(dev, 0x3997, 0x05);
    ret |= sc8238_write_reg(dev, 0x3998, 0x00);
    ret |= sc8238_write_reg(dev, 0x3999, 0x09);
    ret |= sc8238_write_reg(dev, 0x399a, 0x00);
    ret |= sc8238_write_reg(dev, 0x399b, 0x12);
    ret |= sc8238_write_reg(dev, 0x399c, 0x00);
    ret |= sc8238_write_reg(dev, 0x399d, 0x12);
    ret |= sc8238_write_reg(dev, 0x399e, 0x00);
    ret |= sc8238_write_reg(dev, 0x399f, 0x18);
    ret |= sc8238_write_reg(dev, 0x39a0, 0x00);
    ret |= sc8238_write_reg(dev, 0x39a1, 0x14);
    ret |= sc8238_write_reg(dev, 0x39a2, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a3, 0xe3);
    ret |= sc8238_write_reg(dev, 0x39a4, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a5, 0xf2);
    ret |= sc8238_write_reg(dev, 0x39a6, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a7, 0xf6);
    ret |= sc8238_write_reg(dev, 0x39a8, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a9, 0xfa);
    ret |= sc8238_write_reg(dev, 0x39aa, 0x03);
    ret |= sc8238_write_reg(dev, 0x39ab, 0xff);
    ret |= sc8238_write_reg(dev, 0x39ac, 0x00);
    ret |= sc8238_write_reg(dev, 0x39ad, 0x06);
    ret |= sc8238_write_reg(dev, 0x39ae, 0x00);
    ret |= sc8238_write_reg(dev, 0x39af, 0x09);
    ret |= sc8238_write_reg(dev, 0x39b0, 0x00);
    ret |= sc8238_write_reg(dev, 0x39b1, 0x12);
    ret |= sc8238_write_reg(dev, 0x39b2, 0x00);
    ret |= sc8238_write_reg(dev, 0x39b3, 0x22);
    ret |= sc8238_write_reg(dev, 0x39b4, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39b5, 0x1c);
    ret |= sc8238_write_reg(dev, 0x39b6, 0x38);
    ret |= sc8238_write_reg(dev, 0x39b7, 0x5b);
    ret |= sc8238_write_reg(dev, 0x39b8, 0x50);
    ret |= sc8238_write_reg(dev, 0x39b9, 0x38);
    ret |= sc8238_write_reg(dev, 0x39ba, 0x20);
    ret |= sc8238_write_reg(dev, 0x39bb, 0x10);
    ret |= sc8238_write_reg(dev, 0x39bc, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39bd, 0x16);
    ret |= sc8238_write_reg(dev, 0x39be, 0x21);
    ret |= sc8238_write_reg(dev, 0x39bf, 0x36);
    ret |= sc8238_write_reg(dev, 0x39c0, 0x3b);
    ret |= sc8238_write_reg(dev, 0x39c1, 0x2a);
    ret |= sc8238_write_reg(dev, 0x39c2, 0x16);
    ret |= sc8238_write_reg(dev, 0x39c3, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39c5, 0x30);
    ret |= sc8238_write_reg(dev, 0x39c6, 0x07);
    ret |= sc8238_write_reg(dev, 0x39c7, 0xf8);
    ret |= sc8238_write_reg(dev, 0x39c9, 0x07);
    ret |= sc8238_write_reg(dev, 0x39ca, 0xf8);
    ret |= sc8238_write_reg(dev, 0x39cc, 0x00);
    ret |= sc8238_write_reg(dev, 0x39cd, 0x1b);
    ret |= sc8238_write_reg(dev, 0x39ce, 0x00);
    ret |= sc8238_write_reg(dev, 0x39cf, 0x00);
    ret |= sc8238_write_reg(dev, 0x39d0, 0x1b);
    ret |= sc8238_write_reg(dev, 0x39d1, 0x00);
    ret |= sc8238_write_reg(dev, 0x39e2, 0x15);
    ret |= sc8238_write_reg(dev, 0x39e3, 0x87);
    ret |= sc8238_write_reg(dev, 0x39e4, 0x12);
    ret |= sc8238_write_reg(dev, 0x39e5, 0xb7);
    ret |= sc8238_write_reg(dev, 0x39e6, 0x00);
    ret |= sc8238_write_reg(dev, 0x39e7, 0x8c);
    ret |= sc8238_write_reg(dev, 0x39e8, 0x01);
    ret |= sc8238_write_reg(dev, 0x39e9, 0x31);
    ret |= sc8238_write_reg(dev, 0x39ea, 0x01);
    ret |= sc8238_write_reg(dev, 0x39eb, 0xd7);
    ret |= sc8238_write_reg(dev, 0x39ec, 0x08);
    ret |= sc8238_write_reg(dev, 0x39ed, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e00, 0x01);
    ret |= sc8238_write_reg(dev, 0x3e01, 0x18);
    ret |= sc8238_write_reg(dev, 0x3e02, 0xa0);
    ret |= sc8238_write_reg(dev, 0x3e08, 0x03);
    ret |= sc8238_write_reg(dev, 0x3e09, 0x40);
    ret |= sc8238_write_reg(dev, 0x3e0e, 0x09);
    ret |= sc8238_write_reg(dev, 0x3e14, 0x31);
    ret |= sc8238_write_reg(dev, 0x3e16, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e17, 0xac);
    ret |= sc8238_write_reg(dev, 0x3e18, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e19, 0xac);
    ret |= sc8238_write_reg(dev, 0x3e1b, 0x3a);
    ret |= sc8238_write_reg(dev, 0x3e1e, 0x76);
    ret |= sc8238_write_reg(dev, 0x3e25, 0x23);
    ret |= sc8238_write_reg(dev, 0x3e26, 0x40);
    ret |= sc8238_write_reg(dev, 0x4501, 0xa4);
    ret |= sc8238_write_reg(dev, 0x4509, 0x10);
    ret |= sc8238_write_reg(dev, 0x4800, 0x64);
    ret |= sc8238_write_reg(dev, 0x4837, 0x1c);
    ret |= sc8238_write_reg(dev, 0x5799, 0x06);
    ret |= sc8238_write_reg(dev, 0x57aa, 0x2f);
    ret |= sc8238_write_reg(dev, 0x57ab, 0xff);
    ret |= sc8238_write_reg(dev, 0x5988, 0x86);
    ret |= sc8238_write_reg(dev, 0x598e, 0x07);
    ret |= sc8238_write_reg(dev, 0x598f, 0xee);
    ret |= sc8238_write_reg(dev, 0x36e9, 0x23);
    ret |= sc8238_write_reg(dev, 0x36f9, 0x20);
    ret |= sc8238_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

/*
 * 函数功能: sc8238 MIPI 4lanes 1520p线性初始化序列 - 2688x1520
 * 函数参数：
 *      dev -  senor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc8238_4lane_linear_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc8238_write_reg(dev, 0x0103, 0x01);
    ret |= sc8238_write_reg(dev, 0x0100, 0x00);
    ret |= sc8238_write_reg(dev, 0x3208, 0x0a);
    ret |= sc8238_write_reg(dev, 0x3209, 0x80);
    ret |= sc8238_write_reg(dev, 0x320a, 0x05);
    ret |= sc8238_write_reg(dev, 0x320b, 0xf0);
    ret |= sc8238_linear_comm_init(dev);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=================================================================================\n");
    SENSOR_PRINT("====== SC8238_24MInput_MIPI_4lane_10bit_2688x1520_%.3ffps init success!==========\n", MAX_FPS_LINEAR);
    SENSOR_PRINT("=================================================================================\n");
    return XMEDIA_SUCCESS;
}

/*
 * 函数功能: sc8238 MIPI 4lanes 720p线性初始化序列 - 1280x720
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc8238_4lane_linear_1280x720_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc8238_write_reg(dev, 0x0103, 0x01);
    ret |= sc8238_write_reg(dev, 0x0100, 0x00);
    ret |= sc8238_write_reg(dev, 0x3208, 0x05);
    ret |= sc8238_write_reg(dev, 0x3209, 0x00);
    ret |= sc8238_write_reg(dev, 0x320a, 0x02);
    ret |= sc8238_write_reg(dev, 0x320b, 0xd0);
    ret |= sc8238_linear_comm_init(dev);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=================================================================================\n");
    SENSOR_PRINT("===== SC8238_24MInput_MIPI_4lane_10bit_1280x720_%.3ffps init success! ===========\n", MAX_FPS_LINEAR);
    SENSOR_PRINT("=================================================================================\n");
    return XMEDIA_SUCCESS;
}

/*
 * 函数功能: sc8238 MIPI 4lanes 1728p线性初始化序列 - 3072x1728
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc8238_4lane_linear_3072x1728_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc8238_write_reg(dev, 0x0103, 0x10);
    ret |= sc8238_write_reg(dev, 0x0100, 0x00);
    ret |= sc8238_write_reg(dev, 0x36e9, 0x80);
    ret |= sc8238_write_reg(dev, 0x36f9, 0x80);
    ret |= sc8238_write_reg(dev, 0x3018, 0x72);
    ret |= sc8238_write_reg(dev, 0x3019, 0x00);
    ret |= sc8238_write_reg(dev, 0x301f, 0x9b);
    ret |= sc8238_write_reg(dev, 0x3031, 0x0a);
    ret |= sc8238_write_reg(dev, 0x3037, 0x20);
    ret |= sc8238_write_reg(dev, 0x3038, 0x44);
    ret |= sc8238_write_reg(dev, 0x3200, 0x01);
    ret |= sc8238_write_reg(dev, 0x3201, 0x88);
    ret |= sc8238_write_reg(dev, 0x3202, 0x00);
    ret |= sc8238_write_reg(dev, 0x3203, 0xe0);
    ret |= sc8238_write_reg(dev, 0x3204, 0x0d);
    ret |= sc8238_write_reg(dev, 0x3205, 0x97);
    ret |= sc8238_write_reg(dev, 0x3206, 0x07);
    ret |= sc8238_write_reg(dev, 0x3207, 0xaf);
    ret |= sc8238_write_reg(dev, 0x3208, 0x0c);
    ret |= sc8238_write_reg(dev, 0x3209, 0x00);
    ret |= sc8238_write_reg(dev, 0x320a, 0x06);
    ret |= sc8238_write_reg(dev, 0x320b, 0xc0);
    ret |= sc8238_write_reg(dev, 0x320c, 0x09);
    ret |= sc8238_write_reg(dev, 0x320d, 0x60);
    ret |= sc8238_write_reg(dev, 0x320e, 0x07);
    ret |= sc8238_write_reg(dev, 0x320f, 0xd0);
    ret |= sc8238_write_reg(dev, 0x3210, 0x00);
    ret |= sc8238_write_reg(dev, 0x3211, 0x08);
    ret |= sc8238_write_reg(dev, 0x3212, 0x00);
    ret |= sc8238_write_reg(dev, 0x3213, 0x08);
    ret |= sc8238_write_reg(dev, 0x3241, 0x00);
    ret |= sc8238_write_reg(dev, 0x3243, 0x03);
    ret |= sc8238_write_reg(dev, 0x3248, 0x04);
    ret |= sc8238_write_reg(dev, 0x3271, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3273, 0x1f);
    ret |= sc8238_write_reg(dev, 0x3301, 0x78);
    ret |= sc8238_write_reg(dev, 0x3306, 0xa8);
    ret |= sc8238_write_reg(dev, 0x3308, 0x20);
    ret |= sc8238_write_reg(dev, 0x3309, 0x68);
    ret |= sc8238_write_reg(dev, 0x330b, 0x60);
    ret |= sc8238_write_reg(dev, 0x330d, 0x28);
    ret |= sc8238_write_reg(dev, 0x330e, 0x58);
    ret |= sc8238_write_reg(dev, 0x3314, 0x94);
    ret |= sc8238_write_reg(dev, 0x331f, 0x59);
    ret |= sc8238_write_reg(dev, 0x3332, 0x24);
    ret |= sc8238_write_reg(dev, 0x334c, 0x10);
    ret |= sc8238_write_reg(dev, 0x3350, 0x24);
    ret |= sc8238_write_reg(dev, 0x3358, 0x24);
    ret |= sc8238_write_reg(dev, 0x335c, 0x24);
    ret |= sc8238_write_reg(dev, 0x335d, 0x60);
    ret |= sc8238_write_reg(dev, 0x3360, 0x40);
    ret |= sc8238_write_reg(dev, 0x3362, 0x72);
    ret |= sc8238_write_reg(dev, 0x3364, 0x16);
    ret |= sc8238_write_reg(dev, 0x3366, 0x92);
    ret |= sc8238_write_reg(dev, 0x3367, 0x08);
    ret |= sc8238_write_reg(dev, 0x3368, 0x07);
    ret |= sc8238_write_reg(dev, 0x3369, 0x00);
    ret |= sc8238_write_reg(dev, 0x336a, 0x00);
    ret |= sc8238_write_reg(dev, 0x336b, 0x00);
    ret |= sc8238_write_reg(dev, 0x336c, 0xc2);
    ret |= sc8238_write_reg(dev, 0x337f, 0x33);
    ret |= sc8238_write_reg(dev, 0x3390, 0x08);
    ret |= sc8238_write_reg(dev, 0x3391, 0x18);
    ret |= sc8238_write_reg(dev, 0x3392, 0x38);
    ret |= sc8238_write_reg(dev, 0x3393, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3394, 0x28);
    ret |= sc8238_write_reg(dev, 0x3395, 0x60);
    ret |= sc8238_write_reg(dev, 0x3396, 0x08);
    ret |= sc8238_write_reg(dev, 0x3397, 0x18);
    ret |= sc8238_write_reg(dev, 0x3398, 0x38);
    ret |= sc8238_write_reg(dev, 0x3399, 0x1c);
    ret |= sc8238_write_reg(dev, 0x339a, 0x1c);
    ret |= sc8238_write_reg(dev, 0x339b, 0x28);
    ret |= sc8238_write_reg(dev, 0x339c, 0x60);
    ret |= sc8238_write_reg(dev, 0x339e, 0x24);
    ret |= sc8238_write_reg(dev, 0x33aa, 0x24);
    ret |= sc8238_write_reg(dev, 0x33af, 0x48);
    ret |= sc8238_write_reg(dev, 0x33e1, 0x08);
    ret |= sc8238_write_reg(dev, 0x33e2, 0x18);
    ret |= sc8238_write_reg(dev, 0x33e3, 0x10);
    ret |= sc8238_write_reg(dev, 0x33e4, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33e5, 0x10);
    ret |= sc8238_write_reg(dev, 0x33e6, 0x06);
    ret |= sc8238_write_reg(dev, 0x33e7, 0x02);
    ret |= sc8238_write_reg(dev, 0x33e8, 0x18);
    ret |= sc8238_write_reg(dev, 0x33e9, 0x10);
    ret |= sc8238_write_reg(dev, 0x33ea, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33eb, 0x10);
    ret |= sc8238_write_reg(dev, 0x33ec, 0x04);
    ret |= sc8238_write_reg(dev, 0x33ed, 0x02);
    ret |= sc8238_write_reg(dev, 0x33ee, 0xa0);
    ret |= sc8238_write_reg(dev, 0x33ef, 0x08);
    ret |= sc8238_write_reg(dev, 0x33f4, 0x18);
    ret |= sc8238_write_reg(dev, 0x33f5, 0x10);
    ret |= sc8238_write_reg(dev, 0x33f6, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33f7, 0x10);
    ret |= sc8238_write_reg(dev, 0x33f8, 0x06);
    ret |= sc8238_write_reg(dev, 0x33f9, 0x02);
    ret |= sc8238_write_reg(dev, 0x33fa, 0x18);
    ret |= sc8238_write_reg(dev, 0x33fb, 0x10);
    ret |= sc8238_write_reg(dev, 0x33fc, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33fd, 0x10);
    ret |= sc8238_write_reg(dev, 0x33fe, 0x04);
    ret |= sc8238_write_reg(dev, 0x33ff, 0x02);
    ret |= sc8238_write_reg(dev, 0x360f, 0x01);
    ret |= sc8238_write_reg(dev, 0x3622, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3624, 0x45);
    ret |= sc8238_write_reg(dev, 0x3628, 0x83);
    ret |= sc8238_write_reg(dev, 0x3630, 0x80);
    ret |= sc8238_write_reg(dev, 0x3631, 0x80);
    ret |= sc8238_write_reg(dev, 0x3632, 0xa8);
    ret |= sc8238_write_reg(dev, 0x3633, 0x23);
    ret |= sc8238_write_reg(dev, 0x3635, 0x02);
    ret |= sc8238_write_reg(dev, 0x3637, 0x52);
    ret |= sc8238_write_reg(dev, 0x3638, 0x0a);
    ret |= sc8238_write_reg(dev, 0x363a, 0x88);
    ret |= sc8238_write_reg(dev, 0x363b, 0x06);
    ret |= sc8238_write_reg(dev, 0x363d, 0x01);
    ret |= sc8238_write_reg(dev, 0x363e, 0x00);
    ret |= sc8238_write_reg(dev, 0x3641, 0x00);
    ret |= sc8238_write_reg(dev, 0x3670, 0x4a);
    ret |= sc8238_write_reg(dev, 0x3671, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3672, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3673, 0x17);
    ret |= sc8238_write_reg(dev, 0x3674, 0x80);
    ret |= sc8238_write_reg(dev, 0x3675, 0x85);
    ret |= sc8238_write_reg(dev, 0x3676, 0xa5);
    ret |= sc8238_write_reg(dev, 0x367a, 0x48);
    ret |= sc8238_write_reg(dev, 0x367b, 0x78);
    ret |= sc8238_write_reg(dev, 0x367c, 0x48);
    ret |= sc8238_write_reg(dev, 0x367d, 0x78);
    ret |= sc8238_write_reg(dev, 0x3690, 0x53);
    ret |= sc8238_write_reg(dev, 0x3691, 0x63);
    ret |= sc8238_write_reg(dev, 0x3692, 0x54);
    ret |= sc8238_write_reg(dev, 0x3699, 0x88);
    ret |= sc8238_write_reg(dev, 0x369a, 0x9f);
    ret |= sc8238_write_reg(dev, 0x369b, 0x9f);
    ret |= sc8238_write_reg(dev, 0x369c, 0x48);
    ret |= sc8238_write_reg(dev, 0x369d, 0x78);
    ret |= sc8238_write_reg(dev, 0x36a2, 0x48);
    ret |= sc8238_write_reg(dev, 0x36a3, 0x78);
    ret |= sc8238_write_reg(dev, 0x36bb, 0x48);
    ret |= sc8238_write_reg(dev, 0x36bc, 0x78);
    ret |= sc8238_write_reg(dev, 0x36c9, 0x05);
    ret |= sc8238_write_reg(dev, 0x36ca, 0x05);
    ret |= sc8238_write_reg(dev, 0x36cb, 0x05);
    ret |= sc8238_write_reg(dev, 0x36cc, 0x00);
    ret |= sc8238_write_reg(dev, 0x36cd, 0x10);
    ret |= sc8238_write_reg(dev, 0x36ce, 0x1a);
    ret |= sc8238_write_reg(dev, 0x36d0, 0x30);
    ret |= sc8238_write_reg(dev, 0x36d1, 0x48);
    ret |= sc8238_write_reg(dev, 0x36d2, 0x78);
    ret |= sc8238_write_reg(dev, 0x36ea, 0x3b);
    ret |= sc8238_write_reg(dev, 0x36eb, 0x06);
    ret |= sc8238_write_reg(dev, 0x36ec, 0x05);
    ret |= sc8238_write_reg(dev, 0x36ed, 0x14);
    ret |= sc8238_write_reg(dev, 0x36fa, 0x38);
    ret |= sc8238_write_reg(dev, 0x36fb, 0x13);
    ret |= sc8238_write_reg(dev, 0x36fc, 0x10);
    ret |= sc8238_write_reg(dev, 0x36fd, 0x14);
    ret |= sc8238_write_reg(dev, 0x3901, 0x00);
    ret |= sc8238_write_reg(dev, 0x3902, 0xc5);
    ret |= sc8238_write_reg(dev, 0x3904, 0x18);
    ret |= sc8238_write_reg(dev, 0x3905, 0xd8);
    ret |= sc8238_write_reg(dev, 0x394c, 0x0f);
    ret |= sc8238_write_reg(dev, 0x394d, 0x20);
    ret |= sc8238_write_reg(dev, 0x394e, 0x08);
    ret |= sc8238_write_reg(dev, 0x394f, 0x90);
    ret |= sc8238_write_reg(dev, 0x3980, 0x71);
    ret |= sc8238_write_reg(dev, 0x3981, 0x70);
    ret |= sc8238_write_reg(dev, 0x3982, 0x00);
    ret |= sc8238_write_reg(dev, 0x3983, 0x00);
    ret |= sc8238_write_reg(dev, 0x3984, 0x20);
    ret |= sc8238_write_reg(dev, 0x3987, 0x0b);
    ret |= sc8238_write_reg(dev, 0x3990, 0x03);
    ret |= sc8238_write_reg(dev, 0x3991, 0xfd);
    ret |= sc8238_write_reg(dev, 0x3992, 0x03);
    ret |= sc8238_write_reg(dev, 0x3993, 0xfc);
    ret |= sc8238_write_reg(dev, 0x3994, 0x00);
    ret |= sc8238_write_reg(dev, 0x3995, 0x00);
    ret |= sc8238_write_reg(dev, 0x3996, 0x00);
    ret |= sc8238_write_reg(dev, 0x3997, 0x05);
    ret |= sc8238_write_reg(dev, 0x3998, 0x00);
    ret |= sc8238_write_reg(dev, 0x3999, 0x09);
    ret |= sc8238_write_reg(dev, 0x399a, 0x00);
    ret |= sc8238_write_reg(dev, 0x399b, 0x12);
    ret |= sc8238_write_reg(dev, 0x399c, 0x00);
    ret |= sc8238_write_reg(dev, 0x399d, 0x12);
    ret |= sc8238_write_reg(dev, 0x399e, 0x00);
    ret |= sc8238_write_reg(dev, 0x399f, 0x18);
    ret |= sc8238_write_reg(dev, 0x39a0, 0x00);
    ret |= sc8238_write_reg(dev, 0x39a1, 0x14);
    ret |= sc8238_write_reg(dev, 0x39a2, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a3, 0xe3);
    ret |= sc8238_write_reg(dev, 0x39a4, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a5, 0xf2);
    ret |= sc8238_write_reg(dev, 0x39a6, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a7, 0xf6);
    ret |= sc8238_write_reg(dev, 0x39a8, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a9, 0xfa);
    ret |= sc8238_write_reg(dev, 0x39aa, 0x03);
    ret |= sc8238_write_reg(dev, 0x39ab, 0xff);
    ret |= sc8238_write_reg(dev, 0x39ac, 0x00);
    ret |= sc8238_write_reg(dev, 0x39ad, 0x06);
    ret |= sc8238_write_reg(dev, 0x39ae, 0x00);
    ret |= sc8238_write_reg(dev, 0x39af, 0x09);
    ret |= sc8238_write_reg(dev, 0x39b0, 0x00);
    ret |= sc8238_write_reg(dev, 0x39b1, 0x12);
    ret |= sc8238_write_reg(dev, 0x39b2, 0x00);
    ret |= sc8238_write_reg(dev, 0x39b3, 0x22);
    ret |= sc8238_write_reg(dev, 0x39b4, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39b5, 0x1c);
    ret |= sc8238_write_reg(dev, 0x39b6, 0x38);
    ret |= sc8238_write_reg(dev, 0x39b7, 0x5b);
    ret |= sc8238_write_reg(dev, 0x39b8, 0x50);
    ret |= sc8238_write_reg(dev, 0x39b9, 0x38);
    ret |= sc8238_write_reg(dev, 0x39ba, 0x20);
    ret |= sc8238_write_reg(dev, 0x39bb, 0x10);
    ret |= sc8238_write_reg(dev, 0x39bc, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39bd, 0x16);
    ret |= sc8238_write_reg(dev, 0x39be, 0x21);
    ret |= sc8238_write_reg(dev, 0x39bf, 0x36);
    ret |= sc8238_write_reg(dev, 0x39c0, 0x3b);
    ret |= sc8238_write_reg(dev, 0x39c1, 0x2a);
    ret |= sc8238_write_reg(dev, 0x39c2, 0x16);
    ret |= sc8238_write_reg(dev, 0x39c3, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39c5, 0x30);
    ret |= sc8238_write_reg(dev, 0x39c6, 0x07);
    ret |= sc8238_write_reg(dev, 0x39c7, 0xf8);
    ret |= sc8238_write_reg(dev, 0x39c9, 0x07);
    ret |= sc8238_write_reg(dev, 0x39ca, 0xf8);
    ret |= sc8238_write_reg(dev, 0x39cc, 0x00);
    ret |= sc8238_write_reg(dev, 0x39cd, 0x1b);
    ret |= sc8238_write_reg(dev, 0x39ce, 0x00);
    ret |= sc8238_write_reg(dev, 0x39cf, 0x00);
    ret |= sc8238_write_reg(dev, 0x39d0, 0x1b);
    ret |= sc8238_write_reg(dev, 0x39d1, 0x00);
    ret |= sc8238_write_reg(dev, 0x39e2, 0x15);
    ret |= sc8238_write_reg(dev, 0x39e3, 0x87);
    ret |= sc8238_write_reg(dev, 0x39e4, 0x12);
    ret |= sc8238_write_reg(dev, 0x39e5, 0xb7);
    ret |= sc8238_write_reg(dev, 0x39e6, 0x00);
    ret |= sc8238_write_reg(dev, 0x39e7, 0x8c);
    ret |= sc8238_write_reg(dev, 0x39e8, 0x01);
    ret |= sc8238_write_reg(dev, 0x39e9, 0x31);
    ret |= sc8238_write_reg(dev, 0x39ea, 0x01);
    ret |= sc8238_write_reg(dev, 0x39eb, 0xd7);
    ret |= sc8238_write_reg(dev, 0x39ec, 0x08);
    ret |= sc8238_write_reg(dev, 0x39ed, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e00, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e01, 0xf0);
    ret |= sc8238_write_reg(dev, 0x3e02, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e08, 0x03);
    ret |= sc8238_write_reg(dev, 0x3e09, 0x40);
    ret |= sc8238_write_reg(dev, 0x3e0e, 0x09);
    ret |= sc8238_write_reg(dev, 0x3e14, 0x31);
    ret |= sc8238_write_reg(dev, 0x3e16, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e17, 0xac);
    ret |= sc8238_write_reg(dev, 0x3e18, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e19, 0xac);
    ret |= sc8238_write_reg(dev, 0x3e1b, 0x3a);
    ret |= sc8238_write_reg(dev, 0x3e1e, 0x76);
    ret |= sc8238_write_reg(dev, 0x3e25, 0x23);
    ret |= sc8238_write_reg(dev, 0x3e26, 0x40);
    ret |= sc8238_write_reg(dev, 0x4501, 0xa4);
    ret |= sc8238_write_reg(dev, 0x4509, 0x10);
    ret |= sc8238_write_reg(dev, 0x4837, 0x1c);
    ret |= sc8238_write_reg(dev, 0x5799, 0x06);
    ret |= sc8238_write_reg(dev, 0x57aa, 0x2f);
    ret |= sc8238_write_reg(dev, 0x57ab, 0xff);
    ret |= sc8238_write_reg(dev, 0x36e9, 0x14);
    ret |= sc8238_write_reg(dev, 0x36f9, 0x14);
    ret |= sc8238_write_reg(dev, 0x3221, 0x66);
    ret |= sc8238_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("====================================================================================\n");
    SENSOR_PRINT("===== SC8238_24MInput_MIPI_4lane_10bit_3072x1728_30fps init success! ===============\n");
    SENSOR_PRINT("====================================================================================\n");
    return XMEDIA_SUCCESS;
}

#endif

/*
 * 函数功能: sc8238 MIPI 4lane 2160p线性模式初始化序列 - 3840x2160
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc8238_4lane_linear_3840x2160_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc8238_write_reg(dev, 0x0103, 0x01);
    ret |= sc8238_write_reg(dev, 0x0100, 0x00);
    ret |= sc8238_write_reg(dev, 0x3208, 0x0f); // widht
    ret |= sc8238_write_reg(dev, 0x3209, 0x00);
    ret |= sc8238_write_reg(dev, 0x320a, 0x08); // height
    ret |= sc8238_write_reg(dev, 0x320b, 0x70);
    ret |= sc8238_linear_comm_init(dev);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("============================================================================\n");
        SENSOR_PRINT("====== SC8238_24MInput_MIPI_4lane_10bit_3840x2160_%.3ffps init success! ====\n", MAX_FPS_LINEAR);
        SENSOR_PRINT("============================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

/*
 * 函数功能: sc8238 MIPI 4lane 5M线性模式初始化序列 - 2560x1920
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc8238_4lane_linear_2560x1920_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc8238_write_reg(dev, 0x0103, 0x01);
    ret |= sc8238_write_reg(dev, 0x0100, 0x00);
    ret |= sc8238_write_reg(dev, 0x3208, 0x0a); // widht
    ret |= sc8238_write_reg(dev, 0x3209, 0x00);
    ret |= sc8238_write_reg(dev, 0x320a, 0x07); // height
    ret |= sc8238_write_reg(dev, 0x320b, 0x80);
    ret |= sc8238_linear_comm_init(dev);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
    }else {
        SENSOR_PRINT("=============================================================================\n");
        SENSOR_PRINT("===== SC8238_24MInput_MIPI_4lane_10bit_2560x1920_%.3ffps init success!=======\n", MAX_FPS_LINEAR);
        SENSOR_PRINT("=============================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

/*
 * 函数功能: sc8238 MIPI 4lane 3M线性模式初始化序列 - 2048x1536
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc8238_4lane_linear_2048x1536_init(xmedia_u32 dev)
 {
    xmedia_s32 ret = XMEDIA_SUCCESS;
    ret |= sc8238_write_reg(dev, 0x0103, 0x01);
    ret |= sc8238_write_reg(dev, 0x0100, 0x00);
    ret |= sc8238_write_reg(dev, 0x3208, 0x08); // widht
    ret |= sc8238_write_reg(dev, 0x3209, 0x00);
    ret |= sc8238_write_reg(dev, 0x320a, 0x06); // height
    ret |= sc8238_write_reg(dev, 0x320b, 0x00);
    ret |= sc8238_linear_comm_init(dev);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
     }else {
        SENSOR_PRINT("=============================================================================\n");
        SENSOR_PRINT("======= SC8238_24MInput_MIPI_4lane_10bit_2048x1536_%.3ffps init success!=====\n", MAX_FPS_LINEAR);
        SENSOR_PRINT("=============================================================================\n");
        return XMEDIA_SUCCESS;
     }
}

/*
 * 函数功能: sc8238 MIPI 4lanes 1080p线性初始化序列 - 1920x1080x7.8fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc8238_4lane_linear_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    ret |= sc8238_write_reg(dev, 0x0103, 0x01);
    ret |= sc8238_write_reg(dev, 0x0100, 0x00);
    ret |= sc8238_write_reg(dev, 0x3208, 0x07); // widht
    ret |= sc8238_write_reg(dev, 0x3209, 0x80);
    ret |= sc8238_write_reg(dev, 0x320a, 0x04); // height
    ret |= sc8238_write_reg(dev, 0x320b, 0x38);
    ret |= sc8238_linear_comm_init(dev);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
    }else {
        SENSOR_PRINT("=============================================================================\n");
        SENSOR_PRINT("===== SC8238_24MInput_MIPI_4lane_10bit_1920x1080_%.3ffps init success! ======\n", MAX_FPS_LINEAR);
        SENSOR_PRINT("=============================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

#ifdef FPGA
#define MAX_FPS_WDR 1.693
static xmedia_s32 sc8238_wdr_comm_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc8238_write_reg(dev, 0x36e9, 0x80);
    ret |= sc8238_write_reg(dev, 0x36f9, 0x80);
    ret |= sc8238_write_reg(dev, 0x3018, 0x72);
    ret |= sc8238_write_reg(dev, 0x3019, 0x00);
    ret |= sc8238_write_reg(dev, 0x301f, 0xf4);
    ret |= sc8238_write_reg(dev, 0x3031, 0x0a);
    ret |= sc8238_write_reg(dev, 0x3037, 0x20);
    ret |= sc8238_write_reg(dev, 0x3038, 0x44);
    ret |= sc8238_write_reg(dev, 0x3200, 0x00);
    ret |= sc8238_write_reg(dev, 0x3201, 0x00);
    ret |= sc8238_write_reg(dev, 0x3202, 0x00);
    ret |= sc8238_write_reg(dev, 0x3203, 0x00);
    ret |= sc8238_write_reg(dev, 0x3204, 0x0f);
    ret |= sc8238_write_reg(dev, 0x3205, 0x1f);
    ret |= sc8238_write_reg(dev, 0x3206, 0x08);
    ret |= sc8238_write_reg(dev, 0x3207, 0x8f);
    ret |= sc8238_write_reg(dev, 0x320c, 0x08);
    ret |= sc8238_write_reg(dev, 0x320d, 0x34);
    ret |= sc8238_write_reg(dev, 0x320e, 0x11);
    ret |= sc8238_write_reg(dev, 0x320f, 0x94);
    ret |= sc8238_write_reg(dev, 0x3210, 0x00);
    ret |= sc8238_write_reg(dev, 0x3211, 0x10);
    ret |= sc8238_write_reg(dev, 0x3212, 0x00);
    ret |= sc8238_write_reg(dev, 0x3213, 0x10);
    ret |= sc8238_write_reg(dev, 0x3220, 0x50);
    ret |= sc8238_write_reg(dev, 0x3241, 0x00);
    ret |= sc8238_write_reg(dev, 0x3243, 0x03);
    ret |= sc8238_write_reg(dev, 0x3248, 0x04);
    ret |= sc8238_write_reg(dev, 0x3250, 0x3f);
    ret |= sc8238_write_reg(dev, 0x3271, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3273, 0x1f);
    ret |= sc8238_write_reg(dev, 0x3301, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3306, 0xa8);
    ret |= sc8238_write_reg(dev, 0x3308, 0x20);
    ret |= sc8238_write_reg(dev, 0x3309, 0x68);
    ret |= sc8238_write_reg(dev, 0x330b, 0x48);
    ret |= sc8238_write_reg(dev, 0x330d, 0x28);
    ret |= sc8238_write_reg(dev, 0x330e, 0x58);
    ret |= sc8238_write_reg(dev, 0x3314, 0x98);
    ret |= sc8238_write_reg(dev, 0x331f, 0x59);
    ret |= sc8238_write_reg(dev, 0x3332, 0x24);
    ret |= sc8238_write_reg(dev, 0x334a, 0x18);
    ret |= sc8238_write_reg(dev, 0x334c, 0x10);
    ret |= sc8238_write_reg(dev, 0x3350, 0x24);
    ret |= sc8238_write_reg(dev, 0x3358, 0x24);
    ret |= sc8238_write_reg(dev, 0x335c, 0x24);
    ret |= sc8238_write_reg(dev, 0x335d, 0x60);
    ret |= sc8238_write_reg(dev, 0x3364, 0x16);
    ret |= sc8238_write_reg(dev, 0x3366, 0x92);
    ret |= sc8238_write_reg(dev, 0x3367, 0x08);
    ret |= sc8238_write_reg(dev, 0x3368, 0x07);
    ret |= sc8238_write_reg(dev, 0x3369, 0x00);
    ret |= sc8238_write_reg(dev, 0x336a, 0x00);
    ret |= sc8238_write_reg(dev, 0x336b, 0x00);
    ret |= sc8238_write_reg(dev, 0x336c, 0xc2);
    ret |= sc8238_write_reg(dev, 0x336f, 0x58);
    ret |= sc8238_write_reg(dev, 0x337f, 0x33);
    ret |= sc8238_write_reg(dev, 0x3390, 0x08);
    ret |= sc8238_write_reg(dev, 0x3391, 0x18);
    ret |= sc8238_write_reg(dev, 0x3392, 0x38);
    ret |= sc8238_write_reg(dev, 0x3393, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3394, 0x28);
    ret |= sc8238_write_reg(dev, 0x3395, 0x60);
    ret |= sc8238_write_reg(dev, 0x3396, 0x08);
    ret |= sc8238_write_reg(dev, 0x3397, 0x18);
    ret |= sc8238_write_reg(dev, 0x3398, 0x38);
    ret |= sc8238_write_reg(dev, 0x3399, 0x1c);
    ret |= sc8238_write_reg(dev, 0x339a, 0x1c);
    ret |= sc8238_write_reg(dev, 0x339b, 0x28);
    ret |= sc8238_write_reg(dev, 0x339c, 0x60);
    ret |= sc8238_write_reg(dev, 0x339e, 0x24);
    ret |= sc8238_write_reg(dev, 0x33aa, 0x24);
    ret |= sc8238_write_reg(dev, 0x33af, 0x48);
    ret |= sc8238_write_reg(dev, 0x33e1, 0x08);
    ret |= sc8238_write_reg(dev, 0x33e2, 0x18);
    ret |= sc8238_write_reg(dev, 0x33e3, 0x10);
    ret |= sc8238_write_reg(dev, 0x33e4, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33e5, 0x10);
    ret |= sc8238_write_reg(dev, 0x33e6, 0x06);
    ret |= sc8238_write_reg(dev, 0x33e7, 0x02);
    ret |= sc8238_write_reg(dev, 0x33e8, 0x18);
    ret |= sc8238_write_reg(dev, 0x33e9, 0x10);
    ret |= sc8238_write_reg(dev, 0x33ea, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33eb, 0x10);
    ret |= sc8238_write_reg(dev, 0x33ec, 0x04);
    ret |= sc8238_write_reg(dev, 0x33ed, 0x02);
    ret |= sc8238_write_reg(dev, 0x33ee, 0xa0);
    ret |= sc8238_write_reg(dev, 0x33ef, 0x08);
    ret |= sc8238_write_reg(dev, 0x33f4, 0x18);
    ret |= sc8238_write_reg(dev, 0x33f5, 0x10);
    ret |= sc8238_write_reg(dev, 0x33f6, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33f7, 0x10);
    ret |= sc8238_write_reg(dev, 0x33f8, 0x06);
    ret |= sc8238_write_reg(dev, 0x33f9, 0x02);
    ret |= sc8238_write_reg(dev, 0x33fa, 0x18);
    ret |= sc8238_write_reg(dev, 0x33fb, 0x10);
    ret |= sc8238_write_reg(dev, 0x33fc, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33fd, 0x10);
    ret |= sc8238_write_reg(dev, 0x33fe, 0x04);
    ret |= sc8238_write_reg(dev, 0x33ff, 0x02);
    ret |= sc8238_write_reg(dev, 0x360f, 0x01);
    ret |= sc8238_write_reg(dev, 0x3622, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3624, 0x45);
    ret |= sc8238_write_reg(dev, 0x3628, 0x83);
    ret |= sc8238_write_reg(dev, 0x3630, 0x80);
    ret |= sc8238_write_reg(dev, 0x3631, 0x80);
    ret |= sc8238_write_reg(dev, 0x3632, 0xa8);
    ret |= sc8238_write_reg(dev, 0x3633, 0x53);
    ret |= sc8238_write_reg(dev, 0x3635, 0x02);
    ret |= sc8238_write_reg(dev, 0x3636, 0x11);
    ret |= sc8238_write_reg(dev, 0x3637, 0x10);
    ret |= sc8238_write_reg(dev, 0x3638, 0x0a);
    ret |= sc8238_write_reg(dev, 0x363a, 0x88);
    ret |= sc8238_write_reg(dev, 0x363b, 0x06);
    ret |= sc8238_write_reg(dev, 0x363d, 0x01);
    ret |= sc8238_write_reg(dev, 0x363e, 0x00);
    ret |= sc8238_write_reg(dev, 0x3641, 0x00);
    ret |= sc8238_write_reg(dev, 0x3670, 0x4a);
    ret |= sc8238_write_reg(dev, 0x3671, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3672, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3673, 0x17);
    ret |= sc8238_write_reg(dev, 0x3674, 0x80);
    ret |= sc8238_write_reg(dev, 0x3675, 0x85);
    ret |= sc8238_write_reg(dev, 0x3676, 0xa5);
    ret |= sc8238_write_reg(dev, 0x367a, 0x48);
    ret |= sc8238_write_reg(dev, 0x367b, 0x78);
    ret |= sc8238_write_reg(dev, 0x367c, 0x48);
    ret |= sc8238_write_reg(dev, 0x367d, 0x78);
    ret |= sc8238_write_reg(dev, 0x3690, 0x53);
    ret |= sc8238_write_reg(dev, 0x3691, 0x63);
    ret |= sc8238_write_reg(dev, 0x3692, 0x54);
    ret |= sc8238_write_reg(dev, 0x3699, 0x88);
    ret |= sc8238_write_reg(dev, 0x369a, 0x9f);
    ret |= sc8238_write_reg(dev, 0x369b, 0x9f);
    ret |= sc8238_write_reg(dev, 0x369c, 0x48);
    ret |= sc8238_write_reg(dev, 0x369d, 0x78);
    ret |= sc8238_write_reg(dev, 0x36a2, 0x48);
    ret |= sc8238_write_reg(dev, 0x36a3, 0x78);
    ret |= sc8238_write_reg(dev, 0x36bb, 0x48);
    ret |= sc8238_write_reg(dev, 0x36bc, 0x78);
    ret |= sc8238_write_reg(dev, 0x36c9, 0x05);
    ret |= sc8238_write_reg(dev, 0x36ca, 0x05);
    ret |= sc8238_write_reg(dev, 0x36cb, 0x05);
    ret |= sc8238_write_reg(dev, 0x36cc, 0x00);
    ret |= sc8238_write_reg(dev, 0x36cd, 0x10);
    ret |= sc8238_write_reg(dev, 0x36ce, 0x1a);
    ret |= sc8238_write_reg(dev, 0x36d0, 0x30);
    ret |= sc8238_write_reg(dev, 0x36d1, 0x48);
    ret |= sc8238_write_reg(dev, 0x36d2, 0x78);
    ret |= sc8238_write_reg(dev, 0x36ea, 0x36);
    ret |= sc8238_write_reg(dev, 0x36eb, 0x34);
    ret |= sc8238_write_reg(dev, 0x36ec, 0x15);
    ret |= sc8238_write_reg(dev, 0x36ed, 0x14);
    ret |= sc8238_write_reg(dev, 0x36fa, 0x36);
    ret |= sc8238_write_reg(dev, 0x36fb, 0x19);
    ret |= sc8238_write_reg(dev, 0x36fc, 0x11);
    ret |= sc8238_write_reg(dev, 0x36fd, 0x14);
    ret |= sc8238_write_reg(dev, 0x3802, 0x01);
    ret |= sc8238_write_reg(dev, 0x3901, 0x00);
    ret |= sc8238_write_reg(dev, 0x3902, 0xc5);
    ret |= sc8238_write_reg(dev, 0x3904, 0x18);
    ret |= sc8238_write_reg(dev, 0x3905, 0xd8);
    ret |= sc8238_write_reg(dev, 0x394c, 0x0f);
    ret |= sc8238_write_reg(dev, 0x394d, 0x20);
    ret |= sc8238_write_reg(dev, 0x394e, 0x08);
    ret |= sc8238_write_reg(dev, 0x394f, 0x90);
    ret |= sc8238_write_reg(dev, 0x3980, 0x71);
    ret |= sc8238_write_reg(dev, 0x3981, 0x70);
    ret |= sc8238_write_reg(dev, 0x3982, 0x00);
    ret |= sc8238_write_reg(dev, 0x3983, 0x00);
    ret |= sc8238_write_reg(dev, 0x3984, 0x20);
    ret |= sc8238_write_reg(dev, 0x3987, 0x0b);
    ret |= sc8238_write_reg(dev, 0x3990, 0x03);
    ret |= sc8238_write_reg(dev, 0x3991, 0xfd);
    ret |= sc8238_write_reg(dev, 0x3992, 0x03);
    ret |= sc8238_write_reg(dev, 0x3993, 0xfc);
    ret |= sc8238_write_reg(dev, 0x3994, 0x00);
    ret |= sc8238_write_reg(dev, 0x3995, 0x00);
    ret |= sc8238_write_reg(dev, 0x3996, 0x00);
    ret |= sc8238_write_reg(dev, 0x3997, 0x05);
    ret |= sc8238_write_reg(dev, 0x3998, 0x00);
    ret |= sc8238_write_reg(dev, 0x3999, 0x09);
    ret |= sc8238_write_reg(dev, 0x399a, 0x00);
    ret |= sc8238_write_reg(dev, 0x399b, 0x12);
    ret |= sc8238_write_reg(dev, 0x399c, 0x00);
    ret |= sc8238_write_reg(dev, 0x399d, 0x12);
    ret |= sc8238_write_reg(dev, 0x399e, 0x00);
    ret |= sc8238_write_reg(dev, 0x399f, 0x18);
    ret |= sc8238_write_reg(dev, 0x39a0, 0x00);
    ret |= sc8238_write_reg(dev, 0x39a1, 0x14);
    ret |= sc8238_write_reg(dev, 0x39a2, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a3, 0xe3);
    ret |= sc8238_write_reg(dev, 0x39a4, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a5, 0xf2);
    ret |= sc8238_write_reg(dev, 0x39a6, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a7, 0xf6);
    ret |= sc8238_write_reg(dev, 0x39a8, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a9, 0xfa);
    ret |= sc8238_write_reg(dev, 0x39aa, 0x03);
    ret |= sc8238_write_reg(dev, 0x39ab, 0xff);
    ret |= sc8238_write_reg(dev, 0x39ac, 0x00);
    ret |= sc8238_write_reg(dev, 0x39ad, 0x06);
    ret |= sc8238_write_reg(dev, 0x39ae, 0x00);
    ret |= sc8238_write_reg(dev, 0x39af, 0x09);
    ret |= sc8238_write_reg(dev, 0x39b0, 0x00);
    ret |= sc8238_write_reg(dev, 0x39b1, 0x12);
    ret |= sc8238_write_reg(dev, 0x39b2, 0x00);
    ret |= sc8238_write_reg(dev, 0x39b3, 0x22);
    ret |= sc8238_write_reg(dev, 0x39b4, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39b5, 0x1c);
    ret |= sc8238_write_reg(dev, 0x39b6, 0x38);
    ret |= sc8238_write_reg(dev, 0x39b7, 0x5b);
    ret |= sc8238_write_reg(dev, 0x39b8, 0x50);
    ret |= sc8238_write_reg(dev, 0x39b9, 0x38);
    ret |= sc8238_write_reg(dev, 0x39ba, 0x20);
    ret |= sc8238_write_reg(dev, 0x39bb, 0x10);
    ret |= sc8238_write_reg(dev, 0x39bc, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39bd, 0x16);
    ret |= sc8238_write_reg(dev, 0x39be, 0x21);
    ret |= sc8238_write_reg(dev, 0x39bf, 0x36);
    ret |= sc8238_write_reg(dev, 0x39c0, 0x3b);
    ret |= sc8238_write_reg(dev, 0x39c1, 0x2a);
    ret |= sc8238_write_reg(dev, 0x39c2, 0x16);
    ret |= sc8238_write_reg(dev, 0x39c3, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39c5, 0x30);
    ret |= sc8238_write_reg(dev, 0x39c6, 0x07);
    ret |= sc8238_write_reg(dev, 0x39c7, 0xf8);
    ret |= sc8238_write_reg(dev, 0x39c9, 0x07);
    ret |= sc8238_write_reg(dev, 0x39ca, 0xf8);
    ret |= sc8238_write_reg(dev, 0x39cc, 0x00);
    ret |= sc8238_write_reg(dev, 0x39cd, 0x1b);
    ret |= sc8238_write_reg(dev, 0x39ce, 0x00);
    ret |= sc8238_write_reg(dev, 0x39cf, 0x00);
    ret |= sc8238_write_reg(dev, 0x39d0, 0x1b);
    ret |= sc8238_write_reg(dev, 0x39d1, 0x00);
    ret |= sc8238_write_reg(dev, 0x39e2, 0x15);
    ret |= sc8238_write_reg(dev, 0x39e3, 0x87);
    ret |= sc8238_write_reg(dev, 0x39e4, 0x12);
    ret |= sc8238_write_reg(dev, 0x39e5, 0xb7);
    ret |= sc8238_write_reg(dev, 0x39e6, 0x00);
    ret |= sc8238_write_reg(dev, 0x39e7, 0x8c);
    ret |= sc8238_write_reg(dev, 0x39e8, 0x01);
    ret |= sc8238_write_reg(dev, 0x39e9, 0x31);
    ret |= sc8238_write_reg(dev, 0x39ea, 0x01);
    ret |= sc8238_write_reg(dev, 0x39eb, 0xd7);
    ret |= sc8238_write_reg(dev, 0x39ec, 0x08);
    ret |= sc8238_write_reg(dev, 0x39ed, 0x00);
    // ret |= sc8238_write_reg(dev, 0x3e00, 0x02);
    // ret |= sc8238_write_reg(dev, 0x3e01, 0x0c);
    // ret |= sc8238_write_reg(dev, 0x3e02, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e00, 0x01);
    ret |= sc8238_write_reg(dev, 0x3e01, 0xc0);
    ret |= sc8238_write_reg(dev, 0x3e02, 0x00);
    // ret |= sc8238_write_reg(dev, 0x3e04, 0x20);
    // ret |= sc8238_write_reg(dev, 0x3e05, 0xc0);
    ret |= sc8238_write_reg(dev, 0x3e04, 0x70);
    ret |= sc8238_write_reg(dev, 0x3e05, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e06, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e07, 0x80);
    ret |= sc8238_write_reg(dev, 0x3e10, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e11, 0x80);
    ret |= sc8238_write_reg(dev, 0x3e08, 0x07);
    ret |= sc8238_write_reg(dev, 0x3e09, 0x40);
    ret |= sc8238_write_reg(dev, 0x3e12, 0x07);
    ret |= sc8238_write_reg(dev, 0x3e13, 0x40);
    ret |= sc8238_write_reg(dev, 0x3e0e, 0x09);
    ret |= sc8238_write_reg(dev, 0x3e14, 0x31);
    ret |= sc8238_write_reg(dev, 0x3e16, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e17, 0xac);
    ret |= sc8238_write_reg(dev, 0x3e18, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e19, 0xac);
    ret |= sc8238_write_reg(dev, 0x3e1b, 0x3a);
    ret |= sc8238_write_reg(dev, 0x3e1e, 0x76);
    // ret |= sc8238_write_reg(dev, 0x3e23, 0x01);
    // ret |= sc8238_write_reg(dev, 0x3e24, 0x0e);
    ret |= sc8238_write_reg(dev, 0x3e23, 0x03);
    ret |= sc8238_write_reg(dev, 0x3e24, 0x88);
    ret |= sc8238_write_reg(dev, 0x3e25, 0x23);
    ret |= sc8238_write_reg(dev, 0x3e26, 0x40);
    ret |= sc8238_write_reg(dev, 0x4501, 0xa4);
    ret |= sc8238_write_reg(dev, 0x4509, 0x10);
    ret |= sc8238_write_reg(dev, 0x4816, 0x51);
    ret |= sc8238_write_reg(dev, 0x4837, 0xe5);
    ret |= sc8238_write_reg(dev, 0x5799, 0x06);
    ret |= sc8238_write_reg(dev, 0x57aa, 0x2f);
    ret |= sc8238_write_reg(dev, 0x57ab, 0xff);
    ret |= sc8238_write_reg(dev, 0x36e9, 0x44);
    ret |= sc8238_write_reg(dev, 0x36f9, 0x40);
    ret |= sc8238_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

/*
 * 函数功能: sc8238 MIPI 4lanes 720p wdr初始化序列 - 1280x720x10fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc8238_4lane_wdr_1280x720_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc8238_write_reg(dev, 0x0103, 0x01);
    ret |= sc8238_write_reg(dev, 0x0100, 0x00);
    ret |= sc8238_write_reg(dev, 0x36e9, 0x80);
    ret |= sc8238_write_reg(dev, 0x36f9, 0x80);
    ret |= sc8238_write_reg(dev, 0x3018, 0x72);
    ret |= sc8238_write_reg(dev, 0x3019, 0x00);
    ret |= sc8238_write_reg(dev, 0x301f, 0xef);
    ret |= sc8238_write_reg(dev, 0x3031, 0x0a);
    ret |= sc8238_write_reg(dev, 0x3037, 0x20);
    ret |= sc8238_write_reg(dev, 0x3038, 0x44);
    ret |= sc8238_write_reg(dev, 0x3200, 0x05);
    ret |= sc8238_write_reg(dev, 0x3201, 0x00);
    ret |= sc8238_write_reg(dev, 0x3202, 0x02);
    ret |= sc8238_write_reg(dev, 0x3203, 0xde);
    ret |= sc8238_write_reg(dev, 0x3204, 0x0a);
    ret |= sc8238_write_reg(dev, 0x3205, 0x1f);
    ret |= sc8238_write_reg(dev, 0x3206, 0x05);
    ret |= sc8238_write_reg(dev, 0x3207, 0xb1);
    ret |= sc8238_write_reg(dev, 0x3208, 0x05);
    ret |= sc8238_write_reg(dev, 0x3209, 0x00);
    ret |= sc8238_write_reg(dev, 0x320a, 0x02);
    ret |= sc8238_write_reg(dev, 0x320b, 0xd0);
    ret |= sc8238_write_reg(dev, 0x320c, 0x04);
    ret |= sc8238_write_reg(dev, 0x320d, 0x2a);
    ret |= sc8238_write_reg(dev, 0x320e, 0x05);
    ret |= sc8238_write_reg(dev, 0x320f, 0xdc);
    ret |= sc8238_write_reg(dev, 0x3210, 0x00);
    ret |= sc8238_write_reg(dev, 0x3211, 0x10);
    ret |= sc8238_write_reg(dev, 0x3212, 0x00);
    ret |= sc8238_write_reg(dev, 0x3213, 0x02);
    ret |= sc8238_write_reg(dev, 0x3220, 0x50);
    ret |= sc8238_write_reg(dev, 0x3241, 0x00);
    ret |= sc8238_write_reg(dev, 0x3243, 0x03);
    ret |= sc8238_write_reg(dev, 0x3248, 0x04);
    ret |= sc8238_write_reg(dev, 0x3250, 0x3f);
    ret |= sc8238_write_reg(dev, 0x3271, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3273, 0x1f);
    ret |= sc8238_write_reg(dev, 0x3301, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3302, 0x10);
    ret |= sc8238_write_reg(dev, 0x3304, 0x30);
    ret |= sc8238_write_reg(dev, 0x3306, 0x30);
    ret |= sc8238_write_reg(dev, 0x3308, 0x14);
    ret |= sc8238_write_reg(dev, 0x3309, 0x60);
    ret |= sc8238_write_reg(dev, 0x330a, 0x00);
    ret |= sc8238_write_reg(dev, 0x330b, 0x90);
    ret |= sc8238_write_reg(dev, 0x330c, 0x0a);
    ret |= sc8238_write_reg(dev, 0x330d, 0x20);
    ret |= sc8238_write_reg(dev, 0x330e, 0x58);
    ret |= sc8238_write_reg(dev, 0x3314, 0x98);
    ret |= sc8238_write_reg(dev, 0x331e, 0x21);
    ret |= sc8238_write_reg(dev, 0x331f, 0x51);
    ret |= sc8238_write_reg(dev, 0x3332, 0x1e);
    ret |= sc8238_write_reg(dev, 0x334a, 0x18);
    ret |= sc8238_write_reg(dev, 0x334c, 0x10);
    ret |= sc8238_write_reg(dev, 0x3350, 0x1e);
    ret |= sc8238_write_reg(dev, 0x3358, 0x1e);
    ret |= sc8238_write_reg(dev, 0x335c, 0x1e);
    ret |= sc8238_write_reg(dev, 0x335d, 0x60);
    ret |= sc8238_write_reg(dev, 0x3366, 0x4a);
    ret |= sc8238_write_reg(dev, 0x3367, 0x08);
    ret |= sc8238_write_reg(dev, 0x3368, 0x07);
    ret |= sc8238_write_reg(dev, 0x3369, 0x00);
    ret |= sc8238_write_reg(dev, 0x336a, 0x00);
    ret |= sc8238_write_reg(dev, 0x336b, 0x00);
    ret |= sc8238_write_reg(dev, 0x336c, 0xc2);
    ret |= sc8238_write_reg(dev, 0x336f, 0x58);
    ret |= sc8238_write_reg(dev, 0x337f, 0x33);
    ret |= sc8238_write_reg(dev, 0x339e, 0x1e);
    ret |= sc8238_write_reg(dev, 0x33a0, 0x1e);
    ret |= sc8238_write_reg(dev, 0x33a4, 0x1e);
    ret |= sc8238_write_reg(dev, 0x33aa, 0x1e);
    ret |= sc8238_write_reg(dev, 0x33af, 0x48);
    ret |= sc8238_write_reg(dev, 0x33b9, 0x0e);
    ret |= sc8238_write_reg(dev, 0x33e1, 0x08);
    ret |= sc8238_write_reg(dev, 0x33e2, 0x18);
    ret |= sc8238_write_reg(dev, 0x33e3, 0x10);
    ret |= sc8238_write_reg(dev, 0x33e4, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33e5, 0x10);
    ret |= sc8238_write_reg(dev, 0x33e6, 0x06);
    ret |= sc8238_write_reg(dev, 0x33e7, 0x02);
    ret |= sc8238_write_reg(dev, 0x33e8, 0x18);
    ret |= sc8238_write_reg(dev, 0x33e9, 0x10);
    ret |= sc8238_write_reg(dev, 0x33ea, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33eb, 0x10);
    ret |= sc8238_write_reg(dev, 0x33ec, 0x04);
    ret |= sc8238_write_reg(dev, 0x33ed, 0x02);
    ret |= sc8238_write_reg(dev, 0x33ee, 0xa0);
    ret |= sc8238_write_reg(dev, 0x33ef, 0x08);
    ret |= sc8238_write_reg(dev, 0x33f4, 0x18);
    ret |= sc8238_write_reg(dev, 0x33f5, 0x10);
    ret |= sc8238_write_reg(dev, 0x33f6, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33f7, 0x10);
    ret |= sc8238_write_reg(dev, 0x33f8, 0x06);
    ret |= sc8238_write_reg(dev, 0x33f9, 0x02);
    ret |= sc8238_write_reg(dev, 0x33fa, 0x18);
    ret |= sc8238_write_reg(dev, 0x33fb, 0x10);
    ret |= sc8238_write_reg(dev, 0x33fc, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33fd, 0x10);
    ret |= sc8238_write_reg(dev, 0x33fe, 0x04);
    ret |= sc8238_write_reg(dev, 0x33ff, 0x02);
    ret |= sc8238_write_reg(dev, 0x360f, 0x01);
    ret |= sc8238_write_reg(dev, 0x3622, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3624, 0x45);
    ret |= sc8238_write_reg(dev, 0x3625, 0x0a);
    ret |= sc8238_write_reg(dev, 0x3628, 0x83);
    ret |= sc8238_write_reg(dev, 0x3630, 0x80);
    ret |= sc8238_write_reg(dev, 0x3631, 0x80);
    ret |= sc8238_write_reg(dev, 0x3632, 0xa8);
    ret |= sc8238_write_reg(dev, 0x3633, 0x53);
    ret |= sc8238_write_reg(dev, 0x3635, 0x02);
    ret |= sc8238_write_reg(dev, 0x3636, 0x11);
    ret |= sc8238_write_reg(dev, 0x3637, 0x10);
    ret |= sc8238_write_reg(dev, 0x3638, 0x0a);
    ret |= sc8238_write_reg(dev, 0x363a, 0x88);
    ret |= sc8238_write_reg(dev, 0x363b, 0x06);
    ret |= sc8238_write_reg(dev, 0x363d, 0x01);
    ret |= sc8238_write_reg(dev, 0x363e, 0x00);
    ret |= sc8238_write_reg(dev, 0x3641, 0x00);
    ret |= sc8238_write_reg(dev, 0x3670, 0x4a);
    ret |= sc8238_write_reg(dev, 0x3671, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3672, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3673, 0x17);
    ret |= sc8238_write_reg(dev, 0x3674, 0x80);
    ret |= sc8238_write_reg(dev, 0x3675, 0x85);
    ret |= sc8238_write_reg(dev, 0x3676, 0xa5);
    ret |= sc8238_write_reg(dev, 0x367a, 0x48);
    ret |= sc8238_write_reg(dev, 0x367b, 0x78);
    ret |= sc8238_write_reg(dev, 0x367c, 0x48);
    ret |= sc8238_write_reg(dev, 0x367d, 0x78);
    ret |= sc8238_write_reg(dev, 0x3690, 0x53);
    ret |= sc8238_write_reg(dev, 0x3691, 0x63);
    ret |= sc8238_write_reg(dev, 0x3692, 0x54);
    ret |= sc8238_write_reg(dev, 0x3699, 0x88);
    ret |= sc8238_write_reg(dev, 0x369a, 0x9f);
    ret |= sc8238_write_reg(dev, 0x369b, 0x9f);
    ret |= sc8238_write_reg(dev, 0x369c, 0x48);
    ret |= sc8238_write_reg(dev, 0x369d, 0x78);
    ret |= sc8238_write_reg(dev, 0x36a2, 0x48);
    ret |= sc8238_write_reg(dev, 0x36a3, 0x78);
    ret |= sc8238_write_reg(dev, 0x36bb, 0x48);
    ret |= sc8238_write_reg(dev, 0x36bc, 0x78);
    ret |= sc8238_write_reg(dev, 0x36c9, 0x01);
    ret |= sc8238_write_reg(dev, 0x36ca, 0x20);
    ret |= sc8238_write_reg(dev, 0x36cb, 0x20);
    ret |= sc8238_write_reg(dev, 0x36cc, 0x00);
    ret |= sc8238_write_reg(dev, 0x36cd, 0x10);
    ret |= sc8238_write_reg(dev, 0x36ce, 0x1a);
    ret |= sc8238_write_reg(dev, 0x36d0, 0x30);
    ret |= sc8238_write_reg(dev, 0x36d1, 0x48);
    ret |= sc8238_write_reg(dev, 0x36d2, 0x78);
    ret |= sc8238_write_reg(dev, 0x36ea, 0x36);
    ret |= sc8238_write_reg(dev, 0x36eb, 0x34);
    ret |= sc8238_write_reg(dev, 0x36ec, 0x15);
    ret |= sc8238_write_reg(dev, 0x36ed, 0x14);
    ret |= sc8238_write_reg(dev, 0x36fa, 0x36);
    ret |= sc8238_write_reg(dev, 0x36fb, 0x19);
    ret |= sc8238_write_reg(dev, 0x36fc, 0x11);
    ret |= sc8238_write_reg(dev, 0x36fd, 0x14);
    ret |= sc8238_write_reg(dev, 0x3802, 0x01);
    ret |= sc8238_write_reg(dev, 0x3901, 0x00);
    ret |= sc8238_write_reg(dev, 0x3902, 0xc5);
    ret |= sc8238_write_reg(dev, 0x3904, 0x18);
    ret |= sc8238_write_reg(dev, 0x3905, 0xd8);
    ret |= sc8238_write_reg(dev, 0x394c, 0x0f);
    ret |= sc8238_write_reg(dev, 0x394d, 0x20);
    ret |= sc8238_write_reg(dev, 0x394e, 0x08);
    ret |= sc8238_write_reg(dev, 0x394f, 0x90);
    ret |= sc8238_write_reg(dev, 0x3980, 0x71);
    ret |= sc8238_write_reg(dev, 0x3981, 0x70);
    ret |= sc8238_write_reg(dev, 0x3982, 0x00);
    ret |= sc8238_write_reg(dev, 0x3983, 0x00);
    ret |= sc8238_write_reg(dev, 0x3984, 0x20);
    ret |= sc8238_write_reg(dev, 0x3987, 0x0b);
    ret |= sc8238_write_reg(dev, 0x3990, 0x03);
    ret |= sc8238_write_reg(dev, 0x3991, 0xfd);
    ret |= sc8238_write_reg(dev, 0x3992, 0x03);
    ret |= sc8238_write_reg(dev, 0x3993, 0xfc);
    ret |= sc8238_write_reg(dev, 0x3994, 0x00);
    ret |= sc8238_write_reg(dev, 0x3995, 0x00);
    ret |= sc8238_write_reg(dev, 0x3996, 0x00);
    ret |= sc8238_write_reg(dev, 0x3997, 0x05);
    ret |= sc8238_write_reg(dev, 0x3998, 0x00);
    ret |= sc8238_write_reg(dev, 0x3999, 0x09);
    ret |= sc8238_write_reg(dev, 0x399a, 0x00);
    ret |= sc8238_write_reg(dev, 0x399b, 0x12);
    ret |= sc8238_write_reg(dev, 0x399c, 0x00);
    ret |= sc8238_write_reg(dev, 0x399d, 0x12);
    ret |= sc8238_write_reg(dev, 0x399e, 0x00);
    ret |= sc8238_write_reg(dev, 0x399f, 0x18);
    ret |= sc8238_write_reg(dev, 0x39a0, 0x00);
    ret |= sc8238_write_reg(dev, 0x39a1, 0x14);
    ret |= sc8238_write_reg(dev, 0x39a2, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a3, 0xe3);
    ret |= sc8238_write_reg(dev, 0x39a4, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a5, 0xf2);
    ret |= sc8238_write_reg(dev, 0x39a6, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a7, 0xf6);
    ret |= sc8238_write_reg(dev, 0x39a8, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a9, 0xfa);
    ret |= sc8238_write_reg(dev, 0x39aa, 0x03);
    ret |= sc8238_write_reg(dev, 0x39ab, 0xff);
    ret |= sc8238_write_reg(dev, 0x39ac, 0x00);
    ret |= sc8238_write_reg(dev, 0x39ad, 0x06);
    ret |= sc8238_write_reg(dev, 0x39ae, 0x00);
    ret |= sc8238_write_reg(dev, 0x39af, 0x09);
    ret |= sc8238_write_reg(dev, 0x39b0, 0x00);
    ret |= sc8238_write_reg(dev, 0x39b1, 0x12);
    ret |= sc8238_write_reg(dev, 0x39b2, 0x00);
    ret |= sc8238_write_reg(dev, 0x39b3, 0x22);
    ret |= sc8238_write_reg(dev, 0x39b4, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39b5, 0x1c);
    ret |= sc8238_write_reg(dev, 0x39b6, 0x38);
    ret |= sc8238_write_reg(dev, 0x39b7, 0x5b);
    ret |= sc8238_write_reg(dev, 0x39b8, 0x50);
    ret |= sc8238_write_reg(dev, 0x39b9, 0x38);
    ret |= sc8238_write_reg(dev, 0x39ba, 0x20);
    ret |= sc8238_write_reg(dev, 0x39bb, 0x10);
    ret |= sc8238_write_reg(dev, 0x39bc, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39bd, 0x16);
    ret |= sc8238_write_reg(dev, 0x39be, 0x21);
    ret |= sc8238_write_reg(dev, 0x39bf, 0x36);
    ret |= sc8238_write_reg(dev, 0x39c0, 0x3b);
    ret |= sc8238_write_reg(dev, 0x39c1, 0x2a);
    ret |= sc8238_write_reg(dev, 0x39c2, 0x16);
    ret |= sc8238_write_reg(dev, 0x39c3, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39c5, 0x30);
    ret |= sc8238_write_reg(dev, 0x39c6, 0x07);
    ret |= sc8238_write_reg(dev, 0x39c7, 0xf8);
    ret |= sc8238_write_reg(dev, 0x39c9, 0x07);
    ret |= sc8238_write_reg(dev, 0x39ca, 0xf8);
    ret |= sc8238_write_reg(dev, 0x39cc, 0x00);
    ret |= sc8238_write_reg(dev, 0x39cd, 0x1b);
    ret |= sc8238_write_reg(dev, 0x39ce, 0x00);
    ret |= sc8238_write_reg(dev, 0x39cf, 0x00);
    ret |= sc8238_write_reg(dev, 0x39d0, 0x1b);
    ret |= sc8238_write_reg(dev, 0x39d1, 0x00);
    ret |= sc8238_write_reg(dev, 0x39e2, 0x15);
    ret |= sc8238_write_reg(dev, 0x39e3, 0x87);
    ret |= sc8238_write_reg(dev, 0x39e4, 0x12);
    ret |= sc8238_write_reg(dev, 0x39e5, 0xb7);
    ret |= sc8238_write_reg(dev, 0x39e6, 0x00);
    ret |= sc8238_write_reg(dev, 0x39e7, 0x8c);
    ret |= sc8238_write_reg(dev, 0x39e8, 0x01);
    ret |= sc8238_write_reg(dev, 0x39e9, 0x31);
    ret |= sc8238_write_reg(dev, 0x39ea, 0x01);
    ret |= sc8238_write_reg(dev, 0x39eb, 0xd7);
    ret |= sc8238_write_reg(dev, 0x39ec, 0x08);
    ret |= sc8238_write_reg(dev, 0x39ed, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e00, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e01, 0xac);
    ret |= sc8238_write_reg(dev, 0x3e02, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e04, 0x0a);
    ret |= sc8238_write_reg(dev, 0x3e05, 0xc0);
    ret |= sc8238_write_reg(dev, 0x3e08, 0x03);
    ret |= sc8238_write_reg(dev, 0x3e09, 0x40);
    ret |= sc8238_write_reg(dev, 0x3e0e, 0x09);
    ret |= sc8238_write_reg(dev, 0x3e14, 0x31);
    ret |= sc8238_write_reg(dev, 0x3e16, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e17, 0xac);
    ret |= sc8238_write_reg(dev, 0x3e18, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e19, 0xac);
    ret |= sc8238_write_reg(dev, 0x3e1b, 0x3a);
    ret |= sc8238_write_reg(dev, 0x3e1e, 0x76);
    ret |= sc8238_write_reg(dev, 0x3e23, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e24, 0x5e);
    ret |= sc8238_write_reg(dev, 0x3e25, 0x23);
    ret |= sc8238_write_reg(dev, 0x3e26, 0x40);
    ret |= sc8238_write_reg(dev, 0x4501, 0xa4);
    ret |= sc8238_write_reg(dev, 0x4509, 0x10);
    ret |= sc8238_write_reg(dev, 0x4816, 0x51);
    ret |= sc8238_write_reg(dev, 0x4837, 0xe5);
    ret |= sc8238_write_reg(dev, 0x5799, 0x06);
    ret |= sc8238_write_reg(dev, 0x57aa, 0x2f);
    ret |= sc8238_write_reg(dev, 0x57ab, 0xff);
    ret |= sc8238_write_reg(dev, 0x36e9, 0x44);
    ret |= sc8238_write_reg(dev, 0x36f9, 0x40);
    ret |= sc8238_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=================================================================================\n");
    SENSOR_PRINT("=== SC8235_MIPI_24MInput_10bit_4lane_1280x720_10.006fps init success!============\n");
    SENSOR_PRINT("=================================================================================\n");
    return XMEDIA_SUCCESS;
}
#else
#define MAX_FPS_WDR 15.0
static xmedia_s32 sc8238_wdr_comm_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc8238_write_reg(dev, 0x36e9, 0x80);
    ret |= sc8238_write_reg(dev, 0x36f9, 0x80);
    ret |= sc8238_write_reg(dev, 0x3018, 0x72);
    ret |= sc8238_write_reg(dev, 0x3019, 0x00);
    ret |= sc8238_write_reg(dev, 0x301f, 0xbb);
    ret |= sc8238_write_reg(dev, 0x3031, 0x0a);
    ret |= sc8238_write_reg(dev, 0x3037, 0x20);
    ret |= sc8238_write_reg(dev, 0x3038, 0x44);
    ret |= sc8238_write_reg(dev, 0x320c, 0x08);
    ret |= sc8238_write_reg(dev, 0x320d, 0x56);
    ret |= sc8238_write_reg(dev, 0x320e, 0x11);
    ret |= sc8238_write_reg(dev, 0x320f, 0x94);
    ret |= sc8238_write_reg(dev, 0x3220, 0x50);
    ret |= sc8238_write_reg(dev, 0x3241, 0x00);
    ret |= sc8238_write_reg(dev, 0x3243, 0x03);
    ret |= sc8238_write_reg(dev, 0x3248, 0x04);
    ret |= sc8238_write_reg(dev, 0x3250, 0x3f);
    ret |= sc8238_write_reg(dev, 0x3271, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3273, 0x1f);
    ret |= sc8238_write_reg(dev, 0x3301, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3306, 0xa8);
    ret |= sc8238_write_reg(dev, 0x3308, 0x20);
    ret |= sc8238_write_reg(dev, 0x3309, 0x68);
    ret |= sc8238_write_reg(dev, 0x330b, 0x48);
    ret |= sc8238_write_reg(dev, 0x330d, 0x28);
    ret |= sc8238_write_reg(dev, 0x330e, 0x58);
    ret |= sc8238_write_reg(dev, 0x3314, 0x98);
    ret |= sc8238_write_reg(dev, 0x331f, 0x59);
    ret |= sc8238_write_reg(dev, 0x3332, 0x24);
    ret |= sc8238_write_reg(dev, 0x334a, 0x18);
    ret |= sc8238_write_reg(dev, 0x334c, 0x10);
    ret |= sc8238_write_reg(dev, 0x3350, 0x24);
    ret |= sc8238_write_reg(dev, 0x3358, 0x24);
    ret |= sc8238_write_reg(dev, 0x335c, 0x24);
    ret |= sc8238_write_reg(dev, 0x335d, 0x60);
    ret |= sc8238_write_reg(dev, 0x3364, 0x16);
    ret |= sc8238_write_reg(dev, 0x3366, 0x92);
    ret |= sc8238_write_reg(dev, 0x3367, 0x08);
    ret |= sc8238_write_reg(dev, 0x3368, 0x07);
    ret |= sc8238_write_reg(dev, 0x3369, 0x00);
    ret |= sc8238_write_reg(dev, 0x336a, 0x00);
    ret |= sc8238_write_reg(dev, 0x336b, 0x00);
    ret |= sc8238_write_reg(dev, 0x336c, 0xc2);
    ret |= sc8238_write_reg(dev, 0x336f, 0x58);
    ret |= sc8238_write_reg(dev, 0x337f, 0x33);
    ret |= sc8238_write_reg(dev, 0x3390, 0x08);
    ret |= sc8238_write_reg(dev, 0x3391, 0x18);
    ret |= sc8238_write_reg(dev, 0x3392, 0x38);
    ret |= sc8238_write_reg(dev, 0x3393, 0x1c);
    ret |= sc8238_write_reg(dev, 0x3394, 0x28);
    ret |= sc8238_write_reg(dev, 0x3395, 0x60);
    ret |= sc8238_write_reg(dev, 0x3396, 0x08);
    ret |= sc8238_write_reg(dev, 0x3397, 0x18);
    ret |= sc8238_write_reg(dev, 0x3398, 0x38);
    ret |= sc8238_write_reg(dev, 0x3399, 0x1c);
    ret |= sc8238_write_reg(dev, 0x339a, 0x1c);
    ret |= sc8238_write_reg(dev, 0x339b, 0x28);
    ret |= sc8238_write_reg(dev, 0x339c, 0x60);
    ret |= sc8238_write_reg(dev, 0x339e, 0x24);
    ret |= sc8238_write_reg(dev, 0x33aa, 0x24);
    ret |= sc8238_write_reg(dev, 0x33af, 0x48);
    ret |= sc8238_write_reg(dev, 0x33e1, 0x08);
    ret |= sc8238_write_reg(dev, 0x33e2, 0x18);
    ret |= sc8238_write_reg(dev, 0x33e3, 0x10);
    ret |= sc8238_write_reg(dev, 0x33e4, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33e5, 0x10);
    ret |= sc8238_write_reg(dev, 0x33e6, 0x06);
    ret |= sc8238_write_reg(dev, 0x33e7, 0x02);
    ret |= sc8238_write_reg(dev, 0x33e8, 0x18);
    ret |= sc8238_write_reg(dev, 0x33e9, 0x10);
    ret |= sc8238_write_reg(dev, 0x33ea, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33eb, 0x10);
    ret |= sc8238_write_reg(dev, 0x33ec, 0x04);
    ret |= sc8238_write_reg(dev, 0x33ed, 0x02);
    ret |= sc8238_write_reg(dev, 0x33ee, 0xa0);
    ret |= sc8238_write_reg(dev, 0x33ef, 0x08);
    ret |= sc8238_write_reg(dev, 0x33f4, 0x18);
    ret |= sc8238_write_reg(dev, 0x33f5, 0x10);
    ret |= sc8238_write_reg(dev, 0x33f6, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33f7, 0x10);
    ret |= sc8238_write_reg(dev, 0x33f8, 0x06);
    ret |= sc8238_write_reg(dev, 0x33f9, 0x02);
    ret |= sc8238_write_reg(dev, 0x33fa, 0x18);
    ret |= sc8238_write_reg(dev, 0x33fb, 0x10);
    ret |= sc8238_write_reg(dev, 0x33fc, 0x0c);
    ret |= sc8238_write_reg(dev, 0x33fd, 0x10);
    ret |= sc8238_write_reg(dev, 0x33fe, 0x04);
    ret |= sc8238_write_reg(dev, 0x33ff, 0x02);
    ret |= sc8238_write_reg(dev, 0x360f, 0x01);
    ret |= sc8238_write_reg(dev, 0x3622, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3624, 0x45);
    ret |= sc8238_write_reg(dev, 0x3625, 0x0a);
    ret |= sc8238_write_reg(dev, 0x3628, 0x83);
    ret |= sc8238_write_reg(dev, 0x3630, 0x80);
    ret |= sc8238_write_reg(dev, 0x3631, 0x80);
    ret |= sc8238_write_reg(dev, 0x3632, 0xa8);
    ret |= sc8238_write_reg(dev, 0x3633, 0x53);
    ret |= sc8238_write_reg(dev, 0x3635, 0x02);
    ret |= sc8238_write_reg(dev, 0x3636, 0x11);
    ret |= sc8238_write_reg(dev, 0x3637, 0x10);
    ret |= sc8238_write_reg(dev, 0x3638, 0x0a);
    ret |= sc8238_write_reg(dev, 0x363a, 0x88);
    ret |= sc8238_write_reg(dev, 0x363b, 0x06);
    ret |= sc8238_write_reg(dev, 0x363d, 0x01);
    ret |= sc8238_write_reg(dev, 0x363e, 0x00);
    ret |= sc8238_write_reg(dev, 0x3641, 0x00);
    ret |= sc8238_write_reg(dev, 0x3670, 0x4a);
    ret |= sc8238_write_reg(dev, 0x3671, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3672, 0xf7);
    ret |= sc8238_write_reg(dev, 0x3673, 0x17);
    ret |= sc8238_write_reg(dev, 0x3674, 0x80);
    ret |= sc8238_write_reg(dev, 0x3675, 0x85);
    ret |= sc8238_write_reg(dev, 0x3676, 0xa5);
    ret |= sc8238_write_reg(dev, 0x367a, 0x48);
    ret |= sc8238_write_reg(dev, 0x367b, 0x78);
    ret |= sc8238_write_reg(dev, 0x367c, 0x48);
    ret |= sc8238_write_reg(dev, 0x367d, 0x78);
    ret |= sc8238_write_reg(dev, 0x3690, 0x53);
    ret |= sc8238_write_reg(dev, 0x3691, 0x63);
    ret |= sc8238_write_reg(dev, 0x3692, 0x54);
    ret |= sc8238_write_reg(dev, 0x3699, 0x9f);
    ret |= sc8238_write_reg(dev, 0x369a, 0x9f);
    ret |= sc8238_write_reg(dev, 0x369b, 0x9f);
    ret |= sc8238_write_reg(dev, 0x369c, 0x48);
    ret |= sc8238_write_reg(dev, 0x369d, 0x78);
    ret |= sc8238_write_reg(dev, 0x36a2, 0x48);
    ret |= sc8238_write_reg(dev, 0x36a3, 0x78);
    ret |= sc8238_write_reg(dev, 0x36bb, 0x48);
    ret |= sc8238_write_reg(dev, 0x36bc, 0x78);
    ret |= sc8238_write_reg(dev, 0x36c9, 0x05);
    ret |= sc8238_write_reg(dev, 0x36ca, 0x05);
    ret |= sc8238_write_reg(dev, 0x36cb, 0x05);
    ret |= sc8238_write_reg(dev, 0x36cc, 0x00);
    ret |= sc8238_write_reg(dev, 0x36cd, 0x10);
    ret |= sc8238_write_reg(dev, 0x36ce, 0x1a);
    ret |= sc8238_write_reg(dev, 0x36d0, 0x30);
    ret |= sc8238_write_reg(dev, 0x36d1, 0x48);
    ret |= sc8238_write_reg(dev, 0x36d2, 0x78);
    ret |= sc8238_write_reg(dev, 0x36ea, 0x3b);
    ret |= sc8238_write_reg(dev, 0x36eb, 0x06);
    ret |= sc8238_write_reg(dev, 0x36ec, 0x05);
    ret |= sc8238_write_reg(dev, 0x36ed, 0x14);
    ret |= sc8238_write_reg(dev, 0x36fa, 0x30);
    ret |= sc8238_write_reg(dev, 0x36fb, 0x13);
    ret |= sc8238_write_reg(dev, 0x36fc, 0x10);
    ret |= sc8238_write_reg(dev, 0x36fd, 0x34);
    ret |= sc8238_write_reg(dev, 0x3802, 0x01);
    ret |= sc8238_write_reg(dev, 0x3901, 0x00);
    ret |= sc8238_write_reg(dev, 0x3902, 0xc5);
    ret |= sc8238_write_reg(dev, 0x3904, 0x18);
    ret |= sc8238_write_reg(dev, 0x3905, 0xd8);
    ret |= sc8238_write_reg(dev, 0x394c, 0x0f);
    ret |= sc8238_write_reg(dev, 0x394d, 0x20);
    ret |= sc8238_write_reg(dev, 0x394e, 0x08);
    ret |= sc8238_write_reg(dev, 0x394f, 0x90);
    ret |= sc8238_write_reg(dev, 0x3980, 0x71);
    ret |= sc8238_write_reg(dev, 0x3981, 0x70);
    ret |= sc8238_write_reg(dev, 0x3982, 0x00);
    ret |= sc8238_write_reg(dev, 0x3983, 0x00);
    ret |= sc8238_write_reg(dev, 0x3984, 0x20);
    ret |= sc8238_write_reg(dev, 0x3987, 0x0b);
    ret |= sc8238_write_reg(dev, 0x3990, 0x03);
    ret |= sc8238_write_reg(dev, 0x3991, 0xfd);
    ret |= sc8238_write_reg(dev, 0x3992, 0x03);
    ret |= sc8238_write_reg(dev, 0x3993, 0xfc);
    ret |= sc8238_write_reg(dev, 0x3994, 0x00);
    ret |= sc8238_write_reg(dev, 0x3995, 0x00);
    ret |= sc8238_write_reg(dev, 0x3996, 0x00);
    ret |= sc8238_write_reg(dev, 0x3997, 0x05);
    ret |= sc8238_write_reg(dev, 0x3998, 0x00);
    ret |= sc8238_write_reg(dev, 0x3999, 0x09);
    ret |= sc8238_write_reg(dev, 0x399a, 0x00);
    ret |= sc8238_write_reg(dev, 0x399b, 0x12);
    ret |= sc8238_write_reg(dev, 0x399c, 0x00);
    ret |= sc8238_write_reg(dev, 0x399d, 0x12);
    ret |= sc8238_write_reg(dev, 0x399e, 0x00);
    ret |= sc8238_write_reg(dev, 0x399f, 0x18);
    ret |= sc8238_write_reg(dev, 0x39a0, 0x00);
    ret |= sc8238_write_reg(dev, 0x39a1, 0x14);
    ret |= sc8238_write_reg(dev, 0x39a2, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a3, 0xe3);
    ret |= sc8238_write_reg(dev, 0x39a4, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a5, 0xf2);
    ret |= sc8238_write_reg(dev, 0x39a6, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a7, 0xf6);
    ret |= sc8238_write_reg(dev, 0x39a8, 0x03);
    ret |= sc8238_write_reg(dev, 0x39a9, 0xfa);
    ret |= sc8238_write_reg(dev, 0x39aa, 0x03);
    ret |= sc8238_write_reg(dev, 0x39ab, 0xff);
    ret |= sc8238_write_reg(dev, 0x39ac, 0x00);
    ret |= sc8238_write_reg(dev, 0x39ad, 0x06);
    ret |= sc8238_write_reg(dev, 0x39ae, 0x00);
    ret |= sc8238_write_reg(dev, 0x39af, 0x09);
    ret |= sc8238_write_reg(dev, 0x39b0, 0x00);
    ret |= sc8238_write_reg(dev, 0x39b1, 0x12);
    ret |= sc8238_write_reg(dev, 0x39b2, 0x00);
    ret |= sc8238_write_reg(dev, 0x39b3, 0x22);
    ret |= sc8238_write_reg(dev, 0x39b4, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39b5, 0x1c);
    ret |= sc8238_write_reg(dev, 0x39b6, 0x38);
    ret |= sc8238_write_reg(dev, 0x39b7, 0x5b);
    ret |= sc8238_write_reg(dev, 0x39b8, 0x50);
    ret |= sc8238_write_reg(dev, 0x39b9, 0x38);
    ret |= sc8238_write_reg(dev, 0x39ba, 0x20);
    ret |= sc8238_write_reg(dev, 0x39bb, 0x10);
    ret |= sc8238_write_reg(dev, 0x39bc, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39bd, 0x16);
    ret |= sc8238_write_reg(dev, 0x39be, 0x21);
    ret |= sc8238_write_reg(dev, 0x39bf, 0x36);
    ret |= sc8238_write_reg(dev, 0x39c0, 0x3b);
    ret |= sc8238_write_reg(dev, 0x39c1, 0x2a);
    ret |= sc8238_write_reg(dev, 0x39c2, 0x16);
    ret |= sc8238_write_reg(dev, 0x39c3, 0x0c);
    ret |= sc8238_write_reg(dev, 0x39c5, 0x30);
    ret |= sc8238_write_reg(dev, 0x39c6, 0x07);
    ret |= sc8238_write_reg(dev, 0x39c7, 0xf8);
    ret |= sc8238_write_reg(dev, 0x39c9, 0x07);
    ret |= sc8238_write_reg(dev, 0x39ca, 0xf8);
    ret |= sc8238_write_reg(dev, 0x39cc, 0x00);
    ret |= sc8238_write_reg(dev, 0x39cd, 0x1b);
    ret |= sc8238_write_reg(dev, 0x39ce, 0x00);
    ret |= sc8238_write_reg(dev, 0x39cf, 0x00);
    ret |= sc8238_write_reg(dev, 0x39d0, 0x1b);
    ret |= sc8238_write_reg(dev, 0x39d1, 0x00);
    ret |= sc8238_write_reg(dev, 0x39e2, 0x15);
    ret |= sc8238_write_reg(dev, 0x39e3, 0x87);
    ret |= sc8238_write_reg(dev, 0x39e4, 0x12);
    ret |= sc8238_write_reg(dev, 0x39e5, 0xb7);
    ret |= sc8238_write_reg(dev, 0x39e6, 0x00);
    ret |= sc8238_write_reg(dev, 0x39e7, 0x8c);
    ret |= sc8238_write_reg(dev, 0x39e8, 0x01);
    ret |= sc8238_write_reg(dev, 0x39e9, 0x31);
    ret |= sc8238_write_reg(dev, 0x39ea, 0x01);
    ret |= sc8238_write_reg(dev, 0x39eb, 0xd7);
    ret |= sc8238_write_reg(dev, 0x39ec, 0x08);
    ret |= sc8238_write_reg(dev, 0x39ed, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e00, 0x02);
    ret |= sc8238_write_reg(dev, 0x3e01, 0x0f);
    ret |= sc8238_write_reg(dev, 0x3e02, 0xa0);
    ret |= sc8238_write_reg(dev, 0x3e04, 0x20);
    ret |= sc8238_write_reg(dev, 0x3e05, 0xc0);
    ret |= sc8238_write_reg(dev, 0x3e06, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e07, 0x80);
    ret |= sc8238_write_reg(dev, 0x3e08, 0x03);
    ret |= sc8238_write_reg(dev, 0x3e09, 0x40);
    ret |= sc8238_write_reg(dev, 0x3e0e, 0x09);
    ret |= sc8238_write_reg(dev, 0x3e10, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e11, 0x80);
    ret |= sc8238_write_reg(dev, 0x3e12, 0x03);
    ret |= sc8238_write_reg(dev, 0x3e13, 0x40);
    ret |= sc8238_write_reg(dev, 0x3e14, 0x31);
    ret |= sc8238_write_reg(dev, 0x3e16, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e17, 0xac);
    ret |= sc8238_write_reg(dev, 0x3e18, 0x00);
    ret |= sc8238_write_reg(dev, 0x3e19, 0xac);
    ret |= sc8238_write_reg(dev, 0x3e1b, 0x3a);
    ret |= sc8238_write_reg(dev, 0x3e1e, 0x76);
    ret |= sc8238_write_reg(dev, 0x3e23, 0x01);
    ret |= sc8238_write_reg(dev, 0x3e24, 0x0e);
    ret |= sc8238_write_reg(dev, 0x3e25, 0x23);
    ret |= sc8238_write_reg(dev, 0x3e26, 0x40);
    ret |= sc8238_write_reg(dev, 0x4501, 0xa4);
    ret |= sc8238_write_reg(dev, 0x4509, 0x10);
    ret |= sc8238_write_reg(dev, 0x4800, 0x64);
    ret |= sc8238_write_reg(dev, 0x4816, 0x51);
    ret |= sc8238_write_reg(dev, 0x4837, 0x1c);
    ret |= sc8238_write_reg(dev, 0x5799, 0x06);
    ret |= sc8238_write_reg(dev, 0x57aa, 0x2f);
    ret |= sc8238_write_reg(dev, 0x57ab, 0xff);
    ret |= sc8238_write_reg(dev, 0x5988, 0x86);
    ret |= sc8238_write_reg(dev, 0x598e, 0x10);
    ret |= sc8238_write_reg(dev, 0x598f, 0x44);
    ret |= sc8238_write_reg(dev, 0x5a88, 0x86);
    ret |= sc8238_write_reg(dev, 0x5a8e, 0x10);
    ret |= sc8238_write_reg(dev, 0x5a8f, 0x44);
    ret |= sc8238_write_reg(dev, 0x36e9, 0x23);
    ret |= sc8238_write_reg(dev, 0x36f9, 0x20);
    ret |= sc8238_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
            SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
            return XMEDIA_FAILURE;
        }

    return XMEDIA_SUCCESS;
}

/*
 * 函数功能: sc8238 MIPI 4lanes 720p wdr初始化序列 - 1280x720
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 sc8238_4lane_wdr_1280x720_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc8238_write_reg(dev, 0x0103, 0x01);
    ret |= sc8238_write_reg(dev, 0x0100, 0x00);
    ret |= sc8238_write_reg(dev, 0x3208, 0x05);
    ret |= sc8238_write_reg(dev, 0x3209, 0x00);
    ret |= sc8238_write_reg(dev, 0x320a, 0x02);
    ret |= sc8238_write_reg(dev, 0x320b, 0xd0);
    ret |= sc8238_wdr_comm_init(dev);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=================================================================================\n");
    SENSOR_PRINT("=== SC8235_MIPI_24MInput_10bit_4lane_2to1wdr_1280x720_%.3ffps init success!======\n", MAX_FPS_WDR);
    SENSOR_PRINT("=================================================================================\n");
    return XMEDIA_SUCCESS;
}
#endif

static xmedia_s32 sc8238_4lane_wdr_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc8238_write_reg(dev, 0x0103, 0x01);
    ret |= sc8238_write_reg(dev, 0x0100, 0x00);
    ret |= sc8238_write_reg(dev, 0x3208, 0x07);
    ret |= sc8238_write_reg(dev, 0x3209, 0x80);
    ret |= sc8238_write_reg(dev, 0x320a, 0x04);
    ret |= sc8238_write_reg(dev, 0x320b, 0x38);
    ret |= sc8238_wdr_comm_init(dev);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("================================================================================\n");
        SENSOR_PRINT("=== SC8238_MIPI_24MInput_raw10_4lane_2to1wdr_1920X1080_%.3ffps init success! ===\n", MAX_FPS_WDR);
        SENSOR_PRINT("================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

static xmedia_s32 sc8238_4lane_wdr_2048x1536_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    ret |= sc8238_write_reg(dev, 0x0103, 0x01);
    ret |= sc8238_write_reg(dev, 0x0100, 0x00);
    ret |= sc8238_write_reg(dev, 0x3208, 0x08);
    ret |= sc8238_write_reg(dev, 0x3209, 0x00);
    ret |= sc8238_write_reg(dev, 0x320a, 0x06);
    ret |= sc8238_write_reg(dev, 0x320b, 0x00);
    ret |= sc8238_wdr_comm_init(dev);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("================================================================================\n");
        SENSOR_PRINT("===  SC8238_MIPI_24MInput_raw10_4lane_2to1wdr_2048x1536_%.3ffps init success!===\n", MAX_FPS_WDR);
        SENSOR_PRINT("================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

static xmedia_s32 sc8238_4lane_wdr_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    ret |= sc8238_write_reg(dev, 0x0103, 0x01);
    ret |= sc8238_write_reg(dev, 0x0100, 0x00);
    ret |= sc8238_write_reg(dev, 0x3208, 0x0a);
    ret |= sc8238_write_reg(dev, 0x3209, 0x80);
    ret |= sc8238_write_reg(dev, 0x320a, 0x05);
    ret |= sc8238_write_reg(dev, 0x320b, 0xf0);
    ret |= sc8238_wdr_comm_init(dev);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("================================================================================\n");
        SENSOR_PRINT("=== SC8238_MIPI_24MInput_raw10_4lane_2to1wdr_2688X1520_%.3ffps init success!====\n", MAX_FPS_WDR);
        SENSOR_PRINT("================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

static xmedia_s32 sc8238_4lane_wdr_2560x1920_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc8238_write_reg(dev, 0x0103, 0x01);
    ret |= sc8238_write_reg(dev, 0x0100, 0x00);
    ret |= sc8238_write_reg(dev, 0x3208, 0x0a);
    ret |= sc8238_write_reg(dev, 0x3209, 0x00);
    ret |= sc8238_write_reg(dev, 0x320a, 0x07);
    ret |= sc8238_write_reg(dev, 0x320b, 0x80);
    ret |= sc8238_write_reg(dev, 0x320c, 0x08);
    ret |= sc8238_wdr_comm_init(dev);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("================================================================================\n");
        SENSOR_PRINT("===  SC8238_MIPI_24MInput_raw10_4lane_2to1wdr_2560X1920_%.3ffps init success! ==\n", MAX_FPS_WDR);
        SENSOR_PRINT("================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

static xmedia_s32 sc8238_4lane_wdr_3840x2160_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= sc8238_write_reg(dev, 0x0103, 0x01);
    ret |= sc8238_write_reg(dev, 0x0100, 0x00);
    ret |= sc8238_write_reg(dev, 0x3208, 0x0f);
    ret |= sc8238_write_reg(dev, 0x3209, 0x00);
    ret |= sc8238_write_reg(dev, 0x320a, 0x08);
    ret |= sc8238_write_reg(dev, 0x320b, 0x70);
    ret |= sc8238_wdr_comm_init(dev);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("================================================================================\n");
        SENSOR_PRINT("==== SC8238_MIPI_24MInput_raw10_4lane_2to1wdr_3840X2160_%.3ffps init success!===\n", MAX_FPS_WDR);
        SENSOR_PRINT("================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

xmedia_s32 sc8238_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case SC8238_1M_10BIT_LINEAR_MODE:
            ret = sc8238_4lane_linear_1280x720_init(dev);
            break;

        case SC8238_2M_10BIT_LINEAR_MODE:
            ret = sc8238_4lane_linear_1920x1080_init(dev);
            break;

        case SC8238_3M_10BIT_LINEAR_MODE:
            ret = sc8238_4lane_linear_2048x1536_init(dev);
            break;

        case SC8238_4M_10BIT_LINEAR_MODE:
            ret = sc8238_4lane_linear_2688x1520_init(dev);
            break;

        case SC8238_5M_10BIT_LINEAR_MODE:
            ret = sc8238_4lane_linear_2560x1920_init(dev);
            break;

        case SC8238_5M_10BIT_1728P_LINEAR_MODE:
            ret = sc8238_4lane_linear_3072x1728_init(dev);
            break;

        case SC8238_8M_10BIT_LINEAR_MODE:
            ret = sc8238_4lane_linear_3840x2160_init(dev);
            break;

        case SC8238_1M_10BIT_WDR_MODE:
            ret = sc8238_4lane_wdr_1280x720_init(dev);
            break;

        case SC8238_2M_10BIT_WDR_MODE:
            ret = sc8238_4lane_wdr_1920x1080_init(dev);
            break;

        case SC8238_3M_10BIT_WDR_MODE:
            ret = sc8238_4lane_wdr_2048x1536_init(dev);
            break;

        case SC8238_4M_10BIT_WDR_MODE:
            ret = sc8238_4lane_wdr_2688x1520_init(dev);
            break;

        case SC8238_5M_10BIT_WDR_MODE:
            ret = sc8238_4lane_wdr_2560x1920_init(dev);
            break;

        case SC8238_8M_10BIT_WDR_MODE:
            ret = sc8238_4lane_wdr_3840x2160_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc8238_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;

    if (g_sc8238_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_sc8238_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_sc8238_i2c_fd[dev]);
        g_sc8238_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_sc8238_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc8238_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_sc8238_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_sc8238_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_sc8238_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = sc8238_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc8238_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_sc8238_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_sc8238_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc8238_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[SC8238_DATA_BYTE];

    if (g_sc8238_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_sc8238_i2c_fd[dev], g_sc8238_i2c_addr[dev], addr, SC8238_ADDR_BYTE, buf, SC8238_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //SC8238_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 sc8238_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_sc8238_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_sc8238_i2c_fd[dev], g_sc8238_i2c_addr[dev], buf, SC8238_ADDR_BYTE + SC8238_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}
xmedia_s32 sc8238_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    sc8238_read_reg(dev, SC8238_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

    if (mirror_en) {
        mirror_flip_type &= 0xf9;
    } else {
        mirror_flip_type |= 0x06;
    }

    if (flip_en) {
        mirror_flip_type &= 0x9f;
    } else{
        mirror_flip_type |= 0x60;
    }

    ret = sc8238_write_reg(dev, SC8238_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sc8238_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
