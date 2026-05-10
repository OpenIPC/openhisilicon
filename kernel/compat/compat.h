#ifndef COMPAT_H
#define COMPAT_H

#include <linux/i2c.h>
#include <linux/version.h>

#if defined(hi3516ev200)

/* hi3516ev200 hi3516ev300 hi3518ev300 hi3516dv200 */
#define MAIN_CHIPNAME "hi3516ev200"

#define C_HI3516EV200 "hi3516ev200"
#define C_HI3518EV300 "hi3518ev300"
#define C_HI3516EV300 "hi3516ev300"
#define C_HI3516DV200 "hi3516dv200"

#define PLATFORM_NAME "hisilicon"
#define PLATFORM_PRX "hi_"
#define HISI_PRX "hisi-"
#define HI_PRX "hi_"
#define VENDOR_PRX "hi35xx_"

/*
 * hi_i2c_master_send is exported by the vendor i2c-hibvt driver (in-kernel).
 * On mainline 6.6+ where i2c-hibvt may not be built, fall back to the
 * standard i2c_master_send which has the same signature and behavior.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 0, 0)
extern int hi_i2c_master_send(const struct i2c_client *client,
			      const char *buf, int count);
#define I2C_MASTER_SEND hi_i2c_master_send
#else
#define I2C_MASTER_SEND i2c_master_send
#endif
#define GET_CMA_ZONE hisi_get_cma_zone

#define DEFAULT_ALLOCATOR "hisi"

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
#define CONFIG_SNAPSHOT_BOOT 1
#endif

#ifdef CONFIG_CACHE_L2V200
#define CONFIG_CACHE_HIL2V200 1
#endif

#elif defined(gk7205v200)

/* gk7205v200 gk7205v300 gk7202v300 gk7605v100 */
#define MAIN_CHIPNAME "gk7205v200"

#define C_HI3516EV200 "gk7205v200"
#define C_HI3518EV300 "gk7202v300"
#define C_HI3516EV300 "gk7205v300"
#define C_HI3516DV200 "gk7605v100"

#define PLATFORM_NAME "goke"
#define PLATFORM_PRX ""
#define HISI_PRX ""
#define HI_PRX ""
#define VENDOR_PRX "vendor_"

/*
 * The vendor goke i2c-pl031-style driver exports gk_i2c_master_send for
 * sensor probing. On kernels where that symbol is unavailable (mainline
 * 6.x, or kernels built without the in-tree goke-i2c driver), fall back
 * to the standard i2c_master_send which has the same signature.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 0, 0)
extern int gk_i2c_master_send(const struct i2c_client *client,
			      const char *buf, int count);
#define I2C_MASTER_SEND gk_i2c_master_send
#else
#define I2C_MASTER_SEND i2c_master_send
#endif
#define GET_CMA_ZONE goke_get_cma_zone

#define DEFAULT_ALLOCATOR "gk"

#else
#error CHIPARCH must be set to supported values
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
/*
 * 6.4+ reworked symbol export internals (__CRC_SYMBOL, struct kernel_symbol
 * removed from public API). Use a no-op here; callers that need actual
 * aliases for binary blobs must define wrapper functions explicitly.
 * See EXPORT_COMPAT_MMZ in media-mem.c for the hil_* wrappers.
 */
#define EXPORT_ALIAS(sym, alias) /* no-op on 6.4+ */
#else
#define EXPORT_ALIAS(sym, alias)                                            \
	extern typeof(sym) sym;                                             \
	__CRC_SYMBOL(sym, "")                                               \
	static const char __kstrtab_##alias[]                               \
		__attribute__((section("__ksymtab_strings"), aligned(1))) = \
			VMLINUX_SYMBOL_STR(alias);                          \
	static const struct kernel_symbol __ksymtab_##alias __used          \
		__attribute__((section("___ksymtab+" #alias), used)) = {    \
			(unsigned long)&sym, __kstrtab_##alias              \
		}
#endif

#endif /* COMPAT_H */
