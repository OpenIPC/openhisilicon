/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#include "msensor_buf.h"
#include <linux/kernel.h>
#include "msensor_exe.h"
#include "osal.h"

static xmedia_bool g_forward[XMEDIA_MSENSOR_MAX_DEV_NUM] = { XMEDIA_TRUE };
static xmedia_bool g_buf_init[XMEDIA_MSENSOR_MAX_DEV_NUM] = { XMEDIA_FALSE };
msensor_buf_info g_buf_info[XMEDIA_MSENSOR_MAX_DEV_NUM][XMEDIA_MSENSOR_DATA_TYPE_MAX][DATA_BUTT];
static xmedia_s64 g_offset[XMEDIA_MSENSOR_MAX_DEV_NUM] = { 0 };
xmedia_bool g_already_released[XMEDIA_MSENSOR_MAX_DEV_NUM][MAX_USER_NUM] = { XMEDIA_FALSE };
msensor_buf_user_mng g_user_mng[XMEDIA_MSENSOR_MAX_DEV_NUM];
static unsigned long g_msensor_buf_len[XMEDIA_MSENSOR_MAX_DEV_NUM] = { 0 };

#define DATA_RESERVE_NUM                 50
#define xmedia_align(x, a)               ((a) * (((x) + (a) - 1) / (a)))

#define x_start_addr(dev, data_type)     g_buf_info[dev][data_type][DATA_X].start_addr
#define y_start_addr(dev, data_type)     g_buf_info[dev][data_type][DATA_Y].start_addr
#define z_start_addr(dev, data_type)     g_buf_info[dev][data_type][DATA_Z].start_addr
#define temp_start_addr(dev, data_type)  g_buf_info[dev][data_type][DATA_TEMP].start_addr
#define pts_start_addr(dev, data_type)   g_buf_info[dev][data_type][DATA_PTS].start_addr

#define x_write_ptr(dev, data_type)      g_buf_info[dev][data_type][DATA_X].write_ptr
#define y_write_ptr(dev, data_type)      g_buf_info[dev][data_type][DATA_Y].write_ptr
#define z_write_ptr(dev, data_type)      g_buf_info[dev][data_type][DATA_Z].write_ptr
#define temp_write_ptr(dev, data_type)   g_buf_info[dev][data_type][DATA_TEMP].write_ptr
#define pts_write_ptr(dev, data_type)    g_buf_info[dev][data_type][DATA_PTS].write_ptr

#define x_read_ptr(dev, i, data_type)    g_user_mng[dev].user_ctx[i]->read_ptr[data_type][DATA_X]
#define y_read_ptr(dev, i, data_type)    g_user_mng[dev].user_ctx[i]->read_ptr[data_type][DATA_Y]
#define z_read_ptr(dev, i, data_type)    g_user_mng[dev].user_ctx[i]->read_ptr[data_type][DATA_Z]
#define temp_read_ptr(dev, i, data_type) g_user_mng[dev].user_ctx[i]->read_ptr[data_type][DATA_TEMP]
#define pts_read_ptr(dev, i, data_type)  g_user_mng[dev].user_ctx[i]->read_ptr[data_type][DATA_PTS]

msensor_buf_info **msensor_buf_get_info(xmedia_s32 dev)
{
    return (msensor_buf_info **)g_buf_info[dev];
}

xmedia_bool msensor_buf_get_status(xmedia_s32 dev)
{
    return g_buf_init[dev];
}

osal_spinlock_t *msensor_buf_get_lock(xmedia_s32 dev)
{
    return &g_user_mng[dev].mng_lock;
}

static xmedia_s32 msensor_buf_get_user_id(xmedia_s32 dev, xmedia_s32 *id)
{
    xmedia_s32 i;

    for (i = 0; i < MAX_USER_NUM; i++) {
        if (g_user_mng[dev].user_ctx[i] == XMEDIA_NULL) {
            *id = i;
            return XMEDIA_SUCCESS;
        }
    }

    MSENSOR_ERR_TRACE("dev %d no id for user.\n", dev);
    return XMEDIA_FAILURE;
}

static xmedia_s32 msensor_buf_add_user_create_ctx(xmedia_s32 dev, const xmedia_s32 *id)
{
    xmedia_s32 i;
    xmedia_s32 j;
    xmedia_ulong flags;
    msensor_buf_user_context *user_ctx = XMEDIA_NULL;
    msensor_mng_proc_info *proc_info = XMEDIA_NULL;
    proc_info = msensor_mng_get_proc_info(dev);

    user_ctx = (msensor_buf_user_context *)osal_vmalloc(sizeof(msensor_buf_user_context));
    if (user_ctx == XMEDIA_NULL) {
        MSENSOR_ERR_TRACE("dev %d osal_vmalloc failed.\n", dev);
        return XMEDIA_FAILURE;
    }

    (xmedia_void) osal_memset(user_ctx, 0, sizeof(msensor_buf_user_context));

    osal_spin_lock_irqsave(&g_user_mng[dev].read_lock[*id], &flags);

    for (i = 0; i < DATA_BUTT; i++) {
        if (i == DATA_PTS) {
            for (j = 0; j < XMEDIA_MSENSOR_DATA_TYPE_MAX; j++) {
                user_ctx->read_ptr[j][i] = (proc_info->buf_size[j] > DATA_RESERVE_NUM * BUF_BLOCK_NUM)
                    ? (xmedia_u64 *)g_buf_info[dev][j][i].write_ptr
                    : g_buf_info[dev][j][i].write_ptr;
            }
        } else {
            for (j = 0; j < XMEDIA_MSENSOR_DATA_TYPE_MAX; j++) {
                user_ctx->read_ptr[j][i] = (int *)g_buf_info[dev][j][i].write_ptr;
            }
        }
    }

    g_forward[dev] = XMEDIA_TRUE;
    g_user_mng[dev].user_ctx[*id] = user_ctx;
    g_user_mng[dev].user_cnt++;

    osal_spin_unlock_irqrestore(&g_user_mng[dev].read_lock[*id], &flags);
    return XMEDIA_SUCCESS;
}

xmedia_s32 msensor_buf_add_user(xmedia_s32 dev, xmedia_s32 *id)
{
    xmedia_s32 ret;

    if (id == NULL) {
        MSENSOR_ERR_TRACE("dev %d msensor id is null.\n", dev);
        return XMEDIA_ERRCODE_NULL_PTR;
    }

    if (g_buf_init[dev] == XMEDIA_FALSE) {
        MSENSOR_ERR_TRACE("dev %d buf not init, g_buf_init:%d\n", dev, g_buf_init[dev]);
        return XMEDIA_FAILURE;
    }

    osal_mutex_lock(&g_user_mng[dev].mng_mutex);

    /* step 1: get available ID */
    if (g_user_mng[dev].user_cnt > MAX_USER_NUM) {
        MSENSOR_ERR_TRACE("dev %d msensor id has reached toplimit.\n", dev);
        goto err0;
    }

    /* get user id for use */
    ret = msensor_buf_get_user_id(dev, id);
    if (ret != XMEDIA_SUCCESS) {
        *id = -1;
        MSENSOR_ERR_TRACE("dev %d get_user_id failed.\n", dev);
        goto err0;
    }

    if ((*id < 0) || (*id >= MAX_USER_NUM)) {
        MSENSOR_ERR_TRACE("dev %d id(%d) out of range[0,%d].\n", dev, *id, MAX_USER_NUM);
        goto err0;
    }

    /* step 2: create context for new ID */
    ret = msensor_buf_add_user_create_ctx(dev, id);
    if (ret != XMEDIA_SUCCESS) {
        goto err0;
    }

    osal_mutex_unlock(&g_user_mng[dev].mng_mutex);
    osal_msleep(200); /* 200ms */
    return XMEDIA_SUCCESS;
err0:
    osal_mutex_unlock(&g_user_mng[dev].mng_mutex);
    return XMEDIA_FAILURE;
}

xmedia_s32 msensor_buf_delete_user(xmedia_s32 dev, const xmedia_s32 *id)
{
    msensor_buf_user_context *msenser_buf_user_contex_temp = XMEDIA_NULL;
    xmedia_ulong flags;

    if (id == NULL) {
        MSENSOR_ERR_TRACE("dev %d msensor id is null.\n", dev);
        return XMEDIA_ERRCODE_NULL_PTR;
    }

    if ((*id < 0) || (*id >= MAX_USER_NUM)) {
        MSENSOR_ERR_TRACE("dev %d id(%d) out of range[0,%d].\n", dev, *id, MAX_USER_NUM);
        return XMEDIA_FAILURE;
    }

    if (g_buf_init[dev] == XMEDIA_FALSE) {
        MSENSOR_ERR_TRACE("dev %d msensor buffer not inited.\n", dev);
        return XMEDIA_FAILURE;
    }

    if (g_user_mng[dev].user_ctx[*id] == XMEDIA_NULL) {
        msensor_debug_trace("dev %d id is null.\n", dev);
        return XMEDIA_SUCCESS;
    }

    osal_mutex_lock(&g_user_mng[dev].mng_mutex);
    osal_spin_lock_irqsave(&g_user_mng[dev].mng_lock, &flags);

    /* release context */
    msenser_buf_user_contex_temp = g_user_mng[dev].user_ctx[*id];

    g_user_mng[dev].user_ctx[*id] = XMEDIA_NULL;
    g_user_mng[dev].user_cnt--;

    osal_spin_unlock_irqrestore(&g_user_mng[dev].mng_lock, &flags);
    osal_mutex_unlock(&g_user_mng[dev].mng_mutex);

    if (msenser_buf_user_contex_temp != XMEDIA_NULL) {
        osal_vfree(msenser_buf_user_contex_temp);
        msenser_buf_user_contex_temp = XMEDIA_NULL;
    }
    return XMEDIA_SUCCESS;
}

static xmedia_void msensor_buf_init_proc(xmedia_s32 dev, const xmedia_u32 *gyro_block_size,
                                         const xmedia_u32 *acc_block_size, const xmedia_u32 *mag_block_size)
{
    msensor_mng_proc_info *proc_info = XMEDIA_NULL;
    proc_info = msensor_mng_get_proc_info(dev);

    proc_info->buf_addr[XMEDIA_MSENSOR_DATA_TYPE_GYRO] =
        (xmedia_u64)(xmedia_uintptr_t)g_buf_info[dev][XMEDIA_MSENSOR_DATA_TYPE_GYRO][DATA_X].start_addr;
    proc_info->buf_addr[XMEDIA_MSENSOR_DATA_TYPE_ACC] =
        (xmedia_u64)(xmedia_uintptr_t)g_buf_info[dev][XMEDIA_MSENSOR_DATA_TYPE_ACC][DATA_X].start_addr;

    proc_info->buf_size[XMEDIA_MSENSOR_DATA_TYPE_GYRO] = *gyro_block_size * BUF_BLOCK_NUM;
    proc_info->buf_size[XMEDIA_MSENSOR_DATA_TYPE_ACC] = *acc_block_size * BUF_BLOCK_NUM;

    msensor_debug_trace("##### dev %d gyro-au64_buf_addr:%llu acc-au64_buf_addr:%llu\n", dev,
                        proc_info->buf_addr[XMEDIA_MSENSOR_DATA_TYPE_GYRO],
                        proc_info->buf_addr[XMEDIA_MSENSOR_DATA_TYPE_ACC]);
    return;
}

static xmedia_void msensor_buf_allocation_init(xmedia_s32 dev, xmedia_u32 buflen, xmedia_u32 gyro_odr,
                                               xmedia_u32 accel_odr, xmedia_u32 mag_odr, const xmedia_void *vir_addr)
{
    xmedia_s32 i;
    xmedia_u32 gyro_block_size;
    xmedia_u32 acc_block_size;
    xmedia_u32 mag_block_size;

    gyro_block_size = buflen * gyro_odr / (gyro_odr + accel_odr + mag_odr) / BUF_BLOCK_NUM / 32 * 32; /* align by 32 */
    acc_block_size = buflen * accel_odr / (gyro_odr + accel_odr + mag_odr) / BUF_BLOCK_NUM / 32 * 32; /* align by 32 */
    mag_block_size = (buflen / BUF_BLOCK_NUM - gyro_block_size - acc_block_size) / 32 * 32; /* align by 32 */

    msensor_debug_trace("dev %d gyro block size %u, acc block size %d, mag block size %u\n", dev, gyro_block_size,
                        acc_block_size, mag_block_size);

    for (i = 0; i < DATA_BUTT; i++) {
        g_buf_info[dev][XMEDIA_MSENSOR_DATA_TYPE_GYRO][i].start_addr = (xmedia_u8 *)vir_addr + gyro_block_size * i;
        g_buf_info[dev][XMEDIA_MSENSOR_DATA_TYPE_GYRO][i].write_ptr =
            g_buf_info[dev][XMEDIA_MSENSOR_DATA_TYPE_GYRO][i].start_addr;

        g_buf_info[dev][XMEDIA_MSENSOR_DATA_TYPE_ACC][i].start_addr =
            (xmedia_u8 *)vir_addr + gyro_block_size * BUF_BLOCK_NUM + acc_block_size * i;
        g_buf_info[dev][XMEDIA_MSENSOR_DATA_TYPE_ACC][i].write_ptr =
            g_buf_info[dev][XMEDIA_MSENSOR_DATA_TYPE_ACC][i].start_addr;
    }

    msensor_debug_trace("**pts-write_pointer:%p\n", g_buf_info[dev][XMEDIA_MSENSOR_DATA_TYPE_GYRO][DATA_PTS].write_ptr);

    msensor_buf_init_proc(dev, &gyro_block_size, &acc_block_size, &mag_block_size);
    return;
}

xmedia_s32 msensor_buf_init(xmedia_s32 dev, const xmedia_msensor_buf_attr *buf_attr, xmedia_u32 gyro_odr,
                            xmedia_u32 accel_odr, xmedia_u32 mag_odr)
{
    xmedia_void *vir_addr = XMEDIA_NULL;
    xmedia_s32 i;

    if (g_buf_init[dev] == XMEDIA_TRUE) {
        MSENSOR_ERR_TRACE("dev %d buf already inited\n", dev);
        return XMEDIA_SUCCESS;
    }

    if ((buf_attr->phys_addr == 0) || (buf_attr->buf_len == 0)) {
        MSENSOR_ERR_TRACE("dev %d buf addr can not be null and buf size must lager than 0\n", dev);
        return XMEDIA_FAILURE;
    }

    if ((gyro_odr == 0) && (accel_odr == 0) && (mag_odr == 0)) {
        MSENSOR_ERR_TRACE("dev %d can't all frequency be 0\n", dev);
        return XMEDIA_FAILURE;
    }

    /* modify for 64bit chip */
    vir_addr = osal_ioremap_wc((xmedia_u64)buf_attr->phys_addr, xmedia_align(buf_attr->buf_len, 4));
    if (vir_addr == XMEDIA_NULL) {
        MSENSOR_ERR_TRACE("dev %d ioremap err\n", dev);
        return XMEDIA_FAILURE;
    }

    g_msensor_buf_len[dev] = xmedia_align(buf_attr->buf_len, 4);
    (xmedia_void) osal_memset(vir_addr, 0, buf_attr->buf_len);

    msensor_debug_trace("dev %d phy_addr:%llx vir_addr:%p buflen:%d\n", dev, buf_attr->phys_addr, vir_addr,
                        buf_attr->buf_len);

    g_offset[dev] = buf_attr->phys_addr - (xmedia_u64)(xmedia_uintptr_t)vir_addr;

    msensor_buf_allocation_init(dev, buf_attr->buf_len, gyro_odr, accel_odr, mag_odr, vir_addr);

    osal_mutex_init(&g_user_mng[dev].mng_mutex);

    for (i = 0; i < MAX_USER_NUM; i++) {
        g_already_released[dev][i] = XMEDIA_TRUE;
    }
    g_buf_init[dev] = XMEDIA_TRUE;
    return XMEDIA_SUCCESS;
}

xmedia_s32 msensor_buf_exit(xmedia_s32 dev)
{
    xmedia_s32 i;
    xmedia_s32 ret;

    if (g_buf_init[dev] == XMEDIA_FALSE) {
        MSENSOR_ERR_TRACE("dev %d buf already deinited\n", dev);
        return XMEDIA_SUCCESS;
    }

    for (i = 0; i < MAX_USER_NUM; i++) {
        ret = msensor_buf_delete_user(dev, &i);
        if (ret != XMEDIA_SUCCESS) {
            MSENSOR_ERR_TRACE("dev %d msensor delete user failed\n", dev);
        }
    }

    osal_iounmap(g_buf_info[dev][XMEDIA_MSENSOR_DATA_TYPE_GYRO][DATA_X].start_addr);
    g_msensor_buf_len[dev] = 0;

    (xmedia_void) osal_memset(&g_buf_info[dev], 0, sizeof(g_buf_info[dev]));
    g_buf_init[dev] = XMEDIA_FALSE;
    return ret;
}

static xmedia_void msensor_buf_write(xmedia_s32 dev, xmedia_msensor_data_type data_type,
                                     const xmedia_msensor_sample_data *sample_data)
{
    xmedia_void *next_write_pointer = XMEDIA_NULL;

    /* step 2:write data */
    *(xmedia_s32 *)x_write_ptr(dev, data_type) = sample_data->x;
    *(xmedia_s32 *)y_write_ptr(dev, data_type) = sample_data->y;
    *(xmedia_s32 *)z_write_ptr(dev, data_type) = sample_data->z;
    *(xmedia_s32 *)temp_write_ptr(dev, data_type) = sample_data->temperature;
    *(xmedia_u64 *)pts_write_ptr(dev, data_type) = sample_data->pts;

    /* step 3:calculate next write pointer */
    next_write_pointer = (xmedia_u8 *)x_write_ptr(dev, data_type) + sizeof(sample_data->x);

    if (next_write_pointer >= y_start_addr(dev, data_type)) {
        x_write_ptr(dev, data_type) = x_start_addr(dev, data_type);
        y_write_ptr(dev, data_type) = y_start_addr(dev, data_type);
        z_write_ptr(dev, data_type) = z_start_addr(dev, data_type);
        temp_write_ptr(dev, data_type) = temp_start_addr(dev, data_type);
        pts_write_ptr(dev, data_type) = pts_start_addr(dev, data_type);
    } else {
        x_write_ptr(dev, data_type) = (xmedia_u8 *)x_write_ptr(dev, data_type) + sizeof(sample_data->x);
        y_write_ptr(dev, data_type) = (xmedia_u8 *)y_write_ptr(dev, data_type) + sizeof(sample_data->y);
        z_write_ptr(dev, data_type) = (xmedia_u8 *)z_write_ptr(dev, data_type) + sizeof(sample_data->z);
        temp_write_ptr(dev, data_type) = (xmedia_u8 *)temp_write_ptr(dev, data_type) + sizeof(sample_data->temperature);
        pts_write_ptr(dev, data_type) = (xmedia_u8 *)pts_write_ptr(dev, data_type) + sizeof(sample_data->pts);
    }
    return;
}

xmedia_s32 msensor_buf_write_data(xmedia_s32 dev, xmedia_msensor_data_type data_type,
                                  const xmedia_msensor_sample_data *sample_data)
{
    xmedia_s32 i;
    xmedia_void *tmp_ptr = XMEDIA_NULL;
    xmedia_bool condition;
    msensor_mng_proc_info *proc_info = XMEDIA_NULL;
    xmedia_ulong flags;

    proc_info = msensor_mng_get_proc_info(dev);

    if (g_buf_init[dev] == XMEDIA_FALSE) {
        MSENSOR_ERR_TRACE("dev %d when write_data(pts:%lld),msensor buf not be inited yet\n", dev, sample_data->pts);
        return XMEDIA_FAILURE;
    }

    /* step 1:judge if any user overflow */
    if ((xmedia_u8 *)y_start_addr(dev, data_type) - (xmedia_u8 *)x_write_ptr(dev, data_type) >
        (xmedia_s32)(sizeof(sample_data->x) * WR_GAP)) {
        tmp_ptr = (xmedia_u8 *)x_write_ptr(dev, data_type) + sizeof(sample_data->x) * WR_GAP;
    } else {
        tmp_ptr = (xmedia_u8 *)x_start_addr(dev, data_type) + sizeof(sample_data->x) * WR_GAP -
            ((xmedia_u8 *)y_start_addr(dev, data_type) - (xmedia_u8 *)x_write_ptr(dev, data_type));
    }

    for (i = 0; i < MAX_USER_NUM; i++) {
        if (g_user_mng[dev].user_ctx[i] != XMEDIA_NULL) {
            if (((tmp_ptr == y_start_addr(dev, data_type)) &&
                 (x_read_ptr(dev, i, data_type) == x_start_addr(dev, data_type))) ||
                (tmp_ptr == x_read_ptr(dev, i, data_type))) {
                proc_info->buf_overflow[data_type]++;
                proc_info->buf_overflow_id[data_type] = i;

                osal_spin_lock_irqsave(&g_user_mng[dev].read_lock[i], &flags);
                condition =
                    ((xmedia_u32 *)x_read_ptr(dev, i, data_type) + 1) >= (xmedia_u32 *)y_start_addr(dev, data_type);
                x_read_ptr(dev, i, data_type) =
                    condition ? x_start_addr(dev, data_type) : (xmedia_u32 *)x_read_ptr(dev, i, data_type) + 1;
                y_read_ptr(dev, i, data_type) =
                    condition ? y_start_addr(dev, data_type) : (xmedia_u32 *)y_read_ptr(dev, i, data_type) + 1;
                z_read_ptr(dev, i, data_type) =
                    condition ? z_start_addr(dev, data_type) : (xmedia_u32 *)z_read_ptr(dev, i, data_type) + 1;
                temp_read_ptr(dev, i, data_type) =
                    condition ? temp_start_addr(dev, data_type) : (xmedia_u32 *)temp_read_ptr(dev, i, data_type) + 1;
                pts_read_ptr(dev, i, data_type) =
                    condition ? pts_start_addr(dev, data_type) : (xmedia_u64 *)pts_read_ptr(dev, i, data_type) + 1;
                osal_spin_unlock_irqrestore(&g_user_mng[dev].read_lock[i], &flags);
            }
        }
    }

    /* write data */
    msensor_buf_write(dev, data_type, sample_data);
    return XMEDIA_SUCCESS;
}

static xmedia_void msensor_buf_release(xmedia_s32 dev, xmedia_msensor_data_info *data_info)
{
    xmedia_s32 id = data_info->id;
    xmedia_msensor_data_type data_type = data_info->data_type;
    xmedia_ulong flags;

    osal_spin_lock_irqsave(&g_user_mng[dev].read_lock[id], &flags);
    if (data_info->data[1].num == 0) {
        if (data_info->data[0].num > 0) {
            if (((xmedia_u64)(xmedia_uintptr_t)data_info->data[0].x_phys_addr +
                 data_info->data[0].num * sizeof(xmedia_s32) - g_offset[dev]) >=
                ((xmedia_u64)(xmedia_uintptr_t)y_start_addr(dev, data_type))) {
                x_read_ptr(dev, id, data_type) = x_start_addr(dev, data_type);
                y_read_ptr(dev, id, data_type) = y_start_addr(dev, data_type);
                z_read_ptr(dev, id, data_type) = z_start_addr(dev, data_type);
                temp_read_ptr(dev, id, data_type) = temp_start_addr(dev, data_type);
                pts_read_ptr(dev, id, data_type) = pts_start_addr(dev, data_type);
            } else {
                x_read_ptr(dev, id, data_type) = (xmedia_u8 *)data_info->data[0].x_phys_addr +
                    data_info->data[0].num * sizeof(xmedia_s32) - g_offset[dev];
                y_read_ptr(dev, id, data_type) = (xmedia_u8 *)data_info->data[0].y_phys_addr +
                    data_info->data[0].num * sizeof(xmedia_s32) - g_offset[dev];
                z_read_ptr(dev, id, data_type) = (xmedia_u8 *)data_info->data[0].z_phys_addr +
                    data_info->data[0].num * sizeof(xmedia_s32) - g_offset[dev];
                temp_read_ptr(dev, id, data_type) = (xmedia_u8 *)data_info->data[0].temperature_phys_addr +
                    data_info->data[0].num * sizeof(xmedia_s32) - g_offset[dev];
                pts_read_ptr(dev, id, data_type) = (xmedia_u8 *)data_info->data[0].pts_phys_addr +
                    data_info->data[0].num * sizeof(xmedia_u64) - g_offset[dev];
            }
        }
    } else {
        x_read_ptr(dev, id, data_type) =
            (xmedia_u8 *)data_info->data[1].x_phys_addr + data_info->data[1].num * sizeof(xmedia_s32) - g_offset[dev];
        y_read_ptr(dev, id, data_type) =
            (xmedia_u8 *)data_info->data[1].y_phys_addr + data_info->data[1].num * sizeof(xmedia_s32) - g_offset[dev];
        z_read_ptr(dev, id, data_type) =
            (xmedia_u8 *)data_info->data[1].z_phys_addr + data_info->data[1].num * sizeof(xmedia_s32) - g_offset[dev];
        temp_read_ptr(dev, id, data_type) = (xmedia_u8 *)data_info->data[1].temperature_phys_addr +
            data_info->data[1].num * sizeof(xmedia_s32) - g_offset[dev];
        pts_read_ptr(dev, id, data_type) =
            (xmedia_u8 *)data_info->data[1].pts_phys_addr + data_info->data[1].num * sizeof(xmedia_u64) - g_offset[dev];
    }
    data_info->data[0].num = 0;
    data_info->data[1].num = 0;
    g_already_released[dev][id] = XMEDIA_TRUE;

    osal_spin_unlock_irqrestore(&g_user_mng[dev].read_lock[id], &flags);
    return;
}

xmedia_s32 msensor_buf_release_data(xmedia_s32 dev, xmedia_msensor_data_info *data_info)
{
    xmedia_ulong flags;

    if (data_info == XMEDIA_NULL) {
        MSENSOR_ERR_TRACE("dev %d data info is null\n", dev);
        return XMEDIA_ERRCODE_NULL_PTR;
    }

    osal_spin_lock_irqsave(&g_user_mng[dev].mng_lock, &flags);

    if ((data_info->id < 0) || (data_info->id >= MAX_USER_NUM)) {
        osal_spin_unlock_irqrestore(&g_user_mng[dev].mng_lock, &flags);
        MSENSOR_ERR_TRACE("dev %d *id(%d) is out of range[0,%d].\n", dev, data_info->id, MAX_USER_NUM);
        return XMEDIA_FAILURE;
    }

    if (g_buf_init[dev] == XMEDIA_FALSE) {
        osal_spin_unlock_irqrestore(&g_user_mng[dev].mng_lock, &flags);
        MSENSOR_ERR_TRACE("dev %d when release_data, msensor buf not be inited yet\n", dev);
        return XMEDIA_FAILURE;
    }

    if (g_user_mng[dev].user_ctx[data_info->id] == XMEDIA_NULL) {
        osal_spin_unlock_irqrestore(&g_user_mng[dev].mng_lock, &flags);
        MSENSOR_ERR_TRACE("dev %d msensor buf release: id is null.\n", dev);
        return XMEDIA_FAILURE;
    }

    if (data_info->data_type >= XMEDIA_MSENSOR_DATA_TYPE_MAX) {
        osal_spin_unlock_irqrestore(&g_user_mng[dev].mng_lock, &flags);
        MSENSOR_ERR_TRACE("dev %d data_type:%d is out of range.\n", dev, data_info->data_type);
        return XMEDIA_FAILURE;
    }

    if (g_already_released[dev][data_info->id] == XMEDIA_TRUE) {
        osal_spin_unlock_irqrestore(&g_user_mng[dev].mng_lock, &flags);
        return XMEDIA_SUCCESS;
    }

    /* release data */
    msensor_buf_release(dev, data_info);
    osal_spin_unlock_irqrestore(&g_user_mng[dev].mng_lock, &flags);
    return XMEDIA_SUCCESS;
}

static xmedia_s32 msensor_buf_arrive_back_start_addr(xmedia_s32 dev, const xmedia_msensor_data_info *data_info)
{
    xmedia_s32 length[2] = { 0 }; /* 2 : cycle buf */
    xmedia_s32 buftotal_len;
    xmedia_bool loop_flag = XMEDIA_FALSE;
    xmedia_u32 interval;
    xmedia_u64 *p_pts_read_ptr = XMEDIA_NULL;
    xmedia_msensor_data_type data_type = data_info->data_type;
    xmedia_s32 id = data_info->id;

    buftotal_len = (xmedia_u32 *)y_start_addr(dev, data_type) - (xmedia_u32 *)x_start_addr(dev, data_type);

    p_pts_read_ptr = pts_read_ptr(dev, id, data_type);

    while (1) {
        if ((*p_pts_read_ptr <= data_info->begin_pts) || (p_pts_read_ptr == pts_write_ptr(dev, data_type))) {
            if (p_pts_read_ptr == pts_write_ptr(dev, data_type)) {
                MSENSOR_ERR_TRACE("dev %d arrive to write_addr!\n", dev);
            }
            break;
        }

        if (p_pts_read_ptr == pts_start_addr(dev, data_type)) {
            loop_flag = XMEDIA_TRUE;
            p_pts_read_ptr = (xmedia_u64 *)pts_start_addr(dev, data_type) + buftotal_len - 1;
        } else {
            p_pts_read_ptr--;
        }

        g_forward[dev] = XMEDIA_FALSE;

        /* msensor_debug_trace("#####pts_read_pointer:%p %lld\n", pts_read_ptr, *pts_read_ptr) */
        if (loop_flag == XMEDIA_TRUE) {
            length[1]++;
        } else {
            length[0]++;
        }
    }

    if (length[1] > 0) {
        interval = (xmedia_u64 *)pts_start_addr(dev, data_type) + buftotal_len - p_pts_read_ptr;
        x_read_ptr(dev, id, data_type) = (xmedia_u32 *)y_start_addr(dev, data_type) - interval;
        y_read_ptr(dev, id, data_type) = (xmedia_u32 *)z_start_addr(dev, data_type) - interval;
        z_read_ptr(dev, id, data_type) = (xmedia_u32 *)temp_start_addr(dev, data_type) - interval;
        temp_read_ptr(dev, id, data_type) = (xmedia_u32 *)pts_start_addr(dev, data_type) - interval;
        pts_read_ptr(dev, id, data_type) = p_pts_read_ptr;
    } else {
        interval = (xmedia_u64 *)pts_read_ptr(dev, id, data_type) - p_pts_read_ptr;
        x_read_ptr(dev, id, data_type) = (xmedia_u32 *)x_read_ptr(dev, id, data_type) - interval;
        y_read_ptr(dev, id, data_type) = (xmedia_u32 *)y_read_ptr(dev, id, data_type) - interval;
        z_read_ptr(dev, id, data_type) = (xmedia_u32 *)z_read_ptr(dev, id, data_type) - interval;
        temp_read_ptr(dev, id, data_type) = (xmedia_u32 *)temp_read_ptr(dev, id, data_type) - interval;
        pts_read_ptr(dev, id, data_type) = p_pts_read_ptr;
    }
    return XMEDIA_SUCCESS;
}

static xmedia_void msensor_buf_get_data_assign(xmedia_s32 dev, xmedia_msensor_data_info *data_info)
{
    xmedia_s32 id = data_info->id;
    xmedia_msensor_data_type data_type = data_info->data_type;
    xmedia_u32 interval;
    msensor_mng_proc_info *proc_info = XMEDIA_NULL;
    proc_info = msensor_mng_get_proc_info(dev);

    if (data_info->data[0].num > 0) {
        data_info->data[0].x_phys_addr = (xmedia_void *)((xmedia_u8 *)x_read_ptr(dev, id, data_type) + g_offset[dev]);
        data_info->data[0].y_phys_addr = (xmedia_void *)((xmedia_u8 *)y_read_ptr(dev, id, data_type) + g_offset[dev]);
        data_info->data[0].z_phys_addr = (xmedia_void *)((xmedia_u8 *)z_read_ptr(dev, id, data_type) + g_offset[dev]);
        data_info->data[0].temperature_phys_addr =
            (xmedia_void *)((xmedia_u8 *)temp_read_ptr(dev, id, data_type) + g_offset[dev]);
        data_info->data[0].pts_phys_addr =
            (xmedia_void *)((xmedia_u8 *)pts_read_ptr(dev, id, data_type) + g_offset[dev]);
    }

    if (data_info->data[1].num > 0) {
        data_info->data[1].x_phys_addr = (xmedia_void *)((xmedia_u8 *)x_start_addr(dev, data_type) + g_offset[dev]);
        data_info->data[1].y_phys_addr = (xmedia_void *)((xmedia_u8 *)y_start_addr(dev, data_type) + g_offset[dev]);
        data_info->data[1].z_phys_addr = (xmedia_void *)((xmedia_u8 *)z_start_addr(dev, data_type) + g_offset[dev]);
        data_info->data[1].temperature_phys_addr =
            (xmedia_void *)((xmedia_u8 *)temp_start_addr(dev, data_type) + g_offset[dev]);
        data_info->data[1].pts_phys_addr = (xmedia_void *)((xmedia_u8 *)pts_start_addr(dev, data_type) + g_offset[dev]);
    }

    if ((data_info->data[0].num == 0) && (data_info->data[1].num == 0)) {
        g_already_released[dev][id] = XMEDIA_TRUE;
        proc_info->buf_data_unmatch[data_type]++;
        proc_info->buf_data_unmatch_id[data_type] = id;
        interval = (xmedia_u32)(xmedia_uintptr_t)((xmedia_u8 *)pts_read_ptr(dev, id, data_type) -
                                                  (xmedia_u8 *)pts_start_addr(dev, data_type)) >>
            1;
        x_read_ptr(dev, id, data_type) = (xmedia_u8 *)x_start_addr(dev, data_type) + interval;
        y_read_ptr(dev, id, data_type) = (xmedia_u8 *)y_start_addr(dev, data_type) + interval;
        z_read_ptr(dev, id, data_type) = (xmedia_u8 *)z_start_addr(dev, data_type) + interval;
        temp_read_ptr(dev, id, data_type) = (xmedia_u8 *)temp_start_addr(dev, data_type) + interval;
        pts_read_ptr(dev, id, data_type) = pts_read_ptr(dev, id, data_type);
    } else {
        g_already_released[dev][id] = XMEDIA_FALSE;
    }
    data_info->addr_offset = g_offset[dev];
    return;
}

static xmedia_void msensor_buf_get_data_process(xmedia_s32 dev, xmedia_msensor_data_info *data_info)
{
    xmedia_bool loop_flag = XMEDIA_FALSE;
    xmedia_bool first_found = XMEDIA_FALSE;
    xmedia_u32 interval;
    xmedia_u64 *pts_read_ptr = XMEDIA_NULL;
    xmedia_void *pts_start_ptr = XMEDIA_NULL;
    xmedia_s32 id = data_info->id;
    xmedia_msensor_data_type data_type = data_info->data_type;

    pts_read_ptr = pts_read_ptr(dev, id, data_type);
    pts_start_ptr = pts_start_addr(dev, data_type);

    while (1) {
        if (((pts_read_ptr == pts_write_ptr(dev, data_type)) && (g_forward[dev] == XMEDIA_TRUE)) ||
            (*pts_read_ptr > data_info->end_pts)) {
            break;
        }

        if ((*pts_read_ptr >= data_info->begin_pts) && (*pts_read_ptr <= data_info->end_pts)) {
            if (loop_flag == XMEDIA_TRUE) {
                data_info->data[1].num++;
            } else if (first_found == XMEDIA_FALSE) {
                data_info->data[0].num++;

                interval = (xmedia_u32)(xmedia_uintptr_t)((xmedia_u8 *)pts_read_ptr - (xmedia_u8 *)pts_start_ptr) >> 1;
                x_read_ptr(dev, id, data_type) = (xmedia_u8 *)x_start_addr(dev, data_type) + interval;
                y_read_ptr(dev, id, data_type) = (xmedia_u8 *)y_start_addr(dev, data_type) + interval;
                z_read_ptr(dev, id, data_type) = (xmedia_u8 *)z_start_addr(dev, data_type) + interval;
                temp_read_ptr(dev, id, data_type) = (xmedia_u8 *)temp_start_addr(dev, data_type) + interval;
                pts_read_ptr(dev, id, data_type) = pts_read_ptr;
                first_found = XMEDIA_TRUE;
            } else {
                data_info->data[0].num++;
            }
        }

        pts_read_ptr++;
        g_forward[dev] = XMEDIA_TRUE;

        if (pts_read_ptr >= ((xmedia_u64 *)pts_start_ptr +
                             ((xmedia_u8 *)y_start_addr(dev, data_type) - (xmedia_u8 *)x_start_addr(dev, data_type)) /
                                 sizeof(xmedia_s32))) {
            pts_read_ptr = pts_start_ptr;

            if (first_found == XMEDIA_TRUE) {
                loop_flag = XMEDIA_TRUE;
            }
        }
    }

    return;
}

static xmedia_s32 msensor_buf_data_check(xmedia_s32 dev, const xmedia_msensor_data_info *msensor_data)
{
    if (g_buf_init[dev] == XMEDIA_FALSE) {
        MSENSOR_ERR_TRACE("dev %d get data, but msensor buffer hasn't been initialised.\n", dev);
        return XMEDIA_FAILURE;
    }

    if (msensor_data == XMEDIA_NULL) {
        MSENSOR_ERR_TRACE("dev %d msensor_data(%p) is null!!!\n", dev, msensor_data);
        return XMEDIA_FAILURE;
    }

    if ((msensor_data->id < 0) || (msensor_data->id >= MAX_USER_NUM)) {
        MSENSOR_ERR_TRACE("dev %d id(%d) is out of range[0, %d].\n", dev, msensor_data->id, MAX_USER_NUM);
        return XMEDIA_FAILURE;
    }

    if (msensor_data->data_type >= XMEDIA_MSENSOR_DATA_TYPE_MAX) {
        MSENSOR_ERR_TRACE("dev %d data_type:%d out of range.\n", dev, msensor_data->data_type);
        return XMEDIA_FAILURE;
    }

    /* Check whether user is created. */
    if (g_user_mng[dev].user_ctx[msensor_data->id] == XMEDIA_NULL) {
        MSENSOR_ERR_TRACE("dev %d msensor_buf_read_data: id:%d is null.\n", dev, msensor_data->id);
        return XMEDIA_FAILURE;
    }

    if (g_already_released[dev][msensor_data->id] == XMEDIA_FALSE) {
        MSENSOR_ERR_TRACE("dev %d please release last read\n", dev);
        return XMEDIA_FAILURE;
    }

    if ((msensor_data->end_pts - msensor_data->begin_pts) > 5000000) { /* 5000000us */
        MSENSOR_ERR_TRACE("dev %d end pts[%llu] - begin pts[%llu] = %llu, more than 5000000\n", dev,
                          msensor_data->end_pts, msensor_data->begin_pts,
                          msensor_data->end_pts - msensor_data->begin_pts);
        return XMEDIA_FAILURE;
    }

    if (msensor_data->end_pts <= msensor_data->begin_pts) {
        MSENSOR_ERR_TRACE("dev %d end_pts[%llu] <= begin_pts[%llu]\n", dev, msensor_data->end_pts,
                          msensor_data->begin_pts);
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 msensor_buf_get_data(xmedia_s32 dev, xmedia_msensor_data_info *msensor_data)
{
    xmedia_s32 ret;
    xmedia_ulong flags;
    xmedia_ulong flags_read;
    xmedia_msensor_data_info data_info = { 0 };

    ret = msensor_buf_data_check(dev, msensor_data);
    if (ret != XMEDIA_SUCCESS) {
        return ret;
    }

    /* remain check user id to be done */
    osal_memcpy(&data_info, msensor_data, sizeof(xmedia_msensor_data_info));

    if (msensor_data->data_type == XMEDIA_MSENSOR_DATA_TYPE_GYRO) {
        msensor_mng_write_data_2_buf(dev);
    }

    osal_spin_lock_irqsave(&g_user_mng[dev].mng_lock, &flags);

    ret = msensor_buf_data_check(dev, msensor_data);
    if (ret != XMEDIA_SUCCESS) {
        osal_spin_unlock_irqrestore(&g_user_mng[dev].mng_lock, &flags);
        return ret;
    }

    ret = msensor_buf_arrive_back_start_addr(dev, &data_info);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d data_type:%d has no data.\n", dev, msensor_data->data_type);
        osal_spin_unlock_irqrestore(&g_user_mng[dev].mng_lock, &flags);
        return XMEDIA_FAILURE;
    }

    data_info.data[0].num = 0;
    data_info.data[1].num = 0;

    osal_spin_lock_irqsave(&g_user_mng[dev].read_lock[msensor_data->id], &flags_read);
    msensor_buf_get_data_process(dev, &data_info);
    msensor_buf_get_data_assign(dev, &data_info);
    osal_spin_unlock_irqrestore(&g_user_mng[dev].read_lock[msensor_data->id], &flags_read);

    osal_memcpy(msensor_data, &data_info, sizeof(xmedia_msensor_data_info));

    osal_spin_unlock_irqrestore(&g_user_mng[dev].mng_lock, &flags);

    ret = msensor_buf_release_data(dev, &data_info);

    return ret;
}

static xmedia_s32 msensor_buf_sync_init(xmedia_s32 dev)
{
    xmedia_s32 ret;
    xmedia_s32 i;

    for (i = 0; i < MAX_USER_NUM; i++) {
        ret = osal_spin_lock_init(&g_user_mng[dev].read_lock[i]);
        if (ret != XMEDIA_SUCCESS) {
            MSENSOR_ERR_TRACE("dev %d spin_lock_init failed!!!!\n", dev);
            return XMEDIA_FAILURE;
        }
    }
    return XMEDIA_SUCCESS;
}

static xmedia_void msensor_buf_sync_deinit(xmedia_s32 dev)
{
    xmedia_s32 i;

    for (i = 0; i < MAX_USER_NUM; i++) {
        osal_spin_lock_destroy(&g_user_mng[dev].read_lock[i]);
    }
    return;
}

xmedia_s32 msensor_buf_lock_init(xmedia_void)
{
    xmedia_s32 ret;
    xmedia_s32 dev;

    for (dev = 0; dev < XMEDIA_MSENSOR_MAX_DEV_NUM; dev++) {
        (xmedia_void) osal_memset(&g_user_mng[dev], 0, sizeof(msensor_buf_user_mng));

        ret = osal_spin_lock_init(&g_user_mng[dev].mng_lock);
        if (ret != XMEDIA_SUCCESS) {
            MSENSOR_ERR_TRACE("spin_lock_init failed!!!!\n");
            return XMEDIA_FAILURE;
        }

        ret = msensor_buf_sync_init(dev);
        if (ret != XMEDIA_SUCCESS) {
            MSENSOR_ERR_TRACE("msensor buf sync init failed!!!!\n");
            return XMEDIA_FAILURE;
        }

        osal_mutex_init(&g_user_mng[dev].mng_mutex);
    }

    return XMEDIA_SUCCESS;
}

xmedia_void msensor_buf_lock_exit(xmedia_void)
{
    xmedia_s32 dev;

    for (dev = 0; dev < XMEDIA_MSENSOR_MAX_DEV_NUM; dev++) {
        osal_mutex_destroy(&g_user_mng[dev].mng_mutex);

        msensor_buf_sync_deinit(dev);

        osal_spin_lock_destroy(&g_user_mng[dev].mng_lock);
    }

    return;
}

