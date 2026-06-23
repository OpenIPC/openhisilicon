/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "hal_trng.h"
#include "hal_trng_reg.h"

#include "crypto_drv_common.h"

#define TRNG_OSC_SEL   0x02
#define TRNG_POWER_ON  0x05
#define TRNG_POWER_OFF 0x0a

#define TRNG_COMPAT_ERRNO(err_code)     HAL_COMPAT_ERRNO(ERROR_MODULE_TRNG, err_code)

static td_bool g_hal_trng_init_flag = TD_FALSE;

td_s32 hal_cipher_trng_init(td_void)
{
    g_hal_trng_init_flag = TD_TRUE;
    return TD_SUCCESS;
}

td_s32 hal_cipher_trng_deinit(void)
{
    g_hal_trng_init_flag = TD_FALSE;
    return TD_SUCCESS;
}

#define CRYPTO_TRNG_TIMEOUT_IN_US       1000000   /* 10ms. */
static td_bool is_trng_ready(void)
{
    hisc_com_trng_fifo_ready trng_ready = {0};

    if (crypto_get_cpu_type() != CRYPTO_CPU_TYPE_SCPU) { /* Not check status on non-secure Core */
        return TD_TRUE;
    }

    trng_ready.u32 = 0;  /* trng not done, and not ready. */
    trng_ready.u32 = trng_reg_read(HISC_COM_TRNG_FIFO_READY);

    if ((trng_ready.bits).trng_done != TRNG_DONE ||
        (trng_ready.bits).trng_data_ready != TRNG_DATA_READY) {
            return TD_FALSE;
    }
    return TD_TRUE;
}

td_s32 hal_cipher_trng_get_random(td_u32 *randnum)
{
    td_u32 times = 0;
    td_u32 chk_randnum = 0;

    crypto_chk_return(g_hal_trng_init_flag == TD_FALSE, TRNG_COMPAT_ERRNO(ERROR_NOT_INIT), "call init first!\n");
    crypto_chk_return(randnum == TD_NULL, TRNG_COMPAT_ERRNO(ERROR_PARAM_IS_NULL), "randnum is NULL\n");

    while (times < CRYPTO_TRNG_TIMEOUT_IN_US) {
        times++;
        if (is_trng_ready() == TD_FALSE) {
            continue;
        }

        if (is_trng_ready() == TD_TRUE) {
            *randnum = trng_reg_read(HISC_COM_TRNG_FIFO_DATA);
            chk_randnum = trng_reg_read(HISC_COM_TRNG_FIFO_DATA);
            if ((*randnum != 0x00000000) && (*randnum != 0xffffffff) && (*randnum != chk_randnum)) {
                return TD_SUCCESS;
            }
        }
    }

    return TRNG_COMPAT_ERRNO(ERROR_GET_TRNG_TIMEOUT);
}