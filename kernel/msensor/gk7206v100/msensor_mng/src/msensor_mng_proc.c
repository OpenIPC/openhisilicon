/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#include "msensor_mng_proc.h"
#include <linux/kernel.h>
#include "osal.h"
#include "msensor_buf.h"
#include "msensor_exe.h"

#define MSENSOR_MNG_INFO         "msensor_mng"
#define MSENSOR_MNG_VERSION_INFO "msensor_mng debug V0.0.0.1"

static xmedia_void msensor_print_proc_title(osal_proc_entry_t *s, xmedia_char *title)
{
    xmedia_s32 i;

    for (i = 0; i < 50; i++) {
        osal_seq_printf(s, "-");
    }

    osal_seq_printf(s, title);

    for (i = 0; i < 50; i++) {
        osal_seq_printf(s, "-");
    }
    osal_seq_printf(s, "\n");
}

static xmedia_void msensor_proc_show_gyro(xmedia_s32 dev, osal_proc_entry_t *s, msensor_buf_info (*buf_info)[DATA_BUTT])
{
    msensor_mng_proc_info *proc_info = msensor_mng_get_proc_info(dev);

    if (proc_info->gyro_name[0] == 0) {
        return;
    }

    msensor_print_proc_title(s, "gyro sensor name");
    osal_seq_printf(s, "%20s\n", proc_info->gyro_name);

    msensor_print_proc_title(s, "gyro sensor param");
    osal_seq_printf(s, "%16s%20s%16s%16s%16s%16s%16s\n", "dev_no", "buf_addr", "buf_size",
        "overflow", "data_unmatch", "overflow_id", "data_unmatch_id");
    osal_seq_printf(s, "%16d", dev);
    osal_seq_printf(s, "%20llx", proc_info->buf_addr[XMEDIA_MSENSOR_DATA_TYPE_GYRO]);
    osal_seq_printf(s, "%16u", proc_info->buf_size[XMEDIA_MSENSOR_DATA_TYPE_GYRO]);
    osal_seq_printf(s, "%16u", proc_info->buf_overflow[XMEDIA_MSENSOR_DATA_TYPE_GYRO]);
    osal_seq_printf(s, "%16u", proc_info->buf_data_unmatch[XMEDIA_MSENSOR_DATA_TYPE_GYRO]);
    osal_seq_printf(s, "%16d", proc_info->buf_overflow_id[XMEDIA_MSENSOR_DATA_TYPE_GYRO]);
    osal_seq_printf(s, "%16d\n", proc_info->buf_data_unmatch_id[XMEDIA_MSENSOR_DATA_TYPE_GYRO]);

    msensor_print_proc_title(s, "gyro sensor addr");
    osal_seq_printf(s, "%10s%20s%20s", "", "start_addr", "write_addr");
    osal_seq_printf(s, "\n%10s%20px%20px", "x", buf_info[XMEDIA_MSENSOR_DATA_TYPE_GYRO][DATA_X].start_addr,
                    buf_info[XMEDIA_MSENSOR_DATA_TYPE_GYRO][DATA_X].write_ptr);
    osal_seq_printf(s, "\n%10s%20px%20px", "y", buf_info[XMEDIA_MSENSOR_DATA_TYPE_GYRO][DATA_Y].start_addr,
                    buf_info[XMEDIA_MSENSOR_DATA_TYPE_GYRO][DATA_Y].write_ptr);
    osal_seq_printf(s, "\n%10s%20px%20px", "z", buf_info[XMEDIA_MSENSOR_DATA_TYPE_GYRO][DATA_Z].start_addr,
                    buf_info[XMEDIA_MSENSOR_DATA_TYPE_GYRO][DATA_Z].write_ptr);
    osal_seq_printf(s, "\n%10s%20px%20px", "temp", buf_info[XMEDIA_MSENSOR_DATA_TYPE_GYRO][DATA_TEMP].start_addr,
                    buf_info[XMEDIA_MSENSOR_DATA_TYPE_GYRO][DATA_TEMP].write_ptr);
    osal_seq_printf(s, "\n%10s%20px%20px\n", "pts", buf_info[XMEDIA_MSENSOR_DATA_TYPE_GYRO][DATA_PTS].start_addr,
                    buf_info[XMEDIA_MSENSOR_DATA_TYPE_GYRO][DATA_PTS].write_ptr);

    return;
}

static xmedia_void msensor_proc_show_acc(xmedia_s32 dev, osal_proc_entry_t *s, msensor_buf_info (*buf_info)[DATA_BUTT])
{
    msensor_mng_proc_info *proc_info = msensor_mng_get_proc_info(dev);

    if (proc_info->accel_name[0] == 0) {
        return;
    }

    msensor_print_proc_title(s, "acc sensor name");
    osal_seq_printf(s, "%21s\n", proc_info->accel_name);

    msensor_print_proc_title(s, "acc sensor param");

    osal_seq_printf(s, "%16s%20s%16s%16s%16s%16s%16s\n", "dev_no", "buf_addr", "buf_size",
        "overflow", "data_unmatch", "overflow_id", "data_unmatch_id");
    osal_seq_printf(s, "%16d", dev);
    osal_seq_printf(s, "%20llx", proc_info->buf_addr[XMEDIA_MSENSOR_DATA_TYPE_ACC]);
    osal_seq_printf(s, "%16u", proc_info->buf_size[XMEDIA_MSENSOR_DATA_TYPE_ACC]);
    osal_seq_printf(s, "%16u", proc_info->buf_overflow[XMEDIA_MSENSOR_DATA_TYPE_ACC]);
    osal_seq_printf(s, "%16u", proc_info->buf_data_unmatch[XMEDIA_MSENSOR_DATA_TYPE_ACC]);
    osal_seq_printf(s, "%16d", proc_info->buf_overflow_id[XMEDIA_MSENSOR_DATA_TYPE_ACC]);
    osal_seq_printf(s, "%16d\n", proc_info->buf_data_unmatch_id[XMEDIA_MSENSOR_DATA_TYPE_ACC]);

    msensor_print_proc_title(s, "acc sensor addr");
    osal_seq_printf(s, "%10s%20s%20s\n", "", "start_addr", "write_addr");
    osal_seq_printf(s, "%10s%20px%20px", "x", buf_info[XMEDIA_MSENSOR_DATA_TYPE_ACC][DATA_X].start_addr,
                    buf_info[XMEDIA_MSENSOR_DATA_TYPE_ACC][DATA_X].write_ptr);
    osal_seq_printf(s, "\n%10s%20px%20px", "y", buf_info[XMEDIA_MSENSOR_DATA_TYPE_ACC][DATA_Y].start_addr,
                    buf_info[XMEDIA_MSENSOR_DATA_TYPE_ACC][DATA_Y].write_ptr);
    osal_seq_printf(s, "\n%10s%20px%20px", "z", buf_info[XMEDIA_MSENSOR_DATA_TYPE_ACC][DATA_Z].start_addr,
                    buf_info[XMEDIA_MSENSOR_DATA_TYPE_ACC][DATA_Z].write_ptr);
    osal_seq_printf(s, "\n%10s%20px%20px", "temp", buf_info[XMEDIA_MSENSOR_DATA_TYPE_ACC][DATA_TEMP].start_addr,
                    buf_info[XMEDIA_MSENSOR_DATA_TYPE_ACC][DATA_TEMP].write_ptr);
    osal_seq_printf(s, "\n%10s%20px%20px\n", "pts", buf_info[XMEDIA_MSENSOR_DATA_TYPE_ACC][DATA_PTS].start_addr,
                    buf_info[XMEDIA_MSENSOR_DATA_TYPE_ACC][DATA_PTS].write_ptr);

    return;
}

static xmedia_s32 msensor_proc_show(osal_proc_entry_t *s)
{
    xmedia_s32 ret;
    msensor_buf_info(*buf_info)[DATA_BUTT] = XMEDIA_NULL;
    xmedia_s32 i;

    osal_seq_printf(s,
                    "[msensor] version:[" MSENSOR_MNG_VERSION_INFO "], build time["__DATE__
                    ", "__TIME__
                    "]\n");

    for (i = 0 ; i < XMEDIA_MSENSOR_MAX_DEV_NUM; i++) {

        if (msensor_buf_get_status(i) == XMEDIA_FALSE) {
            continue;
        }

        ret = msensor_mng_proc_info_init(i);
        if (ret != XMEDIA_SUCCESS) {
            continue;
        }

        buf_info = (msensor_buf_info(*)[DATA_BUTT])msensor_buf_get_info(i);

        msensor_proc_show_gyro(i, s, buf_info);
        msensor_proc_show_acc(i, s, buf_info);
    }

    return 0;
}

xmedia_s32 msensor_proc_init(xmedia_void)
{
    osal_proc_entry_t *msensor_entry = XMEDIA_NULL;

    msensor_entry = osal_create_proc_entry(MSENSOR_MNG_INFO, XMEDIA_NULL);
    if (msensor_entry == XMEDIA_NULL) {
        printk("osal_create_proc_entry failed!\n");
        return -1;
    }

    msensor_entry->read = msensor_proc_show;
    msensor_entry->write = XMEDIA_NULL;
    return 0;
}

void msensor_proc_exit(xmedia_void)
{
    osal_remove_proc_entry(MSENSOR_MNG_INFO, XMEDIA_NULL);
    return;
}

