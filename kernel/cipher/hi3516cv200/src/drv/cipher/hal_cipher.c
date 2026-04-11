/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hal_cipher.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/

#include <linux/jiffies.h>
#include <linux/delay.h>
#include <asm/barrier.h>    /* mb() */
#include <mach/io.h>
#include "hi_type.h"
#include "hi_error_mpi.h"
#include "drv_cipher_ioctl.h"
#include "drv_cipher_reg.h"
#include "hal_cipher.h"
#include "hi_drv_cipher.h"
#include "drv_hash.h"
#include "drv_cipher_log.h"
#include "drv_cipher_mmz.h"
#include "hal_efuse.h"

/***************************** Macro Definition ******************************/
#define RSA_DATA_CLR            (7<<4)
#define RSA_DATA_CLR_KEY        (1<<4)
#define RSA_DATA_CLR_INPUT      (2<<4)
#define RSA_DATA_CLR_OUTPUT     (4<<4)
#define RSA_MOD_SEL		        (3 << 0)
#define RSA_MOD_SEL_OPT	        (0 << 0)
#define RSA_MOD_SEL_KEY_UPDATA	(1 << 0)
#define RSA_MOD_SEL_RAM_CLAER	(2 << 0)
#define RSA_MOD_SEL_CRC16	    (3 << 0)
#define RSA_BUSY		        (1 << 0)
#define RSA_START		        (1 << 0)

#define RSA_RTY_CNT             5000

/******************************* API declaration *****************************/
HI_S32 HAL_Cipher_GetSrcLstRaddr(HI_U32 chnId, HI_U32 *addr)
{
    HI_U32 regAddr  = 0;
    HI_U32 regValue = 0;

    if (CIPHER_PKGx1_CHAN == chnId || HI_NULL == addr)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_SRC_LST_RADDR(chnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);
    
    *addr = regValue;

    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetDestLstRaddr(HI_U32 chnId, HI_U32 *addr)
{
    HI_U32 regAddr  = 0;
    HI_U32 regValue = 0;

    if (CIPHER_PKGx1_CHAN == chnId || HI_NULL == addr)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_DEST_LST_RADDR(chnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);
    
    *addr = regValue;

    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetInBufNum(HI_U32 chnId, HI_U32 num)
{
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /* register0~15 bit is valid, others bits reserved */
    regAddr = CIPHER_REG_CHANn_IBUF_NUM(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%#x, set to 0xffff\n", num);
        num = 0xffff;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, num);
    
    HI_INFO_CIPHER(" cnt=%u\n", num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetInBufNum(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 regAddr  = 0;
    HI_U32 regValue = 0;

    if (CIPHER_PKGx1_CHAN == chnId || HI_NULL == pNum)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_IBUF_NUM(chnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);
    
    *pNum = regValue;

    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetInBufCnt(HI_U32 chnId, HI_U32 num)
{
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_IBUF_CNT(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, num);
    
    HI_INFO_CIPHER(" HAL_Cipher_SetInBufCnt=%u\n", num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetInBufCnt(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    if ( (CIPHER_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_IBUF_CNT(chnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);
    *pNum = regValue;
    
    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetInBufEmpty(HI_U32 chnId, HI_U32 num)
{
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_IEMPTY_CNT(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, num);
    
    HI_INFO_CIPHER(" cnt=%u\n", num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetInBufEmpty(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    if ( (CIPHER_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_IEMPTY_CNT(chnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);
    
    *pNum = regValue;
    
    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetOutBufNum(HI_U32 chnId, HI_U32 num)
{
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_OBUF_NUM(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, num);
    
    HI_INFO_CIPHER("chn=%d cnt=%u\n", chnId, num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetOutBufNum(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    if ( (CIPHER_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_OBUF_NUM(chnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    *pNum = regValue;

    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetOutBufCnt(HI_U32 chnId, HI_U32 num)
{
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_OBUF_CNT(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, num);
    
    HI_INFO_CIPHER("SetOutBufCnt=%u, chnId=%u\n", num,chnId);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetOutBufCnt(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    if ( (CIPHER_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_OBUF_CNT(chnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    *pNum = regValue;

    HI_INFO_CIPHER(" HAL_Cipher_GetOutBufCnt=%u\n", regValue);
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetOutBufFull(HI_U32 chnId, HI_U32 num)
{
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_OFULL_CNT(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, num);
    
    HI_INFO_CIPHER(" cnt=%u\n", num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetOutBufFull(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    if ( (CIPHER_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_OFULL_CNT(chnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    *pNum = regValue;

    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_WaitIdle(HI_VOID)
{
    HI_S32 i = 0;
    HI_U32 u32RegAddr = 0;
    HI_U32 u32RegValue = 0;

    /* channel 0 configuration register [31-2]:reserved, [1]:ch0_busy, [0]:ch0_start 
         * [1]:channel 0 status signal, [0]:channel 0 encrypt/decrypt start signal
         */

    u32RegAddr = CIPHER_REG_CHAN0_CFG;
    for (i = 0; i < CIPHER_WAIT_IDEL_TIMES; i++)
    {
        (HI_VOID)HAL_CIPHER_ReadReg(u32RegAddr, &u32RegValue);
        if (0x0 == ((u32RegValue >> 1) & 0x01))
        {
            return HI_SUCCESS;
        }
        else
        {
            //udelay(1);
        }
    }

    return HI_FAILURE;
}
/*
just only check for channel 0
 */
HI_BOOL HAL_Cipher_IsIdle(HI_U32 chn)
{
    HI_U32 u32RegValue = 0;

    HI_ASSERT(CIPHER_PKGx1_CHAN == chn);

    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_REG_CHAN0_CFG, &u32RegValue);
    if (0x0 == ((u32RegValue >> 1) & 0x01))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_S32 HAL_Cipher_SetDataSinglePkg(HI_DRV_CIPHER_DATA_INFO_S * info)
{
    HI_U32 regAddr = 0;
    HI_U32 i = 0;

    regAddr = CIPHER_REG_CHAN0_CIPHER_DIN(0);
    
    /***/
    for (i = 0; i < (16/sizeof(HI_U32)); i++)
    {
        (HI_VOID)HAL_CIPHER_WriteReg(regAddr + (i * sizeof(HI_U32)), (*(info->u32DataPkg + i)) );
    }
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_ReadDataSinglePkg(HI_U32 *pData)
{
    HI_U32 regAddr = 0;
    HI_U32 i = 0;

    regAddr = CIPHER_REG_CHAN0_CIPHER_DOUT(0);

    /***/
    for (i = 0; i < (16/sizeof(HI_U32)); i++)
    {
        (HI_VOID)HAL_CIPHER_ReadReg(regAddr + (i * sizeof(HI_U32)), pData+ i);
    }
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_StartSinglePkg(HI_U32 chnId)
{
    HI_U32 u32RegAddr = 0;
    HI_U32 u32RegValue = 0;

    HI_ASSERT(CIPHER_PKGx1_CHAN == chnId);

    u32RegAddr = CIPHER_REG_CHAN0_CFG;
    (HI_VOID)HAL_CIPHER_ReadReg(u32RegAddr, &u32RegValue);
    
    u32RegValue |= 0x1;
    (HI_VOID)HAL_CIPHER_WriteReg(u32RegAddr, u32RegValue); /* start work */
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetBufAddr(HI_U32 chnId, CIPHER_BUF_TYPE_E bufType, HI_U32 addr)
{
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (CIPHER_BUF_TYPE_IN == bufType)
    {
        regAddr = CIPHER_REG_CHANn_SRC_LST_SADDR(chnId);
    }
    else if (CIPHER_BUF_TYPE_OUT == bufType)
    {
        regAddr = CIPHER_REG_CHANn_DEST_LST_SADDR(chnId);
    }
    else
    {
        HI_ERR_CIPHER("SetBufAddr type err:%x.\n", bufType);
        return HI_ERR_CIPHER_INVALID_PARA;
    }


    HI_INFO_CIPHER("Set chn%d '%s' BufAddr to:%x.\n",chnId,
        (CIPHER_BUF_TYPE_IN == bufType)?"In":"Out",  addr);

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, addr);

    return HI_SUCCESS;
}



HI_VOID HAL_Cipher_Reset(HI_VOID)
{

    //(HI_VOID)HAL_CIPHER_WriteReg(CIPHER_SOFT_RESET_ADDR, 1);
    return;
}

HI_S32 HAL_Cipher_GetOutIV(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 i = 0;
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        regAddr = CIPHER_REG_CHAN0_CIPHER_IVOUT(0);
    }
    else
    {
        regAddr = CIPHER_REG_CHAN_CIPHER_IVOUT(chnId);
    }


    /***/
    for (i = 0; i < (CI_IV_SIZE/sizeof(HI_U32)); i++)
    {
        (HI_VOID)HAL_CIPHER_ReadReg(regAddr + (i * sizeof(HI_U32)), &(pCtrl->u32IV[i]));
    }

    return HI_SUCCESS;
}

#ifdef CIPHER_CCM_GCM_SUPPORT
HI_S32 HAL_Cipher_GetTag(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl, HI_U32 *pu32Tag)
{
    HI_U32 i = 0;
    HI_U32 regAddr = 0;
    if (pCtrl->enWorkMode == HI_UNF_CIPHER_WORK_MODE_GCM)
    {
        regAddr = CIPHER_REG_CHANn_GCM_TAG(chnId);
    }
    else
    {
        return HI_FAILURE;
    }
    for (i = 0; i < (CI_IV_SIZE/sizeof(HI_U32)); i++)
    {
        (HI_VOID)HAL_CIPHER_ReadReg(regAddr + (i * sizeof(HI_U32)), &pu32Tag[i]);
    }
    return HI_SUCCESS;
}
HI_S32 HAL_Cipher_SetLen(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 regAddrA0, regAddrA1;
    HI_U32 regAddrP0, regAddrP1;
    HI_U32 regAddrIV, valIV;
    if (pCtrl->enWorkMode == HI_UNF_CIPHER_WORK_MODE_GCM)
    {
        regAddrA0 = CIPHER_REG_CHANn_GCM_A_LEN_0(chnId);
        regAddrP0 = CIPHER_REG_CHANn_GCM_PC_LEN_0(chnId);
        regAddrA1 = CIPHER_REG_CHANn_GCM_A_LEN_1(chnId);
        regAddrP1 = CIPHER_REG_CHANn_GCM_PC_LEN_1(chnId);
        regAddrIV = CIPHER_REG_CHANn_GCM_IV_LEN(chnId);
    }
    else
    {
        return HI_FAILURE;
    }
    (HI_VOID)HAL_CIPHER_ReadReg(regAddrIV, &valIV);
    valIV &= ~(0x1F << ((chnId & 0x03) * 8));
    valIV |= pCtrl->unModeInfo.stGCM.u32IVLen << ((chnId & 0x03) * 8);
    (HI_VOID)HAL_CIPHER_WriteReg(regAddrIV, valIV);
    (HI_VOID)HAL_CIPHER_WriteReg(regAddrA0, pCtrl->unModeInfo.stGCM.u32ALen);
    (HI_VOID)HAL_CIPHER_WriteReg(regAddrP0, pCtrl->unModeInfo.stGCM.u32MLen);
    (HI_VOID)HAL_CIPHER_WriteReg(regAddrA1, 0x00);
    (HI_VOID)HAL_CIPHER_WriteReg(regAddrP1, 0x00);
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_CleanAadEnd(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 regAddr, val;
    if (pCtrl->enWorkMode == HI_UNF_CIPHER_WORK_MODE_GCM)
    {
        regAddr = CIPHER_REG_CHANn_GCM_GHASH_A_END;
    }
    else
    {
        return HI_SUCCESS;
    }

    val = 0x01 << chnId;
    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, val);

    HAL_CIPHER_ReadReg (regAddr, &val);
//    printk("regAddr: 0x%x, val 0x%x\n", regAddr, val);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_WaitAadEnd(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
	HI_U32 regAddr;
    HI_U32 u32Value;
    HI_U32 u32TryCount = 0;

    if (pCtrl->enWorkMode == HI_UNF_CIPHER_WORK_MODE_GCM)
    {
        regAddr = CIPHER_REG_CHANn_GCM_GHASH_A_END;
    }
    else
    {
        return HI_SUCCESS;
    }

	do {

        HAL_CIPHER_ReadReg(regAddr, &u32Value);
 //       printk("regAddr: 0x%x, val 0x%x\n", regAddr, u32Value);
		if ((u32Value >> chnId) & 0x01)
		{
		    return HI_SUCCESS;
        }
		u32TryCount++;
        msleep(1);
	} while (u32TryCount < RSA_RTY_CNT);

    HI_ERR_CIPHER("Wait Aad ghash End timeout, chnid = %d\n", chnId);
    
	return HI_FAILURE;
}

HI_S32 HAL_Cipher_CleanTagVld(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 regAddr, val;

    if (pCtrl->enWorkMode == HI_UNF_CIPHER_WORK_MODE_GCM)
    {
        regAddr = CIPHER_REG_CHANn_GCM_TAG_VLD;
    }
    else
    {
        return HI_SUCCESS;
    }

    val = 0x01 << chnId;
    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, val);

    return HI_SUCCESS;
}

HI_S32 HAL_RSA_WaitTagVld(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
	HI_U32 regAddr;
    HI_U32 u32Value;
    HI_U32 u32TryCount = 0;

    if (pCtrl->enWorkMode == HI_UNF_CIPHER_WORK_MODE_GCM)
    {
        regAddr = CIPHER_REG_CHANn_GCM_TAG_VLD;
    }
    else
    {
        return HI_SUCCESS;
    }

	do {

        HAL_CIPHER_ReadReg(regAddr, &u32Value);
		if ((u32Value >> chnId) & 0x01)
		{
		    return HI_SUCCESS;
        }
		u32TryCount++;
        udelay(500);
	} while (u32TryCount < RSA_RTY_CNT);

    HI_ERR_CIPHER("Wait TAG VLD timeout, chnid = %d\n", chnId);

	return HI_FAILURE;
}
#endif

HI_S32 HAL_Cipher_SetInIV(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 i = 0;
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN != chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHAN0_CIPHER_IVIN(0);

    /***/
    for (i = 0; i < (CI_IV_SIZE/sizeof(HI_U32)); i++)
    {
        (HI_VOID)HAL_CIPHER_WriteReg(regAddr + (i * sizeof(HI_U32)), pCtrl->u32IV[i]);
    }

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetKey(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 i = 0;
    HI_S32 ret = HI_SUCCESS;
    HI_U32 regAddr = 0;
    HI_U32 u32OptId;

    if(NULL == pCtrl)
    {
        HI_ERR_CIPHER("Error, null pointer!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CIPHER_KEY(chnId);

    if (HI_UNF_CIPHER_KEY_SRC_USER == pCtrl->enKeySrc)
    {
        for (i = 0; i < (CI_KEY_SIZE/sizeof(HI_U32)); i++)
        {
            (HI_VOID)HAL_CIPHER_WriteReg(regAddr + (i * sizeof(HI_U32)), pCtrl->u32Key[i]);
        }
    }
    else if ((pCtrl->enKeySrc >= HI_UNF_CIPHER_KEY_SRC_EFUSE_0) && (pCtrl->enKeySrc <= HI_UNF_CIPHER_KEY_SRC_EFUSE_3))
    {
        u32OptId= pCtrl->enKeySrc - HI_UNF_CIPHER_KEY_SRC_EFUSE_0;
        ret = HAL_Efuse_LoadCipherKey(chnId, u32OptId);
    }

    HI_INFO_CIPHER("SetKey: chn%u,Key:%#x, %#x, %#x, %#x.\n", chnId, pCtrl->u32Key[0], pCtrl->u32Key[1], pCtrl->u32Key[2], pCtrl->u32Key[3]);
    
    return ret;
}

/*
=========channel n control register==========
[31:22]             weight [in 64bytes, just only for multi-packet channel encrypt or decrypt, otherwise reserved.]
[21:17]             reserved
[16:14]     RW    key_adder [current key sequence number]
[13]          RW    key_sel [key select control, 0-CPU keys, 1-keys from key Ladder]
[12:11]             reserved
[10:9]      RW      key_length[key length control
                                            (1).AES, 00-128 bits key, 01-192bits 10-256bits, 11-128bits
                                            (2).DES, 00-3 keys, 01-3keys, 10-3keys, 11-2keys]
[8]                     reserved
[7:6]       RW      width[bits width control
                                 (1).for DES/3DES, 00-64bits, 01-8bits, 10-1bit, 11-64bits
                                 (2).for AES, 00-128bits, 01-8bits, 10-1bit, 11-128bits]
[5:4]       RW      alg_sel[algorithm type, 00-DES, 01-3DES, 10-AES, 11-DES]
[3:1]       RW      mode[mode control, 
                                  (1).for AES, 000-ECB, 001-CBC, 010-CFB, 011-OFB, 100-CTR, others-ECB
                                  (2).for DES, 000-ECB, 001-CBC, 010-CFB, 011-OFB, others-ECB]
[0]         RW      decrypt[encrypt or decrypt control, 0 stands for encrypt, 1 stands for decrypt]
*/
HI_S32 HAL_Cipher_Config(HI_U32 chnId, HI_BOOL bDecrypt, HI_BOOL bIVChange, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 keyId = 0;
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    HI_BOOL bKeyByCA = 0;

    if (pCtrl->enKeySrc == HI_UNF_CIPHER_KEY_SRC_USER)
    {
        bKeyByCA = 0;
    }
    else
    {
         bKeyByCA = 1;
    }
    
    if (CIPHER_PKGx1_CHAN == chnId)
    {
        /* channel 0, single packet encrypt or decrypt channel */
        regAddr = CIPHER_REG_CHAN0_CIPHER_CTRL;
    }
    else
    {
        regAddr = CIPHER_REG_CHANn_CIPHER_CTRL(chnId);
    }

    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    if (HI_FALSE == bDecrypt)/* encrypt */
    {
        regValue &= ~(1 << 0);
    }
    else /* decrypt */
    {
        regValue |= 1;
    }

    /* set mode */
    regValue &= ~(0x07 << 1);/* clear bit1~bit3 */
    regValue |= ((pCtrl->enWorkMode & 0x7) << 1);

    /* set algorithm bits */
    regValue &= ~(0x03 << 4); /* clear algorithm bits*/
    regValue |= ((pCtrl->enAlg & 0x3) << 4);

    /* set bits width */
    regValue &= ~(0x03 << 6);
    regValue |= ((pCtrl->enBitWidth & 0x3) << 6);

    regValue &= ~(0x01 << 8);
    regValue |= ((bIVChange & 0x1) << 8);
    if (bIVChange) ///?
    {
        HAL_Cipher_SetInIV(chnId, pCtrl);
    }

    regValue &= ~(0x03 << 9);
    regValue |= ((pCtrl->enKeyLen & 0x3) << 9);

    regValue &= ~(0x01 << 13);
    regValue |= ((bKeyByCA & 0x1) << 13);

//    if (HI_FALSE == bKeyByCA) /* By User */
//    {
        keyId = chnId;/**/

        //HAL_Cipher_SetKey(chnId, pCtrl->u32Key);

        regValue &= ~(0x07 << 14);
        regValue |= ((keyId & 0x7) << 14);
//    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetAGEThreshold(HI_U32 chnId, CIPHER_INT_TYPE_E intType, HI_U32 value)
{
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (CIPHER_INT_TYPE_IN_BUF == intType)
    {
        regAddr = CIPHER_REG_CHANn_IAGE_CNT(chnId);
    }
    else if (CIPHER_INT_TYPE_OUT_BUF == intType)
    {
        regAddr = CIPHER_REG_CHANn_OAGE_CNT(chnId);
    }
    else
    {
        HI_ERR_CIPHER("SetAGEThreshold type err:%x.\n", intType);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (value > 0xffff)
    {
        HI_ERR_CIPHER("SetAGEThreshold value err:%x, set to 0xffff\n", value);
        value = 0xffff;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, value);

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetIntThreshold(HI_U32 chnId, CIPHER_INT_TYPE_E intType, HI_U32 value)
{
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (CIPHER_INT_TYPE_IN_BUF == intType)
    {
        regAddr = CIPHER_REG_CHANn_INT_ICNTCFG(chnId);
    }
    else if (CIPHER_INT_TYPE_OUT_BUF == intType)
    {
        regAddr = CIPHER_REG_CHANn_INT_OCNTCFG(chnId);
    }
    else
    {
        HI_ERR_CIPHER("SetIntThreshold type err:%x.\n", intType);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (value > 0xffff)
    {
        HI_ERR_CIPHER("SetIntThreshold value err:%x, set to 0xffff\n", value);
        value = 0xffff;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, value);

    return HI_SUCCESS;
}

/*
interrupt enable
[31]-----cipher module unitary interrupt enable
[30:16]--reserved
[15] channel 7 output queue data interrupt enable
[14] channel 6 output queue data interrupt enable
[... ] channel ... output queue data interrupt enable
[9]   channel 1 output queue data interrupt enable
[8]   channel 0 data dispose finished interrupt enble
[7] channel 7 input queue data interrupt enable
[6] channel 6 input queue data interrupt enable
...
[1] channel 1 input queue data interrupt enable
[0] reserved
*/
HI_S32 HAL_Cipher_EnableInt(HI_U32 chnId, int intType)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = CIPHER_REG_INT_EN;
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    regValue |= (1 << 31); /* sum switch int_en */
    #if defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100) || defined(CHIP_TYPE_hi3798mv100_a) || defined(CHIP_TYPE_hi3798mv100)
    regValue |= (1 << 30); /* sec_int_en */
    #endif

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        regValue |= (1 << 8);
    }
    else
    {
        if (CIPHER_INT_TYPE_OUT_BUF == (CIPHER_INT_TYPE_OUT_BUF & intType))
        {
            regValue |= (1 << (8 + chnId));
        }

        /* NOT else if */
        if (CIPHER_INT_TYPE_IN_BUF == (CIPHER_INT_TYPE_IN_BUF & intType))
        {
            regValue |= (1 << (0 + chnId));
        }
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);

    HI_INFO_CIPHER("HAL_Cipher_EnableInt: Set INT_EN:%#x\n", regValue);

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_DisableInt(HI_U32 chnId, int intType)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = CIPHER_REG_INT_EN;
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        regValue &= ~(1 << 8);
    }
    else
    {
        if (CIPHER_INT_TYPE_OUT_BUF == (CIPHER_INT_TYPE_OUT_BUF & intType))
        {
            regValue &= ~(1 << (8 + chnId));
        }

        /* NOT else if */
        if (CIPHER_INT_TYPE_IN_BUF == (CIPHER_INT_TYPE_IN_BUF & intType))
        {
            regValue &= ~(1 << (0 + chnId));
        }
    }

    if (0 == (regValue & 0x7fffffff))
    {
        regValue &= ~(1 << 31); /* regValue = 0; sum switch int_en */
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);

    HI_INFO_CIPHER("HAL_Cipher_DisableInt: Set INT_EN:%#x\n", regValue);

    return HI_SUCCESS;
}

HI_VOID HAL_Cipher_DisableAllInt(HI_VOID)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = CIPHER_REG_INT_EN;
    regValue = 0;
    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);
}

/*
interrupt status register
[31:16]--reserved
[15] channel 7 output queue data interrupt enable
[14] channel 6 output queue data interrupt enable
[... ] channel ... output queue data interrupt enable
[9]   channel 1 output queue data interrupt enable
[8]   channel 0 data dispose finished interrupt enble
[7] channel 7 input queue data interrupt enable
[6] channel 6 input queue data interrupt enable
...
[1] channel 1 input queue data interrupt enable
[0] reserved
*/

HI_VOID HAL_Cipher_GetIntState(HI_U32 *pState)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = CIPHER_REG_INT_STATUS;
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    if (pState)
    {
        *pState = regValue;
    }

   HI_INFO_CIPHER("HAL_Cipher_GetIntState=%#x\n", regValue);
}

HI_VOID HAL_Cipher_GetIntEnState(HI_U32 *pState)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = CIPHER_REG_INT_EN;
    
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    if (pState)
    {
        *pState = regValue;
    }

   HI_INFO_CIPHER("HAL_Cipher_GetIntEnState=%#x\n", regValue);
}

HI_VOID HAL_Cipher_GetRawIntState(HI_U32 *pState)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = CIPHER_REG_INT_RAW;
    
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    if (pState)
    {
        *pState = regValue;
    }

    HI_INFO_CIPHER("HAL_Cipher_GetRawIntState=%#x\n", regValue);
}

HI_VOID HAL_Cipher_ClrIntState(HI_U32 intStatus)
{
    HI_U32 regAddr;
    HI_U32 regValue;

    regAddr = CIPHER_REG_INT_RAW;
    regValue = intStatus;
    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);
}

HI_VOID HAL_Cipher_EnableAllSecChn(HI_VOID)
{
	HI_U32 regAddr = CIPHER_REG_SEC_CHN_CFG;
	HI_U32 regValue = 0;

	regValue = 0xffffffff;
    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);
	return;
}

HI_S32 HAL_Cipher_GetRandomNumber(CIPHER_RNG_S *pstRNG)
{
    HI_U32 u32RngStat = 0;

	if(NULL == pstRNG)
	{
        HI_ERR_CIPHER("Invalid params!\n");
        return HI_FAILURE;
	}

	if(0 == pstRNG->u32TimeOutUs)
	{
		/* low 3bit(RNG_data_count[2:0]), indicate how many RNGs in the fifo is available now */
	    (HI_VOID)HAL_CIPHER_ReadReg(REG_RNG_STAT_ADDR, &u32RngStat);
	    if((u32RngStat & 0x7) <= 0)
	    {
	        return HI_ERR_CIPHER_NO_AVAILABLE_RNG;
	    }
	}
	else
	{
	    while(1)
	    {
	    	/* low 3bit(RNG_data_count[2:0]), indicate how many RNGs in the fifo is available now */
	        (HI_VOID)HAL_CIPHER_ReadReg(REG_RNG_STAT_ADDR, &u32RngStat);
	        if((u32RngStat & 0x7) > 0)
	        {
	            break;
	        }

			msleep(1);
	    }
	}

    (HI_VOID)HAL_CIPHER_ReadReg(REG_RNG_NUMBER_ADDR, &pstRNG->u32RNG);

    return HI_SUCCESS;
}

HI_VOID HAL_Cipher_Init(void)
{
    HI_U32 CipherCrgValue;
    HI_U32 pvirt;
    HI_U32 u32RngCtrl = 0;

    /* RNG init*/
    (HI_VOID)HAL_CIPHER_ReadReg(REG_RNG_BASE_ADDR, &u32RngCtrl);
    u32RngCtrl &= 0xfffffffc;
    u32RngCtrl |= 0x2;		/* select rng source 0x2, but 0x03 is ok too */

    /* config post_process_depth */
    u32RngCtrl |= 0x00009000;

    /* config post_process_enable andd drop_enable */
    u32RngCtrl |= 0x000000a0;
    (HI_VOID)HAL_CIPHER_WriteReg(REG_RNG_BASE_ADDR, u32RngCtrl);

    pvirt = (HI_U32)ioremap_nocache(REG_CRG_CLK_PHY_ADDR_CIPHER, 0x100);
	if (pvirt == 0)
    {
        HI_ERR_CIPHER("ioremap_nocache phy addr err:%x.\n",  REG_CRG_CLK_PHY_ADDR_CIPHER);
        return ;
    }

    HAL_CIPHER_ReadReg(pvirt, &CipherCrgValue);       
    HAL_SET_BIT(CipherCrgValue, CRG_RST_BIT_CIPHER); /* reset */ 
    HAL_SET_BIT(CipherCrgValue, CRG_CLK_BIT_CIPHER); /* set the bit 0, clock opened */    
    HAL_CIPHER_WriteReg(pvirt,CipherCrgValue);    

    /* clock select and cancel reset 0x30100*/    
    HAL_CLEAR_BIT(CipherCrgValue, CRG_RST_BIT_CIPHER); /* cancel reset */    
    HAL_SET_BIT(CipherCrgValue, CRG_CLK_BIT_CIPHER);   /* set the bit 0, clock opened */        
    HAL_CIPHER_WriteReg(pvirt,CipherCrgValue);    
    iounmap((volatile void __iomem *)pvirt);

    /* hash sw reset */  
#ifdef CFG_HI_CIPHER_HASH_SUPPORT
    (HI_VOID)HAL_Cipher_HashSoftReset();
#endif

#ifdef CFG_HI_CIPHER_RSA_SUPPORT
    HAL_RSA_Reset();
#endif

#ifdef CFG_HI_CIPHER_RNG_SUPPORT
    HAL_RNG_Reset();
#endif

    return;
}

HI_VOID HAL_Cipher_DeInit(void)
{
    HI_U32 u32RngCtrl = 0;
    HI_U32 CipherCrgValue;
    HI_U32 pvirt;

    (HI_VOID)HAL_CIPHER_ReadReg(REG_RNG_BASE_ADDR, &u32RngCtrl);
    u32RngCtrl &= 0xfffffffc;
    (HI_VOID)HAL_CIPHER_WriteReg(REG_RNG_BASE_ADDR, u32RngCtrl);

    pvirt = (HI_U32)ioremap_nocache(REG_CRG_CLK_PHY_ADDR_CIPHER, 0x100);
	if (pvirt == 0)
    {
        HI_ERR_CIPHER("ioremap_nocache phy addr err:%x.\n", REG_CRG_CLK_PHY_ADDR_CIPHER);
        return ;
    }
	
    HAL_CIPHER_ReadReg(pvirt, &CipherCrgValue);       
    HAL_SET_BIT(CipherCrgValue, 0); /* reset */ 
    HAL_SET_BIT(CipherCrgValue, 1); /* set the bit 0, clock opened */    
    HAL_CIPHER_WriteReg(pvirt,CipherCrgValue);    

    iounmap((volatile void *)pvirt);
    
    return;
}

HI_S32 HAL_CIPHER_ProcGetStatus(CIPHER_CHN_STATUS_S *pstCipherStatus)
{
    HI_U32 u32RegAddr = 0;
    HI_U32 u32RegValue = 0;
	HI_UNF_CIPHER_ALG_E enAlg;
	HI_UNF_CIPHER_WORK_MODE_E enMode;
	HI_UNF_CIPHER_KEY_LENGTH_E enKeyLen;
	HI_BOOL bKeyFrom;
	HI_U32 i = 0;

	if(NULL == pstCipherStatus)
	{
		HI_ERR_CIPHER("HAL_CIPHER_ProcGetStatus failed!\n");
		return HI_FAILURE;
	}

	for(i = 0; i < 8; i++)
	{
	/* get cipher ctrl */
		if(0 != i)
		{
			u32RegAddr = CIPHER_REG_CHANn_CIPHER_CTRL(i);
		}
		else
		{
			u32RegAddr = CIPHER_REG_CHAN0_CIPHER_CTRL;
		}
		
		(HI_VOID)HAL_CIPHER_ReadReg(u32RegAddr, &u32RegValue);
		pstCipherStatus[i].bIsDecrypt = u32RegValue & 1;

		enAlg = (u32RegValue >> 4) & 0x3;
		switch(enAlg)
		{
		    case HI_UNF_CIPHER_ALG_DES:
			{
				pstCipherStatus[i].ps8Alg = "DES ";
				break;
			}
		    case HI_UNF_CIPHER_ALG_3DES:
			{
				pstCipherStatus[i].ps8Alg = "3DES";
				break;
			}
		    case HI_UNF_CIPHER_ALG_AES:
			{
				pstCipherStatus[i].ps8Alg = "AES ";
				break;
			}
			default:
			{
				pstCipherStatus[i].ps8Alg = "BUTT";
				break;
			}
		}
		enMode = (u32RegValue >> 1) & 0x7;
		switch(enMode)
		{
		    case HI_UNF_CIPHER_WORK_MODE_ECB:
			{
				pstCipherStatus[i].ps8Mode= "ECB ";
				break;
			}
		    case HI_UNF_CIPHER_WORK_MODE_CBC:
			{
				pstCipherStatus[i].ps8Mode = "CBC ";
				break;
			}
		    case HI_UNF_CIPHER_WORK_MODE_CFB:
			{
				pstCipherStatus[i].ps8Mode = "CFB ";
				break;
			}
		    case HI_UNF_CIPHER_WORK_MODE_OFB:
			{
				pstCipherStatus[i].ps8Mode = "OFB ";
				break;
			}
		    case HI_UNF_CIPHER_WORK_MODE_CTR:
			{
				pstCipherStatus[i].ps8Mode = "CTR ";
				break;
			}
            case HI_UNF_CIPHER_WORK_MODE_CCM:
			{
				pstCipherStatus[i].ps8Mode = "CCM ";
				break;
			}
            case HI_UNF_CIPHER_WORK_MODE_GCM:
			{
				pstCipherStatus[i].ps8Mode = "GCM ";
				break;
			}
		    default:
			{
				pstCipherStatus[i].ps8Mode = "BUTT";
				break;
			}
		}

		enKeyLen = (u32RegValue >> 9) & 0x3;
		if(enAlg == HI_UNF_CIPHER_ALG_AES)
		{
			switch(enKeyLen)
			{
			    case HI_UNF_CIPHER_KEY_AES_128BIT:
				{
					pstCipherStatus[i].u32KeyLen = 128;
					break;
				}
			    case HI_UNF_CIPHER_KEY_AES_192BIT:
				{
					pstCipherStatus[i].u32KeyLen = 192;
					break;
				}
			    case HI_UNF_CIPHER_KEY_AES_256BIT:
				{
					pstCipherStatus[i].u32KeyLen = 256;
					break;
				}
				default:
				{
					pstCipherStatus[i].u32KeyLen = 0;
					break;
				}
			}
		}
		else
		{
			switch(enKeyLen)
			{
			    case HI_UNF_CIPHER_KEY_DES_3KEY:
				{
					pstCipherStatus[i].u32KeyLen = 3;
					break;
				}
			    case HI_UNF_CIPHER_KEY_DES_2KEY:
				{
					pstCipherStatus[i].u32KeyLen = 2;
					break;
				}
			    default:
				{
					pstCipherStatus[i].u32KeyLen = 0;
					break;
				}
			}
		}
		
		bKeyFrom = (u32RegValue >> 13) & 1;
		if(0 == bKeyFrom)
		{
			pstCipherStatus[i].ps8KeyFrom = "SW";
		}
		else
		{
			pstCipherStatus[i].ps8KeyFrom = "HW";
		}
 
 		/* get data in */
		if(0 != i)
		{
		    u32RegAddr = CIPHER_REG_CHANn_SRC_LST_SADDR(i);
		    (HI_VOID)HAL_CIPHER_ReadReg(u32RegAddr, &u32RegValue);
 			pstCipherStatus[i].u32DataInAddr = u32RegValue;
		}
		else
		{
			u32RegAddr = CIPHER_REG_CHAN0_CIPHER_DIN(0);
			pstCipherStatus[0].u32DataInAddr = u32RegAddr;
		}

 		/* get data out */
		if(0 != i)
		{
		    u32RegAddr = CIPHER_REG_CHANn_DEST_LST_SADDR(i);
		    (HI_VOID)HAL_CIPHER_ReadReg(u32RegAddr, &u32RegValue);

 			pstCipherStatus[i].u32DataOutAddr = u32RegValue;
		}
		else
		{
			u32RegAddr = CIPHER_REG_CHAN0_CIPHER_DOUT(0);
			pstCipherStatus[0].u32DataOutAddr = u32RegAddr;
		}

	/* get INT RAW status */
	    u32RegAddr = CIPHER_REG_INT_RAW;
	    (HI_VOID)HAL_CIPHER_ReadReg(u32RegAddr, &u32RegValue);
		pstCipherStatus[i].bInINTRaw = ( u32RegValue >> i ) & 0x1;
		pstCipherStatus[i].bOutINTRaw = ( u32RegValue >> (i + 8) ) & 0x1;

	/* get INT EN status */
	    u32RegAddr = CIPHER_REG_INT_EN;
	    (HI_VOID)HAL_CIPHER_ReadReg(u32RegAddr, &u32RegValue);
		pstCipherStatus[i].bInINTEn= ( u32RegValue >> i ) & 0x1;
		pstCipherStatus[i].bOutINTEn= ( u32RegValue >> (i + 8) ) & 0x1;
		pstCipherStatus[i].bInINTAllEn= ( u32RegValue >> (i + 31) ) & 0x1;

	/* get INT_OINTCFG */
		u32RegAddr = CIPHER_REG_CHANn_INT_OCNTCFG(i);
	    (HI_VOID)HAL_CIPHER_ReadReg(u32RegAddr, &u32RegValue);
		pstCipherStatus[i].u32OutINTCount = u32RegValue;
	}

	return HI_SUCCESS;
}

HI_VOID HAL_RNG_Reset(HI_VOID)
{
    HI_U32 u32Value;
    HI_U32 u32Virt;

    if (REG_CRG_CLK_PHY_ADDR_RNG == 0x00)
    {
        return;
    }
    
    u32Virt = (HI_U32)ioremap_nocache(REG_CRG_CLK_PHY_ADDR_RNG, 0x100);

    HAL_CIPHER_ReadReg(u32Virt, &u32Value);       
    HAL_SET_BIT(u32Value, CRG_RST_BIT_RNG); /* reset */ 
    HAL_SET_BIT(u32Value, CRG_CLK_BIT_RNG); /* clock opened */    
    HAL_CIPHER_WriteReg(u32Virt, u32Value);    

    /* clock select and cancel reset */    
    HAL_CLEAR_BIT(u32Value, CRG_RST_BIT_RNG); /* cancel reset */    
    HAL_SET_BIT(u32Value, CRG_CLK_BIT_RNG);   /* clock opened */        
    HAL_CIPHER_WriteReg(u32Virt, u32Value);    

    iounmap((void __iomem *)u32Virt);
    
}

HI_VOID HAL_RSA_Reset(HI_VOID)
{
    HI_U32 u32Value;
    HI_U32 u32Virt;

    if (REG_CRG_CLK_PHY_ADDR_RSA == 0x00)
    {
        return;
    }
    
    u32Virt = (HI_U32)ioremap_nocache(REG_CRG_CLK_PHY_ADDR_RSA, 0x100);

    HAL_CIPHER_ReadReg(u32Virt, &u32Value);       
    HAL_SET_BIT(u32Value, CRG_RST_BIT_RSA); /* reset */ 
    HAL_SET_BIT(u32Value, CRG_CLK_BIT_RSA); /* clock opened */    
    HAL_CIPHER_WriteReg(u32Virt, u32Value);    

    /* clock select and cancel reset */    
    HAL_CLEAR_BIT(u32Value, CRG_RST_BIT_RSA); /* cancel reset */    
    HAL_SET_BIT(u32Value, CRG_CLK_BIT_RSA);   /* clock opened */        
    HAL_CIPHER_WriteReg(u32Virt, u32Value);    

    iounmap((void __iomem *)u32Virt);
    
}

HI_VOID HAL_RSA_Start(HI_VOID)
{
    HAL_CIPHER_WriteReg(SEC_RSA_START_REG, 0x01);
}

HI_S32 HAL_RSA_WaitFree(HI_VOID)
{
	HI_U32 u32Value;
    HI_U32 u32TryCount = 0;

	do {

        HAL_CIPHER_ReadReg(SEC_RSA_BUSY_REG, &u32Value);
		if ((u32Value & RSA_BUSY) == 0)
		{
		    return HI_SUCCESS;
        }
		u32TryCount++;
        msleep(1);
	} while (u32TryCount < RSA_RTY_CNT);

	return HI_FAILURE;
}

HI_VOID HAL_RSA_ClearRam(HI_VOID)
{
	HI_U32 u32Value;

    u32Value = RSA_DATA_CLR_INPUT | RSA_DATA_CLR_OUTPUT | RSA_DATA_CLR_KEY | RSA_MOD_SEL_RAM_CLAER;
    HAL_CIPHER_WriteReg(SEC_RSA_MOD_REG, u32Value);
}

HI_VOID HAL_RSA_ConfigMode(CIPHER_RSA_KEY_WIDTH_E enKenWidth)
{
	HI_U32 u32Value;

    u32Value = (enKenWidth << 2) | RSA_MOD_SEL_OPT;
    HAL_CIPHER_WriteReg(SEC_RSA_MOD_REG, u32Value);
}

HI_VOID HAL_RSA_WriteData(CIPHER_RSA_DATA_TYPE_E enDataType, HI_U8 *pu8Data, HI_U32 u32DataLen, HI_U32 u32Len)
{
	HI_U32 u32Value;
    HI_U32 u32Reg;
    HI_U8 *pPos;
    HI_U32 i= 0;

    if (enDataType == CIPHER_RSA_DATA_TYPE_CONTEXT)
    {
        u32Reg = SEC_RSA_WDAT_REG;
    }
    else
    {
        u32Reg = SEC_RSA_WSEC_REG;
    }

    pPos = pu8Data;
    /* write following aligned bytes */
    for(i=0; i<u32Len; i+=4)
    {
        u32Value  = (HI_U32)pPos[0];
        u32Value |= ((HI_U32)pPos[1]) << 8;
        u32Value |= ((HI_U32)pPos[2]) << 16;
        u32Value |= ((HI_U32)pPos[3]) << 24;
        HAL_CIPHER_WriteReg(u32Reg, u32Value); 
        pPos+=4; 
    }
}

HI_VOID HAL_RSA_ReadData(HI_U8 *pu8Data, HI_U32 u32DataLen, HI_U32 u32Klen)
{
	HI_U32 u32Value;
    HI_U8 *pPos;
    HI_U32 i = 0;

    pPos = pu8Data;
   /* 
    for(i=0; i<(u32Klen - u32DataLen); i+=4)
    {
        HAL_CIPHER_ReadReg(SEC_RSA_RRSLT_REG, &u32Value);  
    }*/
    for(i=0; i<u32Klen; i+=4)
    {
        HAL_CIPHER_ReadReg(SEC_RSA_RRSLT_REG, &u32Value);  
        pPos[0] = (HI_U8)(u32Value & 0xFF);
        pPos[1] = (HI_U8)((u32Value >> 8) & 0xFF);
        pPos[2] = (HI_U8)((u32Value >> 16) & 0xFF);
        pPos[3] = (HI_U8)((u32Value >> 24) & 0xFF);
        pPos+=4; 
    }  
}

HI_U32 HAL_RSA_GetErrorCode(HI_VOID)
{
	HI_U32 u32Value;

    HAL_CIPHER_ReadReg(SEC_RSA_ERROR_REG, &u32Value);

    return u32Value;
}

