/*
 * cv200 V2 OSAL shim — re-exports kernel APIs that the binary blobs
 * in kernel/obj/hi3516cv200/ import directly. Targets Linux 6.x and 7.0.
 *
 * V2 (ARM926EJ-S) generation has no OSAL abstraction layer — the vendor
 * blobs were compiled against kernel 4.9.37 and embed the call sites
 * verbatim. Where the underlying kernel API was removed or renamed,
 * load-time linking would fail with "Unknown symbol" errors. This module
 * provides EXPORT_SYMBOL wrappers under the old names so depmod resolves
 * them.
 *
 * Symbols re-exported (initial four from issue #50 + extended set
 * discovered at modpost-time during the cv200_neo firmware bring-up):
 *
 *   do_gettimeofday          (removed 5.0)
 *   register_sysctl_table    (removed 6.6 — best-effort flat path)
 *   register_sysctl_paths    (removed 6.6 — last-component-only)
 *   init_timer_key           (callback signature changed; struct field
 *                             drift — runtime correctness BROKEN for
 *                             blobs that touch timer->data; see caveat
 *                             below)
 *   strlcpy                  (removed 6.8)
 *   _cond_resched            (export removed 5.10 — wrap __cond_resched)
 *   __kmalloc                (renamed __kmalloc_noprof 6.5)
 *   kmem_cache_alloc         (became macro over kmem_cache_alloc_noprof 7.0)
 *   __memzero                (ARM-specific helper, removed long ago)
 *   PDE_DATA                 (renamed pde_data 5.17 — alias-style)
 *   printk                   (renamed _printk 6.0 — varargs wrapper via vprintk)
 *
 * Notably NOT re-exported here (must be handled separately):
 *
 *   unregister_sysctl_table  Still exported natively by the modern
 *                            kernel — no shim needed.
 *
 *   filp_open                Still exported natively — no shim needed.
 *
 *   __pte_offset_map         Source-side, NOT a blob symbol. Modern
 *                            kernel pte_offset_map() inlines to the
 *                            unexported __pte_offset_map(). cv200
 *                            mmz-userdev.c usr_virt_to_phys is patched
 *                            to use pte_offset_kernel() via
 *                            kernel_compat.h — no shim needed.
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
 * Must be inserted before any cv200 blob module (load_hisilicon order:
 * open_mmz → open_himedia → open_v2_shim → open_base → ...).
 */

/* NB: kernel/hi3516cv200.kbuild passes
 *   -D_cond_resched=__v2_shim_cond_resched_unused_inline
 * for this TU. That rename suppresses the static-inline definition of
 * _cond_resched in <linux/sched.h> (which is pulled in via the force-
 * included kernel_compat.h before any source-level #define would
 * take effect), so we can EXPORT_SYMBOL our own version below. The
 * renamed inline is never emitted because we don't reference it. */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/timekeeping.h>
#include <linux/timer.h>
#include <linux/sysctl.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/version.h>
#include <linux/sched.h>      /* __cond_resched */
#include <linux/slab.h>       /* __kmalloc_noprof, kmem_cache_alloc_noprof */
#include <linux/proc_fs.h>    /* pde_data */
#include <linux/vmalloc.h>    /* vmalloc_noprof */
#include <linux/device.h>     /* dev_vprintk_emit */
#include <stdarg.h>           /* va_list, va_start, va_end (vprintk wrapper) */

/* Now that headers are processed, restore the legacy names so our own
 * function definitions below export under the unrenamed symbols. */
#undef _cond_resched
#undef jiffies_to_msecs
/* kernel_compat.h #defines del_timer → timer_delete on 7.0+ for
 * source-side rewrite. v2_shim provides del_timer as an exported
 * function for blob resolution — must use the legacy name verbatim. */
#undef del_timer

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("cv200 V2 OSAL shim — re-exports removed-since-4.9 kernel APIs");

/* The entire shim body is only meaningful on kernels >= 5.0 where the
 * legacy symbols this module re-exports have actually been removed. On
 * older kernels (the cv200_lite production target, 4.9.37) every symbol
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

struct compat_v2_timeval {
	long tv_sec;
	long tv_usec;
};

void __v2_shim_do_gettimeofday(struct compat_v2_timeval *tv)
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
extern void do_gettimeofday(struct compat_v2_timeval *tv)
	__attribute__((alias("__v2_shim_do_gettimeofday")));
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

void __v2_shim_init_timer_key(struct timer_list *timer,
			       void (*func)(struct timer_list *),
			       unsigned int flags,
			       const char *name,
			       struct lock_class_key *key)
{
	timer_setup(timer, func, flags);
	pr_warn_once("v2_shim: init_timer_key() called — blob timer with "
		     "legacy void(*)(unsigned long) callback resolved but "
		     "won't fire correctly\n");
}
extern void init_timer_key(struct timer_list *timer,
			   void (*func)(struct timer_list *),
			   unsigned int flags,
			   const char *name, struct lock_class_key *key)
	__attribute__((alias("__v2_shim_init_timer_key")));
EXPORT_SYMBOL(init_timer_key);

/* -------------------------------------------------------------------
 * strlcpy() — removed in 6.8. Map to strscpy and return size_t (blobs
 * don't check). Internal name to avoid the macro in <linux/string.h>.
 * ------------------------------------------------------------------- */

size_t __v2_shim_strlcpy(char *dst, const char *src, size_t size)
{
	ssize_t r;
	if (!size)
		return strlen(src);
	r = sized_strscpy(dst, src, size);
	return (r < 0) ? size : (size_t)r;
}
/* <linux/string.h> 6.x+ defines strlcpy as a macro to sized_strscpy with
 * __must_be_cstr() compile-time array checks — incompatible with a
 * function declaration. Undef so the alias attaches to the legacy symbol. */
#undef strlcpy
extern size_t strlcpy(char *dst, const char *src, size_t size)
	__attribute__((alias("__v2_shim_strlcpy")));
EXPORT_SYMBOL(strlcpy);

/* -------------------------------------------------------------------
 * _cond_resched() — EXPORT_SYMBOL removed in 5.10 (commit fe32d3cd5e8e)
 * when cond_resched() became a static inline calling __cond_resched().
 * The header's inline definition is suppressed via the macro rename at
 * the top of this file; we provide the legacy export directly.
 * ------------------------------------------------------------------- */

int _cond_resched(void)
{
	return __cond_resched();
}
EXPORT_SYMBOL(_cond_resched);

/* -------------------------------------------------------------------
 * __kmalloc() — renamed __kmalloc_noprof in 6.5 (commit b0fd33f93330).
 * <linux/slab.h> still declares __kmalloc (sometimes as macro, sometimes
 * as static inline depending on config). Alias pattern.
 * ------------------------------------------------------------------- */

void *__v2_shim_kmalloc(size_t size, gfp_t flags)
{
	return __kmalloc_noprof(size, flags);
}
#undef __kmalloc
extern void *__kmalloc(size_t size, gfp_t flags)
	__attribute__((alias("__v2_shim_kmalloc")));
EXPORT_SYMBOL(__kmalloc);

/* -------------------------------------------------------------------
 * kmem_cache_alloc() — became `#define kmem_cache_alloc(...)
 * alloc_hooks(kmem_cache_alloc_noprof(__VA_ARGS__))` in 7.0. Undef the
 * macro, then alias.
 * ------------------------------------------------------------------- */

void *__v2_shim_kmem_cache_alloc(struct kmem_cache *s, gfp_t flags)
{
	return kmem_cache_alloc_noprof(s, flags);
}
#undef kmem_cache_alloc
extern void *kmem_cache_alloc(struct kmem_cache *s, gfp_t flags)
	__attribute__((alias("__v2_shim_kmem_cache_alloc")));
EXPORT_SYMBOL(kmem_cache_alloc);

/* -------------------------------------------------------------------
 * __memzero() — ARM-specific lib helper in 4.9 (arch/arm/lib/memzero.S),
 * gone from modern arch/arm/lib. Not declared in any header on modern
 * kernels, so a plain function definition is safe.
 * ------------------------------------------------------------------- */

void __memzero(void *ptr, size_t n)
{
	memset(ptr, 0, n);
}
EXPORT_SYMBOL(__memzero);

/* -------------------------------------------------------------------
 * PDE_DATA() — renamed pde_data() in 5.17 (commit 359745d78351).
 * kernel_compat.h #defines PDE_DATA → pde_data for source-side
 * compilation. Here we provide the legacy symbol export. Alias pattern
 * after #undef'ing the compat macro.
 * ------------------------------------------------------------------- */

void *__v2_shim_PDE_DATA(const struct inode *inode)
{
	return pde_data(inode);
}
#undef PDE_DATA
extern void *PDE_DATA(const struct inode *inode)
	__attribute__((alias("__v2_shim_PDE_DATA")));
EXPORT_SYMBOL(PDE_DATA);

/* -------------------------------------------------------------------
 * printk() — EXPORT_SYMBOL renamed printk → _printk in 6.0 (commit
 * eda9cec4c9a0). <linux/printk.h> globally #defines printk(fmt, ...)
 * → _printk(fmt, ##__VA_ARGS__) so a function definition under that
 * name needs the macro undef'd first. Variadic prototype matches the
 * legacy export. Funnel through vprintk (still exported).
 * ------------------------------------------------------------------- */

asmlinkage __printf(1, 2) int __v2_shim_printk(const char *fmt, ...)
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
	__attribute__((alias("__v2_shim_printk")));
EXPORT_SYMBOL(printk);

/* -------------------------------------------------------------------
 * register_sysctl_paths() — removed in 6.6 alongside register_sysctl_table
 * (commit c1d967dd49a3). Same best-effort flat handling as the _table
 * variant.
 * ------------------------------------------------------------------- */

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

/* -------------------------------------------------------------------
 * jiffies_to_msecs() — static inline in <linux/jiffies.h> when
 * HZ divides MSEC_PER_SEC (e.g. HZ=100 — our default). Same trick as
 * _cond_resched: header inline is renamed away via a Kbuild macro
 * (-Djiffies_to_msecs=__v2_shim_jiffies_to_msecs_unused_inline) so we
 * can EXPORT_SYMBOL our own.
 * ------------------------------------------------------------------- */

unsigned int jiffies_to_msecs(const unsigned long j)
{
	return (MSEC_PER_SEC / HZ) * j;
}
EXPORT_SYMBOL(jiffies_to_msecs);

/* -------------------------------------------------------------------
 * del_timer() — renamed timer_delete() in 7.0 (commit 9b13df3fb64e).
 * No conflicting declaration in modern <linux/timer.h>.
 * ------------------------------------------------------------------- */

int del_timer(struct timer_list *timer)
{
	return timer_delete(timer);
}
EXPORT_SYMBOL(del_timer);

/* -------------------------------------------------------------------
 * vmalloc() — became `#define vmalloc(...) alloc_hooks(vmalloc_noprof
 * (__VA_ARGS__))` in 7.0. Undef the macro before defining the function
 * under the same name.
 * ------------------------------------------------------------------- */

void *__v2_shim_vmalloc(unsigned long size)
{
	return vmalloc_noprof(size);
}
#undef vmalloc
extern void *vmalloc(unsigned long size)
	__attribute__((alias("__v2_shim_vmalloc")));
EXPORT_SYMBOL(vmalloc);

/* -------------------------------------------------------------------
 * dev_err() — `#define dev_err(dev, fmt, ...)` macro in modern
 * <linux/dev_printk.h> that funnels through _dev_err. The blob
 * references dev_err as a symbol; provide a variadic wrapper using
 * dev_vprintk_emit (still exported, takes va_list).
 *
 * NB: cv200 vendor blobs (compiled against 4.9.37) emit calls to
 * dev_err with KERN_ERR semantics. The first arg to dev_vprintk_emit
 * is the loglevel digit (KERN_ERR == "<3>" → 3).
 * ------------------------------------------------------------------- */

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

/* dev_warn — KERN_WARNING variant. */
#undef dev_warn
asmlinkage __printf(2, 3) void dev_warn(const struct device *dev,
					const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	dev_vprintk_emit(4 /* KERN_WARNING */, dev, fmt, args);
	va_end(args);
}
EXPORT_SYMBOL(dev_warn);

/* -------------------------------------------------------------------
 * sched_setscheduler() — was EXPORT_SYMBOL_GPL'd in 4.9 but the export
 * was removed in 5.9 (commit 7318d4cc14c8); modules are expected to
 * call the named helpers (sched_set_fifo / sched_set_normal). The
 * cv200 vendor blobs still embed direct sched_setscheduler() calls.
 *
 * Best-effort dispatch: SCHED_FIFO / SCHED_RR → sched_set_fifo (MAX_RT_PRIO/2),
 * everything else → sched_set_normal(nice=0). Caller-supplied priority
 * is dropped — sched_set_fifo doesn't accept one. Acceptable for the
 * blobs that just want "raise above SCHED_NORMAL"; truly priority-
 * sensitive code paths would need a recompile.
 * ------------------------------------------------------------------- */

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

static int __init v2_shim_init(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
	pr_info("cv200 V2 OSAL shim: ready "
		"(do_gettimeofday, register_sysctl_table/paths, "
		"init_timer_key, strlcpy, _cond_resched, __kmalloc, "
		"kmem_cache_alloc, __memzero, PDE_DATA, printk)\n");
#else
	pr_info("cv200 V2 OSAL shim: no-op on this kernel (4.x); legacy "
		"symbols still natively exported\n");
#endif
	return 0;
}

static void __exit v2_shim_exit(void)
{
}

module_init(v2_shim_init);
module_exit(v2_shim_exit);
