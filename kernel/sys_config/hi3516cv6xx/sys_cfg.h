/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
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

#define BOARD_SCK       0x0
#define BOARD_NAME_SCK  "sck"
#define BOARD_DMEB_QFN      0x1
#define BOARD_NAME_DMEB_QFN "dmeb_qfn"
#define BOARD_DMEB_BGA      0x2
#define BOARD_NAME_DMEB_BGA "dmeb_bga"
#define BOARD_KOLB        0x3
#define BOARD_NAME_KOLB   "kolb"

bool sys_config_is_hi3516cv610_10b(void);
bool sys_config_is_hi3516cv608(void);
void *sys_config_get_reg_crg(void);
void *sys_config_get_reg_sys(void);
void *sys_config_get_reg_ddr(void);
void *sys_config_get_reg_misc(void);
void *sys_config_get_reg_iocfg(void);
void *sys_config_get_reg_iocfg2(void);
void *sys_config_get_reg_iocfg3(void);
void *sys_config_get_reg_gpio(void);
bool sys_config_get_ir_auto(void);
int sys_config_get_vi_intf_type(void);
int sys_config_get_board_type(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* SYS_CFG_H */
