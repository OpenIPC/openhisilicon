/********************************************************************************
*
*  Copyright (C) 2017 	NEXTCHIP Inc. All rights reserved.
*  Module		: The decoder's coax header file
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
#ifndef __COAX_PROTOCOL_NVP6134_H__
#define __COAX_PROTOCOL_NVP6134_H__

/********************************************************************
 *  define and enum
 ********************************************************************/
#define PACKET_MODE	0x0B

#define AHD2_PEL_D0	0x20
#define AHD2_FHD_D0	0x10
#define AHD2_PEL_OUT	0x0C
#define AHD2_PEL_BAUD	0x02
#define AHD2_PEL_LINE	0x07
#define AHD2_PEL_SYNC	0x0D
#define AHD2_PEL_EVEN	0x2F
#define AHD2_FHD_BAUD	0x00
#define AHD2_FHD_LINE	0x03
#define AHD2_FHD_LINES	0x05
#define AHD2_FHD_BYTE	0x0A
#define AHD2_FHD_MODE	0x0B
#define AHD2_FHD_OUT	0x09
#define ACP_CLR			0x3A

/********************************************************************
 *  structure
 ********************************************************************/
typedef struct _nvp6134_acp_rw_data_
{
	unsigned char opt;
    unsigned char ch;
	unsigned int addr;
	unsigned char data;
}nvp6134_acp_rw_data;

/********************************************************************
 *  external api
 ********************************************************************/
extern void 			nvp6134_set_coax_mode(unsigned char ch);
extern unsigned char 	nvp6134_coax_command(unsigned char pel_ch, unsigned char command);

#endif

/********************************************************************************
* End of file
********************************************************************************/
