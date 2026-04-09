/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2007-2019. All rights reserved.
 * Description:
 * Author: Hisilicon multimedia software group
 * Create: 2007/09/24 - 2007/10/10
 */

#include <linux/module.h>

extern int h264e_module_init(void);
extern void h264e_module_exit(void);

static int __init h264e_mod_init(void)
{
    return h264e_module_init();
}

static void __exit h264e_mod_exit(void){
    h264e_module_exit();
}

module_init(h264e_mod_init);
module_exit(h264e_mod_exit);

MODULE_LICENSE("Proprietary");


