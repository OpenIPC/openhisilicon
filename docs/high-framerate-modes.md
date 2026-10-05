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
