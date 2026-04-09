/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2016-2019.
 * Description: osal header file.
 * Author: Hisilicon multimedia software group
 * Create: 2016-11-11
 */

#ifndef __OSAL_DEF_H__
#define __OSAL_DEF_H__

extern void osal_proc_init(void);
extern void osal_proc_exit(void);
extern int himedia_init(void);
extern void himedia_exit(void);
extern int media_mem_init(void);
extern void media_mem_exit(void);
extern int mem_check_module_param(void);
extern void osal_device_init(void);

#endif