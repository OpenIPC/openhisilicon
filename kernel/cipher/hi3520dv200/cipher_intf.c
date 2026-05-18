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
    Author              :     f47391
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
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <asm/traps.h>

#include <linux/miscdevice.h>

#include "hi_type.h"

#include "priv_cipher.h"
#include "drv_cipher.h"
#include "cipher_log.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiCIPHER_OSR_CHN_S
{
    HI_BOOL g_bSoftChnOpen; /* mark soft channel open or not*/
    HI_BOOL g_bDataDone;    /* mark the data done or not */
    wait_queue_head_t cipher_wait_queue; /* mutex method */
    struct file *pWichFile; /* which file need to operate */
}CIPHER_OSR_CHN_S;

/* initialize mutex variable g_CipherMutexKernel to 1 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
DECLARE_MUTEX(g_CipherMutexKernel);
#else
DEFINE_SEMAPHORE(g_CipherMutexKernel);
#endif

static CIPHER_OSR_CHN_S g_stCipherOsrChn[CIPHER_SOFT_CHAN_NUM];

spinlock_t  cipher_lock;


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
    Author       : vicent feng
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
    Author       : vicent feng
    Modification : Created function
*****************************************************************************/
static HI_S32 DRV_CIPHER_Open(struct inode * inode, struct file * file)
{
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
    Author       : vicent feng
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
    void __user    *argp = (void __user*)arg;
    HI_S32         Ret = 0;

    HI_HANDLE  Cipherchn = 0;   /* cipher handle */
    HI_U32 softChnId = 0;       /* soft channel ID */

    Ret = down_interruptible(&g_CipherMutexKernel);

    switch(cmd)
    {
    case HI_CIPHER_CreateHandle:
    {
        HI_U32 i;
        
        spin_lock(&cipher_lock);

        for(i=1;i<8;i++)
        {
            if (0 == g_stCipherOsrChn[i].g_bSoftChnOpen)
            {
                break;
            }
        }

        if (i >= 8)
        {
            spin_unlock(&cipher_lock);
            Ret = HI_ERR_CIPHER_FAILED_GETHANDLE;
            HI_ERR_CIPHER("No more cipher chan left.\n");
            break;
        }
        else /* get a free chn */
        {
            softChnId = i;
            g_stCipherOsrChn[softChnId].g_bSoftChnOpen = HI_TRUE;
        }

        spin_unlock(&cipher_lock);
        
        Cipherchn = (HI_U32)((HI_ID_CIPHER <<16)+softChnId);

        HI_INFO_CIPHER("the softChnId and the handle are %d %#x\n", softChnId, Cipherchn);

        Ret = DRV_Cipher_OpenChn(softChnId);
        if (HI_SUCCESS != Ret)
        {
            break;
        }

        if (copy_to_user(argp, &Cipherchn, sizeof(Cipherchn)))
        {
            Ret = HI_FAILURE;
            
            break;
        }

        g_stCipherOsrChn[softChnId].pWichFile = file;
        Ret = HI_SUCCESS;
        
        break;
    }

    case HI_CIPHER_DestroyHandle:
    {
        Cipherchn = arg;
        
        softChnId = Cipherchn&0x00ff;

        CIPHER_CHECK_HANDLE(softChnId,Ret);

        if ((HI_FALSE == g_stCipherOsrChn[softChnId].g_bSoftChnOpen)
            ||  (g_stCipherOsrChn[softChnId].pWichFile != file ))
        {
            Ret = HI_SUCCESS; /* success on re-Destroy */
            break;
        }

        g_stCipherOsrChn[softChnId].g_bSoftChnOpen = HI_FALSE;
        g_stCipherOsrChn[softChnId].pWichFile = NULL;
        Ret = DRV_Cipher_CloseChn(softChnId);
        
        break;
    }

    case HI_CIPHER_ConfigHandle:
    {
        CIPHER_Config_CTRL  CIConfig;

        if (copy_from_user(&CIConfig, argp, sizeof(CIConfig)))
        {
            HI_ERR_CIPHER("copy data from user fail!\n");
            Ret = HI_FAILURE;
            break;
        }

        softChnId = CIConfig.CIHandle & 0xffff;

        CIPHER_CHECK_HANDLE(softChnId,Ret);

        if ((HI_FALSE == g_stCipherOsrChn[softChnId].g_bSoftChnOpen)
            ||  (g_stCipherOsrChn[softChnId].pWichFile != file ))
        {
            Ret = HI_ERR_CIPHER_INVALID_HANDLE;
            break;
        }

        Ret = DRV_Cipher_ConfigChn(softChnId, &CIConfig.CIpstCtrl, DRV_CIPHER_UserCommCallBack);
        
        break;
    }

    case HI_CIPHER_Encrypt:
    {
        CIPHER_DATA_S       CIData;
        CIPHER_TASK_S       pCITask;

        if (copy_from_user(&CIData, argp, sizeof(CIData)))
        {
            HI_ERR_CIPHER("copy data from user fail!\n");
            Ret = HI_FAILURE;
            
            break;
        }

        softChnId = CIData.CIHandle&0xffff;

        CIPHER_CHECK_HANDLE(softChnId,Ret);
        
        if ((HI_FALSE == g_stCipherOsrChn[softChnId].g_bSoftChnOpen)
            ||  (g_stCipherOsrChn[softChnId].pWichFile != file ))
        {
            Ret = HI_ERR_CIPHER_INVALID_HANDLE;
            
            break;
        }

        pCITask.stData2Process.u32src = CIData.ScrPhyAddr;
        pCITask.stData2Process.u32dest = CIData.DestPhyAddr;
        pCITask.stData2Process.u32length = CIData.ByteLength;
        pCITask.stData2Process.bDecrypt = HI_FALSE;
        pCITask.u32CallBackArg = softChnId;

        HI_INFO_CIPHER("Start to Encrypt, chnNum = %#x!\n", softChnId);

        g_stCipherOsrChn[softChnId].g_bDataDone = HI_FALSE;

        up(&g_CipherMutexKernel); /* ??? */
        Ret = DRV_Cipher_CreatTask(softChnId,&pCITask, NULL, NULL);
        if (HI_SUCCESS != Ret)
        {
            return Ret;
        }

        //printk("\nencrypt wait channel %d queue event\n", softChnId);

        if (0 == wait_event_interruptible_timeout(g_stCipherOsrChn[softChnId].cipher_wait_queue,
                    g_stCipherOsrChn[softChnId].g_bDataDone != HI_FALSE, 200))
        {
            HI_ERR_CIPHER("Encrypt time out! \n");
            return HI_FAILURE;
        }

        if(down_interruptible(&g_CipherMutexKernel))
        {
            return -ERESTARTSYS;
        }
        
        HI_INFO_CIPHER("Encrypt OK, chnNum = %#x!\n", softChnId);
        Ret = HI_SUCCESS;
        break;
    }

    case HI_CIPHER_Decrypt:
    {
        CIPHER_DATA_S       CIData;
        CIPHER_TASK_S       pCITask;

        if (copy_from_user(&CIData, argp, sizeof(CIData)))
        {
            HI_ERR_CIPHER("copy data from user fail!\n");
            Ret = HI_FAILURE;
            break;
        }

        softChnId = CIData.CIHandle & 0xffff;
        
        CIPHER_CHECK_HANDLE(softChnId,Ret);

        if ((HI_FALSE == g_stCipherOsrChn[softChnId].g_bSoftChnOpen)
            ||  (g_stCipherOsrChn[softChnId].pWichFile != file ))
        {
            Ret = HI_ERR_CIPHER_INVALID_HANDLE;
            break;
        }

        pCITask.stData2Process.u32src=CIData.ScrPhyAddr;
        pCITask.stData2Process.u32dest=CIData.DestPhyAddr;
        pCITask.stData2Process.u32length=CIData.ByteLength;
        pCITask.stData2Process.bDecrypt=HI_TRUE;

        pCITask.u32CallBackArg=softChnId;

        HI_INFO_CIPHER("Start to Decrypt, chnNum = %#x!\n", softChnId);

        g_stCipherOsrChn[softChnId].g_bDataDone = HI_FALSE;

        up(&g_CipherMutexKernel); /* ??? */
        Ret = DRV_Cipher_CreatTask(softChnId,&pCITask,NULL,NULL);
        if (HI_SUCCESS != Ret)
        {
            return Ret;
        }

        //printk("wait channel %d queue event\n", softChnId);

        if (0== wait_event_interruptible_timeout(g_stCipherOsrChn[softChnId].cipher_wait_queue,
                    g_stCipherOsrChn[softChnId].g_bDataDone != HI_FALSE, 200))
        {
            HI_ERR_CIPHER("Decrypt time out \n");
            return HI_FAILURE;
        }

        if(down_interruptible(&g_CipherMutexKernel))
        {
            return -ERESTARTSYS;
        }

        HI_INFO_CIPHER("Decrypt OK, chnNum = %#x!\n", softChnId);
        Ret = HI_SUCCESS;
        break;
    }

    default:
        Ret = HI_FAILURE;
        HI_ERR_CIPHER("Error: Inappropriate ioctl for device. cmd=%d\n", cmd);
        
        break;
    }

    up(&g_CipherMutexKernel);
    
    return Ret;
}


/** <* ref from linux/fs.h @by g00182102 */
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
    Author       : vicent feng
    Modification : Created function
*****************************************************************************/
static int __init DRV_CIPHER_ModInit(void)
{
    HI_U32 i;
    HI_S32 ret;
    
    if (misc_register(&cipher_dev))
    {
        printk("ERROR: could not register cipher devices\n");
		return -1;
    }

    ret = DRV_Cipher_Init();
    if (HI_SUCCESS != ret)
    {
        printk("cipher_device_init fail \n");
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
    
    printk("Load hi_cipher.ko success.\n");

    return HI_SUCCESS;
}

HI_VOID __exit DRV_CIPHER_ModExit(HI_VOID)
{
    misc_deregister(&cipher_dev);
    DRV_Cipher_DeInit();
    return ;
}



/***************************** Macro Definition ******************************/



/*************************** Structure Definition ****************************/



/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/


module_init(DRV_CIPHER_ModInit);
module_exit(DRV_CIPHER_ModExit);

MODULE_AUTHOR("Hi3720 MPP GRP");
MODULE_LICENSE("GPL");
//MODULE_VERSION(MPP_VERSION);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

