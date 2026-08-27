/*****************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : cryp_hash.h
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#ifndef __CRYP_HASH_H_
#define __CRYP_HASH_H_

#include "drv_osal_lib.h"
#include "drv_hash.h"

/******************************* API Declaration *****************************/
/** \addtogroup      hash */
/** @{ */  /** <!--[hash]*/

/** @}*/  /** <!-- ==== Structure Definition end ====*/

/******************************* API Code *****************************/
/** \addtogroup      cipher drivers*/
/** @{*/  /** <!-- [cipher]*/

/**
* \brief          Initialize crypto of hash *
*/
hi_s32 cryp_hash_init(hi_void);

/**
* \brief          Deinitialize crypto of hash
*/
hi_void cryp_hash_deinit(hi_void);

/**
 * \brief          Create hash handle
 *
 * \param mode     Hash mode
 * \return         ctx if successful, or NULL
 */
typedef hi_void *(*func_hash_create)(hash_mode mode);

/**
 * \brief          Clear hash context
 *
 * \param ctx      symc handle to be destory
 */
typedef hi_s32 (*func_hash_destory)( hi_void *ctx);

/**
 * \brief          Hash message chunk calculation
 *
 * Note: the message must be write to the buffer
 * which get from cryp_hash_get_cpu_addr, and the length of message chunk
 * can't large than the length which get from cryp_hash_get_cpu_addr.
 *
 * \param ctx      hash handle to be destory
 * \param chunk    hash message to update
 * \param length   length of hash message
 * \param src      source of hash message
 */
typedef hi_s32 (*func_hash_update)( hi_void *ctx, hi_u8 *chunk, hi_u32 chunkLen, hash_chunk_src src);

/**
 * \brief          HASH final digest
 *
 * \param ctx      Hash handle
 * \param hash     HASH checksum result
 * \param hashlen  Length of HASH checksum result
 */
typedef hi_s32 (*func_hash_finish)( hi_void *ctx,  hi_void *hash, hi_u32 *hashlen);

/*! \struct of Hash function template */
typedef struct {
    hi_u32 valid;                  /*!<  vliad or not */
    hi_u32 mode;                   /*!<  Mode of Hash */
    hi_u32 block_size;             /*!<  block size */
    hi_u32 size;                   /*!<  hash output size */
    func_hash_create  create;      /*!<  Create function */
    func_hash_destory destroy;     /*!<  destroy function */
    func_hash_update  update;      /*!<  update function */
    func_hash_finish  finish;      /*!<  finish function */
} hash_func;

/**
\brief  Clone the function from template of hash engine.
\param[out]  func The struct of function.
\param[in]  mode The work mode.
\retval     On success, HI_SUCCESS is returned.  On error, HI_FAILURE is returned.
*/
hash_func *cryp_get_hash(hash_mode mode);

/** @} */  /** <!-- ==== API declaration end ==== */
#endif
