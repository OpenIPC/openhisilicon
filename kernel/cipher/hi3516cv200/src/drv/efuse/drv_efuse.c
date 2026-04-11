/*********************************************************************************
*
*  Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
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
#include "hi_type.h"
#include "drv_cipher_ioctl.h"
#include "hal_efuse.h"
#include "drv_efuse.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
DECLARE_MUTEX(g_OtpMutexKernel);
#else
DEFINE_SEMAPHORE(g_OtpMutexKernel);
#endif


HI_S32 HI_DRV_CIPHER_WriteOTPKey(CIPHER_OTP_KEY_S *pstOtpKey)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8  au8OtpKey[16] = {0};

    if(HI_NULL == pstOtpKey)
    {
        HI_ERR_CIPHER("NULL point!\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    if (pstOtpKey->u32OtpId > 3)
    {
        HI_ERR_CIPHER("The opt id must be {0,1,2,3}!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (pstOtpKey->u32KeyLen != 16)
    {
        HI_ERR_CIPHER("The length of key must be less than or equal to 16!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

	if(down_interruptible(&g_OtpMutexKernel))
	{
		HI_ERR_CIPHER("down_interruptible failed!\n");
		return HI_FAILURE;
	}

    if ( 0 != copy_from_user(au8OtpKey, (HI_U8*)pstOtpKey->pu8Key, pstOtpKey->u32KeyLen))
    {
		HI_ERR_CIPHER("Invalid pu8Key!\n");
        up(&g_OtpMutexKernel);
		return HI_FAILURE;
    }

	s32Ret = HAL_Efuse_WriteKey((HI_U32*)au8OtpKey, pstOtpKey->u32OtpId);

	up(&g_OtpMutexKernel);

	return s32Ret;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

