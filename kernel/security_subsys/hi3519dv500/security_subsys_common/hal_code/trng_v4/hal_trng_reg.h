/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_TRNG_REG_H
#define HAL_TRNG_REG_H

#include "crypto_type.h"

/*! \Define the offset of TRNG reg */
#define HISC_COM_TRNG_FIFO_DATA          (0x100)
#define HISC_COM_TRNG_FIFO_READY         (0x104)
#define HISC_COM_TRNG_DATA_ST            (0x108)

#define TRNG_DONE                           1
#define TRNG_DATA_READY                     1

/* Define the union hisc_com_trng_fifo_ready */
typedef union {
    /* Define the struct bits */
    struct {
        td_u32    trng_data_ready             :  1;  /* [0]  */
        td_u32    trng_done                   :  1;  /* [1]  */
        td_u32    reserved_0                  :  30; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    td_u32    u32;
} hisc_com_trng_fifo_ready;

#endif
