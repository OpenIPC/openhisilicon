/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_srsa.h
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#ifndef __DRV_RSA_H__
#define __DRV_RSA_H__

/*************************** Structure Definition ****************************/
/** \addtogroup     rsa */
/** @{ */  /** <!-- [rsa] */

/*! Define RSA 1024 key length */
#define RSA_KEY_LEN_1024             128

/*! Define RSA 2048 key length */
#define RSA_KEY_LEN_2048             256

/*! Define RSA 3072 key length */
#define RSA_KEY_LEN_3072             384

/*! Define RSA 4096 key length */
#define RSA_KEY_LEN_4096             512

/*! \rsa capacity, 0-nonsupport, 1-support */
typedef struct {
    hi_u32 rsa         : 1 ;    /*!<  Support RSA */
} rsa_capacity;

/*! \rsa key width */
typedef enum {
    RSA_KEY_WIDTH_1024 = 0x00, /*!<  RSA 1024  */
    RSA_KEY_WIDTH_2048,        /*!<  RSA 2048  */
    RSA_KEY_WIDTH_4096,        /*!<  RSA 4096  */
    RSA_KEY_WIDTH_3072,        /*!<  RSA 3072  */
    RSA_KEY_WIDTH_COUNT,
} rsa_key_width;

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      rsa */
/** @{ */  /** <!--[rsa]*/


/**
\brief  Initialize the rsa module.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 drv_rsa_init(void);

/**
\brief  Deinitialize the rsa module.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 drv_rsa_deinit(void);

/**
\brief  RSA encrypt/decrypt use rsa exponent-modular arithmetic.
\param[in]  n The N of rsa key.
\param[in]  k The d/e of rsa key.
\param[in]  in The input data.
\param[out] out The input data.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 drv_ifep_rsa_exp_mod(hi_u32 ca_type, hi_u8 *n, hi_u8 *k, hi_u8 *in, hi_u8 *out, rsa_key_width width);

/**
\brief  get the hash capacity.
\param[out] capacity The hash capacity.
\retval     NA.
*/
void drv_ifep_rsa_get_capacity(rsa_capacity *capacity);

/**
\brief  get the hash capacity.
\param[out] capacity The hash capacity.
\retval     NA.
*/
void drv_sic_rsa_get_capacity(rsa_capacity *capacity);

/** @} */  /** <!-- ==== API declaration end ==== */

#endif
