/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#include <linux/module.h>
#include "ot_type.h"
#include "ot_common.h"
#include "mod_ext.h"
#include "mm_ext.h"
#include "ot_base_mod_init.h"
#include "ot_debug.h"

EXPORT_SYMBOL(OT_LOG);
EXPORT_SYMBOL(cmpi_mmz_malloc);
EXPORT_SYMBOL(cmpi_mmz_free);
EXPORT_SYMBOL(cmpi_mmz_malloc_nocache);
EXPORT_SYMBOL(cmpi_mmz_malloc_cached);
EXPORT_SYMBOL(cmpi_remap_cached);
EXPORT_SYMBOL(cmpi_remap_nocache);
EXPORT_SYMBOL(cmpi_unmap);
EXPORT_SYMBOL(cmpi_check_mmz_phy_addr);
EXPORT_SYMBOL(cmpi_dcache_region_wb);
EXPORT_SYMBOL(cmpi_invalid_cache_byaddr);
EXPORT_SYMBOL(cmpi_mmz_phys_to_handle);
/*************************MOD********************/
EXPORT_SYMBOL(cmpi_get_module_name);
EXPORT_SYMBOL(cmpi_get_module_func_by_id);
EXPORT_SYMBOL(cmpi_stop_modules);
EXPORT_SYMBOL(cmpi_query_modules);
EXPORT_SYMBOL(cmpi_exit_modules);
EXPORT_SYMBOL(cmpi_init_modules);
EXPORT_SYMBOL(cmpi_stop_modules_for_audio);
EXPORT_SYMBOL(cmpi_query_modules_for_audio);
EXPORT_SYMBOL(cmpi_exit_modules_for_audio);
EXPORT_SYMBOL(cmpi_init_modules_for_audio);
EXPORT_SYMBOL(cmpi_register_module);
EXPORT_SYMBOL(cmpi_unregister_module);

static int __init base_mod_init(void)
{
    return comm_init();
}
static void __exit base_mod_exit(void)
{
    comm_exit();
}

module_init(base_mod_init);
module_exit(base_mod_exit);

MODULE_LICENSE("Proprietary");
