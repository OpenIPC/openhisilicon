# Sony IMX664 for Hi3516CV610

The IMX664 driver of the SIP-K662C6S module's stock firmware (xipc, sensor
type 284), recovered from that firmware and rebuilt against this tree's public
sensor interface and `common/sensor_common.c`. It is the 4 MP sibling of
`sony_imx662`, and follows the same layout.

Mode: 2688x1520 linear at 30 fps, RAW12, 2 MIPI lanes at 1188 Mbps, at the
37.125 MHz sensor clock that `sys_config` feeds an `imx664`. The table writes
VMAX 1650 and HMAX 1500; AE counts the frame as 1651 lines, as the vendor does.
No WDR: `cmos_isp_set_wdr_mode()` rejects every mode but linear.

## Where it comes from

- `imx664_cfg.h`: the power-on register table, byte for byte, from the
  firmware's unrolled bring-up (sub_B1BBC, write helper 0xb1a94).
  `imx664_sensor_ctrl.c` writes it in standby, then cancels standby, waits
  20 ms and starts the master.
- `imx664_cmos.h`: the register map (sub_B1094), the frame timing
  (sub_B14E0), the exposure and gain model, and the AWB calibration
  (sub_B18D0), as the vendor's `cmos_*` functions use them.
- `imx664_cmos_param.h`: the ISP default tables, each a byte image of its
  B051 structure, read from the firmware. A `_Static_assert` beside each one
  refuses to compile if a structure stops matching the image.
  - The Bayer NR table is the same image as `sony_imx662`'s.
  - It gets the same re-lay: two fields this SDK added, `load_ref_en` and
    `snr_version`, go in at byte 152 as 0, and the 8 bytes read past the
    vendor table are dropped.

## Exposure and gain

SHR0 (0x3050) holds the exposure, counted back from the frame end, and is held
in [6, VMAX-1]. An exposure as long as the frame or longer gives the shortest
SHR0, never a wrapped one. The gain register (0x3070) takes 0.3 dB steps.
Analog and digital gain are two indexes into one table, summed into it.
Above index 99 of analog gain, the driver switches the sensor to high
conversion gain (FDG 0x3030) and takes 51 steps (15.3 dB) off the code.

The bring-up replays the AE registers before AE has run. VMAX and SHR0 are
seeded with the mode's frame length and the shortest exposure, so they never
go out as zero.

The vendor's day scene (AE route, Bayer NR, saturation, dehaze) is a host
program's business. It ships as OpenIPC's `iq/imx664.ini` for majestic, not
here.

## Orientation

The vendor driver has none. `pfn_mirror_flip` writes HREVERSE (0x3020) and
VREVERSE (0x3021), as `sony_imx662` does. On STARVIS 2 parts the readout
window follows them and the Bayer phase stays RGGB; the upstream Linux driver
(`sony-starvis2`) relies on the same. The request is kept per pipe and written
again by every bring-up while the sensor is still in standby, under
`cis->mutex`.

## Checked against

There is no independent register table for the IMX664. It is not in the
upstream STARVIS 2 series (OpenIPC/firmware#2736), and Sony's IMX662 sheet does
not describe its analog block: 45 values in 0x3c0a-0x4549 differ. The table is
the vendor's, unchanged.

ADBIT (0x3022) is 0x02 here. Sony's IMX662 and IMX675 sheets define it as
0 = 10-bit and 1 = 12-bit only, with bit 1 "set to 0". The IMX664 datasheet
on hand does not say what 2 means, and MDBIT (0x3023) = 1 selects 12-bit
output. This is the vendor's value; it is unverified.

## Not done here

- Not run on an IMX664 yet.
- `pfn_read_reg` is a stub that reads nothing, as `cis_read_reg` is for every
  sensor in this tree.
