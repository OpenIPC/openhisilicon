cmd_/home/dima/git/openhisilicon/kernel/isp/kernel/arch/gk7205v200/hal/isp_drv.o := arm-openipc-linux-musleabi-gcc -Wp,-MD,/home/dima/git/openhisilicon/kernel/isp/kernel/arch/gk7205v200/hal/.isp_drv.o.d -nostdinc -isystem /home/dima/git/buildroot_toolchain/hi3516ev300/bin/../lib/gcc/arm-openipc-linux-musleabi/7.5.0/include -I./arch/arm/include -I./arch/arm/include/generated/uapi -I./arch/arm/include/generated  -I./include -I./arch/arm/include/uapi -I./include/uapi -I./include/generated/uapi -include ./include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-goke/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -std=gnu89 -fno-PIE -fno-dwarf2-cfi-asm -fno-omit-frame-pointer -mapcs -mno-sched-prolog -fno-ipa-sra -mabi=aapcs-linux -mno-thumb-interwork -mfpu=vfp -marm -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -fno-delete-null-pointer-checks -Wno-frame-address -O2 --param=allow-store-data-races=0 -DCC_HAVE_ASM_GOTO -Wframe-larger-than=1024 -fstack-protector-strong -Wno-unused-but-set-variable -Wno-unused-const-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-var-tracking-assignments -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -Werror=date-time -Werror=incompatible-pointer-types -DRELEASE -DUSER_BIT_32 -DKERNEL_BIT_32 -Wno-date-time -D_GNU_SOURCE -I/home/dima/git/openhisilicon/kernel/../include -I/home/dima/git/openhisilicon/kernel/osal/include -I/home/dima/git/openhisilicon/kernel/osal/linux/kernel -I/home/dima/git/openhisilicon/kernel/osal/linux/kernel/media -I/home/dima/git/openhisilicon/kernel/isp/include -I/home/dima/git/openhisilicon/kernel/isp/kernel/arch/include -I/home/dima/git/openhisilicon/kernel/isp/ext_inc -I/home/dima/git/openhisilicon/kernel/isp/kernel/mkp/include -I/home/dima/git/openhisilicon/kernel/isp/kernel/arch/gk7205v200/include -I/home/dima/git/openhisilicon/kernel/../libraries/isp/arch/gk7205v200/include -I/home/dima/git/openhisilicon/kernel/../libraries/isp/include  -DMODULE  -DKBUILD_BASENAME='"isp_drv"'  -DKBUILD_MODNAME='"isp"' -c -o /home/dima/git/openhisilicon/kernel/isp/kernel/arch/gk7205v200/hal/isp_drv.o /home/dima/git/openhisilicon/kernel/isp/kernel/arch/gk7205v200/hal/isp_drv.c

source_/home/dima/git/openhisilicon/kernel/isp/kernel/arch/gk7205v200/hal/isp_drv.o := /home/dima/git/openhisilicon/kernel/isp/kernel/arch/gk7205v200/hal/isp_drv.c

deps_/home/dima/git/openhisilicon/kernel/isp/kernel/arch/gk7205v200/hal/isp_drv.o := \
    $(wildcard include/config/num.h) \
    $(wildcard include/config/isp/af/support.h) \
  /home/dima/git/openhisilicon/kernel/isp/kernel/arch/include/isp_drv_defines.h \
  /home/dima/git/openhisilicon/kernel/isp/kernel/mkp/include/isp.h \
    $(wildcard include/config/info/s.h) \
    $(wildcard include/config/isp/liteos/bottom/half/support.h) \
  /home/dima/git/openhisilicon/kernel/osal/include/osal.h \
    $(wildcard include/config/compat.h) \
    $(wildcard include/config/snapshot/boot.h) \
  /home/dima/git/openhisilicon/kernel/osal/include/osal_list.h \
  /home/dima/git/openhisilicon/kernel/osal/include/osal_ioctl.h \
  /home/dima/git/buildroot_toolchain/hi3516ev300/lib/gcc/arm-openipc-linux-musleabi/7.5.0/include/stdarg.h \
  /home/dima/git/openhisilicon/kernel/isp/kernel/arch/gk7205v200/include/mkp_isp.h \
    $(wildcard include/config/info/set.h) \
  /home/dima/git/openhisilicon/kernel/../include/type.h \
  include/linux/types.h \
    $(wildcard include/config/have/uid16.h) \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  include/uapi/linux/types.h \
  arch/arm/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  include/uapi/asm-generic/int-ll64.h \
  arch/arm/include/generated/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/uapi/asm-generic/bitsperlong.h \
  include/uapi/linux/posix_types.h \
  include/linux/stddef.h \
  include/uapi/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/kasan.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
    $(wildcard include/config/gcov/kernel.h) \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  arch/arm/include/uapi/asm/posix_types.h \
  include/uapi/asm-generic/posix_types.h \
  /home/dima/git/openhisilicon/kernel/../include/comm_3a.h \
    $(wildcard include/config/set.h) \
    $(wildcard include/config/get.h) \
    $(wildcard include/config/s.h) \
  /home/dima/git/openhisilicon/kernel/../include/common.h \
    $(wildcard include/config/log/trace/support.h) \
  /home/dima/git/openhisilicon/kernel/../include/autoconf.h \
    $(wildcard include/config/gk7205v200.h) \
    $(wildcard include/config/chip/type.h) \
    $(wildcard include/config/arch.h) \
    $(wildcard include/config/subchip/gk7205v200.h) \
    $(wildcard include/config/up.h) \
    $(wildcard include/config/arm/arch/type.h) \
    $(wildcard include/config/a7.h) \
    $(wildcard include/config/cpu/type.h) \
    $(wildcard include/config/version/asic.h) \
    $(wildcard include/config/fpga.h) \
    $(wildcard include/config/linux/os.h) \
    $(wildcard include/config/os/type.h) \
    $(wildcard include/config/linux/4/9/y.h) \
    $(wildcard include/config/kernel/version.h) \
    $(wildcard include/config/kernel/arm/mix100/linux.h) \
    $(wildcard include/config/cross.h) \
    $(wildcard include/config/libc/type.h) \
    $(wildcard include/config/kernel/bit.h) \
    $(wildcard include/config/user/arm/mix100/linux.h) \
    $(wildcard include/config/cross/lib.h) \
    $(wildcard include/config/user/bit.h) \
    $(wildcard include/config/release/type/release.h) \
    $(wildcard include/config/rls/mode.h) \
    $(wildcard include/config/customer/version/common.h) \
    $(wildcard include/config/customer/version.h) \
    $(wildcard include/config/sys/support.h) \
    $(wildcard include/config/sys/scale/coef/support.h) \
    $(wildcard include/config/vi/support.h) \
    $(wildcard include/config/vi/bt656.h) \
    $(wildcard include/config/vi/mipi.h) \
    $(wildcard include/config/isp/2dof/dis/support.h) \
    $(wildcard include/config/vi/ldcv3/support.h) \
    $(wildcard include/config/vi/userpic/support.h) \
    $(wildcard include/config/vi/coverex/overlayex/support.h) \
    $(wildcard include/config/vi/luma/support.h) \
    $(wildcard include/config/vi/fpn/support.h) \
    $(wildcard include/config/vi/extchn/support.h) \
    $(wildcard include/config/isp/support.h) \
    $(wildcard include/config/isp/cr/support.h) \
    $(wildcard include/config/isp/pregamma/support.h) \
    $(wildcard include/config/isp/ca/support.h) \
    $(wildcard include/config/vpss/support.h) \
    $(wildcard include/config/vpss/3dnr.h) \
    $(wildcard include/config/vpss/chn0/buf/wrap.h) \
    $(wildcard include/config/vpss/luma/stat/support.h) \
    $(wildcard include/config/vpss/vgs/ldc/support.h) \
    $(wildcard include/config/vpss/vgs/rotation/support.h) \
    $(wildcard include/config/vpss/cover/support.h) \
    $(wildcard include/config/vpss/mosaic/support.h) \
    $(wildcard include/config/vpss/buffer/reuse/support.h) \
    $(wildcard include/config/vgs/support.h) \
    $(wildcard include/config/vgs/ive/pre/support.h) \
    $(wildcard include/config/vgs/luma/stat/support.h) \
    $(wildcard include/config/chnl/support.h) \
    $(wildcard include/config/venc/support.h) \
    $(wildcard include/config/h265e/support.h) \
    $(wildcard include/config/h265e/userdata/support.h) \
    $(wildcard include/config/h265e/intra/refresh/support.h) \
    $(wildcard include/config/h264e/support.h) \
    $(wildcard include/config/h264e/svc/support.h) \
    $(wildcard include/config/h264e/userdata/support.h) \
    $(wildcard include/config/h264e/intra/refresh/support.h) \
    $(wildcard include/config/jpege/support.h) \
    $(wildcard include/config/mjpege/support.h) \
    $(wildcard include/config/jpege/dcf/support.h) \
    $(wildcard include/config/jpege/userdata/support.h) \
    $(wildcard include/config/venc/lowdelay/support.h) \
    $(wildcard include/config/venc/framebuf/recycle/support.h) \
    $(wildcard include/config/venc/mpf/vgs/support.h) \
    $(wildcard include/config/venc/vgs/support.h) \
    $(wildcard include/config/venc/smartp/support.h) \
    $(wildcard include/config/venc/dualp/support.h) \
    $(wildcard include/config/venc/rcnref/share/support.h) \
    $(wildcard include/config/venc/debreath/support.h) \
    $(wildcard include/config/venc/skipref/support.h) \
    $(wildcard include/config/venc/scene0/support.h) \
    $(wildcard include/config/venc/scene1/support.h) \
    $(wildcard include/config/venc/scene2/support.h) \
    $(wildcard include/config/rc/avbr/support.h) \
    $(wildcard include/config/rc/qpmap/support.h) \
    $(wildcard include/config/rc/qvbr/support.h) \
    $(wildcard include/config/rc/cvbr/support.h) \
    $(wildcard include/config/vdec/ip.h) \
    $(wildcard include/config/vo/support.h) \
    $(wildcard include/config/vo/cover/osd/support.h) \
    $(wildcard include/config/vo/vgs.h) \
    $(wildcard include/config/vo/graph.h) \
    $(wildcard include/config/region/support.h) \
    $(wildcard include/config/rgn/diffpixosd/support.h) \
    $(wildcard include/config/audio/support.h) \
    $(wildcard include/config/acodec/support.h) \
    $(wildcard include/config/acodec/version.h) \
    $(wildcard include/config/acodec/max/gain.h) \
    $(wildcard include/config/acodec/min/gain.h) \
    $(wildcard include/config/acodec/gain/step.h) \
    $(wildcard include/config/acodec/fast/power/support.h) \
    $(wildcard include/config/downvqe/support.h) \
    $(wildcard include/config/talkvqe/support.h) \
    $(wildcard include/config/recordvqe/support.h) \
    $(wildcard include/config/talkvqev2/support.h) \
    $(wildcard include/config/audio/static/register/support.h) \
    $(wildcard include/config/audio/bcd/support.h) \
    $(wildcard include/config/audio/gk7202v300/support/bcd.h) \
    $(wildcard include/config/drv.h) \
    $(wildcard include/config/extdrv.h) \
    $(wildcard include/config/interdrv.h) \
    $(wildcard include/config/cipher.h) \
    $(wildcard include/config/user.h) \
    $(wildcard include/config/mipi/rx.h) \
    $(wildcard include/config/wdg.h) \
    $(wildcard include/config/syscfg.h) \
    $(wildcard include/config/lsadc.h) \
    $(wildcard include/config/gfbg/support.h) \
    $(wildcard include/config/gfbg/vgs.h) \
    $(wildcard include/config/svp/ive.h) \
    $(wildcard include/config/svp/ive/norm/grad.h) \
    $(wildcard include/config/svp/ive/st/candi/corner.h) \
    $(wildcard include/config/svp/ive/cnn/ex.h) \
    $(wildcard include/config/svp/ivp.h) \
    $(wildcard include/config/svp/md.h) \
    $(wildcard include/config/svp/qr.h) \
    $(wildcard include/config/tde/support.h) \
    $(wildcard include/config/pm/support.h) \
    $(wildcard include/config/gdb/no.h) \
    $(wildcard include/config/gdb.h) \
    $(wildcard include/config/log/trace/all.h) \
    $(wildcard include/config/log/trace/level.h) \
  /home/dima/git/openhisilicon/kernel/../include/type.h \
  /home/dima/git/openhisilicon/kernel/../include/defines.h \
  /home/dima/git/openhisilicon/kernel/../include/comm_isp.h \
  /home/dima/git/openhisilicon/kernel/../include/errcode.h \
  /home/dima/git/openhisilicon/kernel/../include/mpp_debug.h \
  /home/dima/git/openhisilicon/kernel/../include/isp_debug.h \
  /home/dima/git/openhisilicon/kernel/../include/comm_video.h \
  /home/dima/git/openhisilicon/kernel/../include/isp_defines.h \
  /home/dima/git/openhisilicon/kernel/../include/comm_sns.h \
  /home/dima/git/openhisilicon/kernel/../include/common.h \
  /home/dima/git/openhisilicon/kernel/../include/defines.h \
  /home/dima/git/openhisilicon/kernel/../libraries/isp/arch/gk7205v200/include/isp_reg_define.h \
  /home/dima/git/openhisilicon/kernel/../libraries/isp/arch/gk7205v200/include/isp_lut_define.h \
  /home/dima/git/openhisilicon/kernel/../libraries/isp/arch/gk7205v200/include/isp_stt_define.h \
  /home/dima/git/openhisilicon/kernel/../include/comm_snap.h \
  /home/dima/git/openhisilicon/kernel/../libraries/isp/include/isp_block.h \
    $(wildcard include/config/h//.h) \
  /home/dima/git/openhisilicon/kernel/../include/comm_video.h \
  /home/dima/git/openhisilicon/kernel/../include/comm_isp.h \
  /home/dima/git/openhisilicon/kernel/isp/ext_inc/isp_ext.h \
  /home/dima/git/openhisilicon/kernel/isp/ext_inc/mod_ext.h \
  /home/dima/git/openhisilicon/kernel/isp/kernel/mkp/include/isp_list.h \
  /home/dima/git/openhisilicon/kernel/../libraries/isp/arch/gk7205v200/include/isp_vreg.h \
    $(wildcard include/config/isp/dpc/static/table/support.h) \
  /home/dima/git/openhisilicon/kernel/isp/ext_inc/vi_ext.h \
  /home/dima/git/openhisilicon/kernel/../include/comm_vi.h \
  /home/dima/git/openhisilicon/kernel/../include/comm_gdc.h \
  /home/dima/git/openhisilicon/kernel/../include/comm_sys.h \
  /home/dima/git/openhisilicon/kernel/../include/math_fun.h \
  /home/dima/git/openhisilicon/kernel/isp/kernel/arch/gk7205v200/include/isp_drv.h \
  /home/dima/git/openhisilicon/kernel/isp/ext_inc/mm_ext.h \
  /home/dima/git/openhisilicon/kernel/osal/include/osal_mmz.h \
  /home/dima/git/openhisilicon/kernel/osal/include/osal.h \
  /home/dima/git/openhisilicon/kernel/isp/ext_inc/sys_ext.h \
    $(wildcard include/config/tzasc/support.h) \

/home/dima/git/openhisilicon/kernel/isp/kernel/arch/gk7205v200/hal/isp_drv.o: $(deps_/home/dima/git/openhisilicon/kernel/isp/kernel/arch/gk7205v200/hal/isp_drv.o)

$(deps_/home/dima/git/openhisilicon/kernel/isp/kernel/arch/gk7205v200/hal/isp_drv.o):
