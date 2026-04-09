#!/bin/bash
# SPDX-License-Identifier: GPL-2.0
#
# run-qemu.sh — build test-ive-ops.cpio and boot it under qemu-system-arm
# on an emulated Hi3516EV300. Asserts "Result: N/N passed" in the output.
#
# Requirements:
#   - qemu-system-arm binary with HiSilicon machine models
#     (from widgetii/qemu-hisilicon; not bundled here).
#     Pass the path via QEMU=... or make sure it's on PATH.
#   - uImage.hi3516ev300 kernel image (from OpenIPC firmware release).
#     Pass via KERNEL=... or place it next to this script.
#   - arm-linux-gnueabi-gcc (or arm-openipc-linux-musleabi-gcc via CC=).
#
# Usage:
#   ./run-qemu.sh
#   QEMU=/path/to/qemu-system-arm KERNEL=/path/to/uImage.hi3516ev300 ./run-qemu.sh
#
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
QEMU="${QEMU:-qemu-system-arm}"
KERNEL="${KERNEL:-$SCRIPT_DIR/uImage.hi3516ev300}"
TIMEOUT_SEC="${TIMEOUT_SEC:-60}"

if ! command -v "$QEMU" >/dev/null 2>&1 && [ ! -x "$QEMU" ]; then
    echo "ERROR: qemu-system-arm not found. Set QEMU=... to point at it." >&2
    exit 1
fi
if [ ! -f "$KERNEL" ]; then
    echo "ERROR: kernel image not found at $KERNEL" >&2
    echo "Hint: download openipc.hi3516ev300-nor-lite.tgz from OpenIPC releases" >&2
    exit 1
fi

cd "$SCRIPT_DIR"
make >/dev/null

OUTPUT="$(mktemp)"
trap 'rm -f "$OUTPUT"' EXIT

timeout "$TIMEOUT_SEC" "$QEMU" \
    -M hi3516ev300 -m 128M \
    -kernel "$KERNEL" \
    -initrd "$SCRIPT_DIR/test-ive-ops.cpio" \
    -nographic -serial mon:stdio \
    -append "console=ttyAMA0,115200 mem=96M mmz_allocator=hisi mmz=anonymous,0,0x46000000,32M" \
    2>&1 | tee "$OUTPUT" || true

echo
if grep -qE "Result: [0-9]+/[0-9]+ passed" "$OUTPUT"; then
    RESULT=$(grep -oE "Result: [0-9]+/[0-9]+ passed" "$OUTPUT" | tail -1)
    echo "=== $RESULT ==="
    exit 0
else
    echo "=== TEST FAILED: no Result: line in output ==="
    tail -20 "$OUTPUT"
    exit 1
fi
