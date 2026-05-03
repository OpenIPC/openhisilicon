# Sensor driver API across HiSilicon / Goke platforms

This repo carries sensor drivers for 8 chip families. They look superficially similar
(`<vendor>_<model>_cmos.c`, `<model>_sensor_ctl.c`, a `Makefile` producing
`libsns_<model>.so`) but the **public API the driver presents to the rest of the SDK
differs in 5 distinct ways**. This document is a field guide for two questions:

1. *I found sensor source on GitHub / CSDN. Which SDK is it from?*
2. *I want to port that source to a different platform. What changes?*

All claims below cite a `path:line` you can open and verify. The flow:
classification → identification flowchart → per-tier fingerprints → side-by-side
porting diffs → kernel-side dependencies → quick-reference appendix.

## 1. Five-tier classification

The 8 platforms collapse into 5 sensor-driver API tiers. Most porting work happens
at the boundaries between tiers; *within* a tier you can usually copy a driver
across platforms with only a `#include` path change.

| Tier | Platforms | Headline shape |
|---|---|---|
| **T1 / V1** | `hi3516cv100` | `int sensor_register_callback(void)` — **one** MPI call, no AE/AWB lib |
| **T2 / V2** | `hi3516cv200`, `hi3516av100`, `hi3516cv300` | `int sensor_register_callback(void)` — **three** MPI calls (ISP/AE/AWB), `IspDev` is a local `0`, internal `ALG_LIB_S stLib` |
| **T3 / V3A** | `hi3519v101` | `static int sensor_register_callback(ISP_DEV IspDev, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)` — caller passes lib handles, first tier to export `ISP_SNS_OBJ_S stSns<X>Obj` |
| **T4 / V4-HI** | `hi3516cv500` | `static HI_S32 sensor_register_callback(VI_PIPE vi_pipe, ALG_LIB_S *pstAeLib, ALG_LIB_S *pstAwbLib)` — `VI_PIPE` not `ISP_DEV`, `ISP_SNS_ATTR_INFO_S` carries the sensor ID, ISP params split into `<sensor>_cmos_ex.h` |
| **T5 / V4-GK** | `hi3516ev200`, `gk7205v200` | T4 shape but `GK_API_*` instead of `HI_MPI_*`, `GK_S32` types, `comm_sns.h`/`sns_ctrl.h` (no `hi_` prefix); `hicompat.h` aliases `GK_*` ↔ `HI_*` at compile time |

Verified anchor points (one driver per tier):

- T1 — `libraries/sensor/hi3516cv100/sony_imx236/imx236_cmos.c:951`
- T2 — `libraries/sensor/hi3516cv200/aptina_9m034/m034_cmos.c:1335`
- T2 — `libraries/sensor/hi3516cv300/sony_imx290/imx290_cmos.c:2030`
- T2 — `libraries/sensor/hi3516av100/aptina_ar0230/ar0230_cmos.c:1329`
- T3 — `libraries/sensor/hi3519v101/sony_imx226/imx226_cmos.c:1458` (`stSnsImx226Obj` at `:1533`)
- T4 — `libraries/sensor/hi3516cv500/sony_imx307/imx307_cmos.c:1238` (`stSnsImx307Obj` at `:1348`)
- T5 — `libraries/sensor/hi3516ev200/galaxycore_gc2053/gc2053_cmos.c:1120` (`stSnsGc2053Obj` at `:1236`)
- T5 — `libraries/sensor/gk7205v200/galaxycore_gc2053/gc2053_cmos.c:1120` (identical to EV200)

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
  Yes (signature has params)
    │
    ├── Q3. First parameter type?
    │       ISP_DEV    → T3 (V101)
    │       VI_PIPE    → Q4
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
| `#include "hi_comm_isp.h"` *and not* `mpi_ae.h` | T1 |
| `mpi_isp.h` + `mpi_ae.h` + `mpi_awb.h` + `#define <X>_ID <num>` + `ISP_DEV IspDev = 0;` in `sensor_register_callback` | T2 |
| `g_pastImx226[ISP_MAX_DEV_NUM]` (or any state array indexed by `ISP_MAX_DEV_NUM`) | T3 |
| `g_pastImx307[ISP_MAX_PIPE_NUM]` + `ISP_SNS_ATTR_INFO_S stSnsAttrInfo` + `ISP_SNS_OBJ_S stSns<X>Obj` | T4 |
| `#include "comm_sns.h"`, `#include "gk_api_isp.h"`, `#include "hicompat.h"`, `GK_S32`, `GK_SUCCESS`, `ISP_TRACE(MODULE_DBG_ERR, ...)` | T5 |

## 3. Per-tier details

### 3.1 T1 — CV100

Single-call ISP-only registration. Everything else (AE curves, exposure math, gain
tables) lives inline as static structs in `*_cmos.c`. There is no `ALG_LIB_S`,
no `ISP_SNS_OBJ_S`, and no AE/AWB MPI surface.

Headers (`imx236_cmos.c:1–11`):

```c
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "hi_comm_sns.h"
#include "hi_comm_isp.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"
```

Registration (`imx236_cmos.c:951–962`):

```c
int sensor_register_callback(void)
{
    int ret;
    ret = HI_MPI_ISP_SensorRegCallBack(&stSensorExpFuncs);
    if (ret) {
        printf("sensor register callback function failed!\n");
        return ret;
    }
    return 0;
}
```

Makefile (`hi3516cv100/sony_imx236/Makefile`):

```makefile
LIB_NAME := libsns_imx236
override CFLAGS += -fPIC \
    -I$(CURDIR)/../../../../kernel/include/hi3516cv100
```

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

T2 sub-distinction — CV200 vs AV100 vs CV300 is *purely* a header-path /
sensor-ID-namespace difference; the function shape, includes, and `ALG_LIB_S`
boilerplate are identical. Confirmed by reading
`hi3516av100/aptina_ar0230/ar0230_cmos.c:1329` and
`hi3516cv300/sony_imx290/imx290_cmos.c:2030` — both follow the same pattern with
their respective sensor IDs.

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

## 7. Quick-reference appendix

Cross-table for ctrl-F lookup. All claims tracked back to the file:line cites
in §1 and §3.

| Platform | Tier | Reg sig | Type prefix | Pipe abstraction | Bus-info pattern | Makefile -I flavour | Output |
|---|---|---|---|---|---|---|---|
| `hi3516cv100` | T1 | `(void)`, 1 MPI call | `HI_*` | none | static globals | `kernel/include/<chiparch>` | `libsns_<x>.{so,a}` |
| `hi3516cv200` | T2 | `(void)`, 3 MPI calls | `HI_*` | `ISP_DEV` (=0) | static globals | `kernel/include/<chiparch>` + `kernel/isp/arch/<chiparch>/include` | `libsns_<x>.{so,a}` |
| `hi3516av100` | T2 | same as CV200 | `HI_*` | `ISP_DEV` (=0) | static globals | same shape, different platform | `libsns_<x>.{so,a}` |
| `hi3516cv300` | T2 | same as CV200 | `HI_*` | `ISP_DEV` (=0) | static globals | same shape, different platform | `libsns_<x>.{so,a}` |
| `hi3519v101` | T3 | `(ISP_DEV, AE*, AWB*)`, 3 MPI calls | `HI_*` | `ISP_DEV` (0–1) | `g_aun<X>BusInfo[ISP_MAX_DEV_NUM]` + `set_bus_info` | `kernel/include/<chiparch>` + `kernel/isp/arch/<chiparch>/include` | `libsns_<x>.{so,a}` |
| `hi3516cv500` | T4 | `(VI_PIPE, AE*, AWB*)`, 3 MPI calls + `ISP_SNS_ATTR_INFO_S` | `HI_*` | `VI_PIPE` | `g_aun<X>BusInfo[ISP_MAX_PIPE_NUM]` + `set_bus_info` | `libraries/isp/include/hi3516cv500/3a` + `libraries/isp/include/hi3516cv500` + `kernel/include/hi3516cv500/adapt` | `libsns_<x>.{so,a}` |
| `hi3516ev200` | T5 | same as CV500 | `GK_*` | `VI_PIPE` | same as CV500 | `-DSDK_CODE=$(SDK_CODE) -I$(CURDIR)/../../../../include` (unified) | `libsns_<x>.{so,a}` |
| `gk7205v200` | T5 | same as CV500 | `GK_*` | `VI_PIPE` | same as CV500 | same as EV200 (sources are identical) | `libsns_<x>.{so,a}` |

Also useful when deciding what tier a found-online file targets:

| If the source has... | Almost certainly tier... |
|---|---|
| `HI_MPI_ISP_SensorRegCallBack(&funcs);` (single arg) | T1 |
| `HI_MPI_ISP_SensorRegCallBack(IspDev, <ID>, &reg);` and `int sensor_register_callback(void)` | T2 |
| `HI_MPI_ISP_SensorRegCallBack(IspDev, <ID>, &reg);` and `(ISP_DEV, ALG_LIB_S*, ALG_LIB_S*)` signature | T3 |
| `HI_MPI_ISP_SensorRegCallBack(vi_pipe, &stSnsAttrInfo, &reg);` | T4 |
| `GK_API_ISP_SensorRegCallBack(...)` *or* `#include "hicompat.h"` | T5 |
