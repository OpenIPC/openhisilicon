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
clamps to the per-mode sensor ceiling.

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

The speckles track the VCO, not the frame rate: VCO 1485 at a 30 fps line
is worse still, and neither a longer line, a slower ACLK nor the PLL trim
registers (0x3304–0x3309) clear them. A line shorter than the active width
stops the stream, so ~61 fps is the sensor's 1080p ceiling.

The encoder is not the limit at 1080p: VENC takes 61 fps from one channel.
With a second VPSS channel, VPSS is: at a 50 fps sensor rate, a second
channel tops out at ~33 fps at any size from 704×576 to 1920×1080, while
1080p50 plus a 25 or 30 fps sub-stream holds both rates.
