# GalaxyCore GC2063 for Hi3516CV610

A source driver for the GalaxyCore GC2063 (2M, I2C 0x6e, 8-bit paged
registers) on Hi3516CV610/CV608. The SIP-K662C6S module's stock firmware (xipc)
has a GC2063 among its sensor types, but no SDK has a GC2063 driver for this SoC.

One mode: 1920x1080 linear at 30 fps in RAW10, 2-lane MIPI, at a 27 MHz sensor
clock ("gc2063 27Mclk 2M30fps(MIPI)"; `sys_config` gives `gc2063` 27 MHz).

## Where it comes from

- `gc2063_cfg.h`: the firmware's GC2063 bring-up, read out of xipc, 80 writes.
  These are, write for write, the Hi3516CV500 `galaxycore_gc2053` driver's
  1080p30 sequence in this tree; the firmware writes the closing stream-on
  (0x3e = 0x91) twice. The GC2063 takes the GC2053's register map, so the rest
  of the sensor side is that driver's:
  - exposure 0x03/0x04, in lines;
  - analog gain as 29 steps, each a row of 0xb4/0xb3/0xb8/0xb9, 1x..110.5x;
  - digital gain 1x..8x in 1/64 steps through the pre-gain 0xb1/0xb2;
  - VMAX 0x41/0x42, 1125 lines at 30 fps, at most 0x3fff, down to 6 fps;
  - exposure up to 2 lines short of the frame;
  - shutter and gains one frame after the frame length;
  - standby and restart through 0xf2/0xfc.
- AWB calibration: the GC2053 driver's (R 388, B 519 at 4908 K).
- `gc2063_cmos_param.h`: `galaxycore_gc4023`'s ISP tables, unchanged and
  untuned for the GC2063. The firmware has no ISP defaults of its own for it.
- Black level 0x400, as every CV610 GalaxyCore driver here has it: the CV610
  ISP takes black level on a fixed 12-bit scale whatever the sensor's depth.
  The 0x100 of the CV500 driver is a different ISP's.
- CFA RGGB, which is what the DNG metadata says. OpenIPC's hi3516cv500 and
  hi3516ev200 GC2053 profiles run this same register sequence with
  `Isp_Bayer=BAYER_RGGB`.

## Orientation

0x17 on page 0: bit 0 mirrors, bit 1 flips, over a base of 0x80, as the
Hi3516CV500 GC2053 driver writes it. Neither GC2053 driver in this tree moves
the window with it, and the XMedia one (`gk7206v100/galaxycore_gc2053`) gives
one Bayer order for all four orientations, so the phase is taken not to move.
Unverified on a GC2063. The orientation is kept per pipe, written by the
callback and again by every bring-up, both under `cis->mutex`. The bring-up
writes it after the table has turned the stream on, so the first frame or two
may come out upright.

## What was added to the recovered driver

- The AE registers the bring-up replays are seeded when AE has not filled them
  yet. They hold the mode's frame length, the table's own exposure and 1x, not
  zeros, so the first frames are not black.
- Bring-up errors are returned and stop `init` from being marked done.
- The I2C bus defaults to 0 until `pfn_set_bus_info`, as in `galaxycore_gc4653`.

Not run on a GC2063 yet.
