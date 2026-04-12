/******************************************************************************

  Copyright (C), 2001-2013, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : os05a_sensor_ctl.c
  Version       : Initial Draft
  Author        : Hisilicon BVT ISP group
  Created       : 2017/08/22
  Description   : Sony os05a sensor driver
  History       :
  1.Date        : 2017/08/22
  Author        : yy
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

const unsigned char os05a_i2c_addr     =    0x6c; /* I2C Address of OS05a */       //0x6c or 0x20
const unsigned int  os05a_addr_byte    =    2;
const unsigned int  os05a_data_byte    =    1;

static int g_fd[ISP_MAX_DEV_NUM] = {-1, -1};
extern ISP_SNS_STATE_S          g_astOs05a[ISP_MAX_DEV_NUM];
extern ISP_SNS_COMMBUS_U        g_aunOs05aBusInfo[];

#define Os05a_5M_2688x1944_12bit_linear30   (0)
#define Os05a_5M_2688x1944_10bit_wdr30      (1)

/* VI 297M */
void os05a_init_5M_2688x1944_10bit_wdr30(ISP_DEV IspDev);
/* VI 250M */
void os05a_init_5M_2688x1944_12bit_linear30(ISP_DEV IspDev);

int os05a_i2c_init(ISP_DEV IspDev)
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
    u8DevNum = g_aunOs05aBusInfo[IspDev].s8I2cDev;
    snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile)-1, "/dev/i2c-%d", u8DevNum);
    
    g_fd[IspDev] = open(acDevFile, O_RDWR);
    if(g_fd[IspDev] < 0)
    {
        printf("Open /dev/i2c-%d error!\n", IspDev);
        return -1;
    }

    ret = ioctl(g_fd[IspDev], I2C_SLAVE_FORCE, (os05a_i2c_addr>>1));
    if (ret < 0)
    {
        printf("CMD_SET_DEV error!\n");
        return ret;
    }
#endif

    return 0;
}

int os05a_i2c_exit(ISP_DEV IspDev)
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
int os05a_write_register(ISP_DEV IspDev,int addr, int data)
{
    if (0 > g_fd[IspDev])
    {
        return 0;
    }

#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = os05a_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = os05a_addr_byte;
    i2c_data.data = data;
    i2c_data.data_byte_num = os05a_data_byte;

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
    if (os05a_addr_byte == 2)
    {
        ret = ioctl(g_fd[IspDev], I2C_16BIT_REG, 1);
        buf[idx++] = (addr >> 8) & 0xff;
    }
    else
    {
    }

    buf[idx++] = data & 0xff;
    if (os05a_data_byte == 2)
    {
    }
    else
    {
        ret = ioctl(g_fd[IspDev], I2C_16BIT_DATA, 0);
    }

    ret = write(g_fd[IspDev], buf, os05a_addr_byte + os05a_data_byte);
    if(ret < 0)
    {
        printf("I2C_WRITE error!\n");
        return -1;
    }

#endif
    return 0;
}
#else
int os05a_write_register(ISP_DEV IspDev, int addr, int data)
{
    if (0 > g_fd[IspDev])
    {
        printf("return\n");
        return 0;
    }

#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = os05a_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = os05a_addr_byte;
    i2c_data.data = data;
    i2c_data.data_byte_num = os05a_data_byte;

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

    if (os05a_addr_byte == 2)
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

    if (os05a_data_byte == 2)
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
    
    ret = write(g_fd[IspDev], buf, os05a_addr_byte + os05a_data_byte);
    if(ret < 0)
    {
        printf("I2C_WRITE error!\n");
        return -1;
    }

#endif
    return 0;
}
#endif

int os05a_read_register(ISP_DEV IspDev,int addr)
{
    // TODO: 
    return 0;
}
/*
static void delay_ms(int ms)
{
    hi_usleep(ms*1000);
}
*/
void os05a_standby(ISP_DEV IspDev)
{
     // TODO:
    return;
}

void os05a_restart(ISP_DEV IspDev)
{
    // TODO:
    return;
}

void os05a_init(ISP_DEV IspDev)
{
    HI_U8   u8ImgMode;
    HI_U32  i;
    u8ImgMode = g_astOs05a[IspDev].u8ImgMode;

    /* 2. imx326 i2c init */
    os05a_i2c_init(IspDev);

    /* When sensor first init, config all registers */
    switch(u8ImgMode)
    {
        case Os05a_5M_2688x1944_12bit_linear30:
            os05a_init_5M_2688x1944_12bit_linear30(IspDev);
            g_astOs05a[IspDev].bInit = HI_TRUE;          
            break;

        case Os05a_5M_2688x1944_10bit_wdr30:
            os05a_init_5M_2688x1944_10bit_wdr30(IspDev);
            g_astOs05a[IspDev].bInit = HI_TRUE;
            break;
            
        default:
            printf("Not support this mode\n");
            break;
    }

	for (i=0; i<g_astOs05a[IspDev].astRegsInfo[0].u32RegNum; i++)
	{
		os05a_write_register(IspDev, g_astOs05a[IspDev].astRegsInfo[0].astI2cData[i].u32RegAddr, g_astOs05a[IspDev].astRegsInfo[0].astI2cData[i].u32Data);
	}
	g_astOs05a[IspDev].bInit = HI_TRUE;
    return ;
}

void os05a_exit(ISP_DEV IspDev)
{
    os05a_i2c_exit(IspDev);
    return;
}

void os05a_init_5M_2688x1944_10bit_wdr30(ISP_DEV IspDev)
{
    os05a_write_register(IspDev, 0x4600, 0x01);
    os05a_write_register(IspDev, 0x4601, 0x04);
    os05a_write_register(IspDev, 0x4603, 0x00);
    os05a_write_register(IspDev, 0x0103, 0x01);
    os05a_write_register(IspDev, 0x0303, 0x01);
    //os05a_write_register(IspDev, 0x0305, 0x2e);
    os05a_write_register(IspDev, 0x0305, 0x5a);
    os05a_write_register(IspDev, 0x0306, 0x00);
    os05a_write_register(IspDev, 0x0307, 0x00);
    os05a_write_register(IspDev, 0x0308, 0x03);
    os05a_write_register(IspDev, 0x0309, 0x04);
    os05a_write_register(IspDev, 0x032a, 0x00);
    os05a_write_register(IspDev, 0x031e, 0x09);
    os05a_write_register(IspDev, 0x0325, 0x48);
    os05a_write_register(IspDev, 0x0328, 0x07);
    os05a_write_register(IspDev, 0x300d, 0x11);
    os05a_write_register(IspDev, 0x300e, 0x11);
    os05a_write_register(IspDev, 0x300f, 0x11);
    os05a_write_register(IspDev, 0x3010, 0x01);
    os05a_write_register(IspDev, 0x3012, 0x41);
    os05a_write_register(IspDev, 0x3016, 0xf0);
    os05a_write_register(IspDev, 0x3018, 0xf0);
    os05a_write_register(IspDev, 0x3028, 0xf0);
    os05a_write_register(IspDev, 0x301e, 0x98);
    os05a_write_register(IspDev, 0x3010, 0x04);
    os05a_write_register(IspDev, 0x3011, 0x06);
    os05a_write_register(IspDev, 0x3031, 0xa9);
    os05a_write_register(IspDev, 0x3103, 0x48);
    os05a_write_register(IspDev, 0x3104, 0x01);
    os05a_write_register(IspDev, 0x3106, 0x10);
    os05a_write_register(IspDev, 0x3501, 0x09);
    //os05a_write_register(IspDev, 0x3502, 0x2c);
    os05a_write_register(IspDev, 0x3502, 0xa0);
    os05a_write_register(IspDev, 0x3505, 0x83);
    os05a_write_register(IspDev, 0x3508, 0x00);
    os05a_write_register(IspDev, 0x3509, 0x80);
    os05a_write_register(IspDev, 0x350a, 0x04);
    os05a_write_register(IspDev, 0x350b, 0x00);
    os05a_write_register(IspDev, 0x350c, 0x00);
    os05a_write_register(IspDev, 0x350d, 0x80);
    os05a_write_register(IspDev, 0x350e, 0x04);
    os05a_write_register(IspDev, 0x350f, 0x00);
    os05a_write_register(IspDev, 0x3600, 0x00);
    os05a_write_register(IspDev, 0x3626, 0xff);
    os05a_write_register(IspDev, 0x3605, 0x50);
    os05a_write_register(IspDev, 0x3609, 0xb5);
    os05a_write_register(IspDev, 0x3610, 0x69);
    os05a_write_register(IspDev, 0x360c, 0x01);
    os05a_write_register(IspDev, 0x3628, 0xa4);
    os05a_write_register(IspDev, 0x3629, 0x6a);
    os05a_write_register(IspDev, 0x362d, 0x10);
    os05a_write_register(IspDev, 0x3660, 0x42);
    os05a_write_register(IspDev, 0x3661, 0x07);
    os05a_write_register(IspDev, 0x3662, 0x00);
    os05a_write_register(IspDev, 0x3663, 0x28);
    os05a_write_register(IspDev, 0x3664, 0x0d);
    os05a_write_register(IspDev, 0x366a, 0x38);
    os05a_write_register(IspDev, 0x366b, 0xa0);
    os05a_write_register(IspDev, 0x366d, 0x00);
    os05a_write_register(IspDev, 0x366e, 0x00);
    os05a_write_register(IspDev, 0x3680, 0x00);
    os05a_write_register(IspDev, 0x3621, 0x81);
    os05a_write_register(IspDev, 0x3634, 0x31);
    os05a_write_register(IspDev, 0x3620, 0x00);
    os05a_write_register(IspDev, 0x3622, 0x00);
    os05a_write_register(IspDev, 0x362a, 0xd0);
    os05a_write_register(IspDev, 0x362e, 0x8c);
    os05a_write_register(IspDev, 0x362f, 0x98);
    os05a_write_register(IspDev, 0x3630, 0xb0);
    os05a_write_register(IspDev, 0x3631, 0xd7);
    os05a_write_register(IspDev, 0x3701, 0x0f);
    os05a_write_register(IspDev, 0x3737, 0x02);
    os05a_write_register(IspDev, 0x3741, 0x04);
    os05a_write_register(IspDev, 0x373c, 0x0f);
    os05a_write_register(IspDev, 0x373b, 0x02);
    os05a_write_register(IspDev, 0x3705, 0x00);
    os05a_write_register(IspDev, 0x3706, 0x50);
    os05a_write_register(IspDev, 0x370a, 0x00);
    os05a_write_register(IspDev, 0x370b, 0xe4);
    os05a_write_register(IspDev, 0x3709, 0x4a);
    os05a_write_register(IspDev, 0x3714, 0x21);
    os05a_write_register(IspDev, 0x371c, 0x00);
    os05a_write_register(IspDev, 0x371d, 0x08);
    os05a_write_register(IspDev, 0x375e, 0x0b);
    os05a_write_register(IspDev, 0x3776, 0x10);
    os05a_write_register(IspDev, 0x3781, 0x02);
    os05a_write_register(IspDev, 0x3782, 0x04);
    os05a_write_register(IspDev, 0x3783, 0x02);
    os05a_write_register(IspDev, 0x3784, 0x08);
    os05a_write_register(IspDev, 0x3785, 0x08);
    os05a_write_register(IspDev, 0x3788, 0x01);
    os05a_write_register(IspDev, 0x3789, 0x01);
    os05a_write_register(IspDev, 0x3797, 0x04);
    os05a_write_register(IspDev, 0x3761, 0x02);
    os05a_write_register(IspDev, 0x3762, 0x0d);
    os05a_write_register(IspDev, 0x3800, 0x00);
    os05a_write_register(IspDev, 0x3801, 0x00);
    os05a_write_register(IspDev, 0x3802, 0x00);
    os05a_write_register(IspDev, 0x3803, 0x0c);
    os05a_write_register(IspDev, 0x3804, 0x0e);
    os05a_write_register(IspDev, 0x3805, 0xff);
    os05a_write_register(IspDev, 0x3806, 0x08);
    os05a_write_register(IspDev, 0x3807, 0x6f);
    os05a_write_register(IspDev, 0x3808, 0x0a);
    //os05a_write_register(IspDev, 0x3809, 0x20);
    os05a_write_register(IspDev, 0x3809, 0x80);
    os05a_write_register(IspDev, 0x380a, 0x07);
    os05a_write_register(IspDev, 0x380b, 0x98);
    //os05a_write_register(IspDev, 0x380c, 0x03);
    os05a_write_register(IspDev, 0x380c, 0x02);
    //os05a_write_register(IspDev, 0x380d, 0xf0);
    os05a_write_register(IspDev, 0x380d, 0xd0);
    os05a_write_register(IspDev, 0x380e, 0x09);
    //os05a_write_register(IspDev, 0x380f, 0x4c);
    os05a_write_register(IspDev, 0x380f, 0xc0);
    os05a_write_register(IspDev, 0x3813, 0x04);
    os05a_write_register(IspDev, 0x3814, 0x01);
    os05a_write_register(IspDev, 0x3815, 0x01);
    os05a_write_register(IspDev, 0x3816, 0x01);
    os05a_write_register(IspDev, 0x3817, 0x01);
    os05a_write_register(IspDev, 0x381c, 0x08);
    os05a_write_register(IspDev, 0x3820, 0x00);
    os05a_write_register(IspDev, 0x3821, 0x24);
    os05a_write_register(IspDev, 0x3823, 0x18);
    os05a_write_register(IspDev, 0x3826, 0x00);
    os05a_write_register(IspDev, 0x3827, 0x01);
    os05a_write_register(IspDev, 0x3832, 0x02);
    os05a_write_register(IspDev, 0x383c, 0x48);
    os05a_write_register(IspDev, 0x383d, 0xff);
    os05a_write_register(IspDev, 0x3843, 0x20);
    os05a_write_register(IspDev, 0x382d, 0x08);
    os05a_write_register(IspDev, 0x3d85, 0x0b);
    os05a_write_register(IspDev, 0x3d84, 0x40);
    os05a_write_register(IspDev, 0x3d8c, 0x63);
    os05a_write_register(IspDev, 0x3d8d, 0x00);
    os05a_write_register(IspDev, 0x4000, 0x78);
    os05a_write_register(IspDev, 0x4001, 0x2b);
    os05a_write_register(IspDev, 0x4005, 0x40);
    os05a_write_register(IspDev, 0x4028, 0x2f);
    os05a_write_register(IspDev, 0x400a, 0x01);
    os05a_write_register(IspDev, 0x4010, 0x12);
    os05a_write_register(IspDev, 0x4008, 0x02);
    os05a_write_register(IspDev, 0x4009, 0x0d);
    os05a_write_register(IspDev, 0x401a, 0x58);
    os05a_write_register(IspDev, 0x4050, 0x00);
    os05a_write_register(IspDev, 0x4051, 0x01);
    os05a_write_register(IspDev, 0x4052, 0x00);
    os05a_write_register(IspDev, 0x4053, 0x80);
    os05a_write_register(IspDev, 0x4054, 0x00);
    os05a_write_register(IspDev, 0x4055, 0x80);
    os05a_write_register(IspDev, 0x4056, 0x00);
    os05a_write_register(IspDev, 0x4057, 0x80);
    os05a_write_register(IspDev, 0x4058, 0x00);
    os05a_write_register(IspDev, 0x4059, 0x80);
    os05a_write_register(IspDev, 0x430b, 0xff);
    os05a_write_register(IspDev, 0x430c, 0xff);
    os05a_write_register(IspDev, 0x430d, 0x00);
    os05a_write_register(IspDev, 0x430e, 0x00);
    os05a_write_register(IspDev, 0x4501, 0x18);
    os05a_write_register(IspDev, 0x4502, 0x00);
    os05a_write_register(IspDev, 0x4643, 0x00);
    os05a_write_register(IspDev, 0x4640, 0x01);
    os05a_write_register(IspDev, 0x4641, 0x04);
    os05a_write_register(IspDev, 0x480e, 0x04);
    os05a_write_register(IspDev, 0x4813, 0x98);
    os05a_write_register(IspDev, 0x4815, 0x2b);
    os05a_write_register(IspDev, 0x486e, 0x36);
    os05a_write_register(IspDev, 0x486f, 0x84);
    os05a_write_register(IspDev, 0x4860, 0x00);
    os05a_write_register(IspDev, 0x4861, 0xa0);
    os05a_write_register(IspDev, 0x484b, 0x05);
    os05a_write_register(IspDev, 0x4850, 0x00);
    os05a_write_register(IspDev, 0x4851, 0xaa);
    os05a_write_register(IspDev, 0x4852, 0xff);
    os05a_write_register(IspDev, 0x4853, 0x8a);
    os05a_write_register(IspDev, 0x4854, 0x08);
    os05a_write_register(IspDev, 0x4855, 0x30);
    os05a_write_register(IspDev, 0x4800, 0x00);
    //os05a_write_register(IspDev, 0x4837, 0x15);
    os05a_write_register(IspDev, 0x4837, 0x0b);
    os05a_write_register(IspDev, 0x5000, 0xc9);
    os05a_write_register(IspDev, 0x5001, 0x43);
    os05a_write_register(IspDev, 0x5211, 0x03);
    os05a_write_register(IspDev, 0x5291, 0x03);
    os05a_write_register(IspDev, 0x520d, 0x0f);
    os05a_write_register(IspDev, 0x520e, 0xfd);
    //os05a_write_register(IspDev, 0x520f, 0xf5);
    os05a_write_register(IspDev, 0x520f, 0xa5);
    //os05a_write_register(IspDev, 0x5210, 0xf5);
    os05a_write_register(IspDev, 0x5210, 0xa5);
    //os05a_write_register(IspDev, 0x528d, 0x02);
    os05a_write_register(IspDev, 0x528d, 0x0f);
    os05a_write_register(IspDev, 0x528e, 0xfd);
    //os05a_write_register(IspDev, 0x528f, 0xf5);
    os05a_write_register(IspDev, 0x528f, 0xa5);
    //os05a_write_register(IspDev, 0x5290, 0xf5);
    os05a_write_register(IspDev, 0x5290, 0xa5);
    os05a_write_register(IspDev, 0x5004, 0x40);
    os05a_write_register(IspDev, 0x5005, 0x00);
    os05a_write_register(IspDev, 0x5180, 0x00);
    os05a_write_register(IspDev, 0x5181, 0x10);
    os05a_write_register(IspDev, 0x5182, 0x0f);
    os05a_write_register(IspDev, 0x5183, 0xff);
    os05a_write_register(IspDev, 0x580b, 0x03);
    
    os05a_write_register(IspDev, 0x4d00, 0x03);
    os05a_write_register(IspDev, 0x4d01, 0xe9);
    os05a_write_register(IspDev, 0x4d02, 0xba);
    os05a_write_register(IspDev, 0x4d03, 0x66);
    os05a_write_register(IspDev, 0x4d04, 0x46);
    os05a_write_register(IspDev, 0x4d05, 0xa5);
    os05a_write_register(IspDev, 0x4603, 0x3c);
    os05a_write_register(IspDev, 0x4703, 0x26);
    os05a_write_register(IspDev, 0x4709, 0x49);
    
    os05a_write_register(IspDev, 0x4000, 0x79);
    //os05a_write_register(IspDev, 0x380c, 0x03);
    os05a_write_register(IspDev, 0x380c, 0x02);
    //os05a_write_register(IspDev, 0x380d, 0xf0);
    os05a_write_register(IspDev, 0x380d, 0xd0);
    //os05a_write_register(IspDev, 0x380e, 0x08);
    os05a_write_register(IspDev, 0x380e, 0x09);
    //os05a_write_register(IspDev, 0x380f, 0x5e);
    os05a_write_register(IspDev, 0x380f, 0xc0);
    //os05a_write_register(IspDev, 0x3501, 0x08);
    os05a_write_register(IspDev, 0x3501, 0x09);
    //os05a_write_register(IspDev, 0x3502, 0x36);
    os05a_write_register(IspDev, 0x3502, 0x98);
    os05a_write_register(IspDev, 0x3511, 0x00);
    os05a_write_register(IspDev, 0x3512, 0x20);
    os05a_write_register(IspDev, 0x0100, 0x01);

    os05a_write_register(IspDev, 0x0100, 0x00);

    os05a_write_register(IspDev, 0x0100, 0x01);
    printf("-------OV os05a_init_5M_2688x1944_10bit_wdr30 Initial OK!-------\n");
}

void os05a_init_5M_2688x1944_12bit_linear30(ISP_DEV IspDev)
{
    os05a_write_register(IspDev, 0x4600, 0x00);
    os05a_write_register(IspDev, 0x4601, 0x10);
    os05a_write_register(IspDev, 0x4603, 0x01);

    os05a_write_register(IspDev, 0x0103, 0x01);
    os05a_write_register(IspDev, 0x0303, 0x01);
    os05a_write_register(IspDev, 0x0305, 0x27);
    os05a_write_register(IspDev, 0x0306, 0x00);
    os05a_write_register(IspDev, 0x0307, 0x00);
    os05a_write_register(IspDev, 0x0308, 0x03);
    os05a_write_register(IspDev, 0x0309, 0x04);
    os05a_write_register(IspDev, 0x032a, 0x00);
    os05a_write_register(IspDev, 0x031e, 0x0a);
    os05a_write_register(IspDev, 0x0325, 0x48);
    os05a_write_register(IspDev, 0x0328, 0x07);
    os05a_write_register(IspDev, 0x300d, 0x11);
    os05a_write_register(IspDev, 0x300e, 0x11);
    os05a_write_register(IspDev, 0x300f, 0x11);
    os05a_write_register(IspDev, 0x3010, 0x01);
    os05a_write_register(IspDev, 0x3012, 0x41);
    os05a_write_register(IspDev, 0x3016, 0xf0);
    os05a_write_register(IspDev, 0x3018, 0xf0);
    os05a_write_register(IspDev, 0x3028, 0xf0);
    os05a_write_register(IspDev, 0x301e, 0x98);
    os05a_write_register(IspDev, 0x3010, 0x04);
    os05a_write_register(IspDev, 0x3011, 0x06);
    os05a_write_register(IspDev, 0x3031, 0xa9);
    os05a_write_register(IspDev, 0x3103, 0x48);
    os05a_write_register(IspDev, 0x3104, 0x01);
    os05a_write_register(IspDev, 0x3106, 0x10);
    os05a_write_register(IspDev, 0x3400, 0x04);
    os05a_write_register(IspDev, 0x3025, 0x03);
    os05a_write_register(IspDev, 0x3425, 0x51);
    os05a_write_register(IspDev, 0x3428, 0x01);
    os05a_write_register(IspDev, 0x3406, 0x08);
    os05a_write_register(IspDev, 0x3408, 0x03);
    os05a_write_register(IspDev, 0x3501, 0x08);
    os05a_write_register(IspDev, 0x3502, 0x6f);
    os05a_write_register(IspDev, 0x3505, 0x83);
    os05a_write_register(IspDev, 0x3508, 0x00);
    os05a_write_register(IspDev, 0x3509, 0x80);
    os05a_write_register(IspDev, 0x350a, 0x04);
    os05a_write_register(IspDev, 0x350b, 0x00);
    os05a_write_register(IspDev, 0x350c, 0x00);
    os05a_write_register(IspDev, 0x350d, 0x80);
    os05a_write_register(IspDev, 0x350e, 0x04);
    os05a_write_register(IspDev, 0x350f, 0x00);
    os05a_write_register(IspDev, 0x3600, 0x00);
    os05a_write_register(IspDev, 0x3626, 0xff);
    os05a_write_register(IspDev, 0x3605, 0x50);
    os05a_write_register(IspDev, 0x3609, 0xdb);
    os05a_write_register(IspDev, 0x3610, 0x69);
    os05a_write_register(IspDev, 0x360c, 0x01);
    os05a_write_register(IspDev, 0x3628, 0xa4);
    os05a_write_register(IspDev, 0x3629, 0x6a);
    os05a_write_register(IspDev, 0x362d, 0x10);
    os05a_write_register(IspDev, 0x3660, 0xd3);
    os05a_write_register(IspDev, 0x3661, 0x06);
    os05a_write_register(IspDev, 0x3662, 0x00);
    os05a_write_register(IspDev, 0x3663, 0x28);
    os05a_write_register(IspDev, 0x3664, 0x0d);
    os05a_write_register(IspDev, 0x366a, 0x38);
    os05a_write_register(IspDev, 0x366b, 0xa0);
    os05a_write_register(IspDev, 0x366d, 0x00);
    os05a_write_register(IspDev, 0x366e, 0x00);
    os05a_write_register(IspDev, 0x3680, 0x00);
    os05a_write_register(IspDev, 0x3621, 0x81);
    os05a_write_register(IspDev, 0x3634, 0x31);
    os05a_write_register(IspDev, 0x3620, 0x00);
    os05a_write_register(IspDev, 0x3622, 0x00);
    os05a_write_register(IspDev, 0x362a, 0xd0);
    os05a_write_register(IspDev, 0x362e, 0x8c);
    os05a_write_register(IspDev, 0x362f, 0x98);
    os05a_write_register(IspDev, 0x3630, 0xb0);
    os05a_write_register(IspDev, 0x3631, 0xd7);
    os05a_write_register(IspDev, 0x3701, 0x0f);
    os05a_write_register(IspDev, 0x3737, 0x02);
    os05a_write_register(IspDev, 0x3741, 0x04);
    os05a_write_register(IspDev, 0x373c, 0x0f);
    os05a_write_register(IspDev, 0x373b, 0x02);
    os05a_write_register(IspDev, 0x3705, 0x00);
    os05a_write_register(IspDev, 0x3706, 0xa0);
    os05a_write_register(IspDev, 0x370a, 0x01);
    os05a_write_register(IspDev, 0x370b, 0xc8);
    os05a_write_register(IspDev, 0x3709, 0x4a);
    os05a_write_register(IspDev, 0x3714, 0x21);
    os05a_write_register(IspDev, 0x371c, 0x00);
    os05a_write_register(IspDev, 0x371d, 0x08);
    os05a_write_register(IspDev, 0x375e, 0x0b);
    os05a_write_register(IspDev, 0x3776, 0x10);
    os05a_write_register(IspDev, 0x3781, 0x02);
    os05a_write_register(IspDev, 0x3782, 0x04);
    os05a_write_register(IspDev, 0x3783, 0x02);
    os05a_write_register(IspDev, 0x3784, 0x08);
    os05a_write_register(IspDev, 0x3785, 0x08);
    os05a_write_register(IspDev, 0x3788, 0x01);
    os05a_write_register(IspDev, 0x3789, 0x01);
    os05a_write_register(IspDev, 0x3797, 0x04);
    os05a_write_register(IspDev, 0x3761, 0x02);
    os05a_write_register(IspDev, 0x3762, 0x0d);
    os05a_write_register(IspDev, 0x3800, 0x00);
    os05a_write_register(IspDev, 0x3801, 0x00);
    os05a_write_register(IspDev, 0x3802, 0x00);
    os05a_write_register(IspDev, 0x3803, 0x0c);
    os05a_write_register(IspDev, 0x3804, 0x0e);
    os05a_write_register(IspDev, 0x3805, 0xff);
    os05a_write_register(IspDev, 0x3806, 0x08);
    os05a_write_register(IspDev, 0x3807, 0x6f);
    os05a_write_register(IspDev, 0x3808, 0x0a);
    os05a_write_register(IspDev, 0x3809, 0x80);
    os05a_write_register(IspDev, 0x380a, 0x07);
    os05a_write_register(IspDev, 0x380b, 0x98);
    os05a_write_register(IspDev, 0x380c, 0x04);
    os05a_write_register(IspDev, 0x380d, 0xd0);
    os05a_write_register(IspDev, 0x380e, 0x08);
    os05a_write_register(IspDev, 0x380f, 0x8f);
    os05a_write_register(IspDev, 0x3813, 0x04);
    os05a_write_register(IspDev, 0x3814, 0x01);
    os05a_write_register(IspDev, 0x3815, 0x01);
    os05a_write_register(IspDev, 0x3816, 0x01);
    os05a_write_register(IspDev, 0x3817, 0x01);
    os05a_write_register(IspDev, 0x381c, 0x00);
    os05a_write_register(IspDev, 0x3820, 0x00);
    os05a_write_register(IspDev, 0x3821, 0x04);
    os05a_write_register(IspDev, 0x3823, 0x18);
    os05a_write_register(IspDev, 0x3826, 0x00);
    os05a_write_register(IspDev, 0x3827, 0x01);
    os05a_write_register(IspDev, 0x3832, 0x02);
    os05a_write_register(IspDev, 0x383c, 0x48);
    os05a_write_register(IspDev, 0x383d, 0xff);
    os05a_write_register(IspDev, 0x3843, 0x20);
    os05a_write_register(IspDev, 0x382d, 0x08);
    os05a_write_register(IspDev, 0x3d85, 0x0b);
    os05a_write_register(IspDev, 0x3d84, 0x40);
    os05a_write_register(IspDev, 0x3d8c, 0x63);
    os05a_write_register(IspDev, 0x3d8d, 0x00);
    os05a_write_register(IspDev, 0x4000, 0x78);
    os05a_write_register(IspDev, 0x4001, 0x2b);
    os05a_write_register(IspDev, 0x4005, 0x40);
    os05a_write_register(IspDev, 0x4028, 0x2f);
    os05a_write_register(IspDev, 0x400a, 0x01);
    os05a_write_register(IspDev, 0x4010, 0x12);
    os05a_write_register(IspDev, 0x4008, 0x02);
    os05a_write_register(IspDev, 0x4009, 0x0d);
    os05a_write_register(IspDev, 0x401a, 0x58);
    os05a_write_register(IspDev, 0x4050, 0x00);
    os05a_write_register(IspDev, 0x4051, 0x01);
    os05a_write_register(IspDev, 0x4052, 0x00);
    os05a_write_register(IspDev, 0x4053, 0x80);
    os05a_write_register(IspDev, 0x4054, 0x00);
    os05a_write_register(IspDev, 0x4055, 0x80);
    os05a_write_register(IspDev, 0x4056, 0x00);
    os05a_write_register(IspDev, 0x4057, 0x80);
    os05a_write_register(IspDev, 0x4058, 0x00);
    os05a_write_register(IspDev, 0x4059, 0x80);
    os05a_write_register(IspDev, 0x430b, 0xff);
    os05a_write_register(IspDev, 0x430c, 0xff);
    os05a_write_register(IspDev, 0x430d, 0x00);
    os05a_write_register(IspDev, 0x430e, 0x00);
    os05a_write_register(IspDev, 0x4501, 0x18);
    os05a_write_register(IspDev, 0x4502, 0x00);
    os05a_write_register(IspDev, 0x4600, 0x00);
    os05a_write_register(IspDev, 0x4601, 0x10);
    os05a_write_register(IspDev, 0x4603, 0x01);
    os05a_write_register(IspDev, 0x4643, 0x00);
    os05a_write_register(IspDev, 0x4640, 0x01);
    os05a_write_register(IspDev, 0x4641, 0x04);
    os05a_write_register(IspDev, 0x480e, 0x00);
    os05a_write_register(IspDev, 0x4813, 0x00);
    os05a_write_register(IspDev, 0x4815, 0x2b);
    os05a_write_register(IspDev, 0x486e, 0x36);
    os05a_write_register(IspDev, 0x486f, 0x84);
    os05a_write_register(IspDev, 0x4860, 0x00);
    os05a_write_register(IspDev, 0x4861, 0xa0);
    os05a_write_register(IspDev, 0x484b, 0x05);
    os05a_write_register(IspDev, 0x4850, 0x00);
    os05a_write_register(IspDev, 0x4851, 0xaa);
    os05a_write_register(IspDev, 0x4852, 0xff);
    os05a_write_register(IspDev, 0x4853, 0x8a);
    os05a_write_register(IspDev, 0x4854, 0x08);
    os05a_write_register(IspDev, 0x4855, 0x30);
    os05a_write_register(IspDev, 0x4800, 0x00);
    os05a_write_register(IspDev, 0x4837, 0x19);
    os05a_write_register(IspDev, 0x5000, 0xc9);
    os05a_write_register(IspDev, 0x5001, 0x43);
    os05a_write_register(IspDev, 0x5211, 0x03);
    os05a_write_register(IspDev, 0x5291, 0x03);
    os05a_write_register(IspDev, 0x520d, 0x0f);
    os05a_write_register(IspDev, 0x520e, 0xfd);
    os05a_write_register(IspDev, 0x520f, 0xa5);
    os05a_write_register(IspDev, 0x5210, 0xa5);
    os05a_write_register(IspDev, 0x528d, 0x0f);
    os05a_write_register(IspDev, 0x528e, 0xfd);
    os05a_write_register(IspDev, 0x528f, 0xa5);
    os05a_write_register(IspDev, 0x5290, 0xa5);
    os05a_write_register(IspDev, 0x5004, 0x40);
    os05a_write_register(IspDev, 0x5005, 0x00);
    os05a_write_register(IspDev, 0x5180, 0x00);
    os05a_write_register(IspDev, 0x5181, 0x10);
    os05a_write_register(IspDev, 0x5182, 0x0f);
    os05a_write_register(IspDev, 0x5183, 0xff);
    os05a_write_register(IspDev, 0x580b, 0x03);

    os05a_write_register(IspDev, 0x4d00, 0x03);
    os05a_write_register(IspDev, 0x4d01, 0xe9);
    os05a_write_register(IspDev, 0x4d02, 0xba);
    os05a_write_register(IspDev, 0x4d03, 0x66);
    os05a_write_register(IspDev, 0x4d04, 0x46);
    os05a_write_register(IspDev, 0x4d05, 0xa5);
    os05a_write_register(IspDev, 0x3603, 0x3c);
    os05a_write_register(IspDev, 0x3703, 0x26);
    os05a_write_register(IspDev, 0x3709, 0x49);
    os05a_write_register(IspDev, 0x4000, 0x79);
    os05a_write_register(IspDev, 0x380c, 0x06);
    os05a_write_register(IspDev, 0x380d, 0x04);
    os05a_write_register(IspDev, 0x380e, 0x09);
    os05a_write_register(IspDev, 0x380f, 0x21);
    os05a_write_register(IspDev, 0x3501, 0x09);
    os05a_write_register(IspDev, 0x3502, 0x19);

    os05a_write_register(IspDev, 0x0100, 0x01);

    os05a_write_register(IspDev, 0x0100, 0x00);

    os05a_write_register(IspDev, 0x0100, 0x01);
    printf("-------OV os05a_init_5M_2688x1944_12bit_linear30 Initial OK!-------\n");
}
