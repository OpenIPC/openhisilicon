/******************************************************************************

  Copyright (C), 2014-2019, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_frameinfo.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 
  Description   : 
  History       :
  1.Date        : 
    Author      : 
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "mpi_sys.h"
#include "isp_frameinfo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

extern HI_S32     g_as32IspFd[ISP_MAX_DEV_NUM];
HI_S32 ISP_FrameInfoInit(ISP_DEV IspDev, ISP_FRAME_INFO_CTRL_S* pstFrameInfoCtrl)
{
    HI_S32 s32Ret;
        
    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_FRAME_INFO_BUF_INIT, &pstFrameInfoCtrl->u32FrameInfoPhyaddr);
    if (HI_SUCCESS != s32Ret)
    {
        pstFrameInfoCtrl->bSupportFrame = HI_FALSE;
        
        if (HI_ERR_ISP_NOT_SUPPORT == s32Ret)
        {
            s32Ret = HI_SUCCESS;
        }
        else
        {
            printf("init frame bufs failed %x!\n", s32Ret);
        }
        return s32Ret;
    }

    pstFrameInfoCtrl->pstIspFrame = HI_MPI_SYS_Mmap(pstFrameInfoCtrl->u32FrameInfoPhyaddr,
        sizeof(FRAME_SUPPLEMENT_INFO_S) * ISP_MAX_FRAMEINFO_BUF_NUM);
    if (HI_NULL == pstFrameInfoCtrl->pstIspFrame)
    {
        printf("mmap frame info buf failed!\n");
        s32Ret = ioctl(g_as32IspFd[IspDev], ISP_FRAME_INFO_BUF_EXIT);
        if (HI_SUCCESS != s32Ret)
        {
            printf("exit frame info buf failed %x!\n", s32Ret);
            return s32Ret;
        }

        return HI_ERR_ISP_NOMEM;
    }
    pstFrameInfoCtrl->bSupportFrame = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 ISP_FrameInfoExit(ISP_DEV IspDev, ISP_FRAME_INFO_CTRL_S* pstFrameInfoCtrl)
{   
    HI_S32 s32Ret;

    if (HI_FALSE == pstFrameInfoCtrl->bSupportFrame)
    {
        return HI_SUCCESS;
    }
    
    if (HI_NULL != pstFrameInfoCtrl->pstIspFrame)
    {
        HI_MPI_SYS_Munmap(pstFrameInfoCtrl->pstIspFrame,
            sizeof(FRAME_SUPPLEMENT_INFO_S) * ISP_MAX_FRAMEINFO_BUF_NUM);
        pstFrameInfoCtrl->pstIspFrame = HI_NULL;
    }
    
    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_FRAME_INFO_BUF_EXIT);
    if (HI_SUCCESS != s32Ret)
    {
        printf("exit frame info buf failed %x!\n", s32Ret);
        return s32Ret;
    }
    
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

