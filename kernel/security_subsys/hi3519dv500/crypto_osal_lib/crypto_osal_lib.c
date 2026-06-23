/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "crypto_osal_adapt.h"
#include "crypto_drv_common.h"
#include "crypto_common_macro.h"
#include "hal_common.h"
#include "mmz.h"
#include "mm_ext.h"

/* Register Operation. */
typedef struct {
    td_char *reg_region_name;
    td_phys_addr_t reg_base_addr;
    td_u32 reg_size;
    td_void *virt_addr;
    td_bool is_used;
    reg_region_e region;
} reg_region_t;

static reg_region_t g_reg_region_list[REG_REGION_NUM] = {
    {
        .reg_region_name = "SPACC", .reg_base_addr = SPACC_REG_BASE_ADDR, .reg_size = SPACC_REG_SIZE,
        .virt_addr = TD_NULL, .is_used = TD_FALSE, .region = REG_REGION_SPACC
    },
    {
        .reg_region_name = "TRNG", .reg_base_addr = TRNG_REG_BASE_ADDR, .reg_size = TRNG_REG_SIZE,
        .virt_addr = TD_NULL, .is_used = TD_FALSE, .region = REG_REGION_TRNG
    },
    {
        .reg_region_name = "PKE", .reg_base_addr = PKE_REG_BASE_ADDR, .reg_size = PKE_REG_SIZE,
        .virt_addr = TD_NULL, .is_used = TD_FALSE, .region = REG_REGION_PKE
    },
    {
        .reg_region_name = "CA_MISC", .reg_base_addr = CA_MISC_REG_BASE_ADDR, .reg_size = CA_MISC_REG_SIZE,
        .virt_addr = TD_NULL, .is_used = TD_FALSE, .region = REG_REGION_CA_MISC
    },
    {
        .reg_region_name = "KM", .reg_base_addr = KM_REG_BASE_ADDR, .reg_size = KM_REG_SIZE,
        .virt_addr = TD_NULL, .is_used = TD_FALSE, .region = REG_REGION_KM
    },
    {
        .reg_region_name = "OTPC", .reg_base_addr = OTPC_BASE_ADDR, .reg_size = OTPC_ADDR_SIZE,
        .virt_addr = TD_NULL, .is_used = TD_FALSE, .region = REG_REGION_OTPC
    },
};

td_s32 crypto_register_reg_map_region(reg_region_e region)
{
    reg_region_t *reg_region = TD_NULL;
    if (region >= REG_REGION_NUM) {
        crypto_log_err("Invalid region\n");
        return TD_FAILURE;
    }

    reg_region = &g_reg_region_list[region];
    if (reg_region->region != region) {
        crypto_log_err("Region is Mismatched!\n");
        return TD_FAILURE;
    }
    if (reg_region->is_used == TD_TRUE) {
        crypto_print("Reg Region %s has remap!\n", reg_region->reg_region_name);
        return TD_SUCCESS;
    }

    reg_region->virt_addr = crypto_ioremap_nocache(reg_region->reg_base_addr, reg_region->reg_size);
    if (reg_region->virt_addr == TD_NULL) {
        crypto_log_err("crypto_ioremap_nocache failed\n");
        return TD_FAILURE;
    }

    reg_region->is_used = TD_TRUE;

#if defined(CONFIG_MEMORY_DEBUG)
    crypto_print("Region %s map virt_addr 0x%llx to phys_addr 0x%x, size 0x%x Bytes\n",
        reg_region->reg_region_name, reg_region->virt_addr, reg_region->reg_base_addr, reg_region->reg_size);
#endif
    return TD_SUCCESS;
}

void crypto_unregister_reg_map_region(reg_region_e region)
{
    reg_region_t *reg_region = TD_NULL;

    if (region >= REG_REGION_NUM) {
        crypto_log_err("Invalid region\n");
        return;
    }

    reg_region = &g_reg_region_list[region];
    if (reg_region->region != region) {
        crypto_log_err("Region is Mismatched!\n");
        return;
    }
    if (reg_region->is_used == TD_FALSE) {
        crypto_print("Reg Region %s has unmap!\n", reg_region->reg_region_name);
        return;
    }

#if defined(CONFIG_MEMORY_DEBUG)
    crypto_print("Region %s unmap virt_addr 0x%llx to phys_addr 0x%x, size 0x%x Bytes\n",
        reg_region->reg_region_name, reg_region->virt_addr, reg_region->reg_base_addr, reg_region->reg_size);
#endif
    reg_region->is_used = TD_FALSE;
    crypto_iounmap(reg_region->virt_addr, reg_region->reg_size);
    reg_region->virt_addr = TD_NULL;
}


#define REG_MAGIC_NUM               0xDEAFBEEF
td_u32 crypto_ex_reg_read(reg_region_e region, td_u32 offset)
{
    reg_region_t *reg_region = TD_NULL;
    td_u32 value = 0;

    if (region >= REG_REGION_NUM) {
        crypto_log_err("Invalid region\n");
        return REG_MAGIC_NUM;
    }

    reg_region = &g_reg_region_list[region];
    if (reg_region->region != region) {
        crypto_log_err("Region is Mismatched!\n");
        return REG_MAGIC_NUM;
    }
    if (reg_region->is_used == TD_FALSE) {
        crypto_log_err("Reg Region %s is not remap!\n", reg_region->reg_region_name);
        return REG_MAGIC_NUM;
    }
    if (offset >= reg_region->reg_size) {
        crypto_log_err("offset 0x%x is not in reg region %s\n", offset, reg_region->reg_region_name);
        return REG_MAGIC_NUM;
    }

    value = crypto_reg_read((volatile td_void *)(reg_region->virt_addr + offset));

    return value;
}

td_void crypto_ex_reg_write(reg_region_e region, td_u32 offset, td_u32 value)
{
    reg_region_t *reg_region = TD_NULL;

    if (region >= REG_REGION_NUM) {
        crypto_log_err("Invalid region\n");
        return;
    }

    reg_region = &g_reg_region_list[region];
    if (reg_region->region != region) {
        crypto_log_err("Region is Mismatched!\n");
        return;
    }
    if (reg_region->is_used == TD_FALSE) {
        crypto_log_err("Reg Region %s is not remap!\n", reg_region->reg_region_name);
        return;
    }
    if (offset >= reg_region->reg_size) {
        crypto_log_err("offset 0x%x is not in reg region %s\n", offset, reg_region->reg_region_name);
        return;
    }

    crypto_reg_write((volatile td_void *)(reg_region->virt_addr + offset), value);
}

unsigned long crypto_osal_get_phys_addr(const crypto_buf_attr *buf)
{
    return buf->phys_addr;
}

td_bool crypto_data_buf_check(const crypto_buf_attr *buf_attr, td_u32 length)
{
    td_s32 ret;
    if (buf_attr == TD_NULL) {
        return TD_FALSE;
    }
    if (length == 0) {
        return TD_TRUE;
    }

    ret = ot_mmz_check_phys_addr(buf_attr->phys_addr, length);
    if (ret != TD_SUCCESS) {
        return TD_FALSE;
    }

    return TD_TRUE;
}

#if defined(CONFIG_PHYS_ADDR_DUMP_ENABLE)
void crypto_dump_phys_addr(const char *name, const td_phys_addr_t phys_addr, td_u32 data_len)
{
    td_u8 *buffer = crypto_ioremap_nocache(phys_addr, data_len);
    if (buffer == TD_NULL) {
        return;
    }
    crypto_dump_data(name, buffer, data_len);
    crypto_iounmap(buffer, data_len);
}
#endif

#define OTP_BASE_ADDR 0x101E0000
#define OTP_REG_SIZE 0x100
#define OTP_SM2_BIT_OFFSET 25
#define OTP_SM3_BIT_OFFSET 26
#define OTP_SM4_BIT_OFFSET 27

td_bool crypto_sm_support(crypto_sm_alg alg)
{
    td_u32 value = 0;
    td_u32 bit_offset = 0;
    td_void *addr = crypto_ioremap_nocache(OTP_BASE_ADDR, OTP_REG_SIZE);
    crypto_chk_return(addr == TD_NULL, TD_FALSE, "ioremap failed\n");

    value = crypto_reg_read(addr);
    crypto_iounmap(addr, OTP_REG_SIZE);

    if (alg == CRYPTO_SM_ALG_SM2) {
        bit_offset = OTP_SM2_BIT_OFFSET;
    } else if (alg == CRYPTO_SM_ALG_SM3) {
        bit_offset = OTP_SM3_BIT_OFFSET;
    } else if (alg == CRYPTO_SM_ALG_SM4) {
        bit_offset = OTP_SM4_BIT_OFFSET;
    } else {
        return TD_FALSE;
    }

    if (value & (1 << bit_offset)) {
        return TD_FALSE;
    }
    return TD_TRUE;
}

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

td_s32 crypto_virt_xor_phys_copy_to_phys(td_phys_addr_t dst_phys_addr, const td_u8 *a_virt_addr,
    td_phys_addr_t b_phys_addr, td_u32 length)
{
    td_s32 ret;
    td_u32 i;
    td_u8 *b_virt_addr = TD_NULL;
    td_u8 *dst_virt_addr = TD_NULL;

    b_virt_addr = cmpi_remap_nocache(b_phys_addr, length);
    crypto_chk_goto_with_ret(b_virt_addr == TD_NULL, exit, TD_FAILURE, "cmpi_remap_nocache failed\n");

    dst_virt_addr = cmpi_remap_nocache(dst_phys_addr, length);
    crypto_chk_goto_with_ret(dst_virt_addr == TD_NULL, exit, TD_FAILURE, "cmpi_remap_nocache failed\n");

#if defined(CRYPTO_CTR_TRACE_ENABLE)
    crypto_dump_data("a_virt_addr", a_virt_addr, length);
    crypto_dump_data("b_virt_addr", b_virt_addr, length);
#endif

    for (i = 0; i < length; i++) {
        dst_virt_addr[i] = a_virt_addr[i] ^ b_virt_addr[i];
    }

#if defined(CRYPTO_CTR_TRACE_ENABLE)
    crypto_dump_data("dst_virt_addr", dst_virt_addr, length);
#endif
    ret = TD_SUCCESS;
exit:
    if (b_virt_addr != TD_NULL) {
        cmpi_unmap(b_virt_addr);
    }
    if (dst_virt_addr != TD_NULL) {
        cmpi_unmap(dst_virt_addr);
    }
    return ret;
}

td_s32 crypto_virt_copy_to_phys(td_phys_addr_t dst_phys_addr, const td_u8 *src_virt_addr, td_u32 length)
{
    td_s32 ret;
    td_u8 *dst_virt_addr = TD_NULL;

    dst_virt_addr = cmpi_remap_nocache(dst_phys_addr, length);
    crypto_chk_goto_with_ret(dst_virt_addr == TD_NULL, exit, TD_FAILURE, "cmpi_remap_nocache failed\n");

    ret = memcpy_s(dst_virt_addr, length, src_virt_addr, length);
    crypto_chk_goto_with_ret(ret != EOK, exit, TD_FAILURE, "memcpy_s failed\n");

    ret = TD_SUCCESS;
exit:
    if (dst_virt_addr != TD_NULL) {
        cmpi_unmap(dst_virt_addr);
    }
    return ret;
}


td_s32 crypto_phys_copy_to_virt(td_u8 *dst_virt_addr, td_phys_addr_t src_phys_addr, td_u32 length)
{
    td_s32 ret;
    td_u8 *src_virt_addr = TD_NULL;

    src_virt_addr = cmpi_remap_nocache(src_phys_addr, length);
    crypto_chk_goto_with_ret(src_virt_addr == TD_NULL, exit, TD_FAILURE, "cmpi_remap_nocache failed\n");

    ret = memcpy_s(dst_virt_addr, length, src_virt_addr, length);
    crypto_chk_goto_with_ret(ret != EOK, exit, TD_FAILURE, "memcpy_s failed\n");

    ret = TD_SUCCESS;
exit:
    if (src_virt_addr != TD_NULL) {
        cmpi_unmap(src_virt_addr);
    }
    return ret;
}