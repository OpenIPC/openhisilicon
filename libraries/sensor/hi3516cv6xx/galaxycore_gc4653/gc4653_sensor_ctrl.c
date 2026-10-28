/*
 * GC4653 2560x1440p30 linear RAW10 init, as the vendor's Hi3516CV610
 * firmware does it: the register table (gc4653_cfg.h), 20 ms, the stream on
 * once more, 20 ms. AE then runs through the sync registers set up in
 * gc4653_cmos.c.
 */
#include <unistd.h>
#include "sensor_common.h"
#include "gc4653_cfg.h"
#include "gc4653_sensor_ctrl.h"

td_s32 gc4653_linear_4m30_10bit_init(cis_info *cis)
{
    td_u32 i;
    sns_check_pointer_return(cis);
    for (i = 0; i < (td_u32)GC4653_INIT_REG_COUNT; ++i) {
        sns_check_return(cis_write_reg(&cis->i2c,
            gc4653_linear_4m30_10bit[i].addr,
            gc4653_linear_4m30_10bit[i].data));
    }
    usleep(20000);
    sns_check_return(cis_write_reg(&cis->i2c, GC4653_STREAM_ADDR, GC4653_STREAM_ON));
    usleep(20000);
    printf("== gc4653 27Mclk 4M30fps(MIPI) 2lane 10bit linear init success! ==\n");
    return TD_SUCCESS;
}
