#ifndef COMPAT_H
#define COMPAT_H

#if defined(hi3516ev200)

#define PLATFORM_NAME "hisilicon"
#define PLATFORM_PRX "hi_"
#define HISI_PRX "hisi-"
#define HI_PRX "hi_"
#define VENDOR_PRX "hi35xx_"

#define I2C_MASTER_SEND hi_i2c_master_send
#define GET_CMA_ZONE hisi_get_cma_zone

#elif defined (gk7205v200)

#define PLATFORM_NAME "goke"
#define PLATFORM_PRX ""
#define HISI_PRX ""
#define HI_PRX ""
#define VENDOR_PRX "vendor_"

#define I2C_MASTER_SEND gk_i2c_master_send
#define GET_CMA_ZONE goke_get_cma_zone

#else
#error CHIPSET must be set to supported values
#endif

#endif /* COMPAT_H */
