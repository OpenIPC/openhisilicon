#ifndef COMPAT_H
#define COMPAT_H

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

#define I2C_MASTER_SEND hi_i2c_master_send
#define GET_CMA_ZONE hisi_get_cma_zone

#define DEFAULT_ALLOCATOR "gk"

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

#define I2C_MASTER_SEND gk_i2c_master_send
#define GET_CMA_ZONE goke_get_cma_zone

#define DEFAULT_ALLOCATOR "hisi"

#else
#error CHIPARCH must be set to supported values
#endif

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

#endif /* COMPAT_H */
