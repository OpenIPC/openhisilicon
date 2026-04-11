/******************************************************************************

  Copyright (C), 2001-2013, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : imx274_sensor_ctl.c
  Version       : Initial Draft
  Author        : Hisilicon BVT ISP group
  Created       : 2014/05/22
  Description   : Sony IMX274 sensor driver
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
#include "imx274_slave_priv.h"
#include "mpi_isp.h"

extern const IMX274_VIDEO_MODE_TBL_S g_astImx274ModeTbl[];
extern ISP_SLAVE_SNS_SYNC_S gstImx274Sync[];

static int g_fd[ISP_MAX_DEV_NUM] = {-1,-1};

extern ISP_SNS_STATE_S             g_astImx274[ISP_MAX_DEV_NUM];
extern ISP_SNS_COMMBUS_U     g_aunImx274BusInfo[];

static const HI_U16 gs_au16SensorCfgSeq[][IMX274_MODE_BUTT+1] = {
    /*  M0     M0_dol     ADDR   */         
    { 0x1a,  0x1a,  0x0000 },           
    { 0x00,  0x00,  0x003e }, 
    { 0x80,  0x80,  0x0120 }, 
    { 0x00,  0x00,  0x0121 }, 
    { 0x03,  0x03,  0x0122 }, 
    { 0x68,  0x68,  0x0129 }, 
    { 0x03,  0x03,  0x012a }, 
    { 0x02,  0x02,  0x012d }, 
    { 0x00,  0x00,  0x010b }, 
    { 0x01,  0x01,  0x00ee }, 
    { 0x94,  0xba,  0x032c }, 
    { 0x11,  0x08,  0x032d }, 
    { 0x94,  0xba,  0x034a }, 
    { 0x11,  0x08,  0x034b }, 
    { 0x94,  0xba,  0x05b6 }, 
    { 0x11,  0x08,  0x05b7 }, 
    { 0x94,  0xba,  0x05b8 }, 
    { 0x11,  0x08,  0x05b9 }, 
    { 0x00,  0x00,  0x004c }, 
    { 0x03,  0x03,  0x004d }, 
    { 0x1a,  0x1a,  0x031c }, 
    { 0x00,  0x00,  0x031d }, 
    { 0x02,  0x02,  0x0502 }, 
    { 0x0e,  0x0e,  0x0529 }, 
    { 0x0e,  0x0e,  0x052a }, 
    { 0x0e,  0x0e,  0x052b }, 
    { 0x0e,  0x0e,  0x0538 }, 
    { 0x0e,  0x0e,  0x0539 }, 
    { 0x00,  0x00,  0x0553 }, 
    { 0x05,  0x05,  0x057d }, 
    { 0x05,  0x05,  0x057f }, 
    { 0x04,  0x04,  0x0581 }, 
    { 0x76,  0x76,  0x0583 }, 
    { 0x01,  0x01,  0x0587 }, 
    { 0x0e,  0x0e,  0x05bb }, 
    { 0x0e,  0x0e,  0x05bc }, 
    { 0x0e,  0x0e,  0x05bd }, 
    { 0x0e,  0x0e,  0x05be }, 
    { 0x0e,  0x0e,  0x05bf }, 
    { 0x00,  0x00,  0x066e }, 
    { 0x00,  0x00,  0x066f }, 
    { 0x00,  0x00,  0x0670 }, 
    { 0x00,  0x00,  0x0671 }, 
    { 	-1,    -1,  0xFFFF },                          
    { 0x01,  0x01,  0x00e6 }, 
    { 0x01,  0x01,  0x00e8 }, 
    { 0x18,  0x18,  0x0000 }, 
    { 	-1,    -1,  0xFFFF }, 
    { 0x10,  0x10,  0x0001 }, 
    { 0x01,  0x01,  0x0009 }, 
    { 0x22,  0x00,  0x0003 }, 
    { 0x00,  0x00,  0x00e2 }, 
    { 0x00,  0x05,  0x0004 }, 
    { 0x07,  0x01,  0x0005 }, 
    { 0x00,  0x00,  0x0006 }, 
    { 0x02,  0x02,  0x0007 }, 
    { 0xff,  0x0a,  0x0342 }, 
    { 0x01,  0x00,  0x0343 }, 
    { 0xff,  0x16,  0x0344 }, 
    { 0x01,  0x00,  0x0345 }, 
    { 0x0f,  0x0e,  0x0528 }, 
    { 0x00,  0x1f,  0x0554 }, 
    { 0x00,  0x01,  0x0555 }, 
    { 0x00,  0x01,  0x0556 }, 
    { 0x00,  0x01,  0x0557 }, 
    { 0x00,  0x01,  0x0558 }, 
    { 0x1f,  0x00,  0x0559 }, 
    { 0x1f,  0x00,  0x055a }, 
    { 0x0f,  0x0e,  0x05ba }, 
    { 0x00,  0x1b,  0x066a }, 
    { 0x00,  0x1a,  0x066b }, 
    { 0x00,  0x19,  0x066c }, 
    { 0x00,  0x17,  0x066d }, 
    { 0x01,  0x01,  0x03a6 }, 
    { 0x07,  0x05,  0x006b },
    { 0x10,  0x11,  0x0019 }, 
#if 0
    { 0x00,  0x00,  0x000a },   /* Again Low 8bits */
    { 0x00,  0x00,  0x000b },   /* Again High 8bits */
    { 0x0c,  0x0c,  0x000c },   /* SHR_Linear Low 8bits */
    { 0x00,  0x00,  0x000d },   /* SHR_Linear High 8bits */
    { 0x00,  0x00,  0x0012 },   /* Dgain */
    { 0x04,  0x06,  0x002e },   /* SHR_DOL1 Low 8bits */
    { 0x00,  0x00,  0x002f },   /* SHR_DOL1 High 8bits */
    { 0x43,  0x3b,  0x0030 },   /* SHR_DOL2 Low 8bits */
    { 0x05,  0x06,  0x0031 },   /* SHR_DOL2 High 8bits */
    { 0x08,  0x32,  0x0032 },   /* RHS1 Low 8bits */
    { 0x00,  0x00,  0x0033 },   /* RHS1 High 8bits */
#else
    { 0x34,  0x74,  0x000a },   /* Again Low 8bits */
    { 0x06,  0x07,  0x000b },   /* Again High 8bits */
    { 0x0c,  0x0c,  0x000c },   /* SHR_Linear Low 8bits */
    { 0x00,  0x00,  0x000d },   /* SHR_Linear High 8bits */
    { 0x00,  0x00,  0x0012 },   /* Dgain */
    { 0x06,  0x06,  0x002e },   /* SHR_DOL1 Low 8bits */
    { 0x00,  0x00,  0x002f },   /* SHR_DOL1 High 8bits */
    { 0x3b,  0x3b,  0x0030 },   /* SHR_DOL2 Low 8bits */
    { 0x06,  0x06,  0x0031 },   /* SHR_DOL2 High 8bits */
    { 0x32,  0x32,  0x0032 },   /* RHS1 Low 8bits */
    { 0x00,  0x00,  0x0033 },   /* RHS1 High 8bits */

#endif
    { 0x30,  0x31,  0x0041 }, 
    { 0x08,  0x07,  0x0042 }, 
    { 0x01,  0x01,  0x0043 }, 
    { 0x00,  0x01,  0x00e9 },
	{   -1,    -1,  0xFFFF } 	
};  

int imx274_spi_init(ISP_DEV IspDev)
{
    char acDevFile[16] = {0};
    HI_U8 u8DevNum, u8Cs;
    
    if(g_fd[IspDev] >= 0)
    {
        return 0;
    }    
    unsigned int value;
    int ret = 0;

    u8DevNum = g_aunImx274BusInfo[IspDev].s8SspDev.bit4SspDev;
    u8Cs = g_aunImx274BusInfo[IspDev].s8SspDev.bit4SspCs;
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

int imx274_spi_exit(ISP_DEV IspDev)
{
    if (g_fd[IspDev] >= 0)
    {
        close(g_fd[IspDev]);
        g_fd[IspDev] = -1;
        return 0;
    }
    return -1;
}

int imx274_write_register(ISP_DEV IspDev,unsigned int addr, unsigned char data)
{
    int ret;
    struct spi_ioc_transfer mesg[1];
    unsigned char  tx_buf[8] = {0};
    unsigned char  rx_buf[8] = {0};
    
    
    tx_buf[0] = 0x81;
    tx_buf[1] = addr >> 8;
    tx_buf[2] = addr & 0xff;
    tx_buf[3] = data;

    if (0 > g_fd[IspDev])
    {
        return 0;
    }

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

int imx274_read_register(ISP_DEV IspDev,unsigned int addr)
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

void imx274_prog(ISP_DEV IspDev,int* rom) 
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
            imx274_write_register(IspDev,addr, data);
        }
    }
}

void imx274_standby(ISP_DEV IspDev)
{
    // TODO:
    return;
}

void imx274_restart(ISP_DEV IspDev)
{
    // TODO:
    return;
}


void imx274_init(ISP_DEV IspDev)
{
    HI_U8            u8ImgMode;
    u8ImgMode   = g_astImx274[IspDev].u8ImgMode;
    
    HI_U16 u16RegData;
    HI_U16 u16RegAddr;
    HI_U32 i;
	HI_U32 u32SeqEntries;

    /* hold sync signal as fixed */
    CHECK_RET(HI_MPI_ISP_GetHvSync(IspDev, &gstImx274Sync[IspDev]));
    gstImx274Sync[IspDev].unCfg.stBits.bitHEnable = 0;
    gstImx274Sync[IspDev].unCfg.stBits.bitVEnable = 0;
    CHECK_RET(HI_MPI_ISP_SetHvSync(IspDev, &gstImx274Sync[IspDev]));

      
    /* 2. sensor spi init */
    imx274_spi_init(IspDev);

    /* When sensor first init, config all registers */
	u32SeqEntries = sizeof(gs_au16SensorCfgSeq) / sizeof(gs_au16SensorCfgSeq[0]);
	
    imx274_write_register (IspDev,0x0000, 0x02); //standy=0[bit(0)],STBLogic=1[bit(1)]
    delay_ms(10); 
    imx274_write_register (IspDev,0x0000, 0x00); //standy=0[bit(0)],STBLogic=0[bit(1)]
    delay_ms(7);
    imx274_write_register (IspDev,0x0001, 0x10); //CLPSQRST=1[bit(4)]
	imx274_write_register (IspDev,0x0009, 0x01); //DCKRST=1[bit(0)]]
	
    for ( i = 0 ; i < u32SeqEntries; i++ )
    {
        u16RegAddr = gs_au16SensorCfgSeq[i][IMX274_MODE_BUTT];
        u16RegData = gs_au16SensorCfgSeq[i][u8ImgMode];
        if (0xFFFF == u16RegAddr)
        {
            delay_ms(20);
        }
        imx274_write_register(IspDev,u16RegAddr, u16RegData);
    }
	
    // release hv sync
    gstImx274Sync[IspDev].u32HsTime = g_astImx274ModeTbl[u8ImgMode].u32InckPerHs;
	if(g_astImx274[IspDev].astRegsInfo[0].stSlvSync.u32SlaveVsTime == 0)
	{
		gstImx274Sync[IspDev].u32VsTime = g_astImx274ModeTbl[u8ImgMode].u32InckPerVs;
	}
	else
	{
		gstImx274Sync[IspDev].u32VsTime = g_astImx274[IspDev].astRegsInfo[0].stSlvSync.u32SlaveVsTime;
	}
    gstImx274Sync[IspDev].unCfg.u32Bytes = 0xc0030000;
    gstImx274Sync[IspDev].u32VsCyc = 0x3;
    gstImx274Sync[IspDev].u32HsCyc = 0x3;
    CHECK_RET(HI_MPI_ISP_SetHvSync(IspDev, &gstImx274Sync[IspDev]));

	for (i=0; i<g_astImx274[IspDev].astRegsInfo[0].u32RegNum; i++)
	{
		imx274_write_register(IspDev, g_astImx274[IspDev].astRegsInfo[0].astSspData[i].u32RegAddr, g_astImx274[IspDev].astRegsInfo[0].astSspData[i].u32Data);
	}

    printf("IMX274 %s init succuss!\n", g_astImx274ModeTbl[u8ImgMode].pszModeName);

    g_astImx274[IspDev].bInit = HI_TRUE;

    return ;
}

void imx274_exit(ISP_DEV IspDev)
{
    imx274_spi_exit(IspDev);

    return;
}


