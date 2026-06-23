/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef HAL_CA_MISC_REG_H
#define HAL_CA_MISC_REG_H

/* ! Define the crg control reg */
#define CPU_ID_STAT                         (0x0018)

typedef enum {
    CPU_ID_AIDSP = 0x00000035,
    CPU_ID_PCPU = 0x0000006a,
    CPU_ID_SCPU = 0x000000a5,
    CPU_ID_ACPU = 0x000000aa,
    CPU_ID_INVALID = 0xffffffff
} ca_misc_cpu_id;

#endif