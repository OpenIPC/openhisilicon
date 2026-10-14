/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __DRV_MSENSOR_H__
#define __DRV_MSENSOR_H__

#include "xmedia_msensor.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

xmedia_s32 msensor_buf_add_user(xmedia_s32 dev, xmedia_s32 *id);
xmedia_s32 msensor_buf_delete_user(xmedia_s32 dev, const xmedia_s32 *id);
xmedia_s32 msensor_buf_get_data(xmedia_s32 dev, xmedia_msensor_data_info *msensor_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__DRV_MSENSOR_H__
