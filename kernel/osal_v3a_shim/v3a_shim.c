/*
 * V3A (hi3519v101) OSAL shim — re-exports kernel APIs and stubs vendor BSP
 * symbols that the binary blobs in kernel/obj/hi3519v101/ import directly.
 * Targets Linux 7.0 on Cortex-A7/A17 (QEMU emulates as single Cortex-A7).
 *
 * Hi3519V101 blobs were compiled against Hi3519V101 SDK V1.0.5.0 atop
 * kernel 3.18.20. They embed call sites verbatim — no OSAL abstraction
 * for kernel APIs that have since been renamed, removed, or inlined.
 *
 * Three categories of symbols re-exported here:
 *
 *  (a) kernel APIs renamed by the memory-allocation profiling rework
 *      (kmalloc_noprof / kmem_cache_alloc_noprof / vmalloc_noprof — 6.5+)
 *      and the timer/sysctl reworks (timer_delete_sync 7.0,
 *      register_sysctl_paths/table removed 6.6);
 *
 *  (b) ARM helpers and per-arch glue gone from mainline
 *      (__memzero, _raw_spin_lock_irqsave/unlock_irqrestore, __current,
 *      arm926_flush_*, arm_dma_ops, dma_alloc_from_coherent);
 *
 *  (c) vendor BSP symbols the blob references but openhisilicon has no
 *      source for and the mainline kernel does not provide
 *      (dis_irq_handle[] GIC SGI array, hios_mcc_* multi-core RPC,
 *      hil_is_phys_in_mmz / hil_map_mmz_check_phys MMZ predicates,
 *      pcit_create_task, get_pf_window_base, g_reg_otp_base_va,
 *      lowercase cmpi_* renames the V3A vendor SDK introduced).
 *
 * Must load BEFORE any V3A blob module (load_hisilicon order:
 * open_osal → open_v3a_shim → open_sys_config → open_base → ...).
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/timer.h>
#include <linux/sysctl.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/version.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/jiffies.h>
#include <linux/math64.h>
#include <linux/random.h>
#include <stdarg.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("V3A (hi3519v101) OSAL shim — re-exports removed-since-3.18 kernel APIs and stubs vendor BSP symbols");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)

/* ============================================================
 * (a) memory-profiling renames (kmalloc_noprof / vmalloc_noprof
 * /kmem_cache_alloc_noprof) introduced in 6.5+, and other API
 * removals.
 * ============================================================ */

/* __kmalloc (renamed __kmalloc_noprof in 6.5). */
void *__v3a_shim_kmalloc(size_t size, gfp_t flags)
{
	return __kmalloc_noprof(size, flags);
}
#undef __kmalloc
extern void *__kmalloc(size_t size, gfp_t flags)
	__attribute__((alias("__v3a_shim_kmalloc")));
EXPORT_SYMBOL(__kmalloc);

/* kmem_cache_alloc (became macro 7.0). */
void *__v3a_shim_kmem_cache_alloc(struct kmem_cache *s, gfp_t flags)
{
	return kmem_cache_alloc_noprof(s, flags);
}
#undef kmem_cache_alloc
extern void *kmem_cache_alloc(struct kmem_cache *s, gfp_t flags)
	__attribute__((alias("__v3a_shim_kmem_cache_alloc")));
EXPORT_SYMBOL(kmem_cache_alloc);

/* vmalloc (became macro 7.0). */
void *__v3a_shim_vmalloc(unsigned long size)
{
	return vmalloc_noprof(size);
}
#undef vmalloc
extern void *vmalloc(unsigned long size)
	__attribute__((alias("__v3a_shim_vmalloc")));
EXPORT_SYMBOL(vmalloc);

/* printk (renamed _printk 6.0). */
asmlinkage __printf(1, 2) int __v3a_shim_printk(const char *fmt, ...)
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
	__attribute__((alias("__v3a_shim_printk")));
EXPORT_SYMBOL(printk);

/* __memzero (ARM-specific lib helper, gone). */
void __memzero(void *ptr, size_t n)
{
	memset(ptr, 0, n);
}
EXPORT_SYMBOL(__memzero);

/* del_timer_sync (renamed timer_delete_sync 7.0). */
#undef del_timer_sync
int del_timer_sync(struct timer_list *timer)
{
	return timer_delete_sync(timer);
}
EXPORT_SYMBOL(del_timer_sync);

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

/* dev_warn (KERN_WARNING; same shape as dev_err). */
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

/* msecs_to_jiffies (became static inline; blob references it as symbol). */
#undef msecs_to_jiffies
unsigned long msecs_to_jiffies(const unsigned int m)
{
	return (m * (unsigned long)HZ + (MSEC_PER_SEC - 1)) / MSEC_PER_SEC;
}
EXPORT_SYMBOL(msecs_to_jiffies);

/* __mutex_init (extern in mutex.h; not exported by 6.x kernels). The
 * Kbuild macro rename suppresses the header prototype so we can supply
 * the symbol from this TU; #undef restores the legacy name for our
 * own declaration. */
#undef __mutex_init
void __v3a_shim___mutex_init(struct mutex *lock, const char *name,
			     struct lock_class_key *key)
{
	memset(lock, 0, sizeof(*lock));
	atomic_long_set(&lock->owner, 0);
	raw_spin_lock_init(&lock->wait_lock);
	INIT_LIST_HEAD(&lock->wait_list);
}
extern void __mutex_init(struct mutex *lock, const char *name,
			 struct lock_class_key *key)
	__attribute__((alias("__v3a_shim___mutex_init")));
EXPORT_SYMBOL(__mutex_init);

/* module_put / try_module_get — static inline in modules.h on modern
 * kernels. Suppress via Kbuild macro rename then #undef here so the
 * extern legacy symbol declarations below don't collide with the
 * (now renamed) header inlines. Best-effort no-op for QEMU boot — the
 * blobs call these for /dev/* misc-device refcounts and the kernel
 * has its own internal refcount independently. */
#undef module_put
void __v3a_shim_module_put_impl(struct module *module)
{
	/* no-op */
}
extern void module_put(struct module *module)
	__attribute__((alias("__v3a_shim_module_put_impl")));
EXPORT_SYMBOL(module_put);

#undef try_module_get
bool __v3a_shim_try_module_get_impl(struct module *module)
{
	return true;
}
extern bool try_module_get(struct module *module)
	__attribute__((alias("__v3a_shim_try_module_get_impl")));
EXPORT_SYMBOL(try_module_get);

/* warn_slowpath_null (renamed warn_slowpath_fmt in 5.x). The replacement
 * __warn_printk() is not exported either, so just emit a plain printk
 * and barrier — semantics are best-effort. */
void warn_slowpath_null(const char *file, int line)
{
	printk(KERN_WARNING "WARNING at %s:%d\n", file, line);
}
EXPORT_SYMBOL(warn_slowpath_null);

/* of_property_read_u32_array became inline wrapper over
 * of_property_read_variable_u32_array(). */
#undef of_property_read_u32_array
int of_property_read_u32_array(const struct device_node *np,
			       const char *propname, u32 *out_values,
			       size_t sz)
{
	int ret = of_property_read_variable_u32_array(np, propname,
						      out_values, sz, 0);
	if (ret >= 0 && (size_t)ret != sz)
		return -EOVERFLOW;
	return ret >= 0 ? 0 : ret;
}
EXPORT_SYMBOL(of_property_read_u32_array);

/* register_sysctl_table — removed in 6.6 (commit c1d967dd49a3). Best
 * effort: count entries and register flat under /proc/sys/hisi/. */
struct ctl_table_header *register_sysctl_table(struct ctl_table *table)
{
	int count = 0;
	while (table && table[count].procname)
		count++;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 11, 0)
	return register_sysctl_sz("hisi", table, count);
#else
	return register_sysctl("hisi", table);
#endif
}
EXPORT_SYMBOL(register_sysctl_table);

/* register_sysctl_paths (removed 6.6 — same flat handling as _table). */
struct ctl_path {
	const char *procname;
};
struct ctl_table_header *register_sysctl_paths(const struct ctl_path *path,
					       struct ctl_table *table)
{
	int count = 0;
	while (table && table[count].procname)
		count++;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 11, 0)
	return register_sysctl_sz("hisi", table, count);
#else
	return register_sysctl("hisi", table);
#endif
}
EXPORT_SYMBOL(register_sysctl_paths);

/* ============================================================
 * (b) ARM helpers and per-arch glue gone from mainline.
 * ============================================================ */

/* __current — ARM-specific accessor. The blob references it as a raw
 * symbol; modern kernels access `current` via per-CPU read of sp_el0
 * (arm64) or by reading thread_info from the stack base (arm32). On
 * arm32 mainline the `current` macro expands to get_current() which
 * is a static inline; the legacy `__current` symbol the blob expects
 * is gone. Provide a global pointer so the symbol resolves; runtime
 * dereferences will read whatever was last written. The blob only
 * uses this for diagnostic prints (per cv300 vendor analysis) — it
 * is NOT load-bearing for the boot/eth path. */
struct task_struct *__current_storage = NULL;
extern struct task_struct *__current
	__attribute__((alias("__current_storage")));
EXPORT_SYMBOL(__current);

/* arm926_flush_kern_cache_all / arm926_flush_kern_dcache_area — the
 * Hi3519V101 die actually has Cortex-A17 + A7 cores (ARMv7), NOT ARM926.
 * The blob has these symbols as dead vendor-SDK leftovers (cv300 family
 * shared blob compile target). Stub as no-op flushes — on Cortex-A7 the
 * generic flush_cache_all() / flush_dcache_page() paths handle cache
 * coherence; if the blob ever called these (it shouldn't, since the
 * code paths are CPU-feature gated) the runtime would mis-flush, but
 * since they're dead they should never fire. */
void arm926_flush_kern_cache_all(void)
{
	flush_cache_all();
}
EXPORT_SYMBOL(arm926_flush_kern_cache_all);

void arm926_flush_kern_dcache_area(void *addr, size_t size)
{
	/* arm32 mainline exposes __cpuc_flush_dcache_area; arm64 renamed
	 * __flush_dcache_area to dcache_clean_inval_poc at 5.15. The
	 * symbol here is a vendor-SDK leftover (V3A blob was compiled
	 * with ARM926 dispatch tables active even though the SoC die
	 * is Cortex-A17 + A7). In practice this call site is dead at
	 * runtime — gated by a CPU-feature check the blob does on
	 * probe. Stub to a generic full-cache flush so worst-case
	 * correctness wins over performance. */
	flush_cache_all();
}
EXPORT_SYMBOL(arm926_flush_kern_dcache_area);

/* arm_dma_ops — old per-arch DMA ops vector, replaced by per-device
 * dma_ops in modern kernels. Provide an empty placeholder so the blob
 * symbol resolves; any blob that actually dereferenced this would
 * NULL-deref a function pointer, which would be visible immediately.
 * Empirically the blobs only take its address (for cache-op wiring) so
 * a non-NULL symbol is enough. */
const struct dma_map_ops arm_dma_ops = { 0 };
EXPORT_SYMBOL(arm_dma_ops);

/* dma_alloc_from_coherent / dma_release_from_coherent — replaced by
 * dma_alloc_from_dev_coherent / dma_release_from_dev_coherent in 4.18+,
 * neither of which is exported in modern kernels. The blob uses these
 * to opt-in to a per-device coherent reserved memory pool; returning 0
 * (= "this device has no coherent pool") makes the blob fall back to
 * the generic dma_alloc_coherent() path, which works on QEMU. */
int dma_alloc_from_coherent(struct device *dev, ssize_t size,
			    dma_addr_t *dma_handle, void **ret)
{
	*ret = NULL;
	return 0;
}
EXPORT_SYMBOL(dma_alloc_from_coherent);

int dma_release_from_coherent(struct device *dev, int order, void *vaddr)
{
	return 0;
}
EXPORT_SYMBOL(dma_release_from_coherent);

/* dma_supported — replaced by per-ops .dma_supported callback. Blob
 * calls it as a top-level function; return 1 (always supported) on
 * QEMU and most hardware paths. */
int dma_supported(struct device *dev, u64 mask)
{
	return 1;
}
EXPORT_SYMBOL(dma_supported);

/* _raw_spin_lock_irqsave / _raw_spin_unlock_irqrestore — debug-only
 * exports in modern kernels (lockdep). When CONFIG_DEBUG_SPINLOCK is
 * off, the raw spinlock ops are inline. Provide non-debug wrappers
 * that the blob can link to. */
#undef _raw_spin_lock_irqsave
unsigned long _raw_spin_lock_irqsave(raw_spinlock_t *lock)
{
	unsigned long flags;
	local_irq_save(flags);
	preempt_disable();
	arch_spin_lock(&lock->raw_lock);
	return flags;
}
EXPORT_SYMBOL(_raw_spin_lock_irqsave);

#undef _raw_spin_unlock_irqrestore
void _raw_spin_unlock_irqrestore(raw_spinlock_t *lock, unsigned long flags)
{
	arch_spin_unlock(&lock->raw_lock);
	preempt_enable();
	local_irq_restore(flags);
}
EXPORT_SYMBOL(_raw_spin_unlock_irqrestore);

/* ============================================================
 * (c) vendor BSP symbols — provide stubs so the blob links.
 *     Runtime behavior is best-effort: code paths that actually
 *     exercise these features (PCIV multi-SoC, GIC SGI, OTP)
 *     will return -ENOSYS / 0 / NULL.
 * ============================================================ */

/* dis_irq_handle — the GIC SGI handler array vendor BSP defined for
 * cross-CPU IRQ forwarding (used in dual-core A17+A7 big.LITTLE).
 * On mainline this array doesn't exist. osal_interrupt.c references
 * it under CONFIG_ARM_GIC. Provide a 2-entry static array (matches
 * DIS_IRQ_CNT in osal_interrupt.c). */
struct gic_sgi_handle_v3a {
	unsigned int irq;
	void (*handle)(unsigned int cpu_intrf,
		       unsigned int irq_num,
		       struct pt_regs *regs);
};
struct gic_sgi_handle_v3a dis_irq_handle[2] = {
	{ .irq = 0xF, .handle = NULL },
	{ .irq = 0xF, .handle = NULL },
};
EXPORT_SYMBOL(dis_irq_handle);

/* hios_mcc_* — Hi3519V101's multi-core comms RPC (A17 host ↔ A7 cmpt).
 * V3A blobs (pciv, pciv_fmw) link these unconditionally even when MCC
 * isn't used (PCIV is for cross-SoC video relay over PCIe; on a single
 * QEMU instance MCC never opens). Stub all to -ENOSYS / no-op. */
int hios_mcc_open(int port) { return -ENOSYS; }
EXPORT_SYMBOL(hios_mcc_open);
int hios_mcc_close(int handle) { return 0; }
EXPORT_SYMBOL(hios_mcc_close);
int hios_mcc_check_remote(int target_id) { return -ENOSYS; }
EXPORT_SYMBOL(hios_mcc_check_remote);
int hios_mcc_getlocalid(void) { return 0; }
EXPORT_SYMBOL(hios_mcc_getlocalid);
int hios_mcc_getremoteids(int *ids, int max) { return 0; }
EXPORT_SYMBOL(hios_mcc_getremoteids);
int hios_mcc_sendto(int handle, const void *buf, int len, int target_id)
{
	return -ENOSYS;
}
EXPORT_SYMBOL(hios_mcc_sendto);
int hios_mcc_setopt(int handle, int opt, const void *value, int len)
{
	return -ENOSYS;
}
EXPORT_SYMBOL(hios_mcc_setopt);

/* pcit_create_task — PCI tunneling task; only invoked when PCIE relay
 * is active. Same as MCC: stub. */
int pcit_create_task(void *args) { return -ENOSYS; }
EXPORT_SYMBOL(pcit_create_task);

/* get_pf_window_base — PCI Express PCIe-physfunc window base accessor.
 * Returns 0 (no window). */
unsigned long get_pf_window_base(int idx) { return 0; }
EXPORT_SYMBOL(get_pf_window_base);

/* g_reg_otp_base_va — OTP register window. Vendor BSP defined this as
 * a global pointer; mainline has no OTP driver. NULL placeholder. */
void __iomem *g_reg_otp_base_va = NULL;
EXPORT_SYMBOL(g_reg_otp_base_va);

/* g_stVouExpFunc — VOU export function table. open_vo.ko (disabled via
 * DISABLE_VO=1 in firmware build) would normally provide this; on
 * neo we ship a stub here so hi_user.ko's link resolves. */
void *g_stVouExpFunc = NULL;
EXPORT_SYMBOL(g_stVouExpFunc);

/* init_timer_key — modern signature requires
 * void (*)(struct timer_list *). Blobs were built with the legacy
 * void (*)(unsigned long). Cast and call timer_setup — the symbol
 * resolves; runtime correctness is broken for blobs that touch
 * timer->data (the field is gone). The blobs affected here are
 * pciv_fmw (PCI tunneling) — never exercised on QEMU boot/eth path. */
void __v3a_shim_init_timer_key(struct timer_list *timer,
			       void (*func)(struct timer_list *),
			       unsigned int flags,
			       const char *name,
			       struct lock_class_key *key)
{
	timer_setup(timer, func, flags);
}
extern void init_timer_key(struct timer_list *timer,
			   void (*func)(struct timer_list *),
			   unsigned int flags,
			   const char *name, struct lock_class_key *key)
	__attribute__((alias("__v3a_shim_init_timer_key")));
EXPORT_SYMBOL(init_timer_key);

/* hil_is_phys_in_mmz / hil_map_mmz_check_phys — MMZ-side predicates
 * the blob calls to validate physical addresses before mapping. The
 * canonical implementations live in our mmz/media-mem.c (hil_mmb_*
 * lookup), but the symbol names diverge. Forward to safe defaults:
 * is_phys_in_mmz → 0 (assume NOT in MMZ, fall through to ioremap path);
 * map_mmz_check_phys → 0 success. */
int hil_is_phys_in_mmz(unsigned long phys, unsigned long size)
{
	return 0;
}
EXPORT_SYMBOL(hil_is_phys_in_mmz);

int hil_map_mmz_check_phys(unsigned long phys, unsigned long size)
{
	return 0;
}
EXPORT_SYMBOL(hil_map_mmz_check_phys);

/* CMPI_* (uppercase, missing-from-blob set) — the V3A vendor blob
 * does NOT implement these five (cv300 generation got them from a
 * different vendor SDK). They're called from blob code paths but
 * never exercised in the QEMU boot/eth path. Stub to safe values. */
int CMPI_CreateProc(const char *name, void *ops, void *data)
{
	return 0;
}
EXPORT_SYMBOL(CMPI_CreateProc);

int CMPI_RemoveProc(const char *name) { return 0; }
EXPORT_SYMBOL(CMPI_RemoveProc);

int CMPI_RegisterDevice(int devno, const char *name, void *ops, void *data)
{
	return 0;
}
EXPORT_SYMBOL(CMPI_RegisterDevice);

int CMPI_UnRegisterDevice(int devno, const char *name) { return 0; }
EXPORT_SYMBOL(CMPI_UnRegisterDevice);

int CMPI_UserCopy(void *to, const void *from, unsigned long n)
{
	return copy_to_user(to, from, n);
}
EXPORT_SYMBOL(CMPI_UserCopy);

/* cmpi_* (lowercase) — V3A's later-vintage refactor renamed several
 * CMPI primitives. Provide self-contained stubs (do NOT forward to the
 * uppercase CMPI_* counterparts in base.o — that would create a
 * v3a_shim -> open_base cyclic module dependency). The lowercase
 * variants are not exercised on the QEMU boot/eth path. */

int cmpi_register_module(void *mod_info) { return 0; }
EXPORT_SYMBOL(cmpi_register_module);

int cmpi_unregister_module(int mod_id) { return 0; }
EXPORT_SYMBOL(cmpi_unregister_module);

int cmpi_get_module_name(int mod_id, char *name) { return 0; }
EXPORT_SYMBOL(cmpi_get_module_name);

int cmpi_get_module_func_by_id(int mod_id) { return 0; }
EXPORT_SYMBOL(cmpi_get_module_func_by_id);

void *cmpi_mmz_free(unsigned int phy_addr, void *vir_addr) { return NULL; }
EXPORT_SYMBOL(cmpi_mmz_free);

int cmpi_mmz_malloc_cached(char *cpMmzName, char *pBufName,
			   unsigned int *pu32PhyAddr, void **ppVitAddr,
			   int s32Len)
{
	return -ENOMEM;
}
EXPORT_SYMBOL(cmpi_mmz_malloc_cached);

int cmpi_mmz_malloc_nocache(char *cpMmzName, char *pBufName,
			    unsigned int *pu32PhyAddr, void **ppVitAddr,
			    int s32Len)
{
	return -ENOMEM;
}
EXPORT_SYMBOL(cmpi_mmz_malloc_nocache);

void *cmpi_remap_cached(unsigned long phys, unsigned long size)
{
	return ioremap_cache(phys, size);
}
EXPORT_SYMBOL(cmpi_remap_cached);

void *cmpi_remap_nocache(unsigned long phys, unsigned long size)
{
	return ioremap(phys, size);
}
EXPORT_SYMBOL(cmpi_remap_nocache);

void cmpi_unmap(void *vaddr)
{
	iounmap((void __iomem *)vaddr);
}
EXPORT_SYMBOL(cmpi_unmap);

/* ============================================================
 * Missing OSAL helpers — should arguably live in osal/hi3519v101
 * but the source was sparse. Define them here for now.
 * ============================================================ */

s64 osal_div_s64(s64 dividend, s32 divisor)
{
	return div_s64(dividend, divisor);
}
EXPORT_SYMBOL(osal_div_s64);

s64 osal_div64_s64(s64 dividend, s64 divisor)
{
	return div64_s64(dividend, divisor);
}
EXPORT_SYMBOL(osal_div64_s64);

u64 osal_div_u64(u64 dividend, u32 divisor)
{
	return div_u64(dividend, divisor);
}
EXPORT_SYMBOL(osal_div_u64);

u32 osal_random(void)
{
	return get_random_u32();
}
EXPORT_SYMBOL(osal_random);

int osal_call_usermodehelper_force(char *path, char **argv, char **envp,
				   int wait)
{
	return call_usermodehelper(path, argv, envp, wait);
}
EXPORT_SYMBOL(osal_call_usermodehelper_force);

/* osal_klib_f* — vendor kernel-side file I/O. On modern kernels with
 * set_fs() gone, file I/O from kernel is done via kernel_read/write.
 * Stub to safe no-ops for QEMU boot path; full impl would mirror
 * cv300's filp_open + kernel_read/write approach. */
void *osal_klib_fopen(const char *path, int flags, int mode)
{
	return NULL;
}
EXPORT_SYMBOL(osal_klib_fopen);

int osal_klib_fclose(void *fp) { return 0; }
EXPORT_SYMBOL(osal_klib_fclose);

int osal_klib_fread(void *buf, size_t size, void *fp) { return 0; }
EXPORT_SYMBOL(osal_klib_fread);

int osal_klib_fwrite(const void *buf, size_t size, void *fp) { return 0; }
EXPORT_SYMBOL(osal_klib_fwrite);

unsigned char osal_platform_get_modparam_uchar(void *pdev, const char *name)
{
	return 0;
}
EXPORT_SYMBOL(osal_platform_get_modparam_uchar);

unsigned short osal_platform_get_modparam_ushort(void *pdev, const char *name)
{
	return 0;
}
EXPORT_SYMBOL(osal_platform_get_modparam_ushort);

#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0) */

static int __init v3a_shim_init(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
	pr_info("v3a_shim: hi3519v101 OSAL shim ready "
		"(printk, kmalloc, vmalloc, kmem_cache_alloc, dev_err/warn, "
		"register_sysctl_table/paths, dis_irq_handle, hios_mcc_*, "
		"hil_*, cmpi_*, osal_div_*)\n");
#else
	pr_info("v3a_shim: no-op on this kernel (<5.0); legacy symbols "
		"still natively exported\n");
#endif
	return 0;
}

static void __exit v3a_shim_exit(void)
{
}

module_init(v3a_shim_init);
module_exit(v3a_shim_exit);
