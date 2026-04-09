/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for drv lib.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "drv_osal_lib.h"
#include "cryp_trng.h"

/* ********************** Internal Structure Definition ********************** */
/* crg register addr size. */
#define CRG_REG_ADDR_SIZE        0x100

/* set a bit within a word. */
#undef  set_bit
#define set_bit(src, bit)        ((src) |= (1 << (bit)))

/* clear a bit within a word. */
#undef  clear_bit
#define clear_bit(src, bit)      ((src) &= ~(1 << (bit)))

/* module already initialize or not. */
static hi_u32 g_module_initialize = HI_FALSE;

/* struct resource_channel
 * the tbale of base info of module, such as addr/reset/clk/ver.
 */
struct sys_arch_hardware_info {
    /* the name of module, used for debug print and request interrupt. */
    const hi_char *name;

    /* smoe field may be needn't to used. */
    hi_u32 reset_valid: 1; /* bis[0], reset availability, 0-valid, 1-invalid. */
    hi_u32 clk_valid: 1;   /* bis[1], clock availability, 0-valid, 1-invalid. */
    hi_u32 phy_valid: 1;   /* bis[2], system address availability, 0-valid, 1-invalid. */
    hi_u32 crg_valid: 1;   /* bis[3], CRG availability, 0-valid, 1-invalid. */
    hi_u32 ver_valid: 1;   /* bis[4], version reg availability, 0-valid, 1-invalid. */
    hi_u32 int_valid: 1;   /* bis[5], interrupt availability, 0-valid, 1-invalid. */
    hi_u32 res_valid: 25;  /* bis[6..31]. */

    /* module base addr. */
    hi_u32 reg_addr_phy;

    /* base logic addr size. */
    hi_u32 reg_addr_size;

    /* crg register addr, which provide the switch of reset and clock. */
    hi_u32 crg_addr_phy;

    /* the clk switch bit index within the crg register,
     * if the switch bis is provided by second crg register, you need to add 32. */
    hi_u32 clk_bit: 8;

    /* the reset switch bit index within the crg register,
     * if the switch bis is provided by second crg register, you need to add 32. */
    hi_u32 reset_bit: 8;

    /* the interrupt number. */
    hi_u32 int_num: 8;

    /* the reserve bits. */
    hi_u32 res: 8;

    /* the offset of version register. */
    hi_u32 version_reg;

    /* the value of version register, you can used it to check the active module. */
    hi_u32 version_val;

    /* cpu address of module register. */
    hi_void *reg_addr_via;

    /* cpu address of crg register. */
    hi_void *crg_addr_via;
};

static struct sys_arch_hardware_info g_hard_info_table[CRYPTO_MODULE_ID_CNT] = {
    HARD_INFO_CIPHER,
    HARD_INFO_CIPHER_KEY,
    HARD_INFO_HASH,
    HARD_INFO_IFEP_RSA,
    HARD_INFO_SIC_RSA,
    HARD_INFO_TRNG,
    HARD_INFO_SM2,
    HARD_INFO_SM4,
    HARD_INFO_SMMU,
};

#ifdef CRYPTO_SWITCH_CPU
static struct sys_arch_hardware_info g_nsec_hard_info_table[CRYPTO_MODULE_ID_CNT] = {
    NSEC_HARD_INFO_CIPHER,
    NSEC_HARD_INFO_CIPHER_KEY,
    NSEC_HARD_INFO_HASH,
    NSEC_HARD_INFO_IFEP_RSA,
    NSEC_HARD_INFO_SIC_RSA,
    NSEC_HARD_INFO_TRNG,
    NSEC_HARD_INFO_SM2,
    NSEC_HARD_INFO_SM4,
    NSEC_HARD_INFO_SMMU,
};

static hi_u32 g_is_secure_cpu = HI_FALSE;
#endif

/* ****************************** API Code **************************** */
static hi_s32 module_addr_map_reg(hi_void)
{
    hi_u32 i;
    struct sys_arch_hardware_info *table = HI_NULL;

    hi_log_func_enter();

    if (g_module_initialize == HI_TRUE) {
        return HI_SUCCESS;
    }

    for (i = 0; i < CRYPTO_MODULE_ID_CNT; i++) {
        table = &g_hard_info_table[i];

        hi_log_info("[%d] %s\n", i, table->name);

        /* io remap crg register. */
        if (table->crg_valid) {
            table->crg_addr_via = crypto_ioremap_nocache(table->crg_addr_phy, CRG_REG_ADDR_SIZE);
            if (table->crg_addr_via == HI_NULL) {
                hi_log_error("iomap reg of module failed\n");
                module_addr_unmap();
                hi_log_print_err_code(HI_ERR_CIPHER_FAILED_MEM);
                return HI_ERR_CIPHER_FAILED_MEM;
            }
            hi_log_info("crg phy 0x%x, via 0x%p\n", table->crg_addr_phy, table->crg_addr_via);
        }

        /* io remap module register. */
        if (table->phy_valid) {
            table->reg_addr_via = crypto_ioremap_nocache(table->reg_addr_phy,
                                                         table->reg_addr_size);
            if (table->reg_addr_via == HI_NULL) {
                hi_log_error("iomap reg of module failed\n");
                module_addr_unmap();
                hi_log_print_err_code(HI_ERR_CIPHER_FAILED_MEM);
                return HI_ERR_CIPHER_FAILED_MEM;
            }
            hi_log_info("reg phy 0x%x, via 0x%p, size 0x%x\n", table->reg_addr_phy,
                table->reg_addr_via, table->reg_addr_size);
        }
    }

    g_module_initialize = HI_TRUE;
    hi_log_func_exit();
    return HI_SUCCESS;
}

/*
 * brief  unmap the physics addr to cpu within the base table, contains the base addr and crg addr.
 */
hi_s32 module_addr_unmap(hi_void)
{
    hi_u32 i;
    struct sys_arch_hardware_info *table = HI_NULL;

    hi_log_func_enter();

    if (g_module_initialize == HI_FALSE) {
        return HI_SUCCESS;
    }

    for (i = 0; i < CRYPTO_MODULE_ID_CNT; i++) {
        table = &g_hard_info_table[i];

        hi_log_info("[%d] %s\n", i, table->name);

        /* io unmap crg register. */
        if (table->crg_valid && table->crg_addr_via) {
            hi_log_info("crg via addr 0x%p\n", table->crg_addr_via);
            crypto_iounmap(table->crg_addr_via, CRG_REG_ADDR_SIZE);
            table->crg_addr_via = HI_NULL;
        }

        /* io unmap module register. */
        if (table->phy_valid && table->reg_addr_via) {
            hi_log_info("reg via addr 0x%p\n", table->reg_addr_via);
            crypto_iounmap(table->reg_addr_via, table->reg_addr_size);
            table->reg_addr_via = HI_NULL;
        }
    }

    g_module_initialize = HI_FALSE;

    hi_log_func_exit();
    return HI_SUCCESS;
}

/*
 * brief  map the physics addr to cpu within the base table, contains the base addr and crg addr.
 */
hi_s32 module_addr_map(hi_void)
{
    hi_s32 ret;

    hi_log_func_enter();

    ret = module_addr_map_reg();
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(module_addr_map_reg, ret);
        return ret;
    }

#ifdef CRYPTO_SWITCH_CPU
    g_is_secure_cpu = drv_symc_is_secure();
    if (g_is_secure_cpu == HI_TRUE) {
        /* default secure cpu, do nothing */
        return HI_SUCCESS;
    }

    hi_log_info("non-secure CPU need to remap reg addr\n");

    /* use non-secure info */
    crypto_memcpy(&g_hard_info_table, sizeof(g_hard_info_table),
                  &g_nsec_hard_info_table, sizeof(g_nsec_hard_info_table));

    /* remap module addr */
    ret = module_addr_unmap();
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(module_addr_unmap, ret);
        return ret;
    }
    ret = module_addr_map_reg();
    if (ret != HI_SUCCESS) {
        hi_log_print_func_err(module_addr_map, ret);
        return ret;
    }
#endif

    hi_log_func_exit();
    return HI_SUCCESS;
}

/*
 * brief  get secure cpu type.
 */
hi_u32 module_get_secure(hi_void)
{
#ifdef CRYPTO_SEC_CPU
    return HI_TRUE;
#elif defined(CRYPTO_SWITCH_CPU)
    return g_is_secure_cpu;
#else
    return HI_FALSE;
#endif
}

/*
 * brief  open clock.
 */
static hi_void module_clock(module_id id, hi_u32 open)
{
    hi_u32 val = 0;
    hi_u32 *addr = HI_NULL;
    struct sys_arch_hardware_info *table = HI_NULL;

    hi_log_func_enter();

    table = &g_hard_info_table[id];

    if (table->clk_valid) {
        if (table->crg_addr_via == HI_NULL) {
            hi_log_error("\033[0;1;31m""%s clock failed, crg addr is null\n""\033[0m", table->name);
            return;
        }

        /* open clk, the addr may be need to add 1. */
        addr = table->crg_addr_via;
        addr += table->clk_bit / WORD_BIT_WIDTH;

        val = crypto_read(addr);

        if (open) {
            set_bit(val, table->clk_bit % WORD_BIT_WIDTH);
        } else {
            clear_bit(val, table->clk_bit % WORD_BIT_WIDTH);
        }

        crypto_write(addr, val);

        /* wait hardware clock active. */
        crypto_msleep(1);

        hi_log_info("%s clock, open %u, bit %u, phy 0x%x, via 0x%p\n",
                    table->name, open, table->clk_bit,
                    table->crg_addr_phy, table->crg_addr_via);
    }

    hi_log_func_exit();

    return;
}

static hi_void module_reset(module_id id, hi_u32 enable)
{
    hi_u32 val = 0;
    hi_u32 *addr = HI_NULL;
    hi_u32 expect = 0;
    struct sys_arch_hardware_info *table = HI_NULL;

    hi_log_func_enter();

    table = &g_hard_info_table[id];

    if (table->reset_valid) {
        if (table->crg_addr_via == HI_NULL) {
            hi_log_error("\033[0;1;31m""%s reset failed, crg addr is null\n""\033[0m", table->name);
            return;
        }

        /* the addr may be need to add 1. */
        addr = table->crg_addr_via;
        addr += table->reset_bit / WORD_BIT_WIDTH;

        val = crypto_read(addr);

        if (enable) {
            set_bit(val, table->reset_bit % WORD_BIT_WIDTH);
            expect = 0;
        } else {
            clear_bit(val, table->reset_bit % WORD_BIT_WIDTH);
            expect = table->version_val;
        }

        crypto_write(addr, val);

        /* wait hardware reset finish. */
        crypto_msleep(1);

        hi_log_info("%s reset, enable %u, bit %u, phy 0x%x, via 0x%p\n",
                    table->name, enable, table->reset_bit,
                    table->crg_addr_phy, table->crg_addr_via);

        /* check the value of version reg to make sure reset success. */
        if (table->ver_valid && table->reg_addr_via) {
            val = crypto_read((hi_u8 *)table->reg_addr_via + table->version_reg);
            if (val != expect) {
                hi_log_error("\033[0;1;31m""%s reset failed, version reg should be 0x%x but 0x%x\n""\033[0m",
                             table->name, expect, val);
                return;
            }

            hi_log_info("%s version reg, offset 0x%x, expect val 0x%x, real val 0x%x\n",
                        table->name, table->version_reg, expect, val);
        }
    }

    hi_log_func_exit();

    return;
}

/*
 * brief  enable a module, open clock  and remove reset signal.
 */
hi_void module_enable(module_id id)
{
    module_clock(id, HI_TRUE);
    module_reset(id, HI_FALSE);
    return;
}

/*
 * brief  disable a module, close clock and set reset signal.
 */
hi_void module_disable(module_id id)
{
    module_reset(id, HI_TRUE);
    module_clock(id, HI_FALSE);
    return;
}

/*
 * brief  get attribute of module.
 */
hi_void module_get_attr(module_id id, hi_u32 *int_valid, hi_u32 *int_num, const hi_char **name)
{
    *int_valid = g_hard_info_table[id].int_valid;
    *int_num = g_hard_info_table[id].int_num;
    if (name != HI_NULL) {
        *name = g_hard_info_table[id].name;
    }

    return;
}

/*
 * brief  get base address of module.
 */
hi_void module_set_irq(module_id id, hi_u32 irq)
{
    g_hard_info_table[id].int_num = irq;
    hi_log_info("%s set irq number 0x%x\n", g_hard_info_table[id].name, irq);
}

/*
 * brief  read a register.
 */
hi_u32 module_reg_read(module_id id, hi_u32 offset)
{
    hi_u32 val;
    hi_void *addr = HI_NULL;
    hi_s32 ret;

    hi_log_chk_param_return(id >= CRYPTO_MODULE_ID_CNT);
    hi_log_chk_param_return(offset >= g_hard_info_table[id].reg_addr_size);

    /* tee may be read trng before cipher module init. */
    if (g_hard_info_table[id].reg_addr_via == HI_NULL) {
        ret = module_addr_map();
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(module_addr_map, ret);
            return 0;
        }
    }

    /* get the absolute address of reg. */
    addr = (hi_u8 *)(g_hard_info_table[id].reg_addr_via) + offset;
    val = crypto_read(addr);

    return val;
}

/*
 * brief  write a register.
 */
hi_void module_reg_write(module_id id, hi_u32 offset, hi_u32 val)
{
    hi_void *addr = HI_NULL;
    hi_s32 ret;

    /* check if module is valid. */
    if (id >= CRYPTO_MODULE_ID_CNT) {
        hi_log_error("error, invalid module id %d\n", id);
        return;
    }

    /* check if offset is valid. */
    if (offset >= g_hard_info_table[id].reg_addr_size) {
        hi_log_error("error, reg offset overflow 0x%x\n", offset);
        return;
    }

    /* tee may be read trng before cipher module init. */
    if (g_hard_info_table[id].reg_addr_via == HI_NULL) {
        ret = module_addr_map();
        if (ret != HI_SUCCESS) {
            hi_log_print_func_err(module_addr_map, ret);
            return;
        }
    }

    /* get the absolute address of reg. */
    addr = (hi_u8 *)g_hard_info_table[id].reg_addr_via + offset;
    crypto_write(addr, val);

    return;
}

/*
 * brief  Initialize the channel list.
 */
hi_s32 crypto_channel_init(channel_context *ctx, hi_u32 num, hi_u32 ctx_size)
{
    hi_u32 size;
    hi_u32 i;
    hi_u8 *buf = HI_NULL;

    hi_log_func_enter();

    hi_log_chk_param_return(ctx == HI_NULL);

    /* clear context. */
    size = sizeof(channel_context) * num;
    crypto_memset(ctx, size, 0, size);

    /* set context buffer. */
    if (ctx_size > 0) {
        buf = (hi_u8 *)crypto_malloc(ctx_size * num);
        if (buf == HI_NULL) {
            return HI_ERR_CIPHER_FAILED_MEM;
        }
        crypto_memset(buf, ctx_size * num, 0, ctx_size * num);

        /* the buffer addresss is stored at ctx[0].ctx. */
        for (i = 0; i < num; i++) {
            ctx[i].ctx = buf;
            buf += ctx_size;
        }
    }

    hi_log_func_exit();
    return HI_SUCCESS;
}

/*
 * brief  denit the channel list.
 */
hi_s32 crypto_channel_deinit(channel_context *ctx, hi_u32 num)
{
    hi_u32 size;

    hi_log_func_enter();

    hi_log_chk_param_return(ctx == HI_NULL);

    /* the buffer addresss is stored at ctx[0].ctx. */
    if (ctx[0].ctx != HI_NULL) {
        crypto_free(ctx[0].ctx);
        ctx[0].ctx = HI_NULL;
    }

    /* clear context. */
    size = sizeof(channel_context) * num;
    crypto_memset(ctx, size, 0, size);

    hi_log_func_exit();
    return HI_SUCCESS;
}

/*
 * brief  allocate a channel.
 */
hi_s32 crypto_channel_alloc(channel_context *ctx, hi_u32 num, hi_u32 mask, hi_u32 *id)
{
    hi_s32 ret = HI_ERR_CIPHER_BUSY;
    hi_u32 i = 0;

    hi_log_func_enter();

    hi_log_chk_param_return(ctx == HI_NULL);
    hi_log_chk_param_return(id == HI_NULL);

    for (i = 0; i < num; i++) {
        /* check the valid channel. */
        if (mask & (0x01 << i)) {
            if (!ctx[i].open) { /* found a free channel. */
                ret = HI_SUCCESS;
                ctx[i].open = HI_TRUE; /* alloc channel. */
                *id = i;
                break;
            }
        }
    }

    if (i == num) {
        hi_log_error("error, all channels are busy.\n");
    }

    hi_log_func_exit();

    return ret;
}

/*
 * brief  free a  channel.
 */
hi_void crypto_channel_free(channel_context *ctx, hi_u32 num, hi_u32 id)
{
    hi_log_func_enter();

    /* free channel. */
    ctx[id].open = HI_FALSE;

    hi_log_func_exit();
    return;
}

/*
 * brief  get the private data of channel.
 */
hi_void *crypto_channel_get_context(channel_context *ctx, hi_u32 num, hi_u32 id)
{
    if (ctx == HI_NULL) {
        hi_log_error("crypto_channel_get_context() ctx is HI_NULL\n");
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_POINT);
        return HI_NULL;
    }

    if ((id >= num) || (!ctx[id].open)) {
        hi_log_error("crypto_channel_get_context()- error, id %u, open %u, num %u\n", id, ctx[id].open, num);
        hi_log_print_err_code(HI_ERR_CIPHER_INVALID_POINT);
        return HI_NULL;
    }

    return ctx[id].ctx;
}

hi_void hex2str(hi_char buf[MUL_VAL_2], hi_u32 buf_len, hi_u8 val)
{
    hi_u8 high, low;

    if (buf_len != MUL_VAL_2) {
        return;
    }

    high = (val >> SHIFT_4BITS) & MAX_LOW_4BITS;
    low =  val & MAX_LOW_4BITS;

    if (high <= 9) {                           /* 0 ~ 9. */
        buf[WORD_IDX_0] = high + '0';
    } else {
        buf[WORD_IDX_0] = (high - 0x0A) + 'A'; /* A ~ F. */
    }

    if (low <= 9) {                            /* 0 ~ 9. */
        buf[WORD_IDX_1] = low + '0';
    } else {                                   /* A ~ F. */
        buf[WORD_IDX_1] = (low - 0x0A) + 'A';
    }
}

/* Implementation that should never be optimized out by the compiler. */
hi_void crypto_zeroize(hi_void *buf, hi_u32 len)
{
    volatile hi_u8 *p = (hi_u8 *)buf;

    if (buf == HI_NULL) {
        return;
    }

    while (len--) {
        *p++ = 0;
    }
}

hi_void *crypto_calloc(size_t n, size_t size)
{
    hi_void *ptr = HI_NULL;

    ptr = crypto_malloc(n * size);
    if (ptr != HI_NULL) {
        crypto_memset(ptr, n * size, 0, n * size);
    }
    return ptr;
}

hi_u32 get_rand(hi_void)
{
    hi_u32 randnum = 0;

    cryp_trng_get_random(&randnum, -1);

    return randnum;
}

#ifdef CIPHER_DEBUG_TEST_SUPPORT
void crypto_print_hex(const hi_char *name, hi_u8 *str, hi_u32 len)
{
    hi_u32 i;
    hi_u8 *tmp_str = (hi_u8 *)(str);

    if (name == HI_NULL) {
        hi_log_error("name is null.\n");
        return;
    }

    if (str == HI_NULL) {
        hi_log_error("str is null.\n");
        return;
    }

    if (len == 0) {
        hi_log_error("len is 0.\n");
        return;
    }

    HI_PRINT("[%s]:\n", (name));
    for (i = 0 ; i < (len); i++) {
        if ((i % PRINT_HEX_BLOCK_LEN == 0) && (i != 0)) {
            HI_PRINT("\n");
        }

        HI_PRINT("\\x%02x", *((tmp_str) + i));
    }
    HI_PRINT("\n");
}
#endif
