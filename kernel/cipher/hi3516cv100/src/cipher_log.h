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


#define HI_LOG_LEVEL 3

#define HI_FATAL_CIPHER(fmt...) \
do{\
    if (HI_LOG_LEVEL <= 4)\
    {\
        printk("CIPHER FATAL:");\
        printk(fmt);\
    }\
}while(0)


#define HI_ERR_CIPHER(fmt...) \
do{\
    if (HI_LOG_LEVEL <= 3)\
    {\
        printk("CIPHER ERROR:");\
        printk(fmt);\
    }\
}while(0)


#define HI_WARN_CIPHER(fmt...) \
do{\
    if (HI_LOG_LEVEL <= 2)\
    {\
        printk("CIPHER WARNING:");\
        printk(fmt);\
    }\
}while(0)

#define HI_INFO_CIPHER(fmt...) \
do{\
    if (HI_LOG_LEVEL <= 1)\
    {\
        printk("CIPHER INFO:");\
        printk(fmt);\
    }\
}while(0)


#define HI_DEBUG_CIPHER(fmt...) \
do{\
    if (HI_LOG_LEVEL <= 0)\
    {\
        printk("CIPHER DEBUG:");\
        printk(fmt);\
    }\
}while(0)

        /* Using samples:   HI_ASSERT(x>y); */
#define HI_ASSERT(expr)               \
do{                                   \
    if (HI_LOG_LEVEL <= 0)\
    {\
        if (!(expr)) {                    \
            printk("\nASSERT failed at:\n"\
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



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif

