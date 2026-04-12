#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_comm_video.h"
#include "hi_sns_ctrl.h"

#ifdef HI_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "hi_i2c.h"
#endif

const unsigned char os08a10_i2c_addr     =    0x6c;        /* I2C Address of os08a10 */
const unsigned int  os08a10_addr_byte    =    2;
const unsigned int  os08a10_data_byte    =    1;
static int g_fd[ISP_MAX_DEV_NUM] = {-1, -1};

extern ISP_SNS_STATE_S        g_astos08a10[ISP_MAX_DEV_NUM];
extern ISP_SNS_COMMBUS_U      g_aunos08a10BusInfo[];
extern ISP_SNS_STATE_S     *g_apstSnsState[ISP_MAX_DEV_NUM];

//sensor fps mode
#define os08a10_SENSOR_4k2k_30FPS_LINEAR_MODE      (1)
#define os08a10_SENSOR_4k2k_30FPS_2t1_WDR_MODE     (3)

int os08a10_i2c_init(ISP_DEV IspDev)
{
    char acDevFile[16] = {0};
    HI_U8 u8DevNum;

    if(g_fd[IspDev] >= 0)
    {
        return 0;
    }
#ifdef HI_GPIO_I2C
    int ret;

    g_fd[IspDev] = open("/dev/gpioi2c_ex", 0);
    if(g_fd[IspDev] < 0)
    {
        printf("Open gpioi2c_ex error!\n");
        return -1;
    }
#else
    int ret;

    u8DevNum = g_aunos08a10BusInfo[IspDev].s8I2cDev;
    snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile)-1, "/dev/i2c-%d", u8DevNum);

    g_fd[IspDev] = open(acDevFile, O_RDWR);
    if(g_fd[IspDev] < 0)
    {
        printf("Open /dev/i2c-%d error!\n", IspDev);
        return -1;
    }

    ret = ioctl(g_fd[IspDev], I2C_SLAVE_FORCE, (os08a10_i2c_addr>>1));
    if (ret < 0)
    {
        printf("CMD_SET_DEV error!\n");
        return ret;
    }
#endif

    return 0;
}

int os08a10_i2c_exit(ISP_DEV IspDev)
{
    if (g_fd[IspDev] >= 0)
    {
        close(g_fd[IspDev]);
        g_fd[IspDev] = -1;
        return 0;
    }
    return -1;
}

int os08a10_read_register(ISP_DEV IspDev,int addr)
{
    // TODO:

    return 0;
}

#ifdef __HuaweiLite__
int os08a10_write_register(ISP_DEV IspDev,int addr, int data)
{
    if (0 > g_fd[IspDev])
    {
        return 0;
    }
#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = os08a10_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = os08a10_addr_byte;
    i2c_data.data = data;
    i2c_data.data_byte_num = os08a10_data_byte;
    ret = ioctl(g_fd[IspDev], GPIO_I2C_WRITE, &i2c_data);
    if (ret)
    {
        printf("GPIO-I2C write faild!\n");
        return ret;
    }
#else
    int idx = 0;
    int ret;
    char buf[8];
    buf[idx++] = addr & 0xff;
    if (os08a10_addr_byte == 2)
    {
        ret = ioctl(g_fd[IspDev], I2C_16BIT_REG, 1);
        buf[idx++] = (addr >> 8) & 0xff;
    }
    else
    {
        //ret = ioctl(g_fd[IspDev], I2C_16BIT_REG, 0);
    }
    buf[idx++] = data & 0xff;
    if (os08a10_data_byte == 2)
    {
        //ret = ioctl(g_fd[IspDev], I2C_16BIT_DATA, 1);
        //buf[idx++] = (data >> 8) & 0xff;
    }
    else
    {
        ret = ioctl(g_fd[IspDev], I2C_16BIT_DATA, 0);
    }
    ret = write(g_fd[IspDev], buf, os08a10_addr_byte + os08a10_data_byte);
    if(ret < 0)
    {
        printf("I2C_WRITE error!\n");
        return -1;
    }
#endif
    return 0;
}
#else
int os08a10_write_register(ISP_DEV IspDev,int addr, int data)
{
    if (0 > g_fd[IspDev])
    {
        return 0;
    }

#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = os08a10_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = os08a10_addr_byte;
    i2c_data.data = data;
    i2c_data.data_byte_num = os08a10_data_byte;

    ret = ioctl(g_fd[IspDev], GPIO_I2C_WRITE, &i2c_data);

    if (ret)
    {
        printf("GPIO-I2C write faild!\n");
        return ret;
    }
#else
    int idx = 0;
    int ret;
    char buf[8];

    if (os08a10_addr_byte == 2)
    {
        buf[idx] = (addr >> 8) & 0xff;
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    }
    else
    {
        //buf[idx] = addr & 0xff;
        //idx++;
    }

    if (os08a10_data_byte == 2)
    {
        //buf[idx] = (data >> 8) & 0xff;
        //idx++;
        //buf[idx] = data & 0xff;
        //idx++;
    }
    else
    {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[IspDev], buf, os08a10_addr_byte + os08a10_data_byte);
    if(ret < 0)
    {
        printf("I2C_WRITE error!\n");
        return -1;
    }

#endif
    return 0;
}
#endif

static void delay_ms(int ms) {
    hi_usleep(ms*1000);
}

void os08a10_prog(ISP_DEV IspDev,int* rom)
{
    int i = 0;
    while (1) {
        int lookup = rom[i++];
        int addr = (lookup >> 16) & 0xFFFF;
        int data = lookup & 0xFFFF;
        if (addr == 0xFFFE) {
            delay_ms(data);
        } else if (addr == 0xFFFF) {
            return;
        } else {
            os08a10_write_register(IspDev,addr, data);
        }
    }
}

void os08a10_standby(ISP_DEV IspDev)
{
    // TODO:
    return;
}

void os08a10_restart(ISP_DEV IspDev)
{
    // TODO:
    return;
}

//void os08a10_wdr_1080p30_2to1_init(ISP_DEV IspDev);
void os08a10_wdr_4k2k30_2to1_init(ISP_DEV IspDev);
void os08a10_wdr_4k2k25_2to1_init(ISP_DEV IspDev);
//void os08a10_wdr_1080p120_2to1_init(ISP_DEV IspDev);
//void os08a10_wdr_720p60_2to1_init(ISP_DEV IspDev);
//void os08a10_wdr_1080p30_3to1_init(ISP_DEV IspDev);
//void os08a10_wdr_1080p120_3to1_init(ISP_DEV IspDev);
//void os08a10_wdr_720p60_3to1_init(ISP_DEV IspDev);
void os08a10_linear_4k2k30_init(ISP_DEV IspDev);
void os08a10_linear_4k2k25_init(ISP_DEV IspDev);
//void os08a10_linear_1080p60_init(ISP_DEV IspDev);
//void os08a10_linear_720p60_init(ISP_DEV IspDev);


void os08a10_init(ISP_DEV IspDev)
{
    HI_U32           i;
    WDR_MODE_E       enWDRMode;
    HI_BOOL          bInit;
    HI_U8 SensorImageMode = g_apstSnsState[IspDev]->u8ImgMode;

    bInit       = g_astos08a10[IspDev].bInit;
    enWDRMode   = g_astos08a10[IspDev].enWDRMode;

    os08a10_i2c_init(IspDev);

    /* When sensor first init, config all registers */
    if (HI_FALSE == bInit)
    {
        if (WDR_MODE_2To1_LINE == enWDRMode)
        {
            //os08a10_wdr_4k2k25_2to1_init(IspDev);
            os08a10_wdr_4k2k30_2to1_init(IspDev);
        }
        else
        {
            if (os08a10_SENSOR_4k2k_30FPS_LINEAR_MODE == SensorImageMode)
            {
                os08a10_linear_4k2k30_init(IspDev);
            }
            else
            {}

        }
    }
    /* When sensor switch mode(linear<->WDR or resolution), config different registers(if possible) */
    else
    {
        if(WDR_MODE_2To1_LINE == enWDRMode)
        {
            //os08a10_wdr_4k2k25_2to1_init(IspDev);
            os08a10_wdr_4k2k30_2to1_init(IspDev);
        }
        else
        {
            if (os08a10_SENSOR_4k2k_30FPS_LINEAR_MODE == SensorImageMode)
            {
                os08a10_linear_4k2k30_init(IspDev);
            }
            else
            {}
        }
    }

    for (i=0; i<g_astos08a10[IspDev].astRegsInfo[0].u32RegNum; i++)
    {
        os08a10_write_register(IspDev, g_astos08a10[IspDev].astRegsInfo[0].astI2cData[i].u32RegAddr, g_astos08a10[IspDev].astRegsInfo[0].astI2cData[i].u32Data);
    }

    g_astos08a10[IspDev].bInit = HI_TRUE;
    //printf("SensorImageMode =  %d\n", SensorImageMode);
    return ;
}

void os08a10_exit(ISP_DEV IspDev)
{
    os08a10_i2c_exit(IspDev);

    return;
}


/* 4k2k30 10bit */
void os08a10_linear_4k2k30_init(ISP_DEV IspDev)
{
    os08a10_write_register (IspDev,0x0103, 0x01);
    os08a10_write_register (IspDev,0x0303, 0x01);
    os08a10_write_register (IspDev,0x0305, 0x5c);
    os08a10_write_register (IspDev,0x0306, 0x00);
    os08a10_write_register (IspDev,0x0307, 0x01);
    os08a10_write_register (IspDev,0x0308, 0x03);
    os08a10_write_register (IspDev,0x0309, 0x04);
    os08a10_write_register (IspDev,0x032a, 0x00);
    os08a10_write_register (IspDev,0x300f, 0x11);
    os08a10_write_register (IspDev,0x3010, 0x01);
    os08a10_write_register (IspDev,0x3011, 0x04);
    os08a10_write_register (IspDev,0x3012, 0x41);
    os08a10_write_register (IspDev,0x3016, 0xf0);
    os08a10_write_register (IspDev,0x301e, 0x98);
    os08a10_write_register (IspDev,0x3031, 0xa9);
    os08a10_write_register (IspDev,0x3603, 0x2c);
    os08a10_write_register (IspDev,0x3103, 0x92);
    os08a10_write_register (IspDev,0x3104, 0x01);
    os08a10_write_register (IspDev,0x3106, 0x10);
    os08a10_write_register (IspDev,0x3400, 0x04);
    os08a10_write_register (IspDev,0x3025, 0x03);
    os08a10_write_register (IspDev,0x3425, 0x51);
    os08a10_write_register (IspDev,0x3428, 0x01);
    os08a10_write_register (IspDev,0x3406, 0x08);
    os08a10_write_register (IspDev,0x3408, 0x03);
    os08a10_write_register (IspDev,0x340c, 0xff);
    os08a10_write_register (IspDev,0x340d, 0xff);
    os08a10_write_register (IspDev,0x031e, 0x09);
    os08a10_write_register (IspDev,0x3501, 0x09);
    os08a10_write_register (IspDev,0x3502, 0x6e);
    os08a10_write_register (IspDev,0x3505, 0x83);
    os08a10_write_register (IspDev,0x3508, 0x01);
    os08a10_write_register (IspDev,0x3509, 0x00);
    os08a10_write_register (IspDev,0x350a, 0x04);
    os08a10_write_register (IspDev,0x350b, 0x00);
    os08a10_write_register (IspDev,0x350c, 0x00);
    os08a10_write_register (IspDev,0x350d, 0x80);
    os08a10_write_register (IspDev,0x350e, 0x04);
    os08a10_write_register (IspDev,0x350f, 0x00);
    os08a10_write_register (IspDev,0x3600, 0x00);
    os08a10_write_register (IspDev,0x3605, 0x50);
    os08a10_write_register (IspDev,0x3609, 0xb5);
    os08a10_write_register (IspDev,0x3610, 0x39);
    os08a10_write_register (IspDev,0x360c, 0x01);
    os08a10_write_register (IspDev,0x360e, 0x86);
    os08a10_write_register (IspDev,0x3628, 0xa4);
    os08a10_write_register (IspDev,0x362d, 0x10);
    os08a10_write_register (IspDev,0x3660, 0x43);
    os08a10_write_register (IspDev,0x3661, 0x06);
    os08a10_write_register (IspDev,0x3662, 0x00);
    os08a10_write_register (IspDev,0x3663, 0x28);
    os08a10_write_register (IspDev,0x3664, 0x0d);
    os08a10_write_register (IspDev,0x366a, 0x38);
    os08a10_write_register (IspDev,0x366b, 0xa0);
    os08a10_write_register (IspDev,0x366d, 0x00);
    os08a10_write_register (IspDev,0x366e, 0x00);
    os08a10_write_register (IspDev,0x3680, 0x00);
    os08a10_write_register (IspDev,0x3705, 0x00);
    os08a10_write_register (IspDev,0x3706, 0x35);
    os08a10_write_register (IspDev,0x370a, 0x00);
    os08a10_write_register (IspDev,0x370b, 0x98);
    os08a10_write_register (IspDev,0x3709, 0x49);
    os08a10_write_register (IspDev,0x3714, 0x21);
    os08a10_write_register (IspDev,0x371c, 0x00);
    os08a10_write_register (IspDev,0x371d, 0x08);
    os08a10_write_register (IspDev,0x375e, 0x0b);
    os08a10_write_register (IspDev,0x3762, 0x11);
    os08a10_write_register (IspDev,0x3776, 0x10);
    os08a10_write_register (IspDev,0x3781, 0x02);
    os08a10_write_register (IspDev,0x3782, 0x04);
    os08a10_write_register (IspDev,0x3783, 0x02);
    os08a10_write_register (IspDev,0x3784, 0x08);
    os08a10_write_register (IspDev,0x3785, 0x08);
    os08a10_write_register (IspDev,0x3788, 0x01);
    os08a10_write_register (IspDev,0x3789, 0x01);
    os08a10_write_register (IspDev,0x3797, 0x04);
    os08a10_write_register (IspDev,0x3800, 0x00);
    os08a10_write_register (IspDev,0x3801, 0x00);
    os08a10_write_register (IspDev,0x3802, 0x00);
    os08a10_write_register (IspDev,0x3803, 0x0c);
    os08a10_write_register (IspDev,0x3804, 0x0e);
    os08a10_write_register (IspDev,0x3805, 0xff);
    os08a10_write_register (IspDev,0x3806, 0x08);
    os08a10_write_register (IspDev,0x3807, 0x6f);
    os08a10_write_register (IspDev,0x3808, 0x0f);
    os08a10_write_register (IspDev,0x3809, 0x00);
    os08a10_write_register (IspDev,0x380a, 0x08);
    os08a10_write_register (IspDev,0x380b, 0x70);
    os08a10_write_register (IspDev,0x380c, 0x08);
    os08a10_write_register (IspDev,0x380d, 0x18);
    os08a10_write_register (IspDev,0x380e, 0x09);
    os08a10_write_register (IspDev,0x380f, 0x0d);
    os08a10_write_register (IspDev,0x3813, 0x10);
    os08a10_write_register (IspDev,0x3814, 0x01);
    os08a10_write_register (IspDev,0x3815, 0x01);
    os08a10_write_register (IspDev,0x3816, 0x01);
    os08a10_write_register (IspDev,0x3817, 0x01);
    os08a10_write_register (IspDev,0x381c, 0x00);
    //os08a10_write_register (IspDev,0x3820, 0x00);
    //os08a10_write_register (IspDev,0x3821, 0x00);
    //mirror
    os08a10_write_register (IspDev,0x3820, 0x00);
    os08a10_write_register (IspDev,0x3821, 0x04);
    os08a10_write_register (IspDev,0x3823, 0x08);
    os08a10_write_register (IspDev,0x3826, 0x00);
    os08a10_write_register (IspDev,0x3827, 0x08);
    os08a10_write_register (IspDev,0x382d, 0x08);
    os08a10_write_register (IspDev,0x3832, 0x02);
    os08a10_write_register (IspDev,0x383c, 0x48);
    os08a10_write_register (IspDev,0x383d, 0xff);
    os08a10_write_register (IspDev,0x3d85, 0x0b);
    os08a10_write_register (IspDev,0x3d84, 0x40);
    os08a10_write_register (IspDev,0x3d8c, 0x63);
    os08a10_write_register (IspDev,0x3d8d, 0xd7);
    os08a10_write_register (IspDev,0x4000, 0xf8);
    os08a10_write_register (IspDev,0x4001, 0x2f);
    os08a10_write_register (IspDev,0x400a, 0x01);
    os08a10_write_register (IspDev,0x400f, 0xa1);
    os08a10_write_register (IspDev,0x4010, 0x12);
    os08a10_write_register (IspDev,0x4018, 0x04);
    os08a10_write_register (IspDev,0x4008, 0x02);
    os08a10_write_register (IspDev,0x4009, 0x0d);
    os08a10_write_register (IspDev,0x401a, 0x58);
    os08a10_write_register (IspDev,0x4050, 0x00);
    os08a10_write_register (IspDev,0x4051, 0x01);
    os08a10_write_register (IspDev,0x4028, 0x0f);
    os08a10_write_register (IspDev,0x4052, 0x00);
    os08a10_write_register (IspDev,0x4053, 0x80);
    os08a10_write_register (IspDev,0x4054, 0x00);
    os08a10_write_register (IspDev,0x4055, 0x80);
    os08a10_write_register (IspDev,0x4056, 0x00);
    os08a10_write_register (IspDev,0x4057, 0x80);
    os08a10_write_register (IspDev,0x4058, 0x00);
    os08a10_write_register (IspDev,0x4059, 0x80);
    os08a10_write_register (IspDev,0x430b, 0xff);
    os08a10_write_register (IspDev,0x430c, 0xff);
    os08a10_write_register (IspDev,0x430d, 0x00);
    os08a10_write_register (IspDev,0x430e, 0x00);
    os08a10_write_register (IspDev,0x4501, 0x18);
    os08a10_write_register (IspDev,0x4502, 0x00);
    os08a10_write_register (IspDev,0x4643, 0x00);
    os08a10_write_register (IspDev,0x4640, 0x01);
    os08a10_write_register (IspDev,0x4641, 0x04);
    os08a10_write_register (IspDev,0x4800, 0x04);
    os08a10_write_register (IspDev,0x4809, 0x2b);
    os08a10_write_register (IspDev,0x4813, 0x90);
    os08a10_write_register (IspDev,0x4817, 0x04);
    os08a10_write_register (IspDev,0x4833, 0x18);
    os08a10_write_register (IspDev,0x4837, 0x15);
    os08a10_write_register (IspDev,0x483b, 0x00);
    os08a10_write_register (IspDev,0x484b, 0x03);
    os08a10_write_register (IspDev,0x4850, 0x7c);
    os08a10_write_register (IspDev,0x4852, 0x06);
    os08a10_write_register (IspDev,0x4856, 0x58);
    os08a10_write_register (IspDev,0x4857, 0xaa);
    os08a10_write_register (IspDev,0x4862, 0x0a);
    os08a10_write_register (IspDev,0x4869, 0x18);
    os08a10_write_register (IspDev,0x486a, 0xaa);
    os08a10_write_register (IspDev,0x486e, 0x03);
    os08a10_write_register (IspDev,0x486f, 0x55);
    os08a10_write_register (IspDev,0x4875, 0xf0);
    os08a10_write_register (IspDev,0x5000, 0x89);
    os08a10_write_register (IspDev,0x5001, 0x42);
    os08a10_write_register (IspDev,0x5004, 0x40);
    os08a10_write_register (IspDev,0x5005, 0x00);
    os08a10_write_register (IspDev,0x5180, 0x00);
    os08a10_write_register (IspDev,0x5181, 0x10);
    os08a10_write_register (IspDev,0x580b, 0x03);
    os08a10_write_register (IspDev,0x4700, 0x2b);
    os08a10_write_register (IspDev,0x4e00, 0x2b);
    delay_ms(5);  //delay 5ms
    os08a10_write_register (IspDev,0x0100, 0x01);

    printf("===os08a10 4k2k 30fps 10bit LINE Init OK!===\n");
    printf("===MIPI data rate = 736Mbps/lane =====\n");
    return;
}

/* 4k2k25 10bit */
void os08a10_linear_4k2k25_init(ISP_DEV IspDev)
{
    os08a10_write_register (IspDev,0x0103, 0x01);
    os08a10_write_register (IspDev,0x0303, 0x01);
    os08a10_write_register (IspDev,0x0305, 0x5c);
    os08a10_write_register (IspDev,0x0306, 0x00);
    os08a10_write_register (IspDev,0x0307, 0x01);
    os08a10_write_register (IspDev,0x0308, 0x03);
    os08a10_write_register (IspDev,0x0309, 0x04);
    os08a10_write_register (IspDev,0x032a, 0x00);
    os08a10_write_register (IspDev,0x300f, 0x11);
    os08a10_write_register (IspDev,0x3010, 0x01);
    os08a10_write_register (IspDev,0x3011, 0x04);
    os08a10_write_register (IspDev,0x3012, 0x41);
    os08a10_write_register (IspDev,0x3016, 0xf0);
    os08a10_write_register (IspDev,0x301e, 0x98);
    os08a10_write_register (IspDev,0x3031, 0xa9);
    os08a10_write_register (IspDev,0x3603, 0x2c);
    os08a10_write_register (IspDev,0x3103, 0x92);
    os08a10_write_register (IspDev,0x3104, 0x01);
    os08a10_write_register (IspDev,0x3106, 0x10);
    os08a10_write_register (IspDev,0x3400, 0x04);
    os08a10_write_register (IspDev,0x3025, 0x03);
    os08a10_write_register (IspDev,0x3425, 0x51);
    os08a10_write_register (IspDev,0x3428, 0x01);
    os08a10_write_register (IspDev,0x3406, 0x08);
    os08a10_write_register (IspDev,0x3408, 0x03);
    os08a10_write_register (IspDev,0x340c, 0xff);
    os08a10_write_register (IspDev,0x340d, 0xff);
    os08a10_write_register (IspDev,0x031e, 0x09);
    os08a10_write_register (IspDev,0x3501, 0x09);
    os08a10_write_register (IspDev,0x3502, 0x6e);
    os08a10_write_register (IspDev,0x3505, 0x83);
    os08a10_write_register (IspDev,0x3508, 0x01);
    os08a10_write_register (IspDev,0x3509, 0x00);
    os08a10_write_register (IspDev,0x350a, 0x04);
    os08a10_write_register (IspDev,0x350b, 0x00);
    os08a10_write_register (IspDev,0x350c, 0x00);
    os08a10_write_register (IspDev,0x350d, 0x80);
    os08a10_write_register (IspDev,0x350e, 0x04);
    os08a10_write_register (IspDev,0x350f, 0x00);
    os08a10_write_register (IspDev,0x3600, 0x00);
    os08a10_write_register (IspDev,0x3605, 0x50);
    os08a10_write_register (IspDev,0x3609, 0xb5);
    os08a10_write_register (IspDev,0x3610, 0x39);
    os08a10_write_register (IspDev,0x360c, 0x01);
    os08a10_write_register (IspDev,0x360e, 0x86);
    os08a10_write_register (IspDev,0x3628, 0xa4);
    os08a10_write_register (IspDev,0x362d, 0x10);
    os08a10_write_register (IspDev,0x3660, 0x43);
    os08a10_write_register (IspDev,0x3661, 0x06);
    os08a10_write_register (IspDev,0x3662, 0x00);
    os08a10_write_register (IspDev,0x3663, 0x28);
    os08a10_write_register (IspDev,0x3664, 0x0d);
    os08a10_write_register (IspDev,0x366a, 0x38);
    os08a10_write_register (IspDev,0x366b, 0xa0);
    os08a10_write_register (IspDev,0x366d, 0x00);
    os08a10_write_register (IspDev,0x366e, 0x00);
    os08a10_write_register (IspDev,0x3680, 0x00);
    os08a10_write_register (IspDev,0x3705, 0x00);
    os08a10_write_register (IspDev,0x3706, 0x35);
    os08a10_write_register (IspDev,0x370a, 0x00);
    os08a10_write_register (IspDev,0x370b, 0x98);
    os08a10_write_register (IspDev,0x3709, 0x49);
    os08a10_write_register (IspDev,0x3714, 0x21);
    os08a10_write_register (IspDev,0x371c, 0x00);
    os08a10_write_register (IspDev,0x371d, 0x08);
    os08a10_write_register (IspDev,0x375e, 0x0b);
    os08a10_write_register (IspDev,0x3762, 0x11);
    os08a10_write_register (IspDev,0x3776, 0x10);
    os08a10_write_register (IspDev,0x3781, 0x02);
    os08a10_write_register (IspDev,0x3782, 0x04);
    os08a10_write_register (IspDev,0x3783, 0x02);
    os08a10_write_register (IspDev,0x3784, 0x08);
    os08a10_write_register (IspDev,0x3785, 0x08);
    os08a10_write_register (IspDev,0x3788, 0x01);
    os08a10_write_register (IspDev,0x3789, 0x01);
    os08a10_write_register (IspDev,0x3797, 0x04);
    os08a10_write_register (IspDev,0x3800, 0x00);
    os08a10_write_register (IspDev,0x3801, 0x00);
    os08a10_write_register (IspDev,0x3802, 0x00);
    os08a10_write_register (IspDev,0x3803, 0x0c);
    os08a10_write_register (IspDev,0x3804, 0x0e);
    os08a10_write_register (IspDev,0x3805, 0xff);
    os08a10_write_register (IspDev,0x3806, 0x08);
    os08a10_write_register (IspDev,0x3807, 0x6f);
    os08a10_write_register (IspDev,0x3808, 0x0f);
    os08a10_write_register (IspDev,0x3809, 0x00);
    os08a10_write_register (IspDev,0x380a, 0x08);
    os08a10_write_register (IspDev,0x380b, 0x70);
    os08a10_write_register (IspDev,0x380c, 0x09);
    os08a10_write_register (IspDev,0x380d, 0xb6);
    os08a10_write_register (IspDev,0x380e, 0x09);
    os08a10_write_register (IspDev,0x380f, 0x0d);
    os08a10_write_register (IspDev,0x3813, 0x10);
    os08a10_write_register (IspDev,0x3814, 0x01);
    os08a10_write_register (IspDev,0x3815, 0x01);
    os08a10_write_register (IspDev,0x3816, 0x01);
    os08a10_write_register (IspDev,0x3817, 0x01);
    os08a10_write_register (IspDev,0x381c, 0x00);
    //os08a10_write_register (IspDev,0x3820, 0x00);
    //os08a10_write_register (IspDev,0x3821, 0x00);
    //mirror
    os08a10_write_register (IspDev,0x3820, 0x00);
    os08a10_write_register (IspDev,0x3821, 0x04);
    os08a10_write_register (IspDev,0x3823, 0x08);
    os08a10_write_register (IspDev,0x3826, 0x00);
    os08a10_write_register (IspDev,0x3827, 0x08);
    os08a10_write_register (IspDev,0x382d, 0x08);
    os08a10_write_register (IspDev,0x3832, 0x02);
    os08a10_write_register (IspDev,0x383c, 0x48);
    os08a10_write_register (IspDev,0x383d, 0xff);
    os08a10_write_register (IspDev,0x3d85, 0x0b);
    os08a10_write_register (IspDev,0x3d84, 0x40);
    os08a10_write_register (IspDev,0x3d8c, 0x63);
    os08a10_write_register (IspDev,0x3d8d, 0xd7);
    os08a10_write_register (IspDev,0x4000, 0xf8);
    os08a10_write_register (IspDev,0x4001, 0x2f);
    os08a10_write_register (IspDev,0x400a, 0x01);
    os08a10_write_register (IspDev,0x400f, 0xa1);
    os08a10_write_register (IspDev,0x4010, 0x12);
    os08a10_write_register (IspDev,0x4018, 0x04);
    os08a10_write_register (IspDev,0x4008, 0x02);
    os08a10_write_register (IspDev,0x4009, 0x0d);
    os08a10_write_register (IspDev,0x401a, 0x58);
    os08a10_write_register (IspDev,0x4050, 0x00);
    os08a10_write_register (IspDev,0x4051, 0x01);
    os08a10_write_register (IspDev,0x4028, 0x0f);
    os08a10_write_register (IspDev,0x4052, 0x00);
    os08a10_write_register (IspDev,0x4053, 0x80);
    os08a10_write_register (IspDev,0x4054, 0x00);
    os08a10_write_register (IspDev,0x4055, 0x80);
    os08a10_write_register (IspDev,0x4056, 0x00);
    os08a10_write_register (IspDev,0x4057, 0x80);
    os08a10_write_register (IspDev,0x4058, 0x00);
    os08a10_write_register (IspDev,0x4059, 0x80);
    os08a10_write_register (IspDev,0x430b, 0xff);
    os08a10_write_register (IspDev,0x430c, 0xff);
    os08a10_write_register (IspDev,0x430d, 0x00);
    os08a10_write_register (IspDev,0x430e, 0x00);
    os08a10_write_register (IspDev,0x4501, 0x18);
    os08a10_write_register (IspDev,0x4502, 0x00);
    os08a10_write_register (IspDev,0x4643, 0x00);
    os08a10_write_register (IspDev,0x4640, 0x01);
    os08a10_write_register (IspDev,0x4641, 0x04);
    os08a10_write_register (IspDev,0x4800, 0x04);
    os08a10_write_register (IspDev,0x4809, 0x2b);
    os08a10_write_register (IspDev,0x4813, 0x90);
    os08a10_write_register (IspDev,0x4817, 0x04);
    os08a10_write_register (IspDev,0x4833, 0x18);
    os08a10_write_register (IspDev,0x4837, 0x15);
    os08a10_write_register (IspDev,0x483b, 0x00);
    os08a10_write_register (IspDev,0x484b, 0x03);
    os08a10_write_register (IspDev,0x4850, 0x7c);
    os08a10_write_register (IspDev,0x4852, 0x06);
    os08a10_write_register (IspDev,0x4856, 0x58);
    os08a10_write_register (IspDev,0x4857, 0xaa);
    os08a10_write_register (IspDev,0x4862, 0x0a);
    os08a10_write_register (IspDev,0x4869, 0x18);
    os08a10_write_register (IspDev,0x486a, 0xaa);
    os08a10_write_register (IspDev,0x486e, 0x03);
    os08a10_write_register (IspDev,0x486f, 0x55);
    os08a10_write_register (IspDev,0x4875, 0xf0);
    os08a10_write_register (IspDev,0x5000, 0x89);
    os08a10_write_register (IspDev,0x5001, 0x42);
    os08a10_write_register (IspDev,0x5004, 0x40);
    os08a10_write_register (IspDev,0x5005, 0x00);
    os08a10_write_register (IspDev,0x5180, 0x00);
    os08a10_write_register (IspDev,0x5181, 0x10);
    os08a10_write_register (IspDev,0x580b, 0x03);
    os08a10_write_register (IspDev,0x4700, 0x2b);
    os08a10_write_register (IspDev,0x4e00, 0x2b);
    delay_ms(5);  //delay 5ms
    os08a10_write_register (IspDev,0x0100, 0x01);

    printf("===os08a10 4k2k 25fps 10bit LINE Init OK!===\n");
    printf("===MIPI data rate = 736Mbps/lane =====\n");
    return;
}

//@@ Res 3840x 2160_30fps_MIPI 1440Mbps STG_HDR_VC_Mode
//;HTS = 1036, VTS = 2316
void os08a10_wdr_4k2k30_2to1_init(ISP_DEV IspDev)
{
#if 1 /* stone */
    os08a10_write_register (IspDev, 0x0103, 0x01);
    os08a10_write_register (IspDev, 0x0303, 0x01);
    os08a10_write_register (IspDev, 0x0305, 0x5c);
    os08a10_write_register (IspDev, 0x0306, 0x00);
    os08a10_write_register (IspDev, 0x0308, 0x03);
    os08a10_write_register (IspDev, 0x0309, 0x04);
    os08a10_write_register (IspDev, 0x032a, 0x00);
    os08a10_write_register (IspDev, 0x300f, 0x11);
    os08a10_write_register (IspDev, 0x3010, 0x01);
    os08a10_write_register (IspDev, 0x3012, 0x41);
    os08a10_write_register (IspDev, 0x3016, 0xf0);
    os08a10_write_register (IspDev, 0x301e, 0x98);
    os08a10_write_register (IspDev, 0x3031, 0xa9);
    os08a10_write_register (IspDev, 0x3103, 0x92);
    os08a10_write_register (IspDev, 0x3104, 0x01);
    os08a10_write_register (IspDev, 0x3106, 0x10);
    os08a10_write_register (IspDev, 0x340c, 0xff);
    os08a10_write_register (IspDev, 0x340d, 0xff);
    os08a10_write_register (IspDev, 0x031e, 0x09);
    os08a10_write_register (IspDev, 0x3501, 0x08);
    os08a10_write_register (IspDev, 0x3502, 0xe5);
    os08a10_write_register (IspDev, 0x3505, 0x83);
    os08a10_write_register (IspDev, 0x3508, 0x01);
    os08a10_write_register (IspDev, 0x3509, 0x00);
    os08a10_write_register (IspDev, 0x350a, 0x04);
    os08a10_write_register (IspDev, 0x350b, 0x00);
    os08a10_write_register (IspDev, 0x350c, 0x00);
    os08a10_write_register (IspDev, 0x350d, 0x80);
    os08a10_write_register (IspDev, 0x350e, 0x04);
    os08a10_write_register (IspDev, 0x350f, 0x00);
    os08a10_write_register (IspDev, 0x3600, 0x00);
    os08a10_write_register (IspDev, 0x3605, 0x50);
    os08a10_write_register (IspDev, 0x3609, 0xb5);
    os08a10_write_register (IspDev, 0x3610, 0x69);
    os08a10_write_register (IspDev, 0x360c, 0x01);
    os08a10_write_register (IspDev, 0x360e, 0x86);
    os08a10_write_register (IspDev, 0x3628, 0xa4);
    os08a10_write_register (IspDev, 0x362d, 0x10);
    os08a10_write_register (IspDev, 0x3660, 0x42);
    os08a10_write_register (IspDev, 0x3661, 0x07);
    os08a10_write_register (IspDev, 0x3662, 0x00);
    os08a10_write_register (IspDev, 0x3663, 0x28);
    os08a10_write_register (IspDev, 0x3664, 0x0d);
    os08a10_write_register (IspDev, 0x366a, 0x38);
    os08a10_write_register (IspDev, 0x366b, 0xa0);
    os08a10_write_register (IspDev, 0x366d, 0x00);
    os08a10_write_register (IspDev, 0x366e, 0x00);
    os08a10_write_register (IspDev, 0x3680, 0x00);
    os08a10_write_register (IspDev, 0x3701, 0x02);
    os08a10_write_register (IspDev, 0x373b, 0x02);
    os08a10_write_register (IspDev, 0x373c, 0x02);
    os08a10_write_register (IspDev, 0x3736, 0x02);
    os08a10_write_register (IspDev, 0x3737, 0x02);
    os08a10_write_register (IspDev, 0x3705, 0x00);
    os08a10_write_register (IspDev, 0x3706, 0x35);
    os08a10_write_register (IspDev, 0x370a, 0x00);
    os08a10_write_register (IspDev, 0x370b, 0x98);
    os08a10_write_register (IspDev, 0x3709, 0x49);
    os08a10_write_register (IspDev, 0x3714, 0x21);
    os08a10_write_register (IspDev, 0x371c, 0x00);
    os08a10_write_register (IspDev, 0x371d, 0x08);
    os08a10_write_register (IspDev, 0x375e, 0x0b);
    os08a10_write_register (IspDev, 0x3776, 0x10);
    os08a10_write_register (IspDev, 0x3781, 0x02);
    os08a10_write_register (IspDev, 0x3782, 0x04);
    os08a10_write_register (IspDev, 0x3783, 0x02);
    os08a10_write_register (IspDev, 0x3784, 0x08);
    os08a10_write_register (IspDev, 0x3785, 0x08);
    os08a10_write_register (IspDev, 0x3788, 0x01);
    os08a10_write_register (IspDev, 0x3789, 0x01);
    os08a10_write_register (IspDev, 0x3797, 0x04);
    os08a10_write_register (IspDev, 0x3800, 0x00);
    os08a10_write_register (IspDev, 0x3801, 0x00);
    os08a10_write_register (IspDev, 0x3802, 0x00);
    os08a10_write_register (IspDev, 0x3803, 0x0c);
    os08a10_write_register (IspDev, 0x3804, 0x0e);
    os08a10_write_register (IspDev, 0x3805, 0xff);
    os08a10_write_register (IspDev, 0x3806, 0x08);
    os08a10_write_register (IspDev, 0x3807, 0x6f);
    os08a10_write_register (IspDev, 0x3808, 0x0f);
    os08a10_write_register (IspDev, 0x3809, 0x00);
    os08a10_write_register (IspDev, 0x380a, 0x08);
    os08a10_write_register (IspDev, 0x380b, 0x70);
    os08a10_write_register (IspDev, 0x380c, 0x04);
    os08a10_write_register (IspDev, 0x380d, 0x0c);
    os08a10_write_register (IspDev, 0x380e, 0x09);
    os08a10_write_register (IspDev, 0x380f, 0x0d);
    os08a10_write_register (IspDev, 0x3813, 0x10);
    os08a10_write_register (IspDev, 0x3814, 0x01);
    os08a10_write_register (IspDev, 0x3815, 0x01);
    os08a10_write_register (IspDev, 0x3816, 0x01);
    os08a10_write_register (IspDev, 0x3817, 0x01);
    os08a10_write_register (IspDev, 0x381c, 0x08);
    os08a10_write_register (IspDev, 0x3820, 0x00);
    os08a10_write_register (IspDev, 0x3821, 0x24); //mirror
    os08a10_write_register (IspDev, 0x3823, 0x08);
    os08a10_write_register (IspDev, 0x3826, 0x00);
    os08a10_write_register (IspDev, 0x3827, 0x08);
    os08a10_write_register (IspDev, 0x382d, 0x08);
    os08a10_write_register (IspDev, 0x3832, 0x02);
    os08a10_write_register (IspDev, 0x383c, 0x48);
    os08a10_write_register (IspDev, 0x383d, 0xff);
    os08a10_write_register (IspDev, 0x3d85, 0x0b);
    os08a10_write_register (IspDev, 0x3d84, 0x40);
    os08a10_write_register (IspDev, 0x3d8c, 0x63);
    os08a10_write_register (IspDev, 0x3d8d, 0xd7);
    os08a10_write_register (IspDev, 0x4000, 0xf8);
    os08a10_write_register (IspDev, 0x4001, 0x2f);
    os08a10_write_register (IspDev, 0x400a, 0x01);
    os08a10_write_register (IspDev, 0x400f, 0xa1);
    os08a10_write_register (IspDev, 0x4010, 0x12);
    os08a10_write_register (IspDev, 0x4018, 0x04);
    os08a10_write_register (IspDev, 0x4008, 0x02);
    os08a10_write_register (IspDev, 0x4009, 0x0d);
    os08a10_write_register (IspDev, 0x401a, 0x58);
    os08a10_write_register (IspDev, 0x4050, 0x00);
    os08a10_write_register (IspDev, 0x4051, 0x01);
    os08a10_write_register (IspDev, 0x4028, 0x0f);
    os08a10_write_register (IspDev, 0x4052, 0x00);
    os08a10_write_register (IspDev, 0x4053, 0x80);
    os08a10_write_register (IspDev, 0x4054, 0x00);
    os08a10_write_register (IspDev, 0x4055, 0x80);
    os08a10_write_register (IspDev, 0x4056, 0x00);
    os08a10_write_register (IspDev, 0x4057, 0x80);
    os08a10_write_register (IspDev, 0x4058, 0x00);
    os08a10_write_register (IspDev, 0x4059, 0x80);
    os08a10_write_register (IspDev, 0x430b, 0xff);
    os08a10_write_register (IspDev, 0x430c, 0xff);
    os08a10_write_register (IspDev, 0x430d, 0x00);
    os08a10_write_register (IspDev, 0x430e, 0x00);
    os08a10_write_register (IspDev, 0x4501, 0x18);
    os08a10_write_register (IspDev, 0x4502, 0x00);
    os08a10_write_register (IspDev, 0x4643, 0x00);
    os08a10_write_register (IspDev, 0x4640, 0x01);
    os08a10_write_register (IspDev, 0x4641, 0x04);
    os08a10_write_register (IspDev, 0x4800, 0x04);
    os08a10_write_register (IspDev, 0x4809, 0x2b);
    os08a10_write_register (IspDev, 0x4813, 0x98);
    os08a10_write_register (IspDev, 0x4817, 0x04);
    os08a10_write_register (IspDev, 0x4833, 0x18);
    os08a10_write_register (IspDev, 0x4837, 0x0a);
    os08a10_write_register (IspDev, 0x483b, 0x00);
    os08a10_write_register (IspDev, 0x484b, 0x03);
    os08a10_write_register (IspDev, 0x4850, 0x7c);
    os08a10_write_register (IspDev, 0x4852, 0x06);
    os08a10_write_register (IspDev, 0x4856, 0x58);
    os08a10_write_register (IspDev, 0x4857, 0xaa);
    os08a10_write_register (IspDev, 0x4862, 0x0a);
    os08a10_write_register (IspDev, 0x4869, 0x18);
    os08a10_write_register (IspDev, 0x486a, 0xaa);
    os08a10_write_register (IspDev, 0x486e, 0x07);
    os08a10_write_register (IspDev, 0x486f, 0x55);
    os08a10_write_register (IspDev, 0x4875, 0xf0);
    os08a10_write_register (IspDev, 0x5000, 0x89);
    os08a10_write_register (IspDev, 0x5001, 0x42);
    os08a10_write_register (IspDev, 0x5004, 0x40);
    os08a10_write_register (IspDev, 0x5005, 0x00);
    os08a10_write_register (IspDev, 0x5180, 0x00);
    os08a10_write_register (IspDev, 0x5181, 0x10);
    os08a10_write_register (IspDev, 0x580b, 0x03);
    os08a10_write_register (IspDev, 0x4700, 0x2b);
    os08a10_write_register (IspDev, 0x4e00, 0x2b);
    delay_ms(5);  //delay 5ms
    os08a10_write_register (IspDev, 0x0100, 0x01);
#else
    os08a10_write_register (IspDev, 0x0103, 0x01);
    os08a10_write_register (IspDev, 0x0303, 0x01);
    os08a10_write_register (IspDev, 0x0305, 0x5c);
    os08a10_write_register (IspDev, 0x0306, 0x00);
    os08a10_write_register (IspDev, 0x0307, 0x00);
    os08a10_write_register (IspDev, 0x0308, 0x03);
    os08a10_write_register (IspDev, 0x0309, 0x04);
    os08a10_write_register (IspDev, 0x032a, 0x00);
    os08a10_write_register (IspDev, 0x300f, 0x11);
    os08a10_write_register (IspDev, 0x3010, 0x01);
    os08a10_write_register (IspDev, 0x3011, 0x04);
    os08a10_write_register (IspDev, 0x3012, 0x41);
    os08a10_write_register (IspDev, 0x3016, 0xf0);
    os08a10_write_register (IspDev, 0x301e, 0x98);
    os08a10_write_register (IspDev, 0x3031, 0xa9);
    os08a10_write_register (IspDev, 0x3603, 0x2c);
    os08a10_write_register (IspDev, 0x3103, 0x92);
    os08a10_write_register (IspDev, 0x3104, 0x01);
    os08a10_write_register (IspDev, 0x3106, 0x10);
    os08a10_write_register (IspDev, 0x3400, 0x04);
    os08a10_write_register (IspDev, 0x3025, 0x03);
    os08a10_write_register (IspDev, 0x3425, 0x51);
    os08a10_write_register (IspDev, 0x3428, 0x01);
    os08a10_write_register (IspDev, 0x3406, 0x08);
    os08a10_write_register (IspDev, 0x3408, 0x03);
    os08a10_write_register (IspDev, 0x340c, 0xff);
    os08a10_write_register (IspDev, 0x340d, 0xff);
    os08a10_write_register (IspDev, 0x031e, 0x09);
    os08a10_write_register (IspDev, 0x3501, 0x08);
    os08a10_write_register (IspDev, 0x3502, 0xe5);
    os08a10_write_register (IspDev, 0x3505, 0x83);
    os08a10_write_register (IspDev, 0x3508, 0x01);
    os08a10_write_register (IspDev, 0x3509, 0x00);
    os08a10_write_register (IspDev, 0x350a, 0x04);
    os08a10_write_register (IspDev, 0x350b, 0x00);
    os08a10_write_register (IspDev, 0x350c, 0x00);
    os08a10_write_register (IspDev, 0x350d, 0x80);
    os08a10_write_register (IspDev, 0x350e, 0x04);
    os08a10_write_register (IspDev, 0x350f, 0x00);
    os08a10_write_register (IspDev, 0x3600, 0x00);
    os08a10_write_register (IspDev, 0x3605, 0x50);
    os08a10_write_register (IspDev, 0x3609, 0xb5);
    os08a10_write_register (IspDev, 0x3610, 0x69);
    os08a10_write_register (IspDev, 0x360c, 0x01);
    os08a10_write_register (IspDev, 0x360e, 0x86);
    os08a10_write_register (IspDev, 0x3628, 0xa4);
    os08a10_write_register (IspDev, 0x362d, 0x10);
    os08a10_write_register (IspDev, 0x3660, 0x42);
    os08a10_write_register (IspDev, 0x3661, 0x07);
    os08a10_write_register (IspDev, 0x3662, 0x00);
    os08a10_write_register (IspDev, 0x3663, 0x28);
    os08a10_write_register (IspDev, 0x3664, 0x0d);
    os08a10_write_register (IspDev, 0x366a, 0x38);
    os08a10_write_register (IspDev, 0x366b, 0xa0);
    os08a10_write_register (IspDev, 0x366d, 0x00);
    os08a10_write_register (IspDev, 0x366e, 0x00);
    os08a10_write_register (IspDev, 0x3680, 0x00);
    os08a10_write_register (IspDev, 0x3705, 0x00);
    os08a10_write_register (IspDev, 0x3706, 0x35);
    os08a10_write_register (IspDev, 0x370a, 0x00);
    os08a10_write_register (IspDev, 0x370b, 0x98);
    os08a10_write_register (IspDev, 0x3709, 0x49);
    os08a10_write_register (IspDev, 0x3714, 0x21);
    os08a10_write_register (IspDev, 0x371c, 0x00);
    os08a10_write_register (IspDev, 0x371d, 0x08);
    os08a10_write_register (IspDev, 0x375e, 0x0b);
    os08a10_write_register (IspDev, 0x3762, 0x11);
    os08a10_write_register (IspDev, 0x3776, 0x10);
    os08a10_write_register (IspDev, 0x3781, 0x02);
    os08a10_write_register (IspDev, 0x3782, 0x04);
    os08a10_write_register (IspDev, 0x3783, 0x02);
    os08a10_write_register (IspDev, 0x3784, 0x08);
    os08a10_write_register (IspDev, 0x3785, 0x08);
    os08a10_write_register (IspDev, 0x3788, 0x01);
    os08a10_write_register (IspDev, 0x3789, 0x01);
    os08a10_write_register (IspDev, 0x3797, 0x04);
    os08a10_write_register (IspDev, 0x3800, 0x00);
    os08a10_write_register (IspDev, 0x3801, 0x00);
    os08a10_write_register (IspDev, 0x3802, 0x00);
    os08a10_write_register (IspDev, 0x3803, 0x0c);
    os08a10_write_register (IspDev, 0x3804, 0x0e);
    os08a10_write_register (IspDev, 0x3805, 0xff);
    os08a10_write_register (IspDev, 0x3806, 0x08);
    os08a10_write_register (IspDev, 0x3807, 0x6f);
    os08a10_write_register (IspDev, 0x3808, 0x0f);
    os08a10_write_register (IspDev, 0x3809, 0x00);
    os08a10_write_register (IspDev, 0x380a, 0x08);
    os08a10_write_register (IspDev, 0x380b, 0x70);
    os08a10_write_register (IspDev, 0x380c, 0x04);
    os08a10_write_register (IspDev, 0x380d, 0x0c);
    os08a10_write_register (IspDev, 0x380e, 0x09);
    os08a10_write_register (IspDev, 0x380f, 0x0d);
    os08a10_write_register (IspDev, 0x3813, 0x10);
    os08a10_write_register (IspDev, 0x3814, 0x01);
    os08a10_write_register (IspDev, 0x3815, 0x01);
    os08a10_write_register (IspDev, 0x3816, 0x01);
    os08a10_write_register (IspDev, 0x3817, 0x01);
    os08a10_write_register (IspDev, 0x381c, 0x08);
    os08a10_write_register (IspDev, 0x3820, 0x00);
    os08a10_write_register (IspDev, 0x3821, 0x20);
    os08a10_write_register (IspDev, 0x3823, 0x08);
    os08a10_write_register (IspDev, 0x3826, 0x00);
    os08a10_write_register (IspDev, 0x3827, 0x08);
    os08a10_write_register (IspDev, 0x382d, 0x08);
    os08a10_write_register (IspDev, 0x3832, 0x02);
    os08a10_write_register (IspDev, 0x383c, 0x48);
    os08a10_write_register (IspDev, 0x383d, 0xff);
    os08a10_write_register (IspDev, 0x3d85, 0x0b);
    os08a10_write_register (IspDev, 0x3d84, 0x40);
    os08a10_write_register (IspDev, 0x3d8c, 0x63);
    os08a10_write_register (IspDev, 0x3d8d, 0xd7);
    os08a10_write_register (IspDev, 0x4000, 0xf8);
    os08a10_write_register (IspDev, 0x4001, 0x2f);
    os08a10_write_register (IspDev, 0x400a, 0x01);
    os08a10_write_register (IspDev, 0x400f, 0xa1);
    os08a10_write_register (IspDev, 0x4010, 0x12);
    os08a10_write_register (IspDev, 0x4018, 0x04);
    os08a10_write_register (IspDev, 0x4008, 0x02);
    os08a10_write_register (IspDev, 0x4009, 0x0d);
    os08a10_write_register (IspDev, 0x401a, 0x58);
    os08a10_write_register (IspDev, 0x4050, 0x00);
    os08a10_write_register (IspDev, 0x4051, 0x01);
    os08a10_write_register (IspDev, 0x4028, 0x0f);
    os08a10_write_register (IspDev, 0x4052, 0x00);
    os08a10_write_register (IspDev, 0x4053, 0x80);
    os08a10_write_register (IspDev, 0x4054, 0x00);
    os08a10_write_register (IspDev, 0x4055, 0x80);
    os08a10_write_register (IspDev, 0x4056, 0x00);
    os08a10_write_register (IspDev, 0x4057, 0x80);
    os08a10_write_register (IspDev, 0x4058, 0x00);
    os08a10_write_register (IspDev, 0x4059, 0x80);
    os08a10_write_register (IspDev, 0x430b, 0xff);
    os08a10_write_register (IspDev, 0x430c, 0xff);
    os08a10_write_register (IspDev, 0x430d, 0x00);
    os08a10_write_register (IspDev, 0x430e, 0x00);
    os08a10_write_register (IspDev, 0x4501, 0x18);
    os08a10_write_register (IspDev, 0x4502, 0x00);
    os08a10_write_register (IspDev, 0x4643, 0x00);
    os08a10_write_register (IspDev, 0x4640, 0x01);
    os08a10_write_register (IspDev, 0x4641, 0x04);
    os08a10_write_register (IspDev, 0x4800, 0x04);
    os08a10_write_register (IspDev, 0x4809, 0x2b);
    os08a10_write_register (IspDev, 0x4813, 0x98);
    os08a10_write_register (IspDev, 0x4817, 0x04);
    os08a10_write_register (IspDev, 0x4833, 0x18);
    os08a10_write_register (IspDev, 0x4837, 0x0a);
    os08a10_write_register (IspDev, 0x483b, 0x00);
    os08a10_write_register (IspDev, 0x484b, 0x03);
    os08a10_write_register (IspDev, 0x4850, 0x7c);
    os08a10_write_register (IspDev, 0x4852, 0x06);
    os08a10_write_register (IspDev, 0x4856, 0x58);
    os08a10_write_register (IspDev, 0x4857, 0xaa);
    os08a10_write_register (IspDev, 0x4862, 0x0a);
    os08a10_write_register (IspDev, 0x4869, 0x18);
    os08a10_write_register (IspDev, 0x486a, 0xaa);
    os08a10_write_register (IspDev, 0x486e, 0x07);
    os08a10_write_register (IspDev, 0x486f, 0x55);
    os08a10_write_register (IspDev, 0x4875, 0xf0);
    os08a10_write_register (IspDev, 0x5000, 0x89);
    os08a10_write_register (IspDev, 0x5001, 0x42);
    os08a10_write_register (IspDev, 0x5004, 0x40);
    os08a10_write_register (IspDev, 0x5005, 0x00);
    os08a10_write_register (IspDev, 0x5180, 0x00);
    os08a10_write_register (IspDev, 0x5181, 0x10);
    os08a10_write_register (IspDev, 0x580b, 0x03);
    os08a10_write_register (IspDev, 0x4700, 0x2b);
    os08a10_write_register (IspDev, 0x4e00, 0x2b);
    delay_ms(5);  //delay 5ms
    os08a10_write_register (IspDev, 0x0100, 0x01);
#endif
    printf("===os08a10 sensor 4k2k30fps 10bit 2to1 WDR(60fps->30fps) init success!=====\n");
    //printf("===MIPI data rate = 1472Mbps/lane =====\n");

    return;
}

void os08a10_wdr_4k2k25_2to1_init(ISP_DEV IspDev)
{
    os08a10_write_register (IspDev,0x0103, 0x01);
    os08a10_write_register (IspDev,0x0303, 0x01);
    os08a10_write_register (IspDev,0x0305, 0x4b);
    os08a10_write_register (IspDev,0x0306, 0x00);
    os08a10_write_register (IspDev,0x0307, 0x00);
    os08a10_write_register (IspDev,0x0308, 0x03);
    os08a10_write_register (IspDev,0x0309, 0x04);
    os08a10_write_register (IspDev,0x032a, 0x00);
    os08a10_write_register (IspDev,0x300f, 0x11);
    os08a10_write_register (IspDev,0x3010, 0x01);
    os08a10_write_register (IspDev,0x3011, 0x04);
    os08a10_write_register (IspDev,0x3012, 0x41);
    os08a10_write_register (IspDev,0x3016, 0xf0);
    os08a10_write_register (IspDev,0x301e, 0x98);
    os08a10_write_register (IspDev,0x3031, 0xa9);
    os08a10_write_register (IspDev,0x3603, 0x28);
    os08a10_write_register (IspDev,0x3103, 0x92);
    os08a10_write_register (IspDev,0x3104, 0x01);
    os08a10_write_register (IspDev,0x3106, 0x10);
    os08a10_write_register (IspDev,0x3400, 0x00);
    os08a10_write_register (IspDev,0x3025, 0x02);
    os08a10_write_register (IspDev,0x3425, 0x00);
    os08a10_write_register (IspDev,0x3428, 0x00);
    os08a10_write_register (IspDev,0x3406, 0x08);
    os08a10_write_register (IspDev,0x3408, 0x01);
    os08a10_write_register (IspDev,0x340c, 0xff);
    os08a10_write_register (IspDev,0x340d, 0xff);
    os08a10_write_register (IspDev,0x031e, 0x09);
    os08a10_write_register (IspDev,0x3501, 0x08);
    os08a10_write_register (IspDev,0x3502, 0xe5);
    os08a10_write_register (IspDev,0x3505, 0x83);
    os08a10_write_register (IspDev,0x3508, 0x01);
    os08a10_write_register (IspDev,0x3509, 0x00);
    os08a10_write_register (IspDev,0x350a, 0x04);
    os08a10_write_register (IspDev,0x350b, 0x00);
    os08a10_write_register (IspDev,0x350c, 0x00);
    os08a10_write_register (IspDev,0x350d, 0x80);
    os08a10_write_register (IspDev,0x350e, 0x04);
    os08a10_write_register (IspDev,0x350f, 0x00);
    os08a10_write_register (IspDev,0x3600, 0x00);
    os08a10_write_register (IspDev,0x3605, 0x50);
    os08a10_write_register (IspDev,0x3609, 0xb5);
    os08a10_write_register (IspDev,0x3610, 0x69);
    os08a10_write_register (IspDev,0x360c, 0x01);
    os08a10_write_register (IspDev,0x360e, 0x86);
    os08a10_write_register (IspDev,0x3628, 0xa4);
    os08a10_write_register (IspDev,0x362d, 0x10);
    os08a10_write_register (IspDev,0x3660, 0x42);
    os08a10_write_register (IspDev,0x3661, 0x07);
    os08a10_write_register (IspDev,0x3662, 0x00);
    os08a10_write_register (IspDev,0x3663, 0x28);
    os08a10_write_register (IspDev,0x3664, 0x0d);
    os08a10_write_register (IspDev,0x366a, 0x38);
    os08a10_write_register (IspDev,0x366b, 0xa0);
    os08a10_write_register (IspDev,0x366d, 0x00);
    os08a10_write_register (IspDev,0x366e, 0x00);
    os08a10_write_register (IspDev,0x3680, 0x00);
    os08a10_write_register (IspDev,0x3705, 0x00);
    os08a10_write_register (IspDev,0x3706, 0x35);
    os08a10_write_register (IspDev,0x370a, 0x00);
    os08a10_write_register (IspDev,0x370b, 0x98);
    os08a10_write_register (IspDev,0x3709, 0x49);
    os08a10_write_register (IspDev,0x3714, 0x21);
    os08a10_write_register (IspDev,0x371c, 0x00);
    os08a10_write_register (IspDev,0x371d, 0x08);
    os08a10_write_register (IspDev,0x375e, 0x0b);
    os08a10_write_register (IspDev,0x3762, 0x12);
    os08a10_write_register (IspDev,0x3776, 0x10);
    os08a10_write_register (IspDev,0x3781, 0x02);
    os08a10_write_register (IspDev,0x3782, 0x04);
    os08a10_write_register (IspDev,0x3783, 0x02);
    os08a10_write_register (IspDev,0x3784, 0x08);
    os08a10_write_register (IspDev,0x3785, 0x08);
    os08a10_write_register (IspDev,0x3788, 0x01);
    os08a10_write_register (IspDev,0x3789, 0x01);
    os08a10_write_register (IspDev,0x3797, 0x04);
    os08a10_write_register (IspDev,0x3800, 0x00);
    os08a10_write_register (IspDev,0x3801, 0x00);
    os08a10_write_register (IspDev,0x3802, 0x00);
    os08a10_write_register (IspDev,0x3803, 0x0c);
    os08a10_write_register (IspDev,0x3804, 0x0e);
    os08a10_write_register (IspDev,0x3805, 0xff);
    os08a10_write_register (IspDev,0x3806, 0x08);
    os08a10_write_register (IspDev,0x3807, 0x6f);
    os08a10_write_register (IspDev,0x3808, 0x0f);
    os08a10_write_register (IspDev,0x3809, 0x00);
    os08a10_write_register (IspDev,0x380a, 0x08);
    os08a10_write_register (IspDev,0x380b, 0x70);
    os08a10_write_register (IspDev,0x380c, 0x04);
    os08a10_write_register (IspDev,0x380d, 0xdb);
    os08a10_write_register (IspDev,0x380e, 0x09);
    os08a10_write_register (IspDev,0x380f, 0x0d);
    os08a10_write_register (IspDev,0x3813, 0x10);
    os08a10_write_register (IspDev,0x3814, 0x01);
    os08a10_write_register (IspDev,0x3815, 0x01);
    os08a10_write_register (IspDev,0x3816, 0x01);
    os08a10_write_register (IspDev,0x3817, 0x01);
    os08a10_write_register (IspDev,0x381c, 0x08);
    //os08a10_write_register (IspDev,0x3820, 0x00);
    //os08a10_write_register (IspDev,0x3821, 0x00);
    //mirror
    os08a10_write_register (IspDev,0x3820, 0x00);
    os08a10_write_register (IspDev,0x3821, 0x24);
    os08a10_write_register (IspDev,0x3823, 0x08);
    os08a10_write_register (IspDev,0x3826, 0x00);
    os08a10_write_register (IspDev,0x3827, 0x08);
    os08a10_write_register (IspDev,0x382d, 0x08);
    os08a10_write_register (IspDev,0x3832, 0x02);
    os08a10_write_register (IspDev,0x383c, 0x48);
    os08a10_write_register (IspDev,0x383d, 0xff);
    os08a10_write_register (IspDev,0x3d85, 0x0b);
    os08a10_write_register (IspDev,0x3d84, 0x40);
    os08a10_write_register (IspDev,0x3d8c, 0x63);
    os08a10_write_register (IspDev,0x3d8d, 0xd7);
    os08a10_write_register (IspDev,0x4000, 0xf8);
    os08a10_write_register (IspDev,0x4001, 0x2f);
    os08a10_write_register (IspDev,0x400a, 0x01);
    os08a10_write_register (IspDev,0x400f, 0xa1);
    os08a10_write_register (IspDev,0x4010, 0x12);
    os08a10_write_register (IspDev,0x4018, 0x04);
    os08a10_write_register (IspDev,0x4008, 0x02);
    os08a10_write_register (IspDev,0x4009, 0x0d);
    os08a10_write_register (IspDev,0x401a, 0x58);
    os08a10_write_register (IspDev,0x4050, 0x00);
    os08a10_write_register (IspDev,0x4051, 0x01);
    os08a10_write_register (IspDev,0x4028, 0x0f);
    os08a10_write_register (IspDev,0x4052, 0x00);
    os08a10_write_register (IspDev,0x4053, 0x80);
    os08a10_write_register (IspDev,0x4054, 0x00);
    os08a10_write_register (IspDev,0x4055, 0x80);
    os08a10_write_register (IspDev,0x4056, 0x00);
    os08a10_write_register (IspDev,0x4057, 0x80);
    os08a10_write_register (IspDev,0x4058, 0x00);
    os08a10_write_register (IspDev,0x4059, 0x80);
    os08a10_write_register (IspDev,0x430b, 0xff);
    os08a10_write_register (IspDev,0x430c, 0xff);
    os08a10_write_register (IspDev,0x430d, 0x00);
    os08a10_write_register (IspDev,0x430e, 0x00);
    os08a10_write_register (IspDev,0x4501, 0x18);
    os08a10_write_register (IspDev,0x4502, 0x00);
    os08a10_write_register (IspDev,0x4643, 0x00);
    os08a10_write_register (IspDev,0x4640, 0x01);
    os08a10_write_register (IspDev,0x4641, 0x04);
    os08a10_write_register (IspDev,0x4800, 0x04);
    os08a10_write_register (IspDev,0x4809, 0x2b);
    os08a10_write_register (IspDev,0x4813, 0x98);
    os08a10_write_register (IspDev,0x4817, 0x04);
    os08a10_write_register (IspDev,0x4833, 0x18);
    os08a10_write_register (IspDev,0x4837, 0x0d);
    os08a10_write_register (IspDev,0x483b, 0x00);
    os08a10_write_register (IspDev,0x484b, 0x03);
    os08a10_write_register (IspDev,0x4850, 0x7c);
    os08a10_write_register (IspDev,0x4852, 0x06);
    os08a10_write_register (IspDev,0x4856, 0x58);
    os08a10_write_register (IspDev,0x4857, 0xaa);
    os08a10_write_register (IspDev,0x4862, 0x0a);
    os08a10_write_register (IspDev,0x4869, 0x18);
    os08a10_write_register (IspDev,0x486a, 0xaa);
    os08a10_write_register (IspDev,0x486e, 0x07);
    os08a10_write_register (IspDev,0x486f, 0x55);
    os08a10_write_register (IspDev,0x4875, 0xf0);
    os08a10_write_register (IspDev,0x5000, 0x89);
    os08a10_write_register (IspDev,0x5001, 0x42);
    os08a10_write_register (IspDev,0x5004, 0x40);
    os08a10_write_register (IspDev,0x5005, 0x00);
    os08a10_write_register (IspDev,0x5180, 0x00);
    os08a10_write_register (IspDev,0x5181, 0x10);
    os08a10_write_register (IspDev,0x580b, 0x03);
    os08a10_write_register (IspDev,0x4700, 0x2b);
    os08a10_write_register (IspDev,0x4e00, 0x2b);
    delay_ms(5);  //delay 5ms
    os08a10_write_register (IspDev,0x0100, 0x01);

    printf("===os08a10 sensor 4k2k25fps 10bit 2to1 WDR(50fps->25fps) init success!=====\n");
    printf("===MIPI data rate = 1200Mbps/lane =====\n");

    return;

}

#if 0
void os08a10_wdr_1080p60_2to1_init(ISP_DEV IspDev)
{
 #if 0
    os08a10_write_register (IspDev,0x3000, 0x01); /* standby */
    os08a10_write_register (IspDev,0x3002, 0x00); /* XTMSTA */

    os08a10_write_register (IspDev,0x3005, 0x01);
    os08a10_write_register (IspDev,0x3007, 0x00);
    os08a10_write_register (IspDev,0x3009, 0x01);
    os08a10_write_register (IspDev,0x300a, 0xf0);
    os08a10_write_register (IspDev,0x300c, 0x11);
    os08a10_write_register (IspDev,0x300f, 0x00);
    os08a10_write_register (IspDev,0x3010, 0x21);
    os08a10_write_register (IspDev,0x3012, 0x64);
    os08a10_write_register (IspDev,0x3016, 0x09);
    os08a10_write_register (IspDev,0x3018, 0x65);
    os08a10_write_register (IspDev,0x3019, 0x04);

    os08a10_write_register (IspDev,0x301c, 0x98); /* HMAX */
    os08a10_write_register (IspDev,0x301d, 0x08); /* HMAX */

    os08a10_write_register (IspDev,0x3045, 0x05);
    os08a10_write_register (IspDev,0x3046, 0x01);
    os08a10_write_register (IspDev,0x304b, 0x0a);

    os08a10_write_register (IspDev,0x305c, 0x18);
    os08a10_write_register (IspDev,0x305d, 0x03);
    os08a10_write_register (IspDev,0x305e, 0x20);
    os08a10_write_register (IspDev,0x305f, 0x01);

    os08a10_write_register (IspDev,0x3070, 0x02);
    os08a10_write_register (IspDev,0x3071, 0x11);

    os08a10_write_register (IspDev,0x309b, 0x10);
    os08a10_write_register (IspDev,0x309c, 0x22);

    os08a10_write_register (IspDev,0x30a2, 0x02);
    os08a10_write_register (IspDev,0x30a6, 0x20);
    os08a10_write_register (IspDev,0x30a8, 0x20);

    os08a10_write_register (IspDev,0x30aa, 0x20);
    os08a10_write_register (IspDev,0x30ac, 0x20);
    os08a10_write_register (IspDev,0x30b0, 0x43);

    os08a10_write_register (IspDev,0x3106, 0x11);
    os08a10_write_register (IspDev,0x3119, 0x9e);
    os08a10_write_register (IspDev,0x311c, 0x1e);
    os08a10_write_register (IspDev,0x311e, 0x08);

    os08a10_write_register (IspDev,0x3128, 0x05);
    os08a10_write_register (IspDev,0x3129, 0x00);
    os08a10_write_register (IspDev,0x313d, 0x83);
    os08a10_write_register (IspDev,0x3150, 0x03);

    os08a10_write_register (IspDev,0x315e, 0x1a);
    os08a10_write_register (IspDev,0x3164, 0x1a);
    os08a10_write_register (IspDev,0x317c, 0x00);
    os08a10_write_register (IspDev,0x317e, 0x00);
    os08a10_write_register (IspDev,0x31ec, 0x00);

    os08a10_write_register (IspDev,0x32b8, 0x50);
    os08a10_write_register (IspDev,0x32b9, 0x10);
    os08a10_write_register (IspDev,0x32ba, 0x00);
    os08a10_write_register (IspDev,0x32bb, 0x04);

    os08a10_write_register (IspDev,0x32c8, 0x50);
    os08a10_write_register (IspDev,0x32c9, 0x10);
    os08a10_write_register (IspDev,0x32ca, 0x00);
    os08a10_write_register (IspDev,0x32cb, 0x04);

    os08a10_write_register (IspDev,0x332c, 0xd3);
    os08a10_write_register (IspDev,0x332d, 0x10);
    os08a10_write_register (IspDev,0x332e, 0x0d);

    os08a10_write_register (IspDev,0x3358, 0x06);
    os08a10_write_register (IspDev,0x3359, 0xe1);
    os08a10_write_register (IspDev,0x335a, 0x11);

    os08a10_write_register (IspDev,0x3360, 0x1e);
    os08a10_write_register (IspDev,0x3361, 0x61);
    os08a10_write_register (IspDev,0x3362, 0x10);

    os08a10_write_register (IspDev,0x33b0, 0x50);
    os08a10_write_register (IspDev,0x33b2, 0x1a);
    os08a10_write_register (IspDev,0x33b3, 0x04);

    os08a10_write_register (IspDev,0x3405, 0x10);
    os08a10_write_register (IspDev,0x3407, 0x03);
    os08a10_write_register (IspDev,0x3414, 0x0a);
    os08a10_write_register (IspDev,0x3415, 0x00);
    os08a10_write_register (IspDev,0x3418, 0xb2);
    os08a10_write_register (IspDev,0x3419, 0x08);

    os08a10_write_register (IspDev,0x3441, 0x0c);
    os08a10_write_register (IspDev,0x3442, 0x0c);
    os08a10_write_register (IspDev,0x3443, 0x03);
    os08a10_write_register (IspDev,0x3444, 0x20);
    os08a10_write_register (IspDev,0x3445, 0x25);
    os08a10_write_register (IspDev,0x3446, 0x57);
    os08a10_write_register (IspDev,0x3447, 0x00);
    os08a10_write_register (IspDev,0x3448, 0x37);
    os08a10_write_register (IspDev,0x3449, 0x00);
    os08a10_write_register (IspDev,0x344a, 0x1f);

    os08a10_write_register (IspDev,0x344b, 0x00);
    os08a10_write_register (IspDev,0x344c, 0x1f);
    os08a10_write_register (IspDev,0x344d, 0x00);
    os08a10_write_register (IspDev,0x344e, 0x1f);
    os08a10_write_register (IspDev,0x344f, 0x00);
    os08a10_write_register (IspDev,0x3450, 0x77);
    os08a10_write_register (IspDev,0x3451, 0x00);
    os08a10_write_register (IspDev,0x3452, 0x1f);
    os08a10_write_register (IspDev,0x3453, 0x00);
    os08a10_write_register (IspDev,0x3454, 0x17);
    os08a10_write_register (IspDev,0x3455, 0x00);
    os08a10_write_register (IspDev,0x3472, 0x9c);
    os08a10_write_register (IspDev,0x3473, 0x07);
    os08a10_write_register (IspDev,0x3480, 0x49);

    os08a10_write_register (IspDev,0x3000, 0x00); /* standby */
    delay_ms(20);
    os08a10_write_register (IspDev,0x3002, 0x00); /* master mode start */

    printf("===os08a10 sensor 1080P30fps 12bit 2to1 WDR(60fps->30fps) init success!=====\n");

    bSensorInit = HI_TRUE;

    return;
 #endif
    os08a10_write_register (IspDev,0x3000, 0x01); /* standby */
    os08a10_write_register (IspDev,0x3002, 0x01); /* XTMSTA */

    os08a10_write_register (IspDev,0x3005, 0x00);
    os08a10_write_register (IspDev,0x3007, 0x00);
    os08a10_write_register (IspDev,0x3009, 0x01);
    os08a10_write_register (IspDev,0x300a, 0x3c);
    os08a10_write_register (IspDev,0x300c, 0x11);
    os08a10_write_register (IspDev,0x300f, 0x00);
    os08a10_write_register (IspDev,0x3010, 0x21);
    os08a10_write_register (IspDev,0x3012, 0x64);
    os08a10_write_register (IspDev,0x3016, 0x09);
    os08a10_write_register (IspDev,0x3018, 0xC4); /* VMAX */
    os08a10_write_register (IspDev,0x3019, 0x04); /* VMAX */
    os08a10_write_register (IspDev,0x301c, 0xEC); /* HMAX */
    os08a10_write_register (IspDev,0x301d, 0x07); /* HMAX */

#if 1
    os08a10_write_register (IspDev,0x3020, 0x03);  /* SHS1 */
    os08a10_write_register (IspDev,0x3021, 0x00);
    //os08a10_write_register (IspDev,0x3022, 0x00);
    os08a10_write_register (IspDev,0x3024, 0x99);  /* SHS2 */
    os08a10_write_register (IspDev,0x3025, 0x00);
    //os08a10_write_register (IspDev,0x3025, 0x00);
    os08a10_write_register (IspDev,0x3028, 0x00);  /* SHS3 */
    os08a10_write_register (IspDev,0x3029, 0x00);
    //os08a10_write_register (IspDev,0x302A, 0x00);
    os08a10_write_register (IspDev,0x3030, 0x93);  /* RHS1 */
    os08a10_write_register (IspDev,0x3031, 0x00);
    os08a10_write_register (IspDev,0x3034, 0x00);  /* RHS2 */
    os08a10_write_register (IspDev,0x3035, 0x00);
#endif

    os08a10_write_register (IspDev,0x3045, 0x05);
    os08a10_write_register (IspDev,0x3046, 0x00);
    os08a10_write_register (IspDev,0x304b, 0x0a);
    os08a10_write_register (IspDev,0x305c, 0x18);
    os08a10_write_register (IspDev,0x305d, 0x03);
    os08a10_write_register (IspDev,0x305e, 0x20);
    os08a10_write_register (IspDev,0x305f, 0x01);
    os08a10_write_register (IspDev,0x3070, 0x02);
    os08a10_write_register (IspDev,0x3071, 0x11);
    os08a10_write_register (IspDev,0x309b, 0x10);
    os08a10_write_register (IspDev,0x309c, 0x22);
    os08a10_write_register (IspDev,0x30a2, 0x02);
    os08a10_write_register (IspDev,0x30a6, 0x20);
    os08a10_write_register (IspDev,0x30a8, 0x20);
    os08a10_write_register (IspDev,0x30aa, 0x20);
    os08a10_write_register (IspDev,0x30ac, 0x20);
    os08a10_write_register (IspDev,0x30b0, 0x43);

    os08a10_write_register (IspDev,0x3106, 0x11);
    os08a10_write_register (IspDev,0x3119, 0x9e);
    os08a10_write_register (IspDev,0x311c, 0x1e);
    os08a10_write_register (IspDev,0x311e, 0x08);
    os08a10_write_register (IspDev,0x3128, 0x05);
    os08a10_write_register (IspDev,0x3129, 0x1d);
    os08a10_write_register (IspDev,0x313d, 0x83);
    os08a10_write_register (IspDev,0x3150, 0x03);
    os08a10_write_register (IspDev,0x315e, 0x1a);
    os08a10_write_register (IspDev,0x3164, 0x1a);
    os08a10_write_register (IspDev,0x317c, 0x12);
    os08a10_write_register (IspDev,0x317e, 0x00);
    os08a10_write_register (IspDev,0x31ec, 0x37);

    os08a10_write_register (IspDev,0x32b8, 0x50);
    os08a10_write_register (IspDev,0x32b9, 0x10);
    os08a10_write_register (IspDev,0x32ba, 0x00);
    os08a10_write_register (IspDev,0x32bb, 0x04);
    os08a10_write_register (IspDev,0x32c8, 0x50);
    os08a10_write_register (IspDev,0x32c9, 0x10);
    os08a10_write_register (IspDev,0x32ca, 0x00);
    os08a10_write_register (IspDev,0x32cb, 0x04);

    os08a10_write_register (IspDev,0x332c, 0xd3);
    os08a10_write_register (IspDev,0x332d, 0x10);
    os08a10_write_register (IspDev,0x332e, 0x0d);
    os08a10_write_register (IspDev,0x3358, 0x06);
    os08a10_write_register (IspDev,0x3359, 0xe1);
    os08a10_write_register (IspDev,0x335a, 0x11);
    os08a10_write_register (IspDev,0x3360, 0x1e);
    os08a10_write_register (IspDev,0x3361, 0x61);
    os08a10_write_register (IspDev,0x3362, 0x10);
    os08a10_write_register (IspDev,0x33b0, 0x50);
    os08a10_write_register (IspDev,0x33b2, 0x1a);
    os08a10_write_register (IspDev,0x33b3, 0x04);

    os08a10_write_register (IspDev,0x3405, 0x10);
    os08a10_write_register (IspDev,0x3407, 0x03);
    os08a10_write_register (IspDev,0x3414, 0x0a);
    os08a10_write_register (IspDev,0x3415, 0x00);
    os08a10_write_register (IspDev,0x3418, 0x32);  /* Y_OUT_SIZE */
    os08a10_write_register (IspDev,0x3419, 0x09);  /* Y_OUT_SIZE */
    os08a10_write_register (IspDev,0x3441, 0x0a);
    os08a10_write_register (IspDev,0x3442, 0x0a);
    os08a10_write_register (IspDev,0x3443, 0x03);
    os08a10_write_register (IspDev,0x3444, 0x20);
    os08a10_write_register (IspDev,0x3445, 0x25);
    os08a10_write_register (IspDev,0x3446, 0x57);
    os08a10_write_register (IspDev,0x3447, 0x00);
    os08a10_write_register (IspDev,0x3448, 0x37);
    os08a10_write_register (IspDev,0x3449, 0x00);
    os08a10_write_register (IspDev,0x344a, 0x1f);
    os08a10_write_register (IspDev,0x344b, 0x00);
    os08a10_write_register (IspDev,0x344c, 0x1f);
    os08a10_write_register (IspDev,0x344d, 0x00);
    os08a10_write_register (IspDev,0x344e, 0x1f);
    os08a10_write_register (IspDev,0x344f, 0x00);
    os08a10_write_register (IspDev,0x3450, 0x77);
    os08a10_write_register (IspDev,0x3451, 0x00);
    os08a10_write_register (IspDev,0x3452, 0x1f);
    os08a10_write_register (IspDev,0x3453, 0x00);
    os08a10_write_register (IspDev,0x3454, 0x17);
    os08a10_write_register (IspDev,0x3455, 0x00);
    os08a10_write_register (IspDev,0x3472, 0x9c);
    os08a10_write_register (IspDev,0x3473, 0x07);
    os08a10_write_register (IspDev,0x3480, 0x49);

    os08a10_write_register (IspDev,0x3000, 0x00); /* standby */
    delay_ms(20);
    os08a10_write_register (IspDev,0x3002, 0x00); /* master mode start */

    printf("===os08a10 sensor 1080P30fps 10bit 2to1 WDR(60fps->30fps) init success!=====\n");

    return;

}
#endif

#if 0
void os08a10_wdr_1080p30_3to1_init(ISP_DEV IspDev)
{
    os08a10_write_register (IspDev,0x3000, 0x01); /* standby */
    os08a10_write_register (IspDev,0x3002, 0x01); /* XTMSTA */

    //12bit
    os08a10_write_register (IspDev,0x3007, 0x00);
    os08a10_write_register (IspDev,0x300c, 0x21);
    os08a10_write_register (IspDev,0x300f, 0x00);
    os08a10_write_register (IspDev,0x3010, 0x21);
    os08a10_write_register (IspDev,0x3012, 0x64);
    os08a10_write_register (IspDev,0x3016, 0x09);
    os08a10_write_register (IspDev,0x3020, 0x04);
    os08a10_write_register (IspDev,0x3021, 0x00);
    os08a10_write_register (IspDev,0x3024, 0xF2);
    os08a10_write_register (IspDev,0x3025, 0x01);
    os08a10_write_register (IspDev,0x3028, 0x57);
    os08a10_write_register (IspDev,0x3029, 0x02);
    os08a10_write_register (IspDev,0x3030, 0xED);
    os08a10_write_register (IspDev,0x3031, 0x01);
    os08a10_write_register (IspDev,0x3034, 0x30);
    os08a10_write_register (IspDev,0x3035, 0x02);
    os08a10_write_register (IspDev,0x3045, 0x05);
    os08a10_write_register (IspDev,0x304b, 0x0a);
    os08a10_write_register (IspDev,0x305c, 0x18);
    os08a10_write_register (IspDev,0x305d, 0x03);
    os08a10_write_register (IspDev,0x305e, 0x20);
    os08a10_write_register (IspDev,0x305f, 0x01);
    os08a10_write_register (IspDev,0x3070, 0x02);
    os08a10_write_register (IspDev,0x3071, 0x11);
    os08a10_write_register (IspDev,0x309b, 0x10);
    os08a10_write_register (IspDev,0x309c, 0x22);
    os08a10_write_register (IspDev,0x30a2, 0x02);
    os08a10_write_register (IspDev,0x30a6, 0x20);
    os08a10_write_register (IspDev,0x30a8, 0x20);
    os08a10_write_register (IspDev,0x30aa, 0x20);
    os08a10_write_register (IspDev,0x30ac, 0x20);
    os08a10_write_register (IspDev,0x30b0, 0x43);
    os08a10_write_register (IspDev,0x3106, 0x33);
    os08a10_write_register (IspDev,0x3119, 0x9e);
    os08a10_write_register (IspDev,0x311c, 0x1e);
    os08a10_write_register (IspDev,0x311e, 0x08);
    os08a10_write_register (IspDev,0x3128, 0x05);
    os08a10_write_register (IspDev,0x313d, 0x83);
    os08a10_write_register (IspDev,0x3150, 0x03);
    os08a10_write_register (IspDev,0x315e, 0x1a);
    os08a10_write_register (IspDev,0x3164, 0x1a);
    os08a10_write_register (IspDev,0x317e, 0x00);
    os08a10_write_register (IspDev,0x32b8, 0x50);
    os08a10_write_register (IspDev,0x32b9, 0x10);
    os08a10_write_register (IspDev,0x32ba, 0x00);
    os08a10_write_register (IspDev,0x32bb, 0x04);
    os08a10_write_register (IspDev,0x32c8, 0x50);
    os08a10_write_register (IspDev,0x32c9, 0x10);
    os08a10_write_register (IspDev,0x32ca, 0x00);
    os08a10_write_register (IspDev,0x32cb, 0x04);
    os08a10_write_register (IspDev,0x332c, 0xd3);
    os08a10_write_register (IspDev,0x332d, 0x10);
    os08a10_write_register (IspDev,0x332e, 0x0d);
    os08a10_write_register (IspDev,0x3358, 0x06);
    os08a10_write_register (IspDev,0x3359, 0xe1);
    os08a10_write_register (IspDev,0x335a, 0x11);
    os08a10_write_register (IspDev,0x3360, 0x1e);
    os08a10_write_register (IspDev,0x3361, 0x61);
    os08a10_write_register (IspDev,0x3362, 0x10);
    os08a10_write_register (IspDev,0x33b0, 0x50);
    os08a10_write_register (IspDev,0x33b2, 0x1a);
    os08a10_write_register (IspDev,0x33b3, 0x04);

    os08a10_write_register (IspDev,0x3418, 0x24); /**** Y_OUT_SIZE *****/
    os08a10_write_register (IspDev,0x3419, 0x0F);

    os08a10_write_register (IspDev,0x3444, 0x20);
    os08a10_write_register (IspDev,0x3445, 0x25);
    os08a10_write_register (IspDev,0x3480, 0x49);

    os08a10_write_register (IspDev,0x3000, 0x00); /* standby */
    delay_ms(20);
    os08a10_write_register (IspDev,0x3002, 0x00); /* master mode start */

    printf("===os08a10 os08a10 1080P15fps 12bit 3to1 WDR(30fps->7p5fps) init success!=====\n");

    return;

}

#endif

#if 0
void os08a10_wdr_720p60_2to1_init(ISP_DEV IspDev)
{
    os08a10_write_register (IspDev,0x3000, 0x01); /* standby */
    os08a10_write_register (IspDev,0x3002, 0x01); /* XTMSTA */

    //12bit
    os08a10_write_register (IspDev,0x3005, 0x01);
    os08a10_write_register (IspDev,0x3007, 0x10);
    os08a10_write_register (IspDev,0x3009, 0x01);
    os08a10_write_register (IspDev,0x300a, 0xf0);
    os08a10_write_register (IspDev,0x300c, 0x11);
    os08a10_write_register (IspDev,0x300f, 0x00);
    os08a10_write_register (IspDev,0x3010, 0x21);
    os08a10_write_register (IspDev,0x3012, 0x64);
    os08a10_write_register (IspDev,0x3016, 0x09);
    os08a10_write_register (IspDev,0x3018, 0xee);
    os08a10_write_register (IspDev,0x3019, 0x02);
    os08a10_write_register (IspDev,0x301c, 0xe4);
    os08a10_write_register (IspDev,0x301d, 0x0c);
    os08a10_write_register (IspDev,0x3045, 0x05);
    os08a10_write_register (IspDev,0x3046, 0x01);
    os08a10_write_register (IspDev,0x304b, 0x0a);

    os08a10_write_register (IspDev,0x305c, 0x20); //INCKSEL1
    os08a10_write_register (IspDev,0x305d, 0x03); //INCKSEL2
    os08a10_write_register (IspDev,0x305e, 0x20); //INCKSEL3
    os08a10_write_register (IspDev,0x305f, 0x01); //INCKSEL4

    os08a10_write_register (IspDev,0x3070, 0x02);
    os08a10_write_register (IspDev,0x3071, 0x11);
    os08a10_write_register (IspDev,0x309b, 0x10);
    os08a10_write_register (IspDev,0x309c, 0x22);
    os08a10_write_register (IspDev,0x30a2, 0x02);
    os08a10_write_register (IspDev,0x30a6, 0x20);
    os08a10_write_register (IspDev,0x30a8, 0x20);
    os08a10_write_register (IspDev,0x30aa, 0x20);
    os08a10_write_register (IspDev,0x30ac, 0x20);
    os08a10_write_register (IspDev,0x30b0, 0x43);

    //Add
    os08a10_write_register (IspDev,0x3106, 0x11);

    os08a10_write_register (IspDev,0x3119, 0x9e);
    os08a10_write_register (IspDev,0x311c, 0x1e);
    os08a10_write_register (IspDev,0x311e, 0x08);
    os08a10_write_register (IspDev,0x3128, 0x05);
    os08a10_write_register (IspDev,0x3129, 0x00);
    os08a10_write_register (IspDev,0x313d, 0x83);
    os08a10_write_register (IspDev,0x3150, 0x03);
    os08a10_write_register (IspDev,0x315e, 0x1a);
    os08a10_write_register (IspDev,0x3164, 0x1a);
    os08a10_write_register (IspDev,0x317c, 0x00);
    os08a10_write_register (IspDev,0x317e, 0x00);
    os08a10_write_register (IspDev,0x31ec, 0x00);

    os08a10_write_register (IspDev,0x32b8, 0x50);
    os08a10_write_register (IspDev,0x32b9, 0x10);
    os08a10_write_register (IspDev,0x32ba, 0x00);
    os08a10_write_register (IspDev,0x32bb, 0x04);
    os08a10_write_register (IspDev,0x32c8, 0x50);
    os08a10_write_register (IspDev,0x32c9, 0x10);
    os08a10_write_register (IspDev,0x32ca, 0x00);
    os08a10_write_register (IspDev,0x32cb, 0x04);

    os08a10_write_register (IspDev,0x332c, 0xd3);
    os08a10_write_register (IspDev,0x332d, 0x10);
    os08a10_write_register (IspDev,0x332e, 0x0d);
    os08a10_write_register (IspDev,0x3358, 0x06);
    os08a10_write_register (IspDev,0x3359, 0xe1);
    os08a10_write_register (IspDev,0x335a, 0x11);
    os08a10_write_register (IspDev,0x3360, 0x1e);
    os08a10_write_register (IspDev,0x3361, 0x61);
    os08a10_write_register (IspDev,0x3362, 0x10);
    os08a10_write_register (IspDev,0x33b0, 0x50);
    os08a10_write_register (IspDev,0x33b2, 0x1a);
    os08a10_write_register (IspDev,0x33b3, 0x04);

    os08a10_write_register (IspDev,0x3405, 0x10);
    os08a10_write_register (IspDev,0x3407, 0x03);
    os08a10_write_register (IspDev,0x3414, 0x04);
    os08a10_write_register (IspDev,0x3418, 0xc6);
    os08a10_write_register (IspDev,0x3419, 0x05);
    os08a10_write_register (IspDev,0x3441, 0x0c);
    os08a10_write_register (IspDev,0x3442, 0x0c);
    os08a10_write_register (IspDev,0x3443, 0x03);
    os08a10_write_register (IspDev,0x3444, 0x20);
    os08a10_write_register (IspDev,0x3445, 0x25);
    os08a10_write_register (IspDev,0x3446, 0x4f);
    os08a10_write_register (IspDev,0x3447, 0x00);
    os08a10_write_register (IspDev,0x3448, 0x2f);
    os08a10_write_register (IspDev,0x3449, 0x00);
    os08a10_write_register (IspDev,0x344a, 0x17);
    os08a10_write_register (IspDev,0x344b, 0x00);
    os08a10_write_register (IspDev,0x344c, 0x17);
    os08a10_write_register (IspDev,0x344d, 0x00);
    os08a10_write_register (IspDev,0x344e, 0x17);
    os08a10_write_register (IspDev,0x344f, 0x00);
    os08a10_write_register (IspDev,0x3450, 0x57);
    os08a10_write_register (IspDev,0x3451, 0x00);
    os08a10_write_register (IspDev,0x3452, 0x17);
    os08a10_write_register (IspDev,0x3453, 0x00);
    os08a10_write_register (IspDev,0x3454, 0x17);
    os08a10_write_register (IspDev,0x3455, 0x00);
    os08a10_write_register (IspDev,0x3472, 0x1c);
    os08a10_write_register (IspDev,0x3473, 0x05);
    os08a10_write_register (IspDev,0x3480, 0x49);

    os08a10_write_register (IspDev,0x3000, 0x00); /* standby */
    delay_ms(20);
    os08a10_write_register (IspDev,0x3002, 0x00); /* master mode start */

    printf("===os08a10 os08a10 720P30fps 12bit 2to1 WDR(60fps->30fps) init success!=====\n");

    return;
}

#endif

#if 0
void os08a10_wdr_720p60_3to1_init(ISP_DEV IspDev)
{
    os08a10_write_register (IspDev,0x3000, 0x01); /* standby */
    os08a10_write_register (IspDev,0x3002, 0x01); /* XTMSTA */

    //12bit
    os08a10_write_register (IspDev,0x3005, 0x01);
    os08a10_write_register (IspDev,0x3007, 0x10);
    os08a10_write_register (IspDev,0x3009, 0x01);
    os08a10_write_register (IspDev,0x300a, 0xf0);
    os08a10_write_register (IspDev,0x300c, 0x31);
    os08a10_write_register (IspDev,0x300f, 0x00);
    os08a10_write_register (IspDev,0x3010, 0x21);
    os08a10_write_register (IspDev,0x3012, 0x64);
    os08a10_write_register (IspDev,0x3016, 0x09);
    os08a10_write_register (IspDev,0x3018, 0xee);
    os08a10_write_register (IspDev,0x3019, 0x02);
    os08a10_write_register (IspDev,0x301c, 0xe4);
    os08a10_write_register (IspDev,0x301d, 0x0c);
    os08a10_write_register (IspDev,0x3045, 0x05);
    os08a10_write_register (IspDev,0x3046, 0x01);
    os08a10_write_register (IspDev,0x304b, 0x0a);

    os08a10_write_register (IspDev,0x305c, 0x20); //INCKSEL1
    os08a10_write_register (IspDev,0x305d, 0x03); //INCKSEL2
    os08a10_write_register (IspDev,0x305e, 0x20); //INCKSEL3
    os08a10_write_register (IspDev,0x305f, 0x01); //INCKSEL4

    os08a10_write_register (IspDev,0x3070, 0x02);
    os08a10_write_register (IspDev,0x3071, 0x11);
    os08a10_write_register (IspDev,0x309b, 0x10);
    os08a10_write_register (IspDev,0x309c, 0x22);
    os08a10_write_register (IspDev,0x30a2, 0x02);
    os08a10_write_register (IspDev,0x30a6, 0x20);
    os08a10_write_register (IspDev,0x30a8, 0x20);
    os08a10_write_register (IspDev,0x30aa, 0x20);
    os08a10_write_register (IspDev,0x30ac, 0x20);
    os08a10_write_register (IspDev,0x30b0, 0x43);

    //Add
    os08a10_write_register (IspDev,0x3106, 0x33);

    os08a10_write_register (IspDev,0x3119, 0x9e);
    os08a10_write_register (IspDev,0x311c, 0x1e);
    os08a10_write_register (IspDev,0x311e, 0x08);
    os08a10_write_register (IspDev,0x3128, 0x05);
    os08a10_write_register (IspDev,0x3129, 0x00);
    os08a10_write_register (IspDev,0x313d, 0x83);
    os08a10_write_register (IspDev,0x3150, 0x03);
    os08a10_write_register (IspDev,0x315e, 0x1a);
    os08a10_write_register (IspDev,0x3164, 0x1a);
    os08a10_write_register (IspDev,0x317c, 0x00);
    os08a10_write_register (IspDev,0x317e, 0x00);
    os08a10_write_register (IspDev,0x31ec, 0x00);

    os08a10_write_register (IspDev,0x32b8, 0x50);
    os08a10_write_register (IspDev,0x32b9, 0x10);
    os08a10_write_register (IspDev,0x32ba, 0x00);
    os08a10_write_register (IspDev,0x32bb, 0x04);
    os08a10_write_register (IspDev,0x32c8, 0x50);
    os08a10_write_register (IspDev,0x32c9, 0x10);
    os08a10_write_register (IspDev,0x32ca, 0x00);
    os08a10_write_register (IspDev,0x32cb, 0x04);

    os08a10_write_register (IspDev,0x332c, 0xd3);
    os08a10_write_register (IspDev,0x332d, 0x10);
    os08a10_write_register (IspDev,0x332e, 0x0d);
    os08a10_write_register (IspDev,0x3358, 0x06);
    os08a10_write_register (IspDev,0x3359, 0xe1);
    os08a10_write_register (IspDev,0x335a, 0x11);
    os08a10_write_register (IspDev,0x3360, 0x1e);
    os08a10_write_register (IspDev,0x3361, 0x61);
    os08a10_write_register (IspDev,0x3362, 0x10);
    os08a10_write_register (IspDev,0x33b0, 0x50);
    os08a10_write_register (IspDev,0x33b2, 0x1a);
    os08a10_write_register (IspDev,0x33b3, 0x04);

    os08a10_write_register (IspDev,0x3405, 0x10);
    os08a10_write_register (IspDev,0x3407, 0x03);
    os08a10_write_register (IspDev,0x3414, 0x04);
    os08a10_write_register (IspDev,0x3418, 0xb5);
    os08a10_write_register (IspDev,0x3419, 0x08);
    os08a10_write_register (IspDev,0x3441, 0x0c);
    os08a10_write_register (IspDev,0x3442, 0x0c);
    os08a10_write_register (IspDev,0x3443, 0x03);
    os08a10_write_register (IspDev,0x3444, 0x20);
    os08a10_write_register (IspDev,0x3445, 0x25);
    os08a10_write_register (IspDev,0x3446, 0x4f);
    os08a10_write_register (IspDev,0x3447, 0x00);
    os08a10_write_register (IspDev,0x3448, 0x2f);
    os08a10_write_register (IspDev,0x3449, 0x00);
    os08a10_write_register (IspDev,0x344a, 0x17);
    os08a10_write_register (IspDev,0x344b, 0x00);
    os08a10_write_register (IspDev,0x344c, 0x17);
    os08a10_write_register (IspDev,0x344d, 0x00);
    os08a10_write_register (IspDev,0x344e, 0x17);
    os08a10_write_register (IspDev,0x344f, 0x00);
    os08a10_write_register (IspDev,0x3450, 0x57);
    os08a10_write_register (IspDev,0x3451, 0x00);
    os08a10_write_register (IspDev,0x3452, 0x17);
    os08a10_write_register (IspDev,0x3453, 0x00);
    os08a10_write_register (IspDev,0x3454, 0x17);
    os08a10_write_register (IspDev,0x3455, 0x00);
    os08a10_write_register (IspDev,0x3472, 0x1c);
    os08a10_write_register (IspDev,0x3473, 0x05);
    os08a10_write_register (IspDev,0x3480, 0x49);

    os08a10_write_register (IspDev,0x3000, 0x00); /* standby */
    delay_ms(20);
    os08a10_write_register (IspDev,0x3002, 0x00); /* master mode start */

    printf("===os08a10 os08a10 720P15fps 12bit 3to1 WDR(60fps->15fps) init success!=====\n");
    return;
}
#endif

#if 0
void os08a10_wdr_1080p120_2to1_init(ISP_DEV IspDev)
{
    os08a10_write_register (IspDev,0x3000, 0x01); /* standby */
    os08a10_write_register (IspDev,0x3002, 0x01); /* XTMSTA */

    os08a10_write_register (IspDev,0x3005, 0x00);
    os08a10_write_register (IspDev,0x3007, 0x00);
    os08a10_write_register (IspDev,0x3009, 0x00);
    os08a10_write_register (IspDev,0x300a, 0x3c);
    os08a10_write_register (IspDev,0x300c, 0x11);
    os08a10_write_register (IspDev,0x300f, 0x00);
    os08a10_write_register (IspDev,0x3010, 0x21);
    os08a10_write_register (IspDev,0x3012, 0x64);
    os08a10_write_register (IspDev,0x3016, 0x09);
    os08a10_write_register (IspDev,0x3018, 0x65);
    os08a10_write_register (IspDev,0x3019, 0x04);
    os08a10_write_register (IspDev,0x301c, 0xF6);
    os08a10_write_register (IspDev,0x301d, 0x03);
    os08a10_write_register (IspDev,0x3020, 0x02);
    os08a10_write_register (IspDev,0x3024, 0xc9);
    os08a10_write_register (IspDev,0x3030, 0x0b);
    os08a10_write_register (IspDev,0x3045, 0x05);
    os08a10_write_register (IspDev,0x3046, 0x00);
    os08a10_write_register (IspDev,0x304b, 0x0a);
    os08a10_write_register (IspDev,0x305c, 0x18);
    os08a10_write_register (IspDev,0x305d, 0x03);
    os08a10_write_register (IspDev,0x305e, 0x20);
    os08a10_write_register (IspDev,0x305f, 0x01);
    os08a10_write_register (IspDev,0x3070, 0x02);
    os08a10_write_register (IspDev,0x3071, 0x11);
    os08a10_write_register (IspDev,0x309b, 0x10);
    os08a10_write_register (IspDev,0x309c, 0x22);
    os08a10_write_register (IspDev,0x30a2, 0x02);
    os08a10_write_register (IspDev,0x30a6, 0x20);
    os08a10_write_register (IspDev,0x30a8, 0x20);
    os08a10_write_register (IspDev,0x30aa, 0x20);
    os08a10_write_register (IspDev,0x30ac, 0x20);
    os08a10_write_register (IspDev,0x30b0, 0x43);
    os08a10_write_register (IspDev,0x3106, 0x11);
    os08a10_write_register (IspDev,0x3119, 0x9e);
    os08a10_write_register (IspDev,0x311c, 0x1e);
    os08a10_write_register (IspDev,0x311e, 0x08);
    os08a10_write_register (IspDev,0x3128, 0x05);
    os08a10_write_register (IspDev,0x3129, 0x1d);
    os08a10_write_register (IspDev,0x313d, 0x83);
    os08a10_write_register (IspDev,0x3150, 0x03);
    os08a10_write_register (IspDev,0x315e, 0x1a);
    os08a10_write_register (IspDev,0x3164, 0x1a);
    os08a10_write_register (IspDev,0x317c, 0x12);
    os08a10_write_register (IspDev,0x317e, 0x00);
    os08a10_write_register (IspDev,0x31ec, 0x37);

    os08a10_write_register (IspDev,0x32b8, 0x50);
    os08a10_write_register (IspDev,0x32b9, 0x10);
    os08a10_write_register (IspDev,0x32ba, 0x00);
    os08a10_write_register (IspDev,0x32bb, 0x04);
    os08a10_write_register (IspDev,0x32c8, 0x50);
    os08a10_write_register (IspDev,0x32c9, 0x10);
    os08a10_write_register (IspDev,0x32ca, 0x00);
    os08a10_write_register (IspDev,0x32cb, 0x04);

    os08a10_write_register (IspDev,0x332c, 0xd3);
    os08a10_write_register (IspDev,0x332d, 0x10);
    os08a10_write_register (IspDev,0x332e, 0x0d);
    os08a10_write_register (IspDev,0x3358, 0x06);
    os08a10_write_register (IspDev,0x3359, 0xe1);
    os08a10_write_register (IspDev,0x335a, 0x11);
    os08a10_write_register (IspDev,0x3360, 0x1e);
    os08a10_write_register (IspDev,0x3361, 0x61);
    os08a10_write_register (IspDev,0x3362, 0x10);
    os08a10_write_register (IspDev,0x33b0, 0x50);
    os08a10_write_register (IspDev,0x33b2, 0x1a);
    os08a10_write_register (IspDev,0x33b3, 0x04);

    os08a10_write_register (IspDev,0x3405, 0x00);
    os08a10_write_register (IspDev,0x3407, 0x03);
    os08a10_write_register (IspDev,0x3414, 0x0a);
    os08a10_write_register (IspDev,0x3418, 0xb2);
    os08a10_write_register (IspDev,0x3419, 0x08);
    os08a10_write_register (IspDev,0x3441, 0x0a);
    os08a10_write_register (IspDev,0x3442, 0x0a);
    os08a10_write_register (IspDev,0x3443, 0x03);
    os08a10_write_register (IspDev,0x3444, 0x20);
    os08a10_write_register (IspDev,0x3445, 0x25);
    os08a10_write_register (IspDev,0x3446, 0x77);
    os08a10_write_register (IspDev,0x3447, 0x00);
    os08a10_write_register (IspDev,0x3448, 0x67);
    os08a10_write_register (IspDev,0x3449, 0x00);
    os08a10_write_register (IspDev,0x344a, 0x47);
    os08a10_write_register (IspDev,0x344b, 0x00);
    os08a10_write_register (IspDev,0x344c, 0x37);
    os08a10_write_register (IspDev,0x344d, 0x00);
    os08a10_write_register (IspDev,0x344e, 0x3f);
    os08a10_write_register (IspDev,0x344f, 0x00);
    os08a10_write_register (IspDev,0x3450, 0xff);
    os08a10_write_register (IspDev,0x3451, 0x00);
    os08a10_write_register (IspDev,0x3452, 0x3f);
    os08a10_write_register (IspDev,0x3453, 0x00);
    os08a10_write_register (IspDev,0x3454, 0x37);
    os08a10_write_register (IspDev,0x3455, 0x00);
    os08a10_write_register (IspDev,0x3472, 0x9c);
    os08a10_write_register (IspDev,0x3473, 0x07);
    os08a10_write_register (IspDev,0x3480, 0x49);

    os08a10_write_register (IspDev,0x3000, 0x00); /* standby */
    delay_ms(20);
    os08a10_write_register (IspDev,0x3002, 0x00); /* master mode start */

    printf("===os08a10 os08a10 1080P60fps 10bit 2to1 WDR(120fps->60fps) init success!=====\n");
    return;
}
#endif

#if 0
void os08a10_wdr_1080p120_3to1_init(ISP_DEV IspDev)
{
    os08a10_write_register (IspDev,0x3000, 0x01); /* standby */
    os08a10_write_register (IspDev,0x3002, 0x01); /* XTMSTA */

    os08a10_write_register (IspDev,0x3005, 0x00);
    os08a10_write_register (IspDev,0x3007, 0x00);
    os08a10_write_register (IspDev,0x3009, 0x00);
    os08a10_write_register (IspDev,0x300a, 0x3c);
    os08a10_write_register (IspDev,0x300c, 0x21);
    os08a10_write_register (IspDev,0x300f, 0x00);
    os08a10_write_register (IspDev,0x3010, 0x21);
    os08a10_write_register (IspDev,0x3012, 0x64);
    os08a10_write_register (IspDev,0x3016, 0x09);
    os08a10_write_register (IspDev,0x3018, 0x65);
    os08a10_write_register (IspDev,0x3019, 0x04);
    os08a10_write_register (IspDev,0x301c, 0x4c);
    os08a10_write_register (IspDev,0x301d, 0x04);

#if 1
    os08a10_write_register (IspDev,0x3020, 0x04);  /* SHS1 */
    os08a10_write_register (IspDev,0x3021, 0x00);
    //os08a10_write_register (IspDev,0x3022, 0x00);
    os08a10_write_register (IspDev,0x3024, 0xf2);  /* SHS2 */
    os08a10_write_register (IspDev,0x3025, 0x01);
    //os08a10_write_register (IspDev,0x3025, 0x00);
    os08a10_write_register (IspDev,0x3028, 0x57);  /* SHS3 */
    os08a10_write_register (IspDev,0x3029, 0x02);
    //os08a10_write_register (IspDev,0x302A, 0x00);
    os08a10_write_register (IspDev,0x3030, 0xed);  /* RHS1 */
    os08a10_write_register (IspDev,0x3031, 0x01);
    os08a10_write_register (IspDev,0x3034, 0x30);  /* RHS2 */
    os08a10_write_register (IspDev,0x3035, 0x02);
#else
    os08a10_write_register (IspDev,0x3020, 0x04);
    os08a10_write_register (IspDev,0x3024, 0x89);
    os08a10_write_register (IspDev,0x3028, 0x93);
    os08a10_write_register (IspDev,0x3029, 0x01);
    os08a10_write_register (IspDev,0x3030, 0x85);
    os08a10_write_register (IspDev,0x3034, 0x92);
#endif

    os08a10_write_register (IspDev,0x3045, 0x05);
    os08a10_write_register (IspDev,0x3046, 0x00);
    os08a10_write_register (IspDev,0x304b, 0x0a);
    os08a10_write_register (IspDev,0x305c, 0x18);
    os08a10_write_register (IspDev,0x305d, 0x03);
    os08a10_write_register (IspDev,0x305e, 0x20);
    os08a10_write_register (IspDev,0x305f, 0x01);
    os08a10_write_register (IspDev,0x3070, 0x02);
    os08a10_write_register (IspDev,0x3071, 0x11);
    os08a10_write_register (IspDev,0x309b, 0x10);
    os08a10_write_register (IspDev,0x309c, 0x22);
    os08a10_write_register (IspDev,0x30a2, 0x02);
    os08a10_write_register (IspDev,0x30a6, 0x20);
    os08a10_write_register (IspDev,0x30a8, 0x20);
    os08a10_write_register (IspDev,0x30aa, 0x20);
    os08a10_write_register (IspDev,0x30ac, 0x20);
    os08a10_write_register (IspDev,0x30b0, 0x43);
    os08a10_write_register (IspDev,0x3106, 0x33);
    os08a10_write_register (IspDev,0x3119, 0x9e);
    os08a10_write_register (IspDev,0x311c, 0x1e);
    os08a10_write_register (IspDev,0x311e, 0x08);
    os08a10_write_register (IspDev,0x3128, 0x05);
    os08a10_write_register (IspDev,0x3129, 0x1d);
    os08a10_write_register (IspDev,0x313d, 0x83);
    os08a10_write_register (IspDev,0x3150, 0x03);
    os08a10_write_register (IspDev,0x315e, 0x1a);
    os08a10_write_register (IspDev,0x3164, 0x1a);
    os08a10_write_register (IspDev,0x317c, 0x12);
    os08a10_write_register (IspDev,0x317e, 0x00);
    os08a10_write_register (IspDev,0x31ec, 0x37);

    os08a10_write_register (IspDev,0x32b8, 0x50);
    os08a10_write_register (IspDev,0x32b9, 0x10);
    os08a10_write_register (IspDev,0x32ba, 0x00);
    os08a10_write_register (IspDev,0x32bb, 0x04);
    os08a10_write_register (IspDev,0x32c8, 0x50);
    os08a10_write_register (IspDev,0x32c9, 0x10);
    os08a10_write_register (IspDev,0x32ca, 0x00);
    os08a10_write_register (IspDev,0x32cb, 0x04);

    os08a10_write_register (IspDev,0x332c, 0xd3);
    os08a10_write_register (IspDev,0x332d, 0x10);
    os08a10_write_register (IspDev,0x332e, 0x0d);
    os08a10_write_register (IspDev,0x3358, 0x06);
    os08a10_write_register (IspDev,0x3359, 0xe1);
    os08a10_write_register (IspDev,0x335a, 0x11);
    os08a10_write_register (IspDev,0x3360, 0x1e);
    os08a10_write_register (IspDev,0x3361, 0x61);
    os08a10_write_register (IspDev,0x3362, 0x10);
    os08a10_write_register (IspDev,0x33b0, 0x50);
    os08a10_write_register (IspDev,0x33b2, 0x1a);
    os08a10_write_register (IspDev,0x33b3, 0x04);

    os08a10_write_register (IspDev,0x3405, 0x00);
    os08a10_write_register (IspDev,0x3407, 0x03);
    os08a10_write_register (IspDev,0x3414, 0x0a);
    os08a10_write_register (IspDev,0x3418, 0x55);
    os08a10_write_register (IspDev,0x3419, 0x11);
    os08a10_write_register (IspDev,0x3441, 0x0a);
    os08a10_write_register (IspDev,0x3442, 0x0a);
    os08a10_write_register (IspDev,0x3443, 0x03);
    os08a10_write_register (IspDev,0x3444, 0x20);
    os08a10_write_register (IspDev,0x3445, 0x25);
    os08a10_write_register (IspDev,0x3446, 0x77);
    os08a10_write_register (IspDev,0x3447, 0x00);
    os08a10_write_register (IspDev,0x3448, 0x67);
    os08a10_write_register (IspDev,0x3449, 0x00);
    os08a10_write_register (IspDev,0x344a, 0x47);
    os08a10_write_register (IspDev,0x344b, 0x00);
    os08a10_write_register (IspDev,0x344c, 0x37);
    os08a10_write_register (IspDev,0x344d, 0x00);
    os08a10_write_register (IspDev,0x344e, 0x3f);
    os08a10_write_register (IspDev,0x344f, 0x00);
    os08a10_write_register (IspDev,0x3450, 0xff);
    os08a10_write_register (IspDev,0x3451, 0x00);
    os08a10_write_register (IspDev,0x3452, 0x3f);
    os08a10_write_register (IspDev,0x3453, 0x00);
    os08a10_write_register (IspDev,0x3454, 0x37);
    os08a10_write_register (IspDev,0x3455, 0x00);
    os08a10_write_register (IspDev,0x3472, 0x9c);
    os08a10_write_register (IspDev,0x3473, 0x07);
    os08a10_write_register (IspDev,0x3480, 0x49);

    os08a10_write_register (IspDev,0x3000, 0x00); /* standby */
    delay_ms(20);
    os08a10_write_register (IspDev,0x3002, 0x00); /* master mode start */

    printf("===os08a10 os08a10 1080P30fps 10bit 3to1 WDR(120fps->30fps) init success!=====\n");

    return;
}

#endif

