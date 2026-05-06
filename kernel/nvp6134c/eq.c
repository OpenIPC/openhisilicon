/********************************************************************************
*
*  Copyright (C) 2017 	NEXTCHIP Inc. All rights reserved.
*  Module		: Equalizer module
*  Description	: set EQ according to the distance.
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
#include "common.h"

#include "eq.h"
#include "acp.h"
#include "eq_recovery.h"
#include "eq_common.h"

#define _EQ_ENHENCED_
/*******************************************************************************
 * extern variable
 *******************************************************************************/
extern unsigned int 	nvp6134_iic_addr[4];
extern unsigned char 	ch_mode_status[16];
extern unsigned char	ch_vfmt_status[16];
extern unsigned int		nvp6134_cnt;
extern unsigned int     g_slp_ahd[16];
extern unsigned char 	g_ch_video_fmt[16];
extern unsigned int     g_eq_set_done;
extern unsigned char 	det_mode[16];
/*******************************************************************************
 * internal variable
 *******************************************************************************/
static volatile unsigned int min_acc[MAX_CH_NUM]={0xFFFF,};
static volatile unsigned int max_acc[MAX_CH_NUM]={0x0000,};
static volatile unsigned int min_ypstage[MAX_CH_NUM]={0xFFFF,};
static volatile unsigned int max_ypstage[MAX_CH_NUM]={0x0000,};
static volatile unsigned int min_ymstage[MAX_CH_NUM]={0xFFFF,};
static volatile unsigned int max_ymstage[MAX_CH_NUM]={0x0000,};

static volatile unsigned int min_fr_ac_min[MAX_CH_NUM]={0xFFFF,};
static volatile unsigned int max_fr_ac_min[MAX_CH_NUM]={0x0000,};
static volatile unsigned int min_fr_ac_max[MAX_CH_NUM]={0xFFFF,};
static volatile unsigned int max_fr_ac_max[MAX_CH_NUM]={0x0000,};
static volatile unsigned int min_dc[MAX_CH_NUM]={0xFFFF,};
static volatile unsigned int max_dc[MAX_CH_NUM]={0x0000,};

static unsigned int eq_chk_cnt[MAX_CH_NUM]={0,};

static int bInitEQ = EQ_INIT_OFF;	/* EQ initialize(structure) */
nvp6134_equalizer s_eq;				/* EQ manager structure */
nvp6134_equalizer s_eq_type;		/* EQ manager structure */


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
*	Description		: EQ configuration value 
*	Argurments		:
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void eq_init_each_format(unsigned char ch, int mode, const unsigned char vfmt )
{
    /* turn off Analog by-pass */
	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
    gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x59, 0x11 );
	
	s_eq.ch_stage[ch] = 0; //Set default stage to 0.
	
	switch( mode )
	{
		case NVP6134_VI_720H: 	  
		case NVP6134_VI_960H:  	  
		case NVP6134_VI_1280H: 	  
		case NVP6134_VI_1440H: 	  
		case NVP6134_VI_1920H:    
		case NVP6134_VI_960H2EX:  
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x01,0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x58,0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x59,0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5B,0x03);

			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x00+ch%4,0x00);
			printk(">>>>> DRV : CH:%d, EQ init, NVP6134_VI_SD, NVP6134_VI_SD - conf\n", ch);
		break;
		case NVP6134_VI_720P_2530:   //HD AHD  @ 30P
		case NVP6134_VI_720P_5060:   //HD AHD  @ 25P
		case NVP6134_VI_HDEX:		 //HD AHD EX
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xC0,0x16);					// TX_PAT_STR
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xC1,0x13);					// ACC_GAIN_STS_SEL & TX_PAT_WIDTH
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xC8,0x04);					// SLOPE_VALUE_2S

			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,ch%4<2?0x0A:0x0B);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], ch%2==0?0x74:0xF4,0x02);		// chX_EQ_SRC_SEL
			printk(">>>>> DRV : CH:%d, EQ init, NVP6134_VI_720P_2530, NVP6134_VI_720P_5060 - conf\n", ch);
		break;
		case NVP6134_VI_3M_NRT:
		case NVP6134_VI_3M:
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xC0,0x17);	// change TX_PATTERN START
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xC1,0x13);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xC8,0x04);

			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,ch%4<2?0x0A:0x0B);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], ch%2==0?0x74:0xF4,0x02);
			printk(">>>>> DRV : CH:%d, EQ init, NVP6134_VI_3M_NRT, NVP6134_VI_3M - conf\n", ch);
			break;
		case NVP6134_VI_1080P_2530:   //FHD AHD
		case NVP6134_VI_5M_NRT:
		case NVP6134_VI_4M:
		case NVP6134_VI_4M_NRT:	
		case NVP6134_VI_5M_20P:
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xC0,0x17);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xC1,0x13);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xC8,0x04);

			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,ch%4<2?0x0A:0x0B);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], ch%2==0?0x74:0xF4,0x02);
			printk(">>>>> DRV : CH:%d, EQ init, NVP6134_VI_1080P_2530 - conf\n", ch);
		break;

		case NVP6134_VI_EXC_720P:   //CHD  @ 30P
		case NVP6134_VI_EXC_720PRT: //CHD   @ 60P
		case NVP6134_VI_EXC_HDEX:	//CHD EX
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xC0,0x16);					// TX_PAT_STR
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xC1,0x13);					// original tx pattern = 4, just read 3 lines for CVI
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xC8,0x04);

			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,ch%4<2?0x0A:0x0B);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], ch%2==0?0x74:0xF4,0x02);
			printk(">>>>> DRV : CH:%d, EQ init, NVP6134_VI_EXC_720P,NVP6134_VI_EXC_720PRT,  conf\n", ch);
		break;

		case NVP6134_VI_EXC_1080P:  //CFHD
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x05+ch%4);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xC0,0x17);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xC1,0x13);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xC8,0x04);

			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,ch%4<2?0x0A:0x0B);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], ch%2==0?0x74:0xF4,0x02);
			printk(">>>>> DRV : CH:%d, EQ init,  NVP6134_VI_EXC_1080P - conf\n", ch);
		break;

		case NVP6134_VI_EXT_720PA:  //THDA  @ 30A
		case NVP6134_VI_EXT_HDAEX:	//THDA EX
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x02);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+((ch%4)*0x10),0xF1);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA1+((ch%4)*0x10),0x29);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA2+((ch%4)*0x10),0x32);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA3+((ch%4)*0x10),0x40);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+((ch%4)*0x10),0xF2);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA5+((ch%4)*0x10),0x02);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA6+((ch%4)*0x10),0x01);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA7+((ch%4)*0x10),0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA8+((ch%4)*0x10),0x04);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA9+((ch%4)*0x10),0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xAA+((ch%4)*0x10),0x08);
			printk(">>>>> DRV : CH:%d, EQ init, NVP6134_VI_EXT_720PA - conf\n", ch);
		break;

		case NVP6134_VI_EXT_720PB:  //THDA  @ 24A
		case NVP6134_VI_EXT_720PRT: //THDA  @ 60
		case NVP6134_VI_EXT_HDBEX:	//THDA EX
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x02);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+((ch%4)*0x10),0xF1);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA1+((ch%4)*0x10),0x29);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA2+((ch%4)*0x10),0x32);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA3+((ch%4)*0x10),0x40);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+((ch%4)*0x10),0xED);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA5+((ch%4)*0x10),0x02);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA6+((ch%4)*0x10),0x01);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA7+((ch%4)*0x10),0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA8+((ch%4)*0x10),0x04);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA9+((ch%4)*0x10),0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xAA+((ch%4)*0x10),0x08);
			printk(">>>>> DRV : CH:%d, EQ init, NVP6134_VI_EXT_720PB, NVP6134_VI_EXT_720PRT - conf\n", ch);
		break;

		case NVP6134_VI_EXT_1080P:   //FHD EXT @ 30P
		case NVP6134_VI_EXT_3M_NRT:
		case NVP6134_VI_EXT_5M_NRT:
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF,0x02);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA0+((ch%4)*0x10),0xF1);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA1+((ch%4)*0x10),0x29);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA2+((ch%4)*0x10),0x32);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA3+((ch%4)*0x10),0x40);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA4+((ch%4)*0x10),(vfmt==PAL)?0x64:0x69);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA5+((ch%4)*0x10),0x04);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA6+((ch%4)*0x10),0x01);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA7+((ch%4)*0x10),0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA8+((ch%4)*0x10),0x04);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xA9+((ch%4)*0x10),0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xAA+((ch%4)*0x10),0x08);
			printk(">>>>> DRV : CH:%d, EQ init, NVP6134_VI_EXT_1080P - conf, %s\n",  ch, (vfmt==PAL) ? "PAL" : "NTSC" );
		break;
	}
}

/*******************************************************************************
*	Description		: Adjust Equalizer(EQ)
*	Argurments		: ch: channel information
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
int nvp6134_set_equalizer(unsigned char ch)
{
	unsigned int  acc_gain_status[16], y_plus_slope[16], y_minus_slope[16], sync_width_value[16];
	unsigned int  fr_ac_min_value[16], fr_ac_max_value[16], fr_dc_value[16];
	unsigned char vloss;
	unsigned char vidformat;
	int ret;

	/*
	 * Initialization structure
	 */
	if( bInitEQ == EQ_INIT_OFF )
	{
		memset(&s_eq, 0x00, sizeof(nvp6134_equalizer));
		memset(&s_eq_type, 0x00, sizeof(nvp6134_equalizer));
		bInitEQ = EQ_INIT_ON;
	}

	/*
	 * exception
	 * skip under NVP6134_VI_720P_2530 mode and over NVP6134_VI_BUTT
	 */
	if(ch_mode_status[ch] >= NVP6134_VI_BUTT )
	{
		s_eq.ch_previdmode[ch] = 0xFF;
		s_eq.ch_curvidmode[ch] = 0xFF;
		s_eq.ch_previdon[ch] = EQ_VIDEO_OFF;
		s_eq.ch_curvidon[ch] = EQ_VIDEO_OFF;
		s_eq.ch_previdformat[ch] = 0xFF;		 // pre video format for Auto detection value
		s_eq.ch_curvidformat[ch] = 0xFF;		 // current video format for Auto detection value
		s_eq.ch_vfmt_status[ch] = PAL;			 // default(PAL) : NTSC(0), PAL(1)ch_vfmt_status
		return 0;
	}

	/*
	 * check and set video loss and video on/off information to buffer
	 */
	if(1)
	{
		/* get video format(video loss), 0:videoloss, 1:video on */
		vidformat = g_ch_video_fmt[ch];
		if( vidformat == 0x00 )
			vloss = 0;
		else
			vloss = 1;

		/* after checking agc locking(signal) and video loss, save Video ON information to buffer */
		if( (vloss == 1) && (nvp6134_GetAgcLockStatus(ch) == 0x01))
		{
			s_eq.ch_curvidon[ch] = EQ_VIDEO_ON;
			s_eq.ch_curvidmode[ch] = ch_mode_status[ch];
			s_eq.ch_curvidformat[ch] = vidformat;
			s_eq.ch_vfmt_status[ch] = ch_vfmt_status[ch];	// NTSC/PAL
		}
		else
		{
			/* These are default value of NO video */
			s_eq.ch_curvidmode[ch] = 0xFF;
			s_eq.ch_curvidon[ch] = EQ_VIDEO_OFF;
			s_eq.ch_curvidformat[ch] = 0xFF;
			s_eq.ch_vfmt_status[ch] = PAL;
			s_eq.ch_stage[ch] = 0;
			s_eq.eq_file_save[ch] = 0;

			g_slp_ahd[ch] = 0;
			eq_chk_cnt[ch] = 0;
			/* set default value for distinguishing between EXC and AHD */
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C, 0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xB8, 0xB9);  //recover this for video loss detection

			/* hide display */
			nvp6134_hide_ch(ch);
		}
	}
	
	/* for setting eq manually */
	if((1==s_eq_type.ch_equalizer_type[ch]) || ((0==s_eq_type.ch_stage[ch]) && (2==s_eq_type.ch_equalizer_type[ch])))
	{
		s_eq.ch_stage[ch] = 0;
	}

	/*
	 * it only want to set EQ(algorithm) once when there is the video signal.
	 *    - PRE(video off) != CUR(video on)
	 *    - PRE(video format) != CUR(Video ON)
	 */
	if( (s_eq.ch_curvidon[ch] != EQ_VIDEO_OFF) && (s_eq.ch_curvidmode[ch] != NVP6134_VI_1080P_NOVIDEO))
	{
		if( 	(s_eq.ch_previdon[ch] != s_eq.ch_curvidon[ch]) 
			|| 	(s_eq.ch_previdmode[ch] != s_eq.ch_curvidmode[ch])
			|| 	(s_eq.ch_stage[ch] == 0))
		{
			printk(">>>>> DRV : CH[%d] EQ algorithm! - SET EQ \n", ch );
			/* set H PLL - No video option  and PN init */
        		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
			if( (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_1080P && s_eq.ch_vfmt_status[ch] == PAL) ||
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_1080P ) ||
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_3M_NRT || s_eq.ch_curvidmode[ch] == NVP6134_VI_3M ||
				 s_eq.ch_curvidmode[ch] == NVP6134_VI_4M_NRT || s_eq.ch_curvidmode[ch] == NVP6134_VI_4M ||
				 s_eq.ch_curvidmode[ch] == NVP6134_VI_5M_NRT || s_eq.ch_curvidmode[ch] == NVP6134_VI_5M_20P) )
				gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xB8, 0x39);
			else
				gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xB8, 0x38);

			SetFSClockmodeForColorlocking( ch );
			msleep(35);

		        /* distinguish between A(THD, CHD) type and B(AHD0 type */
		        if(	(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PA) || \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_HDAEX) || \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PB) || \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_HDBEX) || \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PRT)||	\
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_1080P) ||	\
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_3M_NRT)||	\
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_5M_NRT)|| \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_1080P) || \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_720P)  || \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_720PRT)|| \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_HDEX))
		        {
		            /* Initialize Temporily register */
				if(	(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PA) || \
					(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_HDAEX) || \
					(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PB) || \
					(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_HDBEX) || \
					(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PRT)||	\
					(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_1080P) ||	\
		                    (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_3M_NRT)||	\
		                    (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_5M_NRT) )
				{
					if ((s_eq.ch_curvidmode[ch] != NVP6134_VI_EXT_3M_NRT) || (s_eq.ch_curvidmode[ch] != NVP6134_VI_EXT_5M_NRT))
					{
						gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
						gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C, 0x00 );
					}
					msleep(300);
					/* THD or CHD */
					fr_ac_min_value[ch] = GetACMinValue(ch);
					fr_ac_max_value[ch] = GetACMaxValue(ch);
					fr_dc_value[ch]		= GetDCValue(ch);

		                    acc_gain_status[ch] = GetAccGain(ch);
							
		                    if(acc_gain_status[ch] < min_acc[ch])			min_acc[ch] = acc_gain_status[ch];		// TEST
		                    else if(acc_gain_status[ch] > max_acc[ch])		max_acc[ch] = acc_gain_status[ch];		// TEST

		                    if(fr_ac_min_value[ch] < min_fr_ac_min[ch])			min_fr_ac_min[ch] =fr_ac_min_value[ch];
		                    else if(fr_ac_min_value[ch] > max_fr_ac_min[ch])	max_fr_ac_min[ch] =fr_ac_min_value[ch];

		                    if(fr_ac_max_value[ch] < min_fr_ac_max[ch])			min_fr_ac_max[ch] = fr_ac_max_value[ch];
		                    else if(fr_ac_max_value[ch] > max_fr_ac_max[ch])	max_fr_ac_max[ch] = fr_ac_max_value[ch];

		                    if(fr_dc_value[ch] < min_dc[ch])					min_dc[ch] = fr_dc_value[ch];
		                    else if(fr_dc_value[ch] > max_dc[ch])				max_dc[ch] = fr_dc_value[ch];

		                    printk("---------------DRV: CH:%d, TFHD_THD -------------UP\n", ch);
		                    printk("fr_ac_min_value cur=%3d min=%3d max=%3d\n",fr_ac_min_value[ch], min_fr_ac_min[ch], max_fr_ac_min[ch]);
		                    printk("fr_ac_max_value cur=%3d min=%3d max=%3d\n",fr_ac_max_value[ch], min_fr_ac_max[ch], max_fr_ac_max[ch]);
		                    printk("fr_dc_value     cur=%3d min=%3d max=%3d\n",fr_dc_value[ch],min_dc[ch], max_dc[ch]);
		                    printk("gain_status cur=%3d min=%3d max=%3d\n",acc_gain_status[ch], min_acc[ch], max_acc[ch]);

		                    /* To recovery FSC Locking Miss Situation - Atype is THD */
		                    acc_gain_status[ch] = __eq_recovery_Atype( ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch], acc_gain_status[ch], fr_dc_value[ch], fr_ac_min_value[ch], fr_ac_max_value[ch], fr_dc_value[ch] );

		                    /* save distance information to buffer for display */
		                    s_eq.fr_ac_min_value[ch] = fr_ac_min_value[ch];
		                    s_eq.fr_ac_max_value[ch] = fr_ac_max_value[ch];
		                    s_eq.fr_dc_value[ch] = fr_dc_value[ch];
		                    s_eq.acc_gain[ch] = acc_gain_status[ch];
		            }
		            else
		            {
					gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x09);
		                	gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x80+((ch%4)*0x20), 0x00);
					gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x81+((ch%4)*0x20), 0x00);
					msleep(300);
		                	printk(">>>>> DRV[%s:%d] CH:%d, init EQ setting in Distinguish, EQ changed.\n", __func__, __LINE__, ch );

		               	 /* CVI */
		                	acc_gain_status[ch] = GetAccGain(ch);
		                	y_plus_slope[ch]	= GetYPlusSlope(ch);
		                	y_minus_slope[ch]	= GetYMinusSlope(ch);
					sync_width_value[ch] = StageGetSyncWidth(ch);
						
					if(acc_gain_status[ch] < min_acc[ch])			min_acc[ch] = acc_gain_status[ch];
					else if(acc_gain_status[ch] > max_acc[ch])		max_acc[ch] = acc_gain_status[ch];

					if(y_plus_slope[ch] < min_ypstage[ch])			min_ypstage[ch] = y_plus_slope[ch];
					else if(y_plus_slope[ch] > max_ypstage[ch])		max_ypstage[ch] = y_plus_slope[ch];

					if(y_minus_slope[ch] < min_ymstage[ch])			min_ymstage[ch] = y_minus_slope[ch];
					else if(y_minus_slope[ch] > max_ymstage[ch])	max_ymstage[ch] = y_minus_slope[ch];

					printk("---------------DRV: CH:%d, CFHD_CHD --------------------\n", ch);
					printk("gain_status 	cur=%3d min=%3d max=%3d\n",acc_gain_status[ch], min_acc[ch], max_acc[ch]);
					printk("y_plus_slope    cur=%3d min=%3d max=%3d\n",y_plus_slope[ch],min_ypstage[ch], max_ypstage[ch]);
					printk("y_minus_slope   cur=%3d min=%3d max=%3d\n",y_minus_slope[ch],min_ymstage[ch], max_ymstage[ch]);
					printk("sync_width	    cur=%3d min=%3d max=%3d\n",sync_width_value[ch],0, 0);

					/* To recovery FSC Locking Miss Situation - Btype is AHD, CHD */
					__eq_recovery_Btype( ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch], acc_gain_status[ch], y_minus_slope[ch] , y_plus_slope[ch]);

					/* save distance information to buffer for display */
					s_eq.acc_gain[ch] = acc_gain_status[ch];
					s_eq.y_plus_slope[ch] = y_plus_slope[ch];
					s_eq.y_minus_slope[ch] = y_minus_slope[ch];
		            }


		            	/* get stage according to EQ pattern Color Gain */
				if( (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_1080P) || (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_720PRT ) || \
				     (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_720P ) || (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_HDEX   )  )
				{
		            		s_eq.ch_stage[ch] = eq_get_stage( ch, s_eq.ch_curvidmode[ch], sync_width_value[ch], 0/*Not used*/, 0/*Not used*/, s_eq.ch_vfmt_status[ch] );
				}
				else
				{
		        		s_eq.ch_stage[ch] = eq_get_stage( ch, s_eq.ch_curvidmode[ch], acc_gain_status[ch], 0/*Not used*/, 0/*Not used*/, s_eq.ch_vfmt_status[ch] );
				}
				#ifdef _EQ_ENHENCED_
				if ( s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_1080P )
					s_eq.ch_stage[ch] = eq_get_thd_stage_tfhd_bwmode( ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch] );
				else if ( s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_3M_NRT)
					s_eq.ch_stage[ch] = eq_get_thd_stage_tvi_3m_bwmode( ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch] );
				else if ( s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_5M_NRT)
					s_eq.ch_stage[ch] = eq_get_thd_stage_tvi_5m_bwmode( ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch] );				
				#endif
					
				if( (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_1080P || s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_HDAEX ||
		            	 	s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PA || s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_HDBEX ||
	                 		s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PB || s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PRT ||
				 	s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_3M_NRT ||	s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_5M_NRT )
		                 	&& s_eq.ch_stage[ch] == EQ_STAGE_6 )
		            {
		                	if ( s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_1080P )
		                		s_eq.ch_stage[ch] = eq_get_thd_stage(ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch]);
		                	else if ( s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_3M_NRT )	// by peter.
		                    	s_eq.ch_stage[ch] = eq_get_thd_3m_stage(ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch]);
		                	else if ( s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_5M_NRT ) //by peter
		                    	s_eq.ch_stage[ch] = eq_get_thd_5m_stage(ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch]);
		                	else if ( s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_HDAEX || s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PA )
		                    	s_eq.ch_stage[ch] = eq_get_thd_stage_720P_A(ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch]);
		                	else if( s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_HDBEX || s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PB )
					{
					    	s_eq.ch_stage[ch] = eq_get_thd_stage_720P_B(ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch]);
					   	printk(">>>>> DRV[%s:%d] CH:%d, NVP6134_VI_EXT_HDBEX, NVP6134_VI_EXT_720PB\n", __func__, __LINE__, ch );
					}
					if( s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PRT )
					{
						if(s_eq.ch_vfmt_status[ch] == PAL )
							s_eq.ch_stage[ch] = eq_get_thd_stage_720P50_bwmode(ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch]);
						else
							s_eq.ch_stage[ch] = eq_get_thd_stage_720P60_bwmode(ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch]);
					}

					s_eq.fr_ac_min_value[ch] = GetACMinValue(ch);
					s_eq.fr_ac_max_value[ch] = GetACMaxValue(ch);
					s_eq.fr_dc_value[ch]	 = GetDCValue(ch);
					s_eq.acc_gain[ch]		 = GetAccGain(ch);
		            }

				if( (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_720PRT ) && s_eq.ch_stage[ch] ==  EQ_STAGE_6 )
				{
					if(s_eq.ch_vfmt_status[ch] == PAL )
					    	s_eq.ch_stage[ch] = eq_get_cvi_stage_720P50(ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch]);
					else                        
					    	s_eq.ch_stage[ch] = eq_get_cvi_stage_720P60(ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch]);

					s_eq.acc_gain[ch]		 = GetAccGain(ch);
				}

		            	/* manual set eq stage */
				if(1 == s_eq_type.ch_equalizer_type[ch])
					s_eq.ch_stage[ch] = s_eq_type.ch_stage[ch];
					
					/* adjust EQ depend on distance */
		            	eq_adjust_eqvalue( ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch], s_eq.ch_stage[ch] );
				Set_ColorLockingFilter(ch);

				s_eq.eq_file_save[ch] = 1;
		        }
		       else
			{
				/* AHD */
				acc_gain_status[ch] = GetAccGain(ch);
				y_plus_slope[ch]	= GetYPlusSlope(ch);
				y_minus_slope[ch]	= GetYMinusSlope(ch);

				if(acc_gain_status[ch] < min_acc[ch])			min_acc[ch] = acc_gain_status[ch];
				else if(acc_gain_status[ch] > max_acc[ch])		max_acc[ch] = acc_gain_status[ch];

				if(y_plus_slope[ch] < min_ypstage[ch])			min_ypstage[ch] = y_plus_slope[ch];
				else if(y_plus_slope[ch] > max_ypstage[ch])		max_ypstage[ch] = y_plus_slope[ch];

				if(y_minus_slope[ch] < min_ymstage[ch])			min_ymstage[ch] = y_minus_slope[ch];
				else if(y_minus_slope[ch] > max_ymstage[ch])	max_ymstage[ch] = y_minus_slope[ch];

				printk("---------------DRV: CH:%d, AFHD_AHD --------------------\n", ch);
				printk("gain_status 	cur=%3d min=%3d max=%3d\n",acc_gain_status[ch], min_acc[ch], max_acc[ch]);
				printk("y_plus_slope    cur=%3d min=%3d max=%3d\n",y_plus_slope[ch],min_ypstage[ch], max_ypstage[ch]);
				printk("y_minus_slope   cur=%3d min=%3d max=%3d\n",y_minus_slope[ch],min_ymstage[ch], max_ymstage[ch]);

				/* To recovery FSC Locking Miss Situation - Btype is AHD, CHD */
				__eq_recovery_Btype( ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch], acc_gain_status[ch], y_minus_slope[ch] , y_plus_slope[ch]);

				/* save distance information to buffer for display */
				s_eq.acc_gain[ch] = acc_gain_status[ch];
				s_eq.y_plus_slope[ch] = y_plus_slope[ch];
				s_eq.y_minus_slope[ch] = y_minus_slope[ch];

				if( s_eq.ch_curvidmode[ch] == NVP6134_VI_720P_2530 || s_eq.ch_curvidmode[ch] == NVP6134_VI_HDEX )	
				{
					if((y_plus_slope[ch]<60)&&(y_minus_slope[ch]<60)&&(g_slp_ahd[ch] == 0))
					g_slp_ahd[ch] = 1;
				}
				if ( g_slp_ahd[ch] == 1 )
				{
				    	s_eq.ch_stage[ch] = eq_get_stage( ch, s_eq.ch_curvidmode[ch], acc_gain_status[ch], 0/*Not used*/, 0/*Not used*/, s_eq.ch_vfmt_status[ch] );
				    	printk(">>>>> DRV[%s:%d] CH:%d, no slope. and use acc gain!\n", __func__, __LINE__, ch );
				}
				else
				{
				    	s_eq.ch_stage[ch] = eq_get_stage( ch, s_eq.ch_curvidmode[ch], y_plus_slope[ch], y_minus_slope[ch], 0/*Not used*/, s_eq.ch_vfmt_status[ch] );
				    	printk(">>>>> DRV[%s:%d] CH:%d, slope. and use sloep!\n", __func__, __LINE__, ch );
				}

				/* exception processing */
				if( s_eq.ch_stage[ch] == 0 )
				{
					nvp6134_VD_chnRst(ch);
					printk(">>>>> DRV[%s:%d] CH:%d, AHD, Skip EQ, because the Video signal is Unstable\n", __func__, __LINE__, ch );
					return 1;
				}

				/* manual set eq stage */
				if(1 == s_eq_type.ch_equalizer_type[ch])
					s_eq.ch_stage[ch] = s_eq_type.ch_stage[ch];

				/* adjust EQ depend on distance */
				eq_adjust_eqvalue( ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch], s_eq.ch_stage[ch] );
				Set_ColorLockingFilter(ch);

				y_plus_slope[ch]	= GetYPlusSlope(ch);
				y_minus_slope[ch]	= GetYMinusSlope(ch);
				g_slp_ahd[ch] = 0;

				s_eq.eq_file_save[ch] = 1;
			}
			if(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_1080P||s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_3M_NRT)
			{
				eq_chk_cnt[ch] ++;
				if(eq_chk_cnt[ch] < 2)
				{
					//return 1;
				}
			}
			printk(">>>>> DRV[%s:%d] CH:%d, s_eq.ch_stage:%d\n", __func__, __LINE__, ch, s_eq.ch_stage[ch]);
			/* after getting EQ value, Analog filter bypass option is off */
			if(	(s_eq.ch_curvidmode[ch] == NVP6134_VI_3M)	||
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_4M)	||
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_5M_20P) )
			{
			}
			else
			{
				gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
				gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x59, 0x00);
			}
			printk(">>>>> DRV : getting EQ value, Analog filter bypass option is off\n" );

			/* send eqstage to isp */
			ret = acp_isp_write_communcation( ch, s_eq.ch_stage[ch], s_eq.ch_curvidmode[ch] );
			//eq_set_communication_value( ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch], ret );

			/* show display */
			nvp6134_show_ch(ch);

			g_eq_set_done = g_eq_set_done | (0x1 << ch);
			printk(">>>>> DRV[%s:%d] CH:%d, EQ SET DONE[%x]. \n", __func__, __LINE__, ch, g_eq_set_done );
		}
		else
		{
			 /* distinguish between A type and B type */
			if(	(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PA) || \
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_HDAEX) || \
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PB) || \
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_HDBEX) || \
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PRT)||	\
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_1080P) || \
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_3M_NRT)||	\
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_5M_NRT))
			{
				/* THD or CVI */
				s_eq.cur_fr_ac_min[ch] 	= GetACMinValue(ch);
				s_eq.cur_fr_ac_max[ch] 	= GetACMaxValue(ch);
				s_eq.cur_fr_dc[ch] 		= GetDCValue(ch);
				s_eq.cur_acc_gain[ch] 	= GetAccGain(ch);
			}
			else
			{
				/* AHD */
				s_eq.cur_acc_gain[ch] = GetAccGain(ch);
				s_eq.cur_y_plus_slope[ch] = GetYPlusSlope(ch);
				s_eq.cur_y_minus_slope[ch] = GetYMinusSlope(ch);

				/* continuosly, send eq stage to camera */
				acp_isp_write_eqstage( ch, s_eq.ch_stage[ch], s_eq.ch_curvidmode[ch] );
			}
		}
	}
	#ifndef _EQ_ENHENCED_
    	if( ( s_eq.ch_curvidon[ch] != EQ_VIDEO_OFF ) && (s_eq.ch_curvidmode[ch] != NVP6134_VI_1080P_NOVIDEO) )
	{
		/* In case of AHD, check cuncurrency saturation */
		ret = eq_check_concurrent_saturation( ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch], s_eq.ch_stage[ch] );
		if( ret == 1 && s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_1080P )
		{
			unsigned char stage = 0;
			/* get stage according to EQ pattern Color Gain */
			acc_gain_status[ch] = GetAccGain(ch);
			stage = eq_get_stage( ch, s_eq.ch_curvidmode[ch], acc_gain_status[ch], 0/*Not used*/, 0/*Not used*/, s_eq.ch_vfmt_status[ch] );
			printk( ">>>>> DRV[%s:%d] CH:%d get eq stage, ACCgain:0x%x, PrevStage:%d, NowStage:%d\n",\
			        __func__, __LINE__, ch, acc_gain_status[ch], s_eq.ch_stage[ch], stage );
			if( s_eq.ch_stage[ch] != stage )
			{
				/* set eq */
				printk( ">>>>> DRV[%s:%d] CH:%d set eq value again, ACCgain:0x%x, PrevStage:%d, NowStage:%d\n",\
				        __func__, __LINE__, ch, acc_gain_status[ch], s_eq.ch_stage[ch], stage );

				eq_adjust_eqvalue( ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch], stage );
				Set_ColorLockingFilter(ch);

				s_eq.ch_stage[ch] = stage;
			}
		}
	}
	#endif
	/*
	 * save current status to pre buffer(video on/off, video format)
	 */
	if(1)
	{
		s_eq.ch_previdon[ch] = s_eq.ch_curvidon[ch];
		s_eq.ch_previdmode[ch] =s_eq.ch_curvidmode[ch];
		s_eq.ch_previdformat[ch] = s_eq.ch_curvidformat[ch];
	}

	return 0;
}

/*******************************************************************************
*	Description		: Adjust Equalizer manual mode(EQ)
*	Argurments		: ch: channel information
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
int nvp6134_set_equalizer_manual(unsigned char ch, unsigned char man_stage)
{
	unsigned int  y_plus_slope[16], y_minus_slope[16];
	//unsigned int  fr_ac_min_value[16], fr_ac_max_value[16], fr_dc_value[16];
	unsigned char vloss;
	unsigned char vidformat;
	int ret;

	/*
	 * Initialization structure
	 */
	if( bInitEQ == EQ_INIT_OFF )
	{
		memset(&s_eq, 0x00, sizeof(nvp6134_equalizer));
		memset(&s_eq_type, 0x00, sizeof(nvp6134_equalizer));
		bInitEQ = EQ_INIT_ON;
	}

	/*
	 * exception
	 * skip under NVP6134_VI_720P_2530 mode and over NVP6134_VI_BUTT
	 */
	if(ch_mode_status[ch] >= NVP6134_VI_BUTT )
	{
		s_eq.ch_previdmode[ch] = 0xFF;
		s_eq.ch_curvidmode[ch] = 0xFF;
		s_eq.ch_previdon[ch] = EQ_VIDEO_OFF;
		s_eq.ch_curvidon[ch] = EQ_VIDEO_OFF;
		s_eq.ch_previdformat[ch] = 0xFF;		 // pre video format for Auto detection value
		s_eq.ch_curvidformat[ch] = 0xFF;		 // current video format for Auto detection value
		s_eq.ch_vfmt_status[ch] = PAL;			 // default(PAL) : NTSC(0), PAL(1)ch_vfmt_status
		return 0;
	}

	/*
	 * check and set video loss and video on/off information to buffer
	 */
	if(1)
	{
		/* get video format(video loss), 0:videoloss, 1:video on */
		vidformat = g_ch_video_fmt[ch];
		if( vidformat == 0x00 )
			vloss = 0;
		else
			vloss = 1;

		/* after checking agc locking(signal) and video loss, save Video ON information to buffer */
		if( (vloss == 1) && (nvp6134_GetAgcLockStatus(ch) == 0x01))
		{
			s_eq.ch_curvidon[ch] = EQ_VIDEO_ON;
			s_eq.ch_curvidmode[ch] = ch_mode_status[ch];
			s_eq.ch_curvidformat[ch] = vidformat;
			s_eq.ch_vfmt_status[ch] = ch_vfmt_status[ch];	// NTSC/PAL
		}
		else
		{
			/* These are default value of NO video */
			s_eq.ch_curvidmode[ch] = 0xFF;
			s_eq.ch_curvidon[ch] = EQ_VIDEO_OFF;
			s_eq.ch_curvidformat[ch] = 0xFF;
			s_eq.ch_vfmt_status[ch] = PAL;
			s_eq.ch_stage[ch] = 0;
			s_eq.eq_file_save[ch] = 0;

			g_slp_ahd[ch] = 0;
			eq_chk_cnt[ch] = 0;
			/* set default value for distinguishing between EXC and AHD */
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C, 0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xB8, 0xB9);  //recover this for video loss detection

			/* hide display */
			nvp6134_hide_ch(ch);
		}
	}
	
	s_eq_type.ch_equalizer_type[ch] = 1;
	s_eq_type.ch_stage[ch] = man_stage;

	/*
	 * it only want to set EQ(algorithm) once when there is the video signal.
	 *    - PRE(video off) != CUR(video on)
	 *    - PRE(video format) != CUR(Video ON)
	 */
	if( (s_eq.ch_curvidon[ch] != EQ_VIDEO_OFF) && (s_eq.ch_curvidmode[ch] != NVP6134_VI_1080P_NOVIDEO))
	{
		//if( 	(s_eq.ch_previdon[ch] != s_eq.ch_curvidon[ch]) 
		//	|| 	(s_eq.ch_previdmode[ch] != s_eq.ch_curvidmode[ch])
		//	|| 	(s_eq.ch_stage[ch] == 0))
		{
			printk(">>>>> DRV : CH[%d] EQ algorithm! - SET EQ \n", ch );
			/* set H PLL - No video option  and PN init */
        		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
			if( (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_1080P && s_eq.ch_vfmt_status[ch] == PAL) ||
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_1080P ) ||
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_3M_NRT || s_eq.ch_curvidmode[ch] == NVP6134_VI_3M ||
				 s_eq.ch_curvidmode[ch] == NVP6134_VI_4M_NRT || s_eq.ch_curvidmode[ch] == NVP6134_VI_4M ||
				 s_eq.ch_curvidmode[ch] == NVP6134_VI_5M_NRT || s_eq.ch_curvidmode[ch] == NVP6134_VI_5M_20P) )
				gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xB8, 0x39);
			else
				gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xB8, 0x38);

			SetFSClockmodeForColorlocking( ch );
			msleep(35);

		        /* distinguish between A(THD, CHD) type and B(AHD0 type */
		        if(	(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PA) || \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_HDAEX) || \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PB) || \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_HDBEX) || \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PRT)||	\
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_1080P) ||	\
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_3M_NRT)||	\
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_5M_NRT)|| \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_1080P) || \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_720P)  || \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_720PRT)|| \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_HDEX))
		        {		            
		            	/* manual set eq stage */
				if(1 == s_eq_type.ch_equalizer_type[ch])
					s_eq.ch_stage[ch] = s_eq_type.ch_stage[ch];
					
					/* adjust EQ depend on distance */
		            	eq_adjust_eqvalue( ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch], s_eq.ch_stage[ch] );
				Set_ColorLockingFilter(ch);

				s_eq.eq_file_save[ch] = 1;
		        }
		       else
			{				
				/* manual set eq stage */
				if(1 == s_eq_type.ch_equalizer_type[ch])
					s_eq.ch_stage[ch] = s_eq_type.ch_stage[ch];

				/* adjust EQ depend on distance */
				eq_adjust_eqvalue( ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch], s_eq.ch_stage[ch] );
				Set_ColorLockingFilter(ch);

				y_plus_slope[ch]	= GetYPlusSlope(ch);
				y_minus_slope[ch]	= GetYMinusSlope(ch);
				g_slp_ahd[ch] = 0;

				s_eq.eq_file_save[ch] = 1;
			}
			if(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_1080P||s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_3M_NRT)
			{
				eq_chk_cnt[ch] ++;
				if(eq_chk_cnt[ch] < 2)
				{
					//return 1;
				}
			}
			printk(">>>>> DRV[%s:%d] CH:%d, s_eq.ch_stage:%d\n", __func__, __LINE__, ch, s_eq.ch_stage[ch]);
			/* after getting EQ value, Analog filter bypass option is off */
			if(	(s_eq.ch_curvidmode[ch] == NVP6134_VI_3M)	||
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_4M)	||
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_5M_20P) )
			{
			}
			else
			{
				gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
				gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x59, 0x00);
			}
			printk(">>>>> DRV : getting EQ value, Analog filter bypass option is off\n" );

			/* send eqstage to isp */
			ret = acp_isp_write_communcation( ch, s_eq.ch_stage[ch], s_eq.ch_curvidmode[ch] );
			//eq_set_communication_value( ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch], ret );

			/* show display */
			nvp6134_show_ch(ch);

			g_eq_set_done = g_eq_set_done | (0x1 << ch);
			printk(">>>>> DRV[%s:%d] CH:%d, EQ SET DONE[%x]. \n", __func__, __LINE__, ch, g_eq_set_done );
		}
		
	}

	/*
	 * save current status to pre buffer(video on/off, video format)
	 */
	if(1)
	{
		s_eq.ch_previdon[ch] = s_eq.ch_curvidon[ch];
		s_eq.ch_previdmode[ch] =s_eq.ch_curvidmode[ch];
		s_eq.ch_previdformat[ch] = s_eq.ch_curvidformat[ch];
	}

	return 0;
}


/*******************************************************************************
*	End of file
*******************************************************************************/
