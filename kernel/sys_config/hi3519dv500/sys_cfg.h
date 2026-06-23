/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SYS_CFG_H
#define SYS_CFG_H

#include <linux/printk.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define sys_config_print(format, args...) \
    printk("%s,%d: "format, __FUNCTION__, __LINE__, ##args) \

#define sys_writel(addr, offset, value) ((*((volatile unsigned int *)((uintptr_t)(addr) + (offset)))) = (value))
#define sys_readl(addr, offset) (*((volatile int *)((uintptr_t)(addr) + (offset))))

void *sys_config_get_reg_crg(void);
void *sys_config_get_reg_misc(void);
void *sys_config_get_reg_iocfg(void);
void *sys_config_get_reg_iocfg2(void);
void *sys_config_get_reg_iocfg3(void);
void *sys_config_get_reg_gpio(void);

#define CHIP_HI3519DV500 0x0
#define CHIP_NAME_HI3519DV500 "hi3519dv500"
#define VO_INTF_NAME_MIPI_TX "mipi_tx"
#define ENABLE_STR  "1"

#define BOARD_SCK       0x0
#define BOARD_NAME_SCK      "sck"
#define BOARD_DMEB   0x1
#define BOARD_NAME_DMEB "dmeb"
#define BOARD_DMEBPRO   0x2
#define BOARD_NAME_DMEBPRO "dmebpro"

bool chip_is_hi3516dv500(void);
int sys_config_get_main_board_type(void);
int sys_config_get_board_type(void);
int sys_config_get_vi_vpss_mode(void);
int sys_config_get_vo_intf_type(void);
int sys_config_get_vi_intf_type(void);
int sys_config_get_ir_auto(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* SYS_CFG_H */
