/*
 * Kernel version compatibility shims for building against Linux 4.9 - 6.6+
 *
 * This header provides compatibility wrappers for kernel APIs that changed
 * or were removed across kernel versions, enabling the OSAL layer and
 * wrapper modules to compile against newer kernels while the binary blobs
 * remain unchanged (they only call OSAL via opaque void* wrappers).
 */
#ifndef KERNEL_COMPAT_H
#define KERNEL_COMPAT_H

#include <linux/version.h>

/*
 * On 6.4+, EXPORT_ALIAS is a no-op so the LOG symbol alias of HI_LOG
 * is not created. Map LOG to HI_LOG at the preprocessor level.
 * This is applied globally via -include in the top-level Kbuild.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
#define LOG HI_LOG
#endif

/*
 * Timer API: init_timer() removed in 4.15, timer callback signature changed.
 * 4.15+: timer_setup() + void(*)(struct timer_list*) callback
 * Pre-4.15: init_timer() + void(*)(unsigned long) callback + .data field
 *
 * Since OSAL wraps timers behind opaque void* and the blobs use the old
 * callback signature void(*)(unsigned long), we use from_timer() in the
 * shim callback to recover the original context.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0)
#define COMPAT_TIMER_SETUP 1
#endif

/*
 * access_ok() lost its first (type) parameter in 5.0.
 * Pre-5.0: access_ok(type, addr, size)
 * 5.0+:    access_ok(addr, size)
 *
 * Can't macro-redefine bare access_ok — kernel's own uaccess.h calls it
 * with 2 args which conflicts. OSAL source must call compat_access_ok().
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
#define compat_access_ok(type, addr, size) access_ok(addr, size)
#else
#define compat_access_ok(type, addr, size) access_ok(type, addr, size)
#endif

/*
 * ioremap_nocache() removed in 5.6 (ioremap is already uncached on most archs).
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define ioremap_nocache ioremap
#endif

/*
 * set_fs()/get_fs()/get_ds() removed in 5.10 (UACCESS_BUFFER / addr_limit).
 * Use kernel_read()/kernel_write() instead of vfs_read() with set_fs().
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
#define COMPAT_NO_SET_FS 1
#endif

/*
 * mmap_sem renamed to mmap_lock in 5.8, with helper functions.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 8, 0)
#define compat_mmap_read_lock(mm)   mmap_read_lock(mm)
#define compat_mmap_read_unlock(mm) mmap_read_unlock(mm)
#else
#define compat_mmap_read_lock(mm)   down_read(&(mm)->mmap_sem)
#define compat_mmap_read_unlock(mm) up_read(&(mm)->mmap_sem)
#endif

/*
 * do_gettimeofday() removed in 5.0; struct timeval removed in 5.6.
 * Legacy OSAL still writes:
 *     struct timeval t;
 *     do_gettimeofday(&t);
 *     ... t.tv_sec, t.tv_usec ...
 * Provide both type and function as transparent shims so the source stays
 * untouched. timespec64 is the modern replacement and has the same
 * .tv_sec / .tv_usec pair (well, tv_nsec — but cv300 OSAL only reads
 * tv_sec/tv_usec, and we adapt by mapping tv_usec to tv_nsec/1000).
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
#define COMPAT_NO_DO_GETTIMEOFDAY 1
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#include <linux/time64.h>
/* shim struct: layout-compatible field access (tv_sec, tv_usec). Pad
 * tv_nsec → tv_usec on read. */
struct compat_timeval {
	time64_t tv_sec;
	long tv_usec;
};
#define timeval compat_timeval
static inline void compat_do_gettimeofday(struct compat_timeval *tv)
{
	struct timespec64 ts;
	ktime_get_real_ts64(&ts);
	tv->tv_sec = ts.tv_sec;
	tv->tv_usec = ts.tv_nsec / 1000;
}
#define do_gettimeofday(tvp) compat_do_gettimeofday(tvp)
#endif

/*
 * rtc_time_to_tm/rtc_tm_to_time removed in 5.6 in favor of
 * rtc_time64_to_tm/rtc_tm_to_time64. Redirect legacy calls transparently.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define compat_rtc_time_to_tm  rtc_time64_to_tm
/* rtc_tm_to_time64() returns time64_t instead of storing via pointer */
#define compat_rtc_tm_to_time(tm, timep) do { *(timep) = rtc_tm_to_time64(tm); } while (0)
#define rtc_time_to_tm(time, tm) rtc_time64_to_tm((unsigned long long)(time), (tm))
#define rtc_tm_to_time(tm, timep) do { *(timep) = (unsigned long)rtc_tm_to_time64(tm); } while (0)
#else
#define compat_rtc_time_to_tm  rtc_time_to_tm
#define compat_rtc_tm_to_time  rtc_tm_to_time
#endif

/*
 * register_sysctl_table() removed in 6.6.
 * Use register_sysctl() instead.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 6, 0)
#define COMPAT_NO_SYSCTL_TABLE 1
#endif

/*
 * register_sysctl_paths() removed in 6.6.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 6, 0)
#define COMPAT_NO_SYSCTL_PATHS 1
#endif

/*
 * struct ctl_path removed in 6.6.
 */

/*
 * ARM32 cache flush APIs:
 * __flush_dcache_area renamed to dcache_clean_inval_poc in 5.15+ (arm64)
 * __cpuc_flush_dcache_area still available on ARM32 through 6.x
 * __cpuc_flush_kern_all still available on ARM32 through 6.x
 * outer_flush_range/outer_flush_all still available when CONFIG_CACHE_L2X0
 *
 * Note: ARM32 cache flush functions are arch-specific and remain stable.
 * ARM64 changes are handled separately.
 */
#ifdef CONFIG_64BIT
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 0)
/* __flush_dcache_area was renamed */
#define compat_flush_dcache_area(addr, size) \
	dcache_clean_inval_poc((unsigned long)(addr), \
			       (unsigned long)(addr) + (size))
#else
#define compat_flush_dcache_area(addr, size) __flush_dcache_area(addr, size)
#endif
#endif /* CONFIG_64BIT */

/*
 * strlcpy() deprecated in 6.1, removed in 6.8. Use strscpy() instead.
 * strscpy returns ssize_t (len or -E2BIG); strlcpy returns size_t (src len).
 * Cast to size_t — call sites compare against size, not check for error.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 1, 0)
#define compat_strlcpy(dst, src, size) ((size_t)strscpy(dst, src, size))
#define strlcpy(dst, src, size) ((size_t)strscpy(dst, src, size))
#else
#define compat_strlcpy(dst, src, size) strlcpy(dst, src, size)
#endif

/*
 * print_symbol() removed in 5.15 (commit f74cd6432cb6) — was a wrapper
 * around printk("%pS"). Define as a transparent macro so legacy OSAL
 * osal_addr.c calls keep working unchanged. fmt is expected to contain
 * "%s" (the old contract); we redirect that single conversion to %pS
 * (kernel symbol lookup). Multi-arg fmt strings are not supported —
 * none of the call sites use them. Guard at 5.15.0, NOT 4.0.0: the
 * symbol is still declared in <linux/kallsyms.h> on 4.x and 5.0-5.14,
 * so redefining as a do-while macro collides with the function prototype
 * (cv500/cv200 lite builds against 4.9 fail with "expected identifier
 * or '(' before 'do'").
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 0)
#include <linux/printk.h>
#define print_symbol(fmt, addr) \
	do { \
		printk(KERN_INFO "%pS\n", (void *)(unsigned long)(addr)); \
		(void)(fmt); \
	} while (0)
#endif

/*
 * PDE_DATA() renamed to pde_data() in 5.17. Keep both spellings working:
 * - new code uses compat_pde_data() (canonical, version-agnostic);
 * - legacy OSAL still writes PDE_DATA() — define it as a macro on >= 5.17
 *   so per-generation osal_proc.c stays untouched (cv300, cv500, ...).
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 17, 0)
#define compat_pde_data(inode) pde_data(inode)
#define PDE_DATA(inode) pde_data(inode)
#elif defined(CONFIG_PROC_FS)
#define compat_pde_data(inode) PDE_DATA(inode)
#endif

/*
 * vm_flags field became read-only in 6.3 (use vm_flags_set/vm_flags_clear).
 * For read-only checks this doesn't matter.
 */

/*
 * sched_clock() moved to <linux/sched/clock.h> in 4.11+.
 * signal_pending() moved to <linux/sched/signal.h> in 4.11+.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
#include <linux/sched/clock.h>
#include <linux/sched/signal.h>
#endif

/*
 * struct bus_type: dev_attrs removed in 3.12, use dev_groups instead.
 * The field was technically present but deprecated since 3.11.
 */

/*
 * linux/dma-contiguous.h merged into linux/dma-map-ops.h in 5.x. The
 * exact cutoff differs between upstream (5.16) and the openipc/linux
 * branches we build against — hisilicon-hi3516cv6xx already merged the
 * two on 5.10.221 — so probe by header presence instead of version.
 */
#if __has_include(<linux/dma-map-ops.h>)
#include <linux/dma-map-ops.h>
#elif __has_include(<linux/dma-contiguous.h>)
#include <linux/dma-contiguous.h>
#endif

/*
 * DEFINE_SEMAPHORE gained a count parameter in 6.4 (commit 48380368dec).
 * Old: DEFINE_SEMAPHORE(name)  →  count defaults to 1
 * New: DEFINE_SEMAPHORE(name, count)
 * Provide a helper for the legacy 1-arg form; callers must use the helper
 * rather than the kernel macro directly so the same source compiles on
 * both vintages. Do NOT #undef + redefine the kernel macro — that breaks
 * sources (cv500 / osal-linux mmz) that legitimately call DEFINE_SEMAPHORE
 * with two args on >= 6.4 kernels.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
#define compat_DEFINE_SEMAPHORE(name) DEFINE_SEMAPHORE(name, 1)
#else
#define compat_DEFINE_SEMAPHORE(name) DEFINE_SEMAPHORE(name)
#endif

/*
 * vm_flags became read-only in 6.3. Use vm_flags_set()/vm_flags_clear().
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 3, 0)
#define compat_vm_flags_set(vma, flags) vm_flags_set(vma, flags)
#else
#define compat_vm_flags_set(vma, flags) ((vma)->vm_flags |= (flags))
#endif

/*
 * get_random_int() renamed to get_random_u32() in 6.1.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 1, 0)
#define compat_get_random_int() get_random_u32()
#else
#define compat_get_random_int() get_random_int()
#endif

/*
 * struct file_operations replaced by struct proc_ops for proc files in 5.6.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define COMPAT_USE_PROC_OPS 1
#endif

/*
 * dma_alloc_from_contiguous() gained a 4th 'no_warn' parameter in 4.15.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0)
#define compat_dma_alloc_from_contiguous(dev, count, align) \
	dma_alloc_from_contiguous(dev, count, align, false)
#else
#define compat_dma_alloc_from_contiguous(dev, count, align) \
	dma_alloc_from_contiguous(dev, count, align)
#endif

/*
 * spi_busnum_to_master() removed in 6.4. Provide inline fallback.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
#include <linux/spi/spi.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(7, 0, 0)
extern const struct class spi_controller_class;
#define compat_spi_class spi_controller_class
#else
extern struct class spi_master_class;
#define compat_spi_class spi_master_class
#endif
static inline struct spi_controller *compat_spi_busnum_to_controller(u16 bus_num)
{
	struct device *dev;
	char name[32];
	snprintf(name, sizeof(name), "spi%u", bus_num);
	dev = class_find_device_by_name(&compat_spi_class, name);
	if (!dev)
		return NULL;
	return container_of(dev, struct spi_controller, dev);
}
#define spi_busnum_to_controller compat_spi_busnum_to_controller
#endif

/* Inserted before #endif */
/*
 * i2c_new_device renamed to i2c_new_client_device in 5.2 (and the old name
 * dropped later). Map legacy source forward, normalising the error return
 * (ERR_PTR -> NULL) so callers that check `if (client == NULL)` still work.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 2, 0)
#define i2c_new_client_device i2c_new_device
#else
#include <linux/i2c.h>
#include <linux/err.h>
static inline struct i2c_client *compat_i2c_new_device(
	struct i2c_adapter *adap, struct i2c_board_info const *info)
{
	struct i2c_client *c = i2c_new_client_device(adap, info);
	return IS_ERR(c) ? NULL : c;
}
#define i2c_new_device(adap, info) compat_i2c_new_device(adap, info)
#endif

/*
 * spi_busnum_to_master renamed to spi_busnum_to_controller in 5.x.
 * (6.4+ removal handled separately above)
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0)
#define spi_busnum_to_controller spi_busnum_to_master
#endif

/*
 * Re-export blob symbols only on kernels where modpost ignores the
 * legacy 8-byte __ksymtab_ format. Pre-5.0 modpost honours the legacy
 * blob ksymtab, so a second EXPORT_SYMBOL in our init wrapper would
 * collide ("multiple definition of __ksymtab_X"). On 5.0+ modpost
 * skips the legacy format — we must re-publish via EXPORT_SYMBOL to
 * make the symbol visible to other modules.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
#define COMPAT_REEXPORT_BLOB_SYMBOL(sym) EXPORT_SYMBOL(sym)
#else
#define COMPAT_REEXPORT_BLOB_SYMBOL(sym)
#endif

/*
 * Linux 7.0+ API changes
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(7, 0, 0)

/* platform_device.h no longer implicitly included */
#include <linux/platform_device.h>

/* del_timer moved to linux/timer.h (explicit include needed) */
#include <linux/timer.h>

#endif /* >= 7.0 */

/* del_timer renamed to timer_delete in 7.0 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(7, 0, 0)
#define del_timer timer_delete
#define del_timer_sync timer_delete_sync
#endif

/* platform_driver.remove returns void since 6.11 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 11, 0)
#define compat_platform_remove_ret void
#define compat_platform_remove_return return
#else
#define compat_platform_remove_ret int
#define compat_platform_remove_return return 0
#endif

/* spi_master typedef removed in 7.0 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(7, 0, 0)
#define spi_master spi_controller
#endif

/* from_timer macro removed in 7.0, use timer_container_of */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(7, 0, 0)
#include <linux/timer.h>
#define from_timer(var, callback_timer, timer_fieldname) \
	container_of(callback_timer, typeof(*var), timer_fieldname)
#endif

/* 7.0: register_sysctl() validates sentinel entries, causing spurious errors.
 * Use register_sysctl_sz() with explicit count to exclude sentinel. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(7, 0, 0)
#define compat_register_sysctl(path, table) \
    register_sysctl_sz(path, table, ARRAY_SIZE(table) - 1)
#else
#define compat_register_sysctl(path, table) \
    register_sysctl(path, table)
#endif

/*
 * printk renamed to _printk in 6.x. Blobs compiled against 4.9 reference
 * the old 'printk' symbol. We export a wrapper from the OSAL module.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 0, 0)
#define COMPAT_NEEDS_PRINTK_SHIM 1
#endif

/*
 * no_llseek() helper removed in 6.12 (commit 868941b14441). It was a
 * fops->llseek slot used to declare "this fd doesn't support llseek" —
 * modern code just leaves .llseek = NULL (or doesn't set it), which has
 * the same behaviour. Map to NULL so legacy source compiles unchanged.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 12, 0)
#define no_llseek NULL
#endif

/*
 * pte_offset_map() became an inline that calls __pte_offset_map() which
 * is NOT EXPORT_SYMBOL'd, so modules can't link it. Source walks of
 * user page tables (cv200/av100 mmz-userdev usr_virt_to_phys) need to
 * use an exported equivalent. pte_offset_kernel() is a static inline
 * over pmd_page_vaddr() + pte_index() — same lookup, no RCU section,
 * no exported symbol dependency. Matches the (unlocked) semantics of
 * the legacy code, which already did no locking.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 5, 0)
#define pte_offset_map(pmd, addr) pte_offset_kernel((pmd), (addr))
#endif

/*
 * hrtimer_init() + manual `timer->function = ...` was unified into a
 * single hrtimer_setup(timer, fn, clock, mode) call in Linux 6.7
 * (closes the small window where the timer could fire before the
 * function pointer was assigned). Provide hrtimer_setup() as an
 * inline shim on older kernels so driver code can be written in the
 * new style and still compile on the 3.x / 4.x / 5.x trees this
 * project supports.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 7, 0)
/*
 * Macro rather than a static inline so the compat header doesn't have
 * to force-include <linux/hrtimer.h> into every translation unit (the
 * compat header is `-include`d by kernel/Kbuild for the whole tree).
 * Drivers that actually use hrtimer_setup() include <linux/hrtimer.h>
 * themselves, which provides the type definitions the expanded macro
 * needs.
 */
#define hrtimer_setup(t, fn, clk, mode) do { \
	hrtimer_init((t), (clk), (mode)); \
	(t)->function = (fn); \
} while (0)
#endif

/*
 * gpio_cansleep() — the int-based legacy wrapper — was removed in
 * favour of the descriptor-based gpiod_cansleep() during the gpiolib
 * cleanups around Linux 6.5+. The legacy gpio_request / gpio_set_value
 * etc. integer API otherwise still works via CONFIG_GPIOLIB. Provide a
 * gpio_to_desc() bridge so drivers that hold an integer GPIO number
 * can still query "can this GPIO sleep?" without restructuring.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 5, 0)
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
static inline int gpio_cansleep_compat(unsigned int gpio)
{
	struct gpio_desc *d = gpio_to_desc(gpio);

	return d ? gpiod_cansleep(d) : 0;
}
#define gpio_cansleep(gpio) gpio_cansleep_compat(gpio)
#endif

#endif /* KERNEL_COMPAT_H */

