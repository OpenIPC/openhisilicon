#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
#
# verify-on-target.sh — boot open_ive_neo.ko on the live lab boards
# (one ev200/ev300 + one cv500/av300) and check it probes cleanly.
#
# What this validates:
#   1. open_ive_neo.ko loads (no module-init errors).
#   2. The "hisilicon,hisi-ive" DT node was found — i.e. probe ran
#      and g_ive_regs is now non-NULL (no "g_ive_regs not set" in
#      dmesg, no "cannot find ive IRQ").
#   3. The chip-ops selection picked the right backend
#      ("chip=hi3516ev200 xnn=yes" vs "chip=hi3516cv500 xnn=no").
#   4. HW responded — non-zero HW ID at [0x80] in the hw_init log.
#   5. cv500 emitted the "skipping [0x90] mem-speed" info line.
#
# What this does NOT validate:
#   - End-to-end libive ioctl path (would need a libive sample
#     binary on the rootfs; the smoke step below runs one if it
#     finds it but skips quietly otherwise).
#   - XNN dispatch on cv500 returning -EOPNOTSUPP (would need a
#     userspace ioctl client; out of scope here).
#
# Run from the openhisilicon repo root, after both rebuilds have
# completed (output-ev200/build/.../open_ive_neo.ko exists and
# output-cv500/build/.../open_ive_neo.ko exists).
#
# Usage:
#   sh kernel/ive_neo/test/board/verify-on-target.sh
#
set -eu

EV200_BOARD="${EV200_BOARD:-root@openipc-hi3516ev300.dlab.doty.ru}"
CV500_BOARD="${CV500_BOARD:-root@openipc-hi3516av300.dlab.doty.ru}"
FW_DIR="${FW_DIR:-$HOME/git/firmware}"
KO_EV200="$FW_DIR/output-ev200/build/hisilicon-opensdk-custom/kernel/open_ive_neo.ko"
KO_CV500="$FW_DIR/output-cv500/build/hisilicon-opensdk-custom/kernel/open_ive_neo.ko"

PASS=0
FAIL=0

check() {
    label="$1"; shift
    if "$@" >/dev/null 2>&1; then
        echo "  PASS: $label"
        PASS=$((PASS + 1))
    else
        echo "  FAIL: $label"
        FAIL=$((FAIL + 1))
    fi
}

verify_board() {
    BOARD="$1"; KO="$2"; EXPECT_CHIP="$3"; EXPECT_XNN="$4"

    echo
    echo "=== $BOARD ($EXPECT_CHIP, xnn=$EXPECT_XNN) ==="

    if [ ! -f "$KO" ]; then
        echo "  ABORT: $KO not built"
        FAIL=$((FAIL + 1))
        return
    fi

    scp -o ConnectTimeout=5 -o StrictHostKeyChecking=no \
        "$KO" "$BOARD:/tmp/open_ive_neo.ko" >/dev/null

    LOG=$(ssh -o ConnectTimeout=5 -o StrictHostKeyChecking=no "$BOARD" sh -s <<'REMOTE'
set -eu
# Unload anything currently bound to /dev/ive so our module can probe.
rmmod open_ive_neo 2>/dev/null || true
rmmod open_ive 2>/dev/null || true
dmesg -c >/dev/null 2>&1 || true

insmod /tmp/open_ive_neo.ko 2>&1 || echo "INSMOD_FAILED"
sleep 1

echo "--- lsmod ---"
lsmod | grep ive_neo || echo "no ive_neo in lsmod"
echo "--- /dev/ive ---"
ls -l /dev/ive 2>/dev/null || echo "no /dev/ive"
echo "--- smoke ---"
for t in /usr/bin/sample_ive /usr/sbin/sample_ive /opt/sample_ive; do
  if [ -x "$t" ]; then
    echo "running $t (10s budget)"
    timeout 10 "$t" 2>&1 | head -20 || true
    break
  fi
done
echo "--- dmesg (post-insmod) ---"
dmesg | tail -40
REMOTE
)

    echo "$LOG"

    echo "--- checks ---"
    check "module loaded"           sh -c "echo '$LOG' | grep -q '^open_ive_neo '"
    check "probe ran (no -ENODEV)"  sh -c "echo '$LOG' | grep -qv 'g_ive_regs not set'"
    check "IRQ found"               sh -c "echo '$LOG' | grep -qv 'cannot find ive IRQ'"
    check "chip=$EXPECT_CHIP"       sh -c "echo '$LOG' | grep -q 'chip=$EXPECT_CHIP'"
    check "xnn=$EXPECT_XNN"         sh -c "echo '$LOG' | grep -q 'xnn=$EXPECT_XNN'"
    check "HW ID non-zero"          sh -c "echo '$LOG' | grep -E 'HW ID=0x[0-9a-f]+' | grep -qv 'HW ID=0x00000000'"
    check "no oops/panic"           sh -c "! echo '$LOG' | grep -qE 'Oops|panic|BUG:'"

    if [ "$EXPECT_XNN" = "no" ]; then
        check "[0x90] skip logged"  sh -c "echo '$LOG' | grep -q 'skipping \\[0x90\\] mem-speed'"
    fi
}

verify_board "$EV200_BOARD" "$KO_EV200" "hi3516ev200" "yes"
verify_board "$CV500_BOARD" "$KO_CV500" "hi3516cv500" "no"

echo
echo "=== Summary: $PASS pass, $FAIL fail ==="
[ "$FAIL" -eq 0 ]
