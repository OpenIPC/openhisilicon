#ifndef __XMEDIA_NPU_H__
#define __XMEDIA_NPU_H__

#include "xmedia_npu_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

xmedia_s32 xmedia_npu_set_quick_start_flag(xmedia_u32 flag);
xmedia_s32 xmedia_npu_get_quick_start_flag(xmedia_u32 *flag);
xmedia_s32 xmedia_npu_get_proc_info(xmedia_npu_proc_t *proc);
xmedia_s32 xmedia_npu_get_usage_rate(xmedia_float *usage);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __XMEDIA_NPU_H__ */
