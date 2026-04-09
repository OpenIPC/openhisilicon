/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description   : head file for drv trng.
 * Author        : Hisilicon multimedia software group
 * Create        : 2017-10-20
 */

#ifndef __DRV_TRNG_H__
#define __DRV_TRNG_H__

/* rsa capacity, 0-nonsupport, 1-support. */
typedef struct {
    hi_u32 trng         : 1;    /* Support TRNG. */
} trng_capacity;

/*
 * \brief get rand number.
 * \param[out]  randnum rand number.
 * \param[in]   timeout time out.
 * \retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
 */
hi_s32 drv_trng_randnum(hi_u32 *randnum, hi_u32 timeout);

/*
 * \brief  get the trng capacity.
 * \param[out] capacity The hash capacity.
 * \retval     NA.
 */
void drv_trng_get_capacity(trng_capacity *capacity);
#endif
