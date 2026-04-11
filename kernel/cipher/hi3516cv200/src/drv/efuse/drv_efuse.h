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

#ifndef __DRV_EFUSE_H__
#define __DRV_EFUSE_H__

/* add include here */
#include "hal_cipher.h"
#include "drv_cipher_ioctl.h"
#include "drv_cipher_mmz.h"
#include "drv_cipher_log.h"
#include "hi_error_mpi.h"
#include "hi_drv_cipher.h"


#ifdef __cplusplus
extern "C" {
#endif



/***************************** Macro Definition ******************************/
HI_S32 HI_DRV_CIPHER_WriteOTPKey(CIPHER_OTP_KEY_S *pstOtpKey);


#ifdef __cplusplus
}
#endif
#endif /* __DRV_EFUSE_H__ */

