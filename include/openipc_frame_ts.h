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

/*
 * Event source classification.
 *
 * MIPI_FS  — fired by the MIPI RX driver on the FS short packet (vendor
 *            term: `MIPI_CTRL_INT.int_vsync_st`, Hi3516CV500 DS section
 *            9, register at 0x12F4 bit [4]). Approximately "sensor
 *            starts streaming row 0 of this frame".
 *
 * ISP_FEND — fired by the ISP front-end's ISP_INT_FE register's FEND
 *            bit (bit [4]; kernel macro ISP_INT_FE_FEND). Approximately
 *            "ISP front-end finished receiving the last pixel of this
 *            frame from the sensor and the demosaic/AE/AWB pipeline
 *            entered the back half." On rolling-shutter sensors,
 *            ISP_FEND.wall_ns − MIPI_FS.wall_ns ≈ sensor readout
 *            duration + a small ISP front-end latency (a few hundred
 *            µs to ~1 ms).
 *
 * Consumers that don't care about source filtering can mask off
 * event_type and treat every event as a generic frame marker; the
 * ordering relation MIPI_FS_seq < ISP_FEND_seq holds frame-by-frame.
 */
#define OPENIPC_FT_EVT_MIPI_FS  0u
#define OPENIPC_FT_EVT_ISP_FEND 1u

struct openipc_frame_ts_event {
	__u64 pts_us;       /* same source as VENC_STREAM_S.u64PTS */
	__u64 wall_ns;      /* CLOCK_REALTIME at IRQ entry */
	__u32 vi_chn;       /* sensor channel (combo_dev_t / ISP pipe) */
	__u32 seq;          /* per-(chn, event_type) monotonic, wraps */
	__u32 event_type;   /* OPENIPC_FT_EVT_* — see above */
	__u32 reserved;     /* must be zero; reserved for future event sources */
};

#define OPENIPC_FT_IOC_MAGIC            'o'
#define OPENIPC_FT_IOC_SET_CHANNEL_MASK _IOW(OPENIPC_FT_IOC_MAGIC, 1, __u32)
/*
 * Ring-overflow drops only — events that arrived but couldn't be queued
 * because no reader drained the chrdev in time. This is genuine data
 * loss. Distinct from coalesced (below), which is intentional rejection
 * of within-dedupe-window duplicates from the level-held vsync / FEND
 * bits over-firing at high sensor IRQ rates.
 */
#define OPENIPC_FT_IOC_GET_DROPPED      _IOR(OPENIPC_FT_IOC_MAGIC, 2, __u64)
/*
 * Filter which event_types this fd receives. Bit n set ↔ events with
 * event_type == n pass through. Default is all bits set (every type).
 */
#define OPENIPC_FT_IOC_SET_EVENT_MASK   _IOW(OPENIPC_FT_IOC_MAGIC, 3, __u32)
/*
 * Dedupe-coalesced events — incremented when openipc_frame_ts_push is
 * called for an event_type within its dedupe window of the previous
 * push. These are NOT data loss: they're the level-held-bit duplicates
 * the dedupe is there to absorb. Compare against GET_DROPPED: drops →
 * size the ring or pace the reader; coalesced → tune the dedupe window
 * if it's rejecting real frames you wanted.
 */
#define OPENIPC_FT_IOC_GET_COALESCED    _IOR(OPENIPC_FT_IOC_MAGIC, 4, __u64)

#ifdef __KERNEL__
/*
 * Per-channel event hook. Safe to call from hardirq context.
 * `event_type` must be one of OPENIPC_FT_EVT_*.
 *
 * Called by per-SoC MIPI RX / ISP IRQ handlers at well-defined
 * pipeline points. The chrdev consumer decides which types it cares
 * about via OPENIPC_FT_IOC_SET_EVENT_MASK.
 */
void openipc_frame_ts_push(unsigned int vi_chn, unsigned int event_type);
#endif

#endif /* _OPENIPC_FRAME_TS_H */
