/*
* Copyright (c) 2018 HiSilicon Technologies Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef __HIFB_COEF_ORG_H__
#define __HIFB_COEF_ORG_H__

#include "hifb_coef.h"

/* The size of each set of scaling parameters, Take the maximum setting of 8tap */
#define VOU_ZOOM_COEF_SIZE  (VOU_ZOOM_COEF_ITEM*VOU_ZOOM_COEF_MAXTAP * 2)

/* 8tap*/
extern const int coefficient_lanczos3[18][8];//normalized ok
extern const int coefficient8_cubic[18][8];//normalized ok
extern const int coefficient8_lanczos2_8tap[18][8];//normalized ok
extern const int coefficient8_3M_a19[18][8];//normalized ok
extern const int coefficient8_2M_a05[18][8];
extern const HI_S32 coefficient8_1_5M_a05[18][8]; 

/* 6tap*/
extern const int coefficient6_6M_a25[18][6];//normalized ok
extern const int coefficient6_cubic[18][6];//normalized ok
extern const int coefficient6_5M_a25[18][6];//normalized ok
extern const int coefficient6_4M_a20[18][6];//normalized ok
extern const int coefficient6_3M_a15[18][6];//normalized ok
extern const HI_S32 coefficient6_2M_a05[18][6];
extern const HI_S32 coefficient6_1_5M_a05[18][6]; 

/* 4tap*/
extern const int coefficient4_5M_a15[18][4];//normalized ok
extern const int coefficient4_cubic[18][4];//normalized ok
extern const int coefficient4_4M_a15[18][4];//normalized ok
extern const HI_S32 coefficient4_2M_a05[18][4];
extern const HI_S32 coefficient4_1_5M_a05[18][4];


extern const HI_S32 *g_pOrgZoomCoef[VOU_ZOOM_COEF_BUTT][VOU_ZOOM_TAP_BUTT];

/* RGB->YUV601 Constant coefficient matrix */
extern const CscCoef_S g_stCSC_RGB2YUV601_tv;
/* RGB->YUV601 Constant coefficient matrix */
extern const CscCoef_S g_stCSC_RGB2YUV601_pc;
/* RGB->YUV709 Constant coefficient matrix */
extern const CscCoef_S g_stCSC_RGB2YUV709_tv;
/* RGB->YUV709 Constant coefficient matrix */
extern const CscCoef_S g_stCSC_RGB2YUV709_pc;
/* RGB->YUV2020 Constant coefficient matrix */
extern const CscCoef_S g_stCSC_RGB2YUV2020_pc;
/* YUV601->RGB Constant coefficient matrix */
extern const CscCoef_S g_stCSC_YUV6012RGB_pc;
/* YUV709->RGB Constant coefficient matrix */
extern const CscCoef_S g_stCSC_YUV7092RGB_pc;
/* YUV2020->RGB Constant coefficient matrix */
extern const CscCoef_S g_stCSC_YUV20202RGB_pc;
/* YUV601->YUV709 Constant coefficient matrix */
extern const CscCoef_S g_stCSC_YUV2YUV_601_709;
/* YUV709->YUV601 Constant coefficient matrix */
extern const CscCoef_S g_stCSC_YUV2YUV_709_601;
/* YUV601->YUV709 Constant coefficient matrix */
extern const CscCoef_S g_stCSC_Init;


extern const int HIFB_SIN_TABLE[61];
extern const int HIFB_COS_TABLE[61];


#endif
