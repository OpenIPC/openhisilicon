#include <linux/module.h>
#include <linux/kernel.h>
//#include <asm/div64.h>
#include <linux/random.h>
#include <linux/version.h>
#include "hi_osal.h"

unsigned int osal_do_div(unsigned int n, unsigned int base){
	unsigned long long temp;
	temp = 0xffffffff & (unsigned long long)n;
	do_div(temp , base);
	return (unsigned int)temp;

}

unsigned long long osal_do_div64(unsigned long long n, unsigned int base){
	do_div(n , base);
	return n;
}

unsigned long long osal_do_div_rem64(unsigned long long n, unsigned int base){
	return do_div(n , base);
}

long long osal_do_div64_s64(long long dividend, long long divisor){
	return div64_s64(dividend , divisor);
}

unsigned long long osal_do_div64_u64(unsigned long long dividend, unsigned long long divisor){
	return div64_u64(dividend , divisor);
}


EXPORT_SYMBOL(osal_do_div);

EXPORT_SYMBOL(osal_do_div64);

EXPORT_SYMBOL(osal_do_div_rem64);

EXPORT_SYMBOL(osal_do_div64_s64);

EXPORT_SYMBOL(osal_do_div64_u64);


