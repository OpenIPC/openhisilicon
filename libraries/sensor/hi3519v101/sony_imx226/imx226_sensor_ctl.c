/******************************************************************************

  Copyright (C), 2001-2013, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : imx226_sensor_ctl.c
  Version       : Initial Draft
  Author        : Hisilicon BVT ISP group
  Created       : 2014/05/22
  Description   : Sony IMX226 sensor driver
  History       :
  1.Date        : 2014/05/22
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

#include "hi_spi.h"
#include "imx226_slave_priv.h"
#include "mpi_isp.h"

extern const IMX226_VIDEO_MODE_TBL_S g_astImx226ModeTbl[];
extern ISP_SLAVE_SNS_SYNC_S gstImx226Sync[];

static int g_fd[ISP_MAX_DEV_NUM] = {-1,-1};
extern ISP_SNS_STATE_S             g_astImx226[ISP_MAX_DEV_NUM];
extern ISP_SNS_COMMBUS_U     g_aunImx226BusInfo[];


void imx226_sensor_linear_common_init(ISP_DEV IspDev);


int imx226_spi_init(ISP_DEV IspDev)
{
    char acDevFile[16] = {0};
    HI_U8 u8DevNum, u8Cs;
    
    if(g_fd[IspDev] >= 0)
    {
        return 0;
    }    
    unsigned int value;
    int ret = 0;

	u8DevNum = g_aunImx226BusInfo[IspDev].s8SspDev.bit4SspDev;
    u8Cs = g_aunImx226BusInfo[IspDev].s8SspDev.bit4SspCs;
    snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile)-1, "/dev/spidev%d.%d", u8DevNum, u8Cs);

    g_fd[IspDev] = open(acDevFile, 0);
    if (g_fd[IspDev] < 0)
    {
        printf("Open %s error!\n",acDevFile);
        return -1;
    }

    value = SPI_MODE_3 | SPI_LSB_FIRST;// | SPI_LOOP;
    ret = ioctl(g_fd[IspDev], SPI_IOC_WR_MODE, &value);
    if (ret < 0)
    {
        printf("ioctl SPI_IOC_WR_MODE err, value = %d ret = %d\n", value, ret);
        return ret;
    }

    value = 8;
    ret = ioctl(g_fd[IspDev], SPI_IOC_WR_BITS_PER_WORD, &value);
    if (ret < 0)
    {
        printf("ioctl SPI_IOC_WR_BITS_PER_WORD err, value = %d ret = %d\n",value, ret);
        return ret;
    }

    value = 2000000;
    ret = ioctl(g_fd[IspDev], SPI_IOC_WR_MAX_SPEED_HZ, &value);
    if (ret < 0)
    {
        printf("ioctl SPI_IOC_WR_MAX_SPEED_HZ err, value = %d ret = %d\n",value, ret);
        return ret;
    }

    return 0;
}

int imx226_spi_exit(ISP_DEV IspDev)
{
    if (g_fd[IspDev] >= 0)
    {
        close(g_fd[IspDev]);
        g_fd[IspDev] = -1;
        return 0;
    }
    return -1;
}

int imx226_write_register(ISP_DEV IspDev,unsigned int addr, unsigned char data)
{
    int ret;
    struct spi_ioc_transfer mesg[1];
    unsigned char  tx_buf[8] = {0};
    unsigned char  rx_buf[8] = {0};
    
    if (0 > g_fd[IspDev])
    {
        return 0;
    }

    tx_buf[0] = 0x81;
    tx_buf[1] = addr >> 8;
    tx_buf[2] = addr & 0xff;
    tx_buf[3] = data;

    memset(mesg, 0, sizeof(mesg));  
#ifdef __HuaweiLite__
    mesg[0].tx_buf = (char *)tx_buf; 
    mesg[0].rx_buf = (char *)rx_buf;
#else
    mesg[0].tx_buf = (__u32)tx_buf; 
    mesg[0].rx_buf = (__u32)rx_buf;
#endif
    mesg[0].len    = 4;    
    mesg[0].cs_change = 1;

    ret = ioctl(g_fd[IspDev], SPI_IOC_MESSAGE(1), mesg);
    if (ret < 0) {  
        printf("SPI_IOC_MESSAGE error \n");  
        return -1;  
    }

    return 0;
}

int imx226_read_register(ISP_DEV IspDev,unsigned int addr)
{
    int ret = 0;
    struct spi_ioc_transfer mesg[1];
    unsigned char  tx_buf[8] = {0};
    unsigned char  rx_buf[8] = {0};
    

    tx_buf[0] = 0x80;
    tx_buf[1] = addr >> 8;
    tx_buf[2] = addr & 0xff;
    tx_buf[3] = 0;

    memset(mesg, 0, sizeof(mesg));
#ifdef __HuaweiLite__
    mesg[0].tx_buf = (char *)tx_buf;
    mesg[0].rx_buf = (char *)rx_buf;
#else
    mesg[0].tx_buf = (__u32)tx_buf; 
    mesg[0].rx_buf = (__u32)rx_buf;
#endif
    mesg[0].len    = 4;   
    mesg[0].cs_change = 1;

    ret = ioctl(g_fd[IspDev], SPI_IOC_MESSAGE(1), mesg);
    if (ret  < 0) {  
        printf("SPI_IOC_MESSAGE error \n");  
        return -1;  
    }
    
    return rx_buf[3];
}

static void delay_ms(int ms) { 
    hi_usleep(ms*1000);
}

void imx226_standby(ISP_DEV IspDev)
{
    // TODO:
    return;
}

void imx226_restart(ISP_DEV IspDev)
{
    // TODO:
    return;
}

void imx226_init(ISP_DEV IspDev)
{
    HI_U8            u8ImgMode;

    u8ImgMode   = g_astImx226[IspDev].u8ImgMode;

    HI_U8 u8RegData;
    HI_U16 u16RegAddr;
    HI_U32 i;
   
    /* hold sync signal as fixed */
    CHECK_RET(HI_MPI_ISP_GetHvSync(IspDev, &gstImx226Sync[IspDev]));
    gstImx226Sync[IspDev].unCfg.stBits.bitHEnable = 0;
    gstImx226Sync[IspDev].unCfg.stBits.bitVEnable = 0;
    CHECK_RET(HI_MPI_ISP_SetHvSync(IspDev, &gstImx226Sync[IspDev]));

      
    /* 2. imx226 spi init */
    imx226_spi_init(IspDev);

    /* When sensor first init, config all registers */
    

    // common part of init seq
    imx226_sensor_linear_common_init(IspDev);

    // diff part of each mode
    for ( i = 0 ; i < IMX226_VIDEO_MODE_REG_NUM; i++ )
    {
        u16RegAddr = g_astImx226ModeTbl[u8ImgMode].astVideoModeReg[i].u16Addr;
        u8RegData = g_astImx226ModeTbl[u8ImgMode].astVideoModeReg[i].u8Data;
        imx226_write_register(IspDev,u16RegAddr, u8RegData);
    }

    imx226_write_register (IspDev,0x0000, 0x06 );
    delay_ms(2);   
    imx226_write_register (IspDev,0x0000, 0x04 );
    delay_ms(200); 
    
    // release hv sync
    gstImx226Sync[IspDev].u32HsTime = g_astImx226ModeTbl[u8ImgMode].u32InckPerHs;
	if(g_astImx226[IspDev].astRegsInfo[0].stSlvSync.u32SlaveVsTime == 0)
	{
		gstImx226Sync[IspDev].u32VsTime = g_astImx226ModeTbl[u8ImgMode].u32InckPerVs;
	}
	else
	{
		gstImx226Sync[IspDev].u32VsTime = g_astImx226[IspDev].astRegsInfo[0].stSlvSync.u32SlaveVsTime;
	}
    gstImx226Sync[IspDev].unCfg.u32Bytes = 0xc0030000; 
    gstImx226Sync[IspDev].u32HsCyc = 0x3;
    gstImx226Sync[IspDev].u32VsCyc = 0x3;
    CHECK_RET(HI_MPI_ISP_SetHvSync(IspDev, &gstImx226Sync[IspDev]));

	for (i=0; i<g_astImx226[IspDev].astRegsInfo[0].u32RegNum; i++)
	{
		imx226_write_register(IspDev, g_astImx226[IspDev].astRegsInfo[0].astSspData[i].u32RegAddr, g_astImx226[IspDev].astRegsInfo[0].astSspData[i].u32Data);
	}

    printf("IMX226 %s init succuss!\n", g_astImx226ModeTbl[u8ImgMode].pszModeName);
	g_astImx226[IspDev].bInit = HI_TRUE;

    return ;
}

void imx226_exit(ISP_DEV IspDev)
{
    imx226_spi_exit(IspDev);

    return;
}

void imx226_sensor_linear_common_init(ISP_DEV IspDev)
{
    imx226_write_register (IspDev,0x0000 , 0x02 ); //standy=0[bit(0)],STBLogic=1[bit(1)]
    delay_ms(1); 
    imx226_write_register (IspDev,0x0000 , 0x00 ); //standy=0[bit(0)],STBLogic=0[bit(1)]
    delay_ms(16);
    imx226_write_register (IspDev,0x0001 , 0x11 ); //DCKRST=1[bit(0)],CLPSQRST=1[bit(4)]
    imx226_write_register (IspDev,0x0002 , 0x00 );
    imx226_write_register (IspDev,0x0003 , 0x33 );
    imx226_write_register (IspDev,0x0004 , 0x80 );
    imx226_write_register (IspDev,0x0005 , 0x47 );
    imx226_write_register (IspDev,0x0006 , 0x00 );
    imx226_write_register (IspDev,0x0007 , 0x00 );
    imx226_write_register (IspDev,0x0008 , 0x00 );
    imx226_write_register (IspDev,0x0009 , 0x28 );
    imx226_write_register (IspDev,0x000A , 0x05 );
    imx226_write_register (IspDev,0x000b , 0x0A );
    imx226_write_register (IspDev,0x000C , 0x00 );                     
    imx226_write_register (IspDev,0x000d , 0x00 );
    imx226_write_register (IspDev,0x000E , 0x00 );
    imx226_write_register (IspDev,0x000f , 0x00 );
    imx226_write_register (IspDev,0x0010 , 0x00 );
    imx226_write_register (IspDev,0x0011 , 0x00 );
    imx226_write_register (IspDev,0x0012 , 0x00 );
    imx226_write_register (IspDev,0x001a , 0x00 );
    imx226_write_register (IspDev,0x0026 , 0x74 );
    imx226_write_register (IspDev,0x0027 , 0x74 );
    imx226_write_register (IspDev,0x0028 , 0x74 );
    imx226_write_register (IspDev,0x0045 , 0x32 );
    imx226_write_register (IspDev,0x004d , 0x10 );
    imx226_write_register (IspDev,0x0054 , 0x04 );
    imx226_write_register (IspDev,0x0057 , 0x24 );
    imx226_write_register (IspDev,0x007e , 0x20 );
    imx226_write_register (IspDev,0x007F , 0x01 );
    imx226_write_register (IspDev,0x0080 , 0x00 );
    imx226_write_register (IspDev,0x0081 , 0x00 );
    imx226_write_register (IspDev,0x0082 , 0x00 );
    imx226_write_register (IspDev,0x0083 , 0x00 );
    imx226_write_register (IspDev,0x0084 , 0x00 );
    imx226_write_register (IspDev,0x0085 , 0x00 );
    imx226_write_register (IspDev,0x0086 , 0x00 );
    imx226_write_register (IspDev,0x0087 , 0x00 );
    imx226_write_register (IspDev,0x0095 , 0x00 );
    imx226_write_register (IspDev,0x0096 , 0x00 );
    imx226_write_register (IspDev,0x0097 , 0x00 );
    imx226_write_register (IspDev,0x0098 , 0x00 );
    imx226_write_register (IspDev,0x0099 , 0x00 );
    imx226_write_register (IspDev,0x009a , 0x00 );
    imx226_write_register (IspDev,0x009b , 0x00 );
    imx226_write_register (IspDev,0x009c , 0x00 );
    imx226_write_register (IspDev,0x00b6 , 0x47 );
    imx226_write_register (IspDev,0x00b7 , 0x00 );
    imx226_write_register (IspDev,0x00b8 , 0x00 );
    imx226_write_register (IspDev,0x00b9 , 0x00 );
    imx226_write_register (IspDev,0x00ba , 0x00 );
    imx226_write_register (IspDev,0x00bb , 0x00 );
    imx226_write_register (IspDev,0x00bc , 0x00 );
    imx226_write_register (IspDev,0x00bd , 0x00 );
    imx226_write_register (IspDev,0x00be , 0x00 );
    imx226_write_register (IspDev,0x00bf , 0x00 );
    imx226_write_register (IspDev,0x00c0 , 0x00 );
    imx226_write_register (IspDev,0x00c1 , 0x00 );
    imx226_write_register (IspDev,0x00c2 , 0x00 );
    imx226_write_register (IspDev,0x00c3 , 0x00 );
    imx226_write_register (IspDev,0x00c4 , 0x00 );
    imx226_write_register (IspDev,0x00c5 , 0x00 );
    imx226_write_register (IspDev,0x00c6 , 0x00 );
    imx226_write_register (IspDev,0x00c7 , 0x00 );
    imx226_write_register (IspDev,0x00c8 , 0x00 );
    imx226_write_register (IspDev,0x00c9 , 0x00 );
    imx226_write_register (IspDev,0x00ca , 0x00 );
    imx226_write_register (IspDev,0x00cb , 0x00 );
    imx226_write_register (IspDev,0x00cc , 0x00 );
    imx226_write_register (IspDev,0x00ce , 0x0e );
    imx226_write_register (IspDev,0x0210 , 0x0a );
    imx226_write_register (IspDev,0x0211 , 0x00 );
    imx226_write_register (IspDev,0x0212 , 0x31 );
    imx226_write_register (IspDev,0x0213 , 0x01 );
    imx226_write_register (IspDev,0x021c , 0x64 );
    imx226_write_register (IspDev,0x021d , 0x00 );
    imx226_write_register (IspDev,0x021e , 0xef );
    imx226_write_register (IspDev,0x021f , 0x00 );
    imx226_write_register (IspDev,0x0222 , 0x30 );
    imx226_write_register (IspDev,0x0223 , 0x01 );
    imx226_write_register (IspDev,0x0313 , 0x10 );
    imx226_write_register (IspDev,0x0352 , 0x3c );
    imx226_write_register (IspDev,0x0353 , 0x00 );
    imx226_write_register (IspDev,0x0356 , 0x3b );
    imx226_write_register (IspDev,0x0357 , 0x00 );
    imx226_write_register (IspDev,0x0366 , 0x01 );
    imx226_write_register (IspDev,0x0371 , 0x01 );
    imx226_write_register (IspDev,0x0528 , 0x0f );
    imx226_write_register (IspDev,0x0529 , 0x0f );
    imx226_write_register (IspDev,0x052c , 0x1f );
    imx226_write_register (IspDev,0x052d , 0x01 );
    imx226_write_register (IspDev,0x052e , 0x04 );
    imx226_write_register (IspDev,0x0534 , 0x10 );
    imx226_write_register (IspDev,0x057a , 0x0b );
    imx226_write_register (IspDev,0x057b , 0x00 );
    imx226_write_register (IspDev,0x057d , 0x0a );
    imx226_write_register (IspDev,0x057e , 0x00 );
    imx226_write_register (IspDev,0x0582 , 0x07 );
    imx226_write_register (IspDev,0x0617 , 0x10 );
    imx226_write_register (IspDev,0x0650 , 0x00 );
    imx226_write_register (IspDev,0x065c , 0x07 );
    imx226_write_register (IspDev,0x0700 , 0x19 );
    imx226_write_register (IspDev,0x0701 , 0x19 );

    return ;
}


