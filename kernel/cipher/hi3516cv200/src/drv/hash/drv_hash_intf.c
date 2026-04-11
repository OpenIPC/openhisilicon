/*********************************************************************************
*
*  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
***********************************************************************************/
#include <asm/setup.h>
#include <asm/barrier.h>    /* mb() */
#include <asm/uaccess.h>
#include <linux/version.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0))
#include <asm/system.h>
#endif
#include <mach/io.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/param.h>
#include <linux/delay.h>

#include <linux/init.h>
#include <asm/io.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/version.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/random.h>
#include <linux/signal.h>
#include <linux/seq_file.h>

#include "hi_type.h"
#include "hi_drv_cipher.h"
#include "drv_hash.h"
#include "hi_type.h"
#include "drv_cipher_define.h"
#include "drv_cipher_ioctl.h"
#include "hal_cipher.h"
#include "drv_cipher.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

/* Set the defualt timeout value for hash calculating (5000 ms)*/
#define HASH_MAX_DURATION (5000)
#define HASH_PADDING_LEGNTH     8

#define HASH_MMZ_BUF_LEN	(1*1024*1024)		//1M


#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
DECLARE_MUTEX(g_HashMutexKernel);
#else
DEFINE_SEMAPHORE(g_HashMutexKernel);
#endif

typedef enum
{
    HASH_READY,
    REC_READY,
    DMA_READY,
}HASH_WAIT_TYPE;

HI_S32 HAL_Cipher_HashSoftReset(HI_VOID)
{
    HI_U32 CipherCrgValue;
    HI_U32 pvirt;

    if (REG_CRG_CLK_PHY_ADDR_SHA == 0x00)
    {
        return HI_SUCCESS;
    }

    pvirt = (HI_U32)ioremap_nocache(REG_CRG_CLK_PHY_ADDR_SHA, 0x100);

    HAL_CIPHER_ReadReg(pvirt, &CipherCrgValue);
    HAL_SET_BIT(CipherCrgValue, CRG_RST_BIT_SHA); /* reset */
    HAL_SET_BIT(CipherCrgValue, CRG_CLK_BIT_SHA); /* set the bit 3, clock opened */
    HAL_CIPHER_WriteReg(pvirt,CipherCrgValue);

    /* clock select and cancel reset 0x30100*/
    HAL_CLEAR_BIT(CipherCrgValue, CRG_RST_BIT_SHA); /* cancel reset */
    HAL_SET_BIT(CipherCrgValue, CRG_CLK_BIT_SHA);   /* set the bit 1, clock opened */
    HAL_CIPHER_WriteReg(pvirt,CipherCrgValue);

    iounmap((void __iomem *)pvirt);

    return HI_SUCCESS;
}

inline HI_S32 HASH_WaitReady(HASH_WAIT_TYPE enType, HI_BOOL bFast)
{
    CIPHER_SHA_STATUS_U unCipherSHAstatus;
    HI_U32 u32TimeOut = 0;
    HI_SIZE_T ulStartTime = 0;
    HI_SIZE_T ulLastTime = 0;
    HI_SIZE_T ulDuraTime = 0;

    /* wait for hash_rdy */
    while(1)
    {
        unCipherSHAstatus.u32 = 0;
        (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_STATUS_ADDR, &unCipherSHAstatus.u32);
//        HI_PRINT("unCipherSHAstatus 0x%x\n", unCipherSHAstatus.u32);
        if(HASH_READY == enType)
        {
            if(1 == unCipherSHAstatus.bits.hash_rdy)
            {
                break;
            }
        }
        else if (REC_READY == enType)
        {
            if(1 == unCipherSHAstatus.bits.rec_rdy)
            {
                break;
            }
        }
        else if (DMA_READY == enType)
        {
            if(1 == unCipherSHAstatus.bits.dma_rdy)
            {
                break;
            }
        }
        else
        {
            HI_ERR_CIPHER("Error! Invalid wait type!\n");
            return HI_FAILURE;
        }

        if (bFast)
        {
            u32TimeOut++;
            if (u32TimeOut >= 0x100000)
            {
                HI_ERR_CIPHER("Error! Hash time out (0x%x)!\n", u32TimeOut);
                return HI_FAILURE;
            }

            //No OP instruction
            //__asm__ __volatile__("": : :"memory");
        }
        else
        {
            if(ulStartTime == 0)
            {
                ulStartTime = jiffies;
            }

            ulLastTime = jiffies;
            ulDuraTime = jiffies_to_msecs(ulLastTime - ulStartTime);
    		msleep(1);
            if (ulDuraTime >= HASH_MAX_DURATION )
            {
                HI_ERR_CIPHER("Error! Hash time out!\n");
                HAL_Cipher_HashSoftReset();
                return HI_FAILURE;
            }
        }
    }

    return HI_SUCCESS;
}

#ifdef CIPHER_MHASH_SUPPORT
static HI_U32 HAL_Hash_Small2Large(HI_U32 u32SamllVal)
{
    HI_U32 u32LargeVal = 0;

    u32LargeVal  = (u32SamllVal >> 24) & 0xFF;
    u32LargeVal |= ((u32SamllVal >> 16) & 0xFF) << 8;
    u32LargeVal |= ((u32SamllVal >> 8) & 0xFF) << 16;
    u32LargeVal |= ((u32SamllVal) & 0xFF) << 24;

    return u32LargeVal;
}
#endif

HI_S32 HAL_Cipher_CalcHashInit(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_SUCCESS;
    CIPHER_SHA_CTRL_U unCipherSHACtrl;
    CIPHER_SHA_START_U unCipherSHAStart;
#ifdef CIPHER_MHASH_SUPPORT
    HI_U32 i;
    HI_U32 u32RegAddr;
    HI_U32 u32ShaVal;
#endif
    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error! Null pointer input!\n");
        return HI_FAILURE;
    }

    if(HI_UNF_CIPHER_HASH_TYPE_BUTT <= pCipherHashData->enShaType)
    {
        HI_ERR_CIPHER("Error, Invalid hash type!\n");
        return HI_FAILURE;
    }

    if((HI_CIPHER_HMAC_KEY_FROM_CPU < pCipherHashData->enHMACKeyFrom)
        && (HI_CIPHER_HMAC_KEY_FROM_NONE != pCipherHashData->enHMACKeyFrom))
    {
        HI_ERR_CIPHER("enHMACKeyFrom invalid:0x%x!\n",pCipherHashData->enHMACKeyFrom);
        return HI_FAILURE;
    }

    /* wait for hash_rdy */
    ret = HASH_WaitReady(HASH_READY, HI_FALSE);
    if(HI_SUCCESS != ret)
    {
        return HI_FAILURE;
    }



    /* write total len low and high */
#ifdef CIPHER_MHASH_SUPPORT
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_TOTALLEN_LOW_ADDR, pCipherHashData->u32DataLen);
#else
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_TOTALLEN_LOW_ADDR, 0xFFFFFFFF);
#endif
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_TOTALLEN_HIGH_ADDR, 0);

    /* config sha_ctrl : read by dma first, and by cpu in the hash final function */
    unCipherSHACtrl.u32 = 0;
    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_CTRL_ADDR, &unCipherSHACtrl.u32);
    unCipherSHACtrl.bits.read_ctrl = 0;
    if( HI_UNF_CIPHER_HASH_TYPE_SHA1 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x0;
    }
    else if( HI_UNF_CIPHER_HASH_TYPE_SHA256 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x1;
    }
    else if( HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pCipherHashData->enShaType )
    {
		/* unCipherSHACtrl.bits.hardkey_hmac_flag = 1; */
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x0;
		/* unCipherSHACtrl.bits.hardkey_sel = pCipherHashData->enHMACKeyFrom; */
    }
    else if( HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x1;
		/* unCipherSHACtrl.bits.hardkey_sel = pCipherHashData->enHMACKeyFrom; */
    }
    else
    {
        HI_ERR_CIPHER("Invalid hash type input!\n");
        return HI_FAILURE;
    }

    unCipherSHACtrl.bits.small_end_en = 1;
#ifdef CIPHER_MHASH_SUPPORT
    unCipherSHACtrl.bits.sha_init_update_en = 1;
    u32RegAddr = CIPHER_HASH_REG_INIT1_UPDATE;
    for(i=0; i<8; i++)
    {
        u32ShaVal = HAL_Hash_Small2Large(pCipherHashData->u32ShaVal[i]);
        (HI_VOID)HAL_CIPHER_WriteReg(u32RegAddr+i*4, u32ShaVal);
 //        HI_PRINT ("IV[%d] 0x%x\n", i, u32ShaVal);
    }
#endif
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_CTRL_ADDR, unCipherSHACtrl.u32);

    /* config sha_start */
    unCipherSHAStart.u32 = 0;
    unCipherSHAStart.bits.sha_start = 1;
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_START_ADDR, unCipherSHAStart.u32);

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_CalcHashUpdate(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_SUCCESS;

    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        return HI_FAILURE;
    }

    if(HI_UNF_CIPHER_HASH_TYPE_BUTT <= pCipherHashData->enShaType)
    {
        HI_ERR_CIPHER("Error, Invalid hash sha type!\n");
        return HI_FAILURE;
    }

    if((HI_CIPHER_HMAC_KEY_FROM_CPU < pCipherHashData->enHMACKeyFrom)
        && (HI_CIPHER_HMAC_KEY_FROM_NONE != pCipherHashData->enHMACKeyFrom))
    {
        HI_ERR_CIPHER("enHMACKeyFrom invalid!\n");
        return HI_FAILURE;
    }
    if ((pCipherHashData->u32DataLen > HASH_MMZ_BUF_LEN)  || (pCipherHashData->enShaType >= HI_UNF_CIPHER_HASH_TYPE_BUTT))
	{
        	HI_ERR_CIPHER("Error, Null pointer input!\n");
        	return HI_FAILURE;
	}

    ret= HASH_WaitReady(REC_READY, HI_FALSE);
    if(HI_SUCCESS != ret)
    {
        return HI_FAILURE;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_DMA_START_ADDR, pCipherHashData->u32DataPhy);
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_DMA_LEN, pCipherHashData->u32DataLen);

    ret  = HASH_WaitReady(REC_READY, HI_FALSE);
    if(HI_SUCCESS != ret)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_CalcHashFinal(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_SUCCESS;
    CIPHER_SHA_STATUS_U unCipherSHAStatus;
    HI_U32 u32RegAddr;
    HI_U32 i = 0;

    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        return HI_FAILURE;
    }

    if(HI_UNF_CIPHER_HASH_TYPE_BUTT <= pCipherHashData->enShaType)
    {
        HI_ERR_CIPHER("Error, Invalid hash sha type!\n");
        return HI_FAILURE;
    }

    if((HI_CIPHER_HMAC_KEY_FROM_CPU < pCipherHashData->enHMACKeyFrom)
        && (HI_CIPHER_HMAC_KEY_FROM_NONE != pCipherHashData->enHMACKeyFrom))
    {
        HI_ERR_CIPHER("enHMACKeyFrom invalid!\n");
        return HI_FAILURE;
    }

    /* wait for rec_ready instead of hash_ready */
    ret= HASH_WaitReady(REC_READY, HI_FALSE);
    if(HI_SUCCESS != ret)
    {
        return HI_FAILURE;
    }

    /* read digest */
    unCipherSHAStatus.u32 = 0;
    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_STATUS_ADDR, &unCipherSHAStatus.u32);

    u32RegAddr = CIPHER_HASH_REG_SHA_OUT1;
    if( (0x00 == unCipherSHAStatus.bits.error_state) && (0x00 == unCipherSHAStatus.bits.len_err))
    {
        if( (HI_UNF_CIPHER_HASH_TYPE_SHA1 == pCipherHashData->enShaType)
         || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pCipherHashData->enShaType))
        {

            for(i = 0; i < 5; i++)
            {
                 (HI_VOID)HAL_CIPHER_ReadReg(u32RegAddr+i*4, &pCipherHashData->u32ShaVal[i]);
            }
        }
        else if( (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pCipherHashData->enShaType )
              || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pCipherHashData->enShaType))
        {
            for(i = 0; i < 8; i++)
            {
                 (HI_VOID)HAL_CIPHER_ReadReg(u32RegAddr+i*4, &pCipherHashData->u32ShaVal[i]);
    		}
        }
        else
        {
            HI_ERR_CIPHER("Invalid hash type : %d!\n", pCipherHashData->enShaType);
            return HI_FAILURE;
        }
    }
    else
    {
        HI_ERR_CIPHER("Error! SHA Status Reg: error_state = %d!\n", unCipherSHAStatus.bits.error_state);
        HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 Cipher_CalcHashInit(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_SUCCESS;

    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        return HI_FAILURE;
    }

    if(HI_UNF_CIPHER_HASH_TYPE_BUTT <= pCipherHashData->enShaType)
    {
        HI_ERR_CIPHER("Error, Invalid hash sha type!\n");
        return HI_FAILURE;
    }

    if((HI_CIPHER_HMAC_KEY_FROM_CPU < pCipherHashData->enHMACKeyFrom)
        && (HI_CIPHER_HMAC_KEY_FROM_NONE != pCipherHashData->enHMACKeyFrom))
    {
        HI_ERR_CIPHER("enHMACKeyFrom invalid!\n");
        return HI_FAILURE;
    }

#ifndef CIPHER_MHASH_SUPPORT
    ret = HAL_Cipher_CalcHashInit(pCipherHashData);
    if( HI_SUCCESS != ret )
    {
        HI_ERR_CIPHER("Cipher hash init failed!\n");
        return HI_FAILURE;
    }
#endif

    return ret;
}

static HI_S32 Cipher_CalcHashUpdate(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_SUCCESS;

    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        return HI_FAILURE;
    }

    if( 0 != pCipherHashData->u32DataPhy % 4)
    {
        HI_ERR_CIPHER("Error, invalid data phy!\n");
        return HI_FAILURE;
    }
    if( ((pCipherHashData->u32DataLen % 64) != 0) || (pCipherHashData->u32DataLen == 0))
    {
        HI_ERR_CIPHER("Error, Invalid input data len 0x%x!\n", pCipherHashData->u32DataLen);
        return HI_FAILURE;
    }

    if(HI_UNF_CIPHER_HASH_TYPE_BUTT <= pCipherHashData->enShaType)
    {
        HI_ERR_CIPHER("Error, Invalid hash sha type!\n");
        return HI_FAILURE;
    }

    if((HI_CIPHER_HMAC_KEY_FROM_CPU < pCipherHashData->enHMACKeyFrom)
        && (HI_CIPHER_HMAC_KEY_FROM_NONE != pCipherHashData->enHMACKeyFrom))
    {
        HI_ERR_CIPHER("enHMACKeyFrom invalid!\n");
        return HI_FAILURE;
    }

#ifdef CIPHER_MHASH_SUPPORT
    ret = HAL_Cipher_CalcHashInit(pCipherHashData);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("HAL_Hash_Init failed! ret = 0x%08x\n", ret);
        return HI_FAILURE;
    }
    ret = HAL_Cipher_CalcHashUpdate(pCipherHashData);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("HAL_Hash_Update failed! ret = 0x%08x\n", ret);
        return HI_FAILURE;
    }
    ret = HAL_Cipher_CalcHashFinal(pCipherHashData);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("HAL_Hash_Final failed! ret = 0x%08x\n", ret);
        return HI_FAILURE;
    }
#else
    ret = HAL_Cipher_CalcHashUpdate(pCipherHashData);
    if(HI_FAILURE == ret)
    {
        HI_ERR_CIPHER("Cipher hash update failed!\n");
        return HI_FAILURE;
    }
#endif
    return HI_SUCCESS;

}

HI_S32 Cipher_CalcHashFinal(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_SUCCESS;

    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        return HI_FAILURE;
    }

    ret = Cipher_CalcHashUpdate(pCipherHashData);
    if(HI_FAILURE == ret)
    {
        HI_ERR_CIPHER("Cipher hash final failed!\n");
        return HI_FAILURE;
    }

#ifndef CIPHER_MHASH_SUPPORT
    ret = HAL_Cipher_CalcHashFinal(pCipherHashData);
    if(HI_FAILURE == ret)
    {
        HI_ERR_CIPHER("Cipher hash final failed!\n");
        return HI_FAILURE;
    }
#endif

    return ret;
}

HI_S32 HI_DRV_CIPHER_CalcHashInit(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_SUCCESS;

    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        return HI_FAILURE;
    }

    if(down_interruptible(&g_HashMutexKernel))
    {
    	HI_ERR_CIPHER("down_interruptible failed!\n");
        return HI_FAILURE;
    }

    ret = Cipher_CalcHashInit(pCipherHashData);

	up(&g_HashMutexKernel);

    return ret;
}

HI_S32 HI_DRV_CIPHER_CalcHashUpdate(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if(down_interruptible(&g_HashMutexKernel))
    {
    	HI_ERR_CIPHER("down_interruptible failed!\n");
        return HI_FAILURE;
    }
    s32Ret = Cipher_CalcHashUpdate(pCipherHashData);

	up(&g_HashMutexKernel);

    return s32Ret;
}

HI_S32 HI_DRV_CIPHER_CalcHashFinal(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if(down_interruptible(&g_HashMutexKernel))
    {
    	HI_ERR_CIPHER("down_interruptible failed!\n");
        return HI_FAILURE;
    }

    s32Ret = Cipher_CalcHashFinal(pCipherHashData);

	up(&g_HashMutexKernel);

    return s32Ret;
}

HI_S32 Cipher_CalcHash(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 i;
    HI_U32 u32OutLen;
    CIPHER_SHA_CTRL_U unCipherSHACtrl;
    CIPHER_SHA_START_U unCipherSHAStart;
    CIPHER_SHA_STATUS_U unCipherSHAStatus;

    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        return HI_FAILURE;
    }
    if( ((pCipherHashData->u32DataLen % 64) != 0) || (pCipherHashData->u32DataLen == 0))
    {
        HI_ERR_CIPHER("Error, Invalid input data len 0x%x!\n", pCipherHashData->u32DataLen);
        return HI_FAILURE;
    }
    if( 0 != pCipherHashData->u32DataPhy % 4)
    {
        HI_ERR_CIPHER("Error, invalid data phy!\n");
        return HI_FAILURE;
    }

    if((HI_CIPHER_HMAC_KEY_FROM_CPU < pCipherHashData->enHMACKeyFrom)
        && (HI_CIPHER_HMAC_KEY_FROM_NONE != pCipherHashData->enHMACKeyFrom))
    {
        HI_ERR_CIPHER("enHMACKeyFrom invalid!\n");
        return HI_FAILURE;
    }

    /* wait for hash_rdy */
    ret = HASH_WaitReady(HASH_READY, HI_TRUE);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Hash wait ready failed!\n");
        (HI_VOID)HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }

    /* write total len low and high */
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_TOTALLEN_LOW_ADDR, pCipherHashData->u32DataLen);
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_TOTALLEN_HIGH_ADDR, 0);

    /* config sha_ctrl : read by dma first, and by cpu in the hash final function */
    unCipherSHACtrl.u32 = 0;
    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_CTRL_ADDR, &unCipherSHACtrl.u32);
    unCipherSHACtrl.bits.read_ctrl = 0;
    if( HI_UNF_CIPHER_HASH_TYPE_SHA1 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x0;
        u32OutLen = 20;
    }
    else if( HI_UNF_CIPHER_HASH_TYPE_SHA256 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x1;
        u32OutLen = 32;
    }
    else if( HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pCipherHashData->enShaType )
    {
		/* unCipherSHACtrl.bits.hardkey_hmac_flag = 1; */
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x0;
        u32OutLen = 20;
		/* unCipherSHACtrl.bits.hardkey_sel = pCipherHashData->enHMACKeyFrom; */
    }
    else if( HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x1;
        u32OutLen = 32;
		/* unCipherSHACtrl.bits.hardkey_sel = pCipherHashData->enHMACKeyFrom; */
    }
    else
    {
        HI_ERR_CIPHER("Invalid hash type input!\n");
        (HI_VOID)HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }
    unCipherSHACtrl.bits.small_end_en = 1;
    unCipherSHACtrl.bits.sha_init_update_en = 0;

    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_CTRL_ADDR, unCipherSHACtrl.u32);

    /* config sha_start */
    unCipherSHAStart.u32 = 0;
    unCipherSHAStart.bits.sha_start = 1;
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_START_ADDR, unCipherSHAStart.u32);

    /*Update*/
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_DMA_START_ADDR, pCipherHashData->u32DataPhy);
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_DMA_LEN, pCipherHashData->u32DataLen);

    /* wait for hash_rdy */
    ret = HASH_WaitReady(HASH_READY, HI_TRUE);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Hash wait ready failed!\n");
        (HI_VOID)HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }

    /* read digest */
    unCipherSHAStatus.u32 = 0;
    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_STATUS_ADDR, &unCipherSHAStatus.u32);

    if( (0x00 == unCipherSHAStatus.bits.error_state) && (0x00 == unCipherSHAStatus.bits.len_err))
    {
        if( (HI_UNF_CIPHER_HASH_TYPE_SHA1 == pCipherHashData->enShaType)
         || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pCipherHashData->enShaType))
        {

            for(i = 0; i < 5; i++)
            {
                 (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_SHA_OUT1+i*4, &pCipherHashData->u32ShaVal[i]);
            }
        }
        else if( (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pCipherHashData->enShaType )
              || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pCipherHashData->enShaType))
        {
            for(i = 0; i < 8; i++)
            {
                 (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_SHA_OUT1+i*4, &pCipherHashData->u32ShaVal[i]);
    		}
        }
    }
    else
    {
        HI_ERR_CIPHER("Error! SHA Status Reg: error_state = %d!\n", unCipherSHAStatus.bits.error_state);
        HI_ERR_CIPHER("Error! SHA Status Reg: len_err = %d!\n", unCipherSHAStatus.bits.len_err);
        (HI_VOID)HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_DRV_CIPHER_CalcHash(CIPHER_HASH_DATA_S *pCipherHashData)
{
	return Cipher_CalcHash(pCipherHashData);
}

HI_VOID HASH_DRV_ModInit(HI_VOID)
{
    return ;
}

HI_VOID HASH_DRV_ModDeInit(HI_VOID)
{

}

#ifdef MODULE
//module_init(HASH_DRV_ModInit);
//module_exit(HASH_DRV_ModExit);
#endif

EXPORT_SYMBOL(HI_DRV_CIPHER_CalcHashInit);
EXPORT_SYMBOL(HI_DRV_CIPHER_CalcHashUpdate);
EXPORT_SYMBOL(HI_DRV_CIPHER_CalcHashFinal);


MODULE_AUTHOR("Hi3720 MPP GRP");
MODULE_LICENSE("GPL");

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
