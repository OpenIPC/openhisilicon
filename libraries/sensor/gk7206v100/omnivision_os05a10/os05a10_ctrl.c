#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "os05a10.h"
#include "os05a10_ctrl.h"

#define OS05A10_REG_ADDR_FLIP   0x3820
#define OS05A10_REG_ADDR_MIRROR 0x3821
static xmedia_s32 g_os05a10_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] =
                                                                     SENSOR_I2C_INVALID };
static xmedia_s32 g_os05a10_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] =
                                                                     SENSOR_I2C_INVALID };

 static void os05a10_delay_ms(int ms)
 {
     usleep(ms * 1000); // 1ms: 1000us
     return;
 }

/*
 * 函数功能: os05a10 MIPI 4lane 1944p线性模式初始化序列 - 5M-2688x1944
 * 函数参数：
 *      dev - sensor 设备号
 * 返回值：
 *
 */
#ifdef FPGA
static xmedia_s32 os05a10_linear_comm_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os05a10_write_reg(dev, 0x0103, 0x01);
    ret |= os05a10_write_reg(dev, 0x0303, 0x01);
    ret |= os05a10_write_reg(dev, 0x0305, 0x2b);
    ret |= os05a10_write_reg(dev, 0x0306, 0x00);
    ret |= os05a10_write_reg(dev, 0x0307, 0x00);
    ret |= os05a10_write_reg(dev, 0x0308, 0x03);
    ret |= os05a10_write_reg(dev, 0x0309, 0x04);
    ret |= os05a10_write_reg(dev, 0x030c, 0x01);
    ret |= os05a10_write_reg(dev, 0x0322, 0x01);
    ret |= os05a10_write_reg(dev, 0x032a, 0x00);
    ret |= os05a10_write_reg(dev, 0x031e, 0x09);
    ret |= os05a10_write_reg(dev, 0x0325, 0x48);
    ret |= os05a10_write_reg(dev, 0x0328, 0x07);
    ret |= os05a10_write_reg(dev, 0x300d, 0x11);
    ret |= os05a10_write_reg(dev, 0x300e, 0x11);
    ret |= os05a10_write_reg(dev, 0x300f, 0x11);
    ret |= os05a10_write_reg(dev, 0x3026, 0x00);
    ret |= os05a10_write_reg(dev, 0x3027, 0x00);
    ret |= os05a10_write_reg(dev, 0x3010, 0x01);
    ret |= os05a10_write_reg(dev, 0x3012, 0x41);
    ret |= os05a10_write_reg(dev, 0x3016, 0xf0);
    ret |= os05a10_write_reg(dev, 0x3018, 0xf0);
    ret |= os05a10_write_reg(dev, 0x3028, 0xf0);
    ret |= os05a10_write_reg(dev, 0x301e, 0x98);
    ret |= os05a10_write_reg(dev, 0x3010, 0x01);
    ret |= os05a10_write_reg(dev, 0x3011, 0x04);
    ret |= os05a10_write_reg(dev, 0x3031, 0xa9);
    ret |= os05a10_write_reg(dev, 0x3103, 0x48);
    ret |= os05a10_write_reg(dev, 0x3104, 0x01);
    ret |= os05a10_write_reg(dev, 0x3106, 0x10);
    ret |= os05a10_write_reg(dev, 0x3400, 0x04);
    ret |= os05a10_write_reg(dev, 0x3025, 0x03);
    ret |= os05a10_write_reg(dev, 0x3425, 0x01);
    ret |= os05a10_write_reg(dev, 0x3428, 0x01);
    ret |= os05a10_write_reg(dev, 0x3406, 0x08);
    ret |= os05a10_write_reg(dev, 0x3408, 0x03);
    ret |= os05a10_write_reg(dev, 0x3501, 0x09);
    ret |= os05a10_write_reg(dev, 0x3502, 0xa0);
    ret |= os05a10_write_reg(dev, 0x3505, 0x83);
    ret |= os05a10_write_reg(dev, 0x3508, 0x00);
    ret |= os05a10_write_reg(dev, 0x3509, 0x80);
    ret |= os05a10_write_reg(dev, 0x350a, 0x04);
    ret |= os05a10_write_reg(dev, 0x350b, 0x00);
    ret |= os05a10_write_reg(dev, 0x350c, 0x00);
    ret |= os05a10_write_reg(dev, 0x350d, 0x80);
    ret |= os05a10_write_reg(dev, 0x350e, 0x04);
    ret |= os05a10_write_reg(dev, 0x350f, 0x00);
    ret |= os05a10_write_reg(dev, 0x3600, 0x00);
    ret |= os05a10_write_reg(dev, 0x3626, 0xff);
    ret |= os05a10_write_reg(dev, 0x3605, 0x50);
    ret |= os05a10_write_reg(dev, 0x3609, 0xb5);
    ret |= os05a10_write_reg(dev, 0x3610, 0x69);
    ret |= os05a10_write_reg(dev, 0x360c, 0x01);
    ret |= os05a10_write_reg(dev, 0x3628, 0xa4);
    ret |= os05a10_write_reg(dev, 0x3629, 0x6a);
    ret |= os05a10_write_reg(dev, 0x362d, 0x10);
    ret |= os05a10_write_reg(dev, 0x3660, 0x43);
    ret |= os05a10_write_reg(dev, 0x3661, 0x06);
    ret |= os05a10_write_reg(dev, 0x3662, 0x00);
    ret |= os05a10_write_reg(dev, 0x3663, 0x28);
    ret |= os05a10_write_reg(dev, 0x3664, 0x0d);
    ret |= os05a10_write_reg(dev, 0x366a, 0x38);
    ret |= os05a10_write_reg(dev, 0x366b, 0xa0);
    ret |= os05a10_write_reg(dev, 0x366d, 0x00);
    ret |= os05a10_write_reg(dev, 0x366e, 0x00);
    ret |= os05a10_write_reg(dev, 0x3680, 0x00);
    ret |= os05a10_write_reg(dev, 0x36c0, 0x00);
    ret |= os05a10_write_reg(dev, 0x3621, 0x81);
    ret |= os05a10_write_reg(dev, 0x3634, 0x31);
    ret |= os05a10_write_reg(dev, 0x3620, 0x00);
    ret |= os05a10_write_reg(dev, 0x3622, 0x00);
    ret |= os05a10_write_reg(dev, 0x362a, 0xd0);
    ret |= os05a10_write_reg(dev, 0x362e, 0x8c);
    ret |= os05a10_write_reg(dev, 0x362f, 0x98);
    ret |= os05a10_write_reg(dev, 0x3630, 0xb0);
    ret |= os05a10_write_reg(dev, 0x3631, 0xd7);
    ret |= os05a10_write_reg(dev, 0x3701, 0x0f);
    ret |= os05a10_write_reg(dev, 0x3737, 0x02);
    ret |= os05a10_write_reg(dev, 0x3740, 0x18);
    ret |= os05a10_write_reg(dev, 0x3741, 0x04);
    ret |= os05a10_write_reg(dev, 0x373c, 0x0f);
    ret |= os05a10_write_reg(dev, 0x373b, 0x02);
    ret |= os05a10_write_reg(dev, 0x3705, 0x00);
    ret |= os05a10_write_reg(dev, 0x3706, 0x50);
    ret |= os05a10_write_reg(dev, 0x370a, 0x00);
    ret |= os05a10_write_reg(dev, 0x370b, 0xe4);
    ret |= os05a10_write_reg(dev, 0x3709, 0x4a);
    ret |= os05a10_write_reg(dev, 0x3714, 0x21);
    ret |= os05a10_write_reg(dev, 0x371c, 0x00);
    ret |= os05a10_write_reg(dev, 0x371d, 0x08);
    ret |= os05a10_write_reg(dev, 0x375e, 0x0e);
    ret |= os05a10_write_reg(dev, 0x3760, 0x13);
    ret |= os05a10_write_reg(dev, 0x3776, 0x10);
    ret |= os05a10_write_reg(dev, 0x3781, 0x02);
    ret |= os05a10_write_reg(dev, 0x3782, 0x04);
    ret |= os05a10_write_reg(dev, 0x3783, 0x02);
    ret |= os05a10_write_reg(dev, 0x3784, 0x08);
    ret |= os05a10_write_reg(dev, 0x3785, 0x08);
    ret |= os05a10_write_reg(dev, 0x3788, 0x01);
    ret |= os05a10_write_reg(dev, 0x3789, 0x01);
    ret |= os05a10_write_reg(dev, 0x3797, 0x04);
    ret |= os05a10_write_reg(dev, 0x3798, 0x01);
    ret |= os05a10_write_reg(dev, 0x3799, 0x00);
    ret |= os05a10_write_reg(dev, 0x3761, 0x02);
    ret |= os05a10_write_reg(dev, 0x3762, 0x0d);
    ret |= os05a10_write_reg(dev, 0x380c, 0x04);
    ret |= os05a10_write_reg(dev, 0x380d, 0xd0);
    ret |= os05a10_write_reg(dev, 0x380e, 0x09);
    ret |= os05a10_write_reg(dev, 0x380f, 0xc0);
    ret |= os05a10_write_reg(dev, 0x3811, 0x10);
    ret |= os05a10_write_reg(dev, 0x3813, 0x04);
    ret |= os05a10_write_reg(dev, 0x3814, 0x01);
    ret |= os05a10_write_reg(dev, 0x3815, 0x01);
    ret |= os05a10_write_reg(dev, 0x3816, 0x01);
    ret |= os05a10_write_reg(dev, 0x3817, 0x01);
    ret |= os05a10_write_reg(dev, 0x381c, 0x00);
    ret |= os05a10_write_reg(dev, 0x3820, 0x00);
    ret |= os05a10_write_reg(dev, 0x3821, 0x04);
    ret |= os05a10_write_reg(dev, 0x3822, 0x54);
    ret |= os05a10_write_reg(dev, 0x3823, 0x18);
    ret |= os05a10_write_reg(dev, 0x3826, 0x00);
    ret |= os05a10_write_reg(dev, 0x3827, 0x01);
    ret |= os05a10_write_reg(dev, 0x3833, 0x00);
    ret |= os05a10_write_reg(dev, 0x3832, 0x02);
    ret |= os05a10_write_reg(dev, 0x383c, 0x48);
    ret |= os05a10_write_reg(dev, 0x383d, 0xff);
    ret |= os05a10_write_reg(dev, 0x3843, 0x20);
    ret |= os05a10_write_reg(dev, 0x382d, 0x08);
    ret |= os05a10_write_reg(dev, 0x3d85, 0x0b);
    ret |= os05a10_write_reg(dev, 0x3d84, 0x40);
    ret |= os05a10_write_reg(dev, 0x3d8c, 0x63);
    ret |= os05a10_write_reg(dev, 0x3d8d, 0x00);
    ret |= os05a10_write_reg(dev, 0x4000, 0x78);
    ret |= os05a10_write_reg(dev, 0x4001, 0x2b);
    ret |= os05a10_write_reg(dev, 0x4004, 0x00);
    ret |= os05a10_write_reg(dev, 0x4005, 0x40);
    ret |= os05a10_write_reg(dev, 0x4028, 0x2f);
    ret |= os05a10_write_reg(dev, 0x400a, 0x01);
    ret |= os05a10_write_reg(dev, 0x4010, 0x12);
    ret |= os05a10_write_reg(dev, 0x4008, 0x02);
    ret |= os05a10_write_reg(dev, 0x4009, 0x0d);
    ret |= os05a10_write_reg(dev, 0x401a, 0x58);
    ret |= os05a10_write_reg(dev, 0x4050, 0x00);
    ret |= os05a10_write_reg(dev, 0x4051, 0x01);
    ret |= os05a10_write_reg(dev, 0x4052, 0x00);
    ret |= os05a10_write_reg(dev, 0x4053, 0x80);
    ret |= os05a10_write_reg(dev, 0x4054, 0x00);
    ret |= os05a10_write_reg(dev, 0x4055, 0x80);
    ret |= os05a10_write_reg(dev, 0x4056, 0x00);
    ret |= os05a10_write_reg(dev, 0x4057, 0x80);
    ret |= os05a10_write_reg(dev, 0x4058, 0x00);
    ret |= os05a10_write_reg(dev, 0x4059, 0x80);
    ret |= os05a10_write_reg(dev, 0x430b, 0xff);
    ret |= os05a10_write_reg(dev, 0x430c, 0xff);
    ret |= os05a10_write_reg(dev, 0x430d, 0x00);
    ret |= os05a10_write_reg(dev, 0x430e, 0x00);
    ret |= os05a10_write_reg(dev, 0x4501, 0x18);
    ret |= os05a10_write_reg(dev, 0x4502, 0x00);
    ret |= os05a10_write_reg(dev, 0x4643, 0x00);
    ret |= os05a10_write_reg(dev, 0x4640, 0x01);
    ret |= os05a10_write_reg(dev, 0x4641, 0x04);
    ret |= os05a10_write_reg(dev, 0x480e, 0x00);
    ret |= os05a10_write_reg(dev, 0x4813, 0x00);
    ret |= os05a10_write_reg(dev, 0x4815, 0x2b);
    ret |= os05a10_write_reg(dev, 0x486e, 0x36);
    ret |= os05a10_write_reg(dev, 0x486f, 0x84);
    ret |= os05a10_write_reg(dev, 0x4860, 0x00);
    ret |= os05a10_write_reg(dev, 0x4861, 0xa0);
    ret |= os05a10_write_reg(dev, 0x484b, 0x05);
    ret |= os05a10_write_reg(dev, 0x4850, 0x00);
    ret |= os05a10_write_reg(dev, 0x4851, 0xaa);
    ret |= os05a10_write_reg(dev, 0x4852, 0xff);
    ret |= os05a10_write_reg(dev, 0x4853, 0x8a);
    ret |= os05a10_write_reg(dev, 0x4854, 0x08);
    ret |= os05a10_write_reg(dev, 0x4855, 0x30);
    ret |= os05a10_write_reg(dev, 0x4800, 0x60);
    ret |= os05a10_write_reg(dev, 0x4837, 0x1d);
    ret |= os05a10_write_reg(dev, 0x484a, 0x3f);
    ret |= os05a10_write_reg(dev, 0x5000, 0xc9);
    ret |= os05a10_write_reg(dev, 0x5001, 0x43);
    ret |= os05a10_write_reg(dev, 0x5002, 0x00);
    ret |= os05a10_write_reg(dev, 0x5211, 0x03);
    ret |= os05a10_write_reg(dev, 0x5291, 0x03);
    ret |= os05a10_write_reg(dev, 0x520d, 0x0f);
    ret |= os05a10_write_reg(dev, 0x520e, 0xfd);
    ret |= os05a10_write_reg(dev, 0x520f, 0xa5);
    ret |= os05a10_write_reg(dev, 0x5210, 0xa5);
    ret |= os05a10_write_reg(dev, 0x528d, 0x0f);
    ret |= os05a10_write_reg(dev, 0x528e, 0xfd);
    ret |= os05a10_write_reg(dev, 0x528f, 0xa5);
    ret |= os05a10_write_reg(dev, 0x5290, 0xa5);
    ret |= os05a10_write_reg(dev, 0x5004, 0x40);
    ret |= os05a10_write_reg(dev, 0x5005, 0x00);
    ret |= os05a10_write_reg(dev, 0x5180, 0x00);
    ret |= os05a10_write_reg(dev, 0x5181, 0x10);
    ret |= os05a10_write_reg(dev, 0x5182, 0x0f);
    ret |= os05a10_write_reg(dev, 0x5183, 0xff);
    ret |= os05a10_write_reg(dev, 0x580b, 0x03);
    ret |= os05a10_write_reg(dev, 0x4d00, 0x03);
    ret |= os05a10_write_reg(dev, 0x4d01, 0xe9);
    ret |= os05a10_write_reg(dev, 0x4d02, 0xba);
    ret |= os05a10_write_reg(dev, 0x4d03, 0x66);
    ret |= os05a10_write_reg(dev, 0x4d04, 0x46);
    ret |= os05a10_write_reg(dev, 0x4d05, 0xa5);
    ret |= os05a10_write_reg(dev, 0x3603, 0x3c);
    ret |= os05a10_write_reg(dev, 0x3703, 0x26);
    ret |= os05a10_write_reg(dev, 0x3709, 0x49);
    ret |= os05a10_write_reg(dev, 0x3708, 0x2d);
    ret |= os05a10_write_reg(dev, 0x3719, 0x1c);
    ret |= os05a10_write_reg(dev, 0x371a, 0x06);
    ret |= os05a10_write_reg(dev, 0x4000, 0x79);
    ret |= os05a10_write_reg(dev, 0x4837, 0x1d);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os05a10_write_reg failed\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_4lane_linear_2688x1944_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os05a10_linear_comm_init(dev);
    ret |= os05a10_write_reg(dev, 0x3800, 0x00);
    ret |= os05a10_write_reg(dev, 0x3801, 0x00);
    ret |= os05a10_write_reg(dev, 0x3802, 0x00);
    ret |= os05a10_write_reg(dev, 0x3803, 0x0c);
    ret |= os05a10_write_reg(dev, 0x3804, 0x0e);
    ret |= os05a10_write_reg(dev, 0x3805, 0xff);
    ret |= os05a10_write_reg(dev, 0x3806, 0x08);
    ret |= os05a10_write_reg(dev, 0x3807, 0x6f);
    ret |= os05a10_write_reg(dev, 0x3808, 0x0a);
    ret |= os05a10_write_reg(dev, 0x3809, 0x80);
    ret |= os05a10_write_reg(dev, 0x380a, 0x07);
    ret |= os05a10_write_reg(dev, 0x380b, 0x98);

    os05a10_delay_ms(5);  // delay 5 ms
    ret |= os05a10_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os05a10_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("==== OS05A10_24MInput_MIPI_4lane_340Mbps_10bit_linear_2688x1944_17fps init success!====\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_4lane_linear_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os05a10_linear_comm_init(dev);
    ret |= os05a10_write_reg(dev, 0x3800, 0x00); //X_ADDR_START
    ret |= os05a10_write_reg(dev, 0x3801, 0x00);
    ret |= os05a10_write_reg(dev, 0x3804, 0x0a); //X_ADDR_END
    ret |= os05a10_write_reg(dev, 0x3805, 0x7f);

    ret |= os05a10_write_reg(dev, 0x3802, 0x00); //Y_ADDR_START
    ret |= os05a10_write_reg(dev, 0x3803, 0x0c);
    ret |= os05a10_write_reg(dev, 0x3806, 0x05); //Y_ADDR_END
    ret |= os05a10_write_reg(dev, 0x3807, 0xfb);

    ret |= os05a10_write_reg(dev, 0x3808, 0x0a); //X_OUTSIZE
    ret |= os05a10_write_reg(dev, 0x3809, 0x80);
    ret |= os05a10_write_reg(dev, 0x380a, 0x05); //Y_OUTSIZE
    ret |= os05a10_write_reg(dev, 0x380b, 0xf0);

    os05a10_delay_ms(5);  // delay 5 ms
    ret |= os05a10_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os05a10_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("====================================================================================\n");
        SENSOR_PRINT("=== OS05A10_24MInput_MIPI_4lane_340Mbps_10bit_linear_2688x1520_17fps init success!==\n");
        SENSOR_PRINT("====================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

/*
 * 函数功能: os05a10 MIPI 4lane 1944p wdr模式初始化序列(asic) - 5M-2688x1944
 * 函数参数：
 *      dev - sensor 设备号
 * 返回值：
 *
 */
static xmedia_s32 os05a10_4lane_wdr_2688x1944_init(xmedia_u32 dev)
{

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("=============================== OS05A10 WDR NOT SUPPORT! ===============================\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

static xmedia_s32 os05a10_4lane_wdr_2688x1520_init(xmedia_u32 dev)
{

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("=============================== OS05A10 WDR NOT SUPPORT! ===============================\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_ERRCODE_NOT_SUPPORT;
}

#else
static xmedia_s32 os05a10_linear_comm_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os05a10_write_reg(dev, 0x4600, 0x00);
    ret |= os05a10_write_reg(dev, 0x4601, 0x10);
    ret |= os05a10_write_reg(dev, 0x4603, 0x01);
    ret |= os05a10_write_reg(dev, 0x0103, 0x01);
    ret |= os05a10_write_reg(dev, 0x0303, 0x01);
    ret |= os05a10_write_reg(dev, 0x0305, 0x27);
    ret |= os05a10_write_reg(dev, 0x0306, 0x00);
    ret |= os05a10_write_reg(dev, 0x0307, 0x00);
    ret |= os05a10_write_reg(dev, 0x0308, 0x03);
    ret |= os05a10_write_reg(dev, 0x0309, 0x04);
    ret |= os05a10_write_reg(dev, 0x032a, 0x00);
    ret |= os05a10_write_reg(dev, 0x031e, 0x0a);
    ret |= os05a10_write_reg(dev, 0x0325, 0x48);
    ret |= os05a10_write_reg(dev, 0x0328, 0x07);
    ret |= os05a10_write_reg(dev, 0x300d, 0x11);
    ret |= os05a10_write_reg(dev, 0x300e, 0x11);
    ret |= os05a10_write_reg(dev, 0x300f, 0x11);
    ret |= os05a10_write_reg(dev, 0x3010, 0x01);
    ret |= os05a10_write_reg(dev, 0x3012, 0x41);
    ret |= os05a10_write_reg(dev, 0x3016, 0xf0);
    ret |= os05a10_write_reg(dev, 0x3018, 0xf0);
    ret |= os05a10_write_reg(dev, 0x3028, 0xf0);
    ret |= os05a10_write_reg(dev, 0x301e, 0x98);
    ret |= os05a10_write_reg(dev, 0x3010, 0x04);
    ret |= os05a10_write_reg(dev, 0x3011, 0x06);
    ret |= os05a10_write_reg(dev, 0x3031, 0xa9);
    ret |= os05a10_write_reg(dev, 0x3103, 0x48);
    ret |= os05a10_write_reg(dev, 0x3104, 0x01);
    ret |= os05a10_write_reg(dev, 0x3106, 0x10);
    ret |= os05a10_write_reg(dev, 0x3400, 0x04);
    ret |= os05a10_write_reg(dev, 0x3025, 0x03);
    ret |= os05a10_write_reg(dev, 0x3425, 0x51);
    ret |= os05a10_write_reg(dev, 0x3428, 0x01);
    ret |= os05a10_write_reg(dev, 0x3406, 0x08);
    ret |= os05a10_write_reg(dev, 0x3408, 0x03);
    ret |= os05a10_write_reg(dev, 0x3501, 0x08);
    ret |= os05a10_write_reg(dev, 0x3502, 0x6f);
    ret |= os05a10_write_reg(dev, 0x3505, 0x83);
    ret |= os05a10_write_reg(dev, 0x3508, 0x00);
    ret |= os05a10_write_reg(dev, 0x3509, 0x80);
    ret |= os05a10_write_reg(dev, 0x350a, 0x04);
    ret |= os05a10_write_reg(dev, 0x350b, 0x00);
    ret |= os05a10_write_reg(dev, 0x350c, 0x00);
    ret |= os05a10_write_reg(dev, 0x350d, 0x80);
    ret |= os05a10_write_reg(dev, 0x350e, 0x04);
    ret |= os05a10_write_reg(dev, 0x350f, 0x00);
    ret |= os05a10_write_reg(dev, 0x3600, 0x00);
    ret |= os05a10_write_reg(dev, 0x3626, 0xff);
    ret |= os05a10_write_reg(dev, 0x3605, 0x50);
    ret |= os05a10_write_reg(dev, 0x3609, 0xdb);
    ret |= os05a10_write_reg(dev, 0x3610, 0x69);
    ret |= os05a10_write_reg(dev, 0x360c, 0x01);
    ret |= os05a10_write_reg(dev, 0x3628, 0xa4);
    ret |= os05a10_write_reg(dev, 0x3629, 0x6a);
    ret |= os05a10_write_reg(dev, 0x362d, 0x10);
    ret |= os05a10_write_reg(dev, 0x3660, 0xd3);
    ret |= os05a10_write_reg(dev, 0x3661, 0x06);
    ret |= os05a10_write_reg(dev, 0x3662, 0x00);
    ret |= os05a10_write_reg(dev, 0x3663, 0x28);
    ret |= os05a10_write_reg(dev, 0x3664, 0x0d);
    ret |= os05a10_write_reg(dev, 0x366a, 0x38);
    ret |= os05a10_write_reg(dev, 0x366b, 0xa0);
    ret |= os05a10_write_reg(dev, 0x366d, 0x00);
    ret |= os05a10_write_reg(dev, 0x366e, 0x00);
    ret |= os05a10_write_reg(dev, 0x3680, 0x00);
    ret |= os05a10_write_reg(dev, 0x3621, 0x81);
    ret |= os05a10_write_reg(dev, 0x3634, 0x31);
    ret |= os05a10_write_reg(dev, 0x3620, 0x00);
    ret |= os05a10_write_reg(dev, 0x3622, 0x00);
    ret |= os05a10_write_reg(dev, 0x362a, 0xd0);
    ret |= os05a10_write_reg(dev, 0x362e, 0x8c);
    ret |= os05a10_write_reg(dev, 0x362f, 0x98);
    ret |= os05a10_write_reg(dev, 0x3630, 0xb0);
    ret |= os05a10_write_reg(dev, 0x3631, 0xd7);
    ret |= os05a10_write_reg(dev, 0x3701, 0x0f);
    ret |= os05a10_write_reg(dev, 0x3737, 0x02);
    ret |= os05a10_write_reg(dev, 0x3741, 0x04);
    ret |= os05a10_write_reg(dev, 0x373c, 0x0f);
    ret |= os05a10_write_reg(dev, 0x373b, 0x02);
    ret |= os05a10_write_reg(dev, 0x3705, 0x00);
    ret |= os05a10_write_reg(dev, 0x3706, 0xa0);
    ret |= os05a10_write_reg(dev, 0x370a, 0x01);
    ret |= os05a10_write_reg(dev, 0x370b, 0xc8);
    ret |= os05a10_write_reg(dev, 0x3709, 0x4a);
    ret |= os05a10_write_reg(dev, 0x3714, 0x21);
    ret |= os05a10_write_reg(dev, 0x371c, 0x00);
    ret |= os05a10_write_reg(dev, 0x371d, 0x08);
    ret |= os05a10_write_reg(dev, 0x375e, 0x0b);
    ret |= os05a10_write_reg(dev, 0x3776, 0x10);
    ret |= os05a10_write_reg(dev, 0x3781, 0x02);
    ret |= os05a10_write_reg(dev, 0x3782, 0x04);
    ret |= os05a10_write_reg(dev, 0x3783, 0x02);
    ret |= os05a10_write_reg(dev, 0x3784, 0x08);
    ret |= os05a10_write_reg(dev, 0x3785, 0x08);
    ret |= os05a10_write_reg(dev, 0x3788, 0x01);
    ret |= os05a10_write_reg(dev, 0x3789, 0x01);
    ret |= os05a10_write_reg(dev, 0x3797, 0x04);
    ret |= os05a10_write_reg(dev, 0x3761, 0x02);
    ret |= os05a10_write_reg(dev, 0x3762, 0x0d);
    ret |= os05a10_write_reg(dev, 0x380c, 0x04);
    ret |= os05a10_write_reg(dev, 0x380d, 0xd0);
    ret |= os05a10_write_reg(dev, 0x380e, 0x08);
    ret |= os05a10_write_reg(dev, 0x380f, 0x8f);
    ret |= os05a10_write_reg(dev, 0x3813, 0x04);
    ret |= os05a10_write_reg(dev, 0x3814, 0x01);
    ret |= os05a10_write_reg(dev, 0x3815, 0x01);
    ret |= os05a10_write_reg(dev, 0x3816, 0x01);
    ret |= os05a10_write_reg(dev, 0x3817, 0x01);
    ret |= os05a10_write_reg(dev, 0x381c, 0x00);
    ret |= os05a10_write_reg(dev, 0x3820, 0x00);
    ret |= os05a10_write_reg(dev, 0x3821, 0x04);
    ret |= os05a10_write_reg(dev, 0x3823, 0x18);
    ret |= os05a10_write_reg(dev, 0x3826, 0x00);
    ret |= os05a10_write_reg(dev, 0x3827, 0x01);
    ret |= os05a10_write_reg(dev, 0x3832, 0x02);
    ret |= os05a10_write_reg(dev, 0x383c, 0x48);
    ret |= os05a10_write_reg(dev, 0x383d, 0xff);
    ret |= os05a10_write_reg(dev, 0x3843, 0x20);
    ret |= os05a10_write_reg(dev, 0x382d, 0x08);
    ret |= os05a10_write_reg(dev, 0x3d85, 0x0b);
    ret |= os05a10_write_reg(dev, 0x3d84, 0x40);
    ret |= os05a10_write_reg(dev, 0x3d8c, 0x63);
    ret |= os05a10_write_reg(dev, 0x3d8d, 0x00);
    ret |= os05a10_write_reg(dev, 0x4000, 0x78);
    ret |= os05a10_write_reg(dev, 0x4001, 0x2b);
    ret |= os05a10_write_reg(dev, 0x4005, 0x40);
    ret |= os05a10_write_reg(dev, 0x4028, 0x2f);
    ret |= os05a10_write_reg(dev, 0x400a, 0x01);
    ret |= os05a10_write_reg(dev, 0x4010, 0x12);
    ret |= os05a10_write_reg(dev, 0x4008, 0x02);
    ret |= os05a10_write_reg(dev, 0x4009, 0x0d);
    ret |= os05a10_write_reg(dev, 0x401a, 0x58);
    ret |= os05a10_write_reg(dev, 0x4050, 0x00);
    ret |= os05a10_write_reg(dev, 0x4051, 0x01);
    ret |= os05a10_write_reg(dev, 0x4052, 0x00);
    ret |= os05a10_write_reg(dev, 0x4053, 0x80);
    ret |= os05a10_write_reg(dev, 0x4054, 0x00);
    ret |= os05a10_write_reg(dev, 0x4055, 0x80);
    ret |= os05a10_write_reg(dev, 0x4056, 0x00);
    ret |= os05a10_write_reg(dev, 0x4057, 0x80);
    ret |= os05a10_write_reg(dev, 0x4058, 0x00);
    ret |= os05a10_write_reg(dev, 0x4059, 0x80);
    ret |= os05a10_write_reg(dev, 0x430b, 0xff);
    ret |= os05a10_write_reg(dev, 0x430c, 0xff);
    ret |= os05a10_write_reg(dev, 0x430d, 0x00);
    ret |= os05a10_write_reg(dev, 0x430e, 0x00);
    ret |= os05a10_write_reg(dev, 0x4501, 0x18);
    ret |= os05a10_write_reg(dev, 0x4502, 0x00);
    ret |= os05a10_write_reg(dev, 0x4600, 0x00);
    ret |= os05a10_write_reg(dev, 0x4601, 0x10);
    ret |= os05a10_write_reg(dev, 0x4603, 0x01);
    ret |= os05a10_write_reg(dev, 0x4643, 0x00);
    ret |= os05a10_write_reg(dev, 0x4640, 0x01);
    ret |= os05a10_write_reg(dev, 0x4641, 0x04);
    ret |= os05a10_write_reg(dev, 0x480e, 0x00);
    ret |= os05a10_write_reg(dev, 0x4813, 0x00);
    ret |= os05a10_write_reg(dev, 0x4815, 0x2b);
    ret |= os05a10_write_reg(dev, 0x486e, 0x36);
    ret |= os05a10_write_reg(dev, 0x486f, 0x84);
    ret |= os05a10_write_reg(dev, 0x4860, 0x00);
    ret |= os05a10_write_reg(dev, 0x4861, 0xa0);
    ret |= os05a10_write_reg(dev, 0x484b, 0x05);
    ret |= os05a10_write_reg(dev, 0x4850, 0x00);
    ret |= os05a10_write_reg(dev, 0x4851, 0xaa);
    ret |= os05a10_write_reg(dev, 0x4852, 0xff);
    ret |= os05a10_write_reg(dev, 0x4853, 0x8a);
    ret |= os05a10_write_reg(dev, 0x4854, 0x08);
    ret |= os05a10_write_reg(dev, 0x4855, 0x30);
    ret |= os05a10_write_reg(dev, 0x4800, 0x00);
    ret |= os05a10_write_reg(dev, 0x4837, 0x19);
    ret |= os05a10_write_reg(dev, 0x5000, 0xc9);
    ret |= os05a10_write_reg(dev, 0x5001, 0x43);
    ret |= os05a10_write_reg(dev, 0x5211, 0x03);
    ret |= os05a10_write_reg(dev, 0x5291, 0x03);
    ret |= os05a10_write_reg(dev, 0x520d, 0x0f);
    ret |= os05a10_write_reg(dev, 0x520e, 0xfd);
    ret |= os05a10_write_reg(dev, 0x520f, 0xa5);
    ret |= os05a10_write_reg(dev, 0x5210, 0xa5);
    ret |= os05a10_write_reg(dev, 0x528d, 0x0f);
    ret |= os05a10_write_reg(dev, 0x528e, 0xfd);
    ret |= os05a10_write_reg(dev, 0x528f, 0xa5);
    ret |= os05a10_write_reg(dev, 0x5290, 0xa5);
    ret |= os05a10_write_reg(dev, 0x5004, 0x40);
    ret |= os05a10_write_reg(dev, 0x5005, 0x00);
    ret |= os05a10_write_reg(dev, 0x5180, 0x00);
    ret |= os05a10_write_reg(dev, 0x5181, 0x10);
    ret |= os05a10_write_reg(dev, 0x5182, 0x0f);
    ret |= os05a10_write_reg(dev, 0x5183, 0xff);
    ret |= os05a10_write_reg(dev, 0x580b, 0x03);
    ret |= os05a10_write_reg(dev, 0x4d00, 0x03);
    ret |= os05a10_write_reg(dev, 0x4d01, 0xe9);
    ret |= os05a10_write_reg(dev, 0x4d02, 0xba);
    ret |= os05a10_write_reg(dev, 0x4d03, 0x66);
    ret |= os05a10_write_reg(dev, 0x4d04, 0x46);
    ret |= os05a10_write_reg(dev, 0x4d05, 0xa5);
    ret |= os05a10_write_reg(dev, 0x3603, 0x3c);
    ret |= os05a10_write_reg(dev, 0x3703, 0x26);
    ret |= os05a10_write_reg(dev, 0x3709, 0x49);
    ret |= os05a10_write_reg(dev, 0x4000, 0x79);
    ret |= os05a10_write_reg(dev, 0x380c, 0x06); // HTS
    ret |= os05a10_write_reg(dev, 0x380d, 0x04);
    ret |= os05a10_write_reg(dev, 0x380e, 0x09); // VTS
    ret |= os05a10_write_reg(dev, 0x380f, 0x21);
    ret |= os05a10_write_reg(dev, 0x3501, 0x09);
    ret |= os05a10_write_reg(dev, 0x3502, 0x19);
    ret |= os05a10_write_reg(dev, 0x0100, 0x01);
    ret |= os05a10_write_reg(dev, 0x0100, 0x00);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os05a10_write_reg failed\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}


static xmedia_s32 os05a10_4lane_linear_2688x1944_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os05a10_linear_comm_init(dev);
    ret |= os05a10_write_reg(dev, 0x3800, 0x00);//X_ADDR_START
    ret |= os05a10_write_reg(dev, 0x3801, 0x00);
    ret |= os05a10_write_reg(dev, 0x3802, 0x00);//Y_ADDR_START
    ret |= os05a10_write_reg(dev, 0x3803, 0x0c);
    ret |= os05a10_write_reg(dev, 0x3804, 0x0e);//X_ADDR_END
    ret |= os05a10_write_reg(dev, 0x3805, 0xff);
    ret |= os05a10_write_reg(dev, 0x3806, 0x08);//Y_ADDR_END
    ret |= os05a10_write_reg(dev, 0x3807, 0x6f);
    ret |= os05a10_write_reg(dev, 0x3808, 0x0a);//X_OUTSIZE
    ret |= os05a10_write_reg(dev, 0x3809, 0x80);
    ret |= os05a10_write_reg(dev, 0x380a, 0x07);//Y_OUTSIZE
    ret |= os05a10_write_reg(dev, 0x380b, 0x98);

    os05a10_delay_ms(5);  // delay 5 ms
    ret |= os05a10_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os05a10_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=======================================================================================\n");
        SENSOR_PRINT("==== OS05A10_24MInput_MIPI_4lane_12bit_linear_2688x1944_30fps init success!============\n");
        SENSOR_PRINT("=======================================================================================\n");
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_4lane_linear_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os05a10_linear_comm_init(dev);
    ret |= os05a10_write_reg(dev, 0x3800, 0x00); //X_ADDR_START
    ret |= os05a10_write_reg(dev, 0x3801, 0x00);
    ret |= os05a10_write_reg(dev, 0x3804, 0x0a); //X_ADDR_END
    ret |= os05a10_write_reg(dev, 0x3805, 0x7f);

    ret |= os05a10_write_reg(dev, 0x3802, 0x00); //Y_ADDR_START
    ret |= os05a10_write_reg(dev, 0x3803, 0x0c);
    ret |= os05a10_write_reg(dev, 0x3806, 0x05); //Y_ADDR_END
    ret |= os05a10_write_reg(dev, 0x3807, 0xfb);

    ret |= os05a10_write_reg(dev, 0x3808, 0x0a); //X_OUTSIZE
    ret |= os05a10_write_reg(dev, 0x3809, 0x80);
    ret |= os05a10_write_reg(dev, 0x380a, 0x05); //Y_OUTSIZE
    ret |= os05a10_write_reg(dev, 0x380b, 0xf0);

    os05a10_delay_ms(5);  // delay 5 ms
    ret |= os05a10_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os05a10_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("=== OS05A10_24MInput_MIPI_4lane_12bit_2688x1520_30fps init success!==============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

/*
 * 函数功能: os05a10 MIPI 4lane 1944p wdr模式初始化序列(asic) - 5M-2688x1944
 * 函数参数：
 *      dev - sensor 设备号
 * 返回值：
 *
 */
 static xmedia_s32 os05a10_wdr_comm_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os05a10_write_reg(dev, 0x0100 , 0x00);
    ret |= os05a10_write_reg(dev, 0x0103 , 0x01);
    ret |= os05a10_write_reg(dev, 0x0303 , 0x01);
    ret |= os05a10_write_reg(dev, 0x0305 , 0x3b);
    ret |= os05a10_write_reg(dev, 0x0306 , 0x00);
    ret |= os05a10_write_reg(dev, 0x0307 , 0x00);
    ret |= os05a10_write_reg(dev, 0x0308 , 0x03);
    ret |= os05a10_write_reg(dev, 0x0309 , 0x04);
    ret |= os05a10_write_reg(dev, 0x032a , 0x00);
    ret |= os05a10_write_reg(dev, 0x031e , 0x09);
    ret |= os05a10_write_reg(dev, 0x0325 , 0x48);
    ret |= os05a10_write_reg(dev, 0x0328 , 0x07);
    ret |= os05a10_write_reg(dev, 0x300d , 0x11);
    ret |= os05a10_write_reg(dev, 0x300e , 0x11);
    ret |= os05a10_write_reg(dev, 0x300f , 0x11);
    ret |= os05a10_write_reg(dev, 0x3010 , 0x01);
    ret |= os05a10_write_reg(dev, 0x3012 , 0x41);
    ret |= os05a10_write_reg(dev, 0x3016 , 0xf0);
    ret |= os05a10_write_reg(dev, 0x3018 , 0xf0);
    ret |= os05a10_write_reg(dev, 0x3028 , 0xf0);
    ret |= os05a10_write_reg(dev, 0x301e , 0x98);
    ret |= os05a10_write_reg(dev, 0x3010 , 0x04);
    ret |= os05a10_write_reg(dev, 0x3011 , 0x06);
    ret |= os05a10_write_reg(dev, 0x3031 , 0xa9);
    ret |= os05a10_write_reg(dev, 0x3103 , 0x48);
    ret |= os05a10_write_reg(dev, 0x3104 , 0x01);
    ret |= os05a10_write_reg(dev, 0x3106 , 0x10);
    ret |= os05a10_write_reg(dev, 0x3501 , 0x09);
    ret |= os05a10_write_reg(dev, 0x3502 , 0xa0);
    ret |= os05a10_write_reg(dev, 0x3505 , 0x83);
    ret |= os05a10_write_reg(dev, 0x3508 , 0x00);
    ret |= os05a10_write_reg(dev, 0x3509 , 0x80);
    ret |= os05a10_write_reg(dev, 0x350a , 0x04);
    ret |= os05a10_write_reg(dev, 0x350b , 0x00);
    ret |= os05a10_write_reg(dev, 0x350c , 0x00);
    ret |= os05a10_write_reg(dev, 0x350d , 0x80);
    ret |= os05a10_write_reg(dev, 0x350e , 0x04);
    ret |= os05a10_write_reg(dev, 0x350f , 0x00);
    ret |= os05a10_write_reg(dev, 0x3600 , 0x00);
    ret |= os05a10_write_reg(dev, 0x3626 , 0xff);
    ret |= os05a10_write_reg(dev, 0x3605 , 0x50);
    ret |= os05a10_write_reg(dev, 0x3609 , 0xb5);
    ret |= os05a10_write_reg(dev, 0x3610 , 0x69);
    ret |= os05a10_write_reg(dev, 0x360c , 0x01);
    ret |= os05a10_write_reg(dev, 0x3628 , 0xa4);
    ret |= os05a10_write_reg(dev, 0x3629 , 0x6a);
    ret |= os05a10_write_reg(dev, 0x362d , 0x10);
    ret |= os05a10_write_reg(dev, 0x3660 , 0x42);
    ret |= os05a10_write_reg(dev, 0x3661 , 0x07);
    ret |= os05a10_write_reg(dev, 0x3662 , 0x00);
    ret |= os05a10_write_reg(dev, 0x3663 , 0x28);
    ret |= os05a10_write_reg(dev, 0x3664 , 0x0d);
    ret |= os05a10_write_reg(dev, 0x366a , 0x38);
    ret |= os05a10_write_reg(dev, 0x366b , 0xa0);
    ret |= os05a10_write_reg(dev, 0x366d , 0x00);
    ret |= os05a10_write_reg(dev, 0x366e , 0x00);
    ret |= os05a10_write_reg(dev, 0x3680 , 0x00);
    ret |= os05a10_write_reg(dev, 0x36c0 , 0x00);
    ret |= os05a10_write_reg(dev, 0x3621 , 0x81);
    ret |= os05a10_write_reg(dev, 0x3634 , 0x31);
    ret |= os05a10_write_reg(dev, 0x3620 , 0x00);
    ret |= os05a10_write_reg(dev, 0x3622 , 0x00);
    ret |= os05a10_write_reg(dev, 0x362a , 0xd0);
    ret |= os05a10_write_reg(dev, 0x362e , 0x8c);
    ret |= os05a10_write_reg(dev, 0x362f , 0x98);
    ret |= os05a10_write_reg(dev, 0x3630 , 0xb0);
    ret |= os05a10_write_reg(dev, 0x3631 , 0xd7);
    ret |= os05a10_write_reg(dev, 0x3701 , 0x0f);
    ret |= os05a10_write_reg(dev, 0x3737 , 0x02);
    ret |= os05a10_write_reg(dev, 0x3740 , 0x18);
    ret |= os05a10_write_reg(dev, 0x3741 , 0x04);
    ret |= os05a10_write_reg(dev, 0x373c , 0x0f);
    ret |= os05a10_write_reg(dev, 0x373b , 0x02);
    ret |= os05a10_write_reg(dev, 0x3705 , 0x00);
    ret |= os05a10_write_reg(dev, 0x3706 , 0x50);
    ret |= os05a10_write_reg(dev, 0x370a , 0x00);
    ret |= os05a10_write_reg(dev, 0x370b , 0xe4);
    ret |= os05a10_write_reg(dev, 0x3709 , 0x4a);
    ret |= os05a10_write_reg(dev, 0x3714 , 0x21);
    ret |= os05a10_write_reg(dev, 0x371c , 0x00);
    ret |= os05a10_write_reg(dev, 0x371d , 0x08);
    ret |= os05a10_write_reg(dev, 0x375e , 0x0e);
    ret |= os05a10_write_reg(dev, 0x3760 , 0x13);
    ret |= os05a10_write_reg(dev, 0x3776 , 0x10);
    ret |= os05a10_write_reg(dev, 0x3781 , 0x02);
    ret |= os05a10_write_reg(dev, 0x3782 , 0x04);
    ret |= os05a10_write_reg(dev, 0x3783 , 0x02);
    ret |= os05a10_write_reg(dev, 0x3784 , 0x08);
    ret |= os05a10_write_reg(dev, 0x3785 , 0x08);
    ret |= os05a10_write_reg(dev, 0x3788 , 0x01);
    ret |= os05a10_write_reg(dev, 0x3789 , 0x01);
    ret |= os05a10_write_reg(dev, 0x3797 , 0x84);
    ret |= os05a10_write_reg(dev, 0x3798 , 0x01);
    ret |= os05a10_write_reg(dev, 0x3799 , 0x00);
    ret |= os05a10_write_reg(dev, 0x3761 , 0x02);
    ret |= os05a10_write_reg(dev, 0x3762 , 0x0d);
    ret |= os05a10_write_reg(dev, 0x380c , 0x03);
    ret |= os05a10_write_reg(dev, 0x380d , 0x58);
    ret |= os05a10_write_reg(dev, 0x380e , 0x08);
    ret |= os05a10_write_reg(dev, 0x380f , 0x36);
    ret |= os05a10_write_reg(dev, 0x3813 , 0x04);
    ret |= os05a10_write_reg(dev, 0x3814 , 0x01);
    ret |= os05a10_write_reg(dev, 0x3815 , 0x01);
    ret |= os05a10_write_reg(dev, 0x3816 , 0x01);
    ret |= os05a10_write_reg(dev, 0x3817 , 0x01);
    ret |= os05a10_write_reg(dev, 0x381c , 0x08);
    ret |= os05a10_write_reg(dev, 0x3820 , 0x00);
    ret |= os05a10_write_reg(dev, 0x3821 , 0x24);
    ret |= os05a10_write_reg(dev, 0x3823 , 0x08);
    ret |= os05a10_write_reg(dev, 0x3826 , 0x00);
    ret |= os05a10_write_reg(dev, 0x3827 , 0x01);
    ret |= os05a10_write_reg(dev, 0x3833 , 0x01);
    ret |= os05a10_write_reg(dev, 0x3832 , 0x02);
    ret |= os05a10_write_reg(dev, 0x383c , 0x48);
    ret |= os05a10_write_reg(dev, 0x383d , 0xff);
    ret |= os05a10_write_reg(dev, 0x3843 , 0x20);
    ret |= os05a10_write_reg(dev, 0x382d , 0x08);
    ret |= os05a10_write_reg(dev, 0x3d85 , 0x0b);
    ret |= os05a10_write_reg(dev, 0x3d84 , 0x40);
    ret |= os05a10_write_reg(dev, 0x3d8c , 0x63);
    ret |= os05a10_write_reg(dev, 0x3d8d , 0x00);
    ret |= os05a10_write_reg(dev, 0x4000 , 0x78);
    ret |= os05a10_write_reg(dev, 0x4001 , 0x2b);
    ret |= os05a10_write_reg(dev, 0x4004 , 0x00);
    ret |= os05a10_write_reg(dev, 0x4005 , 0x40);
    ret |= os05a10_write_reg(dev, 0x4028 , 0x2f);
    ret |= os05a10_write_reg(dev, 0x400a , 0x01);
    ret |= os05a10_write_reg(dev, 0x4010 , 0x12);
    ret |= os05a10_write_reg(dev, 0x4008 , 0x02);
    ret |= os05a10_write_reg(dev, 0x4009 , 0x0d);
    ret |= os05a10_write_reg(dev, 0x401a , 0x58);
    ret |= os05a10_write_reg(dev, 0x4050 , 0x00);
    ret |= os05a10_write_reg(dev, 0x4051 , 0x01);
    ret |= os05a10_write_reg(dev, 0x4052 , 0x00);
    ret |= os05a10_write_reg(dev, 0x4053 , 0x80);
    ret |= os05a10_write_reg(dev, 0x4054 , 0x00);
    ret |= os05a10_write_reg(dev, 0x4055 , 0x80);
    ret |= os05a10_write_reg(dev, 0x4056 , 0x00);
    ret |= os05a10_write_reg(dev, 0x4057 , 0x80);
    ret |= os05a10_write_reg(dev, 0x4058 , 0x00);
    ret |= os05a10_write_reg(dev, 0x4059 , 0x80);
    ret |= os05a10_write_reg(dev, 0x430b , 0xff);
    ret |= os05a10_write_reg(dev, 0x430c , 0xff);
    ret |= os05a10_write_reg(dev, 0x430d , 0x00);
    ret |= os05a10_write_reg(dev, 0x430e , 0x00);
    ret |= os05a10_write_reg(dev, 0x4501 , 0x18);
    ret |= os05a10_write_reg(dev, 0x4502 , 0x00);
    ret |= os05a10_write_reg(dev, 0x4643 , 0x00);
    ret |= os05a10_write_reg(dev, 0x4640 , 0x01);
    ret |= os05a10_write_reg(dev, 0x4641 , 0x04);
    ret |= os05a10_write_reg(dev, 0x480e , 0x04);
    ret |= os05a10_write_reg(dev, 0x4813 , 0x98);
    ret |= os05a10_write_reg(dev, 0x4815 , 0x2b);
    ret |= os05a10_write_reg(dev, 0x486e , 0x36);
    ret |= os05a10_write_reg(dev, 0x486f , 0x84);
    ret |= os05a10_write_reg(dev, 0x4860 , 0x00);
    ret |= os05a10_write_reg(dev, 0x4861 , 0xa0);
    ret |= os05a10_write_reg(dev, 0x484b , 0x05);
    ret |= os05a10_write_reg(dev, 0x4850 , 0x00);
    ret |= os05a10_write_reg(dev, 0x4851 , 0xaa);
    ret |= os05a10_write_reg(dev, 0x4852 , 0xff);
    ret |= os05a10_write_reg(dev, 0x4853 , 0x8a);
    ret |= os05a10_write_reg(dev, 0x4854 , 0x08);
    ret |= os05a10_write_reg(dev, 0x4855 , 0x30);
    ret |= os05a10_write_reg(dev, 0x4800 , 0x60);
    ret |= os05a10_write_reg(dev, 0x4837 , 0x0a);
    ret |= os05a10_write_reg(dev, 0x484a , 0x3f);
    ret |= os05a10_write_reg(dev, 0x5000 , 0xc9);
    ret |= os05a10_write_reg(dev, 0x5001 , 0x43);
    ret |= os05a10_write_reg(dev, 0x5002 , 0x00);
    ret |= os05a10_write_reg(dev, 0x5211 , 0x03);
    ret |= os05a10_write_reg(dev, 0x5291 , 0x03);
    ret |= os05a10_write_reg(dev, 0x520d , 0x0f);
    ret |= os05a10_write_reg(dev, 0x520e , 0xfd);
    ret |= os05a10_write_reg(dev, 0x520f , 0xa5);
    ret |= os05a10_write_reg(dev, 0x5210 , 0xa5);
    ret |= os05a10_write_reg(dev, 0x528d , 0x0f);
    ret |= os05a10_write_reg(dev, 0x528e , 0xfd);
    ret |= os05a10_write_reg(dev, 0x528f , 0xa5);
    ret |= os05a10_write_reg(dev, 0x5290 , 0xa5);
    ret |= os05a10_write_reg(dev, 0x5004 , 0x40);
    ret |= os05a10_write_reg(dev, 0x5005 , 0x00);
    ret |= os05a10_write_reg(dev, 0x5180 , 0x00);
    ret |= os05a10_write_reg(dev, 0x5181 , 0x10);
    ret |= os05a10_write_reg(dev, 0x5182 , 0x0f);
    ret |= os05a10_write_reg(dev, 0x5183 , 0xff);
    ret |= os05a10_write_reg(dev, 0x580b , 0x03);
    ret |= os05a10_write_reg(dev, 0x4d00 , 0x03);
    ret |= os05a10_write_reg(dev, 0x4d01 , 0xe9);
    ret |= os05a10_write_reg(dev, 0x4d02 , 0xba);
    ret |= os05a10_write_reg(dev, 0x4d03 , 0x66);
    ret |= os05a10_write_reg(dev, 0x4d04 , 0x46);
    ret |= os05a10_write_reg(dev, 0x4d05 , 0xa5);
    ret |= os05a10_write_reg(dev, 0x3603 , 0x3c);
    ret |= os05a10_write_reg(dev, 0x3703 , 0x26);
    ret |= os05a10_write_reg(dev, 0x3709 , 0x49);
    ret |= os05a10_write_reg(dev, 0x3708 , 0x2d);
    ret |= os05a10_write_reg(dev, 0x3719 , 0x1c);
    ret |= os05a10_write_reg(dev, 0x371a , 0x06);
    ret |= os05a10_write_reg(dev, 0x4000 , 0x79);
    ret |= os05a10_write_reg(dev, 0x380c , 0x03);
    ret |= os05a10_write_reg(dev, 0x380d , 0x58);
    ret |= os05a10_write_reg(dev, 0x380e , 0x08);
    ret |= os05a10_write_reg(dev, 0x380f , 0x36);
    ret |= os05a10_write_reg(dev, 0x3501 , 0x08);
    ret |= os05a10_write_reg(dev, 0x3502 , 0xbb);
    ret |= os05a10_write_reg(dev, 0x3511 , 0x00);
    ret |= os05a10_write_reg(dev, 0x3512 , 0x20);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os05a10_write_reg failed\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}
static xmedia_s32 os05a10_4lane_wdr_2688x1944_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os05a10_wdr_comm_init(dev);
    ret |= os05a10_write_reg(dev, 0x3800 , 0x00);
    ret |= os05a10_write_reg(dev, 0x3801 , 0x00);
    ret |= os05a10_write_reg(dev, 0x3802 , 0x00);
    ret |= os05a10_write_reg(dev, 0x3803 , 0x0c);
    ret |= os05a10_write_reg(dev, 0x3804 , 0x0e);
    ret |= os05a10_write_reg(dev, 0x3805 , 0xff);
    ret |= os05a10_write_reg(dev, 0x3806 , 0x08);
    ret |= os05a10_write_reg(dev, 0x3807 , 0x6f);
    ret |= os05a10_write_reg(dev, 0x3808 , 0x0a);
    ret |= os05a10_write_reg(dev, 0x3809 , 0x80);
    ret |= os05a10_write_reg(dev, 0x380a , 0x07);
    ret |= os05a10_write_reg(dev, 0x380b , 0x98);

    os05a10_delay_ms(5);  // delay 5 ms
    ret |= os05a10_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os05a10_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("======= OS05A10_24MInput_MIPI_4lane_10bit_wdr_2688x1944_30fps init success!============\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_SUCCESS;
}

static xmedia_s32 os05a10_4lane_wdr_2688x1520_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os05a10_wdr_comm_init(dev);
    ret |= os05a10_write_reg(dev, 0x3800, 0x00); //X_ADDR_START
    ret |= os05a10_write_reg(dev, 0x3801, 0x00);
    ret |= os05a10_write_reg(dev, 0x3804, 0x0a); //X_ADDR_END
    ret |= os05a10_write_reg(dev, 0x3805, 0x7f);

    ret |= os05a10_write_reg(dev, 0x3802, 0x00); //Y_ADDR_START
    ret |= os05a10_write_reg(dev, 0x3803, 0x0c);
    ret |= os05a10_write_reg(dev, 0x3806, 0x05); //Y_ADDR_END
    ret |= os05a10_write_reg(dev, 0x3807, 0xfb);

    ret |= os05a10_write_reg(dev, 0x3808, 0x0a); //X_OUTSIZE
    ret |= os05a10_write_reg(dev, 0x3809, 0x80);
    ret |= os05a10_write_reg(dev, 0x380a, 0x05); //Y_OUTSIZE
    ret |= os05a10_write_reg(dev, 0x380b, 0xf0);

    os05a10_delay_ms(5);  // delay 5 ms
    ret |= os05a10_write_reg(dev, 0x0100, 0x01);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os05a10_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("=== OS05A10_24MInput_MIPI_4lane_10bit_wdr_2688x1520_30fps init success!==========\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

#endif

xmedia_void os05a10_master_init(xmedia_u32 dev)
{
    //master
    os05a10_write_reg(dev, 0x0100, 0x00);
    os05a10_write_reg(dev, 0x3002, 0x80);
    os05a10_write_reg(dev, 0x3009, 0x06);
    os05a10_write_reg(dev, 0x377e, 0x08);
    os05a10_write_reg(dev, 0x3797, 0x84);
    os05a10_write_reg(dev, 0x3798, 0x64);
    os05a10_write_reg(dev, 0x3799, 0x64);
    os05a10_write_reg(dev, 0x3818, 0x03); // set to HTS/2
    os05a10_write_reg(dev, 0x3819, 0x02); // set to HTS/2
    os05a10_write_reg(dev, 0x381a, 0x09); // set to VTS-4
    os05a10_write_reg(dev, 0x381b, 0x1D); // set to VTS-4
    os05a10_write_reg(dev, 0x3822, 0x44);
    os05a10_write_reg(dev, 0x3823, 0x08);
    os05a10_write_reg(dev, 0x3824, 0x00);
    os05a10_write_reg(dev, 0x3825, 0x20);
    os05a10_write_reg(dev, 0x3826, 0x00);
    os05a10_write_reg(dev, 0x3827, 0x01);
    os05a10_write_reg(dev, 0x3832, 0x22);
    os05a10_write_reg(dev, 0x3834, 0xf4);
    os05a10_write_reg(dev, 0x3842, 0x00);
    os05a10_write_reg(dev, 0x0100, 0x01);

    SENSOR_PRINT("dev[%d]- OS05A10 master setting init success.\n", dev);

    return;
}

xmedia_void os05a10_slave_init(xmedia_u32 dev)
{
    //slave
    os05a10_write_reg(dev, 0x0100, 0x00);
    os05a10_write_reg(dev, 0x3002, 0x00);
    os05a10_write_reg(dev, 0x3009, 0x02);
    os05a10_write_reg(dev, 0x377e, 0x0a);
    os05a10_write_reg(dev, 0x3797, 0x84);
    os05a10_write_reg(dev, 0x3798, 0x64);
    os05a10_write_reg(dev, 0x3799, 0x64);
    os05a10_write_reg(dev, 0x3818, 0x00);
    os05a10_write_reg(dev, 0x3819, 0x00);
    os05a10_write_reg(dev, 0x381a, 0x00);
    os05a10_write_reg(dev, 0x381b, 0x01);
    os05a10_write_reg(dev, 0x3822, 0x74);
    os05a10_write_reg(dev, 0x3823, 0x50);
    os05a10_write_reg(dev, 0x3824, 0x03); // set to HTS/2
    os05a10_write_reg(dev, 0x3825, 0x02); // set to HTS/2
    os05a10_write_reg(dev, 0x3826, 0x09); // set to VTS-4
    os05a10_write_reg(dev, 0x3827, 0x1d); // set to VTS-4
    os05a10_write_reg(dev, 0x3832, 0x02);
    os05a10_write_reg(dev, 0x3834, 0x04);
    os05a10_write_reg(dev, 0x3842, 0x00);
    os05a10_write_reg(dev, 0x0100, 0x01);

    SENSOR_PRINT("dev[%d]- OS05A10 slave setting init success.\n", dev);

    return;
}

xmedia_s32 os05a10_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case OS05A10_5M_LINEAR_MODE:
            ret = os05a10_4lane_linear_2688x1944_init(dev);
            break;
        case OS05A10_4M_LINEAR_MODE:
            ret = os05a10_4lane_linear_2688x1520_init(dev);
            break;
        case OS05A10_5M_WDR_MODE:
            ret = os05a10_4lane_wdr_2688x1944_init(dev);
            break;
        case OS05A10_4M_WDR_MODE:
            ret = os05a10_4lane_wdr_2688x1520_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (image_mode == OS05A10_5M_LINEAR_MODE || image_mode == OS05A10_4M_LINEAR_MODE) {
        if (work_mode == XMEDIA_SENSOR_WORK_MODE_MASTER) {
            os05a10_master_init(dev);
        } else if(work_mode == XMEDIA_SENSOR_WORK_MODE_SLAVE) {
            os05a10_slave_init(dev);
        }
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os05a10_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_os05a10_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_os05a10_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_os05a10_i2c_fd[dev]);
        g_os05a10_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_os05a10_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os05a10_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_os05a10_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_os05a10_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_os05a10_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = os05a10_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os05a10_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_os05a10_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_os05a10_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os05a10_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[OS05A10_DATA_BYTE];

    if (g_os05a10_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_os05a10_i2c_fd[dev], g_os05a10_i2c_addr[dev], addr, OS05A10_ADDR_BYTE, buf, OS05A10_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //OS05A10_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os05a10_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_os05a10_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_os05a10_i2c_fd[dev], g_os05a10_i2c_addr[dev], buf, OS05A10_ADDR_BYTE + OS05A10_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 os05a10_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_u32 mirror_type;
    xmedia_u32 flip_type;
    xmedia_s32 ret = XMEDIA_SUCCESS;

    os05a10_read_reg(dev, OS05A10_REG_ADDR_MIRROR, &mirror_type);
    os05a10_read_reg(dev, OS05A10_REG_ADDR_FLIP,   &flip_type);

    if (mirror_en){
        mirror_type |= 0x04;
    } else {
        mirror_type &= 0xfb;
    }

    if (flip_en) {
        flip_type |= 0x04;
    } else {
        flip_type &= 0xfb;
    }

    ret =  os05a10_write_reg(dev, OS05A10_REG_ADDR_MIRROR, mirror_type);
    ret |= os05a10_write_reg(dev, OS05A10_REG_ADDR_FLIP, flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os05a10_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
