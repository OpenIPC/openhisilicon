/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include "osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/spinlock.h>
#include <linux/slab.h>

int osal_spin_lock_init(osal_spinlock_t *lock)
{
	spinlock_t *p = NULL;
	if (lock == NULL) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	p = (spinlock_t *)kmalloc(sizeof(spinlock_t), GFP_KERNEL);
	if (p == NULL) {
		osal_printk("%s - kmalloc error!\n", __FUNCTION__);
		return -1;
	}
	spin_lock_init(p);
	lock->lock = p;
	return 0;
}
EXPORT_SYMBOL(osal_spin_lock_init);
void osal_spin_lock(osal_spinlock_t *lock)
{
	spinlock_t *p = NULL;

	p = (spinlock_t *)(lock->lock);
	spin_lock(p);
}
EXPORT_SYMBOL(osal_spin_lock);
int osal_spin_trylock(osal_spinlock_t *lock)
{
	spinlock_t *p = NULL;
	if (lock == NULL) {
		osal_printk("%s - parameter invalid!\n", __FUNCTION__);
		return -1;
	}
	p = (spinlock_t *)(lock->lock);
	return spin_trylock(p);
}
EXPORT_SYMBOL(osal_spin_trylock);
void osal_spin_unlock(osal_spinlock_t *lock)
{
	spinlock_t *p = NULL;

	p = (spinlock_t *)(lock->lock);
	spin_unlock(p);
}
EXPORT_SYMBOL(osal_spin_unlock);
void osal_spin_lock_irqsave(osal_spinlock_t *lock, unsigned long *flags)
{
	spinlock_t *p = NULL;
	unsigned long f;

	p = (spinlock_t *)(lock->lock);
	spin_lock_irqsave(p, f);
	*flags = f;
}
EXPORT_SYMBOL(osal_spin_lock_irqsave);
void osal_spin_unlock_irqrestore(osal_spinlock_t *lock, unsigned long *flags)
{
	spinlock_t *p = NULL;
	unsigned long f;

	p = (spinlock_t *)(lock->lock);
	f = *flags;
	spin_unlock_irqrestore(p, f);
}
EXPORT_SYMBOL(osal_spin_unlock_irqrestore);
void osal_spin_lock_destory(osal_spinlock_t *lock)
{
	spinlock_t *p = NULL;
	p = (spinlock_t *)(lock->lock);
	kfree(p);
	lock->lock = NULL;
}
EXPORT_SYMBOL(osal_spin_lock_destory);
