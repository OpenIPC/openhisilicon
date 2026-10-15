/*
 * SAR ADC for the hi3516cv100 family (hi3516cv100, hi3518cv100, hi3518ev100).
 *
 * The block is a 10-bit, two-channel converter at 0x200B0000, clocked and
 * reset from PERI_CRG32 (Hi3518 Data Sheet, Issue 01, 13.8 and 3.x). Boards
 * hang a photoresistor divider on channel 0 for day/night; HiSilicon's V1 SDK
 * ships no driver for it, so the camera firmware that used it carried its own.
 *
 * Each channel is a misc device, /dev/ADC_CH0 and /dev/ADC_CH1 -- the names
 * that vendor firmware used. A read() of at least four bytes runs one
 * conversion and returns the result as an int, 0..1023.
 *
 * The conversion-done interrupt (19) is shared with the IR receiver, so it
 * stays masked and completion is polled from the raw status: a conversion
 * takes a few microseconds at the 4 MHz working clock.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/uaccess.h>

#define SAR_ADC_BASE 0x200B0000
#define SAR_ADC_SIZE 0x20
#define PERI_CRG32 0x20030080

#define ADC_STATUS 0x00
#define ADC_CTRL 0x04
#define ADC_POWERDOWN 0x08
#define ADC_INT_MASK 0x10
#define ADC_INT_CLR 0x14
#define ADC_INT_RAW 0x18
#define ADC_RESULT 0x1C

#define ADC_CTRL_START (1 << 0)
#define ADC_CTRL_CH(ch) ((ch) << 16)
#define ADC_RESULT_MASK 0x3FF

#define CRG_SAR_ADC_SRST (1 << 0)
#define CRG_SAR_ADC_CKEN (1 << 1)

/* Generous: a conversion at a 200 kHz sample clock is 5 us. */
#define ADC_TIMEOUT_US 1000

static void __iomem *adc_base;
static void __iomem *crg_base;
static DEFINE_MUTEX(adc_lock);

static int adc_convert(unsigned int ch, int *value)
{
	unsigned int waited;

	writel(1, adc_base + ADC_INT_CLR);
	writel(0, adc_base + ADC_STATUS);
	writel(ADC_CTRL_CH(ch) | ADC_CTRL_START, adc_base + ADC_CTRL);

	for (waited = 0; !(readl(adc_base + ADC_INT_RAW) & 1); waited++) {
		if (waited >= ADC_TIMEOUT_US)
			return -ETIMEDOUT;
		udelay(1);
	}

	*value = readl(adc_base + ADC_RESULT) & ADC_RESULT_MASK;
	writel(1, adc_base + ADC_INT_CLR);
	writel(0, adc_base + ADC_STATUS);
	return 0;
}

static struct miscdevice adc_dev[2];

static int adc_open(struct inode *inode, struct file *file)
{
	unsigned int ch;

	for (ch = 0; ch < ARRAY_SIZE(adc_dev); ch++) {
		if (adc_dev[ch].minor == iminor(inode)) {
			file->private_data = &adc_dev[ch];
			return 0;
		}
	}
	return -ENODEV;
}

static ssize_t adc_read(struct file *file, char __user *buf, size_t len,
			loff_t *ppos)
{
	unsigned int ch = (struct miscdevice *)file->private_data - adc_dev;
	int value, ret;

	if (len < sizeof(value))
		return -EINVAL;

	mutex_lock(&adc_lock);
	ret = adc_convert(ch, &value);
	mutex_unlock(&adc_lock);
	if (ret)
		return ret;

	if (copy_to_user(buf, &value, sizeof(value)))
		return -EFAULT;
	return sizeof(value);
}

static const struct file_operations adc_fops = {
	.owner = THIS_MODULE,
	.open = adc_open,
	.read = adc_read,
};

static struct miscdevice adc_dev[2] = {
	{ .minor = MISC_DYNAMIC_MINOR, .name = "ADC_CH0", .fops = &adc_fops },
	{ .minor = MISC_DYNAMIC_MINOR, .name = "ADC_CH1", .fops = &adc_fops },
};

static void adc_power(bool on)
{
	if (on) {
		writel(CRG_SAR_ADC_SRST | CRG_SAR_ADC_CKEN, crg_base);
		udelay(10);
		writel(CRG_SAR_ADC_CKEN, crg_base);
		udelay(10);
		writel(0, adc_base + ADC_POWERDOWN);
		writel(1, adc_base + ADC_INT_MASK);
	} else {
		writel(1, adc_base + ADC_POWERDOWN);
		writel(CRG_SAR_ADC_SRST, crg_base);
	}
}

static int __init adc_init(void)
{
	int ret, discard;

	adc_base = ioremap(SAR_ADC_BASE, SAR_ADC_SIZE);
	crg_base = ioremap(PERI_CRG32, 4);
	if (!adc_base || !crg_base) {
		ret = -ENOMEM;
		goto unmap;
	}

	adc_power(true);
	/* The first conversion out of reset reads 0 whatever is on the pin
	 * (measured on a hi3516cv100); spend it here rather than hand it to the
	 * first reader as a reading of total darkness. It is also the proof the
	 * block converts at all: a module that cannot convert does not load,
	 * rather than leaving nodes whose every read times out. */
	ret = adc_convert(0, &discard);
	if (ret) {
		pr_err("adc: the SAR ADC did not complete a conversion\n");
		goto off;
	}

	ret = misc_register(&adc_dev[0]);
	if (ret)
		goto off;
	ret = misc_register(&adc_dev[1]);
	if (ret)
		goto dereg;
	return 0;

dereg:
	misc_deregister(&adc_dev[0]);
off:
	adc_power(false);
unmap:
	if (crg_base)
		iounmap(crg_base);
	if (adc_base)
		iounmap(adc_base);
	return ret;
}

static void __exit adc_exit(void)
{
	misc_deregister(&adc_dev[1]);
	misc_deregister(&adc_dev[0]);
	adc_power(false);
	iounmap(crg_base);
	iounmap(adc_base);
}

module_init(adc_init);
module_exit(adc_exit);

MODULE_DESCRIPTION("hi3516cv100 SAR ADC");
MODULE_LICENSE("GPL");
