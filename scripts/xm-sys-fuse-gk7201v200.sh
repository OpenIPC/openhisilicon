#!/bin/sh
# Let the XMedia SPC020 V200 sys module accept the GK7201V200 fuse codes.
#
# SYS_ModInit gates on the efuse byte at 0x100a0028: it takes bits [7:4]
# and accepts only 1..8 and 10..12, printing "SDK version do NOT support
# this chip!!!" otherwise. GK7201V200 fuses 0xD0/0xD4 (nibble 13), which
# the vendor's later (Jan 2024) sys accepts but SPC020 predates.
#
# The last whitelist entry is rewritten in place, same length:
#   ubfx r3, r3, #4, #4 ; cmp r3, #12     (bits[7:4] == 12)
# ->ubfx r3, r3, #5, #3 ; cmp r3, #6      (bits[7:5] == 6: nibble 12 or 13)
# so every chip accepted before still is, plus nibble 13.
#
# usage: xm-sys-fuse-gk7201v200.sh <sys.o in> <sys.o out>
set -e
in=$1 out=$2
# .text starts at 0x34 in this object; SYS_ModInit+0x212 = .text+0x1d92
off=$((0x34 + 0x1d92))
old=c3f303130c2b
new=c3f34213062b

# The V500 sys.o carries the same bytes at the same place, so pin the
# exact object rather than trusting the site alone.
sum=$(md5sum "$in" | cut -c1-32)
if [ "$sum" != e018c3997eebd296dda9f6673c69c7bc ]; then
	echo "$0: $in is not the SPC020 V200 sys.o (md5 $sum)" >&2
	exit 1
fi
got=$(od -An -tx1 -j "$off" -N 6 "$in" | tr -d ' \n')
[ "$got" = "$old" ] || { echo "$0: $in: expected $old at $off, found $got" >&2; exit 1; }
cp "$in" "$out.tmp"
printf '\303\363\102\023\006\053' | dd of="$out.tmp" bs=1 seek="$off" conv=notrunc status=none
mv "$out.tmp" "$out"
