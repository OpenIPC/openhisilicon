/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : SAR_ADC_DRIVER.c
  Version       : Initial Draft
  Author        : Hisilicon BVT
  Created       : 2012/5/2
  Description   : 
  History       :
  1.Date        : 2012/5/2
    Author      : hkf67331
    Modification: Created file

******************************************************************************/
#include <linux/version.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0))
#include <asm/system.h>
#endif
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <mach/io.h>
#include "hi_type.h"

#include "drv_cipher_log.h"
#include "drv_cipher_reg.h"
#include "hal_efuse.h"

#define CIPHER_KD_WKEY0      IO_ADDRESS(REG_BASE_PHY_ADDR_EFUSE + 0x800)
#define CIPHER_KD_WKEY1      IO_ADDRESS(REG_BASE_PHY_ADDR_EFUSE + 0x804)
#define CIPHER_KD_WKEY2      IO_ADDRESS(REG_BASE_PHY_ADDR_EFUSE + 0x808)
#define CIPHER_KD_WKEY3      IO_ADDRESS(REG_BASE_PHY_ADDR_EFUSE + 0x80c)
#define CIPHER_KD_CTRL       IO_ADDRESS(REG_BASE_PHY_ADDR_EFUSE + 0x810)
#define CIPHER_KD_STA        IO_ADDRESS(REG_BASE_PHY_ADDR_EFUSE + 0x814)
#define OTP_PGM_TIME         IO_ADDRESS(REG_BASE_PHY_ADDR_EFUSE + 0x818)
#define OTP_RD_TIME          IO_ADDRESS(REG_BASE_PHY_ADDR_EFUSE + 0x81c)
#define OTP_LOGIC_LEVEL      IO_ADDRESS(REG_BASE_PHY_ADDR_EFUSE + 0x820)
#define KD_CTL_MODE_CIPHER_KEY_ADDR(chn_id)  (chn_id<<8)
#define KD_CTL_MODE_OPT_KEY_ADDR(opt_id)     (opt_id<<4)
#define KD_CTL_MODE_HASH_KL                   (0x8)
#define KD_CTL_MODE_OPT_KD                    (0x4)
#define KD_CTL_MODE_CIPHER_KL                 (0x2)
#define KD_CTL_MODE_START                     (0x1)
#define EFUSE_REG(RegAddr) *((volatile unsigned int *)(RegAddr))

#define  REG_SYS_EFUSE_CLK_ADDR_PHY     0x200300D8
#define  REG_SYS_EFUSE_CRL              IO_ADDRESS(REG_SYS_EFUSE_CLK_ADDR_PHY)

/* Define the union U_CIPHER_KD_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    start                 : 1   ; /* [0]  */
        unsigned int    cipher_kl_mode        : 1   ; /* [1]  */
        unsigned int    otp_kd_mode           : 1   ; /* [2]  */
        unsigned int    Reserved_2            : 1   ; /* [3]  */
        unsigned int    otp_key_add           : 2   ; /* [5..4]  */
        unsigned int    Reserved_1            : 2   ; /* [7..6]  */
        unsigned int    cipher_key_add        : 3   ; /* [10..8]  */
        unsigned int    Reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CIPHER_KD_CTRL;

/* Define the union U_CIPHER_KD_STA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cipher_kl_finish      : 1   ; /* [0]  */
        unsigned int    hash_key_read_busy    : 1  ;/* [1]  */
        unsigned int    Reserved_3             : 25  ; /* [26..2]  */
        unsigned int    ctrl_rdy               : 1   ; /* [27]  */
        unsigned int    ctrl_busy0             : 1   ; /* [28]  */
        unsigned int    ctrl_busy1             : 1   ; /* [29]  */
        unsigned int    key_wt_error           : 1   ; /* [30]  */
        unsigned int    key_wt_finish          : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CIPHER_KD_STA;

static HI_U32 s_bIsEfuseBusyFlag = HI_FALSE;

HI_VOID HAL_Efuse_Init(HI_VOID)
{
    HI_U32 CrgValue;

    CrgValue = EFUSE_REG(REG_SYS_EFUSE_CRL);  
    CrgValue |= 0x01;/* reset */ 
    CrgValue |= 0x02;   /* set the bit 0, clock opened */ 
    EFUSE_REG(REG_SYS_EFUSE_CRL) = CrgValue;    

    /* clock select and cancel reset 0x30100*/    
    CrgValue &= (~0x01); /* cancel reset */    
    CrgValue |= 0x02;   /* set the bit 0, clock opened */        
    EFUSE_REG(REG_SYS_EFUSE_CRL) = CrgValue;    

}

HI_S32 HAL_Efuse_WaitWriteKey(HI_VOID)
{
    U_CIPHER_KD_STA efuse_sta;
    HI_U32 ulStartTime = 0;
    HI_U32 ulLastTime = 0;
    HI_U32 ulDuraTime = 0;

    /* wait for hash_rdy */
    ulStartTime = jiffies;
    while(1)
    {
        efuse_sta.u32 = EFUSE_REG(CIPHER_KD_STA);
        if(efuse_sta.bits.key_wt_finish == 1)
        {
            break;
        }

        ulLastTime = jiffies;
        ulDuraTime = jiffies_to_msecs(ulLastTime - ulStartTime);
        if (ulDuraTime >= 50000 )
        { 
            HI_ERR_CIPHER("Error! efuse write key time out!\n");
            return HI_FAILURE;
        }

        msleep(100);
    }
    return HI_SUCCESS;
}

HI_S32 HAL_Efuse_WaitCipherLoadKey(HI_VOID)
{
    U_CIPHER_KD_STA efuse_sta;
    HI_U32 ulStartTime = 0;
    HI_U32 ulLastTime = 0;
    HI_U32 ulDuraTime = 0;

    ulStartTime = jiffies;

    while(1)
    {
        efuse_sta.u32 = EFUSE_REG(CIPHER_KD_STA);
        
        if(efuse_sta.bits.cipher_kl_finish == 1)
        {
            break;
        }

        ulLastTime = jiffies;
        ulDuraTime = jiffies_to_msecs(ulLastTime - ulStartTime);
        if (ulDuraTime >= 50000 )
        { 
            HI_ERR_CIPHER("Error! efuse load key out!\n");
            return HI_FAILURE;
        }
        msleep(100);
    }
    return HI_SUCCESS;
}

HI_S32 HAL_Efuse_WaitHashLoadKey(HI_VOID)
{
    U_CIPHER_KD_STA efuse_sta;
    HI_U32 ulStartTime = 0;
    HI_U32 ulLastTime = 0;
    HI_U32 ulDuraTime = 0;

    ulStartTime = jiffies;

    while(1)
    {
        efuse_sta.u32 = EFUSE_REG(CIPHER_KD_STA);
        
        if(efuse_sta.bits.hash_key_read_busy == 0)
        {
            break;
        }

        ulLastTime = jiffies;
        ulDuraTime = jiffies_to_msecs(ulLastTime - ulStartTime);
        if (ulDuraTime >= 50000 )
        { 
            HI_ERR_CIPHER("Error! efuse load key out!\n");
            return HI_FAILURE;
        }
        msleep(100);
    }
    return HI_SUCCESS;
}

HI_S32 HAL_Efuse_WaitReady(HI_VOID)
{
    U_CIPHER_KD_STA efuse_sta;
    HI_U32 ulStartTime = 0;
    HI_U32 ulLastTime = 0;
    HI_U32 ulDuraTime = 0;

    ulStartTime = jiffies;

    while(1)
    {
        efuse_sta.u32 = EFUSE_REG(CIPHER_KD_STA);
        
        if(efuse_sta.bits.ctrl_rdy && (!efuse_sta.bits.ctrl_busy1) && (!efuse_sta.bits.ctrl_busy0))
        {
            break;
        }

        ulLastTime = jiffies;
        ulDuraTime = jiffies_to_msecs(ulLastTime - ulStartTime);
        if (ulDuraTime >= 50000 )
        { 
            HI_ERR_CIPHER("Error! efuse load key out!\n");
            return HI_FAILURE;
        }
        msleep(100);
    }
    return HI_SUCCESS;
}

HI_S32 HAL_Efuse_GetErrStat(HI_VOID)
{
    U_CIPHER_KD_STA efuse_sta;

    efuse_sta.u32 = EFUSE_REG(CIPHER_KD_STA);
    
    return efuse_sta.bits.key_wt_error;
}

HI_S32 HAL_Efuse_WriteKey(HI_U32 * p_key, HI_U32 opt_id)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 kd_ctl_mode = 0;

    if(s_bIsEfuseBusyFlag)
        return HI_FAILURE;

    s_bIsEfuseBusyFlag = HI_TRUE;
    kd_ctl_mode = KD_CTL_MODE_OPT_KEY_ADDR(opt_id) | KD_CTL_MODE_OPT_KD | KD_CTL_MODE_START;

    EFUSE_REG(CIPHER_KD_WKEY0) = *p_key;
    EFUSE_REG(CIPHER_KD_WKEY1) = *(p_key+1);
    EFUSE_REG(CIPHER_KD_WKEY2) = *(p_key+2);
    EFUSE_REG(CIPHER_KD_WKEY3) = *(p_key+3);
    HAL_Efuse_WaitReady();
    EFUSE_REG(CIPHER_KD_CTRL)  = kd_ctl_mode;

	if(HAL_Efuse_WaitWriteKey())
	{
        s_bIsEfuseBusyFlag = HI_FALSE;
        return HI_FAILURE;
	}
        
    if(HAL_Efuse_GetErrStat())
    {
        HI_ERR_CIPHER("func:%s err, efuse key is already write.\n",__FUNCTION__);
        ret = HI_FAILURE;
    }

    s_bIsEfuseBusyFlag = HI_FALSE;
    
    return ret;
}

HI_S32 HAL_Efuse_LoadCipherKey(HI_U32 chn_id, HI_U32 opt_id)
{
    HI_U32 kd_ctl_mode = 0;

    if(s_bIsEfuseBusyFlag)
        return HI_FAILURE;

    s_bIsEfuseBusyFlag = HI_TRUE;

    kd_ctl_mode = (KD_CTL_MODE_CIPHER_KEY_ADDR(chn_id) | KD_CTL_MODE_OPT_KEY_ADDR(opt_id)\
                | KD_CTL_MODE_CIPHER_KL | KD_CTL_MODE_START);
    HAL_Efuse_WaitReady();
	EFUSE_REG(CIPHER_KD_CTRL) = kd_ctl_mode;
    if(HAL_Efuse_WaitCipherLoadKey())
    {
        s_bIsEfuseBusyFlag = HI_FALSE;
        return HI_FAILURE;
    }

    s_bIsEfuseBusyFlag = HI_FALSE;
    
    return HI_SUCCESS;
}

HI_S32 HAL_Efuse_LoadHashKey(HI_U32 opt_id)
{
    HI_U32 kd_ctl_mode = 0;

    if(s_bIsEfuseBusyFlag)
        return HI_FAILURE;

    s_bIsEfuseBusyFlag = HI_TRUE;
    
    kd_ctl_mode = (KD_CTL_MODE_OPT_KEY_ADDR(opt_id)| KD_CTL_MODE_HASH_KL | KD_CTL_MODE_START);
    HAL_Efuse_WaitReady();
	EFUSE_REG(CIPHER_KD_CTRL) = kd_ctl_mode;
    if(HAL_Efuse_WaitHashLoadKey())
	{
        s_bIsEfuseBusyFlag = HI_FALSE;
        return HI_FAILURE;
    }
    s_bIsEfuseBusyFlag = HI_FALSE;
    
    return HI_SUCCESS;
}


