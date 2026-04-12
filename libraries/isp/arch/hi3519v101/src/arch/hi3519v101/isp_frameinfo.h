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
#ifndef __ISP_FRAMEINFO_H__
#define __ISP_FRAMEINFO_H__

#include "hi_comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiISP_FRAME_INFO_CTRL_S
{
    HI_BOOL                     bSupportFrame;
    HI_U32                      u32FrameInfoPhyaddr;
    FRAME_SUPPLEMENT_INFO_S*    pstIspFrame;
} ISP_FRAME_INFO_CTRL_S;

HI_S32 ISP_FrameInfoInit(ISP_DEV IspDev, ISP_FRAME_INFO_CTRL_S* pstFrameInfoCtrl);

HI_S32 ISP_FrameInfoExit(ISP_DEV IspDev, ISP_FRAME_INFO_CTRL_S* pstFrameInfoCtrl);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
