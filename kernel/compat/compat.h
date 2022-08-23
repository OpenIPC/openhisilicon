#ifndef COMPAT_H
#define COMPAT_H

#if defined(hi3516ev200)

#define I2C_MASTER_SEND hi_i2c_master_send

#elif defined (gk7205v200)

#define I2C_MASTER_SEND gk_i2c_master_send

#else
#error CHIPSET must be set to supported values
#endif

#endif /* COMPAT_H */
