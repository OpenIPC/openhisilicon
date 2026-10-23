#!/bin/bash
# qemu-load-modules.sh <qemu> <machine> <uImage> <rootfs.squashfs> <moddir> <allocator>
#
# Boot <uImage> under qemu-hisilicon with <rootfs.squashfs>, its
# lib/modules/*/hisilicon/*.ko swapped for the ones in <moddir> (this build's),
# on mmz_allocator=<allocator> (ot or cma), and fail unless open_mmz loads.
# modpost only proves a symbol is exported by something; this proves the
# modules link and initialise against the kernel they were built for
# (openhisilicon#275: open_mmz built, then did not load: "Unknown symbol
# init_mm"). The modules that load on top of it are reported, not required.
set -euo pipefail

qemu=$1 machine=$2 kernel=$3 rootfs=$4 moddir=$5 allocator=$6
"$qemu" --version >/dev/null || { echo "FAIL: $qemu does not run" >&2; exit 1; }
work=$(mktemp -d)
trap 'sudo rm -rf "$work"' EXIT

sudo unsquashfs -q -d "$work/rfs" "$rootfs" >/dev/null
hisi=$(echo "$work"/rfs/lib/modules/*/hisilicon)
[ -d "$hisi" ] || { echo "FAIL: no lib/modules/*/hisilicon in $rootfs" >&2; exit 1; }
n=0
for ko in "$moddir"/*.ko; do
	sudo cp "$ko" "$hisi/"
	n=$((n + 1))
done
echo "swapped in $n modules from $moddir"

# Runs after S95majestic. With no sensor (QEMU has none) load_hisilicon
# detects nothing and unloads its detection set again, so the probe loads
# open_osal and open_mmz itself, with the parameters the kernel published for
# it, as load_hisilicon's insert_osal() does, and then the modules on top that
# need no sensor.
sudo tee "$work/rfs/etc/init.d/S99loadprobe" >/dev/null <<'EOS'
#!/bin/sh
[ "$1" = start ] || exit 0
p=/sys/module/hisi_mem/parameters
echo "===CI:ALLOCATOR===$(cat $p/allocator 2>/dev/null)==="
if ! lsmod | grep -q '^open_mmz '; then
	modprobe open_osal
	modprobe open_mmz anony=1 mmz_allocator="$(cat $p/allocator)" mmz="$(cat $p/mmz)"
fi
lsmod | grep -q '^open_mmz ' && echo "===CI:OPEN_MMZ===loaded==="
for m in open_base open_sys open_vb open_rgn; do
	modprobe $m && echo "===CI:LOADED===$m==="
done
dmesg | grep 'Unknown symbol'
cat /proc/umap/media-mem 2>/dev/null | grep -E 'ZONE|MMB' | head -5
echo "===CI:DONE==="
EOS
sudo chmod +x "$work/rfs/etc/init.d/S99loadprobe"
# A stored MAC, so check_mac does not block on the guest CRNG (see qemu-boot).
echo 02:00:00:c1:00:01 | sudo tee "$work/rfs/etc/ethaddr" >/dev/null
sudo mksquashfs "$work/rfs" "$work/rootfs.squashfs" -comp xz -all-root -noappend -no-progress >/dev/null

log=qemu-load-$allocator.txt
timeout 240 "$qemu" -M "$machine" -m 128M -kernel "$kernel" -initrd "$work/rootfs.squashfs" \
	-nographic -serial mon:stdio \
	-append "console=ttyAMA0,115200 panic=20 root=/dev/ram0 rootfstype=squashfs mmz_allocator=$allocator" \
	</dev/null >"$log" 2>&1 &
pid=$!
for _ in $(seq 220); do
	grep -qa '===CI:DONE===' "$log" && break
	kill -0 $pid 2>/dev/null || break
	sleep 1
done
kill $pid 2>/dev/null || true
wait $pid 2>/dev/null || true

fail() { echo "FAIL ($machine, $allocator): $1" >&2; grep -a -E 'Unknown symbol|wrong|Oops|panic|alignment|hisi_mem|mmz' "$log" | head -20 >&2; exit 1; }
grep -qa '===CI:DONE===' "$log" || fail "no probe marker: the boot did not reach S99"
! grep -qa 'Unknown symbol' "$log" || fail "a module has an unresolved symbol"
! grep -qaE 'Internal error: Oops|Kernel panic|Unable to handle kernel|Alignment trap' "$log" || fail "kernel fault"
! grep -qaF "Error: There's something wrong, please check!" "$log" || fail "load_hisilicon reported an error"
grep -qa "===CI:ALLOCATOR===$allocator===" "$log" || fail "allocator is not $allocator"
grep -qa '===CI:OPEN_MMZ===loaded===' "$log" || fail "open_mmz did not load"
loaded=$(grep -oa 'CI:LOADED===open_[a-z]*' "$log" | sed 's/.*===//' | tr '\n' ' ')
echo "ok ($machine, $allocator): open_mmz loaded; on top: ${loaded:-none}"
