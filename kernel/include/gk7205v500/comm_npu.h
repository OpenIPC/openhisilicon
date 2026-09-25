#ifndef __COMM_NPU_H__
#define __COMM_NPU_H__

#include "type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if defined(__arm64__)
typedef XMEDIA_U64 addr_t;
#elif defined(__arm__)
typedef XMEDIA_U32 addr_t;
#else
#error Invalid ARCH
#endif

#define UNUSED(x) (void)(x)

typedef enum npu_attribute {
    NPU_VERSION_PERF = 0,
    NPU_VERSION_PIX,
    NPU_VERSION_PLATFORM,
    NPU_VERSION_SUBVERSION,
    NPU_ATTRIBUTE_MAX,
} xmedia_npu_attribute_e;

typedef enum job_state {
    JOB_PEND = 0,
    JOB_PROCESS = 1,
    JOB_SUCESS_FINISH = 2,
    JOB_SYNC_FINISH = 3,  //SI
    JOB_TEMP_FINISH = 4,  //PC
    JOB_ABORT_FINISH = 5, //DFX
    JOB_STATE_MAX,
} xmedia_npu_job_state_e;

typedef struct xmedia_npu_job_state {
    XMEDIA_U32 job_id;
    xmedia_npu_job_state_e state;
    XMEDIA_U32 abort_value;
    XMEDIA_U64 delay;
} xmedia_npu_job_state_t;

typedef struct {
    addr_t npu_base_addr;
    addr_t npu_end_addr;

#ifdef __KERNEL__
    xmedia_void *npu_reg_base_addr;
#endif
    /*!
    ** Flag to enable/disable the interrupt.
    */
    XMEDIA_BOOL interrupt_enable;
    XMEDIA_BOOL si_interrupt_enable;
    XMEDIA_BOOL dfx_interrupt_enable;
    XMEDIA_BOOL pc_interrupt_enable;
    /*!
    ** Flag to enable/disable the npu module.
    */
    XMEDIA_BOOL npu_low_power;
} xmedia_npu_open_params_s;

typedef struct {
    addr_t npu_cmd_base_addr;
    XMEDIA_U32 npu_cmd_data_size;
    XMEDIA_U32 npu_dynamic_input[4];
    addr_t npu_base_addr;
    addr_t npu_end_addr;
} xmedia_npu_start_params_s;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __COMM_NPU_H__ */
