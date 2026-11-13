# Sony IMX678 for Hi3516CV610

The IMX678 (Starvis 2, 8 MP, chip ID 0x4d1c & 0x3ff = 0x2a6) on this tree's
public sensor interface and `common/sensor_common.c`.

The K662C6S module's stock firmware lists the IMX678 (sensor type 274), but it
carries only the vendor's scene tuning for it (`IMX678_Init_SensorScene`,
`IMX678_ISP_SetBNRAttr`). It has no register table, no I2C code and no
`cmos_*` callbacks for it, so unlike the IMX662 and IMX664 there was no driver
to recover from it. This one is the Hi3519DV500 IMX678 driver (the vendor's
`IMX678_Hi3519DV500-driver.zip`, same OT/B051 driver generation as CV610),
moved off its own I2C layer and onto `cis_info`, with the exposure and gain
model of this tree's IMX662/IMX664 drivers.

Mode: 3840x2160 linear RAW12 at 30 fps, 4 CSI-2 lanes at 1188 Mbps, from the
37.125 MHz sensor clock `sys_config` feeds an `imx678`. HMAX 1100, VMAX 2251.
No WDR: `cmos_isp_set_wdr_mode()` rejects every mode but linear.

The CV610 ISP takes up to 4096x4096, so the frame fits. Whether CV610 keeps
up with 3840x2160 at 30 fps has not been measured.

## Where it comes from

- `imx678_cfg.h`: the Hi3519DV500 driver's `imx678_linear_4K30_12bit_init()`
  write for write, less the standby/master-start framing, which
  `imx678_sensor_ctrl.c` does. On top of it, 13 registers that the upstream
  Linux STARVIS 2 driver sets and that table leaves at reset, marked
  `upstream` (see below). The initial SHR0 is 4, not 3, which is Sony's
  minimum; AE overwrites it before the first frame either way.
- `imx678_cmos.h`: the register map, limits and AWB calibration (450/447 at
  4950 K, curve 18, 238, 0, 212714, 128, -163748) of the Hi3519DV500 driver.
- `imx678_cmos_param.h`: the Hi3519DV500 driver's ISP tables, less its WDR
  variants and the CAC, Bayer-sharpen and pregamma tables it never hands the
  ISP. Its black-level geometry is set to the 3840x2160 readout. Not the
  IMX678's own:
  - **Bayer NR, DRC, mesh shading, ACS:** five structures are laid out
    differently on the Hi3519DV500 than on CV610. Four of them are taken
    instead from the K662C6S firmware's IMX664, the same Starvis 2 family. The
    NR image is re-laid for `load_ref_en`/`snr_version` exactly as in
    `sony_imx662` (it is the same vendor table, byte for byte).
  - **CA:** the fifth comes from `smart_sc450ai`, because there is no CV610
    Sony CA.
  - **Sharpen, CV610-only fields:** the CV610 sharpen structure has fields the
    Hi3519DV500 one does: shoot thresholds, edge reliability and gain by
    motion. Those carry the IMX664/IMX662 values. Left out, they would have
    been zero, including unity edge gains of 32.

  Every byte image has a `_Static_assert` beside it.
- Mesh shading and ACS are off (`bit1_lsc`/`bit1_acs` = 0), as the vendor
  ships every CV610 sensor.

## Exposure and gain

SHR0 (0x3050) holds the exposure, counted back from the frame end, clamped to
[6, VMAX - 1]; the longest exposure is VMAX - 8, the Hi3519DV500 driver's
limit. GAIN (0x3070) takes 0.3 dB steps, and analog and sensor digital gain
are two indexes into one table, summed into it.

Analog gain is 30 dB at low conversion gain. Above analog index 99 the driver
switches the sensor to high conversion gain (FDG_SEL0, 0x3030) and takes 27
steps (8.1 dB) off the code. Where those numbers come from:

- **The 27-step offset** is the Hi3519DV500 driver's HCG step. The datasheet
  gives the HCG/LCG ratio as 2.4 to 2.9, typically 2.7.
- **The threshold of 99** is the IMX662/IMX664 drivers' one, not the
  Hi3519DV500 driver's 27. The IMX678 software reference allows GAIN only
  from 34 to 240 with FDG_SEL0 = 1, and the Hi3519DV500 threshold wrote 0 to
  33 there. Switching at 99 keeps GAIN at 73 or more.

The ceilings are 38.1 dB analog (index 127, HCG included), 42 dB sensor
digital (index 140, so the sum stays within GAIN's 240) and 2x ISP gain.

## Orientation

`pfn_mirror_flip` writes HREVERSE (0x3020) and VREVERSE (0x3021), as
`sony_imx662` does. The request is kept per pipe, written again by every
bring-up while the sensor is still in standby, and handled under
`cis->mutex`. The Hi3519DV500 driver had none. The upstream driver reports
SRGGB12 in every orientation, so the Bayer phase is taken not to move. That
has not been measured here.

## Bring-up timing

Standby cancel, then 24 ms, then master start (XMSTA = 0). The 24 ms is the
datasheet's internal regulator stabilization; the Hi3519DV500 driver waited
18 ms, and upstream waits 25. `pfn_standby`/`pfn_restart` write the same
pair as the Hi3519DV500 driver does.

## Checked against upstream (OpenIPC/firmware#2736)

The register table was compared with `imx678_common_regs` of the upstream
Linux STARVIS 2 series (sony-starvis2, v7, which grew out of mainline
`imx678.c`):

- **No value differs** across the 371 registers both write.
- **13 registers** that upstream sets and the Hi3519DV500 table leaves at
  reset are added: 0x301a, 0x301c, 0x301e, 0x306b, 0x3400, 0x3a64, 0x3c37,
  0x3cf2-0x3cf4, 0x3eb7, 0x3f4c and 0x4423.
- **Only in ours:** the mode registers upstream computes at run time (INCK,
  data rate, ADBIT, HMAX, LANEMODE, SHR0), and REGHOLD.
- **SHR minimum.** Sony allows SHR from 4 to VMAX - 1 (upstream patch 07);
  this driver keeps 6, inside it.
- **Line time.** Upstream's minimum HMAX on 4 lanes at this link rate
  (594 MHz) is 792; 1100 is above it.

## Not done here

- **No 60 fps mode.** The Hi3519DV500 driver declared 8M 60 fps, but its
  bring-up wrote the 4K30 table for every mode, so it never ran at 60. 60 fps
  needs HMAX 550, which upstream allows only at a link rate of 891 MHz or more
  on 4 lanes.
- `pfn_read_reg` is a stub that reads nothing, as `cis_read_reg` is for every
  sensor in this tree. The chip ID is not checked.
- **Scene tuning.** The firmware's IMX678 day scene (AE route, Bayer NR,
  saturation, dehaze) is not here. It is byte-identical to its IMX664 one.
- **Not run on an IMX678 yet.**
