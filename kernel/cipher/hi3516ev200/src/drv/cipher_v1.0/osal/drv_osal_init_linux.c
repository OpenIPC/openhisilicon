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
#include <linux/proc_fs.h>
#include <linux/module.h>
#include <linux/signal.h>
#include <linux/spinlock.h>
#include <linux/personality.h>
#include <linux/ptrace.h>
#include <linux/kallsyms.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/seq_file.h>
#include <linux/version.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <asm/atomic.h>
#include <asm/cacheflush.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <asm/traps.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/of_device.h>
#include "drv_osal_lib.h"
#include "drv_symc.h"
#include "drv_hash.h"

/************************ Internal Structure Definition *********************/

#define CIPHER_PROC_NAME    "driver/hi_cipher"

extern hi_s32 crypto_ioctl(hi_u32 cmd, hi_void *argp);
extern hi_s32 crypto_entry(void);
extern hi_s32 crypto_exit(void);
hi_s32 crypto_release(void);

/** @}*/  /** <!-- ==== Structure Definition end ====*/

/******************************* API Code *****************************/
/** \addtogroup      link*/
/** @{*/  /** <!-- [link]*/

/******* proc function begin ********/
#if (1 == HI_PROC_SUPPORT)
hi_s32 symc_proc_read(struct seq_file *p, hi_void *v)
{
    symc_chn_status *status = HI_NULL;
    int i = 0;
    hi_s32 ret = HI_SUCCESS;

    seq_printf(p, "\n------------------------------------------"
               "CIPHER STATUS-------------------------------"
               "--------------------------------------------"
               "--------------------\n");
    seq_printf(p, "Chnid   Status   Decrypt   Alg   Mode   KeyLen    "
               "Addr in/out      KeyFrom  INT-RAW in/out  INT-EN "
               "in/out INT_OCNTCFG    IVOUT\n");

    status = (symc_chn_status *)crypto_malloc(8 * sizeof(symc_chn_status));
    if (status == HI_NULL) {
        return HI_FAILURE;
    }

    crypto_memset(status, 8 * sizeof(symc_chn_status), 0, 8 * sizeof(symc_chn_status));
    for (i = 0; i < 8; i++) {
        status[i].id = i;
    }

    ret = drv_symc_proc_status(status);
    if (ret != HI_SUCCESS) {
        seq_printf(p, "CIPHER_ProcGetStatus failed!\n");
        crypto_free(status);
        status = NULL;
        return HI_FAILURE;
    }

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        seq_printf(p, " %d       %s      %d      %s  %s    %03d    %08x/%08x   "
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

#ifdef KAPI_TEST_SUPPORT
    {
        extern hi_s32 kapi_test_main(void);

        kapi_test_main();
    }
#endif

    crypto_free(status);
    status = NULL;

    return HI_SUCCESS;
}

static int symc_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, symc_proc_read, NULL);
}

static const struct file_operations DRV_CIPHER_ProcFops = {
    .owner		= THIS_MODULE,
    .open		= symc_proc_open,
    .read		= seq_read,
    .llseek		= seq_lseek,
    .release	= single_release,
};

static hi_void symc_proc_init(hi_void)
{
    struct proc_dir_entry *proc_entry = HI_NULL;

    proc_entry = proc_create(CIPHER_PROC_NAME, 0, NULL, &DRV_CIPHER_ProcFops);
    if (proc_entry == NULL) {
        HI_LOG_ERROR("cipher: can't create %s.\n", CIPHER_PROC_NAME);
    }
}

static hi_void symc_proc_deinit(hi_void)
{
    remove_proc_entry(CIPHER_PROC_NAME, NULL);
}
#endif
/******* proc function end ********/


hi_s32 _crypto_ioctl(struct inode *inode, struct file *file,
                     hi_u32 cmd, hi_void *arg)
{
    return crypto_ioctl(cmd, arg);
}

static hi_s32 hi_cipher_open(struct inode *inode, struct file *file)
{
    if ((!capable(CAP_SYS_RAWIO)) || (!capable(CAP_SYS_ADMIN))) {
        return -EPERM;
    }
    return HI_SUCCESS;
}

static long hi_cipher_ioctl(struct file *ffile, unsigned int cmd, unsigned long arg)
{
    long ret = HI_SUCCESS;
    hi_u8 unCmdParam[256] = {0};

    if ((ffile == HI_NULL) || (ffile->f_path.dentry == HI_NULL) || (arg == 0x00)) {
        HI_LOG_ERROR("Invalid cmd param size!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (_IOC_SIZE(cmd) > sizeof(unCmdParam)) {
        HI_LOG_ERROR("Invalid cmd param size %d!\n", _IOC_SIZE(cmd));
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (((CRYPTO_IOC_DIR(cmd) == CRYPTO_IOC_W) || (CRYPTO_IOC_DIR(cmd) == CRYPTO_IOC_RW))
        && (_IOC_SIZE(cmd) != 0)) {
        ret = copy_from_user((hi_void *)unCmdParam, (void __user *)(HI_UINTPTR_T)arg, _IOC_SIZE(cmd));
        if (ret != HI_SUCCESS) {
            HI_LOG_ERROR("copy data from user failed, ret:0x%lx!\n", ret);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }

    ret = _crypto_ioctl(ffile->f_path.dentry->d_inode, ffile, cmd, (hi_void *)unCmdParam);
    if (ret != HI_SUCCESS) {
        HI_LOG_ERROR("copy data from user failed, ret:0x%lx!\n", ret);
        return ret;
    }

    if (((CRYPTO_IOC_DIR(cmd) == CRYPTO_IOC_R) || (CRYPTO_IOC_DIR(cmd) == CRYPTO_IOC_RW))
        && (_IOC_SIZE(cmd) != 0)) {
        ret = copy_to_user((void __user *)(HI_UINTPTR_T)arg, (const hi_void *)unCmdParam, _IOC_SIZE(cmd));
        if (ret != HI_SUCCESS) {
            HI_LOG_ERROR("copy data to user fail, ret:0x%lx!\n", ret);
            return HI_ERR_CIPHER_INVALID_PARA;
        }
    }
    return HI_SUCCESS;
}

static hi_s32 hi_cipher_release(struct inode *inode, struct file *file)
{
    return crypto_release();
}

static struct file_operations dev_cipher_fops = {
    .owner            = THIS_MODULE,
    .open             = hi_cipher_open,
    .unlocked_ioctl   = hi_cipher_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl     = hi_cipher_ioctl,
#endif
    .release          = hi_cipher_release,
};

static struct miscdevice cipher_dev = {
    .minor      = MISC_DYNAMIC_MINOR,
    .name       = UMAP_DEVNAME_CIPHER,
    .fops       = &dev_cipher_fops,
};

void *cipher_get_device(void)
{
    return (void *)cipher_dev.this_device;
}

int cipher_drv_mod_init(void)
{
    hi_s32 ret = HI_FAILURE;

    if (misc_register(&cipher_dev)) {
        HI_LOG_ERROR("ERROR: could not register cipher devices\n");
        return -1;
    }

    /* dma data structure shall be initialised before being used in Kernel 4.9
     * or else call dma_set_coherent_mask/dma_alloc_coherent will return error
     */
    of_dma_configure(cipher_dev.this_device, cipher_dev.this_device->of_node);

    ret = crypto_entry();
    if (ret != HI_SUCCESS) {
        misc_deregister(&cipher_dev);
        return HI_FAILURE;
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
}

void cipher_drv_mod_exit(void)
{
    /******* proc function begin ********/
#if (1 == HI_PROC_SUPPORT)
    symc_proc_deinit();
#endif
    /******* proc function end ********/
    misc_deregister(&cipher_dev);
    crypto_exit();

    return ;
}

/** @}*/  /** <!-- ==== Structure Definition end ====*/
