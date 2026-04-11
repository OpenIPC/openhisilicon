/******************************************************************************

  Copyright (C), 2014-2019, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_frameinfo.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 
  Description   : 
  History       :
  1.Date        : 
    Author      : 
    Modification: Created file

******************************************************************************/
#ifndef __ISP_DNGINFO_H__
#define __ISP_DNGINFO_H__

#include "hi_comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiISP_DNG_INFO_CTRL_S
{
    HI_BOOL                     bSupportDng;
    HI_U32                      u32DngInfoPhyaddr;
    DNG_IMAGE_STATIC_INFO_S*    pstIspDng;
} ISP_DNG_INFO_CTRL_S;

typedef struct hiDNG_MATRIX_S
{
    HI_DOUBLE adAColorMatrix[9];
    HI_DOUBLE adD50ColorMatrix[9];
    HI_DOUBLE adAForwardMatrix[9];
    HI_DOUBLE adD50ForwardMatrix[9];
}DNG_MATRIX_S;

HI_S32 ISP_UpdateDngImageDynamicInfo(ISP_DEV IspDev);

HI_S32 ISP_DngInfoInit(ISP_DEV IspDev, ISP_DNG_INFO_CTRL_S* pstDngInfoCtrl);

HI_S32 ISP_DngInfoExit(ISP_DEV IspDev, ISP_DNG_INFO_CTRL_S* pstDngInfoCtrl);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
