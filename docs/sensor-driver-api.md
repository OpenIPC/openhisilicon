# Sensor driver API across HiSilicon / Goke platforms

This repo carries sensor drivers for 8 chip families. They look superficially similar
(`<vendor>_<model>_cmos.c`, `<model>_sensor_ctl.c`, a `Makefile` producing
`libsns_<model>.so`) but the **public API the driver presents to the rest of the SDK
differs in 5 distinct ways**. This document is a field guide for two questions:

1. *I found sensor source on GitHub / CSDN. Which SDK is it from?*
2. *I want to port that source to a different platform. What changes?*

The 5 supported tiers (T1, T1+, T2, T3, T4, T5) cover everything the repo can
currently build for. One further real tier (**T6**, post-2022 Shenshu SDK)
appears in archives found online but targets chips this repo does **not** yet
support; §7.2 documents its fingerprints. §7.1 covers a non-tier oddity — a
function-pointer wrapper convention that hides the underlying tier behind a
late-binding indirection — so you can recognise and unwrap it.

All claims below cite a `path:line` you can open and verify. The flow:
classification → identification flowchart → per-tier fingerprints → side-by-side
porting diffs → kernel-side dependencies → boundary tiers → quick-reference
appendix.

## 1. Five-tier classification

The 8 platforms collapse into 5 sensor-driver API tiers. Most porting work happens
at the boundaries between tiers; *within* a tier you can usually copy a driver
across platforms with only a `#include` path change.

| Tier | Platforms | Headline shape |
|---|---|---|
| **T1 / V1** | *(historical mpp/ flavour — not used in this repo, see §3.1)* | `int sensor_register_callback(void)` — **one** MPI call, no AE/AWB lib |
| **T1+ / V1** | `hi3516cv100` | `int sensor_register_callback(void)` — **three** MPI calls (ISP/AE/AWB) but **no `IspDev`** arg in any of them; CV100 has only one ISP. From the mpp2 flavour SDK. |
| **T2 / V2** | `hi3516cv200`, `hi3516av100`, `hi3516cv300` | `int sensor_register_callback(void)` — **three** MPI calls (ISP/AE/AWB), `IspDev` is a local `0`, internal `ALG_LIB_S stLib` |
| **T3 / V3A** | `hi3519v101`, `hi3516av200`† | `static int sensor_register_callback(ISP_DEV IspDev, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)` — caller passes lib handles, first tier to export `ISP_SNS_OBJ_S stSns<X>Obj` |
| **T4 / V4-HI** | `hi3516cv500` | `static HI_S32 sensor_register_callback(VI_PIPE vi_pipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)` — `VI_PIPE` not `ISP_DEV`, `ISP_SNS_ATTR_INFO_S` carries the sensor ID, ISP params split into `<sensor>_cmos_ex.h` |
| **T5 / V4-GK** | `hi3516ev200`, `gk7205v200` | T4 shape but `GK_API_*` instead of `HI_MPI_*`, `GK_S32` types, `comm_sns.h`/`sns_ctrl.h` (no `hi_` prefix); `hicompat.h` aliases `GK_*` ↔ `HI_*` at compile time |

Verified anchor points (one driver per tier):

- T1+ — `libraries/sensor/hi3516cv100/sony_imx236/imx236_cmos.c:902`
- T1+ — `libraries/sensor/hi3516cv100/soi_jxh42/jxh42_cmos.c:507`
- T2 — `libraries/sensor/hi3516cv200/aptina_9m034/m034_cmos.c:1335`
- T2 — `libraries/sensor/hi3516cv300/sony_imx290/imx290_cmos.c:2030`
- T2 — `libraries/sensor/hi3516av100/aptina_ar0230/ar0230_cmos.c:1329`
- T3 — `libraries/sensor/hi3519v101/sony_imx226/imx226_cmos.c:1458` (`stSnsImx226Obj` at `:1533`)
- T3 — `libraries/sensor/hi3519v101/sony_imx327/imx327_cmos.c:1874`
- T4 — `libraries/sensor/hi3516cv500/sony_imx307/imx307_cmos.c:1238` (`stSnsImx307Obj` at `:1348`)
- T5 — `libraries/sensor/hi3516ev200/galaxycore_gc2053/gc2053_cmos.c:1120` (`stSnsGc2053Obj` at `:1236`)
- T5 — `libraries/sensor/gk7205v200/galaxycore_gc2053/gc2053_cmos.c:1120` (identical to EV200)

Note: the legacy T1 (single-call, no AE/AWB) form lives only in the older mpp/ SDK flavour; this repo's `hi3516cv100/` was upgraded to mpp2 (T1+) so every sensor under it now uses the three-call shape — there are no T1 anchors in-repo to cite. See §3.1 for the mpp/mpp2 split.

† **`hi3516av200`** is a sibling chip within the V3A generation; per the
README's "Supported hardware" table, both `hi3519v101` and `hi3516av200`
silicon are built via `CHIPARCH=hi3519v101` (same kernel-side machinery —
`mipi_rx`, `sys_config`, OSAL — and same MPI surface). When OpenIPC's
`sensors/hisilicon/` names a driver `<sensor>_hi3516av200_rev1`, that
source belongs under `libraries/sensor/hi3519v101/` and the resulting
`.so` runs on both chips. Seven of those drivers exist upstream; one
(IMX327) has been ported into `libraries/sensor/hi3519v101/sony_imx327/`.

## 2. Identification flowchart

Open the unknown `*_cmos.c`, jump to `sensor_register_callback`, run this in 30 seconds:

```
Q1. Does sensor_register_callback take any arguments?

  No (signature is `(void)`)
    │
    ├── Q2. Does the body call HI_MPI_AE_SensorRegCallBack?
    │       No  → T1 (CV100)
    │       Yes → T2 (CV200 / AV100 / CV300)
    │             distinguish via Makefile -I path or includes:
    │               kernel/include/hi3516cv200  → CV200
    │               kernel/include/hi3516av100  → AV100
    │               kernel/include/hi3516cv300  → CV300
    │
  Yes — but parameter types are typedefs like `pRegSnsRegCb`, `pAeSnsRegCb`?
    │   → wrapped driver — re-run this flowchart against the *indirect call
    │     site* inside the body, not the outer signature, see §7.1
    │
  Yes (parameter types are concrete: ISP_DEV / VI_PIPE / ot_vi_pipe)
    │
    ├── Q3. First parameter type?
    │       ISP_DEV     → T3 (V101)
    │       VI_PIPE     → Q4
    │       ot_vi_pipe  → T6 (post-2022 Shenshu SDK), see §7.2
    │
    └── Q4. Are the MPI symbols HI_MPI_* or GK_API_*?
            HI_MPI_*   → T4 (CV500)
            GK_API_*   → T5 (EV200 / GK7205V200)
                         distinguish via Makefile / runtime SDK_CODE
                         (the source is identical between EV200 and
                          GK7205V200 — they share the same chip die)
```

Secondary fingerprints, in case the registration function is missing or stripped:

| If you see... | It's probably... |
|---|---|
| `sensor_register_callback(pRegSnsRegCb, pAeSnsRegCb, pAwbSnsRegCb, pSnsRegsCfg)` (typedef-pointer params) | wrapped driver — see §7.1 to identify the underlying tier |
| `#include "hi_comm_isp.h"` *and not* `mpi_ae.h` | T1 (historical, not in this repo) |
| `mpi_isp.h` + `mpi_ae.h` + `mpi_awb.h` + `#define <X>_ID <num>` + `ISP_DEV IspDev = 0;` in `sensor_register_callback` | T2 |
| `g_pastImx226[ISP_MAX_DEV_NUM]` (or any state array indexed by `ISP_MAX_DEV_NUM`) | T3 |
| `g_pastImx307[ISP_MAX_PIPE_NUM]` + `ISP_SNS_ATTR_INFO_S stSnsAttrInfo` + `ISP_SNS_OBJ_S stSns<X>Obj` | T4 |
| `#include "comm_sns.h"`, `#include "gk_api_isp.h"`, `#include "hicompat.h"`, `GK_S32`, `GK_SUCCESS`, `ISP_TRACE(MODULE_DBG_ERR, ...)` | T5 |
| `#include "ot_mpi_isp.h"`, `td_s32`, `ot_vi_pipe`, `ot_isp_sns_obj g_sns_<x>_obj`, `pfn_register_callback` (snake_case) | T6 (§7.2) |

## 3. Per-tier details

### 3.1 T1 / T1+ — CV100

CV100 has two userspace API surfaces in the wild — old (T1, mpp/) and new
(T1+, mpp2/). Both run on the same kernel modules. **This repo's
`hi3516cv100/` is the T1+ (mpp2) flavour**; the older T1 form lives only in
historical SDK trees and isn't built here.

#### T1 — historical mpp/ surface (not in this repo)

Single-call ISP-only registration. No `ALG_LIB_S`, no `ISP_SNS_OBJ_S`, no
AE/AWB MPI surface — exposure math and AE/AWB tables live inline as static
structs in `*_cmos.c`.

Header set:

```c
#include "hi_comm_sns.h"
#include "hi_comm_isp.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"
```

Registration:

```c
int sensor_register_callback(void)
{
    int ret;
    ret = HI_MPI_ISP_SensorRegCallBack(&stSensorExpFuncs);
    /* ... */
}
```

Where it lives: HiSilicon Hi3518 SDK V1.0.B.0's `mpp/component/isp/sensor/`
tree. Not built in this repo (the OpenIPC firmware that consumes this repo
runs on the T1+ MPI runtime, so building T1 sensors here would produce
ABI-incompatible artifacts).

#### T1+ — mpp2 surface (this repo's hi3516cv100)

Three-call ISP/AE/AWB registration — but **no `IspDev`** argument in any
of them, because CV100 has only one ISP. Function signature stays
`int sensor_register_callback(void)`; the body looks like T2 with `IspDev`
removed.

Header set (in `kernel/include/hi3516cv100/`):

```c
#include "hi_comm_sns.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"
#include "mpi_ae.h"      /* T1+ adds these three */
#include "mpi_awb.h"
#include "mpi_af.h"
```

Registration body (anchor: `libraries/sensor/hi3516cv100/sony_imx236/imx236_cmos.c:902`):

```c
int sensor_register_callback(void)
{
    HI_S32 s32Ret;
    ALG_LIB_S stLib;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S  stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;
    /* no IspDev declared anywhere */

    cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret = HI_MPI_ISP_SensorRegCallBack(IMX236_ID, &stIspRegister);     /* 2 args */

    stLib.s32Id = 0;
    strcpy(stLib.acLibName, HI_AE_LIB_NAME);
    cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret = HI_MPI_AE_SensorRegCallBack(&stLib, IMX236_ID, &stAeRegister);   /* 3 args */

    stLib.s32Id = 0;
    strcpy(stLib.acLibName, HI_AWB_LIB_NAME);
    cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    s32Ret = HI_MPI_AWB_SensorRegCallBack(&stLib, IMX236_ID, &stAwbRegister); /* 3 args */
    return 0;
}
```

A second in-repo anchor showing the same shape:
`libraries/sensor/hi3516cv100/soi_jxh42/jxh42_cmos.c:507`.

#### Why the surfaces are genuinely different

The MPI library prototype itself changed between mpp and mpp2 — this isn't
a sensor-side convention difference, it's an ABI break:

```c
/* mpp/include/mpi_isp.h (T1):    one arg, opaque function-pointer struct */
HI_S32 HI_MPI_ISP_SensorRegCallBack(SENSOR_EXP_FUNC_S *pstSensorExpFuncs);

/* kernel/include/hi3516cv100/mpi_isp.h (T1+, mpp2): two args, sensor ID added */
HI_S32 HI_MPI_ISP_SensorRegCallBack(SENSOR_ID SensorId,
                                    ISP_SENSOR_REGISTER_S *pstRegister);
HI_S32 HI_MPI_ISP_SensorUnRegCallBack(SENSOR_ID SensorId);

/* kernel/include/hi3516cv100/mpi_ae.h (T1+, mpp2): 3 args, no IspDev — same in mpi_awb.h */
HI_S32 HI_MPI_AE_SensorRegCallBack(ALG_LIB_S *pstAeLib, SENSOR_ID SensorId,
                                   AE_SENSOR_REGISTER_S *pstRegister);
HI_S32 HI_MPI_AE_SensorUnRegCallBack(ALG_LIB_S *pstAeLib, SENSOR_ID SensorId);
```

`mpp2/include/` adds eight headers absent from `mpp/include/`:
`hi_ae_comm.h`, `hi_af_comm.h`, `hi_awb_comm.h`, `hi_comm_3a.h`,
`hi_vreg.h`, `mpi_ae.h`, `mpi_af.h`, `mpi_awb.h` — these now live
under this repo's `kernel/include/hi3516cv100/` after the mpp2 upgrade.

The `ko/` directory is **identical** between mpp and mpp2 — the T1↔T1+
swap is purely userspace MPP, not a kernel ABI change.

#### Telling T1+ from T2 in found-online source

Both have function signature `int sensor_register_callback(void)` and three
MPI calls. Disambiguate by checking the first arg of
`HI_MPI_ISP_SensorRegCallBack`: T2 has `IspDev`, T1+ has the sensor ID
directly. Equivalently: T2 includes `hi_comm_video.h` and uses `ISP_DEV`
types; T1+ has the new `hi_comm_3a.h` / `hi_ae_comm.h` available but no
`ISP_DEV` references.

### 3.2 T2 — CV200 / AV100 / CV300

Three-call ISP/AE/AWB registration, but the function still takes no arguments.
`IspDev` is hardcoded to `0` and the `ALG_LIB_S` is filled in locally with
`HI_AE_LIB_NAME` / `HI_AWB_LIB_NAME`. Each MPI call takes an explicit sensor ID
enum (e.g. `M034_ID = 9034`, `IMX290_ID = 290`).

Headers (`m034_cmos.c:4–13`):

```c
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hi_comm_sns.h"
#include "hi_comm_video.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"
#include "mpi_af.h"
```

Registration (`m034_cmos.c:1335–1373`, abbreviated):

```c
int sensor_register_callback(void)
{
    ISP_DEV IspDev = 0;
    HI_S32 s32Ret;
    ALG_LIB_S stLib;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S  stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;

    cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret = HI_MPI_ISP_SensorRegCallBack(IspDev, M034_ID, &stIspRegister);
    /* ... */

    stLib.s32Id = 0;
    strncpy(stLib.acLibName, HI_AE_LIB_NAME, sizeof(HI_AE_LIB_NAME));
    cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret = HI_MPI_AE_SensorRegCallBack(IspDev, &stLib, M034_ID, &stAeRegister);
    /* ... */

    stLib.s32Id = 0;
    strncpy(stLib.acLibName, HI_AWB_LIB_NAME, sizeof(HI_AWB_LIB_NAME));
    cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    s32Ret = HI_MPI_AWB_SensorRegCallBack(IspDev, &stLib, M034_ID, &stAwbRegister);
    /* ... */
    return 0;
}
```

Makefile distinction from T1 — adds the `kernel/isp/arch/.../include` path:

```makefile
override CFLAGS += -fPIC \
    -I$(CURDIR)/../../../../kernel/include/hi3516cv200 \
    -I$(CURDIR)/../../../../kernel/isp/arch/hi3516cv200/include
```

T2 sub-distinction — CV200 vs AV100 vs CV300 share the same **MPI registration
ABI** (function signature, MPI symbol arity, `ALG_LIB_S` boilerplate are
identical), confirmed by reading
`hi3516av100/aptina_ar0230/ar0230_cmos.c:1329` and
`hi3516cv300/sony_imx290/imx290_cmos.c:2030`. Within that registration ABI,
T2 is genuinely portable.

#### Caveat — ISP tuning data is *not* portable within T2

Sensor drivers don't only register MPI callbacks — they also populate ISP
tuning structs (`ISP_CMOS_DEFAULT_S` and its sub-structs:
`ISP_CMOS_DEMOSAIC_S`, `ISP_CMOS_GE_S`, `ISP_CMOS_DRC_S`,
`AWB_SENSOR_DEFAULT_S`, etc.) that get memcpy'd into the host's libisp at
`cmos_get_isp_default()` time. **Those structs were rewritten between V2
and V3** — same names, completely different field sets. A T2 sensor source
written against V3 (CV300) headers will not correctly populate V2 (CV200)
ISP defaults even though it links and registers fine.

A first-hand experiment ports `hi3516cv300/jx_f22/` to `hi3516cv200/`
(JXH-22 sensor target: `hi3518ev200` running OpenIPC firmware). Three
kinds of drift had to be handled, in increasing severity:

**1. New types V3 added that V2 doesn't have at all.** Wrap with
`#if defined(hi3516cv300)`:
- The four V3-only ISP-feature struct types: `ISP_CMOS_FCR_S`
  (Frame Color Reproduction), `ISP_CMOS_YUV_SHARPEN_S` (V3 has YUV
  sharpening, V2 has only RGB), `ISP_CMOS_CA_S` (Chromatic Aberration
  correction), `ISP_CMOS_BAYERNR_S` (V3 has Bayer NR, V2 has only UVNR),
  plus their initializers and `cmos_get_isp_default()` `memcpy` calls.
- AWB extension fields V3 added to `AWB_SENSOR_DEFAULT_S`:
  `au32BlcOffset[]`, `u16Golden{R,B}gain`, `u16Sample{R,B}gain`, plus
  the `g_au16Sample{R,B}gain[]` static globals that feed them and the
  `sensor_set_init()` function that updates them.
- LSC table types `ISP_LSC_CABLI_UNI_TABLE_S` /
  `ISP_LSC_CABLI_TABLE_S` (V3-only).

**2. Same struct names, completely different field sets.** Wrap each
struct's *initializer* and the function bodies that consume it with
`#if defined(hi3516cv300) / #else / #endif`, providing the V2 layout's
fields in the `#else` branch. Affected structs:

  | Struct | V2 fields | V3 fields |
  |---|---|---|
  | `ISP_CMOS_DEMOSAIC_S` | `u8VhLimit`, `u8VhOffset`, `u16VhSlope`, `bFcrEnable`, `au8FcrStrength[]`, `au8FcrThreshold[]`, `u16UuSlope`, `au16NpOffset[]` (Vh / false-color / AHD model) | `EdgeSmoothThr[]`, `EdgeSmoothSlope[]`, `AntiAliasThr[]`, `AntiAliasSlope[]`, `NrCoarseStr[]`, `NoiseSuppressStr[]`, `DetailEnhanceStr[]`, `SharpenLumaStr[]`, `ColorNoiseCtrlThr[]` (denoising model) |
  | `ISP_CMOS_GE_S` | scalar `u16Threshold`, `u8Sensitivity`, `u16SensiThreshold`, no `au16NpOffset[]` | array `au16Threshold[]`, renamed `u8SensiSlope` / `u16SensiThr`, plus `au16NpOffset[]` |
  | `ISP_CMOS_DRC_S` | `u16DarkGainLmtY/C`, `u16BrightGainLmt`, `au16ColorCorrectionLut[]` | dropped those, added `u8Compress`, `u8PDStrength`, `u8LocalMixingBrigtht/Dark`, `bOpType`, `u8ManualStrength`, `u8AutoStrength`; renamed LUT to `ColorCorrectionLut[33]` |

Same identifier, **different concept** — V2 demosaic is about
edge/false-color tuning, V3 demosaic is about denoising. There's no
set of values that simultaneously feeds both correctly. The V2 init
data has to be carried in the `#else` branch from a V2-era source
(e.g. OpenIPC's `glutinium/hi35xx_sensor_jxf22`); it cannot be
synthesised from V3 values.

**3. Same function names, different bodies.** `cmos_get_ae_default`,
`cmos_fps_set`, `cmos_slow_framerate_set`, `cmos_inttime_update`,
`cmos_again_calc_table`, `cmos_gains_update`, `cmos_get_awb_default`,
`cmos_get_isp_default`, `cmos_set_pixel_detect`, `cmos_set_wdr_mode`,
`cmos_get_sns_regs_info`, `cmos_set_image_mode`, `sensor_global_init`,
`sensor_init`, and `sensor_linear_1080p30_init` all need V3 and V2
implementations under `#if/#else`. Differences include opposite
byte-orders in `g_stSnsRegsInfo.astI2cData[]` (V3:
`[3]=VMAX_low/[4]=VMAX_high`; V2: opposite), different I²C ioctl
patterns (V3 uses `I2C_SLAVE_FORCE` with the slave address right-shifted
by 1; V2 takes the 8-bit form directly and uses `I2C_16BIT_REG` /
`I2C_16BIT_DATA` per-write), different mode-set dispatch logic, and
different sensor register-init sequences (V2 and V3 program different
PLL/output-pin values).

**A subtle gotcha — symbol visibility.** The V2 source has
`cmos_inttime_update` declared `static` (formatted oddly as `*/static`
on the same line as a comment-end). When transcribing into a merged
file, it's easy to drop the `static` keyword. The resulting non-static
symbol gets exported by the dynamic loader and the host streamer's
sensor function-pointer table — populated through
`cmos_init_ae_exp_function` — gets shadowed at runtime by an
unintended dlsym lookup on the bare symbol name. The streamer then
exhibits venc timeouts because the i2c register byte-order written by
the V3 logic doesn't match what V2's libisp expects to read back. Same
function name, same lookup, wrong byte order. Compare
`nm -D libsns_<x>.so` between your merged build and a known-good
single-platform build: any extra `T` entry that's missing from the
known-good build is suspect.

**Result of a correctly-merged port.** The cv200 build of the
fully-gated source produces a stripped 18 KB ARMv5 EABI5 soft-float
`.so` whose md5 is **byte-identical** to the build of the standalone
V2 source. Deploying it to the hi3518ev200 camera, restarting majestic,
and watching for two CV200 surface gotchas (no `Timeout from venc
channel 0` in the log; correct sensor-init banner from the V2 register
sequence) confirms the port is functional. The cv300 build of the
same source still compiles to a 49 KB binary identical in size to
the original cv300-only build — V3 features are undisturbed.

#### Practical takeaway

For T2, "the source is portable across cv200 / av100 / cv300" is true at
the **registration ABI** layer and false at the **ISP feature ABI** layer.
The sub-tier boundary is V2 vs V3 (CV200 + AV100 ≈ V2; CV300 ≈ V3 — AV100
is a Cortex-A7 die of the V2 SDK, hence still V2-flavoured). When porting
within T2, audit your destination's `kernel/include/<chiparch>/hi_comm_sns.h`
ISP_CMOS_*_S definitions before assuming source compatibility. If they
differ structurally, start from a sensor source written against the
**target's own SDK era**, not from an "equivalent" T2 source on a
different generation.

#### How the JXF22 dual-platform port is laid out in this repo

The merged sensor source lives at
`libraries/sensor/hi3516cv300/jx_f22/{jxf22_cmos.c,jxf22_sensor_ctl.c}`.
`libraries/sensor/hi3516cv200/soi_jxf22/` carries only a Makefile that
references the cv300 source via `vpath` and passes `-Dhi3516cv200`:

```makefile
SRC_DIR := $(CURDIR)/../../hi3516cv300/jx_f22
override CFLAGS += -fPIC -Dhi3516cv200 \
    -I$(CURDIR)/../../../../kernel/include/hi3516cv200 \
    -I$(CURDIR)/../../../../kernel/isp/arch/hi3516cv200/include
SRCS := jxf22_cmos.c jxf22_sensor_ctl.c
vpath %.c $(SRC_DIR)
```

The cv300 Makefile passes `-Dhi3516cv300` and uses its own dir as
the source location. The build system's `find … -name Makefile`
(see `libraries/Makefile`) picks up both directories automatically;
no enumeration to update.

V2-era tuning data for the `#else` branch came from OpenIPC's
[glutinium](https://github.com/OpenIPC/glutinium)
`hi35xx_sensor_jxf22` package (built against `hisi-osdrv2`) — the
canonical V2 source for this sensor. It supplies V2's
`ISP_CMOS_DEMOSAIC_S` (Vh / false-color / AHD), V2's `ISP_CMOS_GE_S`
(scalar `u16Threshold` / `u8Sensitivity`), `ISP_CMOS_RGBSHARPEN_S`,
`ISP_CMOS_UVNR_S`, the V2 sensor i²c programming sequence, and the V2
register byte-order in `g_stSnsRegsInfo.astI2cData[]`.

#### Lesson, restated for emphasis

Within T2, sharing a single sensor source between cv200 and cv300 is
mechanical work but it's not small — most function bodies need
`#if/#else` walls. **You also need a V2-era source for the `#else`
branch**: there's no way to derive V2 ISP tuning from V3 tuning, since
the underlying ISP feature set is different. Line count alone is
misleading: a "smaller / older" upstream driver (like glutinium's V2
sensor sources) is the V2-correct one — it didn't shrink, the V3
version grew V3-only data.

### 3.3 T3 — V101

First tier where the caller (the application) passes `ALG_LIB_S` handles in.
The pipe abstraction is still `ISP_DEV` (range 0–1 per chip). Multi-sensor state
arrays are indexed by `ISP_MAX_DEV_NUM`. First tier to **export an
`ISP_SNS_OBJ_S` symbol** — this is how libisp dynamically dispatches to
sensor-specific callbacks instead of statically linking them.

Registration (`imx226_cmos.c:1458–1490`):

```c
static int sensor_register_callback(ISP_DEV IspDev,
                                    ALG_LIB_S *pstAeLib,
                                    ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S  stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;

    cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret = HI_MPI_ISP_SensorRegCallBack(IspDev, IMX226_ID, &stIspRegister);
    /* ... */
    cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret = HI_MPI_AE_SensorRegCallBack(IspDev, pstAeLib, IMX226_ID, &stAeRegister);
    /* ... */
    cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    s32Ret = HI_MPI_AWB_SensorRegCallBack(IspDev, pstAwbLib, IMX226_ID, &stAwbRegister);
    return 0;
}
```

Exported object (`imx226_cmos.c:1533–1543`):

```c
ISP_SNS_OBJ_S stSnsImx226Obj =
{
    .pfnRegisterCallback    = sensor_register_callback,
    .pfnUnRegisterCallback  = sensor_unregister_callback,
    .pfnStandby             = imx226_standby,
    .pfnRestart             = imx226_restart,
    .pfnWriteReg            = imx226_write_register,
    .pfnReadReg             = imx226_read_register,
    .pfnSetBusInfo          = imx226_set_bus_info,
    .pfnSetInit             = sensor_set_init
};
```

### 3.4 T4 — CV500

Pipe abstraction is now `VI_PIPE` (one Video-In pipeline per sensor instance).
The MPI signature changes again: instead of `(IspDev, SENSOR_ID, &reg)`, you pass
`(vi_pipe, &stSnsAttrInfo, &reg)` — sensor ID is now a field on the attr-info
struct. State arrays grow to `[ISP_MAX_PIPE_NUM]`. ISP parameter tables (LCAC,
DPC, demosaic, GE, CCM, gamma) move from inline `*_cmos.c` statics into a new
header `<sensor>_cmos_ex.h` that's `#include`d at the top.

Headers — note the `hi_comm_*` includes have been **removed**, replaced by local
`<sensor>_cmos.h` and `<sensor>_cmos_ex.h` (`imx307_cmos.c:8–15`):

```c
#include "imx307_cmos.h"
#include "imx307_cmos_ex.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mpi_isp.h"
#include "mpi_ae.h"
#include "mpi_awb.h"
```

Registration (`imx307_cmos.c:1238–1288`, abbreviated):

```c
static HI_S32 sensor_register_callback(VI_PIPE vi_pipe,
                                       ALG_LIB_S *pstAeLib,
                                       ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S  stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;
    ISP_SNS_ATTR_INFO_S   stSnsAttrInfo;

    CMOS_CHECK_POINTER(pstAeLib);
    CMOS_CHECK_POINTER(pstAwbLib);

    s32Ret = sensor_ctx_init(vi_pipe);
    if (s32Ret != HI_SUCCESS) return HI_FAILURE;

    stSnsAttrInfo.eSensorId = IMX307_ID;
    cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret = HI_MPI_ISP_SensorRegCallBack(vi_pipe, &stSnsAttrInfo, &stIspRegister);
    /* ... */
    cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret = HI_MPI_AE_SensorRegCallBack(vi_pipe, pstAeLib, &stSnsAttrInfo, &stAeRegister);
    /* ... */
    cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    s32Ret = HI_MPI_AWB_SensorRegCallBack(vi_pipe, pstAwbLib, &stSnsAttrInfo, &stAwbRegister);
    return HI_SUCCESS;
}
```

Per-pipe state — single static now an array indexed by `vi_pipe`
(`imx307_cmos.c:34`):

```c
ISP_SNS_STATE_S *g_pastImx307[ISP_MAX_PIPE_NUM] = {HI_NULL};
```

Makefile — different include flavor: includes from `libraries/isp/include/.../3a`
**and** `kernel/include/.../adapt`:

```makefile
LIB_NAME := libsns_imx307

ISP_DIR := $(CURDIR)/../../../isp
KO_DIR  := $(CURDIR)/../../../../kernel

override CFLAGS += -fPIC -Wall -O2 \
    -I$(ISP_DIR)/include/hi3516cv500/3a \
    -I$(ISP_DIR)/include/hi3516cv500 \
    -I$(KO_DIR)/include/hi3516cv500 \
    -I$(KO_DIR)/include/hi3516cv500/adapt \
    -I.
```

### 3.5 T5 — EV200 / GK7205V200

Structurally identical to T4. The only differences are: type prefix
(`HI_*` → `GK_*`), MPI symbol prefix (`HI_MPI_*` → `GK_API_*`), and header
namespace (`hi_comm_sns.h` / `hi_sns_ctrl.h` → `comm_sns.h` / `sns_ctrl.h`).
Everything else — pipe model, `ISP_SNS_ATTR_INFO_S`, `ISP_SNS_OBJ_S`, the cmos /
cmos_ex split — carries over unchanged.

Headers (`gc2053_cmos.c:5–15`):

```c
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "comm_sns.h"        /* no hi_ prefix */
#include "comm_video.h"
#include "sns_ctrl.h"
#include "gk_api_isp.h"      /* gk_api_ not mpi_ */
#include "gk_api_ae.h"
#include "gk_api_awb.h"
#include "gc2053_cmos_ex.h"
#include "hicompat.h"        /* shim — see §6 */
```

Registration (`hi3516ev200/galaxycore_gc2053/gc2053_cmos.c:1120–1173`):

```c
static GK_S32 sensor_register_callback(VI_PIPE ViPipe,
                                       ALG_LIB_S *pstAeLib,
                                       ALG_LIB_S *pstAwbLib)
{
    GK_S32 s32Ret;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S  stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;
    ISP_SNS_ATTR_INFO_S   stSnsAttrInfo;

    CMOS_CHECK_POINTER(pstAeLib);
    CMOS_CHECK_POINTER(pstAwbLib);

    s32Ret = sensor_ctx_init(ViPipe);
    if (s32Ret != GK_SUCCESS) return GK_FAILURE;

    stSnsAttrInfo.eSensorId = GC2053_ID;

    s32Ret  = cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret |= GK_API_ISP_SensorRegCallBack(ViPipe, &stSnsAttrInfo, &stIspRegister);
    /* ... */
    s32Ret  = cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
    s32Ret |= GK_API_AE_SensorRegCallBack(ViPipe, pstAeLib, &stSnsAttrInfo, &stAeRegister);
    /* ... */
    s32Ret  = cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
    s32Ret |= GK_API_AWB_SensorRegCallBack(ViPipe, pstAwbLib, &stSnsAttrInfo, &stAwbRegister);
    return GK_SUCCESS;
}
```

Makefile — completely different shape, no per-platform `-I` path; instead a
single unified include and the `SDK_CODE` macro that drives `hicompat.h`:

```makefile
override CFLAGS += -DSDK_CODE=$(SDK_CODE) -fPIC \
    -I$(CURDIR)/../../../../include
```

EV200 and GK7205V200 are pin-compatible silicon (see CLAUDE.md "Goke/HiSilicon
equivalence") and the sensor source is byte-for-byte identical between the two
trees — `gc2053_cmos.c` differs only in the directory it lives in.

## 4. Porting recipes — side-by-side diffs

The three recipes below cover every realistic transition. T2 ↔ T3 is rare in
practice (V101 is a niche dual-ISP chip); for `T1 → anything past T2` go via T2
first; for `T3 → T4` apply T2→T4 with the `ISP_DEV → VI_PIPE` substitution.

### 4.1 T1 → T2 (CV100 → CV200/AV100/CV300)

Cheap port. Add AE/AWB headers, expand the single MPI call into three, declare
`ALG_LIB_S` locally, pick a sensor ID enum.

```diff
  #include <stdio.h>
  #include <string.h>
  #include <assert.h>
  #include "hi_comm_sns.h"
- #include "hi_comm_isp.h"
+ #include "hi_comm_video.h"
  #include "hi_sns_ctrl.h"
  #include "mpi_isp.h"
+ #include "mpi_ae.h"
+ #include "mpi_awb.h"

+ #define IMX236_ID 236

  int sensor_register_callback(void)
  {
-     int ret;
-     ret = HI_MPI_ISP_SensorRegCallBack(&stSensorExpFuncs);
-     if (ret) {
-         printf("sensor register callback function failed!\n");
-         return ret;
-     }
-     return 0;
+     ISP_DEV IspDev = 0;
+     HI_S32 s32Ret;
+     ALG_LIB_S stLib;
+     ISP_SENSOR_REGISTER_S stIspRegister;
+     AE_SENSOR_REGISTER_S  stAeRegister;
+     AWB_SENSOR_REGISTER_S stAwbRegister;
+
+     cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
+     s32Ret = HI_MPI_ISP_SensorRegCallBack(IspDev, IMX236_ID, &stIspRegister);
+     if (s32Ret) return s32Ret;
+
+     stLib.s32Id = 0;
+     strncpy(stLib.acLibName, HI_AE_LIB_NAME, sizeof(HI_AE_LIB_NAME));
+     cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
+     s32Ret = HI_MPI_AE_SensorRegCallBack(IspDev, &stLib, IMX236_ID, &stAeRegister);
+     if (s32Ret) return s32Ret;
+
+     stLib.s32Id = 0;
+     strncpy(stLib.acLibName, HI_AWB_LIB_NAME, sizeof(HI_AWB_LIB_NAME));
+     cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
+     s32Ret = HI_MPI_AWB_SensorRegCallBack(IspDev, &stLib, IMX236_ID, &stAwbRegister);
+     return s32Ret;
  }
```

Makefile:

```diff
  override CFLAGS += -fPIC \
-     -I$(CURDIR)/../../../../kernel/include/hi3516cv100
+     -I$(CURDIR)/../../../../kernel/include/hi3516cv200 \
+     -I$(CURDIR)/../../../../kernel/isp/arch/hi3516cv200/include
```

You also need to provide AE/AWB callback initializers (`cmos_init_ae_exp_function`,
`cmos_init_awb_exp_function`). On T1 those don't exist — split out the relevant
function-pointer fields from your existing `stSensorExpFuncs` struct or copy
the AE/AWB initializers from any T2 sensor as a template.

### 4.2 T2 → T4 (CV200/CV300 → CV500) — the expensive port

The real refactor. `imx290` exists in T2 (CV300) and `imx307` (the same Sony
sensor family) exists in T4 (CV500), so the diff below is what an actual port
looks like.

Function signature:

```diff
- int sensor_register_callback(void)
+ static HI_S32 sensor_register_callback(VI_PIPE vi_pipe,
+                                        ALG_LIB_S *pstAeLib,
+                                        ALG_LIB_S *pstAwbLib)
  {
-     ISP_DEV IspDev = 0;
      HI_S32 s32Ret;
-     ALG_LIB_S stLib;
      ISP_SENSOR_REGISTER_S stIspRegister;
      AE_SENSOR_REGISTER_S  stAeRegister;
      AWB_SENSOR_REGISTER_S stAwbRegister;
+     ISP_SNS_ATTR_INFO_S   stSnsAttrInfo;
+
+     CMOS_CHECK_POINTER(pstAeLib);
+     CMOS_CHECK_POINTER(pstAwbLib);
+
+     s32Ret = sensor_ctx_init(vi_pipe);
+     if (s32Ret != HI_SUCCESS) return HI_FAILURE;
+
+     stSnsAttrInfo.eSensorId = IMX307_ID;
```

ISP MPI call:

```diff
      cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
-     s32Ret = HI_MPI_ISP_SensorRegCallBack(IspDev, IMX290_ID, &stIspRegister);
+     s32Ret = HI_MPI_ISP_SensorRegCallBack(vi_pipe, &stSnsAttrInfo, &stIspRegister);
```

AE / AWB MPI calls:

```diff
-     stLib.s32Id = 0;
-     strncpy(stLib.acLibName, HI_AE_LIB_NAME, sizeof(HI_AE_LIB_NAME));
      cmos_init_ae_exp_function(&stAeRegister.stSnsExp);
-     s32Ret = HI_MPI_AE_SensorRegCallBack(IspDev, &stLib, IMX290_ID, &stAeRegister);
+     s32Ret = HI_MPI_AE_SensorRegCallBack(vi_pipe, pstAeLib, &stSnsAttrInfo, &stAeRegister);
      ...
-     stLib.s32Id = 0;
-     strncpy(stLib.acLibName, HI_AWB_LIB_NAME, sizeof(HI_AWB_LIB_NAME));
      cmos_init_awb_exp_function(&stAwbRegister.stSnsExp);
-     s32Ret = HI_MPI_AWB_SensorRegCallBack(IspDev, &stLib, IMX290_ID, &stAwbRegister);
+     s32Ret = HI_MPI_AWB_SensorRegCallBack(vi_pipe, pstAwbLib, &stSnsAttrInfo, &stAwbRegister);
```

State arrays — every per-sensor scalar becomes an array indexed by `vi_pipe`:

```diff
- static ISP_SNS_STATE_S g_stSnsState;
+ ISP_SNS_STATE_S *g_pastImx307[ISP_MAX_PIPE_NUM] = {HI_NULL};
+ #define IMX307_SENSOR_SET_CTX(dev, pstCtx) ((g_pastImx307[dev]) = (pstCtx))
+ #define IMX307_SENSOR_RESET_CTX(dev)       (g_pastImx307[dev] = HI_NULL)
```

Every cmos function gains a leading `VI_PIPE vi_pipe` parameter. Internally, replace
`g_stSnsState.<field>` with `g_pastImx307[vi_pipe]-><field>`. Add `_get_ctx()`,
`_get_bus_Info()`, and `_set_bus_info()` helpers.

Headers — drop `hi_comm_sns.h`/`hi_comm_video.h`/`hi_sns_ctrl.h`/`mpi_af.h`,
add a local `<sensor>_cmos.h` and `<sensor>_cmos_ex.h`:

```diff
- #include "hi_comm_sns.h"
- #include "hi_comm_video.h"
- #include "hi_sns_ctrl.h"
  #include "mpi_isp.h"
  #include "mpi_ae.h"
  #include "mpi_awb.h"
- #include "mpi_af.h"
+ #include "imx307_cmos.h"
+ #include "imx307_cmos_ex.h"
```

ISP parameter tables (LCAC, DPC, demosaic, GE, CCM, gamma) move from inline
`static const ISP_CMOS_*_S g_stIsp* = { ... }` definitions in `*_cmos.c` into the
new `<sensor>_cmos_ex.h`. Verify against `imx307_cmos_ex.h:19–25` for the
canonical shape.

Add the exported sensor object at the bottom of the file:

```diff
+ ISP_SNS_OBJ_S stSnsImx307Obj = {
+     .pfnRegisterCallback    = sensor_register_callback,
+     .pfnUnRegisterCallback  = sensor_unregister_callback,
+     .pfnStandby             = imx307_standby,
+     .pfnRestart             = imx307_restart,
+     .pfnMirrorFlip          = HI_NULL,
+     .pfnWriteReg            = imx307_write_register,
+     .pfnReadReg             = imx307_read_register,
+     .pfnSetBusInfo          = imx307_set_bus_info,
+     .pfnSetInit             = sensor_set_init
+ };
```

Makefile:

```diff
- override CFLAGS += -fPIC \
-     -I$(CURDIR)/../../../../kernel/include/hi3516cv300 \
-     -I$(CURDIR)/../../../../kernel/isp/arch/hi3516cv300/include
+ ISP_DIR := $(CURDIR)/../../../isp
+ KO_DIR  := $(CURDIR)/../../../../kernel
+ override CFLAGS += -fPIC -Wall -O2 \
+     -I$(ISP_DIR)/include/hi3516cv500/3a \
+     -I$(ISP_DIR)/include/hi3516cv500 \
+     -I$(KO_DIR)/include/hi3516cv500 \
+     -I$(KO_DIR)/include/hi3516cv500/adapt \
+     -I.
```

### 4.3 T4 → T5 (CV500 → EV200 / GK7205V200) — the cheap port

Mechanical, no logic changes. `gc2053_cmos.c` exists in *both* CV500 and EV200,
so the diff is exact.

Headers:

```diff
- #include "gc2053_cmos.h"
- #include "gc2053_cmos_ex.h"
- #include <stdio.h>
+ #include <stdio.h>
  #include <string.h>
  #include <assert.h>
- #include "mpi_isp.h"
- #include "mpi_ae.h"
- #include "mpi_awb.h"
+ #include "comm_sns.h"
+ #include "comm_video.h"
+ #include "sns_ctrl.h"
+ #include "gk_api_isp.h"
+ #include "gk_api_ae.h"
+ #include "gk_api_awb.h"
+ #include "gc2053_cmos_ex.h"
+ #include "hicompat.h"
```

Type and symbol prefix substitution — apply globally:

```
HI_S32       → GK_S32
HI_VOID      → GK_VOID
HI_U32       → GK_U32   (and HI_U8/U16/U64 → GK_U8/U16/U64)
HI_BOOL      → GK_BOOL
HI_NULL      → GK_NULL
HI_TRUE      → GK_TRUE
HI_FALSE     → GK_FALSE
HI_SUCCESS   → GK_SUCCESS
HI_FAILURE   → GK_FAILURE
HI_FLOAT     → GK_FLOAT
HI_MPI_ISP_  → GK_API_ISP_
HI_MPI_AE_   → GK_API_AE_
HI_MPI_AWB_  → GK_API_AWB_
```

Logging — T5 prefers `ISP_TRACE` over `printf`:

```diff
- printf("sensor register callback function failed!\n");
+ ISP_TRACE(MODULE_DBG_ERR, "sensor register callback function failed!\n");
```

Function bodies otherwise unchanged. The `hicompat.h` shim (§6) means even the
`GK_API_*` symbol substitution can be skipped at the source level if your build
sets `SDK_CODE=0x3516E200` — the `#define`s in `hicompat.h` redirect them back to
`HI_MPI_*` at compile time.

Makefile:

```diff
- LIB_NAME := libsns_gc2053
- ISP_DIR := $(CURDIR)/../../../isp
- KO_DIR  := $(CURDIR)/../../../../kernel
- override CFLAGS += -fPIC -Wall -O2 \
-     -I$(ISP_DIR)/include/hi3516cv500/3a \
-     -I$(ISP_DIR)/include/hi3516cv500 \
-     -I$(KO_DIR)/include/hi3516cv500 \
-     -I$(KO_DIR)/include/hi3516cv500/adapt \
-     -I.
+ LIB_NAME := libsns_gc2053
+ override CFLAGS += -DSDK_CODE=$(SDK_CODE) -fPIC \
+     -I$(CURDIR)/../../../../include
```

Cross-platform within T5 (EV200 ↔ GK7205V200) is a *zero*-line port — the source
is identical and the `SDK_CODE` macro selects the right symbol bindings.

## 5. Kernel-side dependencies

The userspace `libsns_*.so` is one half of the picture. To get a sensor working
you also need certain kernel modules loaded and a few device nodes available.

| Platform | MIPI device | I2C node | sys_config (whitelist) | Notes |
|---|---|---|---|---|
| CV100 | — (no MIPI; parallel/SSP only) | `/dev/i2c-X` | not present | Oldest gen |
| CV200 | `/dev/hi_mipi` | `/dev/i2c-X` | `kernel/sys_config/hi3516cv200/` | Recent commit 8bbfd9c added OV2710 to the whitelist — illustrates how to add new sensors |
| AV100 | `/dev/hi_mipi` | `/dev/i2c-X` | not in load order | Sensor clock setup likely via different mechanism |
| CV300 | `/dev/hi_mipi` | `/dev/i2c-X` | `kernel/sys_config/hi3516cv300/` | |
| V101 | `/dev/hi_mipi` | `/dev/i2c-X` | `kernel/sys_config/hi3519v101/` | Dual-ISP |
| CV500 | `/dev/hi_mipi` | `/dev/i2c-X` | `kernel/sys_config/hi3516cv500/` | |
| EV200, GK7205V200 | `/dev/hi_mipi` (reuses CV500 mipi_rx) | `/dev/i2c-X` | not present in tree | |

MIPI ioctl ABI changes between tiers. Key shifts:

- **`HI_MIPI_RESET_SENSOR` arg type**: V1/V4 take `sns_rst_source_t`; V3/V3A take
  `COMBO_DEV` — verify in `kernel/mipi_rx/hi3516cv500/hi_mipi.h:228` vs
  `kernel/mipi_rx/hi3516cv300/hi_mipi.h:251` vs `kernel/mipi_rx/hi3519v101/hi_mipi.h:387`.
- **`HI_MIPI_{ENABLE,DISABLE}_SENSOR_CLOCK` ioctls (0x10/0x11)** exist on V1/V4
  (e.g. `kernel/mipi_rx/hi3516cv500/hi_mipi.h:261–264`) but **not** on V2/V3 —
  on those tiers the master clock comes from the `sys_config` module's parameters.

Module load order is the canonical one in CLAUDE.md — see "Module load order"
there for the full list per generation.

The **sensor whitelist** in `sys_config` is a hard gate. If a sensor name isn't
in `parse_sensor_bus_type()` / `parse_sensor_clock()` for the target platform,
the bus-type and mclk metadata default to wrong values and the sensor won't
initialise. To add a new sensor you edit the platform-specific
`kernel/sys_config/<chiparch>/sys_config.c` (commit 8bbfd9c is a worked example
for OV2710 on CV200).

## 6. The `hicompat.h` shim (T5)

Quoted verbatim from `include/hicompat.h`:

```c
#ifndef HICOMPAT_H
#define HICOMPAT_H

#if defined(SDK_CODE) && SDK_CODE+0 != 0
#if SDK_CODE == 0x3516E200
#define GK_API_ISP_SensorRegCallBack HI_MPI_ISP_SensorRegCallBack
#define GK_API_ISP_SensorUnRegCallBack HI_MPI_ISP_SensorUnRegCallBack
#define GK_API_AE_SensorRegCallBack HI_MPI_AE_SensorRegCallBack
#define GK_API_AE_SensorUnRegCallBack HI_MPI_AE_SensorUnRegCallBack
#define GK_API_AWB_SensorRegCallBack HI_MPI_AWB_SensorRegCallBack
#define GK_API_AWB_SensorUnRegCallBack HI_MPI_AWB_SensorUnRegCallBack
#endif

GK_S32 GK_API_ISP_SensorRegCallBack(VI_PIPE ViPipe,
                                    ISP_SNS_ATTR_INFO_S *pstSnsAttrInfo,
                                    ISP_SENSOR_REGISTER_S *pstRegister);
GK_S32 GK_API_ISP_SensorUnRegCallBack(VI_PIPE ViPipe, SENSOR_ID SensorId);

GK_S32 GK_API_AE_SensorRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib,
                                   ISP_SNS_ATTR_INFO_S *pstSnsAttrInfo,
                                   AE_SENSOR_REGISTER_S *pstRegister);
GK_S32 GK_API_AE_SensorUnRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib,
                                     SENSOR_ID SensorId);

GK_S32 GK_API_AWB_SensorRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAwbLib,
                                    ISP_SNS_ATTR_INFO_S *pstSnsAttrInfo,
                                    AWB_SENSOR_REGISTER_S *pstRegister);
GK_S32 GK_API_AWB_SensorUnRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAwbLib,
                                      SENSOR_ID SensorId);
#endif /* SDK_CODE != 0 */

#endif /* HICOMPAT_H */
```

What it does: when building T5 source with `SDK_CODE=0x3516E200` (the EV200
build), the `GK_API_*` calls are macro-redirected to `HI_MPI_*` so the same
source compiles against the HiSilicon-flavoured implementation. When building
for actual Goke hardware (`SDK_CODE` set to a different value, or unset), the
prototypes declare native `GK_API_*` symbols expected to be provided by a Goke
runtime. The struct types (`ISP_SNS_ATTR_INFO_S`, `ISP_SENSOR_REGISTER_S`,
`ALG_LIB_S`) are intentionally identical across both flavours — only the function
prefix differs.

This shim is what makes T4 ↔ T5 a mechanical port rather than a refactor.

## 7. Edge cases — wrapper convention and out-of-range tiers

Two patterns appear in sensor archives in the wild that don't slot neatly into
T1–T5. §7.1 covers a **wrapper / late-binding convention** — the source uses
function-pointer typedefs instead of direct `HI_MPI_*` calls. It's not a
separate API tier; you recognise it, identify the underlying tier from the
indirect call shape, and unwrap it. §7.2 covers **T6**, the post-2022 Shenshu
SDK for chips this repo doesn't yet build for (Hi3519DV500 / Hi3556DV500) —
that *is* a real distinct API surface, just one we haven't grown support for.

### 7.1 Function-pointer wrapper convention (not a tier)

Some sensor source found online has a registration function that **does not
call `HI_MPI_*` directly** — instead it takes function-pointer typedefs as
parameters and dispatches through them:

```c
int sensor_register_callback(pRegSnsRegCb  snsRegCb,
                             pAeSnsRegCb   AeSnsRegCb,
                             pAwbSnsRegCb  awbSnsRegCb,
                             pSnsRegsCfg   snsRegsCfg)
{
    HI_S32 s32Ret;
    ALG_LIB_S stLib;
    ISP_DEV IspDev = 0;
    ISP_SENSOR_REGISTER_S stIspRegister;
    AE_SENSOR_REGISTER_S  stAeRegister;
    AWB_SENSOR_REGISTER_S stAwbRegister;

    cmos_init_sensor_exp_function(&stIspRegister.stSnsExp);
    s32Ret = snsRegCb(IspDev, IMX185_ID, &stIspRegister);            /* indirect */
    s32Ret = AeSnsRegCb(IspDev, &stLib, IMX185_ID, &stAeRegister);   /* indirect */
    s32Ret = awbSnsRegCb(IspDev, &stLib, IMX185_ID, &stAwbRegister); /* indirect */
}
```

This **is not a separate API tier**. The typedefs `pRegSnsRegCb` /
`pAeSnsRegCb` / `pAwbSnsRegCb` / `pSnsRegsCfg` are not part of any HiSilicon
SDK — a `grep -r pRegSnsRegCb` over Hi3518 SDK V1.0.B.0 returns zero hits.
This is a **wrapper / late-binding convention** layered on top of one of the
real tiers (T1, T1+, T2, T3, T4, T5). Likely origins:

- Third-party NVR or IP-camera firmware that loads sensor `.so`s via `dlopen`
  and provides MPI dispatch at registration time, so the `.so` doesn't bind to
  specific `HI_MPI_*` symbol versions.
- A multi-SDK-rev compatibility shim (one source, many host MPPs).
- Vendor-internal isolation refactor.

#### Identifying the underlying tier

The wrapper hides the call to `HI_MPI_*`, but the **arguments passed to the
indirect call** preserve the underlying tier exactly. Run the standard
flowchart on the body of the function instead of its signature:

| Indirect call shape inside body | Underlying tier |
|---|---|
| `snsRegCb(&funcs)` (1 arg) | T1 |
| `snsRegCb(<ID>, &reg)` (2 args, no IspDev) + `AeSnsRegCb(&lib, <ID>, &reg)` (3 args) | T1+ |
| `snsRegCb(IspDev, <ID>, &reg)` (3 args, IspDev) + `AeSnsRegCb(IspDev, &lib, <ID>, &reg)` (4 args) | T2 |
| `snsRegCb(IspDev, <ID>, &reg)` + outer signature `(ISP_DEV, ALG_LIB_S*, ALG_LIB_S*)` | T3 |
| `snsRegCb(vi_pipe, &stSnsAttrInfo, &reg)` | T4 |

The `IMX185` example above is **wrapped T2** — `IspDev` is present, AE/AWB
calls have 4 args.

#### Other markers

- Frequent sensor-ID mismatch: these archives are often copy-pasted from
  earlier drivers without updating the `_ID` macro (one wild example: a file
  named `imx385_cmos.c` whose internal include guard is `__IMX185_CMOS_H_`
  and which registers `IMX185_ID`).
- Headers and types stay tier-correct (e.g. wrapped T2 still includes
  `hi_comm_video.h` and uses `ISP_DEV`).

#### Unwrapping

To produce a directly-callable tier-N driver, just textually replace each
indirect call with the matching `HI_MPI_*` symbol:

```diff
- s32Ret = snsRegCb(IspDev, IMX185_ID, &stIspRegister);
+ s32Ret = HI_MPI_ISP_SensorRegCallBack(IspDev, IMX185_ID, &stIspRegister);
- s32Ret = AeSnsRegCb(IspDev, &stLib, IMX185_ID, &stAeRegister);
+ s32Ret = HI_MPI_AE_SensorRegCallBack(IspDev, &stLib, IMX185_ID, &stAeRegister);
- s32Ret = awbSnsRegCb(IspDev, &stLib, IMX185_ID, &stAwbRegister);
+ s32Ret = HI_MPI_AWB_SensorRegCallBack(IspDev, &stLib, IMX185_ID, &stAwbRegister);
```

…and change the function signature from `(pRegSnsRegCb, pAeSnsRegCb,
pAwbSnsRegCb, pSnsRegsCfg)` to whatever the underlying tier requires (e.g.
`(void)` for T2).

> An earlier version of this document called this "T0" and speculated about a
> pre-CV200 SDK era. That framing was wrong — the only archive evidence is
> internally a T2 driver wearing a custom indirection. Kept here as a real
> wrapper convention you'll bump into in archives, not a distinct API surface.

### 7.2 T6 — post-2022 "Shenshu" SDK (Hi3519DV500 / Hi3556DV500 family)

After Huawei's HiSilicon was sanctioned and partially renamed to Shenshu Tech,
the SDK underwent a uniform naming cleanup: snake_case throughout, `ot_*`
prefix replaces `HI_*` / `ISP_*` on type names, `td_*` replaces `HI_S32` /
`HI_VOID` on primitives, and the MPI headers move to `ot_mpi_*.h`. Structurally
this is a one-to-one rename of T4/T5 — same fields, same flow, same
`ISP_SNS_ATTR_INFO_S`-equivalent struct, same `ISP_SNS_OBJ_S`-equivalent export
— but every identifier has been touched.

Targets chips that this repo does not currently build for (Hi3519DV500,
Hi3556DV500, Hi3519AV200, and their Shenshu-rebranded variants).

Canonical signature:

```c
#include "ot_mpi_isp.h"
#include "ot_mpi_ae.h"
#include "ot_mpi_awb.h"
#include "securec.h"

#define IMX482_ID 482

static td_s32 sensor_register_callback(ot_vi_pipe vi_pipe,
                                       ot_isp_3a_alg_lib *ae_lib,
                                       ot_isp_3a_alg_lib *awb_lib)
{
    td_s32 ret;
    ot_isp_sensor_register      isp_register;
    ot_isp_ae_sensor_register   ae_register;
    ot_isp_awb_sensor_register  awb_register;
    ot_isp_sns_attr_info        sns_attr_info;

    sensor_check_pointer_return(ae_lib);
    sensor_check_pointer_return(awb_lib);

    ret = sensor_ctx_init(vi_pipe);
    if (ret != TD_SUCCESS) return TD_FAILURE;

    sns_attr_info.sensor_id = IMX482_ID;
    /* ... ot_mpi_isp_sensor_reg_callback(...) etc. ... */
}

ot_isp_sns_obj g_sns_imx482_obj = {
    .pfn_register_callback     = sensor_register_callback,
    .pfn_un_register_callback  = sensor_unregister_callback,
    .pfn_standby               = imx482_standby,
    /* ... */
};
```

Distinguishing markers (any one of these is sufficient):

- `#include "ot_mpi_isp.h"` (or `ot_mpi_ae.h` / `ot_mpi_awb.h`).
- `td_s32`, `td_u32`, `td_void`, `TD_NULL`, `TD_SUCCESS`, `TD_FAILURE` types.
- `ot_*` prefix on struct types: `ot_vi_pipe`, `ot_isp_sns_attr_info`,
  `ot_isp_3a_alg_lib`, `ot_isp_sensor_register`, `ot_isp_sns_obj`.
- snake_case fields: `pfn_register_callback`, `g_sns_<x>_obj`,
  `sns_attr_info.sensor_id`.
- `OT_ISP_MAX_PIPE_NUM` instead of `ISP_MAX_PIPE_NUM`.
- Copyright headers mentioning **"Shenshu Tech. Co., Ltd."** instead of
  "Hisilicon Technologies".
- `securec.h` include (Huawei's hardened C runtime — adopted in the post-2022
  refactor).

**To reach a supported tier**: T6 → T5 is mechanically the same transition as
T4 → T5 documented in §4.3, plus a uniform rename pass:

```
ot_vi_pipe                → VI_PIPE
ot_isp_3a_alg_lib         → ALG_LIB_S
ot_isp_sns_attr_info      → ISP_SNS_ATTR_INFO_S
ot_isp_sensor_register    → ISP_SENSOR_REGISTER_S
ot_isp_ae_sensor_register → AE_SENSOR_REGISTER_S
ot_isp_awb_sensor_register→ AWB_SENSOR_REGISTER_S
ot_isp_sns_obj            → ISP_SNS_OBJ_S
OT_ISP_MAX_PIPE_NUM       → ISP_MAX_PIPE_NUM
td_s32 / td_u32 / td_void → HI_S32 / HI_U32 / HI_VOID  (or GK_* for T5)
TD_NULL / TD_SUCCESS / TD_FAILURE → HI_NULL / HI_SUCCESS / HI_FAILURE
pfn_register_callback     → pfnRegisterCallback        (and other pfn_*)
sns_attr_info.sensor_id   → stSnsAttrInfo.eSensorId
ot_mpi_isp.h              → mpi_isp.h
g_sns_<x>_obj             → stSns<X>Obj
sensor_check_pointer_return → CMOS_CHECK_POINTER
```

The body logic is unchanged. If `securec.h` calls (e.g. `memcpy_s`) are present
they are already supported in T4/T5 source (CV500 imx307 uses `memcpy_s`), so
no substitution is needed there.

The harder path — T6 → kernel-side support for the actual Shenshu chips
(Hi3519DV500 etc.) — is a much bigger lift: the kernel modules in this repo
(`mipi_rx`, `sys_config`, `osal`) would need new per-chip variants that don't
currently exist.

## 8. Quick-reference appendix

Cross-table for ctrl-F lookup. All claims tracked back to the file:line cites
in §1 and §3.

| Platform | Tier | Reg sig | Type prefix | Pipe abstraction | Bus-info pattern | Makefile -I flavour | Output |
|---|---|---|---|---|---|---|---|
| *(historical mpp/, not in repo)* | T1 | `(void)`, 1 MPI call | `HI_*` | none | static globals | varies | `libsns_<x>.{so,a}` |
| `hi3516cv100` | T1+ | `(void)`, 3 MPI calls **without `IspDev`** | `HI_*` | none | static globals | `kernel/include/hi3516cv100` (mpp2 flavour) | `libsns_<x>.{so,a}` |
| `hi3516cv200` | T2 | `(void)`, 3 MPI calls | `HI_*` | `ISP_DEV` (=0) | static globals | `kernel/include/<chiparch>` + `kernel/isp/arch/<chiparch>/include` | `libsns_<x>.{so,a}` |
| `hi3516av100` | T2 | same as CV200 | `HI_*` | `ISP_DEV` (=0) | static globals | same shape, different platform | `libsns_<x>.{so,a}` |
| `hi3516cv300` | T2 | same as CV200 | `HI_*` | `ISP_DEV` (=0) | static globals | same shape, different platform | `libsns_<x>.{so,a}` |
| `hi3519v101` | T3 | `(ISP_DEV, AE*, AWB*)`, 3 MPI calls | `HI_*` | `ISP_DEV` (0–1) | `g_aun<X>BusInfo[ISP_MAX_DEV_NUM]` + `set_bus_info` | `kernel/include/<chiparch>` + `kernel/isp/arch/<chiparch>/include` | `libsns_<x>.{so,a}` |
| `hi3516av200` *(†, built via `CHIPARCH=hi3519v101`)* | T3 | same as V101 | `HI_*` | `ISP_DEV` | same as V101 | same as V101 | `libsns_<x>.{so,a}` |
| `hi3516cv500` | T4 | `(VI_PIPE, AE*, AWB*)`, 3 MPI calls + `ISP_SNS_ATTR_INFO_S` | `HI_*` | `VI_PIPE` | `g_aun<X>BusInfo[ISP_MAX_PIPE_NUM]` + `set_bus_info` | `libraries/isp/include/hi3516cv500/3a` + `libraries/isp/include/hi3516cv500` + `kernel/include/hi3516cv500/adapt` | `libsns_<x>.{so,a}` |
| `hi3516ev200` | T5 | same as CV500 | `GK_*` | `VI_PIPE` | same as CV500 | `-DSDK_CODE=$(SDK_CODE) -I$(CURDIR)/../../../../include` (unified) | `libsns_<x>.{so,a}` |
| `gk7205v200` | T5 | same as CV500 | `GK_*` | `VI_PIPE` | same as CV500 | same as EV200 (sources are identical) | `libsns_<x>.{so,a}` |
| *(Hi3519DV500 / Hi3556DV500, post-2022)* | **T6** *(unsupported, §7.2)* | `(ot_vi_pipe, ot_isp_3a_alg_lib*, ot_isp_3a_alg_lib*)`, 3 `ot_mpi_*` calls + `ot_isp_sns_attr_info` | `td_*` / `ot_*` | `ot_vi_pipe` | `g_<x>_bus_info[OT_ISP_MAX_PIPE_NUM]` (snake_case) | varies (Shenshu SDK) | `libsns_<x>.{so,a}` |

Also useful when deciding what tier a found-online file targets:

| If the source has... | Almost certainly tier... |
|---|---|
| `snsRegCb(<...>, &reg);` (function-pointer dispatch, no direct `HI_MPI_*` calls in body) | wrapped driver — see §7.1 |
| `HI_MPI_ISP_SensorRegCallBack(&funcs);` (single arg) | T1 |
| `HI_MPI_ISP_SensorRegCallBack(<ID>, &reg);` (2 args, no `IspDev`) + `HI_MPI_AE_SensorRegCallBack(&lib, <ID>, &reg)` (3 args) | T1+ (CV100, see §3.1) |
| `HI_MPI_ISP_SensorRegCallBack(IspDev, <ID>, &reg);` and `int sensor_register_callback(void)` | T2 |
| `HI_MPI_ISP_SensorRegCallBack(IspDev, <ID>, &reg);` and `(ISP_DEV, ALG_LIB_S*, ALG_LIB_S*)` signature | T3 |
| `HI_MPI_ISP_SensorRegCallBack(vi_pipe, &stSnsAttrInfo, &reg);` | T4 |
| `GK_API_ISP_SensorRegCallBack(...)` *or* `#include "hicompat.h"` | T5 |
| `ot_mpi_isp_sensor_reg_callback(...)` *or* `#include "ot_mpi_isp.h"` *or* `td_s32 sensor_register_callback(ot_vi_pipe, ...)` | T6 *(unsupported, §7.2)* |
