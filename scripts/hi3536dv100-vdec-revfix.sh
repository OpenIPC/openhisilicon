#!/bin/sh
# hi3536dv100-vdec-revfix.sh -- let the SDK's vdec decode on revision-2 chips
#
# Usage: scripts/hi3536dv100-vdec-revfix.sh <readelf> <vdec.o>          patch
#        scripts/hi3536dv100-vdec-revfix.sh <readelf> <vdec.o> --check  verify
#
# Hi3536DV100_SDK_V2.0.2.0's hi3536dv100_vdec.ko reads one byte of the
# system controller at 0x12060024 in three places -- VDEC_AllocFrame,
# VDEC_SendPic and VDEC_GetImage -- and takes the normal path only when it
# is 0 or 1:
#
#     ldr   r3, [r3, #36]      @ 0x12060000 + 0x24, through osal_ioaddress
#     uxtb  r3, r3
#     cmp   r3, #1
#     bls   <normal path>      @ 9a xx xx xx
#
# Revision-2 chips (SC_SYSID0 0x12050EE0 bits 31:24 = 2) read a larger
# value there, so the module silently refuses every frame: channels are
# created, streams are accepted, and no picture is ever decoded, with no
# error anywhere. Making each of the three branches unconditional (bls ->
# b, condition 0x9 -> 0xe, one byte each) restores decoding; on revision-1
# chips the branch was taken anyway, so nothing changes for them.
#
# The match is by instruction pattern, not by offset, and must find exactly
# three sites inside .text: anything else (another SDK build, an object
# already patched) stops the script without touching the file. --check
# succeeds only for an object that is fully patched, for CI.
set -e

[ $# -ge 2 ] || { sed -n '4,5p' "$0" >&2; exit 1; }
READELF=$1 OBJ=$2 MODE=${3:-patch}

# .text file offset and size (readelf -SW: [Nr] Name Type Addr Off Size ...)
set -- $("$READELF" -SW "$OBJ" | awk '$2 == ".text" { print $5, $6; exit }
                                      $3 == ".text" { print $6, $7; exit }')
[ $# -eq 2 ] || { echo "$OBJ: no .text section" >&2; exit 1; }
text_off=$((0x$1)) text_end=$((0x$1 + 0x$2))

# Offsets of the condition byte of each "ldr r3,[r3,#36]; uxtb r3,r3;
# cmp r3,#1; b<cond>" whose condition is $1 (9a = bls, ea = b).
sites() {
	od -An -tx1 -v "$OBJ" | tr -s ' \n' '\n\n' | sed '/^$/d' | awk -v c="$1" '
		{ b[NR - 1] = $1 }
		END {
			p = "24 30 93 e5 73 30 ef e6 01 00 53 e3"
			n = split(p, w, " ")
			for (i = 0; i + 15 < NR; i++) {
				for (j = 1; j <= n; j++)
					if (b[i + j - 1] != w[j]) break
				if (j > n && b[i + 15] == c) print i + 15
			}
		}'
}

in_text() {
	for o in $1; do
		[ "$o" -ge "$text_off" ] && [ "$o" -lt "$text_end" ] || return 1
	done
}

todo=$(sites 9a) done=$(sites ea)
n_todo=$(echo $todo | wc -w) n_done=$(echo $done | wc -w)

if [ "$MODE" = --check ]; then
	if [ "$n_todo" -eq 0 ] && [ "$n_done" -eq 3 ] && in_text "$done"; then
		echo "$OBJ: revision check bypassed at 3 sites"
		exit 0
	fi
	echo "$OBJ: expected 3 bypassed revision checks and none left, found $n_done and $n_todo" >&2
	exit 1
fi

if [ "$n_todo" -ne 3 ] || [ "$n_done" -ne 0 ] || ! in_text "$todo"; then
	echo "$OBJ: expected 3 unpatched revision checks in .text, found $n_todo (and $n_done patched); not touching it" >&2
	exit 1
fi
for o in $todo; do
	printf '\352' | dd of="$OBJ" bs=1 seek="$o" conv=notrunc 2>/dev/null
done
echo "$OBJ: revision check bypassed at 3 sites"
