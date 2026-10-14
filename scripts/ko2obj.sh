#!/bin/sh
# ko2obj.sh -- turn a complete vendor .ko into a relinkable module object
#
# Usage: scripts/ko2obj.sh <objcopy> <prefix> <in.ko> <out.o>
#   e.g. scripts/ko2obj.sh arm-linux-gnueabi-objcopy gk7206 xm_vi.ko vi.o
#
# Some SDKs (the XMedia "Lotus" SDK behind gk7206v100) ship their closed
# modules only as finished .ko files. Kbuild cannot relink one of those as-is:
# it already carries a struct module (.gnu.linkonce.this_module) and a
# .modinfo with the SDK kernel's vermagic, so the module modpost builds around
# it would have two of each.
#
# .modinfo is dropped. The struct module cannot simply be dropped too: a blob
# that uses THIS_MODULE (fops .owner, platform_driver .owner, ...) relocates
# against __this_module, and objcopy refuses to leave that symbol behind
# without its section. Keeping it as a linkonce section, as the hi3516cv200
# objects do, makes ld keep whichever copy it sees first -- the blob's, laid
# out for the SDK kernel's struct module, so the module only loads while this
# kernel's config matches the SDK's field for field. Instead the section
# becomes ordinary data and __this_module turns weak: the strong definition in
# modpost's <mod>.mod.o wins, every THIS_MODULE in the blob points at the real
# struct module, and the blob's copy is a few hundred dead bytes.
#
# init_module/cleanup_module become _<prefix>_<mod>_init/_exit, which the
# wrapper in kernel/init/<chiparch>/ calls from its own module_init/
# module_exit. <mod> is the .ko basename without its xm_/hi_/gk_ prefix.
#
# Left intact on purpose:
#   __ksymtab*  the blob's EXPORT_SYMBOLs survive, so the wrapper must not
#               re-export them (modpost fails on a duplicate export).
#               A vendor `strip --strip-unneeded` does take away the local
#               __ksymtab_<sym> symbols, though, and those are all modpost
#               looks at to learn what a module exports: without them every
#               module importing from this one fails "undefined!". They are
#               put back, one per 12-byte struct kernel_symbol (32-bit, no
#               PREL32) in the merged __ksymtab and __ksymtab_gpl, named
#               after the symbol its value relocates against. modpost files
#               them as "(unknown)" rather than EXPORT_SYMBOL/_GPL, since the
#               section is no longer ___ksymtab+<sym>; that only feeds its
#               GPL check.
#   __param     module_param()s keep working under the new module name; a
#               wrapper that re-declares one gets a duplicate parameter.
#
# Every output also needs a .<out>.o.cmd stub next to it (see CLAUDE.md);
# this script writes one.
set -e

[ $# -eq 4 ] || { sed -n '2,4p' "$0" >&2; exit 1; }
OBJCOPY=$1 PREFIX=$2 IN=$3 OUT=$4

mod=$(basename "$IN" .ko)
mod=${mod#xm_}; mod=${mod#hi_}; mod=${mod#gk_}

READELF=${OBJCOPY%objcopy}readelf
# Relocation sections print as 'Relocation section '.rel__ksymtab' at ...';
# the section a symbol goes back into is the name without the quotes and .rel.
syms=$("$READELF" -rW "$IN" | awk '
	function hex(s,    i, n) {
		n = 0
		for (i = 1; i <= length(s); i++)
			n = n * 16 + index("0123456789abcdef", substr(s, i, 1)) - 1
		return n
	}
	/^Relocation section/ {
		sec = $3
		gsub(/\047/, "", sec)
		sub(/^\.rel/, "", sec)
		f = (sec == "__ksymtab" || sec == "__ksymtab_gpl")
		next
	}
	f && $1 ~ /^[0-9a-f]+$/ && hex($1) % 12 == 0 {
		printf " --add-symbol __ksymtab_%s=%s:%d,object,local", $5, sec, hex($1)
	}')

# shellcheck disable=SC2086 # $syms is a list of options
"$OBJCOPY" $syms \
	--rename-section .gnu.linkonce.this_module=.data.vendor_this_module \
	--weaken-symbol=__this_module \
	--remove-section=.modinfo \
	--remove-section=.note.Linux \
	--redefine-sym init_module=_${PREFIX}_${mod}_init \
	--redefine-sym cleanup_module=_${PREFIX}_${mod}_exit \
	"$IN" "$OUT"

# kernel/obj/<chiparch>/... relative to the repo, for the savedcmd line
rel=$(realpath --relative-to="$(dirname "$0")/.." "$OUT")
rel=${rel#kernel/}
printf 'savedcmd_kernel/%s := :\n' "$rel" > "$(dirname "$OUT")/.$(basename "$OUT").cmd"
