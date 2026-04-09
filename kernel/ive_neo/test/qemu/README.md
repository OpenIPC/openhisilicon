# IVE register regression test (QEMU)

This directory contains a **vendored copy** of the IVE op register test
from the upstream QEMU machine-model project, used as a regression test
for `kernel/ive_neo/ive_neo.c`. It programs the IVE registers directly
via `/dev/mem` and compares output checksums against known-good values.

**Important caveat**: the test uses `/dev/mem` directly and does **not**
load either `open_ive.ko` (vendor) or `open_ive_neo.ko` (clean-room).
It exercises the IVE **register contract** shared between the driver
and the silicon/QEMU model. If `ive_neo.c` ever hardcodes a wrong
register offset the test won't catch it unless QEMU's model is updated
in lock-step. Full driver-level validation still requires real
hardware runs (see the "Real hardware regression" section of the PR
description).

Running it under `qemu-system-arm -M hi3516ev300` exercises the QEMU
HiSilicon IVE machine model, which is the reference implementation of
what the real hardware does. Drift between the QEMU model and whatever
register layout the driver expects will fail this test.

## Upstream

- **Source**: `widgetii/qemu-hisilicon`, path `qemu-boot/test-ive-ops.c`
- **Last synced**: commit `9e1ea1d` ("Add IVE register tests to CI:
  19 ops verified on every push")
- **Upstream license**: same as the clean-room driver (GPL-2.0)

If the upstream changes, run:
```sh
cp /path/to/qemu-hisilicon/qemu-boot/test-ive-ops.c .
```
and update this README with the new commit SHA.

## How it works

`test-ive-ops.c` is designed to run as PID 1 in a minimal cpio initramfs.
It:
1. Mounts `/proc` and `/sys`
2. Opens `/dev/mem` and mmaps the IVE register block at `0x11320000`
3. Allocates DMA-capable buffers via `mmap` + `pagemap` v→p translation
4. Programs 19+ IVE ops (DMA, SAD, CCL, Sub, Add, Thresh, Hist, Filter,
   Sobel, Dilate, Erode, Integ, Map, NCC, 16to8, OrdStat, MagAndAng,
   CannyEdge) with synthetic 64×64 test patterns
5. Checksums each op's output and prints `PASS` or `FAIL`
6. Prints `Result: N/N passed` and calls `reboot(LINUX_REBOOT_CMD_POWER_OFF)`

## Building and running locally

```sh
# Build the static ARM binary + cpio:
make                        # uses arm-linux-gnueabi-gcc
make CC=arm-openipc-linux-musleabi-gcc   # or the musl cross-compiler

# Run under QEMU (needs qemu-system-arm with HiSilicon machines):
QEMU=/path/to/qemu-system-arm \
KERNEL=/path/to/uImage.hi3516ev300 \
./run-qemu.sh
```

You can get `qemu-system-arm` with the HiSilicon machine models from
[widgetii/qemu-hisilicon](https://github.com/widgetii/qemu-hisilicon).
An `openipc.hi3516ev300-nor-lite.tgz` with a matching kernel is in
every OpenIPC firmware release.

## Running in CI

`.github/workflows/build.yml` has a `qemu-ive-ops` job that:
1. Builds `qemu-system-arm` from `widgetii/qemu-hisilicon` (cached)
2. Downloads an `openipc.hi3516ev300-nor-lite.tgz` release tarball
3. Runs `make` in this directory to build the cpio
4. Launches QEMU with the kernel + cpio and greps for `Result: N/N passed`

If the test ever prints `Result: 18/19 passed` or similar, CI turns red
and the PR is blocked until `ive_neo.c` is fixed or the test expectation
is explicitly updated.
