/*
 * VS4388 2560x1440p30 linear RAW10 init, as the vendor's Hi3516CV610
 * firmware does it: the register table (vs4388_cfg.h), soft reset first and
 * stream on last, with no pauses. AE then runs through the sync registers
 * set up in vs4388_cmos.c.
 */
#include "sensor_common.h"
#include "vs4388_cfg.h"
#include "vs4388_sensor_ctrl.h"

td_s32 vs4388_linear_1440p30_10bit_init(cis_info *cis)
{
    td_u32 i;
    sns_check_pointer_return(cis);
    for (i = 0; i < (td_u32)VS4388_INIT_REG_COUNT; ++i) {
        sns_check_return(cis_write_reg(&cis->i2c,
            vs4388_linear_1440p30_10bit[i].addr,
            vs4388_linear_1440p30_10bit[i].data));
    }
    printf("== vs4388 1440P30fps(MIPI) 2lane 10bit linear init success! ==\n");
    return TD_SUCCESS;
}
