/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for linux drv osal init of cipher.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

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

#ifdef KAPI_TEST_SUPPORT
#include "kapi_symc_test.h"
#endif

#define CIPHER_PROC_NAME    "driver/hi_cipher"

/* ****************************** API Code **************************** */
#if (1 == HI_PROC_SUPPORT)
hi_s32 symc_proc_read(struct seq_file *p, hi_void *v)
{
    symc_chn_status *status = HI_NULL;
    int i;
    hi_s32 ret;

    if (p == HI_NULL || v == HI_NULL) {
        hi_log_error("Invalid point\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    seq_printf(p, "\n------------------------------------------"
               "CIPHER STATUS-------------------------------"
               "--------------------------------------------"
               "--------------------\n");
    seq_printf(p, "Chnid   Status   Decrypt   Alg   Mode   KeyLen    "
               "Addr in/out      KeyFrom  INT-RAW in/out  INT-EN "
               "in/out INT_OCNTCFG    IVOUT\n");

    status = (symc_chn_status *)crypto_calloc(CRYPTO_HARD_CHANNEL_MAX, sizeof(symc_chn_status));
    if (status == HI_NULL) {
        return HI_ERR_CIPHER_FAILED_MEM;
    }

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        status[i].id = i;
    }

    ret = drv_symc_proc_status(status);
    if (ret != HI_SUCCESS) {
        seq_printf(p, "CIPHER_ProcGetStatus failed!\n");
        crypto_free(status);
        status = NULL;
        return ret;
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
    kapi_test_main();
#endif

    crypto_free(status);
    status = NULL;

    return HI_SUCCESS;
}

static int symc_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, symc_proc_read, NULL);
}

static const struct file_operations g_drv_cipher_proc_fops = {
    .owner      = THIS_MODULE,
    .open       = symc_proc_open,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release,
};

static hi_void symc_proc_init(hi_void)
{
    struct proc_dir_entry *proc_entry = HI_NULL;

    proc_entry = proc_create(CIPHER_PROC_NAME, 0, NULL, &g_drv_cipher_proc_fops);
    if (proc_entry == NULL) {
        hi_log_error("cipher: can't create %s.\n", CIPHER_PROC_NAME);
    }
}

static hi_void symc_proc_deinit(hi_void)
{
    remove_proc_entry(CIPHER_PROC_NAME, NULL);
}
#endif /* ****** proc function end ******* */

static hi_s32 hi_cipher_open(struct inode *inode, struct file *file)
{
    if ((!capable(CAP_SYS_RAWIO)) || (!capable(CAP_SYS_ADMIN))) {
        return -EPERM;
    }
    return HI_SUCCESS;
}

static long hi_cipher_ioctl(struct file *ffile, unsigned int cmd, unsigned long arg)
{
    long ret;
    hi_u8 cmd_param[CRYPTO_CMD_PARAM_SIZE] = {0};

    if ((ffile == HI_NULL) || (ffile->f_path.dentry == HI_NULL) || (arg == 0x00)) {
        hi_log_error("Invalid cmd param size!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (_IOC_SIZE(cmd) > sizeof(cmd_param)) {
        hi_log_error("Invalid cmd param size %d!\n", _IOC_SIZE(cmd));
        return HI_ERR_CIPHER_INVALID_PARAM;
    }

    if (((crypto_ioc_dir(cmd) == CRYPTO_IOC_W) || (crypto_ioc_dir(cmd) == CRYPTO_IOC_RW)) && (_IOC_SIZE(cmd) != 0)) {
        ret = copy_from_user((hi_void *)cmd_param, (void __user *)(hi_uintptr_t)arg, _IOC_SIZE(cmd));
        if (ret != HI_SUCCESS) {
            hi_log_error("copy data from user failed, ret:0x%lx!\n", ret);
            return HI_ERR_CIPHER_INVALID_PARAM;
        }
    }

    ret = crypto_ioctl(cmd, (hi_void *)cmd_param);
    if (ret != HI_SUCCESS) {
        hi_log_error("copy data from user failed, ret:0x%lx!\n", ret);
        return ret;
    }

    if (((crypto_ioc_dir(cmd) == CRYPTO_IOC_R) || (crypto_ioc_dir(cmd) == CRYPTO_IOC_RW)) && (_IOC_SIZE(cmd) != 0)) {
        ret = copy_to_user((hi_void __user *)(hi_uintptr_t)arg, (const hi_void *)cmd_param, _IOC_SIZE(cmd));
        if (ret != HI_SUCCESS) {
            hi_log_error("copy data to user fail, ret:0x%lx!\n", ret);
            return HI_ERR_CIPHER_INVALID_PARAM;
        }
    }
    return HI_SUCCESS;
}

static hi_s32 hi_cipher_release(struct inode *inode, struct file *file)
{
    return crypto_release();
}

static struct file_operations g_dev_cipher_fops = {
    .owner            = THIS_MODULE,
    .open             = hi_cipher_open,
    .unlocked_ioctl   = hi_cipher_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl     = hi_cipher_ioctl,
#endif
    .release          = hi_cipher_release,
};

static struct miscdevice g_cipher_dev = {
    .minor      = MISC_DYNAMIC_MINOR,
    .name       = UMAP_DEVNAME_CIPHER,
    .fops       = &g_dev_cipher_fops,
};

hi_void *cipher_get_device(hi_void)
{
    return (hi_void *)g_cipher_dev.this_device;
}

hi_s32 cipher_drv_mod_init(hi_void)
{
    hi_s32 ret;

    if (misc_register(&g_cipher_dev)) {
        hi_log_error("ERROR: could not register cipher devices\n");
        return HI_ERR_CIPHER_ILLEGAL_DATA;
    }

    /* dma data structure shall be initialised before being used in Kernel 4.9
     * or else call dma_set_coherent_mask/dma_alloc_coherent will return error
     */
    of_dma_configure(g_cipher_dev.this_device, g_cipher_dev.this_device->of_node);

    ret = crypto_entry();
    if (ret != HI_SUCCESS) {
        misc_deregister(&g_cipher_dev);
        return ret;
    }

    /* ****** proc function begin ******* */
#if (1 == HI_PROC_SUPPORT)
    symc_proc_init();
#endif /* ****** proc function end ******* */

#ifdef MODULE
    HI_PRINT("Load hi_cipher.ko success.\n");
#endif

    return HI_SUCCESS;
}

void cipher_drv_mod_exit(void)
{
    /* ****** proc function begin ******* */
#if (1 == HI_PROC_SUPPORT)
    symc_proc_deinit();
#endif /* ****** proc function end ******* */
    misc_deregister(&g_cipher_dev);
    crypto_exit();
}
