/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#include "osal.h"

#include "type.h"
#include "common.h"

#ifndef __MOD_EXT_H__
#define __MOD_EXT_H__

#define MAX_MPP_MODULES MOD_ID_BUTT

#define VERSION_MAGIC   20190906

#define MAX_MOD_NAME    16

typedef enum MOD_NOTICE_ID_E {
    MOD_NOTICE_STOP = 0x11,
} MOD_NOTICE_ID_E;

typedef enum MOD_STATE_E {
    MOD_STATE_FREE = 0x11,
    MOD_STATE_BUSY = 0X22,
} MOD_STATE_E;

typedef XMEDIA_S32 FN_MOD_Init(XMEDIA_VOID *);
typedef XMEDIA_VOID FN_MOD_Exit(XMEDIA_VOID);
typedef XMEDIA_VOID FN_MOD_Notify(MOD_NOTICE_ID_E enNoticeId);
typedef XMEDIA_VOID FN_MOD_QueryState(MOD_STATE_E *pstState);
typedef XMEDIA_U32 FN_MOD_VerChecker(XMEDIA_VOID);

typedef struct MPP_MODULE_S {
    struct osal_list_head list;

    XMEDIA_CHAR aModName[MAX_MOD_NAME];
    MOD_ID_E enModId;

    FN_MOD_Init *pfnInit;
    FN_MOD_Exit *pfnExit;
    FN_MOD_QueryState *pfnQueryState;
    FN_MOD_Notify *pfnNotify;
    FN_MOD_VerChecker *pfnVerChecker;

    XMEDIA_BOOL bInited;

    XMEDIA_VOID *pstExportFuncs;
    XMEDIA_VOID *pData;

    XMEDIA_CHAR *pVersion;
} UMAP_MODULE_S;

extern XMEDIA_CHAR *CMPI_GetModuleName(MOD_ID_E enModId);
extern UMAP_MODULE_S *CMPI_GetModuleById(MOD_ID_E enModId);
extern XMEDIA_VOID *CMPI_GetModuleFuncById(MOD_ID_E enModId);

extern XMEDIA_VOID CMPI_StopModules(XMEDIA_VOID);
extern XMEDIA_S32 CMPI_QueryModules(XMEDIA_VOID);
extern XMEDIA_S32 CMPI_InitModules(XMEDIA_VOID);
extern XMEDIA_VOID CMPI_ExitModules(XMEDIA_VOID);
extern XMEDIA_S32 CMPI_RegisterModule(UMAP_MODULE_S *pstModules);
extern XMEDIA_VOID CMPI_UnRegisterModule(MOD_ID_E enModId);

#define FUNC_ENTRY(type, id) ((type *)CMPI_GetModuleFuncById(id))
#define CHECK_FUNC_ENTRY(id) (CMPI_GetModuleFuncById(id) != NULL)
#define FUNC_ENTRY_NULL(id) (!CHECK_FUNC_ENTRY(id))

#endif /*  __MOD_EXT_H__ */

