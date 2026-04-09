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

#include "../compat/compat.h"

#ifdef hi3516cv500
extern int H265E_ModInit(void);
extern void H265E_ModExit(void);
#define h265e_module_init H265E_ModInit
#define h265e_module_exit H265E_ModExit
#else
extern int h265e_module_init(void);
extern void h265e_module_exit(void);
#endif

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

MODULE_LICENSE("GPL");
