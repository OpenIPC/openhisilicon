/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef CRYPTO_OSAL_ADAPT_H
#define CRYPTO_OSAL_ADAPT_H

#include "crypto_osal_lib.h"
#include "crypto_common_struct.h"

/* Timer. */
#ifndef crypto_msleep
#define crypto_msleep           osal_msleep
#endif

#ifndef crypto_udelay
#define crypto_udelay           osal_udelay
#endif

/* map. */
#ifndef crypto_ioremap_nocache
#define crypto_ioremap_nocache  osal_ioremap_nocache
#define crypto_iounmap          osal_iounmap
#endif

/* Mutex. */
#ifndef crypto_mutex_init
#define crypto_mutex            osal_mutex
#define crypto_mutex_init       osal_mutex_init
#define crypto_mutex_destroy    osal_mutex_destroy
#define crypto_mutex_lock       osal_mutex_lock
#define crypto_mutex_unlock     osal_mutex_unlock
#endif

/* Log. */
#ifndef crypto_print
#define crypto_print            osal_printk
#endif

#if !defined(CRYPTO_LOG_DEF)
#define crypto_log_fmt(LOG_LEVEL_LABEL, fmt, ...)   \
    crypto_print("[%s:%d]" LOG_LEVEL_LABEL ": " fmt, __func__, __LINE__, ##__VA_ARGS__)

#ifndef CRYPTO_LOG_LEVEL
#define CRYPTO_LOG_LEVEL 0
#endif
#if defined(CRYPTO_LOG_LEVEL) && (CRYPTO_LOG_LEVEL == 0)
#define crypto_log_err(fmt, ...) crypto_log_fmt("ERROR", fmt, ##__VA_ARGS__)
#define crypto_log_warn(fmt, ...)
#define crypto_log_notice(fmt, ...)
#define crypto_log_dbg(fmt, ...)
#define crypto_log_trace(fmt, ...)
#elif defined(CRYPTO_LOG_LEVEL) && (CRYPTO_LOG_LEVEL == 1)
#define crypto_log_err(fmt, ...) crypto_log_fmt("ERROR", fmt, ##__VA_ARGS__)
#define crypto_log_warn(fmt, ...) crypto_log_fmt("WARN:", fmt, ##__VA_ARGS__)
#define crypto_log_notice(fmt, ...)
#define crypto_log_dbg(fmt, ...)
#define crypto_log_trace(fmt, ...)
#elif defined(CRYPTO_LOG_LEVEL) && (CRYPTO_LOG_LEVEL == 2)
#define crypto_log_err(fmt, ...) crypto_log_fmt("ERROR", fmt, ##__VA_ARGS__)
#define crypto_log_warn(fmt, ...) crypto_log_fmt("WARN:", fmt, ##__VA_ARGS__)
#define crypto_log_notice(fmt, ...) crypto_log_fmt("NOTICE", fmt, ##__VA_ARGS__)
#define crypto_log_dbg(fmt, ...)
#define crypto_log_trace(fmt, ...)
#elif defined(CRYPTO_LOG_LEVEL) && (CRYPTO_LOG_LEVEL == 3)
#define crypto_log_err(fmt, ...) crypto_log_fmt("ERROR", fmt, ##__VA_ARGS__)
#define crypto_log_warn(fmt, ...) crypto_log_fmt("WARN:", fmt, ##__VA_ARGS__)
#define crypto_log_notice(fmt, ...) crypto_log_fmt("NOTICE", fmt, ##__VA_ARGS__)
#define crypto_log_dbg(fmt, ...) crypto_log_fmt("DBG", fmt, ##__VA_ARGS__)
#define crypto_log_trace(fmt, ...)
#elif defined(CRYPTO_LOG_LEVEL) && (CRYPTO_LOG_LEVEL == 4)
#define crypto_log_err(fmt, ...) crypto_log_fmt("ERROR", fmt, ##__VA_ARGS__)
#define crypto_log_warn(fmt, ...) crypto_log_fmt("WARN:", fmt, ##__VA_ARGS__)
#define crypto_log_notice(fmt, ...) crypto_log_fmt("NOTICE", fmt, ##__VA_ARGS__)
#define crypto_log_dbg(fmt, ...) crypto_log_fmt("DBG", fmt, ##__VA_ARGS__)
#define crypto_log_trace(fmt, ...) crypto_log_fmt("TRACE", fmt, ##__VA_ARGS__)
#endif

#endif

/* Malloc. */
#ifndef crypto_malloc
#define crypto_malloc(x)        (((x) > 0) ? osal_kmalloc((x), OSAL_GFP_KERNEL) : TD_NULL)
#define crypto_free(x)          {if (((x) != TD_NULL)) osal_kfree(x);}
#endif

/* align check */
#ifndef crypto_addr_align_check
#define crypto_addr_align_check(addr)       ((((addr) % 4) != 0) ? TD_FALSE : TD_TRUE)
#endif

/* ioctl_cmd. */
#ifndef CRYPTO_IOCTL_CMD_DEF
#define crypto_ioctl_cmd        osal_ioctl_cmd
#endif

/* copy from/to user. */
td_s32 crypto_copy_from_user(td_void *to, unsigned long to_len, const td_void *from, unsigned long from_len);
td_s32 crypto_copy_to_user(td_void  *to, unsigned long to_len, const td_void *from, unsigned long from_len);

/* crypto dump functions. */
#if defined(CONFIG_HEX_DATA_DUMP_ENABLE)
void crypto_dump_data(const char *name, const td_u8 *data, td_u32 data_len);
#else
#define crypto_dump_data(...)
#endif
void crypto_dump_phys_addr(const char *name, const td_phys_addr_t phys_addr, td_u32 data_len);

/* Check the Validation of but_attr. */
td_bool crypto_data_buf_check(const crypto_buf_attr *buf_attr, td_u32 length);

/* Crypto Owner Operation. */
#if !defined(crypto_owner) && (CRYPTO_ERROR_ENV != ERROR_ENV_NOOS)
#define crypto_owner                        td_u32
#define crypto_owner_dump(owner)            crypto_log_dbg("Owner's pid is 0x%x\n", owner)
static inline td_s32 crypto_get_owner(crypto_owner *owner)
{
    if (owner == TD_NULL) {
        return CRYPTO_FAILURE;
    }
    *owner = crypto_getpid();
    return CRYPTO_SUCCESS;
}
#endif

#endif