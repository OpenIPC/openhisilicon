/* SPDX-License-Identifier: GPL-2.0 */
/*
 * /dev/npu.0 ioctl ABI, as libxmedia_npu.so (XMediaIPCLinuxV100R002C00SPC020)
 * issues it. The command numbers and payload sizes are fixed by that binary;
 * the payloads mirror xmedia_npu_open_params_s, xmedia_npu_start_params_s and
 * xmedia_npu_job_state_t from the SDK's public comm_npu.h.
 */
#ifndef NPU_IOCTL_H
#define NPU_IOCTL_H

#include <linux/ioctl.h>
#include <linux/types.h>

/* Kernel layout of xmedia_npu_open_params_s: only the window is used. */
struct npu_open_params {
	__u32 base_addr;	/* NPU DDR window start (physical) */
	__u32 end_addr;		/* NPU DDR window end (physical) */
	__u32 reg_base;		/* kernel pointer slot, ignored */
	__u32 int_en;
	__u32 si_int_en;
	__u32 dfx_int_en;
	__u32 pc_int_en;
	__u32 low_power;
};

/* xmedia_npu_start_params_s */
struct npu_start_params {
	__u32 cmd_addr;		/* compiled command stream (physical) */
	__u32 cmd_size;
	__u32 dynamic_input[4];	/* mailbox values, ~0 = leave unset */
	__u32 base_addr;	/* 0 = keep the window set at open */
	__u32 end_addr;
};

/* xmedia_npu_job_state_e */
enum npu_job_state {
	NPU_JOB_PEND = 0,
	NPU_JOB_PROCESS = 1,
	NPU_JOB_SUCCESS = 2,
	NPU_JOB_SYNC = 3,	/* SI: sync point reached, job still owns the NPU */
	NPU_JOB_TEMP = 4,	/* PC: breakpoint reached, job still owns the NPU */
	NPU_JOB_ABORT = 5,	/* DFX: hardware error, abort_value has the report */
	NPU_JOB_UNKNOWN = 6,	/* finished but already collected, or never seen */
};

/* xmedia_npu_job_state_t; delay is the wait timeout in ms (0 = forever) */
struct npu_job_status {
	__u32 job_id;
	__u32 state;
	__u32 abort_value;
	__u32 pad;
	__u64 delay;
};

struct npu_profiling {
	__u32 stct_addr;
	__u32 stct_size;
	__u32 reserved[6];
};

#define NPU_IOC_MAGIC 'n'

#define NPU_IOC_OPEN		_IOW(NPU_IOC_MAGIC, 0x1e, struct npu_open_params)
#define NPU_IOC_CREATE_QUEUE	_IOWR(NPU_IOC_MAGIC, 0x1f, __u32)
#define NPU_IOC_DESTROY_QUEUE	_IOWR(NPU_IOC_MAGIC, 0x20, __u32)
#define NPU_IOC_SUBMIT_JOB	_IOWR(NPU_IOC_MAGIC, 0x21, struct npu_start_params)
#define NPU_IOC_WAIT_JOB	_IOWR(NPU_IOC_MAGIC, 0x22, struct npu_job_status)
#define NPU_IOC_QUERY_JOB	_IOWR(NPU_IOC_MAGIC, 0x23, struct npu_job_status)
#define NPU_IOC_CLOSE		_IOR(NPU_IOC_MAGIC, 0x24, __u32)
#define NPU_IOC_CLR_WFI		_IOW(NPU_IOC_MAGIC, 0x25, __u32)
#define NPU_IOC_RESET		_IOWR(NPU_IOC_MAGIC, 0x26, __u32)
#define NPU_IOC_SET_PROFILING	_IOWR(NPU_IOC_MAGIC, 0x27, struct npu_profiling)
#define NPU_IOC_GET_CAPABILITY	_IOWR(NPU_IOC_MAGIC, 0x28, __u32)
#define NPU_IOC_GET_MEMSIZE	_IOWR(NPU_IOC_MAGIC, 0x29, __u32)
#define NPU_IOC_SET_MEMSIZE	_IOWR(NPU_IOC_MAGIC, 0x2a, __u32)

#endif /* NPU_IOCTL_H */
