/********************************************************************************
*
*  Copyright (C) 2017 	NEXTCHIP Inc. All rights reserved.
*  Module		: AHD Coax protocol header file
*  Description	: communicate between Decoder and ISP
*  Author		:
*  Date         :
*  Version		: Version 2.0
*
********************************************************************************
*  History      :
*
*
********************************************************************************/
#ifndef __ACP_NVP6134_H__
#define __ACP_NVP6134_H__


/********************************************************************
 *  define and enum
 ********************************************************************/
/* init ACP buffer flag */
#define ACP_INIT_ON				0x00
#define ACP_INIT_OFF			0x01

/* common ACP define */
#define ACP_COMMON_ON			0x00
#define ACP_COMMON_OFF			0x01

/* receive maximum line */
#define ACP_RECV_MAX_LINE_8		0x00
#define ACP_RECV_MAX_LINE_4		0x01

/* max channel number */
#define MAX_CH_NUM				16

/* ACP command status */
#define ACP_CAM_STAT			0x55
#define ACP_REG_WR				0x60
#define ACP_REG_RD				0x61
#define ACP_MODE_ID				0x60

/* for baud rate */
#define ACP_PACKET_MODE			0x0B
#define ACP_AHD2_FHD_D0			0x10
#define ACP_AHD2_PEL_BAUD		0x02
#define ACP_AHD2_PEL_LINE		0x07
#define ACP_AHD2_PEL_SYNC		0x0D
#define ACP_AHD2_PEL_EVEN		0x2F
#define ACP_AHD2_FHD_BAUD		0x00
#define ACP_AHD2_FHD_LINE		0x03
#define ACP_AHD2_FHD_LINES		0x05
#define ACP_AHD2_FHD_BYTE		0x0A
#define ACP_AHD2_FHD_MODE		0x0B
#define ACP_AHD2_FHD_OUT		0x09
#define ACP_CLR_REG				0x3A

/********************************************************************
 *  structure
 ********************************************************************/
/* ACP structure, this structure shared with application */
typedef struct _nvp6134_acp_
{
	unsigned char ch_recvmaxline[MAX_CH_NUM];		// receive max line(ACP_RECV_MAX_LINE_8, ACP_RECV_MAX_LINE_4)

} nvp6134_acp;
 
/* acp read/write extention structure */
typedef struct _nvp6134_acp_rw_data_extention_
{
	unsigned char ch;		// channel
	unsigned char data[16];	// data including command

}nvp6134_acp_rw_data_extention;
 
/********************************************************************
 *  external api
 ********************************************************************/
extern void 			init_acp(unsigned char ch);
extern void 			acp_each_setting(unsigned char ch);
extern void 			acp_read(nvp6134_input_videofmt *pvideoacp, unsigned char ch);
extern unsigned char 	acp_isp_read(unsigned char ch, unsigned int reg_addr);
extern void 			acp_isp_write(unsigned char ch, unsigned int reg_addr, unsigned char reg_data);
extern void 			acp_isp_write_extention( unsigned char ch, void *p_param );
extern void 			acp_reg_rx_clear(unsigned char ch);
extern int 				acp_isp_write_eqstage( unsigned char ch, unsigned char stage, unsigned char vidmode );
extern int 				acp_isp_write_communcation( unsigned char ch, unsigned char stage, unsigned char vidmode );
extern void 			acp_ctl_dec( unsigned char ch, void *p_param );

#endif	// End of __ACP_NVP6134_H__

/********************************************************************
 *  End of file
 ********************************************************************/
