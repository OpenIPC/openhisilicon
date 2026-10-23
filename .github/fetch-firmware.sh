#!/bin/sh
# fetch-firmware.sh <firmware> [variant]
#
# Unpack OpenIPC/firmware's latest openipc.<firmware>-nor-<variant>.tgz into
# the current directory as uImage.<firmware> and rootfs.squashfs.<firmware>,
# the files the QEMU jobs boot.
#
# A family that went split-only (hi3516ev300 lite) publishes only
# openipc.<firmware>-nor-<variant>-split.tgz, whose firmware.bin.<firmware> is
# the uImage followed by the squashfs at the next 64 KiB. That one is cut back
# into the two files.
set -eu

fw=$1
variant=${2:-lite}
base=https://github.com/OpenIPC/firmware/releases/download/latest

if curl -sSLf -o fw.tgz "$base/openipc.$fw-nor-$variant.tgz"; then
	tar xzf fw.tgz
else
	echo "openipc.$fw-nor-$variant.tgz not published, trying the split image"
	curl -sSLf -o fw.tgz "$base/openipc.$fw-nor-$variant-split.tgz"
	tar xzf fw.tgz
	python3 - "$fw" <<'EOF'
import struct, sys
fw = sys.argv[1]
data = open(f"firmware.bin.{fw}", "rb").read()
magic, = struct.unpack_from(">I", data, 0)
assert magic == 0x27051956, f"firmware.bin.{fw}: no uImage at offset 0"
end = 64 + struct.unpack_from(">I", data, 12)[0]  # header + ih_size
sq = (end + 0xFFFF) & ~0xFFFF
assert data[sq:sq + 4] == b"hsqs", f"no squashfs at 0x{sq:x}"
used, = struct.unpack_from("<Q", data, sq + 40)       # bytes_used
# A short image must fail here, not as a truncated file at boot.
assert sq + used <= len(data), \
    f"squashfs needs 0x{sq + used:x} bytes, firmware.bin.{fw} has 0x{len(data):x}"
open(f"uImage.{fw}", "wb").write(data[:end])
open(f"rootfs.squashfs.{fw}", "wb").write(data[sq:sq + used])
print(f"uImage.{fw}: {end} bytes, rootfs.squashfs.{fw}: {used} bytes at 0x{sq:x}")
EOF
fi
ls -l "uImage.$fw"
