/********************************************************************************
*
*  Copyright (C) 2017 	NEXTCHIP Inc. All rights reserved.
*  Module		: Equalizer module header file
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
#ifndef __EQUALIZER_NVP6134_H__
#define __EQUALIZER_NVP6134_H__


/********************************************************************
 *  define and enum
 ********************************************************************/
/* init equalizer buffer flag */
#define EQ_INIT_ON		0x00
#define EQ_INIT_OFF		0x01

/* common EQ define */
#define EQ_COMMON_ON	0x00
#define EQ_COMMON_OFF	0x01

/* equalizer - video on/off */
#define EQ_VIDEO_OFF	0x00
#define EQ_VIDEO_ON		0x01

/* max channel number */
#define MAX_CH_NUM		16

/********************************************************************
 *  structure
 ********************************************************************/
/* EQ structure, this structure shared with application */
typedef struct _nvp6134_equalizer_
{
	
	unsigned char ch_equalizer_type[MAX_CH_NUM];	//0->auto, 1->manually, 2->nextchip test auto
	unsigned char ch_detect_type[MAX_CH_NUM];	//0->auto, 1->AHD, 2->CHD, 3->THD, 4->CVBS
	unsigned char ch_cable_type[MAX_CH_NUM];	//CABLE_TYPE_COAX[0], CABLE_TYPE_UTP[1]
	unsigned char ch_previdmode[MAX_CH_NUM];	// pre video mode
	unsigned char ch_curvidmode[MAX_CH_NUM];	// current video mode
	unsigned char ch_previdon[MAX_CH_NUM];		// pre video on/off status
	unsigned char ch_curvidon[MAX_CH_NUM];		// current video on/off status
	unsigned char ch_previdformat[MAX_CH_NUM];	// pre video format for Auto detection value
	unsigned char ch_curvidformat[MAX_CH_NUM];	// current video format for Auto detection value


	int acc_gain[MAX_CH_NUM];					// first, get value: acc gain(value when video is ON status)
	int y_plus_slope[MAX_CH_NUM];				// first, get value: y plus slope(value when video is ON status)
	int y_minus_slope[MAX_CH_NUM];				// first, get value: y minus slope(value when video is ON status)

	int cur_y_plus_slope[MAX_CH_NUM];			// current y plus slope
	int cur_acc_gain[MAX_CH_NUM];				// current acc gain
	int cur_y_minus_slope[MAX_CH_NUM];			// current y minus slope

	int fr_ac_min_value[MAX_CH_NUM];			// first AC Min Value
	int fr_ac_max_value[MAX_CH_NUM];			// first AC Max Value
	int fr_dc_value[MAX_CH_NUM];				// first DC value

	int cur_fr_ac_min[MAX_CH_NUM];				// current y plus slope
	int cur_fr_ac_max[MAX_CH_NUM];				// current acc gain
	int cur_fr_dc[MAX_CH_NUM];					// current y minus slope

	int fr_sync_width[MAX_CH_NUM];
	int cur_sync_width[MAX_CH_NUM];

	unsigned char ch_stage[MAX_CH_NUM];			// stage of channel
	unsigned char ch_vfmt_status[MAX_CH_NUM];  	// NTSC/PAL

	int eq_file_save[MAX_CH_NUM];				// for save file

}nvp6134_equalizer;
 
/********************************************************************
 *  external api
 ********************************************************************/
extern void eq_init_each_format(unsigned char ch, int mode, const unsigned char vfmt );
extern int	nvp6134_set_equalizer(unsigned char ch);
unsigned int distinguish_GetYPlusSlope(unsigned char ch);
unsigned int distinguish_GetYMinusSlope(unsigned char ch);
unsigned int distinguish_GetAccGain(unsigned char ch);

extern int eq_check_concurrent_saturation( unsigned char ch, unsigned char resol, int vfmt, int stage );

#endif	// End of __EQUALIZER_NVP6134_H__

/********************************************************************
 *  End of file
 ********************************************************************/

