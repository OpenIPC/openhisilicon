# OpenHisilicon

Open-source kernel modules and sensor libraries for HiSilicon/Goke IP camera SoCs.

Replaces the proprietary SDK that HiSilicon ships to camera manufacturers. Used by [OpenIPC](https://openipc.org/) firmware and can be used independently in your own projects.

## Supported hardware

| SoC family | Chip IDs | CHIPARCH | SDK_CODE |
|------------|----------|----------|----------|
| HiSilicon hi3516ev200 | hi3516ev200, hi3516ev300, hi3518ev300, hi3516dv200 | `hi3516ev200` | `0x3516E200` |
| Goke gk7205v200 | gk7205v200, gk7205v300, gk7202v300, gk7605v100 | `gk7205v200` | `0x7205200` |

HiSilicon and Goke chips are pin-compatible — the same source code compiles for both via the `CHIPARCH` flag. A [conversion script](scripts/hi2gk.sh) can translate between the two SDK naming conventions.

## Supported kernels

| Kernel | Status | Notes |
|--------|--------|-------|
| 4.9.37 (vendor) | Production | Used by `hi3516ev300_lite`, `gk7205v200_lite` |
| 6.6 LTS | Production | Used by `hi3516ev300_neo` |
| 6.18 LTS | Tested | CI-verified |
| 7.0-rc6 (mainline) | Production | Used by `hi3516ev300_neo` |

All kernel versions are supported from a single codebase via the compatibility layer in [`kernel/compat/`](kernel/compat/). No `#ifdef` soup in driver code — the compat headers handle API differences transparently.

## Repository structure

```
├── kernel/                  Kernel modules (out-of-tree, loaded at runtime)
│   ├── osal/                OS Abstraction Layer (memory, proc, device mgmt)
│   ├── base/                Base module (module registry, IPC)
│   ├── sys/                 System control
│   ├── isp/                 Image Signal Processor
│   ├── vi/                  Video input
│   ├── vpss/                Video processing subsystem
│   ├── venc/                Video encoder framework
│   ├── h264e/, h265e/       H.264/H.265 codec engines
│   ├── jpege/               JPEG encoder
│   ├── rgn/                 OSD / region overlay
│   ├── vgs/                 Video graphics subsystem
│   ├── ive/                 Intelligent video engine (motion detection)
│   ├── ai/, ao/, aio/       Audio input/output
│   ├── aenc/, adec/         Audio encode/decode
│   ├── acodec/              Audio codec
│   ├── mipi_rx/             MIPI CSI-2 receiver
│   ├── sensor_i2c/          I2C sensor bus driver
│   ├── sensor_spi/          SPI sensor bus driver
│   ├── pwm/                 PWM controller (DC-iris)
│   ├── wdt/                 Watchdog timer
│   ├── compat/              Kernel version compatibility (4.9 – 7.0)
│   └── ext_inc/             Cross-module function export headers
│
├── libraries/               User-space libraries
│   ├── sensor/              Image sensor drivers (30 sensors)
│   └── isp/                 ISP algorithm libraries
│
├── include/                 Shared headers (kernel + userspace)
└── scripts/                 Build utilities
```

## Building

### Via OpenIPC firmware (recommended)

```bash
# Clone firmware
git clone --depth 1 https://github.com/openipc/firmware
cd firmware

# Build for HiSilicon (4.9 kernel)
make BOARD=hi3516ev300_lite br-hisilicon-opensdk

# Build for Goke (4.9 kernel)
make BOARD=gk7205v200_lite br-hisilicon-opensdk

# Build for mainline kernel (7.0)
make BOARD=hi3516ev300_neo br-hisilicon-opensdk
```

The firmware build system handles the toolchain, kernel headers, and all dependencies automatically.

### Standalone (for development)

If you already have an OpenIPC build tree at `/path/to/firmware`:

```bash
# Kernel modules
make -C /path/to/firmware/output/build/linux-custom \
    M=$(pwd)/kernel \
    CHIPARCH=hi3516ev200 \
    DISABLE_IST=1 DISABLE_PM=1 DISABLE_TDE=1 DISABLE_VO=1 \
    modules

# Sensor libraries
export SDK_CODE=0x3516E200
make CROSS_COMPILE=arm-openipc-linux-musleabi- \
    CHIPARCH=hi3516ev200 \
    -C libraries all
```

### Deploy to camera

```bash
# Copy a sensor driver
scp libraries/sensor/hi3516ev200/sony_imx335/libsns_imx335.so \
    root@192.168.1.10:/usr/lib/sensors/

# Copy a kernel module
scp kernel/open_isp.ko \
    root@192.168.1.10:/lib/modules/$(uname -r)/updates/
```

## Supported sensors

| Manufacturer | Sensors |
|-------------|---------|
| **Sony** | imx290, imx307, imx327, imx335 (+ FPV variant) |
| **SmartSens** | sc2231, sc2235, sc223a, sc3235, sc3335, sc3336, sc4236, sc500ai |
| **GalaxyCore** | gc2053, gc4023, gc4653, gc5603 |
| **OmniVision** | os02g10, os05a, ov2718 |
| **SOI** | f37, h63 |
| **ImageDesign** | mis2008 |
| **SuperPix** | sp2305 |
| **Aptina** | ar0237 |

Each sensor has both `.so` (shared) and `.a` (static) library builds.

## Kernel modules

All modules are prefixed `open_` to distinguish from vendor SDK modules:

| Module | Function |
|--------|----------|
| `open_osal` | OS abstraction layer, MMZ memory allocator |
| `open_sys_config` | SoC pin/clock configuration |
| `open_base` | Module registry and inter-module calls |
| `open_sys` | System control and scaling coefficients |
| `open_isp` | Image Signal Processor pipeline |
| `open_vi` | Video input (sensor → ISP path) |
| `open_vpss` | Video processing (crop, scale, rotate) |
| `open_venc` | Video encoder framework |
| `open_h264e` / `open_h265e` | H.264 / H.265 codec engines |
| `open_jpege` | JPEG encoder |
| `open_rgn` | OSD regions and overlays |
| `open_vgs` | Video graphics operations |
| `open_ive` | Intelligent video (motion detection, CNN) |
| `open_ai` / `open_ao` / `open_aio` | Audio input/output |
| `open_aenc` / `open_adec` | Audio encode/decode |
| `open_acodec` | On-chip audio codec |
| `open_mipi_rx` | MIPI CSI-2 receiver |
| `open_sensor_i2c` / `open_sensor_spi` | Sensor bus drivers |
| `open_pwm` | PWM (DC-iris lens control) |
| `open_wdt` | Watchdog timer |

Load order matters — `open_osal` → `open_sys_config` → `open_base` → `open_sys` → everything else. See `/usr/bin/load_hisilicon` on the camera for the full sequence.

## FPV sensor modes (IMX335)

High-framerate modes for FPV applications:

| Mode | Resolution | Max FPS (hi3516ev300) | Max FPS (gk7205v300) |
|------|-----------|----------------------|---------------------|
| Cropped 16:9 | 2592x1520 | 50 fps | 47 fps |
| Full scale boosted | 2592x1944 | 39 fps | 33 fps |
| Cropped 1.5x zoom | 1920x1080 | 55 fps | — |
| Binning | 1296x972 | 65 fps | — |
| Stock full scale | 2592x1944 | 30 fps | 30 fps |

Configure via `Isp_FrameRate` in majestic settings.

## Kernel compatibility layer

The [`kernel/compat/kernel_compat.h`](kernel/compat/kernel_compat.h) header provides transparent API compatibility across Linux 4.9 through 7.0, covering:

- Timer API (`init_timer` → `timer_setup` → `timer_delete`)
- Memory mapping (`ioremap_nocache` removal, `set_fs` removal)
- SPI/I2C API renames and signature changes
- `platform_driver.remove` return type change (int → void)
- sysctl, proc_ops, vm_flags, mmap_lock API evolution
- CMA/DMA contiguous allocator interface changes

## CI

Every push and PR is tested against:
- hi3516ev200 + kernel 4.9 (HiSilicon)
- gk7205v200 + kernel 4.9 (Goke)
- hi3516ev300 + kernel 6.6, 6.18, 7.0 (mainline)

[![Build Status](https://github.com/OpenIPC/openhisilicon/actions/workflows/build.yml/badge.svg)](https://github.com/OpenIPC/openhisilicon/actions/workflows/build.yml)

## License

- **Kernel modules**: GPL v3
- **Sensor libraries**: MIT (for use with OpenIPC and third-party software)
