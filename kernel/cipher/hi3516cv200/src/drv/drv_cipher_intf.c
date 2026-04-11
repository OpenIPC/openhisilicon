/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_cipher_intf.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
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
#include <asm/atomic.h>
#include <asm/cacheflush.h>
#include <asm/io.h>
#include <linux/version.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0))
#include <asm/system.h>
#endif
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <asm/traps.h>
#include <linux/miscdevice.h>

#include "hi_type.h"
#include "hal_cipher.h"
#include "drv_cipher.h"
#include "drv_cipher_ioctl.h"
#include "drv_hash.h"
#include "drv_cipher_log.h"
#include "hi_drv_cipher.h"
#include "drv_efuse.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


extern HI_VOID DRV_CIPHER_UserCommCallBack(HI_U32 arg);

HI_U32 g_u32CipherBase = 0;

extern int cipher_irq;

HI_VOID HAL_CIPHER_ReadReg(HI_U32 addr, HI_U32 *pu32Val)
{
    if (HI_NULL == pu32Val)
    {
        return;
    }
    HI_REG_READ32(addr, *pu32Val);
    return;
}

HI_VOID HAL_CIPHER_WriteReg(HI_U32 addr, HI_U32 u32Val)
{
    HI_REG_WRITE32(addr, u32Val);
    return;
}


static HI_S32 CIPHER_ProcGetStatus(CIPHER_CHN_STATUS_S *pstCipherStatus)
{
#ifdef CFG_HI_CIPHER_MULTICIPHER_SUPPORT
	return DRV_CIPHER_ProcGetStatus(pstCipherStatus);
#else
    return HI_SUCCESS;
#endif
}

static int CIPHER_ProcRead(struct seq_file *p, HI_VOID *v)
{
	CIPHER_CHN_STATUS_S stCipherStatus[8];
	int i = 0;
	HI_S32 ret = HI_FAILURE;

    if (HI_NULL == p)
    {
        HI_ERR_CIPHER("Invalid point!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    seq_printf(p, "\n------------------------------------------CIPHER STATUS-----------------------------------------------------------------------------------------------\n");
    seq_printf(p, "Chnid   Status   Decrypt   Alg   Mode   KeyLen    Phy-Addr in/out      KeyFrom  INT-RAW in/out  INT-EN in/out INT_OCNTCFG\n");

	memset(&stCipherStatus, 0, sizeof(stCipherStatus));
	for(i = 0; i < 8; i++)
	{
		stCipherStatus[i].u32ChnId = i;
	}

	ret = CIPHER_ProcGetStatus((CIPHER_CHN_STATUS_S *)&stCipherStatus);
	if(HI_SUCCESS != ret)
	{
    	seq_printf(p, "CIPHER_ProcGetStatus failed!\n");
        return HI_FAILURE;
	}

	for(i = 0; i < 8; i++)
	{
	    seq_printf(p, " %d       %s      %d      %s  %s    %03d      %08x/%08x    %s           %d/%d            %d/%d        %08x\n",
						i,
						stCipherStatus[i].ps8Openstatus,
						stCipherStatus[i].bIsDecrypt,
						stCipherStatus[i].ps8Alg,
						stCipherStatus[i].ps8Mode,
						stCipherStatus[i].u32KeyLen,
						stCipherStatus[i].u32DataInAddr,
						stCipherStatus[i].u32DataOutAddr,
						stCipherStatus[i].ps8KeyFrom,
						stCipherStatus[i].bInINTRaw,
						stCipherStatus[i].bOutINTRaw,
						stCipherStatus[i].bInINTEn,
						stCipherStatus[i].bOutINTEn,
						stCipherStatus[i].u32OutINTCount);
	}

    return HI_SUCCESS;
}

HI_S32 CIPHER_ProcWrite(struct file * file, const char __user * buf, size_t count, loff_t *ppos)
{
    return count;
}

static int DRV_CIPHER_ProcOpen(struct inode* inode, struct file* file)
{
    return single_open(file, CIPHER_ProcRead, NULL);
}

static const struct file_operations DRV_CIPHER_ProcFops =
{
    .owner		= THIS_MODULE,
    .open		= DRV_CIPHER_ProcOpen,
    .read		= seq_read,
    .llseek		= seq_lseek,
    .release	= single_release,
};

HI_S32 CIPHER_Ioctl(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *argp)
{
    HI_S32 ret = HI_FAILURE;

    if (HI_NULL == argp)
    {
        HI_ERR_CIPHER("Invalid point!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    HI_INFO_CIPHER("start cmd, MOD_ID=0x%02X, NR=0x%02x, SIZE=0x%02x!\n",
                _IOC_TYPE (cmd), _IOC_NR (cmd), _IOC_SIZE(cmd));

    switch(cmd)
    {
#ifdef CFG_HI_CIPHER_MULTICIPHER_SUPPORT
	    case CMD_CIPHER_CREATEHANDLE:
		{
	        CIPHER_HANDLE_S *pstCIHandle = (CIPHER_HANDLE_S *)argp;

			ret = HI_DRV_CIPHER_CreateHandle(pstCIHandle, (HI_U32)file);

	        break;
	    }

	    case CMD_CIPHER_DESTROYHANDLE:
	    {
	        HI_HANDLE hCipherChn = *(HI_HANDLE *)argp;

			ret = HI_DRV_CIPHER_DestroyHandle(hCipherChn);
	        break;
	    }
	    case CMD_CIPHER_CONFIGHANDLE:
	    {
	        CIPHER_Config_CTRL stCIConfig = *(CIPHER_Config_CTRL *)argp;

	        ret = HI_DRV_CIPHER_ConfigChn(stCIConfig.CIHandle, &stCIConfig.CIpstCtrl);
	        break;
	    }
	    case CMD_CIPHER_ENCRYPT:
	    {
	        CIPHER_DATA_S *pstCIData = (CIPHER_DATA_S *)argp;

			ret = HI_DRV_CIPHER_Encrypt(pstCIData);
			break;
	    }
	    case CMD_CIPHER_DECRYPT:
	    {
	        CIPHER_DATA_S *pstCIData = (CIPHER_DATA_S *)argp;

			ret = HI_DRV_CIPHER_Decrypt(pstCIData);
			break;
	    }
	    case CMD_CIPHER_ENCRYPTMULTI:
	    {
	        CIPHER_DATA_S *pstCIData = (CIPHER_DATA_S *)argp;

			ret = HI_DRV_CIPHER_EncryptMulti(pstCIData);
			break;
	    }
	    case CMD_CIPHER_DECRYPTMULTI:
	    {
	        CIPHER_DATA_S *pstCIData = (CIPHER_DATA_S *)argp;

			ret = HI_DRV_CIPHER_DecryptMulti(pstCIData);
	        break;
	    }
        case HI_CIPHER_ENCRYPTMULTI_EX:
	    {
	        CIPHER_MUTIL_EX_DATA_S *pstCIDataEx = (CIPHER_MUTIL_EX_DATA_S *)argp;

            ret = HI_DRV_CIPHER_EncryptMultiEx(pstCIDataEx);
			break;
	    }
	    case HI_CIPHER_DECRYPTMULTI_EX:
	    {
	        CIPHER_MUTIL_EX_DATA_S *pstCIDataEx = (CIPHER_MUTIL_EX_DATA_S *)argp;

            ret = HI_DRV_CIPHER_DecryptMultiEx(pstCIDataEx);
            break;
        }
#ifdef CIPHER_CCM_GCM_SUPPORT
        case CMD_CIPHER_GETTAG:
        {
	        CIPHER_TAG *pstCITag = (CIPHER_TAG *)argp;

			ret = HI_DRV_CIPHER_GetTag(pstCITag);
	        break;
	    }
#endif
#ifdef CFG_HI_CIPHER_GETHANDLECONFIG_SUPPORT
	    case CMD_CIPHER_GETHANDLECONFIG:
	    {
	        CIPHER_Config_CTRL *pstCIData = (CIPHER_Config_CTRL *)argp;

	        ret = HI_DRV_CIPHER_GetHandleConfig(pstCIData);
	        break;
	    }
#endif
#endif
#ifdef CFG_HI_CIPHER_RNG_SUPPORT
	    case CMD_CIPHER_GETRANDOMNUMBER:
	    {
	        CIPHER_RNG_S *pstRNG  = (CIPHER_RNG_S *)argp;
			ret = HI_DRV_CIPHER_GetRandomNumber(pstRNG);
	        break;
	    }
#endif
#ifdef CFG_HI_CIPHER_HASH_SUPPORT
	    case CMD_CIPHER_CALCHASHINIT:
	    {
	        CIPHER_HASH_DATA_S *pstCipherHashData = (CIPHER_HASH_DATA_S*)argp;
	        ret = HI_DRV_CIPHER_CalcHashInit(pstCipherHashData);
	        break;
	    }
	    case CMD_CIPHER_CALCHASHUPDATE:
	    {
	        CIPHER_HASH_DATA_S *pstCipherHashData = (CIPHER_HASH_DATA_S*)argp;

	        ret = HI_DRV_CIPHER_CalcHashUpdate(pstCipherHashData);
	        break;
	    }
	    case CMD_CIPHER_CALCHASHFINAL:
	    {
	        CIPHER_HASH_DATA_S *pstCipherHashData = (CIPHER_HASH_DATA_S*)argp;

	        ret = HI_DRV_CIPHER_CalcHashFinal(pstCipherHashData);
	        break;
	    }
        case CMD_CIPHER_CALCHASH:
	    {
	        CIPHER_HASH_DATA_S *pstCipherHashData = (CIPHER_HASH_DATA_S*)argp;

	        ret = HI_DRV_CIPHER_CalcHash(pstCipherHashData);
	        break;
	    }
#endif
#ifdef CFG_HI_CIPHER_RSA_SUPPORT
        case CMD_CIPHER_CALCRSA:
        {
	        CIPHER_RSA_DATA_S *pCipherRsaData = (CIPHER_RSA_DATA_S*)argp;
	        ret = HI_DRV_CIPHER_CalcRsa(pCipherRsaData);
	        break;
	    }
#endif
        case CMD_CIPHER_WRITEOTPKEY:
        {
            CIPHER_OTP_KEY_S *pstOtpKey = (CIPHER_OTP_KEY_S*)argp;
	        ret = HI_DRV_CIPHER_WriteOTPKey(pstOtpKey);
            break;
        }

	    default:
	    {
	        HI_ERR_CIPHER("Unknow cmd, MOD_ID=0x%02X, NR=0x%02x, SIZE=0x%02x!\n",
                _IOC_TYPE (cmd), _IOC_NR (cmd), _IOC_SIZE(cmd));
	        ret = HI_FAILURE;
	        break;
	    }
    }

    HI_INFO_CIPHER("end cmd, MOD_ID=0x%02X, NR=0x%02x, SIZE=0x%02x!\n",
                _IOC_TYPE (cmd), _IOC_NR (cmd), _IOC_SIZE(cmd));

    return ret;
}

static long DRV_CIPHER_Ioctl(struct file *ffile, unsigned int cmd, unsigned long arg)
{
    long ret;
    CMD_CIPHER_PARAM_U unCmdParam;

    if ((HI_NULL == ffile) || (HI_NULL == ffile->f_path.dentry))
    {
        HI_ERR_CIPHER("Invalid cmd param size!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if(_IOC_SIZE(cmd) > sizeof(unCmdParam))
    {
        HI_ERR_CIPHER("Invalid cmd param size %d!\n", _IOC_SIZE(cmd));
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (copy_from_user(&unCmdParam, (void __user *)arg, _IOC_SIZE(cmd)))
    {
        HI_ERR_CIPHER("copy data from user fail!\n");
        return HI_FAILURE;
    }

    ret = CIPHER_Ioctl(ffile->f_path.dentry->d_inode, ffile, cmd, (HI_VOID*)&unCmdParam);

    if (copy_to_user((void __user *)arg, &unCmdParam, _IOC_SIZE(cmd)))
    {
        HI_ERR_CIPHER("copy data to user fail!\n");
        return HI_FAILURE;
    }

    return ret;
}

/** <* ref from linux/fs.h */
static struct file_operations DRV_CIPHER_Fops=
{
    .owner            = THIS_MODULE,
#if defined(CFG_HI_CIPHER_MULTICIPHER_SUPPORT)
    .open             = DRV_CIPHER_Open,
#endif
    .unlocked_ioctl   = DRV_CIPHER_Ioctl,
#if defined(CFG_HI_CIPHER_MULTICIPHER_SUPPORT)
    .release          = DRV_CIPHER_Release,
#endif
};

static struct miscdevice cipher_dev =
{
    .minor      = MISC_DYNAMIC_MINOR,
    .name       = UMAP_DEVNAME_CIPHER,
    .fops       = &DRV_CIPHER_Fops,
};

HI_S32 __init CIPHER_DRV_ModInit(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    struct proc_dir_entry *pstProcEntry = HI_NULL;

   if (misc_register(&cipher_dev))
   {
       HI_ERR_CIPHER("ERROR: could not register cipher devices\n");
       return -1;
   }

   g_u32CipherBase = (HI_U32)ioremap_nocache(REG_BASE_PHY_ADDR_CIPHER, REG_CI_BASE_SIZE);
   if (0 == g_u32CipherBase)
   {
       HI_ERR_CIPHER("ERROR: could not ioremap cipher base.\n");
       misc_deregister(&cipher_dev);
       return -1;
   }

#ifdef CFG_HI_CIPHER_MULTICIPHER_SUPPORT
    ret = DRV_CIPHER_Init();
    if (HI_SUCCESS != ret)
    {
        misc_deregister(&cipher_dev);
        if (g_u32CipherBase != 0)
        {
            iounmap((void __iomem *)g_u32CipherBase);
            g_u32CipherBase = 0;
        }
        return ret;
    }
#endif

#ifdef CFG_HI_CIPHER_HASH_SUPPORT
    (HI_VOID)HASH_DRV_ModInit();
    (HI_VOID)HAL_Cipher_HashSoftReset();
#endif

    pstProcEntry = proc_create(CIPHER_PROC_NAME, 0, NULL, &DRV_CIPHER_ProcFops);
    if (NULL == pstProcEntry)
    {
        HI_ERR_CIPHER(KERN_ERR "cipher: can't create %s.\n", CIPHER_PROC_NAME);
        misc_deregister(&cipher_dev);
        if (g_u32CipherBase != 0)
        {
            iounmap((void __iomem *)g_u32CipherBase);
            g_u32CipherBase = 0;
        }

        return HI_FAILURE;
    }

    printk("Load hi_cipher.ko success.\n");

    return HI_SUCCESS;
}

HI_VOID __exit CIPHER_DRV_ModExit(HI_VOID)
{
    remove_proc_entry(CIPHER_PROC_NAME, NULL);

#ifdef CFG_HI_CIPHER_MULTICIPHER_SUPPORT
    (HI_VOID)DRV_CIPHER_DeInit();
#endif

#ifdef CFG_HI_CIPHER_HASH_SUPPORT
    (HI_VOID)HASH_DRV_ModDeInit();
#endif
    misc_deregister(&cipher_dev);

    if (g_u32CipherBase != 0)
    {
        iounmap((void __iomem *)g_u32CipherBase);
        g_u32CipherBase = 0;
    }
    HI_PRINT("Unload hi_cipher.ko success.\n");
}

/***************************** Macro Definition ******************************/



/*************************** Structure Definition ****************************/



/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/

static int hi35xx_cipher_probe(struct platform_device *pdev)
{
    struct resource *mem = HI_NULL;
    if (HI_NULL == pdev)
    {
        return -1;
    }

    mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    g_u32CipherBase = (HI_U32)devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR((void *)g_u32CipherBase))
            return PTR_ERR((void *)g_u32CipherBase);

    cipher_irq = platform_get_irq(pdev, 0);
    if (cipher_irq <= 0) {
            dev_err(&pdev->dev, "cannot find cipher IRQ\n");
    }

    if(0 != CIPHER_DRV_ModInit())
    {
        return -1;
    }

    return 0;
}

static int hi35xx_cipher_remove(struct platform_device *pdev)
{
    CIPHER_DRV_ModExit();
    return 0;
}

static const struct of_device_id hi35xx_cipher_match[] = {
        { .compatible = "hisilicon,hi_cipher" },
        {},
};
MODULE_DEVICE_TABLE(of, hi35xx_cipher_match);

static struct platform_driver hi35xx_cipher_driver = {
        .probe          = hi35xx_cipher_probe,
        .remove         = hi35xx_cipher_remove,
        .driver         = {
                .name   = "hi35xx_cipher",
                .of_match_table = hi35xx_cipher_match,
        },
};

module_platform_driver(hi35xx_cipher_driver);

MODULE_AUTHOR("Hi3720 MPP GRP");
MODULE_LICENSE("GPL");

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

