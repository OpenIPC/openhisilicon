/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2007-2019. All rights reserved.
 * Description:
 * Author: Hisilicon multimedia software group
 * Create: 2007/09/24 - 2007/10/10
 */

#include <linux/module.h>

extern int H264E_ModInit(void);
extern void H264E_ModExit(void);

static int __init h264e_mod_init(void)
{
	return H264E_ModInit();
}
static void __exit h264e_mod_exit(void)
{
	H264E_ModExit();
}

module_init(h264e_mod_init);
module_exit(h264e_mod_exit);

MODULE_LICENSE("GPL");
