/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef OT_WTDG_HAL_H
#define OT_WTDG_HAL_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define OSDRV_MODULE_VERSION_STRING "WDT-MDC030001"

#define OT_WDT_BASE      0x11030000    /* define watchdog IO */
#define WDT_REGS_LENGTH  0x1000
#define WATCHDOG_MINOR  101

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
