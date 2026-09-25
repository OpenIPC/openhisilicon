/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef __MODE_DEBUG_H__
#define __MODE_DEBUG_H__

#ifndef __KERNEL__
#include <stdio.h>
#include <stdarg.h>
#endif

#include "type.h"
#include "common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define _EX__FILE_LINE(fxx,lxx) "[File]:"fxx"\n[Line]:"#lxx"\n[Info]:"
#define EX__FILE_LINE(fxx,lxx) _EX__FILE_LINE(fxx,lxx)
#define __FILE_LINE__ EX__FILE_LINE(__FILE__, __LINE__)

#define MODULE_DBG_EMERG      0
#define MODULE_DBG_ALERT      1
#define MODULE_DBG_CRIT       2
#define MODULE_DBG_ERR        3
#define MODULE_DBG_WARN       4
#define MODULE_DBG_NOTICE     5
#define MODULE_DBG_INFO       6
#define MODULE_DBG_DEBUG      7

typedef struct LOG_LEVEL_CONF_S
{
    MOD_ID_E  enModId;
    XMEDIA_S32    s32Level;
    XMEDIA_CHAR   cModName[16];
} LOG_LEVEL_CONF_S;

#ifndef __KERNEL__

#define XMEDIA_PRINT printf

#ifdef CONFIG_LOG_TRACE_SUPPORT
    #define XMEDIA_ASSERT(expr)               \
    do{                                   \
        if (!(expr)) {                    \
            printf("\nASSERT at:\n"       \
                   "  >Function : %s\n"   \
                   "  >Line No. : %d\n"   \
                   "  >Condition: %s\n",  \
                   __FUNCTION__, __LINE__, #expr);\
            _exit(-1);\
        } \
    }while(0)

    #define MODULE_TRACE(level, enModId, fmt...) fprintf(stderr,##fmt)
#else
    #define XMEDIA_ASSERT(expr)           \
    do{                                   \
        if (!(expr)) {                    \
        } \
    }while(0)

    // for clear compile warning
    #define MODULE_TRACE(level, enModId, fmt...)     \
    do {                                             \
         if (0) {                                    \
            fprintf(stderr,##fmt);                   \
         }                                           \
    }while(0)

#endif

#else
#define XMEDIA_PRINT osal_printk

extern XMEDIA_S32 ChkLogLevel(XMEDIA_S32 s32Levle, MOD_ID_E enModId);

int LOG(XMEDIA_S32 level, MOD_ID_E enModId,const char *fmt, ...) __attribute__((format(printf,3,4)));

#ifdef CONFIG_LOG_TRACE_SUPPORT
    #define XMEDIA_ASSERT(expr)               \
    do{                                   \
        if (!(expr)) {                    \
            osal_panic("\nASSERT at:\n"   \
                  "  >Function : %s\n"    \
                  "  >Line No. : %d\n"    \
                  "  >Condition: %s\n",   \
                  __FUNCTION__, __LINE__, #expr);\
        } \
    }while(0)

    #define MODULE_TRACE LOG
#else
    #define XMEDIA_ASSERT(expr)           \
    do{                                   \
        if (!(expr)) {                    \
        } \
    }while(0)

    // for clear compile warning
    #define MODULE_TRACE(level, enModId, fmt...)     \
    do {                                             \
         if (0) {                                    \
            LOG(level,enModId,fmt);                  \
         }                                           \
    }while(0)
#endif


#endif


#define XMEDIA_PRINT_BLOCK(pu8Datablock, u32Length)  \
{  \
    XMEDIA_U32 u32ii = 0;  \
    XMEDIA_U8* pu8VirAddr = (XMEDIA_U8*)(pu8Datablock);  \
    if(XMEDIA_NULL != pu8VirAddr)\
    {\
        XMEDIA_PRINT("\n[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, #pu8Datablock);  \
        for (u32ii = 0; u32ii < (u32Length);)  \
        {  \
            XMEDIA_PRINT(" %02X", *pu8VirAddr);\
            pu8VirAddr++;\
            u32ii++;\
            if(0 == (u32ii % 16))XMEDIA_PRINT("\n");\
        }  \
        XMEDIA_PRINT("\n\n");\
    }\
    else\
    {\
        XMEDIA_PRINT("\n[Func]:%s [Line]:%d [Info]:pointer(%s) is null!\n", __FUNCTION__, __LINE__, #pu8Datablock);  \
    }\
}




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

