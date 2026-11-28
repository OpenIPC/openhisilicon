/*
 * Copyright (c) Lontium Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: lt8618sxb driver
 * Author: Lontium Technologies Co., Ltd.
 * Create: 2019/02/20
 * Modification: 2020/02/25, Hisilicon.
 */

#include "lt8618sx.h"
#include <linux/module.h>
#ifndef __LITEOS__
#include <linux/miscdevice.h>
#include <linux/reboot.h>
#include <linux/notifier.h>
#endif
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/i2c.h>
#include <linux/ioctl.h>
#include "ot_osal.h"

#ifdef OT_FPGA
#define I2C_INDEX_0 1
#else
#define I2C_INDEX_0 7
#endif

/*
 * Use the synchronization signal inside the bt1120 signal
 */
#define _Embedded_sync_
/*
 * need to provide external h sync, V sync and de signals to lt8618sxb
 * #define _External_sync_
 */
/*
 * IIC Address If CI2CA pins are low(0~400mV)
 */
#define _LT8618SX_ADR 0x72
#define DEV_NAME "lt8618sx"
#define DEBUG_LEVEL 1
#define DPRINTK(level, fmt, args...)                                                              \
    do {                                                                                          \
        if (level < DEBUG_LEVEL)                                                                  \
            printk(KERN_INFO "%s [%s ,%d]: " fmt "\n", DEV_NAME, __FUNCTION__, __LINE__, ##args); \
    } while (0)

#ifdef __LITEOS__

static struct i2c_client *lt8168sx_client = NULL;

struct i2c_client lt_client_obj;

static int i2c_client_init(void)
{
    int ret;

    struct i2c_client *i2c_client0 = &lt_client_obj;

    i2c_client0->addr = _LT8618SX_ADR >> 1;
    ret = client_attach(i2c_client0, I2C_INDEX_0);
    if (ret) {
        dprintf("Fail to attach client!\n");
        return -1;
    }
    lt8168sx_client = &lt_client_obj;

    return 0;
}

static void i2c_client_exit(void)
{
    struct i2c_client *i2c_client0 = &lt_client_obj;
    client_deinit(i2c_client0);
}

#else

static struct i2c_client *lt8168sx_client = NULL;

static struct i2c_board_info lt8168sx_i2c_info = {
    I2C_BOARD_INFO("lt8168sx", _LT8618SX_ADR >> 1),
};

static int i2c_client_init(void)
{
    struct i2c_adapter *i2c_adap = NULL;

    i2c_adap = i2c_get_adapter(I2C_INDEX_0);
    if (i2c_adap == NULL) {
        DPRINTK(0, "i2c%d get adapter failed.\n", I2C_INDEX_0);
        return -1;
    }

    lt8168sx_client = i2c_new_client_device(i2c_adap, &lt8168sx_i2c_info);
    if (lt8168sx_client == NULL) {
        DPRINTK(0, "i2c%d get new device failed.\n", I2C_INDEX_0);
        return -1;
    }
    i2c_put_adapter(i2c_adap);

    return 0;
}

static void i2c_client_exit(void)
{
    if (lt8168sx_client != NULL) {
        i2c_unregister_device(lt8168sx_client);
        lt8168sx_client = NULL;
    }
}
#endif

static unsigned char _i2c_read_byte(unsigned char devaddress, unsigned char address)
{
#ifdef DRV_I2C
    return drv_i2c_read(I2C_INDEX_0, devaddress, address, 1, 1);
#else
    unsigned char ret_data = 0xFF;
    int ret;
    struct i2c_client *client = lt8168sx_client;
    struct i2c_msg msg[2];
    unsigned char buffer;

    (void)devaddress;
    buffer = address;
    msg[0].addr = client->addr;
    msg[0].flags = 0;
    msg[0].len = 1;
    msg[0].buf = &buffer;

    msg[1].addr = client->addr;
    msg[1].flags = client->flags | I2C_M_RD;
    msg[1].len = 1;
    msg[1].buf = &buffer;

    ret = bsp_i2c_transfer(client->adapter, msg, 2);
    if (ret < 0) {
        printk("i2c read failed.\n");
    } else {
        ret_data = buffer;
    }
    return ret_data;
#endif
}

static int _i2c_write_byte(unsigned char devaddress, unsigned char address, unsigned char data)
{
#ifdef DRV_I2C
    return drv_i2c_write(I2C_INDEX_0, devaddress, address, 1, data, 1);
#else
    int ret;
    unsigned char buf[2];
    struct i2c_client *client = lt8168sx_client;

    (void)devaddress;
    lt8168sx_client->addr = (_LT8618SX_ADR >> 1);

    buf[0] = address;
    buf[1] = data;

    ret = i2c_master_send(client, buf, 2);
    if (ret < 0) {
        printk("[%s][%d] i2c write failed.\n", __FUNCTION__, __LINE__);
    }
    udelay(300); /* 300us */
    return ret;
#endif
}

/*
 * --------------------------------
 * CI2CA              I2C address
 * --------------------------------
 * (0~400mV)----------0x72(default)
 * (400mV~800mV)------0x7a
 * (800mV~1200mV)-----0x90
 * (1200mV~1600mV) ---0x92
 *
 * (2000mV~2400mV)----0x94
 * (2400mV~2800mV)----0x7e
 * (2800mV~3300mV)----0x76
 * -------------------------------
 */
/*
 * The lowest bit 0 of the IIC address 0x72 (0x76) of LT8618SXB is the read-write flag bit.
 * In the case of Linux IIC, the highest bit is the read-write flag bit, The IIC address needs to be moved one bit to
 * the right, and the IIC address becomes 0x39 (0x3B). IIC rate should not exceed 100K. If the IIC address of LT8618SXB
 * is not 0x72, you need to reset LT8618SXB with the master GPIO, pull down 100ms, then pull up, delay 100ms, and
 * initialize the LT8618SXB register.
 */
static void HDMI_WriteI2C_Byte(unsigned char reg_addr, unsigned char value)
{
    _i2c_write_byte(_LT8618SX_ADR, reg_addr, value);
    return;
}
unsigned int HDMI_ReadI2C_Byte(unsigned char reg_addr)
{
    unsigned char read_ret;
    read_ret = _i2c_read_byte(_LT8618SX_ADR, reg_addr);
    return read_ret;
}

/*
 * 1 Reset LT8618SX
 * 2 LT8618SX Initial setting:
 * unsigned short hfp, hs_width, hbp, h_act, h_tal, v_act, v_tal, vfp, vs_width, vbp;
 * unsigned char HDMI_VIC = 0x00;
 * unsigned char HDMI_Y = 0x00;
 * bool hs_pol, vs_pol;
 */
/*
 * LT8618SXB_D0_D7_Bit_Swap, 0x01: Red D0 ~ D7 swap enable; 0x00 : Normal
 * LT8618SXB_D8_D15_Bit_Swap, 0x02: Red D8 ~ D15 swap enable; 0x00 : Normal
 * LT8618SXB_D16_D23_Bit_Swap, 0x04: Red D16 ~ D23 swap enable; 0x00 : Normal
 */
#define LT8618SXB_D0_D7_Bit_Swap_0 0x00
#define LT8618SXB_D8_D15_Bit_Swap_0 0x00
#define LT8618SXB_D16_D23_Bit_Swap_0 0x00

#define LT8618SXB_D0_D7_Bit_Swap_1 0x01
#define LT8618SXB_D8_D15_Bit_Swap_2 0x02
#define LT8618SXB_D16_D23_Bit_Swap_4 0x04

/*
 * _D0_D15_In, BT1120 input from D0 to D15 of LT8618SXB pins. D0 ~ D7 Y ; D8 ~ D15 C
 * _D8_D23_In, BT1120 input from D8 to D23 of LT8618SXB pins. D8 ~ D15 Y ; D16 ~ D23 C
 * _D0_D15_In_2, BT1120 input from D0 to D15 of LT8618SXB pins. D0 ~ D7 C ; D8 ~ D15 Y
 * _D8_D23_In_2, BT1120 input from D8 to D23 of LT8618SXB pins. D8 ~ D15 C ; D16 ~ D23 Y
 */
#define _D0_D15_In 0x30
#define _D8_D23_In 0x70
#define _D0_D15_In_2 0x00
#define _D8_D23_In_2 0x60

/*
 * 1: DDR mode; 0: SDR (normal) mode
 */
bool Use_DDRCLK = 0;
#ifndef MODULE
osal_setup_num_param(Use_DDRCLK, Use_DDRCLK);
#else
module_param(Use_DDRCLK, bool, S_IRUGO);
#endif

unsigned char I2CADR;

/*
 * --------------------------------------
 * Resolution          VIC_Num
 * --------------------------------------
 * 640x480             1
 * 720x480P 60Hz       2
 * 720x480i 60Hz       6
 * 720x576P 50Hz       17
 * 720x576i 50Hz       21
 * 1280x720P 24Hz      60
 * 1280x720P 25Hz      61
 * 1280x720P 30Hz      62
 * 1280x720P 50Hz      19
 * 1280x720P 60Hz      4
 * 1920x1080P 24Hz     32
 * 1920x1080P 25Hz     33
 * 1920x1080P 30Hz     34
 * 1920x1080i 50Hz     20
 * 1920x1080i 60Hz     5
 * 1920x1080P 50Hz     31
 * 1920x1080P 60Hz     16
 * other resolution    0(default)
 * --------------------------------------
 */
#define VIC_640X480 1
#define VIC_720X480P60Hz 2
#define VIC_720X480I60Hz 6
#define VIC_720X576P50Hz 17
#define VIC_720X576I50Hz 21
#define VIC_1280X720P24Hz 60
#define VIC_1280X720P25Hz 61
#define VIC_1280X720P30Hz 62
#define VIC_1280X720P50Hz 19
#define VIC_1280X720P60Hz 4
#define VIC_1920X1080P24Hz 32
#define VIC_1920X1080P25Hz 33
#define VIC_1920X1080P30Hz 34
#define VIC_1920X1080I50Hz 20
#define VIC_1920X1080I60Hz 5
#define VIC_1920X1080P50Hz 31
#define VIC_1920X1080P60Hz 16
#define VIC_3840x2160_24Hz 93
#define VIC_3840x2160_25Hz 94
#define VIC_3840x2160_30Hz 95
#define VIC_DEFAULT 0

/*
 * vic ,0x10: 1080P ;  0x04 : 720P ; Refer to the following list
 */
unsigned char VIC_Num = VIC_1920X1080P60Hz;

bool flag_Ver_u3 = 0;

#ifdef _Read_TV_EDID_

unsigned char Sink_EDID[256];

#endif

enum {
    _32KHz = 0,
    _44d1KHz = 1,
    _48KHz = 2,
    _88d2KHz = 3,
    _96KHz = 4,
    _176Khz = 5,
    _196KHz = 6,

    FREQ_BUTT = 7,
};

/*
 * 4096  : 32K
 * 6272  : 44.1K
 * 6144  : 48K
 * 12544 : 88.2K
 * 12288 : 96K
 * 25088 : 176K
 * 24576 : 196K
 */
unsigned short IIS_N[FREQ_BUTT] = {
    4096,
    6272,
    6144,
    12544,
    12288,
    25088,
    24576,
};

/*
 * 0x30 : 32K
 * 0x00 : 44.1K
 * 0x20 : 48K
 * 0x80 : 88.2K
 * 0xa0 : 96K
 * 0xc0 : 176K
 * 0xe0 : 196K
 */
unsigned short Sample_Freq[FREQ_BUTT] = {
    0x30,
    0x00,
    0x20,
    0x80,
    0xa0,
    0xc0,
    0xe0,
};

typedef enum {
    LT_OUT_PAL = 0,       /* PAL standard */
    LT_OUT_NTSC = 1,      /* NTSC standard */
    LT_OUT_960H_PAL = 2,  /* ITU-R BT.1302 960 x 576 at 50 Hz (interlaced) */
    LT_OUT_960H_NTSC = 3, /* ITU-R BT.1302 960 x 480 at 60 Hz (interlaced) */

    LT_OUT_640x480_60 = 4,    /* VESA 640 x 480 at 60 Hz (non-interlaced) CVT */
    LT_OUT_480P60 = 5,        /* 720  x  480 at 60 Hz. */
    LT_OUT_576P50 = 6,        /* 720  x  576 at 50 Hz. */
    LT_OUT_800x600_60 = 7,    /* VESA 800 x 600 at 60 Hz (non-interlaced) */
    LT_OUT_1024x768_60 = 8,   /* VESA 1024 x 768 at 60 Hz (non-interlaced) */
    LT_OUT_720P50 = 9,        /* 1280 x  720 at 50 Hz. */
    LT_OUT_720P60 = 10,       /* 1280 x  720 at 60 Hz. */
    LT_OUT_1280x800_60 = 11,  /* 1280*800@60Hz VGA@60Hz */
    LT_OUT_1280x1024_60 = 12, /* VESA 1280 x 1024 at 60 Hz (non-interlaced) */
    LT_OUT_1366x768_60 = 13,  /* VESA 1366 x 768 at 60 Hz (non-interlaced) */
    LT_OUT_1400x1050_60 = 14, /* VESA 1400 x 1050 at 60 Hz (non-interlaced) CVT */
    LT_OUT_1440x900_60 = 15,  /* VESA 1440 x 900 at 60 Hz (non-interlaced) CVT Compliant */
    LT_OUT_1680x1050_60 = 16, /* VESA 1680 x 1050 at 60 Hz (non-interlaced) */

    LT_OUT_1080P24 = 17, /* 1920 x 1080 at 24 Hz. */
    LT_OUT_1080P25 = 18, /* 1920 x 1080 at 25 Hz. */
    LT_OUT_1080P30 = 19, /* 1920 x 1080 at 30 Hz. */
    LT_OUT_1080I50 = 20, /* 1920 x 1080 at 50 Hz, interlaced. */
    LT_OUT_1080I60 = 21, /* 1920 x 1080 at 60 Hz, interlaced. */
    LT_OUT_1080P50 = 22, /* 1920 x 1080 at 50 Hz. */
    LT_OUT_1080P60 = 23, /* 1920 x 1080 at 60 Hz. */

    LT_OUT_1600x1200_60 = 24, /* VESA 1600 x 1200 at 60 Hz (non-interlaced) */
    LT_OUT_1920x1200_60 = 25, /* VESA 1920 x 1600 at 60 Hz (non-interlaced) CVT (Reduced Blanking) */
    LT_OUT_1920x2160_30 = 26, /* 1920x2160_30 */
    LT_OUT_2560x1440_30 = 27, /* 2560x1440_30 */
    LT_OUT_2560x1440_60 = 28, /* 2560x1440_60 */
    LT_OUT_2560x1600_60 = 29, /* 2560x1600_60 */

    LT_OUT_3840x2160_24 = 30, /* 3840x2160_24 */
    LT_OUT_3840x2160_25 = 31, /* 3840x2160_25 */
    LT_OUT_3840x2160_30 = 32, /* 3840x2160_30 */

    LT_OUT_USER = 33, /* User timing. */

    LT_OUT_BUTT = 34,
} lt_intf_sync;

enum {
    Less_than_50M = 0,
    Bound_50_100M,
    Greater_than_100M
};

static unsigned char Resolution_Num = LT_OUT_1080P60;
#define _16_9_ 0x2A
#define _4_3_ 0x19

enum {
    H_act = 0,
    V_act = 1,
    H_tol = 2,
    V_tol = 3,

    H_bp = 4,
    H_sync = 5,
    H_fp = 6,

    V_fp = 7,
    V_sync = 8,
    V_bp = 9,
    Vic = 10,
    Pic_Ratio = 11,     /* Image proportion */
    Clk_bound_SDR = 12, /* SDR */
    Clk_bound_DDR = 13, /* DDR */

    TIMING_BUTT = 14,
};

static int Format_Timing[LT_OUT_BUTT][TIMING_BUTT] = {
    /* H_act V_act H_total V_total H_BP H_sync  H_FP V_FP V_sync    V_BP Vic Pic_Ratio Clk_bound_SDR Clk_bound_DDR */
    {720,  288,  864,  312,  6, 126, 12, 2, 3, 19, VIC_720X576I50Hz, _4_3_, Less_than_50M, Less_than_50M},
    {720,  240,  858,  262,  0, 124, 19, 4, 3, 15, VIC_720X480I60Hz, _4_3_, Less_than_50M, Less_than_50M},
    {960,  288,  1152, 312,  8, 168, 16, 2, 3, 19, VIC_DEFAULT, _4_3_, Less_than_50M, Less_than_50M},
    {960,  240,  1144, 262,  0, 168, 21, 4, 3, 15, VIC_DEFAULT, _4_3_, Less_than_50M, Less_than_50M},
    {640,  480,  800,  525,  48, 96, 16, 10, 2, 33, VIC_640X480, _4_3_, Less_than_50M, Less_than_50M},
    {720,  480,  858,  525,  60, 62, 16, 9, 6, 30, VIC_720X480P60Hz, _4_3_, Less_than_50M, Less_than_50M},
    {720,  576,  864,  625,  68, 64, 12, 5, 5, 39, VIC_720X576P50Hz, _4_3_, Less_than_50M, Less_than_50M},
    {800,  600,  1056, 628,  88, 128, 40, 1, 4, 23, VIC_DEFAULT, _4_3_, Less_than_50M, Less_than_50M},
    {1024, 768,  1344, 806,  160, 136, 24, 3, 6, 29, VIC_DEFAULT, _4_3_, Bound_50_100M, Less_than_50M},
    {1280, 720,  1980, 750,  220, 40, 440, 5, 5, 20, VIC_1280X720P50Hz, _16_9_, Bound_50_100M, Less_than_50M},
    {1280, 720,  1650, 750,  220, 40, 110, 5, 5, 20, VIC_1280X720P60Hz, _16_9_, Bound_50_100M, Less_than_50M},
    {1280, 800,  1680, 831,  200, 128, 72, 3, 6, 22, VIC_DEFAULT, _4_3_, Bound_50_100M, Less_than_50M},
    {1280, 1024, 1688, 1066, 248, 112, 48, 1, 3, 38, VIC_DEFAULT, _4_3_, Greater_than_100M, Bound_50_100M},
    {1366, 768,  1792, 798,  213, 143, 70, 3, 3, 24, VIC_DEFAULT, _4_3_, Bound_50_100M, Less_than_50M},
    {1400, 1050, 1864, 1089, 232, 144, 88, 3, 4, 32, VIC_DEFAULT, _4_3_, Greater_than_100M, Bound_50_100M},
    {1440, 900,  1904, 934,  232, 152, 80, 3, 6, 25, VIC_DEFAULT, _4_3_, Greater_than_100M, Bound_50_100M},
    {1680, 1050, 2240, 1089, 280, 176, 104, 3, 6, 30, VIC_DEFAULT, _4_3_, Greater_than_100M, Bound_50_100M},
    {1920, 1080, 2750, 1125, 148, 44, 638, 4, 5, 36, VIC_1920X1080P24Hz, _16_9_, Bound_50_100M, Less_than_50M},
    {1920, 1080, 2640, 1125, 148, 44, 528, 4, 5, 36, VIC_1920X1080P25Hz, _16_9_, Bound_50_100M, Less_than_50M},
    {1920, 1080, 2200, 1125, 148, 44, 88, 4, 5, 36, VIC_1920X1080P30Hz, _16_9_, Bound_50_100M, Less_than_50M},
    {1920, 540,  2640, 562,   148, 44, 528, 2, 5, 15, VIC_1920X1080I50Hz, _16_9_, Bound_50_100M, Less_than_50M},
    {1920, 540,  2200, 562,   148, 44, 88, 2, 5, 15, VIC_1920X1080I60Hz, _16_9_, Bound_50_100M, Less_than_50M},
    {1920, 1080, 2640, 1125,  148, 44, 528, 4, 5, 36, VIC_1920X1080P50Hz, _16_9_, Greater_than_100M, Bound_50_100M},
    {1920, 1080, 2200, 1125,  148, 44, 88, 4, 5, 36, VIC_1920X1080P60Hz, _16_9_, Greater_than_100M, Bound_50_100M},
    {1600, 1200, 2160, 1250, 304, 192, 64, 1, 3, 46, VIC_DEFAULT, _4_3_, Greater_than_100M, Bound_50_100M},
    {1920, 1200, 2080, 1235,  80, 32,  48, 3, 6, 26, VIC_1920X1080P60Hz, _16_9_, Greater_than_100M, Bound_50_100M},
    {1920, 2160, 2200, 2240,  148, 44, 88, 8, 5, 67, VIC_1920X1080P60Hz, _4_3_, Greater_than_100M, Bound_50_100M},
    {2560, 1440, 2720, 1481,  80, 32, 48, 2, 5, 34, VIC_DEFAULT, _4_3_, Greater_than_100M, Bound_50_100M},
    {2560, 1440, 2720, 1481,  80, 32, 48, 2, 5, 34, VIC_DEFAULT, _4_3_, Greater_than_100M, Greater_than_100M},
    {2560, 1600, 2720, 1646,  80, 32, 48, 3, 6, 37, VIC_DEFAULT, _4_3_, Greater_than_100M, Greater_than_100M},
    {3840, 2160, 5500, 2250, 296, 88, 1276, 8, 10, 72, VIC_3840x2160_24Hz, _16_9_, Greater_than_100M, Greater_than_100M},
    {3840, 2160, 5280, 2250, 296, 88, 1056, 8, 10, 72, VIC_3840x2160_25Hz, _16_9_, Greater_than_100M, Greater_than_100M},
    {3840, 2160, 4400, 2250, 296, 88, 176, 8, 10, 72, VIC_3840x2160_30Hz, _16_9_, Greater_than_100M, Greater_than_100M},
    {0}, /* user */
};

#ifdef _External_sync_
unsigned short hfp, hs_width, hbp, h_act, h_tal, v_act, v_tal, vfp, vs_width, vbp;
bool hs_pol, vs_pol;
#else
unsigned short h_act, h_tal, v_act;
#endif
unsigned int CLK_Cnt;

static unsigned char LT8618SXB_PLL_u3[3][3] =
{
    { 0x00, 0x9e, 0xaa },   // < 50MHz
    { 0x00, 0x9e, 0x99 },   // 50 ~ 100M
    { 0x00, 0x9e, 0x88 },   // > 100M
};

static unsigned char LT8618SXB_PLL_u2[3][3] =
{
    { 0x00, 0x94, 0xaa },   // < 50MHz
    { 0x01, 0x94, 0x99 },   // 50 ~ 100M
    { 0x03, 0x94, 0x88 },   // > 100M
};

unsigned char CLK_bound;

/*
 * SDR:1080P 60/50Hz; DDR:4K30Hz. -- 148.5M
 */
unsigned char CLK_Num = Greater_than_100M;

static unsigned int norm = LT_OUT_1080P60;
static unsigned int yc_reverse = 0; /* 0: yuyv, 1: uyvy */
static unsigned int bit_reverse = 0; /* 0: default, 1: reverse */

#ifndef MODULE
osal_setup_num_param(norm, norm);
osal_setup_num_param(yc_reverse, yc_reverse);
osal_setup_num_param(bit_reverse, bit_reverse);
#else
module_param(norm, int, S_IRUGO);
module_param(yc_reverse, int, S_IRUGO);
module_param(bit_reverse, int, S_IRUGO);
#endif

static void LT8618SX_VideoSel(unsigned int vmode)
{
    if (vmode > LT_OUT_USER) {
        /* defalut: 1080P60 */
        VIC_Num = VIC_1920X1080P60Hz;
        /* 6: HDMI_1080P60 */
        Resolution_Num = HDMI_1080P60;
        /* 148.5M */
        CLK_Num = Greater_than_100M;
        printk("Havn't the mode:%d\n", vmode);
        return;
    }
    if (Format_Timing[vmode][H_act] == 0) {
        printk("\nwarning: video format(%u)'s timing must be set.\n\n", vmode);
    }
    Resolution_Num = vmode;
    VIC_Num = Format_Timing[vmode][Vic];
    CLK_Num = Format_Timing[vmode][Clk_bound_SDR];
}

static void lt_device_init(unsigned int norm_tmp)
{
    Resolution_change(norm_tmp);
}

#ifdef __LITEOS__

static unsigned int open_cnt = 0;

static int lt_open(struct file *file)
{
    (void)file;

    if ((open_cnt++) == 0) {
        return 0;
    }

    DPRINTK(0, "you should close the device first!");
    return -1;
}

static int lt_close(struct file *file)
{
    (void)file;

    open_cnt--;
    return 0;
}

static int lt_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    unsigned int norm_tmp = 0;
    unsigned int use_ddr_clk = 0;

    (void)file;

    if ((char *)(uintptr_t)arg == NULL) {
        return -1;
    }

    if (((_IOC_SIZE(cmd) == 0) && (_IOC_DIR(cmd) != _IOC_NONE))) {
        printk("%s %d - input param err, size of cmd is 0!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    if ((_IOC_DIR(cmd) != _IOC_NONE) && (((char *)(uintptr_t)arg) == NULL)) {
        printk("%s %d - input param err, arg is null!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    if (_IOC_DIR(cmd) != _IOC_WRITE) {
        printk("%s %d - input param err, cmd is not write!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    switch (cmd) {
        case (LT_CMD_SETCLK): {
            if (copy_from_user(&use_ddr_clk, (void *)(uintptr_t)arg, sizeof(unsigned int))) {
                return -1;
            }

            if (norm_tmp <= LT_CLK_EDGE_DUAL) {
                Use_DDRCLK = use_ddr_clk;
                return 0;
            } else {
                printk("%s havn't the Use_DDRCLK:%u\n", DEV_NAME, use_ddr_clk);
            }
            break;
        }

        case (LT_CMD_SETMODE): {
            if (copy_from_user(&norm_tmp, (void *)(uintptr_t)arg, sizeof(unsigned int))) {
                return -1;
            }

            if ((norm_tmp >= LT_OUT_640x480_60) && (norm_tmp < LT_OUT_USER)) {
                lt_device_init(norm_tmp);
                return 0;
            } else {
                printk("%s havn't the mode:%u\n", DEV_NAME, norm_tmp);
            }
            break;
        }

        default: {
            printk("%s havn't the cmd:0x%x\n", DEV_NAME, cmd);
            break;
        }
    }
    return 0;
}

const static struct file_operations_vfs lt_fops = {
    .open = lt_open,
    .close = lt_close,
    .ioctl = lt_ioctl,
};

#else

static unsigned int open_cnt = 0;

static int lt_open(struct inode *inode, struct file *file)
{
    (void)inode;
    (void)file;

    if ((open_cnt++) == 0) {
        return 0;
    }

    DPRINTK(0, "you should close the device first!");
    return -1;
}

static int lt_close(struct inode *inode, struct file *file)
{
    (void)inode;
    (void)file;

    open_cnt--;
    return 0;
}

static long lt_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    unsigned int norm_tmp = 0;
    unsigned int use_ddr_clk = 0;

    (void)file;

    if ((char *)(uintptr_t)arg == NULL) {
        return -1;
    }

    if (((_IOC_SIZE(cmd) == 0) && (_IOC_DIR(cmd) != _IOC_NONE))) {
        printk("%s %d - input param err, size of cmd is 0!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    if ((_IOC_DIR(cmd) != _IOC_NONE) && (((char *)(uintptr_t)arg) == NULL)) {
        printk("%s %d - input param err, arg is null!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    if (_IOC_DIR(cmd) != _IOC_WRITE) {
        printk("%s %d - input param err, cmd is not write!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    switch (cmd) {
        case (LT_CMD_SETCLK): {
            if (copy_from_user(&use_ddr_clk, (void *)(uintptr_t)arg, sizeof(unsigned int))) {
                return -1;
            }

            if (norm_tmp <= LT_CLK_EDGE_DUAL) {
                Use_DDRCLK = use_ddr_clk;
                return 0;
            } else {
                printk("%s havn't the Use_DDRCLK:%u\n", DEV_NAME, use_ddr_clk);
            }
            break;
        }

        case (LT_CMD_SETMODE): {
            if (copy_from_user(&norm_tmp, (void *)(uintptr_t)arg, sizeof(unsigned int))) {
                return -1;
            }

            if ((norm_tmp >= LT_OUT_640x480_60) && (norm_tmp < LT_OUT_USER)) {
                lt_device_init(norm_tmp);
                return 0;
            } else {
                printk("%s havn't the mode:%u\n", DEV_NAME, norm_tmp);
            }
            break;
        }

        default: {
            printk("%s havn't the cmd:0x%x\n", DEV_NAME, cmd);
            break;
        }
    }
    return 0;
}

static struct file_operations lt_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = lt_ioctl,
    .open = lt_open,
    .release = lt_close
};

static struct miscdevice lt_dev = {
    MISC_DYNAMIC_MINOR,
    DEV_NAME,
    &lt_fops,
};
#endif

static void lt_reset_ttl(void)
{
    /* register bank */
    HDMI_WriteI2C_Byte(0xff, 0x80);
    HDMI_WriteI2C_Byte(0xee, 0x01);

    /* reset MIPI Rx logic. */
    HDMI_WriteI2C_Byte(0x11, 0x00);

    HDMI_WriteI2C_Byte(0x13, 0xf1);
}

#ifndef __LITEOS__
static int lt_reboot(struct notifier_block *self, unsigned long event, void *data)
{
    (void)self;
    (void)event;
    (void)data;
    lt_reset_ttl();
    return NOTIFY_OK;
}

static struct notifier_block lt_reboot_notifier = {
    .notifier_call = lt_reboot,
};

static int lt_reboot_init(void)
{
    register_reboot_notifier(&lt_reboot_notifier);
    return 0;
}

static void lt_reboot_exit(void)
{
    unregister_reboot_notifier(&lt_reboot_notifier);
}
#endif

#ifdef _LT8618_HDCP_

static void LT8618SX_HDCP_Init(void)
{
    HDMI_WriteI2C_Byte(0xff, 0x85);
    HDMI_WriteI2C_Byte(0x07, 0x1f);
    /*
     * [7]=force_hpd, [6]=force_rsen, [5]=vsync_pol, [4]=hsync_pol,
     * [3]=hdmi_mode, [2]=no_accs_when_rdy, [1]=skip_wt_hdmi
     */
    HDMI_WriteI2C_Byte(0x13, 0xfe);

    /*
     * [7]=ri_short_read, [3]=sync_pol_mode, [2]=srm_chk_done,
     * [1]=bksv_srm_pass, [0]=ksv_list_vld
     */
    HDMI_WriteI2C_Byte(0x17, 0x0f);

    HDMI_WriteI2C_Byte(0x15, 0x05);
}

static void LT8618SX_HDCP_Enable(void)
{
    HDMI_WriteI2C_Byte(0xff, 0x80);
    HDMI_WriteI2C_Byte(0x14, 0x00);
    HDMI_WriteI2C_Byte(0x14, 0xff);
    HDMI_WriteI2C_Byte(0xff, 0x85);
    /* disable HDCP */
    HDMI_WriteI2C_Byte(0x15, 0x01);
    /* enable HDCP */
    HDMI_WriteI2C_Byte(0x15, 0x71);
    /* enable HDCP */
    HDMI_WriteI2C_Byte(0x15, 0x65);
}

static void LT8618SX_HDCP_Disable(void)
{
    HDMI_WriteI2C_Byte(0xff, 0x85);
    /* enable HDCP */
    HDMI_WriteI2C_Byte(0x15, 0x45);
}

#endif

static void LT8918SXB_Read_EDID(void)
{
#ifdef _Read_TV_EDID_

    unsigned char i, j;
    unsigned char extended_flag = 0x00;

    HDMI_WriteI2C_Byte(0xff, 0x85);
    /*
     * I2C 100K
     */
    HDMI_WriteI2C_Byte(0x03, 0xc9);
    /* 0xA0 is EDID device address */
    HDMI_WriteI2C_Byte(0x04, 0xa0);
    /* 0x00 is EDID offset address */
    HDMI_WriteI2C_Byte(0x05, 0x00);
    /* length for read */
    HDMI_WriteI2C_Byte(0x06, 0x20);
    HDMI_WriteI2C_Byte(0x14, 0x7f);

    /* block 0 & 1 */
    for (i = 0; i < 8; i++) {
        /* 0x00 is EDID offset address */
        HDMI_WriteI2C_Byte(0x05, i * 32);
        HDMI_WriteI2C_Byte(0x07, 0x36);
        /* 0x31 */
        HDMI_WriteI2C_Byte(0x07, 0x34);
        /* 0x37 */
        HDMI_WriteI2C_Byte(0x07, 0x37);
        /* wait 5ms for reading edid data. */
        msleep(5);
        if (HDMI_ReadI2C_Byte(0x40) & 0x02) {
            /* DDC No Ack or Abitration lost */
            if (HDMI_ReadI2C_Byte(0x40) & 0x50) {
                printk("\r\nread edid failed: no ack");
                goto end;
            } else {
                printk("\r\n");
                for (j = 0; j < 32; j++) {
                    Sink_EDID[i * 32 + j] = HDMI_ReadI2C_Byte(0x83);
                    printk("%u ", Sink_EDID[i * 32 + j]);
                    if ((i == 3) && (j == 30)) {
                        extended_flag = Sink_EDID[i * 32 + j] & 0x03;
                    }
                }
                if (i == 3) {
                    /* no block 1, stop reading edid. */
                    if (extended_flag < 1) {
                        goto end;
                    }
                }
            }
        } else {
            printk("\r\nread edid failed: accs not done");
            goto end;
        }
    }

    /* no block 2, stop reading edid. */
    if (extended_flag < 2) {
        goto end;
    }

    /* block 2 & 3 */
    for (i = 0; i < 8; i++) {
        /* 0x00 is EDID offset address */
        HDMI_WriteI2C_Byte(0x05, i * 32);
        HDMI_WriteI2C_Byte(0x07, 0x76);
        HDMI_WriteI2C_Byte(0x07, 0x74);
        HDMI_WriteI2C_Byte(0x07, 0x77);
        /* wait 5ms for reading edid data. */
        msleep(5);
        if (HDMI_ReadI2C_Byte(0x40) & 0x02) {
            /* DDC No Ack or Abitration lost */
            if (HDMI_ReadI2C_Byte(0x40) & 0x50) {
                printk("\r\nread edid failed: no ack");
                goto end;
            } else {
                printk("\r\n");
                for (j = 0; j < 32; j++) {
                    Sink_EDID2[i * 32 + j] = HDMI_ReadI2C_Byte(0x83);
                    printk("%u ", Sink_EDID2[i * 32 + j]);
                }
                if (i == 3) {
                    /* no block 1, stop reading edid. */
                    if (extended_flag < 3) {
                        goto end;
                    }
                }
            }
        } else {
            printk("\r\nread edid failed: accs not done");
            goto end;
        }
    }
end:
    HDMI_WriteI2C_Byte(0x03, 0xc2);
    HDMI_WriteI2C_Byte(0x07, 0x1f);
#endif
}

bool LT8618SXB_Chip_ID( void )
{
    unsigned char id0 = 0;
    unsigned char id1 = 0;
    unsigned char id2 = 0;
    unsigned int timeout_cnt = 0;

    HDMI_WriteI2C_Byte(0xFF, 0x80);     // register bank
    HDMI_WriteI2C_Byte(0xee, 0x01);

    while ((id0 != 0x17) || (id1 != 0x02) || ((id2 & 0xfc) != 0xe0)) {
        id0 = HDMI_ReadI2C_Byte(0x00);
        id1 = HDMI_ReadI2C_Byte(0x01);
        id2 = HDMI_ReadI2C_Byte(0x02);
        msleep(100);
        if (timeout_cnt++ > 10) {
            printk("i2c time out !!!\n");
            return false;
        }
    }

#ifdef _DEBUG_MODE_
    Debug_Printf("\r\nRead LT8618SXB ID ");
    Debug_Printf("\r\n");
    Debug_DispStrNum("\r\nLT8618SXB Chip ID = ", HDMI_ReadI2C_Byte(0x00));    // 0x17
    Debug_DispStrNum(" ", HDMI_ReadI2C_Byte(0x01));                           // 0x02
    Debug_DispStrNum(" ", HDMI_ReadI2C_Byte(0x02));                           // 0xE1 / 0xE2
    Debug_Printf( "\r\n ");
#endif

    /* _Ver_U2 ID: 0x17 0x02 0xE1
        _Ver_U3 ID: 0x17 0x02 0xE2
    */
    if (id2 == 0xe2) {
        flag_Ver_u3 = 1;        // u3
    } else if (id2 == 0xe1) {
        flag_Ver_u3 = 0;        // u2
    }
    return true;
}

/* The threshold value of high HPD detected by lt8618sxb is 1.2V */
unsigned char LT8618SXB_HPD_status(void)
{
    unsigned char HPD_Status = 0;

    HDMI_WriteI2C_Byte(0xff, 0x82); // Register bank

    if((HDMI_ReadI2C_Byte(0x5e) & 0x05 ) == 0x05){
        HPD_Status = 1;                     // HPD is High
    }

    return HPD_Status;
}

void LT8618SXB_RST_PD_Init(void)
{
    HDMI_WriteI2C_Byte(0xff, 0x80);
    HDMI_WriteI2C_Byte(0x11, 0x00);   //reset MIPI Rx logic.

    HDMI_WriteI2C_Byte(0x13, 0xf1);
    HDMI_WriteI2C_Byte(0x13, 0xf9);   // Reset TTL video process
}

static void LT8618SXB_Video_check(void)
{
    /* video */
    HDMI_WriteI2C_Byte(0xff, 0x82);

#ifdef _Embedded_sync_

    h_tal = HDMI_ReadI2C_Byte(0x8f);
    h_tal = (h_tal << 8) + HDMI_ReadI2C_Byte(0x90);

    v_act = HDMI_ReadI2C_Byte(0x8b);
    v_act = (v_act << 8) + HDMI_ReadI2C_Byte(0x8c);

    h_act = HDMI_ReadI2C_Byte(0x8d);
    h_act = (h_act << 8) + HDMI_ReadI2C_Byte(0x8e);

    CLK_Cnt = (HDMI_ReadI2C_Byte(0x1d) & 0x0f) * 0x10000 + HDMI_ReadI2C_Byte(0x1e) * 0x100 + HDMI_ReadI2C_Byte(0x1f);
#else
    unsigned char temp;

    vs_pol = 0;
    hs_pol = 0;

    /* hs vs polarity */
    temp = HDMI_ReadI2C_Byte(0x70);
    if (temp & 0x02) {
        vs_pol = 1;
    }
    if (temp & 0x01) {
        hs_pol = 1;
    }

    vs_width = HDMI_ReadI2C_Byte(0x71);

    hs_width = HDMI_ReadI2C_Byte(0x72);
    hs_width = (hs_width << 8) + HDMI_ReadI2C_Byte(0x73);

    vbp = HDMI_ReadI2C_Byte(0x74);
    vfp = HDMI_ReadI2C_Byte(0x75);

    hbp = HDMI_ReadI2C_Byte(0x76);
    hbp = (hbp << 8) + HDMI_ReadI2C_Byte(0x77);

    hfp = HDMI_ReadI2C_Byte(0x78);
    hfp = (hfp << 8) + HDMI_ReadI2C_Byte(0x79);

    v_tal = HDMI_ReadI2C_Byte(0x7a);
    v_tal = (v_tal << 8) + HDMI_ReadI2C_Byte(0x7b);

    h_tal = HDMI_ReadI2C_Byte(0x7c);
    h_tal = (h_tal << 8) + HDMI_ReadI2C_Byte(0x7d);

    v_act = HDMI_ReadI2C_Byte(0x7e);
    v_act = (v_act << 8) + HDMI_ReadI2C_Byte(0x7f);

    h_act = HDMI_ReadI2C_Byte(0x80);
    h_act = (h_act << 8) + HDMI_ReadI2C_Byte(0x81);

    CLK_Cnt = (HDMI_ReadI2C_Byte(0x1d) & 0x0f) * 0x10000 + HDMI_ReadI2C_Byte(0x1e) * 0x100 + HDMI_ReadI2C_Byte(0x1f);
#endif
}

static void LT8618SXB_TTL_Input_Analog(void)
{
    /* TTL mode */
    /* register bank */
    HDMI_WriteI2C_Byte(0xff, 0x81);
    HDMI_WriteI2C_Byte(0x02, 0x66);
    HDMI_WriteI2C_Byte(0x0a, 0x06);
    HDMI_WriteI2C_Byte(0x15, 0x06);

    /* for U2 */
    HDMI_WriteI2C_Byte(0x4e, 0xa8);

    HDMI_WriteI2C_Byte(0xff, 0x82);
    HDMI_WriteI2C_Byte(0x1b, 0x77);
    /* 25000 */
    HDMI_WriteI2C_Byte(0x1c, 0xEC);
}

typedef enum {
    LT8618SX_YC_NORM_BIT_NORM = 0,
    LT8618SX_YC_REVERSE_BIT_NORM,
    LT8618SX_YC_NORM_BIT_REVERSE,
    LT8618SX_YC_REVERSE_BIT_REVERSE,
} lt8618sx_yc_bit_mode;

static unsigned char lt8618sx_get_yc_bit_mode(void)
{
    unsigned char mode_value;
    lt8618sx_yc_bit_mode mode_flag = LT8618SX_YC_NORM_BIT_NORM;

    mode_value = _D8_D23_In + LT8618SXB_D0_D7_Bit_Swap_0 + LT8618SXB_D8_D15_Bit_Swap_0 +
        LT8618SXB_D16_D23_Bit_Swap_0;

    if((yc_reverse == 1) && (bit_reverse == 0)) {
        mode_flag = LT8618SX_YC_REVERSE_BIT_NORM;
    } else if((yc_reverse == 0) && (bit_reverse == 1)) {
        mode_flag = LT8618SX_YC_REVERSE_BIT_REVERSE;
    } else if((yc_reverse == 1) && (bit_reverse == 1)) {
        mode_flag = LT8618SX_YC_NORM_BIT_REVERSE;
    }

    switch(mode_flag) {
        case LT8618SX_YC_REVERSE_BIT_NORM:
            mode_value = _D8_D23_In_2 + LT8618SXB_D0_D7_Bit_Swap_0 + LT8618SXB_D8_D15_Bit_Swap_0 +
                LT8618SXB_D16_D23_Bit_Swap_0;
            break;
        case LT8618SX_YC_NORM_BIT_REVERSE:
            mode_value = _D8_D23_In + LT8618SXB_D0_D7_Bit_Swap_1 + LT8618SXB_D8_D15_Bit_Swap_2 +
                LT8618SXB_D16_D23_Bit_Swap_4;
            break;
        case LT8618SX_YC_REVERSE_BIT_REVERSE:
            mode_value = _D8_D23_In_2 + LT8618SXB_D0_D7_Bit_Swap_1 + LT8618SXB_D8_D15_Bit_Swap_2 +
                LT8618SXB_D16_D23_Bit_Swap_4;
            break;
        default:
            break;
    }
    printk("lt8618sx: yc_reverse: %d, bit_reverse: %d\n", yc_reverse, bit_reverse);
    return mode_value;
}

static void LT8618SXB_TTL_Input_Digtal(void)
{
    unsigned char yc_bit_mode_value = lt8618sx_get_yc_bit_mode();

    HDMI_WriteI2C_Byte(0xff, 0x80);
#ifdef _Embedded_sync_
    /* Internal generate sync/de control logic clock enable */
    HDMI_WriteI2C_Byte(0x0A, 0xF0);
#else
    /* _External_sync_ */
    HDMI_WriteI2C_Byte(0x0A, 0xC0);
#endif

    /* TTL_Input_Digtal */
    /* register bank */
    HDMI_WriteI2C_Byte(0xff, 0x82);
    /* YC channel swap */
    HDMI_WriteI2C_Byte(0x45, yc_bit_mode_value);

    if (Use_DDRCLK) {
        /* 0x80: dclk */
        HDMI_WriteI2C_Byte(0x4f, 0x80);
    } else {
        /* 0x40: txpll_clk */
        HDMI_WriteI2C_Byte(0x4f, 0x40);
    }
    HDMI_WriteI2C_Byte(0x50, 0x00);
#ifdef _Embedded_sync_

    /* Select BT rx decode det_vs/hs/de */
    HDMI_WriteI2C_Byte(0x51, 0x42);

    /* Embedded sync mode input enable. */
    HDMI_WriteI2C_Byte(0x48, 0x08);
#else
    /* _External_sync_ */
    /* Select TTL process module input video data */
    HDMI_WriteI2C_Byte(0x51, 0x00);
    HDMI_WriteI2C_Byte(0x48, 0x00);
#endif
}

static void LT8618SXB_PLL_setting(void)
{
    unsigned char read_val;
    unsigned char j;
    unsigned char cali_done;
    unsigned char cali_val;
    unsigned char lock;

    CLK_bound = (unsigned char)Format_Timing[Resolution_Num][Clk_bound_SDR + (unsigned char)( Use_DDRCLK )];

    HDMI_WriteI2C_Byte(0xff, 0x81);
    HDMI_WriteI2C_Byte(0x23, 0x40);
    HDMI_WriteI2C_Byte(0x24, 0x64); //icp set
    HDMI_WriteI2C_Byte(0x26, 0x55);

    HDMI_WriteI2C_Byte(0x29, 0x04); //for U3 for U3 SDR/DDR fixed phase

    if (flag_Ver_u3) {
        HDMI_WriteI2C_Byte( 0x25, LT8618SXB_PLL_u3[CLK_bound][0] );
        HDMI_WriteI2C_Byte( 0x2c, LT8618SXB_PLL_u3[CLK_bound][1] );
        HDMI_WriteI2C_Byte( 0x2d, LT8618SXB_PLL_u3[CLK_bound][2] );
    } else {
        HDMI_WriteI2C_Byte( 0x25, LT8618SXB_PLL_u2[CLK_bound][0] );
        HDMI_WriteI2C_Byte( 0x2c, LT8618SXB_PLL_u2[CLK_bound][1] );
        HDMI_WriteI2C_Byte( 0x2d, LT8618SXB_PLL_u2[CLK_bound][2] );
    }

    if (Use_DDRCLK) {
        if (flag_Ver_u3)
        {
            HDMI_WriteI2C_Byte( 0x4d, 0x05 );
            HDMI_WriteI2C_Byte( 0x27, 0x60 ); //0x60 //ddr 0x66
            HDMI_WriteI2C_Byte( 0x28, 0x88 );
        } else {
            read_val = HDMI_ReadI2C_Byte( 0x2c ) & 0x7f;
            read_val = read_val * 2 | 0x80;
            HDMI_WriteI2C_Byte( 0x2c, read_val );

            HDMI_WriteI2C_Byte( 0x4d, 0x04 );
            HDMI_WriteI2C_Byte( 0x27, 0x60 );
            HDMI_WriteI2C_Byte( 0x28, 0x88 );
        }

#ifdef _DEBUG_MODE_
        Debug_Printf( "\r\n PLL DDR" );
#endif
    }else {
        if ( flag_Ver_u3 ) {
            HDMI_WriteI2C_Byte( 0x4d, 0x00 );
            HDMI_WriteI2C_Byte( 0x27, 0x60 ); //0x06
            HDMI_WriteI2C_Byte( 0x28, 0x88 ); // 0x88
        } else {
            HDMI_WriteI2C_Byte( 0x4d, 0x00 );
            HDMI_WriteI2C_Byte( 0x27, 0x60 ); //0x06
            HDMI_WriteI2C_Byte( 0x28, 0x00 ); // 0x88
        }

#ifdef _DEBUG_MODE_
        Debug_Printf( "\r\n PLL SDR" );
#endif
    }

    HDMI_WriteI2C_Byte( 0xff, 0x81 );

    read_val = HDMI_ReadI2C_Byte( 0x2b );
    HDMI_WriteI2C_Byte( 0x2b, read_val & 0xfd );  // sw_en_txpll_cal_en

    read_val = HDMI_ReadI2C_Byte( 0x2e );
    HDMI_WriteI2C_Byte( 0x2e, read_val & 0xfe );  //sw_en_txpll_iband_set

    HDMI_WriteI2C_Byte( 0xff, 0x82 );
    HDMI_WriteI2C_Byte( 0xde, 0x00 );
    HDMI_WriteI2C_Byte( 0xde, 0xc0 );

    HDMI_WriteI2C_Byte( 0xff, 0x80 );
    HDMI_WriteI2C_Byte( 0x16, 0xf1 );
    HDMI_WriteI2C_Byte( 0x18, 0xdc ); //txpll _sw_rst_n
    HDMI_WriteI2C_Byte( 0x18, 0xfc );
    HDMI_WriteI2C_Byte( 0x16, 0xf3 );

    HDMI_WriteI2C_Byte( 0xff, 0x81 );

    if (flag_Ver_u3) {
        if (Use_DDRCLK) {
            HDMI_WriteI2C_Byte( 0x2a, 0x10 );
            HDMI_WriteI2C_Byte( 0x2a, 0x30 );
        } else {
            HDMI_WriteI2C_Byte( 0x2a, 0x00 );
            HDMI_WriteI2C_Byte( 0x2a, 0x20 );
        }
    }

    for (j = 0; j < 0x05; j++)
    {
        msleep( 10 );
        HDMI_WriteI2C_Byte( 0xff, 0x80 );
        HDMI_WriteI2C_Byte( 0x16, 0xe3 ); /* pll lock logic reset */
        HDMI_WriteI2C_Byte( 0x16, 0xf3 );

        HDMI_WriteI2C_Byte( 0xff, 0x82 );
        lock       = 0x80 & HDMI_ReadI2C_Byte( 0x15 );
        cali_val   = HDMI_ReadI2C_Byte( 0xea );
        cali_done  = 0x80 & HDMI_ReadI2C_Byte( 0xeb );

        if( lock && cali_done && ( cali_val != 0xff ) )
        {
#ifdef _DEBUG_MODE_

            Debug_Printf( "\r\nTXPLL Lock" );
#endif
        } else {
            HDMI_WriteI2C_Byte( 0xff, 0x80 );
            HDMI_WriteI2C_Byte( 0x16, 0xf1 );
            HDMI_WriteI2C_Byte( 0x18, 0xdc ); //txpll _sw_rst_n
            HDMI_WriteI2C_Byte( 0x18, 0xfc );
            HDMI_WriteI2C_Byte( 0x16, 0xf3 );
#ifdef _DEBUG_MODE_
            Debug_Printf( "\r\nTXPLL Reset" );
#endif
        }
    }
}

static void LT8618SXB_Audio_setting(void)
{
    /* IIS Input */
    /* register bank */
    HDMI_WriteI2C_Byte(0xff, 0x82);
    /* bit7 = 0 : DVI output; bit7 = 1: HDMI output */
    HDMI_WriteI2C_Byte(0xd6, 0x8e);
    /* sync polarity */
    HDMI_WriteI2C_Byte(0xd7, 0x04);
    /* register bank */
    HDMI_WriteI2C_Byte(0xff, 0x84);
    HDMI_WriteI2C_Byte(0x06, 0x08);
    /* SD0 channel selected */
    HDMI_WriteI2C_Byte(0x07, 0x10);
    /* 0x00 :Left justified; default */
    HDMI_WriteI2C_Byte(0x09, 0x00);
    /* 0x02 :Right justified; */
    HDMI_WriteI2C_Byte(0x0f, 0x0b + Sample_Freq[_48KHz]);
    /* CTS_N / 2; 32bit */
    HDMI_WriteI2C_Byte(0x34, 0xd4);
    /* CTS_N / 4; 16bit */
    HDMI_WriteI2C_Byte(0x35, (unsigned char)(IIS_N[_48KHz] / 0x10000));
    HDMI_WriteI2C_Byte(0x36, (unsigned char)((IIS_N[_48KHz] & 0x00FFFF) / 0x100));
    HDMI_WriteI2C_Byte(0x37, (unsigned char)(IIS_N[_48KHz] & 0x0000FF));
    /* Null packet enable */
    HDMI_WriteI2C_Byte(0x3c, 0x21);
}

static void LT8618SXB_CSC_setting(void)
{
    /* register bank  */
    HDMI_WriteI2C_Byte(0xff, 0x82);

    /* No csc */
    HDMI_WriteI2C_Byte(0xb9, 0x00);
}

static void LT8618SXB_AVI_setting(void)
{
    unsigned char AVI_PB0;
    unsigned char AVI_PB1;
    unsigned char AVI_PB2;

    /* PB1,color space: YUV444 0x70;YUV422 0x30; RGB 0x10 */
    AVI_PB1 = 0x30;

    /* PB2; picture aspect rate: 0x19:4:3 ;     0x2A : 16:9 */
    AVI_PB2 = Format_Timing[Resolution_Num][Pic_Ratio];
    AVI_PB0 = ((AVI_PB1 + AVI_PB2 + VIC_Num) <= 0x6f) ? (0x6f - AVI_PB1 - AVI_PB2 - VIC_Num) :
                                                        (0x16f - AVI_PB1 - AVI_PB2 - VIC_Num);
    /* register bank */
    HDMI_WriteI2C_Byte(0xff, 0x84);
    /* PB0,avi packet checksum */
    HDMI_WriteI2C_Byte(0x43, AVI_PB0);
    /* PB1,color space: YUV444 0x70;YUV422 0x30; RGB 0x10 */
    HDMI_WriteI2C_Byte(0x44, AVI_PB1);
    /* PB2;picture aspect rate: 0x19:4:3 ; 0x2A : 16:9 */
    HDMI_WriteI2C_Byte(0x45, AVI_PB2);
    /* PB4;vic ,0x10: 1080P ;  0x04 : 720P */
    HDMI_WriteI2C_Byte(0x47, VIC_Num);
    /* data iland */
    HDMI_WriteI2C_Byte(0x10, 0x30);
    /* act_h_blank */
    HDMI_WriteI2C_Byte(0x12, 0x64);
    if (VIC_Num == 95) {
        /* UD1 infoframe enable */
        HDMI_WriteI2C_Byte(0x3d, 0x2a);
        HDMI_WriteI2C_Byte(0x74, 0x81);
        HDMI_WriteI2C_Byte(0x75, 0x01);
        HDMI_WriteI2C_Byte(0x76, 0x05);
        HDMI_WriteI2C_Byte(0x77, 0x49);
        HDMI_WriteI2C_Byte(0x78, 0x03);
        HDMI_WriteI2C_Byte(0x79, 0x0c);
        HDMI_WriteI2C_Byte(0x7a, 0x00);
        HDMI_WriteI2C_Byte(0x7b, 0x20);
        HDMI_WriteI2C_Byte(0x7c, 0x01);
    } else {
        /*
         * UD1 infoframe disable
         */
        HDMI_WriteI2C_Byte(0x3d, 0x0a);
    }
}

static void LT8618SXB_TX_Phy(void)
{
    /*
     * register bank
     */
    HDMI_WriteI2C_Byte(0xff, 0x81);
    HDMI_WriteI2C_Byte(0x30, 0xea);
    HDMI_WriteI2C_Byte(0x31, 0x44);
    HDMI_WriteI2C_Byte(0x32, 0x4a);
    HDMI_WriteI2C_Byte(0x33, 0x0b);
    HDMI_WriteI2C_Byte(0x34, 0x00);
    HDMI_WriteI2C_Byte(0x35, 0x00);
    HDMI_WriteI2C_Byte(0x36, 0x00);
    HDMI_WriteI2C_Byte(0x37, 0x44);
    HDMI_WriteI2C_Byte(0x3f, 0x0f);
    /*
     * 0xa0 -- CLK tap0 swing
     */
    HDMI_WriteI2C_Byte(0x40, 0xa0);
    /*
     * 0xa0 -- D0 tap0 swing
     */
    HDMI_WriteI2C_Byte(0x41, 0xa0);
    /*
     * 0xa0 -- D1 tap0 swing
     */
    HDMI_WriteI2C_Byte(0x42, 0xa0);
    /*
     * 0xa0 -- D2 tap0 swing
     */
    HDMI_WriteI2C_Byte(0x43, 0xa0);
    HDMI_WriteI2C_Byte(0x44, 0x0a);
}

#ifdef _Embedded_sync_
static void LT8618SXB_BT_Timing_setting(void)
{
    HDMI_WriteI2C_Byte(0xff, 0x82);
    HDMI_WriteI2C_Byte(0x20, (unsigned char)(Format_Timing[Resolution_Num][H_act] / 256));
    HDMI_WriteI2C_Byte(0x21, (unsigned char)(Format_Timing[Resolution_Num][H_act] % 256));
    HDMI_WriteI2C_Byte(0x22, (unsigned char)(Format_Timing[Resolution_Num][H_fp] / 256));
    HDMI_WriteI2C_Byte(0x23, (unsigned char)(Format_Timing[Resolution_Num][H_fp] % 256));
    HDMI_WriteI2C_Byte(0x24, (unsigned char)(Format_Timing[Resolution_Num][H_sync] / 256));
    HDMI_WriteI2C_Byte(0x25, (unsigned char)(Format_Timing[Resolution_Num][H_sync] % 256));
    HDMI_WriteI2C_Byte(0x26, 0x00);
    HDMI_WriteI2C_Byte(0x27, 0x00);
    HDMI_WriteI2C_Byte(0x36, (unsigned char)(Format_Timing[Resolution_Num][V_act] / 256));
    HDMI_WriteI2C_Byte(0x37, (unsigned char)(Format_Timing[Resolution_Num][V_act] % 256));
    HDMI_WriteI2C_Byte(0x38, (unsigned char)(Format_Timing[Resolution_Num][V_fp] / 256));
    HDMI_WriteI2C_Byte(0x39, (unsigned char)(Format_Timing[Resolution_Num][V_fp] % 256));
    HDMI_WriteI2C_Byte(0x3a, (unsigned char)(Format_Timing[Resolution_Num][V_bp] / 256));
    HDMI_WriteI2C_Byte(0x3b, (unsigned char)(Format_Timing[Resolution_Num][V_bp] % 256));
    HDMI_WriteI2C_Byte(0x3c, (unsigned char)(Format_Timing[Resolution_Num][V_sync] / 256));
    HDMI_WriteI2C_Byte(0x3d, (unsigned char)(Format_Timing[Resolution_Num][V_sync] % 256));
}

unsigned char LT8618SX_Phase( void )
{
    unsigned char temp       = 0;
    unsigned char read_value = 0;
    unsigned char b_ok       = 0;
    unsigned char Temp_f     = 0;

    for (temp = 0; temp < 0x0a; temp++) {
        HDMI_WriteI2C_Byte( 0xff, 0x81 );
        HDMI_WriteI2C_Byte( 0x27, ( 0x60 + temp ) );
        HDMI_WriteI2C_Byte( 0x4d, 0x01 );
        HDMI_WriteI2C_Byte( 0x4d, 0x09 );

        read_value = HDMI_ReadI2C_Byte( 0x50 );

#ifdef _Phase_Debug_
        Debug_Printf( "\r\ntemp=" );
        Debug_DispNum( temp );
        Debug_Printf( "\r\nread_value=" );
        Debug_DispNum( read_value );
#endif
        if (read_value == 0x00) {
            if (b_ok == 0) {
                Temp_f = temp;
            }
            b_ok = 1;
        } else {
            b_ok = 0;
        }
    }
#ifdef _Phase_Debug_
    Debug_Printf( "\r\nTemp_f=" );
    Debug_DispNum( Temp_f );
#endif
    HDMI_WriteI2C_Byte( 0xff, 0x81 );
    HDMI_WriteI2C_Byte( 0x27, ( 0x60 + Temp_f ) );
    return Temp_f;
}

static void LT8618SX_Phase_1( void )
{
	u8		temp		   = 0;
	u8		read_val	   = 0;
//	u8		Pre_read_val   = 0;
//	bool	b_ok		   = 0;

	u8		OK_CNT	   = 0x00;
	u8		OK_CNT_1   = 0x00;
	u8		OK_CNT_2   = 0x00;
	u8		OK_CNT_3   = 0x00;
	u8		Jump_CNT   = 0x00;
	u8		Jump_Num   = 0x00;
	u8		Jump_Num_1 = 0x00;
	u8		Jump_Num_2 = 0x00;
	u8		Jump_Num_3 = 0x00;
	bool	temp0_ok   = 0;
	bool	temp9_ok   = 0;
	bool	b_OK	   = 0;

	HDMI_WriteI2C_Byte( 0xff, 0x80 );	// register bank
	HDMI_WriteI2C_Byte( 0x13, 0xf1 );
	mdelay( 5 );
	HDMI_WriteI2C_Byte( 0x13, 0xf9 );	// Reset TTL video process
	mdelay( 10 );

	HDMI_WriteI2C_Byte( 0xff, 0x81 );

	for( temp = 0; temp < 0x0a; temp++ )
	{
		HDMI_WriteI2C_Byte( 0x27, ( 0x60 + temp ) );
//#ifdef _DDR_
		if( Use_DDRCLK )
		{
			HDMI_WriteI2C_Byte( 0x4d, 0x05 );
			mdelay( 5 );
			HDMI_WriteI2C_Byte( 0x4d, 0x0d );
			//	mdelay( 50 );
		}
else
		{
			HDMI_WriteI2C_Byte( 0x4d, 0x01 );
			mdelay( 5 );
			HDMI_WriteI2C_Byte( 0x4d, 0x09 );
		}
//#endif
		mdelay( 10 );
		read_val = HDMI_ReadI2C_Byte( 0x50 ) & 0x01;

		if( read_val == 0 )
		{
			OK_CNT++;

			if( b_OK == 0 )
			{
				b_OK = 1;
				Jump_CNT++;

				if( Jump_CNT == 1 )
				{
					Jump_Num_1 = temp;
				}else if( Jump_CNT == 3 )
				{
					Jump_Num_2 = temp;
				}else if( Jump_CNT == 5 )
				{
					Jump_Num_3 = temp;
				}
			}

			if( Jump_CNT == 1 )
			{
				OK_CNT_1++;
			}else if( Jump_CNT == 3 )
			{
				OK_CNT_2++;
			}else if( Jump_CNT == 5 )
			{
				OK_CNT_3++;
			}

			if( temp == 0 )
			{
				temp0_ok = 1;
			}
			if( temp == 9 )
			{
				Jump_CNT++;
				temp9_ok = 1;
			}
		}else
		{
			if( b_OK )
			{
				b_OK = 0;
				Jump_CNT++;
			}
		}

	}


	if( ( Jump_CNT == 0 ) || ( Jump_CNT > 6 ) )
	{
		return;
	}

	if( ( temp9_ok == 1 ) && ( temp0_ok == 1 ) )
	{
		if( Jump_CNT == 6 )
		{
			OK_CNT_3   = OK_CNT_3 + OK_CNT_1;
			OK_CNT_1   = 0;
		}else if( Jump_CNT == 4 )
		{
			OK_CNT_2   = OK_CNT_2 + OK_CNT_1;
			OK_CNT_1   = 0;
		}
	}
	if( Jump_CNT >= 2 )
	{
		if( OK_CNT_1 >= OK_CNT_2 )
		{
			if( OK_CNT_1 >= OK_CNT_3 )
			{
				OK_CNT	   = OK_CNT_1;
				Jump_Num   = Jump_Num_1;
			}else
			{
				OK_CNT	   = OK_CNT_3;
				Jump_Num   = Jump_Num_3;
			}
		}else
		{
			if( OK_CNT_2 >= OK_CNT_3 )
			{
				OK_CNT	   = OK_CNT_2;
				Jump_Num   = Jump_Num_2;
			}else
			{
				OK_CNT	   = OK_CNT_3;
				Jump_Num   = Jump_Num_3;
			}
		}
	}

	HDMI_WriteI2C_Byte( 0xff, 0x81 );

	if( ( Jump_CNT == 2 ) || ( Jump_CNT == 4 ) || ( Jump_CNT == 6 ) )
	{
		HDMI_WriteI2C_Byte( 0x27, ( 0x60 + ( Jump_Num + ( OK_CNT / 2 ) ) % 0x0a ) );
	}
	else
	if( OK_CNT >= 0x09 )
	{
		HDMI_WriteI2C_Byte( 0x27, 0x65 );
	}

	return;
}


bool LT8618SXB_Phase_config( void )
{
    unsigned char      Temp       = 0x00;
    unsigned char      Temp_f     = 0x00;
    unsigned char      OK_CNT     = 0x00;
    unsigned char      OK_CNT_1   = 0x00;
    unsigned char      OK_CNT_2   = 0x00;
    unsigned char      OK_CNT_3   = 0x00;
    unsigned char      Jump_CNT   = 0x00;
    unsigned char      Jump_Num   = 0x00;
    unsigned char      Jump_Num_1 = 0x00;
    unsigned char      Jump_Num_2 = 0x00;
    unsigned char      Jump_Num_3 = 0x00;
    bool    temp0_ok   = 0;
    bool    temp9_ok   = 0;
    bool    b_OK       = 0;
    unsigned short     V_ACT      = 0x0000;
    unsigned short     H_ACT      = 0x0000;
    unsigned short     H_TOTAL    = 0x0000;

    Temp_f = LT8618SX_Phase( );                 //it's setted before video check

    while( Temp <= 0x09 )
    {
        HDMI_WriteI2C_Byte( 0xff, 0x81 );
        HDMI_WriteI2C_Byte( 0x27, ( 0x60 + Temp ) );
        HDMI_WriteI2C_Byte( 0xff, 0x80 );
        HDMI_WriteI2C_Byte( 0x13, 0xf1 ); //ttl video process reset///20191121
        HDMI_WriteI2C_Byte( 0x12, 0xfb ); //video check reset//20191121
        msleep( 5 );                          //add 20191121
        HDMI_WriteI2C_Byte( 0x12, 0xff ); //20191121
        HDMI_WriteI2C_Byte( 0x13, 0xf9 ); //20191121

        msleep( 80 );

        HDMI_WriteI2C_Byte( 0xff, 0x81 );
        HDMI_WriteI2C_Byte( 0x51, 0x42 );

        HDMI_WriteI2C_Byte( 0xff, 0x82 );
        H_TOTAL    = HDMI_ReadI2C_Byte( 0x8f );
        H_TOTAL    = ( H_TOTAL << 8 ) + HDMI_ReadI2C_Byte( 0x90 );
        V_ACT      = HDMI_ReadI2C_Byte( 0x8b );
        V_ACT      = ( V_ACT << 8 ) + HDMI_ReadI2C_Byte( 0x8c );
        H_ACT      = HDMI_ReadI2C_Byte( 0x8d );
        H_ACT      = ( H_ACT << 8 ) + HDMI_ReadI2C_Byte( 0x8e ) - 0x04; //note

#ifdef _Phase_Debug_
        Debug_Printf( "\r\n h_total=" );
        Debug_DispNum( H_TOTAL );
        Debug_Printf( "\r\n v_act=" );
        Debug_DispNum( V_ACT );
        Debug_Printf( "\r\n h_act=" );
        Debug_DispNum( H_ACT );
#endif
        if( ( V_ACT > ( Format_Timing[Resolution_Num][V_act] - 5 ) )
            && ( V_ACT < ( Format_Timing[Resolution_Num][V_act] + 5 ) )
            && ( H_ACT > ( Format_Timing[Resolution_Num][H_act] - 5 ) )
            && ( H_ACT < ( Format_Timing[Resolution_Num][H_act] + 5 ) )
            && ( H_TOTAL > ( Format_Timing[Resolution_Num][H_tol] - 5 ) )
            && ( H_TOTAL < ( Format_Timing[Resolution_Num][H_tol] + 5 ) ) )

        {
            OK_CNT++;

            if( b_OK == 0 )
            {
                b_OK = 1;
                Jump_CNT++;

                if( Jump_CNT == 1 )
                {
                    Jump_Num_1 = Temp;
                }else if( Jump_CNT == 3 )
                {
                    Jump_Num_2 = Temp;
                }else if( Jump_CNT == 5 )
                {
                    Jump_Num_3 = Temp;
                }
            }

            if( Jump_CNT == 1 )
            {
                OK_CNT_1++;
            }else if( Jump_CNT == 3 )
            {
                OK_CNT_2++;
            }else if( Jump_CNT == 5 )
            {
                OK_CNT_3++;
            }

            if( Temp == 0 )
            {
                temp0_ok = 1;
            }
            if( Temp == 9 )
            {
                Jump_CNT++;
                temp9_ok = 1;
            }

#ifdef _Phase_Debug_
            Debug_Printf( "\r\n this phase is ok,temp=" );
            Debug_DispNum( Temp );
            Debug_Printf( "\r\n Jump_CNT=" );
            Debug_DispNum( Jump_CNT );
#endif
        }else

        {
            if( b_OK )
            {
                b_OK = 0;
                Jump_CNT++;
            }
#ifdef _Phase_Debug_
            Debug_Printf( "\r\n this phase is fail,temp=" );
            Debug_DispNum( Temp );
            Debug_Printf( "\r\n Jump_CNT=" );
            Debug_DispNum( Jump_CNT );
#endif
        }

        Temp++;
    }

#ifdef _Phase_Debug_
    Debug_Printf( "\r\n OK_CNT_1=" );
    Debug_DispNum( OK_CNT_1 );
    Debug_Printf( "\r\n OK_CNT_2=" );
    Debug_DispNum( OK_CNT_2 );
    Debug_Printf( "\r\n OK_CNT_3=" );
    Debug_DispNum( OK_CNT_3 );
#endif

    if( ( Jump_CNT == 0 ) || ( Jump_CNT > 6 ) )
    {
#ifdef _Phase_Debug_
        Debug_Printf( "\r\ncali phase fail" );
#endif
        return 0;
    }

    if( ( temp9_ok == 1 ) && ( temp0_ok == 1 ) )
    {
        if( Jump_CNT == 6 )
        {
            OK_CNT_3   = OK_CNT_3 + OK_CNT_1;
            OK_CNT_1   = 0;
        }else if( Jump_CNT == 4 )
        {
            OK_CNT_2   = OK_CNT_2 + OK_CNT_1;
            OK_CNT_1   = 0;
        }
    }
    if( Jump_CNT >= 2 )
    {
        if( OK_CNT_1 >= OK_CNT_2 )
        {
            if( OK_CNT_1 >= OK_CNT_3 )
            {
                OK_CNT     = OK_CNT_1;
                Jump_Num   = Jump_Num_1;
            }else
            {
                OK_CNT     = OK_CNT_3;
                Jump_Num   = Jump_Num_3;
            }
        }else
        {
            if( OK_CNT_2 >= OK_CNT_3 )
            {
                OK_CNT     = OK_CNT_2;
                Jump_Num   = Jump_Num_2;
            }else
            {
                OK_CNT     = OK_CNT_3;
                Jump_Num   = Jump_Num_3;
            }
        }
    }
    HDMI_WriteI2C_Byte( 0xff, 0x81 );

    if( ( Jump_CNT == 2 ) || ( Jump_CNT == 4 ) || ( Jump_CNT == 6 ) )
    {
        HDMI_WriteI2C_Byte( 0x27, ( 0x60 + ( Jump_Num + ( OK_CNT / 2 ) ) % 0x0a ) );
    }

    if( OK_CNT == 0x0a )
    {
        HDMI_WriteI2C_Byte( 0x27, ( 0x60 + ( Temp_f + 5 ) % 0x0a ) );
    }

#ifdef _Phase_Debug_
    Debug_DispStrNum( "cail phase is 0x%x", HDMI_ReadI2C_Byte( 0x27 ) );
#endif

    return 1;
}

#endif

static bool lt_check_module_param(void)
{
    if (norm >= LT_OUT_BUTT) {
        printk("module param norm(%u) invalid, it should be in [%u,%u]\n", norm, 0, LT_OUT_USER);
        return -1;
    }

    if ((Use_DDRCLK !=  LT_CLK_EDGE_SINGLE) && (Use_DDRCLK != LT_CLK_EDGE_DUAL)) {
        printk("module param Use_DDRCLK(%u) invalid, it should be 0 or 1\n", Use_DDRCLK);
        return -1;
    }
    return 0;
}

/*
 * When the lt8618sxb works, the resolution of the bt1120 signal changes.
 * The following settings need to be configured.
 */
void Resolution_change(unsigned int Resolution)
{
    /*
     * Parameters required by LT8618SXB_BT_Timing_setting( void )
     */
    Resolution_Num = Resolution;

    /*
     * Parameters required by LT8618SXB_PLL_setting( void )
     */
    CLK_bound = Format_Timing[Resolution_Num][Clk_bound_SDR + (unsigned char)(Use_DDRCLK)];
    VIC_Num = Format_Timing[Resolution_Num][Vic];
    LT8618SXB_PLL_setting();
    LT8618SXB_AVI_setting();
#ifdef _Embedded_sync_
    LT8618SXB_BT_Timing_setting();
    if (!flag_Ver_u3) {
        LT8618SXB_Phase_config();
    } else {
        LT8618SX_Phase_1();
    }
#endif
}

#ifdef __LITEOS__

int LT8618SX_Initial(void *args)
{
    int ret = 0;

    if (args != NULL) {
        norm = *(unsigned int *)args;
        DPRINTK(DEBUG_LEVEL, "init video norm = %u\n", norm);
    }

    if (lt_check_module_param() != 0) {
        return -1;
    }

    I2CADR = _LT8618SX_ADR;

    ret = register_driver("/dev/" DEV_NAME, &lt_fops, 0666, 0); /* 0666 is RW */
    if (ret) {
        DPRINTK(0, "could not register %s device", DEV_NAME);
        goto end0;
    }

    ret = i2c_client_init();
    if (ret != 0) {
        DPRINTK(0, "could not init i2c client\n");
        goto end1;
    }
    LT8618SX_VideoSel(norm);

    /* wait 2000ms, otherwise i2c not work */
    DPRINTK(DEBUG_LEVEL, "mdelay 2000.\n");
    mdelay(2000);  /* delay 2000 ms */

    HDMI_WriteI2C_Byte(0xff, 0x80); // register bank
    HDMI_WriteI2C_Byte(0xee, 0x01); // enable IIC

    LT8618SXB_Chip_ID();

    LT8618SXB_RST_PD_Init();

    /* TTL mode */
    LT8618SXB_TTL_Input_Analog();

    LT8618SXB_TTL_Input_Digtal();

    /*
     * Wait for the signal to be stable and decide whether the delay is
     * necessary according to the actual situation
     */
    mdelay(1000);

    /*
     * For debug
     */
    LT8618SXB_Video_check();

    /*
     * PLL
     */
    LT8618SXB_PLL_setting();
    LT8618SXB_Audio_setting();
    LT8618SXB_CSC_setting();
#ifdef _LT8618_HDCP_
    LT8618SXB_HDCP_Init();
#endif
    LT8618SXB_AVI_setting();
    /*
     * This operation is not necessary. Read TV EDID if necessary.
     * Read TV  EDID
     */
    LT8918SXB_Read_EDID();
#ifdef _LT8618_HDCP_
    LT8618SXB_HDCP_Enable();
#endif

#ifdef _Embedded_sync_
    LT8618SXB_BT_Timing_setting();
    if (!flag_Ver_u3) {
        LT8618SXB_Phase_config();
    }
#endif

    LT8618SXB_RST_PD_Init();
    LT8618SXB_TX_Phy();

    printk("load lt8618sx.ko ....OK!\n");
    return 0;
end1:
    unregister_driver("/dev/" DEV_NAME);
end0:
    printk("load lt8618sx.ko ....failed!\n");
    return -1;
}

void LT8618SX_DeInitial(void)
{
    lt_reset_ttl();
    i2c_client_exit();
    unregister_driver("/dev/" DEV_NAME);
    printk("unload lt8618sx.ko ....OK!\n");
}

#else

static int __init LT8618SX_Initial(void)
{
    int ret = 0;

    if (lt_check_module_param() != 0) {
        return -1;
    }

    I2CADR = _LT8618SX_ADR;

    ret = misc_register(&lt_dev);
    if (ret) {
        DPRINTK(0, "could not register lt8618sx device\n");
        goto end0;
    }
    lt_reboot_init();

    ret = i2c_client_init();
    if (ret != 0) {
        DPRINTK(0, "could not init i2c client\n");
        goto end1;
    }
    LT8618SX_VideoSel(norm);

    HDMI_WriteI2C_Byte(0xff, 0x80); // register bank
    HDMI_WriteI2C_Byte(0xee, 0x01); // enable IIC

    if (LT8618SXB_Chip_ID() == false) {
        i2c_client_exit();
        goto end1;
    }

    LT8618SXB_RST_PD_Init();

    /* TTL mode */
    LT8618SXB_TTL_Input_Analog();

    LT8618SXB_TTL_Input_Digtal();

    /*
     * Wait for the signal to be stable and decide whether the delay is
     * necessary according to the actual situation
     */
    mdelay(1000);

    /*
     * For debug
     */
    LT8618SXB_Video_check();

    /*
     * PLL
     */
    LT8618SXB_PLL_setting();
    LT8618SXB_Audio_setting();
    LT8618SXB_CSC_setting();
#ifdef _LT8618_HDCP_
    LT8618SXB_HDCP_Init();
#endif
    LT8618SXB_AVI_setting();
    /*
     * This operation is not necessary. Read TV EDID if necessary.
     * Read TV  EDID
     */
    LT8918SXB_Read_EDID();
#ifdef _LT8618_HDCP_
    LT8618SXB_HDCP_Enable();
#endif

#ifdef _Embedded_sync_
    LT8618SXB_BT_Timing_setting();
    if (!flag_Ver_u3) {
        LT8618SXB_Phase_config();
    } else {
        LT8618SX_Phase_1();
    }
#endif

    LT8618SXB_RST_PD_Init();
    LT8618SXB_TX_Phy();

    printk("load lt8618sx.ko ....OK!\n");
    return 0;
end1:
    lt_reboot_exit();
    misc_deregister(&lt_dev);
end0:
    printk("load lt8618sx.ko ....failed!\n");
    return -1;
}

static void __exit LT8618SX_DeInitial(void)
{
    lt_reset_ttl();
    i2c_client_exit();
    lt_reboot_exit();
    misc_deregister(&lt_dev);
    printk("unload lt8618sx.ko ....OK!\n");
}

module_init(LT8618SX_Initial);
module_exit(LT8618SX_DeInitial);

MODULE_LICENSE("GPL");

#endif

