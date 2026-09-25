#ifndef __XMEDIA_NPU_API_H__
#define __XMEDIA_MPU_API_H__

#include "comm_npu.h"

#ifndef __KERNEL__
#include "streamer.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

xmedia_s32 xmedia_npu_module_init(xmedia_void);
xmedia_s32 xmedia_npu_module_uninit(xmedia_void);
xmedia_s32 xmedia_npu_open(xmedia_u32 dev_index, xmedia_u32 *dev_fd, xmedia_npu_open_params_s *npu_open_params);
xmedia_void xmedia_npu_close(xmedia_u32 dev_fd);
#ifndef __KERNEL__
xmedia_u32 xmedia_npu_get_memsize(xmedia_void);
xmedia_void xmedia_npu_set_external_mem(xmedia_u32 size, xmedia_void **virtaddr,
                                        xmedia_void **phyaddr);
xmedia_s32 xmedia_npu_get_devcnt(xmedia_u32 *devcnt);
xmedia_void xmedia_npu_reset(xmedia_u32 dev_fd);
xmedia_s32 xmedia_npu_create_queue(xmedia_u32 dev_fd);
xmedia_s32 xmedia_npu_destroy_queue(xmedia_u32 dev_fd);
xmedia_s32 xmedia_npu_query_job(xmedia_u32 dev_fd, xmedia_npu_job_state_t *status);
xmedia_s32 xmedia_npu_wait_job(xmedia_u32 dev_fd, xmedia_npu_job_state_t *status);
xmedia_s32 xmedia_npu_getabort(xmedia_npu_job_state_t *status);
xmedia_void xmedia_npu_get_base_addr(xmedia_u32 dev_fd, xmedia_u32 *addr);
#endif
xmedia_s32 xmedia_npu_submit_job(xmedia_u32 dev_fd, xmedia_npu_start_params_s *params, xmedia_u32 *jb_id);
xmedia_s32 xmedia_npu_get_capability(xmedia_u32 dev_fd, xmedia_npu_attribute_e param, xmedia_s32 *value);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __XMEDIA_NPU_API_H__ */
