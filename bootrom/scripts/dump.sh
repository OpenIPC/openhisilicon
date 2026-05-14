#!/bin/sh
# Dump a HiSilicon mask-ROM from a live camera via ipctool, then verify
# against the tracked MANIFEST.
#
# Usage: dump.sh <ssh-host> <chip>
#
# Requires: ssh access to <ssh-host> as root, ipctool with `bootrom`
# subcommand on the camera (in PATH or via OpenIPC's auto-fetch shim).

set -eu

if [ $# -ne 2 ]; then
    echo "usage: $0 <ssh-host> <chip>" >&2
    exit 64
fi

HOST=$1
CHIP=$2
ROOT=$(cd "$(dirname "$0")/.." && pwd)
CHIPDIR=$ROOT/$CHIP

if [ ! -d "$CHIPDIR" ]; then
    echo "$0: $CHIPDIR does not exist" >&2
    exit 65
fi

OUT=$CHIPDIR/bootrom.bin
ssh -o BatchMode=yes "root@$HOST" 'ipctool bootrom --dump' > "$OUT"

SIZE=$(wc -c < "$OUT")
echo "wrote $OUT ($SIZE bytes)"

exec "$ROOT/scripts/verify.sh" "$CHIP"
