/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef CA_MISC_REGISTER_H
#define CA_MISC_REGISTER_H

/* ! Define the crg control reg */
#define PKE_CRG_CTRL                        (0x0028)
#define CORE_LP_CTRL                        (0x002C)

#define CPU_ID_STAT                         (0x0018)

typedef enum {
    CPU_ID_AIDSP = 0x00000035,
    CPU_ID_PCPU = 0x0000006a,
    CPU_ID_SCPU = 0x000000a5,
    CPU_ID_ACPU = 0x000000aa,
    CPU_ID_INVALID = 0xffffffff
} ca_misc_cpu_id;

#define SPACC_SEC_DFA_EN                    (0x0120)
#define SPACC_DFA_ENABLE                    (0x05)
#define SPACC_DFA_DISABLE                   (0x0A)

#endif