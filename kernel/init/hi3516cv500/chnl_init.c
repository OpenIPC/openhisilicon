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


extern int chnl_module_init(void);
extern void chnl_module_exit(void);


static int __init chnl_mod_init(void)
{
    chnl_module_init();
    return 0;
}
static void __exit chnl_mod_exit(void)
{
    chnl_module_exit();
}

module_init(chnl_mod_init);
module_exit(chnl_mod_exit);

MODULE_LICENSE("Proprietary");

