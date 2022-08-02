/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <linux/module.h>
#include <linux/kernel.h>
// #include <asm/div64.h>
#include <linux/random.h>
#include <linux/version.h>
#include "osal.h"

/* the result of u64/u32. */
unsigned long long osal_div_u64(unsigned long long dividend, unsigned int divisor)
{
    return div_u64(dividend, divisor);
}
EXPORT_SYMBOL(osal_div_u64);

/* the result of s64/s32. */
long long osal_div_s64(long long dividend, int divisor)
{
    return div_s64(dividend, divisor);
}
EXPORT_SYMBOL(osal_div_s64);

/* the result of u64/u64. */
unsigned long long osal_div64_u64(unsigned long long dividend, unsigned long long divisor)
{
    return div64_u64(dividend, divisor);
}
EXPORT_SYMBOL(osal_div64_u64);

/* the result of s64/s64. */
long long osal_div64_s64(long long dividend, long long divisor)
{
    return div64_s64(dividend, divisor);
}
EXPORT_SYMBOL(osal_div64_s64);

/* the remainder of u64/u32. */
unsigned long long osal_div_u64_rem(unsigned long long dividend, unsigned int divisor)
{
    unsigned int remainder;

    div_u64_rem(dividend, divisor, &remainder);

    return remainder;
}
EXPORT_SYMBOL(osal_div_u64_rem);

/* the remainder of s64/s32. */
long long osal_div_s64_rem(long long dividend, int divisor)
{
    int remainder;

    div_s64_rem(dividend, divisor, &remainder);

    return remainder;
}
EXPORT_SYMBOL(osal_div_s64_rem);

/* the remainder of u64/u64. */
unsigned long long osal_div64_u64_rem(unsigned long long dividend, unsigned long long divisor)
{
    unsigned long long remainder;

    div64_u64_rem(dividend, divisor, &remainder);

    return remainder;
}
EXPORT_SYMBOL(osal_div64_u64_rem);

unsigned int osal_random(void)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0)
    return random32();
#else
    return get_random_int();
#endif
}
EXPORT_SYMBOL(osal_random);
