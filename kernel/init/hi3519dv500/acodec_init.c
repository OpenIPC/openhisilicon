/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_acodec_mod_init.h"
#include "ot_osal.h"

static td_u32 init_mode = 0;
#ifndef MODULE
osal_setup_num_param(init_mode, init_mode);
#else
module_param(init_mode, uint, S_IRUGO);
#endif

static __init int acodec_mod_init(void)
{
    if (init_mode <= 1) {
        acodec_set_init_mode(init_mode);
    } else {
        osal_printk("invalid init_mode(%d), range is [0,1].\n", init_mode);
    }

    return acodec_init();
}

static __exit void acodec_mod_exit(void)
{
    acodec_exit();
}

module_init(acodec_mod_init);
module_exit(acodec_mod_exit);

MODULE_LICENSE("Proprietary");
