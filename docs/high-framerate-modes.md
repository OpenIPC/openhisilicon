# High-framerate sensor modes

Sensor modes that run faster than the vendor driver's, how each is selected
from the sensor INI, and the frame rates measured on real boards. Every
figure is the encoded rate VENC delivers, read from the `Send` counter in
`/proc/umap/venc`.

## IMX335 high-framerate modes (hi3516ev200 only)

Encoded fps delivered by VENC (`/proc/umap/venc` `VENC SEND1` `Send`
counter, delta over 8 s after a 6 s warm-up), measured side-by-side on
`openipc-hi3516ev300` and `openipc-gk7205v300` with identical sensor INI
and majestic config (4 Mbps, `video0.size = 1920x1080` for high-res
4:3 / 16:9 sensor modes — the typical IP-cam streaming target — and
sensor-crop-native for smaller modes; VPSS handles the downscale and
center-crops 4:3 sensors when streaming 16:9). H.264 and H.265 produce
identical fps in every mode at this bitrate (verified codec-by-codec on
both boards); the encoder is not the bottleneck.

| Mode | Sensor crop | hi3516ev300 | gk7205v300 | Selected by |
|------|-------------|------------|------------|-------------|
| Stock full-scale | 2592×1944 | 30 fps | 30 fps | `DevRect_w=2592 DevRect_h=1944` (default) |
| Cropped 16:9 | 2592×1520 | 49 fps | 45 fps | `DevRect_w=2592 DevRect_h=1520` |
| Binning | 1296×972 | 64 fps | 64 fps | `DevRect_w=1296 DevRect_h=972` |
| Cropped 1.5x zoom | 1920×1080 | 55 fps | 55 fps | `DevRect_w=1920 DevRect_h=1080` |
| Boost-1944p | 2592×1944 | 39 fps (`Isp_FrameRate=45`) | 31 fps (`Isp_FrameRate=36`) | `Isp_SnsMode=6` |
| Flexible crop | arbitrary W×H | up to **147 fps** at 800×480 | up to **147 fps** at 800×480 | `Isp_SnsMode=4` + `Isp_W=...` + `Isp_H=...` |

Flexible-crop ceiling rises as crop shrinks; per-size points measured:

| Flex crop W×H | hi3516ev300 | gk7205v300 |
|---|---|---|
| 1280×720 @ 100 fps | 98 fps | 98 fps |
| 1024×576 @ 120 fps | 118 fps | 118 fps |
| 800×480 @ 130 fps | 128 fps | 128 fps |
| 800×480 @ 150 fps | 147 fps | 147 fps |

Set `Isp_FrameRate` in the sensor INI to request a target rate; the driver
clamps to the per-mode sensor ceiling. The per-size points above were capped
by the rate requested; the next table gives the ceilings.

### Flexible crop: the line (HMAX)

The flex crop used to inherit the 1080p crop's line, HMAX 366 (`0x016E`).
It now uses 300, and the fps and AE line-rate constants derive from that
value (`IMX335_FLEX_HMAX`). HMAX counts the 74.25 MHz internal clock, so 300
is a 247500 lines/s line rate.

Before, the fps-to-VMAX conversion assumed HMAX 366, and AE's
`LinesPer500ms` was 1.47× too short even at 366, so AE's exposure times
were off by that factor.

Measured on `openipc-hi3516ev300` by rewriting HMAX live in the flex mode,
`Isp_FrameRate=240`, slow shutter off. Each figure is the VENC `Send` rate;
predicted sensor rates (74.25 MHz / (HMAX × VMAX)) are in brackets where
delivery falls short of them:

| Crop | HMAX 366 | HMAX 300 | HMAX 290 | HMAX 256 |
|---|---|---|---|---|
| 1920×1080 | 54 (sensor 88) | 54 | 54 | — |
| 1280×720 | 121 (sensor 129) | 122 | 122 | — |
| 800×480 | 186 | 227 | 235 | 168 (sensor 265), **picture corrupt** |
| 480×352 | 218 | — | 300 | — |

What the table shows:

- **Where the limit is.** The picture is clean at HMAX 280 and above, and
  breaks at 270 at every crop width tried (800 and 1920 wide alike): a
  purple cast, column banding and noise. So the limit is the line's own
  timing, not MIPI, which showed no CRC or ECC errors at any value.
- **Why 300.** It keeps a margin to that limit.
- **Where it helps.** Shortening the line only helps below about 720p. At
  1280×720 and 1920×1080 the hi3516ev300's encode pipeline caps delivery
  at about 121 and 54 fps, whatever the sensor does.
- **What is not measured.** gk7205v300 has not been re-measured.

### Full resolution: the encoder binds

The table above streams 1920x1080, where the encoder is not the bottleneck.
Encoding the full 2592x1944 frame, it is. Measured on `openipc-gk7205v300`
in the stock full-scale mode (sensor at 30), 60 s per reading, frames lost
read from VI's `LostFrame`/`VbFail` in `/proc/umap/vi`:

| Encoded 2592x1944 | H.265 | H.264 | VI lost |
|---|---|---|---|
| asked for 26 | 21 fps | 21 fps | 5 a second |
| asked for 20 | 20 fps | 20 fps | none |

So the two codecs still agree at full resolution, and about 21 fps (some
107 Mpixel/s) is the ceiling; 20 is the highest rate delivered in full. That
is the same pixel rate as the hi3516ev300's 54 fps at 1080p and 121 fps at
720p in the HMAX table above.

**In 2:1 line WDR the sensor must stay at that rate as well.** The 5M WDR
mode runs at about 22 fps. There VI writes both exposures of every frame to
DDR from the same buffer pool the encoder's input comes from, so a stall in
the encoder reaches VI, and a 2592x1944 keyframe is such a stall. With the
sensor at 22 and the stream decimated to 20 or 15 fps, VI still lost one
frame per keyframe; with the sensor at 20, none. A linear mode does not need
this: a 30 fps sensor decimated to a 20 fps stream lost nothing. Past the
limit in WDR, H.264 degrades more than H.265 (17 fps against 21 with the
sensor at 22).

Ruled out along the way, each leaving the losses unchanged:

- **ISP back-end clock.** VIPROC already runs at its fastest documented
  offline clock, 257 MHz (`PERI_CRG63[2:0]=0`). Setting `PERI_CRG63[3]`
  hands it the 360 MHz VI pixel clock instead, which the user guide lists as
  not selectable in that mode. Its hardware time per WDR frame (both
  exposures, one pass) fell from 23.2 to 16.6 ms, with no errors or visible
  damage over 90 s. The frame losses did not change. VI capture (360 MHz),
  VPSS (257 MHz) and VEDU (450 MHz) are already on their top settings.
- **A bigger VB pool.** Eight blocks rather than six lost as many.
- **JPEG snapshots.** Off, the same.

The SoC read 84-89 °C throughout, and nothing throttled.

## IMX335 on two CSI-2 lanes (hi3516cv6xx)

For boards that route only two of the sensor's four data lanes. Every other
IMX335 mode drives four, which such a board receives as CRC errors and no
frame at all. The two-lane modes are selected by number only (`Isp_SnsMode`),
never from size and rate: the lane count belongs to the board, not to the
picture asked for.

Measured on a Hi3516CV613 board with the lanes on MIPI-RX PHY lanes 0 and 2,
majestic at `video0.size = 1920x1080`, H.265, slow shutter off, daylight at
1x analog gain. Rates are encoded frames counted in `/proc/umap/venc`.

| Mode | Picture | Encoded | Sensor clocks | Selected by |
|------|---------|---------|---------------|-------------|
| 6 | 2592×1944 | 30 fps | Sony's two-lane setting | `Isp_SnsMode=6`, `Isp_FrameRate=30` |
| 7 | 2592×1944 | 40 fps | Sony's, shorter line | `Isp_SnsMode=7`, `Isp_FrameRate=40` |
| 8 | 2592×1944 | 45 fps | PLL 6.25% fast | `Isp_SnsMode=8`, `Isp_FrameRate=45` |
| 9 | 2560×1440 window | 60 fps | PLL 6.25% fast | `Isp_SnsMode=9`, `Isp_FrameRate=60`, `Isp_W=2560`, `Isp_H=1440`, `DevRect_w=2560`, `DevRect_h=1440` |

Against the stock `imx335_i2c_5M.ini` every one of them also needs
`raw_bitness=10`, `Isp_W`/`Isp_H` set to the mode's picture, and the board's
lanes in `lane_id` (`0|2|-1|-1|-1|-1|-1|-1|` on the measured board). The
driver refuses a two-lane `Isp_SnsMode` whose size, rate or WDR mode does not
fit the mode, and logs which one. A lower `Isp_FrameRate` is reached by
lengthening the frame, in every mode, down to the rate at which VMAX is full
(0.12 fps in mode 6, 0.16 in mode 7, 0.17 in modes 8 and 9). Majestic's slow shutter
(`isp.slowShutter`, default `medium`) lowers the rate in the dark; `disabled`
holds it.

The OpenIPC firmware profiles for these modes are `imx335_i2c_5M_2lane.ini`
(mode 6) in `/etc/sensors`, and `imx335_2lane_40.ini`, `imx335_2lane_45.ini`
and `imx335_2lane_1440p60.ini` (modes 7 to 9) in `/etc/sensors/modes`. With
`isp.sensorConfig` empty, majestic tries every `/etc/sensors/imx335*` file in
name order and keeps the first that gets frames into VI within 2 s. A
two-lane board therefore comes up at 30 fps on its own, about 4 s later than
a four-lane one, which succeeds on `imx335_i2c_5M.ini` and never tries the
two-lane file. The other three are chosen by setting `isp.sensorConfig`.

Only mode 6 is a vendor setting. Each of the others held its rate from a
clean start with no lost frame and no MIPI error: 40.0 fps for 3 minutes,
45.0 for 3, 60.0 for 5, and the same rates counted at an RTSP client.

### What bounds them

`fps = 74.25 MHz × INCKSEL1 / 0x80 / (HMAX × VMAX)`. `INCKSEL1` (0x314C)
scales the sensor's whole clock tree: pixel clock, ADC and link together.

| Limit | Value | How it fails |
|---|---|---|
| HMAX floor | 426, or 421 in the 2560-wide window | Picture breaks into shifted bands; every counter stays clean |
| VMAX floor | the picture's rows plus about 50 blank: 3984 at 1944 rows, 2980 at 1440 | No frames; VPSS counts a `frame_err` each |
| `INCKSEL1` in a bright scene (1x gain) | 0x89 clean, 0x8C broken | Pattern noise in dark areas, blacks lifted |
| `INCKSEL1` in a dark scene (full gain) | 0x98 clean, 0x9A broken | Blue cast, column stripes |
| ISP row time | 10.37 us (2737 cycles of the 264 MHz ISP clock) | No frames; VPSS counts a `frame_err` each |

- **The HMAX floor cannot be seen in any counter.** From 425 down to 414 the
  receiver still measures full frames with no CRC error and the encoder runs
  at the full rate, while the picture is damaged. Only a snapshot shows it.
- **The PLL has to be checked at both ends of the gain range.** A 45 fps
  setting on `INCKSEL1` 0x90 passed in room light and at night and failed in
  daylight. Modes 8 and 9 therefore stay at 0x88 and take the rest from HMAX
  and VMAX.
- With the PLL held there the two-lane link is what binds: 43.7 fps on
  Sony's clocks and 46.5 at 0x88 for the full sensor, 62.9 for the window.
  The ISP limit is only reached on clocks the picture does not allow.
- The window's rate follows its rows, not its pixel count: 1440 rows plus 50
  blank against 1944 plus 48 is 1.34 times fewer, and 45 fps × 1.34 is 60.
- Encoding the native size, the encoder binds instead: about 34 fps at
  2592×1944 and 45 fps at 2560×1440 under majestic.
- Vertical flip works in modes 6 to 8 and is not implemented in the window,
  which stays upright and logs it. Mirror works in all four. In mode 8 the
  flipped readout starts at row 4112, past its VMAX of 4040; a snapshot shows
  it intact, like mode 6's.
- The window is stretched through VMAX like the full-frame modes, unlike the
  four-lane crop modes: 50, 30 and 5 fps and majestic's slow shutter (18 fps
  at full gain in a dim room) delivered every frame with the picture intact.

Not measured: temperature, other boards, modes 7 and 8 at night (every
`INCKSEL1` up to 0x98 was clean at full gain at 30 fps timing; mode 9 ran at
full gain in a dim room), and anything longer than 5 minutes.

## IMX307 high-framerate modes (hi3516ev200 / gk7205v200)

Wire fps from VENC `Send` counter (5 s window, h.265 @ 4 Mbps wire) on
`openipc-gk7205v200` with the `sony_imx307_2L` driver (2-lane MIPI
CSI-2). `Isp_SnsMode` dispatch follows the same IMX335 PR #99 pattern
(`u8SnsMode=4` → window crop, programmable W×H). Register values
verified against Sony IMX307LQD-C datasheet pp.49 / 54-58 / 60-65 /
66-68. The 4-lane variant (`sony_imx307`) carries the same dispatch
infrastructure with MIPI D-PHY timings from the datasheet's "4-Lane"
columns — not yet validated on real hardware.

| Mode | Sensor crop | Wire fps | Encoder ceiling | Selected by |
|------|-------------|----------|------------------|-------------|
| Stock 1080p | 1920×1080 | 30 fps | — | (default) |
| 1080p60 boost | 1920×1080 | 44 fps | **44** (encoder-bound) | `Isp_FrameRate=60` |
| 720p60 sub-readout | 1280×720 | 60 fps | 60 (sensor PHY tier) | `Isp_SnsMode=1` |
| 720p flex | 1280×720 | up to **91 fps** | 91 (encoder-bound) | `Isp_SnsMode=4` + `Isp_W=1280 Isp_H=720` |
| VGA flex | 640×480 | up to **130 fps** | 130 (encoder-bound) | `Isp_SnsMode=4` + `Isp_W=640 Isp_H=480` |
| CIF flex | 368×304 | up to **200 fps** | 200-219 (encoder-bound) | `Isp_SnsMode=4` + `Isp_W=368 Isp_H=304` |

Lower bound on flex-crop dimensions is 368×304 (datasheet WINMODE=4h
constraint: WINWH ≥ 368 mult-of-4, WINWV ≥ 304). At 1080p the encoder
caps wire fps regardless of sensor rate; sub-1080p resolutions lift
the ceiling roughly with the reciprocal of pixel count, until either
the encoder hits a per-frame budget or the sensor reaches its own
VMAX-floor at the requested crop.

gk7205v200 requires `clock=37.125MHz` in the INI's `[mode]` section
(vendor blob defaults to 27 MHz INCK; without the override the
delivered rate is ~73% of nominal). Same gotcha applies to IMX335
high-fps presets on the gk side.

## MIS2009 high-framerate mode (gk7205v500)

Wire fps from the VENC `Send` counter (8 s window, H.265 at 4 Mbps,
1920×1080) on a gk7205v510 with the `imagedesign_mis2009` driver, 27 MHz
clock, 2-lane MIPI RAW10. `Isp_FrameRate` above 30 in the sensor INI selects
the faster PLL; 30 and below keep the vendor's.

| Mode | PLL (VCO) | Line rate | Wire fps | Selected by |
|------|-----------|-----------|----------|-------------|
| Vendor 1080p30 | 756 MHz | 33780 lines/s | 30 | `Isp_FrameRate` ≤ 30 (default) |
| 1080p50 | 1296 MHz | 57857 lines/s | 50 (51 max) | `Isp_FrameRate=50` |

The 1080p50 mode is the vendor table with only the PLL raised (FBDIV 28 →
48, plus the datasheet's TSDIV/CPDIV and the MIPI `clk_period` for 648 Mbps
per lane). The line stays 2240 PCLK, so the timing generator, counted in
ACLK cycles, needs no change.

The MIS2008 datasheet's 1080p60 row (FBDIV 55, VCO 1485 MHz, line 2154 PCLK)
does stream at 61 fps on this part, and VENC encodes all of it, but bright
pixels come out dark with a coloured fringe. Isolated speckles per frame,
same scene, line 2154:

| VCO | 756 | 972 | 1080 | 1188 | 1296 | 1404 | 1485 |
|-----|-----|-----|------|------|------|------|------|
| fps | 31 | 40 | 45 | 49 | 54 | 58 | 61 |
| speckles | 2–3 | 3 | 3 | 2–3 | 1–7 | 6 | 26–30 |

The speckles track the pixel clock, not the VCO or the frame rate:

- **Not the VCO.** With FRANGE1 1 and the vendor's own VCO of 756 MHz,
  PCLK is 151.2 MHz, and the picture speckles just the same (10–21 a frame
  at a 2154 line, 13–21 with ACLK slowed to 252 MHz and the line
  lengthened to 2560).
- **Not the frame rate.** VCO 1485 at a 30 fps line is worse still.
- **Not the trims.** Neither a longer line, a slower ACLK nor the PLL trim
  registers (0x3304–0x3309) clear them.

Clean runs reach PCLK 129.6 MHz; 140 MHz already speckles. In RAW10 over two
lanes the MIPI bit clock is always 5× PCLK, so these runs cannot tell the
two apart. A line shorter than the active width stops the stream, so 1080p
is held to about 54 fps by the clean PCLK, whatever the input clock.

The encoder is not the limit at 1080p: VENC takes 61 fps from one channel.

### Crop and subsampled modes

Selected by `Isp_SnsMode` in the sensor INI, at the size its `DevRect` gives:

- **`Isp_SnsMode=4` (crop):** a centred crop of any size up to
  1920×1080, keeping the colour filter phase (GRBG).
- **`Isp_SnsMode=5` (subsampled):** the full field read 2×2-subsampled to
  960×540. The window has to start one column further right, so the
  profile says `Isp_Bayer=BAYER_RGGB`.

Both modes run the 1080p50 PLL (PCLK 129.6 MHz), and the frame is the rows
read plus the vendor's 46 lines of blanking.

| Mode | Line | Line rate | Frame rate |
|---|---|---|---|
| Crop (4) | 2154 PCLK | 60167 lines/s | 60167 / (H + 46) |
| Subsampled (5) | 2400 PCLK | 54000 lines/s | 54000 / 586 |

Why those lines:

- **Crop: 2154 PCLK.** The timing generator needs about 4270 ACLK cycles
  a line; 4000 breaks the picture. 2154 PCLK is 4308 cycles.
- **Subsampled: 2400 PCLK.** Frames stop at 2240 PCLK on this PLL and flow
  at 2272; 2400 keeps a margin.

Measured with `Isp_FrameRate` at each mode's ceiling and slow shutter off.
In a dim scene, AE's slow shutter lengthens the frame and the rate falls
below these. MIPI CRC/ECC errors were 0 in every run, and no frame
speckled:

| Mode | Size | Ceiling | Delivered |
|---|---|---|---|
| crop | 1920×1080 | 53 | 53.0 |
| crop | 1600×900 | 63 | 63.1 |
| crop | 1280×720 | 78 | 78.2 |
| crop | 1024×576 | 96 | 96.4 |
| crop | 800×480 | 114 | 114.4 |
| crop | 640×360 | 148 | 149.0 |
| crop | 320×240 | 210 | 211.1 |
| crop | 320×180 | 233 | 231.8 |
| crop | 256×144 | 240 | 240.8 |
| subsampled | 960×540, full field | 92 | 92.2 |

Limits found along the way:

- **Minimum width.** Below 256 wide the ISP refuses its AE/AF statistics
  configuration. 192×160 streams but without them; 160×128 produces no
  frames at all.
- **Exposure.** The driver tells AE each mode's frame length. Without that,
  AE asks for an exposure longer than the frame and the sensor outputs
  black.
- **Mirror and flip.** Both work in the crop, which moves its window by one
  pixel as the 1080p modes do. Subsampled, flip works but mirror does not:
  with the columns read in pairs, a mirrored readout is garbage at every
  window start tried (columns 5 to 12). The driver leaves the picture
  unmirrored there and logs it.

### Two streams

A second stream from the same VPSS group is bounded by what the pipeline
carries in all, about 80 frames of 1080p a second (166 Mpix/s): two 1080p
streams at 50 come out at 42 and 41.

## OS08A20 and OS04A10 (hi3519dv500)

Measured on a Hi3519DV500 DMEBPRO kit with an OS04A10 on SENSOR0 and an
OS08A20 on SENSOR1, under majestic. Each stream is H.265 at the sensor's
output size, with slow shutter off. The scene is a dim room, so AE ran at
full gain. Rates are VENC's `send` count over 8 s.

### Modes

| Sensor | Mode | Picture | Bits | Rate | Selected by |
|---|---|---|---|---|---|
| OS08A20 | vendor | 3840×2160 | 12 | 30 | default |
| OS08A20 | 2×2 binning | 1920×1080, full field | 10 | up to 120 | `DevRect` 1920×1080, `Isp_FrameRate` above 30 |
| OS08A20 | native window | any W×H to 3840×2160 | 12 | 44 at 3840×2160 to 173 at 640×480 | `Isp_SnsMode=4`, `raw_bitness=12` |
| OS08A20 | binned window | any W×H to 1920×1080, field 2W×2H | 10 | 120 at 1920×1080 to 240 at 640×480 | `Isp_SnsMode=5`, `raw_bitness=10` |
| OS04A10 | vendor | 2688×1520 | 12 | 30 | default |
| OS04A10 | 4M60 | 2688×1520 | 12 | 60 | `Isp_FrameRate` above 30 |
| OS04A10 | window | any W×H to 2688×1520 | 12 | 92 at 2688×1520 to 240 at 640×480 | `Isp_SnsMode=4`, `raw_bitness=12` |

A window is centred on the array and sized by `DevRect_w`/`DevRect_h`.
- **Size rules.** Width must be a multiple of 8, height a multiple of 4,
  and both at least 320×240. The driver refuses any other size, and logs
  why.
- **Line.** The window keeps its base mode's line. The frame is as short
  as the window's rows allow.
- **Requests.** A requested rate above the ceiling runs at the ceiling. A
  lower one lengthens the frame, down to the longest frame the sensor
  takes: 1.53 fps (OS08A20 native), 2.12 (binned) and 15.2 (OS04A10).
- **Profile.** Set `Isp_FrameRate` and `Isp_MaxFrameRate` to the rate you
  want, at most the ceiling, so majestic paces the encoder to it.

| Window | Line | Frame | Ceiling (fps) |
|---|---|---|---|
| OS08A20 native (4) | 1440 PCLK, 10.01 µs | H + 96, +16 above 2136 rows | 99888 / frame |
| OS08A20 binned (5) | 1036 PCLK, 7.20 µs | H + 72 | 138840 / frame |
| OS04A10 (4) | 732 PCLK, 6.78 µs | H + 80 | 147480 / frame |

| Sensor | Window | Ceiling | Delivered |
|---|---|---|---|
| OS08A20 | native 3840×2160 | 44.0 | 44 |
| OS08A20 | native 3840×1600 | 58.9 | 59 |
| OS08A20 | native 1920×1080 | 84.9 | 85 |
| OS08A20 | native 1280×720 | 122.4 | 122 |
| OS08A20 | native 640×480 | 173.4 | 173 |
| OS08A20 | binned 1920×1080 | 120.5 | 120 |
| OS08A20 | binned 1280×720 | 175.3 | 175 |
| OS08A20 | binned 640×480 | 251.5 | 240 (majestic's cap) |
| OS04A10 | 2688×1520 | 92.2 | 91 |
| OS04A10 | 1920×1080 | 127.1 | 127 |
| OS04A10 | 1280×720 | 184.4 | 184 |
| OS04A10 | 640×480 | 263.4 | 240 (majestic's cap) |

MIPI CRC/ECC errors were 0 and the stream decoded without error in every
run. Each picture was checked against the same window at a long frame.

Two of these beat the vendor's own modes, at the same picture:
- **OS08A20 4K at 44 fps in 12 bits,** against 30.
- **OS04A10 full array at 92 fps in 12 bits.** Its datasheet gives 45 for
  12-bit linear at 720 Mbps a lane (table 3-2). The window runs on the
  vendor 2:1 WDR mode's PLL and line, which drive the lanes faster.

### Latency

From the sensor's readout start to the encoded pack, p50, with VPSS low
delay, 9 slices a frame and slice output (`docs/hisi/low-latency.md` in
majestic has the method):

| Sensor | Mode | First slice | Whole frame |
|---|---|---|---|
| OS08A20 | binned 1920×1080 at 60 | 2.63 ms | 8.37 ms |
| OS08A20 | binned 1920×1080 at 120 | 2.58 ms | 8.36 ms |
| OS08A20 | binned window 1280×720 at 175 | 2.22 ms | 5.68 ms |
| OS04A10 | 4M60, 2688×1520 to 1080p | 2.94 ms | 10.92 ms |
| OS04A10 | window 1920×1080 at 127 | 2.52 ms | 7.89 ms |

The whole-frame time follows the rows read times the line, not the frame
rate: 1080 binned rows take 7.8 ms at 60 fps and at 120 alike. A faster
rate shortens the wait for the next frame, by half a frame period on
average, and the exposure. A smaller window shortens the readout itself.

### What bounds them

| Limit | Value | How it fails |
|---|---|---|
| OS08A20 native line | 1400 clean, at 1920 and 3840 wide; 1440 kept | 1300: a white picture; 1200: no frame |
| OS08A20 binned line | 1000 clean; the vendor's 1036 kept | 960: blue cast; 900: purple; 860: no frame |
| OS04A10 line | the 4M60 mode's 732 | 710: magenta; 680: no frame |
| Frame past the window | about 56 rows; 64 kept | broken picture at the full rate; further below, no frame |
| OS08A20 native at full height | 16 rows more | 3840×2160 broke at a blank of 96, clean at 110 |

- **Line floors.** Each line was judged by SSIM against the same window at
  the vendor line and a long frame, and by the mean colour, where a cast
  shows first.
- **Frame past the window.** Each window is taller than its output: by 8
  binned rows on the OS08A20 binned, 32 on the native, 16 on the OS04A10.
  The frame has to run about 56 rows past the window. OS08A20 binned broke
  at a blank of 60 and was clean at 64; native broke at 80 and was clean
  at 90. OS04A10 was clean at 60 and gave no frame at 40.
- **Full height.** A native window taller than 2136 rows cannot start 24
  rows above its picture, so it is clamped to the whole array. 3840×2048,
  which is not clamped, ran clean at the 96-row blank and nearly the same
  pixel rate.
- **No counter shows any of these.** As with the IMX335 line, MIPI counted
  no CRC or ECC error and the encoder ran at full rate while the picture
  was broken. Only a snapshot shows it.
- **The OS08A20's first output rows.** The OS08A20 outputs some 18 black
  rows at the start of the window unless its ISP Y offset (0x3812) is 24:
  measured, 16 still left 4 dark rows. The vendor 4K list had offset 0,
  and its picture carried that black band at the top. The list now starts
  the window at row 0, with offset 24.
- **Mirror and flip.** Neither driver implements them, in any mode.

Not measured: a bright scene (every run was at full gain), temperature, runs
longer than 30 s, and two sensors both in window modes.
