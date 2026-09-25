/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __ISP_SNS_CTRL_H__
#define __ISP_SNS_CTRL_H__

#include "type.h"
#include "comm_3a.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


typedef struct ISP_SNS_STATE_S {
    XMEDIA_BOOL     bInit;                  /* XMEDIA_TRUE: Sensor init */
    XMEDIA_BOOL     bSyncInit;              /* XMEDIA_TRUE: Sync Reg init */
    XMEDIA_U8       u8ImgMode;
    XMEDIA_U8       u8Hdr;               /* XMEDIA_TRUE: HDR enbale */
    WDR_MODE_E  enWDRMode;

    ISP_SNS_REGS_INFO_S astRegsInfo[2]; /* [0]: Sensor reg info of cur-frame; [1]: Sensor reg info of pre-frame ; */

    XMEDIA_U32      au32FL[2];              /* [0]: FullLines of cur-frame; [1]: Pre FullLines of pre-frame */
    XMEDIA_U32      u32FLStd;               /* FullLines std */
    XMEDIA_U32      au32WDRIntTime[4];
} ISP_SNS_STATE_S;

typedef enum ISP_SNS_MIRRORFLIP_TYPE_E {
    ISP_SNS_NORMAL      = 0,
    ISP_SNS_MIRROR      = 1,
    ISP_SNS_FLIP        = 2,
    ISP_SNS_MIRROR_FLIP = 3,
    ISP_SNS_BUTT
} ISP_SNS_MIRRORFLIP_TYPE_E;

typedef struct ISP_SNS_OBJ_S {
    XMEDIA_S32 (*pfnRegisterCallback)(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib);
    XMEDIA_S32 (*pfnUnRegisterCallback)(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib);
    XMEDIA_S32 (*pfnSetBusInfo)(VI_PIPE ViPipe, ISP_SNS_COMMBUS_U unSNSBusInfo);
    XMEDIA_VOID (*pfnStandby)(VI_PIPE ViPipe);
    XMEDIA_VOID (*pfnRestart)(VI_PIPE ViPipe);
    XMEDIA_VOID (*pfnMirrorFlip)(VI_PIPE ViPipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip);
    XMEDIA_S32 (*pfnWriteReg)(VI_PIPE ViPipe, XMEDIA_S32 s32Addr, XMEDIA_S32 s32Data);
    XMEDIA_S32 (*pfnReadReg)(VI_PIPE ViPipe, XMEDIA_S32 s32Addr);
    XMEDIA_S32 (*pfnSetInit)(VI_PIPE ViPipe, ISP_INIT_ATTR_S *pstInitAttr);
} ISP_SNS_OBJ_S;

extern ISP_SNS_OBJ_S stSnsMn34220Obj;
extern ISP_SNS_OBJ_S stSnsImx377Obj;
extern ISP_SNS_OBJ_S stSnsImx299Obj;
extern ISP_SNS_OBJ_S stSnsImx477Obj;
extern ISP_SNS_OBJ_S stSnsImx299SlvsObj;
extern ISP_SNS_OBJ_S stSnsImx290Obj;
extern ISP_SNS_OBJ_S stSnsImx327Obj;
extern ISP_SNS_OBJ_S stSnsImx327_2l_Obj;
extern ISP_SNS_OBJ_S stSnsImx334Obj;
extern ISP_SNS_OBJ_S stSnsOV2718Obj;
extern ISP_SNS_OBJ_S stSnsAr0237Obj;

extern ISP_SNS_OBJ_S stSnsCmv50000Obj;
extern ISP_SNS_OBJ_S stSnsImx277SlvsObj;
extern ISP_SNS_OBJ_S stSnsImx117Obj;
extern ISP_SNS_OBJ_S stSnsImx290SlaveObj;
extern ISP_SNS_OBJ_S stSnsImx334SlaveObj;
extern ISP_SNS_OBJ_S stSnsImx226Obj;
extern ISP_SNS_OBJ_S stSnsImx335Obj;
extern ISP_SNS_OBJ_S stSnsImx307Obj;
extern ISP_SNS_OBJ_S stSnsImx307_2l_Obj;
extern ISP_SNS_OBJ_S stSnsImx458Obj;
extern ISP_SNS_OBJ_S stSnsSc1346Obj;
extern ISP_SNS_OBJ_S stSnsSc4236Obj;
extern ISP_SNS_OBJ_S stSnsSc4210Obj;
extern ISP_SNS_OBJ_S stSnsSc2231Obj;
extern ISP_SNS_OBJ_S stSnsSc2235Obj;
extern ISP_SNS_OBJ_S stSnsSc2336Obj;
extern ISP_SNS_OBJ_S stSnsSc3235Obj;
extern ISP_SNS_OBJ_S stSnsSc3335Obj;
extern ISP_SNS_OBJ_S stSnsSc3336Obj;
extern ISP_SNS_OBJ_S stSnsSoiF37Obj;
extern ISP_SNS_OBJ_S g_sns_q03_obj;
extern ISP_SNS_OBJ_S stSnsOs05aObj;
extern ISP_SNS_OBJ_S stSnsOS08A10Obj;
extern ISP_SNS_OBJ_S stSnsOs05a_2lObj;
extern ISP_SNS_OBJ_S stSnsOs04c10Obj;
extern ISP_SNS_OBJ_S stSnsOs04c10_2lObj;
extern ISP_SNS_OBJ_S stSnsGc1054Obj;
extern ISP_SNS_OBJ_S stSnsGc2053Obj;
extern ISP_SNS_OBJ_S stSnsGc2083Obj;
extern ISP_SNS_OBJ_S stSnsGc2093Obj;
extern ISP_SNS_OBJ_S stSnsgc4653_2lObj;
extern ISP_SNS_OBJ_S stSnsGc4663Obj;
extern ISP_SNS_OBJ_S stSnsSharp8kObj;
extern ISP_SNS_OBJ_S stSnsOv12870Obj;
extern ISP_SNS_OBJ_S stSnsGc2053ForCarObj;
extern ISP_SNS_OBJ_S g_sns_sc500ai_obj;
extern ISP_SNS_OBJ_S g_sns_q03p_obj;
extern ISP_SNS_OBJ_S stSnsSoiK17Obj;
extern ISP_SNS_OBJ_S stSnsSc223aObj;
extern ISP_SNS_OBJ_S stSnsSoiK06Obj;
extern ISP_SNS_OBJ_S stSnsSoiK05Obj;
extern ISP_SNS_OBJ_S stSnsSoiK04Obj;
extern ISP_SNS_OBJ_S stSnsSoiK303Obj;
extern ISP_SNS_OBJ_S stSnsSoiK03Obj;
extern ISP_SNS_OBJ_S stSnsSc401aiObj;
extern ISP_SNS_OBJ_S stSnsSc530aiObj;
extern ISP_SNS_OBJ_S stSnsSc530ai_2lObj;
extern ISP_SNS_OBJ_S stSnsSc4336Obj;
extern ISP_SNS_OBJ_S stSnsSc5336Obj;
extern ISP_SNS_OBJ_S stSnsGc5603Obj;
extern ISP_SNS_OBJ_S stSnsGc4023Obj;
extern ISP_SNS_OBJ_S stSnsGc3023Obj;
extern ISP_SNS_OBJ_S stSnsOs04b10_2lObj;
extern ISP_SNS_OBJ_S stSnsOs03b10Obj;
extern ISP_SNS_OBJ_S g_stSnsPs5260_2l_Obj;
extern ISP_SNS_OBJ_S stSnsSp2305Obj;
extern ISP_SNS_OBJ_S stSnsSp2308Obj;
extern ISP_SNS_OBJ_S stSnsSc3336Obj;
extern ISP_SNS_OBJ_S stSnsSoiF37PObj;
extern ISP_SNS_OBJ_S stSnsOs02g10Obj;
extern ISP_SNS_OBJ_S stSnsPs6210Obj;
extern ISP_SNS_OBJ_S stSnsGc4663Obj;
extern ISP_SNS_OBJ_S stSnsSc8238Obj;


#define CMOS_CHECK_POINTER(ptr)\
    do {\
        if (ptr == XMEDIA_NULL)\
        {\
            ISP_TRACE(MODULE_DBG_ERR, "Null Pointer!\n");\
            return ERR_CODE_ISP_NULL_PTR;\
        }\
    }while(0)

#define CMOS_CHECK_POINTER_VOID(ptr)\
    do {\
        if (ptr == XMEDIA_NULL)\
        {\
            ISP_TRACE(MODULE_DBG_ERR, "Null Pointer!\n");\
            return;\
        }\
    }while(0)

#define SENSOR_FREE(ptr)\
    do{\
        if (ptr != XMEDIA_NULL)\
        {\
            free(ptr);\
            ptr = XMEDIA_NULL;\
        }\
    } while (0)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_SNS_CTRL_H__ */
