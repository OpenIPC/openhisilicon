/********************************************************************************
*
*  Copyright (C) 2017 	NEXTCHIP Inc. All rights reserved.
*  Module		: AHD Coax protocol
*  Description	: communicate between Decoder and ISP
*  				  get information(AGC, motion, FPS) of ISP
*  				  set NRT/RT
*  				  upgrade Firmware of ISP
*  Author		:
*  Date         :
*  Version		: Version 2.0
*
********************************************************************************
*  History      :
*
*
********************************************************************************/
#include <linux/string.h>
#include <linux/delay.h>

#include "video.h"
#include "acp.h"
#include "eq.h"

/*******************************************************************************
 * extern variable
 *******************************************************************************/
extern int 				chip_id[4];				/* Chip ID */
extern unsigned int 	nvp6134_cnt;			/* Chip count */
extern unsigned char 	ch_mode_status[16];		/* Video format each channel */
extern unsigned char 	ch_vfmt_status[16];		/* NTSC(0x00), PAL(0x01) */
extern unsigned int 	nvp6134_iic_addr[4];	/* Slave address of Chip */
extern nvp6134_equalizer s_eq;					/* EQ manager structure */

/*******************************************************************************
 * internal variable
 *******************************************************************************/
const unsigned char ACP_RX_D0 = 0x50;					/* Default read start address */
nvp6134_acp	s_acp;								/* ACP manager structure */

/*******************************************************************************
 * internal functions
 *******************************************************************************/
void 			init_acp(unsigned char ch);
void			acp_reg_rx_clear(unsigned char ch);
void 			get_acp_reg_rd(unsigned char ch, unsigned char bank, unsigned char addr);
void 			init_acp_reg_rd(unsigned char ch);
unsigned char	read_acp_status(unsigned char ch);
void 			acp_set_baudrate(unsigned char ch);


/*******************************************************************************
 *
 *
 *
 *  Internal Functions
 *
 *
 *
 *******************************************************************************/
/*******************************************************************************
*	Description		: initialize acp register for read
*	Argurments		: ch(channel ID)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void init_acp_reg_rd(unsigned char ch)
{
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_LINES+((ch%2)*0x80), 0x07);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_MODE_ID+((ch%2)*0x80), ACP_REG_RD);
}

/*******************************************************************************
*	Description		: read acp status(header value)
*	Argurments		: ch(channel ID),
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned char read_acp_status(unsigned char ch)
{
	unsigned char val;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_MODE_ID+((ch%2)*0x80), ACP_CAM_STAT);
	val = gpio_i2c_read( nvp6134_iic_addr[ch/4], 0x50+((ch%2)*0x80));
	

	return val;
}

/*******************************************************************************
*	Description		: ?
*	Argurments		: ch(channel ID),
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void get_acp_reg_rd(unsigned char ch, unsigned char bank, unsigned char addr)
{
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_D0+(ch%2)*0x80, ACP_REG_RD);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_D0+1+(ch%2)*0x80, bank);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_D0+2+(ch%2)*0x80, addr);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_D0+3+(ch%2)*0x80, 0x00);//Dummy
	gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_OUT+(ch%2)*0x80, 0x08);
	msleep( 150 );
}

/*******************************************************************************
*	Description		: ?
*	Argurments		: ch(channel ID),
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void acp_reg_rx_clear(unsigned char ch)
{
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_CLR_REG+((ch%2)*0x80), 0x01);
	msleep(10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_CLR_REG+((ch%2)*0x80), 0x00);
	msleep(200);
}

/*******************************************************************************
*	Description		: Initialize ACP each CHIP ID
*	Argurments		: ch(channel ID)
*	Return value	: void
*	Modify			:
*	warning			: Now, The Chip ID of NVP6134 and NVP6134B is 0x90
*******************************************************************************/
void init_acp(unsigned char ch)
{
	acp_reg_rx_clear(ch);
}

/*******************************************************************************
*	Description		: set each channel's baud rate of coax
*	Argurments		: ch(channel ID)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void acp_set_baudrate(unsigned char ch)
{
	/* set baud rate */
	
	if((ch_mode_status[ch] < NVP6134_VI_720P_2530))
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7C, 0x11);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_PEL_BAUD+((ch%2)*0x80), ch_vfmt_status[ch]==PAL? 0x1B:0x1B);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_PEL_LINE+((ch%2)*0x80), ch_vfmt_status[ch]==PAL? 0x0E:0x0E);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_PACKET_MODE+((ch%2)*0x80), 0x06);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_PEL_SYNC+((ch%2)*0x80), ch_vfmt_status[ch]==PAL? 0x20:0xd4);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_PEL_SYNC+1+((ch%2)*0x80), ch_vfmt_status[ch]==PAL?0x06:0x05);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_PEL_EVEN+((ch%2)*0x80), 0x01);

		printk(">>>>> DRV[%s:%d] NVP6134_VI_CVBS COAXIAL PROTOCOL IS SETTING....\n", __func__, __LINE__ );
	}
	#ifdef AHD_PELCO_16BIT
	else if(ch_mode_status[ch] == NVP6134_VI_720P_2530||ch_mode_status[ch] == NVP6134_VI_HDEX)
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7C, 0x01);
		if(ch_mode_status[ch] == NVP6134_VI_HDEX)  //check AHDEX adc clock at the same time
		{
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
			gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_PEL_BAUD+((ch%2)*0x80), ch_vfmt_status[ch]==PAL?0x26:0x26);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_PEL_LINE+((ch%2)*0x80), ch_vfmt_status[ch]==PAL?0x0E:0x0E);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_PEL_SYNC+((ch%2)*0x80), ch_vfmt_status[ch]==PAL? 0x00:0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_PEL_SYNC+1+((ch%2)*0x80), ch_vfmt_status[ch]==PAL?0x01:0x01);
		}
		else
		{
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
			gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_PEL_BAUD+((ch%2)*0x80), ch_vfmt_status[ch]==PAL? 0x12:0x12);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_PEL_LINE+((ch%2)*0x80), ch_vfmt_status[ch]==PAL? 0x0D:0x0E);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_PEL_SYNC+((ch%2)*0x80), ch_vfmt_status[ch]==PAL? 0x50:0x50);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_PEL_SYNC+1+((ch%2)*0x80), ch_vfmt_status[ch]==PAL?0x00:0x00);
		}	
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_PACKET_MODE+((ch%2)*0x80), 0x06);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_PEL_EVEN+((ch%2)*0x80), 0x00);

		printk(">>>>> DRV[%s:%d] NVP6134_VI_720P_2530 COAXIAL PROTOCOL IS SETTING....\n", __func__, __LINE__ );
	}
	#else
	else if(ch_mode_status[ch] == NVP6134_VI_720P_2530 || ch_mode_status[ch] == NVP6134_VI_HDEX )
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7C, 0x11);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
		if(ch_mode_status[ch] == NVP6134_VI_HDEX)
		{
			gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_BAUD+((ch%2)*0x80),0x2C);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_LINE+((ch%2)*0x80),ch_vfmt_status[ch]==PAL? 0x0E:0x0E);
		}
		else
		{
			gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_BAUD+((ch%2)*0x80),0x15);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_LINE+((ch%2)*0x80),ch_vfmt_status[ch]==PAL? 0x0D:0x0E);
		}
		gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0D)+((ch%2)*0x80), ch_vfmt_status[ch]==PAL? 0x35:0x30);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0E)+((ch%2)*0x80), 0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_LINES+((ch%2)*0x80), 0x03);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_BYTE+((ch%2)*0x80), 0x03);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_MODE+((ch%2)*0x80), 0x10);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_PEL_EVEN+((ch%2)*0x80), 0x00);
		printk(">>>>> DRV[%s:%d] NVP6134_VI_720P_2530 COAXIAL PROTOCOL IS SETTING....\n", __func__, __LINE__ );
	}
	#endif
	else if( ch_mode_status[ch] == NVP6134_VI_720P_5060)
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7C, 0x01);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_BAUD+((ch%2)*0x80),0x1A);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_LINE+((ch%2)*0x80),ch_vfmt_status[ch]==PAL? 0x0D:0x0E);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0D)+((ch%2)*0x80), ch_vfmt_status[ch]==PAL? 0x16:0x20);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0E)+((ch%2)*0x80), 0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_LINES+((ch%2)*0x80), 0x03);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_BYTE+((ch%2)*0x80), 0x03);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_MODE+((ch%2)*0x80), 0x10);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_PEL_EVEN+((ch%2)*0x80), 0x00);
		printk(">>>>> DRV[%s:%d] NVP6134_VI_720P_5060 COAXIAL PROTOCOL IS SETTING....\n", __func__, __LINE__ );
	}
	else if(ch_mode_status[ch] == NVP6134_VI_1080P_2530 || ch_mode_status[ch] == NVP6134_VI_1080P_NRT )
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7C, 0x11);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_BAUD+((ch%2)*0x80),0x27);		
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_LINE+((ch%2)*0x80),ch_vfmt_status[ch]==PAL? 0x0E:0x0E);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0D)+((ch%2)*0x80), ch_vfmt_status[ch]==PAL? 0xB4:0xBB);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0E)+((ch%2)*0x80), ch_vfmt_status[ch]==PAL? 0x00:0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_LINES+((ch%2)*0x80), 0x03);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_BYTE+((ch%2)*0x80), 0x03);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_MODE+((ch%2)*0x80), 0x10);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_PEL_EVEN+((ch%2)*0x80), 0x00);

		printk(">>>>> DRV[%s:%d] NVP6124_VI_1080P_2530, NVP6134_VI_1080P_NRT COAXIAL PROTOCOL IS SETTING....\n", __func__, __LINE__ );
	}
	else if(ch_mode_status[ch] == NVP6134_VI_EXC_720PRT ||
			ch_mode_status[ch] == NVP6134_VI_EXC_1080P)
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7C, 0x11);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7D, 0x80);

		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
		if ( ch_mode_status[ch] == NVP6134_VI_EXC_720PRT )
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+((ch%2)*0x80), 0x16);
		else
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+((ch%2)*0x80), 0x17);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x03+((ch%2)*0x80), 0x10);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05+((ch%2)*0x80), 0x05);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0A+((ch%2)*0x80), 0x08);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0B+((ch%2)*0x80), 0x40);

		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0D+((ch%2)*0x80), 0x00);
		if ( ch_mode_status[ch] == NVP6134_VI_EXC_720PRT )
			gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0E)+((ch%2)*0x80), ch_vfmt_status[ch]==PAL? 0x02:0x03);
		else
		{
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0D+((ch%2)*0x80), 0x40);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0E)+((ch%2)*0x80), ch_vfmt_status[ch]==PAL?0x01:0x02);
		}

		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2F+((ch%2)*0x80), 0x00);

		printk(">>>>> DRV[%s:%d] EXC_VI_720P_5060/EXC_VI_1080P_2530 COAXIAL PROTOCOL IS SETTING....\n", __func__, __LINE__ );
	}
	else if(ch_mode_status[ch] == NVP6134_VI_EXC_720P || ch_mode_status[ch] == NVP6134_VI_EXC_HDEX )
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7C, 0x01);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7D, 0x80);

		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+((ch%2)*0x80), 0x0B);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x03+((ch%2)*0x80), 0x10);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05+((ch%2)*0x80), 0x05);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0A+((ch%2)*0x80), 0x08);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0B+((ch%2)*0x80), 0x40);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0D+((ch%2)*0x80), 0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0E)+((ch%2)*0x80), 0x02);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2F+((ch%2)*0x80), 0x00);

		printk(">>>>> DRV[%s:%d] EXC_VI_720P_2530 COAXIAL PROTOCOL IS SETTING....\n", __func__, __LINE__ );
	}
	else if(ch_mode_status[ch] == NVP6134_VI_EXT_720PA || ch_mode_status[ch] == NVP6134_VI_EXT_HDAEX ||
			ch_mode_status[ch] == NVP6134_VI_EXT_720PB || ch_mode_status[ch] == NVP6134_VI_EXT_HDBEX ||
			ch_mode_status[ch] == NVP6134_VI_EXT_720PRT || ch_mode_status[ch] == NVP6134_VI_EXT_1080P )
	{
		if (ch_mode_status[ch] == NVP6134_VI_EXT_720PA || ch_mode_status[ch] == NVP6134_VI_EXT_HDAEX ||
			ch_mode_status[ch] == NVP6134_VI_EXT_720PB || ch_mode_status[ch] == NVP6134_VI_EXT_HDBEX )
		{
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7C, 0x01);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7D, 0x80);
		}
		else
		{
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7C, 0x11);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7D, 0x80);
		}
		

		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+((ch%2)*0x80), 0x34);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x03+((ch%2)*0x80), 0x0A);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05+((ch%2)*0x80), 0x04);
		if (ch_mode_status[ch] == NVP6134_VI_EXT_720PA || ch_mode_status[ch] == NVP6134_VI_EXT_HDAEX ||
			ch_mode_status[ch] == NVP6134_VI_EXT_720PB || ch_mode_status[ch] == NVP6134_VI_EXT_HDBEX )
		{
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0D+((ch%2)*0x80), 0x52);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0E)+((ch%2)*0x80),  0x00);
		}
		else
		{
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0D+((ch%2)*0x80), 0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0E)+((ch%2)*0x80),  0x00);
		}

		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0A+((ch%2)*0x80), 0x08);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0B+((ch%2)*0x80), 0x08);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2F+((ch%2)*0x80), 0x00);

		printk(">>>>> DRV[%s:%d] EXT COAXIAL PROTOCOL IS SETTING....\n", __func__, __LINE__ );
	}
	else if ( ch_mode_status[ch] == NVP6134_VI_EXT_3M_NRT || ch_mode_status[ch] ==  NVP6134_VI_EXT_5M_NRT )
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7C, 0x11);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7D, 0x80);

		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+((ch%2)*0x80), 0x34);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x03+((ch%2)*0x80), 0x0A);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05+((ch%2)*0x80), 0x04);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0D+((ch%2)*0x80), 0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0E)+((ch%2)*0x80), 0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0A+((ch%2)*0x80), 0x08);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0B+((ch%2)*0x80), 0x08);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2F+((ch%2)*0x80), 0x00);

		printk(">>>>> DRV[%s:%d] NVP6134_VI_EXT_3M_NRT, NVP6134_VI_EXT_5M_NRT COAXIAL PROTOCOL IS SETTING....\n", __func__, __LINE__ );
	}
	else if (ch_mode_status[ch] == NVP6134_VI_3M_NRT || ch_mode_status[ch] ==  NVP6134_VI_3M || ch_mode_status[ch] == NVP6134_VI_5M_NRT)
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7C, 0x01);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7D, 0x80);

		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
		if(ch_mode_status[ch] == NVP6134_VI_5M_NRT)
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+((ch%2)*0x80), 0x2C);
		else
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+((ch%2)*0x80), 0x33);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x03+((ch%2)*0x80), 0x0E);
		if(ch_mode_status[ch] == NVP6134_VI_5M_NRT)
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05+((ch%2)*0x80), 0x07);	// AHD 5M@12p(use Active line:8line)
		else
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05+((ch%2)*0x80), 0x03);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0D+((ch%2)*0x80), 0x30);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0E)+((ch%2)*0x80), 0x01);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0A+((ch%2)*0x80), 0x07);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0B+((ch%2)*0x80), 0x10);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2F+((ch%2)*0x80), 0x00);
	}
	else if ( ch_mode_status[ch] == NVP6134_VI_4M || ch_mode_status[ch] == NVP6134_VI_4M_NRT)
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7C, 0x11);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7D, 0x80);

		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
		if(ch_mode_status[ch] == NVP6134_VI_4M_NRT)
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+((ch%2)*0x80), 0x2C);
		else
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+((ch%2)*0x80), 0x34);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x03+((ch%2)*0x80), 0x0E);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05+((ch%2)*0x80), 0x07);	// AHD 4M@25p30p15p(use Active line:8line)
		if(ch_mode_status[ch] == NVP6134_VI_4M_NRT)
		{
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0D+((ch%2)*0x80), 0x60);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0E)+((ch%2)*0x80), 0x01);
		}
		else
		{
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0D+((ch%2)*0x80), 0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0E)+((ch%2)*0x80), 0x00);
		}		
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0A+((ch%2)*0x80), 0x07);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0B+((ch%2)*0x80), 0x10);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2F+((ch%2)*0x80), 0x00);

		printk(">>>>> DRV[%s:%d] NVP6134_VI_4M / NVP6134_VI_4M_NRT COAXIAL PROTOCOL IS SETTING....\n", __func__, __LINE__ );
	}
	else if ( ch_mode_status[ch] == NVP6134_VI_5M_20P )
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7C, 0x01);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7D, 0x80);

		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+((ch%2)*0x80), 0x35);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x03+((ch%2)*0x80), 0x0E);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05+((ch%2)*0x80), 0x07);	// AHD 5M@20p(use Active line:8line)
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0D+((ch%2)*0x80), 0x88);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0E)+((ch%2)*0x80), 0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0A+((ch%2)*0x80), 0x07);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0B+((ch%2)*0x80), 0x10);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2F+((ch%2)*0x80), 0x00);

		printk(">>>>> DRV[%s:%d] NVP6134_VI_5M_20P COAXIAL PROTOCOL IS SETTING....\n", __func__, __LINE__ );
	}
	else
	{
		printk(">>>>> DRV[%s:%d] COAXIAL MODE NOT RIGHT...\n", __func__, __LINE__ );
	}
}


/*******************************************************************************
 *
 *
 *
 *  External Functions
 *
 *
 *
 *******************************************************************************/
/*******************************************************************************
*	Description		: set each acp
*	Argurments		: ch(channel ID),
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void acp_each_setting(unsigned char ch)
{
	int vidmode = 0;
	//unsigned char val, val1;
	
	/* mpp setting */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);		/*   - set band(1) */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA8+ch%4, (ch%4)<2?0x01:0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xBC+ch%4, (ch%2)==0?0x07:0x0F);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);//   - set bank(5)
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2F, 0x00);		//  (+) - internal MPP, HVF(Horizontal Vertical Field sync inversion option)
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30, 0x00);		// H sync start position
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x31, 0x43);		// H sync start position
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x32, 0xa2);		// H sync end position
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7C, 0x11);		// RX coax Input selection
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7D, 0x80);		// RX threshold

	/* set baud rate each format - TX */
	acp_set_baudrate(ch);

	/* a-cp setting - RX */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));		//   - set bank
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05+((ch%2)*0x80), 0x07);	// TX active Max line(8 line)
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x60+((ch%2)*0x80), 0x55);	// RX [coax header value]

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0b+((ch%2)*0x80), 0x10 );	// change coaxial mode

	vidmode = ch_mode_status[ch];
	if( vidmode == NVP6134_VI_1080P_2530 || vidmode == NVP6134_VI_720P_5060 || \
		vidmode == NVP6134_VI_720P_2530  || vidmode == NVP6134_VI_HDEX      || \
		vidmode == NVP6134_VI_3M_NRT     || vidmode == NVP6134_VI_3M		||
		vidmode == NVP6134_VI_4M_NRT     || vidmode == NVP6134_VI_4M		||
		vidmode == NVP6134_VI_5M_NRT	 || vidmode == NVP6134_VI_5M_20P )
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x66+((ch%2)*0x80), 0x80);	// RX Auto duty 
		if( vidmode == NVP6134_VI_3M_NRT || vidmode == NVP6134_VI_3M ||
			vidmode == NVP6134_VI_4M_NRT || vidmode == NVP6134_VI_4M ||
			vidmode == NVP6134_VI_5M_NRT || vidmode == NVP6134_VI_5M_20P )
		{
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x62+((ch%2)*0x80), 0x06);	// RX receive start line (Change AHD mode)
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x68+((ch%2)*0x80), 0x70);	// RX(Receive max line - 8 line, high-4bits)
		}
		else if( vidmode == NVP6134_VI_1080P_2530 )// 1080P
		{
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x62+((ch%2)*0x80), 0x06);	// RX receive start line
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x68+((ch%2)*0x80), 0x70);	// RX(Receive max line - 8 line, high-4bits)
		}
		else // 720P
		{
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x62+((ch%2)*0x80), ch_vfmt_status[ch]==PAL?0x05:0x05);	// RX receive start line
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x68+((ch%2)*0x80), 0x70);	// RX(Receive max line - 8 line, high-4bits)
		}
	}
	else if( vidmode == NVP6134_VI_EXC_1080P )
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x66+((ch%2)*0x80), 0x83);	// RX Auto duty 
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x62+((ch%2)*0x80), 0x16);	// RX receive start line (Change AHD mode)
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x68+((ch%2)*0x80), 0x30);	// RX(Receive max line - 4 line, high-4bits)

	}
	/* change Duty Type from Auto to Manual */
	else if( vidmode == NVP6134_VI_EXC_720P   || vidmode == NVP6134_VI_EXC_HDEX || \
			 vidmode == NVP6134_VI_EXC_720PRT )
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x66+((ch%2)*0x80), 0x03);	// RX Manual duty
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x62+((ch%2)*0x80), 0x16);	// RX receive start line (Change AHD mode)
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x68+((ch%2)*0x80), 0x30);	// RX(Receive max line - 4 line, high-4bits)
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x69+((ch%2)*0x80), ch_vfmt_status[ch]==PAL?0x2b:0x2c );	// RX Manual duty
	}
	else if( vidmode == NVP6134_VI_EXT_720PRT || vidmode == NVP6134_VI_EXT_1080P  || \
			 vidmode == NVP6134_VI_EXT_3M_NRT || vidmode == NVP6134_VI_EXT_5M_NRT )
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x66+((ch%2)*0x80), 0x83);	// RX Auto duty 
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x62+((ch%2)*0x80), 0x15);	// RX receive start line (Change AHD mode)
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x68+((ch%2)*0x80), 0x30);	// RX(Receive max line - 4 line, high-4bits)
	}
	/* change Duty Type from Auto to Manual */
	else if( vidmode == NVP6134_VI_EXT_720PA  || vidmode == NVP6134_VI_EXT_HDAEX  || \
			 vidmode == NVP6134_VI_EXT_720PB  || vidmode == NVP6134_VI_EXT_HDBEX  )
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x66+((ch%2)*0x80), 0x03);	// RX Manual duty
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x62+((ch%2)*0x80), 0x15);	// RX receive start line (Change AHD mode)
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x68+((ch%2)*0x80), 0x30);	// RX(Receive max line - 4 line, high-4bits)
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x69+((ch%2)*0x80), 0x2d);	// RX Manual duty value
	}

	/*********************************************************************
	 * recognize our code
	 *********************************************************************/

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x63+((ch%2)*0x80), 0x01);	// RX device(module) ON(1), OFF(0)
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+((ch%2)*0x80), 0x00);	// Delay count
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x67+((ch%2)*0x80), 0x01);	// RX(1:interrupt enable), (0:interrupt disable)

	acp_reg_rx_clear(ch);												// reset
}

/*******************************************************************************
*	Description		: read acp data of ISP
*	Argurments		: ch(channel ID), reg_addr(high[1byte]:bank, low[1byte]:register)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned char acp_isp_read(unsigned char ch, unsigned int reg_addr)
{
	unsigned int data;
	unsigned char bank;
	unsigned char addr;

	bank = (reg_addr>>8)&0xFF;
	addr = reg_addr&0xFF;

	init_acp_reg_rd(ch);
	get_acp_reg_rd(ch, bank, addr);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
	data = gpio_i2c_read(nvp6134_iic_addr[ch/4],ACP_RX_D0+3+((ch%2)*0x80));

	gpio_i2c_write( nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_OUT+((ch%2)*0x80), 0x10);
	gpio_i2c_write( nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_OUT+((ch%2)*0x80), 0x00);
	acp_reg_rx_clear(ch);
	//printk("acp_isp_read ch = %d, reg_addr = %x, reg_data = %x\n", ch,reg_addr, data);

	return data;
}

/*******************************************************************************
*	Description		: write data to ISP
*	Argurments		: ch(channel ID),reg_addr(high[1byte]:bank, low[1byte]:register)
*					  reg_data(data)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void acp_isp_write(unsigned char ch, unsigned int reg_addr, unsigned char reg_data)
{
	unsigned char bankaddr = 0x00;
	unsigned char device_id = 0x00;


	/* set coax RX device ID */
	bankaddr = (reg_addr>>8)&0xFF;
	if( bankaddr >= 0xB0 && bankaddr <= 0xB4 )
	{
		device_id = 0x55;
	}
	else
	{
		device_id = ACP_REG_WR;
	}

	/* write data to isp */
	gpio_i2c_write( nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
	gpio_i2c_write( nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_LINES+((ch%2)*0x80), 0x07);	// CH Active line(0x07->8Line)
	gpio_i2c_write( nvp6134_iic_addr[ch/4], ACP_MODE_ID+((ch%2)*0x80), ACP_REG_WR);
	gpio_i2c_write( nvp6134_iic_addr[ch/4], 0x10+((ch%2)*0x80), ACP_REG_WR);			// data1(#define ACP_AHD2_FHD_D0	0x10)
	gpio_i2c_write( nvp6134_iic_addr[ch/4], 0x11+((ch%2)*0x80), (reg_addr>>8)&0xFF);	// data2(bank)
	gpio_i2c_write( nvp6134_iic_addr[ch/4], 0x12+((ch%2)*0x80), reg_addr&0xFF);			// data3(address)
	gpio_i2c_write( nvp6134_iic_addr[ch/4], 0x13+((ch%2)*0x80), reg_data);				// data4(Don't care)
	gpio_i2c_write( nvp6134_iic_addr[ch/4], 0x60+((ch%2)*0x80), device_id);				// data4(DEVICE ID)
	gpio_i2c_write( nvp6134_iic_addr[ch/4], 0x09+((ch%2)*0x80), 0x08);					//   - pulse on(trigger)
	msleep(140);																		// sleep to recognize NRT(15fps) signal for ISP  (M)
	gpio_i2c_write( nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
	gpio_i2c_write( nvp6134_iic_addr[ch/4], 0x09+((ch%2)*0x80), 0x10);					// reset - pulse off
	gpio_i2c_write( nvp6134_iic_addr[ch/4], 0x09+((ch%2)*0x80), 0x00);					//   - pulse off

	#if 0 // debuggin
	printk(">>>>> CH:%d NORMAL, RX->TX : ", ch );
	printk("%02x ", ACP_REG_WR );
	printk("%02x ", (reg_addr>>8)&0xFF );
	printk("%02x ", reg_addr&0xFF );
	printk("%02x ", reg_data );
	printk("\n");
        #endif
}

/*******************************************************************************
*	Description		: send data to ISP(TX)
*	Argurments		: ch(channel), nvp6134_acp_rw_data_extention(acp data)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void acp_isp_write_extention( unsigned char ch, void *p_param )
{
	int i;
	unsigned char linecnt 	= 0x07; 	// Line count(0x07->8line)
	unsigned char AcpModeID = 0x55; 	// ACP Mode ID
	nvp6134_acp_rw_data_extention *pacpdata = (nvp6134_acp_rw_data_extention*)p_param;

	unsigned char curvidmode = ch_mode_status[ch];	/* Video format each channel */
	unsigned char vfmt = ch_vfmt_status[ch];		/* NTSC(0x00), PAL(0x01) */

	// adjust TX start line
	if( curvidmode == NVP6134_VI_HDEX)
	{
		gpio_i2c_write( nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));						// bank
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_BAUD+((ch%2)*0x80),0x2C);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_LINE+((ch%2)*0x80),vfmt==PAL? 0x0E:0x0E);
		printk(">>>>> CH:%d, NVP6134_VI_HDEX, TX start Line : 0x%x\n", ch, vfmt==PAL? 0x0E:0x0E );
	}
	else if( curvidmode == NVP6134_VI_720P_2530 )
	{
		gpio_i2c_write( nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));						// bank
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_BAUD+((ch%2)*0x80),0x15);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_LINE+((ch%2)*0x80),vfmt==PAL? 0x0D:0x0D);
		printk(">>>>> CH:%d, NVP6134_VI_720P_2530, TX start Line : 0x%x\n", ch, vfmt==PAL? 0x0D:0x0D );
	}

	/* send data to isp */
	gpio_i2c_write( nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));						// bank
	gpio_i2c_write( nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_LINES+((ch%2)*0x80), linecnt);	// CH Active line(0x07->8Line), ACP_AHD2_FHD_LINES(0x05)
	gpio_i2c_write( nvp6134_iic_addr[ch/4], ACP_MODE_ID+((ch%2)*0x80), AcpModeID);		// ACP MODE ID
	for( i = 0; i < linecnt+1; i++ )
	{
		gpio_i2c_write( nvp6134_iic_addr[ch/4], (0x10+i)+((ch%2)*0x80), pacpdata->data[i]);	// data
	}

	/* trigger on and off */
	gpio_i2c_write( nvp6134_iic_addr[ch/4], 0x09+((ch%2)*0x80), 0x08);					// pulse on(trigger)
	msleep(140);

	// status is trigger on.
	gpio_i2c_write( nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));						// bank
	gpio_i2c_write( nvp6134_iic_addr[ch/4], 0x09+((ch%2)*0x80), 0x10);					// reset - pulse off
	gpio_i2c_write( nvp6134_iic_addr[ch/4], 0x09+((ch%2)*0x80), 0x00);					// pulse off

    #if 0   // debuging
	printk(">>>>> CH:%d EXTENSION, RX->TX : ", ch );
	for( i = 0; i < 8; i++ )
		printk("%02x ", pacpdata->data[i] );
	printk("\n");
    #endif
}

/*******************************************************************************
*	Description		: read data from ISP
*	Argurments		: pvideoacp( channel's read information), ch(channel)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void acp_read(nvp6134_input_videofmt *pvideoacp, unsigned char ch)
{
	unsigned int buf[16];
	unsigned char val, i;

	/*
	 * check status and set/get information
	 */
	val = read_acp_status(ch);
	if(val == ACP_CAM_STAT)
	{
		for(i=0;i<8;i++)
		{
			buf[i] = gpio_i2c_read(nvp6134_iic_addr[ch/4],ACP_RX_D0+((ch%2)*0x80)+i);
			pvideoacp->getacpdata[ch][i] = buf[i];
		}

		#if 0
			/* for Debuging message [ISP<->DECODER] */
			if(buf[7] == 0x00)
				printk(">>>>> DRV[%s:%d] CH:%d, ACP_CAM_STATUS[STATUS MODE] = ", __func__, __LINE__, ch );
			else if(buf[7] == 0x01)
				printk(">>>>> DRV[%s:%d] CH:%d, ACP_CAM_STATUS[MOTION INFO] = ", __func__, __LINE__, ch );
			else if(buf[7] == 0x02)
				printk(">>>>> DRV[%s:%d] CH:%d, ACP_CAM_STATUS[FIRM UPGRADE] = ", __func__, __LINE__, ch );
			else if(buf[7] == 0x03)
				printk(">>>>> DRV[%s:%d] CH:%d, ACP_CAM_STATUS[FRAME RATE] = ", __func__, __LINE__, ch );
			printk("[%02x %02x %02x %02x %02x %02x %02x %02x]\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
		#endif

		/* control dec according to the isp information */
		acp_ctl_dec( ch, pvideoacp );
	}
	else if(val == ACP_REG_WR)
	{
		for(i=0;i<4;i++)
		{
			buf[i] = gpio_i2c_read(nvp6134_iic_addr[ch/4],ACP_RX_D0+((ch%2)*0x80)+i);
			pvideoacp->getacpdata[ch][i] = buf[i];
		}
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_OUT+((ch%2)*0x80), 0x00);
		//printk(">>>>> DRV[%s:%d] CH:%D, ACP_Write = 0x%02x 0x%02x 0x%02x 0x%02x\n", __func__, __LINE__, ch, buf[0], buf[1], buf[2]. buf[3]);
	}
	else if(val == ACP_REG_RD)
	{
		for(i=0;i<4;i++)
		{
			buf[i] = gpio_i2c_read(nvp6134_iic_addr[ch/4],ACP_RX_D0+((ch%2)*0x80)+i);
			pvideoacp->getacpdata[ch][i] = buf[i];
		}
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_OUT+((ch%2)*0x80), 0x00);
		//printk(">>>>> DRV[%s:%d] CH:%d, ACP_REG_RD, ACP_Read = 0x%02x 0x%02x 0x%02x 0x%02x\n", __func__, __LINE__, ch, buf[0], buf[1], buf[2], buf[3]);
	}
	else
	{
		for(i=0;i<8;i++)
		{
			pvideoacp->getacpdata[ch][i] = 0x00;
		}
		gpio_i2c_write(nvp6134_iic_addr[ch/4], ACP_AHD2_FHD_OUT+((ch%2)*0x80), 0x00);
		//printk(">>>>> DRV[%s:%d] CH:%d, ACP_RX_Error!!!!\n", __func__, __LINE__, ch );
	}
	acp_reg_rx_clear(ch);
}

/*******************************************************************************
*	Description		: set eq stage to camera
*	Argurments		: stage
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned char g_tuningmode = 0; // ISP Tune mode => 0:default, 1:indoor, 2:outdoor, 3:reserved
int acp_isp_write_eqstage( unsigned char ch, unsigned char stage, unsigned char vidmode )
{
	nvp6134_acp_rw_data_extention acpdata;

	if(stage <= 0)
		return -1;

	/* communicate each and set value */
	if( vidmode == NVP6134_VI_4M_NRT || vidmode == NVP6134_VI_4M    ||
		vidmode == NVP6134_VI_5M_NRT || vidmode == NVP6134_VI_5M_20P )
	{
		/* set buffer */
		memset( &acpdata, 0x00, sizeof(nvp6134_acp_rw_data_extention) );
		acpdata.ch = ch;

		/* ACP user protocol */
		acpdata.data[0] = 0x60;
		acpdata.data[1] = 0xB2;
		acpdata.data[2] = 0x03;
		acpdata.data[3] = ((g_tuningmode << 4) & 0xF0) | ( stage & 0x0F); // stage(1:short, 2:100m, 3:200m, 4:300m, 5:400m, 6:500m )

		/* Send EQ stage to camera */
		if( nvp6134_GetAgcLockStatus(ch) == 0x01 )
		{
			acp_isp_write_extention( acpdata.ch, &acpdata );
		}
	}

	return 0;
}

/*******************************************************************************
*	Description		: communicate each(dec<->ISP)
*	Argurments		: stage
*	Return value	: -2(failed communcation), stage(ok communication)
*	Modify			:
*	warning			:
*******************************************************************************/
int acp_isp_write_communcation( unsigned char ch, unsigned char stage, unsigned char vidmode )
{
	int ret = 0;
	int cnt = 0;
	int MaxCnt = 5;
	int recvcnt = 0;
	int breakflag = 0;
	unsigned char data1, data2, data3;
	nvp6134_acp_rw_data_extention acpdata;

	if(stage <= 0)
		return -1;

	if( vidmode == NVP6134_VI_3M_NRT || vidmode == NVP6134_VI_3M ||
		vidmode == NVP6134_VI_4M_NRT || vidmode == NVP6134_VI_4M    ||
		vidmode == NVP6134_VI_5M_NRT || vidmode == NVP6134_VI_5M_20P )
	{
		/* 1).initialize buffer */
		memset( &acpdata, 0x00, sizeof(nvp6134_acp_rw_data_extention) );
		acpdata.ch = ch;

		/* 2).Send ACP user command to ISP */
		acpdata.data[0] = 0x60;
		acpdata.data[1] = 0xB2;
		acpdata.data[2] = 0x03;
		acpdata.data[3] = ((g_tuningmode << 4) & 0xF0) | ( stage & 0x0F); // stage(1:short, 2:100m, 3:200m, 4:300m, 5:400m, 6:500m )
		acp_isp_write_extention( acpdata.ch, &acpdata );

		/* 3).Receive ACP user command from ISP */
		while( cnt++ < MaxCnt )
		{
			while( recvcnt++ < 250 )
			{
			gpio_i2c_write( nvp6134_iic_addr[ch/4], 0xFF, 0x03+((ch%4)/2));
			gpio_i2c_write( nvp6134_iic_addr[ch/4], 0x60+((ch%2)*0x80), 0x60);
			data1 = gpio_i2c_read( nvp6134_iic_addr[ch/4], 0x50+((ch%2)*0x80));
			data2 = gpio_i2c_read( nvp6134_iic_addr[ch/4], 0x51+((ch%2)*0x80));
			data3 = gpio_i2c_read( nvp6134_iic_addr[ch/4], 0x52+((ch%2)*0x80));
			if( (data1 == acpdata.data[0]) && (data2 == acpdata.data[1]) && (data3 == acpdata.data[2]) )
			{
				printk(">>>>> DRV[%s:%d] CH:%d, Send and Receive OK\n", __func__, __LINE__, ch );
					breakflag = 1;
				break;
			}
			}

			if( breakflag == 1 )
			{
				break;
			}
			else
			{
				printk(">>>>> DRV[%s:%d] CH:%d, Send and Receive Failed\n", __func__, __LINE__, ch );

				/* reset decoder */
				nvp6134_VD_chnRst(ch);
				msleep(30);

				/* send again */
				acp_isp_write_extention( acpdata.ch, &acpdata );

				/* clear recv count */
				recvcnt = 0;

			}
			msleep(1);
		}

		/* 4).decision result of communication */
		ret = ( MaxCnt < cnt ) ? -2 : 0;
		if( ret == -2 )
		{
			nvp6134_dump_reg( ch, 0x03 );
			nvp6134_dump_reg( ch, 0x04 );
		}
	}

	return ret;
}

/*******************************************************************************
*	Description		: control decoder according to the ISP information(NVP2475H)
*	Argurments		: ch(channel), p_param(ISP information)
*	Return value	: void
*	Modify			:
*	warning			: N/A
*******************************************************************************/
#define DAY_NIGHT_MODE_BW_OFF	0x00
#define DAY_NIGHT_MODE_BW_ON	0x01

unsigned char s_transflag[16] = {DAY_NIGHT_MODE_BW_OFF, };
void acp_ctl_dec( unsigned char ch, void *p_param )
{
	unsigned char vidmode;
	unsigned char bwinfo;
	unsigned char stage;
	nvp6134_input_videofmt *pvideoacp = (nvp6134_input_videofmt*)p_param;

	vidmode = ch_mode_status[ch];
	if( vidmode == NVP6134_VI_3M_NRT || vidmode == NVP6134_VI_3M ||
		vidmode == NVP6134_VI_4M_NRT || vidmode == NVP6134_VI_4M    ||
		vidmode == NVP6134_VI_5M_NRT || vidmode == NVP6134_VI_5M_20P )
	{
		/* control dec according to ISP information */
		bwinfo = (pvideoacp->getacpdata[ch][1]>>1)&0x01;
		stage  = s_eq.ch_stage[ch];
		if( bwinfo == DAY_NIGHT_MODE_BW_OFF && s_transflag[ch] != DAY_NIGHT_MODE_BW_OFF )
		{
			/* Normal mode(tune) */
			switch( stage )
			{
				case 1:	break;
				case 2: break;
				case 3: break;
				case 4: break;
				case 5: break;
				case 6: break;
				default: break;
			}

			/* set flag */
			s_transflag[ch] = DAY_NIGHT_MODE_BW_OFF;
			printk(">>>>> DRV[%s:%d] CH:%d, stage:%d, Change mode BW OFF\n", __func__, __LINE__, ch, stage );
		}
		else if( bwinfo == DAY_NIGHT_MODE_BW_ON && s_transflag[ch] != DAY_NIGHT_MODE_BW_ON )
		{
			/* BW mode(tune) */
			switch( stage )
			{
				case 1:	break;
				case 2: break;
				case 3: break;
				case 4: break;
				case 5: break;
				case 6: break;
				default: break;
			}

			/* set flag */
			s_transflag[ch] = DAY_NIGHT_MODE_BW_ON;
			printk(">>>>> DRV[%s:%d] CH:%d, stage:%d, Change mode BW ON\n", __func__, __LINE__, ch, stage );
		}
	}

	#if 0 // debuging mode
	int i = 0;
	printk(">>>>> DRV[%s:%d] CH:%d, [", __func__, __LINE__, ch );
	for(i = 0; i < 8; i++ )
		printk("%02x ", pvideoacp->getacpdata[ch][i]);
	printk("]\n");
	#endif

	return;
}

/*******************************************************************************
*	End of file
*******************************************************************************/
