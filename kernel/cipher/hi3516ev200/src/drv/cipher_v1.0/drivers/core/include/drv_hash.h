/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_hash.h
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#ifndef __DRV_HASH_H__
#define __DRV_HASH_H__

/*************************** Structure Definition ****************************/
/** \addtogroup     hash */
/** @{ */  /** <!-- [hash] */

/*! \hash block size */
#define HASH_BLOCK_SIZE_64         (64)   /* SHA1, SHA224, SHA256 */
#define HASH_BLOCK_SIZE_128        (128)  /* SHA384, SHA512 */

/*! \hash capacity, 0-nonsupport, 1-support */
typedef struct {
    hi_u32 sha1        : 1 ;    /*!<  Support SHA1  */
    hi_u32 sha224      : 1 ;    /*!<  Support SHA224  */
    hi_u32 sha256      : 1 ;    /*!<  Support SHA256  */
    hi_u32 sha384      : 1 ;    /*!<  Support SHA384  */
    hi_u32 sha512      : 1 ;    /*!<  Support SHA512  */
    hi_u32 sm3         : 1 ;    /*!<  Support SM3  */
} hash_capacity;

/*! \hash mode */
typedef enum {
    HASH_MODE_SHA1,      /*!<  SHA1  */
    HASH_MODE_SHA224,    /*!<  SHA2 224  */
    HASH_MODE_SHA256,    /*!<  SHA2 256  */
    HASH_MODE_SHA384,    /*!<  SHA2 384  */
    HASH_MODE_SHA512,    /*!<  SHA2 512  */
    HASH_MODE_SM3,       /*!<  SM3  */
    HASH_MODE_COUNT,
} hash_mode;

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      hash */
/** @{ */  /** <!--[hash]*/


/**
\brief  Initialize the hash module.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 drv_hash_init(void);

/**
\brief  Deinitialize the hash module.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 drv_hash_deinit(void);

/**
\brief  suspend the hash module.
\retval     NA.
*/
void drv_hash_suspend(void);

/**
\brief  resume the hash module.
\retval     NA.
*/
void drv_hash_resume(void);

/**
\brief  set work params.
\param[in]  chn_num The channel number.
\param[in]  mode The hash mode.
\param[in] state The hash initial result.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 drv_hash_config(hi_u32 chn_num, hash_mode mode, hi_u32 state[HASH_RESULT_MAX_SIZE_IN_WORD]);

/**
\brief  start hash calculation.
\param[in]  chn_num The channel number.
\param[in]  buf_phy The MMZ/SMMU address of in buffer.
\param[in]  buf_size The MMZ/SMMU siae of in buffer.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hi_s32 drv_hash_start(hi_u32 chn_num, crypto_mem *mem, hi_u32 buf_size);

/**
\brief  wait running finished.
\param[in]  chn_num The channel number.
\param[out] state The hash result.
\param[in]  hashLen The length of hash result.
\retval     On received interception, HI_TRUE is returned  otherwise HI_FALSE is returned.
*/
hi_s32 drv_hash_wait_done(hi_u32 chn_num, hi_u32 *state);

/**
\brief  reset hash after hash finished.
\param[in]  chn_num The channel number.
\retval     NA.
*/
void drv_hash_reset(hi_u32 chn_num);

/**
\brief  compute a block hmac.
\param[in]  din block data.
\param[out] hamc The output.
\retval     On received interception, HI_TRUE is returned  otherwise HI_FALSE is returned.
*/
hi_s32 drv_hmac256_block(hi_u32 *din, hi_u32 *hamc);

/**
\brief  get the hash capacity.
\param[out] capacity The hash capacity.
\retval     NA.
*/
void drv_hash_get_capacity(hash_capacity *capacity);

/** @} */  /** <!-- ==== API declaration end ==== */

#endif
