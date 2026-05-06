/********************************************************************************
*
*  Copyright (C) 2017 	NEXTCHIP Inc. All rights reserved.
*  Module		: Equalizer module header file( for recovery of EQ )
*  Description	: recovery header of EQ
*  Author		:
*  Date         :
*  Version		: Version 2.0
*
********************************************************************************
*  History      :
*
*
********************************************************************************/
#ifndef __EQUALIZER_RECOVERY_H__
#define __EQUALIZER_RECOVERY_H__


/********************************************************************
 *  define and enum
 ********************************************************************/

/********************************************************************
 *  structure
 ********************************************************************/
/* EQ structure for Recovery */
typedef struct _nvp6134_eq_recovery_
{
	unsigned char ch_recovery[4];	// reserved

} nvp6134_eq_recovery_;
 
/********************************************************************
 *  external api
 ********************************************************************/
extern int __eq_recovery_Atype( int ch, int curvidmode, int vfmt, int acc_gain_status, int dc_val, int ac_min_val, int ac_max_val, int dc_value );
extern void __eq_recovery_Btype( int ch, int curvidmode, int vfmt, int acc_gain_status, int y_minus_slope, int y_plus_slope );

#endif	// End of __EQUALIZER_RECOVERY_H__

/********************************************************************
 *  End of file
 ********************************************************************/

