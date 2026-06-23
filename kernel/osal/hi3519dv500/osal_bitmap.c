/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/bitmap.h>
#include <linux/bitops.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include "ot_osal.h"
#include "securec.h"
#include "osal_inner.h"

void osal_bitmap_set_bit(int nr, unsigned long *addr)
{
    if (addr == NULL) {
        osal_log("parameter invalid!\n");
        osal_dump_stack();
        return;
    }
    set_bit(nr, addr);
}
EXPORT_SYMBOL(osal_bitmap_set_bit);

void osal_bitmap_clear_bit(int nr, unsigned long *addr)
{
    if (addr == NULL) {
        osal_log("parameter invalid!\n");
        osal_dump_stack();
        return;
    }
    clear_bit(nr, addr);
}
EXPORT_SYMBOL(osal_bitmap_clear_bit);

void osal_bitmap_change_bit(int nr, unsigned long *addr)
{
    if (addr == NULL) {
        osal_log("parameter invalid!\n");
        osal_dump_stack();
        return;
    }
    change_bit(nr, addr);
}
EXPORT_SYMBOL(osal_bitmap_change_bit);

int  osal_bitmap_test_bit(int nr, unsigned long *addr)
{
    if (addr == NULL) {
        osal_log("parameter invalid!\n");
        osal_dump_stack();
        return OSAL_FAILURE;
    }
    return test_bit(nr, addr);
}
EXPORT_SYMBOL(osal_bitmap_test_bit);

int  osal_bitmap_test_and_set_bit(int nr, unsigned long *addr)
{
    if (addr == NULL) {
        osal_log("parameter invalid!\n");
        osal_dump_stack();
        return OSAL_FAILURE;
    }
    return test_and_set_bit(nr, addr);
}
EXPORT_SYMBOL(osal_bitmap_test_and_set_bit);

int  osal_bitmap_test_and_clear_bit(int nr, unsigned long *addr)
{
    if (addr == NULL) {
        osal_log("parameter invalid!\n");
        osal_dump_stack();
        return OSAL_FAILURE;
    }
    return test_and_clear_bit(nr, addr);
}
EXPORT_SYMBOL(osal_bitmap_test_and_clear_bit);

int  osal_bitmap_test_and_change_bit(int nr, unsigned long *addr)
{
    if (addr == NULL) {
        osal_log("parameter invalid!\n");
        osal_dump_stack();
        return OSAL_FAILURE;
    }
    return test_and_change_bit(nr, addr);
}
EXPORT_SYMBOL(osal_bitmap_test_and_change_bit);

int osal_bitmap_find_first_zero_bit(const unsigned long *name, unsigned size)
{
    if (name == NULL) {
        osal_log("kmalloc error!\n");
        osal_dump_stack();
        return OSAL_FAILURE;
    }
    return find_first_zero_bit(name, size);
}
EXPORT_SYMBOL(osal_bitmap_find_first_zero_bit);

int osal_bitmap_find_first_bit(const unsigned long *name, unsigned size)
{
    if (name == NULL) {
        osal_log("kmalloc error!\n");
        osal_dump_stack();
        return OSAL_FAILURE;
    }
    return find_first_bit(name, size);
}
EXPORT_SYMBOL(osal_bitmap_find_first_bit);


int osal_bitmap_find_next_zero_bit(const unsigned long *name, int size, int offset)
{
    if (name == NULL) {
        osal_log("kmalloc error!\n");
        osal_dump_stack();
        return OSAL_FAILURE;
    }
    return find_next_zero_bit(name, size, offset);
}
EXPORT_SYMBOL(osal_bitmap_find_next_zero_bit);

int osal_bitmap_find_next_bit(const unsigned long *name, unsigned size, int offset)
{
    if (name == NULL) {
        osal_log("kmalloc error!\n");
        osal_dump_stack();
        return OSAL_FAILURE;
    }
    return find_next_bit(name, size, offset);
}
EXPORT_SYMBOL(osal_bitmap_find_next_bit);
