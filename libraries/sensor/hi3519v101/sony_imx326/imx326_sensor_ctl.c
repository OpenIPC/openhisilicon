/******************************************************************************

  Copyright (C), 2001-2013, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : imx326_sensor_ctl.c
  Version       : Initial Draft
  Author        : Hisilicon BVT ISP group
  Created       : 2017/04/05
  Description   : Sony IMX326 sensor driver
  History       :
  1.Date        : 2017/04/05
  Author        : 
  Modification  : Created file

******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "hi_comm_video.h"
#include "hi_sns_ctrl.h"
#include "mpi_sys.h"

#ifdef HI_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "hi_i2c.h"
#endif

const unsigned char imx326_i2c_addr     =    0x34;        /* I2C Address of imx377 */
const unsigned int  imx326_addr_byte    =    2;
const unsigned int  imx326_data_byte    =    1;

static int g_fd[ISP_MAX_DEV_NUM] = {-1,-1};
extern ISP_SNS_STATE_S          g_astImx326[ISP_MAX_DEV_NUM];
extern ISP_SNS_COMMBUS_U        g_aunImx326BusInfo[];

#define IMX326_3072x1728_5M_30FPS_12BIT_LINEAR_MODE 1
#define IMX326_2592x1944_5M_30FPS_12BIT_LINEAR_MODE 2
#define IMX326_2560x1440_4M_30FPS_12BIT_LINEAR_MODE 3
#define IMX326_1920x1080_2M_30FPS_12BIT_LINEAR_MODE 4

void imx326_linear_3072x1728_5M30fps_init(ISP_DEV IspDev);
void imx326_linear_2592x1944_5M30fps_init(ISP_DEV IspDev);
void imx326_linear_2560x1440_4M30fps_init(ISP_DEV IspDev);
void imx326_linear_1920x1080_2M30fps_init(ISP_DEV IspDev);

int imx326_i2c_init(ISP_DEV IspDev)
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
    u8DevNum = g_aunImx326BusInfo[IspDev].s8I2cDev;
    snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile)-1, "/dev/i2c-%d", u8DevNum);
    
    g_fd[IspDev] = open(acDevFile, O_RDWR);
    if(g_fd[IspDev] < 0)
    {
        printf("Open /dev/i2c-%d error!\n", IspDev);
        return -1;
    }

    ret = ioctl(g_fd[IspDev], I2C_SLAVE_FORCE, (imx326_i2c_addr>>1));
    if (ret < 0)
    {
        printf("CMD_SET_DEV error!\n");
        return ret;
    }
#endif

    return 0;
}

int imx326_i2c_exit(ISP_DEV IspDev)
{
    if (g_fd[IspDev] >= 0)
    {
        close(g_fd[IspDev]);
        g_fd[IspDev] = -1;
        return 0;
    }
    return -1;
}

#ifdef __HuaweiLite__
int imx326_write_register(ISP_DEV IspDev,int addr, int data)
{
    if (0 > g_fd[IspDev])
    {
        return 0;
    }

#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = imx326_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = imx326_addr_byte;
    i2c_data.data = data;
    i2c_data.data_byte_num = imx326_data_byte;

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
    if (imx326_addr_byte == 2)
    {
        ret = ioctl(g_fd[IspDev], I2C_16BIT_REG, 1);
        buf[idx++] = (addr >> 8) & 0xff;
    }
    else
    {
    }

    buf[idx++] = data & 0xff;
    if (imx326_data_byte == 2)
    {
    }
    else
    {
        ret = ioctl(g_fd[IspDev], I2C_16BIT_DATA, 0);
    }

    ret = write(g_fd[IspDev], buf, imx326_addr_byte + imx326_data_byte);
    if(ret < 0)
    {
        printf("I2C_WRITE error!\n");
        return -1;
    }

#endif
    return 0;
}
#else
int imx326_write_register(ISP_DEV IspDev, int addr, int data)
{
    if (0 > g_fd[IspDev])
    {
        printf("return\n");
        return 0;
    }

#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = imx326_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = imx326_addr_byte;
    i2c_data.data = data;
    i2c_data.data_byte_num = imx326_data_byte;

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

    if (imx326_addr_byte == 2)
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

    if (imx326_data_byte == 2)
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
    
    ret = write(g_fd[IspDev], buf, imx326_addr_byte + imx326_data_byte);
    if(ret < 0)
    {
        printf("I2C_WRITE error!\n");
        return -1;
    }

#endif
    return 0;
}
#endif

int imx326_read_register(ISP_DEV IspDev,int addr)
{
    // TODO: 
    return 0;
}

static void delay_ms(int ms)
{
    hi_usleep(ms*1000);
}

void imx326_standby(ISP_DEV IspDev)
{
     // TODO:
    return;
}

void imx326_restart(ISP_DEV IspDev)
{
    // TODO:
    return;
}

void imx326_init(ISP_DEV IspDev)
{
    HI_U8   u8ImgMode;
    HI_U32  i;
    u8ImgMode = g_astImx326[IspDev].u8ImgMode;

    /* 2. imx326 spi init */
    imx326_i2c_init(IspDev);

    /* When sensor first init, config all registers */
    switch(u8ImgMode)
    {
        case IMX326_3072x1728_5M_30FPS_12BIT_LINEAR_MODE:
            imx326_linear_3072x1728_5M30fps_init(IspDev);
            break;
            
        case IMX326_2592x1944_5M_30FPS_12BIT_LINEAR_MODE:
            imx326_linear_2592x1944_5M30fps_init(IspDev);
            break;
            
        case IMX326_2560x1440_4M_30FPS_12BIT_LINEAR_MODE:
            imx326_linear_2560x1440_4M30fps_init(IspDev);
            break;

        case IMX326_1920x1080_2M_30FPS_12BIT_LINEAR_MODE:
            imx326_linear_1920x1080_2M30fps_init(IspDev);
            break;
            
        default:
            printf("Not support this mode\n");
            return;
            break;
    }

	for (i=0; i<g_astImx326[IspDev].astRegsInfo[0].u32RegNum; i++)
	{
		imx326_write_register(IspDev, g_astImx326[IspDev].astRegsInfo[0].astI2cData[i].u32RegAddr, g_astImx326[IspDev].astRegsInfo[0].astI2cData[i].u32Data);
	}
	g_astImx326[IspDev].bInit = HI_TRUE;
    return ;
}

void imx326_exit(ISP_DEV IspDev)
{
    imx326_i2c_exit(IspDev);
    return;
}

void imx326_linear_3072x1728_5M30fps_init(ISP_DEV IspDev)
{
    imx326_write_register(IspDev, 0x3000, 0x12);
    imx326_write_register(IspDev, 0x3120, 0xF0);
    imx326_write_register(IspDev, 0x3121, 0x00);
    imx326_write_register(IspDev, 0x3122, 0x02);
    imx326_write_register(IspDev, 0x3123, 0x01);
    imx326_write_register(IspDev, 0x3129, 0x9C);
    imx326_write_register(IspDev, 0x312A, 0x02);
    imx326_write_register(IspDev, 0x312D, 0x02);
    imx326_write_register(IspDev, 0x3AC4, 0x01);
    imx326_write_register(IspDev, 0x310B, 0x00);
    imx326_write_register(IspDev, 0x304C, 0x00);
    imx326_write_register(IspDev, 0x304D, 0x03);
    imx326_write_register(IspDev, 0x331C, 0x1A);
    imx326_write_register(IspDev, 0x331D, 0x00);
    imx326_write_register(IspDev, 0x3502, 0x02);
    imx326_write_register(IspDev, 0x3529, 0x0E);
    imx326_write_register(IspDev, 0x352A, 0x0E);
    imx326_write_register(IspDev, 0x352B, 0x0E);
    imx326_write_register(IspDev, 0x3538, 0x0E);
    imx326_write_register(IspDev, 0x3539, 0x0E);
    imx326_write_register(IspDev, 0x3553, 0x00);
    imx326_write_register(IspDev, 0x357D, 0x05);
    imx326_write_register(IspDev, 0x357F, 0x05);
    imx326_write_register(IspDev, 0x3581, 0x04);
    imx326_write_register(IspDev, 0x3583, 0x76);
    imx326_write_register(IspDev, 0x3587, 0x01);
    imx326_write_register(IspDev, 0x35BB, 0x0E);
    imx326_write_register(IspDev, 0x35BC, 0x0E);
    imx326_write_register(IspDev, 0x35BD, 0x0E);
    imx326_write_register(IspDev, 0x35BE, 0x0E);
    imx326_write_register(IspDev, 0x35BF, 0x0E);
    imx326_write_register(IspDev, 0x366E, 0x00);
    imx326_write_register(IspDev, 0x366F, 0x00);
    imx326_write_register(IspDev, 0x3670, 0x00);
    imx326_write_register(IspDev, 0x3671, 0x00);
    imx326_write_register(IspDev, 0x30EE, 0x01);
    imx326_write_register(IspDev, 0x3304, 0x32);
    imx326_write_register(IspDev, 0x3305, 0x00);
    imx326_write_register(IspDev, 0x3306, 0x32);
    imx326_write_register(IspDev, 0x3307, 0x00);
    imx326_write_register(IspDev, 0x3590, 0x32);
    imx326_write_register(IspDev, 0x3591, 0x00);
    imx326_write_register(IspDev, 0x3686, 0x32);
    imx326_write_register(IspDev, 0x3687, 0x00);
    imx326_write_register(IspDev, 0x3134, 0x77);
    imx326_write_register(IspDev, 0x3135, 0x00);
    imx326_write_register(IspDev, 0x3136, 0x67);
    imx326_write_register(IspDev, 0x3137, 0x00);
    imx326_write_register(IspDev, 0x3138, 0x37);
    imx326_write_register(IspDev, 0x3139, 0x00);
    imx326_write_register(IspDev, 0x313A, 0x37);
    imx326_write_register(IspDev, 0x313B, 0x00);
    imx326_write_register(IspDev, 0x313C, 0x37);
    imx326_write_register(IspDev, 0x313D, 0x00);
    imx326_write_register(IspDev, 0x313E, 0xDF);
    imx326_write_register(IspDev, 0x313F, 0x00);
    imx326_write_register(IspDev, 0x3140, 0x37);
    imx326_write_register(IspDev, 0x3141, 0x00);
    imx326_write_register(IspDev, 0x3142, 0x2F);
    imx326_write_register(IspDev, 0x3143, 0x00);
    imx326_write_register(IspDev, 0x3144, 0x0F);
    imx326_write_register(IspDev, 0x3145, 0x00);
    imx326_write_register(IspDev, 0x3A86, 0x47);
    imx326_write_register(IspDev, 0x3A87, 0x00);

    //Set the required shutter and gain registers
    imx326_write_register(IspDev, 0x3004, 0x00);
    imx326_write_register(IspDev, 0x3005, 0x07);
    imx326_write_register(IspDev, 0x3006, 0x00);
    imx326_write_register(IspDev, 0x3007, 0xA2);
    imx326_write_register(IspDev, 0x3037, 0x01);
    imx326_write_register(IspDev, 0x3038, 0x80);
    imx326_write_register(IspDev, 0x3039, 0x01);
    imx326_write_register(IspDev, 0x303A, 0x98);
    imx326_write_register(IspDev, 0x303B, 0x0D);
    imx326_write_register(IspDev, 0x306B, 0x07);
    imx326_write_register(IspDev, 0x30DD, 0x01);
    imx326_write_register(IspDev, 0x30DE, 0xD8);
    imx326_write_register(IspDev, 0x30DF, 0x00);
    imx326_write_register(IspDev, 0x30E0, 0x06);
    imx326_write_register(IspDev, 0x30E1, 0x00);
    imx326_write_register(IspDev, 0x30E2, 0x00);
    imx326_write_register(IspDev, 0x30EE, 0x01);
    imx326_write_register(IspDev, 0x30F6, 0xF4);    //HMAX_Low
    imx326_write_register(IspDev, 0x30F7, 0x01);    //HMAX_High
    imx326_write_register(IspDev, 0x30F8, 0xC0);    //VMAX_Low
    imx326_write_register(IspDev, 0x30F9, 0x12);    //VMAX_High
    imx326_write_register(IspDev, 0x30FA, 0x00);
    imx326_write_register(IspDev, 0x3130, 0xFA);
    imx326_write_register(IspDev, 0x3131, 0x06);
    imx326_write_register(IspDev, 0x3132, 0xEA);
    imx326_write_register(IspDev, 0x3133, 0x06);
    imx326_write_register(IspDev, 0x3342, 0xFF);
    imx326_write_register(IspDev, 0x3343, 0x01);
    imx326_write_register(IspDev, 0x3344, 0xFF);
    imx326_write_register(IspDev, 0x3345, 0x01);
    imx326_write_register(IspDev, 0x33A6, 0x01);
    imx326_write_register(IspDev, 0x3528, 0x0F);
    imx326_write_register(IspDev, 0x3554, 0x00);
    imx326_write_register(IspDev, 0x3555, 0x00);
    imx326_write_register(IspDev, 0x3556, 0x00);
    imx326_write_register(IspDev, 0x3557, 0x00);
    imx326_write_register(IspDev, 0x3558, 0x00);
    imx326_write_register(IspDev, 0x3559, 0x1F);
    imx326_write_register(IspDev, 0x355A, 0x1F);
    imx326_write_register(IspDev, 0x35BA, 0x0F);
    imx326_write_register(IspDev, 0x366A, 0x00);
    imx326_write_register(IspDev, 0x366B, 0x00);
    imx326_write_register(IspDev, 0x366C, 0x00);
    imx326_write_register(IspDev, 0x366D, 0x00);
    imx326_write_register(IspDev, 0x3A41, 0x10);
    delay_ms(10);
    imx326_write_register(IspDev, 0x3000, 0x00);
    imx326_write_register(IspDev, 0x303E, 0x02);
    delay_ms(7);
    imx326_write_register(IspDev, 0x30F4, 0x00);
    imx326_write_register(IspDev, 0x3018, 0xA2);
    printf("===imx326 linear 3072x1728 5M30fps Init OK!===\n");
    return;
}

void imx326_linear_2592x1944_5M30fps_init(ISP_DEV IspDev)
{
    imx326_write_register(IspDev, 0x3000, 0x12);
    imx326_write_register(IspDev, 0x3120, 0xF0);
    imx326_write_register(IspDev, 0x3121, 0x00);
    imx326_write_register(IspDev, 0x3122, 0x02);
    imx326_write_register(IspDev, 0x3123, 0x01);
    imx326_write_register(IspDev, 0x3129, 0x9C);
    imx326_write_register(IspDev, 0x312A, 0x02);
    imx326_write_register(IspDev, 0x312D, 0x02);
    imx326_write_register(IspDev, 0x3AC4, 0x01);
    imx326_write_register(IspDev, 0x310B, 0x00);
    imx326_write_register(IspDev, 0x304C, 0x00);
    imx326_write_register(IspDev, 0x304D, 0x03);
    imx326_write_register(IspDev, 0x331C, 0x1A);
    imx326_write_register(IspDev, 0x331D, 0x00);
    imx326_write_register(IspDev, 0x3502, 0x02);
    imx326_write_register(IspDev, 0x3529, 0x0E);
    imx326_write_register(IspDev, 0x352A, 0x0E);
    imx326_write_register(IspDev, 0x352B, 0x0E);
    imx326_write_register(IspDev, 0x3538, 0x0E);
    imx326_write_register(IspDev, 0x3539, 0x0E);
    imx326_write_register(IspDev, 0x3553, 0x00);
    imx326_write_register(IspDev, 0x357D, 0x05);
    imx326_write_register(IspDev, 0x357F, 0x05);
    imx326_write_register(IspDev, 0x3581, 0x04);
    imx326_write_register(IspDev, 0x3583, 0x76);
    imx326_write_register(IspDev, 0x3587, 0x01);
    imx326_write_register(IspDev, 0x35BB, 0x0E);
    imx326_write_register(IspDev, 0x35BC, 0x0E);
    imx326_write_register(IspDev, 0x35BD, 0x0E);
    imx326_write_register(IspDev, 0x35BE, 0x0E);
    imx326_write_register(IspDev, 0x35BF, 0x0E);
    imx326_write_register(IspDev, 0x366E, 0x00);
    imx326_write_register(IspDev, 0x366F, 0x00);
    imx326_write_register(IspDev, 0x3670, 0x00);
    imx326_write_register(IspDev, 0x3671, 0x00);
    imx326_write_register(IspDev, 0x30EE, 0x01);
    imx326_write_register(IspDev, 0x3304, 0x32);
    imx326_write_register(IspDev, 0x3305, 0x00);
    imx326_write_register(IspDev, 0x3306, 0x32);
    imx326_write_register(IspDev, 0x3307, 0x00);
    imx326_write_register(IspDev, 0x3590, 0x32);
    imx326_write_register(IspDev, 0x3591, 0x00);
    imx326_write_register(IspDev, 0x3686, 0x32);
    imx326_write_register(IspDev, 0x3687, 0x00);
    imx326_write_register(IspDev, 0x3134, 0x77);
    imx326_write_register(IspDev, 0x3135, 0x00);
    imx326_write_register(IspDev, 0x3136, 0x67);
    imx326_write_register(IspDev, 0x3137, 0x00);
    imx326_write_register(IspDev, 0x3138, 0x37);
    imx326_write_register(IspDev, 0x3139, 0x00);
    imx326_write_register(IspDev, 0x313A, 0x37);
    imx326_write_register(IspDev, 0x313B, 0x00);
    imx326_write_register(IspDev, 0x313C, 0x37);
    imx326_write_register(IspDev, 0x313D, 0x00);
    imx326_write_register(IspDev, 0x313E, 0xDF);
    imx326_write_register(IspDev, 0x313F, 0x00);
    imx326_write_register(IspDev, 0x3140, 0x37);
    imx326_write_register(IspDev, 0x3141, 0x00);
    imx326_write_register(IspDev, 0x3142, 0x2F);
    imx326_write_register(IspDev, 0x3143, 0x00);
    imx326_write_register(IspDev, 0x3144, 0x0F);
    imx326_write_register(IspDev, 0x3145, 0x00);
    imx326_write_register(IspDev, 0x3A86, 0x47);
    imx326_write_register(IspDev, 0x3A87, 0x00);

    //Set the required shutter and gain registers
    imx326_write_register(IspDev, 0x3004, 0x00);
    imx326_write_register(IspDev, 0x3005, 0x07);
    imx326_write_register(IspDev, 0x3006, 0x00);
    imx326_write_register(IspDev, 0x3007, 0xA2);
    imx326_write_register(IspDev, 0x3037, 0x01);
    imx326_write_register(IspDev, 0x3038, 0x70);
    imx326_write_register(IspDev, 0x3039, 0x02);
    imx326_write_register(IspDev, 0x303A, 0xA8);
    imx326_write_register(IspDev, 0x303B, 0x0C);
    imx326_write_register(IspDev, 0x306B, 0x07);
    imx326_write_register(IspDev, 0x30DD, 0x01);
    imx326_write_register(IspDev, 0x30DE, 0x6C);
    imx326_write_register(IspDev, 0x30DF, 0x00);
    imx326_write_register(IspDev, 0x30E0, 0x36);
    imx326_write_register(IspDev, 0x30E1, 0x00);
    imx326_write_register(IspDev, 0x30E2, 0x00);
    imx326_write_register(IspDev, 0x30EE, 0x01);
    imx326_write_register(IspDev, 0x30F6, 0xF4);	//HMAX_Low
    imx326_write_register(IspDev, 0x30F7, 0x01);	//HMAX_High
    imx326_write_register(IspDev, 0x30F8, 0xC0);	//VMAX_Low
    imx326_write_register(IspDev, 0x30F9, 0x12);	//VMAX_High
    imx326_write_register(IspDev, 0x30FA, 0x00);
    imx326_write_register(IspDev, 0x3130, 0xD2);
    imx326_write_register(IspDev, 0x3131, 0x07);
    imx326_write_register(IspDev, 0x3132, 0xC2);
    imx326_write_register(IspDev, 0x3133, 0x07);
    imx326_write_register(IspDev, 0x3342, 0xFF);
    imx326_write_register(IspDev, 0x3343, 0x01);
    imx326_write_register(IspDev, 0x3344, 0xFF);
    imx326_write_register(IspDev, 0x3345, 0x01);
    imx326_write_register(IspDev, 0x33A6, 0x01);
    imx326_write_register(IspDev, 0x3528, 0x0F);
    imx326_write_register(IspDev, 0x3554, 0x00);
    imx326_write_register(IspDev, 0x3555, 0x00);
    imx326_write_register(IspDev, 0x3556, 0x00);
    imx326_write_register(IspDev, 0x3557, 0x00);
    imx326_write_register(IspDev, 0x3558, 0x00);
    imx326_write_register(IspDev, 0x3559, 0x1F);
    imx326_write_register(IspDev, 0x355A, 0x1F);
    imx326_write_register(IspDev, 0x35BA, 0x0F);
    imx326_write_register(IspDev, 0x366A, 0x00);
    imx326_write_register(IspDev, 0x366B, 0x00);
    imx326_write_register(IspDev, 0x366C, 0x00);
    imx326_write_register(IspDev, 0x366D, 0x00);
    imx326_write_register(IspDev, 0x3A41, 0x10);
    delay_ms(10);
    imx326_write_register(IspDev, 0x3000, 0x00);
    imx326_write_register(IspDev, 0x303E, 0x02);
    delay_ms(7);
    imx326_write_register(IspDev, 0x30F4, 0x00);
    imx326_write_register(IspDev, 0x3018, 0xA2);
    printf("===imx326 linear 2592x1944 5M30fps Init OK!===\n");
}

void imx326_linear_2560x1440_4M30fps_init(ISP_DEV IspDev)
{
    imx326_write_register(IspDev, 0x3000, 0x12);
    imx326_write_register(IspDev, 0x3120, 0xF0);
    imx326_write_register(IspDev, 0x3121, 0x00);
    imx326_write_register(IspDev, 0x3122, 0x02);
    imx326_write_register(IspDev, 0x3123, 0x01);
    imx326_write_register(IspDev, 0x3129, 0x9C);
    imx326_write_register(IspDev, 0x312A, 0x02);
    imx326_write_register(IspDev, 0x312D, 0x02);
    imx326_write_register(IspDev, 0x3AC4, 0x01);
    imx326_write_register(IspDev, 0x310B, 0x00);
    imx326_write_register(IspDev, 0x304C, 0x00);
    imx326_write_register(IspDev, 0x304D, 0x03);
    imx326_write_register(IspDev, 0x331C, 0x1A);
    imx326_write_register(IspDev, 0x331D, 0x00);
    imx326_write_register(IspDev, 0x3502, 0x02);
    imx326_write_register(IspDev, 0x3529, 0x0E);
    imx326_write_register(IspDev, 0x352A, 0x0E);
    imx326_write_register(IspDev, 0x352B, 0x0E);
    imx326_write_register(IspDev, 0x3538, 0x0E);
    imx326_write_register(IspDev, 0x3539, 0x0E);
    imx326_write_register(IspDev, 0x3553, 0x00);
    imx326_write_register(IspDev, 0x357D, 0x05);
    imx326_write_register(IspDev, 0x357F, 0x05);
    imx326_write_register(IspDev, 0x3581, 0x04);
    imx326_write_register(IspDev, 0x3583, 0x76);
    imx326_write_register(IspDev, 0x3587, 0x01);
    imx326_write_register(IspDev, 0x35BB, 0x0E);
    imx326_write_register(IspDev, 0x35BC, 0x0E);
    imx326_write_register(IspDev, 0x35BD, 0x0E);
    imx326_write_register(IspDev, 0x35BE, 0x0E);
    imx326_write_register(IspDev, 0x35BF, 0x0E);
    imx326_write_register(IspDev, 0x366E, 0x00);
    imx326_write_register(IspDev, 0x366F, 0x00);
    imx326_write_register(IspDev, 0x3670, 0x00);
    imx326_write_register(IspDev, 0x3671, 0x00);
    imx326_write_register(IspDev, 0x30EE, 0x01);
    imx326_write_register(IspDev, 0x3304, 0x32);
    imx326_write_register(IspDev, 0x3305, 0x00);
    imx326_write_register(IspDev, 0x3306, 0x32);
    imx326_write_register(IspDev, 0x3307, 0x00);
    imx326_write_register(IspDev, 0x3590, 0x32);
    imx326_write_register(IspDev, 0x3591, 0x00);
    imx326_write_register(IspDev, 0x3686, 0x32);
    imx326_write_register(IspDev, 0x3687, 0x00);
    imx326_write_register(IspDev, 0x3134, 0x77);
    imx326_write_register(IspDev, 0x3135, 0x00);
    imx326_write_register(IspDev, 0x3136, 0x67);
    imx326_write_register(IspDev, 0x3137, 0x00);
    imx326_write_register(IspDev, 0x3138, 0x37);
    imx326_write_register(IspDev, 0x3139, 0x00);
    imx326_write_register(IspDev, 0x313A, 0x37);
    imx326_write_register(IspDev, 0x313B, 0x00);
    imx326_write_register(IspDev, 0x313C, 0x37);
    imx326_write_register(IspDev, 0x313D, 0x00);
    imx326_write_register(IspDev, 0x313E, 0xDF);
    imx326_write_register(IspDev, 0x313F, 0x00);
    imx326_write_register(IspDev, 0x3140, 0x37);
    imx326_write_register(IspDev, 0x3141, 0x00);
    imx326_write_register(IspDev, 0x3142, 0x2F);
    imx326_write_register(IspDev, 0x3143, 0x00);
    imx326_write_register(IspDev, 0x3144, 0x0F);
    imx326_write_register(IspDev, 0x3145, 0x00);
    imx326_write_register(IspDev, 0x3A86, 0x47);
    imx326_write_register(IspDev, 0x3A87, 0x00);

    //Set the required shutter and gain registers
    imx326_write_register(IspDev, 0x3004, 0x00);
    imx326_write_register(IspDev, 0x3005, 0x07);
    imx326_write_register(IspDev, 0x3006, 0x00);
    imx326_write_register(IspDev, 0x3007, 0xA2);
    imx326_write_register(IspDev, 0x3037, 0x01);
    imx326_write_register(IspDev, 0x3038, 0x7C);
    imx326_write_register(IspDev, 0x3039, 0x02);
    imx326_write_register(IspDev, 0x303A, 0x9C);
    imx326_write_register(IspDev, 0x303B, 0x0C);
    imx326_write_register(IspDev, 0x306B, 0x07);
    imx326_write_register(IspDev, 0x30DD, 0x01);
    imx326_write_register(IspDev, 0x30DE, 0x68);
    imx326_write_register(IspDev, 0x30DF, 0x01);
    imx326_write_register(IspDev, 0x30E0, 0xB4);
    imx326_write_register(IspDev, 0x30E1, 0x00);
    imx326_write_register(IspDev, 0x30E2, 0x00);
    imx326_write_register(IspDev, 0x30EE, 0x01);
    imx326_write_register(IspDev, 0x30F6, 0xF4);	//HMAX_Low
    imx326_write_register(IspDev, 0x30F7, 0x01);	//HMAX_High
    imx326_write_register(IspDev, 0x30F8, 0xC0);	//VMAX_Low
    imx326_write_register(IspDev, 0x30F9, 0x12);	//VMAX_High
    imx326_write_register(IspDev, 0x30FA, 0x00);
    imx326_write_register(IspDev, 0x3130, 0xDA);
    imx326_write_register(IspDev, 0x3131, 0x05);
    imx326_write_register(IspDev, 0x3132, 0xCA);
    imx326_write_register(IspDev, 0x3133, 0x05);
    imx326_write_register(IspDev, 0x3342, 0xFF);
    imx326_write_register(IspDev, 0x3343, 0x01);
    imx326_write_register(IspDev, 0x3344, 0xFF);
    imx326_write_register(IspDev, 0x3345, 0x01);
    imx326_write_register(IspDev, 0x33A6, 0x01);
    imx326_write_register(IspDev, 0x3528, 0x0F);
    imx326_write_register(IspDev, 0x3554, 0x00);
    imx326_write_register(IspDev, 0x3555, 0x00);
    imx326_write_register(IspDev, 0x3556, 0x00);
    imx326_write_register(IspDev, 0x3557, 0x00);
    imx326_write_register(IspDev, 0x3558, 0x00);
    imx326_write_register(IspDev, 0x3559, 0x1F);
    imx326_write_register(IspDev, 0x355A, 0x1F);
    imx326_write_register(IspDev, 0x35BA, 0x0F);
    imx326_write_register(IspDev, 0x366A, 0x00);
    imx326_write_register(IspDev, 0x366B, 0x00);
    imx326_write_register(IspDev, 0x366C, 0x00);
    imx326_write_register(IspDev, 0x366D, 0x00);
    imx326_write_register(IspDev, 0x3A41, 0x10);
    delay_ms(10);
    imx326_write_register(IspDev, 0x3000, 0x00);
    imx326_write_register(IspDev, 0x303E, 0x02);
    delay_ms(7);
    imx326_write_register(IspDev, 0x30F4, 0x00);
    imx326_write_register(IspDev, 0x3018, 0xA2);
    printf("===imx326 linear 2560x1440 4M30fps Init OK!===\n");
}

void imx326_linear_1920x1080_2M30fps_init(ISP_DEV IspDev)
{
    imx326_write_register(IspDev, 0x3000, 0x12);
    imx326_write_register(IspDev, 0x3120, 0xF0);
    imx326_write_register(IspDev, 0x3121, 0x00);
    imx326_write_register(IspDev, 0x3122, 0x02);
    imx326_write_register(IspDev, 0x3123, 0x01);
    imx326_write_register(IspDev, 0x3129, 0x9C);
    imx326_write_register(IspDev, 0x312A, 0x02);
    imx326_write_register(IspDev, 0x312D, 0x02);
    imx326_write_register(IspDev, 0x3AC4, 0x01);
    imx326_write_register(IspDev, 0x310B, 0x00);
    imx326_write_register(IspDev, 0x304C, 0x00);
    imx326_write_register(IspDev, 0x304D, 0x03);
    imx326_write_register(IspDev, 0x331C, 0x1A);
    imx326_write_register(IspDev, 0x331D, 0x00);
    imx326_write_register(IspDev, 0x3502, 0x02);
    imx326_write_register(IspDev, 0x3529, 0x0E);
    imx326_write_register(IspDev, 0x352A, 0x0E);
    imx326_write_register(IspDev, 0x352B, 0x0E);
    imx326_write_register(IspDev, 0x3538, 0x0E);
    imx326_write_register(IspDev, 0x3539, 0x0E);
    imx326_write_register(IspDev, 0x3553, 0x00);
    imx326_write_register(IspDev, 0x357D, 0x05);
    imx326_write_register(IspDev, 0x357F, 0x05);
    imx326_write_register(IspDev, 0x3581, 0x04);
    imx326_write_register(IspDev, 0x3583, 0x76);
    imx326_write_register(IspDev, 0x3587, 0x01);
    imx326_write_register(IspDev, 0x35BB, 0x0E);
    imx326_write_register(IspDev, 0x35BC, 0x0E);
    imx326_write_register(IspDev, 0x35BD, 0x0E);
    imx326_write_register(IspDev, 0x35BE, 0x0E);
    imx326_write_register(IspDev, 0x35BF, 0x0E);
    imx326_write_register(IspDev, 0x366E, 0x00);
    imx326_write_register(IspDev, 0x366F, 0x00);
    imx326_write_register(IspDev, 0x3670, 0x00);
    imx326_write_register(IspDev, 0x3671, 0x00);
    imx326_write_register(IspDev, 0x30EE, 0x01);
    imx326_write_register(IspDev, 0x3304, 0x32);
    imx326_write_register(IspDev, 0x3305, 0x00);
    imx326_write_register(IspDev, 0x3306, 0x32);
    imx326_write_register(IspDev, 0x3307, 0x00);
    imx326_write_register(IspDev, 0x3590, 0x32);
    imx326_write_register(IspDev, 0x3591, 0x00);
    imx326_write_register(IspDev, 0x3686, 0x32);
    imx326_write_register(IspDev, 0x3687, 0x00);
    imx326_write_register(IspDev, 0x3134, 0x77);
    imx326_write_register(IspDev, 0x3135, 0x00);
    imx326_write_register(IspDev, 0x3136, 0x67);
    imx326_write_register(IspDev, 0x3137, 0x00);
    imx326_write_register(IspDev, 0x3138, 0x37);
    imx326_write_register(IspDev, 0x3139, 0x00);
    imx326_write_register(IspDev, 0x313A, 0x37);
    imx326_write_register(IspDev, 0x313B, 0x00);
    imx326_write_register(IspDev, 0x313C, 0x37);
    imx326_write_register(IspDev, 0x313D, 0x00);
    imx326_write_register(IspDev, 0x313E, 0xDF);
    imx326_write_register(IspDev, 0x313F, 0x00);
    imx326_write_register(IspDev, 0x3140, 0x37);
    imx326_write_register(IspDev, 0x3141, 0x00);
    imx326_write_register(IspDev, 0x3142, 0x2F);
    imx326_write_register(IspDev, 0x3143, 0x00);
    imx326_write_register(IspDev, 0x3144, 0x0F);
    imx326_write_register(IspDev, 0x3145, 0x00);
    imx326_write_register(IspDev, 0x3A86, 0x47);
    imx326_write_register(IspDev, 0x3A87, 0x00);

    //Set the required shutter and gain registers
    imx326_write_register(IspDev, 0x3004, 0x00);
    imx326_write_register(IspDev, 0x3005, 0x07);
    imx326_write_register(IspDev, 0x3006, 0x00);
    imx326_write_register(IspDev, 0x3007, 0xA2);
    imx326_write_register(IspDev, 0x3037, 0x01);
    imx326_write_register(IspDev, 0x3038, 0xC0);
    imx326_write_register(IspDev, 0x3039, 0x03);
    imx326_write_register(IspDev, 0x303A, 0x58);
    imx326_write_register(IspDev, 0x303B, 0x0B);
    imx326_write_register(IspDev, 0x306B, 0x07);
    imx326_write_register(IspDev, 0x30DD, 0x01);
    imx326_write_register(IspDev, 0x30DE, 0x1C);
    imx326_write_register(IspDev, 0x30DF, 0x02);
    imx326_write_register(IspDev, 0x30E0, 0x0E);
    imx326_write_register(IspDev, 0x30E1, 0x01);
    imx326_write_register(IspDev, 0x30E2, 0x00);
    imx326_write_register(IspDev, 0x30EE, 0x01);
    imx326_write_register(IspDev, 0x30F6, 0xF4);	//HMAX_Low
    imx326_write_register(IspDev, 0x30F7, 0x01);	//HMAX_High
    imx326_write_register(IspDev, 0x30F8, 0xC0);	//VMAX_Low
    imx326_write_register(IspDev, 0x30F9, 0x12);	//VMAX_High
    imx326_write_register(IspDev, 0x30FA, 0x00);
    imx326_write_register(IspDev, 0x3130, 0x72);
    imx326_write_register(IspDev, 0x3131, 0x04);
    imx326_write_register(IspDev, 0x3132, 0x62);
    imx326_write_register(IspDev, 0x3133, 0x04);
    imx326_write_register(IspDev, 0x3342, 0xFF);
    imx326_write_register(IspDev, 0x3343, 0x01);
    imx326_write_register(IspDev, 0x3344, 0xFF);
    imx326_write_register(IspDev, 0x3345, 0x01);
    imx326_write_register(IspDev, 0x33A6, 0x01);
    imx326_write_register(IspDev, 0x3528, 0x0F);
    imx326_write_register(IspDev, 0x3554, 0x00);
    imx326_write_register(IspDev, 0x3555, 0x00);
    imx326_write_register(IspDev, 0x3556, 0x00);
    imx326_write_register(IspDev, 0x3557, 0x00);
    imx326_write_register(IspDev, 0x3558, 0x00);
    imx326_write_register(IspDev, 0x3559, 0x1F);
    imx326_write_register(IspDev, 0x355A, 0x1F);
    imx326_write_register(IspDev, 0x35BA, 0x0F);
    imx326_write_register(IspDev, 0x366A, 0x00);
    imx326_write_register(IspDev, 0x366B, 0x00);
    imx326_write_register(IspDev, 0x366C, 0x00);
    imx326_write_register(IspDev, 0x366D, 0x00);
    imx326_write_register(IspDev, 0x3A41, 0x10);
    delay_ms(10);
    imx326_write_register(IspDev, 0x3000, 0x00);
    imx326_write_register(IspDev, 0x303E, 0x02);
    delay_ms(7);
    imx326_write_register(IspDev, 0x30F4, 0x00);
    imx326_write_register(IspDev, 0x3018, 0xA2);
    printf("===imx326 linear 1920x1080 2M30fps Init OK!===\n");
}