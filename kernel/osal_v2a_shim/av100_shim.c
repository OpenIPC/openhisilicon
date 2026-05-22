/*
 * av100 V2A OSAL shim — re-exports kernel APIs that the binary blobs
 * in kernel/obj/hi3516av100/ import directly. Targets Linux 6.x and 7.0.
 *
 * V2A (Cortex-A7) generation has no OSAL abstraction layer — the vendor
 * blobs were compiled against kernel 4.9.37 (Hi3516A_SDK_V1.0.8.0) and
 * embed the call sites verbatim. Per issue #50, the API surface is
 * narrower than cv200 (V2, ARM926EJ-S): no strlcpy, no ARMv5-specific
 * cache ops (Cortex-A7 = ARMv7), no create_proc_entry, no __arm_ioremap,
 * no do_mmap_pgoff, no filp_open restrictions. Same extended set as
 * cv200 minus strlcpy:
 *
 *   do_gettimeofday          (removed 5.0)
 *     used by: ai, ao, h264e, isp, ive, rc, sys, vgs, viu, vou, vpss (10)
 *   register_sysctl_table    (removed 6.6 — best-effort flat path)
 *   register_sysctl_paths    (removed 6.6 — last-component-only)
 *     used by: base (and indirectly sys via unregister_sysctl_table,
 *     which the modern kernel still exports natively)
 *   init_timer_key           (callback signature changed; struct field
 *                             drift — runtime correctness BROKEN for
 *                             blobs that touch timer->data; see caveat)
 *     used by: chnl, viu, vpss
 *   _cond_resched            (export removed 5.10 — wrap __cond_resched)
 *   __kmalloc                (renamed __kmalloc_noprof 6.5)
 *   kmem_cache_alloc         (became macro over kmem_cache_alloc_noprof 7.0)
 *   __memzero                (ARM helper, removed long ago)
 *   PDE_DATA                 (renamed pde_data 5.17 — alias-style)
 *   printk                   (renamed _printk 6.0 — varargs wrapper via vprintk)
 *
 * Notably NOT re-exported here:
 *
 *   unregister_sysctl_table  Still exported natively by the modern
 *                            kernel — no shim needed.
 *
 * struct timer_list .data caveat
 * ------------------------------
 * 4.15 timer rework dropped the .data field; the callback signature
 * changed from `void(*)(unsigned long data)` to `void(*)(struct timer_list*)`.
 * Blobs that use init_timer_key (chnl, viu, vpss) embed assumptions
 * about both. The shim resolves the symbol but timers will never fire
 * with the correct argument — those modules need a kernel patch to
 * restore the .data field or a blob recompile to fix the workflow.
 *
 * Must be inserted before any av100 blob module (load_hisilicon order:
 * open_mmz → open_himedia → open_v2a_shim → open_base → ...).
 */

/* NB: kernel/hi3516av100.kbuild passes
 *   -D_cond_resched=__v2a_shim_cond_resched_unused_inline
 * to this TU to suppress the static-inline definition of
 * _cond_resched in <linux/sched.h>. See cv200_shim.c for rationale. */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/timekeeping.h>
#include <linux/timer.h>
#include <linux/sysctl.h>
#include <linux/string.h>     /* memset (for __memzero shim) */
#include <linux/types.h>
#include <linux/version.h>
#include <linux/sched.h>      /* __cond_resched */
#include <linux/slab.h>       /* __kmalloc_noprof, kmem_cache_alloc_noprof */
#include <linux/proc_fs.h>    /* pde_data */
#include <linux/vmalloc.h>    /* vmalloc_noprof */
#include <linux/device.h>     /* dev_vprintk_emit */
#include <stdarg.h>           /* va_list, va_start, va_end (vprintk wrapper) */

#undef _cond_resched
#undef jiffies_to_msecs
#undef del_timer

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("av100 V2A OSAL shim — re-exports removed-since-4.9 kernel APIs");

/* The entire shim body is only meaningful on kernels >= 5.0 where the
 * legacy symbols this module re-exports have actually been removed. On
 * older kernels (the av100_lite production target, 4.9.37) every symbol
 * here is already exported by the kernel itself and re-defining them
 * causes "conflicting types" errors at module-compile time, plus the
 * runtime would refuse to load duplicate exports anyway. Make the
 * module a no-op shell on those kernels — the Kbuild dispatch stays
 * unchanged so the same Kbuild line works on both 4.9 and 7.0. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)

/* -------------------------------------------------------------------
 * do_gettimeofday() — removed in 5.0 (commit 192a82f9000b).
 * Define a shim with a unique internal name, then alias the legacy
 * symbol to it via asm. Avoids the do_gettimeofday() macro in
 * kernel_compat.h that would collide with a function definition.
 * ------------------------------------------------------------------- */

struct compat_v2a_timeval {
	long tv_sec;
	long tv_usec;
};

void __v2a_shim_do_gettimeofday(struct compat_v2a_timeval *tv)
{
	struct timespec64 ts;
	ktime_get_real_ts64(&ts);
	tv->tv_sec = (long)ts.tv_sec;
	tv->tv_usec = ts.tv_nsec / 1000;
}
/* kernel_compat.h #defines do_gettimeofday(tvp) → compat_do_gettimeofday(tvp).
 * Undef before declaring the alias, otherwise the extern declaration would
 * alias 'compat_do_gettimeofday' instead of the legacy 'do_gettimeofday'. */
#undef do_gettimeofday
extern void do_gettimeofday(struct compat_v2a_timeval *tv)
	__attribute__((alias("__v2a_shim_do_gettimeofday")));
EXPORT_SYMBOL(do_gettimeofday);

/* -------------------------------------------------------------------
 * register_sysctl_table() — removed in 6.6 (commit c1d967dd49a3) in
 * favour of register_sysctl() which takes a path string. Blob (base/sys)
 * walks the legacy .child-linked ctl_table tree. Register the leaf
 * flat under /proc/sys/hisi/ — good enough for symbol resolution; the
 * blob's expected paths may not materialize.
 * ------------------------------------------------------------------- */

struct ctl_table_header *register_sysctl_table(struct ctl_table *table)
{
	int count = 0;
	while (table[count].procname)
		count++;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 11, 0)
	return register_sysctl_sz("hisi", table, count);
#else
	return register_sysctl("hisi", table);
#endif
}
EXPORT_SYMBOL(register_sysctl_table);

/* -------------------------------------------------------------------
 * init_timer_key() — modern signature requires
 * void (*)(struct timer_list *). Blobs were built with the legacy
 * void (*)(unsigned long). Cast and call timer_setup — symbol resolves;
 * runtime correctness is broken (see caveat at top of file).
 * ------------------------------------------------------------------- */

void __v2a_shim_init_timer_key(struct timer_list *timer,
			       void (*func)(struct timer_list *),
			       unsigned int flags,
			       const char *name,
			       struct lock_class_key *key)
{
	timer_setup(timer, func, flags);
	pr_warn_once("v2a_shim: init_timer_key() called — blob timer with "
		     "legacy void(*)(unsigned long) callback resolved but "
		     "won't fire correctly\n");
}
extern void init_timer_key(struct timer_list *timer,
			   void (*func)(struct timer_list *),
			   unsigned int flags,
			   const char *name, struct lock_class_key *key)
	__attribute__((alias("__v2a_shim_init_timer_key")));
EXPORT_SYMBOL(init_timer_key);

/* See cv200_shim.c for the full rationale on each of the following;
 * the alias-attribute pattern lets us name a wrapper internally and
 * provide the legacy symbol via the linker without colliding with
 * static-inline declarations or macros in the modern kernel headers. */

/* _cond_resched (export gone 5.10; header has static inline suppressed
 * via the macro rename at the top of this file). */
int _cond_resched(void) { return __cond_resched(); }
EXPORT_SYMBOL(_cond_resched);

/* __kmalloc (renamed __kmalloc_noprof 6.5). */
void *__v2a_shim_kmalloc(size_t size, gfp_t flags)
{
	return __kmalloc_noprof(size, flags);
}
#undef __kmalloc
extern void *__kmalloc(size_t size, gfp_t flags)
	__attribute__((alias("__v2a_shim_kmalloc")));
EXPORT_SYMBOL(__kmalloc);

/* kmem_cache_alloc (became macro 7.0). */
void *__v2a_shim_kmem_cache_alloc(struct kmem_cache *s, gfp_t flags)
{
	return kmem_cache_alloc_noprof(s, flags);
}
#undef kmem_cache_alloc
extern void *kmem_cache_alloc(struct kmem_cache *s, gfp_t flags)
	__attribute__((alias("__v2a_shim_kmem_cache_alloc")));
EXPORT_SYMBOL(kmem_cache_alloc);

/* __memzero (ARM-specific lib helper, gone). */
void __memzero(void *ptr, size_t n)
{
	memset(ptr, 0, n);
}
EXPORT_SYMBOL(__memzero);

/* PDE_DATA (renamed pde_data 5.17). */
void *__v2a_shim_PDE_DATA(const struct inode *inode)
{
	return pde_data(inode);
}
#undef PDE_DATA
extern void *PDE_DATA(const struct inode *inode)
	__attribute__((alias("__v2a_shim_PDE_DATA")));
EXPORT_SYMBOL(PDE_DATA);

/* printk (renamed _printk 6.0). */
asmlinkage __printf(1, 2) int __v2a_shim_printk(const char *fmt, ...)
{
	va_list args;
	int r;

	va_start(args, fmt);
	r = vprintk(fmt, args);
	va_end(args);
	return r;
}
#undef printk
extern __printf(1, 2) int printk(const char *fmt, ...)
	__attribute__((alias("__v2a_shim_printk")));
EXPORT_SYMBOL(printk);

/* register_sysctl_paths (removed 6.6 — same flat handling as _table). */
struct ctl_path {
	const char *procname;
};

struct ctl_table_header *register_sysctl_paths(const struct ctl_path *path,
					       struct ctl_table *table)
{
	int count = 0;
	while (table[count].procname)
		count++;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 11, 0)
	return register_sysctl_sz("hisi", table, count);
#else
	return register_sysctl("hisi", table);
#endif
}
EXPORT_SYMBOL(register_sysctl_paths);

/* jiffies_to_msecs (static inline in jiffies.h when HZ | MSEC_PER_SEC —
 * suppressed via Kbuild macro rename, same as _cond_resched). */
unsigned int jiffies_to_msecs(const unsigned long j)
{
	return (MSEC_PER_SEC / HZ) * j;
}
EXPORT_SYMBOL(jiffies_to_msecs);

/* del_timer (renamed timer_delete 7.0). */
int del_timer(struct timer_list *timer)
{
	return timer_delete(timer);
}
EXPORT_SYMBOL(del_timer);

/* vmalloc (became macro 7.0). */
void *__v2a_shim_vmalloc(unsigned long size)
{
	return vmalloc_noprof(size);
}
#undef vmalloc
extern void *vmalloc(unsigned long size)
	__attribute__((alias("__v2a_shim_vmalloc")));
EXPORT_SYMBOL(vmalloc);

/* dev_err (macro that funnels through _dev_err — blob references the
 * macro-name symbol; provide variadic wrapper via dev_vprintk_emit). */
#undef dev_err
asmlinkage __printf(2, 3) void dev_err(const struct device *dev,
				       const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	dev_vprintk_emit(3 /* KERN_ERR */, dev, fmt, args);
	va_end(args);
}
EXPORT_SYMBOL(dev_err);

/* sched_setscheduler (export removed 5.9; best-effort dispatch — drops
 * caller priority). See cv200_shim.c for rationale. */
int sched_setscheduler(struct task_struct *p, int policy,
		       const struct sched_param *param)
{
	if (policy == SCHED_FIFO || policy == SCHED_RR) {
		sched_set_fifo(p);
		return 0;
	}
	sched_set_normal(p, 0);
	return 0;
}
EXPORT_SYMBOL(sched_setscheduler);

#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0) */

/* -------------------------------------------------------------------
 * Module init — empty; we exist purely to provide exports (or are a
 * no-op on pre-5.0 kernels where the kernel still ships every symbol
 * we'd otherwise re-export).
 * ------------------------------------------------------------------- */

static int __init v2a_shim_init(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
	pr_info("av100 V2A OSAL shim: ready "
		"(do_gettimeofday, register_sysctl_table/paths, init_timer_key, "
		"_cond_resched, __kmalloc, kmem_cache_alloc, __memzero, "
		"PDE_DATA, printk)\n");
#else
	pr_info("av100 V2A OSAL shim: no-op on this kernel (4.x); legacy "
		"symbols still natively exported\n");
#endif
	return 0;
}

static void __exit v2a_shim_exit(void)
{
}

module_init(v2a_shim_init);
module_exit(v2a_shim_exit);
