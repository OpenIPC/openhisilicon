/*
 * cv100 V1 OSAL shim — re-exports kernel APIs that the binary blobs
 * in kernel/obj/hi3516cv100/ import directly. Targets Linux 6.x and 7.0.
 *
 * V1 (ARM926EJ-S / ARMv5TE) is the oldest legacy generation in this tree
 * — older than V2 (cv200) which the v2_shim handles. cv100 vendor blobs
 * were compiled against the Hi3518 SDK V1.0.B.0 layout running on Linux
 * 3.0.8 (openipc/linux:hisilicon-hi3516cv100 branch). The blobs embed
 * kernel API call sites verbatim with no OSAL abstraction. The legacy
 * subset cv100 blobs import is a STRICT SUBSET of the cv200 v2_shim
 * coverage — same removed-since-4.9 symbols (cv100 has no PDE_DATA,
 * dev_err, jiffies_to_msecs, or register_sysctl_paths blob references).
 *
 * Symbols re-exported (audited via arm-none-eabi-nm -u against the cv100
 * .o set; intersection with kernel-removed APIs):
 *
 *   do_gettimeofday          (removed 5.0)
 *   register_sysctl_table    (removed 6.6 — best-effort flat path)
 *   init_timer_key           (callback signature changed; struct field
 *                             drift — runtime correctness BROKEN for
 *                             blobs that touch timer->data; see caveat
 *                             below)
 *   strlcpy                  (removed 6.8)
 *   _cond_resched            (export removed 5.10 — wrap __cond_resched)
 *   __kmalloc                (renamed __kmalloc_noprof 6.5)
 *   kmem_cache_alloc         (became macro over kmem_cache_alloc_noprof 7.0)
 *   __memzero                (ARM-specific helper, removed long ago)
 *   printk                   (renamed _printk 6.0 — varargs wrapper via vprintk)
 *   del_timer                (renamed timer_delete 7.0)
 *   vmalloc                  (became macro over vmalloc_noprof 7.0)
 *   sched_setscheduler       (export removed 5.9 — best-effort dispatch)
 *
 * Notably NOT re-exported here (extra v2_shim coverage cv100 doesn't need):
 *
 *   PDE_DATA                 cv100 proc-fs use sites store data via the
 *                            data pointer at create_proc_entry time and
 *                            read inode->i_private directly — no PDE_DATA
 *                            call.
 *
 *   dev_err / dev_warn       cv100 blobs (Hi3518 SDK V1.0.B.0 era) predate
 *                            wide use of the dev_*() printk family; their
 *                            error paths go through plain printk().
 *
 *   jiffies_to_msecs         Not in cv100 undef set — blobs compute msec
 *                            arithmetic inline rather than calling the
 *                            inline helper.
 *
 *   register_sysctl_paths    Single legacy sysctl call site uses
 *                            register_sysctl_table only.
 *
 * struct timer_list .data caveat
 * ------------------------------
 * 4.15 timer rework dropped the .data field; the callback signature
 * changed from `void(*)(unsigned long data)` to `void(*)(struct timer_list*)`.
 * cv100 blobs that use init_timer_key (chnl, viu, vpss, audio path) embed
 * assumptions about both. The shim resolves the symbol but timers will
 * never fire with the correct argument — those modules need a kernel
 * patch to restore the .data field or a blob recompile to fix the
 * workflow. Same caveat as v2_shim.
 *
 * Must be inserted before any cv100 blob module (load_hisilicon order:
 * open_mmz → open_v1_shim → open_base → open_sys → ...).
 *
 * On the production cv100_lite target (kernel 3.0.8) every symbol here
 * is still natively exported by the kernel; the shim's body is gated
 * `#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)` and compiles down
 * to an informational pr_info() init, leaving the lite build a no-op.
 */

/* NB: kernel/hi3516cv100.kbuild passes
 *   -D_cond_resched=__v1_shim_cond_resched_unused_inline
 * for this TU. That rename suppresses the static-inline definition of
 * _cond_resched in <linux/sched.h> (which is pulled in via the force-
 * included kernel_compat.h before any source-level #define would
 * take effect), so we can EXPORT_SYMBOL our own version below. The
 * renamed inline is never emitted because we don't reference it. */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/version.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("cv100 V1 OSAL shim — re-exports removed-since-3.0 kernel APIs");

/* The entire shim body is only meaningful on kernels >= 5.0 where the
 * legacy symbols this module re-exports have actually been removed. On
 * older kernels (the cv100_lite production target, 3.0.8) every symbol
 * here is already exported by the kernel itself and re-defining them
 * causes "conflicting types" errors at module-compile time, plus the
 * runtime would refuse to load duplicate exports anyway. Make the
 * module a no-op shell on those kernels — the Kbuild dispatch stays
 * unchanged so the same Kbuild line works on both 3.0 and 7.0.
 *
 * The "modern-only" includes (timekeeping.h, slab.h ->_noprof helpers,
 * etc.) live inside this #if too — cv100_lite's 3.0.8 kernel headers
 * predate linux/timekeeping.h (added in 3.17) so an unconditional
 * include breaks the lite build. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)

#include <linux/timekeeping.h>
#include <linux/timer.h>
#include <linux/sysctl.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/sched.h>      /* __cond_resched */
#include <linux/slab.h>       /* __kmalloc_noprof, kmem_cache_alloc_noprof */
#include <linux/vmalloc.h>    /* vmalloc_noprof */
#include <stdarg.h>           /* va_list, va_start, va_end (vprintk wrapper) */

/* Now that headers are processed, restore the legacy names so our own
 * function definitions below export under the unrenamed symbols. */
#undef _cond_resched
/* kernel_compat.h #defines del_timer → timer_delete on 7.0+ for
 * source-side rewrite. v1_shim provides del_timer as an exported
 * function for blob resolution — must use the legacy name verbatim. */
#undef del_timer
/* hi3516cv100.kbuild also passes -D__copy_to_user=...unused_inline /
 * -D__copy_from_user=...unused_inline so the static-inline definitions
 * in <linux/uaccess.h> get renamed away (no symbol emitted). Restore the
 * real names so our function definitions below export under them. */
#undef __copy_to_user
#undef __copy_from_user

/* -------------------------------------------------------------------
 * do_gettimeofday() — removed in 5.0 (commit 192a82f9000b).
 * ------------------------------------------------------------------- */

struct compat_v1_timeval {
	long tv_sec;
	long tv_usec;
};

void __v1_shim_do_gettimeofday(struct compat_v1_timeval *tv)
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
extern void do_gettimeofday(struct compat_v1_timeval *tv)
	__attribute__((alias("__v1_shim_do_gettimeofday")));
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

void __v1_shim_init_timer_key(struct timer_list *timer,
			       void (*func)(struct timer_list *),
			       unsigned int flags,
			       const char *name,
			       struct lock_class_key *key)
{
	timer_setup(timer, func, flags);
	pr_warn_once("v1_shim: init_timer_key() called — blob timer with "
		     "legacy void(*)(unsigned long) callback resolved but "
		     "won't fire correctly\n");
}
extern void init_timer_key(struct timer_list *timer,
			   void (*func)(struct timer_list *),
			   unsigned int flags,
			   const char *name, struct lock_class_key *key)
	__attribute__((alias("__v1_shim_init_timer_key")));
EXPORT_SYMBOL(init_timer_key);

/* -------------------------------------------------------------------
 * strlcpy() — removed in 6.8. Map to strscpy and return size_t (blobs
 * don't check). Internal name to avoid the macro in <linux/string.h>.
 * ------------------------------------------------------------------- */

size_t __v1_shim_strlcpy(char *dst, const char *src, size_t size)
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
	__attribute__((alias("__v1_shim_strlcpy")));
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
 * ------------------------------------------------------------------- */

void *__v1_shim_kmalloc(size_t size, gfp_t flags)
{
	return __kmalloc_noprof(size, flags);
}
#undef __kmalloc
extern void *__kmalloc(size_t size, gfp_t flags)
	__attribute__((alias("__v1_shim_kmalloc")));
EXPORT_SYMBOL(__kmalloc);

/* -------------------------------------------------------------------
 * kmem_cache_alloc() — became `#define kmem_cache_alloc(...)
 * alloc_hooks(kmem_cache_alloc_noprof(__VA_ARGS__))` in 7.0.
 * ------------------------------------------------------------------- */

void *__v1_shim_kmem_cache_alloc(struct kmem_cache *s, gfp_t flags)
{
	return kmem_cache_alloc_noprof(s, flags);
}
#undef kmem_cache_alloc
extern void *kmem_cache_alloc(struct kmem_cache *s, gfp_t flags)
	__attribute__((alias("__v1_shim_kmem_cache_alloc")));
EXPORT_SYMBOL(kmem_cache_alloc);

/* -------------------------------------------------------------------
 * __memzero() — ARM-specific lib helper in 3.x (arch/arm/lib/memzero.S),
 * gone from modern arch/arm/lib. Not declared in any header on modern
 * kernels, so a plain function definition is safe.
 * ------------------------------------------------------------------- */

void __memzero(void *ptr, size_t n)
{
	memset(ptr, 0, n);
}
EXPORT_SYMBOL(__memzero);

/* -------------------------------------------------------------------
 * printk() — EXPORT_SYMBOL renamed printk → _printk in 6.0 (commit
 * eda9cec4c9a0). <linux/printk.h> globally #defines printk(fmt, ...)
 * → _printk(fmt, ##__VA_ARGS__) so a function definition under that
 * name needs the macro undef'd first. Variadic prototype matches the
 * legacy export. Funnel through vprintk (still exported).
 * ------------------------------------------------------------------- */

asmlinkage __printf(1, 2) int __v1_shim_printk(const char *fmt, ...)
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
	__attribute__((alias("__v1_shim_printk")));
EXPORT_SYMBOL(printk);

/* -------------------------------------------------------------------
 * del_timer() — renamed timer_delete() in 7.0 (commit 9b13df3fb64e).
 * ------------------------------------------------------------------- */

int del_timer(struct timer_list *timer)
{
	return timer_delete(timer);
}
EXPORT_SYMBOL(del_timer);

/* -------------------------------------------------------------------
 * vmalloc() — became `#define vmalloc(...) alloc_hooks(vmalloc_noprof
 * (__VA_ARGS__))` in 7.0.
 * ------------------------------------------------------------------- */

void *__v1_shim_vmalloc(unsigned long size)
{
	return vmalloc_noprof(size);
}
#undef vmalloc
extern void *vmalloc(unsigned long size)
	__attribute__((alias("__v1_shim_vmalloc")));
EXPORT_SYMBOL(vmalloc);

/* -------------------------------------------------------------------
 * sched_setscheduler() — was EXPORT_SYMBOL_GPL'd in 3.x but the export
 * was removed in 5.9 (commit 7318d4cc14c8); modules are expected to
 * call the named helpers (sched_set_fifo / sched_set_normal). The
 * cv100 vendor blobs still embed direct sched_setscheduler() calls.
 *
 * Best-effort dispatch — caller-supplied priority is dropped.
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

/* -------------------------------------------------------------------
 * The cv100 blobs (V1, pre-3.10 SDK) call a handful of ARM/kernel
 * internals that have been renamed or removed in the modern tree.
 * The set below is what modpost flags when linking the V1 module set
 * against a Linux 7.0 ARMv5 kernel.
 * ------------------------------------------------------------------- */

#include <linux/mm.h>           /* vm_mmap, vm_munmap */
#include <linux/uaccess.h>      /* arm_copy_to_user, arm_copy_from_user */
#include <linux/proc_fs.h>      /* proc_create, struct proc_dir_entry */
#include <asm/io.h>             /* ioremap / iounmap */

/* __arm_ioremap() — ARMv5/v6 pre-3.10 ioremap entry point. Modern kernels
 * use ioremap() directly. The legacy signature took (phys, size,
 * mtype, caller); blobs only care about the first two. */
void __iomem *__v1_shim___arm_ioremap(phys_addr_t phys_addr,
				      size_t size,
				      unsigned int mtype)
{
	(void)mtype;
	return ioremap(phys_addr, size);
}
extern void __iomem *__arm_ioremap(phys_addr_t phys_addr,
				   size_t size,
				   unsigned int mtype)
	__attribute__((alias("__v1_shim___arm_ioremap")));
EXPORT_SYMBOL(__arm_ioremap);

/* __iounmap() — pre-3.x ARM iounmap variant; just call iounmap(). */
#undef __iounmap
void __v1_shim___iounmap(volatile void __iomem *addr)
{
	iounmap((void __iomem *)addr);
}
extern void __iounmap(volatile void __iomem *addr)
	__attribute__((alias("__v1_shim___iounmap")));
EXPORT_SYMBOL(__iounmap);

/* create_proc_entry() — removed 3.10. Best-effort wrap: register via
 * proc_create_data() with NULL fops and return the resulting handle.
 * Blobs (cv100 mmz, base) immediately overwrite ->proc_fops and
 * ->write_proc post-call, but those struct fields are gone in 7.0;
 * the cb writes silently miss. Acceptable for symbol resolution —
 * the cv100 procfs nodes are diagnostic, not functional dependencies. */
struct proc_dir_entry *__v1_shim_create_proc_entry(const char *name,
						   umode_t mode,
						   struct proc_dir_entry *parent)
{
	return proc_create_data(name, mode, parent, NULL, NULL);
}
extern struct proc_dir_entry *create_proc_entry(const char *name,
						umode_t mode,
						struct proc_dir_entry *parent)
	__attribute__((alias("__v1_shim_create_proc_entry")));
EXPORT_SYMBOL(create_proc_entry);

/* __bug() — BUG() arch backend. Modern ARM uses a different name
 * (bug_handler) but the legacy __bug() callers always panic. Map to
 * panic() with a fixed string. */
void __v1_shim___bug(const char *file, int line)
{
	panic("V1 blob BUG at %s:%d\n", file, line);
}
extern void __bug(const char *file, int line)
	__attribute__((alias("__v1_shim___bug")));
EXPORT_SYMBOL(__bug);

/* do_mmap_pgoff() — kernel-internal mmap helper, unexported since 4.x
 * for security reasons. The cv100 mmz blob uses it to map MMZ regions
 * into userspace. The modern equivalent is vm_mmap(). Blob's caller
 * doesn't care about the slight semantic difference (vm_mmap calls
 * security_mmap_addr() first); the userspace-visible behaviour is the
 * same modulo SELinux denials, which are off in OpenIPC's kernel
 * config anyway. */
unsigned long __v1_shim_do_mmap_pgoff(struct file *file, unsigned long addr,
				      unsigned long len, unsigned long prot,
				      unsigned long flags, unsigned long pgoff)
{
	return vm_mmap(file, addr, len, prot, flags, pgoff << PAGE_SHIFT);
}
extern unsigned long do_mmap_pgoff(struct file *file, unsigned long addr,
				   unsigned long len, unsigned long prot,
				   unsigned long flags, unsigned long pgoff)
	__attribute__((alias("__v1_shim_do_mmap_pgoff")));
EXPORT_SYMBOL(do_mmap_pgoff);

/* do_munmap() — unexported alongside do_mmap_pgoff. Wrap vm_munmap(),
 * which is exported. The blob caller discards the result. */
int __v1_shim_do_munmap(struct mm_struct *mm, unsigned long start,
			size_t len, struct list_head *uf)
{
	(void)mm; (void)uf;
	return vm_munmap(start, len);
}
extern int do_munmap(struct mm_struct *mm, unsigned long start,
		     size_t len, struct list_head *uf)
	__attribute__((alias("__v1_shim_do_munmap")));
EXPORT_SYMBOL(do_munmap);

/* __copy_to_user() / __copy_from_user() — renamed on modern ARM
 * to arm_copy_{to,from}_user, with the leading underscore prefix
 * gone. Both are still in Module.symvers under the new names; just
 * alias the legacy ones. The signatures are identical: (dst, src, n). */
unsigned long __v1_shim___copy_to_user(void __user *to,
				       const void *from, unsigned long n)
{
	return arm_copy_to_user(to, from, n);
}
extern unsigned long __copy_to_user(void __user *to,
				    const void *from, unsigned long n)
	__attribute__((alias("__v1_shim___copy_to_user")));
EXPORT_SYMBOL(__copy_to_user);

unsigned long __v1_shim___copy_from_user(void *to,
					 const void __user *from,
					 unsigned long n)
{
	return arm_copy_from_user(to, from, n);
}
extern unsigned long __copy_from_user(void *to,
				      const void __user *from,
				      unsigned long n)
	__attribute__((alias("__v1_shim___copy_from_user")));
EXPORT_SYMBOL(__copy_from_user);

#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0) */

/* -------------------------------------------------------------------
 * Module init — empty; we exist purely to provide exports (or are a
 * no-op on pre-5.0 kernels where the kernel still ships every symbol
 * we'd otherwise re-export).
 * ------------------------------------------------------------------- */

static int __init v1_shim_init(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
	pr_info("cv100 V1 OSAL shim: ready "
		"(do_gettimeofday, register_sysctl_table, init_timer_key, "
		"strlcpy, _cond_resched, __kmalloc, kmem_cache_alloc, "
		"__memzero, printk, del_timer, vmalloc, sched_setscheduler)\n");
#else
	pr_info("cv100 V1 OSAL shim: no-op on this kernel (3.x/4.x); legacy "
		"symbols still natively exported\n");
#endif
	return 0;
}

static void __exit v1_shim_exit(void)
{
}

module_init(v1_shim_init);
module_exit(v1_shim_exit);
