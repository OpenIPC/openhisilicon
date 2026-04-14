#include <linux/module.h>

extern int _cv200_hi3518e_sys_init(void);
extern void _cv200_hi3518e_sys_exit(void);

#define DECLARE_BLOB_SYM(sym) extern char sym[]

/* Scaling coefficients */
DECLARE_BLOB_SYM(coefficient4_cubic);		EXPORT_SYMBOL(coefficient4_cubic);
DECLARE_BLOB_SYM(coefficient4_1_5M_a05);	EXPORT_SYMBOL(coefficient4_1_5M_a05);
DECLARE_BLOB_SYM(coefficient4_2M_a05);		EXPORT_SYMBOL(coefficient4_2M_a05);
DECLARE_BLOB_SYM(coefficient4_5M_a15);		EXPORT_SYMBOL(coefficient4_5M_a15);
DECLARE_BLOB_SYM(coefficient6_cubic);		EXPORT_SYMBOL(coefficient6_cubic);
DECLARE_BLOB_SYM(coefficient6_1_5M_a05);	EXPORT_SYMBOL(coefficient6_1_5M_a05);
DECLARE_BLOB_SYM(coefficient6_2M_a05);		EXPORT_SYMBOL(coefficient6_2M_a05);
DECLARE_BLOB_SYM(coefficient6_3M_a15);		EXPORT_SYMBOL(coefficient6_3M_a15);
DECLARE_BLOB_SYM(coefficient6_4M_a20);		EXPORT_SYMBOL(coefficient6_4M_a20);
DECLARE_BLOB_SYM(coefficient6_5M_a25);		EXPORT_SYMBOL(coefficient6_5M_a25);
DECLARE_BLOB_SYM(coefficient8_cubic);		EXPORT_SYMBOL(coefficient8_cubic);
DECLARE_BLOB_SYM(coefficient8_1_5M_a05);	EXPORT_SYMBOL(coefficient8_1_5M_a05);
DECLARE_BLOB_SYM(coefficient8_2M_a05);		EXPORT_SYMBOL(coefficient8_2M_a05);
DECLARE_BLOB_SYM(coefficient8_3M_a19);		EXPORT_SYMBOL(coefficient8_3M_a19);
DECLARE_BLOB_SYM(coefficient8_lanczos2_8tap);	EXPORT_SYMBOL(coefficient8_lanczos2_8tap);

DECLARE_BLOB_SYM(g_benchmark_list);		EXPORT_SYMBOL(g_benchmark_list);
DECLARE_BLOB_SYM(g_u32FuncId);			EXPORT_SYMBOL(g_u32FuncId);

/* These are module_param'd inside the blob — use matching types */
extern unsigned int mem_total;			EXPORT_SYMBOL(mem_total);
extern unsigned int vi_vpss_online;		EXPORT_SYMBOL(vi_vpss_online);

static int __init sys_mod_init(void)
{
	return _cv200_hi3518e_sys_init();
}

static void __exit sys_mod_exit(void)
{
	_cv200_hi3518e_sys_exit();
}

module_init(sys_mod_init);
module_exit(sys_mod_exit);
MODULE_LICENSE("Proprietary");
