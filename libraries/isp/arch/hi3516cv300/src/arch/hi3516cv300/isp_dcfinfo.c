/******************************************************************************

  Copyright (C), 2014-2019, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_dcfinfo.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/06/17
  Description   : 
  History       :
  1.Date        : 2014/06/17
    Author      : z00183560
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "mpi_sys.h"
#include "isp_dcfinfo.h"
#include "isp_ext_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

extern HI_S32     g_as32IspFd[ISP_MAX_DEV_NUM];
HI_S32 ISP_UpdateInfoInit(ISP_DEV IspDev,ISP_UPDATE_INFO_CTRL_S* pstUpdateInfoCtrl)
{
    HI_S32 s32Ret;

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_UPDATEINFO_BUF_INIT, &pstUpdateInfoCtrl->u32UpdateInfoPhyaddr);
    if (HI_SUCCESS != s32Ret)
    {

        printf("init update info bufs failed %x!\n", s32Ret);
        return s32Ret;
    }

    hi_ext_system_update_info_phyaddr_write(pstUpdateInfoCtrl->u32UpdateInfoPhyaddr);

    pstUpdateInfoCtrl->pstIspUpdateInfo = HI_MPI_SYS_MmapCache(pstUpdateInfoCtrl->u32UpdateInfoPhyaddr,
        (sizeof(ISP_UPDATE_INFO_S) + sizeof(ISP_DCF_CONST_INFO_S)));    
    if (HI_NULL == pstUpdateInfoCtrl->pstIspUpdateInfo)
    {
        printf("mmap update info buf failed!\n");
        s32Ret = ioctl(g_as32IspFd[IspDev], ISP_UPDATEINFO_BUF_EXIT);
        if(HI_SUCCESS != s32Ret)
		{
          printf("ISP update info buf exit failed!\n");
		}
        return HI_ERR_ISP_NOMEM;
    }

    pstUpdateInfoCtrl->pstIspDCFConstInfo = (ISP_DCF_CONST_INFO_S*)(pstUpdateInfoCtrl->pstIspUpdateInfo + 1);
    
    return HI_SUCCESS;
}

HI_S32 ISP_UpdateInfoExit(ISP_DEV IspDev,ISP_UPDATE_INFO_CTRL_S* pstUpdateInfoCtrl)
{   
    HI_S32 s32Ret;
 
    if (HI_NULL != pstUpdateInfoCtrl->pstIspUpdateInfo)
    {
        HI_MPI_SYS_Munmap(pstUpdateInfoCtrl->pstIspUpdateInfo,
                             (sizeof(ISP_UPDATE_INFO_S) + sizeof (ISP_DCF_CONST_INFO_S)));
    }
    
    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_UPDATEINFO_BUF_EXIT);
    if (HI_SUCCESS != s32Ret)
    {
        printf("exit update info buf failed %x!\n", s32Ret);
        return s32Ret;
    }
    
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

