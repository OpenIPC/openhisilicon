# SmartSens SC850SL for Hi3516CV610

The SC850SL driver of the SIP-K662C6S module's stock firmware (xipc), recovered
from that firmware. It is rebuilt on the layout of the vendor's CV610
SmartSens driver (`smart_sc450ai`, from the SDK) and links against this tree's
`common/sensor_common.c`. No SDK has an SC850SL driver for this SoC.

Modes, both at a 27 MHz sensor clock (`sys_config` gives `sc850sl` 27 MHz):

- 3840x2160 linear at 30 fps in RAW12, 4 lanes at 1080 Mbps ("SC850SL_MIPI_27MInput_4lane_1080Mbps_12bit_30fps");
- 3840x2160 2-to-1 line WDR (virtual channel) at 30 fps in RAW10, 4 lanes at
  1458 Mbps.

## Where it comes from

- `sc850sl_cfg.h`: the power-on register tables, byte for byte. xipc does not
  keep them as tables: it writes them one call at a time, and they were read
  back out of the code. `sc850sl_sensor_ctrl.c` issues a soft reset (0x0103)
  first and turns the stream on (0x0100) after the table and the AE
  registers, as `smart_sc450ai` does.
- `sc850sl_cmos.h` and `sc850sl_cmos.c`: the SC850SL's frame lengths, gain
  tables and limits, black level and AWB calibration on the `smart_sc450ai`
  code. This includes 3.125x conversion gain, 1x..49.6x analog, 8x sensor
  digital and black level 0x444. The firmware's sensor callback table
  (`cmos_register_callback`, sensor ID 850) is where these were read.
- `sc850sl_cmos_param.h`: the ISP default tables, each a byte image of its
  structure as the firmware holds it, with the firmware address of each in
  its comment. A `_Static_assert` beside each one refuses to compile if a
  structure stops matching the image. A few the firmware does not reach from
  that callback table come from `smart_sc450ai` and say so: piris, the WDR AWB
  tables and the colour sectors.
- The two Bayer NR tables were built against an SDK whose `ot_isp_nr_attr` has
  no `load_ref_en`/`snr_version`. They are re-laid the way the IMX662 driver's
  is: both fields 0, with the rest moved down 8 bytes.

## Exposure and gain

The exposure (0x3e00..0x3e02) and the frame length (0x320e/0x320f) are in half
lines; the frame length the ISP sees is twice VMAX. Analog gain is 0x3e08/0x3e09
from a 256-step table, with high conversion gain (0x2000 in the code) above
1x x 3.125. Sensor digital gain is 0x3e06/0x3e07, from a second 256-step table.
VMAX is 2250 linear and 4500 in WDR, at most 0x79e0 linear, which is down to
1.5 fps.

## Orientation

Both tables leave 0x3221 at 0x66 (mirrored and flipped). That is how the
vendor's module is mounted, so it is this driver's normal orientation.
Mirror and flip toggle bits [2:1] and [6:5] away from it. The XMedia SC850SL
driver in this tree (`gk7206v100/smartsens_sc850sl`) reports BGGR in all four
orientations, so the Bayer phase does not move. The orientation is kept per pipe,
written by the callback and again by every bring-up before the stream starts,
both under `cis->mutex`.

## What was added to the vendor's code

- Standby and restart stop and resume the stream (0x0100). The vendor's are
  empty.
- The AE registers the bring-up replays are seeded when AE has not filled them
  yet: the mode's frame length and, in linear mode, the table's own exposure
  at 1x. They are not a frame length of 0.
- Bring-up errors are returned and stop `init` from being marked done.
- The I2C bus defaults to 0 until `pfn_set_bus_info`, as in `galaxycore_gc4653`.

Not run on an SC850SL yet.
