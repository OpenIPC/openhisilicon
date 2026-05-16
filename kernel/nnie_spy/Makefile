# Standalone build of the debug-only nnie_spy.ko.
# Usage:
#   make -C /path/to/cv500/kernel/build M=$(pwd) \
#        ARCH=arm CROSS_COMPILE=arm-openipc-linux-gnueabi- modules
#
# Requires the cv500 kernel build tree to be configured with
# CONFIG_KPROBES=y (already true on the av300 4.9.37 ship build).

obj-m += nnie_spy.o
