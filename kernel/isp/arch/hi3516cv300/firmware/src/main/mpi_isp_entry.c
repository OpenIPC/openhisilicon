/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_isp.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/01/05
  Description   :
  History       :
  1.Date        : 2011/01/05
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include <stdio.h>
#include <string.h>
#if 0
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/prctl.h>

#include "mpi_sys.h"
#include "mkp_isp.h"
#include "hi_isp_debug.h"
#include "isp_debug.h"
#include "isp_defaults.h"
#include "isp_main.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_statistics.h"
#include "isp_regcfg.h"
#include "isp_proc.h"
#include "hi_vreg.h"
#include "isp_config.h"
#include "isp_ext_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/* Isp Version Proc will be Hi3518_ISP_V1.0.4.x, ISP_LIB_VERSION stands for x */
#define ISP_LIB_VERSION "0"    //[0~F]

/****************************************************************************
 * GLOBAL VARIABLES                                                         *
 ****************************************************************************/

ISP_CTX_S  g_astIspCtx[ISP_MAX_DEV_NUM] = {{0}};
HI_S32     g_as32IspFd[ISP_MAX_DEV_NUM] = {-1};

#ifndef CONFIG_QUCIK_START
HI_BOOL  bQuickStartSensorInit = HI_TRUE;
#endif


#ifdef _MSC_VER
#define MUTEX_INIT_LOCK(mutex) InitializeCriticalSection(&mutex)
#define MUTEX_LOCK(mutex) EnterCriticalSection(&mutex)
#define MUTEX_UNLOCK(mutex) LeaveCriticalSection(&mutex)
#define MUTEX_DESTROY(mutex) DeleteCriticalSection(&mutex)
#else
#define MUTEX_INIT_LOCK(mutex) \
do \
{ \
(void)pthread_mutex_init(&mutex, NULL); \
}while(0)
#define MUTEX_LOCK(mutex) \
do \
{ \
(void)pthread_mutex_lock(&mutex); \
}while(0)
#define MUTEX_UNLOCK(mutex) \
do \
{ \
(void)pthread_mutex_unlock(&mutex); \
}while(0)
#define MUTEX_DESTROY(mutex) \
do \
{ \
(void)pthread_mutex_destroy(&mutex); \
}while(0)
#endif

#if 0
static ISP_VERSION_S gs_stVersion =
{
    .au8MppVersion = ISP_LIB_VERSION,
    .au8Date = __DATE__,
    .au8Time = __TIME__,
    .u32Magic = 0,
};
#endif

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/

#define ISP_CHECK_SENSOR_REGISTER(dev)\
do {\
    if (g_astIspCtx[dev].bSnsReg != HI_TRUE)\
    {\
        printf("Sensor doesn't register to ISP[%d]!\n", dev);\
        return HI_ERR_ISP_SNS_UNREGISTER;\
    }\
}while(0)

#if 0
#define ISP_VERSION_MAGIC    20130305

/****************************************************************************
 * static functions
 ****************************************************************************/
static HI_BOOL ISP_GetVersion(HI_VOID)
{
    HI_S32 s32Ret;

    ISP_CHECK_OPEN(0);

    s32Ret = ioctl(g_as32IspFd[0], ISP_GET_VERSION, &gs_stVersion);
    if (s32Ret)
    {
        printf("register ISP lib info ec %x!\n", s32Ret);
        return HI_FALSE;
    }

    /* 20130314, disable isp magic version check */
    #if 0
    if (gs_stVersion.u32Magic != ISP_VERSION_MAGIC)
    {
        printf("isp lib version doesn't match with sdk!\n");
        return HI_FALSE;
    }
    #endif

    return HI_TRUE;
}
#endif

/*****************************************************************************
 Prototype       : HI_MPI_ISP_MemInit
 Description     : isp initial extent memory
 Input           : I_VOID  **
 Output          : None
 Return Value    :
 Process         :
 Note            :

  History
  1.Date         : 2014/8/13
    Author       : p00272155
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_MemInit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    /* 1. check status */
    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_SENSOR_REGISTER(IspDev);

    if (ioctl(g_as32IspFd[IspDev], ISP_MEM_INFO_GET, &pstIspCtx->bMemInit))
    {
        printf("ISP[%d] get Mem info failed!\n", IspDev);
        return HI_ERR_ISP_MEM_NOT_INIT;
    }
    if (HI_TRUE == pstIspCtx->bMemInit)
    {
        VReg_Exit(ISP_VREG_BASE, ISP_VREG_SIZE);
        pstIspCtx->bMemInit = HI_FALSE;
        if (ioctl(g_as32IspFd[IspDev], ISP_MEM_INFO_SET, &pstIspCtx->bMemInit))
        {
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

#if 0
    if (HI_TRUE != ISP_GetVersion())
    {
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
#endif

    /* 2. creat extern registers */
    s32Ret = VReg_Init(ISP_VREG_BASE, ISP_VREG_SIZE);
    if (HI_SUCCESS != s32Ret)
    {
        printf("ISP[%d] init Mem failed\n", IspDev);
        return s32Ret;
    }

    pstIspCtx->bMemInit = HI_TRUE;
    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_MEM_INFO_SET, &pstIspCtx->bMemInit);
    if (s32Ret)
    {
        pstIspCtx->bMemInit = HI_FALSE;
        VReg_Exit(ISP_VREG_BASE, ISP_VREG_SIZE);

        printf("ISP[%d] set Mem info failed!\n", IspDev);
        return s32Ret;
    }

    memset(&pstIspCtx->stIspParaRec, 0, sizeof(ISP_PARA_REC_S));
    hi_ext_top_wdr_cfg_write(pstIspCtx->stIspParaRec.bWDRCfg);
    hi_ext_top_pub_attr_cfg_write(pstIspCtx->stIspParaRec.bPubCfg);

    hi_ext_top_wdr_switch_write(HI_FALSE);
    hi_ext_top_res_switch_write(HI_FALSE);

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_Init
 Description     : isp initial process, include extent memory, top structure,
                    default value, etc.
 Input           : I_VOID  **
 Output          : None
 Return Value    :
 Process         :
 Note             :

  History
  1.Date         : 2011/1/13
    Author       : x00100808
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_Init(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    HI_U8 u8WDRMode;
    HI_U32 u32Value = 0;
    HI_VOID *pValue = HI_NULL;
    ISP_CMOS_SENSOR_IMAGE_MODE_S stSnsImageMode;

    /* 1. check status */
    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_SENSOR_REGISTER(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstIspCtx->stIspParaRec.bWDRCfg = hi_ext_top_wdr_cfg_read();
    ISP_CHECK_ISP_WDR_CFG(IspDev);

    pstIspCtx->stIspParaRec.bPubCfg = hi_ext_top_pub_attr_cfg_read();
    ISP_CHECK_ISP_PUB_ATTR_CFG(IspDev);

    if (HI_TRUE == pstIspCtx->stIspParaRec.bInit)
    {
        return HI_SUCCESS;
    }

    /* 2. init statistics bufs. */
    s32Ret = ISP_StatisticsInit(IspDev);
    if (HI_SUCCESS != s32Ret)
    {
        printf("ISP[%d] Init statistics failed\n", IspDev);
        goto fail0;
    }

    /* 3. init proc bufs. */
    s32Ret = ISP_ProcInit(IspDev);
    if (HI_SUCCESS != s32Ret)
    {
        goto fail1;
    }

    /* 4. init ispupdate bufs. */
    s32Ret = ISP_UpdateInfoInit(IspDev, &pstIspCtx->stUpdateInfoCtrl);
    if (HI_SUCCESS != s32Ret)
    {
        goto fail2;
    }

    /* 5. set WDR mode to sensor and update isp param from sensor and sensor init */
    u8WDRMode = hi_ext_top_sensor_wdr_mode_read();
    s32Ret = ISP_SensorSetWDRMode(IspDev, u8WDRMode);
    if (HI_SUCCESS != s32Ret)
    {
        printf("ISP[%d] set sensor WDR mode failed!\n", IspDev);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto fail3;
    }

    stSnsImageMode.u16Width = hi_ext_sync_total_width_read();
    stSnsImageMode.u16Height = hi_ext_sync_total_height_read();
    u32Value = hi_ext_system_fps_base_read();
    pValue = (HI_VOID *)&u32Value;
    stSnsImageMode.f32Fps = *(HI_FLOAT *)pValue;
    s32Ret = ISP_SensorSetImageMode(IspDev, &stSnsImageMode);
    if (HI_SUCCESS != s32Ret)
    {
        printf("ISP[%d] set sensor image mode failed!\n", IspDev);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto fail3;
    }

#ifdef CONFIG_QUICK_START
    if (HI_FALSE == bQuickStartSensorInit)
    {
        s32Ret = ISP_SensorInit(IspDev);
        if (HI_SUCCESS != s32Ret)
        {
            printf("ISP[%d] init sensor failed!\n", IspDev);
            s32Ret = HI_ERR_ISP_NOT_INIT;
            goto fail3;
        }
    }
    else
    {
        bQuickStartSensorInit = HI_FALSE;
    }
#else
    s32Ret = ISP_SensorInit(IspDev);
    if (HI_SUCCESS != s32Ret)
    {
        printf("ISP[%d] init sensor failed!\n", IspDev);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto fail3;
    }
#endif

    s32Ret = ISP_SensorUpdateAll(IspDev);
    if (HI_SUCCESS != s32Ret)
    {
        printf("ISP[%d] update sensor all failed!\n", IspDev);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto fail3;
    }

    /* 6. init the common part of extern registers and real registers */
    ISP_ExtRegsDefault();
    ISP_RegsDefault();
    ISP_ExtRegsInitialize(IspDev);
    ISP_RegsInitialize(IspDev);

    /* 7. isp algs global variable initialize */
    ISP_GlobalInitialize(IspDev);
    /* 8. register all algorithm to isp, and init them. */
    ISP_AlgsRegister(IspDev);
    ISP_AlgsInit(pstIspCtx->astAlgs, IspDev);
    /* 9. set WDR mode to kernel. */
    s32Ret = ISP_WDRCfgSet(IspDev);
    if (HI_SUCCESS != s32Ret)
    {
        printf("ISP[%d] set WDR mode to kernel failed!\n", IspDev);
        goto fail3;
    }
    /* 10. init isp global variables */
    if (pthread_mutex_init(&pstIspCtx->stLock, HI_NULL) != 0)
    {
        s32Ret = HI_FAILURE;
        goto fail3;
    }

    if (ioctl(g_as32IspFd[IspDev], ISP_SET_REGNEWER) < 0)
    {
        perror("Set ISP Regnewer failed!\n");
        goto fail4;;
    }

    MUTEX_LOCK(pstIspCtx->stLock);
    pstIspCtx->stIspParaRec.bInit = HI_TRUE;
    pstIspCtx->stIspParaRec.bRunEn = HI_TRUE;
    MUTEX_UNLOCK(pstIspCtx->stLock);

    return HI_SUCCESS;
fail4:
    pthread_mutex_destroy(&pstIspCtx->stLock);
fail3:
    ISP_UpdateInfoExit(IspDev, &pstIspCtx->stUpdateInfoCtrl);
fail2:
    ISP_ProcExit(IspDev);
fail1:
    ISP_StatisticsExit(IspDev);
fail0:
    //VReg_Exit(ISP_VREG_BASE, ISP_VREG_SIZE);

    return s32Ret;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_Run
 Description     : isp firmware recurrent task, always run in a single thread.
 Input           : I_VOID  **
 Output          : None
 Return Value    :
 Process         :
 Note             :

  History
  1.Date         : 2011/1/13
    Author       : x00100808
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_Run(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    HI_U32 u32IntStatus;
    HI_BOOL bEn;
    HI_U32 u32WDRmode;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    /* 1. check status */
    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_SENSOR_REGISTER(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    ISP_CHECK_ISP_INIT(IspDev);
    prctl(PR_SET_NAME, (unsigned long)"hi_isp_run", 0, 0, 0);

    MUTEX_LOCK(pstIspCtx->stLock);

    /* Sometimes HI_MPI_ISP_Run thread is not scheduled to run before calling HI_MPI_ISP_Exit. */
    if (HI_FALSE == pstIspCtx->stIspParaRec.bRunEn)
    {
        MUTEX_UNLOCK(pstIspCtx->stLock);
        return HI_SUCCESS;
    }

    /* 2. enable interrupt */
    bEn = HI_TRUE;
    if (ioctl(g_as32IspFd[IspDev], ISP_SET_INT_ENABLE, &bEn) < 0)
    {

        perror("Enable ISP interrupt failed!\n");
        MUTEX_UNLOCK(pstIspCtx->stLock);
        return -1;
    }
    MUTEX_UNLOCK(pstIspCtx->stLock);

    while (1)
    {
        MUTEX_LOCK(pstIspCtx->stLock);
        if (HI_FALSE == pstIspCtx->stIspParaRec.bRunEn)
        {
            MUTEX_UNLOCK(pstIspCtx->stLock);
            break;
        }

       /*change  resolution  */
        ISP_SwitchImageMode(IspDev);

        u32WDRmode = hi_ext_top_sensor_wdr_mode_read();
        /* swtich linear/WDR mode, width/height, fps  */
        if (pstIspCtx->u8SnsWDRMode != u32WDRmode)
        {
            pstIspCtx->u8SnsWDRMode = u32WDRmode;
            ISP_SwitchWDRMode(IspDev);
        }

        MUTEX_UNLOCK(pstIspCtx->stLock);

        u32IntStatus = 0;
        /* 3. waked up by the interrupt */
        s32Ret = ioctl(g_as32IspFd[IspDev], ISP_GET_FRAME_EDGE, &u32IntStatus);
        if (HI_SUCCESS == s32Ret)
        {
            /* 4.isp firmware calculate, include AE/AWB, etc. */
            if (ISP_1ST_INT & u32IntStatus)
            {
                MUTEX_LOCK(pstIspCtx->stLock);
                if (HI_FALSE == pstIspCtx->stIspParaRec.bRunEn)
                {
                    MUTEX_UNLOCK(pstIspCtx->stLock);
                    return HI_SUCCESS;
                }

                ISP_Run(IspDev);
                MUTEX_UNLOCK(pstIspCtx->stLock);
            }
        }
    }

    /* 8. disable interrupt */
    bEn = HI_FALSE;
    if (ioctl(g_as32IspFd[IspDev], ISP_SET_INT_ENABLE, &bEn) < 0)
    {
        perror("Disable ISP interrupt failed!\n");
    }

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_Exit
 Description     : control isp to exit recurrent task, always run in main process.
 Input           : I_VOID  **
 Output          : None
 Return Value    :
 Process         :
 Note             :

  History
  1.Date         : 2011/1/13
    Author       : x00100808
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_Exit(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;

    /* 1. Check status */
    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    /* 2. exit global variables */
    MUTEX_LOCK(pstIspCtx->stLock);
    memset(&pstIspCtx->stIspParaRec, 0, sizeof(ISP_PARA_REC_S));
    hi_ext_top_wdr_cfg_write(pstIspCtx->stIspParaRec.bWDRCfg);
    hi_ext_top_pub_attr_cfg_write(pstIspCtx->stIspParaRec.bPubCfg);

    /* 3. exit algorthmics */
    ISP_AlgsExit(pstIspCtx->astAlgs, IspDev);

    /* 4. unregister algorthmics, 3a libs */
    ISP_AlgsUnRegister(IspDev);
    ISP_LibsUnRegister(IspDev);

    /* 5. sensor exit */
    ISP_SensorExit(IspDev);

    /* 6. release proc bufs. */
    ISP_ProcExit(IspDev);

    /* 7. exit update info bufs. */
    ISP_UpdateInfoExit(IspDev, &pstIspCtx->stUpdateInfoCtrl);

    /* 7. exit statistics bufs. */
    ISP_StatisticsExit(IspDev);

    /* 8. release vregs */
    VReg_Munmap();
    VReg_Exit(ISP_VREG_BASE, ISP_VREG_SIZE);

    MUTEX_UNLOCK(pstIspCtx->stLock);

    pstIspCtx->bMemInit = HI_FALSE;
    if (ioctl(g_as32IspFd[IspDev], ISP_MEM_INFO_SET, &pstIspCtx->bMemInit))
    {
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_SetRegister
 Description     : set isp register, include extent memory.
 Input           : u32Addr   **
                   u32Value  **
 Output          : None
 Return Value    :
 Process         :
 Note             :

  History
  1.Date         : 2011/1/13
    Author       : x00100808
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_SetRegister(ISP_DEV IspDev, HI_U32 u32Addr, HI_U32 u32Value)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
    IO_WRITE32(u32Addr, u32Value);

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_GetRegister
 Description     : get isp register, include extent memory.
 Input           : u32Addr    **
                   pu32Value  **
 Output          : None
 Return Value    :
 Process         :
 Note             :

  History
  1.Date         : 2011/1/13
    Author       : x00100808
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_GetRegister(ISP_DEV IspDev, HI_U32 u32Addr, HI_U32 *pu32Value)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
    ISP_CHECK_POINTER(pu32Value);
    *pu32Value = IO_READ32(u32Addr);

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_SensorRegister
 Description     : sensor register callback function to isp firmware
 Input           : pstIspSensorRegister  **
 Output          : None
 Return Value    :
 Process         :
 Note             :

  History
  1.Date         : 2011/1/21
    Author       : x00100808
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_SensorRegCallBack(ISP_DEV IspDev, SENSOR_ID SensorId, ISP_SENSOR_REGISTER_S *pstRegister)
{
    HI_S32 s32Ret;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstRegister);

    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    s32Ret = ISP_SensorRegCallBack(IspDev, SensorId, pstRegister);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    pstIspCtx->stBindAttr.SensorId = SensorId;
    pstIspCtx->bSnsReg = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_AELibRegCallBack(ISP_DEV IspDev, ALG_LIB_S *pstAeLib,
        ISP_AE_REGISTER_S *pstRegister)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstAeLibNode = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    /* check null point */
    ISP_CHECK_POINTER(pstAeLib);
    ISP_CHECK_POINTER(pstRegister);

    ISP_CHECK_POINTER(pstRegister->stAeExpFunc.pfn_ae_init);
    ISP_CHECK_POINTER(pstRegister->stAeExpFunc.pfn_ae_run);
    ISP_CHECK_POINTER(pstRegister->stAeExpFunc.pfn_ae_ctrl);
    ISP_CHECK_POINTER(pstRegister->stAeExpFunc.pfn_ae_exit);

    /* whether can we register a new lib  */
    pstAeLibNode = ISP_SearchLib(pstIspCtx->stAeLibInfo.astLibs);
    if (HI_NULL == pstAeLibNode)
    {
        printf("can't register aelib to isp, there is too many libs.\n");
        return HI_FAILURE;
    }

    /* record register info */
    memcpy(&pstAeLibNode->stAlgLib, pstAeLib, sizeof(ALG_LIB_S));
    memcpy(&pstAeLibNode->stAeRegsiter, pstRegister, sizeof(ISP_AE_REGISTER_S));
    pstAeLibNode->bUsed = HI_TRUE;

    /* set active lib */
    pstIspCtx->stAeLibInfo.u32ActiveLib = ISP_FindLib(pstIspCtx->stAeLibInfo.astLibs, pstAeLib);
    memcpy(&pstIspCtx->stBindAttr.stAeLib, pstAeLib, sizeof(ALG_LIB_S));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_AWBLibRegCallBack(ISP_DEV IspDev, ALG_LIB_S *pstAwbLib,
        ISP_AWB_REGISTER_S *pstRegister)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstAwbLibNode = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    /* check null point */
    ISP_CHECK_POINTER(pstAwbLib);
    ISP_CHECK_POINTER(pstRegister);

    ISP_CHECK_POINTER(pstRegister->stAwbExpFunc.pfn_awb_init);
    ISP_CHECK_POINTER(pstRegister->stAwbExpFunc.pfn_awb_run);
    ISP_CHECK_POINTER(pstRegister->stAwbExpFunc.pfn_awb_ctrl);
    ISP_CHECK_POINTER(pstRegister->stAwbExpFunc.pfn_awb_exit);

    /* whether can we register a new lib  */
    pstAwbLibNode = ISP_SearchLib(pstIspCtx->stAwbLibInfo.astLibs);
    if (HI_NULL == pstAwbLibNode)
    {
        printf("can't register awblib to isp, there is too many libs.\n");
        return HI_FAILURE;
    }

    /* record register info */
    memcpy(&pstAwbLibNode->stAlgLib, pstAwbLib, sizeof(ALG_LIB_S));
    memcpy(&pstAwbLibNode->stAwbRegsiter, pstRegister, sizeof(ISP_AWB_REGISTER_S));
    pstAwbLibNode->bUsed = HI_TRUE;

    /* set active lib */
    pstIspCtx->stAwbLibInfo.u32ActiveLib = ISP_FindLib(pstIspCtx->stAwbLibInfo.astLibs, pstAwbLib);
    memcpy(&pstIspCtx->stBindAttr.stAwbLib, pstAwbLib, sizeof(ALG_LIB_S));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_AFLibRegCallBack(ISP_DEV IspDev, ALG_LIB_S *pstAfLib,
        ISP_AF_REGISTER_S *pstRegister)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstAfLibNode = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    /* check null point */
    ISP_CHECK_POINTER(pstAfLib);
    ISP_CHECK_POINTER(pstRegister);

    ISP_CHECK_POINTER(pstRegister->stAfExpFunc.pfn_af_init);
    ISP_CHECK_POINTER(pstRegister->stAfExpFunc.pfn_af_run);
    ISP_CHECK_POINTER(pstRegister->stAfExpFunc.pfn_af_ctrl);
    ISP_CHECK_POINTER(pstRegister->stAfExpFunc.pfn_af_exit);

    /* whether can we register a new lib  */
    pstAfLibNode = ISP_SearchLib(pstIspCtx->stAfLibInfo.astLibs);
    if (HI_NULL == pstAfLibNode)
    {
        printf("can't register aflib to isp, there is too many libs.\n");
        return HI_FAILURE;
    }

    /* record register info */
    memcpy(&pstAfLibNode->stAlgLib, pstAfLib, sizeof(ALG_LIB_S));
    memcpy(&pstAfLibNode->stAfRegsiter, pstRegister, sizeof(ISP_AF_REGISTER_S));
    pstAfLibNode->bUsed = HI_TRUE;

    /* set active lib */
    pstIspCtx->stAfLibInfo.u32ActiveLib = ISP_FindLib(pstIspCtx->stAfLibInfo.astLibs, pstAfLib);
    memcpy(&pstIspCtx->stBindAttr.stAfLib, pstAfLib, sizeof(ALG_LIB_S));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SensorUnRegCallBack(ISP_DEV IspDev, SENSOR_ID SensorId)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    /* check sensor id */
    if (pstIspCtx->stBindAttr.SensorId != SensorId)
    {
        printf("UnReg ERR! Registered sensor is %d, present sensor is %d.\n",
            pstIspCtx->stBindAttr.SensorId, SensorId);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    pstIspCtx->stBindAttr.SensorId = 0;
    pstIspCtx->bSnsReg = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_AELibUnRegCallBack(ISP_DEV IspDev, ALG_LIB_S *pstAeLib)
{
    HI_S32 s32SearchId;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    /* check null point */
    ISP_CHECK_POINTER(pstAeLib);

    s32SearchId = ISP_FindLib(pstIspCtx->stAeLibInfo.astLibs, pstAeLib);
    if (-1 == s32SearchId)
    {
        printf("can't find ae lib in isp.\n");
        return HI_FAILURE;
    }

    memset(&pstIspCtx->stAeLibInfo.astLibs[s32SearchId], 0, sizeof(ISP_LIB_NODE_S));

    /* set active lib */
    pstIspCtx->stAeLibInfo.u32ActiveLib = 0;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_AWBLibUnRegCallBack(ISP_DEV IspDev, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32SearchId;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    /* check null point */
    ISP_CHECK_POINTER(pstAwbLib);

    s32SearchId = ISP_FindLib(pstIspCtx->stAwbLibInfo.astLibs, pstAwbLib);
    if (-1 == s32SearchId)
    {
        printf("can't find awb lib in isp.\n");
        return HI_FAILURE;
    }

    memset(&pstIspCtx->stAwbLibInfo.astLibs[s32SearchId], 0, sizeof(ISP_LIB_NODE_S));

    /* set active lib */
    pstIspCtx->stAwbLibInfo.u32ActiveLib = 0;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_AFLibUnRegCallBack(ISP_DEV IspDev, ALG_LIB_S *pstAfLib)
{
    HI_S32 s32SearchId;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    /* check null point */
    ISP_CHECK_POINTER(pstAfLib);

    s32SearchId = ISP_FindLib(pstIspCtx->stAfLibInfo.astLibs, pstAfLib);
    if (-1 == s32SearchId)
    {
        printf("can't find af lib in isp.\n");
        return HI_FAILURE;
    }

    memset(&pstIspCtx->stAfLibInfo.astLibs[s32SearchId], 0, sizeof(ISP_LIB_NODE_S));

    /* set active lib */
    pstIspCtx->stAfLibInfo.u32ActiveLib = 0;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetBindAttr(ISP_DEV IspDev, const ISP_BIND_ATTR_S *pstBindAttr)
{
    SENSOR_ID SensorId;
    HI_S32    s32SearchId;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    ISP_CHECK_POINTER(pstBindAttr);

    ISP_SensorGetId(IspDev, &SensorId);
    /* check sensor id */
    if (pstBindAttr->SensorId != SensorId)
    {
        printf("Register sensor is %d, present sensor is %d.\n",
            SensorId, pstBindAttr->SensorId);
        return HI_FAILURE;
    }

    /* check ae lib */
    s32SearchId = ISP_FindLib(pstIspCtx->stAeLibInfo.astLibs, &pstBindAttr->stAeLib);
    if (-1 != s32SearchId)
    {
        pstIspCtx->stAeLibInfo.u32ActiveLib = s32SearchId;
    }
    else
    {
        return HI_FAILURE;
    }

    /* check awb lib */
    s32SearchId = ISP_FindLib(pstIspCtx->stAwbLibInfo.astLibs, &pstBindAttr->stAwbLib);
    if (-1 != s32SearchId)
    {
        pstIspCtx->stAwbLibInfo.u32ActiveLib = s32SearchId;
    }
    else
    {
        return HI_FAILURE;
    }

    /* check af lib */
    s32SearchId = ISP_FindLib(pstIspCtx->stAfLibInfo.astLibs, &pstBindAttr->stAfLib);
    if (-1 != s32SearchId)
    {
        pstIspCtx->stAfLibInfo.u32ActiveLib = s32SearchId;
    }
    else
    {
        return HI_FAILURE;
    }

    /* save global variable */
    memcpy(&pstIspCtx->stBindAttr, pstBindAttr, sizeof(ISP_BIND_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetBindAttr(ISP_DEV IspDev, ISP_BIND_ATTR_S *pstBindAttr)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    ISP_CHECK_POINTER(pstBindAttr);

    /* get global variable */
    memcpy(pstBindAttr, &pstIspCtx->stBindAttr, sizeof(ISP_BIND_ATTR_S));

    return HI_SUCCESS;
}

#if 0
#define ISP_DATA_REVERSE(value, bytenum)\
do{\
    HI_S32 j, s32Revs = 0;\
    for(j=0;j<(bytenum*8);j++)\
    {\
        s32Revs = s32Revs + ((((value)>>j)&(0x01))<<((bytenum*8-1)-j));\
    }\
    value = s32Revs;\
}while(0)
#else
#define ISP_DATA_REVERSE(value, bytenum)
#endif

HI_S32 HI_MPI_ISP_SnsRegsCfg(ISP_DEV IspDev, ISP_SNS_REGS_INFO_S *pstSnsRegsInfo)
{
    HI_S32 i;

    ISP_CHECK_POINTER(pstSnsRegsInfo);
    ISP_CHECK_OPEN(IspDev);

    if ((pstSnsRegsInfo->enSnsType >= ISP_SNS_TYPE_BUTT))
    {
        printf("senor's regs info invalid, enSnsType %d\n", pstSnsRegsInfo->enSnsType);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstSnsRegsInfo->u32RegNum > ISP_MAX_SNS_REGS)
    {
        printf("senor's regs info invalid, u32RegNum %d\n", pstSnsRegsInfo->u32RegNum);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (ISP_SNS_SSP_TYPE == pstSnsRegsInfo->enSnsType)
    {
        for (i=0; i<pstSnsRegsInfo->u32RegNum; i++)
        {

            ISP_DATA_REVERSE(pstSnsRegsInfo->astSspData[i].u32DevAddr,
                pstSnsRegsInfo->astSspData[i].u32DevAddrByteNum);
            ISP_DATA_REVERSE(pstSnsRegsInfo->astSspData[i].u32RegAddr,
                pstSnsRegsInfo->astSspData[i].u32RegAddrByteNum);
            ISP_DATA_REVERSE(pstSnsRegsInfo->astSspData[i].u32Data,
                pstSnsRegsInfo->astSspData[i].u32DataByteNum);

            pstSnsRegsInfo->astSspData[i].u32RegAddr &= 0xffffffff;   // 0 means write
            pstSnsRegsInfo->astSspData[i].u32DevAddr &= 0xfffffffe;   // 0 means write
        }
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetVDTimeOut(ISP_DEV IspDev, ISP_VD_INFO_S *pstIspVdInfo, HI_U32 u32MilliSec)
{
    ISP_VD_TIMEOUT_S stIspVdTimeOut;
    HI_S32 s32Ret;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    stIspVdTimeOut.u32MilliSec = u32MilliSec;
    stIspVdTimeOut.s32IntStatus = 0x0;

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_GET_VD_TIMEOUT, &stIspVdTimeOut);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if (HI_NULL != pstIspVdInfo)
    {
        memcpy(pstIspVdInfo, &stIspVdTimeOut.stVdInfo, sizeof(ISP_VD_INFO_S));
    }

    if (0 == (0x1 & stIspVdTimeOut.s32IntStatus))
    {
       return -1;
    }

   return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetVDEndTimeOut(ISP_DEV IspDev, ISP_VD_INFO_S *pstIspVdInfo, HI_U32 u32MilliSec)
{
    ISP_VD_TIMEOUT_S stIspVdTimeOut;
    HI_S32 s32Ret;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    stIspVdTimeOut.u32MilliSec = u32MilliSec;
    stIspVdTimeOut.s32IntStatus = 0x0;

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_GET_VD_END_TIMEOUT, &stIspVdTimeOut);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if (HI_NULL != pstIspVdInfo)
    {
        memcpy(pstIspVdInfo, &stIspVdTimeOut.stVdInfo, sizeof(ISP_VD_INFO_S));
    }

    if (0 == (0x1 & stIspVdTimeOut.s32IntStatus))
    {
       return -1;
    }

   return HI_SUCCESS;
}

#ifdef ENABLE_JPEGEDCF
/*****************************************************************************
 Prototype       : HI_MPI_ISP_SetDCFInfo
 Description     : set dcf info to isp firmware
 Input           : ISP_DCF_S *pstIspDCF
 Output          : None
 Return Value    :
 Process         :
 Note            :

  History
  1.Date         : 2014/6/13
    Author       : z00183560
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_SetDCFInfo(const ISP_DCF_INFO_S *pstIspDCF)
{
    ISP_DEV IspDev = 0;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    HI_BOOL bTempMap = HI_FALSE;

    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstIspDCF);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if (HI_NULL == pstIspCtx->stUpdateInfoCtrl.u32UpdateInfoPhyaddr)
    {
        pstIspCtx->stUpdateInfoCtrl.u32UpdateInfoPhyaddr = hi_ext_system_update_info_phyaddr_read();

        pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo = HI_MPI_SYS_MmapCache(pstIspCtx->stUpdateInfoCtrl.u32UpdateInfoPhyaddr,
                                                            (sizeof(ISP_UPDATE_INFO_S) + sizeof(ISP_DCF_CONST_INFO_S)));
        if (HI_NULL == pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo)
        {
            printf("set dcf info mmap failed!\n");
            return HI_ERR_ISP_NOMEM;
        }

        pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo = (ISP_DCF_CONST_INFO_S*)(pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo + 1);

        bTempMap = HI_TRUE;
    }

    memcpy(pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo,
              pstIspDCF, sizeof(HI_U8)*DCF_DRSCRIPTION_LENGTH*4);

    pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u32FNumber              = pstIspDCF->u32FNumber;
    pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u32MaxApertureValue     = pstIspDCF->u32MaxApertureValue;
    pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u8LightSource           = pstIspDCF->u8LightSource;
    pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u32FocalLength          = pstIspDCF->u32FocalLength;
    pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u8SceneType             = pstIspDCF->u8SceneType;
    pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u8CustomRendered        = pstIspDCF->u8CustomRendered;
    pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u8FocalLengthIn35mmFilm = pstIspDCF->u8FocalLengthIn35mmFilm;
    pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u8SceneCaptureType      = pstIspDCF->u8SceneCaptureType;
    pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u8GainControl           = pstIspDCF->u8GainControl;
    pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u8Contrast              = pstIspDCF->u8Contrast;
    pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u8Saturation            = pstIspDCF->u8Saturation;
    pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u8Sharpness             = pstIspDCF->u8Sharpness;

    if (bTempMap)
    {
        HI_MPI_SYS_Munmap(pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo,
                             (sizeof(ISP_UPDATE_INFO_S) + sizeof (ISP_DCF_CONST_INFO_S)));

        pstIspCtx->stUpdateInfoCtrl.u32UpdateInfoPhyaddr = HI_NULL;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_GetDCFInfo
 Description     : get dcf info from isp firmware
 Input           : ISP_DCF_S *pstIspDCF
 Output          : None
 Return Value    :
 Process         :
 Note            :

  History
  1.Date         : 2014/6/16
    Author       : z00183560
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_GetDCFInfo(ISP_DCF_INFO_S *pstIspDCF)
{
    ISP_DEV IspDev = 0;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    HI_BOOL bTempMap = HI_FALSE;

    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstIspDCF);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    if (HI_NULL == pstIspCtx->stUpdateInfoCtrl.u32UpdateInfoPhyaddr)
    {
        pstIspCtx->stUpdateInfoCtrl.u32UpdateInfoPhyaddr = hi_ext_system_update_info_phyaddr_read();

        pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo = HI_MPI_SYS_MmapCache(pstIspCtx->stUpdateInfoCtrl.u32UpdateInfoPhyaddr,
                                                            (sizeof(ISP_UPDATE_INFO_S) + sizeof(ISP_DCF_CONST_INFO_S)));
        if (HI_NULL == pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo)
        {
            printf("get dcf info mmap failed!\n");
            return HI_ERR_ISP_NOMEM;
        }

        pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo = (ISP_DCF_CONST_INFO_S*)(pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo + 1);

        bTempMap = HI_TRUE;
    }

    memcpy(pstIspDCF, pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo,
              sizeof(HI_U8)*DCF_DRSCRIPTION_LENGTH*4);

    pstIspDCF->u32FNumber               = pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u32FNumber;
    pstIspDCF->u32MaxApertureValue      = pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u32MaxApertureValue;
    pstIspDCF->u8LightSource            = pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u8LightSource;
    pstIspDCF->u32FocalLength           = pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u32FocalLength;
    pstIspDCF->u8SceneType              = pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u8SceneType;
    pstIspDCF->u8CustomRendered         = pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u8CustomRendered;
    pstIspDCF->u8FocalLengthIn35mmFilm  = pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u8FocalLengthIn35mmFilm;
    pstIspDCF->u8SceneCaptureType       = pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u8SceneCaptureType;
    pstIspDCF->u8GainControl            = pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u8GainControl;
    pstIspDCF->u8Contrast               = pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u8Contrast;
    pstIspDCF->u8Saturation             = pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u8Saturation;
    pstIspDCF->u8Sharpness              = pstIspCtx->stUpdateInfoCtrl.pstIspDCFConstInfo->u8Sharpness;

    pstIspDCF->u16ISOSpeedRatings       = pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo->u16ISOSpeedRatings;
    pstIspDCF->u32ExposureTime          = pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo->u32ExposureTime;
    pstIspDCF->u32ExposureBiasValue     = pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo->u32ExposureBiasValue;
    pstIspDCF->u8ExposureProgram        = pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo->u8ExposureProgram;
    pstIspDCF->u8MeteringMode           = pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo->u8MeteringMode;
    pstIspDCF->u8ExposureMode           = pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo->u8ExposureMode;
    pstIspDCF->u8WhiteBalance           = pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo->u8WhiteBalance;

    if (bTempMap)
    {
        HI_MPI_SYS_Munmap(pstIspCtx->stUpdateInfoCtrl.pstIspUpdateInfo,
                             (sizeof(ISP_UPDATE_INFO_S) + sizeof (ISP_DCF_CONST_INFO_S)));

        pstIspCtx->stUpdateInfoCtrl.u32UpdateInfoPhyaddr = HI_NULL;
    }

    return HI_SUCCESS;
}
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

