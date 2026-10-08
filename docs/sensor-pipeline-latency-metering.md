# Measuring glass-to-glass latency on V4 SoCs

How to measure where the delay goes between light reaching the sensor and the
picture on a screen, what has been measured on gk7205v300 + IMX335, and what
to change when latency matters more than resolution.

The camera-side and browser-side measurements below were made with
**[OpenIPC/latency-probe](https://github.com/OpenIPC/latency-probe)**. It needs
no extra hardware: ssh access to the camera and Docker on the host.

## Why this exists

Most fps numbers in this repo (`docs/high-framerate-modes.md`, the sensor INI
presets) measure **frame rate**: frames per second crossing a counter. That is
the right metric for recording and batch inference.

For FPV, drones, control loops, video calls and machine vision, the metric that
matters is **glass-to-glass latency**: the wall-clock delay from a change of
light at the lens to the change being visible at the receiver. Two presets with
the same wire fps can have very different latencies, and a single receiver
setting can add more delay than the whole camera.

## What "latency" means in this pipeline

```
light changes
  │
  ├─ sensor integration (exposure time, programmable)
  │     IMX335 SHR0 register; 0.05-30 ms in daylight. Darkness forces it long.
  │
  ├─ sensor readout (HMAX × VMAX / pixel clock), rolling shutter
  │     IMX335 stock 5M: 33 ms, 1080p crop: ~11 ms, flex 800×480: ~4 ms
  │
  ├─ MIPI CSI-2 transport (overlaps readout)
  │
  ├─ VI capture → ISP → VPSS
  │     online (line-by-line) or offline (whole frame through DDR between
  │     stages). majestic logs which: "SDK is in 'VI_ONLINE_VPSS_ONLINE' mode"
  │     gk7205v300 + IMX335: online at 1080p, VI offline at 2592×1944
  │
  ├─ VENC encode (H.264 / H.265)
  │     starts after the whole frame unless isp.lowDelay lets VPSS hand it
  │     over by lines. Full-frame encode is the bulk of capture→wire:
  │     ~25 ms at 1080p, ~9 ms at 720p, ~60 ms at 5M on gk7205v300
  │
  ├─ majestic packetiser: RTP (RTSP, WebRTC) or fMP4 (/ws/video, MSE)
  │     whole access units; slices do not leave early (measured)
  │
  ├─ kernel send, wire (LAN ≤ 1 ms; radio 5-50 ms)
  │
  ├─ receiver: jitter buffer / MSE buffer, decode
  │     WebRTC ~17 ms; WebUI MSE player 60+ ms (see below)
  │
  └─ display: refresh wait + panel response
        60 Hz LCD: ~8 ms average wait + 5-15 ms response; 120 Hz OLED: ≤ 9 ms
```

Different "latency" can mean different subsets of this:
- **End-to-end (glass-to-glass)**: light → display lit
- **Camera-side (glass-to-wire)**: light → last packet of the frame leaves majestic
- **Majestic-side (capture stamp → wire)**: majestic's own capture timestamp
  (the `prft` box on every `/ws/video` fragment) → packet out
- **Receiver-side**: packet in → frame presented

Match the method to the question.

## Method 1: phone screen + slow-motion capture

The FPV-reviewer standard. End-to-end, glass-to-glass.

1. **Source**: a phone showing a millisecond timer (`performance.now()`).
   OLED preferred (~5 ms response against LCD's ~15 ms).
2. **Camera**: pointed at the phone, ~30 cm away, timer legible in the frame.
3. **Receiver**: the viewer you actually use (browser WebUI, VLC,
   `ffplay -fflags nobuffer -flags low_delay -framedrop rtsp://…`) on a screen
   next to the source phone.
4. **Capture**: a third device in slow motion (240 fps or more) films both
   screens in one shot.

Read `latency = source_timer − displayed_timer` on ≥ 10 paused frames; report
median and spread.

**Pros:** no special hardware, includes everything the viewer sees, including
the display. It is the only method here that covers the receiver you actually
use. **Cons:** ±2-4 ms quantisation at 240-480 fps, screen scan-out bias at
both ends, poor reproducibility across labs.

Use it to answer "how much latency do *I* see". Use the methods below to find
out which stage is responsible.

## Method 2: register step on the camera (latency-probe)

Instead of an LED in front of the lens, change the light *inside* the sensor:
a small probe running on the camera steps the sensor's analog gain over I2C
and timestamps, on the camera's own clock, when the brightened frame arrives
at a client on the same camera (loopback RTSP and `/ws/video`). The saved
stream is decoded on the host to find the first changed frame. Nothing needs
soldering or clock sync, and the network to the camera is excluded, so a camera
across a VPN measures the same as one on the bench.

```sh
git clone https://github.com/OpenIPC/latency-probe && cd latency-probe
make image docker                    # docker image + static ARM probe
./run.sh root@CAMERA baseline -d 45   # ~40 steps, prints the split
```

**What it captures:** sensor register latch → readout → VI/ISP/VPSS → VENC →
majestic packetiser, and majestic's capture stamp on the way. **What it does
not:** exposure (gain is applied after integration) and the receiver.

**Caveat:** a gain write takes effect on a frame boundary up to two frames
later (`u8Cfg2ValidDelayMax = 2` in `sony_imx335/imx335_cmos.c`), while a real
change of light shows up in the frame being exposed. The absolute figure
therefore overstates glass-to-wire by about one frame period. Differences
between two settings measured this way are exact, and that is the job it is
built for.

**Reproducibility: high.** Randomised step phase, ~20 steps per direction,
same clock for stimulus and arrival.

The same repository times the **receiver half** in Chromium with the camera's
own WebUI player (`host/browser.py --mode mse|mse-chase|webrtc`), from frame
arrival to `requestVideoFrameCallback` `expectedDisplayTime`.

### Method 2a: LED + receiver pcap

The original plan for this doc, still valid when the stimulus must be real
light: a GPIO-driven LED in the field of view, its trigger time logged on the
camera, RTP arrival captured on an NTP/PTP-synced receiver, LED-on frame found
by decoding. It includes exposure, which Method 2 does not. The price is
soldering, clock sync, and a lit LED in the scene.

## Method 3: photodiode → oscilloscope (gold standard)

Photodiode #1 at the light source, photodiode #2 on the receiver's screen where
the source appears, both into a 2-channel scope, trigger on #1, read the delta.
Sub-millisecond, includes everything, needs photodiodes, transimpedance
amplifiers and a scope. Cross-lab comparable if the display type is reported.

## Measured: gk7205v300 + IMX335

2026-10-08, openhisilicon `77992b6`, majestic `master+b767dd9`, firmware
nightly `988f385`. H.264, 4096 kbit, `profile: base`, `aeMode: manual`,
exposure 8 ms. Camera side is the p50 of ~21 brightening steps (Method 2).

| Sensor INI / stream | Setting | gain write → frame out of majestic | capture stamp → out |
|---|---|---|---|
| `high-fps/imx335_1920x1080_55fps.ini`, 1080p55 | default | 94.9 ms | 27.1 ms |
| same | `isp.lowDelay: true` | 77.6 ms | 17.5 ms |
| same | lowDelay + `video0.sliceUnits: 17` | 80.6 ms | 17.9 ms |
| `high-fps/imx335_1280x720_120fps.ini`, 720p120 | lowDelay | 48.0 ms | 15.7 ms |
| same sensor mode, `video0.fps: 60` | lowDelay | 63.0 ms | 11.1 ms |
| `5M_imx335.ini`, 2592×1944 (encoder caps at 26 fps) | default | 230 ms | 60.5 ms |

Receiver side, the same camera at 1080p55, headless Chromium (no display lag):

| Player | frame arrival → expected display, p50 |
|---|---|
| WebUI MSE (`/ws/video`) | 62.5 ms, buffer 60-82 ms ahead of the playhead |
| same player with `playbackRate` 1.25 while > 30 ms buffered | 19 ms |
| WebUI WebRTC | 16.8 ms, jitter buffer ~8 ms |

At 5M (~15 fps delivered) the gap grows: MSE 108 ms, WebRTC 26 ms.

## Answers to the questions this doc used to leave open

- **Is boost-1944p faster than stock 5M?** On gk7205v300 it cannot be
  tested through majestic today. The encoder caps 2592×1944 at 26 fps,
  majestic drives the ISP at the encoder rate ("ISP driven at 30 fps to meet
  the configured streams; the sensor profile asks for 45"), and the driver
  then picks the stock 30 fps mode. The sensor-side saving the analysis
  predicted (~8 ms readout plus a fresher frame) is real on paper, but majestic
  never runs that mode. At 5M the pipeline is also
  `VI_OFFLINE_VPSS_ONLINE`, and VI drops frames for want of buffers ("VI is
  losing 6.5 of the sensor's 30 frames a second"). Those dwarf the readout
  difference.
- **Does `isp.lowDelay: true` help?** Yes, about one frame at 1080p55
  (−17 ms), because VENC starts before the frame is complete. majestic switches
  it off while `motionDetect` is enabled ("motion detection reads an extended
  channel, which a low-delay channel does not feed"), and it rules out
  `storageSaver` frame dropping.
- **Do slices (`sliceUnits` / `sliceBytes`) help?** No. majestic sends whole
  access units, so nothing leaves before the frame is done.
- **Does latency scale with fps in the small modes?** Roughly with the frame
  period: 720p120 halves 1080p55. The encoder must run at the sensor rate,
  though: the same 120 fps mode encoded at 60 fps lost 15 ms.
- **Open still:** rcMode cbr/vbr effect on variance, WDR 2-to-1 buffering,
  and whether exposure-inclusive (LED) numbers match the register-step
  numbers minus one frame.

## Getting latency down

In order of payoff on the hardware above:

1. **Use WebRTC, not MSE, in the browser.** It's the WebUI's default
   transport. The MSE player keeps whatever was buffered when playback began
   and only seeks to the live edge once that excess passes a second: −45 ms
   at 1080p55. Outside the browser, use a player with buffering off
   (`ffplay -fflags nobuffer -flags low_delay -framedrop`).
2. **Trade resolution for frame rate.** Every per-frame stage scales with the
   frame period. Pick a smaller sensor mode from
   [high-framerate-modes.md](high-framerate-modes.md) and set `video0.fps` to
   the sensor rate.
3. **`isp.lowDelay: true`**, with motion detection off.
4. **Shorten the exposure.** Integration is in the path, and a dark scene
   forces it towards the frame period. A faster lens (lower f-number) and
   more light let auto-exposure pick a short shutter.
5. **Measure your own chain.** Method 1 with your actual receiver and screen,
   and latency-probe to see which half to work on. Numbers measured on
   someone else's setup don't transfer to yours.
6. **Pick the hardware for the job.** gk7205v300 / hi3516ev300 is a
   single-core SoC whose encoder tops out at 5M@26 fps. A 5 MP sensor at full
   resolution is the slowest configuration it has (230 ms camera side above).
   If minimum latency is the requirement, start from a faster SoC and a sensor
   whose native mode matches the output resolution.

## Standardisation checklist

Report alongside any latency number:
- [ ] SoC, sensor model, sensor INI filename
- [ ] majestic, firmware and openhisilicon versions
- [ ] VI/VPSS mode from majestic's log, `isp.lowDelay`
- [ ] codec, rcMode, bitrate, `video0.size`, `video0.fps`
- [ ] exposure (manual value, or the AE result under the test light)
- [ ] method; for Method 2 the register stepped and the step size
- [ ] receiver: player and transport (WebRTC / MSE / RTSP client), display
- [ ] network: loopback, LAN, radio, VPN

## Provenance

Started 2026-05-13 during firmware PR #2090 as a methodology spec, when the
boost-1944p preset was kept for a latency benefit nobody had measured.
Measurements added 2026-10-08 for
[OpenIPC/firmware#2551](https://github.com/OpenIPC/firmware/issues/2551)
(gk7205v300 + IMX335, 120 ms seen through the WebUI's MSE player), using a
lab board and the harness published as OpenIPC/latency-probe.
