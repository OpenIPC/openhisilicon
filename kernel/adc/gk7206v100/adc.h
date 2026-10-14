/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __ADC_H__
#define __ADC_H__
#include "drv_type_ioctl.h"
#include "xmedia_type.h"

typedef enum IOC_NR_LSADC_E
{
    IOC_NR_LSADC_MODEL_SEL = 0,
    IOC_NR_LSADC_CHN_ENABLE,
    IOC_NR_LSADC_CHN_DISABLE,
    IOC_NR_LSADC_START,
    IOC_NR_LSADC_STOP,
    IOC_NR_LSADC_GET_CHNVAL,
    IOC_NR_LSADC_BUTT
}IOC_NR_LSADC_E;

#define LSADC_IOC_MODEL_SEL         _IOWR(IOC_TYPE_LSADC, IOC_NR_LSADC_MODEL_SEL, xmedia_s32)
#define LSADC_IOC_CHN_ENABLE        _IOW(IOC_TYPE_LSADC, IOC_NR_LSADC_CHN_ENABLE, xmedia_s32)
#define LSADC_IOC_CHN_DISABLE       _IOW(IOC_TYPE_LSADC, IOC_NR_LSADC_CHN_DISABLE, xmedia_s32)
#define LSADC_IOC_START             _IO(IOC_TYPE_LSADC, IOC_NR_LSADC_START)
#define LSADC_IOC_STOP              _IO(IOC_TYPE_LSADC, IOC_NR_LSADC_STOP)
#define LSADC_IOC_GET_CHNVAL        _IOWR(IOC_TYPE_LSADC, IOC_NR_LSADC_GET_CHNVAL, xmedia_s32)




#endif /* __ADC_H__ */

