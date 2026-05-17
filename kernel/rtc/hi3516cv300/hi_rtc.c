/* hi_rtc.c
 *
 * Copyright (c) 2012 Hisilicon Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;
 *
 * History:
 *      2012.05.15 create this file <pengkang@huawei.com>
 *      2012.11.20 add temp select method  <sunny.liucan@huawei.com>
 */

#include <linux/module.h>
#include "hi_osal.h"
#include "hi_rtc.h"

#define RTC_NAME	"hi_rtc"
//#define HIDEBUG

#ifdef HIDEBUG
#define HI_MSG(x...) \
do { \
	osal_printk("%s->%d: ", __FUNCTION__, __LINE__); \
	osal_printk(x); \
	osal_printk("\n"); \
} while (0)
#else
#define HI_MSG(args...) do { } while (0)
#endif

#define HI_ERR(x...) \
do { \
	osal_printk("%s->%d: ", __FUNCTION__, __LINE__); \
	osal_printk(x); \
	osal_printk("\n"); \
} while (0)

#ifndef NULL
#define NULL  ((void *)0)
#endif

#define OSDRV_MODULE_VERSION_STRING "HISI_RTC @Hi3516CV100"

volatile void *pRtcRegBase = NULL;

/* RTC Control over SPI */
#define RTC_SPI_BASE_ADDR	0x12090000
#define SPI_CLK_DIV			((unsigned long)pRtcRegBase + 0x000)
#define SPI_RW				((unsigned long)pRtcRegBase + 0x004)

/* Define the union SPI_RW */
typedef union
{
    struct
    {
        unsigned int spi_wdata		: 8; /* [7:0] */
        unsigned int spi_rdata		: 8; /* [15:8] */
        unsigned int spi_addr		: 7; /* [22:16] */
        unsigned int spi_rw		    : 1; /* [23] */
        unsigned int spi_start		: 1; /* [24] */
        unsigned int reserved		: 6; /* [30:25] */
        unsigned int spi_busy		: 1; /* [31] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} U_SPI_RW;

#define SPI_WRITE		(0)
#define SPI_READ		(1)

#define RTC_IRQ			(2)
unsigned int rtc_irq = 0;

/* RTC REG */
#define RTC_10MS_COUN	0x00
#define RTC_S_COUNT  	0x01
#define RTC_M_COUNT  	0x02
#define RTC_H_COUNT  	0x03
#define RTC_D_COUNT_L	0x04
#define RTC_D_COUNT_H	0x05

#define RTC_MR_10MS		0x06
#define RTC_MR_S		0x07
#define RTC_MR_M		0x08
#define RTC_MR_H		0x09
#define RTC_MR_D_L		0x0A
#define RTC_MR_D_H		0x0B

#define RTC_LR_10MS		0x0C
#define RTC_LR_S		0x0D
#define RTC_LR_M		0x0E
#define RTC_LR_H		0x0F
#define RTC_LR_D_L		0x10
#define RTC_LR_D_H		0x11

#define RTC_LORD		0x12

#define RTC_IMSC		0x13
#define RTC_INT_CLR		0x14
#define RTC_INT     	0x15
#define RTC_INT_RAW		0x16

#define RTC_CLK			0x17
#define RTC_POR_N		0x18
#define RTC_SAR_CTRL	0x1A

#define RTC_FREQ_H		0x51
#define RTC_FREQ_L		0x52

#define FREQ_H_DEFAULT  0x8
#define FREQ_L_DEFAULT  0x1B

#define RETRY_CNT 500

#define FREQ_MAX_VAL	3277000
#define FREQ_MIN_VAL	3276000

#define REG_INDEX_MAX_VAL	7
unsigned char reg_val[] = {0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A};


static osal_spinlock_t rtc_spin_lock;
static osal_mutex_t hirtc_lock;

static int alm_itrp;

static int rtc_spi_write(char reg, unsigned char val)
{
    U_SPI_RW w_data, r_data;
	int cnt = RETRY_CNT;

    HI_MSG("WRITE, reg 0x%02x, val 0x%02x", reg, val);

    r_data.u32 = 0;
    w_data.u32 = 0;

    w_data.bits.spi_wdata = val;
    w_data.bits.spi_addr = reg;
    w_data.bits.spi_rw = SPI_WRITE;
    w_data.bits.spi_start = 0x1;

    HI_MSG("val 0x%x\n", w_data.u32);


    osal_writel(w_data.u32, SPI_RW);

    do
    {
        r_data.u32 = osal_readl(SPI_RW);
        HI_MSG("read 0x%x", r_data.u32);
        HI_MSG("test busy %d", r_data.bits.spi_busy);
    }
    while (r_data.bits.spi_busy && --cnt);

	if (r_data.bits.spi_busy)
    {
        HI_MSG("IO err.\n");
		return -1;
    }

    return 0;
}


static int spi_rtc_write(char reg, unsigned char val)
{
	int ret;
    osal_mutex_lock(&hirtc_lock);
    ret = rtc_spi_write(reg, val);
    osal_mutex_unlock(&hirtc_lock);

    return ret;
}

static int rtc_spi_read(char reg, unsigned char* val)
{
    U_SPI_RW w_data, r_data;
	int cnt = RETRY_CNT;

    HI_MSG("READ, reg 0x%02x", reg);
    r_data.u32 = 0;
    w_data.u32 = 0;
    w_data.bits.spi_addr = reg;
    w_data.bits.spi_rw = SPI_READ;
    w_data.bits.spi_start = 0x1;

    HI_MSG("val 0x%x\n", w_data.u32);

    osal_writel(w_data.u32, SPI_RW);

    do
    {
        r_data.u32 = osal_readl(SPI_RW);
        HI_MSG("read 0x%x\n", r_data.u32);
        HI_MSG("test busy %d\n", r_data.bits.spi_busy);
    }
    while (r_data.bits.spi_busy && (--cnt));

	if (r_data.bits.spi_busy)
    {
        HI_MSG("IO err.\n");
		return -1;
    }

    *val = r_data.bits.spi_rdata;

    return 0;
}



static int spi_rtc_read(char reg, unsigned char* val)
{
	int ret;
    osal_mutex_lock(&hirtc_lock);
    ret = rtc_spi_read(reg, val);
    osal_mutex_unlock(&hirtc_lock);

    return ret;
}

static int rtc_reset_first(void)
{
	spi_rtc_write(RTC_POR_N, 0);
	spi_rtc_write(RTC_CLK, 0x01);
	return 0;
}

/*
 * converse the data type from year.mouth.data.hour.minite.second to second
 * define 2000.1.1.0.0.0 as jumping-off point
 */
static int rtcdate2second(rtc_time_t compositetime,
		unsigned long *ptimeOfsecond)
{
	struct osal_rtc_time tmp;

    if (compositetime.weekday > 6)
    { return -1; }

    tmp.tm_year = compositetime.year - 1900;
    tmp.tm_mon = compositetime.month - 1;
    tmp.tm_mday = compositetime.date;
    tmp.tm_wday = compositetime.weekday;
    tmp.tm_hour = compositetime.hour;
    tmp.tm_min = compositetime.minute;
    tmp.tm_sec = compositetime.second;
    tmp.tm_isdst = 0;
    tmp.tm_yday = 0;

    if (osal_rtc_valid_tm(&tmp))
    { return -1; }

    osal_rtc_tm_to_time(&tmp, ptimeOfsecond);
    return 0;
}

/*
 * converse the data type from second to year.mouth.data.hour.minite.second
 * define 2000.1.1.0.0.0 as jumping-off point
 */
static int rtcSecond2Date(rtc_time_t *compositetime,
	unsigned long timeOfsecond)
{
	struct osal_rtc_time tmp ;

	osal_rtc_time_to_tm(timeOfsecond, &tmp);

	compositetime->year    = (unsigned int)tmp.tm_year + 1900;
	compositetime->month   = (unsigned int)tmp.tm_mon + 1;
	compositetime->date    = (unsigned int)tmp.tm_mday;
	compositetime->hour    = (unsigned int)tmp.tm_hour;
	compositetime->minute  = (unsigned int)tmp.tm_min;
	compositetime->second  = (unsigned int)tmp.tm_sec;
	compositetime->weekday = (unsigned int)tmp.tm_wday;

    HI_MSG("=================RTC read time\n");
    HI_MSG("\tyear %d", compositetime->year);
    HI_MSG("\tmonth %d", compositetime->month);
    HI_MSG("\tdate %d", compositetime->date);
    HI_MSG("\thour %d", compositetime->hour);
    HI_MSG("\tminute %d", compositetime->minute);
    HI_MSG("\tsecond %d", compositetime->second);
    HI_MSG("\tweekday %d", compositetime->weekday);
    HI_MSG("===============================\n");
    return 0;
}

static int hirtc_get_alarm(rtc_time_t *compositetime)
{
	unsigned char dayl, dayh;
	unsigned char second, minute, hour;
	unsigned long seconds = 0;
	unsigned int day;
	int ret = 0;

	ret |= spi_rtc_read(RTC_MR_S, &second);
	ret |= spi_rtc_read(RTC_MR_M, &minute);
	ret |= spi_rtc_read(RTC_MR_H, &hour);
	ret |= spi_rtc_read(RTC_MR_D_L, &dayl);
	ret |= spi_rtc_read(RTC_MR_D_H, &dayh);
	if (ret)
		return ret;

	day = (unsigned int)(dayl | (dayh << 8));
	HI_MSG("day %d\n", day);
	seconds = second + minute*60 + hour*60*60 + day*24*60*60;

	rtcSecond2Date(compositetime, seconds);
	return 0;
}

static int hirtc_set_alarm(rtc_time_t compositetime)
{
	unsigned int days;
	unsigned long seconds = 0;
	int ret = 0;

	HI_MSG("RTC read time");
	HI_MSG("\tyear %d", compositetime.year);
	HI_MSG("\tmonth %d", compositetime.month);
	HI_MSG("\tdate %d", compositetime.date);
	HI_MSG("\thour %d", compositetime.hour);
	HI_MSG("\tminute %d", compositetime.minute);
	HI_MSG("\tsecond %d", compositetime.second);
	HI_MSG("\tweekday %d", compositetime.weekday);

	rtcdate2second(compositetime, &seconds);
	days = seconds/(60*60*24);

	ret |= spi_rtc_write(RTC_MR_10MS, 0);
	ret |= spi_rtc_write(RTC_MR_S, compositetime.second);
	ret |= spi_rtc_write(RTC_MR_M, compositetime.minute);
	ret |= spi_rtc_write(RTC_MR_H, compositetime.hour);
	ret |= spi_rtc_write(RTC_MR_D_L, (days & 0xFF));
	ret |= spi_rtc_write(RTC_MR_D_H, (days >> 8));
	return ret;
}

static int hirtc_get_time(rtc_time_t *compositetime)
{
	unsigned char dayl, dayh;
	unsigned char second, minute, hour;
	unsigned long seconds = 0;
	unsigned int day;

    unsigned char raw_value;
	int ret;
	int cnt = RETRY_CNT;

	ret = spi_rtc_read(RTC_INT_RAW, &raw_value);
	if (ret)
		return ret;

	if (raw_value & 0x2)
		osal_printk("low voltage detected, date/time is not reliable.\n");

    ret = spi_rtc_read(RTC_LORD, &raw_value);
	if (ret)
		return ret;

    if (raw_value & 0x4)
    {
        ret |= spi_rtc_write(RTC_LORD, (~(1 << 2)) & raw_value);
    }

    ret |= spi_rtc_read(RTC_LORD, &raw_value);
    ret |= spi_rtc_write(RTC_LORD, (1 << 1) | raw_value); //lock the time

    do
    {
        ret |= spi_rtc_read(RTC_LORD, &raw_value);
		if (ret)
			return -1;
		osal_msleep(1);  // must delay 1 ms
    }
    while ((raw_value & 0x2) && (--cnt));

	if (raw_value & 0x2)
	{
        HI_ERR("check spi state error!\n");
	}

    ret |= spi_rtc_read(RTC_S_COUNT, &second);
    ret |= spi_rtc_read(RTC_M_COUNT, &minute);
    ret |= spi_rtc_read(RTC_H_COUNT, &hour);
    ret |= spi_rtc_read(RTC_D_COUNT_L, &dayl);
    ret |= spi_rtc_read(RTC_D_COUNT_H, &dayh);
	if (ret)
		return -1;

    day = (dayl | (dayh << 8));

	HI_MSG("day %d\n", day);

	seconds = second + minute*60 + hour*60*60 + day*24*60*60;

	rtcSecond2Date(compositetime, seconds);
	return 0;
}

static int hirtc_set_time(rtc_time_t compositetime)
{
    int ret = 0;
    unsigned char raw_value = 0;
    unsigned int days;
    unsigned long seconds = 0;
    unsigned int cnt = RETRY_CNT;
    unsigned int tmp_ret;

	HI_MSG("RTC read time");
	HI_MSG("\tyear %d", compositetime.year);
	HI_MSG("\tmonth %d", compositetime.month);
	HI_MSG("\tdate %d", compositetime.date);
	HI_MSG("\thour %d", compositetime.hour);
	HI_MSG("\tminute %d", compositetime.minute);
	HI_MSG("\tsecond %d", compositetime.second);
	HI_MSG("\tweekday %d", compositetime.weekday);

	ret = rtcdate2second(compositetime, &seconds);
	if (ret)
	{
        return -1;
	}
	days = seconds/(60*60*24);

	HI_MSG("day %d\n", days);

    ret |= spi_rtc_write(RTC_LR_10MS, 0);
    ret |= spi_rtc_write(RTC_LR_S, compositetime.second);
    ret |= spi_rtc_write(RTC_LR_M, compositetime.minute);
    ret |= spi_rtc_write(RTC_LR_H, compositetime.hour);
    ret |= spi_rtc_write(RTC_LR_D_L, (days & 0xFF));
    ret |= spi_rtc_write(RTC_LR_D_H, (days >> 8));
    tmp_ret = ret;
    ret |= spi_rtc_write(RTC_LORD, (tmp_ret | 0x1));
	if (ret)
	{
        HI_MSG("IO err.\n");
		return -1;
	}

    /* wait rtc load flag */
    do
    {
        ret = spi_rtc_read(RTC_LORD, &raw_value);
		if (ret)
		{
       	 	HI_MSG("IO err.ret :%d\n",ret);
			return -1;
		}
        osal_msleep(1);
        cnt++;
    }
    while ((raw_value & 0x1) && (--cnt));

    if (raw_value & 0x1)
    {
        HI_ERR("check spi state error!\n");
        return -1;
    }

	HI_MSG("set time ok!\n");
	return 0;
}

static long hi_rtc_ioctl (unsigned int cmd, unsigned long arg, void *private_data)
{
	int ret = 0;

    rtc_time_t time;
    reg_data_t reg_info;
    unsigned char temp_reg;

    switch (cmd)
    {
            /* Alarm interrupt on/off */
        case HI_RTC_AIE_ON:
		{
            HI_MSG("HI_RTC_AIE_ON");
            ret |= spi_rtc_read(RTC_IMSC, &temp_reg);
            ret |= spi_rtc_write(RTC_IMSC, temp_reg | 0x1);
			if (ret)
                return ret;
            break;
        }
        case HI_RTC_AIE_OFF:
		{
            HI_MSG("HI_RTC_AIE_OFF");
            ret |= spi_rtc_read(RTC_IMSC, &temp_reg);
            ret |= spi_rtc_write(RTC_IMSC, temp_reg & ~0x2);
			if (ret)
				return ret;
            break;
        }
        case HI_RTC_SET_FREQ:
        {
            unsigned char freq_l, freq_h;
            unsigned int freq;
            rtc_freq_t value;

            osal_memcpy(&value, (struct rtc_freq_t*) arg, sizeof(value));

            freq = value.freq_l;

            /* freq = 32700 + (freq /3052)*100 */

            if (freq > FREQ_MAX_VAL || freq < FREQ_MIN_VAL)
            {
                HI_MSG("[error]Invalid argument\n");
                return -1;
            }

            freq = (freq - 3270000) * 3052 / 10000;

            freq_l = (freq & 0xff);
            freq_h = ((freq >> 8) & 0xf);

            ret |= spi_rtc_write(RTC_FREQ_H, freq_h);
            ret |=spi_rtc_write(RTC_FREQ_L, freq_l);
			if (ret)
				return ret;

            break;
        }
        case HI_RTC_GET_FREQ:
        {
            unsigned char freq_l, freq_h;
            unsigned int freq;
            rtc_freq_t value;

            ret |= spi_rtc_read(RTC_FREQ_H, &freq_h);
            ret |= spi_rtc_read(RTC_FREQ_L, &freq_l);
			if (ret)
				return ret;

            HI_MSG("freq_h=%x\n", freq_h);
            HI_MSG("freq_l=%x\n", freq_l);

            freq = ((freq_h & 0xf) << 8) + freq_l;
            value.freq_l = 3270000 + (freq * 10000) / 3052;

            HI_MSG("freq=%d\n", value.freq_l);
            #if 0
            if (osal_copy_to_user((void*)arg, &value, sizeof(rtc_freq_t)))
            {
                HI_MSG("[error]Bad addres\n");
                return -1;
            }
            #else
            osal_memcpy((void*)arg, &value, sizeof(rtc_freq_t));
            #endif
            break;
        }
        case HI_RTC_ALM_READ:
		{
            ret = hirtc_get_alarm(&time);
			if (ret)
				return ret;
            #if 0
            if (osal_copy_to_user((void*)arg, &time, sizeof(time)))
            {
                HI_MSG("[error]Bad addres\n");
                return -1;
            }
            #else
            osal_memcpy((void*)arg, &time, sizeof(time));
            #endif
            break;
        }
        case HI_RTC_ALM_SET:
		{
            #if 0
            if (osal_copy_from_user(&time, (struct rtc_time_t*) arg,
                               sizeof(time)))
            {
                HI_MSG("[error]Bad addres\n");
                return -1;
            }
            #else
            osal_memcpy(&time, (struct rtc_time_t*) arg, sizeof(time));
            #endif
            ret = hirtc_set_alarm(time);
            if (ret)
            {
                HI_MSG("[error]Bad addres\n");
                return -1;
            }
            break;
        }
        case HI_RTC_RD_TIME:
		{
            HI_MSG("HI_RTC_RD_TIME");
            ret = hirtc_get_time(&time);
			if (ret)
            {
                HI_ERR("[error]Get time failed\n");
                return -1;
            }
            #if 0
            ret = osal_copy_to_user((void*)arg, &time, sizeof(time));
            if (ret)
            {
                return -1;
            }
            #else
            osal_memcpy((void*)arg, &time, sizeof(time));
            #endif
            break;
        }
        case HI_RTC_SET_TIME:
		{
            HI_MSG("HI_RTC_SET_TIME");

            osal_memcpy(&time, (struct rtc_time_t*) arg, sizeof(time));

            ret = hirtc_set_time(time);
			if (ret)
            {
                HI_ERR("[error]Set time failed\n");
                return -1;
            }
            break;
        }
        case HI_RTC_RESET:
		{
            rtc_reset_first();
            break;
        }
        case HI_RTC_REG_SET:
		{
            #if 0
            ret = osal_copy_from_user(&reg_info, (struct reg_data_t*) arg,
                                 sizeof(reg_info));
            if (ret)
            {
                HI_MSG("[error]Bad addres\n");
                return -1;
            }
            #else
            osal_memcpy(&reg_info, (struct reg_data_t*) arg, sizeof(reg_info));
            #endif

			if (reg_info.reg_index > REG_INDEX_MAX_VAL)
            {
                HI_ERR("[error]reg_index(%u) is illegal\n", reg_info.reg_index);
                return -1;
            }
            ret = spi_rtc_write(reg_val[reg_info.reg_index], reg_info.val);
			if (ret)
				return ret;
            break;
        }
        case HI_RTC_REG_READ:
		{
            #if 0
            ret = osal_copy_from_user(&reg_info, (struct reg_data_t*) arg,
                                 sizeof(reg_info));
            if (ret)
            {
                HI_MSG("[error]Bad addres\n");
                return -1;
            }
            #else
            osal_memcpy(&reg_info, (struct reg_data_t*) arg, sizeof(reg_info));
            #endif
			if (reg_info.reg_index > REG_INDEX_MAX_VAL)
            {
                HI_ERR("[error]reg_index(%u) is illegal\n", reg_info.reg_index);
                return -1;
            }
            ret = spi_rtc_read(reg_val[reg_info.reg_index], &reg_info.val);
			if (ret)
				return ret;
            #if 0
            ret = osal_copy_to_user((void*)arg, &reg_info, sizeof(reg_info));
            if (ret)
            {
                HI_MSG("[error]Bad addres\n");
                return -1;
            }
            #else
            osal_memcpy((void*)arg, &reg_info, sizeof(reg_info));
            #endif
            break;
       }
	default:
		return -1;
	}

	return 0;
}

/*
 * interrupt function
 * do nothing. left for future
 */
static int rtc_alm_interrupt(int irq, void *dev_id)
{
	int ret = OSAL_IRQ_NONE;
    unsigned char val;

    rtc_spi_read(RTC_INT, &val);
    rtc_spi_write(RTC_INT_CLR, 0x1);

    osal_printk("interrupt %#x\n", val);

	if (val & 0x1)
    {
		/*FIXME: do what you want here. such as wake up a pending thread.*/
    }

	if (val & 0x2)
    {
		/* close low voltage int */
		rtc_spi_read(RTC_IMSC, &val);
		rtc_spi_write(RTC_IMSC, val & ~(0x2));
    }

	ret = OSAL_IRQ_HANDLED;
	return ret;
}

static int hi_rtc_open(void *private_data)
{
	return 0;
}

static int hi_rtc_release(void *private_data)
{
	return 0;
}

static struct osal_fileops hi_rtc_fops =
{
    .unlocked_ioctl = hi_rtc_ioctl,
    .open = hi_rtc_open,
    .release = hi_rtc_release,
};

static osal_dev_t *rtc_dev = 0;

int  rtc_init(void)
{
	int ret = 0;
	unsigned char val = 0;

    if (rtc_irq <= 0)
    {
        rtc_irq = RTC_IRQ;
    }

    if (!pRtcRegBase)
    {
        pRtcRegBase = (volatile void *)osal_ioremap_nocache(RTC_SPI_BASE_ADDR, 0x100);
        if (!pRtcRegBase)
        {
            osal_printk("osal_ioremap_nocache err. \n");
            return -1;
        }
    }

    ret = osal_spin_lock_init(&rtc_spin_lock);
	if (0 != ret)
	{
		HI_ERR("failed to init spin lock!\n");
        return -1;
    }


	ret = osal_mutex_init(&hirtc_lock);
	if (0 != ret)
	{
		HI_ERR("failed to init mutex!\n");
        goto RTC_INIT_FAIL0;
    }

    rtc_dev = osal_createdev(RTC_NAME);
    if (NULL == rtc_dev)
	{
	    HI_ERR("cretate rtc device failed!\n");
        goto RTC_INIT_FAIL1;
	}
	rtc_dev->fops = &hi_rtc_fops;
    rtc_dev->minor = 255;
	ret = osal_registerdevice(rtc_dev);
	if (0 != ret)
	{
		HI_ERR("rtc device register failed!\n");
        goto RTC_INIT_FAIL2;
    }

	ret = osal_request_irq(rtc_irq, &rtc_alm_interrupt, NULL,
			"RTC Alarm", (void*)&alm_itrp);
	if(0 != ret)
	{
		HI_ERR("hi3516CV300 rtc: failed to register IRQ(%d)\n", RTC_IRQ);
		goto RTC_INIT_FAIL3;
	}

    /* clk div value = (apb_clk/spi_clk)/2-1, for asic ,
       apb clk = 100MHz, spi_clk = 10MHz,so value= 0x4 */
    osal_writel(0x4, SPI_CLK_DIV);

	ret |= spi_rtc_write(RTC_IMSC, 0x4);
	ret |= spi_rtc_write(RTC_SAR_CTRL, 0x20);

    /* default SDM_COEF_OUSIDE */
    ret |= spi_rtc_write(RTC_FREQ_H, FREQ_H_DEFAULT);
    ret |= spi_rtc_write(RTC_FREQ_L, FREQ_L_DEFAULT);

    ret |= spi_rtc_read(RTC_INT_RAW, &val);
	if (ret) {
		goto RTC_INIT_FAIL4;
	}

	if (val & 0x2) {
		osal_printk("low voltage detected, date/time is not reliable.\n");
	}

	osal_printk("hirtc init ok. ver=%s, %s.\n", __DATE__, __TIME__);
	return 0;

RTC_INIT_FAIL4:
	osal_free_irq(rtc_irq, (void*)&alm_itrp);

RTC_INIT_FAIL3:
	osal_deregisterdevice(rtc_dev);

RTC_INIT_FAIL2:
    osal_destroydev(rtc_dev);

RTC_INIT_FAIL1:
    osal_mutex_destory(&hirtc_lock);

RTC_INIT_FAIL0:
    osal_spin_lock_destory(&rtc_spin_lock);

	return -1;
}

void  rtc_exit(void)
{
	osal_free_irq(rtc_irq, (void*)&alm_itrp);

	osal_deregisterdevice(rtc_dev);
	osal_destroydev(rtc_dev);

	osal_mutex_destory(&hirtc_lock);


	osal_spin_lock_destory(&rtc_spin_lock);

    osal_printk("hirtc exit ok.\n");
}

MODULE_LICENSE("GPL");
