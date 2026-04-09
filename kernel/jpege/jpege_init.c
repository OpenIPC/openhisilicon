/******************************************************************************

  Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : jpege_init.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2017
  Description   :
******************************************************************************/

#include <linux/module.h>

#include "../compat/compat.h"

#ifdef hi3516cv500
extern int JPEGE_ModInit(void);
extern void JPEGE_ModExit(void);
#define jpege_module_init JPEGE_ModInit
#define jpege_module_exit JPEGE_ModExit
#else
extern int jpege_module_init(void);
extern void jpege_module_exit(void);
#endif

static int __init jpege_mod_init(void)
{
	jpege_module_init();
	return 0;
}
static void __exit jpege_mod_exit(void)
{
	jpege_module_exit();
}

module_init(jpege_mod_init);
module_exit(jpege_mod_exit);

MODULE_LICENSE("GPL");
