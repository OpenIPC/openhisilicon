/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef _COMMOM_QR_H_
#define _COMMOM_QR_H_

#include "type.h"
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define QR_MAX_PAYLOAD_LEN 256

typedef enum {
    QR_STATE_DECODE_SUCCESS   = 0x00,
    QR_STATE_NON_EXISTENT     = 0x01,
    QR_STATE_DECODE_FAIL      = 0x02,
    QR_STATE_DATA_OVERFLOW    = 0x03,
    QR_STATE_BUTT
} qr_decode_state;

#define ERR_CODE_QR_ILLEGAL_PARAM     DEFINE_ERR_CODE(MOD_ID_QR, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define ERR_CODE_QR_EXIST             DEFINE_ERR_CODE(MOD_ID_QR, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
#define ERR_CODE_QR_NULL_PTR          DEFINE_ERR_CODE(MOD_ID_QR, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define ERR_CODE_QR_NO_MEM            DEFINE_ERR_CODE(MOD_ID_QR, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define ERR_CODE_QR_NOT_READY         DEFINE_ERR_CODE(MOD_ID_QR, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)

typedef struct {
    qr_decode_state state;
    xmedia_s32  version;
    xmedia_s32  ecc_level;
    xmedia_s32  mask;
    xmedia_s32  data_type;
    xmedia_s32  payload_len;
    xmedia_u32  eci;
    xmedia_u8   payload[QR_MAX_PAYLOAD_LEN];
} qr_info;

typedef struct {
    xmedia_u64 phys_addr;
    xmedia_u64 virt_addr;
    xmedia_u32 mem_size;
} qr_mem_info;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
