/********************************************************************************
*
*  Copyright (C) 2017 	NEXTCHIP Inc. All rights reserved.
*  Module		:  The decoder's motion detection header file
*  Description	:  Now, Not used
*  Author		:
*  Date         :
*  Version		: Version 2.0
*
********************************************************************************
*  History      :
*
*
********************************************************************************/
#ifndef _MOTION_H_
#define _MOTION_H_

/********************************************************************
 *  define and enum
 ********************************************************************/

/********************************************************************
 *  structure
 ********************************************************************/

/********************************************************************
 *  external api
 ********************************************************************/
extern void nvp6134_motion_init(unsigned char ch, unsigned char onoff);
extern unsigned int nvp6134_get_motion_ch(void);
extern void nvp6134_motion_display(unsigned char ch, unsigned char onoff);
extern void nvp6134_motion_sensitivity(unsigned int sens[16]);
extern void nvp6134_motion_area_mask(unsigned char ch, unsigned int *blockset);

#endif	// end of _MOTION_H_
