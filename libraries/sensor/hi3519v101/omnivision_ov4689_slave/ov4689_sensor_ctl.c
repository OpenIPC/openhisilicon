/******************************************************************************

  Copyright (C), 2001-2013, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ov4689_sensor_ctl.c
  Version       : Initial Draft
  Author        : Hisilicon BVT ISP group
  Created       : 2016/03/07
  Description   : Omnivision OV4689 Slave Mode sensor driver
  History       :
  1.Date        : 2016/03/07
  Author        : xgl
  Modification  : Created file

******************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_comm_video.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"
#include "ov4689_slave_priv.h"


#ifdef HI_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "hi_i2c.h"
#endif

const unsigned char ov4689_i2c_addr     =    0x42; /* I2C Address of ov4689 */
const unsigned int  ov4689_addr_byte    =    2;
const unsigned int  ov4689_data_byte    =    1;
static int g_fd[ISP_MAX_DEV_NUM] = {-1,-1};

extern ISP_SNS_STATE_S          g_astOv4689[ISP_MAX_DEV_NUM];
extern ISP_SLAVE_SNS_SYNC_S     gstOv4689Sync[];
extern OV4689_VIDEO_MODE_TBL_S  g_astOv4689ModeTbl[];
extern ISP_SNS_COMMBUS_U  g_aunOv4689BusInfo[];

int ov4689_i2c_init(ISP_DEV IspDev)
{
    char acDevFile[16] = {0};
    HI_U8 u8DevNum = 0;
	
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

	u8DevNum = g_aunOv4689BusInfo[IspDev].s8I2cDev;
    snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile)-1, "/dev/i2c-%d", u8DevNum);
	
    g_fd[IspDev] = open(acDevFile, O_RDWR);
    if(g_fd[IspDev] < 0)
    {
        printf("Open /dev/i2c-%d error!\n", IspDev);
        return -1;
    }

    ret = ioctl(g_fd[IspDev], I2C_SLAVE_FORCE, (ov4689_i2c_addr>>1));
    if (ret < 0)
    {
        printf("CMD_SET_DEV error!\n");
        return ret;
    }
#endif

    return 0;
}

int ov4689_i2c_exit(ISP_DEV IspDev)
{
    if (g_fd[IspDev] >= 0)
    {
        close(g_fd[IspDev]);
        g_fd[IspDev] = -1;
        return 0;
    }
    return -1;
}

static void delay_ms(int ms) { 
    hi_usleep(ms*1000);
}

int ov4689_read_register(ISP_DEV IspDev, int addr)
{
    // TODO: 
    
    return 0;
}

int ov4689_write_register(ISP_DEV IspDev, int addr, int data)
{
    if (0 > g_fd[IspDev])
    {
        return 0;
    }
#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = ov4689_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = ov4689_addr_byte;
    i2c_data.data = data;
    i2c_data.data_byte_num = ov4689_data_byte;

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

    if (ov4689_addr_byte == 2)
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

    if (ov4689_data_byte == 2)
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

    ret = write(g_fd[IspDev], buf, ov4689_addr_byte + ov4689_data_byte);
    if(ret < 0)
    {
        printf("I2C_WRITE error!\n");
        return -1;
    }
#endif
    return 0;
}

void ov4689_standby(ISP_DEV IspDev)
{
    // TODO:
    return;
}

void ov4689_restart(ISP_DEV IspDev)
{
    // TODO:
    return;
}


void ov4689_linear_4M30(ISP_DEV IspDev); 
void ov4689_linear_1080p30(ISP_DEV IspDev);


void ov4689_init(ISP_DEV IspDev)
{
	HI_U32           i;
    HI_U8            u8ImgMode;
    HI_BOOL          bInit;
	
    bInit       = g_astOv4689[IspDev].bInit;
    u8ImgMode   = g_astOv4689[IspDev].u8ImgMode;

     /* hold sync signal as fixed */                                  
    CHECK_RET(HI_MPI_ISP_GetHvSync(IspDev, &gstOv4689Sync[IspDev]));
    gstOv4689Sync[IspDev].unCfg.stBits.bitHEnable = 0;
    gstOv4689Sync[IspDev].unCfg.stBits.bitVEnable = 0; 
    CHECK_RET(HI_MPI_ISP_SetHvSync(IspDev, &gstOv4689Sync[IspDev]));  

    /* 2. sensor i2c init */ 
    ov4689_i2c_init(IspDev);

	if(HI_FALSE == bInit)
	{

		switch (u8ImgMode)
		{
			case OV4689_MODE1:  
				ov4689_linear_4M30(IspDev);
			break;
			
			case OV4689_MODE0:  
				ov4689_linear_1080p30(IspDev);
			break;
			
			default:
				printf("Not support this mode\n");				
		}
	}
	else
	{
		switch (u8ImgMode)
		{
			case OV4689_MODE1:  
				ov4689_linear_4M30(IspDev);
			break;
			
			case OV4689_MODE0:  
				ov4689_linear_1080p30(IspDev);
			break;
			
			default:
				printf("Not support this mode\n");				
		}
	}

    // release hv sync  
    if(g_astOv4689[IspDev].astRegsInfo[0].stSlvSync.u32SlaveVsTime == 0)
	{
		gstOv4689Sync[IspDev].u32VsTime = g_astOv4689ModeTbl[u8ImgMode].u32InckPerVs;
	}
    else
	{
		gstOv4689Sync[IspDev].u32VsTime = g_astOv4689[IspDev].astRegsInfo[0].stSlvSync.u32SlaveVsTime;
	}            
    gstOv4689Sync[IspDev].unCfg.u32Bytes = 0x80000000;                                         
    gstOv4689Sync[IspDev].u32VsCyc = 0x3;                                                      
    CHECK_RET(HI_MPI_ISP_SetHvSync(IspDev, &gstOv4689Sync[IspDev])); 
	
	for (i=0; i<g_astOv4689[IspDev].astRegsInfo[0].u32RegNum; i++)
	{
		ov4689_write_register(IspDev, g_astOv4689[IspDev].astRegsInfo[0].astI2cData[i].u32RegAddr, g_astOv4689[IspDev].astRegsInfo[0].astI2cData[i].u32Data);
	}

    printf("OV4689 %s init succuss!\n", g_astOv4689ModeTbl[u8ImgMode].pszModeName);

	g_astOv4689[IspDev].bInit = HI_TRUE;
	
	return; 
}

void ov4689_exit(ISP_DEV IspDev)
{
    ov4689_i2c_exit(IspDev);

    return;
}


void ov4689_linear_4M30(ISP_DEV IspDev)
{
 #if 0
    delay_ms(10);
    ov4689_write_register(IspDev, 0x0103, 0x01);
    ov4689_write_register(IspDev, 0x3638, 0x00);
    ov4689_write_register(IspDev, 0x0300, 0x00);
    ov4689_write_register(IspDev, 0x0302, 0x2a);
    ov4689_write_register(IspDev, 0x0303, 0x00);
    ov4689_write_register(IspDev, 0x0304, 0x03);
    ov4689_write_register(IspDev, 0x030b, 0x00);
    ov4689_write_register(IspDev, 0x030d, 0x1e);
    ov4689_write_register(IspDev, 0x030e, 0x04);
    ov4689_write_register(IspDev, 0x030f, 0x01);
    ov4689_write_register(IspDev, 0x0312, 0x01);
    ov4689_write_register(IspDev, 0x031e, 0x00);
    ov4689_write_register(IspDev, 0x3000, 0x20);
    ov4689_write_register(IspDev, 0x3002, 0x00);
    ov4689_write_register(IspDev, 0x3018, 0x72);
    ov4689_write_register(IspDev, 0x3020, 0x93);
    ov4689_write_register(IspDev, 0x3021, 0x03);
    ov4689_write_register(IspDev, 0x3022, 0x01);
    ov4689_write_register(IspDev, 0x3031, 0x0c);
    ov4689_write_register(IspDev, 0x303f, 0x0c);
    ov4689_write_register(IspDev, 0x3305, 0xf1);
    ov4689_write_register(IspDev, 0x3307, 0x04);
    ov4689_write_register(IspDev, 0x3309, 0x29);
    ov4689_write_register(IspDev, 0x3500, 0x00);
    ov4689_write_register(IspDev, 0x3501, 0x60);
    ov4689_write_register(IspDev, 0x3502, 0x00);
    ov4689_write_register(IspDev, 0x3503, 0x04);
    ov4689_write_register(IspDev, 0x3504, 0x00);
    ov4689_write_register(IspDev, 0x3505, 0x00);
    ov4689_write_register(IspDev, 0x3506, 0x00);
    ov4689_write_register(IspDev, 0x3507, 0x00);
    ov4689_write_register(IspDev, 0x3508, 0x00);
    ov4689_write_register(IspDev, 0x3509, 0x80);
    ov4689_write_register(IspDev, 0x350a, 0x00);
    ov4689_write_register(IspDev, 0x350b, 0x00);
    ov4689_write_register(IspDev, 0x350c, 0x00);
    ov4689_write_register(IspDev, 0x350d, 0x00);
    ov4689_write_register(IspDev, 0x350e, 0x00);
    ov4689_write_register(IspDev, 0x350f, 0x80);
    ov4689_write_register(IspDev, 0x3510, 0x00);
    ov4689_write_register(IspDev, 0x3511, 0x00);
    ov4689_write_register(IspDev, 0x3512, 0x00);
    ov4689_write_register(IspDev, 0x3513, 0x00);
    ov4689_write_register(IspDev, 0x3514, 0x00);
    ov4689_write_register(IspDev, 0x3515, 0x80);
    ov4689_write_register(IspDev, 0x3516, 0x00);
    ov4689_write_register(IspDev, 0x3517, 0x00);
    ov4689_write_register(IspDev, 0x3518, 0x00);
    ov4689_write_register(IspDev, 0x3519, 0x00);
    ov4689_write_register(IspDev, 0x351a, 0x00);
    ov4689_write_register(IspDev, 0x351b, 0x80);
    ov4689_write_register(IspDev, 0x351c, 0x00);
    ov4689_write_register(IspDev, 0x351d, 0x00);
    ov4689_write_register(IspDev, 0x351e, 0x00);
    ov4689_write_register(IspDev, 0x351f, 0x00);
    ov4689_write_register(IspDev, 0x3520, 0x00);
    ov4689_write_register(IspDev, 0x3521, 0x80);
    ov4689_write_register(IspDev, 0x3522, 0x08);
    ov4689_write_register(IspDev, 0x3524, 0x08);
    ov4689_write_register(IspDev, 0x3526, 0x08);
    ov4689_write_register(IspDev, 0x3528, 0x08);
    ov4689_write_register(IspDev, 0x352a, 0x08);
    ov4689_write_register(IspDev, 0x3602, 0x00);
    ov4689_write_register(IspDev, 0x3603, 0x40);
    ov4689_write_register(IspDev, 0x3604, 0x02);
    ov4689_write_register(IspDev, 0x3605, 0x00);
    ov4689_write_register(IspDev, 0x3606, 0x00);
    ov4689_write_register(IspDev, 0x3607, 0x00);
    ov4689_write_register(IspDev, 0x3609, 0x12);
    ov4689_write_register(IspDev, 0x360a, 0x40);
    ov4689_write_register(IspDev, 0x360c, 0x08);
    ov4689_write_register(IspDev, 0x360f, 0xe5);
    ov4689_write_register(IspDev, 0x3608, 0x8f);
    ov4689_write_register(IspDev, 0x3611, 0x00);
    ov4689_write_register(IspDev, 0x3613, 0xf7);
    ov4689_write_register(IspDev, 0x3616, 0x58);
    ov4689_write_register(IspDev, 0x3619, 0x99);
    ov4689_write_register(IspDev, 0x361b, 0x60);
    ov4689_write_register(IspDev, 0x361c, 0x7a);
    ov4689_write_register(IspDev, 0x361e, 0x79);
    ov4689_write_register(IspDev, 0x361f, 0x02);
    ov4689_write_register(IspDev, 0x3632, 0x00);
    ov4689_write_register(IspDev, 0x3633, 0x10);
    ov4689_write_register(IspDev, 0x3634, 0x10);
    ov4689_write_register(IspDev, 0x3635, 0x10);
    ov4689_write_register(IspDev, 0x3636, 0x15);
    ov4689_write_register(IspDev, 0x3646, 0x86);
    ov4689_write_register(IspDev, 0x364a, 0x0b);
    ov4689_write_register(IspDev, 0x3700, 0x17);
    ov4689_write_register(IspDev, 0x3701, 0x22);
    ov4689_write_register(IspDev, 0x3703, 0x10);
    ov4689_write_register(IspDev, 0x370a, 0x37);
    ov4689_write_register(IspDev, 0x3705, 0x00);
    ov4689_write_register(IspDev, 0x3706, 0x63);
    ov4689_write_register(IspDev, 0x3709, 0x3c);
    ov4689_write_register(IspDev, 0x370b, 0x01);
    ov4689_write_register(IspDev, 0x370c, 0x30);
    ov4689_write_register(IspDev, 0x3710, 0x24);
    ov4689_write_register(IspDev, 0x3711, 0x0c);
    ov4689_write_register(IspDev, 0x3716, 0x00);
    ov4689_write_register(IspDev, 0x3720, 0x28);
    ov4689_write_register(IspDev, 0x3729, 0x7b);
    ov4689_write_register(IspDev, 0x372a, 0x84);
    ov4689_write_register(IspDev, 0x372b, 0xbd);
    ov4689_write_register(IspDev, 0x372c, 0xbc);
    ov4689_write_register(IspDev, 0x372e, 0x52);
    ov4689_write_register(IspDev, 0x373c, 0x0e);
    ov4689_write_register(IspDev, 0x373e, 0x33);
    ov4689_write_register(IspDev, 0x3743, 0x10);
    ov4689_write_register(IspDev, 0x3744, 0x88);
    ov4689_write_register(IspDev, 0x3745, 0xc0);
    ov4689_write_register(IspDev, 0x374a, 0x43);
    ov4689_write_register(IspDev, 0x374c, 0x00);
    ov4689_write_register(IspDev, 0x374e, 0x23);
    ov4689_write_register(IspDev, 0x3751, 0x7b);
    ov4689_write_register(IspDev, 0x3752, 0x84);
    ov4689_write_register(IspDev, 0x3753, 0xbd);
    ov4689_write_register(IspDev, 0x3754, 0xbc);
    ov4689_write_register(IspDev, 0x3756, 0x52);
    ov4689_write_register(IspDev, 0x375c, 0x00);
    ov4689_write_register(IspDev, 0x3760, 0x00);
    ov4689_write_register(IspDev, 0x3761, 0x00);
    ov4689_write_register(IspDev, 0x3762, 0x00);
    ov4689_write_register(IspDev, 0x3763, 0x00);
    ov4689_write_register(IspDev, 0x3764, 0x00);
    ov4689_write_register(IspDev, 0x3767, 0x04);
    ov4689_write_register(IspDev, 0x3768, 0x04);
    ov4689_write_register(IspDev, 0x3769, 0x08);
    ov4689_write_register(IspDev, 0x376a, 0x08);
    ov4689_write_register(IspDev, 0x376b, 0x20);
    ov4689_write_register(IspDev, 0x376c, 0x00);
    ov4689_write_register(IspDev, 0x376d, 0x00);
    ov4689_write_register(IspDev, 0x376e, 0x00);
    ov4689_write_register(IspDev, 0x3773, 0x00);
    ov4689_write_register(IspDev, 0x3774, 0x51);
    ov4689_write_register(IspDev, 0x3776, 0xbd);
    ov4689_write_register(IspDev, 0x3777, 0xbd);
    ov4689_write_register(IspDev, 0x3781, 0x18);
    ov4689_write_register(IspDev, 0x3783, 0x25);
    ov4689_write_register(IspDev, 0x3798, 0x1b);
    ov4689_write_register(IspDev, 0x3800, 0x00);
    ov4689_write_register(IspDev, 0x3801, 0x08);
    ov4689_write_register(IspDev, 0x3802, 0x00);
    ov4689_write_register(IspDev, 0x3803, 0x04);
    ov4689_write_register(IspDev, 0x3804, 0x0a);
    ov4689_write_register(IspDev, 0x3805, 0x97);
    ov4689_write_register(IspDev, 0x3806, 0x05);
    ov4689_write_register(IspDev, 0x3807, 0xfb);
    ov4689_write_register(IspDev, 0x3808, 0x0a);
    ov4689_write_register(IspDev, 0x3809, 0x80);
    ov4689_write_register(IspDev, 0x380a, 0x05);
    ov4689_write_register(IspDev, 0x380b, 0xf0);
    ov4689_write_register(IspDev, 0x380c, 0x0a);
    ov4689_write_register(IspDev, 0x380d, 0x18);
    ov4689_write_register(IspDev, 0x380e, 0x06);
    ov4689_write_register(IspDev, 0x380f, 0x12);
    ov4689_write_register(IspDev, 0x3810, 0x00);
    ov4689_write_register(IspDev, 0x3811, 0x08);
    ov4689_write_register(IspDev, 0x3812, 0x00);
    ov4689_write_register(IspDev, 0x3813, 0x04);
    ov4689_write_register(IspDev, 0x3814, 0x01);
    ov4689_write_register(IspDev, 0x3815, 0x01);
    ov4689_write_register(IspDev, 0x3819, 0x01);
    ov4689_write_register(IspDev, 0x3820, 0x06);
    ov4689_write_register(IspDev, 0x3821, 0x00);
    ov4689_write_register(IspDev, 0x3823, 0x50);
    ov4689_write_register(IspDev, 0x3824, 0x00);
    ov4689_write_register(IspDev, 0x3825, 0x20);
    ov4689_write_register(IspDev, 0x3826, 0x06);
    ov4689_write_register(IspDev, 0x3827, 0x10);
    ov4689_write_register(IspDev, 0x3829, 0x00);
    ov4689_write_register(IspDev, 0x382a, 0x01);
    ov4689_write_register(IspDev, 0x382b, 0x01);
    ov4689_write_register(IspDev, 0x382d, 0x7f);
    ov4689_write_register(IspDev, 0x3830, 0x04);
    ov4689_write_register(IspDev, 0x3836, 0x01);
    ov4689_write_register(IspDev, 0x3837, 0x00);
    ov4689_write_register(IspDev, 0x3841, 0x02);
    ov4689_write_register(IspDev, 0x3846, 0x08);
    ov4689_write_register(IspDev, 0x3847, 0x07);
    ov4689_write_register(IspDev, 0x3d85, 0x36);
    ov4689_write_register(IspDev, 0x3d8c, 0x71);
    ov4689_write_register(IspDev, 0x3d8d, 0xcb);
    ov4689_write_register(IspDev, 0x3f0a, 0x00);
    ov4689_write_register(IspDev, 0x4000, 0xf1);
    ov4689_write_register(IspDev, 0x4001, 0x40);
    ov4689_write_register(IspDev, 0x4002, 0x04);
    ov4689_write_register(IspDev, 0x4003, 0x14);
    ov4689_write_register(IspDev, 0x400e, 0x00);
    ov4689_write_register(IspDev, 0x4011, 0x00);
    ov4689_write_register(IspDev, 0x401a, 0x00);
    ov4689_write_register(IspDev, 0x401b, 0x00);
    ov4689_write_register(IspDev, 0x401c, 0x00);
    ov4689_write_register(IspDev, 0x401d, 0x00);
    ov4689_write_register(IspDev, 0x401f, 0x00);
    ov4689_write_register(IspDev, 0x4020, 0x00);
    ov4689_write_register(IspDev, 0x4021, 0x10);
    ov4689_write_register(IspDev, 0x4022, 0x07);
    ov4689_write_register(IspDev, 0x4023, 0xcf);
    ov4689_write_register(IspDev, 0x4024, 0x09);
    ov4689_write_register(IspDev, 0x4025, 0x60);
    ov4689_write_register(IspDev, 0x4026, 0x09);
    ov4689_write_register(IspDev, 0x4027, 0x6f);
    ov4689_write_register(IspDev, 0x4028, 0x00);
    ov4689_write_register(IspDev, 0x4029, 0x02);
    ov4689_write_register(IspDev, 0x402a, 0x06);
    ov4689_write_register(IspDev, 0x402b, 0x04);
    ov4689_write_register(IspDev, 0x402c, 0x02);
    ov4689_write_register(IspDev, 0x402d, 0x02);
    ov4689_write_register(IspDev, 0x402e, 0x0e);
    ov4689_write_register(IspDev, 0x402f, 0x04);
    ov4689_write_register(IspDev, 0x4302, 0xff);
    ov4689_write_register(IspDev, 0x4303, 0xff);
    ov4689_write_register(IspDev, 0x4304, 0x00);
    ov4689_write_register(IspDev, 0x4305, 0x00);
    ov4689_write_register(IspDev, 0x4306, 0x00);
    ov4689_write_register(IspDev, 0x4308, 0x02);
    ov4689_write_register(IspDev, 0x4500, 0x6c);
    ov4689_write_register(IspDev, 0x4501, 0xc4);
    ov4689_write_register(IspDev, 0x4502, 0x40);
    ov4689_write_register(IspDev, 0x4503, 0x01);
    ov4689_write_register(IspDev, 0x4601, 0xA7);
    ov4689_write_register(IspDev, 0x4800, 0x04);
    ov4689_write_register(IspDev, 0x4813, 0x08);
    ov4689_write_register(IspDev, 0x481f, 0x40);
    ov4689_write_register(IspDev, 0x4829, 0x78);
    ov4689_write_register(IspDev, 0x4837, 0x10);
    ov4689_write_register(IspDev, 0x4b00, 0x2a);
    ov4689_write_register(IspDev, 0x4b0d, 0x00);
    ov4689_write_register(IspDev, 0x4d00, 0x04);
    ov4689_write_register(IspDev, 0x4d01, 0x42);
    ov4689_write_register(IspDev, 0x4d02, 0xd1);
    ov4689_write_register(IspDev, 0x4d03, 0x93);
    ov4689_write_register(IspDev, 0x4d04, 0xf5);
    ov4689_write_register(IspDev, 0x4d05, 0xc1);
    ov4689_write_register(IspDev, 0x5000, 0xf3);
    ov4689_write_register(IspDev, 0x5001, 0x11);
    ov4689_write_register(IspDev, 0x5004, 0x00);
    ov4689_write_register(IspDev, 0x500a, 0x00);
    ov4689_write_register(IspDev, 0x500b, 0x00);
    ov4689_write_register(IspDev, 0x5032, 0x00);
    ov4689_write_register(IspDev, 0x5040, 0x00);
    ov4689_write_register(IspDev, 0x5050, 0x0c);
    ov4689_write_register(IspDev, 0x5500, 0x00);
    ov4689_write_register(IspDev, 0x5501, 0x10);
    ov4689_write_register(IspDev, 0x5502, 0x01);
    ov4689_write_register(IspDev, 0x5503, 0x0f);
    ov4689_write_register(IspDev, 0x8000, 0x00);
    ov4689_write_register(IspDev, 0x8001, 0x00);
    ov4689_write_register(IspDev, 0x8002, 0x00);
    ov4689_write_register(IspDev, 0x8003, 0x00);
    ov4689_write_register(IspDev, 0x8004, 0x00);
    ov4689_write_register(IspDev, 0x8005, 0x00);
    ov4689_write_register(IspDev, 0x8006, 0x00);
    ov4689_write_register(IspDev, 0x8007, 0x00);
    ov4689_write_register(IspDev, 0x8008, 0x00);
    ov4689_write_register(IspDev, 0x3638, 0x00);
    ov4689_write_register(IspDev, 0x0100, 0x01);
#endif

    delay_ms(10);
    ov4689_write_register(IspDev, 0x0103, 0x01);  
    ov4689_write_register(IspDev, 0x3638, 0x00);  
    ov4689_write_register(IspDev, 0x0300, 0x00);  
    ov4689_write_register(IspDev, 0x0302, 0x2a);  
    ov4689_write_register(IspDev, 0x0303, 0x01);  
    ov4689_write_register(IspDev, 0x0304, 0x03);  
    ov4689_write_register(IspDev, 0x030b, 0x00);  
    ov4689_write_register(IspDev, 0x030d, 0x1e);  
    ov4689_write_register(IspDev, 0x030e, 0x04);  
    ov4689_write_register(IspDev, 0x030f, 0x01);  
    ov4689_write_register(IspDev, 0x0312, 0x01);  
    ov4689_write_register(IspDev, 0x031e, 0x00);  
    ov4689_write_register(IspDev, 0x3000, 0x00);  
    ov4689_write_register(IspDev, 0x3002, 0x00);  
    ov4689_write_register(IspDev, 0x3018, 0x72);  
    ov4689_write_register(IspDev, 0x3020, 0x93);  
    ov4689_write_register(IspDev, 0x3021, 0x03);  
    ov4689_write_register(IspDev, 0x3022, 0x01);  
    ov4689_write_register(IspDev, 0x3031, 0x0a);  
    ov4689_write_register(IspDev, 0x303f, 0x0c);  
    ov4689_write_register(IspDev, 0x3305, 0xf1);  
    ov4689_write_register(IspDev, 0x3307, 0x04);  
    ov4689_write_register(IspDev, 0x3309, 0x29);  
    ov4689_write_register(IspDev, 0x3500, 0x00);  
    ov4689_write_register(IspDev, 0x3501, 0x60);  
    ov4689_write_register(IspDev, 0x3502, 0x00);  
    ov4689_write_register(IspDev, 0x3503, 0x04);  
    ov4689_write_register(IspDev, 0x3504, 0x00);  
    ov4689_write_register(IspDev, 0x3505, 0x00);  
    ov4689_write_register(IspDev, 0x3506, 0x00);  
    ov4689_write_register(IspDev, 0x3507, 0x00);  
    ov4689_write_register(IspDev, 0x3508, 0x00);  
    ov4689_write_register(IspDev, 0x3509, 0x80);  
    ov4689_write_register(IspDev, 0x350a, 0x00);  
    ov4689_write_register(IspDev, 0x350b, 0x00);  
    ov4689_write_register(IspDev, 0x350c, 0x00);  
    ov4689_write_register(IspDev, 0x350d, 0x00);  
    ov4689_write_register(IspDev, 0x350e, 0x00);  
    ov4689_write_register(IspDev, 0x350f, 0x80);  
    ov4689_write_register(IspDev, 0x3510, 0x00);  
    ov4689_write_register(IspDev, 0x3511, 0x00);  
    ov4689_write_register(IspDev, 0x3512, 0x00);  
    ov4689_write_register(IspDev, 0x3513, 0x00);  
    ov4689_write_register(IspDev, 0x3514, 0x00);  
    ov4689_write_register(IspDev, 0x3515, 0x80);  
    ov4689_write_register(IspDev, 0x3516, 0x00);  
    ov4689_write_register(IspDev, 0x3517, 0x00);  
    ov4689_write_register(IspDev, 0x3518, 0x00);  
    ov4689_write_register(IspDev, 0x3519, 0x00);  
    ov4689_write_register(IspDev, 0x351a, 0x00);  
    ov4689_write_register(IspDev, 0x351b, 0x80);  
    ov4689_write_register(IspDev, 0x351c, 0x00);  
    ov4689_write_register(IspDev, 0x351d, 0x00);  
    ov4689_write_register(IspDev, 0x351e, 0x00);  
    ov4689_write_register(IspDev, 0x351f, 0x00);  
    ov4689_write_register(IspDev, 0x3520, 0x00);  
    ov4689_write_register(IspDev, 0x3521, 0x80);  
    ov4689_write_register(IspDev, 0x3522, 0x08);  
    ov4689_write_register(IspDev, 0x3524, 0x08);  
    ov4689_write_register(IspDev, 0x3526, 0x08);  
    ov4689_write_register(IspDev, 0x3528, 0x08);  
    ov4689_write_register(IspDev, 0x352a, 0x08);  
    ov4689_write_register(IspDev, 0x3602, 0x00);  
    ov4689_write_register(IspDev, 0x3603, 0x40);  
    ov4689_write_register(IspDev, 0x3604, 0x02);  
    ov4689_write_register(IspDev, 0x3605, 0x00);  
    ov4689_write_register(IspDev, 0x3606, 0x00);  
    ov4689_write_register(IspDev, 0x3607, 0x00);  
    ov4689_write_register(IspDev, 0x3609, 0x12);  
    ov4689_write_register(IspDev, 0x360a, 0x40);  
    ov4689_write_register(IspDev, 0x360c, 0x08);  
    ov4689_write_register(IspDev, 0x360f, 0xe5);  
    ov4689_write_register(IspDev, 0x3608, 0x8f);  
    ov4689_write_register(IspDev, 0x3611, 0x00);  
    ov4689_write_register(IspDev, 0x3613, 0xf7);  
    ov4689_write_register(IspDev, 0x3616, 0x58);  
    ov4689_write_register(IspDev, 0x3619, 0x99);  
    ov4689_write_register(IspDev, 0x361b, 0x60);  
    ov4689_write_register(IspDev, 0x361c, 0x7a);  
    ov4689_write_register(IspDev, 0x361e, 0x79);  
    ov4689_write_register(IspDev, 0x361f, 0x02);  
    ov4689_write_register(IspDev, 0x3632, 0x00);  
    ov4689_write_register(IspDev, 0x3633, 0x10);  
    ov4689_write_register(IspDev, 0x3634, 0x10);  
    ov4689_write_register(IspDev, 0x3635, 0x10);  
    ov4689_write_register(IspDev, 0x3636, 0x15);  
    ov4689_write_register(IspDev, 0x3646, 0x86);  
    ov4689_write_register(IspDev, 0x364a, 0x0b);  
    ov4689_write_register(IspDev, 0x3700, 0x17);  
    ov4689_write_register(IspDev, 0x3701, 0x22);  
    ov4689_write_register(IspDev, 0x3703, 0x10);  
    ov4689_write_register(IspDev, 0x370a, 0x37);  
    ov4689_write_register(IspDev, 0x3705, 0x00);  
    ov4689_write_register(IspDev, 0x3706, 0x63);  
    ov4689_write_register(IspDev, 0x3709, 0x3c);  
    ov4689_write_register(IspDev, 0x370b, 0x01);  
    ov4689_write_register(IspDev, 0x370c, 0x30);  
    ov4689_write_register(IspDev, 0x3710, 0x24);  
    ov4689_write_register(IspDev, 0x3711, 0x0c);  
    ov4689_write_register(IspDev, 0x3716, 0x00);  
    ov4689_write_register(IspDev, 0x3720, 0x28);  
    ov4689_write_register(IspDev, 0x3729, 0x7b);  
    ov4689_write_register(IspDev, 0x372a, 0x84);  
    ov4689_write_register(IspDev, 0x372b, 0xbd);  
    ov4689_write_register(IspDev, 0x372c, 0xbc);  
    ov4689_write_register(IspDev, 0x372e, 0x52);  
    ov4689_write_register(IspDev, 0x373c, 0x0e);  
    ov4689_write_register(IspDev, 0x373e, 0x33);  
    ov4689_write_register(IspDev, 0x3743, 0x10);  
    ov4689_write_register(IspDev, 0x3744, 0x88);  
    ov4689_write_register(IspDev, 0x3745, 0xc0);  
    ov4689_write_register(IspDev, 0x374a, 0x43);  
    ov4689_write_register(IspDev, 0x374c, 0x00);  
    ov4689_write_register(IspDev, 0x374e, 0x23);  
    ov4689_write_register(IspDev, 0x3751, 0x7b);  
    ov4689_write_register(IspDev, 0x3752, 0x84);  
    ov4689_write_register(IspDev, 0x3753, 0xbd);  
    ov4689_write_register(IspDev, 0x3754, 0xbc);  
    ov4689_write_register(IspDev, 0x3756, 0x52);  
    ov4689_write_register(IspDev, 0x375c, 0x00);  
    ov4689_write_register(IspDev, 0x3760, 0x00);  
    ov4689_write_register(IspDev, 0x3761, 0x00);  
    ov4689_write_register(IspDev, 0x3762, 0x00);  
    ov4689_write_register(IspDev, 0x3763, 0x00);  
    ov4689_write_register(IspDev, 0x3764, 0x00);  
    ov4689_write_register(IspDev, 0x3767, 0x04);  
    ov4689_write_register(IspDev, 0x3768, 0x04);  
    ov4689_write_register(IspDev, 0x3769, 0x08);  
    ov4689_write_register(IspDev, 0x376a, 0x08);  
    ov4689_write_register(IspDev, 0x376b, 0x20);  
    ov4689_write_register(IspDev, 0x376c, 0x00);  
    ov4689_write_register(IspDev, 0x376d, 0x00);  
    ov4689_write_register(IspDev, 0x376e, 0x00);  
    ov4689_write_register(IspDev, 0x3773, 0x00);  
    ov4689_write_register(IspDev, 0x3774, 0x51);  
    ov4689_write_register(IspDev, 0x3776, 0xbd);  
    ov4689_write_register(IspDev, 0x3777, 0xbd);  
    ov4689_write_register(IspDev, 0x3781, 0x18);  
    ov4689_write_register(IspDev, 0x3783, 0x25);  
    ov4689_write_register(IspDev, 0x3798, 0x1b);  
    ov4689_write_register(IspDev, 0x3800, 0x00);  
    ov4689_write_register(IspDev, 0x3801, 0x08);  
    ov4689_write_register(IspDev, 0x3802, 0x00);  
    ov4689_write_register(IspDev, 0x3803, 0x04);  
    ov4689_write_register(IspDev, 0x3804, 0x0a);  
    ov4689_write_register(IspDev, 0x3805, 0x97);  
    ov4689_write_register(IspDev, 0x3806, 0x05);  
    ov4689_write_register(IspDev, 0x3807, 0xfb);  
    ov4689_write_register(IspDev, 0x3808, 0x0a);  
    ov4689_write_register(IspDev, 0x3809, 0x80);  
    ov4689_write_register(IspDev, 0x380a, 0x05);  
    ov4689_write_register(IspDev, 0x380b, 0xf0);  
    ov4689_write_register(IspDev, 0x380c, 0x09);
    ov4689_write_register(IspDev, 0x380d, 0xc4);
    ov4689_write_register(IspDev, 0x380e, 0x06);  
    ov4689_write_register(IspDev, 0x380f, 0x40);
    ov4689_write_register(IspDev, 0x3810, 0x00);  
    ov4689_write_register(IspDev, 0x3811, 0x08);  
    ov4689_write_register(IspDev, 0x3812, 0x00);  
    ov4689_write_register(IspDev, 0x3813, 0x04);  
    ov4689_write_register(IspDev, 0x3814, 0x01);  
    ov4689_write_register(IspDev, 0x3815, 0x01);  
    ov4689_write_register(IspDev, 0x3819, 0x01);  
    ov4689_write_register(IspDev, 0x3820, 0x06);  
    ov4689_write_register(IspDev, 0x3821, 0x00); 
    ov4689_write_register(IspDev, 0x3823, 0x50);
    ov4689_write_register(IspDev, 0x3824, 0x00);
    ov4689_write_register(IspDev, 0x3825, 0x20);
    ov4689_write_register(IspDev, 0x3826, 0x00);
    ov4689_write_register(IspDev, 0x3827, 0x00);
    ov4689_write_register(IspDev, 0x3829, 0x00);  
    ov4689_write_register(IspDev, 0x382a, 0x01);  
    ov4689_write_register(IspDev, 0x382b, 0x01);  
    ov4689_write_register(IspDev, 0x382d, 0x7f);  
    ov4689_write_register(IspDev, 0x3830, 0x04);  
    ov4689_write_register(IspDev, 0x3836, 0x01);  
    ov4689_write_register(IspDev, 0x3837, 0x00);  
    ov4689_write_register(IspDev, 0x3841, 0x02);  
    ov4689_write_register(IspDev, 0x3846, 0x08);  
    ov4689_write_register(IspDev, 0x3847, 0x07);  
    ov4689_write_register(IspDev, 0x3d85, 0x36);  
    ov4689_write_register(IspDev, 0x3d8c, 0x71);  
    ov4689_write_register(IspDev, 0x3d8d, 0xcb);  
    ov4689_write_register(IspDev, 0x3f0a, 0x00);  
    ov4689_write_register(IspDev, 0x4000, 0xf1);  
    ov4689_write_register(IspDev, 0x4001, 0x40);  
    ov4689_write_register(IspDev, 0x4002, 0x04);  
    ov4689_write_register(IspDev, 0x4003, 0x14);  
    ov4689_write_register(IspDev, 0x400e, 0x00);  
    ov4689_write_register(IspDev, 0x4011, 0x00);  
    ov4689_write_register(IspDev, 0x401a, 0x00);  
    ov4689_write_register(IspDev, 0x401b, 0x00);  
    ov4689_write_register(IspDev, 0x401c, 0x00);  
    ov4689_write_register(IspDev, 0x401d, 0x00);  
    ov4689_write_register(IspDev, 0x401f, 0x00);  
    ov4689_write_register(IspDev, 0x4020, 0x00);  
    ov4689_write_register(IspDev, 0x4021, 0x10);  
    ov4689_write_register(IspDev, 0x4022, 0x07);  
    ov4689_write_register(IspDev, 0x4023, 0xcf);  
    ov4689_write_register(IspDev, 0x4024, 0x09);  
    ov4689_write_register(IspDev, 0x4025, 0x60);  
    ov4689_write_register(IspDev, 0x4026, 0x09);  
    ov4689_write_register(IspDev, 0x4027, 0x6f);  
    ov4689_write_register(IspDev, 0x4028, 0x00);  
    ov4689_write_register(IspDev, 0x4029, 0x02);  
    ov4689_write_register(IspDev, 0x402a, 0x06);  
    ov4689_write_register(IspDev, 0x402b, 0x04);  
    ov4689_write_register(IspDev, 0x402c, 0x02);  
    ov4689_write_register(IspDev, 0x402d, 0x02);  
    ov4689_write_register(IspDev, 0x402e, 0x0e);  
    ov4689_write_register(IspDev, 0x402f, 0x04);  
    ov4689_write_register(IspDev, 0x4302, 0xff);  
    ov4689_write_register(IspDev, 0x4303, 0xff);  
    ov4689_write_register(IspDev, 0x4304, 0x00);  
    ov4689_write_register(IspDev, 0x4305, 0x00);  
    ov4689_write_register(IspDev, 0x4306, 0x00);  
    ov4689_write_register(IspDev, 0x4308, 0x02);  
    ov4689_write_register(IspDev, 0x4500, 0x6c);  
    ov4689_write_register(IspDev, 0x4501, 0xc4);  
    ov4689_write_register(IspDev, 0x4502, 0x40);  
    ov4689_write_register(IspDev, 0x4503, 0x01);  
    ov4689_write_register(IspDev, 0x4601, 0xA7);  
    ov4689_write_register(IspDev, 0x4800, 0x04);  
    ov4689_write_register(IspDev, 0x4813, 0x08);  
    ov4689_write_register(IspDev, 0x481f, 0x40);  
    ov4689_write_register(IspDev, 0x4829, 0x78);  
    ov4689_write_register(IspDev, 0x4837, 0x20);  
    ov4689_write_register(IspDev, 0x4b00, 0x2a);  
    ov4689_write_register(IspDev, 0x4b0d, 0x00);  
    ov4689_write_register(IspDev, 0x4d00, 0x04);  
    ov4689_write_register(IspDev, 0x4d01, 0x42);  
    ov4689_write_register(IspDev, 0x4d02, 0xd1);  
    ov4689_write_register(IspDev, 0x4d03, 0x93);  
    ov4689_write_register(IspDev, 0x4d04, 0xf5);  
    ov4689_write_register(IspDev, 0x4d05, 0xc1);  
    ov4689_write_register(IspDev, 0x5000, 0xf3);  
    ov4689_write_register(IspDev, 0x5001, 0x11);  
    ov4689_write_register(IspDev, 0x5004, 0x00);  
    ov4689_write_register(IspDev, 0x500a, 0x00);  
    ov4689_write_register(IspDev, 0x500b, 0x00);  
    ov4689_write_register(IspDev, 0x5032, 0x00);  
    ov4689_write_register(IspDev, 0x5040, 0x00);  
    ov4689_write_register(IspDev, 0x5050, 0x0c);  
    ov4689_write_register(IspDev, 0x5500, 0x00);  
    ov4689_write_register(IspDev, 0x5501, 0x10);  
    ov4689_write_register(IspDev, 0x5502, 0x01);  
    ov4689_write_register(IspDev, 0x5503, 0x0f);  
    ov4689_write_register(IspDev, 0x8000, 0x00);  
    ov4689_write_register(IspDev, 0x8001, 0x00);  
    ov4689_write_register(IspDev, 0x8002, 0x00);  
    ov4689_write_register(IspDev, 0x8003, 0x00);  
    ov4689_write_register(IspDev, 0x8004, 0x00);  
    ov4689_write_register(IspDev, 0x8005, 0x00);  
    ov4689_write_register(IspDev, 0x8006, 0x00);  
    ov4689_write_register(IspDev, 0x8007, 0x00);  
    ov4689_write_register(IspDev, 0x8008, 0x00);  
    ov4689_write_register(IspDev, 0x3638, 0x00);  
    ov4689_write_register(IspDev, 0x0100, 0x01);  


    printf("-------OV4689 Sensor 4M@30fps Linear Initial OK!-------\n");
}

void ov4689_linear_1080p30(ISP_DEV IspDev)
{
   delay_ms(10);                                                           
   ov4689_write_register(IspDev, 0x0103, 0x01);
   ov4689_write_register(IspDev, 0x3638, 0x00);
   ov4689_write_register(IspDev, 0x0300, 0x00);
   ov4689_write_register(IspDev, 0x0302, 0x53);
   ov4689_write_register(IspDev, 0x0303, 0x03);
   ov4689_write_register(IspDev, 0x0304, 0x03);
   ov4689_write_register(IspDev, 0x0305, 0x01);
   ov4689_write_register(IspDev, 0x0306, 0x01);
   ov4689_write_register(IspDev, 0x030a, 0x01);
   ov4689_write_register(IspDev, 0x030b, 0x00);
   ov4689_write_register(IspDev, 0x030d, 0x1e);
   ov4689_write_register(IspDev, 0x030e, 0x06);
   ov4689_write_register(IspDev, 0x030f, 0x02);
   ov4689_write_register(IspDev, 0x0312, 0x01);
   ov4689_write_register(IspDev, 0x031e, 0x00);
   ov4689_write_register(IspDev, 0x3000, 0x20);
   ov4689_write_register(IspDev, 0x3002, 0x00);
   ov4689_write_register(IspDev, 0x3018, 0x72);
   ov4689_write_register(IspDev, 0x3020, 0x93);
   ov4689_write_register(IspDev, 0x3021, 0x03);
   ov4689_write_register(IspDev, 0x3022, 0x01);
   ov4689_write_register(IspDev, 0x3031, 0x0a);
   ov4689_write_register(IspDev, 0x303f, 0x0c);
   ov4689_write_register(IspDev, 0x3305, 0xf1);
   ov4689_write_register(IspDev, 0x3307, 0x04);
   ov4689_write_register(IspDev, 0x3309, 0x29);
   ov4689_write_register(IspDev, 0x3500, 0x00);
   ov4689_write_register(IspDev, 0x3501, 0x1d);
   ov4689_write_register(IspDev, 0x3502, 0x00);
   ov4689_write_register(IspDev, 0x3503, 0x04);
   ov4689_write_register(IspDev, 0x3504, 0x00);
   ov4689_write_register(IspDev, 0x3505, 0x00);
   ov4689_write_register(IspDev, 0x3506, 0x00);
   ov4689_write_register(IspDev, 0x3507, 0x00);
   ov4689_write_register(IspDev, 0x3508, 0x00);
   ov4689_write_register(IspDev, 0x3509, 0x80);
   ov4689_write_register(IspDev, 0x350a, 0x00);
   ov4689_write_register(IspDev, 0x350b, 0x00);
   ov4689_write_register(IspDev, 0x350c, 0x00);
   ov4689_write_register(IspDev, 0x350d, 0x00);
   ov4689_write_register(IspDev, 0x350e, 0x00);
   ov4689_write_register(IspDev, 0x350f, 0x80);
   ov4689_write_register(IspDev, 0x3510, 0x00);
   ov4689_write_register(IspDev, 0x3511, 0x00);
   ov4689_write_register(IspDev, 0x3512, 0x00);
   ov4689_write_register(IspDev, 0x3513, 0x00);
   ov4689_write_register(IspDev, 0x3514, 0x00);
   ov4689_write_register(IspDev, 0x3515, 0x80);
   ov4689_write_register(IspDev, 0x3516, 0x00);
   ov4689_write_register(IspDev, 0x3517, 0x00);
   ov4689_write_register(IspDev, 0x3518, 0x00);
   ov4689_write_register(IspDev, 0x3519, 0x00);
   ov4689_write_register(IspDev, 0x351a, 0x00);
   ov4689_write_register(IspDev, 0x351b, 0x80);
   ov4689_write_register(IspDev, 0x351c, 0x00);
   ov4689_write_register(IspDev, 0x351d, 0x00);
   ov4689_write_register(IspDev, 0x351e, 0x00);
   ov4689_write_register(IspDev, 0x351f, 0x00);
   ov4689_write_register(IspDev, 0x3520, 0x00);
   ov4689_write_register(IspDev, 0x3521, 0x80);
   ov4689_write_register(IspDev, 0x3522, 0x08);
   ov4689_write_register(IspDev, 0x3524, 0x08);
   ov4689_write_register(IspDev, 0x3526, 0x08);
   ov4689_write_register(IspDev, 0x3528, 0x08);
   ov4689_write_register(IspDev, 0x352a, 0x08);
   ov4689_write_register(IspDev, 0x3602, 0x00);
   ov4689_write_register(IspDev, 0x3603, 0x40);
   ov4689_write_register(IspDev, 0x3604, 0x02);
   ov4689_write_register(IspDev, 0x3605, 0x00);
   ov4689_write_register(IspDev, 0x3606, 0x00);
   ov4689_write_register(IspDev, 0x3607, 0x00);
   ov4689_write_register(IspDev, 0x3609, 0x12);
   ov4689_write_register(IspDev, 0x360a, 0x40);
   ov4689_write_register(IspDev, 0x360c, 0x08);
   ov4689_write_register(IspDev, 0x360f, 0xe5);
   ov4689_write_register(IspDev, 0x3608, 0x8f);
   ov4689_write_register(IspDev, 0x3611, 0x00);
   ov4689_write_register(IspDev, 0x3613, 0xf7);
   ov4689_write_register(IspDev, 0x3616, 0x58);
   ov4689_write_register(IspDev, 0x3619, 0x99);
   ov4689_write_register(IspDev, 0x361b, 0x60);
   ov4689_write_register(IspDev, 0x361c, 0x7a);
   ov4689_write_register(IspDev, 0x361e, 0x79);
   ov4689_write_register(IspDev, 0x361f, 0x02);
   ov4689_write_register(IspDev, 0x3632, 0x00);
   ov4689_write_register(IspDev, 0x3633, 0x10);
   ov4689_write_register(IspDev, 0x3634, 0x10);
   ov4689_write_register(IspDev, 0x3635, 0x10);
   ov4689_write_register(IspDev, 0x3636, 0x15);
   ov4689_write_register(IspDev, 0x3646, 0x86);
   ov4689_write_register(IspDev, 0x364a, 0x0b);
   ov4689_write_register(IspDev, 0x3700, 0x17);
   ov4689_write_register(IspDev, 0x3701, 0x22);
   ov4689_write_register(IspDev, 0x3703, 0x10);
   ov4689_write_register(IspDev, 0x370a, 0x37);
   ov4689_write_register(IspDev, 0x3705, 0x00);
   ov4689_write_register(IspDev, 0x3706, 0x63);
   ov4689_write_register(IspDev, 0x3709, 0x3c);
   ov4689_write_register(IspDev, 0x370b, 0x01);
   ov4689_write_register(IspDev, 0x370c, 0x30);
   ov4689_write_register(IspDev, 0x3710, 0x24);
   ov4689_write_register(IspDev, 0x3711, 0x0c);
   ov4689_write_register(IspDev, 0x3716, 0x00);
   ov4689_write_register(IspDev, 0x3720, 0x28);
   ov4689_write_register(IspDev, 0x3729, 0x7b);
   ov4689_write_register(IspDev, 0x372a, 0x84);
   ov4689_write_register(IspDev, 0x372b, 0xbd);
   ov4689_write_register(IspDev, 0x372c, 0xbc);
   ov4689_write_register(IspDev, 0x372e, 0x52);
   ov4689_write_register(IspDev, 0x373c, 0x0e);
   ov4689_write_register(IspDev, 0x373e, 0x33);
   ov4689_write_register(IspDev, 0x3743, 0x10);
   ov4689_write_register(IspDev, 0x3744, 0x88);
   ov4689_write_register(IspDev, 0x3745, 0xc0);
   ov4689_write_register(IspDev, 0x374a, 0x43);
   ov4689_write_register(IspDev, 0x374c, 0x00);
   ov4689_write_register(IspDev, 0x374e, 0x23);
   ov4689_write_register(IspDev, 0x3751, 0x7b);
   ov4689_write_register(IspDev, 0x3752, 0x84);
   ov4689_write_register(IspDev, 0x3753, 0xbd);
   ov4689_write_register(IspDev, 0x3754, 0xbc);
   ov4689_write_register(IspDev, 0x3756, 0x52);
   ov4689_write_register(IspDev, 0x375c, 0x00);
   ov4689_write_register(IspDev, 0x3760, 0x00);
   ov4689_write_register(IspDev, 0x3761, 0x00);
   ov4689_write_register(IspDev, 0x3762, 0x00);
   ov4689_write_register(IspDev, 0x3763, 0x00);
   ov4689_write_register(IspDev, 0x3764, 0x00);
   ov4689_write_register(IspDev, 0x3767, 0x04);
   ov4689_write_register(IspDev, 0x3768, 0x04);
   ov4689_write_register(IspDev, 0x3769, 0x08);
   ov4689_write_register(IspDev, 0x376a, 0x08);
   ov4689_write_register(IspDev, 0x376b, 0x20);
   ov4689_write_register(IspDev, 0x376c, 0x00);
   ov4689_write_register(IspDev, 0x376d, 0x00);
   ov4689_write_register(IspDev, 0x376e, 0x00);
   ov4689_write_register(IspDev, 0x3773, 0x00);
   ov4689_write_register(IspDev, 0x3774, 0x51);
   ov4689_write_register(IspDev, 0x3776, 0xbd);
   ov4689_write_register(IspDev, 0x3777, 0xbd);
   ov4689_write_register(IspDev, 0x3781, 0x18);
   ov4689_write_register(IspDev, 0x3783, 0x25);
   ov4689_write_register(IspDev, 0x3798, 0x1b);
   ov4689_write_register(IspDev, 0x3800, 0x01);
   ov4689_write_register(IspDev, 0x3801, 0x88);
   ov4689_write_register(IspDev, 0x3802, 0x00);
   ov4689_write_register(IspDev, 0x3803, 0xe0);
   ov4689_write_register(IspDev, 0x3804, 0x09);
   ov4689_write_register(IspDev, 0x3805, 0x17);
   ov4689_write_register(IspDev, 0x3806, 0x05);
   ov4689_write_register(IspDev, 0x3807, 0x1f);
   ov4689_write_register(IspDev, 0x3808, 0x07);
   ov4689_write_register(IspDev, 0x3809, 0x80);
   ov4689_write_register(IspDev, 0x380a, 0x04);
   ov4689_write_register(IspDev, 0x380b, 0x38);
   ov4689_write_register(IspDev, 0x380C, 0x06);
   ov4689_write_register(IspDev, 0x380D, 0xa6);
   ov4689_write_register(IspDev, 0x380E, 0x04);
   ov4689_write_register(IspDev, 0x380F, 0x97);
   ov4689_write_register(IspDev, 0x3810, 0x00);
   ov4689_write_register(IspDev, 0x3811, 0x08);
   ov4689_write_register(IspDev, 0x3812, 0x00);
   ov4689_write_register(IspDev, 0x3813, 0x04);
   ov4689_write_register(IspDev, 0x3814, 0x01);
   ov4689_write_register(IspDev, 0x3815, 0x01);
   ov4689_write_register(IspDev, 0x3819, 0x01);
   ov4689_write_register(IspDev, 0x3820, 0x06);
   ov4689_write_register(IspDev, 0x3821, 0x00);
   ov4689_write_register(IspDev, 0x3823, 0x50);
   ov4689_write_register(IspDev, 0x3824, 0x00);
   ov4689_write_register(IspDev, 0x3825, 0x20);
   ov4689_write_register(IspDev, 0x3826, 0x04);
   ov4689_write_register(IspDev, 0x3827, 0x95);
   ov4689_write_register(IspDev, 0x3829, 0x00);
   ov4689_write_register(IspDev, 0x382a, 0x01);
   ov4689_write_register(IspDev, 0x382b, 0x01);
   ov4689_write_register(IspDev, 0x382d, 0x7f);
   ov4689_write_register(IspDev, 0x3830, 0x04);
   ov4689_write_register(IspDev, 0x3836, 0x01);
   ov4689_write_register(IspDev, 0x3837, 0x00);
   ov4689_write_register(IspDev, 0x3841, 0x02);
   ov4689_write_register(IspDev, 0x3846, 0x08);
   ov4689_write_register(IspDev, 0x3847, 0x07);
   ov4689_write_register(IspDev, 0x3d85, 0x36);
   ov4689_write_register(IspDev, 0x3d8c, 0x71);
   ov4689_write_register(IspDev, 0x3d8d, 0xcb);
   ov4689_write_register(IspDev, 0x3f0a, 0x00);
   ov4689_write_register(IspDev, 0x4000, 0xf1);
   ov4689_write_register(IspDev, 0x4001, 0x40);
   ov4689_write_register(IspDev, 0x4002, 0x04);
   ov4689_write_register(IspDev, 0x4003, 0x14);
   ov4689_write_register(IspDev, 0x400e, 0x00);
   ov4689_write_register(IspDev, 0x4011, 0x00);
   ov4689_write_register(IspDev, 0x401a, 0x00);
   ov4689_write_register(IspDev, 0x401b, 0x00);
   ov4689_write_register(IspDev, 0x401c, 0x00);
   ov4689_write_register(IspDev, 0x401d, 0x00);
   ov4689_write_register(IspDev, 0x401f, 0x00);
   ov4689_write_register(IspDev, 0x4020, 0x00);
   ov4689_write_register(IspDev, 0x4021, 0x10);
   ov4689_write_register(IspDev, 0x4022, 0x06);
   ov4689_write_register(IspDev, 0x4023, 0x13);
   ov4689_write_register(IspDev, 0x4024, 0x07);
   ov4689_write_register(IspDev, 0x4025, 0x40);
   ov4689_write_register(IspDev, 0x4026, 0x07);
   ov4689_write_register(IspDev, 0x4027, 0x50);
   ov4689_write_register(IspDev, 0x4028, 0x00);
   ov4689_write_register(IspDev, 0x4029, 0x02);
   ov4689_write_register(IspDev, 0x402a, 0x06);
   ov4689_write_register(IspDev, 0x402b, 0x04);
   ov4689_write_register(IspDev, 0x402c, 0x02);
   ov4689_write_register(IspDev, 0x402d, 0x02);
   ov4689_write_register(IspDev, 0x402e, 0x0e);
   ov4689_write_register(IspDev, 0x402f, 0x04);
   ov4689_write_register(IspDev, 0x4302, 0xff);
   ov4689_write_register(IspDev, 0x4303, 0xff);
   ov4689_write_register(IspDev, 0x4304, 0x00);
   ov4689_write_register(IspDev, 0x4305, 0x00);
   ov4689_write_register(IspDev, 0x4306, 0x00);
   ov4689_write_register(IspDev, 0x4308, 0x02);
   ov4689_write_register(IspDev, 0x4500, 0x6c);
   ov4689_write_register(IspDev, 0x4501, 0xc4);
   ov4689_write_register(IspDev, 0x4502, 0x40);
   ov4689_write_register(IspDev, 0x4503, 0x01);
   ov4689_write_register(IspDev, 0x4600, 0x00);
   ov4689_write_register(IspDev, 0x4601, 0x77);
   ov4689_write_register(IspDev, 0x4800, 0x04);
   ov4689_write_register(IspDev, 0x4813, 0x08);
   ov4689_write_register(IspDev, 0x481f, 0x40);
   ov4689_write_register(IspDev, 0x4829, 0x78);
   ov4689_write_register(IspDev, 0x4837, 0x40);
   ov4689_write_register(IspDev, 0x4b00, 0x2a);
   ov4689_write_register(IspDev, 0x4b0d, 0x00);
   ov4689_write_register(IspDev, 0x4d00, 0x04);
   ov4689_write_register(IspDev, 0x4d01, 0x42);
   ov4689_write_register(IspDev, 0x4d02, 0xd1);
   ov4689_write_register(IspDev, 0x4d03, 0x93);
   ov4689_write_register(IspDev, 0x4d04, 0xf5);
   ov4689_write_register(IspDev, 0x4d05, 0xc1);
   ov4689_write_register(IspDev, 0x5000, 0xf3);
   ov4689_write_register(IspDev, 0x5001, 0x11);
   ov4689_write_register(IspDev, 0x5004, 0x00);
   ov4689_write_register(IspDev, 0x500a, 0x00);
   ov4689_write_register(IspDev, 0x500b, 0x00);
   ov4689_write_register(IspDev, 0x5032, 0x00);
   ov4689_write_register(IspDev, 0x5040, 0x00);
   ov4689_write_register(IspDev, 0x5050, 0x0c);
   ov4689_write_register(IspDev, 0x5500, 0x00);
   ov4689_write_register(IspDev, 0x5501, 0x10);
   ov4689_write_register(IspDev, 0x5502, 0x01);
   ov4689_write_register(IspDev, 0x5503, 0x0f);
   ov4689_write_register(IspDev, 0x8000, 0x00);
   ov4689_write_register(IspDev, 0x8001, 0x00);
   ov4689_write_register(IspDev, 0x8002, 0x00);
   ov4689_write_register(IspDev, 0x8003, 0x00);
   ov4689_write_register(IspDev, 0x8004, 0x00);
   ov4689_write_register(IspDev, 0x8005, 0x00);
   ov4689_write_register(IspDev, 0x8006, 0x00);
   ov4689_write_register(IspDev, 0x8007, 0x00);
   ov4689_write_register(IspDev, 0x8008, 0x00);
   ov4689_write_register(IspDev, 0x3638, 0x00);
   ov4689_write_register(IspDev, 0x0100, 0x01);
                                                                          
   printf("-------OV4689 Sensor 1080p@30fps Slave Mode Linear Initial OK!-------\n");    

}

