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
 * do_gettimeofday() removed in 5.0.
 * Use ktime_get_real_ts64() + timespec64 instead.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
#define COMPAT_NO_DO_GETTIMEOFDAY 1
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#endif

/*
 * rtc_time_to_tm/rtc_tm_to_time removed in 5.6 in favor of
 * rtc_time64_to_tm/rtc_tm_to_time64.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define compat_rtc_time_to_tm  rtc_time64_to_tm
#define compat_rtc_tm_to_time  rtc_tm_to_time64
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
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 1, 0)
#define compat_strlcpy(dst, src, size) strscpy(dst, src, size)
#else
#define compat_strlcpy(dst, src, size) strlcpy(dst, src, size)
#endif

/*
 * PDE_DATA() renamed to pde_data() in 5.17.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 17, 0)
#define compat_pde_data(inode) pde_data(inode)
#elif defined(CONFIG_PROC_FS)
#define compat_pde_data(inode) PDE_DATA(inode)
#endif

/*
 * vm_flags field became read-only in 6.3 (use vm_flags_set/vm_flags_clear).
 * For read-only checks this doesn't matter.
 */

#endif /* KERNEL_COMPAT_H */
