/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <asm/io.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/slab.h>

#include <securec.h>

#include "ot_osal.h"
#include "mm_ext.h"
#include "crypto_common_macro.h"
#include "crypto_drv_common.h"
#include "init_km.h"

#ifdef MODULE
#define CPU_ID_STAT                     (0x0018)
#define CRYPTO_CPU_ID_SCPU              (0xa5)
#define CRYPTO_CPU_ID_ACPU              (0xaa)

crypto_cpu_type crypto_get_cpu_type(td_void)
{
    td_u32 cpu_id = ca_misc_reg_read(CPU_ID_STAT) & 0x00ff;
    if (cpu_id == CRYPTO_CPU_ID_SCPU) {
        return CRYPTO_CPU_TYPE_SCPU;
    } else if (cpu_id == CRYPTO_CPU_ID_ACPU) {
        return CRYPTO_CPU_TYPE_ACPU;
    }
    return CRYPTO_CPU_TYPE_INVALID;
}

static td_void *g_km_reg_virt = TD_NULL;
static td_void *g_ca_misc_reg_virt = TD_NULL;

td_u32 crypto_ex_reg_read(reg_region_e region, td_u32 offset)
{
    if (region == REG_REGION_CA_MISC) {
        return crypto_reg_read((volatile td_void *)(g_ca_misc_reg_virt + offset));
    }
    return crypto_reg_read((volatile td_void *)(g_km_reg_virt + offset));
}

td_void crypto_ex_reg_write(reg_region_e region, td_u32 offset, td_u32 value)
{
    if (region == REG_REGION_CA_MISC) {
        crypto_reg_write((volatile td_void *)(g_ca_misc_reg_virt + offset), value);
    }
    crypto_reg_write((volatile td_void *)(g_km_reg_virt + offset), value);
}

td_s32 crypto_copy_from_user(td_void *to, unsigned long to_len, const td_void *from, unsigned long from_len)
{
    if (from_len == 0) {
        return TD_SUCCESS;
    }

    if (to == TD_NULL) {
        crypto_log_err("to is NULL\n");
        return TD_FAILURE;
    }
    if (from == TD_NULL) {
        crypto_log_err("from is NULL\n");
        return TD_FAILURE;
    }
    if (to_len < from_len) {
        crypto_log_err("to_len is Less Than from_len!\n");
        return TD_FAILURE;
    }

    return osal_copy_from_user(to, from, from_len);
}

td_u32 crypto_get_value_by_index(const crypto_table_item *table, td_u32 table_size,
    td_u32 index, td_u32 *value)
{
    const crypto_table_item *item = TD_NULL;
    td_u32 i;

    crypto_chk_return(table == TD_NULL, TD_FAILURE, "table is NULL\n");
    crypto_chk_return(value == TD_NULL, TD_FAILURE, "value is NULL\n");

    for (i = 0; i < table_size; i++) {
        item = &table[i];
        if (item->index == index) {
            *value = item->value;
            return TD_SUCCESS;
        }
    }
    crypto_log_err("Invalid Index!\n");
    return TD_FAILURE;
}
#else
static int crypto_reg_map(td_void)
{
    td_s32 ret;
 
    /* CS MISC Region. */
    ret = crypto_register_reg_map_region(REG_REGION_CA_MISC);
    crypto_chk_return(ret != TD_SUCCESS, ret, "crypto_register_reg_map_region CA_MISC failed, ret is 0x%x\n", ret);
 
    /* KM Region. */
    ret = crypto_register_reg_map_region(REG_REGION_KM);
    crypto_chk_goto(ret != TD_SUCCESS, error_ca_misc_unmap, "crypto_register_reg_map_region KM failed,  \
        ret is 0x%x\n", ret);
 
    return TD_SUCCESS;

error_ca_misc_unmap:
    crypto_unregister_reg_map_region(REG_REGION_CA_MISC);
    return TD_FAILURE;
}
 
static void crypto_reg_unmap(td_void)
{
    crypto_unregister_reg_map_region(REG_REGION_KM);
    crypto_unregister_reg_map_region(REG_REGION_CA_MISC);
}
#endif

td_s32 g_klad_tee_irq_num;
td_s32 g_klad_ree_irq_num;
td_s32 g_rkp_tee_irq_num;
td_s32 g_rkp_ree_irq_num;

static int ot_km_probe(struct platform_device *pdev)
{
    td_s32 ret = TD_SUCCESS;

    g_rkp_tee_irq_num = osal_platform_get_irq_byname(pdev, "rkp_tee");
    g_rkp_ree_irq_num = osal_platform_get_irq_byname(pdev, "rkp_ree");
    g_klad_tee_irq_num = osal_platform_get_irq_byname(pdev, "klad_tee");
    g_klad_ree_irq_num = osal_platform_get_irq_byname(pdev, "klad_ree");

#ifdef MODULE
    g_km_reg_virt = crypto_ioremap_nocache(KM_REG_BASE_ADDR, KM_REG_SIZE);
    if (g_km_reg_virt == TD_NULL) {
        crypto_log_err("crypto_ioremap_nocache failed for km_reg\n");
        return TD_FAILURE;
    }

    g_ca_misc_reg_virt = crypto_ioremap_nocache(CA_MISC_REG_BASE_ADDR, CA_MISC_REG_SIZE);
    if (g_ca_misc_reg_virt == TD_NULL) {
        crypto_log_err("crypto_ioremap_nocache failed for ca_misc_reg\n");
        crypto_iounmap(g_km_reg_virt, KM_REG_SIZE);
        return TD_FAILURE;
    }
#else
    ret = crypto_reg_map();
    crypto_chk_return(ret != TD_SUCCESS, ret, "crypto_reg_map failed, ret is 0x%x\n", ret);
#endif

    ret = crypto_km_init();
    crypto_chk_goto(ret != TD_SUCCESS, reg_unmap_error, "crypto_otp_init failed, ret is 0x%x\n", ret);

    crypto_print("load ot_km.ko ....OK!\n");

    return ret;

reg_unmap_error:
#ifdef MODULE
    crypto_iounmap(g_ca_misc_reg_virt, CA_MISC_REG_SIZE);
    crypto_iounmap(g_km_reg_virt, KM_REG_SIZE);
#else
    crypto_reg_unmap();
#endif
    return ret;
}

static int ot_km_remove(struct platform_device *pdev)
{
    crypto_unused(pdev);
    crypto_km_deinit();
#ifdef MODULE
    crypto_iounmap(g_ca_misc_reg_virt, CA_MISC_REG_SIZE);
    crypto_iounmap(g_km_reg_virt, KM_REG_SIZE);
#else
    crypto_reg_unmap();
#endif
    crypto_print("unload ot_km.ko ....OK!\n");
    return TD_SUCCESS;
}

static const struct of_device_id g_ot_km_match[] = {
    { .compatible = "vendor,km" },
    { },
};
MODULE_DEVICE_TABLE(of, g_ot_km_match);

static struct platform_driver g_ot_km_driver = {
    .probe          = ot_km_probe,
    .remove         = ot_km_remove,
    .driver         = {
        .name   = "ot_km",
        .of_match_table = g_ot_km_match,
    },
};

osal_module_platform_driver(g_ot_km_driver);
MODULE_LICENSE("GPL");
