#ifndef __XMEDIA_NPU_COMMON_H__
#define __XMEDIA_NPU_COMMON_H__

#include "xmedia_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if defined(__arm64__)
typedef xmedia_u64 xmedia_addr;
#elif defined(__arm__)
typedef xmedia_u32 xmedia_addr;
#else
#error Invalid ARCH
#endif

typedef enum job_state {
    JOB_PEND = 0,
    JOB_PROCESS = 1,
    JOB_SUCESS_FINISH = 2,
    JOB_SYNC_FINISH = 3,
    JOB_TEMP_FINISH = 4,
    JOB_ABORT_FINISH = 5,
    JOB_STATE_MAX,
} xmedia_npu_job_state_e;

typedef struct {
    xmedia_addr phy_addr;
    xmedia_u32 buff_len;
} xmedia_npu_proc_t;

typedef enum {
    XMEDIA_NPU_FORMAT_RGB = 0,
    XMEDIA_NPU_FORMAT_RGrGbB,
    XMEDIA_NPU_FORMAT_BGbGrR,
    XMEDIA_NPU_FORMAT_GrRBGb = 3,
    XMEDIA_NPU_FORMAT_GbBRGr,
    XMEDIA_NPU_FORMAT_YUV,
    XMEDIA_NPU_FORMAT_YVU = 6,
} xmedia_npu_data_format;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __XMEDIA_NPU_COMMON_H__ */
