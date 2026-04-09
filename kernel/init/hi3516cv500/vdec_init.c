#include <linux/module.h>
#include "hi_type.h"
#include "vdec_exp.h"

extern int vdec_module_init(void);
extern void vdec_module_exit(void);

extern VDEC_EXPORT_SYMBOL_S  g_stVdecExpSymbol;

EXPORT_SYMBOL(g_stVdecExpSymbol);

extern hi_u32 VdecMaxChnNum;
module_param(VdecMaxChnNum, uint, S_IRUGO);

extern hi_u32 VdecDebugMode;
module_param(VdecDebugMode, uint, S_IRUGO);

static int __init vdec_mod_init(void)
{
    vdec_module_init();
    return 0;
}

static void __exit vdec_mod_exit(void)
{
    vdec_module_exit();
}

module_init(vdec_mod_init);
module_exit(vdec_mod_exit);

MODULE_LICENSE("Proprietary");
