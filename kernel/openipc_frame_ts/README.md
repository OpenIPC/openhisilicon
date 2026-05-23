# openipc_frame_ts — sensor frame-start timestamp chrdev

Captures `(encoder_pts, CLOCK_REALTIME)` pairs at the earliest kernel-visible
sensor frame-start IRQ and exposes them via `/dev/openipc-frame-ts`.

Purpose: anchor encoded frames to wall-clock time with ~3–5 ms accuracy
(NTP-disciplined) for RTCP Sender Reports, ROS 2 multi-sensor fusion, OSD
timecode, evidence chains. See [issue #144][issue].

## Attach point per SoC

| SoC family                  | Hook                                     | Build status     |
| --------------------------- | ---------------------------------------- | ---------------- |
| hi3516ev200 / ev300         | MIPI RX (shared `mipi_rx_hal.c`)         | active           |
| gk7205v200                  | MIPI RX (shared `mipi_rx_hal.c`)         | active           |
| hi3516cv500 / hi3516av300   | MIPI RX (`hi3516cv500/mipi_rx_hal.c`)    | active           |
| hi3516cv200 / hi3518ev200   | ISP `ISP_ISR` (`VI_PT0_INT_FSTART`)      | **not validated**, hook disabled (see below) |
| hi3516cv300, av100, v101    | n/a — ISP ships as prebuilt blob today   | blocked by #58   |
| hi3516cv100                 | n/a — no MIPI, no sensor frame-start IRQ | not supported    |

### hi3516cv200 / hi3518ev200 family — hook disabled

These two SoCs share `BR2_OPENIPC_SOC_FAMILY=hi3516cv200` in the firmware build,
which means our `open_isp.ko` is renamed to `hi3518e_isp.ko` and used as the
ISP driver on **both** hi3516cv200 cameras AND hi3518ev200 cameras. The single
`openipc_frame_ts_push(...)` call inside the cv200 `ISP_ISR` adds a few µs to
the ISR hot path which, on a real hi3518ev200 board, tips a latent
i2c-from-hardirq race (`rt_mutex_trylock` WARN at `rtmutex.c:1545` via
`hi_sensor_i2c_write → i2c_transfer`); majestic can no longer read the sensor
and `/image.jpg` returns HTTP 000 / 10 s timeout. The bug isn't strictly new —
backtrace matches a pre-existing issue — but our code is what tipped it over
the threshold.

The push call is therefore commented out in
`kernel/isp/arch/hi3516cv200/firmware/drv/isp.c`. The chrdev `/dev/openipc-frame-ts`
still exists on these cameras (the module loads cleanly), it just never
receives events. Consumers that probe for it will see an empty stream and fall
back to `clock_gettime()`.

Re-enable here only after the i2c-in-IRQ path is fixed upstream **or** after
the hook has been hardware-validated on both hi3516cv200 and hi3518ev200
boards under a real majestic stream. The wider re-scope is tracked alongside
the openipc/firmware revert of the opensdk bump.

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
narrows the per-fd channel set. `OPENIPC_FT_IOC_GET_DROPPED` returns total **ring-overflow** drops across
all channels (genuine data loss — events arrived but no reader drained
the ring in time). The ring is depth 256 per channel (≈ 0.5 s of buffer
at 240 fps × 2 event types). `OPENIPC_FT_IOC_GET_COALESCED` returns
total **dedupe rejections** — duplicate events the level-held vsync /
FEND bits emitted within the dedupe window of a previously-pushed event.
Coalesced events are *not* data loss; they're the over-fired duplicates
the dedupe is there to absorb. A nonzero `dropped` means reader can't
keep up; a nonzero `coalesced` is expected steady-state at high IRQ
rates and means the dedupe is doing its job.

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
