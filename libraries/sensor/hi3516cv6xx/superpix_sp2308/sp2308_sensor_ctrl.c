/*
 * SP2308 1080p30 linear RAW10 init: the hi3516ev200 driver's register table
 * (sp2308_cfg.h). It ends with the stream-on writes; AE then runs through the
 * sync registers set up in sp2308_cmos.c.
 */
#include "sensor_common.h"
#include "sp2308_cfg.h"
#include "sp2308_sensor_ctrl.h"

td_s32 sp2308_linear_1080p30_10bit_init(cis_info *cis)
{
    td_u32 i;
    sns_check_pointer_return(cis);
    for (i = 0; i < (td_u32)SP2308_INIT_REG_COUNT; ++i) {
        sns_check_return(cis_write_reg(&cis->i2c,
            sp2308_linear_1080p30_10bit[i].addr,
            sp2308_linear_1080p30_10bit[i].data));
    }
    printf("== sp2308 24Mclk 1080P30fps(MIPI) 2lane 10bit linear init success! ==\n");
    return TD_SUCCESS;
}
