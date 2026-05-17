# nnie_re — vendor open_nnie.ko RE tooling

Reusable scaffolding for reverse-engineering the cv500/av300/dv300 NNIE
HW interface from vendor's `open_nnie.ko` + `libnnie.so`. Use when
extending `kernel/nnie_neo` / `libraries/nnie_neo` to support new
model topologies or HW features.

## Pieces

- `libdump_bbox.c` — LD_PRELOAD shim wrapping `ioctl(2)`. Intercepts
  `NNIE_IOC_FORWARD` (`0xc6584d00`) and `NNIE_IOC_FORWARD_WITH_BBOX`
  (`0xc6c04d01`), captures the user arg buffer + CTRL_S fields,
  sleeps 30s after the kernel returns so MMZ buffers stay live for
  inspection. Cross-build with the OpenIPC toolchain.

- `nnie_phys_dump/` — one-shot kernel module accepting `phys=<u64>`
  and `size=<u32>` module params. Calls `memremap(WB)` + `print_hex_dump`
  to dmesg, then exits. Avoids `/dev/mem` `STRICT_DEVMEM` blockers.

## Typical capture flow (target av300 / cv500 dev board)

```sh
# 1. Build host artefacts.
$CROSS_CC -O2 -Wall -fPIC -shared libdump_bbox.c -o libdump_bbox.so -ldl
make -C nnie_phys_dump KDIR=<linux-custom-build-dir> \
    ARCH=arm CROSS_COMPILE=arm-openipc-linux-gnueabi-

# 2. Push to board.
scp libdump_bbox.so nnie_phys_dump/nnie_phys_dump.ko root@board:/tmp/

# 3. Load vendor module + run a sample, snoop the ioctl.
ssh root@board '
    insmod /lib/modules/4.9.37/hisilicon/open_nnie.ko
    cd /path/to/vendor/sample
    LD_PRELOAD=/tmp/libdump_bbox.so ./sample_nnie_main a > /tmp/log.txt 2>&1 &
    sleep 5   # wait for the snoop to print PRE/POST + the 30s sleep to start
    PHYS=$(grep "PRE.*tsk=0x" /tmp/log.txt | grep -oE "tsk=0x[0-9a-f]+" | head -1 | sed s/tsk=//)
    SIZE=$(grep "PRE.*tsk=0x" /tmp/log.txt | grep -oE "tsk=0x[0-9a-f]+/[0-9]+" | head -1 | sed "s/.*\///")
    dmesg -c >/dev/null
    insmod /tmp/nnie_phys_dump.ko phys=$PHYS size=$SIZE
    dmesg | grep "dump " > /tmp/tskbuf.hex
    rmmod nnie_phys_dump
'
scp root@board:/tmp/tskbuf.hex .
```

## Caveats

- Writing the wrong tskbuf bytes back to HW via our driver can hang
  the NNIE block persistently — survives module reload, requires a
  full board reboot. Iterate on a dev board, not production.
- `nnie_phys_dump` requires the running kernel to have
  `CONFIG_MODULE_UNLOAD=y`. If it doesn't, vermagic at load time
  rejects the .ko with "version magic mismatch".
- `LD_PRELOAD` only works for dynamically-linked sample binaries.
  The vendor `sample_nnie_main` is dynamically linked against
  libnnie / libmpi / libsvpruntime, all of which are on the OpenIPC
  cv500 rootfs (`/usr/lib/libnnie.so`, etc).

## See also

- `kaeru://openhisilicon/nnie-neo-cv500-vendor-bbox-tskbuf-dump-2026-05-17`
  — first empirical dump of the bbox tskbuf from a pvanet run, plus
  partial layout decode (96 B header + 16-byte-per-anchor records).
- `kaeru://openhisilicon/nnie-neo-cv500-detector-tskbuf-pattern-2026-05-17`
  — the two libnnie tskbuf-management patterns and the CTRL_S
  layout decode from snooping mnist + SSD calls.
