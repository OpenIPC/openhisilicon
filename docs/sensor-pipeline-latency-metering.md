# Measuring glass-to-glass latency on V4 SoCs

A research handoff doc. The work below is *not* done — this is a
methodology spec the next person should pick up.

## Why this exists

Most fps numbers in this repo (and in PR #99's README, `docs/imx335-v4-high-fps.md`,
the various sensor INI presets) measure **frame rate** — frames per second
crossing some counter. That's the right metric for some workloads (video
recording, ML batch inference).

For FPV, drones, control loops, and machine vision, the metric that actually
matters is **glass-to-glass latency** — the wall-clock delay from a photon
landing on the sensor to the corresponding pixel becoming visible at the
receiver. Two presets with identical wire fps can have very different
latencies, and our flagship high-fps work has at least one case where the
*latency* benefit is the only real benefit:

**IMX335 boost-1944p** (sensor 2592×1944 at ~40 fps internal) caps wire-rate
at ~21 fps for full 4:3 h.264 4 Mbps output — the same wire fps as stock 5M
at 30 fps. The encoder pixel-rate budget at full 5M is the binding. So why
ship boost as a preset? Because the sensor side IS faster:

| Mode | HMAX | VMAX | Sensor readout / frame |
|------|------|------|------------------------|
| Stock 5M @30 fps | 0x0226 = 550 | 4500 lines | 550 × 4500 / 74.6 MHz ≈ **33 ms** |
| Boost-1944p | 0x01A0 = 416 | 4500 lines | 416 × 4500 / 74.6 MHz ≈ **25 ms** |

That's 8 ms shorter sensor readout per frame. Plus the encoder picks from
a 40 fps stream instead of a 30 fps stream — so the frame it actually
encodes is on average ~4 ms fresher (half the inter-frame interval). Sum:
**~16 ms shorter glass-to-glass latency for boost-1944p vs stock 5M, at
identical wire fps**. That's a meaningful FPV win.

The number above is analytical. **Nobody has measured it.** This doc
proposes how.

## What "latency" means in this pipeline

```
photon
  │
  ├─ sensor integration (exposure time, programmable)
  │     IMX335 SHR0 register controls; usually 0.05-30 ms in daylight
  │
  ├─ sensor readout (HMAX × VMAX / pixel_clock)
  │     IMX335 boost: 25 ms, stock 5M: 33 ms, M7 flex 480×352: 4 ms
  │
  ├─ MIPI CSI-2 transport (data lanes; usually ≤ readout time)
  │
  ├─ VI capture interrupt (one per frame, fires at frame-end)
  │
  ├─ ISP processing (3DNR, AE, AWB, gamma, sharpening, demosaic)
  │     ~3-7 ms on V4 at typical resolutions; longer in WDR / hi-res
  │     /proc/umap/isp IntRat = ISP completion-IRQ rate
  │
  ├─ VPSS scaling / format convert
  │     ~1-3 ms; downscale is faster than upscale
  │
  ├─ VENC encode (h264 / h265)
  │     ~5-25 ms depending on rc-mode, gop, profile, pixel count, bitrate
  │     /proc/umap/venc SEND counter = frame-emit rate
  │
  ├─ majestic RTP packetizer (smolrtsp NAL fragmentation)
  │     ~0.5-2 ms; one libevent callback per frame
  │
  ├─ kernel send (UDP socket → eth0 qdisc → MAC)
  │     ~0.1-0.5 ms on 100 Mbit LAN
  │
  ├─ wire (LAN ≤ 1 ms; radio link 5-50 ms typical)
  │
  ├─ receiver kernel UDP recv
  │
  ├─ decoder buffering (ffmpeg / ffplay; can be milliseconds or hundreds)
  │
  ├─ receiver display (scan-out + pixel response)
  │     OLED phone @120Hz: ≤9 ms; LCD monitor @60Hz: ~17 ms + LCD
  │     response (5-15 ms more)
  │
display lights up
```

Different "latency" can mean any of these:
- **End-to-end (glass-to-glass)**: photon → display lit
- **Camera-side (capture-to-wire)**: photon → last RTP packet leaves NIC
- **Encoder-side (capture-to-VENC-emit)**: photon → /proc/umap/venc Send ticks
- **Sensor-only**: photon → end of sensor readout (the 33/25 ms above)

Different methods measure different subsets. Match the method to the question.

## Method 1 — Phone screen + slow-mo capture (FPV-reviewer standard)

The community-favourite. Works for end-to-end glass-to-glass.

**Setup:**
1. **Source**: a phone in browser showing a millisecond timer
   (`performance.now()`-driven). OLED preferred (5 ms response vs LCD's
   15 ms). 120 Hz screen quantises to 8 ms per tick.
2. **UUT**: camera under test, pointed at the phone screen, ~30 cm away,
   timer large enough to read in the receiver's frame.
3. **Receiver**: `ffplay -fflags nobuffer -flags low_delay -framedrop -i recv.sdp`
   on a fast laptop with low-latency display. Show ffplay's window next
   to the source phone.
4. **Capture**: a third device (another phone in slow-mo at 240 fps or
   480 fps if available) films both the source phone and the ffplay
   window in a single frame.

**Read:**
- Pause the slow-mo, find a frame where both timers are legible
- `latency = source_timer_value − ffplay_timer_value`
- Repeat ≥ 10 times; report median + IQR

**Pros:** zero hardware cost (phones), well-understood, easy to repeat.
**Cons:**
- 240 fps slow-mo resolves ±4 ms; 480 fps resolves ±2 ms; iPhone Pro
  models hit 240 reliably
- Source phone screen scan-out adds 4–8 ms ahead of the "displayed" timer
  value, biasing low; same on receiver side
- Wildly hard to reproduce across labs unless source/receiver hardware is
  pinned (different phone OLEDs, different ffplay decoders, different
  monitors all add noise)

**Reproducibility verdict: low.** Useful for tuning your own setup,
useless for "lab A vs lab B numbers".

## Method 2 — LED trigger + RTP timestamp (mid-rigour, ~1 ms)

Use a GPIO-driven LED placed in the camera's FoV as the photon source.
Trigger from a script on the camera itself, log the trigger time and the
RTP arrival time at the receiver.

**Setup:**
1. Solder an LED to a free GPIO on the camera dev board (the V4 OpenIPC
   boards expose several; pick one that isn't in use by the existing
   peripheral set — `/sys/class/gpio/` lookups).
2. Hardware-trigger script on the camera:
   ```sh
   echo "$(date +%s.%N) trigger" >> /tmp/trigger.log
   echo 1 > /sys/class/gpio/gpioN/value
   sleep 0.01
   echo 0 > /sys/class/gpio/gpioN/value
   ```
3. Receiver: pcap capture of the RTP stream — `tcpdump -i eth0 -w /tmp/cap.pcap udp port 5600`.
4. Both camera and receiver NTP-synced (or PTP for higher precision).

**Read:**
- Trigger the LED N times with random delays
- For each trigger, find the first RTP packet that contains a frame with
  the LED lit (decode + visual or run an ML detector over decoded frames)
- `latency = rtp_arrival_time − trigger_time`
- Subtract one-way network latency (`ping` floor, usually ~0.5 ms LAN)

**Pros:**
- Excludes phone-screen / monitor scan-out noise from both ends
- Reproducible across labs if the GPIO LED hookup is standardised
- ±1 ms accuracy with NTP sync; ±10 µs with PTP
- Programmatic — easy to run hundreds of trials and get statistics

**Cons:**
- Requires soldering / probe access on the camera
- Needs decoder-side LED detection (manual frame-by-frame OR a small CV
  script with an HSV threshold)
- Doesn't include receiver display latency — so it's "camera + network"
  not "glass-to-glass"

**Reproducibility verdict: high** for camera-side latency. Misses display.

## Method 3 — Bench rig: photodiode → oscilloscope (gold standard)

True glass-to-glass with photon-level precision.

**Setup:**
1. Fast photodiode + transimpedance amp #1 placed at the LED source.
2. Fast photodiode + transimpedance amp #2 placed against the receiver's
   display (where the LED would appear on the decoded image).
3. Both photodiodes feed a 2-channel scope, edge-trigger on channel 1.
4. Pulse the source LED from a function generator (or GPIO).
5. Read the time-delta between channel 1 rising edge and channel 2
   rising edge on the scope.

**Pros:**
- Sub-millisecond accuracy, often <100 µs
- Captures everything: sensor + ISP + VENC + RTP + network + decoder + display
- Easy to repeat — push the trigger N times, average

**Cons:**
- Hardware: photodiodes ($10), transimpedance amps ($30 each or built),
  2-channel scope ($300+ used, $1k+ new), function generator (optional,
  $100)
- Setup is meticulous — alignment of photodiodes against fast-pixel parts
  of the display matters
- Display response curve (OLED < 1 ms; LCD 5–15 ms) adds to but doesn't
  invalidate the measurement

**Reproducibility verdict: high.** Cross-lab comparisons work if both
labs use OLED receivers and report which they used.

## Recommended methodology for this repo

For "is boost-1944p actually lower latency than stock 5M" — the question
this doc started with — **Method 2 is the right level**. It excludes
display variability (which differs by lab), captures the camera-side
pipeline accurately, and isolates the contribution of the sensor +
encoder mode change.

The expected delta is ~16 ms. Method 2 resolves to ±1 ms — comfortably
inside the signal.

### Test protocol for someone picking this up

1. Pick a free GPIO on hi3516ev300 / gk7205v300 dev boards. Document
   pin number + how to enable.
2. Build the trigger script (above).
3. Receiver side: write a small script that ingests the RTP stream,
   decodes to YUV frames, detects the LED-on transition (e.g. mean
   brightness in a 16×16 box around expected LED position crossing
   a threshold), reports the RTP arrival timestamp of the first
   "LED-on" frame.
4. Run 100 trials per mode (stock 5M, boost-1944p, plus optionally the
   M7 flex-crop modes). Report median, p95, IQR.
5. Compare. The boost vs stock 5M delta should be 16 ± 2 ms if the
   analytical model is right. Save to kaeru as
   `v4-glass-to-glass-latency-2026-XX-XX` with the per-mode numbers.

### Standardization checklist (so labs can compare)

Any lab reporting latency numbers from this rig should pin:
- [ ] Sensor model + sensor INI filename
- [ ] Sensor INCK frequency (37.125 MHz on V4 with correct INI)
- [ ] majestic version (commit or release date)
- [ ] openhisilicon version (commit or release date)
- [ ] Encoder codec (h.264 / h.265)
- [ ] Encoder rcMode + bitrate target
- [ ] video0.size (encoder output)
- [ ] Network: localhost, LAN, radio
- [ ] Time sync: NTP / PTP / GPS
- [ ] (Method 1 only) source phone model + receiver display model
- [ ] (Method 3 only) photodiode response curves + display response

## Open questions for the next person

- Does the libevent serialization (single-thread RTP send loop in
  `majestic/src/hisi/sdk.c:1011-1057`) add measurable jitter at high fps?
  Method 2 should distribute trigger times uniformly across frame phase
  and see if p95 latency >> median latency.
- Does `isp.lowDelay: true` in majestic.yaml shave latency? Worth a
  before/after pass at the same fps.
- For the M7 flex-crop modes (480×352 @240, 800×480 @240) where wire fps
  is high, does latency scale inversely with fps? Or is encoder phase
  dominant?
- Does `rcMode=cbr` vs `vbr` change latency variance?
- WDR mode adds frame-buffering (stitching long+short exposures). How
  much per IMX335 WDR_MODE_2To1_LINE?

## Provenance

Captured 2026-05-13 during firmware PR #2090 follow-up. The boost-1944p
preset (`imx335_2592x1944_45fps.ini`) was almost dropped from the
high-fps directory because its wire fps doesn't beat stock 5M. Realised
the actual benefit is latency, not fps; this doc spells out how to
verify that claim. The INI's header now links here for the next person
who wants to actually run the numbers.
