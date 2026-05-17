/* SPDX-License-Identifier: GPL-2.0 */
/*
 * OpenIPC frame-timestamp chrdev ABI + kernel hook API.
 *
 * The struct openipc_frame_ts_event layout is a stable userspace ABI.
 * Never modify without an ABI bump.
 */
#ifndef _OPENIPC_FRAME_TS_H
#define _OPENIPC_FRAME_TS_H

#include <linux/types.h>
#include <linux/ioctl.h>

struct openipc_frame_ts_event {
	__u64 pts_us;    /* same source as VENC_STREAM_S.u64PTS */
	__u64 wall_ns;   /* CLOCK_REALTIME at frame-start IRQ */
	__u32 vi_chn;    /* sensor channel (combo_dev_t) */
	__u32 seq;       /* per-channel monotonic, wraps */
};

#define OPENIPC_FT_IOC_MAGIC            'o'
#define OPENIPC_FT_IOC_SET_CHANNEL_MASK _IOW(OPENIPC_FT_IOC_MAGIC, 1, __u32)
#define OPENIPC_FT_IOC_GET_DROPPED      _IOR(OPENIPC_FT_IOC_MAGIC, 2, __u64)

#ifdef __KERNEL__
/*
 * Per-channel frame-start hook. Safe to call from hardirq context.
 * Called by per-SoC MIPI RX / ISP IRQ handlers.
 */
void openipc_frame_ts_push(unsigned int vi_chn);
#endif

#endif /* _OPENIPC_FRAME_TS_H */
