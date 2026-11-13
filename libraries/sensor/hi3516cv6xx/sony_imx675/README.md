# Sony IMX675 for Hi3516CV610

An IMX675 driver for the Hi3516CV610, rebuilt against this tree's public sensor
interface and `common/sensor_common.c`. The SIP-K662C6S module's stock firmware
(xipc) has no MPP driver for the IMX675; it has only a vendor scene layer and
the sensor's register bring-up, inlined. That bring-up is taken from the
firmware. The rest comes from `sony_imx664`, the closest STARVIS 2 sibling
the firmware does carry.

Modes, both linear, 30 fps, RAW10, 4 MIPI lanes at 594 Mbps, at the 37.125 MHz
sensor clock that `sys_config` feeds an `imx675`:

| mode | readout |
|---|---|
| 0 | 2592x1520, centred crop (PIX_VST 212, PIX_VWIDTH 1520) |
| 1 | 2592x1944, all pixels |

VMAX is 2200 and HMAX is 1125 in both. No WDR.

## Where it comes from

- `imx675_cfg.h`: the power-on register tables from the firmware's bring-up.
  - 5M is sub_AB358 and 4M is sub_ABB80, each a flat run of 16-bit address
    and 8-bit data writes. The firmware prints "IMX675 {5M|4M}@30FPS 10bit
    Initial OK".
  - 4M is 5M plus the window-cropping registers (WINMODE 4, PIX_HWIDTH 2592,
    PIX_VST 212, PIX_VWIDTH 1520). Both offsets are even, so the Bayer phase
    stays RGGB.
  - 25 analog registers follow Sony's sheet instead (see "Checked against").
    Each is marked, with the vendor's value beside it.
  - `imx675_sensor_ctrl.c` writes standby and master stop, the table and the
    AE registers, then cancels standby, waits 18 ms (the vendor's usleep) and
    starts the master.
- `imx675_cmos.h`, `imx675_cmos_param.h`: borrowed from `sony_imx664`, since
  the firmware has no IMX675 MPP data. That covers:
  - the register map, which is the same on every STARVIS 2 part (SHR0 0x3050,
    GAIN 0x3070, VMAX 0x3028, HMAX 0x302c, FDG 0x3030);
  - the exposure and gain model;
  - the AWB calibration, the DNG colour data and every ISP default table.

  The ISP tables carry the same `_Static_assert` guard and Bayer NR re-lay as
  `sony_imx664`. They are untuned for the IMX675.
- Black level 0x320 (800) in the ISP's 12-bit-normalised domain, as for the
  other STARVIS 2 drivers; DNG white level 1023.

## Exposure and gain

As `sony_imx664`:
- SHR0 is held in [6, VMAX-1];
- the gain register is 0.3 dB per step, one table for analog and digital;
- above analog index 99 the sensor switches to high conversion gain and 51
  steps come off the code;
- VMAX and SHR0 are seeded before the first AE run.

The vendor's day scene (AE route, Bayer NR, saturation, dehaze, black level)
ships as OpenIPC's `iq/imx675.ini` for majestic, not here.

## Orientation

Not in the vendor's bring-up. `pfn_mirror_flip` writes HREVERSE (0x3020) and
VREVERSE (0x3021), kept per pipe, written in standby by every bring-up and live
by the callback, under `cis->mutex`. The upstream Linux driver keeps SRGGB
under both flips, so the CFA is not changed.

## Checked against

Three references, for OpenIPC/firmware#2736:

- **Sony, IMX675 Standard Register Setting Ver1.0**, sheet AllPixel_Binning,
  column "All-pixel scan, CSI-2 4lane, AD 10bit, 594Mbps, 30fps". This is the
  vendor's mode.
  - Every register the sheet sets matches, except 25 analog values where the
    vendor table holds an earlier revision's:
    - 0x3c0a-0x3c0f: vendor 0x00, Sony 0x03;
    - 0x4550-0x4558 and 0x455c-0x4564: vendor 0x11, Sony 0x10;
    - 0x347b: the vendor does not write it, Sony sets 0x02.

    Those follow Sony here.
  - What is left is the vendor's own choice:
    - INCK 37.125 MHz (Sony's sheet uses 74.25);
    - HMAX 1125 and VMAX 2200 (Sony: 1100 and 2250, also 30 fps).
- **Upstream Linux, `sony-starvis2` v7, patch 12/18 (IMX675)**.
  - It agrees with Sony's sheet on 0x347b and 0x4550-0x4564.
  - Its table is 12-bit, so it has 0x1f at 0x3c0a-0x3c0f, as Sony's 12-bit
    columns do.
  - Chip ID 0x4d12 & 0xfff = 0x576; it is not probed here, since
    `cis_read_reg` is a stub.
- **Lanes and line time.** LANEMODE (0x3040) is not written, and its reset
  value, 3, is 4 lanes. At 594 Mbps the shortest line is 1100 by Sony's sheet
  and 1022 by the upstream driver's limits, so HMAX 1125 clears both.
- **Exposure limits.** Sony's limit is SHR0 >= 4 and an exposure of at least
  1 line (upstream patch 07/18). This driver holds SHR0 >= 6, which is
  stricter.

The control for this method was `sony_imx662`, which runs on hardware. Against
its own Sony sheet (Ver2.0) and the upstream table it differs in one value and
two missing registers.

## Not done here

- Not run on an IMX675 yet. The analog register update above is from Sony's
  sheet and the upstream driver; neither has been checked on a sensor here.
  To return to the vendor table, restore the marked values in `imx675_cfg.h`.
- No 12-bit, binning or HDR modes.
- `pfn_read_reg` is a stub that reads nothing, as for every sensor in this
  tree.
