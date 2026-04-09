/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : drivers for drv trng v200.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#include "drv_trng_v200.h"
#include "drv_trng.h"

#ifdef CHIP_TRNG_VER_V200

/* ************************** Internal Structure Definition *************************** */
/* Define the osc sel */
#define TRNG_OSC_SEL                0x02
/* ****************************** API Declaration **************************** */
hi_s32 drv_trng_randnum(hi_u32 *randnum, hi_u32 timeout)
{
    hisec_com_trng_data_st stat;
    hisec_com_trng_ctrl ctrl;
    static hi_u32 last = 0x0A;
    hi_u32 times = 0;

    hi_log_func_enter();

    hi_log_chk_param_return(randnum == HI_NULL);

    ctrl.u32 = trng_read(HISEC_COM_TRNG_CTRL);
    if (ctrl.u32 != last) {
        module_enable(CRYPTO_MODULE_ID_TRNG);

        ctrl.bits.mix_enable = 0x00;
        ctrl.bits.drop_enable = 0x00;
        ctrl.bits.pre_process_enable = 0x00;
        ctrl.bits.post_process_enable = 0x00;
        ctrl.bits.post_process_depth = 0x00;
        ctrl.bits.drbg_enable = 0x01;
        ctrl.bits.osc_sel = TRNG_OSC_SEL;
        trng_write(HISEC_COM_TRNG_CTRL, ctrl.u32);
        last = ctrl.u32;
    }

    if (timeout == 0) { /* unblock */
        /* trng number is valid ? */
        stat.u32 = trng_read(HISEC_COM_TRNG_DATA_ST);
        if (stat.bits.trng_fifo_data_cnt == 0) {
            return HI_ERR_CIPHER_NO_AVAILABLE_RNG;
        }
    } else { /* block */
        while (times++ < timeout) {
            /* trng number is valid ? */
            stat.u32 = trng_read(HISEC_COM_TRNG_DATA_ST);
            if (stat.bits.trng_fifo_data_cnt > 0) {
                break;
            }
        }

        /* time out */
        if (times >= timeout) {
            return HI_ERR_CIPHER_NO_AVAILABLE_RNG;
        }
    }

    /* read valid randnum */
    *randnum = trng_read(HISEC_COM_TRNG_FIFO_DATA);
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
#endif /* End of CHIP_TRNG_VER_V200 */
