# Sony IMX662 for Hi3516CV610

A source-built HiSilicon V5 sensor driver, developed and verified on the
SIP-K662C6S module (Hi3516CV610, four MIPI lanes) by
[snokvist](https://github.com/snokvist/waybeam_venc/tree/master/sensors/cv610/imx662)
and contributed here (OpenIPC/defib#148). It builds against this tree's
public sensor interface and `common/sensor_common.c` like the other cv6xx
drivers; no SDK source or binary is involved.

Modes verified there: 1920x1080 at 30 and 60 fps in RAW12, and 90 and 100
fps in RAW10. 100 fps needs a 27 MHz sensor clock; `sys_config` feeds an
`imx662` 37.125 MHz, which is right for the others, so `cmos_set_image_mode()`
refuses anything above 90 fps until that is switched.

Linear mode only: `cmos_set_wdr_mode()` rejects every WDR mode.

**Not tuned.** The ISP tables are seeds borrowed from `smart_sc450ai` (see
below), there is no measured IMX662 calibration, and the default image came
out blue-tinted. Part of that was a black level of 200 where the ISP wanted
800 (see `imx662_cmos_param.h`); 800 is derived from the register units and
has not yet been checked on a sensor. A calibrated IQ profile is the main
thing missing.

## What the plugin programs

`imx662_cfg.h` holds Sony's power-on sequence — the reserved analog/ADC trim,
the MIPI TX timing, the readout window and the HDR context that linear mode
needs parked. It runs once, in standby, before the per-mode writes, and the
per-mode writes win where the two overlap. That ordering matters for
`0x3A50/51/52`, which keys off ADBIT rather than output depth: the table holds
the 12-bit form and the RAW10 modes overwrite it.

`cmos_get_isp_default()` and `cmos_get_awb_default()` seed the ISP once at init.
The algorithm tables in `imx662_cmos_param.h` are ported from the in-tree
`smart_sc450ai` driver, which targets this same ISP silicon. The blocks that
describe ISP behaviour (gamma, CLUT, demosaic, CAC, anti-false-colour, LDCI,
dehaze, CA) transfer on principle; the noise-fitted ones (bayer_nr, sharpen,
noise_calibration, DRC) are borrowed from a 4 MP sensor and are kept on a
hardware A/B rather than a calibration — a measured IMX662 fit should replace
them. The AWB saturation curve is scaled up from sc450ai's at the low-ISO end.

Two blocks are seeded but not switched on: `g_cmos_drc` and `g_cmos_dehaze`
carry `enable = 0`, inherited from sc450ai's linear-mode defaults, and the ISP
bypasses both at runtime (confirmed by `ss_mpi_isp_get_module_ctrl()`).

## Low-light behaviour

The sharpen auto tables are tapered along their ISO axis to zero by bucket 9,
with the daylight buckets (0-3) left byte-identical. Sharpening is a high-pass
filter, so at high gain the sc450ai tables — which *rise* along ISO, edge
strength 195 to 500 — were amplifying noise into the encoder: measured 65.4 to
7.8 Mbps on the bench with sharpen off. The AWB saturation curve tapers harder
above daylight for the same reason, chroma noise being the ugliest high-gain
artifact.

Conversion gain follows the analog gain: `FDG_SEL0` selects HCG above a
threshold and LCG below it, with the written gain code compensated by 15.3 dB
(Rcg typ 5.8) so the switch is continuous. `max_isp_dgain_target` is 4x, which
puts the total ceiling at 398 x 4 = 1592x (~64 dB) and makes AE exhaust analog
before it reaches for digital.

`g_cmos_noise_calibration` is still sc450ai's photon-transfer fit and is the
main thing left wrong for this sensor.
