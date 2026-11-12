# Sony IMX662 for Hi3516CV610

The IMX662 driver of the SIP-K662C6S module's stock firmware (xipc, sensor
type 272), recovered from that firmware and rebuilt against this tree's public
sensor interface and `common/sensor_common.c`.

Modes: 1920x1080 linear at 30 and 60 fps in RAW12 and 90 fps in RAW10, all at
the 37.125 MHz sensor clock `sys_config` feeds an `imx662`. The frame rate is
set by HMAX; VMAX is 1251 in every mode. No WDR: `cmos_isp_set_wdr_mode()`
rejects every mode but linear.

## Where it comes from

- `imx662_cfg.h`: the power-on register tables, byte for byte, one per mode.
  `imx662_sensor_ctrl.c` writes them in standby, then cancels standby and
  starts the master.
- `imx662_cmos.h`: the register map, exposure and gain model, limits and AWB
  calibration, as the vendor's `cmos_*` functions use them.
- `imx662_cmos_param.h`: the ISP default tables (CLUT, DPC, LSC, ACS,
  demosaic, sharpen, DRC, Bayer NR, anti-false-colour, LDCI, gamma, dehaze,
  noise calibration, black level, colour matrix, AWB saturation by gain).
  Each one is a byte image of its B051 structure. A `_Static_assert` beside
  each one refuses to compile if a structure stops matching the image, rather
  than letting the ISP read a shifted table.

## Exposure and gain

SHR0 (0x3050) holds the exposure, counted back from the frame end. The gain
register (0x3070) takes 0.3 dB steps. Analog and digital gain are two indexes
into one table, summed into it. Above index 99 of analog gain, the driver
switches the sensor to high conversion gain (FDG 0x3030) and takes 51 steps
(15.3 dB) off the code.

The ceilings are 45.3 dB analog (high conversion gain included), 42 dB sensor
digital and 2x ISP gain. The AE route a host program runs is its own business:
the vendor firmware's own day scene (route, Bayer NR, DPC, saturation) ships as
OpenIPC's `iq/imx662.ini` for majestic, not here.

## Compared with the driver it replaced

On the same module, at night, at an equal exposure and gain (50 ms, about 37x):

- **Colour.** This driver's AWB calibration, colour matrix and saturation
  table come from the vendor, and a street at night keeps its colour. The
  previous driver borrowed them from `smart_sc450ai` and gave an almost grey
  picture.
- **Detail.** This picture is slightly softer.
- **Shortest exposure.** It is 9 lines (240 us), not 1.

## Orientation

`pfn_mirror_flip` writes HREVERSE (0x3020) and VREVERSE (0x3021); the readout
window and its Bayer phase follow them on this sensor, so nothing else moves.
Measured on the module: all four orientations, live and from a restart, with
R/G and B/G within 0.04 of the upright picture. The request is kept per pipe
and written again by every bring-up while the sensor is still in standby, under
`cis->mutex`, which the callback takes as well.

## White balance

The AWB calibration and colour matrices are those of the stock firmware's day
scene, not the driver defaults it carries. Once its ISP is up the stock
firmware writes its own AWB curve, reference gains (418/545 at 4950 K) and a
four-temperature colour matrix table (7500, 6500, 4800, 2600 K) over what the
driver supplied, so the defaults were never what the module ran.

On the module, with an unchanged host program and IQ profile, a street scene
measured on the delivered JPEG (R/G and B/G of neutral surfaces):

| | driver defaults | day scene |
|---|---|---|
| grey pavement, overcast | 0.62 / 0.55-0.59 | 0.78-0.80 / 0.89 |
| sky, overcast | 0.79 / 0.88 | 0.84 / 1.03 |
| white fascia, sun | - | 1.01-1.03 / 0.94-0.97 |

The defaults turned the picture green and the sky cyan. With the day scene a
sunlit white is within a few percent of neutral, and against raw frames AWB
lands within 0-5% of the neutral red gain and 4-7% short of the blue. Under
an overcast sky the picture stays slightly cool: that case has not been
measured against raw.

The stock firmware's other scenes (1 to 4, with their own curves and
matrices) are not carried, and neither is its black level of 820 (this
driver's is 800).

## Not done here

- `pfn_read_reg` is a stub that reads nothing, as `cis_read_reg` is for every
  sensor in this tree and as it was in the driver this replaced.
- No 100 fps mode: it needs a 27 MHz sensor clock. The driver this replaced
  declared one and refused it at 37.125 MHz.
