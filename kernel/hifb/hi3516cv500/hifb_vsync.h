/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: hifb hal operation
 */

#ifndef __HIFB_VSYNC_H__
#define __HIFB_VSYNC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

int hifb_pts_init(void);

void hifb_pts_exit(void);

int hifb_vsync_init(void);

void hifb_vsync_exit(void);

int hifb_vsync_notify(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HIFB_VSYNC_H__ */
