/********************************************************************************
*
*  Copyright (C) 2017 	NEXTCHIP Inc. All rights reserved.
*  Module		: The decoder's video format module
*  Description	: Video format
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

#include "eq_common.h"
#include "video.h"
#include "coax.h"
#include "acp.h"
#include "eq.h"

/*******************************************************************************
 * extern variable
 *******************************************************************************/
extern unsigned int nvp6134_cnt;
extern int chip_id[4];
extern unsigned int g_vloss;
extern unsigned int nvp6134_iic_addr[4];
unsigned char g_ch_video_fmt[16] = {[0 ... 15] = 0xFF};	// save user's video format
extern unsigned char det_mode[16];

//#define THD_3MNRT_HSCALER  	//THD 1920*1536==>2048*1536

/*nvp6134 3M 色彩推荐配置*/
#define BRI_CENTER_VAL_NTSC_3M 0x00
#define BRI_CENTER_VAL_PAL_3M  0x00
#define CON_CENTER_VAL_NTSC_3M 0x8B
#define CON_CENTER_VAL_PAL_3M  0x8B
#define SAT_CENTER_VAL_NTSC_3M 0x85
#define SAT_CENTER_VAL_PAL_3M  0x85                       
#define HUE_CENTER_VAL_NTSC_3M 0xFE
#define HUE_CENTER_VAL_PAL_3M  0xFE

/*nvp6134 1080P 色彩推荐配置*/
#define BRI_CENTER_VAL_NTSC 0x02
#define BRI_CENTER_VAL_PAL  0x02
#define CON_CENTER_VAL_NTSC 0x98
#define CON_CENTER_VAL_PAL  0x98
#define SAT_CENTER_VAL_NTSC 0x84
#define SAT_CENTER_VAL_PAL  0x84                       
#define HUE_CENTER_VAL_NTSC 0x00
#define HUE_CENTER_VAL_PAL  0x00


/*nvp6134 720P 色彩推荐配置*/
#define BRI_CENTER_VAL_NTSC_720P 0x08
#define BRI_CENTER_VAL_PAL_720P  0x08
#define CON_CENTER_VAL_NTSC_720P 0x88
#define CON_CENTER_VAL_PAL_720P  0x88
#define SAT_CENTER_VAL_NTSC_720P 0x84
#define SAT_CENTER_VAL_PAL_720P  0x84                       
#define HUE_CENTER_VAL_NTSC_720P 0x00
#define HUE_CENTER_VAL_PAL_720P  0x00
                           
/*nvp6134 960H 色彩推荐配置*/
#define BRI_CENTER_VAL_NTSC_960H 0x08
#define BRI_CENTER_VAL_PAL_960H  0x08
#define CON_CENTER_VAL_NTSC_960H 0x88
#define CON_CENTER_VAL_PAL_960H  0x88
#define SAT_CENTER_VAL_NTSC_960H 0x84
#define SAT_CENTER_VAL_PAL_960H  0x84                       
#define HUE_CENTER_VAL_NTSC_960H 0x00
#define HUE_CENTER_VAL_PAL_960H  0x00

/*******************************************************************************
 * internal variable
 *******************************************************************************/
unsigned int nvp6134_con_tbl_3M[2]  = {CON_CENTER_VAL_NTSC_3M, CON_CENTER_VAL_PAL_3M};
unsigned int nvp6134_hue_tbl_3M[2]  = {HUE_CENTER_VAL_NTSC_3M, HUE_CENTER_VAL_PAL_3M};
unsigned int nvp6134_sat_tbl_3M[2]  = {SAT_CENTER_VAL_NTSC_3M, SAT_CENTER_VAL_PAL_3M};
unsigned int nvp6134_bri_tbl_3M[2]  = {BRI_CENTER_VAL_NTSC_3M, BRI_CENTER_VAL_PAL_3M};

unsigned int nvp6134_con_tbl[2]  = {CON_CENTER_VAL_NTSC, CON_CENTER_VAL_PAL};
unsigned int nvp6134_hue_tbl[2]  = {HUE_CENTER_VAL_NTSC, HUE_CENTER_VAL_PAL};
unsigned int nvp6134_sat_tbl[2]  = {SAT_CENTER_VAL_NTSC, SAT_CENTER_VAL_PAL};
unsigned int nvp6134_bri_tbl[2]  = {BRI_CENTER_VAL_NTSC, BRI_CENTER_VAL_PAL};

unsigned int nvp6134_con_tbl_720P[2]  = {CON_CENTER_VAL_NTSC_720P, CON_CENTER_VAL_PAL_720P};
unsigned int nvp6134_hue_tbl_720P[2]  = {HUE_CENTER_VAL_NTSC_720P, HUE_CENTER_VAL_PAL_720P};
unsigned int nvp6134_sat_tbl_720P[2]  = {SAT_CENTER_VAL_NTSC_720P, SAT_CENTER_VAL_PAL_720P};
unsigned int nvp6134_bri_tbl_720P[2]  = {BRI_CENTER_VAL_NTSC_720P, BRI_CENTER_VAL_PAL_720P};


unsigned int nvp6134_con_tbl_960H[2]  = {CON_CENTER_VAL_NTSC_960H, CON_CENTER_VAL_PAL_960H};
unsigned int nvp6134_hue_tbl_960H[2]  = {HUE_CENTER_VAL_NTSC_960H, HUE_CENTER_VAL_PAL_960H};
unsigned int nvp6134_sat_tbl_960H[2]  = {SAT_CENTER_VAL_NTSC_960H, SAT_CENTER_VAL_PAL_960H};
unsigned int nvp6134_bri_tbl_960H[2]  = {BRI_CENTER_VAL_NTSC_960H, BRI_CENTER_VAL_PAL_960H};

unsigned char nvp6134_motion_sens_tbl[8]= {0xe0,0xc8,0xa0,0x98,0x78,0x68,0x50,0x48};
unsigned char ch_mode_status[16]={[0 ... 15]=0xff};
unsigned char ch_vfmt_status[16]={[0 ... 15]=0xff};

void nvp6134_dump_reg( unsigned char ch, unsigned char bank )
{
	int tmp = 0;
	int i = 0, j= 0;

	printk("***************IIC ADDR 0x%02x - CH[%02d] *****************\r\n", nvp6134_iic_addr[ch/4], ch );
	printk("***************Chip[0x%02x] Bank[0x%x]*****************\r\n", nvp6134_iic_addr[ch/4], bank );
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, bank );
	for (i = 0;i<=0xF;i++)
	{
		if(i==0)
		{
			printk("     0x%02x ",i);
		}
		else if (i==0xF)
		{
			printk("0x%02x\r\n",i);
		}
		else
		{
			printk("0x%02x ",i);
		}
	}
	for (i = 0;i<=0xF;i++)
	{
		for(j = 0;j<=0xF;j++)
		{
			tmp = gpio_i2c_read(nvp6134_iic_addr[ch/4], (i<<4)|j);
			if (j==0)
			{
				printk("0x%02x-0x%02x ",(i<<4)|j,tmp);
			}
			else if (j==0xF)
			{
				printk("0x%02x\r\n",tmp);
			}
			else
			{
				printk("0x%02x ",tmp);
			}
		}
	}
}

unsigned char nvp6134_video_get_adcclk(unsigned char ch)
{
	unsigned char adc_value;

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	adc_value = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x84+ch%4);
	printk(">>>>> DRV[%s:%d] CH:%d, Bank:0x%02x, ADC clock delay:0x%x\n", __func__, __LINE__, ch, nvp6134_iic_addr[ch/4], adc_value );
	return adc_value;
}

void nvp6134_video_set_adcclk(unsigned char ch, unsigned char value)
{
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4, value);
	printk(">>>>> DRV[%s:%d] CH:%d, Bank:0x%02x, ADC clock delay:0x%x\n", __func__, __LINE__, ch, nvp6134_iic_addr[ch/4], value );
}

void NVP6134_set_afe(unsigned char ch, unsigned char onoff)
{
	unsigned char afe_value;
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
		afe_value = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x00+ch%4);
		if(onoff==1)
			CLE_BIT(afe_value, 0);
		else
			SET_BIT(afe_value, 0);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+ch%4, afe_value);
		msleep(10);
		printk("NVP6134_set_afe ch[%d] [%s] done\n", ch, onoff?"ON":"OFF");
	}	
}

void nvp6134_datareverse(unsigned char chip, unsigned char port)
{
/*
BANK1 0xCB[3:0],每个bit控制一个bt656的数据顺序，1为反序，0为正序。
*/
	unsigned char tmp;
	gpio_i2c_write(nvp6134_iic_addr[chip], 0xFF, 0x01);
	tmp = gpio_i2c_read(nvp6134_iic_addr[chip], 0xCB);
	SET_BIT(tmp, port);
	gpio_i2c_write(nvp6134_iic_addr[chip], 0xCB, tmp); 
	printk("nvp6134[%d] port[%d] data reversed\n", chip, port);
}

void nvp6134_system_init(unsigned char chip)
{
	gpio_i2c_write(nvp6134_iic_addr[chip], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[chip], 0x80, 0x0F);

	gpio_i2c_write(nvp6134_iic_addr[chip], 0xFF, 0x01); 
	if(chip_id[chip] == NVP6134B_R0_ID)
		gpio_i2c_write(nvp6134_iic_addr[chip], 0xCA, 0x66);  	//NVP6134C/6134B ONLY HAS 2 PORTS
	else
		gpio_i2c_write(nvp6134_iic_addr[chip], 0xCA, 0xFF);		//NVP6134 HAS 4 PORTS

	printk("nvp6134[C]_system_init\n");
}

/*******************************************************************************
*	Description		: Initialize common value of AHD
*	Argurments		: dec(slave address)
*	Return value	: rev ID
*	Modify			:
*	warning			:
*******************************************************************************/
void nvp6134_common_init(unsigned char chip)
{	
	int ch;
	/* initialize chip */
	nvp6134_system_init(chip);

	for(ch=0;ch<4;ch++)
	{
		gpio_i2c_write(nvp6134_iic_addr[chip], 0xFF, 0x03+(ch%4)/2);
		gpio_i2c_write(nvp6134_iic_addr[chip], 0x6B+0x80*(ch%2), 0x00);

		init_acp(ch);
	}
}

unsigned char trans_ahd_to_chd( unsigned char vfmt )
{
	unsigned char format;

	/* mapping video type between AHD and CHD */
	if( vfmt == 0x04 )
	{
		format = 0x11;
	}
	else if( vfmt == 0x08 )
	{
		format = 0x12;
	}
	else if( vfmt == 0x10 )
	{
		format = 0x51;
	}
	else if( vfmt == 0x20 )
	{
		format = 0x52;
	}
	else if( vfmt == 0x40 )
	{
		format = 0x71;
	}
	else if( vfmt == 0x80 )
	{
		format = 0x72;
	}
	else
		format = vfmt;

	return format;
}

unsigned char trans_chd_to_ahd( unsigned char vfmt )
{
	unsigned char format;

	/* mapping video type between AHD and CHD */
	if( vfmt == 0x11 )
	{
		format = 0x04;
	}
	else if( vfmt == 0x12 )
	{
		format = 0x08;
	}
	else if( vfmt == 0x51 )
	{
		format = 0x10;
	}
	else if( vfmt == 0x52 )
	{
		format = 0x20;
	}
	else if( vfmt == 0x71 )
	{
		format = 0x40;
	}
	else if( vfmt == 0x72 )
	{
		format = 0x80;
	}
	else
		format = vfmt;

	return format;
}

int isItAHDmode( unsigned char vfmt )
{
	int ret = 0;
	if( vfmt==0x04 || vfmt==0x08 || vfmt==0x10 || vfmt==0x20 || vfmt==0x40 || vfmt==0x80 )
	{
		ret = 1;
	}
	return ret;
}

int isItCHDmode( unsigned char vfmt )
{
	int ret = 0;
	if( vfmt==0x11 || vfmt==0x12 || vfmt==0x51 || vfmt==0x52 || vfmt==0x71 || vfmt==0x72 )
	{
		ret = 1;
	}
	return ret;
}

int isItTHDmode( unsigned char vfmt )
{
	int ret = 0;
	if( vfmt==0x31 || vfmt==0x32 || vfmt==0x34 || vfmt==0x38 || vfmt==0x54 || vfmt==0x58 || vfmt==0x74 || vfmt==0x78 || vfmt==0x93 )
	{
		ret = 1;
	}
	return ret;
}

/*nvp6134b视频模式值转换*/
unsigned char nvp6134_vfmt_convert(unsigned char vdsts, unsigned char g_ck_fmt)
{
	unsigned int ret;
	switch(vdsts)
	{
		case 0x00:  ret = 0x01;	break;  //cvbs ntsc
		case 0x10:  ret = 0x02; break;	//cvbs pal
		case 0x20:	ret = 0x04; break;	//720p ntsc
 		case 0x21:  ret = 0x08; break;	//720p pal
		case 0x22:  ret = 0x51; break;	//720P@RT ntsc
		case 0x23:  ret = 0x52; break;	//720P@RT pal
		case 0x30:  ret = 0x40;	break;	//1080p ntsc
		case 0x31:	ret = 0x80;	break;	//1080p pal
		case 0x2B:  ret = 0x11; break;  //HD exc  @ 30P
		case 0x2C:  ret = 0x12; break;  //HD EXC  @ 25P
		case 0x25:  ret = 0x31; break;  //HD EXT  @ 30A
		case 0x26:  ret = 0x32; break;  //HD EXT  @ 24A
		case 0x29:  ret = 0x34; break;  //HD EXT  @ 30B
		case 0x2A:  ret = 0x38; break;  //HD EXT  @ 25B
		case 0x2D:  ret = 0x51; break;  //HD EXC  @ 60P
		case 0x2E:  ret = 0x52; break;  //HD EXC  @ 50P
		case 0x27:  ret = 0x54; break;  //HD EXT  @ 60
		case 0x28:  ret = 0x58; break;  //HD EXT  @ 50
		case 0x35:  ret = 0x71; break;  //FHD EXC @ 30P
		case 0x36:  ret = 0x72; break;  //FHD EXC @ 25P
		case 0x33:  ret = 0x74; break;  //FHD EXT @ 30P
		case 0x34:  ret = 0x78; break;  //FHD EXT @ 25P
//		case 0x3F:  ret = 0x44; break;  //FHD NRT @ 30P
		case 0x40:  ret = 0x81; break;  //QHD AHD @ 30P
		case 0x41:  ret = 0x82; break;  //QHD AHD @ 25P
		case 0x4F:  ret = 0x83; break;  //QHD AHD @ NRT(15P)
		case 0xA0:  ret = 0xA0; break;  //5M AHD @ 12.5P
		case 0xA1:	ret = 0xA1;	break;	//5M 20P
		case 0xA2:	ret = 0xA2;	break;	//EXT @ 5M @ 12.5p
		case 0x03:
		case 0x04:	ret = 0x90;	break;	//AHD @ 3M NRT-18p
		case 0x64:	ret = 0x93;	break;	//EXT @ 3M NRT-18p
		case 0x01:
		case 0x02:
			if((g_ck_fmt>>4) == 0x02)	ret = 0x91;	//AHD @ 3M RT-30P
			else 						ret = 0x92;	//AHD @ 3M RT-25P
		break;	
		default:
		case 0xFF:	ret = 0x00;	break;	//not detects
	}
	return ret;
}
unsigned int  s_fmt_set_done = 0;
unsigned int  g_eq_set_done = 0;
unsigned char s_fmt_dbnc_cnt[16]  = {[0 ... 15] = 0x00};
unsigned char s_fmt_dbnc_buf0[16] = {[0 ... 15] = 0x00};
unsigned char s_fmt_dbnc_buf1[16] = {[0 ... 15] = 0x00};
unsigned char s_fmt_dbnc_buf2[16] = {[0 ... 15] = 0x00};

unsigned char video_fmt_debounce(unsigned char ch, unsigned char keep_fmt, unsigned int keep_sync_width)
{
	unsigned char reg_vfmt_F0 = 0;
	unsigned char reg_vfmt_F2=0;
	unsigned char reg_vfmt_F3=0;
	unsigned char reg_vfmt_F4=0;
	unsigned char reg_vfmt_F5=0;
	unsigned char vfmt=0xFF;
	unsigned char ret_vfmt=0;
	unsigned int acc_gain_val=0;
	unsigned int y_p_slp = 0;
	unsigned int y_m_slp = 0;
		

	/* get video format */
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
		reg_vfmt_F0 = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xF0);
		reg_vfmt_F2 = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xF2);
		reg_vfmt_F3 = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xF3);
		reg_vfmt_F4 = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xF4);
		reg_vfmt_F5 = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xF5);
	
	/* 20:AHD@30p, 2B:CHD@30p, 30:AFHD@30p, 35:CFHD@30p */
		if( (reg_vfmt_F0 != 0xFF) && ( (keep_fmt == 0x20) || (keep_fmt == 0x2B) ||
								   (keep_fmt == 0x30) || (keep_fmt == 0x35) ))
		{
		/* 21:AHD@25p, 2C:CHD@25p, 31:AFHD@25p, 36:CFHD@25p */
		if( (reg_vfmt_F0 == 0x21) || (reg_vfmt_F0 == 0x2C) ||
			(reg_vfmt_F0 == 0x31) || (reg_vfmt_F0 == 0x36))
			{
				vfmt = 0xFF;
				printk(">>>>> DRV[%s:%d] CH:%d, 30->25P confuse. return novideo.\n", __func__, __LINE__, ch);
				return ret_vfmt;
			}
			else
			{
				y_p_slp		 = GetYPlusSlope(ch);
				y_m_slp		 = GetYMinusSlope(ch);
				acc_gain_val = distinguish_GetAccGain(ch);

			/* 30:AFHD@30p, 35:CFHD@30p */
				if((keep_fmt == 0x30) || (keep_fmt == 0x35)	)
				{
					if( (acc_gain_val < 2010) || (y_p_slp < 80) || ( y_m_slp < 80) )
					{
						vfmt = keep_fmt;
					printk(">>>>> DRV[%s:%d] CH:%d, AHD/CVI 1080p Distinguish. acc gain&Y_slp is OKAY[acc:%d,yp:%d,ym:%d]\n", \
								__func__, __LINE__, ch, acc_gain_val, y_p_slp, y_m_slp);
					}
					else
					{
						//@@todo. sync width.
						vfmt = 0xFF;
					printk(">>>>> DRV[%s:%d] CH:%d, AHD/CVI 1080p Distinguish. acc gain&Y_slp is NG[acc:%d,yp:%d,ym:%d]\n", \
								__func__, __LINE__, ch, acc_gain_val, y_p_slp, y_m_slp);
					}
				}
			/* 20:AHD@30p, 2B:CHD@30p */
				else
				{
				if(acc_gain_val < 2010)
					{
						vfmt = keep_fmt;
					printk(">>>>> DRV[%s:%d] CH:%d, AHD/CVI 720p Distinguish. acc gain&Y_slp is OKAY[acc:%d,yp:%d,ym:%d]\n", \
								__func__, __LINE__, ch, acc_gain_val, y_p_slp, y_m_slp);
					}
					else
					{
						//@@todo. sync width.
						vfmt = 0xFF;
					printk(">>>>> DRV[%s:%d] CH:%d, AHD/CVI 720p Distinguish. acc gain&Y_slp is NG[acc:%d,yp:%d,ym:%d]\n", \
								__func__, __LINE__, ch, acc_gain_val, y_p_slp, y_m_slp);
					}
				}
			}
		}
	/* 21:AHD@25p, 2C:CHD@25p, 31:AFHD@25p, 36:CFHD@25p */
		else if( (reg_vfmt_F0 != 0xFF) && ( (keep_fmt == 0x21) || (keep_fmt == 0x2C) ||
											(keep_fmt == 0x31) || (keep_fmt == 0x36) ) 	)
		{
		/* 20:AHD@30p, 2B:CHD@30p, 30:AFHD@30p, 35:CFHD@30p */
		if( (reg_vfmt_F0 == 0x20) || (reg_vfmt_F0 == 0x2B) ||
				(reg_vfmt_F0 == 0x30) || (reg_vfmt_F0 == 0x35)   )
			{
				vfmt = 0xFF;
			printk(">>>>> DRV[%s:%d] CH:%d, 25->30P confuse. return novideo. \n", __func__, __LINE__, ch);
				return ret_vfmt;
			}
			else
			{
				y_p_slp		 = GetYPlusSlope(ch);
				y_m_slp		 = GetYMinusSlope(ch);
				acc_gain_val = distinguish_GetAccGain(ch);

			/* 31:AFHD@25p, 36:CFHD@25p */
				if( (keep_fmt == 0x31) || (keep_fmt == 0x36)	)
				{
					if( (acc_gain_val < 2010) || (y_p_slp < 80) || ( y_m_slp < 80) )
					{
						vfmt = keep_fmt;
					printk(">>>>> DRV[%s:%d] CH:%d, AHD/CVI 1080p Distinguish. acc gain&Y_slp is OKAY[acc:%d,yp:%d,ym:%d]\n", \
								__func__, __LINE__, ch,acc_gain_val, y_p_slp, y_m_slp);
					}
					else
					{
						//@@todo. sync width.
						vfmt = 0xFF;
					printk(">>>>> DRV[%s:%d] CH:%d, AHD/CVI 1080p Distinguish. acc gain&Y_slp is NG[acc:%d,yp:%d,ym:%d]\n", \
								__func__, __LINE__, ch, acc_gain_val, y_p_slp, y_m_slp);
					}
				}
			/* 20:AHD@30p, 2B:CHD@30p */
				else
				{
				if(acc_gain_val < 2010)
					{
						vfmt = keep_fmt;
					printk(">>>>> DRV[%s:%d] CH:%d, AHD/CVI 720p Distinguish. acc gain&Y_slp is OKAY[acc:%d,yp:%d,ym:%d]\n", \
								__func__, __LINE__, ch, acc_gain_val, y_p_slp, y_m_slp);
					}
					else
					{
						//@@todo. sync width.
						vfmt = 0xFF;
					printk(">>>>> DRV[%s:%d] CH:%d, AHD/CVI 720p Distinguish. acc gain&Y_slp is NG[acc:%d,yp:%d,ym:%d]\n", \
								__func__, __LINE__, ch, acc_gain_val, y_p_slp, y_m_slp);
					}
				}
			}
	}
	/* AHD 3/4(NRT)/5, EXT 3/5 NRT */
		else
		{
		/* AHD 3M(RT/NRT) detection */
		if (reg_vfmt_F0 == 0x6F)
			{
				vfmt = reg_vfmt_F3;
			}
		/* AHD 4M NRT detection */
		else if (reg_vfmt_F0 == 0x4F)
		{
			vfmt = reg_vfmt_F0;
		}
		/* AHD 5M detection */
		else if (reg_vfmt_F0 == 0x7F)
			{
			/* 5M 12_5P */
			if (reg_vfmt_F3 == 0x04)
						vfmt = 0xA0;
			/* 5M 20P */
			else if (reg_vfmt_F3 == 0x02)
						vfmt = 0xA1;
			}
		/* EXT 3M 18P */
		else if( (reg_vfmt_F5 == 0x06) && ((reg_vfmt_F4 == 0x30) || (reg_vfmt_F4 == 0x31)) )
			{
			vfmt = 0x64;
			}
		/* EXT  5M 12.5P */
			else if( (reg_vfmt_F5 == 0x07) && ((reg_vfmt_F4 == 0xD0) || (reg_vfmt_F4 == 0xD1)) )	//TVI  5M vcnt 0xF0 == 0xFF
			{
			vfmt = 0xA2;
			}
			else if( reg_vfmt_F0 == 0xFF)
			{
				vfmt = 0xFF;
			printk(">>>>> DRV[%s:%d] CH:%d, format is novideo.[%x]", __func__, __LINE__, ch, reg_vfmt_F0);
			}
			else
			{
				vfmt = keep_fmt;
			}
		}

	/* save video format each buffer of verification */
	ret_vfmt = nvp6134_vfmt_convert(vfmt, reg_vfmt_F2);
	if(s_fmt_dbnc_cnt[ch] == 0)
		s_fmt_dbnc_buf0[ch] = ret_vfmt;
	else if(s_fmt_dbnc_cnt[ch] == 1)
		s_fmt_dbnc_buf1[ch] = ret_vfmt;
	else if(s_fmt_dbnc_cnt[ch] == 2)
		s_fmt_dbnc_buf2[ch] = ret_vfmt;
	else
		printk(">>>>> DRV[%s:%d] CH:%d, s_fmt_dbnc_cnt range out!!!\n", __func__, __LINE__, ch);

		
	if( (s_fmt_dbnc_buf0[ch] == s_fmt_dbnc_buf1[ch]) && (s_fmt_dbnc_buf1[ch] == s_fmt_dbnc_buf2[ch]) )
	{
		ret_vfmt = s_fmt_dbnc_buf2[ch];
		if(ret_vfmt == 0)
			{
			s_fmt_set_done = s_fmt_set_done & ~(0x1 << ch) ;	// if real novideo. s_fmt_set_done reset to zero.
			g_eq_set_done  = g_eq_set_done & ~(0x1 << ch) ;	// if real novideo. s_fmt_set_done reset to zero.
			printk(">>>>> DRV[%s:%d] CH:%d, Novideo...[%x], buf[%x,%x,%x], vfmt=[%x]\n", \
					__func__, __LINE__, ch, keep_fmt, s_fmt_dbnc_buf0[ch], s_fmt_dbnc_buf1[ch], s_fmt_dbnc_buf2[ch], ret_vfmt);
			}
		}
	else
	{
		ret_vfmt = nvp6134_vfmt_convert(keep_fmt, reg_vfmt_F2);		
	}

	printk(">>>>> DRV[%s:%d] CH:%d, result keep_fmt[regval:%x], buf[trans:%x,%x,%x], vfmt=[tran:%x]\n",\
			__func__, __LINE__, ch, keep_fmt, s_fmt_dbnc_buf0[ch], s_fmt_dbnc_buf1[ch], s_fmt_dbnc_buf2[ch], ret_vfmt);

	/* for verification's buffer */
	if(s_fmt_dbnc_cnt[ch] == 2)
		s_fmt_dbnc_cnt[ch] = 0;
	else
		s_fmt_dbnc_cnt[ch]++;


	return ret_vfmt;
}

unsigned char s_keep_fmt[16] = {[0 ... 15] = 0xFF};
unsigned int  s_keep_sync_width[16] = {[0 ... 15] = 0x00};
/*******************************************************************************
*	Description		: get videoloss information and get video format.
*	Argurments		: pvideofmt(video format buffer point)
*	Return value	: vloss(video loss information)
*	Modify			:
*	warning			:
*******************************************************************************/
unsigned int video_fmt_det(nvp6134_input_videofmt *pvideofmt)
{
	int i;
	unsigned int vloss = 0;
	unsigned char reg_vfmt_F0 = 0xFF;
	unsigned char reg_vfmt_F2=0;
	unsigned char reg_vfmt_F3=0;
	unsigned char reg_vfmt_F4=0;
	unsigned char reg_vfmt_F5=0;

	unsigned char pre_fmt = 0x0;
	unsigned char pre_adc_clk = 0x0;
	unsigned char pre_pre_clk = 0x0;
	unsigned int  acc_gain_sts;
	unsigned int  y_p_slp;
	unsigned int  y_m_slp;


	/* get video loss information */
	vloss = nvp6134_getvideoloss();
	msleep(200);

	printk(">>>>> DRV[%s:%d] vloss = [0x%08X], s_fmt_set_done[0x%08X], g_eq_set_done[0x%08X]\n", \
			__func__, __LINE__, vloss, s_fmt_set_done, g_eq_set_done);

	for(i=0; i<nvp6134_cnt*4; i++)
	{
		gpio_i2c_write(nvp6134_iic_addr[i/4], 0xFF, 0x05+(i%4));
		reg_vfmt_F0 = gpio_i2c_read(nvp6134_iic_addr[i/4], 0xF0);
		reg_vfmt_F2 = gpio_i2c_read(nvp6134_iic_addr[i/4], 0xF2);
		reg_vfmt_F3 = gpio_i2c_read(nvp6134_iic_addr[i/4], 0xF3);
		reg_vfmt_F4 = gpio_i2c_read(nvp6134_iic_addr[i/4], 0xF4);
		reg_vfmt_F5 = gpio_i2c_read(nvp6134_iic_addr[i/4], 0xF5);

		if( ((s_fmt_set_done>>i) & 0x1) && ((vloss >> i)&0x1) )	// format setting not yet.
		{
			s_fmt_set_done = s_fmt_set_done & ~(0x1 << i) ;	// if real novideo. s_fmt_set_done reset to zero.
			g_eq_set_done  = g_eq_set_done & ~(0x1 << i) ;	// if real novideo. s_fmt_set_done reset to zero.
			pvideofmt->getvideofmt[i] = nvp6134_vfmt_convert(0xFF, reg_vfmt_F2);		
			printk(">>>>> DRV[%s:%d] CH:%d, video_fmt_det, NO video set vloss[%x], fmt[%x], s_fmt_set_done[%x] \n", \
					__func__, __LINE__, i, vloss, reg_vfmt_F0, s_fmt_set_done);
		}
		else if( !((s_fmt_set_done >> i) & 0x1)  )	// format setting not yet.
		{
			if(  (( (reg_vfmt_F0) &  0x0F) != 0xF) &&  (( (reg_vfmt_F0) & 0xF0) != 0xF0)  )
			{
				/* distinguish video format about AHD and CHD.
				 * 2B:CHD@30p, 2C:CHD@25p, 35:CFHD@30p, 36:CFHD@25p, 20:AHD@30p, 21:AHD@25p, 30:AFHD@30p, 31:AFHD@25p */
				if( (reg_vfmt_F0 == 0x2B) || (reg_vfmt_F0 == 0x2C) ||(reg_vfmt_F0 == 0x35) ||
					(reg_vfmt_F0 == 0x36) || (reg_vfmt_F0 == 0x20) || (reg_vfmt_F0 == 0x21) ||
					(reg_vfmt_F0 == 0x30) || (reg_vfmt_F0 == 0x31) )	// AHD or CVI with No LE
				{
					switch(reg_vfmt_F0)
					{
						case 0x20 :	{ pre_fmt = 0x06; pre_adc_clk = 0x0; pre_pre_clk = 0x00; } break;
						case 0x21 :	{ pre_fmt = 0x07; pre_adc_clk = 0x0; pre_pre_clk = 0x00; } break;
						case 0x2B :	{ pre_fmt = 0x06; pre_adc_clk = 0x0; pre_pre_clk = 0x00; } break;
						case 0x2C :	{ pre_fmt = 0x07; pre_adc_clk = 0x0; pre_pre_clk = 0x00; } break;
						case 0x30 :	{ pre_fmt = 0x02; pre_adc_clk = 0x4; pre_pre_clk = 0x55; } break;
						case 0x31 :	{ pre_fmt = 0x03; pre_adc_clk = 0x4; pre_pre_clk = 0x55; } break;
						case 0x35 :	{ pre_fmt = 0x02; pre_adc_clk = 0x4; pre_pre_clk = 0x55; } break;
						case 0x36 :	{ pre_fmt = 0x03; pre_adc_clk = 0x4; pre_pre_clk = 0x55; } break;
						default   : pre_fmt = 0x03; break;
					}
				
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0xFF, 0x00);
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0x81+(i%4), pre_fmt);	// each reg_set
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0x85+(i%4), 0x02);	// special mode
				
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0xFF, 0x01);
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0x84+(i%4), pre_adc_clk);	// ADC clock delay
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0x8C+(i%4), pre_pre_clk);	// DEC Pre clock
						
					/* 20:AHD@30p, 21:AHD@25p, 2B:CHD@30p, 2C:CHD@25p --> HD */
					if( (reg_vfmt_F0 == 0x20) || (reg_vfmt_F0 == 0x21) ||
						(reg_vfmt_F0 == 0x2B) || (reg_vfmt_F0 == 0x2C) )
					{
			             		gpio_i2c_write(nvp6134_iic_addr[i/4], 0xFF, 0x09);
			            		gpio_i2c_write(nvp6134_iic_addr[i/4], 0x80+((i%4)*0x20), 0xa4);	//Digital EQ0
						gpio_i2c_write(nvp6134_iic_addr[i/4], 0x81+((i%4)*0x20), 0x00);	//Digital EQ1
						gpio_i2c_write(nvp6134_iic_addr[i/4], 0xFF, 0x05+(i%4));
			            		gpio_i2c_write(nvp6134_iic_addr[i/4], 0x27, 0x08);		// ACC reference
			            		gpio_i2c_write(nvp6134_iic_addr[i/4], 0x58, 0x03);  	// Analog EQ

						printk(">>>>> DRV[%s:%d] CH:%d, 720P Gain control for pre detection. Current fmt[%x]\n", \
								__func__, __LINE__, i, reg_vfmt_F0);
					}
					/* 30:AFHD@30p, 31:AFHD@25p, 35:CFHD@30p, 36:CFHD@25p --> FHD */
					else
					{
				        	gpio_i2c_write(nvp6134_iic_addr[i/4], 0xFF, 0x05+(i%4));                             
						gpio_i2c_write(nvp6134_iic_addr[i/4], 0x24, 0x18);	// Burst val sel
				        	gpio_i2c_write(nvp6134_iic_addr[i/4], 0x58, 0xD3);	// Analog EQ
				        	printk(">>>>> DRV[%s:%d] CH:%d, 1080P Gain control for pre detection ref.acc:[%x], Current fmt[%x]\n", \
				        		__func__, __LINE__, i, gpio_i2c_read(nvp6134_iic_addr[i/4], 0x27), reg_vfmt_F0 );
					}
					msleep(50);
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0xFF, 0x05+(i%4));
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0x90, 0x05);	//comb_mode
					msleep(100);
			
					/* 20:AHD@30p, 21:AHD@25p, 2B:CHD@30p, 2C:CHD@25p --> HD */
					if( (reg_vfmt_F0 == 0x20) || (reg_vfmt_F0 == 0x21) ||
						(reg_vfmt_F0 == 0x2B) || (reg_vfmt_F0 == 0x2C) )
					{
						/* want to be stable...*/
						GetDistinguishValue(i, &y_p_slp, &y_m_slp, &acc_gain_sts);
					}
					else
					{
						y_p_slp		 = GetYPlusSlope(i);
						y_m_slp		 = GetYMinusSlope(i);
						acc_gain_sts = distinguish_GetAccGain(i);
					}

					printk(">>>>> DRV[%s:%d] CH:%d, pre_distinguish_value. yp:%d, ym:%d, acc:%d\n", \
							__func__, __LINE__, i, y_p_slp, y_m_slp, acc_gain_sts );
					printk(">>>>> DRV[%s:%d] CH:%d, pre_distinguish_value. yp:%d, ym:%d, acc:%d\n", \
							__func__, __LINE__, i, y_p_slp, y_m_slp, acc_gain_sts );
					printk(">>>>> DRV[%s:%d] CH:%d, pre_distinguish_value. yp:%d, ym:%d, acc:%d\n", \
							__func__, __LINE__, i, y_p_slp, y_m_slp, acc_gain_sts );
					printk(">>>>> DRV[%s:%d] CH:%d, pre_distinguish_value. yp:%d, ym:%d, acc:%d\n", \
							__func__, __LINE__, i, y_p_slp, y_m_slp, acc_gain_sts );
					printk(">>>>> DRV[%s:%d] CH:%d, pre_distinguish_value. yp:%d, ym:%d, acc:%d\n", \
							__func__, __LINE__, i, y_p_slp, y_m_slp, acc_gain_sts );
				
					/* 20:AHD@30p, 21:AHD@25p */
					if( (reg_vfmt_F0 == 0x20) || (reg_vfmt_F0 == 0x21) )	
					{
						/* AHD@25p */
						if( acc_gain_sts > 2010 )
							reg_vfmt_F0 = reg_vfmt_F0;
						/* CHD@25p */
						else
							reg_vfmt_F0 = reg_vfmt_F0 + 0x0B;
					}	
					/* 2B:CHD@30p, 2C:CHD@25p */
					else if( (reg_vfmt_F0 == 0x2B) || (reg_vfmt_F0 == 0x2C) )	
					{
						/* AHD@30p */
						if( acc_gain_sts > 2010 )
							reg_vfmt_F0 = reg_vfmt_F0 - 0x0B;
						/* CHD@30p */
						else
							reg_vfmt_F0 = reg_vfmt_F0;
					}	
					/* 30:AFHD@30p, 31:AFHD@25p */
					else if( (reg_vfmt_F0 == 0x30) || (reg_vfmt_F0 == 0x31) )	
					{
						/* CFHD@2530p */
						if( (y_p_slp < 80) || (y_m_slp < 80) )
						{
							reg_vfmt_F0 = reg_vfmt_F0 + 0x05;
						}
						else
						{
							/* AFHD@2530p */
							if( acc_gain_sts > 2010 )
								reg_vfmt_F0 = reg_vfmt_F0;
							/* CFHD@2530p */
							else
								reg_vfmt_F0 = reg_vfmt_F0 + 0x05;
						}
					}
					/* 35:CFHD@30p, 36:CFHD@25p */
					else if( (reg_vfmt_F0 == 0x35) || (reg_vfmt_F0 == 0x36) )	
					{
						/* CFHD@2530p */
						if( (y_p_slp < 80) || (y_m_slp < 80) )
						{
							reg_vfmt_F0 = reg_vfmt_F0;
						}
						else
						{
							/* AFHD@2530p */
							if( acc_gain_sts > 2010 )
								reg_vfmt_F0 = reg_vfmt_F0 - 0x05;
							/* CFHD@2530p */
							else
								reg_vfmt_F0 = reg_vfmt_F0;
						}
					}
				
					/* set default value(off) to AEQ(B5x58), DEQ0(B9x80), DEQ1(B9x81) */
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0xFF, 0x09);
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0x80+(i%4)*0x20, 0x00);
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0x81+(i%4)*0x20, 0x00);
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0xFF, 0x05+(i%4));
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0x58, 0x03);
				}

				s_fmt_set_done = s_fmt_set_done | (0x1 << i) ;
				printk(">>>>> DRV[%s:%d] CH:%d, Format Detection.  fmt[%x] \n", __func__, __LINE__, i, reg_vfmt_F0);
				pvideofmt->getvideofmt[i] = nvp6134_vfmt_convert(reg_vfmt_F0, reg_vfmt_F2);		
				s_keep_fmt[i] = reg_vfmt_F0;
				s_fmt_dbnc_buf0[i] = pvideofmt->getvideofmt[i];
				s_fmt_dbnc_buf1[i] = pvideofmt->getvideofmt[i];
				s_fmt_dbnc_buf2[i] = pvideofmt->getvideofmt[i];
			}
			/* AHD 3[RT/NRT]/4[NRT]/5[RT/NRT], THD 3[NRT]/5[NRT] */
			else	
			{
				/* AHD 3M(RT/NRT) detection */
				if (reg_vfmt_F0 == 0x6F && ((reg_vfmt_F3 == 0x02) || (reg_vfmt_F3 == 0x03)) )
				{
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0xFF, 0x05+(i%4));
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0x05, 0x24);
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0x06, 0x40);
					msleep(100);

					reg_vfmt_F0 = reg_vfmt_F3;
					s_fmt_set_done = s_fmt_set_done | (0x1 << i) ;
					printk(">>>>> DRV[%s:%d] CH:%d, video_fmt_det, format detection AHD 3M NRT.  fmt[%x]\n", \
							__func__, __LINE__, i, reg_vfmt_F0);
					pvideofmt->getvideofmt[i] = nvp6134_vfmt_convert(reg_vfmt_F0, reg_vfmt_F2);		
					s_keep_fmt[i] = reg_vfmt_F0;
					s_fmt_dbnc_buf0[i] = pvideofmt->getvideofmt[i];
					s_fmt_dbnc_buf1[i] = pvideofmt->getvideofmt[i];
					s_fmt_dbnc_buf2[i] = pvideofmt->getvideofmt[i];
				}
				/* AHD 4M NRT detection */
				else if (reg_vfmt_F0 == 0x4F  && reg_vfmt_F3 == 0x04 )
				{
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0xFF, 0x05+(i%4));
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0x05, 0x24);
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0x06, 0x40);
					msleep(100);

					s_fmt_set_done = s_fmt_set_done | (0x1 << i) ;
					printk(">>>>> DRV[%s:%d] CH:%d, video_fmt_det, format detection AHD 4M NRT.  fmt[%x]\n", \
							__func__, __LINE__, i, reg_vfmt_F0);
					pvideofmt->getvideofmt[i] = nvp6134_vfmt_convert(reg_vfmt_F0, reg_vfmt_F2);
					s_keep_fmt[i] = reg_vfmt_F0;
					s_fmt_dbnc_buf0[i] = pvideofmt->getvideofmt[i];
					s_fmt_dbnc_buf1[i] = pvideofmt->getvideofmt[i];
					s_fmt_dbnc_buf2[i] = pvideofmt->getvideofmt[i];
				}
				/* AHD 5M detection */
				else if (reg_vfmt_F0 == 0x7F && ((reg_vfmt_F3 == 0x02) || (reg_vfmt_F3 == 0x04)) )
				{
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0xFF, 0x05+(i%4));
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0x05, 0x24);
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0x06, 0x40);
					msleep(100);

					/* 5M 12_5P */
					if (reg_vfmt_F3 == 0x04)
						reg_vfmt_F0 = 0xA0;
					/* 5M 20P */
					else if (reg_vfmt_F3 == 0x02)
						reg_vfmt_F0 = 0xA1;

					s_fmt_set_done = s_fmt_set_done | (0x1 << i) ;
					printk(">>>>> DRV[%s:%d] CH:%d, video_fmt_det, format detection AHD 5M FMT.  fmt[%x] \n", \
							__func__, __LINE__, i, reg_vfmt_F0);
					pvideofmt->getvideofmt[i] = nvp6134_vfmt_convert(reg_vfmt_F0, reg_vfmt_F2);		
					s_keep_fmt[i] = reg_vfmt_F0;
					s_fmt_dbnc_buf0[i] = pvideofmt->getvideofmt[i];
					s_fmt_dbnc_buf1[i] = pvideofmt->getvideofmt[i];
					s_fmt_dbnc_buf2[i] = pvideofmt->getvideofmt[i];
				}
				/* EXT 3M 18P */
				else if( (reg_vfmt_F5 == 0x06) && ((reg_vfmt_F4 == 0x30) || (reg_vfmt_F4 == 0x31)) )	
				{
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0xFF, 0x05+(i%4));
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0x05, 0x24);
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0x06, 0x40);
					msleep(100);

					reg_vfmt_F0 = 0x64;		// EXT 3M 18P		
					s_fmt_set_done = s_fmt_set_done | (0x1 << i) ;
					printk(">>>>> DRV[%s:%d] CH:%d, video_fmt_det, format detection TVI 3M NRT.  fmt[%x] \n", \
							__func__, __LINE__, i, reg_vfmt_F0);
					pvideofmt->getvideofmt[i] = nvp6134_vfmt_convert(reg_vfmt_F0, reg_vfmt_F2);
					s_keep_fmt[i] = reg_vfmt_F0;
					s_fmt_dbnc_buf0[i] = pvideofmt->getvideofmt[i];
					s_fmt_dbnc_buf1[i] = pvideofmt->getvideofmt[i];
					s_fmt_dbnc_buf2[i] = pvideofmt->getvideofmt[i];
				}
				/* EXT 5M vcnt 0xF0 == 0xFF */
				else if( (reg_vfmt_F5 == 0x07) && ((reg_vfmt_F4 == 0xD0) || (reg_vfmt_F4 == 0xD1)) )
				{
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0xFF, 0x05+(i%4));
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0x05, 0x24);
					gpio_i2c_write(nvp6134_iic_addr[i/4], 0x06, 0x40);
					msleep(100);

					reg_vfmt_F0 = 0xA2;		//EXT  5M 12.5P
					s_fmt_set_done = s_fmt_set_done | (0x1 << i) ;
					printk(">>>>> DRV[%s:%d] CH:%d, video_fmt_det, format detection TVI 5M NRT.  fmt[%x] \n", \
							__func__, __LINE__, i, reg_vfmt_F0);
					pvideofmt->getvideofmt[i] = nvp6134_vfmt_convert(reg_vfmt_F0, reg_vfmt_F2);		
					s_keep_fmt[i] = reg_vfmt_F0;
					s_fmt_dbnc_buf0[i] = pvideofmt->getvideofmt[i];
					s_fmt_dbnc_buf1[i] = pvideofmt->getvideofmt[i];
					s_fmt_dbnc_buf2[i] = pvideofmt->getvideofmt[i];
				}
				else
				{
					s_fmt_set_done = s_fmt_set_done & ~(0x1 << i) ;	// if real novideo. s_fmt_set_done reset to zero.
					g_eq_set_done  = g_eq_set_done & ~(0x1 << i) ;	// if real novideo. s_fmt_set_done reset to zero.
					pvideofmt->getvideofmt[i] = nvp6134_vfmt_convert(reg_vfmt_F0, reg_vfmt_F2);		
					s_keep_fmt[i] = reg_vfmt_F0;
					s_fmt_dbnc_buf0[i] = pvideofmt->getvideofmt[i];
					s_fmt_dbnc_buf1[i] = pvideofmt->getvideofmt[i];
					s_fmt_dbnc_buf2[i] = pvideofmt->getvideofmt[i];
				}
			}
		}
		/* already fmt set. */
		else
		{
			/* AHD 3M(RT/NRT) detection */
			if (reg_vfmt_F0 == 0x6F)
		{
				pvideofmt->getvideofmt[i] = video_fmt_debounce(i, s_keep_fmt[i], s_keep_sync_width[i]);
				printk(">>>>> DRV[%s:%d] CH:%d, already set. fmt[%x],  pvideofmt[%x]\n", \
						__func__, __LINE__, i,s_keep_fmt[i],pvideofmt->getvideofmt[i] );
			}				\
			/* AHD 4M NRT detection */
			else if (reg_vfmt_F0 == 0x4F)
			{
				pvideofmt->getvideofmt[i] = video_fmt_debounce(i, s_keep_fmt[i], s_keep_sync_width[i]);
				printk(">>>>> DRV[%s:%d] CH:%d, already set. fmt[%x],  pvideofmt[%x]\n", \
						__func__, __LINE__, i,s_keep_fmt[i],pvideofmt->getvideofmt[i] );
			}
			/* AHD 5M detection */
			else if (reg_vfmt_F0 == 0x7F)
			{
				pvideofmt->getvideofmt[i] = video_fmt_debounce(i, s_keep_fmt[i], s_keep_sync_width[i]);
				printk(">>>>> DRV[%s:%d] CH:%d, already set. fmt[%x],  pvideofmt[%x]\n", \
						__func__, __LINE__, i,s_keep_fmt[i],pvideofmt->getvideofmt[i] );
			}
			 /* EXT 3M 18P */
			else if( (reg_vfmt_F5 == 0x06) && ((reg_vfmt_F4 == 0x30) || (reg_vfmt_F4 == 0x31)) )	
			{
				pvideofmt->getvideofmt[i] = video_fmt_debounce(i, s_keep_fmt[i], s_keep_sync_width[i]);
				printk(">>>>> DRV[%s:%d] CH:%d, already set. fmt[%x],  pvideofmt[%x]\n", \
						__func__, __LINE__, i,s_keep_fmt[i],pvideofmt->getvideofmt[i] );
			}
			/* TVI  5M vcnt 0xF0 == 0xFF */
			else if( (reg_vfmt_F5 == 0x07) && ((reg_vfmt_F4 == 0xD0) || (reg_vfmt_F4 == 0xD1)) )
			{
				pvideofmt->getvideofmt[i] = video_fmt_debounce(i, s_keep_fmt[i], s_keep_sync_width[i]);
				printk(">>>>> DRV[%s:%d] CH:%d, already set. fmt[%x],  pvideofmt[%x]\n",\
						__func__, __LINE__, i,s_keep_fmt[i],pvideofmt->getvideofmt[i] );
			}
			/* other video format */
			else if(  (( (reg_vfmt_F0) &  0x0F) != 0xF) &&  (( (reg_vfmt_F0) & 0xF0) != 0xF0)  )
			{
				/* Run below routine in case of CVI */
				if( (s_keep_fmt[i] != reg_vfmt_F0) )
				{
					if( (g_eq_set_done >> i) & 0x1)
					{
						pvideofmt->getvideofmt[i] = video_fmt_debounce(i, s_keep_fmt[i], s_keep_sync_width[i]);
						if( pvideofmt->getvideofmt[i] == 0)
						{
							s_fmt_set_done = s_fmt_set_done & ~(0x1 << i) ;	// if real novideo. s_fmt_set_done reset to zero.
							g_eq_set_done  = g_eq_set_done & ~(0x1 << i) ;	// if real novideo. s_fmt_set_done reset to zero.
							printk(">>>>> DRV[%s:%d] CH:%d, NO video set vfc fail[%x], fmt[%x], s_fmt_set_done[%x] \n", \
									__func__, __LINE__, i,vloss, reg_vfmt_F0,s_fmt_set_done);
						}
					}
					else
					{
						printk(">>>>> DRV[%s:%d] CH:%d, g_eq_set_done NOT YET. can't debounce format. g_eq_set_done[%x]\n", \
								__func__, __LINE__, i,g_eq_set_done);
				}
				}
				printk(">>>>> DRV[%s:%d] CH:%d, already set no 0xFF . fmt[keep:%x,get:%x],  pvideofmt[trans:%x]\n", \
						__func__, __LINE__, i,s_keep_fmt[i],reg_vfmt_F0,pvideofmt->getvideofmt[i] );
			}
			else
			{
				printk(">>>>> DRV[%s:%d] CH:%d, 0xA8 is OK[%x], but VFC is vloss. fmt[%x], s_fmt_set_done[%x] \n", \
						__func__, __LINE__, i,vloss, reg_vfmt_F0,s_fmt_set_done);
			}
		}
	
		/* save video format for sharing Video Format to use other routine. */
		g_ch_video_fmt[i] = pvideofmt->getvideofmt[i];
		
		//printk(">>>>> DRV[%s:%d] CH:%d, final result, pvideofmt[0x%x]\n", __func__, __LINE__, i, pvideofmt->getvideofmt[i] );
	}
	
	return vloss;

}

unsigned char s_slice_cnt = 0;

unsigned int nvp6134_getvideoloss(void)
{
	unsigned int vloss=0, i, ch;
    unsigned char vlossperchip[4];
	
	for(i=0;i<nvp6134_cnt;i++)
	{
		gpio_i2c_write(nvp6134_iic_addr[i], 0xFF, 0x00);
		vlossperchip[i] = (gpio_i2c_read(nvp6134_iic_addr[i], 0xA8)&0x0F);
		vloss |= (vlossperchip[i]<<(4*i));
	}
    
	/* flag of changing slice level on NOVIDEO status  */
	if( s_slice_cnt > 100 )
		s_slice_cnt = 0;
	else
		s_slice_cnt++;

    for(ch=0; ch<(4*nvp6134_cnt); ch++)
    {
        if( (vlossperchip[ch/4] & ( 0x01 << (ch%4))) == (0x01 << (ch%4))  )
        {
            gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff, 0x05+ch%4);
			if( (s_fmt_set_done >> ch) & 0x1)
            {
				printk(">>>>> DRV[%s:%d] CH:%d, already fmt set done is ok\n", __func__, __LINE__, ch);
            }
            else
            {
				if(s_slice_cnt%3 == 0)
				{
					gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08, 0x50);
				}
				else if(s_slice_cnt%3 == 1)
				{
					gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08, 0x58);
				}		
				else
				{
					gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08, 0x60);
				}
			}

			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05, 0xA4);

			nvp6134_cvbs_slicelevel_con(ch, 0);
        }
		else
		{
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff, 0x05+ch%4);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05, 0x24);

			nvp6134_cvbs_slicelevel_con(ch, 1);
			if( (nvp6134_GetFSCLockStatus(ch) == 0) && 
				( 	(ch_mode_status[ch] == NVP6134_VI_EXT_1080P) ||
					(ch_mode_status[ch] == NVP6134_VI_EXT_3M_NRT)||
					(ch_mode_status[ch] == NVP6134_VI_EXT_720PA)||
					(ch_mode_status[ch] == NVP6134_VI_EXT_HDAEX)))
			{
				nvp6134_ResetFSCLock(ch);
			}
		}
    }
	
	return vloss;
}


/*
设置通道模式
变量
ch: 通道号，取值范围0~(nvp6134_cnt*4-1)
vfmt: 0:NTSC, 1:PAL
chnmode:通道模式，参考NVP6134_VI_MODE.
*/
int nvp6134_set_chnmode(const unsigned char ch, const unsigned char vfmt, const unsigned char chnmode)
{  
	//unsigned char tmp;
	
	if(ch >= (nvp6134_cnt*4))
	{
		printk("func[nvp6134_set_chnmode] Channel %d is out of range!!!\n", ch);
		return -1;
	}
	if(vfmt > PAL)
	{
		printk("func[nvp6134_set_chnmode] vfmt %d is out of range!!!\n", vfmt);
		return -1;
	}

	/* set video format each format */
	if(chnmode < NVP6134_VI_BUTT) 
	{
		/*  (+) - set these value */
		nvp6134_set_common_value( ch, chnmode );
		
		switch(chnmode)
		{
			case NVP6134_VI_720H:		nvp6134_setchn_720h(ch, vfmt);			break;
			case NVP6134_VI_960H:		nvp6134_setchn_960h(ch, vfmt);			break;
			case NVP6134_VI_1280H:		nvp6134_setchn_1280h(ch, vfmt);			break;
			case NVP6134_VI_1440H:		nvp6134_setchn_1440h(ch, vfmt);			break;
			case NVP6134_VI_1920H:		nvp6134_setchn_1920h(ch, vfmt);			break;	
			case NVP6134_VI_960H2EX:	nvp6134_setchn_3840h(ch, vfmt);			break;
			case NVP6134_VI_720P_2530:	nvp6134_setchn_ahd_720p(ch, vfmt);		break;
			case NVP6134_VI_HDEX:		nvp6134_setchn_ahd_720pex(ch, vfmt);	break;
			case NVP6134_VI_EXC_720P:	nvp6134_setchn_exc_720p(ch, vfmt);		break;
			case NVP6134_VI_EXC_HDEX:	nvp6134_setchn_exc_720pex(ch, vfmt);	break;
			case NVP6134_VI_EXT_720PA:	nvp6134_setchn_exta_720p(ch, vfmt);		break;
			case NVP6134_VI_EXT_HDAEX:	nvp6134_setchn_exta_720pex(ch, vfmt);	break;
			case NVP6134_VI_EXT_720PB:	nvp6134_setchn_extb_720p(ch, vfmt);		break;
			case NVP6134_VI_EXT_HDBEX:	nvp6134_setchn_extb_720pex(ch, vfmt);	break;
			case NVP6134_VI_720P_5060:	nvp6134_setchn_ahd_720p5060(ch, vfmt);	break;	
			case NVP6134_VI_EXC_720PRT:	nvp6134_setchn_exc_720p5060(ch, vfmt);	break;
			case NVP6134_VI_EXT_720PRT:	nvp6134_setchn_ext_720p5060(ch, vfmt);	break;	
			case NVP6134_VI_1080P_NRT:	nvp6134_setchn_ahd_1080p_NRT(ch, vfmt);	break;
			case NVP6134_VI_1080P_2530:	nvp6134_setchn_ahd_1080p2530(ch, vfmt);	break;
			case NVP6134_VI_EXC_1080P:	nvp6134_setchn_exc_1080p2530(ch, vfmt);	break;	
			case NVP6134_VI_EXT_1080P:	nvp6134_setchn_ext_1080p2530(ch, vfmt);	break;
			case NVP6134_VI_3M_NRT:		nvp6134_setchn_ahd_3MNRT(ch, vfmt);		break;
			case NVP6134_VI_EXT_3M_NRT: nvp6134_setchn_ext_3MNRT(ch, vfmt);		break;
			case NVP6134_VI_3M:			nvp6134_setchn_ahd_3M(ch, vfmt);		break;
			case NVP6134_VI_4M_NRT:		nvp6134_setchn_ahd_QHD_NRT(ch, vfmt);	break;
			case NVP6134_VI_4M:			nvp6134_setchn_ahd_QHD(ch, vfmt);		break;
			case NVP6134_VI_5M_NRT: 	nvp6134_setchn_ahd_5MNRT(ch, vfmt);		break;
			case NVP6134_VI_5M_20P: 	nvp6134_setchn_ahd_5M_20p(ch, vfmt);	break;
			case NVP6134_VI_EXT_5M_NRT: nvp6134_setchn_ext_5MNRT(ch, vfmt);		break;
			case NVP6134_VI_1080P_NOVIDEO:
			default:	
				nvp6134_setchn_ahd_1080p_novideo(ch, vfmt);
				printk("Default Set to 1080P novideo mode[ch%d]\n", ch);			
			break;
		}

		/* save Video mode and video format(NTSC/PAL) */
		ch_mode_status[ch] = chnmode;
		ch_vfmt_status[ch] = vfmt;

		if(NVP6134_VI_1080P_NOVIDEO != ch_mode_status[ch])
		{
			/* Initalize ACP protocol each mode */
			acp_each_setting(ch);

			/*  (+) - set EQ configuration */
			eq_init_each_format( ch, chnmode, vfmt );
		}
		
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,0x61); 	
        msleep(35);
		if(NVP6134_VI_720P_2530 > ch_mode_status[ch])
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,0x60); //for comet setting
		else
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,0x00); 
	}

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+ch%4,0x00); 

	return 0;
}


/*
nvp6134和nvp6134b共用同一函数
portsel上有差异，nvp6134b只能使用1和2，nvp6134有4个port，可以使用0~3。
chip:chip select[0,1,2,3];
portsel: port select->6134b[1,2],6134[0,1,2,3];
portmode: port mode select[1mux,2mux,4mux]
chid:  channel id, 1mux[0,1,2,3], 2mux[0,1], 4mux[0]
*/
/*******************************************************************************
*	Description		: select port
*	Argurments		: chip(chip select[0,1,2,3]),
*					  portsel(port select->6134b[1,2],6134[0,1,2,3];)
*					  portmode(port mode select[1mux,2mux,4mux]),
*					  chid(channel id, 1mux[0,1,2,3], 2mux[0,1], 4mux[0])
*	Return value	: 0
*	Modify			:
*	warning			:
*******************************************************************************/
int nvp6134_set_portmode(const unsigned char chip, const unsigned char portsel, const unsigned char portmode, const unsigned char chid)
{
	unsigned char chipaddr = nvp6134_iic_addr[chip];
	unsigned char tmp=0, tmp1=0, reg1=0, reg2=0;
	
	if((portsel!=1) && (portsel!=2) && (chip_id[chip]==NVP6134B_R0_ID))
	{
		printk("nvp6134b_set_portmode portsel[%d] error!!!\n", portsel);
		//return -1;
	}
	
	switch(portmode)
	{
		case NVP6134_OUTMODE_1MUX_SD:
			/*输出720H或者960H单通道,数据37.125MHz,时钟37.125MHz,单沿采样.*/
			gpio_i2c_write(chipaddr, 0xFF, 0x00);
			gpio_i2c_write(chipaddr, 0x56, 0x10);
			gpio_i2c_write(chipaddr, 0xFF, 0x01);
			gpio_i2c_write(chipaddr, 0xC0+portsel*2, (chid<<4)|chid);
			gpio_i2c_write(chipaddr, 0xC1+portsel*2, (chid<<4)|chid);
			tmp = gpio_i2c_read(chipaddr, 0xC8+(portsel/2)) & (portsel%2?0x0F:0xF0);
			gpio_i2c_write(chipaddr, 0xC8+(portsel/2), tmp);
			gpio_i2c_write(chipaddr, 0xCC+portsel, 0x86);			
		break;
		case NVP6134_OUTMODE_1MUX_HD:
			/*输出720P或者1280H或者1440H单通道,数据74.25MHz,时钟74.25MHz,单沿采样.*/
			gpio_i2c_write(chipaddr, 0xFF, 0x00);
			gpio_i2c_write(chipaddr, 0x56, 0x10);
			gpio_i2c_write(chipaddr, 0xFF, 0x01);
			gpio_i2c_write(chipaddr, 0xC0+portsel*2, (chid<<4)|chid); 
			gpio_i2c_write(chipaddr, 0xC1+portsel*2, (chid<<4)|chid);
			tmp = gpio_i2c_read(chipaddr, 0xC8+(portsel/2)) & (portsel%2?0x0F:0xF0);
			gpio_i2c_write(chipaddr, 0xC8+(portsel/2), tmp);
			gpio_i2c_write(chipaddr, 0xCC+portsel, 0x16);   
		break;
		case NVP6134_OUTMODE_1MUX_HD5060:
		case NVP6134_OUTMODE_1MUX_FHD:
		case NVP6134_OUTMODE_1MUX_4M_NRT:	
			/*输出720P@5060或者1080P单通道,数据148.5MHz,时钟148.5MHz,单沿采样.*/
			gpio_i2c_write(chipaddr, 0xFF, 0x00);
			gpio_i2c_write(chipaddr, 0x56, 0x10);
			gpio_i2c_write(chipaddr, 0xFF, 0x01);
			gpio_i2c_write(chipaddr, 0xC0+portsel*2, (chid<<4)|chid);  
			gpio_i2c_write(chipaddr, 0xC1+portsel*2, (chid<<4)|chid);
			tmp = gpio_i2c_read(chipaddr, 0xC8+(portsel/2)) & (portsel%2?0x0F:0xF0);
			gpio_i2c_write(chipaddr, 0xC8+(portsel/2), tmp);
			gpio_i2c_write(chipaddr, 0xCC+portsel, 0x46);
			break;
		case NVP6134_OUTMODE_2MUX_SD:
			/*输出720H或者960H 2通道,数据74.25MHz,时钟74.25MHz,单沿采样.*/
			gpio_i2c_write(chipaddr, 0xFF, 0x00);
			gpio_i2c_write(chipaddr, 0x56, 0x10);
			gpio_i2c_write(chipaddr, 0xFF, 0x01);
			gpio_i2c_write(chipaddr, 0xC0+portsel*2, chid==0?0x10:0x32);
			gpio_i2c_write(chipaddr, 0xC1+portsel*2, chid==0?0x10:0x32);
			tmp = gpio_i2c_read(chipaddr, 0xC8+(portsel/2)) & (portsel%2?0x0F:0xF0);
			tmp |= (portsel%2?0x20:0x02);
			gpio_i2c_write(chipaddr, 0xC8+(portsel/2), tmp);
			gpio_i2c_write(chipaddr, 0xCC+portsel, 0x16);
			break;
		case NVP6134_OUTMODE_2MUX_HD_X:
			/*输出HD-X 2通道,数据74.25MHz,时钟74.25MHz,单沿采样.*/
			gpio_i2c_write(chipaddr, 0xFF, 0x00);
			gpio_i2c_write(chipaddr, 0x56, 0x10);
			gpio_i2c_write(chipaddr, 0xFF, 0x01);
			gpio_i2c_write(chipaddr, 0xC0+portsel*2, chid==0?0x98:0xBA);
			gpio_i2c_write(chipaddr, 0xC1+portsel*2, chid==0?0x98:0xBA);
			tmp = gpio_i2c_read(chipaddr, 0xC8+(portsel/2)) & (portsel%2?0x0F:0xF0);
			tmp |= (portsel%2?0x20:0x02);
			gpio_i2c_write(chipaddr, 0xC8+(portsel/2), tmp);
			gpio_i2c_write(chipaddr, 0xCC+portsel, 0x16);
			break;			
		case NVP6134_OUTMODE_2MUX_HD:
			/*输出HD 2通道,数据148.5MHz,时钟148.5MHz,单沿采样.*/
			gpio_i2c_write(chipaddr, 0xFF, 0x00);
			gpio_i2c_write(chipaddr, 0x56, 0x10);
			gpio_i2c_write(chipaddr, 0xFF, 0x01);
			gpio_i2c_write(chipaddr, 0xC0+portsel*2, chid==0?0x10:0x32);
			gpio_i2c_write(chipaddr, 0xC1+portsel*2, chid==0?0x10:0x32);
			tmp = gpio_i2c_read(chipaddr, 0xC8+(portsel/2)) & (portsel%2?0x0F:0xF0);
			tmp |= (portsel%2?0x20:0x02);
			gpio_i2c_write(chipaddr, 0xC8+(portsel/2), tmp);
			gpio_i2c_write(chipaddr, 0xCC+portsel, 0x46);			
			break;
		case NVP6134_OUTMODE_4MUX_SD:
			/*输出720H或者960H 4通道,数据148.5MHz,时钟148.5MHz,单沿采样.*/
			gpio_i2c_write(chipaddr, 0xFF, 0x00);
			gpio_i2c_write(chipaddr, 0x56, 0x32);
			gpio_i2c_write(chipaddr, 0xFF, 0x01);
			gpio_i2c_write(chipaddr, 0xC0+portsel*2, 0x10);    
			gpio_i2c_write(chipaddr, 0xC1+portsel*2, 0x32);
			tmp = gpio_i2c_read(chipaddr, 0xC8+(portsel/2)) & (portsel%2?0x0F:0xF0);
			tmp |= (portsel%2?0x80:0x08);
			gpio_i2c_write(chipaddr, 0xC8+(portsel/2), tmp);
			gpio_i2c_write(chipaddr, 0xCC+portsel, 0x46);  
			break;
		case NVP6134_OUTMODE_4MUX_HD:	
			/*输出720P 4通道,数据297MHz,时钟297MHz,单沿采样.*/
			gpio_i2c_write(chipaddr, 0xFF, 0x00);
			gpio_i2c_write(chipaddr, 0x56, 0x32);
			gpio_i2c_write(chipaddr, 0xFF, 0x01);
			gpio_i2c_write(chipaddr, 0xC0+portsel*2, 0x10);    
			gpio_i2c_write(chipaddr, 0xC1+portsel*2, 0x32);
			tmp = gpio_i2c_read(chipaddr, 0xC8+(portsel/2)) & (portsel%2?0x0F:0xF0);
			tmp |= (portsel%2?0x80:0x08);
			gpio_i2c_write(chipaddr, 0xC8+(portsel/2), tmp);
			gpio_i2c_write(chipaddr, 0xCC+portsel, 0x46);
			//gpio_i2c_write(chipaddr, 0xCC+portsel, 0x66);  //single up
			break;
		case NVP6134_OUTMODE_4MUX_HD_X:
			/*输出HD-X 4通道,数据148.5MHz,时钟148.5MHz,单沿采样.*/
			gpio_i2c_write(chipaddr, 0xFF, 0x00);
			gpio_i2c_write(chipaddr, 0x56, 0x32);
			gpio_i2c_write(chipaddr, 0xFF, 0x01);
			gpio_i2c_write(chipaddr, 0xC0+portsel*2, 0x98);    
			gpio_i2c_write(chipaddr, 0xC1+portsel*2, 0xBA);
			tmp = gpio_i2c_read(chipaddr, 0xC8+(portsel/2)) & (portsel%2?0x0F:0xF0);
			tmp |= (portsel%2?0x80:0x08);
			gpio_i2c_write(chipaddr, 0xC8+(portsel/2), tmp);
			gpio_i2c_write(chipaddr, 0xCC+portsel, 0x46);
			break;
		case NVP6134_OUTMODE_2MUX_FHD:	
			/*5M_20P,5M_12P,4M_RT,4M_15P,3M_RT/NRT,FHD,3840H,HDEX 2mux任意混合,数据297MHz,时钟148.5MHz,双沿采样.
			SOC VI端通过丢点，实现3840H->960H, HDEX->720P  */
			gpio_i2c_write(chipaddr, 0xFF, 0x00);
			gpio_i2c_write(chipaddr, 0x56, 0x10);
			#if 1
			//CHANNEL 1 JUDGE
			tmp  = gpio_i2c_read(chipaddr, 0x81)&0x0F;
			tmp1 = gpio_i2c_read(chipaddr, 0x85)&0x0F;
			if(((tmp == 0x02) || (tmp == 0x03)) && (tmp1 == 0x04))
				reg1 |= 0x08;							//3M_RT, THEN OUTPUT 3M_CIF DATA
			else if(((tmp == 0x0E) || (tmp == 0x0F)) && (tmp1 == 0x00))
				reg1 |= 0x08;							//4M, THEN OUTPUT 4M_CIF DATA
			else if(ch_mode_status[(chip*4)] == NVP6134_VI_5M_20P) //5M_20P, THEN OUTPUT 5M_CIF DATA
				reg1 |= 0x08;
			else				
				reg1 &= 0xF0;
			//CHANNEL 2 JUDGE
			tmp  = gpio_i2c_read(chipaddr, 0x82)&0x0F;
			tmp1 = gpio_i2c_read(chipaddr, 0x86)&0x0F;
			if(((tmp == 0x02) || (tmp == 0x03)) && (tmp1 == 0x04))
				reg1 |= 0x80;
			else if(((tmp == 0x0E) || (tmp == 0x0F)) && (tmp1 == 0x00))
				reg1 |= 0x80;
			else if(ch_mode_status[(chip*4+1)] == NVP6134_VI_5M_20P) //5M_20P, THEN OUTPUT 5M_CIF DATA
				reg1 |= 0x80;
			else
				reg1 &= 0x0F;
			//CHANNEL 3 JUDGE
			tmp  = gpio_i2c_read(chipaddr, 0x83)&0x0F;
			tmp1 = gpio_i2c_read(chipaddr, 0x87)&0x0F;
			if(((tmp == 0x02) || (tmp == 0x03)) && (tmp1 == 0x04))
				reg2 |= 0x08;
			else if(((tmp == 0x0E) || (tmp == 0x0F)) && (tmp1 == 0x00))
				reg2 |= 0x08;
			else if(ch_mode_status[(chip*4+2)] == NVP6134_VI_5M_20P) //5M_20P, THEN OUTPUT 5M_CIF DATA
				reg2 |= 0x08;
			else
				reg2 &= 0xF0;
			//CHANNEL 4 JUDGE
			tmp  = gpio_i2c_read(chipaddr, 0x84)&0x0F;
			tmp1 = gpio_i2c_read(chipaddr, 0x88)&0x0F;
			if(((tmp == 0x02) || (tmp == 0x03)) && (tmp1 == 0x04))
				reg2 |= 0x80;
			else if(((tmp == 0x0E) || (tmp == 0x0F)) && (tmp1 == 0x00))
				reg2 |= 0x80;
			else if(ch_mode_status[(chip*4+3)] == NVP6134_VI_5M_20P) //5M_20P, THEN OUTPUT 5M_CIF DATA
				reg2 |= 0x80;
			else
				reg2 &= 0x0F;
			gpio_i2c_write(chipaddr, 0xFF, 0x01);
			gpio_i2c_write(chipaddr, 0xC0+portsel*2, chid==0?(0x10|reg1):(0x32|reg2));
			gpio_i2c_write(chipaddr, 0xC1+portsel*2, chid==0?(0x10|reg1):(0x32|reg2));
			#else
			gpio_i2c_write(chipaddr, 0xFF, 0x01);
			gpio_i2c_write(chipaddr, 0xC0+portsel*2, chid==0?0x10:0x32);
			gpio_i2c_write(chipaddr, 0xC1+portsel*2, chid==0?0x10:0x32);
			#endif
			tmp = gpio_i2c_read(chipaddr, 0xC8+(portsel/2)) & (portsel%2?0x0F:0xF0);
			tmp |= (portsel%2?0x20:0x02);
			gpio_i2c_write(chipaddr, 0xC8+(portsel/2), tmp);
			gpio_i2c_write(chipaddr, 0xCC+portsel, 0x56);
			//gpio_i2c_write(chipaddr, 0xCC+portsel, 0x66);  //single up
			break;	
		case NVP6134_OUTMODE_4MUX_FHD_X:
			/*输出FHD-X 4通道,数据297MHz,时钟148.5MHz,双沿采样.*/
			gpio_i2c_write(chipaddr, 0xFF, 0x00);
			gpio_i2c_write(chipaddr, 0x56, 0x32);
			gpio_i2c_write(chipaddr, 0xFF, 0x01);
			gpio_i2c_write(chipaddr, 0xC0+portsel*2, 0x98);    
			gpio_i2c_write(chipaddr, 0xC1+portsel*2, 0xBA);
			tmp = gpio_i2c_read(chipaddr, 0xC8+(portsel/2)) & (portsel%2?0x0F:0xF0);
			tmp |= (portsel%2?0x80:0x08);
			gpio_i2c_write(chipaddr, 0xC8+(portsel/2), tmp);
			gpio_i2c_write(chipaddr, 0xCC+portsel, 0x46);  
			//gpio_i2c_write(chipaddr, 0xCC+portsel, 0x66);  //single up
			break;
		case NVP6134_OUTMODE_4MUX_MIX: 
			/*HD,1920H,FHD-X 4mux任意混合,数据297MHz,时钟148.5MHz,双沿采样
			SOC VI端通过丢点，实现1920H->960H  */
			gpio_i2c_write(chipaddr, 0xFF, 0x00);
			gpio_i2c_write(chipaddr, 0x56, 0x32);
			tmp = gpio_i2c_read(chipaddr, 0x81)&0x0F;
			if(((tmp) == 0x02) || ((tmp) == 0x03))
				reg1 |= 0x08;
			else
				reg1 &= 0xF0;
			tmp = gpio_i2c_read(chipaddr, 0x82)&0x0F;
			if((tmp == 0x02) || (tmp == 0x03))
				reg1 |= 0x80;
			else
				reg1 &= 0x0F;
			tmp = gpio_i2c_read(chipaddr, 0x83)&0x0F;
			if(((tmp) == 0x02) || ((tmp) == 0x03))
				reg2 |= 0x08;
			else
				reg2 &= 0xF0;
			tmp = gpio_i2c_read(chipaddr, 0x84)&0x0F;
			if((tmp == 0x02) || (tmp == 0x03))
				reg2 |= 0x80;
			else
				reg2 &= 0x0F;
			gpio_i2c_write(chipaddr, 0xFF, 0x01);
			gpio_i2c_write(chipaddr, 0xC0+portsel*2, 0x10|reg1);    
			gpio_i2c_write(chipaddr, 0xC1+portsel*2, 0x32|reg2);
			tmp = gpio_i2c_read(chipaddr, 0xC8+(portsel/2)) & (portsel%2?0x0F:0xF0);
			tmp |= (portsel%2?0x80:0x08);
			gpio_i2c_write(chipaddr, 0xC8+(portsel/2), tmp);
			gpio_i2c_write(chipaddr, 0xCC+portsel, 0x46);  
			//gpio_i2c_write(chipaddr, 0xCC+portsel, 0x66);  //single up
			break;	
		case NVP6134_OUTMODE_2MUX_MIX: 
		case NVP6134_OUTMODE_2MUX_FHD_X:	
			/*HD,1920H,FHD-X 2MUX任意混合,数据148.5MHz,时钟148.5MHz,单沿采样	
			SOC VI端通过丢点，实现1920H->960H  */
			gpio_i2c_write(chipaddr, 0xFF, 0x00);
			gpio_i2c_write(chipaddr, 0x56, 0x10);
			tmp = gpio_i2c_read(chipaddr, 0x81)&0x0F;
			if(((tmp) == 0x02) || ((tmp) == 0x03))
				reg1 |= 0x08;
			else
				reg1 &= 0xF0;
			tmp = gpio_i2c_read(chipaddr, 0x82)&0x0F;
			if((tmp == 0x02) || (tmp == 0x03))
				reg1 |= 0x80;
			else
				reg1 &= 0x0F;
			tmp = gpio_i2c_read(chipaddr, 0x83)&0x0F;
			if(((tmp) == 0x02) || ((tmp) == 0x03))
				reg2 |= 0x08;
			else
				reg2 &= 0xF0;
			tmp = gpio_i2c_read(chipaddr, 0x84)&0x0F;
			if((tmp == 0x02) || (tmp == 0x03))
				reg2 |= 0x80;
			else
				reg2 &= 0x0F;
			gpio_i2c_write(chipaddr, 0xFF, 0x01);
			gpio_i2c_write(chipaddr, 0xC0+portsel*2, chid==0?(0x10|reg1):(0x32|reg2));
			gpio_i2c_write(chipaddr, 0xC1+portsel*2, chid==0?(0x10|reg1):(0x32|reg2));
			tmp = gpio_i2c_read(chipaddr, 0xC8+(portsel/2)) & (portsel%2?0x0F:0xF0);
			tmp |= (portsel%2?0x20:0x02);
			gpio_i2c_write(chipaddr, 0xC8+(portsel/2), tmp);
			gpio_i2c_write(chipaddr, 0xCC+portsel, 0x46); 				
			break;	
		case NVP6134_OUTMODE_1MUX_BT1120S:	
			gpio_i2c_write(chipaddr, 0xFF, 0x00);
			gpio_i2c_write(chipaddr, 0x56, 0x10);
			gpio_i2c_write(chipaddr, 0xFF, 0x01);
			if(chip_id[chip] == NVP6134B_R0_ID)
			{
				//6134C makes 2 bt656 ports to 1 bt1120 port.  portsel=[1,2] to choose clock.
				gpio_i2c_write(chipaddr, 0xC2, ((chid%4)+0x0C));
				gpio_i2c_write(chipaddr, 0xC3, ((chid%4)+0x0C));
				gpio_i2c_write(chipaddr, 0xC4, ((chid%4)+0x04));   
				gpio_i2c_write(chipaddr, 0xC5, ((chid%4)+0x04));
				gpio_i2c_write(chipaddr, 0xC8, 0x00);
				gpio_i2c_write(chipaddr, 0xC9, 0x00);
				gpio_i2c_write(chipaddr, 0xCC+portsel, 0x06);		//74.25MHz clock
			}
			else
			{
				//6134 makes 4 bt656 ports to 2 bt1120 port.   portsel=[0,1] to choose clock.
				gpio_i2c_write(chipaddr, 0xC0+portsel*4, ((chid%4)+0x0C));   
				gpio_i2c_write(chipaddr, 0xC1+portsel*4, ((chid%4)+0x0C));
				gpio_i2c_write(chipaddr, 0xC2+portsel*4, ((chid%4)+0x04));   
				gpio_i2c_write(chipaddr, 0xC3+portsel*4, ((chid%4)+0x04));
				gpio_i2c_write(chipaddr, 0xC8+(portsel), 0x00);
				gpio_i2c_write(chipaddr, 0xCC+portsel*2, 0x06);		//74.25MHz clock
			}
			break;
		case NVP6134_OUTMODE_1MUX_3M_RT:
			/*1MUX数据输出，时钟是297MHZ输出*/
			gpio_i2c_write(chipaddr, 0xFF, 0x00);
			gpio_i2c_write(chipaddr, 0x56, 0x10);
			gpio_i2c_write(chipaddr, 0xFF, 0x01);
			gpio_i2c_write(chipaddr, 0xC0+portsel*2, (chid<<4)|chid);   /* Port selection */
			gpio_i2c_write(chipaddr, 0xC1+portsel*2, (chid<<4)|chid);   /* Port selection */
			tmp = gpio_i2c_read(chipaddr, 0xC8+(portsel/2)) & (portsel%2?0x0F:0xF0);
			gpio_i2c_write(chipaddr, 0xC8+(portsel/2), tmp);
			//gpio_i2c_write(chipaddr, 0xCC+portsel, 0x46);
			gpio_i2c_write(chipaddr, 0xCC+portsel, 0x66);
			break;
		default:
			printk("portmode %d not supported yet\n", portmode);
			break;		
  	}
	
	if(	portmode==NVP6134_OUTMODE_2MUX_MIX  ||
		portmode==NVP6134_OUTMODE_2MUX_SD	||
		portmode==NVP6134_OUTMODE_2MUX_HD_X ||
		portmode==NVP6134_OUTMODE_2MUX_FHD_X)
	{
		gpio_i2c_write(chipaddr, 0xFF, 0x01);
		gpio_i2c_write(chipaddr, 0xE4, 0x10);  //enable 2mix cif mode
		gpio_i2c_write(chipaddr, 0xE5, 0x10);
	}
	else 
	{
		gpio_i2c_write(chipaddr, 0xFF, 0x01);
		gpio_i2c_write(chipaddr, 0xE4, 0x00);  //disable 2mix cif mode
		gpio_i2c_write(chipaddr, 0xE5, 0x00);
	}

	if(	portmode==NVP6134_OUTMODE_2MUX_SD || 
		portmode==NVP6134_OUTMODE_4MUX_SD || 
		portmode==NVP6134_OUTMODE_4MUX_HD_X)
	{
		gpio_i2c_write(chipaddr, 0xFF, 0x01);
		gpio_i2c_write(chipaddr, 0xA0+portsel, 0x20);  //TM clock mode sel manual
	}
	else 
	{
		gpio_i2c_write(chipaddr, 0xFF, 0x01);
		gpio_i2c_write(chipaddr, 0xA0+portsel, 0x00);  //TM clock mode sel auto
	}
	
	printk("nvp6134(b)_set_portmode portsel %d portmode %d setting\n", portsel, portmode);
	return 0;
}


/*
chip:0~3
portsel: 6134b/c->1/2, 6134->0~3
enclk: enable clock pin,  1:enable,0:disable;
endata: enable data port, 1:enable,0:disable;
*/
void nvp6134_set_portcontrol(const unsigned char chip, const unsigned char portsel, const unsigned char enclk, const unsigned char endata)
{
	unsigned char reg_portctl;
	gpio_i2c_write(nvp6134_iic_addr[chip], 0xFF, 0x01);
	reg_portctl = gpio_i2c_read(nvp6134_iic_addr[chip], 0xCA);
	if(chip_id[chip] == NVP6134B_R0_ID)
	{
		if(enclk == 1)
			SET_BIT(reg_portctl, (portsel+5));
		else
			CLE_BIT(reg_portctl, (portsel+5));

		if(endata == 1)
			SET_BIT(reg_portctl, portsel);
		else
			CLE_BIT(reg_portctl, portsel);
	}
	else if(chip_id[chip] == NVP6134_R0_ID)
	{
		if(enclk == 1)
			SET_BIT(reg_portctl, (portsel+4));
		else
			CLE_BIT(reg_portctl, (portsel+4));

		if(endata == 1)
			SET_BIT(reg_portctl, portsel);
		else
			CLE_BIT(reg_portctl, portsel);
	}
}

void nvp6134_VD_chnRst(unsigned char ch)
{
	unsigned char reg_1x97;
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	reg_1x97 = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x97);
	CLE_BIT(reg_1x97,(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97, reg_1x97);
	msleep(10);
	SET_BIT(reg_1x97,(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97, reg_1x97);
}

/*0:agc unlocked; 1:agc locked*/
int nvp6134_GetAgcLockStatus(unsigned char ch)
{
	int agc_lock, ret;
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	agc_lock = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xE0);
	ret = ((agc_lock>>(ch%4))&0x01);
	
	return ret;
}

/*0:fsc unlocked; 1:fsc locked*/
int nvp6134_GetFSCLockStatus(unsigned char ch)
{
	int fsc_lock, ret;
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	fsc_lock = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xE8+(ch%4));
	ret = ((fsc_lock>>1)&0x01);
	
	return ret;
}

void nvp6134_ResetFSCLock(unsigned char ch)
{
	unsigned char acc_ref=0;
	unsigned char check_cnt = 4;
	do{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
		acc_ref = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x27);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23, 0x80);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x27, 0x10);
		msleep(35);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23, 0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x27, acc_ref);
		msleep(300);
	}
	while((nvp6134_GetFSCLockStatus(ch)==0) && ((check_cnt--)>0));
	
	printk("%s, %d\n", __FUNCTION__, __LINE__);
}

void nvp6134_chn_killcolor(unsigned char ch, unsigned char onoff)
{
	unsigned char colorkill;
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	colorkill = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x22+(ch%4)*4);
	if(onoff==1)
		SET_BIT(colorkill, 4);
	else
		CLE_BIT(colorkill, 4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x22+(ch%4)*4, colorkill);
	printk("%s, %d %x %x\n", __FUNCTION__, __LINE__, onoff, colorkill);
}

void nvp6134_cvbs_slicelevel_con(unsigned char ch, int onoff)
{
	if(ch_mode_status[ch]<NVP6134_VI_720P_2530)
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
		if(onoff == 1)
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08, 0x70);
		else
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08, 0x50);
		printk("%s, %d\n", __FUNCTION__, __LINE__);
	}
}

void nvp6134_hide_ch(unsigned char ch)
{
	unsigned char reg_0x7a;
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	reg_0x7a = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x7A+((ch%4)/2));
	reg_0x7a &= (ch%2==0?0xF0:0x0F);
	reg_0x7a |= (ch%2==0?0x0F:0xF0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7A+((ch%4)/2),reg_0x7a);
	//printk("%s, %d\n", __FUNCTION__, __LINE__);
}

void nvp6134_show_ch(unsigned char ch)
{
	unsigned char reg_0x7a;
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	reg_0x7a = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x7A+((ch%4)/2));
	reg_0x7a &= (ch%2==0?0xF0:0x0F);
	reg_0x7a |= (ch%2==0?0x01:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x7A+((ch%4)/2),reg_0x7a);
	//printk("%s, %d\n", __FUNCTION__, __LINE__);
}

/*
support AHD 3M/4M/5M real-time to non real-time switch
*/
void nvp6134_acp_RT2NRT(unsigned char ch, unsigned char vfmt)
{
	if(ch_mode_status[ch] == NVP6134_VI_3M)
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+(ch%4), (vfmt==PAL?0x03:0x02));
		acp_isp_write(ch, 0xB101, 0x00);
		msleep(300);
	}
	else if(ch_mode_status[ch] == NVP6134_VI_4M)
	{
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+(ch%4), (vfmt==PAL?0x0F:0x0E));
		acp_isp_write(ch, 0xB101, 0x00);
		msleep(300);
	}
	else if(ch_mode_status[ch] == NVP6134_VI_5M_20P)
	{
		acp_isp_write(ch, 0xB101, 0x00);
		msleep(300);
	}
}

/*
support AHD 3M/4M/5M non real-time to real-time switch
*/
void nvp6134_acp_NRT2RT(unsigned char ch)
{
	if(	ch_mode_status[ch] == NVP6134_VI_3M_NRT 	||
		ch_mode_status[ch] == NVP6134_VI_4M_NRT		||
		ch_mode_status[ch] == NVP6134_VI_5M_NRT )
	{
		acp_isp_write(ch, 0xB100, 0x00);
		msleep(300);
	}
}

/*
support AHD 3M/4M/5M real-time to non real-time switch - extension
*/
void nvp6134_acp_RT2NRT_extension(unsigned char ch)
{
	if(	ch_mode_status[ch] == NVP6134_VI_3M 	||
		ch_mode_status[ch] == NVP6134_VI_4M		||
		ch_mode_status[ch] == NVP6134_VI_5M_20P )
	{
		nvp6134_acp_rw_data_extention acpdata;

		memset( &acpdata, 0x00, sizeof(nvp6134_acp_rw_data_extention));
		acpdata.ch = ch;
		acpdata.data[0] = 0x60;
		acpdata.data[1] = 0xB1;
		acpdata.data[2] = 0x01;
		acpdata.data[3] = 0x00;
		acpdata.data[4] = 0x00;
		acpdata.data[5] = 0x00;
		acpdata.data[6] = 0x00;
		acpdata.data[7] = 0x00;
		acp_isp_write_extention( ch, &acpdata );
		msleep(300);
	}
}

/*
support AHD 3M/4M/5M non real-time to real-time switch - extension
*/
void nvp6134_acp_NRT2RT_extension(unsigned char ch)
{
	if(	ch_mode_status[ch] == NVP6134_VI_3M_NRT 	||
		ch_mode_status[ch] == NVP6134_VI_4M_NRT		||
		ch_mode_status[ch] == NVP6134_VI_5M_NRT )
	{
		nvp6134_acp_rw_data_extention acpdata;

		memset( &acpdata, 0x00, sizeof(nvp6134_acp_rw_data_extention));
		acpdata.ch = ch;
		acpdata.data[0] = 0x60;
		acpdata.data[1] = 0xB1;
		acpdata.data[2] = 0x00;
		acpdata.data[3] = 0x00;
		acpdata.data[4] = 0x00;
		acpdata.data[5] = 0x00;
		acpdata.data[6] = 0x00;
		acpdata.data[7] = 0x00;
		acp_isp_write_extention( ch, &acpdata );
		msleep(300);
	}
}

/*
support AHD 3M/4M real-time camera switch between NTSC and PAL
*/
int nvp6134_acp_SetVFmt(unsigned char ch, const unsigned char vfmt)
{
	nvp6134_acp_rw_data_extention acpdata;

	if((vfmt!=NTSC) && (vfmt!=PAL))
	{
		printk("%s vfmt[%d] out of range!!!\n", __FUNCTION__, vfmt);
		return -1;
	}
	if(ch_vfmt_status[ch] == vfmt)
	{
		printk("%s vfmt is %d now!!!\n", __FUNCTION__, vfmt);
		return -2;
	}
	
	acpdata.ch = ch;
	acpdata.data[0] = 0x60;		// register write
	acpdata.data[1] = 0x82;		// Output mode command
	acpdata.data[2] = 0x19;		// Output Format Change mode
	acpdata.data[3] = 0x00;		// Output Mode value
	acpdata.data[4] = 0x00;
	acpdata.data[5] = 0x00;
	acpdata.data[6] = 0x00;
	acpdata.data[7] = 0x00;
	if(	(ch_mode_status[ch] == NVP6134_VI_3M 	 	||
		 ch_mode_status[ch] == NVP6134_VI_3M_NRT 	||
		 ch_mode_status[ch] == NVP6134_VI_4M_NRT 	||
		 ch_mode_status[ch] == NVP6134_VI_4M	) 	&& 
		nvp6134_GetAgcLockStatus(ch)==1) 
	{
		acpdata.data[3] = vfmt^1;   //CAUTION!!! IN CAMERA SIDE 0:PAL, 1:NTSC.
		acp_isp_write_extention(ch, &acpdata);
		msleep(100);
		printk("%s change ch[%d] to %s!!!\n", __FUNCTION__, ch, vfmt==NTSC?"NTSC":"PAL");
	}

	return 0;
}

/*
设置对比度，value是根据nextchip demoboard转换过的，取值范围是0~100.
请自行修改。
亮度，色度，饱和度，锐度 同。
*/
void nvp6134_video_set_contrast(unsigned int ch, unsigned int value, unsigned int v_format)
{	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	if(value >= 100)
	{
		if(ch_mode_status[ch] == NVP6134_VI_1080P_2530)
			gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x10+(ch%4)),(nvp6134_con_tbl[v_format]+value-100));
		else if(ch_mode_status[ch] == NVP6134_VI_EXC_1080P)
		{}
		else if(ch_mode_status[ch] == NVP6134_VI_EXT_1080P)
		{}
		else if(ch_mode_status[ch] == NVP6134_VI_720P_2530 || 
				ch_mode_status[ch] == NVP6134_VI_720P_5060 || 
				ch_mode_status[ch] == NVP6134_VI_HDEX)
			gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x10+(ch%4)),(nvp6134_con_tbl_720P[v_format]+value-100));
		else if(ch_mode_status[ch] == NVP6134_VI_EXC_720P  || 
				ch_mode_status[ch] == NVP6134_VI_EXC_720PRT|| 
				ch_mode_status[ch] == NVP6134_VI_EXC_HDEX)
		{}
		else if(ch_mode_status[ch] == NVP6134_VI_EXT_720PA ||
				ch_mode_status[ch] == NVP6134_VI_EXT_720PB ||
				ch_mode_status[ch] == NVP6134_VI_EXT_720PRT|| 
				ch_mode_status[ch] == NVP6134_VI_EXT_HDAEX || 
				ch_mode_status[ch] == NVP6134_VI_EXT_HDBEX)
		{}
		else if(ch_mode_status[ch] == NVP6134_VI_3M		   || 
			ch_mode_status[ch] == NVP6134_VI_3M_NRT)
		{
			//gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x10+(ch%4)), (nvp6134_con_tbl_3M[v_format]+value-100));
		}
		else if(ch_mode_status[ch] < NVP6134_VI_720P_2530)
			gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x10+(ch%4)),(nvp6134_con_tbl_960H[v_format]+value-100));
		else
		{
			//printk(">>>>DRV:nvp6134_video_set_contrast\n\n");
		}
	}
	else
	{
		if(ch_mode_status[ch] == NVP6134_VI_1080P_2530)
			gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x10+(ch%4)),(nvp6134_con_tbl[v_format]+(0xff-(98-value))));
		else if(ch_mode_status[ch] == NVP6134_VI_EXC_1080P)
		{}
		else if(ch_mode_status[ch] == NVP6134_VI_EXT_1080P)
		{}
		else if(ch_mode_status[ch] == NVP6134_VI_720P_2530 || ch_mode_status[ch] == NVP6134_VI_720P_5060)
			gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x10+(ch%4)),(nvp6134_con_tbl_720P[v_format]+(0xff-(98-value))));
		else if(ch_mode_status[ch] == NVP6134_VI_EXC_720P  || 
				ch_mode_status[ch] == NVP6134_VI_EXC_720PRT|| 
				ch_mode_status[ch] == NVP6134_VI_EXC_HDEX)
		{}
		else if(ch_mode_status[ch] == NVP6134_VI_EXT_720PA ||
				ch_mode_status[ch] == NVP6134_VI_EXT_720PB ||
				ch_mode_status[ch] == NVP6134_VI_EXT_720PRT|| 
				ch_mode_status[ch] == NVP6134_VI_EXT_HDAEX || 
				ch_mode_status[ch] == NVP6134_VI_EXT_HDBEX)
		{}
		else if(ch_mode_status[ch] == NVP6134_VI_3M		   || 
			ch_mode_status[ch] == NVP6134_VI_3M_NRT)
		{
			//gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x10+(ch%4)), (nvp6134_con_tbl_3M[v_format]+(0xff-(98-value))));
		}
		else if(ch_mode_status[ch] < NVP6134_VI_720P_2530)
			gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x10+(ch%4)),(nvp6134_con_tbl_960H[v_format]+(0xff-(98-value))));
		else
		{
			//printk(">>>>DRV:nvp6134_video_set_contrast\n\n");
		}
	}
}

void nvp6134_video_set_brightness(unsigned int ch, unsigned int value, unsigned int v_format)
{
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	if(value >= 100)
	{
		if(ch_mode_status[ch] == NVP6134_VI_1080P_2530)
			gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0C+(ch%4)),(nvp6134_bri_tbl[v_format]+value-100));
		else if(ch_mode_status[ch] == NVP6134_VI_EXC_1080P)
		{}
		else if(ch_mode_status[ch] == NVP6134_VI_EXT_1080P)
		{}
		else if(ch_mode_status[ch] == NVP6134_VI_720P_2530 || 
				ch_mode_status[ch] == NVP6134_VI_720P_5060 || 
				ch_mode_status[ch] == NVP6134_VI_HDEX)
			gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0C+(ch%4)),(nvp6134_bri_tbl_720P[v_format]+value-100));
		else if(ch_mode_status[ch] == NVP6134_VI_EXC_720P  || 
				ch_mode_status[ch] == NVP6134_VI_EXC_720PRT|| 
				ch_mode_status[ch] == NVP6134_VI_EXC_HDEX)
		{}
		else if(ch_mode_status[ch] == NVP6134_VI_EXT_720PA ||
				ch_mode_status[ch] == NVP6134_VI_EXT_720PB ||
				ch_mode_status[ch] == NVP6134_VI_EXT_720PRT|| 
				ch_mode_status[ch] == NVP6134_VI_EXT_HDAEX || 
				ch_mode_status[ch] == NVP6134_VI_EXT_HDBEX)
		{}
		else if(ch_mode_status[ch] == NVP6134_VI_3M		   || 
			ch_mode_status[ch] == NVP6134_VI_3M_NRT)
		{
			//gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0C+(ch%4)), (nvp6134_bri_tbl_3M[v_format]+value-100));
		}
		else if(ch_mode_status[ch] < NVP6134_VI_720P_2530)
			gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0C+(ch%4)),(nvp6134_bri_tbl_960H[v_format]+value-100));
		else
		{
			//printk(">>>>DRV:nvp6134_video_set_brightness\n\n");
		}
	}	
	else
	{
		if(ch_mode_status[ch] == NVP6134_VI_1080P_2530)
			gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0C+(ch%4)),(nvp6134_bri_tbl[v_format]+(0xff-(98-value))));
		else if(ch_mode_status[ch] == NVP6134_VI_EXC_1080P)
		{}
		else if(ch_mode_status[ch] == NVP6134_VI_EXT_1080P)
		{}
		else if(ch_mode_status[ch] == NVP6134_VI_720P_2530 || 
				ch_mode_status[ch] == NVP6134_VI_720P_5060 || 
				ch_mode_status[ch] == NVP6134_VI_HDEX)
			gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0C+(ch%4)),(nvp6134_bri_tbl_720P[v_format]+(0xff-(98-value))));
		else if(ch_mode_status[ch] == NVP6134_VI_EXC_720P  || 
				ch_mode_status[ch] == NVP6134_VI_EXC_720PRT|| 
				ch_mode_status[ch] == NVP6134_VI_EXC_HDEX)
		{}
		else if(ch_mode_status[ch] == NVP6134_VI_EXT_720PA ||
				ch_mode_status[ch] == NVP6134_VI_EXT_720PB ||
				ch_mode_status[ch] == NVP6134_VI_EXT_720PRT|| 
				ch_mode_status[ch] == NVP6134_VI_EXT_HDAEX || 
				ch_mode_status[ch] == NVP6134_VI_EXT_HDBEX)
		{}
		else if(ch_mode_status[ch] == NVP6134_VI_3M		   || 
			ch_mode_status[ch] == NVP6134_VI_3M_NRT)
		{
			//gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0C+(ch%4)), (nvp6134_bri_tbl_3M[v_format]+(0xff-(98-value))));
		}
		else if(ch_mode_status[ch] < NVP6134_VI_720P_2530)
			gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0C+(ch%4)),(nvp6134_bri_tbl_960H[v_format]+(0xff-(98-value))));
		else
		{
			//printk(">>>>DRV:nvp6134_video_set_brightness\n\n");
		}
	}	
}

void nvp6134_video_set_saturation(unsigned int ch, unsigned int value, unsigned int v_format)
{
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	if(ch_mode_status[ch] == NVP6134_VI_1080P_2530)
		gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x3C+(ch%4)),(nvp6134_sat_tbl[v_format]+value-100));
	else if(ch_mode_status[ch] == NVP6134_VI_EXC_1080P)
	{}
	else if(ch_mode_status[ch] == NVP6134_VI_EXT_1080P)
	{}
	else if(ch_mode_status[ch] == NVP6134_VI_720P_2530 || 
			ch_mode_status[ch] == NVP6134_VI_720P_5060 || 
			ch_mode_status[ch] == NVP6134_VI_HDEX)
		gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x3C+(ch%4)),(nvp6134_sat_tbl_720P[v_format]+value-100));
	else if(ch_mode_status[ch] == NVP6134_VI_EXC_720P  || 
			ch_mode_status[ch] == NVP6134_VI_EXC_720PRT|| 
			ch_mode_status[ch] == NVP6134_VI_EXC_HDEX)
	{}
	else if(ch_mode_status[ch] == NVP6134_VI_EXT_720PA ||
			ch_mode_status[ch] == NVP6134_VI_EXT_720PB ||
			ch_mode_status[ch] == NVP6134_VI_EXT_720PRT|| 
			ch_mode_status[ch] == NVP6134_VI_EXT_HDAEX || 
			ch_mode_status[ch] == NVP6134_VI_EXT_HDBEX)
	{}
	else if(ch_mode_status[ch] == NVP6134_VI_3M		   || 
			ch_mode_status[ch] == NVP6134_VI_3M_NRT)
	{
		//gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x3C+(ch%4)), (nvp6134_sat_tbl_3M[v_format]+value-100));
	}		
	else if(ch_mode_status[ch] < NVP6134_VI_720P_2530)
		gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x3C+(ch%4)),(nvp6134_sat_tbl_960H[v_format]+value-100));
	else
	{
		//printk(">>>>DRV:nvp6134_video_set_saturation\n\n");
	}
}

void nvp6134_video_set_hue(unsigned int ch, unsigned int value, unsigned int v_format)
{
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	if(ch_mode_status[ch] == NVP6134_VI_1080P_2530)
		gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x40+(ch%4)), (nvp6134_hue_tbl[v_format]+value-100));
	else if(ch_mode_status[ch] == NVP6134_VI_EXC_1080P)
	{}
	else if(ch_mode_status[ch] == NVP6134_VI_EXT_1080P)
	{}
	else if(ch_mode_status[ch] == NVP6134_VI_720P_2530 || 
			ch_mode_status[ch] == NVP6134_VI_720P_5060 || 
			ch_mode_status[ch] == NVP6134_VI_HDEX)
		gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x40+(ch%4)), (nvp6134_hue_tbl_720P[v_format]+value-100));
	else if(ch_mode_status[ch] == NVP6134_VI_EXC_720P  || 
			ch_mode_status[ch] == NVP6134_VI_EXC_720PRT|| 
			ch_mode_status[ch] == NVP6134_VI_EXC_HDEX)
	{}
	else if(ch_mode_status[ch] == NVP6134_VI_EXT_720PA ||
			ch_mode_status[ch] == NVP6134_VI_EXT_720PB ||
			ch_mode_status[ch] == NVP6134_VI_EXT_720PRT|| 
			ch_mode_status[ch] == NVP6134_VI_EXT_HDAEX || 
			ch_mode_status[ch] == NVP6134_VI_EXT_HDBEX)
	{}
	else if(ch_mode_status[ch] == NVP6134_VI_3M		   || 
			ch_mode_status[ch] == NVP6134_VI_3M_NRT)
	{
		//gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x40+(ch%4)), (nvp6134_hue_tbl[v_format]+value-100));
	}
	else if(ch_mode_status[ch] < NVP6134_VI_720P_2530)
		gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x40+(ch%4)), (nvp6134_hue_tbl_960H[v_format]+value-100));
	else
	{
		//printk(">>>>DRV:nvp6134_video_set_hue\n\n");
	}
}

void nvp6134_video_set_sharpness(unsigned int ch, unsigned int value)
{
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x14+(ch%4)), (0x90+value-100));
}

//u-gain value B0 0x44~0x47
void nvp6134_video_set_ugain(unsigned int ch, unsigned int value)
{
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x44+(ch%4)), value);
}

//v-gain value B0 0x48~0x4b
void nvp6134_video_set_vgain(unsigned int ch, unsigned int value)
{
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x48+(ch%4)), value);
}

/*******************************************************************************
*	Description		: set this value
*	Argurments		: ch(channel)
*	Return value	: void
*	Modify			:
*	warning			: You don't have to change these values.
*******************************************************************************/
void nvp6134_set_common_value(unsigned char ch, int mode)
{
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+(ch%4),0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x22+(ch%4)*4,0x0B);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23+(ch%4)*4,0x43);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x34+ch%4,0x02); //08.25
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x93+ch%4,0x00);	//Hzoom off
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);

	/* Analog IP - EQ bypass 
	 * bank[5~8], 0x58[7~4]:0(bypass), [3~0]:10M, 20M, 50M, 80M */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00, 0xC0); // Clamp speed
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x01, 0x02); // Backend Antialiasing Filter Bandwidth 50Mhz
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05, 0x24);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08, 0x50);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0e, 0x30);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x11, 0x06);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23, 0x00); // PN init
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A, 0x52);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0e, 0x30);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x02);	// Analog IP(bank[5~8], 0x58[7~4]:0(bypass), [3~0]:10M, 20M, 50M, 80M)
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x59, 0x11); // Analog filter bypass( bypass on )
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xB8, 0xB9); // H-PLL No video option These will be recovery by EQ routine.

	/* for EQ(common) */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xC8, 0x04);
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x0A+(ch%4)/2);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], ((ch%2)*0x80+0x74), 0x02);
	printk(">>>>> DRV : CHIPID:%d[sa:0x%X], CH:%d, init Analog IP-EQ bypass and EQ\n", ch/4, nvp6134_iic_addr[ch/4], ch );
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64, 0xB8); 	//for thd A/B detection
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6A, 0x18);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6B, 0x50);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6C, 0xC0); 
	printk(">>>>> DRV : CHIPID:%d[sa:0x%X], CH:%d, Set VFC parameter\n", ch/4, nvp6134_iic_addr[ch/4], ch );

	/* Initialize Digital EQ - disable Digital EQ ( CH1=9x80, CH2=9xA0,  CH3=9xC0, CH4=9xE0 ) */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x80+(0x20*(ch%4)), 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+(0x20*(ch%4)), 0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x0A+(ch%4)/2);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x60+(ch%2)*0x80,0x02); //recovery bankA data
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x61+(ch%2)*0x80,0x01); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x62+(ch%2)*0x80,0x00); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x63+(ch%2)*0x80,0x01); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+(ch%2)*0x80,0x03); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x65+(ch%2)*0x80,0xA0); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x66+(ch%2)*0x80,0x04); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x67+(ch%2)*0x80,0x03); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x68+(ch%2)*0x80,0x03); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x69+(ch%2)*0x80,0x0B); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6A+(ch%2)*0x80,0x0A); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6B+(ch%2)*0x80,0x11); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6C+(ch%2)*0x80,0x0D); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6D+(ch%2)*0x80,0x06); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x6E)+(ch%2)*0x80,0x27);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6F+(ch%2)*0x80,0x00); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x70+(ch%2)*0x80,0x4E); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x71+(ch%2)*0x80,0x6D); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x72+(ch%2)*0x80,0x74); 
}


void nvp6134_setchn_common_cvbs(const unsigned char ch, const unsigned char vfmt)
{
	unsigned char YCmerge, PN_set;

	//analog setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+ch%4,0x00); 

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00,0xC0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x01,0x00); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58,0x00); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x59,0x00); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5B,0x03); 

	//common image setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+ch%4,vfmt==PAL?0xDD:0xA0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0c+ch%4,0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x10+ch%4,vfmt==PAL?0x88:0x88);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x14+ch%4,0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x18+ch%4,0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x21+(ch%4)*4,vfmt==PAL?0x02:0x82);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x3C+ch%4,0x84);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x48+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x4c+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+ch%4,0x00);
	
	//BT656 or BT1120 SET
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	YCmerge = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xed);
	CLE_BIT(YCmerge, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xed, YCmerge);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x69,0x00);

	//SYNC_Detection_Setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05, 0x24);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0E, 0x30);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,vfmt==PAL?0x04:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,vfmt==PAL?0x84:0x84);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x00);	// ( no video option - sync enable)
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,0x00);	// ( sync width )
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd1,vfmt==PAL?0x10:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x57, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90, 0x01);
	
	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08,0x50);     
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x11,0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x1b,vfmt==PAL?0x20:0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,vfmt==PAL?0x10:0x2A);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25,vfmt==PAL?0xCA:0xDA);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x26,0x30);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29,0x30); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2a,0x52); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2b, 0xa8);					

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5f,0x70);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x56,vfmt==PAL?0x00:0x00);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90,vfmt==PAL?0x0D:0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x9b,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb5,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb7,0xff);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb8,0xB8);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xbb,vfmt==PAL?0xb8:0xb8);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd1,vfmt==PAL?0x20:0x00);  

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x20,vfmt==PAL?0x84:0x84);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x27,vfmt==PAL?0x57:0x57);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x76,vfmt==PAL?0x01:0x01);	


	//V_DELAY setting	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6E,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6F,vfmt==PAL?0x00:0x00);

	//each format FSC
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+(ch%4),  vfmt==PAL?0x00:0x00);
	PN_set = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x44);
	CLE_BIT(PN_set, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44, PN_set);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+(ch%4)*4,vfmt==PAL?0xCB:0x1E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x51+(ch%4)*4,vfmt==PAL?0x8A:0x7C);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x52+(ch%4)*4,vfmt==PAL?0x09:0xF0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x53+(ch%4)*4,vfmt==PAL?0x2A:0x21);	
	// H_SCAILER	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97+((ch%4)*0x20),0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x98+((ch%4)*0x20),0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x99+((ch%4)*0x20),0x00);

	// Motion setting     
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x02+((ch%4)*0x07),0x03);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x28+((ch%4)*0x06),0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0x3C);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A+((ch%4)*0x06),vfmt==PAL?0x0C:0x0A);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2B+((ch%4)*0x06),0x06);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x36);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x11);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+(ch%4)*0x20,0x00);
}

void nvp6134_setchn_720h(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> NVP6124_VI_720H(%s), CH:%d <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC", ch );
	nvp6134_setchn_common_cvbs(ch, vfmt);

	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x47:0x47);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0xA6:0xA6);

	//each format standard setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x70:0x60);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x00);

	//each format delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x18+ch%4,0x14);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x30:0x40);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C+ch%4,vfmt==PAL?0x1E:0x1E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0x2D:0x28);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x30:0x30);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,vfmt==PAL?0x18:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);

	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64, 0x00);
	
	// Motion setting   
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x28+((ch%4)*0x06),0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0x2D);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A+((ch%4)*0x06),vfmt==PAL?0x0C:0x0A);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x27);
}

void nvp6134_setchn_960h(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> NVP6124_VI_960H(%s), CH:%d <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC", ch );
	nvp6134_setchn_common_cvbs(ch, vfmt);

	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x47:0x47);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0xA6:0xA6);

	//each format standard setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x10:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x00);

	//each format delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x80:0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C+ch%4,vfmt==PAL?0x1E:0x1E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0x2D:0x28);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x03:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,vfmt==PAL?0x08:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);
	
	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64, 0x00);
	
	// Motion setting   
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x28+((ch%4)*0x06),0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0x3C);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A+((ch%4)*0x06),vfmt==PAL?0x0C:0x0A);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x36);
}

void nvp6134_setchn_1280h(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> NVP6124_VI_1280H(%s), CH:%d <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC", ch );
	nvp6134_setchn_common_cvbs(ch, vfmt);

	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x47:0x47);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x06:0x06);

	//each format standard setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x30:0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x00);

	//each format delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x80:0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C+ch%4,vfmt==PAL?0x1E:0x1E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0x2D:0x28);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x07:0x0a);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,vfmt==PAL?0x18:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);

	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64,vfmt==PAL?0x00:0x01);

	// Motion setting   
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x28+((ch%4)*0x06),0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0x50);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A+((ch%4)*0x06),vfmt==PAL?0x0C:0x0A);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x4A);
}

void nvp6134_setchn_1440h(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> NVP6124_VI_1440H(%s), CH:%d <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC", ch );
	nvp6134_setchn_common_cvbs(ch, vfmt);

	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x47:0x47);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x06:0x06);

	//each format standard setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x50:0x40);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x00);

	//each format delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x10:0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C+ch%4,vfmt==PAL?0x1E:0x1E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0x2D:0x28);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x08:0x07);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,vfmt==PAL?0x18:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x98+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x99+((ch%4)*0x20),vfmt==PAL?0x00:0x00);

	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x56,vfmt==PAL?0x08:0x08);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64, 0x00);
	
	// Motion setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x28+((ch%4)*0x06),0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0x5A);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A+((ch%4)*0x06),vfmt==PAL?0x0C:0x0A);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x54);
}

void nvp6134_setchn_1920h(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_1920h(%s), CH:%d <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC", ch );
	nvp6134_setchn_common_cvbs(ch, vfmt);

	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x47:0x47);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x06:0x06);

	//each format standard setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0xB0:0xA0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x00);

	//each format delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x80:0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C+ch%4,vfmt==PAL?0x1E:0x1E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0x2D:0x28);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,vfmt==PAL?0x18:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);

	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64, 0x00);
	
	// Motion setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x28+((ch%4)*0x06),0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0x78);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A+((ch%4)*0x06),vfmt==PAL?0x0C:0x0A);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x72);
}

void nvp6134_setchn_3840h(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_3840h(%s), CH:%d <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC", ch );
	nvp6134_setchn_common_cvbs(ch, vfmt);

	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x47:0x47); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x46:0x46);

	//each format standard setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0xD0:0xC0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x00);

	//each format delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x84:0x84);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C+ch%4,vfmt==PAL?0x1E:0x1E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0x2D:0x28);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x08:0x06);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x93+ch%4,vfmt==PAL?0x01:0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x98+ch%4,vfmt==PAL?0x0D:0x07);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,vfmt==PAL?0x18:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);

	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64, 0x00);
	
	// Motion setting     
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x28+((ch%4)*0x06),0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0xF0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A+((ch%4)*0x06),vfmt==PAL?0x0C:0x0A);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0xEA);
}

void nvp6134_setchn_common_720p(const unsigned char ch, const unsigned char vfmt)
{
	unsigned char YCmerge, PN_set;
	printk(">>>>> nvp6134_setchn_common_720p(%s), <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );

	//analog setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+ch%4,0x00); 

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00,0xC0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x01,0x02);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58,0x02);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x59,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5B,0x03);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x69,0x00);

	//common image setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0c+ch%4,0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x10+ch%4,vfmt==PAL?0x8D:0x8B);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x14+ch%4,0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x18+ch%4,0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x21+(ch%4)*4,0x82); //reduce color filter's bandwidth
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x3C+ch%4,0x84);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x01:0xF7);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44+ch%4,vfmt==PAL?0x20:0x1A);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x48+ch%4,vfmt==PAL?0x11:0x11);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x4c+ch%4,vfmt==PAL?0xF6:0xF4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+ch%4,vfmt==PAL?0xF5:0xF6);
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,vfmt==PAL?0x04:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,vfmt==PAL?0x84:0x84);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x20,vfmt==PAL?0x87:0x86);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x27,vfmt==PAL?0x57:0x57);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x76,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x57, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2b, 0xa8);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb8, 0x38);                 


	//V_DELAY setting	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6E,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6F,vfmt==PAL?0x00:0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	YCmerge = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xed);
	CLE_BIT(YCmerge, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xed, YCmerge);
	
	// Motion setting     
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x02+((ch%4)*0x07),0x23);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x28+((ch%4)*0x06),0x11);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0x50);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A+((ch%4)*0x06),0x1E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2B+((ch%4)*0x06),0x06);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x4A);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+(ch%4),  vfmt==PAL?0x00:0x00);
	PN_set = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x44);
	CLE_BIT(PN_set, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44, PN_set);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97+((ch%4)*0x20),0x00);
	// H_EXT_MODE ON/OFF
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x11);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+(ch%4)*0x20,0x00);	
}

void nvp6134_setchn_ahd_720p(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_ahd720p(%s), <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	nvp6134_setchn_common_720p(ch, vfmt);

	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x0A:0x0A);

	//each format standard setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x07:0x06);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x00);

	//each format delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x08:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x80:0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C+ch%4,vfmt==PAL?0x1E:0x1E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0x31:0x32);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x09);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x98+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x99+((ch%4)*0x20),vfmt==PAL?0x00:0x00);

	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x1b,vfmt==PAL?0x08:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,vfmt==PAL?0x20:0x20);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25,0xdc);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x26,0xF0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2a,0x52);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5f,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x00);	// ( no video option - sync enable)
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,0x00);	// ( sync width )
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90,0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x9b,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb5,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb7,0xff);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb8,0x38);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xbb,vfmt==PAL?0x04:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd1,0x00);

	//each format FSC
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+(ch%4)*4,vfmt==PAL?0x45:0xED);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x51+(ch%4)*4,vfmt==PAL?0x08:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x52+(ch%4)*4,vfmt==PAL?0x10:0xE5);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x53+(ch%4)*4,vfmt==PAL?0x4F:0x4E);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
}

void nvp6134_setchn_ahd_720pex(const unsigned char ch, const unsigned char vfmt)
{
	unsigned char PN_set=0;
	printk(">>>>> nvp6134_setchn_ahd_720pex(%s), <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	nvp6134_setchn_common_720p(ch, vfmt);

	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x00:0x00); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x40:0x40);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x11);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+(ch%4)*0x20, vfmt==PAL?0x0d:0x0d);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x01+(ch%4)*0x20, vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x02+(ch%4)*0x20, vfmt==PAL?0x80:0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x03+(ch%4)*0x20, vfmt==PAL?0x0a:0x0a);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x04+(ch%4)*0x20, vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05+(ch%4)*0x20, vfmt==PAL?0x0f:0x0c);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x06+(ch%4)*0x20, vfmt==PAL?0x78:0xe4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x07+(ch%4)*0x20, vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+(ch%4)*0x20, vfmt==PAL?0xdc:0xdc);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x10+(ch%4)*0x20, vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x11+(ch%4)*0x20, vfmt==PAL?0xf8:0xf8);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x12+(ch%4)*0x20, vfmt==PAL?0x01:0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x13+(ch%4)*0x20, vfmt==PAL?0xa4:0xa4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x14+(ch%4)*0x20, vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x15+(ch%4)*0x20, vfmt==PAL?0x33:0x33);

	//each format standard setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+ch%4,0x60);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x0E:0x0F);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x04);

	//each format delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x80:0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C+ch%4,vfmt==PAL?0x1E:0x1E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0x31:0x32);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x20:0x21);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,vfmt==PAL?0x00:0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x98+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x99+((ch%4)*0x20),vfmt==PAL?0x00:0x00);

	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x1b,vfmt==PAL?0x08:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,vfmt==PAL?0x20:0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25,0xDB);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x26,0xF0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2a,0x52);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5f,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x75,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x01);	// ( no video option - sync enable)
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,0x20);	// ( sync width )
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90,0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x9b,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb5,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb7,0xff);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb8,0x38);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xbb,vfmt==PAL?0x04:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd1,0x00);

	//each format FSC
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	PN_set = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x44);
	SET_BIT(PN_set, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44, PN_set);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+(ch%4)*4,vfmt==PAL?0x22:0x76);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x51+(ch%4)*4,vfmt==PAL?0x04:0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x52+(ch%4)*4,vfmt==PAL?0x88:0x72);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x53+(ch%4)*4,vfmt==PAL?0x27:0x27);

	// Motion setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0xA0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x9A);
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x0A+(ch%4)/2);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x60+(ch%2)*0x80,0x2A); // by peter 16-08-29
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x61+(ch%2)*0x80,0x03); // by peter 16-08-29
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x62+(ch%2)*0x80,0x07); // by peter 16-08-29
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x63+(ch%2)*0x80,0x05); // by peter 16-08-29
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+(ch%2)*0x80,0x07); // by peter 16-08-29
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x65+(ch%2)*0x80,0xA0); // by peter 16-08-29
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x66+(ch%2)*0x80,0x14); // by peter 16-08-29
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x67+(ch%2)*0x80,0x12); // by peter 16-08-29
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x68+(ch%2)*0x80,0x06); // by peter 16-08-29
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x69+(ch%2)*0x80,0x22); // by peter 16-08-29
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6A+(ch%2)*0x80,0x0A); // by peter 16-08-29
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6B+(ch%2)*0x80,0x26); // by peter 16-08-29
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6C+(ch%2)*0x80,0x04); // by peter 16-08-29
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6D+(ch%2)*0x80,0x29); // by peter 16-08-29
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x6E)+(ch%2)*0x80,0x38); // by peter 16-08-29
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6F+(ch%2)*0x80,0x18); // by peter 16-08-29
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x70+(ch%2)*0x80,0x15); // by peter 16-08-29
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x71+(ch%2)*0x80,0x24); // by peter 16-08-29
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x72+(ch%2)*0x80,0xC0); // by peter 16-08-29
}

void nvp6134_setchn_exc_720p(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_exc_720p(%s), <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	//nvp6134_setchn_common_720p(ch, vfmt); 

	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x20:0x20);

	// by romoe(2016-06-24) stand setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x07:0x06);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x02);

	nvp6134_setchn_common_720p(ch, vfmt);	

	//each format standard setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x30:0x30);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x07:0x06);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x02);

	//each format delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x80:0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C+ch%4,vfmt==PAL?0x1E:0x1E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0x2E:0x2E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x00:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x00:0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97+((ch%4)*0x20),vfmt==PAL?0x29:0x19);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x98+((ch%4)*0x20),vfmt==PAL?0xC0:0xA2);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x99+((ch%4)*0x20),vfmt==PAL?0x01:0x01);

	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x1b,vfmt==PAL?0x08:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x20,vfmt==PAL?0x90:0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,vfmt==PAL?0x30:0x30);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25,vfmt==PAL?0xDC:0xDC);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x26,0x30);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2a,0xD2);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5f,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x01);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,0x40);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90,0x05);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x9b,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb5,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb7,0xff);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb8,0x38);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xbb,vfmt==PAL?0x04:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd1,0x00);

	//each format FSC
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+(ch%4)*4,vfmt==PAL?0x61:0x60);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x51+(ch%4)*4,vfmt==PAL?0xDE:0xDE);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x52+(ch%4)*4,vfmt==PAL?0xCE:0xCE);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x53+(ch%4)*4,vfmt==PAL?0x90:0x90);
}

void nvp6134_setchn_exc_720pex(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_exc_720pex(%s), <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	nvp6134_setchn_exc_720p(ch, vfmt);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x80:0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x0B:0x0A);		// set video format
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x24:0x01);
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x40:0x40);
	
	// Motion setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0xA0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x9A);
}


void nvp6134_setchn_ext_bsf(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_ext_bsf(%s), <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x0A+((ch%4)/2));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x60+((ch%2)*0x80),0xA0);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x61+((ch%2)*0x80),0x1 );
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x62+((ch%2)*0x80),0x1 );
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x63+((ch%2)*0x80),0x1 );
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+((ch%2)*0x80),0x3 );			
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x65+((ch%2)*0x80),0x2 );
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x66+((ch%2)*0x80),0x3 );
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x67+((ch%2)*0x80),0x2 );
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x68+((ch%2)*0x80),0x9 );			
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x69+((ch%2)*0x80),0x9 );
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6A+((ch%2)*0x80),0x2a);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6B+((ch%2)*0x80),0x4 );
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6C+((ch%2)*0x80),0x17);			
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6D+((ch%2)*0x80),0x1b);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x6E)+((ch%2)*0x80),0x5 );
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6F+((ch%2)*0x80),0x0 );
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x70+((ch%2)*0x80),0x46);			
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x71+((ch%2)*0x80),0x89);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x72+((ch%2)*0x80),0xa6);
		                                                        
}

void nvp6134_setchn_exta_720p(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_exta_720p(%s), <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );

	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x55:0x55);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x04:0x04);

	//each format standard setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+ch%4,0x60);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x02:0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x07:0x06);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x01);

	nvp6134_setchn_common_720p(ch, vfmt);
	nvp6134_setchn_ext_bsf(ch, vfmt);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,vfmt==PAL?0x04:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,vfmt==PAL?0x84:0x84);

	//each format delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x80:0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C+ch%4,vfmt==PAL?0x1E:0x1E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0xae:0xae);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97+((ch%4)*0x20),vfmt==PAL?0x51:0x51);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x98+((ch%4)*0x20),vfmt==PAL?0x80:0x0C);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x99+((ch%4)*0x20),vfmt==PAL?0x00:0x00);

	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x1b,vfmt==PAL?0x08:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x20,vfmt==PAL?0x98:0x98);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,vfmt==PAL?0x12:0x12);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25,0xDB);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x26,0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2a,0xD2);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5f,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x01);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,vfmt==PAL?0x20:0x40);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90,vfmt==PAL?0x05:0x05);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x9b,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb5,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb7,0xff);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb8,0xB8);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xbb,vfmt==PAL?0x04:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd1,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd4,0x1F);	// for fsc_locking.
        
	//each format FSC
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+(ch%4)*4,0x8B);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x51+(ch%4)*4,0x2E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x52+(ch%4)*4,0xBB);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x53+(ch%4)*4,0x48);
}

void nvp6134_setchn_exta_720pex(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_exta_720pex(%s), <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	nvp6134_setchn_exta_720p(ch, vfmt);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x0B:0x0A);	// set video format
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x60:0xA0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x00:0x20);
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x44:0x44);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x00);	      // sync width - enable
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,0x40);	      // sync width - max value

	// Motion setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0xA0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x9A);
}

void nvp6134_setchn_extb_720p(const unsigned char ch, const unsigned char vfmt)
{
	unsigned char PN_set = 0x00;

	printk(">>>>> nvp6134_setchn_extb_720p(%s), <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );

	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x20:0x20);

	//each format standard setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+ch%4,0x60);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x07:0x06);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x03);

	nvp6134_setchn_common_720p(ch, vfmt);
	nvp6134_setchn_ext_bsf(ch, vfmt);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,vfmt==PAL?0x04:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,vfmt==PAL?0x84:0x84);

	//each format delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x14:0x14);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x84:0x84);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C+ch%4,vfmt==PAL?0x1E:0x1E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0x2E:0x2E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x98+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x99+((ch%4)*0x20),vfmt==PAL?0x00:0x00);

	//common image setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0c+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x10+ch%4,vfmt==PAL?0x90:0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x3C+ch%4,0xb0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x93+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x98+ch%4,0x00);	
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x20,vfmt==PAL?0x86:0x86);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x27,vfmt==PAL?0x57:0x57);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2b,vfmt==PAL?0xA8:0xA8);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,vfmt==PAL?0x04:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,vfmt==PAL?0x84:0x84);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x57, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x76,vfmt==PAL?0x00:0x00);

	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x1b,vfmt==PAL?0x08:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,vfmt==PAL?0x3f:0x3f);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25,0xda);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x26,0xF0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2a,0xD2);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5f,0x50);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6e,0x10);  // V-delay enable
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6f,0x18);  // V-delay control
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x00);	// ( no video option - sync enable)
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,0x00);	// ( sync width )
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90,0x05);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x9b,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb5,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb7,0xff);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb8,0x38);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xbb,vfmt==PAL?0x04:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd1,0x00);

	//each format FSC
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	PN_set = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x44);
	SET_BIT(PN_set, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44, PN_set);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+(ch%4)*4,vfmt==PAL?0x86:0x86);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x51+(ch%4)*4,vfmt==PAL?0x2e:0x2e);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x52+(ch%4)*4,vfmt==PAL?0xbc:0xbc);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x53+(ch%4)*4,vfmt==PAL?0x48:0x48);
}

void nvp6134_setchn_extb_720pex(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_extb_720pex(%s), <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	nvp6134_setchn_extb_720p(ch, vfmt);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x07:0x06);	// set video format
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,0x40);
	
	// Motion setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0xA0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x9A);
  	
	// H_EXT_MODE ON/OFF
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x11);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+(ch%4)*0x20,0x10);
}

void nvp6134_setchn_ahd_720p5060(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_ahd_720p5060(%s), <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	nvp6134_setchn_common_720p(ch, vfmt);

	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x40:0x40);

	//each format standard setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0C+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x10+ch%4,0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x3C+ch%4,0x7C);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x03:0x03);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x48+ch%4,vfmt==PAL?0xF8:0xF8);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x4C+ch%4,vfmt==PAL?0xF2:0xF2);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+ch%4,vfmt==PAL?0xF6:0xF6);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x05:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,vfmt==PAL?0x04:0xEE);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,vfmt==PAL?0x86:0xC6);

	//each format delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x80:0x82);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C+ch%4,vfmt==PAL?0x1E:0x1E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0x30:0x31);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x09);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x98+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x99+((ch%4)*0x20),vfmt==PAL?0x00:0x00);

	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x1b,vfmt==PAL?0x08:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x20,vfmt==PAL?0x82:0x82); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,vfmt==PAL?0x2A:0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25,0xdc);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x26,0x30);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29,0x1F);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2a,0x52);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5f,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x01);	// ( no video option - sync enable)
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,0x30);	// ( sync width )
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90,0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x9b,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb5,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb7,vfmt==PAL?0xFF:0xFC);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb8,0x38);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xbb,vfmt==PAL?0x04:0xFE);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd1,0x00);

	//each format FSC
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+(ch%4)*4,0x2C);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x51+(ch%4)*4,0xE7);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x52+(ch%4)*4,0xCF);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x53+(ch%4)*4,0x52);

	// H_EXT_MODE ON/OFF
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x11);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+(ch%4)*0x20,0x00);	//(00=OFF / 10=H_EX_MODE_ON)
}
/* for nextchip test only */
void nvp6134_setchn_ahd_M_720p5060(const unsigned char ch, const unsigned char vfmt)
{
	unsigned char YCmerge;

	printk(">>>>> nvp6134_setchn_ahd_M_720p5060(%s), <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	nvp6134_setchn_common_720p(ch, vfmt);

		//each format basic clk
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
		//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x00:0x00);   // ADC Clock
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,0xE4);   // ADC Clock by song + 2016-06-05
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x44:0x44);

		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);

		YCmerge = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xed);
		CLE_BIT(YCmerge, (ch%4));
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xed, YCmerge);

		//Comb filter coeff manual value. //have to check the channel index
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x0A);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x60, 0x80);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x61, 0x01);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x62, 0x02);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x63, 0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64, 0x03);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x65, 0x0A);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x66, 0x05);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x67, 0x01);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x68, 0x09);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x69, 0x0D);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6A, 0xA8);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6B, 0x01);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6C, 0x16);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6D, 0x1F);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6E, 0x02);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6F, 0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x70, 0x43);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x71, 0x8D);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x72, 0xAC);

		//each format standard setting
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+ch%4,0x60);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x18+ch%4,0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23+(ch*4),0x43);
		//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x20:0x20);	// by romeo(-) 2016-04-26
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x00:0x00);		// by romeo(+) 2016-04-26
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x05:0x04);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x01);

		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,vfmt==PAL?0xee:0xee);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,vfmt==PAL?0xc6:0xc6);

		//each format delay
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x10:0x10);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,0x2D);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C+ch%4,vfmt==PAL?0x1E:0x1E);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0xAF:0xAF);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x10:0x10);
		//gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x0D:0x10);	// by romeo(-) 2016-04-26
		gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x04:0x10);	// by romeo(+) 2016-04-26
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,0x08);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);


		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97+(ch*0x20),vfmt==PAL?0x00:0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x98+(ch*0x20),vfmt==PAL?0x00:0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x99+(ch*0x20),vfmt==PAL?0x00:0x00);

		//common image hidden
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08,0x50);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x1b,vfmt==PAL?0x08:0x08);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,vfmt==PAL?0x20:0x20);
		//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25,0xdc);	// by Guy(-)
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25,0xcc);	// by Guy(+) - 2016-04-28
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x26,0x30);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29,0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2a,0xD2);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5f,0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x01);	// romeo_0420 - for test
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,0x20);	// romeo_0420 - for test
		//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x00);	// romeo_0420( org )
		//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,0x00);	// romeo_0420( org )
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x01);	// by song(+)( no video option - sync enable)
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,0x50);	// by song(+)( sync width )
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90,0x05);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x9b,0x80);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb5,0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb7,0xff);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb8,0x38);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xbb,vfmt==PAL?0x04:0x04);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd1,0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xD4,0x1F);	//16.06.04	FSC Auto reset


		//each format FSC
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+(ch%4),vfmt==PAL?0x00:0x00);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44,         YCmerge);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+(ch%4)*4,0x72);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x51+(ch%4)*4,0xF5);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x52+(ch%4)*4,0x31);
		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x53+(ch%4)*4,0x37);

}

void nvp6134_setchn_exc_720p5060(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_exc_720p5060(%s), <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	//nvp6134_setchn_common_720p(ch, vfmt); 

	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x55:0x55);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x44:0x44);

	//each format standard setting( by romeo(2016-06-24)
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x05:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x02);

	nvp6134_setchn_common_720p(ch, vfmt); 
	nvp6134_setchn_ext_bsf(ch, vfmt);

	//each format standard setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+ch%4,0x60);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x20:0x20);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,vfmt==PAL?0xEE:0xEE);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,vfmt==PAL?0xC6:0xC6);

	//each format delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x80:0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C+ch%4,vfmt==PAL?0x1E:0x1E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0x2E:0x2F);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x00:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x00:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97+((ch%4)*0x20),vfmt==PAL?0x29:0x19);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x98+((ch%4)*0x20),vfmt==PAL?0xC4:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x99+((ch%4)*0x20),vfmt==PAL?0x01:0x00);

	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x1b,vfmt==PAL?0x08:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,vfmt==PAL?0x20:0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25,0xdc);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x26,0x30);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2a,0xD2);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5f,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x75,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x01);	// ( no video option - sync enable)
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,0x40);	// ( sync width )
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90,0x05);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x9b,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb5,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb7,0xff);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb8,0x38);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xbb,vfmt==PAL?0x04:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd1,0x00);

	//each format FSC
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+(ch%4)*4,0x4B);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x51+(ch%4)*4,0x4F);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x52+(ch%4)*4,0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x53+(ch%4)*4,0x83);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8d+((ch%4)*0x20),vfmt==PAL?0xa0:0xa0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+((ch%4)*0x20),vfmt==PAL?0x9c:0x9c);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8f+((ch%4)*0x20),vfmt==PAL?0x3d:0x3d);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90+((ch%4)*0x20),vfmt==PAL?0xff:0xff);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x91+((ch%4)*0x20),vfmt==PAL?0xf7:0xf7);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x92+((ch%4)*0x20),vfmt==PAL?0x04:0x04);
	// Motion setting   /* (+) by romeo_0425 */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0x78);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x72);
}

void nvp6134_setchn_ext_720p5060(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_ext_720p5060(%s), <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );

	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x55:0x55);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x44:0x44);

	//each format standard setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+ch%4,0x60);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x00:0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x05:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x01);

	nvp6134_setchn_common_720p(ch, vfmt);
	nvp6134_setchn_ext_bsf(ch, vfmt);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,vfmt==PAL?0xEE:0xEE);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,vfmt==PAL?0xC6:0xC6);

	//each format delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x80:0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C+ch%4,vfmt==PAL?0x1E:0x1E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0xAF:0xAF);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x14:0x14);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x98+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x99+((ch%4)*0x20),vfmt==PAL?0x00:0x00);

	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x1b,vfmt==PAL?0x08:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,vfmt==PAL?0x20:0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25,0xcc);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x26,0x30);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2a,0xD2);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5f,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90,0x05);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x9b,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb5,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb7,0xff);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb8,0x38);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xbb,vfmt==PAL?0x04:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd1,0x00);

	//each format FSC
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+(ch%4)*4,0x4B);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x51+(ch%4)*4,0x4F);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x52+(ch%4)*4,0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x53+(ch%4)*4,0x83);
}

void nvp6134_setchn_common_fhd(const unsigned char ch, const unsigned char vfmt)
{
	unsigned char YCmerge, PN_set;

	printk(">>>>> NVP6124_VI_1080P_2530(%s), CH:%d, <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC", ch );
	//analog setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+ch%4,0x10);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00,0xC0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x01,0x03);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58,0x03);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x59,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5B,0x03);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x69,0x00);

	//common image setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0c+ch%4,0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x10+ch%4,0x88);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x14+ch%4,0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x18+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x21+(ch%4)*4,0x82);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x22+(ch%4)*4,0x0b);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x3C+ch%4,0x84);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x48+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x4c+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+ch%4,0x00);

	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);    
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x1b, 0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x20,vfmt==PAL?0x84:0x84);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x26, 0xF0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x27,vfmt==PAL?0x57:0x57);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x76,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x57, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5f, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2b, 0xa8);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xD1, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xD4, 0x00);                 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb8, 0xb9);	

	//V_DELAY setting	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0x80:0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6E,vfmt==PAL?0x10:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6F,vfmt==PAL?0x2a:0x2b);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);					
	YCmerge = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xed);
	CLE_BIT(YCmerge, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xed, YCmerge);
	
	// Motion setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x02+((ch%4)*0x07),0x23);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x28+((ch%4)*0x06),0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0x78);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A+((ch%4)*0x06),0x2D);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2B+((ch%4)*0x06),0x06);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x72);
	//each format FSC
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+(ch%4),0x00);
	PN_set = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x44);
	CLE_BIT(PN_set, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44, PN_set);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97+((ch%4)*0x20),0x00);

	// H_EXT_MODE ON/OFF
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x11);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+(ch%4)*0x20,0x00);	//(00=OFF / 10=H_EX_MODE_ON)
}

void nvp6134_setchn_ahd_1080p2530(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_ahd_1080p2530(%s), CH:%d, <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC", ch );
	nvp6134_setchn_common_fhd(ch, vfmt);

	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x40:0x40);

	//each format standard setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x18+ch%4,0x30);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x03:0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,vfmt==PAL?0x04:0xEE);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,vfmt==PAL?0x84:0xC6);
	
	//each format delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x7D:0x89);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C+ch%4,vfmt==PAL?0x9E:0x9E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0x80:0xC0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,vfmt==PAL?0x09:0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x98+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x99+((ch%4)*0x20),vfmt==PAL?0x00:0x00);

	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x1b,vfmt==PAL?0x08:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x20,vfmt==PAL?0x90:0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,vfmt==PAL?0x10:0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25,vfmt==PAL?0xDC:0xDC);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x26,0xF0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2a,0x52);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58,0x03);	// Analog IP(bank[5~8], 0x58[7~4]:0(bypass), [3~0]:10M, 20M, 50M, 80M)

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5f,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x75,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x01);	// ( no video option - sync enable)
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,0x20);	// ( sync width )
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90,vfmt==PAL?0x01:0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x9b,0x80);		
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb5,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb7,vfmt==PAL?0xff:0xff);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb8,0x38);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xbb,vfmt==PAL?0x04:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd1,0x20);
	
	//each format FSC
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+(ch%4)*4,vfmt==PAL?0xAB:0x2C);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x51+(ch%4)*4,vfmt==PAL?0x7D:0xAF);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x52+(ch%4)*4,vfmt==PAL?0xC3:0xCA);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x53+(ch%4)*4,vfmt==PAL?0x52:0x52);
}

//视频丢失的时候，设置为此模式
void nvp6134_setchn_ahd_1080p_novideo(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_ahd_1080p_novideo(%s), CH:%d, <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC", ch );
	nvp6134_setchn_common_fhd(ch, vfmt);

	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x40:0x40);

	//each format standard setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23+(ch%4)*4,0x41);	//novideo detection 06.17
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x03:0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,vfmt==PAL?0x04:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,0xEE);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,vfmt==PAL?0x86:0x86);	

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x76,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x77,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x78,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x79,0x11);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x98+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x99+((ch%4)*0x20),vfmt==PAL?0x00:0x00);

	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58,0x03);	//  (+) Analog IP(bank[5~8], 0x58[7~4]:0(bypass), [3~0]:10M, 20M, 50M, 80M)
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb5,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb7,vfmt==PAL?0xf0:0xff);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb8,0xb9);  //16-06-27
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xbb,vfmt==PAL?0xbb:0x04);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84, 0x01);	// sync width - enable
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86, 0x30);	// sync width - max value

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05, 0xA4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08, 0x60);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0E, 0x32);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x11, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24, 0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25, 0xDC);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x27, 0x57);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C, 0x00);  //clean status
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+ch%4,0x00);

	printk( ">>>>> DRV[%s:%d] CH:%d, NO video!!!!!\n", __func__, __LINE__, ch );
}

void nvp6134_setchn_ahd_1080p_NRT(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_ahd_1080p_NRT(%s), CH:%d, <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC", ch );
	nvp6134_setchn_ahd_1080p2530(ch, vfmt);


	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x0a:0x0a);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x0a:0x0a);

	//each format delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x89:0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x00:0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,vfmt==PAL?0x09:0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);

	// Motion setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0x3c);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x39);
	printk(">>>>> DRV[%s:%d] CH:%d, Motion setting!!!!!\n", __func__, __LINE__, ch ); //Andy

	// H_EXT_MODE ON/OFF
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x11);
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+(ch%4)*0x20,0x10);	//(When AUTO mode : H_EX_MODE_ON(11x00=0x10))
	                                                         	   	//(When 1Port 4ch NRT mode : H_EX_MODE_ON(11x00=0x00))
}

void nvp6134_setchn_exc_1080p2530(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_exc_1080p2530(%s), CH:%d, <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC", ch );

	//nvp6134_setchn_common_fhd(ch, vfmt);

	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);					
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0x55:0x55);		
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x44:0x44);

	//each format standard setting 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x03:0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x02);

	//each format standard setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+ch%4,0x60);                 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x20:0x20);
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x03:0x02); 
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x02);				

	nvp6134_setchn_common_fhd(ch, vfmt);	
	nvp6134_setchn_ext_bsf(ch, vfmt);


	//each format delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x3C+ch%4,0x90); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x70:0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C+ch%4,vfmt==PAL?0x9E:0x9E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0xBF:0xBF);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,vfmt==PAL?0x08:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97+((ch%4)*0x20),vfmt==PAL?0x49:0x39);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x98+((ch%4)*0x20),vfmt==PAL?0x50:0x70);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x99+((ch%4)*0x20),vfmt==PAL?0x02:0x01);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,vfmt==PAL?0x04:0xEE);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,vfmt==PAL?0x84:0xC6);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08,0x56);
	
	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x20,vfmt==PAL?0x90:0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x1b,vfmt==PAL?0x08:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,vfmt==PAL?0x18:0x18);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25,vfmt==PAL?0xDC:0xDC);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x26,0xD0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29,0x1F);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2a,0xD2);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5f,vfmt==PAL?0x30:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,0x40);	// ( sync width )
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90,vfmt==PAL?0x05:0x05);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x9b,vfmt==PAL?0x00:0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb5,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb7,0xff);
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb8,0x38);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xbb,vfmt==PAL?0x04:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd1,vfmt==PAL?0x10:0x10);

	//each format FSC
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+(ch%4)*4,vfmt==PAL?0x4C:0x4B);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x51+(ch%4)*4,vfmt==PAL?0x4F:0x4F);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x52+(ch%4)*4,vfmt==PAL?0x04:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x53+(ch%4)*4,vfmt==PAL?0x83:0x83);
	
    gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8C+((ch%4)*0x20)  ,vfmt==PAL?0x00:0x00);
    gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8D+((ch%4)*0x20)  ,vfmt==PAL?0x90:0x90);
    gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8E)+((ch%4)*0x20),vfmt==PAL?0x9c:0x9c);
    gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8F+((ch%4)*0x20)  ,vfmt==PAL?0x3d:0x3d);
    gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90+((ch%4)*0x20)  ,vfmt==PAL?0xff:0xff);
    gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x91+((ch%4)*0x20)  ,vfmt==PAL?0xf7:0xf7);
    gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x92+((ch%4)*0x20)  ,vfmt==PAL?0x04:0x10);
}

void nvp6134_setchn_ext_1080p2530(const unsigned char ch, const unsigned char vfmt)
{
	unsigned char PN_set = 0;
	printk(">>>>> nvp6134_setchn_ext_1080p2530(%s), CH:%d, <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC", ch );

	nvp6134_setchn_common_fhd(ch, vfmt);
	
	//each format basic clk
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4, (ch%2==0)?0xf5:0x05);	// Clk delay
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,vfmt==PAL?0xE5:0xD5);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,vfmt==PAL?0x44:0x44);

	//each format standard setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x03:0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x01);
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,vfmt==PAL?0x04:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,vfmt==PAL?0x84:0x84);

	//each format delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,vfmt==PAL?0x10:0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,vfmt==PAL?0x60:0x60);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C+ch%4,vfmt==PAL?0x9E:0x9E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,vfmt==PAL?0xBF:0xBF);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,vfmt==PAL?0x10:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,vfmt==PAL?0x08:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);
	
	//common image setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0c+ch%4,0x00);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x10+ch%4,0xA0);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x14+ch%4,0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x18+ch%4,0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x21+(ch%4)*4,0x82);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x3C+ch%4,0xB0); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,0x00); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44+ch%4,0x00); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x48+ch%4,0x00); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x4c+ch%4,0x00); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+ch%4,0x00); 	
	
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xff,0x09);
	//original pn value
	#if 1	//crystal uses this setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+(ch%4)*4,vfmt==PAL?0x16:0xC5); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x51+(ch%4)*4,vfmt==PAL?0x5D:0x20); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x52+(ch%4)*4,vfmt==PAL?0x76:0x7B); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x53+(ch%4)*4,vfmt==PAL?0x91:0x95);
	PN_set = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x44); 
	CLE_BIT(PN_set, (ch%4)); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44,PN_set);
	#else
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+(ch%4)*4,vfmt==PAL?0x8B:0x8B);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x51+(ch%4)*4,vfmt==PAL?0x2E:0x2E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x52+(ch%4)*4,vfmt==PAL?0xBA:0xBA);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x53+(ch%4)*4,vfmt==PAL?0x48:0x48);
	PN_set = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x44);
	SET_BIT(PN_set, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44,PN_set);
	#endif
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x97+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x98+((ch%4)*0x20),vfmt==PAL?0x00:0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x99+((ch%4)*0x20),vfmt==PAL?0x00:0x00);

	//common image hidden
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x1b,vfmt==PAL?0x08:0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x20,vfmt==PAL?0x86:0x86);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,vfmt==PAL?0x18:0x18);
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25,0xDC);  //color locking issue modified by song 09.25
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25,0xCC);  // re-modefied by peter 16.10.09
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x26,0x90);  // org 0xF0
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2a,0xD2);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2b,0xA8);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5f,0x00);
    gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6E,0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6F,0x29);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x01);	// ( no video option - sync enable)
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,vfmt==PAL?0x20:0x40);	// ( sync width )
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90,vfmt==PAL?0x05:0x05);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x9b,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb5,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb7,0xff);
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xb8,0x38);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xbb,vfmt==PAL?0x04:0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd1,0x00);
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd4,0x1F);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd4,0x00);
}

void nvp6134_setchn_common_3M(const unsigned char ch, const unsigned char vfmt)
{
	//analog setting
	nvp6134_setchn_common_fhd(ch, vfmt);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00,0xC0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x01,0x03);  // Analog Front Filter 20MHz
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58,0x03);  // Analog Back Filter 20MHz
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x59,0x00);  // Aliasing Filter path
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5B,0x03);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x69,0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x11);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+((ch%4)*0x20), 0x00);	

	//common image setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0c+ch%4,0x00);		
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x10+ch%4,0x8B);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x14+ch%4,0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x18+ch%4,0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x21+(ch%4)*4,0x82);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x3C+ch%4,0x85);		
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,0xFE);		
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x48+ch%4,0x07);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x4c+ch%4,0xFB);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+ch%4,0xFB);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x20,0x83);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x27,0x57);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A,0xD2);     	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2B,0xA8);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29,0x30);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,0xEE);
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,0xC6);  //TODO
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,0x46);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xB8,0x38);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xBB,0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xD1,0x00);

	// Motion setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x02+((ch%4)*0x07),0x23);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x28+((ch%4)*0x06),0x11);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A+((ch%4)*0x06),0x40);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2B+((ch%4)*0x06),0x06);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x3A);
	
	// H_EXT_MODE ON/OFF
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x11);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+(ch%4)*0x20,0x00);	//(00=OFF / 10=H_EX_MODE_ON)
}

void nvp6134_setchn_ahd_3M(const unsigned char ch, const unsigned char vfmt)
{
	unsigned char YCmerge, PN_set;

	printk(">>>>> nvp6134_setchn_ahd_3M (%s) <<<<<\n", (vfmt==PAL) ? "25P" : "30P" );
	nvp6134_setchn_common_3M(ch, vfmt);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x18+ch%4,0x20);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23+(ch%4)*4,0x41);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,0x5E);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,0x80); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x03:0x02);  //0x02=30fps
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,0x07);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4, (ch%2==0)?0xf5:0x05);	// Clk delay
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,0x05);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,0x44);
	
	YCmerge = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xed);
	SET_BIT(YCmerge, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xED, YCmerge);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x57, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x69,0x10);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6E,0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6F,0x68);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,vfmt==PAL?0x20:0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4],0xff,0x09);
	// original PN value
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x50+((ch%4)*0x4),vfmt==PAL?0x06:0xEC);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x51+((ch%4)*0x4),vfmt==PAL?0x3A:0xc3);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x52+((ch%4)*0x4),vfmt==PAL?0x6D:0x67);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x53+((ch%4)*0x4),vfmt==PAL?0x48:0x48);
	PN_set = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x44);
	SET_BIT(PN_set, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44, PN_set);	

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x01);	// ( no video option - sync enable)
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,0x30);	// ( sync width )
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23,0x80);

	// Motion setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0x40);
}

void nvp6134_setchn_ahd_3MNRT(const unsigned char ch, const unsigned char vfmt)
{
	unsigned char YCmerge, PN_set;
	printk(">>>>> nvp6134_setchn_ahd_3M_18P(%s) <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	nvp6134_setchn_common_3M(ch, vfmt);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23+(ch%4)*4,0x41);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,0x04);  	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,0x03);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,0x0A);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,0x43);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,0x40);
	YCmerge = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xed);
	CLE_BIT(YCmerge, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xed, YCmerge);
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x20,0x82);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,0x20);		
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6E,0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6F,0x68);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x01);		
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,0x20);	

	//common image setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0c+ch%4,0x00);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x10+ch%4,0x8B);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x3C+ch%4,0x83); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,0x00); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44+ch%4,0x00); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x48+ch%4,0x05); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x4c+ch%4,0xFA); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+ch%4,0xF9); 

	gpio_i2c_write(nvp6134_iic_addr[ch/4],0xff,0x09);
	//original pn value
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x50+((ch%4)*0x4),0xA7);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x51+((ch%4)*0x4),0x57);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x52+((ch%4)*0x4),0xCA);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x53+((ch%4)*0x4),0x52);
	PN_set = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x44);
	SET_BIT(PN_set, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44, PN_set);
}

void nvp6134_setchn_ext_3MNRT(const unsigned char ch, const unsigned char vfmt)
{
	unsigned char PN_set;
 	printk(">>>>> nvp6134_setchn_ext_3M_18P(%s) <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	nvp6134_setchn_ahd_3MNRT(ch, vfmt);
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4, 0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4, 0x04);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4, 0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4, 0x10);
	#ifdef  THD_3MNRT_HSCALER
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,0x02);
	#else
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,0x10);
	#endif 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,0x00);  
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4, (ch%2==0)?0x45:0x95);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,0x44);

	//common image setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0c+ch%4,0x00);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x10+ch%4,0x80); //08.25
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,0x1F); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x34+ch%4,0xA6); //08.25
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x3C+ch%4,0xA8); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x40+ch%4,0x04); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44+ch%4,0xF8); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x48+ch%4,0x00); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x4c+ch%4,0x00); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50+ch%4,0xF9);
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x20, 0x86); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23, 0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24, 0x0E); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25, 0xDA);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x26, 0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x57, 0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6E, 0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6F, 0x2B); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90, 0x05);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xD0, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xD1, 0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4],0xff,0x09);
	//original pn value
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x50+((ch%4)*0x4),0x8B);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x51+((ch%4)*0x4),0x2E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x52+((ch%4)*0x4),0xBA);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x53+((ch%4)*0x4),0x48);
	PN_set = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x44);
	SET_BIT(PN_set, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44, PN_set);

	#ifdef THD_3MNRT_HSCALER  //  THD 1920*1536==>2048*1536
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x98+((ch%4)*0x20),0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x99+((ch%4)*0x20),0x03);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x9A+((ch%4)*0x20),0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x9B+((ch%4)*0x20),0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x9C+((ch%4)*0x20),0x77);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x9D+((ch%4)*0x20),0xc8);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],(0x9E)+((ch%4)*0x20),0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x97+((ch%4)*0x20),0xe1);
	#endif 

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x11);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+((ch%4)*0x20), 0x0F);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x01+((ch%4)*0x20), 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x02+((ch%4)*0x20), 0xA0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x03+((ch%4)*0x20), 0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x04+((ch%4)*0x20), 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05+((ch%4)*0x20), 0x13);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x06+((ch%4)*0x20), 0x88);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x07+((ch%4)*0x20), 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+((ch%4)*0x20), 0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0A+((ch%4)*0x20), 0x06);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0B+((ch%4)*0x20), 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0C+((ch%4)*0x20), 0x06);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0D+((ch%4)*0x20), 0x72);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x10+((ch%4)*0x20), 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x11+((ch%4)*0x20), 0x2C);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x12+((ch%4)*0x20), 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x13+((ch%4)*0x20), 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x14+((ch%4)*0x20), 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x15+((ch%4)*0x20), 0x20);
}

void nvp6134_setchn_ahd_QHD(const unsigned char ch, const unsigned char vfmt)
{
	unsigned char YCmerge;
	unsigned char PN_set;
	
	printk(">>>>> nvp6134_setchn_ahd_QHD (%s) <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	nvp6134_setchn_common_3M(ch, vfmt);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23+(ch%4)*4,0x43);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,0x2A);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x0F:0x0E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4, (ch%2==0)?0xe5:0x05);	// Clk delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,0x44);
	YCmerge = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xed);
	SET_BIT(YCmerge, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xED, YCmerge);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x1b,0x24);			// COMB_TH
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,0x3F);
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x26,0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,0xEE);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,0xC6);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x01);	// ( no video option - sync enable)
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,0x30);	// ( sync width )

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5F,0x50);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x69,0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6E,0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6F,0x32);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xB7,0xFF);  //09.08
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xBB,0x0A);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xD1,0x40);	

	gpio_i2c_write(nvp6134_iic_addr[ch/4],0xff,0x09);
	//original pn value
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x50+((ch%4)*0x4),vfmt==PAL?0x86:0x86);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x51+((ch%4)*0x4),vfmt==PAL?0xB5:0xBE);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x52+((ch%4)*0x4),vfmt==PAL?0x6F:0x6A);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x53+((ch%4)*0x4),vfmt==PAL?0x48:0x48);
	PN_set = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x44);
	SET_BIT(PN_set, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44, PN_set);


	// Motion setting   
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0x50);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A+((ch%4)*0x06),0x3C);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x4A);
}

void nvp6134_setchn_ahd_QHD_NRT(const unsigned char ch, const unsigned char vfmt)
{
	unsigned char YCmerge;
	unsigned char PN_set;
	//unsigned char PN_set;

	printk(">>>>> nvp6134_setchn_ahd_QHD_15P (%s) <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	//nvp6134_setchn_common_3M(ch, vfmt);
	nvp6134_setchn_ahd_QHD(ch, vfmt);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2a, 0xd2);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd1, 0x2a);   //burst valid point
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xd4, 0x1f);
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x09);
	//original PN value
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x50+((ch%4)*0x4),0x2C); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x51+((ch%4)*0x4),0xE7);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x52+((ch%4)*0x4),0xCF);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x53+((ch%4)*0x4),0x52);
	PN_set = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x44);
	SET_BIT(PN_set, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44, PN_set);
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x04);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,0x80);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8E)+ch%4,0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,0x00);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,0x40); 
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,0x00); 
	
	YCmerge =	gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xED);
	CLE_BIT(YCmerge, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xED, YCmerge); 

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24, 0x3F);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50, 0xC6);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x57, 0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5F, 0x50);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x69, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xD1, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xB7, 0xF0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xBB, 0x06);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x11);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x01+(ch%4)*0x20 , 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x02+(ch%4)*0x20 , 0xEA);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x03+(ch%4)*0x20 , 0x0A);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x04+(ch%4)*0x20 , 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05+(ch%4)*0x20 , 0x0C);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x06+(ch%4)*0x20 , 0xE4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x07+(ch%4)*0x20 , 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+(ch%4)*0x20 , 0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0A+(ch%4)*0x20 , 0x05);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0B+(ch%4)*0x20 , 0xA0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0C+(ch%4)*0x20 , 0x05);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0D+(ch%4)*0x20 , 0xDB);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x0E)+(ch%4)*0x20 , 0x05);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0F+(ch%4)*0x20 , 0xD1);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x10+(ch%4)*0x20 , 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x11+(ch%4)*0x20 , 0xA0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x12+(ch%4)*0x20 , 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x13+(ch%4)*0x20 , 0x2C);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x14+(ch%4)*0x20 , 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x15+(ch%4)*0x20 , 0x30);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+(ch%4)*0x20 , 0x0F);

	// Motion setting
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0xA0);
	printk(">>>>> nvp6134_setchn_ahd_QHD_NRT_using BANK 11 (%s) <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
}

void nvp6134_setchn_ahd_QHD_X(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_ahd_QHD_X (%s) <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	nvp6134_setchn_ahd_QHD(ch, vfmt);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x0F:0x0E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,0x03);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,0x44);
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,0xC6);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x69,0x10);
}

void nvp6134_setchn_ahd_5MNRT(const unsigned char ch, const unsigned char vfmt)
{
	unsigned char YCmerge, PN_set;

	printk(">>>>> nvp6134_setchn_ahd_5M_12_5p(%s) <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	nvp6134_setchn_common_3M(ch, vfmt);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23+(ch%4)*4,0x41);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,0x14);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x05);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,0x08);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,0x40);
	YCmerge = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xed);
	CLE_BIT(YCmerge, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xed, YCmerge);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,0x0A);	
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x26,0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,0x84);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x69,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6E,0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6F,0x1C);

	gpio_i2c_write(nvp6134_iic_addr[ch/4],0xff,0x09);
	//original PN value
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x50+((ch%4)*0x4),0x5F);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x51+((ch%4)*0x4),0xE5);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x52+((ch%4)*0x4),0xD0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x53+((ch%4)*0x4),0x52);
	PN_set = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x44);
	SET_BIT(PN_set, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44, PN_set);


	// Motion setting     
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0x51);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A+((ch%4)*0x06),0x51);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x4B);
}

void nvp6134_setchn_ahd_5M_20p(const unsigned char ch, const unsigned char vfmt)
{
	unsigned char YCmerge, PN_set;
	printk(">>>>> nvp6134_setchn_ahd_5M_20p(%s) <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	nvp6134_setchn_common_3M(ch, vfmt);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23+(ch%4)*4,0x43);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x30+ch%4,0x14);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x05);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,0x07);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+ch%4,0x06);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4, (ch%2==0)?0xe5:0x05);	// Clk delay
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,0x44);

	YCmerge = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0xed);
	SET_BIT(YCmerge, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xED, YCmerge);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,0x08);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A,0xD2);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,0xee);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,0xC6);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x69,0x10);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6E,0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6F,0x1C);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xB7,0xF0);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xB8,0x38);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xBB,0x0A);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84,0x01);	// ( no video option - sync enable)
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x86,0x30);	// ( sync width )

	gpio_i2c_write(nvp6134_iic_addr[ch/4],0xff,0x09);
	//original pn value
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x50+((ch%4)*0x4),0x9B);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x51+((ch%4)*0x4),0x0E);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x52+((ch%4)*0x4),0x77);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x53+((ch%4)*0x4),0x48);  
	PN_set = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x44);
	SET_BIT(PN_set, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44, PN_set);

	// Motion setting   
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0x51);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A+((ch%4)*0x06),0x51);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x4B);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x11);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+((ch%4)*0x20), 0x0D);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x01+((ch%4)*0x20), 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x02+((ch%4)*0x20), 0x86);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x03+((ch%4)*0x20), 0x0A);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x04+((ch%4)*0x20), 0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05+((ch%4)*0x20), 0x0E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x06+((ch%4)*0x20), 0xA6);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x07+((ch%4)*0x20), 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+((ch%4)*0x20), 0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x10+((ch%4)*0x20), 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x11+((ch%4)*0x20), 0x60);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x12+((ch%4)*0x20), 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x13+((ch%4)*0x20), 0xCC);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x14+((ch%4)*0x20), 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x15+((ch%4)*0x20), 0x3C);
}

void nvp6134_setchn_ext_5MNRT(const unsigned char ch, const unsigned char vfmt)
{
	unsigned char PN_set;

 	printk(">>>>> nvp6134_setchn_ext_5M_12.5P(%s) <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	nvp6134_setchn_ahd_5MNRT(ch, vfmt);
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58, 0x70);
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,0x04);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,0xF5);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x05);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x89+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,0x00);
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4, (ch%2==0)?0x45:0x95);
	//gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,0x44);
	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24, 0x20);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x57, 0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6E, 0x10);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x6F, 0x28);	
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x90, 0x05);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xD0, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xD1, 0x20);

	gpio_i2c_write(nvp6134_iic_addr[ch/4],0xff,0x09);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x50+((ch%4)*0x4),0x8B);  
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x51+((ch%4)*0x4),0x2E);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x52+((ch%4)*0x4),0xBA);
	gpio_i2c_write(nvp6134_iic_addr[ch/4],0x53+((ch%4)*0x4),0x48);
	PN_set = gpio_i2c_read(nvp6134_iic_addr[ch/4], 0x44);
	SET_BIT(PN_set, (ch%4));
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x44, PN_set);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x11);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+((ch%4)*0x20), 0x0F);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x01+((ch%4)*0x20), 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x02+((ch%4)*0x20), 0xCE);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x03+((ch%4)*0x20), 0x0A);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x04+((ch%4)*0x20), 0x34);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x05+((ch%4)*0x20), 0x17);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x06+((ch%4)*0x20), 0x70);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x07+((ch%4)*0x20), 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x08+((ch%4)*0x20), 0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0A+((ch%4)*0x20), 0x07);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0B+((ch%4)*0x20), 0x98);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0C+((ch%4)*0x20), 0x07);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x0D+((ch%4)*0x20), 0xD0); 
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x10+((ch%4)*0x20), 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x11+((ch%4)*0x20), 0xC0);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x12+((ch%4)*0x20), 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x13+((ch%4)*0x20), 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x14+((ch%4)*0x20), 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x15+((ch%4)*0x20), 0x00);
}

void nvp6134_setchn_ahd_8M_NRT(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_ahd_8M_NRT(%s) <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	
	nvp6134_setchn_common_3M(ch, vfmt);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,0x03);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x06);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,0x03);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,0x40);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xED,0x00);	

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,0x0A);	//romeo(+)	08.01
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,0x84);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x69,0x00);
	
	// Motion setting   /* (+) by romeo_0425 */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0x78);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A+((ch%4)*0x06),0x5A);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x72);
}

void nvp6134_setchn_ahd_UHDX(const unsigned char ch, const unsigned char vfmt)
{
	printk(">>>>> nvp6134_setchn_ahd_UHDX(%s) <<<<<\n", (vfmt==PAL) ? "PAL" : "NTSC" );
	nvp6134_setchn_common_3M(ch, vfmt);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58+ch%4,0x90);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x64+ch%4,0x80);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+ch%4,vfmt==PAL?0x0D:0x0C);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x85+ch%4,0x00);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], (0x8e)+ch%4,0x00);

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x01);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x84+ch%4,0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x8c+ch%4,0x44);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xED,0x0F);		

	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x24,0x0A);	//romeo(+)	08.01
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x47,0x04);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x50,0x84);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x69,0x10);
	
	// Motion setting   /* (+) by romeo_0425 */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x02);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x29+((ch%4)*0x06),0x3C);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2A+((ch%4)*0x06),0x5A);
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x2C+((ch%4)*0x06),0x36);
			
}
/********************************************************************************
* End of file
********************************************************************************/
