#!/bin/sh
# Verify a local bootrom dump matches the tracked MANIFEST.
#
# Usage: verify.sh <chip>

set -eu

if [ $# -ne 1 ]; then
    echo "usage: $0 <chip>" >&2
    exit 64
fi

CHIP=$1
ROOT=$(cd "$(dirname "$0")/.." && pwd)
CHIPDIR=$ROOT/$CHIP
BIN=$CHIPDIR/bootrom.bin
MANIFEST=$CHIPDIR/MANIFEST

if [ ! -f "$BIN" ]; then
    echo "$0: $BIN missing — run dump.sh first" >&2
    exit 66
fi

if [ ! -f "$MANIFEST" ]; then
    echo "$0: $MANIFEST missing" >&2
    exit 66
fi

EXPECTED=$(awk '/^sha256:/ { print $2 }' "$MANIFEST")
ACTUAL=$(sha256sum "$BIN" | awk '{ print $1 }')

if [ "$EXPECTED" != "$ACTUAL" ]; then
    echo "FAIL: sha256 mismatch for $BIN" >&2
    echo "  expected $EXPECTED" >&2
    echo "  got      $ACTUAL" >&2
    exit 1
fi

EXPECTED_SIZE=$(awk '/^size:/ { print $2 }' "$MANIFEST")
ACTUAL_SIZE=$(wc -c < "$BIN")
if [ "$EXPECTED_SIZE" != "$ACTUAL_SIZE" ]; then
    echo "FAIL: size mismatch ($ACTUAL_SIZE vs $EXPECTED_SIZE)" >&2
    exit 1
fi

echo "OK: $BIN matches $MANIFEST"
