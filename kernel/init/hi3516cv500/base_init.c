#include <linux/module.h>

#include "mod_ext.h"

extern int HI_LOG(hi_s32 level, hi_mod_id mod_id, const char *fmt, ...);
EXPORT_SYMBOL(HI_LOG);

extern hi_u32  cmpi_mmz_malloc(hi_char *mmz_name, hi_char *buf_name, hi_ulong size);
EXPORT_SYMBOL(cmpi_mmz_malloc);
extern hi_void cmpi_mmz_free(hi_u64 phy_addr, hi_void* virt_addr);
EXPORT_SYMBOL(cmpi_mmz_free);
extern hi_s32  cmpi_mmz_malloc_nocache(hi_char* cp_mmz_name, hi_char* buf_name,
    hi_u64* phy_addr, hi_void** pp_vit_addr, hi_ulong len);
EXPORT_SYMBOL(cmpi_mmz_malloc_nocache);
extern hi_s32  cmpi_mmz_malloc_cached(hi_char* cp_mmz_name, hi_char* buf_name,
    hi_u64* phy_addr, hi_void** pp_vit_addr, hi_ulong len);
EXPORT_SYMBOL(cmpi_mmz_malloc_cached);

extern hi_void * cmpi_remap_cached(hi_u64 phy_addr, hi_ulong size);
EXPORT_SYMBOL(cmpi_remap_cached);

extern hi_void * cmpi_remap_nocache(hi_u64 phy_addr, hi_ulong size);
EXPORT_SYMBOL(cmpi_remap_nocache);

extern hi_void   cmpi_unmap(hi_void *virt_addr);
EXPORT_SYMBOL(cmpi_unmap);


extern hi_u32 vb_force_exit;
module_param(vb_force_exit, uint, S_IRUGO);


/*************************MOD********************/
EXPORT_SYMBOL(cmpi_get_module_name);
EXPORT_SYMBOL(cmpi_get_module_by_id);
EXPORT_SYMBOL(cmpi_get_module_func_by_id);
EXPORT_SYMBOL(cmpi_stop_modules);
EXPORT_SYMBOL(cmpi_query_modules);
EXPORT_SYMBOL(cmpi_exit_modules);
EXPORT_SYMBOL(cmpi_init_modules);
EXPORT_SYMBOL(cmpi_register_module);
EXPORT_SYMBOL(cmpi_unregister_module);

 

/******* create node  /proc/sys/dev/debug/proc_message_enable ************/
extern hi_s32 g_proc_enable;
EXPORT_SYMBOL(g_proc_enable);

static struct ctl_table comm_eproc_table[] = {
    {
        .procname       = "proc_message_enable",
        .data           = &g_proc_enable,
        .maxlen         = sizeof(g_proc_enable),
        .mode           = 0644,
        .proc_handler   = proc_dointvec
    },
    {}
};

static struct ctl_table comm_dir_table[] = {
    {
        .procname       = "debug",
        .mode           = 0555,
        .child          = comm_eproc_table
    },
    {}
};

static struct ctl_table comm_parent_tbl[] = {
    {
     .procname       = "dev",
     .mode           = 0555,
     .child          = comm_dir_table
    },
    {}
};

static struct ctl_table_header *comm_eproc_tbl_head;

int __init comm_init_proc_ctrl(void)
{
    comm_eproc_tbl_head = register_sysctl_table(comm_parent_tbl);
    if (!comm_eproc_tbl_head)
        return -ENOMEM;
    return 0;
}

void comm_exit_proc_ctrl(void)
{
    unregister_sysctl_table(comm_eproc_tbl_head);
}



extern int comm_init(void);
extern void comm_exit(void);

static int __init base_mod_init(void)
{
    comm_init();

    /* init proc switch */
    if(HI_SUCCESS != comm_init_proc_ctrl())
    {
        comm_exit();
        return -1;
    }

    return 0;
}
static void __exit base_mod_exit(void)
{
    comm_exit_proc_ctrl();
    comm_exit();
}

module_init(base_mod_init);
module_exit(base_mod_exit);

MODULE_LICENSE("Proprietary");

