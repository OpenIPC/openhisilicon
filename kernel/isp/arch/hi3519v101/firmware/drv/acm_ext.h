/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_ext.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/07/17
  Description   : 
  History       :
  1.Date        : 2013/07/17
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#ifndef __ACM_EXT_H__
#define __ACM_EXT_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define COEFACMCNT 850
#define ADDROFFSET (9*13*29*3*2)
#define LUTDEFNUM  (9*13*29)
#define ACMADDROFFSET 850*16

typedef struct hi_ISP_ACM_CTX_S
{
    ISP_ACM_ATTR_S        stAcmCtrl   ;
    ISP_ACM_BUFFER_S     stMmzBuff   ;
    ISP_ACM_BUFFER_S     stCoefBuff  ;
    ISP_ACM_LUT_S          *pstCoefLut  ;
    struct osal_semaphore      stAcmSem    ;
}ISP_ACM_CTX_S;

typedef struct 
{
    HI_S32    bits_0        :    9    ;
    HI_S32    bits_1        :    9    ;
    HI_S32    bits_2        :    9    ;
    HI_S32    bits_35       :    5    ;
    
    HI_S32    bits_34       :    4    ;
    HI_S32    bits_4        :    7    ;
    HI_S32    bits_5        :    7    ;
    HI_S32    bits_6        :    9    ;
    HI_S32    bits_75       :    5    ;
    
    HI_S32    bits_74       :    4    ;
    HI_S32    bits_8        :    9    ;
    HI_S32    bits_9        :    9    ;
    HI_S32    bits_10       :    7    ;
    HI_S32    bits_113      :    3    ;
    
    HI_S32    bits_114      :    4    ;
    HI_S32    bits_12       :    28    ;

} ACM_COEF_BIT_S;

typedef struct
{
    HI_U32          u32Size;
    ACM_COEF_BIT_S  stBit[COEFACMCNT]; 
} ACM_COEF_BITARRAY_S;

typedef struct
{
    const HI_S16 *ps16Luma;
    const HI_S16 *ps16Sat;
    const HI_S16 *ps16Hue;
} ACM_PSCoef;

extern HI_S32 ISP_ACM_DRV_Init(ISP_DEV IspDev);
extern HI_S32 ISP_ACM_DRV_Exit(ISP_DEV IspDev);
extern HI_S32 ISP_ACM_DRV_Ioctrl(unsigned int cmd, unsigned long arg, void *private_data);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
