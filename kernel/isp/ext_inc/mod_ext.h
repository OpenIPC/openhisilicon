/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
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

typedef GK_S32 FN_MOD_Init(GK_VOID *);
typedef GK_VOID FN_MOD_Exit(GK_VOID);
typedef GK_VOID FN_MOD_Notify(MOD_NOTICE_ID_E enNoticeId);
typedef GK_VOID FN_MOD_QueryState(MOD_STATE_E *pstState);
typedef GK_U32 FN_MOD_VerChecker(GK_VOID);

typedef struct MPP_MODULE_S {
    struct osal_list_head list;

    GK_CHAR aModName[MAX_MOD_NAME];
    MOD_ID_E enModId;

    FN_MOD_Init *pfnInit;
    FN_MOD_Exit *pfnExit;
    FN_MOD_QueryState *pfnQueryState;
    FN_MOD_Notify *pfnNotify;
    FN_MOD_VerChecker *pfnVerChecker;

    GK_BOOL bInited;

    GK_VOID *pstExportFuncs;
    GK_VOID *pData;

    GK_CHAR *pVersion;
} UMAP_MODULE_S;

extern GK_CHAR *CMPI_GetModuleName(MOD_ID_E enModId);
extern UMAP_MODULE_S *CMPI_GetModuleById(MOD_ID_E enModId);
extern GK_VOID *CMPI_GetModuleFuncById(MOD_ID_E enModId);

extern GK_VOID CMPI_StopModules(GK_VOID);
extern GK_S32 CMPI_QueryModules(GK_VOID);
extern GK_S32 CMPI_InitModules(GK_VOID);
extern GK_VOID CMPI_ExitModules(GK_VOID);
extern GK_S32 CMPI_RegisterModule(UMAP_MODULE_S *pstModules);
extern GK_VOID CMPI_UnRegisterModule(MOD_ID_E enModId);

#define FUNC_ENTRY(type, id) ((type *)CMPI_GetModuleFuncById(id))
#define CHECK_FUNC_ENTRY(id) (CMPI_GetModuleFuncById(id) != NULL)
#define FUNC_ENTRY_NULL(id) (!CHECK_FUNC_ENTRY(id))

#endif /*  __MOD_EXT_H__ */

