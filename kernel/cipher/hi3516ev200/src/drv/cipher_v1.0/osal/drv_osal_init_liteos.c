/******************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : ext_aead.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "drv_osal_lib.h"
#include "drv_symc.h"
#include "drv_hash.h"

/************************ Internal Structure Definition *********************/

extern hi_s32 crypto_ioctl(hi_u32 cmd, hi_void *argp);
extern hi_s32 crypto_entry(void);
extern hi_s32 crypto_exit(void);
extern hi_s32 crypto_recover_hdcp_key(void);

static osal_dev_t    *cipher_device;

/** @}*/  /** <!-- ==== Structure Definition end ====*/

/******************************* API Code *****************************/
/** \addtogroup      link*/
/** @{*/  /** <!-- [link]*/

/******* proc function begin ********/
#if (1 == HI_PROC_SUPPORT)
hi_s32 symc_proc_read(struct osal_proc_dir_entry *p)
{
    symc_chn_status *status = HI_NULL;
    int i = 0;
    hi_s32 ret = HI_SUCCESS;

    PROC_PRINT(p, "\n------------------------------------------"
               "CIPHER STATUS-------------------------------"
               "--------------------------------------------"
               "--------------------\n");
    PROC_PRINT(p, "Chnid   Status   Decrypt   Alg   Mode   KeyLen    "
               "Addr in/out      KeyFrom  INT-RAW in/out  INT-EN "
               "in/out INT_OCNTCFG    IVOUT\n");

    status = (symc_chn_status *)crypto_malloc(sizeof(symc_chn_status) * 8);
    if (status == HI_NULL) {
        PROC_PRINT(p, "crypto malloc for status buff failed!\n");
        return HI_FAILURE;
    }

    crypto_memset(status, sizeof(symc_chn_status) * 8, 0, sizeof(symc_chn_status) * 8);
    for (i = 0; i < 8; i++) {
        status[i].id = i;
    }

    ret = drv_symc_proc_status(status);
    if (ret != HI_SUCCESS) {
        PROC_PRINT(p, "CIPHER_ProcGetStatus failed!\n");
        crypto_free(status);
        status = HI_NULL;
        return HI_FAILURE;
    }

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        PROC_PRINT(p, " %d       %s      %d      %s  %s    %03d    %08x/%08x   "
                   " %s           %d/%d            %d/%d        %08x     %s\n",
                   i,
                   status[i].open,
                   status[i].decrypt,
                   status[i].alg,
                   status[i].mode,
                   status[i].klen,
                   status[i].inaddr,
                   status[i].outaddr,
                   status[i].ksrc,
                   status[i].inraw,
                   status[i].outraw,
                   status[i].inten,
                   status[i].outen,
                   status[i].outintcnt,
                   status[i].iv);
    }
    crypto_free(status);
    status = HI_NULL;

    return HI_SUCCESS;
}

static hi_void symc_proc_init(hi_void)
{
    osal_proc_entry_t *proc_entry = HI_NULL;

    proc_entry = osal_create_proc_entry(UMAP_DEVNAME_CIPHER, HI_NULL);
    if (proc_entry == HI_NULL) {
        HI_LOG_ERROR("cipher: can't create proc.\n");
        return;
    }
    proc_entry->read = symc_proc_read;
}

static hi_void symc_proc_deinit(hi_void)
{
    osal_remove_proc_entry(UMAP_DEVNAME_CIPHER, NULL);
}
#endif
/******* proc function end ********/

static long hi_cipher_ioctl(hi_u32 cmd,  unsigned long arg, void *private_data)
{
    return crypto_ioctl(cmd, (void *)arg);
}

static osal_fileops_t dev_cipher_fops = {
    .open             = HI_NULL,
    .unlocked_ioctl   = hi_cipher_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl     = hi_cipher_ioctl,
#endif
    .release          = HI_NULL,
};

int cipher_drv_mod_init(void)
{
    hi_s32 ret = HI_FAILURE;

    cipher_device = osal_createdev(UMAP_DEVNAME_CIPHER);
    cipher_device->fops = &dev_cipher_fops;
    cipher_device->minor = UMAP_MIN_MINOR_CIPHER;


    ret = osal_registerdevice(cipher_device);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("register CIPHER failed.\n");
        return ret;
    }

    ret = crypto_entry();
    if (ret != HI_SUCCESS) {
        goto error;
    }

    /******* proc function begin ********/
#if (1 == HI_PROC_SUPPORT)
    symc_proc_init();
#endif
    /******* proc function end ********/

#ifdef MODULE
    HI_PRINT("Load hi_cipher.ko success.\n");
#endif

    return HI_SUCCESS;

error:
    osal_deregisterdevice(cipher_device);
    osal_destroydev(cipher_device);

    return ret;
}

void cipher_drv_mod_exit(void)
{

    /******* proc function begin ********/
#if (1 == HI_PROC_SUPPORT)
    symc_proc_deinit();
#endif
    /******* proc function end ********/

    (hi_void)crypto_exit();
    osal_deregisterdevice(cipher_device);
    osal_destroydev(cipher_device);

    return ;
}

/** @}*/  /** <!-- ==== Structure Definition end ====*/
