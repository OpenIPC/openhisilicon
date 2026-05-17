# openipc_frame_ts — sensor frame-start timestamp chrdev

Captures `(encoder_pts, CLOCK_REALTIME)` pairs at the earliest kernel-visible
sensor frame-start IRQ and exposes them via `/dev/openipc-frame-ts`.

Purpose: anchor encoded frames to wall-clock time with ~3–5 ms accuracy
(NTP-disciplined) for RTCP Sender Reports, ROS 2 multi-sensor fusion, OSD
timecode, evidence chains. See [issue #144][issue].

## Attach point per SoC

| SoC family                  | Hook                                     | Build status   |
| --------------------------- | ---------------------------------------- | -------------- |
| hi3516ev200 / ev300         | MIPI RX (shared `mipi_rx_hal.c`)         | active         |
| gk7205v200                  | MIPI RX (shared `mipi_rx_hal.c`)         | active         |
| hi3516cv500 / hi3516av300   | MIPI RX (`hi3516cv500/mipi_rx_hal.c`)    | active         |
| hi3516cv200                 | ISP `ISP_ISR` (`VI_PT0_INT_FSTART`)      | active         |
| hi3516cv300, av100, v101    | n/a — ISP ships as prebuilt blob today   | blocked by #58 |
| hi3516cv100                 | n/a — no MIPI, no sensor frame-start IRQ | not supported  |

MIPI RX is the preferred attach point where the IP exposes a positive
frame-start interrupt (`MIPI_CTRL_INT.int_vsync` bit 4 or
`LVDS_CTRL_INT.lvds_vsync` bit 28). On older SoCs whose MIPI RX hardware
carries only `vsync_err_*` bits, the ISP `ISP_ISR` top-half is the fallback.

## Userspace ABI

See `include/openipc_frame_ts.h`. Stable; never modify
`struct openipc_frame_ts_event` without an ABI bump.

```c
struct openipc_frame_ts_event {
    __u64 pts_us;    /* same source as VENC_STREAM_S.u64PTS */
    __u64 wall_ns;   /* CLOCK_REALTIME at frame-start IRQ */
    __u32 vi_chn;    /* sensor channel (combo_dev_t) */
    __u32 seq;       /* per-channel monotonic, wraps */
};
```

`read()` blocks (or returns `-EAGAIN` if `O_NONBLOCK`) until at least one
event is available, then drains as many full events as fit in the supplied
buffer. `poll()`/`select()` supported. `OPENIPC_FT_IOC_SET_CHANNEL_MASK`
narrows the per-fd channel set. `OPENIPC_FT_IOC_GET_DROPPED` returns total
drops across all channels (the ring drops oldest on overflow at depth 64
per channel).

## Module loading

`open_mipi_rx.ko` (and on hi3516cv200, `open_isp.ko`) reference
`openipc_frame_ts_push` via `EXPORT_SYMBOL`, so modpost stamps
`depends=open_openipc_frame_ts` into their `modinfo`. `depmod` propagates
this into `modules.dep`, and `modprobe open_mipi_rx` (used by
`load_hisilicon`) auto-loads `open_openipc_frame_ts.ko` first.

No `MODULE_SOFTDEP` is needed — the symbol-level dependency is sufficient
under `modprobe`. If you must use `insmod` directly, insert
`open_openipc_frame_ts.ko` before `open_mipi_rx.ko`.

## Test

```sh
cd samples/openipc_frame_ts && make
scp openipc_frame_ts_test root@camera:/tmp/
ssh root@camera /tmp/openipc_frame_ts_test       # one log line per event
ssh root@camera /tmp/openipc_frame_ts_test -s    # per-second summary
```

Validated on a Hi3516AV300 (cv500 family) with a 21 fps sensor: 100
consecutive events with monotonic `seq`, mean inter-frame PTS = 47.0 ms,
PTS-vs-wall_ns drift = ~3 µs/frame.

[issue]: https://github.com/OpenIPC/openhisilicon/issues/144
