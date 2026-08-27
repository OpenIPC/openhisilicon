/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : user_osal_lib.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/

#ifndef __USER_OSAL_LIB_H__
#define __USER_OSAL_LIB_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "hi_osal.h"
#include "hi_types.h"
#include "hi_debug.h"
#include "drv_cipher_kapi.h"
#include "hi_unf_cipher.h"
#include "sys_cipher.h"

/** @}*/  /** <!-- ==== Structure Definition end ====*/

/*! \****************************** API Declaration *****************************/
/*! \addtogroup    osal lib */
/** @{ */  /** <!--[osal]*/

#ifndef HI_PRINT
#define HI_PRINT                    printf
#endif

#define crypto_malloc(x)            malloc(x)
#define crypto_free(x)              free(x)

#define crypto_mutex                pthread_mutex_t
#define crypto_mutex_init(x)        (hi_void)pthread_mutex_init(x, NULL)
#define crypto_mutex_lock(x)        (hi_void)pthread_mutex_lock(x)
#define crypto_mutex_unlock(x)      (hi_void)pthread_mutex_unlock(x)
#define crypto_mutex_destroy(x)     pthread_mutex_destroy(x)

#define crypto_open(a, b, c)        open(a, b, c)
#define crypto_close(x)             close(x)

extern hi_s32 cipher_dev_fd;
#define CRYPTO_IOCTL(cmd, argp)     ioctl(cipher_dev_fd, cmd, argp)

void print_string(const hi_s8 *name, hi_u8 *string, hi_u32 size);

/* Implementation that should never be optimized out by the compiler */
void crypto_zeroize( hi_void *buf, hi_u32 len );

/**< allow modules to modify, default value is HI_ID_STB, the general module id*/
#define LOG_D_MODULE_ID             HI_ID_CIPHER
#define LOG_D_FUNCTRACE             (0)
#define LOG_D_UNFTRACE              (0)

/**< allow modules to define internel error code, from 0x1000*/
#define LOG_ERRCODE_DEF(errid)      (hi_u32)(((LOG_D_MODULE_ID) << 16)  | (errid))

/**< General Error Code, All modules can extend according to the rule */
#define HI_LOG_ERR_MEM              LOG_ERRCODE_DEF(0x0001)      /**< Memory Operation Error */
#define HI_LOG_ERR_SEM              LOG_ERRCODE_DEF(0x0002)      /**< Semaphore Operation Error */
#define HI_LOG_ERR_FILE             LOG_ERRCODE_DEF(0x0003)      /**< File Operation Error */
#define HI_LOG_ERR_LOCK             LOG_ERRCODE_DEF(0x0004)      /**< Lock Operation Error */
#define HI_LOG_ERR_PARAM            LOG_ERRCODE_DEF(0x0005)      /**< Invalid Parameter */
#define HI_LOG_ERR_TIMER            LOG_ERRCODE_DEF(0x0006)      /**< Timer error */
#define HI_LOG_ERR_THREAD           LOG_ERRCODE_DEF(0x0007)      /**< Thread Operation Error */
#define HI_LOG_ERR_TIMEOUT          LOG_ERRCODE_DEF(0x0008)      /**< Time Out Error */
#define HI_LOG_ERR_DEVICE           LOG_ERRCODE_DEF(0x0009)      /**< Device Operation Error */
#define HI_LOG_ERR_STATUS           LOG_ERRCODE_DEF(0x0010)      /**< Status Error */
#define HI_LOG_ERR_IOCTRL           LOG_ERRCODE_DEF(0x0011)      /**< IO Operation Error */
#define HI_LOG_ERR_INUSE            LOG_ERRCODE_DEF(0x0012)      /**< In use */
#define HI_LOG_ERR_EXIST            LOG_ERRCODE_DEF(0x0013)      /**< Have exist */
#define HI_LOG_ERR_NOEXIST          LOG_ERRCODE_DEF(0x0014)      /**< no exist */
#define HI_LOG_ERR_UNSUPPORTED      LOG_ERRCODE_DEF(0x0015)      /**< Unsupported */
#define HI_LOG_ERR_UNAVAILABLE      LOG_ERRCODE_DEF(0x0016)      /**< Unavailable */
#define HI_LOG_ERR_UNINITED         LOG_ERRCODE_DEF(0x0017)      /**< Uninited */
#define HI_LOG_ERR_DATABASE         LOG_ERRCODE_DEF(0x0018)      /**< Database Operation Error */
#define HI_LOG_ERR_OVERFLOW         LOG_ERRCODE_DEF(0x0019)      /**< Overflow */
#define HI_LOG_ERR_EXTERNAL         LOG_ERRCODE_DEF(0x0020)      /**< External Error */
#define HI_LOG_ERR_UNKNOWNED        LOG_ERRCODE_DEF(0x0021)      /**< Unknow Error */
#define HI_LOG_ERR_FLASH            LOG_ERRCODE_DEF(0x0022)      /**< Flash Operation Error*/
#define HI_LOG_ERR_ILLEGAL_IMAGE    LOG_ERRCODE_DEF(0x0023)      /**< Illegal Image */
#define HI_LOG_ERR_ILLEGAL_UUID     LOG_ERRCODE_DEF(0x0023)      /**< Illegal UUID */
#define HI_LOG_ERR_NOPERMISSION     LOG_ERRCODE_DEF(0x0023)      /**< No Permission */

#define HI_LOG_FATAL(fmt...) \
    do{ \
        printf("[FATAL-HI_CIPHER]:%s[%d]:",(hi_u8*)__FUNCTION__,__LINE__); \
        printf(fmt); \
    }while(0)
#define HI_LOG_ERROR(fmt...) \
    do{ \
        printf("[ERROR-HI_CIPHER]:%s[%d]:",(hi_u8*)__FUNCTION__,__LINE__); \
        printf(fmt); \
    }while(0)

#define HI_LOG_WARN(fmt...)
#define HI_LOG_INFO(fmt...)
#define HI_LOG_DEBUG(fmt...)

/**< Function trace log, strictly prohibited to expand */
#define HI_LOG_PRINT_FUNC_WAR(func, err_code)  HI_LOG_WARN("Call %s return [0x%08X]\n", #func, err_code);
#define HI_LOG_PRINT_FUNC_ERR(func, err_code)  HI_LOG_ERROR("Call %s return [0x%08X]\n", #func, err_code);
#define HI_LOG_PRINT_ERR_CODE(err_code)        HI_LOG_ERROR("Error Code: [0x%08X]\n", err_code);

/**< Used for displaying more detailed error information */
#define HI_ERR_PRINT_S32(val)                HI_LOG_ERROR("%s = %d\n",        #val, val)
#define HI_ERR_PRINT_U32(val)                HI_LOG_ERROR("%s = %u\n",        #val, val)
#define HI_ERR_PRINT_S64(val)                HI_LOG_ERROR("%s = %lld\n",      #val, val)
#define HI_ERR_PRINT_U64(val)                HI_LOG_ERROR("%s = %llu\n",      #val, val)
#define HI_ERR_PRINT_H32(val)                HI_LOG_ERROR("%s = 0x%08X\n",    #val, val)
#define HI_ERR_PRINT_H64(val)                HI_LOG_ERROR("%s = 0x%016llX\n", #val, val)
#define HI_ERR_PRINT_STR(val)                HI_LOG_ERROR("%s = %s\n",        #val, val)
#define HI_ERR_PRINT_VOID(val)               HI_LOG_ERROR("%s = %p\n",        #val, val)
#define HI_ERR_PRINT_FLOAT(val)              HI_LOG_ERROR("%s = %f\n",        #val, val)
#define HI_ERR_PRINT_INFO(val)               HI_LOG_ERROR("<%s>\n", val)

/**< Used for displaying more detailed warning information */
#define HI_LOG_PRINT_S32(val)                HI_LOG_WARN("%s = %d\n",        #val, val)
#define HI_LOG_PRINT_U32(val)                HI_LOG_WARN("%s = %u\n",        #val, val)
#define HI_LOG_PRINT_S64(val)                HI_LOG_WARN("%s = %lld\n",      #val, val)
#define HI_LOG_PRINT_U64(val)                HI_LOG_WARN("%s = %llu\n",      #val, val)
#define HI_LOG_PRINT_H32(val)                HI_LOG_WARN("%s = 0x%08X\n",    #val, val)
#define HI_LOG_PRINT_H64(val)                HI_LOG_WARN("%s = 0x%016llX\n", #val, val)
#define HI_LOG_PRINT_STR(val)                HI_LOG_WARN("%s = %s\n",        #val, val)
#define HI_LOG_PRINT_VOID(val)               HI_LOG_WARN("%s = %p\n",        #val, val)
#define HI_LOG_PRINT_FLOAT(val)              HI_LOG_WARN("%s = %f\n",        #val, val)
#define HI_LOG_PRINT_INFO(val)               HI_LOG_WARN("<%s>\n", val)

/**< Only used for self debug, Can be expanded as needed */
#define HI_DBG_PRINT_S32(val)                HI_LOG_DEBUG("%s = %d\n",       #val, val)
#define HI_DBG_PRINT_U32(val)                HI_LOG_DEBUG("%s = %u\n",       #val, val)
#define HI_DBG_PRINT_S64(val)                HI_LOG_DEBUG("%s = %lld\n",     #val, val)
#define HI_DBG_PRINT_U64(val)                HI_LOG_DEBUG("%s = %llu\n",     #val, val)
#define HI_DBG_PRINT_H32(val)                HI_LOG_DEBUG("%s = 0x%08X\n",   #val, val)
#define HI_DBG_PRINT_H64(val)                HI_LOG_DEBUG("%s = 0x%016llX\n",#val, val)
#define HI_DBG_PRINT_STR(val)                HI_LOG_DEBUG("%s = %s\n",       #val, val)
#define HI_DBG_PRINT_VOID(val)               HI_LOG_DEBUG("%s = %p\n",       #val, val)
#define HI_DBG_PRINT_FLOAT(val)              HI_LOG_DEBUG("%s = %f\n",       #val, val)
#define HI_DBG_PRINT_INFO(val)               HI_LOG_DEBUG("<%s>\n", val)

#if (LOG_D_FUNCTRACE == 1) || (LOG_D_UNFTRACE == 1)
#define HI_UNF_FUNC_ENTER()                  HI_LOG_DEBUG(" >>>>>>[Enter]\n")    /**< Only used for unf interface */
#define HI_UNF_FUNC_EXIT()                   HI_LOG_DEBUG(" <<<<<<[Exit]\n")     /**< Only used for unf interface */
#else
#define HI_UNF_FUNC_ENTER()
#define HI_UNF_FUNC_EXIT()
#endif

#if LOG_D_FUNCTRACE
#define HI_LOG_FUNC_ENTER()                  HI_LOG_DEBUG(" =====>[Enter]\n")    /**< Used for all interface except unf */
#define HI_LOG_FUNC_EXIT()                   HI_LOG_DEBUG(" =====>[Exit]\n")     /**< Used for all interface except unf */
#else
#define HI_LOG_FUNC_ENTER()
#define HI_LOG_FUNC_EXIT()
#endif

#define HI_LOG_CHECK(fn_func)                                  \
    do                                                         \
    {                                                          \
        hi_s32 _i_err_code = fn_func;                          \
        if (_i_err_code != HI_SUCCESS)                         \
        {                                                      \
            HI_LOG_PRINT_FUNC_ERR(#fn_func, _i_err_code);      \
        }                                                      \
    } while (0)


#define HI_LOG_CHECK_PARAM(_val)                               \
    do                                                         \
    {                                                          \
        if (_val)                                              \
        {                                                      \
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_INVALID_PARA); \
            return HI_ERR_CIPHER_INVALID_PARA;                 \
        }                                                      \
    } while (0)


#define HI_LOG_CHECK_INITED(_init_count)                       \
    do                                                         \
    {                                                          \
        if (_init_count == 0)                                  \
        {                                                      \
            HI_LOG_PRINT_ERR_CODE(HI_ERR_CIPHER_NOT_INIT);     \
            return HI_ERR_CIPHER_NOT_INIT;                     \
        }                                                      \
    } while (0)


#define HI_LOG_PRINT_BLOCK(data, length)

#endif  /* End of #ifndef __USER_OSAL_LIB_H__*/
