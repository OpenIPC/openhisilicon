/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "crypto_drv_irq.h"
#include "crypto_hash_struct.h"
#include "hal_hash.h"
#include "hal_symc.h"
#include "hal_pke.h"

#include "drv_hash.h"
#include "drv_inner.h"

#include "crypto_drv_common.h"
#include "crypto_common_macro.h"

#if defined(CRYPTO_OS_INT_SUPPORT)

#define SPACC_REE_IRQ_NUM   60
#define SPACC_TEE_IRQ_NUM   59
#define PKE_REE_IRQ_NUM     62
#define PKE_TEE_IRQ_NUM     61

#define IRQ_TIMEOUT_MS                          (1000 * 5)

#define CONFIG_PKE_IRQ_ENABLE
#define CONFIG_SPACC_IRQ_ENABLE

#ifdef CONFIG_PKE_IRQ_ENABLE
static osal_wait g_pke_wait_queue     = { 0 };
#endif

#ifdef CONFIG_SPACC_IRQ_ENABLE
static osal_wait g_spacc_wait_queue    = { 0 };
#endif

#ifdef CONFIG_SPACC_IRQ_ENABLE

static int spacc_interrupt_irq(int irq, void *param)
{
    td_u32 i;
    for (i = 0; i < CRYPTO_HASH_HARD_CHN_CNT; i++) {
        if (((1 << i) & CRYPTO_HASH_HARD_CHN_MASK) == 0) {
            continue;
        }
        if (hal_cipher_hash_done_try(i)) {
            hal_cipher_hash_done_notify(i);
            osal_wait_wakeup(&g_spacc_wait_queue);
            return 0;
        }
    }
    for (i = 1; i < CRYPTO_SYMC_HARD_CHANNEL_MAX; i++) {
        if (((1 << i) & CRYPTO_SYMC_HARD_CHANNEL_MASK) == 0) {
            continue;
        }

        if (hal_cipher_symc_done_try(i) == TD_SUCCESS) {
            hal_cipher_symc_done_notify(i);
            osal_wait_wakeup(&g_spacc_wait_queue);
            return 0;
        }
    }
    return 0;
}
#endif

static td_s32 spacc_irq_init(td_void)
{
#ifdef CONFIG_SPACC_IRQ_ENABLE
    td_s32 ret = TD_SUCCESS;
    crypto_cpu_type cpu_type;
    td_u32 i;

    /* IRQ Init. */
    ret = osal_wait_init(&g_spacc_wait_queue);
    crypto_chk_return(ret != TD_SUCCESS, ret, "osal_wait_init failed, ret is 0x%x\n", ret);

    for (i = 0; i < CRYPTO_HASH_HARD_CHN_CNT; i++) {
        if (((1 << i) & CRYPTO_HASH_HARD_CHN_MASK) == 0) {
            continue;
        }
        (td_void)hal_cipher_hash_register_wait_func(i, &g_spacc_wait_queue,
            (crypto_wait_timeout_interruptible)osal_wait_timeout_interruptible, IRQ_TIMEOUT_MS);
    }
    for (i = 0; i < CRYPTO_SYMC_HARD_CHANNEL_MAX; i++) {
        if (((1 << i) & CRYPTO_SYMC_HARD_CHANNEL_MASK) == 0) {
            continue;
        }
        (td_void)hal_cipher_symc_register_wait_func(i, &g_spacc_wait_queue,
            (crypto_wait_timeout_interruptible)osal_wait_timeout_interruptible, IRQ_TIMEOUT_MS);
    }

    cpu_type = crypto_get_cpu_type();
    if (cpu_type == CRYPTO_CPU_TYPE_ACPU) {
        ret = osal_irq_request(g_spacc_ree_irq_num, spacc_interrupt_irq, TD_NULL, "nsec_spacc", TD_NULL);
        crypto_chk_goto(ret != TD_SUCCESS, error_spacc_wait_deinit, "osal_irq_request failed, ret is 0x%x\n", ret);
    } else if (cpu_type == CRYPTO_CPU_TYPE_SCPU) {
        ret = osal_irq_request(g_spacc_tee_irq_num, spacc_interrupt_irq, TD_NULL, "sec_spacc", TD_NULL);
        crypto_chk_goto(ret != TD_SUCCESS, error_spacc_wait_deinit, "osal_irq_request failed, ret is 0x%x\n", ret);
    }

    return ret;
error_spacc_wait_deinit:
    osal_wait_destroy(&g_spacc_wait_queue);
    return ret;
#endif
    return CRYPTO_SUCCESS;
}

static td_void spacc_irq_deinit(td_void)
{
#ifdef CONFIG_SPACC_IRQ_ENABLE
    crypto_cpu_type cpu_type;
    cpu_type = crypto_get_cpu_type();
    if (cpu_type == CRYPTO_CPU_TYPE_ACPU) {
        osal_irq_free(g_spacc_ree_irq_num, TD_NULL);
    } else if (cpu_type == CRYPTO_CPU_TYPE_SCPU) {
        osal_irq_free(g_spacc_tee_irq_num, TD_NULL);
    }
    osal_wait_destroy(&g_spacc_wait_queue);
#endif
}

#ifdef CONFIG_PKE_IRQ_ENABLE
static int pke_irq_handler(int a __attribute__((unused)), void *param __attribute__((unused)))
{
    if (hal_pke_done_try()) {
        hal_pke_done_notify();
        osal_wait_wakeup(&g_pke_wait_queue);
        return 0;
    }

    return 0;
}
#endif

static td_s32 pke_irq_init(td_void)
{
#ifdef CONFIG_PKE_IRQ_ENABLE
    td_s32 ret = TD_SUCCESS;
    crypto_cpu_type cpu_type;

    /* IRQ Init. */
    ret = osal_wait_init(&g_pke_wait_queue);
    crypto_chk_return(ret != TD_SUCCESS, ret, "osal_wait_init failed, ret is 0x%x\n", ret);

    (td_void)hal_cipher_pke_register_wait_func(&g_pke_wait_queue,
        (crypto_wait_timeout_interruptible)osal_wait_timeout_interruptible, IRQ_TIMEOUT_MS);

    cpu_type = crypto_get_cpu_type();
    if (cpu_type == CRYPTO_CPU_TYPE_ACPU) {
        ret = osal_irq_request(g_pke_ree_irq_num, pke_irq_handler, TD_NULL, "nsec_pke", TD_NULL);
        crypto_chk_goto(ret != TD_SUCCESS, error_pke_wait_deinit, "osal_irq_request failed, ret is 0x%x\n", ret);
    } else if (cpu_type == CRYPTO_CPU_TYPE_SCPU) {
        ret = osal_irq_request(g_pke_tee_irq_num, pke_irq_handler, TD_NULL, "sec_pke", TD_NULL);
        crypto_chk_goto(ret != TD_SUCCESS, error_pke_wait_deinit, "osal_irq_request failed, ret is 0x%x\n", ret);
    }

    return ret;

error_pke_wait_deinit:
    osal_wait_destroy(&g_pke_wait_queue);
    return ret;

#endif
    return 0;
}

td_void pke_irq_deinit(td_void)
{
#ifdef CONFIG_PKE_IRQ_ENABLE
    crypto_cpu_type cpu_type;
    cpu_type = crypto_get_cpu_type();
    if (cpu_type == CRYPTO_CPU_TYPE_ACPU) {
        osal_irq_free(g_pke_ree_irq_num, TD_NULL);
    } else if (cpu_type == CRYPTO_CPU_TYPE_SCPU) {
        osal_irq_free(g_pke_tee_irq_num, TD_NULL);
    }
    osal_wait_destroy(&g_pke_wait_queue);
#endif
}

td_s32 crypto_irq_init(td_void)
{
    td_s32 ret;
    ret = spacc_irq_init();
    crypto_chk_return(ret != TD_SUCCESS, ret, "spacc_irq_init failed\n");

    ret = pke_irq_init();
    crypto_chk_goto(ret != TD_SUCCESS, error_spacc_irq_deinit, "pke_irq_init failed\n");

    return ret;
error_spacc_irq_deinit:
    spacc_irq_deinit();
    return ret;
}

td_void crypto_irq_deinit(td_void)
{
    spacc_irq_deinit();
    pke_irq_deinit();
}

#endif /* CRYPTO_OS_INT_SUPPORT */