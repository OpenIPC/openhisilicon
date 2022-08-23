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

extern int CHNL_ModInit(void);
extern void CHNL_ModExit(void);

static int __init chnl_mod_init(void)
{
	CHNL_ModInit();
	return 0;
}
static void __exit chnl_mod_exit(void)
{
	CHNL_ModExit();
}

module_init(chnl_mod_init);
module_exit(chnl_mod_exit);

MODULE_LICENSE("GPL");
