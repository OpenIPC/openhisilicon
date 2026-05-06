/********************************************************************************
*
*  Copyright (C) 2017 	NEXTCHIP Inc. All rights reserved.
*  Module		: The decoder's coax module
*  Description	: Coaxial
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
#include "coax.h"
#include "acp.h"

extern int chip_id[4];
extern unsigned int nvp6134_cnt;
extern unsigned char ch_mode_status[16];
extern unsigned char ch_vfmt_status[16];
extern unsigned int nvp6134_iic_addr[4];

/**************************************************************************
	Function prototype
**************************************************************************/
void nvp6134_send_comm(unsigned char pel_ch, bool stop);
void nvp6134_presend_comm(unsigned char pel_ch);
unsigned char nvp6134_coax_command(unsigned char pel_ch, unsigned char command);

extern int chip_id[4];

/*******************************************************************************
*	Description		: set each coaxial mode
*	Argurments		: pel_ch( channel )
*	Return value	: VOID
*	Modify			:
*	warning			: // by song(2016-06-29)
*******************************************************************************/
void __nvp6134_set_each_coax_mode(unsigned char pel_ch )
{
	int vidmode = 0;
	int ch = pel_ch ;

	vidmode = ch_mode_status[ch];

	gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0xFF, 0x03+((pel_ch%4)/2));

	if(vidmode < NVP6134_VI_720P_2530)
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_MODE+((pel_ch%2)*0x80), 0x06);
		printk(">>>>> DRV[%s:%d] Change mode CVBS\n", __func__, __LINE__ );
	}
	else if( vidmode == NVP6134_VI_1080P_2530 || vidmode == NVP6134_VI_720P_5060 || \
		vidmode == NVP6134_VI_3M_NRT|| vidmode == NVP6134_VI_3M )
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_MODE+((pel_ch%2)*0x80), 0x10);
		printk(">>>>> DRV[%s:%d] Change mode AHD\n", __func__, __LINE__ );
	}
	#ifdef AHD_PELCO_16BIT
	else if(vidmode == NVP6134_VI_720P_2530  || vidmode == NVP6134_VI_HDEX)
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_MODE+((pel_ch%2)*0x80), 0x06);
		printk(">>>>> DRV[%s:%d] Change mode AHD@16BIT\n", __func__, __LINE__ );
	}
	#else
	else if(vidmode == NVP6134_VI_720P_2530  || vidmode == NVP6134_VI_HDEX)
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_MODE+((pel_ch%2)*0x80), 0x10);
		printk(">>>>> DRV[%s:%d] Change mode AHD@8BIT\n", __func__, __LINE__ ); 
	}
	#endif
	else if( vidmode == NVP6134_VI_EXC_720P   || vidmode == NVP6134_VI_EXC_HDEX || \
			 vidmode == NVP6134_VI_EXC_720PRT || vidmode == NVP6134_VI_EXC_1080P )
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_MODE+((pel_ch%2)*0x80), 0x40);
		printk(">>>>> DRV[%s:%d] Change mode CHD\n", __func__, __LINE__ );
	}
	else if( vidmode == NVP6134_VI_EXT_720PA  || vidmode == NVP6134_VI_EXT_HDAEX  || \
			 vidmode == NVP6134_VI_EXT_720PB  || vidmode == NVP6134_VI_EXT_HDBEX  || \
			 vidmode == NVP6134_VI_EXT_720PRT || vidmode == NVP6134_VI_EXT_1080P  || \
			 vidmode == NVP6134_VI_EXT_3M_NRT || vidmode == NVP6134_VI_EXT_5M_NRT )
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_MODE+((pel_ch%2)*0x80), 0x08);
		printk(">>>>> DRV[%s:%d] Change mode THD\n", __func__, __LINE__ );
	}
	else 
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_MODE+((pel_ch%2)*0x80), 0x10);
		printk(">>>>> DRV[%s:%d] Default Change mode AHD\n", __func__, __LINE__ );
	}
}

unsigned char nvp6134_coax_command(unsigned char pel_ch, unsigned char command)
{
	int i;
	unsigned char str[8];
	unsigned char str_exc[14];
	unsigned char str_ext[10];

	__nvp6134_set_each_coax_mode( pel_ch );
	
	
	msleep( 20 );	

	if(	ch_mode_status[pel_ch] == NVP6134_VI_1080P_2530 || 
		ch_mode_status[pel_ch] == NVP6134_VI_3M			|| 
		ch_mode_status[pel_ch] == NVP6134_VI_3M_NRT		||
		ch_mode_status[pel_ch] == NVP6134_VI_4M_NRT		||
		ch_mode_status[pel_ch] == NVP6134_VI_4M			||
		ch_mode_status[pel_ch] == NVP6134_VI_5M_NRT		||
		ch_mode_status[pel_ch] == NVP6134_VI_5M_20P)
	{
		switch(command)
		{
			case  PELCO_CMD_RESET :
				str[0] = 0x00;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_SET:
				str[0] = 0x02;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_UP:			
				str[0] = 0x00;str[1] = 0x08;str[2] = 0x00;str[3] = 0x32;
			break;
			case  PELCO_CMD_DOWN:			
				str[0] = 0x00;str[1] = 0x10;str[2] = 0x00;str[3] = 0x32;
			break;
			case  PELCO_CMD_LEFT:			
				str[0] = 0x00;str[1] = 0x04;str[2] = 0x32;str[3] = 0x00;
			break;
			case  PELCO_CMD_RIGHT:			
				str[0] = 0x00;str[1] = 0x02;str[2] = 0x32;str[3] = 0x00;
			break;
			case  PELCO_CMD_OSD:			
				str[0] = 0x00;str[1] = 0x03;str[2] = 0x00;str[3] = 0x5F;
			break;
			case  PELCO_CMD_IRIS_OPEN:
				str[0] = 0x02;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_IRIS_CLOSE:	
				str[0] = 0x04;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_FOCUS_NEAR:	
				str[0] = 0x01;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_FOCUS_FAR:		
				str[0] = 0x00;str[1] = 0x80;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_ZOOM_WIDE:		
				str[0] = 0x00;str[1] = 0x40;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_ZOOM_TELE:		
				str[0] = 0x00;str[1] = 0x20;str[2] = 0x00;str[3] = 0x00;
			break;

			default : return 0; //unexpected command
		}		
	}
	#ifdef AHD_PELCO_16BIT
	else if( ch_mode_status[pel_ch] == NVP6134_VI_720P_2530 ||	ch_mode_status[pel_ch] == NVP6134_VI_HDEX )
	{
		switch(command)
		{
			case  PELCO_CMD_RESET :
				str[0] = 0x00;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_SET:
				str[0] = 0x40;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_UP:			
				str[0] = 0x00;str[1] = 0x10;str[2] = 0x10;str[3] = 0x4C;
			break;
			case  PELCO_CMD_DOWN:				
				str[0] = 0x00;str[1] = 0x08;str[2] = 0x08;str[3] = 0x4C;
			break;
			case  PELCO_CMD_LEFT:				
				str[0] = 0x00;str[1] = 0x20;str[2] = 0x20;str[3] = 0x00;
			break;
			case  PELCO_CMD_RIGHT:				
				str[0] = 0x00;str[1] = 0x40;str[2] = 0x40;str[3] = 0x00;
			break;
			case  PELCO_CMD_OSD:				
				str[0] = 0x00;str[1] = 0xC0;str[2] = 0xC0;str[3] = 0xFA;
			break;
			case  PELCO_CMD_IRIS_OPEN:
				str[0] = 0x40;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_IRIS_CLOSE:		
				str[0] = 0x20;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_FOCUS_NEAR:		
				str[0] = 0x80;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_FOCUS_FAR:				
				str[0] = 0x00;str[1] = 0x01;str[2] = 0x01;str[3] = 0x00;
			break;
			case  PELCO_CMD_ZOOM_WIDE:				
				str[0] = 0x00;str[1] = 0x02;str[2] = 0x02;str[3] = 0x00;
			break;
			case  PELCO_CMD_ZOOM_TELE:				
				str[0] = 0x00;str[1] = 0x04;str[2] = 0x04;str[3] = 0x00;
			break;
			case  PELCO_CMD_SCAN_SR:			
				str[0] = 0x00;str[1] = 0xE0;str[2] = 0xE0;str[3] = 0x46;
			break;
			case  PELCO_CMD_SCAN_ST:			
				str[0] = 0x00;str[1] = 0xE0;str[2] = 0xE0;str[3] = 0x00;
			break;
			case  PELCO_CMD_PRESET1:			
				str[0] = 0x00;str[1] = 0xE0;str[2] = 0xE0;str[3] = 0x80;
			break;
			case  PELCO_CMD_PRESET2:		
				str[0] = 0x00;str[1] = 0xE0;str[2] = 0xE0;str[3] = 0x40;
			break;
			case  PELCO_CMD_PRESET3:			
				str[0] = 0x00;str[1] = 0xE0;str[2] = 0xE0;str[3] = 0xC0;
			break;
			case  PELCO_CMD_PTN1_SR:			
				str[0] = 0x00;str[1] = 0xF8;str[2] = 0xF8;str[3] = 0x01;
			break;
			case  PELCO_CMD_PTN1_ST:			
				str[0] = 0x00;str[1] = 0x84;str[2] = 0x84;str[3] = 0x01;
			break;
			case  PELCO_CMD_PTN2_SR:			
				str[0] = 0x00;str[1] = 0xF8;str[2] = 0xF8;str[3] = 0x02;
			break;
			case  PELCO_CMD_PTN2_ST:			
				str[0] = 0x00;str[1] = 0x84;str[2] = 0x84;str[3] = 0x02;
			break;
			case  PELCO_CMD_PTN3_SR:			
				str[0] = 0x00;str[1] = 0xF8;str[2] = 0xF8;str[3] = 0x03;
			break;
			case  PELCO_CMD_PTN3_ST:			
				str[0] = 0x00;str[1] = 0x84;str[2] = 0x84;str[3] = 0x03;
			break;
			case  PELCO_CMD_RUN:				
				str[0] = 0x00;str[1] = 0xC4;str[2] = 0xC4;str[3] = 0x00;
			break;

			default : return 0; //unexpected command
		}		
	}
	#else
	else if( ch_mode_status[pel_ch] == NVP6134_VI_720P_2530 ||	ch_mode_status[pel_ch] == NVP6134_VI_HDEX )
	{
		switch(command)
		{
			case  PELCO_CMD_RESET :
				str[0] = 0x00;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_SET:
				str[0] = 0x02;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_UP:			
				str[0] = 0x00;str[1] = 0x08;str[2] = 0x00;str[3] = 0x32;
			break;
			case  PELCO_CMD_DOWN:			
				str[0] = 0x00;str[1] = 0x10;str[2] = 0x00;str[3] = 0x32;
			break;
			case  PELCO_CMD_LEFT:			
				str[0] = 0x00;str[1] = 0x04;str[2] = 0x32;str[3] = 0x00;
			break;
			case  PELCO_CMD_RIGHT:			
				str[0] = 0x00;str[1] = 0x02;str[2] = 0x32;str[3] = 0x00;
			break;
			case  PELCO_CMD_OSD:			
				str[0] = 0x00;str[1] = 0x03;str[2] = 0x00;str[3] = 0x5F;
			break;
			case  PELCO_CMD_IRIS_OPEN:
				str[0] = 0x02;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_IRIS_CLOSE:	
				str[0] = 0x04;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_FOCUS_NEAR:	
				str[0] = 0x01;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_FOCUS_FAR:		
				str[0] = 0x00;str[1] = 0x80;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_ZOOM_WIDE:		
				str[0] = 0x00;str[1] = 0x40;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_ZOOM_TELE:		
				str[0] = 0x00;str[1] = 0x20;str[2] = 0x00;str[3] = 0x00;
			break;

			default : return 0; //unexpected command
		}		
	}
	#endif
	else if(ch_mode_status[pel_ch] == NVP6134_VI_EXC_720P || ch_mode_status[pel_ch] == NVP6134_VI_EXC_HDEX ||
			ch_mode_status[pel_ch] == NVP6134_VI_EXC_720PRT || ch_mode_status[pel_ch] == NVP6134_VI_EXC_1080P)
	{
		printk("[%s:%d] command %d", __func__, __LINE__, command);
		switch(command)
		{
			case EXC_CMD_OSD_UP:
			str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x81;str_exc[5] = 0x91;str_exc[6] = 0x81;str_exc[7] = 0x20;str_exc[8] = 0x80;str_exc[9] = 0x00;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x80;str_exc[13] = 0xCC;
			break;
			case EXC_CMD_OSD_DOWN:
			str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x81;str_exc[5] = 0x91;str_exc[6] = 0x80;str_exc[7] = 0xA0;str_exc[8] = 0x80;str_exc[9] = 0x00;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x81;str_exc[13] = 0x2C;
			break;
			case EXC_CMD_OSD_LEFT:
			str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x81;str_exc[5] = 0x91;str_exc[6] = 0x80;str_exc[7] = 0x60;str_exc[8] = 0x80;str_exc[9] = 0x00;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x80;str_exc[13] = 0xAC;
			break;
			case EXC_CMD_OSD_RIGHT:
			str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x81;str_exc[5] = 0x91;str_exc[6] = 0x81;str_exc[7] = 0xE0;str_exc[8] = 0x80;str_exc[9] = 0x00;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x80;str_exc[13] = 0x6C;
			break;
			
			case  EXC_CMD_RESET :
				str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x80;str_exc[5] = 0x00;str_exc[6] = 0x80;str_exc[7] = 0x00;str_exc[8] = 0x80;str_exc[9] = 0x00;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x80;str_exc[13] = 0x65;
			break;
			case  EXC_CMD_SET: //normal camera osd enter command
				str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x80;str_exc[5] = 0x0a;str_exc[6] = 0x80;str_exc[7] = 0x00;str_exc[8] = 0x80;str_exc[9] = 0x00;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x80;str_exc[13] = 0x6f;
			break;
			case  EXC_CMD_UP:
				str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x81;str_exc[5] = 0x10;str_exc[6] = 0x80;str_exc[7] = 0x00;str_exc[8] = 0x80;str_exc[9] = 0xf9;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x80;str_exc[13] = 0xb2;
			break;
			case  EXC_CMD_DOWN:
				str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x81;str_exc[5] = 0x20;str_exc[6] = 0x80;str_exc[7] = 0x00;str_exc[8] = 0x80;str_exc[9] = 0xf9;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x81;str_exc[13] = 0x92;
			break;
			case  EXC_CMD_LEFT:
				str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x81;str_exc[5] = 0x40;str_exc[6] = 0x80;str_exc[7] = 0xf9;str_exc[8] = 0x80;str_exc[9] = 0x00;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x80;str_exc[13] = 0xe2;
			break;
			case  EXC_CMD_RIGHT:
				str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x81;str_exc[5] = 0x80;str_exc[6] = 0x80;str_exc[7] = 0xf9;str_exc[8] = 0x80;str_exc[9] = 0x00;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x81;str_exc[13] = 0x62;
			break;
			case  EXC_CMD_OSD_ENTER:
				str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x81;str_exc[5] = 0x91;str_exc[6] = 0x81;str_exc[7] = 0x10;str_exc[8] = 0x80;str_exc[9] = 0x00;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x81;str_exc[13] = 0xEC;
			break;
			case EXC_CMD_OSD_CLOSE:
				str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x81;str_exc[5] = 0x91;str_exc[6] = 0x80;str_exc[7] = 0x00;str_exc[8] = 0x80;str_exc[9] = 0x00;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x81;str_exc[13] = 0xF4;
			break;
			case EXC_CMD_OSD_OPEN:
				str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x81;str_exc[5] = 0x91;str_exc[6] = 0x81;str_exc[7] = 0x80;str_exc[8] = 0x80;str_exc[9] = 0x00;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x80;str_exc[13] = 0x0C;
			break;
			case  EXC_CMD_IRIS_OPEN:
				str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x80;str_exc[5] = 0x0A;str_exc[6] = 0x80;str_exc[7] = 0x00;str_exc[8] = 0x80;str_exc[9] = 0x00;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x80;str_exc[13] = 0x6f;
			break;
			case  EXC_CMD_IRIS_CLOSE:
				str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x80;str_exc[5] = 0x06;str_exc[6] = 0x80;str_exc[7] = 0x00;str_exc[8] = 0x80;str_exc[9] = 0x00;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x80;str_exc[13] = 0x60;
			break;
			case  EXC_CMD_FOCUS_NEAR:
				str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x80;str_exc[5] = 0x12;str_exc[6] = 0x80;str_exc[7] = 0x00;str_exc[8] = 0x80;str_exc[9] = 0x00;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x80;str_exc[13] = 0x77;
			break;
			case  EXC_CMD_FOCUS_FAR:
				str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x80;str_exc[5] = 0x22;str_exc[6] = 0x80;str_exc[7] = 0x00;str_exc[8] = 0x80;str_exc[9] = 0x00;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x81;str_exc[13] = 0x57;
			break;
			case  EXC_CMD_ZOOM_WIDE:
				str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x80;str_exc[5] = 0x42;str_exc[6] = 0x80;str_exc[7] = 0x00;str_exc[8] = 0x80;str_exc[9] = 0x00;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x80;str_exc[13] = 0x17;
			break;
			case  EXC_CMD_ZOOM_TELE:
				str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x80;str_exc[5] = 0x82;str_exc[6] = 0x80;str_exc[7] = 0x00;str_exc[8] = 0x80;str_exc[9] = 0x00;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x80;str_exc[13] = 0xe7;
			break;
			case  EXC_CMD_ZOOM_STOP :
  				str_exc[0] = 0x80;str_exc[1] = 0xa5;str_exc[2] = 0x81;str_exc[3] = 0x80;str_exc[4] = 0x81;str_exc[5] = 0x02;str_exc[6] = 0x80;str_exc[7] = 0x00;str_exc[8] = 0x80;str_exc[9] = 0x00;str_exc[10] = 0x80;str_exc[11] = 0x00;str_exc[12] = 0x81;str_exc[13] = 0x67;
			break;
			default : return 0; //add ext cmd
		}
	}
	else if(ch_mode_status[pel_ch] == NVP6134_VI_EXT_720PA  || ch_mode_status[pel_ch] == NVP6134_VI_EXT_HDAEX ||
			ch_mode_status[pel_ch] == NVP6134_VI_EXT_720PB  || ch_mode_status[pel_ch] == NVP6134_VI_EXT_HDBEX ||
			ch_mode_status[pel_ch] == NVP6134_VI_EXT_720PRT || ch_mode_status[pel_ch] ==  NVP6134_VI_EXT_1080P||
			ch_mode_status[pel_ch] == NVP6134_VI_EXT_3M_NRT || ch_mode_status[pel_ch] == NVP6134_VI_EXT_5M_NRT )
	{
		switch(command)
		{
			case  EXT_CMD_RESET :
				str_ext[0] = 0xb5;str_ext[1] = 0x00;str_ext[2] = 0x14;str_ext[3] = 0x00;str_ext[4] = 0x80;str_ext[5] = 0x00;str_ext[6] = 0x00;str_ext[7] = 0x00;str_ext[8] = 0xc9;str_ext[9] = 0x80;
			break;
			case  EXT_CMD_SET:
				str_ext[0] = 0xb5;str_ext[1] = 0x00;str_ext[2] = 0x0f;str_ext[3] = 0x00;str_ext[4] = 0x80;str_ext[5] = 0x00;str_ext[6] = 0x00;str_ext[7] = 0x00;str_ext[8] = 0xc4;str_ext[9] = 0x80;
			break;
			case  EXT_CMD_UP:
				str_ext[0] = 0xb5;str_ext[1] = 0x00;str_ext[2] = 0x06;str_ext[3] = 0x24;str_ext[4] = 0x80;str_ext[5] = 0x00;str_ext[6] = 0x00;str_ext[7] = 0x00;str_ext[8] = 0xdf;str_ext[9] = 0x80;
			break;
			case  EXT_CMD_DOWN:
				str_ext[0] = 0xb5;str_ext[1] = 0x00;str_ext[2] = 0x07;str_ext[3] = 0x24;str_ext[4] = 0x80;str_ext[5] = 0x00;str_ext[6] = 0x00;str_ext[7] = 0x00;str_ext[8] = 0xe0;str_ext[9] = 0x80;
			break;
			case  EXT_CMD_LEFT:
				str_ext[0] = 0xb5;str_ext[1] = 0x00;str_ext[2] = 0x09;str_ext[3] = 0x00;str_ext[4] = 0x80;str_ext[5] = 0x24;str_ext[6] = 0x00;str_ext[7] = 0x00;str_ext[8] = 0xe2;str_ext[9] = 0x80;
			break;
			case  EXT_CMD_RIGHT:
				str_ext[0] = 0xb5;str_ext[1] = 0x00;str_ext[2] = 0x08;str_ext[3] = 0x00;str_ext[4] = 0x80;str_ext[5] = 0x24;str_ext[6] = 0x00;str_ext[7] = 0x00;str_ext[8] = 0xe1;str_ext[9] = 0x80;
			break;
			case  EXT_CMD_OSD:
				str_ext[0] = 0xb5;str_ext[1] = 0x00;str_ext[2] = 0x0f;str_ext[3] = 0x00;str_ext[4] = 0x80;str_ext[5] = 0x00;str_ext[6] = 0x00;str_ext[7] = 0x00;str_ext[8] = 0xc4;str_ext[9] = 0x80;
			break;
			case  EXT_CMD_IRIS_OPEN:
				str_ext[0] = 0xb5;str_ext[1] = 0x00;str_ext[2] = 0x0f;str_ext[3] = 0x00;str_ext[4] = 0x80;str_ext[5] = 0x00;str_ext[6] = 0x00;str_ext[7] = 0x00;str_ext[8] = 0xc4;str_ext[9] = 0x80;
			break;
			case  EXT_CMD_IRIS_CLOSE:
				str_ext[0] = 0xb5;str_ext[1] = 0x00;str_ext[2] = 0x0e;str_ext[3] = 0x00;str_ext[4] = 0x80;str_ext[5] = 0x00;str_ext[6] = 0x00;str_ext[7] = 0x00;str_ext[8] = 0xc3;str_ext[9] = 0x80;
			break;
			case  EXT_CMD_FOCUS_NEAR:
				str_ext[0] = 0xb5;str_ext[1] = 0x00;str_ext[2] = 0x10;str_ext[3] = 0x00;str_ext[4] = 0x80;str_ext[5] = 0x00;str_ext[6] = 0x00;str_ext[7] = 0x00;str_ext[8] = 0xc5;str_ext[9] = 0x80;
			break;
			case  EXT_CMD_FOCUS_FAR:
				str_ext[0] = 0xb5;str_ext[1] = 0x00;str_ext[2] = 0x11;str_ext[3] = 0x00;str_ext[4] = 0x80;str_ext[5] = 0x00;str_ext[6] = 0x00;str_ext[7] = 0x00;str_ext[8] = 0xc6;str_ext[9] = 0x80;
			break;
			case  EXT_CMD_ZOOM_WIDE:
				str_ext[0] = 0xb5;str_ext[1] = 0x00;str_ext[2] = 0x12;str_ext[3] = 0x00;str_ext[4] = 0x80;str_ext[5] = 0x00;str_ext[6] = 0x00;str_ext[7] = 0x00;str_ext[8] = 0xc7;str_ext[9] = 0x80;
			break;
			case  EXT_CMD_ZOOM_TELE:
				str_ext[0] = 0xb5;str_ext[1] = 0x00;str_ext[2] = 0x13;str_ext[3] = 0x00;str_ext[4] = 0x80;str_ext[5] = 0x00;str_ext[6] = 0x00;str_ext[7] = 0x00;str_ext[8] = 0xc8;str_ext[9] = 0x80;
			break;
			case  EXT_CMD_MOTOR_STOP:
				str_ext[0] = 0xb5;str_ext[1] = 0x00;str_ext[2] = 0x14;str_ext[3] = 0x00;str_ext[4] = 0x80;str_ext[5] = 0x00;str_ext[6] = 0x00;str_ext[7] = 0x00;str_ext[8] = 0xc9;str_ext[9] = 0x80;
			break;
			case  EXT_CMD_VER_SWITCH:
				str_ext[0] = 0xb5;str_ext[1] = 0x00;str_ext[2] = 0x87;str_ext[3] = 0x00;str_ext[4] = 0x80;str_ext[5] = 0x00;str_ext[6] = 0x01;str_ext[7] = 0x01;str_ext[8] = 0x3E;str_ext[9] = 0x80;
			break;
			default : return 0; 
		}
	}
	else
	{
		switch(command)
		{
			case  PELCO_CMD_RESET :
				str[0] = 0x00;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_SET:
				str[0] = 0x40;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_UP:
				str[0] = 0x00;str[1] = 0x10;str[2] = 0x00;str[3] = 0x4C;
			break;
			case  PELCO_CMD_DOWN:
				str[0] = 0x00;str[1] = 0x08;str[2] = 0x00;str[3] = 0x4C;
			break;
			case  PELCO_CMD_LEFT:
				str[0] = 0x00;str[1] = 0x20;str[2] = 0x4C;str[3] = 0x00;
			break;
			case  PELCO_CMD_RIGHT:
				str[0] = 0x00;str[1] = 0x40;str[2] = 0x4C;str[3] = 0x00;
			break;
			case  PELCO_CMD_OSD:
				str[0] = 0x00;str[1] = 0xC0;str[2] = 0x00;str[3] = 0xFA;
			break;
			case  PELCO_CMD_IRIS_OPEN:
				str[0] = 0x40;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_IRIS_CLOSE:
				str[0] = 0x20;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_FOCUS_NEAR:
				str[0] = 0x80;str[1] = 0x00;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_FOCUS_FAR:
				str[0] = 0x00;str[1] = 0x01;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_ZOOM_WIDE:
				str[0] = 0x00;str[1] = 0x02;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_ZOOM_TELE:
				str[0] = 0x00;str[1] = 0x04;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_SCAN_SR:
				str[0] = 0x00;str[1] = 0xE0;str[2] = 0x00;str[3] = 0x46;
			break;
			case  PELCO_CMD_SCAN_ST:
				str[0] = 0x00;str[1] = 0xE0;str[2] = 0x00;str[3] = 0x00;
			break;
			case  PELCO_CMD_PRESET1:
				str[0] = 0x00;str[1] = 0xE0;str[2] = 0x00;str[3] = 0x80;
			break;
			case  PELCO_CMD_PRESET2:
				str[0] = 0x00;str[1] = 0xE0;str[2] = 0x00;str[3] = 0x40;
			break;
			case  PELCO_CMD_PRESET3:
				str[0] = 0x00;str[1] = 0xE0;str[2] = 0x00;str[3] = 0xC0;
			break;
			case  PELCO_CMD_PTN1_SR:
				str[0] = 0x00;str[1] = 0xF8;str[2] = 0x00;str[3] = 0x01;
			break;
			case  PELCO_CMD_PTN1_ST:
				str[0] = 0x00;str[1] = 0x84;str[2] = 0x00;str[3] = 0x01;
			break;
			case  PELCO_CMD_PTN2_SR:
				str[0] = 0x00;str[1] = 0xF8;str[2] = 0x00;str[3] = 0x02;
			break;
			case  PELCO_CMD_PTN2_ST:
				str[0] = 0x00;str[1] = 0x84;str[2] = 0x00;str[3] = 0x02;
			break;
			case  PELCO_CMD_PTN3_SR:
				str[0] = 0x00;str[1] = 0xF8;str[2] = 0x00;str[3] = 0x03;
			break;
			case  PELCO_CMD_PTN3_ST:
				str[0] = 0x00;str[1] = 0x84;str[2] = 0x00;str[3] = 0x03;
			break;
			case  PELCO_CMD_RUN:
				str[0] = 0x00;str[1] = 0xC4;str[2] = 0x00;str[3] = 0x00;
			break;

			default : return 0;
		}
	}

	gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0xFF, 0x03+((pel_ch%4)/2));
		
	if(	(ch_mode_status[pel_ch] < NVP6134_VI_720P_2530) )   //cvbs ģʽ
	{
		for(i=0;i<4;i++)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_PEL_D0+i+(((pel_ch)%2)*0x80), 0x00);
		}
		nvp6134_presend_comm(pel_ch);

		for(i=0;i<4;i++)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_PEL_D0+i+(((pel_ch)%2)*0x80), str[i]);
		}
		nvp6134_send_comm(pel_ch, 1);
		printk("\r\nPelco protocl shout!");
	}
	else if( ch_mode_status[pel_ch] == NVP6134_VI_720P_5060)
	{
		for(i=0;i<4;i++)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_PEL_D0+i+(((pel_ch)%2)*0x80), 0x00);
		}

		nvp6134_presend_comm(pel_ch);


		for(i=0;i<4;i++)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_PEL_D0+i+(((pel_ch)%2)*0x80), str[i]);
		}
		nvp6134_send_comm(pel_ch, 1);
		printk("\r\nNVP6134_VI_HDEX/5060 Coaxial protocl shout!");
	}
	//else if(ch_mode_status[pel_ch] == NVP6124_VI_1080P_2530 || ch_mode_status[pel_ch] == NVP6124_VI_720P_2530 || ch_mode_status[pel_ch] == NVP6124_VI_3M_2530)
	#ifdef AHD_PELCO_16BIT
	else if( ch_mode_status[pel_ch] == NVP6134_VI_720P_2530 ||ch_mode_status[pel_ch] == NVP6134_VI_HDEX )
	{
		for(i=0;i<4;i++)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_PEL_D0+i+(((pel_ch)%2)*0x80), 0x00);
		}

		nvp6134_presend_comm(pel_ch);

		for(i=0;i<4;i++)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_PEL_D0+i+(((pel_ch)%2)*0x80), str[i]);
		}
		nvp6134_send_comm(pel_ch, 1);
		printk("\r\nNVP6134_VI_HDEX/2530 Coaxial protocl shout!");
	}
	#else
	else if( ch_mode_status[pel_ch] == NVP6134_VI_720P_2530 ||ch_mode_status[pel_ch] == NVP6134_VI_HDEX )
	{
		for(i=0;i<4;i++)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_D0+i+(((pel_ch)%2)*0x80), 0x00);
		}

		nvp6134_presend_comm(pel_ch);

		for(i=0;i<4;i++)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_D0+i+(((pel_ch)%2)*0x80), str[i]);
		}

		nvp6134_send_comm(pel_ch, 1);
		printk("\r\nNVP6134_VI_HDEX/2530 Coaxial protocl shout!");
	}
	#endif
	else if(ch_mode_status[pel_ch] == NVP6134_VI_1080P_2530 || 
			ch_mode_status[pel_ch] == NVP6134_VI_3M			||
			ch_mode_status[pel_ch] == NVP6134_VI_3M_NRT	)
	{
		for(i=0;i<4;i++)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_D0+i+(((pel_ch)%2)*0x80), 0x00);
		}

		nvp6134_presend_comm(pel_ch);

		for(i=0;i<4;i++)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_D0+i+(((pel_ch)%2)*0x80), str[i]);
		}

		nvp6134_send_comm(pel_ch, 1);
		printk("\r\nNVP6134_VI_1080P/720P/3M/3M_NRT Coaxial protocl shout!");
	}
	else if(ch_mode_status[pel_ch] == NVP6134_VI_4M_NRT		||
			ch_mode_status[pel_ch] == NVP6134_VI_4M			||
			ch_mode_status[pel_ch] == NVP6134_VI_5M_NRT		||
			ch_mode_status[pel_ch] == NVP6134_VI_5M_20P		)
	{
		for(i=0;i<8;i++)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_D0+i+(((pel_ch)%2)*0x80), 0x00);
		}

		nvp6134_presend_comm(pel_ch);

		for(i=0;i<8;i++)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_D0+i+(((pel_ch)%2)*0x80), str[i]);
		}

		nvp6134_send_comm(pel_ch, 1);
		printk("\r\nNVP6134_VI_4M_NRT/4M/5M_NRT/5M_20P Coaxial protocl shout!");
	}
	else if(ch_mode_status[pel_ch] == NVP6134_VI_EXC_720P || ch_mode_status[pel_ch] == NVP6134_VI_EXC_HDEX ||
			ch_mode_status[pel_ch] == NVP6134_VI_EXC_720PRT || ch_mode_status[pel_ch] == NVP6134_VI_EXC_1080P)
	{
		for(i=0;i<12;i++)
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_D0+i+(((pel_ch)%2)*0x80), str_exc[i]);

		nvp6134_presend_comm(pel_ch);

		//if (ch_mode_status[pel_ch] == NVP6134_VI_EXC_720PRT)
		//	(ch_vfmt_status[pel_ch]==PAL? msleep(60):msleep(10));
		//else
		//	msleep( 30 );

		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x10+((pel_ch%2)*0x80), str_exc[12]);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x11+((pel_ch%2)*0x80), str_exc[13]);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x12+((pel_ch%2)*0x80), 0xc1);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x13+((pel_ch%2)*0x80), 0xff);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x14+((pel_ch%2)*0x80), 0xc1);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x15+((pel_ch%2)*0x80), 0xff);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x16+((pel_ch%2)*0x80), 0xc1);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x17+((pel_ch%2)*0x80), 0xff);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x18+((pel_ch%2)*0x80), 0xc1);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x19+((pel_ch%2)*0x80), 0xff);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x1a+((pel_ch%2)*0x80), 0xc1);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x1b+((pel_ch%2)*0x80), 0xff);

		nvp6134_send_comm(pel_ch, 1);

		printk("\r\nEXC Coaxial protocl shout!");
	}
	else if(ch_mode_status[pel_ch] == NVP6134_VI_EXT_720PA || ch_mode_status[pel_ch] == NVP6134_VI_EXT_HDAEX ||
			ch_mode_status[pel_ch] == NVP6134_VI_EXT_720PB || ch_mode_status[pel_ch] == NVP6134_VI_EXT_HDBEX ||
			ch_mode_status[pel_ch] == NVP6134_VI_EXT_720PRT || ch_mode_status[pel_ch] == NVP6134_VI_EXT_1080P ||
			ch_mode_status[pel_ch] == NVP6134_VI_EXT_3M_NRT || ch_mode_status[pel_ch] == NVP6134_VI_EXT_5M_NRT )
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0xff, 0x03+((pel_ch%4)/2));

		for(i=0;i<10;i++)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_D0+i+(((pel_ch)%2)*0x80), str_ext[i]);
			printk(" \r str[%d] :: 0x%x \n", i, str_ext[i]);
		}
#if 0
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x09+((pel_ch%2)*0x80), 0x08);

		nvp6134_presend_comm(pel_ch);

		msleep( 30 );

		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x10+((pel_ch%2)*0x80), 0xB5);  //stop command is optional
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x11+((pel_ch%2)*0x80), 0x00);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x12+((pel_ch%2)*0x80), 0x14);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x13+((pel_ch%2)*0x80), 0x00);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x14+((pel_ch%2)*0x80), 0x80);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x15+((pel_ch%2)*0x80), 0x00);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x16+((pel_ch%2)*0x80), 0x00);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x17+((pel_ch%2)*0x80), 0x00);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x18+((pel_ch%2)*0x80), 0xC9);

		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x09+((pel_ch%2)*0x80), 0x08);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0x09+((pel_ch%2)*0x80), 0x00);
#endif
		nvp6134_send_comm(pel_ch, 1);

		printk("\r\next Coaxial protocl shout!");

	}
	return 1;
}

void nvp6134_send_comm(unsigned char pel_ch, bool stop)
{
	gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0xFF, 0x03+((pel_ch%4)/2));
	if((ch_mode_status[pel_ch] <  NVP6134_VI_720P_2530))
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_PEL_OUT+(((pel_ch)%2)*0x80), 0x01);
		msleep( 30 );
		if(stop == 1)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_PEL_OUT+(((pel_ch)%2)*0x80), 0x10);
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_PEL_OUT+(((pel_ch)%2)*0x80), 0x00);
			msleep( 30 );
		}
	}
	else if(ch_mode_status[pel_ch] == NVP6134_VI_EXC_720P	|| ch_mode_status[pel_ch] == NVP6134_VI_EXC_HDEX ||
			ch_mode_status[pel_ch] == NVP6134_VI_EXC_HDEX	|| ch_mode_status[pel_ch] == NVP6134_VI_EXC_720PRT ||
			ch_mode_status[pel_ch] == NVP6134_VI_EXC_1080P)
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x08);
		if (ch_mode_status[pel_ch] == NVP6134_VI_EXC_720PRT)
			(ch_vfmt_status[pel_ch]==PAL? msleep(60):msleep(10));
		else
			msleep( 25 );
		if(stop == 1)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x10);
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x00);
			if (ch_mode_status[pel_ch] == NVP6134_VI_EXC_720PRT)
				(ch_vfmt_status[pel_ch]==PAL? msleep(60):msleep(10));
			else
				msleep( 25 );
		}
	}
	else if(ch_mode_status[pel_ch] == NVP6134_VI_3M)
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x08);
		msleep( 25 );
		if(stop == 1)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x10);
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x00);
			msleep( 25 );
		}
	}
	else if(ch_mode_status[pel_ch] == NVP6134_VI_EXT_720PA || ch_mode_status[pel_ch] == NVP6134_VI_EXT_HDAEX ||
			ch_mode_status[pel_ch] == NVP6134_VI_EXT_720PB || ch_mode_status[pel_ch] == NVP6134_VI_EXT_HDBEX ||
			ch_mode_status[pel_ch] == NVP6134_VI_EXT_720PRT|| ch_mode_status[pel_ch] == NVP6134_VI_EXT_1080P)
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x08);
		msleep( 25 );
		if(stop == 1)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x10);
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x00);
			msleep( 25 );
		}
	}
	#ifdef AHD_PELCO_16BIT
	else if(ch_mode_status[pel_ch] == NVP6134_VI_720P_2530||ch_mode_status[pel_ch] == NVP6134_VI_HDEX)
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_PEL_OUT+(((pel_ch)%2)*0x80), 0x01);
		msleep( 70 );
		if(stop == 1)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_PEL_OUT+(((pel_ch)%2)*0x80), 0x10);
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_PEL_OUT+(((pel_ch)%2)*0x80), 0x00);
			msleep( 30 );
		}
	}
	#endif
	else if( ch_mode_status[pel_ch] == NVP6134_VI_3M_NRT || ch_mode_status[pel_ch] == NVP6134_VI_5M_20P)  
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x08);
		msleep( 55 );
		if(stop == 1)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x10);		
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x00);		
		}
	}
	else if( ch_mode_status[pel_ch] == NVP6134_VI_4M_NRT )  
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x08);
		msleep( 70 );
		if(stop == 1)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x10);		
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x00);		
		}
	}
	else if( ch_mode_status[pel_ch] == NVP6134_VI_5M_NRT)
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x08);
		msleep( 90 );
		if(stop == 1)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x10);		// reset - by song(2016-07-01)
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x00);		// clear - by song(2016-07-01)
		}
	}
	else 
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x08);
		msleep( 25 );
		if(stop == 1)
		{
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x10);
			gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x00);
			msleep( 25 );
		}
	}

	/* change AHD mode */
	gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0xFF, 0x03+((pel_ch%4)/2));
	gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_MODE+((pel_ch)*0x80), 0x10);
	printk(">>>>> DRV[%s:%d] Normaly, Change mode, AHD\n", __func__, __LINE__ );

}

void nvp6134_presend_comm(unsigned char pel_ch)
{
	gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], 0xFF, 0x03+((pel_ch%4)/2));
	if((ch_mode_status[pel_ch] < NVP6134_VI_720P_2530))
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_PEL_OUT+(((pel_ch)%2)*0x80), 0x01);
		msleep( 30 );
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_PEL_OUT+(((pel_ch)%2)*0x80), 0x00);
	}	
	else if( ch_mode_status[pel_ch] == NVP6134_VI_720P_5060)
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_PEL_OUT+(((pel_ch)%2)*0x80), 0x01);
		msleep( 25 );
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_PEL_OUT+(((pel_ch)%2)*0x80), 0x00);
	}
	#ifdef AHD_PELCO_16BIT
	else if( ch_mode_status[pel_ch] == NVP6134_VI_720P_2530||ch_mode_status[pel_ch] == NVP6134_VI_HDEX)
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_PEL_OUT+(((pel_ch)%2)*0x80), 0x01);
		msleep( 30 );
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_PEL_OUT+(((pel_ch)%2)*0x80), 0x00);
	}
	#endif
	else 
	{
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x08);

		if (ch_mode_status[pel_ch] == NVP6134_VI_EXC_720PRT)
			(ch_vfmt_status[pel_ch]==PAL? msleep(60):msleep(10));
		else
			msleep( 25 );
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x10);
		gpio_i2c_write(nvp6134_iic_addr[pel_ch/4], AHD2_FHD_OUT+(((pel_ch)%2)*0x80), 0x00);
	}
}


/********************************************************************************
* End of file
********************************************************************************/
