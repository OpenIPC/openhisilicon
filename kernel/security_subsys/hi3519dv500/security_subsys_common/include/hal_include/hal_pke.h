/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_PKE_H
#define HAL_PKE_H


#include "crypto_common_struct.h"
#include "hal_common.h"

#define ECC_SET_KEY_LEN  80 // varies from chip to chip

#define RSA_KEY_LEN_2048 256
#define RSA_KEY_LEN_3072 384
#define RSA_KEY_LEN_4096 512

#define ECC_MAX_KEY_LEN  72

/* ! \Define the offset of reg */
#define PKE_VERSION_DATE      0x0

#define PKE_KEY_CRC           0x20
#define PKE_MRAM              0x200
#define PKE_PRAM              0x300
#define PKE_NRAM              0x600
#define PKE_KRAM              0xa00
#define PKE_RRAM              0xe00

#define PKE_MG_A24           (PKE_NRAM + 0x120)      // 0x720, for MG mul dot
#define PKE_MG_K             (PKE_KRAM + 0x20)       // 0xa20, for MG mul dot
#define PKE_ED_PD            (PKE_NRAM + 0x180)      // 0x780, for ED mul dot
#define PKE_ED_K             (PKE_KRAM + 0x20)       // 0xa20, for ED mul dot
#define PKE_ED_MRAM(i)       (PKE_MRAM + 0x20 * (i)) // 0x200 + 0x20 * i, for ED mul dot and ED add dot
#define PKE_ED_RRAM(i)       (PKE_RRAM + 0x20 * (i)) // 0xe00 + 0x20 * i, for ED mul dot and ED add dot
#define PKE_ED_NRAM(i)       (PKE_NRAM + 0x20 * (i)) // 0x600 + 0x20 * i, for ED add dot

/* ! Define the length of zero padding for mul-dot */
#define PKE_LEN_BLOCK_IN_WOED 0x02
#define PKE_LEN_BLOCK_IN_BYTE 0x08

#ifndef WORD_WIDTH
#define WORD_WIDTH 4
#endif

typedef enum {
    HAL_SET_KEY_LEN = 0,
    HAL_RAM_SECTION_LEN,
    HAL_ECC_512_LEN,
    HAL_ECC_521_LEN,
    HAL_ALIGN_LEN = 0xff,
} diff_len_in_chips;

typedef union {
    td_u8 byte[PKE_LEN_BLOCK_IN_BYTE];
    td_u32 word[PKE_LEN_BLOCK_IN_WOED];
} pke_block;

typedef enum {
    HAL_PKE_MODE_CLR_RAM        = 0,
    HAL_PKE_MODE_EXP_MOD,
    HAL_PKE_MODE_MUL_DOT,
    HAL_PKE_MODE_ADD_DOT,
    HAL_PKE_MODE_TIMES_DOT,
    HAL_PKE_MODE_MINV_MOD,
    HAL_PKE_MODE_SUB_MOD,
    HAL_PKE_MODE_MUL_MOD,
    HAL_PKE_MODE_ADD_MOD,
    HAL_PKE_MODE_MOD,
    HAL_PKE_MODE_MUL,
    HAL_PKE_MODE_MG_MUL_DOT,
    HAL_PKE_MODE_ED_MUL_DOT,
    HAL_PKE_MODE_ED_ADD_DOT,
    HAL_PKE_MODE_KGEN_NO_E,
    HAL_PKE_MODE_KGEN_WITH_E,
    HAL_PKE_MODE_KTRANS
} hal_pke_mode;

typedef enum {
    PKE_DATA_LEN_BYTE_32 = 4,
    PKE_DATA_LEN_BYTE_48 = 6,
    PKE_DATA_LEN_BYTE_64 = 8,
    PKE_DATA_LEN_BYTE_72 = 9,
    PKE_DATA_LEN_BYTE_128 = 16,
    PKE_DATA_LEN_BYTE_256 = 32,
    PKE_DATA_LEN_BYTE_384 = 48,
    PKE_DATA_LEN_BYTE_512 = 64,
} pke_data_len;

typedef enum {
    PKE_RAM_TYPE_MRAM = 0, /* 0x0200 ~ 0x05FF */
    PKE_RAM_TYPE_NRAM,     /* 0x0600 ~ 0x09FF */
    PKE_RAM_TYPE_KRAM,     /* 0x0a00 ~ 0x0dFF */
    PKE_RAM_TYPE_RRAM,     /* 0x0e00 ~ 0x11FF */
} pke_ram_type;


td_s32 hal_pke_init(void);

td_s32 hal_pke_deinit(void);

td_s32 hal_pke_lock(void);

td_s32 hal_pke_unlock(void);

td_s32 hal_pke_wait_free(void);

td_void hal_pke_start(void);

td_u32 hal_pke_done_try(void);

td_void hal_pke_done_notify(void);

td_s32 hal_pke_wait_done(void);

td_s32 hal_pke_set_mode(hal_pke_mode mode, td_u32 len);

td_void hal_pke_set_ram(td_u32 addr, const td_u8 *ram, td_u32 data_len, td_u32 padded_len);

td_void hal_pke_get_ram(td_u32 addr, td_u8 *ram, td_u32 klen);

td_void hal_pke_set_key(const td_u8 *inkey, td_u8 *outkey, td_u32 klen, pke_block *random, td_u32 pad_len);

td_s32 hal_pke_align_len(td_u32 a_len, td_u32 *a_len_aligned, diff_len_in_chips len);

td_void hal_pke_secure(td_bool enable);

td_s32 hal_cipher_pke_register_wait_func(td_void *wait,
    crypto_wait_timeout_interruptible wait_func, td_u32 timeout_ms);

#endif