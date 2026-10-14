#ifndef __BF3A03_H__
#define __BF3A03_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BF3A03_FULL_LINES_MAX      0xFFFF // 未找到vmax对应的寄存器
#define BF3A03_VMAX_640X480_LINEAR 0x465  // 未找到vmax对应的寄存器

#define BF3A03_EXP_OFFSET_LINEAR 0 // TODO: 待确认

#define BF3A03_EXP_SHIFT 0

typedef enum {
    BF3A03_REG_MAX_NUM
} bf3a03_reg_info;

#ifdef __cplusplus
}
#endif

#endif // __BF3A03_H__
