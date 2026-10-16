// SPDX-License-Identifier: GPL-2.0
/*
 * Entry points for the vendor hi3536dv100_vdec.ko (Hi3536DV100_SDK_V2.0.2.0), relinked from
 * obj/hi3536dv100/vdec.o (see scripts/ko2obj.sh). The blob keeps its own
 * EXPORT_SYMBOLs and module_params, so nothing is re-declared here.
 */
#include <linux/module.h>

extern int _hi3536dv100_vdec_init(void);
extern void _hi3536dv100_vdec_exit(void);

static int __init vdec_mod_init(void)
{
	return _hi3536dv100_vdec_init();
}

static void __exit vdec_mod_exit(void)
{
	_hi3536dv100_vdec_exit();
}

module_init(vdec_mod_init);
module_exit(vdec_mod_exit);
/* hi3536dv100_vdec.ko declares no license, which the kernel treats as proprietary;
 * say so rather than leave modpost to warn about it. */
MODULE_LICENSE("Proprietary");
