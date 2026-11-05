/*
 * MIS20S1 1080p30 linear RAW12 init, as the vendor's Hi3516CV610 firmware
 * does it: the register table (mis20s1_cfg.h), with its pauses. AE then runs
 * through the sync registers set up in mis20s1_cmos.c.
 */
#include <stdio.h>
#include "sensor_common.h"
#include "mis20s1_cfg.h"
#include "mis20s1_sensor_ctrl.h"

td_s32 mis20s1_linear_1080p30_12bit_init(cis_info *cis)
{
    td_u32 i;
    sns_check_pointer_return(cis);
    cis_delay_ms(1); /* the vendor's loader starts with one */
    for (i = 0; i < (td_u32)MIS20S1_INIT_REG_COUNT; ++i) {
        if (mis20s1_linear_1080p30_12bit[i].addr == MIS20S1_TBL_DELAY) {
            cis_delay_ms(mis20s1_linear_1080p30_12bit[i].data);
            continue;
        }
        sns_check_return(cis_write_reg(&cis->i2c,
            mis20s1_linear_1080p30_12bit[i].addr,
            mis20s1_linear_1080p30_12bit[i].data));
    }
    printf("== mis20s1 1080P30fps(MIPI) 2lane 12bit linear init success! ==\n");
    return TD_SUCCESS;
}
