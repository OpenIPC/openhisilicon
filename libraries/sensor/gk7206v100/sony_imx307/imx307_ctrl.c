#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "imx307.h"
#include "imx307_ctrl.h"

static xmedia_s32 g_imx307_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
static xmedia_s32 g_imx307_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

#define IMX307_REG_ADDR_MIRROR_FLIP 0x3007

xmedia_void imx307_delay_ms(xmedia_s32 ms)
{
    usleep(ms * 1000);
}

// 1080P30 and 1080P25
xmedia_s32 imx307_2l_linear_1080p30_init(xmedia_u32 dev)
{
    // Enter Standby
    imx307_write_reg(dev, 0x3000, 0x01); // Standby mode
    imx307_write_reg(dev, 0x3002, 0x00); // XMSTA
    imx307_write_reg(dev, 0x3005, 0x00); // ADBIT-10bit
    imx307_write_reg(dev, 0x3007, 0x00); // VREVERSE & HREVERSE & WINMODE
    imx307_write_reg(dev, 0x3009, 0x02); // FRSEL & FDG_SEL
    imx307_write_reg(dev, 0x300A, 0x3C); // BLKLEVEL
    imx307_write_reg(dev, 0x3011, 0x0A);
    imx307_write_reg(dev, 0x3018, 0x65); // VMAX
    imx307_write_reg(dev, 0x3019, 0x04); // VMAX
    imx307_write_reg(dev, 0x301C, 0x30); // HMAX;
    imx307_write_reg(dev, 0x301D, 0x11); // HMAX;
    imx307_write_reg(dev, 0x3046, 0x00); // ODBIT & OPORTSEL
    imx307_write_reg(dev, 0x304B, 0x0A); // XVSOUTSEL & XHSOUTSEL
    imx307_write_reg(dev, 0x305C, 0x18); // INCKSEL1 37.125MHz
    imx307_write_reg(dev, 0x305D, 0x03); // INCKSEL2
    imx307_write_reg(dev, 0x305E, 0x20);
    imx307_write_reg(dev, 0x305F, 0x01);
    imx307_write_reg(dev, 0x309E, 0x4A);
    imx307_write_reg(dev, 0x309F, 0x4A);

    imx307_write_reg(dev, 0x311C, 0x0E);
    imx307_write_reg(dev, 0x3128, 0x04);
    imx307_write_reg(dev, 0x3129, 0x1D); // ADBIT1
    imx307_write_reg(dev, 0x313B, 0x41);
    imx307_write_reg(dev, 0x315E, 0x1A); // INCKSEL5 37.125MHz INCK5 Setting
    imx307_write_reg(dev, 0x3164, 0x1A); // INCKSEL6 37.125MHz
    imx307_write_reg(dev, 0x317C, 0x12); // ADBIT2
    imx307_write_reg(dev, 0x31EC, 0x37); // ADBIT3

    imx307_write_reg(dev, 0x3405, 0x10); // REPETITION
    imx307_write_reg(dev, 0x3407, 0x01); // PHYSICAL_LANE_NUM
    imx307_write_reg(dev, 0x3414, 0x0A); // OPB_SIZE_V
    imx307_write_reg(dev, 0x3418, 0x49); // Y_OUT_SIZE
    imx307_write_reg(dev, 0x3419, 0x04); // Y_OUT_SIZE
    imx307_write_reg(dev, 0x3441, 0x0A); // CSI_DT_FMT
    imx307_write_reg(dev, 0x3442, 0x0A); // CSI_DT_FMT
    imx307_write_reg(dev, 0x3443, 0x01); // CSI_LANE_MODE---mipi通道数
    imx307_write_reg(dev, 0x3444, 0x20); // EXTCK_FREQ
    imx307_write_reg(dev, 0x3445, 0x25); // EXTCK_FREQ
    imx307_write_reg(dev, 0x3446, 0x57); // TCLKPOST
    imx307_write_reg(dev, 0x3447, 0x00);
    imx307_write_reg(dev, 0x3448, 0x37); // THSZERO
    imx307_write_reg(dev, 0x3449, 0x00);
    imx307_write_reg(dev, 0x344A, 0x1F); // THSPREPARE
    imx307_write_reg(dev, 0x344B, 0x00);
    imx307_write_reg(dev, 0x344C, 0x1F); // TCLKTRAIL
    imx307_write_reg(dev, 0x344D, 0x00);
    imx307_write_reg(dev, 0x344E, 0x1F); // THSTRAIL
    imx307_write_reg(dev, 0x344F, 0x00);
    imx307_write_reg(dev, 0x3450, 0x77); // TCLKZERO
    imx307_write_reg(dev, 0x3451, 0x00);
    imx307_write_reg(dev, 0x3452, 0x1F); // TCLKPREPARE
    imx307_write_reg(dev, 0x3453, 0x00);
    imx307_write_reg(dev, 0x3454, 0x17); // TLPX
    imx307_write_reg(dev, 0x3455, 0x00);
    imx307_write_reg(dev, 0x3472, 0x9C); // X_OUT_SIZE
    imx307_write_reg(dev, 0x3473, 0x07); // X_OUT_SIZE
    imx307_write_reg(dev, 0x3480, 0x49); // INCKSEL7

    // Standby Cancel
    imx307_write_reg(dev, 0x3000, 0x00); // standby

    printf("=====================================================================\n");
    printf("=====Sony imx307_2l sensor 1080P30fps(MIPI) Linear init success!=====\n");
    printf("=====================================================================\n");

    return XMEDIA_SUCCESS;
}

xmedia_s32  imx307_2l_wdr_1080p30_2to1_init(xmedia_u32 dev)
{
    // 10bit
    imx307_write_reg(dev, 0x3000, 0x01); // standby

    imx307_write_reg(dev, 0x3002, 0x00); // XMSTA;
    imx307_write_reg(dev, 0x3005, 0x00); // ADBIT 0x00->10Bit;
    imx307_write_reg(dev, 0x3007, 0x00); // VREVERSE & HREVERSE & WINMODE
    imx307_write_reg(dev, 0x3009, 0x01); // FRSEL & FDG SEL
    imx307_write_reg(dev, 0x300A, 0x3C); // BLKLEVEL
    imx307_write_reg(dev, 0x300C, 0x11);
    imx307_write_reg(dev, 0x3011, 0x0A); // Change after reset;
    imx307_write_reg(dev, 0x3018, 0x65); // VMAX[7:0]
    imx307_write_reg(dev, 0x3019, 0x04); // VMAX[15:8]
    imx307_write_reg(dev, 0x301C, 0x98); // HMAX[7:0]      0x14a0->25fps;
    imx307_write_reg(dev, 0x301D, 0x08); // HMAX[15:8]     0x1130->30fps;
    imx307_write_reg(dev, 0x3020, 0x02); // SHS1
    imx307_write_reg(dev, 0x3021, 0x00); // SHS1
    imx307_write_reg(dev, 0x3024, 0x49); // SHS2
    imx307_write_reg(dev, 0x3025, 0x08); // SHS2
    imx307_write_reg(dev, 0x3030, 0x0B); // RHS1
    imx307_write_reg(dev, 0x3031, 0x00); // RHS1
    imx307_write_reg(dev, 0x3045, 0x05); // DOLSCDEN & DOLSYDINFOEN & HINFOEN
    imx307_write_reg(dev, 0x3046, 0x00); // OPORTSEL & ODBIT
    imx307_write_reg(dev, 0x304B, 0x0A); // XVSOUTSEL & XHSOUTSEL
    imx307_write_reg(dev, 0x305C, 0x18); // INCKSEL1,1080P,CSI-2,37.125MHz;74.25MHz->0x0C
    imx307_write_reg(dev, 0x305D, 0x03); // INCKSEL2,1080P,CSI-2,37.125MHz;74.25MHz->0x03
    imx307_write_reg(dev, 0x305E, 0x20); // INCKSEL3,1080P,CSI-2,37.125MHz;74.25MHz->0x10
    imx307_write_reg(dev, 0x305F, 0x01); // INCKSEL4,1080P,CSI-2,37.125MHz;74.25MHz->0x01
    imx307_write_reg(dev, 0x309E, 0x4A);
    imx307_write_reg(dev, 0x309F, 0x4A);

    imx307_write_reg(dev, 0x3106, 0x11);
    imx307_write_reg(dev, 0x311C, 0x0E);
    imx307_write_reg(dev, 0x3128, 0x04);
    imx307_write_reg(dev, 0x3129, 0x1D); // ADBIT1,12Bit; 0x1D->10Bit;
    imx307_write_reg(dev, 0x313B, 0x41);
    imx307_write_reg(dev, 0x315E, 0x1A); // INCKSEL5,1080P,CSI-2,37.125MHz;74.25MHz->0x1B
    imx307_write_reg(dev, 0x3164, 0x1A); // INCKSEL6,1080P,CSI-2,37.125MHz;74.25MHz->0x1B
    imx307_write_reg(dev, 0x317C, 0x12); // ADBIT2,12Bit;0x12->10Bit;
    imx307_write_reg(dev, 0x31EC, 0x37); // ADBIT3,12Bit;0x37->10Bit;

    // MIPI setting
    imx307_write_reg(dev, 0x3405, 0x00); // REPETITION
    imx307_write_reg(dev, 0x3407, 0x01); // VREVERSE & HREVERSE & WINMODE
    imx307_write_reg(dev, 0x3414, 0x0A); // OPB_SIZE_V
    imx307_write_reg(dev, 0x3415, 0x00);
    imx307_write_reg(dev, 0x3418, 0x9C); // Y_OUT_SIZE
    imx307_write_reg(dev, 0x3419, 0x08); // Y_OUT_SIZE
    imx307_write_reg(dev, 0x3441, 0x0A); // CSI_DT_FMT 10Bit
    imx307_write_reg(dev, 0x3442, 0x0A);
    imx307_write_reg(dev, 0x3443, 0x01); // CSI_LANE_MODE
    imx307_write_reg(dev, 0x3444, 0x20); // EXTCK_FREQ
    imx307_write_reg(dev, 0x3445, 0x25); // EXTCK_FREQ
    imx307_write_reg(dev, 0x3446, 0x77); // TCLKPOST
    imx307_write_reg(dev, 0x3447, 0x00);
    imx307_write_reg(dev, 0x3448, 0x67); // THSZERO
    imx307_write_reg(dev, 0x3449, 0x00);
    imx307_write_reg(dev, 0x344A, 0x47); // THSPREPARE
    imx307_write_reg(dev, 0x344B, 0x00);
    imx307_write_reg(dev, 0x344C, 0x37); // TCLKTRAIL
    imx307_write_reg(dev, 0x344D, 0x00);
    imx307_write_reg(dev, 0x344E, 0x3F); // THSTRAIL
    imx307_write_reg(dev, 0x344F, 0x00);
    imx307_write_reg(dev, 0x3450, 0xFF); // TCLKZERO
    imx307_write_reg(dev, 0x3451, 0x00);
    imx307_write_reg(dev, 0x3452, 0x3F); // TCLKPREPARE
    imx307_write_reg(dev, 0x3453, 0x00);
    imx307_write_reg(dev, 0x3454, 0x37); // TLPX
    imx307_write_reg(dev, 0x3455, 0x00);
    imx307_write_reg(dev, 0x3472, 0xA0); // X_OUT_SIZE
    imx307_write_reg(dev, 0x3473, 0x07);
    imx307_write_reg(dev, 0x347B, 0x23);
    imx307_write_reg(dev, 0x3480, 0x49); // INCKSEL7,1080P,CSI-2,37.125MHz;74.25MHz->0x92

    imx307_write_reg(dev, 0x3000, 0x00); // Standby Cancel

    printf("============================================================================\n");
    printf("===Imx307_2l sensor 1080P30fps 10bit 2to1 WDR(60fps->30fps) init success!===\n");
    printf("============================================================================\n");

    return XMEDIA_SUCCESS;
}

/* 1080P30 and 1080P25 */
xmedia_s32  imx307_4l_linear_1080p30_init(xmedia_u32 dev)
{
    // Enter Standby
    imx307_write_reg(dev, 0x3000, 0x01); // Standby mode
    imx307_write_reg(dev, 0x3002, 0x01); // Master mode stop

    // Mode register setting
    imx307_write_reg(dev, 0x3005, 0x01); //12bit
    imx307_write_reg(dev, 0x3007, 0x00);
    imx307_write_reg(dev, 0x3009, 0x02); // 60fps;0x00->120fps
    imx307_write_reg(dev, 0x300c, 0x00);
    imx307_write_reg(dev, 0x3010, 0x21);
    imx307_write_reg(dev, 0x3011, 0x0a);
    imx307_write_reg(dev, 0x3014, 0x00); // gain
    imx307_write_reg(dev, 0x3018, 0x65); // VMAX
    imx307_write_reg(dev, 0x3019, 0x04);
    imx307_write_reg(dev, 0x301c, 0x30); // HMAX;
    imx307_write_reg(dev, 0x301d, 0x11); // HMAX;
    imx307_write_reg(dev, 0x3020, 0x01); // SHS1
    imx307_write_reg(dev, 0x3021, 0x00); // SHS1
    imx307_write_reg(dev, 0x3022, 0x00); // SHS1
    imx307_write_reg(dev, 0x3030, 0x0B); // RHS1
    imx307_write_reg(dev, 0x3031, 0x00); // RHS1
    imx307_write_reg(dev, 0x3032, 0x00); // RHS1
    imx307_write_reg(dev, 0x3024, 0x00); // SHS2
    imx307_write_reg(dev, 0x3025, 0x00); // SHS2
    imx307_write_reg(dev, 0x3026, 0x00); // SHS2
    imx307_write_reg(dev, 0x3045, 0x01);
    imx307_write_reg(dev, 0x3046, 0x01); // MIPI
    imx307_write_reg(dev, 0x305c, 0x18); // 37.125MHz INCK Setting
    imx307_write_reg(dev, 0x305d, 0x03);
    imx307_write_reg(dev, 0x305e, 0x20);
    imx307_write_reg(dev, 0x305f, 0x01);
    imx307_write_reg(dev, 0x309e, 0x4a);
    imx307_write_reg(dev, 0x309f, 0x4a);
    imx307_write_reg(dev, 0x3106, 0x00);
    imx307_write_reg(dev, 0x311c, 0x0e);
    imx307_write_reg(dev, 0x3128, 0x04);
    imx307_write_reg(dev, 0x3129, 0x00);
    imx307_write_reg(dev, 0x313b, 0x41);
    imx307_write_reg(dev, 0x315e, 0x1a); // 37.125MHz INCK5 Setting
    imx307_write_reg(dev, 0x3164, 0x1a);
    imx307_write_reg(dev, 0x3480, 0x49); // 37.125MHz INCK7 Setting
    imx307_write_reg(dev, 0x3129, 0x00); // ADBIT1,12Bit;
    imx307_write_reg(dev, 0x317c, 0x00); // ADBIT2,12Bit;
    imx307_write_reg(dev, 0x31ec, 0x0e); // ADBIT3,12Bit;

    // Standby Cancel
    imx307_write_reg(dev, 0x3000, 0x00); // standby
    usleep(20000);                             // DELAY20mS
    imx307_write_reg(dev, 0x3002, 0x00); // master mode start
    imx307_write_reg(dev, 0x304B, 0x0a); // XVSOUTSEL XHSOUTSEL enable output
    usleep(20000);

    printf("===================================================================\n");
    printf("===Sony imx307_4L sensor 1080P30fps(MIPI) Linear init success!=====\n");
    printf("===================================================================\n");

    return XMEDIA_SUCCESS;
}

xmedia_s32  imx307_4l_wdr_1080p30_2to1_init(xmedia_u32 dev)
{
    // 10bit
    imx307_write_reg(dev, 0x3000, 0x01); // standby
    imx307_delay_ms(200);

    imx307_write_reg(dev, 0x3005, 0x00); // 12Bit, 0x00,10Bit;
    imx307_write_reg(dev, 0x3007, 0x40); // VREVERSE & HREVERSE & WINMODE
    imx307_write_reg(dev, 0x3009, 0x01); // FRSEL&HCG
    imx307_write_reg(dev, 0x300A, 0x3C); // BLKLEVEL
    imx307_write_reg(dev, 0x300C, 0x11);
    imx307_write_reg(dev, 0x3011, 0x0A); // Change after reset;
    imx307_write_reg(dev, 0x3014, 0x0D); // Gain
    imx307_write_reg(dev, 0x3018, 0xB9); // VMAX[7:0]
    imx307_write_reg(dev, 0x3019, 0x05); // VMAX[15:8]
    imx307_write_reg(dev, 0x301A, 0x00); // VMAX[16]
    imx307_write_reg(dev, 0x301C, 0xEC); // HMAX[7:0]      0x14a0->25fps;
    imx307_write_reg(dev, 0x301D, 0x07); // HMAX[15:8]     0x1130->30fps;

    imx307_write_reg(dev, 0x3020, 0x02); // SHS1
    imx307_write_reg(dev, 0x3021, 0x00);
    imx307_write_reg(dev, 0x3022, 0x00);
    imx307_write_reg(dev, 0x3024, 0x83); // SHS2
    imx307_write_reg(dev, 0x3025, 0x01);
    imx307_write_reg(dev, 0x3026, 0x00);
    imx307_write_reg(dev, 0x3030, 0x79); // RHS1
    imx307_write_reg(dev, 0x3031, 0x01);
    imx307_write_reg(dev, 0x3032, 0x00);
    imx307_write_reg(dev, 0x303A, 0x08);
    imx307_write_reg(dev, 0x303C, 0x04); // WINPV
    imx307_write_reg(dev, 0x303D, 0x00);
    imx307_write_reg(dev, 0x303E, 0x41); // WINWV
    imx307_write_reg(dev, 0x303F, 0x04);
    imx307_write_reg(dev, 0x3045, 0x05); // DOLSCDEN & DOLSYDINFOEN & HINFOEN
    imx307_write_reg(dev, 0x3046, 0x00); // OPORTSE & ODBIT
    imx307_write_reg(dev, 0x304B, 0x0A); // XVSOUTSEL & XHSOUTSEL
    imx307_write_reg(dev, 0x305C, 0x18); // INCKSEL1,1080P,CSI-2,37.125MHz;74.25MHz->0x0C
    imx307_write_reg(dev, 0x305D, 0x03); // INCKSEL2,1080P,CSI-2,37.125MHz;74.25MHz->0x03
    imx307_write_reg(dev, 0x305E, 0x20); // INCKSEL3,1080P,CSI-2,37.125MHz;74.25MHz->0x10
    imx307_write_reg(dev, 0x305F, 0x01); // INCKSEL4,1080P,CSI-2,37.125MHz;74.25MHz->0x01
    imx307_write_reg(dev, 0x309E, 0x4A);
    imx307_write_reg(dev, 0x309F, 0x4A);

    imx307_write_reg(dev, 0x3106, 0x11);
    imx307_write_reg(dev, 0x311C, 0x0E);
    imx307_write_reg(dev, 0x3128, 0x04);
    imx307_write_reg(dev, 0x3129, 0x1D); // ADBIT1,12Bit;0x1D->10Bit;
    imx307_write_reg(dev, 0x313B, 0x41);
    imx307_write_reg(dev, 0x315E, 0x1A); // INCKSEL5,1080P,CSI-2,37.125MHz;74.25MHz->0x1B
    imx307_write_reg(dev, 0x3164, 0x1A); // INCKSEL6,1080P,CSI-2,37.125MHz;74.25MHz->0x1B
    imx307_write_reg(dev, 0x317C, 0x12); // ADBIT2,12Bit;0x12->10Bit;
    imx307_write_reg(dev, 0x31EC, 0x37); // ADBIT3,12Bit;0x37->10Bit;

    // MIPI setting
    imx307_write_reg(dev, 0x3405, 0x10); // REPETITION
    imx307_write_reg(dev, 0x3407, 0x03);
    imx307_write_reg(dev, 0x3414, 0x00);
    imx307_write_reg(dev, 0x3415, 0x00);
    imx307_write_reg(dev, 0x3418, 0x7A); // Y_OUT_SIZE
    imx307_write_reg(dev, 0x3419, 0x09); // Y_OUT_SIZE
    imx307_write_reg(dev, 0x3441, 0x0A); // CSI_DT_FMT 10Bit
    imx307_write_reg(dev, 0x3442, 0x0A);
    imx307_write_reg(dev, 0x3443, 0x03); // CSI_LANE_MODE MIPI 4CH
    imx307_write_reg(dev, 0x3444, 0x20); // EXTCK_FREQ
    imx307_write_reg(dev, 0x3445, 0x25);
    imx307_write_reg(dev, 0x3446, 0x57);
    imx307_write_reg(dev, 0x3447, 0x00);
    imx307_write_reg(dev, 0x3448, 0x37);
    imx307_write_reg(dev, 0x3449, 0x00);
    imx307_write_reg(dev, 0x344A, 0x1F); // THSPREPARE
    imx307_write_reg(dev, 0x344B, 0x00);
    imx307_write_reg(dev, 0x344C, 0x1F);
    imx307_write_reg(dev, 0x344D, 0x00);
    imx307_write_reg(dev, 0x344E, 0x1F); // THSTRAIL
    imx307_write_reg(dev, 0x344F, 0x00);
    imx307_write_reg(dev, 0x3450, 0x77); // TCLKZERO
    imx307_write_reg(dev, 0x3451, 0x00);
    imx307_write_reg(dev, 0x3452, 0x1F); // TCLKPREPARE
    imx307_write_reg(dev, 0x3453, 0x00);
    imx307_write_reg(dev, 0x3454, 0x17); // TIPX
    imx307_write_reg(dev, 0x3455, 0x00);
    imx307_write_reg(dev, 0x3472, 0xA0); // X_OUT_SIZE
    imx307_write_reg(dev, 0x3473, 0x07);
    imx307_write_reg(dev, 0x347B, 0x23);
    imx307_write_reg(dev, 0x3480, 0x49); // INCKSEL7,1080P,CSI-2,37.125MHz;74.25MHz->0x92

    imx307_delay_ms(200);
    imx307_write_reg(dev, 0x3000, 0x00); // Standby Cancel
    imx307_write_reg(dev, 0x3002, 0x00);
    imx307_write_reg(dev, 0x304b, 0x0a);

    printf("============================================================================\n");
    printf("===Imx307_4L sensor 1080P30fps 10bit 2to1 WDR(60fps->30fps) init success!===\n");
    printf("============================================================================\n");

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx307_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case IMX307_2M_2L_10BIT_LINEAR_MODE:
            ret = imx307_2l_linear_1080p30_init(dev);
            break;
        case IMX307_2M_4L_10BIT_LINEAR_MODE:
            ret = imx307_4l_linear_1080p30_init(dev);
            break;
        case IMX307_2M_2L_10BIT_2TO1_WDR_MODE:
            ret = imx307_2l_wdr_1080p30_2to1_init(dev);
            break;
        case IMX307_2M_4L_10BIT_2TO1_WDR_MODE:
            ret = imx307_4l_wdr_1080p30_2to1_init(dev);
            break;
        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }
    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 imx307_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_imx307_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_imx307_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_imx307_i2c_fd[dev]);
        g_imx307_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_imx307_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 imx307_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_imx307_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_imx307_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_imx307_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = imx307_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 imx307_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_imx307_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_imx307_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 imx307_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[IMX307_DATA_BYTE];

    if (g_imx307_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_imx307_i2c_fd[dev], g_imx307_i2c_addr[dev], addr, IMX307_ADDR_BYTE, buf, IMX307_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //IMX307_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 imx307_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_imx307_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_imx307_i2c_fd[dev], g_imx307_i2c_addr[dev], buf, IMX307_ADDR_BYTE + IMX307_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 imx307_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;

    imx307_read_reg(dev, IMX307_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

    if (mirror_en) {
        mirror_flip_type |= 0x02;
    } else {
        mirror_flip_type &= 0xfd;
    }

    if (flip_en) {
        mirror_flip_type |= 0x01;
    } else {
        mirror_flip_type &= 0xfe;
    }

    ret = imx307_write_reg(dev, IMX307_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
            SENSOR_TRACE(MODULE_DBG_ERR, "imx307_write_reg failed\n");
    }
    return XMEDIA_SUCCESS;
}
