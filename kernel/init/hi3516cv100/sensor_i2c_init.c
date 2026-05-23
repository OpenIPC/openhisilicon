#include <linux/module.h>
extern int _cv100_hi_i2c_init(void);
extern void _cv100_hi_i2c_exit(void);
static int __init sensor_i2c_mod_init(void) { return _cv100_hi_i2c_init(); }
static void __exit sensor_i2c_mod_exit(void) { _cv100_hi_i2c_exit(); }
module_init(sensor_i2c_mod_init);
module_exit(sensor_i2c_mod_exit);
MODULE_LICENSE("GPL");

#define DECLARE_BLOB_FUNC(sym) extern int sym(void)

/* Re-export blob-internal __ksymtab entries from hi_i2c.o so modpost on
 * Linux 7.0 can resolve them across modules. The legacy ksymtab section
 * format (struct kernel_symbol, 8 bytes) is no longer recognized by
 * modern modpost; declaring each symbol via DECLARE_BLOB_FUNC + EXPORT_SYMBOL
 * regenerates the modern ksymtab metadata. */
DECLARE_BLOB_FUNC(HI_I2C_Read);				EXPORT_SYMBOL(HI_I2C_Read);
DECLARE_BLOB_FUNC(HI_I2C_Write);				EXPORT_SYMBOL(HI_I2C_Write);
DECLARE_BLOB_FUNC(HI_I2C_WriteConfig);		EXPORT_SYMBOL(HI_I2C_WriteConfig);
DECLARE_BLOB_FUNC(I2C_Close);					EXPORT_SYMBOL(I2C_Close);
DECLARE_BLOB_FUNC(I2C_Open);					EXPORT_SYMBOL(I2C_Open);
