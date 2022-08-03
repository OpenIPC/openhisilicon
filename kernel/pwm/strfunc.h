/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __STRFUNC_H__
#define __STRFUNC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define STRFMT_ADDR32 "%#010lX"
#define STRFMT_ADDR32_2 "0x%08lX"

extern int StrToNumber(char *str, unsigned int *ulValue);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __STRFUNC_H__ */
