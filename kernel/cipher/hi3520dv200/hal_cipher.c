/**
\file
\brief cipher hal interface
\copyright Shenzhen Hisilicon Co., Ltd.
\date 2008-2018
\version draft
\author QuYaxin 46153
\date 2009-11-3
*/
#include <asm/io.h>

#include "hi_type.h"
#include "hi_error_ecs.h"

#include "hal_cipher.h"
#include "priv_cipher.h"
#include "cipher_log.h"


/***************************** Macro Definition ******************************/
/* cipher reg read and write macro define */
#ifndef CIPHER_READ_REG
#define CIPHER_READ_REG(reg, result) ((result) = *(volatile unsigned int *)(reg))
#endif

#ifndef CIPHER_WRITE_REG
#define CIPHER_WRITE_REG(reg, data) (*((volatile unsigned int *)(reg)) = (data))
#endif

/*process of bit*/
#define HAL_SET_BIT(src, bit)        ((src) |= (1<<bit))
#define HAL_CLEAR_BIT(src,bit)      ((src) &= ~(1<<bit))

static HI_U32 REG_CI_BASE_ADDR = 0;
//static volatile void __iomem *REG_CI_BASE_ADDR = NULL;

/*************************** Structure Definition ****************************/

/******************************* API declaration *****************************/

HI_S32 HAL_Cipher_SetInBufNum(HI_U32 chnId, HI_U32 num)
{
    HI_U32 regAddr = 0;

    if (CI_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /* register0~15 bit is valid, others bits reserved */
    regAddr = REG_CI_CHANn_IBUF_NUM(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%#x, set to 0xffff\n", num);
        num = 0xffff;
    }

    CIPHER_WRITE_REG(regAddr, num);
    
    HI_INFO_CIPHER(" cnt=%u\n", num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetInBufNum(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 regAddr  = 0;
    HI_U32 regValue = 0;

    if (CI_PKGx1_CHAN == chnId || HI_NULL == pNum)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = REG_CI_CHANn_IBUF_NUM(chnId);
    CIPHER_READ_REG(regAddr, regValue);
    
    *pNum = regValue;

    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetInBufCnt(HI_U32 chnId, HI_U32 num)
{
    HI_U32 regAddr = 0;

    if (CI_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = REG_CI_CHANn_IBUF_CNT(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    CIPHER_WRITE_REG(regAddr, num);
    
    HI_INFO_CIPHER(" HAL_Cipher_SetInBufCnt=%u\n", num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetInBufCnt(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 regAddr;
    HI_U32 regValue;

    if ( (CI_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = REG_CI_CHANn_IBUF_CNT(chnId);
    CIPHER_READ_REG(regAddr, regValue);
    *pNum = regValue;
    
    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetInBufEmpty(HI_U32 chnId, HI_U32 num)
{
    HI_U32 regAddr;

    if (CI_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = REG_CI_CHANn_IEMPTY_CNT(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    CIPHER_WRITE_REG(regAddr, num);
    
    HI_INFO_CIPHER(" cnt=%u\n", num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetInBufEmpty(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 regAddr;
    HI_U32 regValue;

    if ( (CI_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = REG_CI_CHANn_IEMPTY_CNT(chnId);
    CIPHER_READ_REG(regAddr, regValue);
    
    *pNum = regValue;
    
    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetOutBufNum(HI_U32 chnId, HI_U32 num)
{
    HI_U32 regAddr;

    if (CI_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = REG_CI_CHANn_OBUF_NUM(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    CIPHER_WRITE_REG(regAddr, num);
    
    HI_INFO_CIPHER("chn=%d cnt=%u\n", chnId, num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetOutBufNum(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 regAddr;
    HI_U32 regValue;

    if ( (CI_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = REG_CI_CHANn_OBUF_NUM(chnId);
    CIPHER_READ_REG(regAddr, regValue);

    *pNum = regValue;

    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetOutBufCnt(HI_U32 chnId, HI_U32 num)
{
    HI_U32 regAddr;

    if (CI_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = REG_CI_CHANn_OBUF_CNT(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    CIPHER_WRITE_REG(regAddr, num);
    
    HI_INFO_CIPHER("SetOutBufCnt=%u, chnId=%u\n", num,chnId);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetOutBufCnt(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 regAddr;
    HI_U32 regValue;

    if ( (CI_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = REG_CI_CHANn_OBUF_CNT(chnId);
    CIPHER_READ_REG(regAddr, regValue);

    *pNum = regValue;

    HI_INFO_CIPHER(" HAL_Cipher_GetOutBufCnt=%u\n", regValue);
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetOutBufFull(HI_U32 chnId, HI_U32 num)
{
    HI_U32 regAddr;

    if (CI_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = REG_CI_CHANn_OFULL_CNT(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    CIPHER_WRITE_REG(regAddr, num);
    
    HI_INFO_CIPHER(" cnt=%u\n", num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetOutBufFull(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 regAddr;
    HI_U32 regValue;

    if ( (CI_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = REG_CI_CHANn_OFULL_CNT(chnId);
    CIPHER_READ_REG(regAddr, regValue);

    *pNum = regValue;

    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_WaitIdle(HI_VOID)
{
    HI_S32 i;
    HI_U32 u32RegAddr;
    HI_U32 u32RegValue;

    /* channel 0 configuration register [31-2]:reserved, [1]:ch0_busy, [0]:ch0_start 
         * [1]:channel 0 status signal, [0]:channel 0 encrypt/decrypt start signal
         */
    u32RegAddr = REG_CI_CHAN0_CFG;

    for (i = 0; i < CIPHER_WAIT_IDEL_TIMES; i++)
    {
        CIPHER_READ_REG(u32RegAddr, u32RegValue);
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
    HI_U32 u32RegValue;

    HI_ASSERT(CI_PKGx1_CHAN == chn);

    CIPHER_READ_REG(REG_CI_CHAN0_CFG, u32RegValue);
    if (0x0 == ((u32RegValue >> 1) & 0x01))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_S32 HAL_Cipher_SetDataSinglePkg(HI_UNF_CIPHER_INFO_S * info)
{
    HI_U32 regAddr;
    HI_U32 i;

    regAddr = REG_CI_CHAN0_CIPHER_DIN(0);
    
    /***/
    for (i = 0; i < (16/sizeof(HI_U32)); i++)
    {
        CIPHER_WRITE_REG(regAddr + (i * sizeof(HI_U32)), (*(info->u32DataPkg + i)) );
    }
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_ReadDataSinglePkg(HI_U32 *pData)
{
    HI_U32 regAddr;
    HI_U32 i;

    regAddr = REG_CI_CHAN0_CIPHER_DOUT(0);
    
    /***/
    for (i = 0; i < (16/sizeof(HI_U32)); i++)
    {
        CIPHER_READ_REG(regAddr + (i * sizeof(HI_U32)), (*(pData+ i)) );
    }
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_StartSinglePkg(HI_U32 chnId)
{
    HI_U32 u32RegAddr;
    HI_U32 u32RegValue;

    HI_ASSERT(CI_PKGx1_CHAN == chnId);

    u32RegAddr = REG_CI_CHAN0_CFG;
    CIPHER_READ_REG(u32RegAddr, u32RegValue);
    
    u32RegValue |= 0x1;
    CIPHER_WRITE_REG(u32RegAddr, u32RegValue); /* start work */
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetBufAddr(HI_U32 chnId, CIPHER_BUF_TYPE_E bufType, HI_U32 addr)
{
    HI_U32 regAddr;

    if (CI_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (CIPHER_BUF_TYPE_IN == bufType)
    {
        regAddr = REG_CI_CHANn_SRC_LST_SADDR(chnId);
    }
    else if (CIPHER_BUF_TYPE_OUT == bufType)
    {
        regAddr = REG_CI_CHANn_DEST_LST_SADDR(chnId);
    }
    else
    {
        HI_ERR_CIPHER("SetBufAddr type err:%x.\n", bufType);
        return HI_ERR_CIPHER_INVALID_PARA;
    }


    HI_INFO_CIPHER("Set chn%d '%s' BufAddr to:%x.\n",chnId,
        (CIPHER_BUF_TYPE_IN == bufType)?"In":"Out",  addr);

    CIPHER_WRITE_REG(regAddr, addr);

    return HI_SUCCESS;
}



HI_VOID HAL_Cipher_Reset(HI_VOID)
{

    //CIPHER_WRITE_REG(CIPHER_SOFT_RESET_ADDR, 1);
    return;
}

HI_S32 HAL_Cipher_GetOutIV(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 i;
    HI_U32 regAddr;

    if (CI_PKGx1_CHAN != chnId)
    {
        regAddr = REG_CI_CHAN0_CIPHER_IVIN(0);
    }
    else
    {
        regAddr = REG_CI_CHAN_CIPHER_IVOUT(chnId);
    }


    /***/
    for (i = 0; i < (CI_IV_SIZE/sizeof(HI_U32)); i++)
    {
        CIPHER_READ_REG(regAddr + (i * sizeof(HI_U32)), pCtrl->u32IV[i]);
    }

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetInIV(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 i;
    HI_U32 regAddr;

    if (CI_PKGx1_CHAN != chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = REG_CI_CHAN0_CIPHER_IVIN(0);

    /***/
    for (i = 0; i < (CI_IV_SIZE/sizeof(HI_U32)); i++)
    {
        CIPHER_WRITE_REG(regAddr + (i * sizeof(HI_U32)), pCtrl->u32IV[i]);
    }

    return HI_SUCCESS;
}

extern HI_S32 CA_DecryptCipher(HI_U32 AddrID, HI_U32 * pDataIn);

HI_S32 HAL_Cipher_SetKey(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 i = 0;
    HI_U32 regAddr = 0;

    regAddr = REG_CI_CIPHER_KEY(chnId);

    /***/
    if (HI_FALSE == pCtrl->bKeyByCA)
    {
        for (i = 0; i < (CI_KEY_SIZE/sizeof(HI_U32)); i++)
        {
            CIPHER_WRITE_REG(regAddr + (i * sizeof(HI_U32)), pCtrl->u32Key[i]);
        }
    }
    else
    {
    //    NOT to do this now!!!   
    //
    //    CA_DecryptCipher(chnId,pCtrl->u32Key); 
    }

    HI_INFO_CIPHER("SetKey: chn%u,Key:%#x, %#x, %#x, %#x.\n", chnId,
        pCtrl->u32Key[0], pCtrl->u32Key[1], pCtrl->u32Key[2], pCtrl->u32Key[3]);
    
    return HI_SUCCESS;
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
    HI_U32 keyId;
    HI_U32 regAddr;
    HI_U32 regValue;

    HI_BOOL bKeyByCA = pCtrl->bKeyByCA;

    if (CI_PKGx1_CHAN == chnId)
    {
        /* channel 0, single packet encrypt or decrypt channel */
        regAddr = REG_CI_CHAN0_CIPHER_CTRL;
    }
    else
    {
        regAddr = REG_CI_CHANn_CIPHER_CTRL(chnId);
    }

    CIPHER_READ_REG(regAddr, regValue);

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

//    regValue &= ~(0x01 << 8);
//    regValue |= ((bIVChange & 0x1) << 8);
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

    CIPHER_WRITE_REG(regAddr, regValue);

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetAGEThreshold(HI_U32 chnId, CIPHER_INT_TYPE_E intType, HI_U32 value)
{
    HI_U32 regAddr;

    if (CI_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (CIPHER_INT_TYPE_IN_BUF == intType)
    {
        regAddr = REG_CI_CHANn_IAGE_CNT(chnId);
    }
    else if (CIPHER_INT_TYPE_OUT_BUF == intType)
    {
        regAddr = REG_CI_CHANn_OAGE_CNT(chnId);
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

    CIPHER_WRITE_REG(regAddr, value);

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetIntThreshold(HI_U32 chnId, CIPHER_INT_TYPE_E intType, HI_U32 value)
{
    HI_U32 regAddr;

    if (CI_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (CIPHER_INT_TYPE_IN_BUF == intType)
    {
        regAddr = REG_CI_CHANn_INT_ICNTCFG(chnId);
    }
    else if (CIPHER_INT_TYPE_OUT_BUF == intType)
    {
        regAddr = REG_CI_CHANn_INT_OCNTCFG(chnId);
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

    CIPHER_WRITE_REG(regAddr, value);

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

    regAddr = REG_CI_INT_EN;
    CIPHER_READ_REG(regAddr, regValue);

    regValue |= (1 << 31); /* sum switch int_en */

    if (CI_PKGx1_CHAN == chnId)
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

    CIPHER_WRITE_REG(regAddr, regValue);

    HI_INFO_CIPHER("HAL_Cipher_EnableInt: Set INT_EN:%#x\n", regValue);

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_DisableInt(HI_U32 chnId, int intType)
{
    HI_U32 regAddr;
    HI_U32 regValue;

    regAddr = REG_CI_INT_EN;
    CIPHER_READ_REG(regAddr, regValue);

    if (CI_PKGx1_CHAN == chnId)
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

    CIPHER_WRITE_REG(regAddr, regValue);

    HI_INFO_CIPHER("HAL_Cipher_DisableInt: Set INT_EN:%#x\n", regValue);

    return HI_SUCCESS;
}

HI_VOID HAL_Cipher_DisableAllInt(HI_VOID)
{
    HI_U32 regAddr;
    HI_U32 regValue;

    regAddr = REG_CI_INT_EN;
    regValue = 0;
    CIPHER_WRITE_REG(regAddr, regValue);
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
    HI_U32 regAddr;
    HI_U32 regValue;

    regAddr = REG_CI_INT_STATUS;
    CIPHER_READ_REG(regAddr, regValue);

    if (pState)
    {
        *pState = regValue;
    }

   HI_INFO_CIPHER("HAL_Cipher_GetIntState=%#x\n", regValue);
}

HI_VOID HAL_Cipher_GetIntEnState(HI_U32 *pState)
{
    HI_U32 regAddr;
    HI_U32 regValue;

    regAddr = REG_CI_INT_EN;
    
    CIPHER_READ_REG(regAddr, regValue);

    if (pState)
    {
        *pState = regValue;
    }

   HI_INFO_CIPHER("HAL_Cipher_GetIntEnState=%#x\n", regValue);
}

HI_VOID HAL_Cipher_GetRawIntState(HI_U32 *pState)
{
    HI_U32 regAddr;
    HI_U32 regValue;

    regAddr = REG_CI_INT_RAW;
    
    CIPHER_READ_REG(regAddr, regValue);

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

    regAddr = REG_CI_INT_RAW;
    regValue = intStatus;
    CIPHER_WRITE_REG(regAddr, regValue);
}

HI_VOID HAL_Cipher_Init(void)
{
    HI_U32 CipherCrgValue;
    void __iomem *pvirt = NULL;

    REG_CI_BASE_ADDR = (HI_U32)ioremap_nocache(REG_CI_BASE_ADDR_PHY,REG_CI_BASE_SIZE);

#if 0 //by g00182102 fro debug clock register
    HI_U32 uCipherValue = 0;
#endif

    pvirt = ioremap_nocache(REG_SYS_CIPHER_CLK_ADDR_PHY,sizeof(HI_U32));

    CIPHER_READ_REG(pvirt,CipherCrgValue);
    
    HAL_SET_BIT(CipherCrgValue, 0); /* reset */
    HAL_SET_BIT(CipherCrgValue, 1); /* set the bit 1, clock opened */

    CIPHER_WRITE_REG(pvirt,CipherCrgValue);

    #if 0 //debug by g00182102 debug clock register
    CIPHER_READ_REG(IO_ADDRESS(REG_SYS_CIPHER_CLK_ADDR_PHY),uCipherValue);
    printk(KERN_WARNING "after reset clock register:0x%08x\n", uCipherValue);
    #endif
    
    /* clock select and cancel reset 0x30100*/
    HAL_CLEAR_BIT(CipherCrgValue, 0); /* cancel reset */
    HAL_SET_BIT(CipherCrgValue, 1);   /* set the bit 5, clock opened */
    
    CIPHER_WRITE_REG(pvirt,CipherCrgValue);

    #if 0 //debug by g00182102 debug clock register
    CIPHER_READ_REG(IO_ADDRESS(REG_SYS_CIPHER_CLK_ADDR_PHY),uCipherValue);
    printk(KERN_WARNING "after cancel reset clock register:0x%08x\n", uCipherValue);
    #endif
    
    if (pvirt != NULL)
    {
        iounmap(pvirt);
        pvirt = NULL;
    }
}

HI_VOID HAL_Cipher_DeInit(void)
{
    /* use the same clock of CA */
    //CIPHER_WRITE_REG(IO_ADDRESS(REG_SYS_CIPHER_CLK_ADDR_PHY), 0x0103); 
    //CIPHER_WRITE_REG(IO_ADDRESS(REG_SYS_CIPHER_CLK_ADDR_PHY), 0x3); /* close cipher's clock */
    if (REG_CI_BASE_ADDR != 0)
    {
        iounmap((void __iomem *)REG_CI_BASE_ADDR);
        REG_CI_BASE_ADDR = 0;
    }
}

