/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2016-2019. All rights reserved.
 * Description: Common data types of mipi
 * Author: Hisilicon multimedia software group
 * Create: 2016-11-08
 */

#ifndef __TYPE_H__
#define __TYPE_H__

#include <linux/types.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifndef NULL
    #define NULL    0L
#endif

#ifndef FALSE
    #define FALSE   0
#endif

#ifndef TRUE
    #define TRUE    1
#endif

#ifndef HI_SUCCESS
#define HI_SUCCESS  0
#endif

#ifndef  HI_FAILURE
#define HI_FAILURE  (-1)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __TYPE_H__ */
