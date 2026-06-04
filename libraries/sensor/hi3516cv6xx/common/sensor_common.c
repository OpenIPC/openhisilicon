/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#ifdef OT_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "ot_i2c.h"
#endif

#include "sensor_common.h"
#include "securec.h"

td_void cis_delay_ms(td_u32 ms)
{
    usleep(ms * 1000); /* 1ms: 1000us */
    return;
}

static td_void cis_mutex_lock(pthread_mutex_t *m)
{
    if (pthread_mutex_lock(m) != 0) {
        isp_err_trace("pthread_mutex_lock failed!\n");
    }
}

static td_void cis_mutex_unlock(pthread_mutex_t *m)
{
    if (pthread_mutex_unlock(m) != 0) {
        isp_err_trace("pthread_mutex_unlock failed!\n");
    }
}

static td_s32 cis_create_state(cis_info *cis)
{
    ot_isp_sns_state *state = TD_NULL;

    cis_mutex_lock(&cis->mutex);

    if (cis->sns_state != TD_NULL) {
        isp_err_trace("sns has been created!\n");
        goto out;
    }

    state = (ot_isp_sns_state *)malloc(sizeof(ot_isp_sns_state));
    if (state == TD_NULL) {
        isp_err_trace("malloc memory failed!\n");
        goto out;
    }

    (td_void)memset_s(state, sizeof(ot_isp_sns_state), 0, sizeof(ot_isp_sns_state));

    cis->sns_state = state;

    cis_mutex_unlock(&cis->mutex);

    return TD_SUCCESS;

out:
    cis_mutex_unlock(&cis->mutex);
    return OT_ERR_ISP_NOMEM;
}

static td_void cis_destroy_state(cis_info *cis)
{
    cis_mutex_lock(&cis->mutex);

    sns_free(cis->sns_state);

    cis->sns_state = TD_NULL;

    cis_mutex_unlock(&cis->mutex);

    return;
}

td_s32 cis_register_callback(cis_info *cis, cis_register *reg)
{
    td_s32 ret;
    ot_isp_sns_attr_info sns_attr_info;

    sns_check_pointer_return(cis);
    sns_check_pointer_return(reg);

    sns_check_return(cis_create_state(cis));

    sns_attr_info.sns_id = cis->sns_id;
    ret = ot_mpi_isp_sensor_reg_callback(cis->pipe, &sns_attr_info, &reg->isp_register);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor register callback function failed!\n");
        goto OUT0;
    }

    ret = ot_mpi_ae_sensor_reg_callback(cis->pipe, reg->ae_lib, &sns_attr_info, &reg->ae_register);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor register callback function to ae lib failed!\n");
        goto OUT1;
    }

    ret = ot_mpi_awb_sensor_reg_callback(cis->pipe, reg->awb_lib, &sns_attr_info, &reg->awb_register);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor register callback function to awb lib failed!\n");
        goto OUT2;
    }

    return TD_SUCCESS;

OUT2:
    (td_void)ot_mpi_ae_sensor_unreg_callback(cis->pipe, reg->ae_lib, cis->sns_id);
OUT1:
    (td_void)ot_mpi_isp_sensor_unreg_callback(cis->pipe, cis->sns_id);
OUT0:
    cis_destroy_state(cis);
    return ret;
}

td_s32 cis_unregister_callback(cis_info *cis, cis_register *reg)
{
    td_s32 ret;

    sns_check_pointer_return(cis);
    sns_check_pointer_return(reg);

    ret = ot_mpi_isp_sensor_unreg_callback(cis->pipe, cis->sns_id);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor unregister callback function failed!\n");
        return ret;
    }

    ret = ot_mpi_ae_sensor_unreg_callback(cis->pipe, reg->ae_lib, cis->sns_id);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor unregister callback function to ae lib failed!\n");
        return ret;
    }

    ret = ot_mpi_awb_sensor_unreg_callback(cis->pipe, reg->awb_lib, cis->sns_id);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sensor unregister callback function to awb lib failed!\n");
        return ret;
    }

    cis_destroy_state(cis);

    return TD_SUCCESS;
}

td_s32 cis_i2c_init(cis_info *cis)
{
    cis_i2c *i2c = TD_NULL;

    sns_check_pointer_return(cis);

    i2c = &cis->i2c;
    if (i2c->fd >= 0) {
        isp_info_trace("i2c has inited,fd:%d\n", i2c->fd);
        return TD_SUCCESS;
    }

#ifdef OT_GPIO_I2C
    i2c->fd = open("/dev/gpioi2c_ex", O_RDONLY, S_IRUSR);
    if (i2c->fd < 0) {
        isp_err_trace("Open gpioi2c_ex error!\n");
        return TD_FAILURE;
    }
#else
    int ret;
    char dev_file[I2C_DEV_FILE_NUM] = {0};
    td_u8 dev_num;
    ot_isp_sns_commbus *bus_info = &cis->bus_info;
    dev_num = (td_u8)bus_info->i2c_dev;
    if (snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", dev_num) < 0) {
        isp_err_trace("snprintf_s error!\n");
        return TD_FAILURE;
    }

    i2c->fd = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
    if (i2c->fd < 0) {
        isp_err_trace("Open /dev/ot_i2c_drv-%u error!\n", dev_num);
        return TD_FAILURE;
    }

    ret = ioctl(i2c->fd, OT_I2C_SLAVE_FORCE, (i2c->addr >> 1));
    if (ret < 0) {
        isp_err_trace("I2C_SLAVE_FORCE error!\n");
        close(i2c->fd);
        i2c->fd = -1;
        return ret;
    }
#endif

    return TD_SUCCESS;
}

int cis_i2c_exit(cis_info *cis)
{
    cis_i2c *i2c = TD_NULL;

    sns_check_pointer_return(cis);

    i2c = &cis->i2c;
    if (i2c->fd >= 0) {
        close(i2c->fd);
        i2c->fd = -1;
        return TD_SUCCESS;
    }

    return TD_FAILURE;
}

td_s32 cis_read_reg(cis_i2c *i2c, td_u32 addr)
{
    ot_unused(i2c);
    ot_unused(addr);
    return TD_SUCCESS;
}

td_s32 cis_write_reg(cis_i2c *i2c, td_u32 addr, td_u32 data)
{
    td_s32 ret;

    sns_check_pointer_return(i2c);

    if (i2c->fd < 0) {
        isp_err_trace("fd is invalid!\n");
        return TD_FAILURE;
    }

#ifdef OT_GPIO_I2C
    i2c_data.dev_addr = i2c->addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = i2c->addr_byte_num;
    i2c_data.data = data;
    i2c_data.data_byte_num = i2c->data_byte_num;

    ret = ioctl(i2c->fd, GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        isp_err_trace("GPIO-I2C write failed!\n");
        return ret;
    }
#else
    td_u32 idx = 0;
    td_u8 buf[I2C_BUF_NUM];

    if (i2c->addr_byte_num == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
        buf[idx] = addr & 0xff;
        idx++;
    }

    if (i2c->data_byte_num == 2) {  /* 2 byte */
        buf[idx] = (data >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = data & 0xff;
        idx++;
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(i2c->fd, buf, i2c->addr_byte_num + i2c->data_byte_num);
    if (ret < 0) {
        isp_err_trace("I2C_WRITE error!\n");
        return TD_FAILURE;
    }
#endif

    return TD_SUCCESS;
}

td_s32 cis_wdr_range_check(td_u32 *ratio)
{
    if (ratio[0] < 64 || ratio[0] > 2047) { // 0: idx 64: min_ratio, 2047: max_ratio
        isp_err_trace("err ratio %u, range [64, 2048)\n", ratio[0]);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_void cis_init_attr(cis_info *cis, ot_isp_init_attr *init_attr)
{
    sns_check_pointer_void_return(cis);
    sns_check_pointer_void_return(init_attr);

    cis->init_exposure  = init_attr->exposure;
    cis->init_int_time  = init_attr->exp_time;
    cis->init_again     = init_attr->a_gain;
    cis->init_dgain     = init_attr->d_gain;
    cis->init_isp_dgain = init_attr->ispd_gain;
    cis->lines_per500ms = init_attr->lines_per500ms;
    cis->init_wb_r_gain = init_attr->wb_r_gain;
    cis->init_wb_g_gain = init_attr->wb_g_gain;
    cis->init_wb_b_gain = init_attr->wb_b_gain;
    cis->sample_r_gain = init_attr->sample_r_gain;
    cis->sample_b_gain = init_attr->sample_b_gain;
    cis->quick_start_en = init_attr->quick_start_en;
    cis->ae_stat_pos       = (td_u8)init_attr->ae_stat_pos;
    cis->ae_route_ex_valid = init_attr->ae_route_ex_valid;

    (td_void)memcpy_s(&cis->init_ae_route, sizeof(ot_isp_ae_route),
                      &init_attr->ae_route, sizeof(ot_isp_ae_route));
    (td_void)memcpy_s(&cis->init_ae_route_ex, sizeof(ot_isp_ae_route_ex),
                      &init_attr->ae_route_ex, sizeof(ot_isp_ae_route_ex));
    (td_void)memcpy_s(&cis->init_ae_route_sf, sizeof(ot_isp_ae_route),
                      &init_attr->ae_route_sf, sizeof(ot_isp_ae_route));
    (td_void)memcpy_s(&cis->init_ae_route_sf_ex, sizeof(ot_isp_ae_route_ex),
                      &init_attr->ae_route_sf_ex, sizeof(ot_isp_ae_route_ex));
}

