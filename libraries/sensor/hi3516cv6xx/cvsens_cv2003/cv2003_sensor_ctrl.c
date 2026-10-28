/*
 * CV2003 1080p30 linear RAW10 init, as the vendor's CV608 firmware does it:
 * a soft reset, a millisecond for it to take, the register table
 * (cv2003_cfg.h), then the stream. AE then runs through the sync registers
 * set up in cv2003_cmos.c.
 */
#include <unistd.h>
#include "sensor_common.h"
#include "cv2003_cfg.h"
#include "cv2003_sensor_ctrl.h"

td_s32 cv2003_linear_1080p30_10bit_init(cis_info *cis)
{
    td_u32 i;
    sns_check_pointer_return(cis);
    sns_check_return(cis_write_reg(&cis->i2c, CV2003_RESET_ADDR, 0x01));
    sns_check_return(cis_write_reg(&cis->i2c, CV2003_RESET_ADDR, 0x00));
    usleep(1000);
    for (i = 0; i < (td_u32)CV2003_INIT_REG_COUNT; ++i) {
        sns_check_return(cis_write_reg(&cis->i2c,
            cv2003_linear_1080p30_10bit[i].addr,
            cv2003_linear_1080p30_10bit[i].data));
    }
    sns_check_return(cis_write_reg(&cis->i2c, CV2003_STREAM_ADDR, 0x00));
    printf("== cv2003 24Mclk 1080P30fps(MIPI) 2lane 10bit linear init success! ==\n");
    return TD_SUCCESS;
}
