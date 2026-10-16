/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name             :     adec_intf.c
  Version               :     Initial Draft
  Author                :     Hisilicon multimedia software group
  Created               :     2006/01/23
  Last Modified         :
  Description           :
  Function List         :    So Much ....
  History               :
  1.Date                :     2006/01/23
    Modification        :    Created file

******************************************************************************/
#include <linux/proc_fs.h>
//#include <linux/config.h>
#include <linux/module.h>
#include <linux/signal.h>
#include <linux/spinlock.h>
#include <linux/personality.h>
#include <linux/ptrace.h>
#include <linux/kallsyms.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/seq_file.h>

#include <asm/atomic.h>
#include <asm/cacheflush.h>
#include <asm/io.h>
//#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <asm/traps.h>

#include <linux/miscdevice.h>
#include <linux/of_platform.h>
#include "hi_type.h"

#include "priv_cipher.h"
#include "drv_cipher.h"
#include "cipher_log.h"




#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */



/* initialize mutex variable g_CipherMutexKernel to 1 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
DECLARE_MUTEX(g_CipherMutexKernel);
#else
DEFINE_SEMAPHORE(g_CipherMutexKernel);
#endif
CIPHER_OSR_CHN_S g_stCipherOsrChn[CIPHER_SOFT_CHAN_NUM];

spinlock_t  cipher_lock;

extern unsigned int isr_irq;

/*****************************************************************************
 Prototype    : DRV_CIPHER_UserCommCallBack
 Description  :
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :
  History        :
  1.Date         :
    Author       :
    Modification :
*****************************************************************************/
static HI_VOID DRV_CIPHER_UserCommCallBack(HI_U32 arg)
{
    //HI_INFO_CIPHER("arg=%#x.\n", arg);

    g_stCipherOsrChn[arg].g_bDataDone = HI_TRUE;
    wake_up_interruptible(&(g_stCipherOsrChn[arg].cipher_wait_queue));

    return ;
}


/*****************************************************************************
 Prototype    : DRV_CIPHER_Release
 Description  :
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :
  History        :
  1.Date         : 2006/1/18
    Modification : Created function
*****************************************************************************/
static HI_S32 DRV_CIPHER_Release(struct inode * inode, struct file * file)
{
    HI_U32 i;

    for (i = 0; i < CIPHER_SOFT_CHAN_NUM; i++)
    {
        if (g_stCipherOsrChn[i].pWichFile == file)
        {
            DRV_Cipher_CloseChn(i);
            g_stCipherOsrChn[i].g_bSoftChnOpen = HI_FALSE;
            g_stCipherOsrChn[i].pWichFile = NULL;
        }
    }

    return 0;
}

/*****************************************************************************
 Prototype    : DRV_CIPHER_Open
 Description  :
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :
  History        :
  1.Date         : 2006/1/18
    Modification : Created function
*****************************************************************************/
static HI_S32 DRV_CIPHER_Open(struct inode * inode, struct file * file)
{
	if (!capable(CAP_SYS_RAWIO) || !capable(CAP_SYS_ADMIN))
	{
		return -EPERM;
	}

    return 0;
}



/*****************************************************************************
 Prototype    : DRV_CIPHER_Ioctl
 Description  :
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :
  History        :
  1.Date         : 2006/1/18
    Modification : Created function
*****************************************************************************/
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
static HI_S32 DRV_CIPHER_Ioctl(struct inode *inode,
                            struct file *file,
                            HI_U32 cmd,
                            unsigned long arg)
#else
static long DRV_CIPHER_Ioctl(struct file *file, HI_U32 cmd, unsigned long arg)
#endif
{
    void __user*    argp = (void __user*)arg;
    HI_S32  s32Ret = 0;

    HI_HANDLE  Cipherchn = 0;   /* cipher handle */
    HI_U32 softChnId = 0;       /* soft channel ID */

    s32Ret = down_interruptible(&g_CipherMutexKernel);

    switch (cmd)
    {
        case HI_CIPHER_CreateHandle:
        {
            HI_U32 i;

            spin_lock(&cipher_lock);

            for (i = 1; i < 8; i++)
            {
                if (HI_FALSE == g_stCipherOsrChn[i].g_bSoftChnOpen)
                {
                    break;
                }
            }

            if (i >= 8)
            {
                spin_unlock(&cipher_lock);
                s32Ret = HI_ERR_CIPHER_FAILED_GETHANDLE;
                HI_ERR_CIPHER("No more cipher chan left.\n");
                break;
            }
            else /* get a free chn */
            {
                softChnId = i;
                g_stCipherOsrChn[softChnId].g_bSoftChnOpen = HI_TRUE;
            }

            spin_unlock(&cipher_lock);

            s32Ret = DRV_Cipher_OpenChn(softChnId);
            if (HI_SUCCESS != s32Ret)
            {
                break;
            }

            Cipherchn = HI_HANDLE_MAKEHANDLE(HI_ID_CIPHER, 0, softChnId);

            HI_INFO_CIPHER("the softChnId and the handle are %d %#x\n", softChnId, Cipherchn);

            if (copy_to_user(argp, &Cipherchn, sizeof(Cipherchn)))
            {
                s32Ret = HI_FAILURE;

                break;
            }

            g_stCipherOsrChn[softChnId].pWichFile = file;
            s32Ret = HI_SUCCESS;

            break;
        }

        case HI_CIPHER_DestroyHandle:
        {
            Cipherchn = arg;

            CIPHER_CHECK_HANDLE(Cipherchn, s32Ret);

            softChnId = HI_HANDLE_GET_CHNID(Cipherchn);

            if ((HI_FALSE == g_stCipherOsrChn[softChnId].g_bSoftChnOpen)
                ||  (g_stCipherOsrChn[softChnId].pWichFile != file ))
            {
                s32Ret = HI_SUCCESS; /* success on re-Destroy */
                break;
            }

            g_stCipherOsrChn[softChnId].g_bSoftChnOpen = HI_FALSE;
            g_stCipherOsrChn[softChnId].pWichFile = NULL;
            s32Ret = DRV_Cipher_CloseChn(softChnId);

            break;
        }

        case HI_CIPHER_ConfigHandle:
        {
            CIPHER_Config_CTRL  CIConfig;

            if (copy_from_user(&CIConfig, argp, sizeof(CIConfig)))
            {
                HI_ERR_CIPHER("copy data from user fail!\n");
                s32Ret = HI_FAILURE;
                break;
            }

            CIPHER_CHECK_HANDLE(CIConfig.CIHandle, s32Ret);
            softChnId = HI_HANDLE_GET_CHNID(CIConfig.CIHandle);

            if ((HI_FALSE == g_stCipherOsrChn[softChnId].g_bSoftChnOpen)
                ||  (g_stCipherOsrChn[softChnId].pWichFile != file ))
            {
                s32Ret = HI_ERR_CIPHER_INVALID_HANDLE;
                break;
            }

            s32Ret = DRV_Cipher_ConfigChn(softChnId, &CIConfig.CIpstCtrl, DRV_CIPHER_UserCommCallBack);

            break;
        }

        case HI_CIPHER_Encrypt:
        {

            CIPHER_DATA_S       CIData;

            if (copy_from_user(&CIData, argp, sizeof(CIData)))
            {
                HI_ERR_CIPHER("copy data from user fail!\n");
                s32Ret = HI_FAILURE;

                break;
            }

            CIPHER_CHECK_HANDLE(CIData.CIHandle, s32Ret);
            softChnId = HI_HANDLE_GET_CHNID(CIData.CIHandle);

            if ((HI_FALSE == g_stCipherOsrChn[softChnId].g_bSoftChnOpen)
                ||  (g_stCipherOsrChn[softChnId].pWichFile != file ))
            {
                s32Ret = HI_ERR_CIPHER_INVALID_HANDLE;

                break;
            }

            s32Ret = HI_DRV_CIPHER_Encrypt(&CIData);
            break;
        }

        case HI_CIPHER_Decrypt:
        {
            CIPHER_DATA_S       CIData;

            if (copy_from_user(&CIData, argp, sizeof(CIData)))
            {
                HI_ERR_CIPHER("copy data from user fail!\n");
                s32Ret = HI_FAILURE;
                break;
            }

            CIPHER_CHECK_HANDLE(CIData.CIHandle, s32Ret);
            softChnId = HI_HANDLE_GET_CHNID(CIData.CIHandle);

            if ((HI_FALSE == g_stCipherOsrChn[softChnId].g_bSoftChnOpen)
                ||  (g_stCipherOsrChn[softChnId].pWichFile != file ))
            {
                s32Ret = HI_ERR_CIPHER_INVALID_HANDLE;
                break;
            }

            s32Ret = HI_DRV_CIPHER_Decrypt(&CIData);
            break;
        }

        default:
            s32Ret = HI_FAILURE;
            HI_ERR_CIPHER("Error: Inappropriate ioctl for device. cmd=%d\n", cmd);

            break;
    }

    up(&g_CipherMutexKernel);

    return s32Ret;
}


/** <* ref from linux/fs.h */
static struct file_operations DRV_CIPHER_Fops=
{
    .owner            = THIS_MODULE,
    .open             = DRV_CIPHER_Open,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
    .ioctl            = DRV_CIPHER_Ioctl,
#else
    .unlocked_ioctl   = DRV_CIPHER_Ioctl,
#endif
    .release          = DRV_CIPHER_Release,
};


static struct miscdevice cipher_dev =
{
	.minor		= MISC_DYNAMIC_MINOR,
    .name		= "cipher",
    .fops  		= &DRV_CIPHER_Fops,
};

/*****************************************************************************
 Prototype    : DRV_CIPHER_ModExit
 Description  :
 Input        : None
 Output       : None
 Return Value :
 Calls        :
 Called By    :
  History        :
  1.Date         : 2006/1/18
    Modification : Created function
*****************************************************************************/
int hisir_init(void)
{
    HI_U32 i;
    HI_S32 ret;

    if (misc_register(&cipher_dev))
    {
        HI_ERR_CIPHER("ERROR: could not register cipher devices\n");
		return -1;
    }

    ret = DRV_Cipher_Init();
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("cipher_device_init fail \n");
        misc_deregister(&cipher_dev);
        return ret;
    }

    for (i = 0; i < CIPHER_SOFT_CHAN_NUM; i++)
    {
        g_stCipherOsrChn[i].g_bSoftChnOpen = HI_FALSE;
        g_stCipherOsrChn[i].g_bDataDone = HI_FALSE;
        g_stCipherOsrChn[i].pWichFile = NULL;
        init_waitqueue_head(&(g_stCipherOsrChn[i].cipher_wait_queue));
    }
    spin_lock_init(&cipher_lock);

    HI_PRINT_CIPHER("Load hi_cipher.ko success.\n");

    return HI_SUCCESS;
}

HI_VOID hisir_exit(HI_VOID)
{
    misc_deregister(&cipher_dev);
    DRV_Cipher_DeInit();
    printk("unload hi_cipher.ko success.\n");
    return ;
}



/***************************** Macro Definition ******************************/



/*************************** Structure Definition ****************************/



/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/

static int hi_isr_probe(struct platform_device *pdev)
{
    isr_irq = platform_get_irq(pdev, 0);
    if (isr_irq == 0)
    {
        dev_err(&pdev->dev, "cannot find isr IRQ%d. \n", isr_irq);
    }

    return hisir_init();
}

static int hi_isr_remove(struct platform_device *pdev)
{
    hisir_exit();
    return 0;
}

static const struct of_device_id hi_isr_match[] = {
    { .compatible = "hisilicon,hi_isr" },
    {},
};

static struct platform_driver hi_isr_driver = {
    .probe  = hi_isr_probe,
    .remove = hi_isr_remove,
    .driver =  { .name = "hi_isr",
                .of_match_table = hi_isr_match,
               },
};

module_platform_driver(hi_isr_driver);
MODULE_AUTHOR("Hisilicon MPP GRP");
MODULE_LICENSE("GPL");


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

