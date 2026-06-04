/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef TRANSFER_DATA_H
#define TRANSFER_DATA_H
#include "hal_spi.h"

int transfer_data_init(td_s32 irq_num, td_s32 irq_gpio_num);

void transfer_data_exit(td_u32 irq_num, td_s32 irq_gpio_num);

int transfer_data_send(unsigned long long memaddr, unsigned int length, int timeout);

#endif