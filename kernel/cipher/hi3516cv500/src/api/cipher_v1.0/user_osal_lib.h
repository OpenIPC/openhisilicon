/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : head file for user osal lib of cipher.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

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

/* ***************************** API declaration for user osal lib of cipher **************************** */
#ifndef HI_PRINT
#define HI_PRINT                    printf
#endif

#ifndef MPI_STATIC
#define MPI_STATIC static inline
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

extern hi_s32 g_cipher_dev_fd;
#define crypto_ioctl(cmd, argp)     ioctl(g_cipher_dev_fd, cmd, argp)

void print_string(const hi_s8 *name, hi_u8 *string, hi_u32 size);

/* Implementation that should never be optimized out by the compiler */
void crypto_zeroize(hi_void *buf, hi_u32 len);

/* allow modules to modify, default value is HI_ID_STB, the general module id. */
#define LOG_D_MODULE_ID             HI_ID_CIPHER
#define LOG_D_FUNCTRACE             0
#define LOG_D_UNFTRACE              0

/* allow modules to define internel error code, from 0x1000. */
#define log_errcode_def(errid)      (hi_u32)(((LOG_D_MODULE_ID) << 16)  | (errid))

/* General Error Code, All modules can extend according to the rule */
#define HI_LOG_ERR_MEM              log_errcode_def(0x0001)      /* Memory Operation Error */
#define HI_LOG_ERR_SEM              log_errcode_def(0x0002)      /* Semaphore Operation Error */
#define HI_LOG_ERR_FILE             log_errcode_def(0x0003)      /* File Operation Error */
#define HI_LOG_ERR_LOCK             log_errcode_def(0x0004)      /* Lock Operation Error */
#define HI_LOG_ERR_PARAM            log_errcode_def(0x0005)      /* Invalid Parameter */
#define HI_LOG_ERR_TIMER            log_errcode_def(0x0006)      /* Timer error */
#define HI_LOG_ERR_THREAD           log_errcode_def(0x0007)      /* Thread Operation Error */
#define HI_LOG_ERR_TIMEOUT          log_errcode_def(0x0008)      /* Time Out Error */
#define HI_LOG_ERR_DEVICE           log_errcode_def(0x0009)      /* Device Operation Error */
#define HI_LOG_ERR_STATUS           log_errcode_def(0x0010)      /* Status Error */
#define HI_LOG_ERR_IOCTRL           log_errcode_def(0x0011)      /* IO Operation Error */
#define HI_LOG_ERR_INUSE            log_errcode_def(0x0012)      /* In use */
#define HI_LOG_ERR_EXIST            log_errcode_def(0x0013)      /* Have exist */
#define HI_LOG_ERR_NOEXIST          log_errcode_def(0x0014)      /* no exist */
#define HI_LOG_ERR_UNSUPPORTED      log_errcode_def(0x0015)      /* Unsupported */
#define HI_LOG_ERR_UNAVAILABLE      log_errcode_def(0x0016)      /* Unavailable */
#define HI_LOG_ERR_UNINITED         log_errcode_def(0x0017)      /* Uninited */
#define HI_LOG_ERR_DATABASE         log_errcode_def(0x0018)      /* Database Operation Error */
#define HI_LOG_ERR_OVERFLOW         log_errcode_def(0x0019)      /* Overflow */
#define HI_LOG_ERR_EXTERNAL         log_errcode_def(0x0020)      /* External Error */
#define HI_LOG_ERR_UNKNOWNED        log_errcode_def(0x0021)      /* Unknow Error */
#define HI_LOG_ERR_FLASH            log_errcode_def(0x0022)      /* Flash Operation Error. */
#define HI_LOG_ERR_ILLEGAL_IMAGE    log_errcode_def(0x0023)      /* Illegal Image */
#define HI_LOG_ERR_ILLEGAL_UUID     log_errcode_def(0x0023)      /* Illegal UUID */
#define HI_LOG_ERR_NOPERMISSION     log_errcode_def(0x0023)      /* No Permission */

#define hi_log_fatal(fmt...) \
    do{ \
        printf("[FATAL-HI_CIPHER]:%s[%d]:", __FUNCTION__, __LINE__); \
        printf(fmt); \
    } while (0)
#define hi_log_error(fmt...) \
    do{ \
        printf("[ERROR-HI_CIPHER]:%s[%d]:", __FUNCTION__, __LINE__); \
        printf(fmt); \
    } while (0)

#define hi_log_warn(fmt...)
#define hi_log_info(fmt...)
#define hi_log_debug(fmt...)

/* Function trace log, strictly prohibited to expand */
#define hi_log_print_func_war(func, err_code)  hi_log_warn("Call %s return [0x%08X]\n", #func, err_code);
#define hi_log_print_func_err(func, err_code)  hi_log_error("Call %s return [0x%08X]\n", #func, err_code);
#define hi_log_print_err_code(err_code)        hi_log_error("Error Code: [0x%08X]\n", err_code);

/* Used for displaying more detailed error information */
#define hi_err_print_s32(val)                hi_log_error("%s = %d\n",        #val, val)
#define hi_err_print_u32(val)                hi_log_error("%s = %u\n",        #val, val)
#define hi_err_print_s64(val)                hi_log_error("%s = %lld\n",      #val, val)
#define hi_err_print_u64(val)                hi_log_error("%s = %llu\n",      #val, val)
#define hi_err_print_h32(val)                hi_log_error("%s = 0x%08X\n",    #val, val)
#define hi_err_print_h64(val)                hi_log_error("%s = 0x%016llX\n", #val, val)
#define hi_err_print_str(val)                hi_log_error("%s = %s\n",        #val, val)
#define hi_err_print_void(val)               hi_log_error("%s = %p\n",        #val, val)
#define hi_err_print_float(val)              hi_log_error("%s = %f\n",        #val, val)
#define hi_err_print_info(val)               hi_log_error("<%s>\n", val)

/* Used for displaying more detailed warning information */
#define hi_log_print_s32(val)                hi_log_warn("%s = %d\n",        #val, val)
#define hi_log_print_u32(val)                hi_log_warn("%s = %u\n",        #val, val)
#define hi_log_print_s64(val)                hi_log_warn("%s = %lld\n",      #val, val)
#define hi_log_print_u64(val)                hi_log_warn("%s = %llu\n",      #val, val)
#define hi_log_print_h32(val)                hi_log_warn("%s = 0x%08X\n",    #val, val)
#define hi_log_print_h64(val)                hi_log_warn("%s = 0x%016llX\n", #val, val)
#define hi_log_print_str(val)                hi_log_warn("%s = %s\n",        #val, val)
#define hi_log_print_void(val)               hi_log_warn("%s = %p\n",        #val, val)
#define hi_log_print_float(val)              hi_log_warn("%s = %f\n",        #val, val)
#define hi_log_print_info(val)               hi_log_warn("<%s>\n", val)

/* Only used for self debug, Can be expanded as needed */
#define hi_dbg_print_s32(val)                hi_log_debug("%s = %d\n",       #val, val)
#define hi_dbg_print_u32(val)                hi_log_debug("%s = %u\n",       #val, val)
#define hi_dbg_print_s64(val)                hi_log_debug("%s = %lld\n",     #val, val)
#define hi_dbg_print_u64(val)                hi_log_debug("%s = %llu\n",     #val, val)
#define hi_dbg_print_h32(val)                hi_log_debug("%s = 0x%08X\n",   #val, val)
#define hi_dbg_print_h64(val)                hi_log_debug("%s = 0x%016llX\n", #val, val)
#define hi_dbg_print_str(val)                hi_log_debug("%s = %s\n",       #val, val)
#define hi_dbg_print_void(val)               hi_log_debug("%s = %p\n",       #val, val)
#define hi_dbg_print_float(val)              hi_log_debug("%s = %f\n",       #val, val)
#define hi_dbg_print_info(val)               hi_log_debug("<%s>\n", val)

#if (LOG_D_FUNCTRACE == 1) || (LOG_D_UNFTRACE == 1)
/* Only used for unf interface */
#define hi_unf_func_enter()                  hi_log_debug(" >>>>>>[Enter]\n")
#define hi_unf_func_exit()                   hi_log_debug(" <<<<<<[Exit]\n")
#else
#define hi_unf_func_enter()
#define hi_unf_func_exit()
#endif

#if LOG_D_FUNCTRACE
/* Used for all interface except unf */
#define hi_log_func_enter()                  hi_log_debug(" =====>[Enter]\n")
#define hi_log_func_exit()                   hi_log_debug(" =====>[Exit]\n")
#else
#define hi_log_func_enter()
#define hi_log_func_exit()
#endif

#define hi_log_check(fn_func)                                  \
    do {                                                       \
        hi_s32 _i_err_code = fn_func;                          \
        if (_i_err_code != HI_SUCCESS) {                       \
            hi_log_print_func_err(#fn_func, _i_err_code);      \
        }                                                      \
    } while (0)


#define hi_log_chk_param_return(_val)                           \
    do {                                                        \
        if (_val) {                                             \
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARAM); \
            return HI_ERR_CIPHER_INVALID_PARAM;                 \
        }                                                       \
    } while (0)


#define hi_log_chk_init_err_return(_init_count)                \
    do  {                                                      \
        if ((_init_count) == 0) {                              \
            hi_log_print_err_code(HI_ERR_CIPHER_NOT_INIT);     \
            return HI_ERR_CIPHER_NOT_INIT;                     \
        }                                                      \
    } while (0)

#ifdef CIPHER_DEBUG_TEST_SUPPORT
#define PRINT_HEX_BLOCK_LEN     16
void crypto_print_hex(const hi_char *name, hi_u8 *str, hi_u32 len);
#define HI_PRINT_HEX(name, str, len) crypto_print_hex((const hi_char *)name, (hi_u8 *)str, (hi_u32)len)
#else
#define HI_PRINT_HEX print_string
#endif

#endif  /* End of #ifndef __USER_OSAL_LIB_H__ */
