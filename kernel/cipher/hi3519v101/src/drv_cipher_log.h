/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : cipher_log.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/05/28
  Description   : 
  History       :
  1.Date        : 2011/05/28
    Author      : j00169368
    Modification: Created file

******************************************************************************/
#ifndef __CIPHER_LOG_H__
#define __CIPHER_LOG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include "drv_osal_lib.h"
    
#define HI_LOG_LEVEL 3

#define HI_FATAL_CIPHER(fmt...) \
do{\
    if (HI_LOG_LEVEL <= 4)\
    {\
        HI_PRINT("CIPHER FATAL:");\
        HI_PRINT(fmt);\
    }\
}while(0)


#define HI_ERR_CIPHER(fmt...) \
do{\
    if (HI_LOG_LEVEL <= 3)\
    {\
        HI_PRINT("%s() ERROR:", __FUNCTION__);\
        HI_PRINT(fmt);\
    }\
}while(0)



#define HI_WARN_CIPHER(fmt...) \
do{\
    if (HI_LOG_LEVEL <= 2)\
    {\
        HI_PRINT("CIPHER WARNING:");\
        HI_PRINT(fmt);\
    }\
}while(0)

#define HI_INFO_CIPHER(fmt...) \
do{\
    if (HI_LOG_LEVEL <= 1)\
    {\
        HI_PRINT("CIPHER INFO:");\
        HI_PRINT(fmt);\
    }\
}while(0)


#define HI_DEBUG_CIPHER(fmt...) \
do{\
    if (HI_LOG_LEVEL <= 0)\
    {\
        HI_PRINT("CIPHER DEBUG:");\
        HI_PRINT(fmt);\
    }\
}while(0)

        /* Using samples:   HI_ASSERT(x>y); */
#define HI_ASSERT(expr)               \
do{                                   \
    if (HI_LOG_LEVEL <= 0)\
    {\
        if (!(expr)) {                    \
            HI_PRINT("\nASSERT failed at:\n"\
                   "  >File name: %s\n"   \
                   "  >Function : %s\n"   \
                   "  >Line No. : %d\n"   \
                   "  >Condition: %s\n",  \
                    __FILE__,__FUNCTION__, __LINE__, #expr);\
            /*_exit(-1);*/\
        } \
    }\
    else\
    {\
        /* do nothing*/\
    }\
}while(0)

#define HI_PRINT_HEX(name, str, len) \
{\
    HI_U32 _i = 0;\
    HI_PRINT("[%s]:\n", name);\
    for ( _i = 0 ; _i < (len); _i++ )\
    {\
        if( (_i % 16 == 0) && (_i != 0)) HI_PRINT("\n");\
        HI_PRINT("0x%02x ", *((str)+_i));\
    }\
    HI_PRINT("\n");\
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif

