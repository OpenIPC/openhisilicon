/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#ifndef TRANSFER_DEV_H
#define TRANSFER_DEV_H

#include "hal_spi.h"

struct transfer_frame_info {
    unsigned int size;
    unsigned long long frame_phys_addr;
    int timeout;
    unsigned long long frame_virt_addr;
};

#define TRANSFER_IOC_MAGIC 'S'
#define TRANSFER_SEND_FRAME _IOW(TRANSFER_IOC_MAGIC, 1, struct transfer_frame_info)

#endif