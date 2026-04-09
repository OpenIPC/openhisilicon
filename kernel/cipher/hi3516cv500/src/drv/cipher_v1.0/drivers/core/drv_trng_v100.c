/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for drv trng v100.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "drv_trng_v100.h"
#include "drv_trng.h"

#ifdef CHIP_TRNG_VER_V100

/* ************************** Internal Structure Definition *************************** */
/* Define the post process depth */
#define TRNG_POST_PROCESS_DEPTH     0x10

/* Define the osc sel */
#define TRNG_OSC_SEL                0x02

/* ****************************** API Declaration **************************** */
hi_s32 drv_trng_randnum(hi_u32 *randnum, hi_u32 timeout)
{
    rng_stat stat;
    rng_ctrl ctrl;
    hi_u32 times = 0;
    static hi_u32 last = 0x1082;

    hi_log_func_enter();

    hi_log_chk_param_return(randnum == HI_NULL);

    if (ctrl.u32 != last) {
        module_enable(CRYPTO_MODULE_ID_TRNG);

        ctrl.u32 = trng_read(RNG_CTRL);
        ctrl.bits.filter_enable = 0x00;
        ctrl.bits.mix_en = 0x00;
        ctrl.bits.drop_enable = 0x00;
        ctrl.bits.post_process_enable = 0x01;
        ctrl.bits.post_process_depth = TRNG_POST_PROCESS_DEPTH;
        ctrl.bits.osc_sel = TRNG_OSC_SEL;
        trng_write(RNG_CTRL, ctrl.u32);
    }

#if defined(HI_PLATFORM_TYPE_LINUX)
    if (timeout == 0) { /* unblock */
        /* trng number is valid ? */
        stat.u32 = trng_read(RNG_STAT);
        if (stat.bits.rng_data_count == 0x00) {
            return HI_ERR_CIPHER_NO_AVAILABLE_RNG;
        }
    } else {
        /* block */
        while (times++ < TRNG_TIMEOUT) {
            /* trng number is valid ? */
            stat.u32 = trng_read(RNG_STAT);
            if (stat.bits.rng_data_count > 0x00) {
                break;
            }
        }

        /* time out */
        if (times >= TRNG_TIMEOUT) {
            return HI_ERR_CIPHER_NO_AVAILABLE_RNG;
        }
    }
#else
    while (times++ < TRNG_TIMEOUT) {
        /* trng number is valid ? */
        stat.u32 = trng_read(RNG_STAT);
        if (stat.bits.rng_data_count > 0x00) {
            break;
        }
    }

    /* time out */
    if (times >= TRNG_TIMEOUT) {
        return HI_ERR_CIPHER_NO_AVAILABLE_RNG;
    }
#endif
    /* read valid randnum */
    *randnum = trng_read(RNG_FIFO_DATA);

    hi_log_info("randnum: 0x%x\n", *randnum);

    hi_log_func_exit();
    return HI_SUCCESS;
}

void drv_trng_get_capacity(trng_capacity *capacity)
{
    crypto_memset(capacity, sizeof(trng_capacity), 0,  sizeof(trng_capacity));

    capacity->trng = CRYPTO_CAPACITY_SUPPORT;

    return;
}
#endif /* End of CHIP_TRNG_VER_V100 */
