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

#ifndef __DRV_CIPHER_SHA__
#define __DRV_CIPHER_SHA__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CIPHER_HASH_SUPPORT
HI_VOID HASH_DRV_ModInit(HI_VOID);
HI_VOID HASH_DRV_ModDeInit(HI_VOID);

HI_S32 HAL_Cipher_CalcHashInit(CIPHER_HASH_DATA_S *pCipherHashData);
HI_S32 HAL_Cipher_CalcHashUpdate(CIPHER_HASH_DATA_S *pCipherHashData);
HI_S32 HAL_Cipher_CalcHashFinal(CIPHER_HASH_DATA_S *pCipherHashData);
HI_S32 HAL_Cipher_HashSoftReset(HI_VOID);
#endif

#ifdef __cplusplus
}
#endif

#endif /* sha2.h */
