/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/slab.h>
#include <linux/rwlock.h>
#include "ot_osal.h"

int osal_rwlock_init(osal_rwlock *rw)
{
    if (rw == NULL || rw->rwlock != NULL) {
        return OSAL_FAILURE;
    }
    rw->rwlock = kmalloc(sizeof(rwlock_t), GFP_KERNEL);
    if (rw->rwlock == NULL) {
        return OSAL_FAILURE;
    }
    rwlock_init(((rwlock_t *)(rw->rwlock)));
    return OSAL_SUCCESS;
}
EXPORT_SYMBOL(osal_rwlock_init);

void osal_rwlock_read_lock(osal_rwlock *rw)
{
    if (rw == NULL || rw->rwlock == NULL) {
        return;
    }
    read_lock(rw->rwlock);
}
EXPORT_SYMBOL(osal_rwlock_read_lock);

void osal_rwlock_read_unlock(osal_rwlock *rw)
{
    if (rw == NULL || rw->rwlock == NULL) {
        return;
    }
    read_unlock(rw->rwlock);
}
EXPORT_SYMBOL(osal_rwlock_read_unlock);

void osal_rwlock_write_lock(osal_rwlock *rw)
{
    if (rw == NULL || rw->rwlock == NULL) {
        return;
    }
    write_lock(rw->rwlock);
}
EXPORT_SYMBOL(osal_rwlock_write_lock);

void osal_rwlock_write_unlock(osal_rwlock *rw)
{
    if (rw == NULL || rw->rwlock == NULL) {
        return;
    }
    write_unlock(rw->rwlock);
}
EXPORT_SYMBOL(osal_rwlock_write_unlock);

void osal_rwlock_destory(osal_rwlock *rw)
{
    if (rw == NULL || rw->rwlock == NULL) {
        return;
    }
    kfree(rw->rwlock);
    rw->rwlock = NULL;
    return;
}
EXPORT_SYMBOL(osal_rwlock_destory);
