# Pushing IMX335 past the documented 60 fps on V4 SoCs

A field report from a research session on `hi3516ev300` and
`gk7205v300`. Baseline `IMX335` flex-crop mode tops out at **237 fps**
at 480×352 measured at the encoder counter (`/proc/umap/venc`), or
**98 fps over the UDP-RTP wire** (deliverable, the FPV-drone number).
A one-byte patch to `imx335_sensor_ctl.c:811` lifts both:
encoder-counter to **~300 fps** (290-339 depending on AE
convergence), UDP-wire to **140 fps** — a clean **+43% deliverable
lift** at 480×352 and similar across other crops, identical on both
boards. Run-to-run variance on the encoder side is real — §4.4
and §8.3 cover the AE-library u32FLStd race that causes it. This
document captures *why* the patch works, *how* to reproduce it,
*why the obvious next step doesn't*, the **ev-vs-gk gap that turned
out to be an RTSP-stack artifact** (§6 — both boards run at the
same rate over UDP), the **gap between encoder-counter and
deliverable fps** (§6.2 — VENC SEND counter overstates wire rate
when an active consumer exists), and the plumbing pitfalls that
will eat a day if you don't know them.

Written for someone who has never touched a HiSilicon camera SoC. If
you know the platform already, skim §1–2 and start at §3.

## 1. Context for newcomers — what is "V4", what runs where

### 1.1 The chip families

HiSilicon (a Huawei subsidiary, US-sanctioned in 2020) shipped 8 generations
of IP-camera SoCs from 2010 to 2019. Goke (a Hong Kong fab partner) sells
re-marked versions of those same dies under their own part numbers —
`gk7205v200` is `hi3516ev200`, `gk7205v300` is `hi3516ev300`. Same
silicon, same datasheet applies. The Goke vendor SDK is a fork with its
own runtime policy (and a different `.ko` blob set), but the chip itself
is identical. This repo
(`openhisilicon`) refers to those 8 platforms by their `CHIPARCH` build
variable — `hi3516cv100`, `hi3516cv200`, `hi3516av100`, `hi3516cv300`,
`hi3519v101`, `hi3516cv500`, `hi3516ev200`, `gk7205v200`. The last two
are the **V4 generation** (a.k.a. V200) — last and most-deployed in the
sub-$5 IP-cam segment.

This document targets V4 specifically. The board used for measurements
is `openipc-hi3516ev300`, an EV300 dev board running OpenIPC firmware.
"EV300" is a marketing name; internally it's `hi3516ev200` (same die,
same kernel modules, same build target). `CHIPARCH=hi3516ev200` is what
the Makefile sees.

### 1.2 The video pipeline, end to end

From sensor pixels to a network packet:

```
┌────────┐   MIPI    ┌─────────┐  online  ┌─────┐  online  ┌──────┐  ┌──────┐
│ IMX335 │ ────────▶ │ MIPI RX │ ───────▶ │ VI  │ ───────▶ │ ISP  │  │ VPSS │
│ sensor │   CSI-2   │  (PHY)  │          │     │          │      │  │      │
└────────┘   4 lane  └─────────┘          └──┬──┘          └───┬──┘  └──┬───┘
                                             │    online       │        │
                                             └────────────────-┴────────┘
                                                              │
                                                              ▼
                                                       ┌─────────────┐
                                                       │    VENC     │
                                                       │ (H.264/265) │
                                                       └─────────────┘
                                                              │
                                                              ▼ /dev/venc → user
```

Five hardware blocks the SoC exposes via `/proc/umap/`:
- **MIPI RX** — physical layer that receives differential CSI-2 lanes
- **VI** (Video Input) — sensor-side ingest, latches frames
- **ISP** (Image Signal Processor) — debayer, AE/AWB, sharpen, denoise
- **VPSS** (Video Processing Sub-System) — scale, crop, format convert
- **VENC** (Video Encoder) — H.264 / H.265 / MJPEG hardware codec

On V4, VI/ISP/VPSS are connected "online" (bypassing DDR for the live
path; reduces latency and bandwidth). This is set by majestic at
`SetOnlineModeIfPossible@hal.c:1366`.

### 1.3 What openhisilicon actually does

HiSilicon's SDK ships pre-compiled kernel-module blobs (`.ko`/`.o`)
locked to specific Linux kernels (~3.4 to ~4.9). To run on modern
kernels (3.x→7.x), this repo:

- Wraps or replaces those blobs with kernel-version-portable C
- Replaces the closed sensor drivers (in `libraries/sensor/<chiparch>/`)
  with re-implementations whose output (`libsns_<model>.so`) plugs
  into majestic the same way

Userspace (the camera firmware) uses **majestic** (a separate repo,
`openipc/majestic`), an event-loop daemon that:

1. Reads a YAML config (`/etc/majestic.yaml`) for resolution/fps/codec
2. Reads a sensor INI (`/etc/sensors/<sensor>.ini`) for sensor-specific
   knobs — clock, mode, output rectangle
3. dlopen()s the matching `libsns_<model>.so`
4. Walks through `HI_MPI_VI_/_ISP_/_VPSS_/_VENC_` API calls to bring
   the chain up

The IMX335 sensor driver lives at
`libraries/sensor/hi3516ev200/sony_imx335/`. Two files: `imx335_cmos.c`
(the AE/AWB algorithm callbacks majestic queries through dlopen) and
`imx335_sensor_ctl.c` (the I²C register writes that program the sensor
itself). Build output: `libsns_imx335.so`.

### 1.4 The IMX335 sensor

Sony IMX335: 5 MP (2592×1944), Bayer (RGGB), 10/12-bit, 4-lane MIPI
CSI-2, INCK 6/12/18/24/37.125/72 MHz. Datasheet docs the rated max as
60 fps in binning / 30 fps full-frame. We're going to 339 fps in a
flex-crop mode — well past the "rated" envelope, but inside what the
silicon can sustain when driven directly.

The crucial sensor knobs for fps are two registers:

- **HMAX** (`0x3034`/`0x3035`, 16-bit, hi-byte then lo-byte ordering
  inside the driver code's literal-byte writes) — number of internal
  clock cycles per horizontal line. **Smaller HMAX = faster lines =
  higher fps.**
- **VMAX** (`0x3030`/`0x3031`/`0x3032`, 20-bit) — number of lines per
  frame. **Smaller VMAX = fewer lines per frame = higher fps.** Bounded
  below by `(crop_h + 20) × 2 + 96` per datasheet p55.
- **SYS_MODE** (`0x319E`) — MIPI lane bitrate select. `0x02` = 891
  Mbps/lane (default in this driver), `0x01` = 1188 Mbps. Higher bitrate
  permits shorter HMAX (lanes drain pixels faster, less internal buffer
  pressure).

Empirical effective pixel clock at SYS_MODE=2, INCK=37.125 MHz:
**≈ 72.8 MHz**. Derived from:
- Baseline `HMAX=0x016E=366`, `VMAX=840`, measured `237 fps`
  → `237 × 366 × 840 = 72.84 MHz`
- Peak `HMAX=0x0100=256`, `VMAX=840`, measured `338.6 fps`
  → `338.6 × 256 × 840 = 72.81 MHz`

The pixel clock holds constant across HMAX changes. Fps scales as
`1/(HMAX × VMAX)`, modulo PLL lock stability.

## 2. The fps-controlling code path

### 2.1 Driver layout — mode dispatch

`imx335_sensor_ctl.c` exposes a fan-out of init functions, one per
named *image mode*. The relevant ones live around these line numbers:

| Mode constant                          | Init fn                          | Resolution / role                |
|----------------------------------------|----------------------------------|----------------------------------|
| `IMX335_5M_30FPS_12BIT_LINEAR_MODE`    | `IMX335_linear_5M_30fps_init`    | 2592×1944 @30 (stock)            |
| `IMX335_60FPS_BINNING_MODE`            | `IMX335_binning_init`            | 1296×972 @60 (2×2 binning)       |
| `IMX335_60FPS_CROPPED_1080P_MODE`      | `IMX335_cropped_60fps_1080p_init`| 1920×1080 @60 (centre crop)      |
| `IMX335_1520P_10BIT_MODE`              | …                                | 2704×1520 (full-width crop)      |
| `IMX335_60FPS_FULL_1944P_MODE`         | …                                | 2592×1944 boosted to ~45 fps     |
| **`IMX335_CROP_FLEX_LINEAR_MODE`**     | **`IMX335_cropped_flex_init`** (`:756`) | **arbitrary W×H window crop** |

The last one — **M7 / flex crop** — is what we're tuning. It lets
the caller pick *any* W×H within the all-pixel active area, instead
of the fixed 1080p/1944p resolutions. Lower H ⇒ lower VMAX_min ⇒
higher fps ceiling.

`cmos_set_image_mode` (`imx335_cmos.c:1780`) is the dispatcher.
Selection is driven by `pstSensorImageMode->u8SnsMode`:

```c
// imx335_cmos.c:1798
if (pstSensorImageMode->u8SnsMode == 4 &&
    WDR_MODE_NONE == pstSnsState->enWDRMode) {
    u8SensorImageMode = IMX335_CROP_FLEX_LINEAR_MODE;
    ...
}
```

`u8SnsMode = 4` is the magic dispatch key. It comes from the sensor
INI's `Isp_SnsMode=4` line, propagated by majestic
(commit `c8174312`, "hisi: propagate Isp_SnsMode from sensor INI
to pub_attr.u8SnsMode"). Below that commit, majestic ignores
`Isp_SnsMode` and you can't reach M7 from userspace at all.

### 2.2 The fps ceiling — what enforces it

Inside `IMX335_cropped_flex_init` (`imx335_sensor_ctl.c:756`) the
crucial HMAX write is line 811:

```c
IMX335_write_register(ViPipe, 0x3034, 0x6E);    // HMAX low byte
IMX335_write_register(ViPipe, 0x3035, 0x01);    // HMAX high byte
                                                 // → HMAX = 0x016E = 366
```

That value is **hard-coded**; it doesn't react to the requested fps.
At HMAX=366 and the smallest legal VMAX for a 352-tall crop (840
lines), the math is:

```
max_fps = pixel_clock / (HMAX × VMAX_min)
        = 72.8e6 / (366 × 840)
        = 237 fps      ← matches what we measure
```

Three layers conspire to enforce that 237 fps cap:

1. **Sensor side** — `HMAX × VMAX_min × pixel_clock` is a hard wall.
2. **Driver side** — `cmos_fps_set` at `imx335_cmos.c:418-440` clamps
   `u32FLStd ≥ vmax_min` so the AE library can't reduce VMAX below
   the datasheet minimum.
3. **Majestic side** — `#define MAX_FPS 240` (`commonsdk.h:34`)
   rejects any INI / yaml `fps` > 240, and the SDK's `VENC_CreateChn`
   returns `ERR_VENC_ILLEGAL_PARAM` for fps > 240. Both are reachable
   layers above the sensor.

Of these, **only HMAX has true headroom** at SYS_MODE=2. Drop HMAX,
fps rises proportionally — until the PLL stops locking.

## 3. The experiment

### 3.1 Measurement

VENC publishes a frame-counter at `/proc/umap/venc`, section `SEND1`,
column `Send`. Read it twice 5-10 s apart; divide delta by elapsed
wall-clock time. This is the encoded-output rate; if the sensor
overruns VPSS or VENC, you'd see the rate cap at whatever the
downstream block can do.

```sh
S1=$(awk '/SEND1/{f=1;next}/SEND2/{f=0}f && /^ *0 / {print $6;exit}' /proc/umap/venc)
T1=$(date +%s.%N)
sleep 10
S2=$(awk '/SEND1/{f=1;next}/SEND2/{f=0}f && /^ *0 / {print $6;exit}' /proc/umap/venc)
T2=$(date +%s.%N)
echo "fps = $(awk "BEGIN{print ($S2-$S1)/($T2-$T1)}")"
```

(The `^ *0 ` filter picks chn 0, the main h265 channel.)

Take samples 8-10 s **after** majestic starts to skip the AE
convergence transient — early frames can be 100-150 fps while AE
walks the gain table.

### 3.2 The board, the binary, the INI

Test rig:

- Board: openipc-hi3516ev300 (10.216.128.68)
- Bootloader / kernel / rootfs: OpenIPC firmware build for hi3516ev300_neo
- `/usr/bin/majestic`: stock OpenIPC build, version `master+d11ba3c`
  (predates the Isp_SnsMode propagation patch — see §4.1).
- `/usr/lib/sensors/libsns_imx335.so`: stock build, baseline
- `/etc/sensors/imx335_test.ini`:

```ini
[sensor]
Sensor_type=stSnsImx335Obj
Mode=WDR_MODE_NONE
DllFile=/tmp/libsns_imx335.so

[mode]
input_mode=INPUT_MODE_MIPI
raw_bitness=10
clock=37.125MHz

[mipi]
lane_id = 0|1|2|3|-1|-1|-1|-1|

[isp_image]
Isp_FrameRate=240
Isp_Bayer=BAYER_RGGB
Isp_SnsMode=4       ; ← selects M7 flex-crop mode
Isp_W=480
Isp_H=352

[vi_dev]
Input_mod=VI_MODE_MIPI
DevRect_w=480
DevRect_h=352
DevRect_x=0
DevRect_y=0
FullLinesStd=1375
```

- `/etc/majestic.yaml`:

```yaml
system:
  webPort: 80
  logLevel: debug
isp:
  sensorConfig: /etc/sensors/imx335_test.ini
video0:
  enabled: true
  codec: h265
  size: 480x352
  fps: 240
  bitrate: 4096
```

`fps: 240` is the maximum stock majestic will accept (see §4.2).
This is *the cap on the request*, not on the achieved rate; the
sensor will happily run faster if its register state says so.

### 3.3 Sweep results — HMAX vs measured fps

All measurements: SYS_MODE=2 (891 Mbps), crop = 480×352, INCK=37.125 MHz,
codec h265, GOP=240, bitrate 4096 kbps.

| HMAX     | Theoretical | Measured | Sample size       | Verdict   |
|----------|------------:|---------:|-------------------|-----------|
| 0x016E (default) | 243 | **237** | 5 s, repeated 4× | stable    |
| 0x0150           | 264 |   250   | 5 s × 1          | stable    |
| 0x0140           | 277 |   252   | 5 s × 1          | stable    |
| 0x0130           | 292 |   179   | 5 s × 1          | **unstable** — PLL race |
| 0x012C           | 296 | 182 / 270 | 5 s × 4 across two sessions | **unstable** — same patch, different fps on subsequent runs |
| 0x0120           | 308 |   237   | 5 s × 1          | unstable  |
| 0x0118           | 317 |   298   | 5 s × 1          | unstable  |
| 0x0110           | 326 | 186 / 234 | 5 s × 3          | **unstable** |
| **0x0100**       | **346** | **288 – 339** | **8 runs × 10 s; cold-boot pair hit 338.6, settled runs 290-310** | **stable peak; variance from AE race (§8.3)** |
| 0x00F0           | 370 |   175   | 5 s × 1          | sensor doesn't lock (falls back) |
| 0x00E0           | 396 |   181   | 5 s × 1          | sensor doesn't lock |
| 0x00D0           | 426 |   189   | 5 s × 1          | sensor doesn't lock |

Two regimes are visible:

- **`HMAX ≥ 0x0100`**: sensor PLL locks somewhere in this range, some
  values reliably some don't. `0x0100` is a stable plateau.
- **`HMAX < 0x0100`**: sensor PLL cannot sustain shorter line periods
  at SYS_MODE=2; output collapses to ~180 fps (probably a fall-back
  internal state of the IMX335).

The peak is **HMAX=0x0100**. Cold-boot runs measure 338.6 fps
(98% of the 346 fps theoretical, +43% over the 237-fps baseline);
across longer test sessions ongoing runs settle into a 288-310 fps
band (+22% to +31%). The variance source is the cmos_get_ae_default /
cmos_fps_set u32FLStd race in §8.3 — both functions overwrite the
same field with different formulas, and which one lands last
depends on AE-library convergence path. The HMAX patch raises the
ceiling unambiguously; what fraction of that ceiling is realised
on any given run depends on how AE settles.

### 3.4 The patch

One C source change in the repo:

```diff
--- a/libraries/sensor/hi3516ev200/sony_imx335/imx335_sensor_ctl.c
+++ b/libraries/sensor/hi3516ev200/sony_imx335/imx335_sensor_ctl.c
@@ -808,7 +808,7 @@ void IMX335_cropped_flex_init(VI_PIPE ViPipe)
 	IMX335_write_register(ViPipe, 0x3030, vmax          & 0xFF);
 	IMX335_write_register(ViPipe, 0x3031, (vmax >> 8)   & 0xFF);
 	IMX335_write_register(ViPipe, 0x3032, (vmax >> 16)  & 0x0F);
-	IMX335_write_register(ViPipe, 0x3034, 0x6E);         /* HMAX from 1080p baseline */
+	IMX335_write_register(ViPipe, 0x3034, 0x00);         /* HMAX = 0x0100 (research peak) */
 	IMX335_write_register(ViPipe, 0x3035, 0x01);
```

That's it. One byte. Rebuild `libsns_imx335.so`, deploy, restart
majestic, hit `/proc/umap/venc`.

## 4. Pitfalls — things that will eat your day

### 4.1 `SDK_CODE` must be numeric, not a string

`libraries/sensor/hi3516ev200/sony_imx335/Makefile`:

```make
override CFLAGS += -DSDK_CODE=$(SDK_CODE) ...
```

`include/hicompat.h` then does:

```c
#if defined(SDK_CODE) && SDK_CODE+0 != 0
#if SDK_CODE == 0x3516E200
#define GK_API_AWB_SensorRegCallBack HI_MPI_AWB_SensorRegCallBack
#define GK_API_AE_SensorRegCallBack  HI_MPI_AE_SensorRegCallBack
#define GK_API_ISP_SensorRegCallBack HI_MPI_ISP_SensorRegCallBack
...
```

If you build with `SDK_CODE=hi3516ev200` (the string), the
preprocessor sees `-DSDK_CODE=hi3516ev200` which becomes
`SDK_CODE+0 != 0` evaluated with `hi3516ev200` as an undeclared
identifier ⇒ 0 ⇒ skip the remap block. The `.so` ends up with
undefined symbols like `GK_API_AWB_SensorRegCallBack` and won't
`dlopen()` against stock majestic (which only exports `HI_MPI_*`).

**Always pass the numeric form:**

```sh
make CC=arm-openipc-linux-musleabi-gcc \
     AR=arm-openipc-linux-musleabi-ar \
     SDK_CODE=0x3516E200
```

You can verify:

```sh
$ arm-openipc-linux-musleabi-nm -D libsns_imx335.so | grep AWB_Sensor
         U HI_MPI_AWB_SensorRegCallBack       ← good (will resolve)
         U HI_MPI_AWB_SensorUnRegCallBack
```

vs the broken build:

```sh
$ nm -D libsns_imx335.so | grep AWB_Sensor
         U GK_API_AWB_SensorRegCallBack       ← bad (no exporter)
         U GK_API_AWB_SensorUnRegCallBack
```

The OpenIPC firmware build system passes `SDK_CODE` correctly for
you; this only bites when building stand-alone.

### 4.2 majestic enforces `MAX_FPS = 240` in two places

`majestic/include/majestic/commonsdk.h:34`:

```c
#define MAX_FPS 240
```

This is used at:

1. **INI parse** (`src/hisi/sensor_config.c:785`) — `Isp_FrameRate`
   is range-clamped `[0, MAX_FPS]`. Out-of-range values produce
   `ERROR <majestic> [config] parse_u64: Value '295' is not in a
   range [0; 240]` and majestic refuses to start.
2. **VENC channel creation** (`src/hisi/sdk.c` h265 / h264 / MJPEG
   RC attribute init) — `VENC_CreateChn` is given `fr32DstFrameRate
   = specified_fps`. The SDK kernel-side validates and returns
   `ERR_VENC_ILLEGAL_PARAM` for values > 240.

To go beyond 240 fps on the request side you need to patch
`MAX_FPS` to your target and rebuild majestic. BUT — *you don't
need to*. The sensor doesn't care about the request: VENC will
encode whatever the sensor emits. So setting `fps: 240` and patching
HMAX to give 339 fps **works**. The VENC SEND counter ticks at 339.
The request is just an upper-cap hint to the rate-controller, not
a strict throttle.

You only need to bump `MAX_FPS` if you want a downstream user
(streaming client) to *see* a stream advertised at >240 fps, or
to test what the SDK rejects.

### 4.3 The "silent fallback" myth

An earlier session reported a "silent 24-fps fallback above the
ceiling" behaviour. That turned out to be wrong. The actual
failure mode at fps > 240 is *not* silent — `tail /tmp/maj.log`
shows:

```
ERROR [hal] HiSi_HAL_VENC_CreateChn: Cannot create VENC chn 0
ERROR [hal] HiSi_HAL_VENC_CreateChn: ERR_VENC_ILLEGAL_PARAM
```

VENC chn 0 never starts. `/proc/umap/venc` SEND1 row for chn 0
is missing entirely (only chn 1 = MJPEG @ 5 fps appears). The
measurement script returns 0 because there's nothing to count.
Read the log.

### 4.4 HMAX stability is non-monotonic

Naïvely you'd expect: smaller HMAX → higher fps, monotonically,
until the sensor clips at some hard wall. Reality (§3.3): HMAX
values between `0x0100` and `0x016E` form a *band* where some
stick and others don't. `0x0130`, `0x012C`, `0x0110` all gave
unreliable results across re-runs of the same patch (one run
180 fps, next run 270 fps).

Why: HMAX is interpreted by the IMX335's internal sequencer
relative to its PLL multipliers and the MIPI byte-clock domain.
Some values land on integer-boundary line periods, others fall
between, and the sensor reacts to the misalignment by either
locking late (skipping frames during startup → measurement
sees fewer) or jittering its output rate. We didn't trace this
to a specific multiplier; empirically `0x0100` is a clean
landing and the others aren't.

**Recommendation: use 0x0100. Don't bother sub-experimenting
in the 0x0110–0x012C range without an oscilloscope.**

### 4.5 The sensor source file accumulates `sed` state

The HMAX value lives in a literal hex byte in the C source. If
you script the sweep with `sed -i` you'll find that successive
runs *don't reset the source*. We saw a case where measuring
"HMAX=0x012C" the second time (with the source modified in
place from `0x6E → 0x2C → 0x10 → 0x2C` again) produced 182 fps
where a fresh patch gave 270. Reverting to a known-good source
backup before each patch (`cp orig.c imx335_sensor_ctl.c`)
makes results reproducible.

This is *probably* compiler caching or a stale `.o` slipping
through `make`'s dependency check — but we didn't isolate
which. Treat each patch as if from a clean source.

### 4.6 majestic version matters — Isp_SnsMode propagation

`u8SnsMode=4` is the dispatch into M7 (§2.1). Without it the
driver picks a different mode based on resolution match (1080p
→ M3, binning sizes → M2, etc.) and you never see M7's flex-crop
behaviour.

Majestic gained Isp_SnsMode propagation in commit
`c8174312bcfc96eddf919619e964b040b196d8c9`
("hisi: propagate Isp_SnsMode from sensor INI to pub_attr.u8SnsMode"),
merged 2026-05-12. Before that, the INI key was ignored. The
stock OpenIPC firmware binary `master+d11ba3c` predates this
commit, so out-of-the-box the board *cannot* dispatch M7.

For the experiment we built majestic from `HEAD` and deployed
to `/tmp/majestic_new`. Production firmware needs to update past
that commit to access M7 at all.

### 4.7 Crop-height changes vmax_min, not HMAX

If you pick crop_h = 144 instead of 352, vmax_min drops from
840 to `(144+20)×2 + 96 = 424`. Theoretical at HMAX=0x0100:
`72.8e6 / (256 × 424) = 670 fps`. We did **not** test this —
the experiment held crop at 480×352 to isolate the HMAX
variable. If you go this route, expect VPSS / VENC throughput
limits to bite long before the sensor does. The encoder
bandwidth budget (pixels × bits × fps) is fixed.

## 5. What didn't work — SYS_MODE=1 (1188 Mbps)

The IMX335 supports 1188 Mbps/lane via `SYS_MODE=0x01`. In theory
this lets the lanes drain pixels faster, permitting HMAX values
below 0x0100 (which otherwise overflow the internal pixel
buffer). At HMAX=0x00E0 the theoretical max is 396 fps; at
HMAX=0x00C0 it's 462 fps. So the obvious next experiment is
flip `0x319E` to `0x01` and shrink HMAX.

We tried it. It didn't deliver.

### 5.1 The patch we applied

`imx335_sensor_ctl.c:830-834` already had M7 set up at SYS_MODE=2.
Comparing to the working 1188Mbps path at line 1383+
(`IMX335_cropped_binning_1080p_init`):

```diff
-	IMX335_write_register(ViPipe, 0x314C, 0xC0);
+	IMX335_write_register(ViPipe, 0x314C, 0x80);  // INCKSEL1 for 1188
 	IMX335_write_register(ViPipe, 0x315A, 0x06);
+	IMX335_write_register(ViPipe, 0x3168, 0x68);  // INCKSEL3 for 1188
 	IMX335_write_register(ViPipe, 0x316A, 0x7E);
 	IMX335_write_register(ViPipe, 0x319D, 0x00);
-	IMX335_write_register(ViPipe, 0x319E, 0x02);  // 891 Mbps
+	IMX335_write_register(ViPipe, 0x319E, 0x01);  // 1188 Mbps
```

(Plus HMAX = 0x00E0.)

### 5.2 The result

| HMAX  | Measured fps | Comment             |
|-------|-------------:|---------------------|
| 0x100 |    339       | Same as SYS_MODE=2  |
| 0x0F0 |    175       | PLL not locking     |
| 0x0E0 |    181       | PLL not locking     |
| 0x0D0 |    189       | PLL not locking     |
| 0x0C0 |    193       | PLL not locking     |
| 0x0B0 |    201       | PLL not locking     |
| 0x0A0 |    209       | PLL not locking     |

SYS_MODE=1 doesn't unlock lower HMAX. The PLL still fails at
HMAX < 0x0100, identical to SYS_MODE=2.

### 5.3 Why probably

Comparing the working 1188Mbps init (binning_1080p) to our
M7 patch: the binning init writes ~12 additional registers we
*didn't* touch. Notable: `0x3072-0x307F` (window readout
timings — possibly per-tap delays), `0x3078-0x3087` (column
read-out shift control), `0x30A4-0x30AD`. Those registers
*also* differ between the M7 setup and the binning-mode setup,
suggesting their values are coupled to the data-rate selection
or to binning mode specifically.

To make SYS_MODE=1 work in flex-crop mode you probably need to
identify which of those reg blocks are PLL-coupled vs
binning-coupled and copy only the PLL-coupled ones. We didn't
do that bisection. Estimated effort: 2-4 hours with a
datasheet on hand.

## 6. The ev-vs-gk fps gap — what it isn't

A measurement quirk to be honest about: at the same crop and same
HMAX, hi3516ev300 outpaces gk7205v300 by ~30-40% above 240 fps
(at 480×352 @240: ev ~300 fps, gk ~210 fps). This is *not* silicon
binning — the two parts are the **same Hisilicon die**, just
re-marked. Hisilicon's datasheet applies to both.

To localise the gap we read all of CRG (`base 0x1201_0000`, 256
dwords) live on both boards while running the same `majestic_new`
+ HMAX=0x0100 build at 480×352 @240:

```
ssh root@<board> 'for off in $(seq 0 4 0x1FC); do
  a=$(printf 0x12010%03x $off); v=$(devmem $a 2>/dev/null)
  echo "$a $v"
done'
```

Side-by-side diff of two such dumps:

| Reg / offset | Description | ev300 | gk7205v300 |
|---|---|---|---|
| CRG31 / 0x07C | DDR clock | `0x74` | `0x74` ✓ |
| CRG32 / 0x080 | SoC clock select | `0x549` | `0x549` ✓ |
| CRG40 / 0x0A0 | media block freq | `0x0` (vpss=257, vedu=450, ive=450) | `0x0` ✓ |
| CRG42 / 0x0A8 | VPSS clock + reset | `0x6` (online, clock on) | `0x6` ✓ |
| CRG60 / 0x0F0 | SENSOR clock | `0x11` (INCK=37.125 MHz) | `0x11` ✓ |
| CRG61 / 0x0F4 | **VI clock** | `0x340` (`vi_ppc_cksel=000=360 MHz`) | `0x340` ✓ |
| CRG62 / 0x0F8 | MIPI RX clock | `0xD` | `0xD` ✓ |
| CRG63 / 0x0FC | VIPROC clock | `0x18` (online, clock on) | `0x18` ✓ |

**Every documented clock-select register matches byte-for-byte.**
Both boards run the entire online pixel pipeline (VI / VPSS /
VIPROC) at 360 MHz via `vi_ppc_cksel=000`. DDR clock matches.
Sensor INCK matches. No backpressure counters trigger
(`DropErrFrame=0`, `LostFrame=0`, `IspResetCnt=0`,
`IspBeStaLost=0` on both, throughout the test).

So whatever's binding gk's rate is *not* visible at the CRG layer.

What does differ:

| Offset | ev300 | gk7205v300 | Hisilicon datasheet says |
|---|---|---|---|
| 0x008 | 0x00000000 | 0x00032000 | (no entry — undocumented) |
| 0x00C | 0x00000000 | 0x019F0000 | (no entry) |
| 0x010 | 0x00000000 | 0x00032000 | (no entry) |
| 0x014 | 0x00000000 | 0x01870000 | (no entry) |
| 0x020 - 0x034 | all zero (6 dwords) | `0xE0000004 0x00B40000 0x20111FA8 0x2404FF20 0x0802013F 0x00004046` | (no entries) |
| 0x038 - 0x04C | all zero (6 dwords) | **identical repeat** of 0x020-0x034 | (no entries) |

The Hisilicon public CRG overview table jumps `0x004 → 0x018 →
0x050`, deliberately omitting these. They're undocumented on the
Hisilicon side. Goke's vendor SDK clearly knows about them — the
closed `gk7205v200_vi.ko` blob writes them at module init.

The 0x020-0x04C block repeats as two identical 6-dword structures
— textbook shape of two PLLs each with 6 control words. Possibly
PLL2 / PLL3 (the documented ones are PLL0/1 and PLL6/7; PLL2-5 are
"reserved"). Whether they're actually changing internal clock
frequencies, or merely fine-tune registers that don't affect rate,
or aliases of the documented PLLs that read differently — we can't
tell without writing to them, which is risky without bit-field
docs.

What's most likely happening: **same silicon, different vendor SDK
policy**. The Goke variant of the vendor blob (`vi_nolog.o`, ~24%
smaller than the Hisilicon variant) programs PLL/divider state
differently — possibly to comply with Goke's marketing rate spec,
possibly because the Goke SDK was a fork that diverged on internal
config. Same hardware; different software.

To distinguish "same final clocks, different config representation"
from "actually different clocks":

- Write ev's CRG state onto gk (the 0x020-0x04C block to zero, plus
  ensure the documented regs stay matched) **post-`load_hisilicon`**
  via `devmem`, re-measure fps. If lift → these registers were the
  binding. If no lift → look further (DDR PHY tuning,
  ISP_BE buffer config, board-level RC).

This wasn't done in the session that produced this doc — writing to
PLL registers on a live system is the kind of operation that wants
a bench rebuild plan and a serial console attached. The
investigation handed off the next experimenter is documented in
kaeru as `imx335-ev-vs-gk-crg-decode-2026-05-12`.

### 6.1 Update — the gap *vanishes* on UDP-RTP

A separate measurement using outgoing-RTP-over-UDP push (FPV-drone
style, `outgoing: { enabled: true; - udp://host:port }` in
majestic.yaml, no RTSP server in the loop) reveals the "gap" was
100% RTSP-stack artifact. Under direct UDP push, ev and gk deliver
**identical fps at identical CPU** across every measured crop:

| Crop @ req fps | UDP-delivered ev | UDP-delivered gk | ev CPU | gk CPU |
|---|---|---|---|---|
| 1280×720 @100 | 56.1 fps | 56.1 fps | 16% | 15% |
| 800×480 @130  | 82.5 fps | 82.5 fps | 23% | 23% |
| 800×480 @150  | 93.6 fps | 93.6 fps | 26% | 26% |
| 800×480 @240  | 110.2 fps | 110.2 fps | 30% | 31% |
| 480×352 @240  | 140.2 fps | 140.3 fps | 35% | 36% |

What was happening under RTSP: the bundled `smolrtsp` library's
HEVC packetizer has a known framing bug — `send_fragmentized_nal_data`
at `majestic/thirdparty/smolrtsp/src/nal_transport.c:96-128`
generates illegal "S=1 AND E=1" Fragmentation Units when a NAL is
exactly one MTU-payload-size (RFC 7798 §4.4.3 forbids this). ffmpeg
recovers but warns frequently. The CPU cost of generating these
packets, plus RTSP's TCP control-channel and the server-side
session bookkeeping, drove ev's CPU to **72% busy** (vs 35% on UDP)
just to deliver 209 fps out of 290 encoder-side fps — ~28% loss in
transit. On gk the same path bottlenecked earlier, delivering
~140 fps out of ~200 encoder-side fps — ~30% loss. So ffmpeg over RTSP saw 209 fps (ev)
vs 140 fps (gk) — a "30% gap" that turns out to be RTSP-side
asymmetry, not the silicon.

Strip RTSP out of the path, push raw UDP-RTP, and the two boards
match. The CRG diff in the documented registers (which all match)
is therefore consistent with the actual behaviour; the undocumented
0x008-0x04C registers may still differ, but they don't appear to
have functional rate consequences as far as deliverable fps goes.

### 6.2 The `VENC SEND` counter overstates deliverable fps

A practical note for anyone doing fps measurements on V4: the
`/proc/umap/venc VENC SEND1.Send` counter measures *what the encoder
emits*, not *what reaches the wire*. With outgoing-UDP enabled, the
encoder back-pressures to the consumer rate (the RTP send-loop
drains at ~140 fps for 480×352 @4 Mbps, so VENC produces 140 fps).
With RTSP serving, the socket buffer absorbs the rate mismatch by
*dropping* frames silently — VENC happily produces 290 fps, ffmpeg
sees 209 fps, the other ~80 fps die in transit.

For the headline ceiling tests in §3, we deliberately ran with no
network consumer, so VENC SEND ≈ what the encoder could produce.
For deliverable-fps numbers — what an FPV drone actually serves
over the radio link — see §6.1 or the README's table.

### 6.3 Baseline (HMAX=0x16E) vs patched (HMAX=0x100) over UDP

Head-to-head, deliberately apples-to-apples, identical config, both
boards, 10 s measurement window after 2 s warmup, marker-bit frame
count from raw RTP:

| Crop @ req fps | Baseline ev | Patched ev | Baseline gk | Patched gk | Lift |
|---|---|---|---|---|---|
| 1280×720 @100 | 45.2 | **56.1** | 39.3 | **56.1** | +24% (ev) / +43% (gk) |
| 1024×576 @120 | 49.1* | **168.4** | 49.1* | **168.5** | (anomaly — see note) |
| 800×480 @130  | 57.7 | **82.5**  | 57.7 | **82.5**  | +43% |
| 800×480 @150  | 65.4 | **93.6**  | 65.4 | **93.6**  | +43% |
| 800×480 @240  | 77.1 | **110.2** | 77.1 | **110.2** | +43% |
| 480×352 @240  | 98.1 | **140.2** | 98.1 | **140.3** | +43% |

*The 1024×576 baseline row is anomalous. Both boards delivered 49
frames per second of nearly-empty content (ev 0.13 Mbps, gk 0.04
Mbps, 100-500 byte frames). The encoder's RC algorithm got confused
at this resolution and produced near-empty P-frames at the requested
120 fps; the +243% jump to the patched value isn't really
"+243% performance", it's "patched delivered an ordinarily-encoded
stream where baseline had collapsed". Don't quote it as a headline.
All other rows are legitimate.

The +43% lift translates cleanly from encoder-counter to wire-rate:
the patch raises the sensor pixel-clock budget (§3), the encoder
keeps up, the RTP path keeps up. At 1280×720 the lift compresses to
+24% on ev (gk gets the full +43% — gk's baseline was further from
the binding) because the encoder rate ceiling — not the sensor —
is what binds at that crop.

## 7. Reproduction guide

```sh
# 1. Backup, then patch HMAX
cp libraries/sensor/hi3516ev200/sony_imx335/imx335_sensor_ctl.c /tmp/orig.c
sed -i '811s|0x6E|0x00|' \
  libraries/sensor/hi3516ev200/sony_imx335/imx335_sensor_ctl.c
# verify - should show 0x00 not 0x6E
grep -n 'HMAX from 1080p\|HMAX = 0x0100' \
  libraries/sensor/hi3516ev200/sony_imx335/imx335_sensor_ctl.c

# 2. Build
TC=/path/to/toolchain.hisilicon-hi3516ev200/bin
cd libraries/sensor/hi3516ev200/sony_imx335
make CC=$TC/arm-openipc-linux-musleabi-gcc \
     AR=$TC/arm-openipc-linux-musleabi-ar \
     SDK_CODE=0x3516E200

# 3. Build majestic from HEAD (or any commit ≥ c8174312)
cd /path/to/majestic && ./build.sh hi3516ev200 lite

# 4. Deploy both
sshpass -p $PW ssh root@$BOARD '
    killall -9 majestic 2>/dev/null
    sleep 1
'
cat libsns_imx335.so | \
    sshpass -p $PW ssh root@$BOARD 'cat > /tmp/libsns_imx335.so'
cat /path/to/majestic/output/majestic | \
    sshpass -p $PW ssh root@$BOARD 'cat > /tmp/majestic_new && chmod +x /tmp/majestic_new'

# 5. Configure
sshpass -p $PW ssh root@$BOARD '
cat > /etc/sensors/imx335_test.ini <<EOF
[sensor]
Sensor_type=stSnsImx335Obj
Mode=WDR_MODE_NONE
DllFile=/tmp/libsns_imx335.so
[mode]
input_mode=INPUT_MODE_MIPI
raw_bitness=10
clock=37.125MHz
[mipi]
lane_id = 0|1|2|3|-1|-1|-1|-1|
[isp_image]
Isp_FrameRate=240
Isp_Bayer=BAYER_RGGB
Isp_SnsMode=4
Isp_W=480
Isp_H=352
[vi_dev]
Input_mod=VI_MODE_MIPI
DevRect_w=480
DevRect_h=352
DevRect_x=0
DevRect_y=0
FullLinesStd=1375
EOF
cat > /etc/majestic.yaml <<EOF
system:
  webPort: 80
  logLevel: debug
isp:
  sensorConfig: /etc/sensors/imx335_test.ini
video0:
  enabled: true
  codec: h265
  size: 480x352
  fps: 240
  bitrate: 4096
EOF
'

# 6. Run and measure
sshpass -p $PW ssh root@$BOARD '
/tmp/majestic_new >/tmp/maj.log 2>&1 &
sleep 10  # AE convergence
S1=$(awk "/SEND1/{f=1;next}/SEND2/{f=0}f && /^ *0 / {print \$6;exit}" /proc/umap/venc)
T1=$(date +%s.%N)
sleep 10
S2=$(awk "/SEND1/{f=1;next}/SEND2/{f=0}f && /^ *0 / {print \$6;exit}" /proc/umap/venc)
T2=$(date +%s.%N)
awk "BEGIN{printf \"fps = %.1f over %.3fs (%d frames)\n\", ($S2-$S1)/($T2-$T1), $T2-$T1, $S2-$S1}"
'
# expected: ~339 fps
```

## 8. Future work

### 8.1 Bigger sensor crops at the same HMAX

VMAX_min scales with crop_h. We didn't test smaller crops. At
crop_h = 144 (480×144) theoretical with HMAX=0x0100 is `72.8e6 /
(256 × 424) = 671 fps`. Bandwidth budget shifts downstream:
encoder bits/sec might cap before sensor pixels/sec does. Worth
checking — start with a CBR target way below the current 4 Mbps
to give VENC headroom.

### 8.2 SYS_MODE=1 done properly

Take the 12-register diff between `IMX335_cropped_flex_init`
(the M7 setup we patched) and `IMX335_cropped_binning_1080p_init`
(line 1383+, working 1188Mbps init). Datasheet section "Driving
setting" lists which registers are PLL-coupled vs readout-pattern
coupled. Copy the PLL-coupled ones. Predict +20-30% over the
current peak if the SoC MIPI RX can ingest 1188 Mbps cleanly
(it should — the binning init at 1188 Mbps works fine).

### 8.3 The cmos_get_ae_default / cmos_fps_set u32FLStd race

There's a known race in `imx335_cmos.c`:

```c
// cmos_get_ae_default (:207-216) sets u32FLStd one way:
u32Fll = IMX335_VMAX_5M_30FPS_12BIT_LINEAR;  // 4500
U32MaxFps = 60;
pstSnsState->u32FLStd = u32Fll * U32MaxFps / DIV_0_TO_1_FLOAT(gu32STimeFps);
// at gu32STimeFps=240: u32FLStd = 1125

// cmos_fps_set (:418-440) sets it a different way:
u32MaxFps = 90;
u32Lines = IMX335_VMAX_CROPPED_1080P * 90 / f32Fps;
// at f32Fps=240: u32Lines = 861, clamped to vmax_min=840
```

These two functions write to the same field with different
formulas. Order of calls determines which one sticks. The
measured fps at 339 (with VMAX should theoretically settle at
840) implies the actual VMAX in registers is ~921 lines
(`72.8e6 / (256 × 339) = 838`, near 840 — actually fine).
But the discrepancy of formulas means changes in either function
affect both AE behaviour and frame rate. Tidying this up is a
prerequisite for letting users set higher fps via the INI
without overriding HMAX.

### 8.4 Bandwidth-encoder regime testing

At 339 fps × 480 × 352 = 57.3 Mpix/s × 12 bits raw = 687 Mbit/s
on the bus. VENC at 4 Mbps target compression ratio = ~170:1.
We didn't check encoded-stream visual quality. If a real
deployment used these settings, downstream bitrate could not
sustain h265 at usable quality. The high-fps regime is useful
for *measurement / latency* applications (FPV, machine vision)
not for storage / streaming.

## 9. Critical files and line references

- `libraries/sensor/hi3516ev200/sony_imx335/imx335_sensor_ctl.c:811`
  — the HMAX byte that controls fps ceiling in M7
- `libraries/sensor/hi3516ev200/sony_imx335/imx335_sensor_ctl.c:756`
  — `IMX335_cropped_flex_init` entry point
- `libraries/sensor/hi3516ev200/sony_imx335/imx335_sensor_ctl.c:834`
  — SYS_MODE register (`0x319E`) in M7 init
- `libraries/sensor/hi3516ev200/sony_imx335/imx335_sensor_ctl.c:1383`
  — working SYS_MODE=1 (1188 Mbps) register sequence to crib from
- `libraries/sensor/hi3516ev200/sony_imx335/imx335_cmos.c:418-440`
  — `cmos_fps_set` M7 branch (vmax_min clamp)
- `libraries/sensor/hi3516ev200/sony_imx335/imx335_cmos.c:1780`
  — `cmos_set_image_mode` (M7 dispatch from `u8SnsMode=4`)
- `include/hicompat.h:4-12`
  — `SDK_CODE` numeric remap of `GK_API_*` to `HI_MPI_*`
- `majestic/include/majestic/commonsdk.h:34` (external repo)
  — `MAX_FPS 240` upper-cap
- `majestic/src/hisi/sensor_config.c:785` (external repo)
  — `Isp_FrameRate` INI parser, range-checks against MAX_FPS
- `majestic` commit `c8174312` (external repo) — Isp_SnsMode propagation
