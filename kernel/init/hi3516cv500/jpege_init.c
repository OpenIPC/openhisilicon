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

extern int jpege_module_init(void);
extern void jpege_module_exit(void);

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

MODULE_LICENSE("Proprietary");




