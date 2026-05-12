# OpenHisilicon

Open-source kernel modules and sensor libraries for HiSilicon/Goke IP camera SoCs, covering seven chip generations from V1 through V4.

Replaces the proprietary SDK that HiSilicon ships to camera manufacturers. Used by [OpenIPC](https://openipc.org/) firmware and can be used independently in your own projects.

## Supported hardware

| Generation | SoC family | Chip IDs | CPU | CHIPARCH |
|------------|------------|----------|-----|----------|
| V1 | hi3516cv100 | hi3516cv100, hi3518cv100, hi3518ev100 | ARM926EJ-S | `hi3516cv100` |
| V2 | hi3516cv200 | hi3516cv200, hi3518ev200, hi3518ev201 | ARM926EJ-S | `hi3516cv200` |
| V2A | hi3516av100 | hi3516av100, hi3516dv100 | Cortex-A7 | `hi3516av100` |
| V3 | hi3516cv300 | hi3516cv300, hi3516ev100 | ARM926EJ-S | `hi3516cv300` |
| V3A | hi3519v101 | hi3519v101, hi3516av200 | Cortex-A17+A7 | `hi3519v101` |
| V3.5 | hi3516cv500 | hi3516cv500, hi3516av300, hi3516dv300 | Cortex-A7 | `hi3516cv500` |
| V4 | hi3516ev200 | hi3516ev200, hi3516ev300, hi3518ev300, hi3516dv200 | Cortex-A7 | `hi3516ev200` |
| V4/Goke | gk7205v200 | gk7205v200, gk7205v300, gk7202v300, gk7605v100 | Cortex-A7 | `gk7205v200` |

Generation labels match [qemu-hisilicon](https://github.com/widgetii/qemu-hisilicon). The hi3516ev200 and gk7205v200 are pin-compatible — the same source compiles for both via `CHIPARCH`. A [conversion script](scripts/hi2gk.sh) translates between HiSilicon and Goke naming.

## How HiSilicon SDK modules work

If you're new to HiSilicon camera SoCs, this section explains the concepts you'll encounter throughout the codebase.

### The vendor SDK problem

HiSilicon ships camera manufacturers a proprietary SDK containing pre-compiled kernel modules (`.ko` files) built for a specific Linux kernel version. These modules implement the video pipeline — capturing frames from the image sensor, running them through the ISP (Image Signal Processor), encoding to H.264/H.265, and outputting the stream. The source code for most of these modules is not provided.

When projects like OpenIPC want to run a different kernel version, or build firmware from source, these vendor `.ko` files become a problem: a module compiled for kernel 3.18 won't load on kernel 4.9 due to symbol versioning (`vermagic`). This repository provides the infrastructure to rebuild loadable modules that incorporate the vendor binary code.

### Key SDK components

**MMZ (Media Memory Zone)** — IP cameras need large contiguous blocks of physical memory for DMA transfers between hardware blocks (sensor → ISP → encoder). Linux's default memory allocator doesn't guarantee contiguous pages. MMZ carves out a dedicated region of RAM at boot (configured via kernel bootargs like `mmz=anonymous,0,0x82000000,32M`) and provides `hil_mmb_alloc`/`hil_mmb_free` for the video pipeline to allocate from it. Think of it as a specialized memory pool sitting alongside Linux's normal memory management.

**himedia** — A media device registration framework. Where Linux has `misc_register()` for simple character devices, HiSilicon's SDK has `himedia_register()` which creates `/dev/` entries for each pipeline stage (ISP, VI, VPSS, VENC, etc.) and manages their lifecycle. The base module uses himedia to register itself and provide inter-module communication.

**OSAL (OS Abstraction Layer)** — HiSilicon shipped each SDK in two variants: one for Linux and one for Huawei LiteOS (their proprietary RTOS for resource-constrained devices). OSAL was created to isolate the multimedia IP block drivers from the OS — the video pipeline code calls `osal_mutex_lock()` instead of Linux's `mutex_lock()` or LiteOS's `LOS_MuxPend()`. The OSAL source provides the OS-specific implementation (mutexes, timers, proc filesystem, memory mapping, interrupts), while the pre-compiled multimedia blobs remain identical across both operating systems. For us, this design is a gift: because the blobs call OSAL functions instead of kernel functions directly, the OSAL source can be recompiled for a new kernel version while the blobs remain unchanged. This is the key mechanism that allows vendor modules from kernel 4.9 to run on kernel 7.0.

### How it evolved across generations

The generation **number** tracks the peripheral address map and SDK architecture. The **letter suffix** tracks CPU upgrades within the same architecture:

| Generation | SoC | CPU | Memory layer | Module init | Notes |
|---|---|---|---|---|---|
| V1 | CV100 | ARM926EJ-S | Standalone MMZ | `.ko` blobs via objcopy | First generation, VIC interrupts |
| V2 | CV200 | ARM926EJ-S | MMZ + himedia | `.ko` blobs via objcopy | Added himedia device framework |
| V2A | AV100 | Cortex-A7 | MMZ + himedia | `.ko` blobs via objcopy | V2 SDK with ARM upgrade + GIC |
| V3 | CV300 | ARM926EJ-S | OSAL | Raw `.o` + init wrappers | New `0x12xxxxxx` address map, OSAL replaces MMZ/himedia |
| V3A | 3519V101 | Cortex-A17+A7 | OSAL | `.ko` blobs via objcopy | V3 SDK with big.LITTLE for 4K |
| V3.5 | CV500 | Cortex-A7 | OSAL | Raw `.o` + init wrappers | Incremental V3, snake_case SDK symbols |
| V4 | EV200 | Cortex-A7 | OSAL | Source-based Kbuild | Redesigned modular SDK, Goke compat, mainline kernel |

### Dealing with vendor .ko blobs

A vendor `.ko` is an ELF relocatable object containing:
- The module's compiled code (video pipeline logic, hardware register programming)
- `init_module` / `cleanup_module` entry points
- `__ksymtab_*` entries for exported symbols (like `CMPI_MmzMalloc`, `VB_Init`)

We use two strategies to build new `.ko` modules that incorporate this vendor code:

**Strategy A: Raw `.o` blobs** (CV300, CV500 SDKs). The SDK provides separate `.o` object files without `init_module`/`cleanup_module`. We write a thin C wrapper (`init/hi3516cv300/base_init.c`) that calls the blob's init function and re-exports its symbols, then link them together via Kbuild into a new `open_base.ko`. Clean and simple.

**Strategy B: objcopy on `.ko`** (CV100, CV200, AV100, 3519V101). The SDK only provides complete `.ko` files. We use `objcopy --redefine-sym init_module=_renamed_init` to rename the embedded entry points, write a new wrapper that forwards to them, and link both into our `open_*.ko`. The blob's `__ksymtab_*` entries survive, so the wrapper must not re-export the same symbols.

**Strategy C: Source compilation** (all platforms). For modules where HiSilicon provides GPL source (MMZ, OSAL, sys_config, MIPI, watchdog, ISP firmware, cipher, etc.), we compile directly from source. These have no kernel version lock — they build against whatever kernel headers are available.

### The kernel compatibility layer

The file [`kernel/compat/kernel_compat.h`](kernel/compat/kernel_compat.h) is included in every compilation unit via `-include` in the top-level Kbuild. It provides transparent API compatibility across Linux 3.0 through 7.0:

- Timer API: `init_timer` (3.x) → `timer_setup` (4.15+) → `timer_delete` (7.0)
- Memory mapping: `ioremap_nocache` removed in 5.6, `set_fs`/`get_fs` removed in 5.10
- SPI/I2C: `spi_busnum_to_master` → `spi_busnum_to_controller` (5.0) → removed (6.4)
- Proc filesystem: `struct file_operations` → `struct proc_ops` (5.6)
- Sysctl: `register_sysctl_table` removed in 6.6
- CMA: `dma_alloc_from_contiguous` gained `no_warn` parameter in 4.15
- Platform driver: `.remove` return type changed from `int` to `void` in 6.11

No `#ifdef` soup in driver code — the compat header handles everything. This is how the hi3516ev300 builds cleanly on kernels 4.9, 6.6, 6.18, and 7.0 from identical source.

## Supported kernels

| Kernel | Status | Platforms |
|--------|--------|-----------|
| 3.0.8 (vendor) | Production | hi3516cv100 |
| 3.18.20 (vendor) | Production | hi3516cv300, hi3519v101 |
| 4.9.37 (vendor) | Production | hi3516cv200, hi3516av100, hi3516cv500, hi3516ev300, gk7205v200 |
| 6.6 LTS | Production | hi3516ev300 (neo) |
| 6.18 LTS | Tested | hi3516ev300 (CI-verified) |
| 7.0-rc6 (mainline) | Production | hi3516ev300 (neo) |

## Repository structure

```
├── kernel/                      Kernel modules (out-of-tree)
│   ├── hi3516cv100.kbuild       V1 monolithic build config
│   ├── hi3516cv200.kbuild       V2 monolithic build config
│   ├── hi3516cv300.kbuild       V3 monolithic build config (OSAL)
│   ├── hi3516cv500.kbuild       V3.5 monolithic build config (OSAL)
│   ├── hi3516av100.kbuild       V2A monolithic build config
│   ├── hi3519v101.kbuild        V3A monolithic build config (OSAL)
│   ├── Kbuild                   Main entry — dispatches by CHIPARCH
│   ├── compat/                  Kernel version compatibility (3.0 – 7.0)
│   ├── obj/<chiparch>/          Pre-built vendor .o blobs
│   ├── init/<chiparch>/         Module init/exit wrappers for blobs
│   ├── include/<chiparch>/      SDK headers per platform
│   ├── osal/<chiparch>/         OSAL source (V3, V3A, V3.5)
│   ├── mmz/<chiparch>/          MMZ source (V1, V2, V2A)
│   ├── himedia/<chiparch>/      himedia source (V2, V2A)
│   ├── isp/arch/<chiparch>/     ISP firmware source
│   ├── sys_config/<chiparch>/   Pin mux / clock config source
│   ├── mipi_rx/<chiparch>/      MIPI CSI-2 receiver source
│   ├── cipher/<chiparch>/       Cipher / crypto source
│   ├── wdt/, ir/, rtc/, pwm/   Peripheral driver sources
│   ├── sensor_i2c/, sensor_spi/ Sensor bus driver sources
│   ├── osal/linux/kernel/       V4 shared OSAL (hi3516ev200 path)
│   ├── base/, sys/, vi/, ...    V4 per-module Kbuild dirs
│   ├── ive/                     Vendor IVE blob (all platforms)
│   ├── ive_neo/                 Clean-room IVE (EV200/EV300 only)
│   └── ext_inc/                 Cross-module headers (V4)
│
├── libraries/                   User-space libraries
│   ├── include/                 Shared HI_* headers
│   ├── sensor/<chiparch>/       Image sensor drivers per platform
│   ├── isp/arch/<chiparch>/     ISP algorithm source per platform
│   ├── mpi_neo/                 libmpi_neo.so (platform-agnostic)
│   ├── ive_neo/                 libive_neo.so (platform-agnostic)
│   └── ivp_neo/                 libivp_neo.so (platform-agnostic)
│
├── include/                     Shared headers (kernel + userspace, V4)
└── scripts/                     Build utilities
```

Each `CHIPARCH` has its own monolithic `.kbuild` file (except V4 which uses modular per-module `Kbuild` files). The main `kernel/Kbuild` dispatches to the right one based on the `CHIPARCH` variable.

## Building

### Via OpenIPC firmware (recommended)

```bash
git clone --depth 1 https://github.com/openipc/firmware
cd firmware

# Build for any supported platform:
make BOARD=hi3516cv100_lite br-hisilicon-opensdk   # V1
make BOARD=hi3516cv200_lite br-hisilicon-opensdk   # V2
make BOARD=hi3516av100_lite br-hisilicon-opensdk   # V2A
make BOARD=hi3516cv300_lite br-hisilicon-opensdk   # V3
make BOARD=hi3519v101_lite br-hisilicon-opensdk    # V3A
make BOARD=hi3516cv500_lite br-hisilicon-opensdk   # V3.5
make BOARD=hi3516ev300_lite br-hisilicon-opensdk   # V4
make BOARD=gk7205v200_lite br-hisilicon-opensdk    # V4/Goke
make BOARD=hi3516ev300_neo br-hisilicon-opensdk    # V4 mainline kernel
```

### Standalone (for development)

```bash
# Kernel modules — set CHIPARCH for your platform
make -C /path/to/firmware/output/build/linux-custom \
    M=$(pwd)/kernel CHIPARCH=hi3516ev200 modules

# Sensor libraries
make CROSS_COMPILE=arm-openipc-linux-musleabi- \
    CHIPARCH=hi3516ev200 -C libraries all
```

## Supported sensors

Sensor drivers are per-platform — each generation's SDK provides drivers for the sensors available on boards of that era. Some sensors (like IMX307, OV2718) appear across multiple platforms.

For the API surface differences between platforms — what changes in the
`sensor_register_callback` signature across CV100 / CV200 / V101 / CV500 /
EV200, the side-by-side porting diffs, and how to identify which SDK a
sensor driver found online came from — see
[**docs/sensor-driver-api.md**](docs/sensor-driver-api.md).

| Manufacturer | Sensor | CV100 | CV200 | AV100 | CV300 | 3519V101 | CV500 | EV200 |
|---|---|---|---|---|---|---|---|---|
| **Sony** | IMX104 | x | | | | | | |
| | IMX117 | | | x | | | | |
| | IMX122 | x | | | | | | |
| | IMX123 | | | x | | | | |
| | IMX138 | x | | | | | | |
| | IMX178 | | | x | | | | |
| | IMX185 | | | x | | | | |
| | IMX222 | | x | | | | | |
| | IMX226 | | | | | x | | |
| | IMX236 | x | | | | | | |
| | IMX274 | | | | | x | | |
| | IMX290 | | | | x | x | | x |
| | IMX307 | | | | x | | x | x |
| | IMX323 | | | | x | | | |
| | IMX326 | | | | | x | | |
| | IMX327 | | | | | x | x | x |
| | IMX335 | | | | | | x | x |
| | IMX385 | | | | x | x | | |
| | IMX390 | | | | | | x | |
| | IMX415 | | | | | | x | |
| | IMX458 | | | | | | x | |
| | ICX692 | x | | | | | | |
| **Aptina** | AR0130 | x | x | | | | | |
| | AR0230 | | x | x | | | | |
| | AR0237 | | | x | x | | | x |
| | AR0330 | x | | x | | | | |
| | MT9P006 | x | | | | | | |
| | 9M034 | x | x | | | | | |
| **OmniVision** | OV2718 | | x | | x | | | x |
| | OV4689 | | | x | | x | | |
| | OV5658 | | | x | | | | |
| | OV9712 | x | x | | | | | |
| | OV9732 | | x | | | | | |
| | OV9750 | | x | | | | | |
| | OV9752 | | x | | | | | |
| | OS04B10 | | | | | | x | |
| | OS05A | | | | | x | x | x |
| | OS08A | | | | | x | x | |
| **SmartSens** | SC1135 | | x | | | | | |
| | SC2035 | | x | | | | | |
| | SC2135 | | x | | | | | |
| | SC2235 | | x | | x | | | x |
| | SC4210 | | | | | | x | |
| | SC4236 | | | x | | | | x |
| **Panasonic** | MN34031 | x | | | | | | |
| | MN34041 | x | | | | | | |
| | MN34220 | | | x | | | x | |
| | MN34222 | | x | | | | | |
| **Silicon Optronics (JX)** | JXH22 | x | | | | | | |
| | JXH42 | x | x | | | | | |
| | JXH63 | | | | | | | x |
| | JXF22 | | x | | x | | | |
| | JXF23 | | x | | | | | |
| | JXF37 | | | | | | | x |
| **GalaxyCore** | GC2053 | | | | | | x | x |

Each sensor has `.so` (shared) and `.a` (static) library builds. The goal is to eventually unify sensor drivers across platforms where the same sensor model is used.

### IMX335 high-framerate modes (hi3516ev200 only)

Fps measured at the receiving end of a UDP-RTP push from camera →
this machine (`outgoing: { enabled: true; - udp://<host>:<port> }`),
counting RTP marker-bit transitions over a 10 s window after 2 s
warm-up. This is the FPV-drone wire-rate, which equals everything
downstream (sensor → VI → ISP → VPSS → VENC → RTP packetizer → UDP).
H.265, 4 Mbps target, INCK 37.125 MHz, identical INI and yaml on
both `openipc-hi3516ev300` and `openipc-gk7205v300`. CPU column is
total busy% (user+sys+sirq) on the camera during the 8 s window.

| Mode | Sensor crop | hi3516ev300 | gk7205v300 | Selected by |
|------|-------------|------------|------------|-------------|
| Stock full-scale | 2592×1944 | 30 fps | 30 fps | `DevRect_w=2592 DevRect_h=1944` (default) |
| Cropped 16:9 | 2592×1520 | 49 fps | 45 fps | `DevRect_w=2592 DevRect_h=1520` |
| Binning | 1296×972 | 64 fps | 64 fps | `DevRect_w=1296 DevRect_h=972` |
| Cropped 1.5x zoom | 1920×1080 | 55 fps | 55 fps | `DevRect_w=1920 DevRect_h=1080` |
| Boost-1944p | 2592×1944 | 39 fps (`Isp_FrameRate=45`) | 31 fps (`Isp_FrameRate=36`) | `Isp_SnsMode=6` |
| Flexible crop | arbitrary W×H | up to **140 fps** at 480×352 | up to **140 fps** at 480×352 | `Isp_SnsMode=4` + `Isp_W=...` + `Isp_H=...` |

Flexible-crop UDP-RTP wire rate, baseline (HMAX = 0x016E) vs the
HMAX = 0x0100 patch at `imx335_sensor_ctl.c:811`. Both boards
deliver identically:

| Flex crop W×H @ req | Baseline (both) | **Patched (both)** | Lift | CPU on cam (patched) |
|---|---|---|---|---|
| 1280×720 @ 100 fps | 45 fps | **56 fps** | +24% | 16% |
| 800×480 @ 130 fps  | 58 fps | **83 fps**  | +43% | 23% |
| 800×480 @ 150 fps  | 65 fps | **94 fps**  | +43% | 26% |
| 800×480 @ 240 fps  | 77 fps | **110 fps** | +43% | 30% |
| 480×352 @ 240 fps  | 98 fps | **140 fps** | +43% | 35% |

(1024×576 omitted from the headline table — encoder produced
anomalously small frames on the baseline at that resolution which
inflated the apparent lift to +243%; included in
[docs/imx335-v4-high-fps.md](docs/imx335-v4-high-fps.md) §6 with the
raw numbers.)

Three structural points the data exposes:

1. **The encoder's `VENC SEND` counter overstates deliverable fps.**
   At 480×352 the encoder produces ~290 fps internally but the RTP
   send path drains ~140 fps, and the encoder back-pressures down to
   match. The 290 fps "encoder ceiling" measurement is only achievable
   if the encoded frames are *discarded* (e.g. via RTSP, which drops
   frames at the socket boundary rather than throttling). For
   FPV-style outgoing-RTP — where every frame must reach the wire —
   the deliverable column is what counts.
2. **ev and gk match identically over UDP-RTP.** The ~30% gap visible
   under RTSP serving was an RTSP-stack artifact (ev burned more CPU
   on RTSP framing than gk did). Strip RTSP, push raw UDP, both boards
   produce exactly the same frames per second at the same CPU%. Same
   silicon; no binning.
3. **HMAX patch's value is real over UDP.** Baseline vs patched at the
   same crop / req-fps shows +43% at every measured point above
   480×352, dropping to +24% at 1280×720 (where the encoder rate
   ceiling — not the sensor — is the binding constraint).

Set `Isp_FrameRate` in the sensor INI to request a target rate; the
driver clamps to the per-mode sensor ceiling. Sensor delivers
whatever rate the HMAX register supports — requesting > the practical
ceiling still produces the ceiling (not a fallback).

Flex-crop ceiling is controlled by `HMAX = 0x0100` at
`libraries/sensor/hi3516ev200/sony_imx335/imx335_sensor_ctl.c:811`.
That value sweeps through an unstable band on the way down from the
prior 0x016E default; `0x0100` is the empirically-stable peak at
SYS_MODE=2 (891 Mbps lanes). The SYS_MODE=1 (1188 Mbps) path *might*
unlock another 20-30% headroom but requires more than the 3-register
patch we tried; see [docs/imx335-v4-high-fps.md](docs/imx335-v4-high-fps.md)
for the stability map, the failed SYS_MODE=1 attempt, the AE-library
u32FLStd race that causes encoder-side run-to-run variance, the
RTSP-vs-UDP delivery-rate comparison, and a self-contained
build/deploy recipe.

### IMX307 high-framerate modes (hi3516ev200 / gk7205v200)

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

## Kernel modules

All modules are prefixed `open_` to distinguish from vendor SDK modules. The set of modules varies by platform generation:

| Module | Function | V1 | V2 | V2A | V3 | V3A | V3.5 | V4 |
|--------|----------|---|---|---|---|---|---|---|
| `open_mmz` | Media memory zone | x | x | x | | | | |
| `open_himedia` | Media device framework | | x | x | | | | |
| `open_osal` | OS abstraction layer | | | | x | x | x | x |
| `open_sys_config` | Pin/clock configuration | | x | | x | x | x | x |
| `open_base` | Module registry, IPC | x | x | x | x | x | x | x |
| `open_sys` | System control | x | x | x | x | x | x | x |
| `open_isp` | Image Signal Processor | x | x | x | x | x | x | x |
| `open_vi` | Video input | x | x | x | x | x | x | x |
| `open_vpss` | Video processing | x | x | x | x | x | x | x |
| `open_venc` | Video encoder framework | x | x | x | x | x | x | x |
| `open_h264e` | H.264 codec engine | x | x | x | x | x | x | x |
| `open_h265e` | H.265 codec engine | | | x | x | x | x | x |
| `open_jpege` | JPEG encoder | x | x | x | x | x | x | x |
| `open_rc` | Rate control | x | x | x | x | x | x | x |
| `open_rgn` | OSD / region overlay | x | x | x | x | x | x | x |
| `open_vgs` | Video graphics / DSU | x | x | x | x | x | x | x |
| `open_ive` | Intelligent video engine | x | x | x | x | x | x | x |
| `open_ive_neo` | IVE clean-room (C source) | | | | | | | x |
| `open_tde` | 2D graphics engine | x | x | x | | x | x | x |
| `open_aio` | Audio I/O | | x | x | x | x | x | x |
| `open_ai` / `open_ao` | Audio input/output | x | x | x | x | x | x | x |
| `open_aenc` / `open_adec` | Audio encode/decode | x | x | x | x | x | x | x |
| `open_acodec` | On-chip audio codec | x | x | x | x | x | x | x |
| `open_mipi_rx` | MIPI CSI-2 receiver | | x | x | x | x | x | x |
| `open_sensor_i2c` | I2C sensor bus | x | x | x | x | x | x | x |
| `open_sensor_spi` | SPI sensor bus | | x | x | x | x | | x |
| `open_pwm` | PWM controller | x | x | x | x | x | x | x |
| `open_wdt` | Watchdog timer | x | x | x | x | x | x | x |
| `open_vo` | Video output | x | x | x | x | x | x | |
| `open_cipher` | Crypto engine | | x | | | | x | |
| `open_vedu` | Video encoder device | | | | x | x | x | x |
| `open_hidmac` | DMA controller | x | | | | | | |
| `open_vda` | Video detection analysis | x | | x | | | | |
| `open_ir` | Infrared receiver | | x | x | x | x | x | |
| `open_rtc` | Real-time clock | x | x | x | x | x | | |
| `open_pm` | Power management | | | | | x | | |
| `open_fisheye` | Fisheye correction | | | | | x | | |
| `open_nnie` | Neural network engine | | | | | | x | |
| `open_gdc` | Geometric distortion | | | | | | x | |
| `open_dis` | Digital stabilization | | | | | | x | |

### Module load order

Load order matters — see `/usr/bin/load_hisilicon` on the camera:

- **V1**: `mmz` → `base` → `sys` → everything else
- **V2/V2A**: `mmz` → `himedia` → (`sys_config`) → `base` → `sys` → everything else
- **V3/V3A/V3.5/V4**: `osal` → `sys_config` → `base` → `sys` → everything else

## User-space libraries (neo)

Three clean-room libraries replace vendor binaries for IVE motion detection, MMZ memory allocation, and object detection:

| Library | Replaces | API |
|---|---|---|
| `libmpi_neo.so` | `libmpi.so` | `HI_MPI_SYS_Init/Exit/MmzAlloc/MmzFree/MmzFlushCache` |
| `libive_neo.so` | `libive.so` | `HI_MPI_IVE_*` (19 IVE ops) + SVP/XNN helpers |
| `libivp_neo.so` | `libivp.so` | `hi_ivp_*` object-detection API |

These are **platform-agnostic** — the same binaries work across all CHIPARCH targets. The public headers match the vendor ABI byte-for-byte (struct sizes locked via `_Static_assert` in CI).

The `open_ive_neo.ko` kernel module (the driver that `libive_neo.so` talks to) is currently **hi3516ev200/ev300 only**. On other platforms, use the vendor `open_ive.ko` blob with `libive_neo.so` in userspace.

Build:

```bash
make -C libraries/mpi_neo CC=arm-openipc-linux-musleabi-gcc
make -C libraries/ive_neo CC=arm-openipc-linux-musleabi-gcc
make -C libraries/ivp_neo CC=arm-openipc-linux-musleabi-gcc
```

## CI

Every push and PR runs **22 jobs**:

**Build SDK** (8 platforms):

| Platform | Kernel | Generation |
|---|---|---|
| hi3516cv100_lite | 3.0.8 | V1 |
| hi3516cv200_lite | 4.9.37 | V2 |
| hi3516av100_lite | 4.9.37 | V2A |
| hi3516cv300_lite | 3.18.20 | V3 |
| hi3519v101_lite | 3.18.20 | V3A |
| hi3516cv500_lite | 4.9.37 | V3.5 |
| hi3516ev200_lite | 4.9.37 | V4 |
| gk7205v200_lite | 4.9.37 | V4/Goke |

Plus 3 mainline kernel builds for hi3516ev300_neo (6.6, 6.18, 7.0).

**QEMU boot smoke-tests** (8 platforms): CV100, CV200, AV100, CV300, 3519V101, CV500, EV200, GK7205V200 — boots OpenIPC firmware under [qemu-hisilicon](https://github.com/widgetii/qemu-hisilicon) and verifies the kernel reaches userspace.

**Library checks** (3 jobs): header syntax + ABI struct sizes, ARM cross-compile + symbol verification, QEMU IVE register regression.

[![Build Status](https://github.com/OpenIPC/openhisilicon/actions/workflows/build.yml/badge.svg)](https://github.com/OpenIPC/openhisilicon/actions/workflows/build.yml)

## License

- **Kernel modules**: GPL v3
- **Sensor libraries**: MIT (for use with OpenIPC and third-party software)
