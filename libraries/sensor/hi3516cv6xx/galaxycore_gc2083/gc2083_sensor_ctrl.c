/*
 * GC2083 1080p30 linear RAW10 init: the vendor register table, verified
 * against an I2C capture of the vendor firmware (136/136 entries, #260).
 * The table ends with the stream-on writes; AE then runs through the sync
 * registers set up in gc2083_cmos.c.
 */
#include "sensor_common.h"
#include "gc2083_cfg.h"
#include "gc2083_sensor_ctrl.h"

td_s32 gc2083_linear_1080p30_10bit_init(cis_info *cis)
{
    td_u32 i;
    sns_check_pointer_return(cis);
    for (i = 0; i < (td_u32)GC2083_INIT_REG_COUNT; ++i) {
        sns_check_return(cis_write_reg(&cis->i2c,
            gc2083_linear_1080p30_10bit[i].addr,
            gc2083_linear_1080p30_10bit[i].data));
    }
    printf("== gc2083 27Mclk 1080P30fps(MIPI) 10bit linear init success! ==\n");
    return TD_SUCCESS;
}
