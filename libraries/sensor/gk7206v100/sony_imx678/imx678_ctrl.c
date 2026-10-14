#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "imx678.h"
#include "imx678_ctrl.h"

#define IMX678_REG_ADDR_MIRROR 0x3020
#define IMX678_REG_ADDR_FLIP   0x3021

static xmedia_s32 g_imx678_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
static xmedia_s32 g_imx678_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

xmedia_void imx678_delay_ms(xmedia_s32 ms)
{
    sleep_us(ms * 1000);
}

xmedia_s32 imx678_default_reg_init(xmedia_u32 dev)
{
    xmedia_u32      i;
    sensor_context *imx678_ctx = XMEDIA_NULL;

    IMX678_GET_CTX(dev, imx678_ctx);
    SENSOR_CHECK_PTR_RETURN(imx678_ctx);

    for (i = 0; i < imx678_ctx->regs_info[SENSOR_CUR_FRAME].reg_num; i++) {
        imx678_write_reg(dev, imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].reg_addr,
                                imx678_ctx->regs_info[SENSOR_CUR_FRAME].i2c_data[i].data);
    }

    return XMEDIA_SUCCESS;
}

//goke_IMX678_RegisterSetting_Ver5.0_20230419_No1.def
// 4K@30fps
xmedia_s32 imx678_4lane_linear_3840x2160_init(xmedia_u32 dev)
{
    //All-pixel scan
    //CSI-2_4lane
    //37.125MHz
    //AD:12bit Output:12bit
    //1440Mbps
    //Master Mode
    //LCG Mode
    //30fps
    //Integration Time
    //33.678ms
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= imx678_write_reg(dev, 0x3000, 0x01);
    ret |= imx678_write_reg(dev, 0x3001, 0x00);
    ret |= imx678_write_reg(dev, 0x3002, 0x01);
    ret |= imx678_write_reg(dev, 0x3014, 0x01);
    ret |= imx678_write_reg(dev, 0x3015, 0x03);
    ret |= imx678_write_reg(dev, 0x3018, 0x00);
    ret |= imx678_write_reg(dev, 0x301A, 0x00);
    ret |= imx678_write_reg(dev, 0x301B, 0x00);
    ret |= imx678_write_reg(dev, 0x301C, 0x00);
    ret |= imx678_write_reg(dev, 0x301E, 0x01);
    ret |= imx678_write_reg(dev, 0x3020, 0x00);
    ret |= imx678_write_reg(dev, 0x3021, 0x00);
    ret |= imx678_write_reg(dev, 0x3022, 0x01);
    ret |= imx678_write_reg(dev, 0x3023, 0x01);
    ret |= imx678_write_reg(dev, 0x3028, 0xa6);
    ret |= imx678_write_reg(dev, 0x3029, 0x0e);
    ret |= imx678_write_reg(dev, 0x302A, 0x00);
    ret |= imx678_write_reg(dev, 0x302C, 0x94);
    ret |= imx678_write_reg(dev, 0x302D, 0x02);
    ret |= imx678_write_reg(dev, 0x3030, 0x00);
    ret |= imx678_write_reg(dev, 0x3031, 0x00);
    ret |= imx678_write_reg(dev, 0x3032, 0x00);
    ret |= imx678_write_reg(dev, 0x303C, 0x00);
    ret |= imx678_write_reg(dev, 0x303D, 0x00);
    ret |= imx678_write_reg(dev, 0x303E, 0x10);
    ret |= imx678_write_reg(dev, 0x303F, 0x0F);
    ret |= imx678_write_reg(dev, 0x3040, 0x03);
    ret |= imx678_write_reg(dev, 0x3042, 0x00);
    ret |= imx678_write_reg(dev, 0x3043, 0x00);
    ret |= imx678_write_reg(dev, 0x3044, 0x00);
    ret |= imx678_write_reg(dev, 0x3045, 0x00);
    ret |= imx678_write_reg(dev, 0x3046, 0x84);
    ret |= imx678_write_reg(dev, 0x3047, 0x08);
    ret |= imx678_write_reg(dev, 0x3050, 0x03);
    ret |= imx678_write_reg(dev, 0x3051, 0x00);
    ret |= imx678_write_reg(dev, 0x3052, 0x00);
    ret |= imx678_write_reg(dev, 0x3054, 0x0E);
    ret |= imx678_write_reg(dev, 0x3055, 0x00);
    ret |= imx678_write_reg(dev, 0x3056, 0x00);
    ret |= imx678_write_reg(dev, 0x3058, 0x8A);
    ret |= imx678_write_reg(dev, 0x3059, 0x01);
    ret |= imx678_write_reg(dev, 0x305A, 0x00);
    ret |= imx678_write_reg(dev, 0x3060, 0x16);
    ret |= imx678_write_reg(dev, 0x3061, 0x01);
    ret |= imx678_write_reg(dev, 0x3062, 0x00);
    ret |= imx678_write_reg(dev, 0x3064, 0xC4);
    ret |= imx678_write_reg(dev, 0x3065, 0x0C);
    ret |= imx678_write_reg(dev, 0x3066, 0x00);
    ret |= imx678_write_reg(dev, 0x3069, 0x00);
    ret |= imx678_write_reg(dev, 0x306B, 0x00);
    ret |= imx678_write_reg(dev, 0x3070, 0x00);
    ret |= imx678_write_reg(dev, 0x3071, 0x00);
    ret |= imx678_write_reg(dev, 0x3072, 0x00);
    ret |= imx678_write_reg(dev, 0x3073, 0x00);
    ret |= imx678_write_reg(dev, 0x3074, 0x00);
    ret |= imx678_write_reg(dev, 0x3075, 0x00);
    ret |= imx678_write_reg(dev, 0x3081, 0x00);
    ret |= imx678_write_reg(dev, 0x308C, 0x00);
    ret |= imx678_write_reg(dev, 0x308D, 0x01);
    ret |= imx678_write_reg(dev, 0x3094, 0x00);
    ret |= imx678_write_reg(dev, 0x3095, 0x00);
    ret |= imx678_write_reg(dev, 0x309C, 0x00);
    ret |= imx678_write_reg(dev, 0x309D, 0x00);
    ret |= imx678_write_reg(dev, 0x30A4, 0xAA);
    ret |= imx678_write_reg(dev, 0x30A6, 0x00);
    ret |= imx678_write_reg(dev, 0x30CC, 0x00);
    ret |= imx678_write_reg(dev, 0x30CD, 0x00);
    ret |= imx678_write_reg(dev, 0x30DC, 0x32);
    ret |= imx678_write_reg(dev, 0x30DD, 0x40);
    ret |= imx678_write_reg(dev, 0x3400, 0x01);
    ret |= imx678_write_reg(dev, 0x3458, 0x00);
    ret |= imx678_write_reg(dev, 0x3460, 0x22);
    ret |= imx678_write_reg(dev, 0x355A, 0x64);
    ret |= imx678_write_reg(dev, 0x3A02, 0x7A);
    ret |= imx678_write_reg(dev, 0x3A10, 0xEC);
    ret |= imx678_write_reg(dev, 0x3A12, 0x71);
    ret |= imx678_write_reg(dev, 0x3A14, 0xDE);
    ret |= imx678_write_reg(dev, 0x3A20, 0x2B);
    ret |= imx678_write_reg(dev, 0x3A24, 0x22);
    ret |= imx678_write_reg(dev, 0x3A25, 0x25);
    ret |= imx678_write_reg(dev, 0x3A26, 0x2A);
    ret |= imx678_write_reg(dev, 0x3A27, 0x2C);
    ret |= imx678_write_reg(dev, 0x3A28, 0x39);
    ret |= imx678_write_reg(dev, 0x3A29, 0x38);
    ret |= imx678_write_reg(dev, 0x3A30, 0x04);
    ret |= imx678_write_reg(dev, 0x3A31, 0x04);
    ret |= imx678_write_reg(dev, 0x3A32, 0x03);
    ret |= imx678_write_reg(dev, 0x3A33, 0x03);
    ret |= imx678_write_reg(dev, 0x3A34, 0x09);
    ret |= imx678_write_reg(dev, 0x3A35, 0x06);
    ret |= imx678_write_reg(dev, 0x3A38, 0xCD);
    ret |= imx678_write_reg(dev, 0x3A3A, 0x4C);
    ret |= imx678_write_reg(dev, 0x3A3C, 0xB9);
    ret |= imx678_write_reg(dev, 0x3A3E, 0x30);
    ret |= imx678_write_reg(dev, 0x3A40, 0x2C);
    ret |= imx678_write_reg(dev, 0x3A42, 0x39);
    ret |= imx678_write_reg(dev, 0x3A4E, 0x00);
    ret |= imx678_write_reg(dev, 0x3A52, 0x00);
    ret |= imx678_write_reg(dev, 0x3A56, 0x00);
    ret |= imx678_write_reg(dev, 0x3A5A, 0x00);
    ret |= imx678_write_reg(dev, 0x3A5E, 0x00);
    ret |= imx678_write_reg(dev, 0x3A62, 0x00);
    ret |= imx678_write_reg(dev, 0x3A64, 0x00);
    ret |= imx678_write_reg(dev, 0x3A6E, 0xA0);
    ret |= imx678_write_reg(dev, 0x3A70, 0x50);
    ret |= imx678_write_reg(dev, 0x3A8C, 0x04);
    ret |= imx678_write_reg(dev, 0x3A8D, 0x03);
    ret |= imx678_write_reg(dev, 0x3A8E, 0x09);
    ret |= imx678_write_reg(dev, 0x3A90, 0x38);
    ret |= imx678_write_reg(dev, 0x3A91, 0x42);
    ret |= imx678_write_reg(dev, 0x3A92, 0x3C);
    ret |= imx678_write_reg(dev, 0x3B0E, 0xF3);
    ret |= imx678_write_reg(dev, 0x3B12, 0xE5);
    ret |= imx678_write_reg(dev, 0x3B27, 0xC0);
    ret |= imx678_write_reg(dev, 0x3B2E, 0xEF);
    ret |= imx678_write_reg(dev, 0x3B30, 0x6A);
    ret |= imx678_write_reg(dev, 0x3B32, 0xF6);
    ret |= imx678_write_reg(dev, 0x3B36, 0xE1);
    ret |= imx678_write_reg(dev, 0x3B3A, 0xE8);
    ret |= imx678_write_reg(dev, 0x3B5A, 0x17);
    ret |= imx678_write_reg(dev, 0x3B5E, 0xEF);
    ret |= imx678_write_reg(dev, 0x3B60, 0x6A);
    ret |= imx678_write_reg(dev, 0x3B62, 0xF6);
    ret |= imx678_write_reg(dev, 0x3B66, 0xE1);
    ret |= imx678_write_reg(dev, 0x3B6A, 0xE8);
    ret |= imx678_write_reg(dev, 0x3B88, 0xEC);
    ret |= imx678_write_reg(dev, 0x3B8A, 0xED);
    ret |= imx678_write_reg(dev, 0x3B94, 0x71);
    ret |= imx678_write_reg(dev, 0x3B96, 0x72);
    ret |= imx678_write_reg(dev, 0x3B98, 0xDE);
    ret |= imx678_write_reg(dev, 0x3B9A, 0xDF);
    ret |= imx678_write_reg(dev, 0x3C0F, 0x06);
    ret |= imx678_write_reg(dev, 0x3C10, 0x06);
    ret |= imx678_write_reg(dev, 0x3C11, 0x06);
    ret |= imx678_write_reg(dev, 0x3C12, 0x06);
    ret |= imx678_write_reg(dev, 0x3C13, 0x06);
    ret |= imx678_write_reg(dev, 0x3C18, 0x20);
    ret |= imx678_write_reg(dev, 0x3C37, 0x10);
    ret |= imx678_write_reg(dev, 0x3C3A, 0x7A);
    ret |= imx678_write_reg(dev, 0x3C40, 0xF4);
    ret |= imx678_write_reg(dev, 0x3C48, 0xE6);
    ret |= imx678_write_reg(dev, 0x3C54, 0xCE);
    ret |= imx678_write_reg(dev, 0x3C56, 0xD0);
    ret |= imx678_write_reg(dev, 0x3C6C, 0x53);
    ret |= imx678_write_reg(dev, 0x3C6E, 0x55);
    ret |= imx678_write_reg(dev, 0x3C70, 0xC0);
    ret |= imx678_write_reg(dev, 0x3C72, 0xC2);
    ret |= imx678_write_reg(dev, 0x3C7E, 0xCE);
    ret |= imx678_write_reg(dev, 0x3C8C, 0xCF);
    ret |= imx678_write_reg(dev, 0x3C8E, 0xEB);
    ret |= imx678_write_reg(dev, 0x3C98, 0x54);
    ret |= imx678_write_reg(dev, 0x3C9A, 0x70);
    ret |= imx678_write_reg(dev, 0x3C9C, 0xC1);
    ret |= imx678_write_reg(dev, 0x3C9E, 0xDD);
    ret |= imx678_write_reg(dev, 0x3CB0, 0x7A);
    ret |= imx678_write_reg(dev, 0x3CB2, 0xBA);
    ret |= imx678_write_reg(dev, 0x3CC8, 0xBC);
    ret |= imx678_write_reg(dev, 0x3CCA, 0x7C);
    ret |= imx678_write_reg(dev, 0x3CD4, 0xEA);
    ret |= imx678_write_reg(dev, 0x3CD5, 0x01);
    ret |= imx678_write_reg(dev, 0x3CD6, 0x4A);
    ret |= imx678_write_reg(dev, 0x3CD8, 0x00);
    ret |= imx678_write_reg(dev, 0x3CD9, 0x00);
    ret |= imx678_write_reg(dev, 0x3CDA, 0xFF);
    ret |= imx678_write_reg(dev, 0x3CDB, 0x03);
    ret |= imx678_write_reg(dev, 0x3CDC, 0x00);
    ret |= imx678_write_reg(dev, 0x3CDD, 0x00);
    ret |= imx678_write_reg(dev, 0x3CDE, 0xFF);
    ret |= imx678_write_reg(dev, 0x3CDF, 0x03);
    ret |= imx678_write_reg(dev, 0x3CE4, 0x4C);
    ret |= imx678_write_reg(dev, 0x3CE6, 0xEC);
    ret |= imx678_write_reg(dev, 0x3CE7, 0x01);
    ret |= imx678_write_reg(dev, 0x3CE8, 0xFF);
    ret |= imx678_write_reg(dev, 0x3CE9, 0x03);
    ret |= imx678_write_reg(dev, 0x3CEA, 0x00);
    ret |= imx678_write_reg(dev, 0x3CEB, 0x00);
    ret |= imx678_write_reg(dev, 0x3CEC, 0xFF);
    ret |= imx678_write_reg(dev, 0x3CED, 0x03);
    ret |= imx678_write_reg(dev, 0x3CEE, 0x00);
    ret |= imx678_write_reg(dev, 0x3CEF, 0x00);
    ret |= imx678_write_reg(dev, 0x3CF2, 0xFF);
    ret |= imx678_write_reg(dev, 0x3CF3, 0x03);
    ret |= imx678_write_reg(dev, 0x3CF4, 0x00);
    ret |= imx678_write_reg(dev, 0x3E28, 0x82);
    ret |= imx678_write_reg(dev, 0x3E2A, 0x80);
    ret |= imx678_write_reg(dev, 0x3E30, 0x85);
    ret |= imx678_write_reg(dev, 0x3E32, 0x7D);
    ret |= imx678_write_reg(dev, 0x3E5C, 0xCE);
    ret |= imx678_write_reg(dev, 0x3E5E, 0xD3);
    ret |= imx678_write_reg(dev, 0x3E70, 0x53);
    ret |= imx678_write_reg(dev, 0x3E72, 0x58);
    ret |= imx678_write_reg(dev, 0x3E74, 0xC0);
    ret |= imx678_write_reg(dev, 0x3E76, 0xC5);
    ret |= imx678_write_reg(dev, 0x3E78, 0xC0);
    ret |= imx678_write_reg(dev, 0x3E79, 0x01);
    ret |= imx678_write_reg(dev, 0x3E7A, 0xD4);
    ret |= imx678_write_reg(dev, 0x3E7B, 0x01);
    ret |= imx678_write_reg(dev, 0x3EB4, 0x0B);
    ret |= imx678_write_reg(dev, 0x3EB5, 0x02);
    ret |= imx678_write_reg(dev, 0x3EB6, 0x4D);
    ret |= imx678_write_reg(dev, 0x3EB7, 0x42);
    ret |= imx678_write_reg(dev, 0x3EEC, 0xF3);
    ret |= imx678_write_reg(dev, 0x3EEE, 0xE7);
    ret |= imx678_write_reg(dev, 0x3F01, 0x01);
    ret |= imx678_write_reg(dev, 0x3F24, 0x10);
    ret |= imx678_write_reg(dev, 0x3F28, 0x2D);
    ret |= imx678_write_reg(dev, 0x3F2A, 0x2D);
    ret |= imx678_write_reg(dev, 0x3F2C, 0x2D);
    ret |= imx678_write_reg(dev, 0x3F2E, 0x2D);
    ret |= imx678_write_reg(dev, 0x3F30, 0x23);
    ret |= imx678_write_reg(dev, 0x3F38, 0x2D);
    ret |= imx678_write_reg(dev, 0x3F3A, 0x2D);
    ret |= imx678_write_reg(dev, 0x3F3C, 0x2D);
    ret |= imx678_write_reg(dev, 0x3F3E, 0x28);
    ret |= imx678_write_reg(dev, 0x3F40, 0x1E);
    ret |= imx678_write_reg(dev, 0x3F48, 0x2D);
    ret |= imx678_write_reg(dev, 0x3F4A, 0x2D);
    ret |= imx678_write_reg(dev, 0x3F4C, 0x00);
    ret |= imx678_write_reg(dev, 0x4004, 0xE4);
    ret |= imx678_write_reg(dev, 0x4006, 0xFF);
    ret |= imx678_write_reg(dev, 0x4018, 0x69);
    ret |= imx678_write_reg(dev, 0x401A, 0x84);
    ret |= imx678_write_reg(dev, 0x401C, 0xD6);
    ret |= imx678_write_reg(dev, 0x401E, 0xF1);
    ret |= imx678_write_reg(dev, 0x4038, 0xDE);
    ret |= imx678_write_reg(dev, 0x403A, 0x00);
    ret |= imx678_write_reg(dev, 0x403B, 0x01);
    ret |= imx678_write_reg(dev, 0x404C, 0x63);
    ret |= imx678_write_reg(dev, 0x404E, 0x85);
    ret |= imx678_write_reg(dev, 0x4050, 0xD0);
    ret |= imx678_write_reg(dev, 0x4052, 0xF2);
    ret |= imx678_write_reg(dev, 0x4108, 0xDD);
    ret |= imx678_write_reg(dev, 0x410A, 0xF7);
    ret |= imx678_write_reg(dev, 0x411C, 0x62);
    ret |= imx678_write_reg(dev, 0x411E, 0x7C);
    ret |= imx678_write_reg(dev, 0x4120, 0xCF);
    ret |= imx678_write_reg(dev, 0x4122, 0xE9);
    ret |= imx678_write_reg(dev, 0x4138, 0xE6);
    ret |= imx678_write_reg(dev, 0x413A, 0xF1);
    ret |= imx678_write_reg(dev, 0x414C, 0x6B);
    ret |= imx678_write_reg(dev, 0x414E, 0x76);
    ret |= imx678_write_reg(dev, 0x4150, 0xD8);
    ret |= imx678_write_reg(dev, 0x4152, 0xE3);
    ret |= imx678_write_reg(dev, 0x417E, 0x03);
    ret |= imx678_write_reg(dev, 0x417F, 0x01);
    ret |= imx678_write_reg(dev, 0x4186, 0xE0);
    ret |= imx678_write_reg(dev, 0x4190, 0xF3);
    ret |= imx678_write_reg(dev, 0x4192, 0xF7);
    ret |= imx678_write_reg(dev, 0x419C, 0x78);
    ret |= imx678_write_reg(dev, 0x419E, 0x7C);
    ret |= imx678_write_reg(dev, 0x41A0, 0xE5);
    ret |= imx678_write_reg(dev, 0x41A2, 0xE9);
    ret |= imx678_write_reg(dev, 0x41C8, 0xE2);
    ret |= imx678_write_reg(dev, 0x41CA, 0xFD);
    ret |= imx678_write_reg(dev, 0x41DC, 0x67);
    ret |= imx678_write_reg(dev, 0x41DE, 0x82);
    ret |= imx678_write_reg(dev, 0x41E0, 0xD4);
    ret |= imx678_write_reg(dev, 0x41E2, 0xEF);
    ret |= imx678_write_reg(dev, 0x4200, 0xDE);
    ret |= imx678_write_reg(dev, 0x4202, 0xDA);
    ret |= imx678_write_reg(dev, 0x4218, 0x63);
    ret |= imx678_write_reg(dev, 0x421A, 0x5F);
    ret |= imx678_write_reg(dev, 0x421C, 0xD0);
    ret |= imx678_write_reg(dev, 0x421E, 0xCC);
    ret |= imx678_write_reg(dev, 0x425A, 0x82);
    ret |= imx678_write_reg(dev, 0x425C, 0xEF);
    ret |= imx678_write_reg(dev, 0x4348, 0xFE);
    ret |= imx678_write_reg(dev, 0x4349, 0x06);
    ret |= imx678_write_reg(dev, 0x4352, 0xCE);
    ret |= imx678_write_reg(dev, 0x4420, 0x0B);
    ret |= imx678_write_reg(dev, 0x4421, 0x02);
    ret |= imx678_write_reg(dev, 0x4422, 0x4D);
    ret |= imx678_write_reg(dev, 0x4423, 0x0A);
    ret |= imx678_write_reg(dev, 0x4426, 0xF5);
    ret |= imx678_write_reg(dev, 0x442A, 0xE7);
    ret |= imx678_write_reg(dev, 0x4432, 0xF5);
    ret |= imx678_write_reg(dev, 0x4436, 0xE7);
    ret |= imx678_write_reg(dev, 0x4466, 0xB4);
    ret |= imx678_write_reg(dev, 0x446E, 0x32);
    ret |= imx678_write_reg(dev, 0x449F, 0x1C);
    ret |= imx678_write_reg(dev, 0x44A4, 0x2C);
    ret |= imx678_write_reg(dev, 0x44A6, 0x2C);
    ret |= imx678_write_reg(dev, 0x44A8, 0x2C);
    ret |= imx678_write_reg(dev, 0x44AA, 0x2C);
    ret |= imx678_write_reg(dev, 0x44B4, 0x2C);
    ret |= imx678_write_reg(dev, 0x44B6, 0x2C);
    ret |= imx678_write_reg(dev, 0x44B8, 0x2C);
    ret |= imx678_write_reg(dev, 0x44BA, 0x2C);
    ret |= imx678_write_reg(dev, 0x44C4, 0x2C);
    ret |= imx678_write_reg(dev, 0x44C6, 0x2C);
    ret |= imx678_write_reg(dev, 0x44C8, 0x2C);
    ret |= imx678_write_reg(dev, 0x4506, 0xF3);
    ret |= imx678_write_reg(dev, 0x450E, 0xE5);
    ret |= imx678_write_reg(dev, 0x4516, 0xF3);
    ret |= imx678_write_reg(dev, 0x4522, 0xE5);
    ret |= imx678_write_reg(dev, 0x4524, 0xF3);
    ret |= imx678_write_reg(dev, 0x452C, 0xE5);
    ret |= imx678_write_reg(dev, 0x453C, 0x22);
    ret |= imx678_write_reg(dev, 0x453D, 0x1B);
    ret |= imx678_write_reg(dev, 0x453E, 0x1B);
    ret |= imx678_write_reg(dev, 0x453F, 0x15);
    ret |= imx678_write_reg(dev, 0x4540, 0x15);
    ret |= imx678_write_reg(dev, 0x4541, 0x15);
    ret |= imx678_write_reg(dev, 0x4542, 0x15);
    ret |= imx678_write_reg(dev, 0x4543, 0x15);
    ret |= imx678_write_reg(dev, 0x4544, 0x15);
    ret |= imx678_write_reg(dev, 0x4548, 0x00);
    ret |= imx678_write_reg(dev, 0x4549, 0x01);
    ret |= imx678_write_reg(dev, 0x454A, 0x01);
    ret |= imx678_write_reg(dev, 0x454B, 0x06);
    ret |= imx678_write_reg(dev, 0x454C, 0x06);
    ret |= imx678_write_reg(dev, 0x454D, 0x06);
    ret |= imx678_write_reg(dev, 0x454E, 0x06);
    ret |= imx678_write_reg(dev, 0x454F, 0x06);
    ret |= imx678_write_reg(dev, 0x4550, 0x06);
    ret |= imx678_write_reg(dev, 0x4554, 0x55);
    ret |= imx678_write_reg(dev, 0x4555, 0x02);
    ret |= imx678_write_reg(dev, 0x4556, 0x42);
    ret |= imx678_write_reg(dev, 0x4557, 0x05);
    ret |= imx678_write_reg(dev, 0x4558, 0xFD);
    ret |= imx678_write_reg(dev, 0x4559, 0x05);
    ret |= imx678_write_reg(dev, 0x455A, 0x94);
    ret |= imx678_write_reg(dev, 0x455B, 0x06);
    ret |= imx678_write_reg(dev, 0x455D, 0x06);
    ret |= imx678_write_reg(dev, 0x455E, 0x49);
    ret |= imx678_write_reg(dev, 0x455F, 0x07);
    ret |= imx678_write_reg(dev, 0x4560, 0x7F);
    ret |= imx678_write_reg(dev, 0x4561, 0x07);
    ret |= imx678_write_reg(dev, 0x4562, 0xA5);
    ret |= imx678_write_reg(dev, 0x4564, 0x55);
    ret |= imx678_write_reg(dev, 0x4565, 0x02);
    ret |= imx678_write_reg(dev, 0x4566, 0x42);
    ret |= imx678_write_reg(dev, 0x4567, 0x05);
    ret |= imx678_write_reg(dev, 0x4568, 0xFD);
    ret |= imx678_write_reg(dev, 0x4569, 0x05);
    ret |= imx678_write_reg(dev, 0x456A, 0x94);
    ret |= imx678_write_reg(dev, 0x456B, 0x06);
    ret |= imx678_write_reg(dev, 0x456D, 0x06);
    ret |= imx678_write_reg(dev, 0x456E, 0x49);
    ret |= imx678_write_reg(dev, 0x456F, 0x07);
    ret |= imx678_write_reg(dev, 0x4572, 0xA5);
    ret |= imx678_write_reg(dev, 0x460C, 0x7D);
    ret |= imx678_write_reg(dev, 0x460E, 0xB1);
    ret |= imx678_write_reg(dev, 0x4614, 0xA8);
    ret |= imx678_write_reg(dev, 0x4616, 0xB2);
    ret |= imx678_write_reg(dev, 0x461C, 0x7E);
    ret |= imx678_write_reg(dev, 0x461E, 0xA7);
    ret |= imx678_write_reg(dev, 0x4624, 0xA8);
    ret |= imx678_write_reg(dev, 0x4626, 0xB2);
    ret |= imx678_write_reg(dev, 0x462C, 0x7E);
    ret |= imx678_write_reg(dev, 0x462E, 0x8A);
    ret |= imx678_write_reg(dev, 0x4630, 0x94);
    ret |= imx678_write_reg(dev, 0x4632, 0xA7);
    ret |= imx678_write_reg(dev, 0x4634, 0xFB);
    ret |= imx678_write_reg(dev, 0x4636, 0x2F);
    ret |= imx678_write_reg(dev, 0x4638, 0x81);
    ret |= imx678_write_reg(dev, 0x4639, 0x01);
    ret |= imx678_write_reg(dev, 0x463A, 0xB5);
    ret |= imx678_write_reg(dev, 0x463B, 0x01);
    ret |= imx678_write_reg(dev, 0x463C, 0x26);
    ret |= imx678_write_reg(dev, 0x463E, 0x30);
    ret |= imx678_write_reg(dev, 0x4640, 0xAC);
    ret |= imx678_write_reg(dev, 0x4641, 0x01);
    ret |= imx678_write_reg(dev, 0x4642, 0xB6);
    ret |= imx678_write_reg(dev, 0x4643, 0x01);
    ret |= imx678_write_reg(dev, 0x4644, 0xFC);
    ret |= imx678_write_reg(dev, 0x4646, 0x25);
    ret |= imx678_write_reg(dev, 0x4648, 0x82);
    ret |= imx678_write_reg(dev, 0x4649, 0x01);
    ret |= imx678_write_reg(dev, 0x464A, 0xAB);
    ret |= imx678_write_reg(dev, 0x464B, 0x01);
    ret |= imx678_write_reg(dev, 0x464C, 0x26);
    ret |= imx678_write_reg(dev, 0x464E, 0x30);
    ret |= imx678_write_reg(dev, 0x4654, 0xFC);
    ret |= imx678_write_reg(dev, 0x4656, 0x08);
    ret |= imx678_write_reg(dev, 0x4658, 0x12);
    ret |= imx678_write_reg(dev, 0x465A, 0x25);
    ret |= imx678_write_reg(dev, 0x4662, 0xFC);
    ret |= imx678_write_reg(dev, 0x46A2, 0xFB);
    ret |= imx678_write_reg(dev, 0x46D6, 0xF3);
    ret |= imx678_write_reg(dev, 0x46E6, 0x00);
    ret |= imx678_write_reg(dev, 0x46E8, 0xFF);
    ret |= imx678_write_reg(dev, 0x46E9, 0x03);
    ret |= imx678_write_reg(dev, 0x46EC, 0x7A);
    ret |= imx678_write_reg(dev, 0x46EE, 0xE5);
    ret |= imx678_write_reg(dev, 0x46F4, 0xEE);
    ret |= imx678_write_reg(dev, 0x46F6, 0xF2);
    ret |= imx678_write_reg(dev, 0x470C, 0xFF);
    ret |= imx678_write_reg(dev, 0x470D, 0x03);
    ret |= imx678_write_reg(dev, 0x470E, 0x00);
    ret |= imx678_write_reg(dev, 0x4714, 0xE0);
    ret |= imx678_write_reg(dev, 0x4716, 0xE4);
    ret |= imx678_write_reg(dev, 0x471E, 0xED);
    ret |= imx678_write_reg(dev, 0x472E, 0x00);
    ret |= imx678_write_reg(dev, 0x4730, 0xFF);
    ret |= imx678_write_reg(dev, 0x4731, 0x03);
    ret |= imx678_write_reg(dev, 0x4734, 0x7B);
    ret |= imx678_write_reg(dev, 0x4736, 0xDF);
    ret |= imx678_write_reg(dev, 0x4754, 0x7D);
    ret |= imx678_write_reg(dev, 0x4756, 0x8B);
    ret |= imx678_write_reg(dev, 0x4758, 0x93);
    ret |= imx678_write_reg(dev, 0x475A, 0xB1);
    ret |= imx678_write_reg(dev, 0x475C, 0xFB);
    ret |= imx678_write_reg(dev, 0x475E, 0x09);
    ret |= imx678_write_reg(dev, 0x4760, 0x11);
    ret |= imx678_write_reg(dev, 0x4762, 0x2F);
    ret |= imx678_write_reg(dev, 0x4766, 0xCC);
    ret |= imx678_write_reg(dev, 0x4776, 0xCB);
    ret |= imx678_write_reg(dev, 0x477E, 0x4A);
    ret |= imx678_write_reg(dev, 0x478E, 0x49);
    ret |= imx678_write_reg(dev, 0x4794, 0x7C);
    ret |= imx678_write_reg(dev, 0x4796, 0x8F);
    ret |= imx678_write_reg(dev, 0x4798, 0xB3);
    ret |= imx678_write_reg(dev, 0x4799, 0x00);
    ret |= imx678_write_reg(dev, 0x479A, 0xCC);
    ret |= imx678_write_reg(dev, 0x479C, 0xC1);
    ret |= imx678_write_reg(dev, 0x479E, 0xCB);
    ret |= imx678_write_reg(dev, 0x47A4, 0x7D);
    ret |= imx678_write_reg(dev, 0x47A6, 0x8E);
    ret |= imx678_write_reg(dev, 0x47A8, 0xB4);
    ret |= imx678_write_reg(dev, 0x47A9, 0x00);
    ret |= imx678_write_reg(dev, 0x47AA, 0xC0);
    ret |= imx678_write_reg(dev, 0x47AC, 0xFA);
    ret |= imx678_write_reg(dev, 0x47AE, 0x0D);
    ret |= imx678_write_reg(dev, 0x47B0, 0x31);
    ret |= imx678_write_reg(dev, 0x47B1, 0x01);
    ret |= imx678_write_reg(dev, 0x47B2, 0x4A);
    ret |= imx678_write_reg(dev, 0x47B3, 0x01);
    ret |= imx678_write_reg(dev, 0x47B4, 0x3F);
    ret |= imx678_write_reg(dev, 0x47B6, 0x49);
    ret |= imx678_write_reg(dev, 0x47BC, 0xFB);
    ret |= imx678_write_reg(dev, 0x47BE, 0x0C);
    ret |= imx678_write_reg(dev, 0x47C0, 0x32);
    ret |= imx678_write_reg(dev, 0x47C1, 0x01);
    ret |= imx678_write_reg(dev, 0x47C2, 0x3E);
    ret |= imx678_write_reg(dev, 0x47C3, 0x01);

    ret |= imx678_write_reg(dev, 0x3000, 0x00);
    imx678_delay_ms(20);
    ret |= imx678_write_reg(dev, 0x3002, 0x00);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "imx678_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=================================================================================\n");
    SENSOR_PRINT("=== IMX678_37.125MInput_MIPI_4lane_12bit_3840x2160_30fps_LINEAR init success!====\n");
    SENSOR_PRINT("=================================================================================\n");

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx678_4lane_wdr_3840x2160_init(xmedia_u32 dev)
{
    // 10bit Register
    //All-pixel scan
    //CSI-2_4lane
    //37.125MHz
    //AD:10bit Output:10bit
    //1440Mbps
    //Master Mode
    //LCG Mode
    //DOL HDR 2frame VC
    //30fps
    //Integration Time
    //LEF:16ms
    //SEF:1.007ms

    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= imx678_write_reg(dev, 0x3000, 0x01);
    ret |= imx678_write_reg(dev, 0x3001, 0x00);
    ret |= imx678_write_reg(dev, 0x3002, 0x01);
    ret |= imx678_write_reg(dev, 0x3014, 0x01);
    ret |= imx678_write_reg(dev, 0x3015, 0x03);
    ret |= imx678_write_reg(dev, 0x3018, 0x00);
    ret |= imx678_write_reg(dev, 0x301A, 0x01);
    ret |= imx678_write_reg(dev, 0x301B, 0x00);
    ret |= imx678_write_reg(dev, 0x301C, 0x01);
    ret |= imx678_write_reg(dev, 0x301E, 0x01);
    ret |= imx678_write_reg(dev, 0x3020, 0x00);
    ret |= imx678_write_reg(dev, 0x3021, 0x00);
    ret |= imx678_write_reg(dev, 0x3022, 0x00);
    ret |= imx678_write_reg(dev, 0x3023, 0x00);
    ret |= imx678_write_reg(dev, 0x3028, 0xca);
    ret |= imx678_write_reg(dev, 0x3029, 0x08);
    ret |= imx678_write_reg(dev, 0x302A, 0x00);
    ret |= imx678_write_reg(dev, 0x302C, 0x26);
    ret |= imx678_write_reg(dev, 0x302D, 0x02);
    ret |= imx678_write_reg(dev, 0x3030, 0x00);
    ret |= imx678_write_reg(dev, 0x3031, 0x00);
    ret |= imx678_write_reg(dev, 0x3032, 0x00);
    ret |= imx678_write_reg(dev, 0x303C, 0x00);
    ret |= imx678_write_reg(dev, 0x303D, 0x00);
    ret |= imx678_write_reg(dev, 0x303E, 0x10);
    ret |= imx678_write_reg(dev, 0x303F, 0x0F);
    ret |= imx678_write_reg(dev, 0x3040, 0x03);
    ret |= imx678_write_reg(dev, 0x3042, 0x00);
    ret |= imx678_write_reg(dev, 0x3043, 0x00);
    ret |= imx678_write_reg(dev, 0x3044, 0x00);
    ret |= imx678_write_reg(dev, 0x3045, 0x00);
    ret |= imx678_write_reg(dev, 0x3046, 0x84);
    ret |= imx678_write_reg(dev, 0x3047, 0x08);
    ret |= imx678_write_reg(dev, 0x3050, 0x24);
    ret |= imx678_write_reg(dev, 0x3051, 0x09);
    ret |= imx678_write_reg(dev, 0x3052, 0x00);
    ret |= imx678_write_reg(dev, 0x3054, 0x05);
    ret |= imx678_write_reg(dev, 0x3055, 0x00);
    ret |= imx678_write_reg(dev, 0x3056, 0x00);
    ret |= imx678_write_reg(dev, 0x3058, 0x8A);
    ret |= imx678_write_reg(dev, 0x3059, 0x01);
    ret |= imx678_write_reg(dev, 0x305A, 0x00);
    ret |= imx678_write_reg(dev, 0x3060, 0x8D);
    ret |= imx678_write_reg(dev, 0x3061, 0x00);
    ret |= imx678_write_reg(dev, 0x3062, 0x00);
    ret |= imx678_write_reg(dev, 0x3064, 0xC4);
    ret |= imx678_write_reg(dev, 0x3065, 0x0C);
    ret |= imx678_write_reg(dev, 0x3066, 0x00);
    ret |= imx678_write_reg(dev, 0x3069, 0x00);
    ret |= imx678_write_reg(dev, 0x306B, 0x00);
    ret |= imx678_write_reg(dev, 0x3070, 0x00);
    ret |= imx678_write_reg(dev, 0x3071, 0x00);
    ret |= imx678_write_reg(dev, 0x3072, 0x00);
    ret |= imx678_write_reg(dev, 0x3073, 0x00);
    ret |= imx678_write_reg(dev, 0x3074, 0x00);
    ret |= imx678_write_reg(dev, 0x3075, 0x00);
    ret |= imx678_write_reg(dev, 0x3081, 0x00);
    ret |= imx678_write_reg(dev, 0x308C, 0x00);
    ret |= imx678_write_reg(dev, 0x308D, 0x01);
    ret |= imx678_write_reg(dev, 0x3094, 0x00);
    ret |= imx678_write_reg(dev, 0x3095, 0x00);
    ret |= imx678_write_reg(dev, 0x309C, 0x00);
    ret |= imx678_write_reg(dev, 0x309D, 0x00);
    ret |= imx678_write_reg(dev, 0x30A4, 0xAA);
    ret |= imx678_write_reg(dev, 0x30A6, 0x00);
    ret |= imx678_write_reg(dev, 0x30CC, 0x00);
    ret |= imx678_write_reg(dev, 0x30CD, 0x00);
    ret |= imx678_write_reg(dev, 0x30DC, 0x32);
    ret |= imx678_write_reg(dev, 0x30DD, 0x40);
    ret |= imx678_write_reg(dev, 0x3400, 0x01);
    ret |= imx678_write_reg(dev, 0x3460, 0x22);
    ret |= imx678_write_reg(dev, 0x355A, 0x64);
    ret |= imx678_write_reg(dev, 0x3A02, 0x7A);
    ret |= imx678_write_reg(dev, 0x3A10, 0xEC);
    ret |= imx678_write_reg(dev, 0x3A12, 0x71);
    ret |= imx678_write_reg(dev, 0x3A14, 0xDE);
    ret |= imx678_write_reg(dev, 0x3A20, 0x2B);
    ret |= imx678_write_reg(dev, 0x3A24, 0x22);
    ret |= imx678_write_reg(dev, 0x3A25, 0x25);
    ret |= imx678_write_reg(dev, 0x3A26, 0x2A);
    ret |= imx678_write_reg(dev, 0x3A27, 0x2C);
    ret |= imx678_write_reg(dev, 0x3A28, 0x39);
    ret |= imx678_write_reg(dev, 0x3A29, 0x38);
    ret |= imx678_write_reg(dev, 0x3A30, 0x04);
    ret |= imx678_write_reg(dev, 0x3A31, 0x04);
    ret |= imx678_write_reg(dev, 0x3A32, 0x03);
    ret |= imx678_write_reg(dev, 0x3A33, 0x03);
    ret |= imx678_write_reg(dev, 0x3A34, 0x09);
    ret |= imx678_write_reg(dev, 0x3A35, 0x06);
    ret |= imx678_write_reg(dev, 0x3A38, 0xCD);
    ret |= imx678_write_reg(dev, 0x3A3A, 0x4C);
    ret |= imx678_write_reg(dev, 0x3A3C, 0xB9);
    ret |= imx678_write_reg(dev, 0x3A3E, 0x30);
    ret |= imx678_write_reg(dev, 0x3A40, 0x2C);
    ret |= imx678_write_reg(dev, 0x3A42, 0x39);
    ret |= imx678_write_reg(dev, 0x3A4E, 0x00);
    ret |= imx678_write_reg(dev, 0x3A52, 0x00);
    ret |= imx678_write_reg(dev, 0x3A56, 0x00);
    ret |= imx678_write_reg(dev, 0x3A5A, 0x00);
    ret |= imx678_write_reg(dev, 0x3A5E, 0x00);
    ret |= imx678_write_reg(dev, 0x3A62, 0x00);
    ret |= imx678_write_reg(dev, 0x3A64, 0x00);
    ret |= imx678_write_reg(dev, 0x3A6E, 0xA0);
    ret |= imx678_write_reg(dev, 0x3A70, 0x50);
    ret |= imx678_write_reg(dev, 0x3A8C, 0x04);
    ret |= imx678_write_reg(dev, 0x3A8D, 0x03);
    ret |= imx678_write_reg(dev, 0x3A8E, 0x09);
    ret |= imx678_write_reg(dev, 0x3A90, 0x38);
    ret |= imx678_write_reg(dev, 0x3A91, 0x42);
    ret |= imx678_write_reg(dev, 0x3A92, 0x3C);
    ret |= imx678_write_reg(dev, 0x3B0E, 0xF3);
    ret |= imx678_write_reg(dev, 0x3B12, 0xE5);
    ret |= imx678_write_reg(dev, 0x3B27, 0xC0);
    ret |= imx678_write_reg(dev, 0x3B2E, 0xEF);
    ret |= imx678_write_reg(dev, 0x3B30, 0x6A);
    ret |= imx678_write_reg(dev, 0x3B32, 0xF6);
    ret |= imx678_write_reg(dev, 0x3B36, 0xE1);
    ret |= imx678_write_reg(dev, 0x3B3A, 0xE8);
    ret |= imx678_write_reg(dev, 0x3B5A, 0x17);
    ret |= imx678_write_reg(dev, 0x3B5E, 0xEF);
    ret |= imx678_write_reg(dev, 0x3B60, 0x6A);
    ret |= imx678_write_reg(dev, 0x3B62, 0xF6);
    ret |= imx678_write_reg(dev, 0x3B66, 0xE1);
    ret |= imx678_write_reg(dev, 0x3B6A, 0xE8);
    ret |= imx678_write_reg(dev, 0x3B88, 0xEC);
    ret |= imx678_write_reg(dev, 0x3B8A, 0xED);
    ret |= imx678_write_reg(dev, 0x3B94, 0x71);
    ret |= imx678_write_reg(dev, 0x3B96, 0x72);
    ret |= imx678_write_reg(dev, 0x3B98, 0xDE);
    ret |= imx678_write_reg(dev, 0x3B9A, 0xDF);
    ret |= imx678_write_reg(dev, 0x3C0F, 0x06);
    ret |= imx678_write_reg(dev, 0x3C10, 0x06);
    ret |= imx678_write_reg(dev, 0x3C11, 0x06);
    ret |= imx678_write_reg(dev, 0x3C12, 0x06);
    ret |= imx678_write_reg(dev, 0x3C13, 0x06);
    ret |= imx678_write_reg(dev, 0x3C18, 0x20);
    ret |= imx678_write_reg(dev, 0x3C37, 0x10);
    ret |= imx678_write_reg(dev, 0x3C3A, 0x7A);
    ret |= imx678_write_reg(dev, 0x3C40, 0xF4);
    ret |= imx678_write_reg(dev, 0x3C48, 0xE6);
    ret |= imx678_write_reg(dev, 0x3C54, 0xCE);
    ret |= imx678_write_reg(dev, 0x3C56, 0xD0);
    ret |= imx678_write_reg(dev, 0x3C6C, 0x53);
    ret |= imx678_write_reg(dev, 0x3C6E, 0x55);
    ret |= imx678_write_reg(dev, 0x3C70, 0xC0);
    ret |= imx678_write_reg(dev, 0x3C72, 0xC2);
    ret |= imx678_write_reg(dev, 0x3C7E, 0xCE);
    ret |= imx678_write_reg(dev, 0x3C8C, 0xCF);
    ret |= imx678_write_reg(dev, 0x3C8E, 0xEB);
    ret |= imx678_write_reg(dev, 0x3C98, 0x54);
    ret |= imx678_write_reg(dev, 0x3C9A, 0x70);
    ret |= imx678_write_reg(dev, 0x3C9C, 0xC1);
    ret |= imx678_write_reg(dev, 0x3C9E, 0xDD);
    ret |= imx678_write_reg(dev, 0x3CB0, 0x7A);
    ret |= imx678_write_reg(dev, 0x3CB2, 0xBA);
    ret |= imx678_write_reg(dev, 0x3CC8, 0xBC);
    ret |= imx678_write_reg(dev, 0x3CCA, 0x7C);
    ret |= imx678_write_reg(dev, 0x3CD4, 0xEA);
    ret |= imx678_write_reg(dev, 0x3CD5, 0x01);
    ret |= imx678_write_reg(dev, 0x3CD6, 0x4A);
    ret |= imx678_write_reg(dev, 0x3CD8, 0x00);
    ret |= imx678_write_reg(dev, 0x3CD9, 0x00);
    ret |= imx678_write_reg(dev, 0x3CDA, 0xFF);
    ret |= imx678_write_reg(dev, 0x3CDB, 0x03);
    ret |= imx678_write_reg(dev, 0x3CDC, 0x00);
    ret |= imx678_write_reg(dev, 0x3CDD, 0x00);
    ret |= imx678_write_reg(dev, 0x3CDE, 0xFF);
    ret |= imx678_write_reg(dev, 0x3CDF, 0x03);
    ret |= imx678_write_reg(dev, 0x3CE4, 0x4C);
    ret |= imx678_write_reg(dev, 0x3CE6, 0xEC);
    ret |= imx678_write_reg(dev, 0x3CE7, 0x01);
    ret |= imx678_write_reg(dev, 0x3CE8, 0xFF);
    ret |= imx678_write_reg(dev, 0x3CE9, 0x03);
    ret |= imx678_write_reg(dev, 0x3CEA, 0x00);
    ret |= imx678_write_reg(dev, 0x3CEB, 0x00);
    ret |= imx678_write_reg(dev, 0x3CEC, 0xFF);
    ret |= imx678_write_reg(dev, 0x3CED, 0x03);
    ret |= imx678_write_reg(dev, 0x3CEE, 0x00);
    ret |= imx678_write_reg(dev, 0x3CEF, 0x00);
    ret |= imx678_write_reg(dev, 0x3CF2, 0xFF);
    ret |= imx678_write_reg(dev, 0x3CF3, 0x03);
    ret |= imx678_write_reg(dev, 0x3CF4, 0x00);
    ret |= imx678_write_reg(dev, 0x3E28, 0x82);
    ret |= imx678_write_reg(dev, 0x3E2A, 0x80);
    ret |= imx678_write_reg(dev, 0x3E30, 0x85);
    ret |= imx678_write_reg(dev, 0x3E32, 0x7D);
    ret |= imx678_write_reg(dev, 0x3E5C, 0xCE);
    ret |= imx678_write_reg(dev, 0x3E5E, 0xD3);
    ret |= imx678_write_reg(dev, 0x3E70, 0x53);
    ret |= imx678_write_reg(dev, 0x3E72, 0x58);
    ret |= imx678_write_reg(dev, 0x3E74, 0xC0);
    ret |= imx678_write_reg(dev, 0x3E76, 0xC5);
    ret |= imx678_write_reg(dev, 0x3E78, 0xC0);
    ret |= imx678_write_reg(dev, 0x3E79, 0x01);
    ret |= imx678_write_reg(dev, 0x3E7A, 0xD4);
    ret |= imx678_write_reg(dev, 0x3E7B, 0x01);
    ret |= imx678_write_reg(dev, 0x3EB4, 0x0B);
    ret |= imx678_write_reg(dev, 0x3EB5, 0x02);
    ret |= imx678_write_reg(dev, 0x3EB6, 0x4D);
    ret |= imx678_write_reg(dev, 0x3EB7, 0x42);
    ret |= imx678_write_reg(dev, 0x3EEC, 0xF3);
    ret |= imx678_write_reg(dev, 0x3EEE, 0xE7);
    ret |= imx678_write_reg(dev, 0x3F01, 0x01);
    ret |= imx678_write_reg(dev, 0x3F24, 0x10);
    ret |= imx678_write_reg(dev, 0x3F28, 0x2D);
    ret |= imx678_write_reg(dev, 0x3F2A, 0x2D);
    ret |= imx678_write_reg(dev, 0x3F2C, 0x2D);
    ret |= imx678_write_reg(dev, 0x3F2E, 0x2D);
    ret |= imx678_write_reg(dev, 0x3F30, 0x23);
    ret |= imx678_write_reg(dev, 0x3F38, 0x2D);
    ret |= imx678_write_reg(dev, 0x3F3A, 0x2D);
    ret |= imx678_write_reg(dev, 0x3F3C, 0x2D);
    ret |= imx678_write_reg(dev, 0x3F3E, 0x28);
    ret |= imx678_write_reg(dev, 0x3F40, 0x1E);
    ret |= imx678_write_reg(dev, 0x3F48, 0x2D);
    ret |= imx678_write_reg(dev, 0x3F4A, 0x2D);
    ret |= imx678_write_reg(dev, 0x3F4C, 0x00);
    ret |= imx678_write_reg(dev, 0x4004, 0xE4);
    ret |= imx678_write_reg(dev, 0x4006, 0xFF);
    ret |= imx678_write_reg(dev, 0x4018, 0x69);
    ret |= imx678_write_reg(dev, 0x401A, 0x84);
    ret |= imx678_write_reg(dev, 0x401C, 0xD6);
    ret |= imx678_write_reg(dev, 0x401E, 0xF1);
    ret |= imx678_write_reg(dev, 0x4038, 0xDE);
    ret |= imx678_write_reg(dev, 0x403A, 0x00);
    ret |= imx678_write_reg(dev, 0x403B, 0x01);
    ret |= imx678_write_reg(dev, 0x404C, 0x63);
    ret |= imx678_write_reg(dev, 0x404E, 0x85);
    ret |= imx678_write_reg(dev, 0x4050, 0xD0);
    ret |= imx678_write_reg(dev, 0x4052, 0xF2);
    ret |= imx678_write_reg(dev, 0x4108, 0xDD);
    ret |= imx678_write_reg(dev, 0x410A, 0xF7);
    ret |= imx678_write_reg(dev, 0x411C, 0x62);
    ret |= imx678_write_reg(dev, 0x411E, 0x7C);
    ret |= imx678_write_reg(dev, 0x4120, 0xCF);
    ret |= imx678_write_reg(dev, 0x4122, 0xE9);
    ret |= imx678_write_reg(dev, 0x4138, 0xE6);
    ret |= imx678_write_reg(dev, 0x413A, 0xF1);
    ret |= imx678_write_reg(dev, 0x414C, 0x6B);
    ret |= imx678_write_reg(dev, 0x414E, 0x76);
    ret |= imx678_write_reg(dev, 0x4150, 0xD8);
    ret |= imx678_write_reg(dev, 0x4152, 0xE3);
    ret |= imx678_write_reg(dev, 0x417E, 0x03);
    ret |= imx678_write_reg(dev, 0x417F, 0x01);
    ret |= imx678_write_reg(dev, 0x4186, 0xE0);
    ret |= imx678_write_reg(dev, 0x4190, 0xF3);
    ret |= imx678_write_reg(dev, 0x4192, 0xF7);
    ret |= imx678_write_reg(dev, 0x419C, 0x78);
    ret |= imx678_write_reg(dev, 0x419E, 0x7C);
    ret |= imx678_write_reg(dev, 0x41A0, 0xE5);
    ret |= imx678_write_reg(dev, 0x41A2, 0xE9);
    ret |= imx678_write_reg(dev, 0x41C8, 0xE2);
    ret |= imx678_write_reg(dev, 0x41CA, 0xFD);
    ret |= imx678_write_reg(dev, 0x41DC, 0x67);
    ret |= imx678_write_reg(dev, 0x41DE, 0x82);
    ret |= imx678_write_reg(dev, 0x41E0, 0xD4);
    ret |= imx678_write_reg(dev, 0x41E2, 0xEF);
    ret |= imx678_write_reg(dev, 0x4200, 0xDE);
    ret |= imx678_write_reg(dev, 0x4202, 0xDA);
    ret |= imx678_write_reg(dev, 0x4218, 0x63);
    ret |= imx678_write_reg(dev, 0x421A, 0x5F);
    ret |= imx678_write_reg(dev, 0x421C, 0xD0);
    ret |= imx678_write_reg(dev, 0x421E, 0xCC);
    ret |= imx678_write_reg(dev, 0x425A, 0x82);
    ret |= imx678_write_reg(dev, 0x425C, 0xEF);
    ret |= imx678_write_reg(dev, 0x4348, 0xFE);
    ret |= imx678_write_reg(dev, 0x4349, 0x06);
    ret |= imx678_write_reg(dev, 0x4352, 0xCE);
    ret |= imx678_write_reg(dev, 0x4420, 0x0B);
    ret |= imx678_write_reg(dev, 0x4421, 0x02);
    ret |= imx678_write_reg(dev, 0x4422, 0x4D);
    ret |= imx678_write_reg(dev, 0x4423, 0x0A);
    ret |= imx678_write_reg(dev, 0x4426, 0xF5);
    ret |= imx678_write_reg(dev, 0x442A, 0xE7);
    ret |= imx678_write_reg(dev, 0x4432, 0xF5);
    ret |= imx678_write_reg(dev, 0x4436, 0xE7);
    ret |= imx678_write_reg(dev, 0x4466, 0xB4);
    ret |= imx678_write_reg(dev, 0x446E, 0x32);
    ret |= imx678_write_reg(dev, 0x449F, 0x1C);
    ret |= imx678_write_reg(dev, 0x44A4, 0x2C);
    ret |= imx678_write_reg(dev, 0x44A6, 0x2C);
    ret |= imx678_write_reg(dev, 0x44A8, 0x2C);
    ret |= imx678_write_reg(dev, 0x44AA, 0x2C);
    ret |= imx678_write_reg(dev, 0x44B4, 0x2C);
    ret |= imx678_write_reg(dev, 0x44B6, 0x2C);
    ret |= imx678_write_reg(dev, 0x44B8, 0x2C);
    ret |= imx678_write_reg(dev, 0x44BA, 0x2C);
    ret |= imx678_write_reg(dev, 0x44C4, 0x2C);
    ret |= imx678_write_reg(dev, 0x44C6, 0x2C);
    ret |= imx678_write_reg(dev, 0x44C8, 0x2C);
    ret |= imx678_write_reg(dev, 0x4506, 0xF3);
    ret |= imx678_write_reg(dev, 0x450E, 0xE5);
    ret |= imx678_write_reg(dev, 0x4516, 0xF3);
    ret |= imx678_write_reg(dev, 0x4522, 0xE5);
    ret |= imx678_write_reg(dev, 0x4524, 0xF3);
    ret |= imx678_write_reg(dev, 0x452C, 0xE5);
    ret |= imx678_write_reg(dev, 0x453C, 0x22);
    ret |= imx678_write_reg(dev, 0x453D, 0x1B);
    ret |= imx678_write_reg(dev, 0x453E, 0x1B);
    ret |= imx678_write_reg(dev, 0x453F, 0x15);
    ret |= imx678_write_reg(dev, 0x4540, 0x15);
    ret |= imx678_write_reg(dev, 0x4541, 0x15);
    ret |= imx678_write_reg(dev, 0x4542, 0x15);
    ret |= imx678_write_reg(dev, 0x4543, 0x15);
    ret |= imx678_write_reg(dev, 0x4544, 0x15);
    ret |= imx678_write_reg(dev, 0x4548, 0x00);
    ret |= imx678_write_reg(dev, 0x4549, 0x01);
    ret |= imx678_write_reg(dev, 0x454A, 0x01);
    ret |= imx678_write_reg(dev, 0x454B, 0x06);
    ret |= imx678_write_reg(dev, 0x454C, 0x06);
    ret |= imx678_write_reg(dev, 0x454D, 0x06);
    ret |= imx678_write_reg(dev, 0x454E, 0x06);
    ret |= imx678_write_reg(dev, 0x454F, 0x06);
    ret |= imx678_write_reg(dev, 0x4550, 0x06);
    ret |= imx678_write_reg(dev, 0x4554, 0x55);
    ret |= imx678_write_reg(dev, 0x4555, 0x02);
    ret |= imx678_write_reg(dev, 0x4556, 0x42);
    ret |= imx678_write_reg(dev, 0x4557, 0x05);
    ret |= imx678_write_reg(dev, 0x4558, 0xFD);
    ret |= imx678_write_reg(dev, 0x4559, 0x05);
    ret |= imx678_write_reg(dev, 0x455A, 0x94);
    ret |= imx678_write_reg(dev, 0x455B, 0x06);
    ret |= imx678_write_reg(dev, 0x455D, 0x06);
    ret |= imx678_write_reg(dev, 0x455E, 0x49);
    ret |= imx678_write_reg(dev, 0x455F, 0x07);
    ret |= imx678_write_reg(dev, 0x4560, 0x7F);
    ret |= imx678_write_reg(dev, 0x4561, 0x07);
    ret |= imx678_write_reg(dev, 0x4562, 0xA5);
    ret |= imx678_write_reg(dev, 0x4564, 0x55);
    ret |= imx678_write_reg(dev, 0x4565, 0x02);
    ret |= imx678_write_reg(dev, 0x4566, 0x42);
    ret |= imx678_write_reg(dev, 0x4567, 0x05);
    ret |= imx678_write_reg(dev, 0x4568, 0xFD);
    ret |= imx678_write_reg(dev, 0x4569, 0x05);
    ret |= imx678_write_reg(dev, 0x456A, 0x94);
    ret |= imx678_write_reg(dev, 0x456B, 0x06);
    ret |= imx678_write_reg(dev, 0x456D, 0x06);
    ret |= imx678_write_reg(dev, 0x456E, 0x49);
    ret |= imx678_write_reg(dev, 0x456F, 0x07);
    ret |= imx678_write_reg(dev, 0x4572, 0xA5);
    ret |= imx678_write_reg(dev, 0x460C, 0x7D);
    ret |= imx678_write_reg(dev, 0x460E, 0xB1);
    ret |= imx678_write_reg(dev, 0x4614, 0xA8);
    ret |= imx678_write_reg(dev, 0x4616, 0xB2);
    ret |= imx678_write_reg(dev, 0x461C, 0x7E);
    ret |= imx678_write_reg(dev, 0x461E, 0xA7);
    ret |= imx678_write_reg(dev, 0x4624, 0xA8);
    ret |= imx678_write_reg(dev, 0x4626, 0xB2);
    ret |= imx678_write_reg(dev, 0x462C, 0x7E);
    ret |= imx678_write_reg(dev, 0x462E, 0x8A);
    ret |= imx678_write_reg(dev, 0x4630, 0x94);
    ret |= imx678_write_reg(dev, 0x4632, 0xA7);
    ret |= imx678_write_reg(dev, 0x4634, 0xFB);
    ret |= imx678_write_reg(dev, 0x4636, 0x2F);
    ret |= imx678_write_reg(dev, 0x4638, 0x81);
    ret |= imx678_write_reg(dev, 0x4639, 0x01);
    ret |= imx678_write_reg(dev, 0x463A, 0xB5);
    ret |= imx678_write_reg(dev, 0x463B, 0x01);
    ret |= imx678_write_reg(dev, 0x463C, 0x26);
    ret |= imx678_write_reg(dev, 0x463E, 0x30);
    ret |= imx678_write_reg(dev, 0x4640, 0xAC);
    ret |= imx678_write_reg(dev, 0x4641, 0x01);
    ret |= imx678_write_reg(dev, 0x4642, 0xB6);
    ret |= imx678_write_reg(dev, 0x4643, 0x01);
    ret |= imx678_write_reg(dev, 0x4644, 0xFC);
    ret |= imx678_write_reg(dev, 0x4646, 0x25);
    ret |= imx678_write_reg(dev, 0x4648, 0x82);
    ret |= imx678_write_reg(dev, 0x4649, 0x01);
    ret |= imx678_write_reg(dev, 0x464A, 0xAB);
    ret |= imx678_write_reg(dev, 0x464B, 0x01);
    ret |= imx678_write_reg(dev, 0x464C, 0x26);
    ret |= imx678_write_reg(dev, 0x464E, 0x30);
    ret |= imx678_write_reg(dev, 0x4654, 0xFC);
    ret |= imx678_write_reg(dev, 0x4656, 0x08);
    ret |= imx678_write_reg(dev, 0x4658, 0x12);
    ret |= imx678_write_reg(dev, 0x465A, 0x25);
    ret |= imx678_write_reg(dev, 0x4662, 0xFC);
    ret |= imx678_write_reg(dev, 0x46A2, 0xFB);
    ret |= imx678_write_reg(dev, 0x46D6, 0xF3);
    ret |= imx678_write_reg(dev, 0x46E6, 0x00);
    ret |= imx678_write_reg(dev, 0x46E8, 0xFF);
    ret |= imx678_write_reg(dev, 0x46E9, 0x03);
    ret |= imx678_write_reg(dev, 0x46EC, 0x7A);
    ret |= imx678_write_reg(dev, 0x46EE, 0xE5);
    ret |= imx678_write_reg(dev, 0x46F4, 0xEE);
    ret |= imx678_write_reg(dev, 0x46F6, 0xF2);
    ret |= imx678_write_reg(dev, 0x470C, 0xFF);
    ret |= imx678_write_reg(dev, 0x470D, 0x03);
    ret |= imx678_write_reg(dev, 0x470E, 0x00);
    ret |= imx678_write_reg(dev, 0x4714, 0xE0);
    ret |= imx678_write_reg(dev, 0x4716, 0xE4);
    ret |= imx678_write_reg(dev, 0x471E, 0xED);
    ret |= imx678_write_reg(dev, 0x472E, 0x00);
    ret |= imx678_write_reg(dev, 0x4730, 0xFF);
    ret |= imx678_write_reg(dev, 0x4731, 0x03);
    ret |= imx678_write_reg(dev, 0x4734, 0x7B);
    ret |= imx678_write_reg(dev, 0x4736, 0xDF);
    ret |= imx678_write_reg(dev, 0x4754, 0x7D);
    ret |= imx678_write_reg(dev, 0x4756, 0x8B);
    ret |= imx678_write_reg(dev, 0x4758, 0x93);
    ret |= imx678_write_reg(dev, 0x475A, 0xB1);
    ret |= imx678_write_reg(dev, 0x475C, 0xFB);
    ret |= imx678_write_reg(dev, 0x475E, 0x09);
    ret |= imx678_write_reg(dev, 0x4760, 0x11);
    ret |= imx678_write_reg(dev, 0x4762, 0x2F);
    ret |= imx678_write_reg(dev, 0x4766, 0xCC);
    ret |= imx678_write_reg(dev, 0x4776, 0xCB);
    ret |= imx678_write_reg(dev, 0x477E, 0x4A);
    ret |= imx678_write_reg(dev, 0x478E, 0x49);
    ret |= imx678_write_reg(dev, 0x4794, 0x7C);
    ret |= imx678_write_reg(dev, 0x4796, 0x8F);
    ret |= imx678_write_reg(dev, 0x4798, 0xB3);
    ret |= imx678_write_reg(dev, 0x4799, 0x00);
    ret |= imx678_write_reg(dev, 0x479A, 0xCC);
    ret |= imx678_write_reg(dev, 0x479C, 0xC1);
    ret |= imx678_write_reg(dev, 0x479E, 0xCB);
    ret |= imx678_write_reg(dev, 0x47A4, 0x7D);
    ret |= imx678_write_reg(dev, 0x47A6, 0x8E);
    ret |= imx678_write_reg(dev, 0x47A8, 0xB4);
    ret |= imx678_write_reg(dev, 0x47A9, 0x00);
    ret |= imx678_write_reg(dev, 0x47AA, 0xC0);
    ret |= imx678_write_reg(dev, 0x47AC, 0xFA);
    ret |= imx678_write_reg(dev, 0x47AE, 0x0D);
    ret |= imx678_write_reg(dev, 0x47B0, 0x31);
    ret |= imx678_write_reg(dev, 0x47B1, 0x01);
    ret |= imx678_write_reg(dev, 0x47B2, 0x4A);
    ret |= imx678_write_reg(dev, 0x47B3, 0x01);
    ret |= imx678_write_reg(dev, 0x47B4, 0x3F);
    ret |= imx678_write_reg(dev, 0x47B6, 0x49);
    ret |= imx678_write_reg(dev, 0x47BC, 0xFB);
    ret |= imx678_write_reg(dev, 0x47BE, 0x0C);
    ret |= imx678_write_reg(dev, 0x47C0, 0x32);
    ret |= imx678_write_reg(dev, 0x47C1, 0x01);
    ret |= imx678_write_reg(dev, 0x47C2, 0x3E);
    ret |= imx678_write_reg(dev, 0x47C3, 0x01);

    ret |= imx678_write_reg(dev, 0x3000, 0x00);
    imx678_delay_ms(18);
    ret |= imx678_write_reg(dev, 0x3002, 0x00);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "imx678_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=================================================================================\n");
    SENSOR_PRINT("====imx678 sensor 2160P30fps 10bit 4lane 2to1 WDR(60fps->30fps) init success!====\n");
    SENSOR_PRINT("=================================================================================\n");

    return XMEDIA_SUCCESS;

}

xmedia_s32 imx678_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case IMX678_8M_12BIT_LINEAR_MODE:
            ret = imx678_4lane_linear_3840x2160_init(dev);
            break;
        case IMX678_8M_10BIT_WDR_MODE:
            ret = imx678_4lane_wdr_3840x2160_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }
    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 imx678_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;

    if (g_imx678_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_imx678_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_imx678_i2c_fd[dev]);
        g_imx678_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_imx678_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 imx678_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_imx678_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_imx678_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_imx678_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = imx678_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 imx678_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_imx678_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_imx678_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 imx678_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[IMX678_DATA_BYTE];

    if (g_imx678_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_imx678_i2c_fd[dev], g_imx678_i2c_addr[dev], addr, IMX678_ADDR_BYTE, buf, IMX678_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //IMX678_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 imx678_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_imx678_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_imx678_i2c_fd[dev], g_imx678_i2c_addr[dev], buf, IMX678_ADDR_BYTE + IMX678_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}
xmedia_s32 imx678_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_type;
    xmedia_u32 flip_type;

    imx678_read_reg(dev, IMX678_REG_ADDR_MIRROR, &mirror_type);
    imx678_read_reg(dev, IMX678_REG_ADDR_FLIP, &flip_type);

    if (mirror_en) {
        mirror_type |= 0x01;
    } else {
        mirror_type &= 0xfe;
    }

    if (flip_en) {
        flip_type |= 0x02;
    } else {
        flip_type &= 0xfd;
    }

    ret =  imx678_write_reg(dev, IMX678_REG_ADDR_MIRROR, mirror_type);
    ret |= imx678_write_reg(dev, IMX678_REG_ADDR_FLIP, flip_type);

    if (ret != XMEDIA_SUCCESS) {
            SENSOR_TRACE(MODULE_DBG_ERR, "imx678_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
