/******************************************************************************

  Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : h265e_init.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2017
  Description   :
******************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include "hi_type.h"

extern int h265e_module_init(void);
extern void h265e_module_exit(void);
extern HI_U32 H265eFeatureEnable;
module_param(H265eFeatureEnable, uint, S_IRUGO);

static int __init h265e_mod_init(void)
{
    return h265e_module_init();
}

static void __exit h265e_mod_exit(void)
{
    h265e_module_exit();
}

module_init(h265e_mod_init);
module_exit(h265e_mod_exit);

MODULE_LICENSE("Proprietary");



