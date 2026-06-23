/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef OT_ADC_HAL_H
#define OT_ADC_HAL_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define LSADC_IRQ_ID        79
#define LSADC_BASE_ADDR     0x11100000
#define LSADC_REGS_LENGTH   0x100

#define LSADC_CRG_ADDR      0x110146C0
#define ADC_BIT             4
#define ADC_RESET_BIT       0

#define LSADC_MAX_CHN_NUM   4
#define LSADC_NUM_BITS      10
#define LSADC_CHN_MASK      0xF

#define GLITCH              3000
#define TIME_SCAN           0x20
#define LSADC_ZERO          0x1

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
