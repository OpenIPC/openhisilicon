/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2016-2019. All rights reserved.
 * Description: mipi tx header file.
 * Author: Hisilicon multimedia software group
 * Create: 2016/06/28
 */

#ifndef __MIPI_TX_H__
#define __MIPI_TX_H__

#include "hi_osal.h"
#include "type.h"
#include "hi_mipi_tx.h"

#define hi_mipi_tx_err(x...) \
    do { \
        osal_printk("%s(%d): ", __FUNCTION__, __LINE__); \
        osal_printk(x); \
    } while (0)

#define mipi_tx_check_null_ptr_return(ptr)\
    do {\
        if ((ptr) == NULL) {\
            hi_mipi_tx_err("NULL point \r\n");\
            return -1;\
        }\
    } while (0)

typedef struct {
    combo_dev_cfg_t dev_cfg;
} mipi_tx_dev_ctx_t;

#endif
