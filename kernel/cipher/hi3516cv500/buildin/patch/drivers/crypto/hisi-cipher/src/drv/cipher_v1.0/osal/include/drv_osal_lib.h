/*
 * Copyright (C), Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : head file for cipher drv osal lib.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#ifndef __DRV_OSAL_LIB_H__
#define __DRV_OSAL_LIB_H__

#include "drv_cipher_define.h"
#ifdef __HuaweiLite__
#include "drv_osal_lib_liteos.h"
#else
#include "drv_osal_lib_linux.h"
#endif

#define CIPHER_BUILDIN

#ifdef CIPHER_CHECK_MMZ_PHY
#include "osal_mmz.h"
extern hil_mmb_t *hil_mmb_getby_phys_2(unsigned long addr, unsigned long *out_offset);
#endif

#ifdef CIPHER_BUILDIN
extern int is_hicma_address(phys_addr_t phys, unsigned long size);
#endif

/* return uuid */
#define crypto_chk_owner_err_return(local) \
    do { \
        crypto_owner owner;\
        crypto_get_owner(&owner); \
        if (memcmp(&owner, local, sizeof(owner)) != 0) { \
            hi_log_error("return user uuid failed\n"); \
            hi_log_print_err_code(HI_ERR_CIPHER_ILLEGAL_UUID);\
            return HI_ERR_CIPHER_ILLEGAL_UUID; \
        } \
    } while (0)

/* max length module name */
#define CRYPTO_MODULE_NAME_LEN             16

/* the max cipher hard channel count. */
#define CRYPTO_HARD_CHANNEL_MAX            0x08

/* serure mmz or not, not used */
#define SEC_MMZ                            0x00

/* Cipher cmd param buffer size for ioctrl. */
#define CRYPTO_CMD_PARAM_SIZE              256

#ifdef DISABLE_DEBUG_INFO
#define HI_PROC_SUPPORT                 0
#else
#define HI_PROC_SUPPORT                 1
#endif

/* struct channel
 * the context of hardware channel.
 */
typedef struct {
    /* the state of instance, open or closed. */
    hi_u32 open;

    /* the context of channel, which is defined by specific module. */
    hi_void *ctx;
} channel_context;

/* struct of crypto_mem. */
typedef struct {
    compat_addr dma_addr;      /* dam addr, may be mmz or smmu */
    compat_addr mmz_addr;      /* mmz addr, sometimes the smmu must maped from mmz */
    hi_void *dma_virt;         /* cpu virtual addr maped from dam addr */
    hi_u32 dma_size;           /* dma memory size */
    hi_void *user_buf;         /* buffer of user */
} crypto_mem;

/* cipher drv mode init. */
hi_s32 cipher_drv_mod_init(hi_void);

/* cipher drv mode deinit. */
hi_void cipher_drv_mod_exit(hi_void);

/* cipher check addr. */
hi_s32 cipher_check_mmz_phy_addr(hi_u64 phy_addr, hi_u64 length);

/* cipher crypto ioctl. */
hi_s32 crypto_ioctl(hi_u32 cmd, hi_void *argp);

/* cipher crypto entry for module init. */
hi_s32 crypto_entry(void);

/* cipher crypto entry for module exit. */
hi_s32 crypto_exit(void);

/* cipher crypto release. */
hi_s32 crypto_release(void);

/*
 * \brief  cipher get device.
 */
hi_void *cipher_get_device(hi_void);

/*
 * \brief  init dma memory.
 */
hi_void crypto_mem_init(hi_void);

/*
 * \brief  deinit dma memory.
 */
hi_void crypto_mem_deinit(hi_void);

/*
 * \brief  crypto cpuc flush dcache area.
 */
hi_void crypto_cpuc_flush_dcache_area(hi_void *kvir, hi_u32 length);

/*
 * \brief  allocate and map a dma memory.
 * \param[in] mem  The struct of crypto_mem.
 * \param[in] size The size of mem.
 * \param[in] name The name of mem.
 * \return         HI_SUCCESS if successful, or HI_BASE_ERR_MALLOC_FAILED.
 */
hi_s32 crypto_mem_create(crypto_mem *mem, hi_u32 type, const char *name, hi_u32 size);

/*
 * \brief  destory and unmap a dma memory.
 * \param[in] mem  The struct of crypto_mem.
 * \return         0 if successful, or HI_BASE_ERR_UNMAP_FAILED.
 */
hi_s32 crypto_mem_destory(crypto_mem *mem);

/*
 * \brief  allocate and map a dma memory.
 * \param[in] mem  The struct of crypto_mem.
 * \param[in] size The size of mem.
 * \param[in] name The name of mem.
 * \return         HI_SUCCESS if successful, or HI_BASE_ERR_MALLOC_FAILED.
 */
hi_s32 hash_mem_create(crypto_mem *mem, hi_u32 type, const char *name, hi_u32 size);

/*
 * \brief  destory and unmap a dma memory.
 * \param[in] mem  The struct of crypto_mem.
 * \return         0 if successful, or HI_BASE_ERR_UNMAP_FAILED.
 */
hi_s32 hash_mem_destory(crypto_mem *mem);

/*
 * \brief  map a dma memory.
 * \param[in] mem  The struct of crypto_mem.
 * \param[in] dma_ddr The address of dma mem.
 * \param[in] dma_size The size of dma mem.
 * \return         HI_SUCCESS if successful, or HI_BASE_ERR_MAP_FAILED.
 */
hi_s32 crypto_mem_open(crypto_mem *mem, compat_addr dma_ddr, hi_u32 dma_size);

/*
 * \brief  unmap a dma memory.
 * \param[in] mem  The struct of crypto_mem.
 * \param[in] dma_ddr The address of dma mem.
 * \return         HI_SUCCESS if successful, or HI_BASE_ERR_UNMAP_FAILED.
 */
hi_s32 crypto_mem_close(crypto_mem *mem);

/*
 * \brief  attach a cpu buffer with dma memory.
 * \param[in] mem  The struct of crypto_mem.
 * \param[in] buffer The user's buffer.
 * \return         HI_SUCCESS if successful, or HI_FAILURE.
 */
hi_s32 crypto_mem_attach(crypto_mem *mem, hi_void *buffer);

/*
 * \brief  flush dma memory,
 * \param[in] mem The struct of crypto_mem.
 * \param[in] dma2user 1-data from dma to user, 0-data from user to dma.
 * \param[in] offset The offset of data to be flush.
 * \param[in] data_size The size of data to be flush.
 * \return         HI_SUCCESS if successful, or HI_FAILURE.
 */
hi_s32 crypto_mem_flush(crypto_mem *mem, hi_u32 dma2user, hi_u32 offset, hi_u32 data_size);

/*
 * \brief  get dma memory physical address
 * \param[in] mem The struct of crypto_mem.
 * \return         dma_addr if successful, or zero.
 */
hi_s32 crypto_mem_phys(crypto_mem *mem, compat_addr *dma_addr);

/*
 * \brief  get dma memory virtual address
 * \param[in] mem The struct of crypto_mem.
 * \return         dma_addr if successful, or zero.
 */
hi_void *crypto_mem_virt(crypto_mem *mem);

/*
 * \brief  check whether cpu is secure or not.
 * \retval secure cpu, true is returned otherwise false is returned.
 */
hi_u32 crypto_is_sec_cpu(hi_void);

/*
 * \brief  map the physics addr to cpu within the base table, contains the base addr and crg addr.
 * \retval    on success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 module_addr_map(hi_void);

/*
 * \brief  unmap the physics addr to cpu within the base table, contains the base addr and crg addr.
 * \retval    on success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 module_addr_unmap(hi_void);

/*
 * \brief  get secure cpu type.
 */
hi_u32 module_get_secure(hi_void);

/*
 * \brief  enable a module, open clock  and remove reset signal.
 * \param[in]  id The module id.
 * \retval    NA
 */
hi_void module_enable(module_id id);

/*
 * \brief  disable a module, close clock and set reset signal.
 * \param[in] id The module id.
 * \retval    NA
 */
hi_void module_disable(module_id id);

/*
 * \brief  get attribute of module.
 * \param[in]  id The module id.
 * \param[out] int_valid enable interrupt or not.
 * \param[out] int_num interrupt number of module.
 * \param[out] name name of module.
 * \retval    on success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_void module_get_attr(module_id id, hi_u32 *int_valid, hi_u32 *int_num, const char **name);

/*
 * \brief  set irq number.
 * \param[in]  id The module id.
 * \param[in]  irq irq numbert.
 * \retval    NA.
 */
hi_void module_set_irq(module_id id, hi_u32 irq);

/*
 * \brief  read a register.
 * \param[in]  id The module id.
 * \param[in]  offset The module id.
 * \retval    the value of register
 */
hi_u32 module_reg_read(module_id id, hi_u32 offset);

/*
 * \brief  hex to string.
 * \param[in]  buf The string buffer.
 * \param[in]  val The value of hex.
 * \retval    NA
 */
hi_void hex2str(char buf[MUL_VAL_2], hi_u32 buf_len, hi_u8 val);

/*
 * \brief  Implementation that should never be optimized out by the compiler
 * \param[in]  buf The string buffer.
 * \param[in]  the length of the buf.
 * \retval    NA
 */
hi_void crypto_zeroize(hi_void *buf, hi_u32 len);

/*
 * \brief  write a register.
 * \param[in]  id The module id.
 * \retval    NA
 */
hi_void module_reg_write(module_id id, hi_u32 offset, hi_u32 val);

/* cipher module read and write a register */
#define symc_read(offset)               module_reg_read(CRYPTO_MODULE_ID_SYMC, offset)
#define symc_write(offset, val)         module_reg_write(CRYPTO_MODULE_ID_SYMC, offset, val)

/* hash module read and write a register */
#define hash_read(offset)               module_reg_read(CRYPTO_MODULE_ID_HASH, offset)
#define hash_write(offset, val)         module_reg_write(CRYPTO_MODULE_ID_HASH, offset, val)

/* rsa module read and write a register */
#define ifep_rsa_read(offset)           module_reg_read(CRYPTO_MODULE_ID_IFEP_RSA, offset)
#define ifep_rsa_write(offset, val)     module_reg_write(CRYPTO_MODULE_ID_IFEP_RSA, offset, val)

/* trng module read and write a register */
#define trng_read(offset)               module_reg_read(CRYPTO_MODULE_ID_TRNG, offset)
#define trng_write(offset, val)         module_reg_write(CRYPTO_MODULE_ID_TRNG, offset, val)

/* sm2 module read and write a register */
#define sm2_read(offset)                module_reg_read(CRYPTO_MODULE_ID_SM2, offset)
#define sm2_write(offset, val)          module_reg_write(CRYPTO_MODULE_ID_SM2, offset, val)

/* smmu module read and write a register */
#define smmu_read(offset)               module_reg_read(CRYPTO_MODULE_ID_SMMU, offset)
#define smmu_write(offset, val)         module_reg_write(CRYPTO_MODULE_ID_SMMU, offset, val)

/*
 * \brief  Initialize the channel list.
 * \param[in]  ctx The context of channel.
 * \param[in]  num The channel numbers, max is 32.
 * \param[in]  ctx_size The size of context.
 * \retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 crypto_channel_init(channel_context *ctx, hi_u32 num, hi_u32 ctx_size);

/*
 * \brief  Deinitialize the channel list.
 * \param[in]  ctx The context of channel.
 * \param[in]  num The channel numbers, max is 32.
 * \retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 crypto_channel_deinit(channel_context *ctx, hi_u32 num);

/*
 * \brief  allocate a channel.
 * \param[in]  ctx The context of channel.
 * \param[in]  num The channel numbers, max is 32.
 * \param[in]  mask Mask whick channel allowed be alloc, max is 32.
 * \param[out] id The id of channel.
 * \retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 crypto_channel_alloc(channel_context *ctx, hi_u32 num, hi_u32 mask, hi_u32 *id);

/*
 * \brief  free a channel.
 * \param[in]  ctx The context of channel.
 * \param[in]  num The channel numbers, max is 32.
 * \param[in] id The id of channel.
 * \retval    on success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_void crypto_channel_free(channel_context *ctx, hi_u32 num, hi_u32 id);

/*
 * \brief  get the private data of hard channel.
 * \param[in] ctx The context of channel.
 * \param[in] num The channel numbers, max is 32.
 * \param[in] id The id of channel.
 * \retval    on success, the address of context is returned.  On error, NULL is returned..
 */
hi_void *crypto_channel_get_context(channel_context *ctx, hi_u32 num, hi_u32 id);

/*
 * \brief  get the rang.
 * \retval    random number.
 */
hi_u32 get_rand(hi_void);

hi_void smmu_get_table_addr(hi_u64 *rdaddr, hi_u64 *wraddr, hi_u64 *table);

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

/* Function trace log, strictly prohibited to expand */
#define hi_log_print_func_war(Func, ErrCode)  hi_log_warn("Call %s return [0x%08X]\n", #Func, (unsigned int)(ErrCode));
#define hi_log_print_func_err(Func, ErrCode)  hi_log_error("Call %s return [0x%08X]\n", #Func, (unsigned int)(ErrCode));
#define hi_log_print_err_code(ErrCode)        hi_log_error("Error Code: [0x%08X]\n", (unsigned int)(ErrCode));

/* Used for displaying more detailed error information */
#define hi_err_print_s32(val)     hi_log_error("%s = %d\n",        #val, (val))
#define hi_err_print_u32(val)     hi_log_error("%s = %u\n",        #val, (val))
#define hi_err_print_s64(val)     hi_log_error("%s = %lld\n",      #val, (val))
#define hi_err_print_u64(val)     hi_log_error("%s = %llu\n",      #val, (val))
#define hi_err_print_h32(val)     hi_log_error("%s = 0x%08X\n",    #val, (val))
#define hi_err_print_h64(val)     hi_log_error("%s = 0x%016llX\n", #val, (val))
#define hi_err_print_str(val)     hi_log_error("%s = %s\n",        #val, (val))
#define hi_err_print_void(val)    hi_log_error("%s = %p\n",        #val, (val))
#define hi_err_print_float(val)   hi_log_error("%s = %f\n",        #val, (val))
#define hi_err_print_info(val)    hi_log_error("<%s>\n", (val))

/* Used for displaying more detailed warning information */
#define hi_log_print_s32(val)     hi_log_warn("%s = %d\n",        #val, (val))
#define hi_log_print_u32(val)     hi_log_warn("%s = %u\n",        #val, (val))
#define hi_log_print_s64(val)     hi_log_warn("%s = %lld\n",      #val, (val))
#define hi_log_print_u64(val)     hi_log_warn("%s = %llu\n",      #val, (val))
#define hi_log_print_h32(val)     hi_log_warn("%s = 0x%08X\n",    #val, (val))
#define hi_log_print_h64(val)     hi_log_warn("%s = 0x%016llX\n", #val, (val))
#define hi_log_print_str(val)     hi_log_warn("%s = %s\n",        #val, (val))
#define hi_log_print_void(val)    hi_log_warn("%s = %p\n",        #val, (val))
#define hi_log_print_float(val)   hi_log_warn("%s = %f\n",        #val, (val))
#define hi_log_print_info(val)    hi_log_warn("<%s>\n", (val))

/* Only used for self debug, Can be expanded as needed */
#define hi_dbg_print_s32(val)     hi_log_debug("%s = %d\n",       #val, (val))
#define hi_dbg_print_u32(val)     hi_log_debug("%s = %u\n",       #val, (val))
#define hi_dbg_print_s64(val)     hi_log_debug("%s = %lld\n",     #val, (val))
#define hi_dbg_print_u64(val)     hi_log_debug("%s = %llu\n",     #val, (val))
#define hi_dbg_print_h32(val)     hi_log_debug("%s = 0x%08X\n",   #val, (val))
#define hi_dbg_print_h64(val)     hi_log_debug("%s = 0x%016llX\n", #val, (val))
#define hi_dbg_print_str(val)     hi_log_debug("%s = %s\n",       #val, (val))
#define hi_dbg_print_void(val)    hi_log_debug("%s = %p\n",       #val, (val))
#define hi_dbg_print_float(val)   hi_log_debug("%s = %f\n",       #val, (val))
#define hi_dbg_print_info(val)    hi_log_debug("<%s>\n", (val))

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

#define hi_log_check(fn_func)                                 \
    do {                                                      \
        hi_s32 _ierr_code = fn_func;                          \
        if (_ierr_code != HI_SUCCESS) {                       \
            hi_log_print_func_err(#fn_func, _ierr_code);      \
        }                                                     \
    } while (0)


#define hi_log_chk_param_return(_val)                         \
    do {                                                      \
        if (_val) {                                           \
            hi_log_print_err_code(HI_ERR_CIPHER_INVALID_PARA);\
            return HI_ERR_CIPHER_INVALID_PARA;                \
        }                                                     \
    } while (0)


#define hi_log_chk_init_err_return(_init_count)               \
    do {                                                      \
        if ((_init_count) == 0) {                             \
            hi_log_print_err_code(HI_ERR_CIPHER_NOT_INIT);    \
            return HI_ERR_CIPHER_NOT_INIT;                    \
        }                                                     \
    } while (0)

#ifdef CIPHER_DEBUG_TEST_SUPPORT

#define PRINT_HEX_BLOCK_LEN     16
void crypto_print_hex(const hi_char *name, hi_u8 *str, hi_u32 len);
#define HI_PRINT_HEX(name, str, len) crypto_print_hex((const hi_char *)name, (hi_u8 *)str, (hi_u32)len)
#else
#define HI_PRINT_HEX print_string
#endif

#endif  /* End of #ifndef __DRV_OSAL_LIB_H__ */
