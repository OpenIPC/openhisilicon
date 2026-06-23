/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef PWM_HAL_H
#define PWM_HAL_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define PWMI_ADRESS_BASE 0x11082000  // PWD 2 Channel 0
#define PWM_NUM_MAX      0x05
#define PWM_REG_OFFSET   0x100
#define PWM2_CHN0_WORK_MODE     0x14
/* PWMI */
#define ot_io_pwmi_address(x) ((void IOMEM *)((unsigned char IOMEM *)g_reg_pwm_ibase + ((x)-(PWMI_ADRESS_BASE))))

#define pwm_cfg_reg0(idx)   ot_io_pwmi_address(PWMI_ADRESS_BASE + (0x0000 + (idx) * PWM_REG_OFFSET))
#define pwm_cfg_reg1(idx)   ot_io_pwmi_address(PWMI_ADRESS_BASE + (0x0004 + (idx) * PWM_REG_OFFSET))
#define pwm_cfg_reg2(idx)   ot_io_pwmi_address(PWMI_ADRESS_BASE + (0x0010 + (idx) * PWM_REG_OFFSET))
#define pwm_ctrl_reg(idx)   ot_io_pwmi_address(PWMI_ADRESS_BASE + (0x0014 + (idx) * PWM_REG_OFFSET))

#define PWM2_CHN0_CFG_REG0   ot_io_pwmi_address(PWMI_ADRESS_BASE + 0x0000)
#define PWM2_CHN0_CFG_REG1   ot_io_pwmi_address(PWMI_ADRESS_BASE + 0x0004)
#define PWM2_CHN0_CFG_REG2   ot_io_pwmi_address(PWMI_ADRESS_BASE + 0x0010)
#define PWM2_CHN0_CTRL_REG   ot_io_pwmi_address(PWMI_ADRESS_BASE + 0x0014)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* PWM_HAL_H */
