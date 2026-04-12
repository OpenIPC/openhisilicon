/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of strfunc.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __STRFUNC_H__
#define __STRFUNC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define STRFMT_ADDR32   "%#010lX"
#define STRFMT_ADDR32_2 "0x%08lX"

int str_to_number(char *str, unsigned int *ul_value);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __STRFUNC_H__ */
