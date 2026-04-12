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

#include "hi_osal.h"

#include "hi_rtc.h"
#include "rtc_temp_lut_tbl.h"

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

#define OSDRV_MODULE_VERSION_STRING "HISI_RTC @Hi3519V100"


#define CRG_BASE_ADDR		osal_ioremap_nocache(0x12010000, 0x10000)
#define PERI_CRG57			(CRG_BASE_ADDR + 0xE4)
#define BIT_TEMP_SRST_REQ	3


/* RTC Control over SPI */
#define RTC_SPI_BASE_ADDR	0x12090000
#define RTC_SPI_REG_LENGTH 0x10000
volatile void* pRtcRegBase = NULL;

#define SPI_CLK_DIV			(pRtcRegBase + 0x000)
#define SPI_RW				(pRtcRegBase + 0x004)

/* RTC temperature reg */
#define RTC_TEMP_START		(pRtcRegBase + 0x80)
#define RTC_TEMP_CRC		(pRtcRegBase + 0x84)
#define RTC_TEMP_INT_MASK	(pRtcRegBase + 0x88)
#define RTC_TEMP_INT_CLEAR	(pRtcRegBase + 0x8c)
#define RTC_TEMP_STAT		(pRtcRegBase + 0x90)
#define RTC_TEMP_INT_RAW	(pRtcRegBase + 0x94)
#define RTC_TEMP_INT_STAT	(pRtcRegBase + 0x98)
#define RTC_TEMP_VAL		(pRtcRegBase + 0x9c)

#define RTC_TEMP_IRQ_NUM	(33)

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

#define RTC_IRQ			(33)
unsigned int rtc_irq = RTC_IRQ;

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
#define RTC_INT_MASK	0x15
#define RTC_INT_RAW		0x16

#define RTC_CLK			0x17
#define RTC_POR_N		0x18
#define RTC_SAR_CTRL	0x1A
#define RTC_ANA_CTRL	0x1B
#define TOT_OFFSET_L	0x1C
#define TOT_OFFSET_H	0x1D
#define TEMP_OFFSET		0x1E
#define OUTSIDE_TEMP	0x1F
#define INTERNAL_TEMP	0x20
#define TEMP_SEL		0x21
#define LUT1			0x22

#define RTC_FREQ_H		0x51
#define RTC_FREQ_L		0x52

#define NORMAL_TEMP_VALUE 25
#define TEMP_TO_RTC(value) (((value) + 40)*255/180)

#define TEMP_ENV_OFFSET   27
#define TEMP_OFFSET_TO_RTC(value) ((value)*255/180)
#define RETRY_CNT 100

#define FREQ_MAX_VAL	3277000
#define FREQ_MIN_VAL	3276000

static osal_spinlock_t rtc_spin_lock;
static osal_mutex_t hirtc_lock;

static int is_comp_off = 0;

static struct osal_timer temperature_timer;
static enum temp_sel_mode mode = TEMP_SEL_FIXMODE;
int t_second = 5;


static int alm_itrp;
static int temperature_itrp;

static int spi_write(char reg, char val)
{
    U_SPI_RW w_data, r_data;

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
    while (r_data.bits.spi_busy);

    return 0;
}


static int spi_rtc_write(char reg, char val)
{
    osal_mutex_lock(&hirtc_lock);
    spi_write(reg, val);
    osal_mutex_unlock(&hirtc_lock);

    return 0;
}

static int spi_read(char reg, unsigned char* val)
{
    U_SPI_RW w_data, r_data;

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
    while (r_data.bits.spi_busy);

    *val = r_data.bits.spi_rdata;

    return 0;
}



static int spi_rtc_read(char reg, unsigned char* val)
{
    osal_mutex_lock(&hirtc_lock);
    spi_read(reg, val);
    osal_mutex_unlock(&hirtc_lock);

    return 0;
}

static int temp_crg_reset(void)
{
    int value = osal_readl(PERI_CRG57);
    osal_mb();
    osal_writel(value | (1 << BIT_TEMP_SRST_REQ),  PERI_CRG57);
    osal_mb();
    osal_writel(value & ~(1 << BIT_TEMP_SRST_REQ), PERI_CRG57);

    return 0;
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
                          unsigned long* ptimeOfsecond)
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
static int rtcSecond2Date(rtc_time_t* compositetime,
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

static int hirtc_get_alarm(rtc_time_t* compositetime)
{
    unsigned char dayl, dayh;
    unsigned char second, minute, hour;
    unsigned long seconds = 0;
    unsigned int day;

    spi_rtc_read(RTC_MR_S, &second);
    spi_rtc_read(RTC_MR_M, &minute);
    spi_rtc_read(RTC_MR_H, &hour);
    spi_rtc_read(RTC_MR_D_L, &dayl);
    spi_rtc_read(RTC_MR_D_H, &dayh);
    day = (unsigned int)(dayl | (dayh << 8));
    HI_MSG("day %d\n", day);
    seconds = second + minute * 60 + hour * 60 * 60 + day * 24 * 60 * 60;

    rtcSecond2Date(compositetime, seconds);
    return 0;
}

static int hirtc_set_alarm(rtc_time_t compositetime)
{
    unsigned int days;
    unsigned long seconds = 0;

    HI_MSG("RTC read time");
    HI_MSG("\tyear %d", compositetime.year);
    HI_MSG("\tmonth %d", compositetime.month);
    HI_MSG("\tdate %d", compositetime.date);
    HI_MSG("\thour %d", compositetime.hour);
    HI_MSG("\tminute %d", compositetime.minute);
    HI_MSG("\tsecond %d", compositetime.second);
    HI_MSG("\tweekday %d", compositetime.weekday);

    rtcdate2second(compositetime, &seconds);
    days = seconds / (60 * 60 * 24);

    spi_rtc_write(RTC_MR_10MS, 0);
    spi_rtc_write(RTC_MR_S, compositetime.second);
    spi_rtc_write(RTC_MR_M, compositetime.minute);
    spi_rtc_write(RTC_MR_H, compositetime.hour);
    spi_rtc_write(RTC_MR_D_L, (days & 0xFF));
    spi_rtc_write(RTC_MR_D_H, (days >> 8));
    return 0;
}

static int hirtc_get_time(rtc_time_t* compositetime)
{
    unsigned char dayl, dayh;
    unsigned char second, minute, hour;
    unsigned long seconds = 0;
    unsigned int day;

    unsigned char raw_value;

    spi_rtc_read(RTC_LORD, &raw_value);
    if (raw_value & 0x4)
    {
        spi_rtc_write(RTC_LORD, (~(1 << 2)) & raw_value);
    }

    spi_rtc_read(RTC_LORD, &raw_value);
    spi_rtc_write(RTC_LORD, (1 << 1) | raw_value); //lock the time

    do
    {
        spi_rtc_read(RTC_LORD, &raw_value);
    }
    while (raw_value & 0x2);

    osal_msleep(1);  // must delay 1 ms

    spi_rtc_read(RTC_S_COUNT, &second);
    spi_rtc_read(RTC_M_COUNT, &minute);
    spi_rtc_read(RTC_H_COUNT, &hour);
    spi_rtc_read(RTC_D_COUNT_L, &dayl);
    spi_rtc_read(RTC_D_COUNT_H, &dayh);
    day = (dayl | (dayh << 8));

    HI_MSG("day %d\n", day);

    seconds = second + minute * 60 + hour * 60 * 60 + day * 24 * 60 * 60;

    rtcSecond2Date(compositetime, seconds);
    return 0;
}

static int hirtc_set_time(rtc_time_t compositetime)
{
    unsigned char ret = 0;
    unsigned int days;
    unsigned long seconds = 0;
    unsigned int cnt = 0;

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
    days = seconds / (60 * 60 * 24);

    HI_MSG("day %d\n", days);

    spi_rtc_write(RTC_LR_10MS, 0);
    spi_rtc_write(RTC_LR_S, compositetime.second);
    spi_rtc_write(RTC_LR_M, compositetime.minute);
    spi_rtc_write(RTC_LR_H, compositetime.hour);
    spi_rtc_write(RTC_LR_D_L, (days & 0xFF));
    spi_rtc_write(RTC_LR_D_H, (days >> 8));

    spi_rtc_write(RTC_LORD, (ret | 0x1));

    /* wait rtc load flag */
    do
    {
        spi_rtc_read(RTC_LORD, &ret);
        osal_msleep(1);
        cnt++;
    }
    while ((ret & 0x1) && (cnt < RETRY_CNT));

    if (cnt >= RETRY_CNT)
    {
        HI_ERR("check state error!\n");
        return -1;
    }

    /* must sleep 10ms, internal clock is 100Hz */
    osal_msleep(10);

    HI_MSG("set time ok!\n");
    return 0;
}

static long hi_rtc_ioctl (unsigned int cmd, unsigned long arg, void* private_data)
{
    unsigned long flags;
    int ret;

    rtc_time_t time;
    reg_data_t reg_info;
    reg_temp_mode_t temp_mode;
    unsigned char temp_reg;

    switch (cmd)
    {
            /* Alarm interrupt on/off */
        case HI_RTC_AIE_ON:
            HI_MSG("HI_RTC_AIE_ON");
            spi_rtc_write(RTC_IMSC, 0x5);
            break;

        case HI_RTC_AIE_OFF:
            HI_MSG("HI_RTC_AIE_OFF");
            spi_rtc_write(RTC_IMSC, 0x0);
            break;

        case HI_RTC_COMP_OFF:
            if (is_comp_off)
            { break; }

            /*The timer may still in use , turn off it!*/
            osal_del_timer(&temperature_timer);

            spi_rtc_write(TEMP_SEL, 0x6);
            break;

        case HI_RTC_COMP_ON:
            spi_rtc_write(TEMP_SEL, 0x1);
            is_comp_off = 0;
            break;

        case HI_RTC_SET_FREQ:
        {
            char freq_l, freq_h;
            unsigned int freq;
            rtc_freq_t value;
#if 0
            if (osal_copy_from_user(&value, (struct rtc_freq_t*) arg, sizeof(value)))
            {
                HI_MSG("[error]Bad addres\n");
                return -1;
            }
#else
            osal_memcpy(&value, (struct rtc_freq_t*) arg, sizeof(value));
#endif

            freq = value.freq_l;

            /* freq = 32700 + (freq /3052)*100 */

            if (freq > FREQ_MAX_VAL || freq < FREQ_MIN_VAL)
            {
                HI_MSG("[error]Invalid argument\n");
                return -1;
            }

            freq = (freq - 3270000) * 3052 / 10000;

            freq_l = (char)(freq & 0xff);
            freq_h = (char)((freq >> 8) & 0xf);

            spi_rtc_write(RTC_FREQ_H, freq_h);
            spi_rtc_write(RTC_FREQ_L, freq_l);

            break;
        }

        case HI_RTC_GET_FREQ:
        {
            unsigned char freq_l, freq_h;
            unsigned int freq;
            rtc_freq_t value;

            spi_rtc_read(RTC_FREQ_H, &freq_h);
            spi_rtc_read(RTC_FREQ_L, &freq_l);

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
            hirtc_get_alarm(&time);
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

        case HI_RTC_ALM_SET:
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

        case HI_RTC_RD_TIME:
            HI_MSG("HI_RTC_RD_TIME");
            hirtc_get_time(&time);
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

        case HI_RTC_SET_TIME:
            HI_MSG("HI_RTC_SET_TIME");
#if 0
            ret = osal_copy_from_user(&time,
                                      (struct rtc_time_t*) arg,
                                      sizeof(time));
            if (ret)
            {
                HI_MSG("[error]Bad addres\n");
                return -1;
            }
#else
            osal_memcpy(&time, (struct rtc_time_t*) arg, sizeof(time));
#endif
            hirtc_set_time(time);
            break;

        case HI_RTC_RESET:
            rtc_reset_first();
            break;

        case HI_RTC_REG_SET:
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
            spi_rtc_write(reg_info.reg, reg_info.val);
            break;

        case HI_RTC_REG_READ:
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

            spi_rtc_read(reg_info.reg, &reg_info.val);
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

        case HI_RTC_SET_TEMP_MODE:

            if (is_comp_off)
            {
                HI_ERR("compensation off, please SET_COMP_ON first!\n");
                return -1;
            }
#if 0

            ret = osal_copy_from_user(&temp_mode, (struct reg_temp_mode_t*) arg,
                                      sizeof(temp_mode));
            if (ret)
            {
                HI_MSG("[error]Bad addres\n");
                return -1;
            }
#else
            osal_memcpy(&temp_mode, (struct reg_temp_mode_t*) arg, sizeof(temp_mode));
#endif

            osal_spin_lock_irqsave(&rtc_spin_lock, &flags);
            mode = temp_mode.mode;
            osal_spin_unlock_irqrestore(&rtc_spin_lock, &flags);

            switch (mode)
            {
                case TEMP_SEL_FIXMODE:
                    if (temp_mode.value > 140 || temp_mode.value < -40)
                    {
                        HI_MSG("invalid value %d", temp_mode.value);
                        return -1;
                    }

                    osal_del_timer(&temperature_timer);

                    spi_rtc_write(OUTSIDE_TEMP, TEMP_TO_RTC(temp_mode.value));
                    break;

                case TEMP_SEL_INTERNAL:
                case TEMP_SEL_OUTSIDE:
                    osal_set_timer(&temperature_timer, 1000);
                    break;

                default:
                    return -1;
            }
            break;

        case HI_RTC_GET_TEMP:
            HI_MSG("HI_RTC_GET_TEMP");
            spi_rtc_read(INTERNAL_TEMP, &temp_reg);
#if 0
            ret = osal_copy_to_user((int*)arg, &temp_reg, sizeof(temp_reg));
            if (ret)
            {
                HI_MSG("[error]Bad addres\n");
                return -1;
            }
#else
            osal_memcpy((int*)arg, &temp_reg, sizeof(temp_reg));
#endif
            break;

        default:
            return -1;
    }

    return 0;
}



static void temperature_detection(unsigned long arg)
{
    int ret;
    int cnt = 0;

    HI_MSG("START temperature adjust");

    if (mode == TEMP_SEL_OUTSIDE)
    {

        do
        {
            ret = osal_readl(RTC_TEMP_STAT);
            osal_udelay(10);
            cnt++;
        }
        while (ret && (cnt < RETRY_CNT));

        if (cnt >= RETRY_CNT)
        {
            /*maybe temperature capture module is error,
              need reset */
            HI_ERR("RTC_TEMP_STAT not ready,please check pin configuration\n");
            temp_crg_reset();
            goto timer_again;
        }

        HI_MSG("WRITE RTC_TEMP_START");

        osal_writel(0x1, RTC_TEMP_START);
    }
    else if (mode == TEMP_SEL_INTERNAL)
    {
        unsigned char temp = TEMP_TO_RTC(25);

        spi_read(INTERNAL_TEMP, &temp);

        HI_MSG("internal temp is %d\n", temp);

        /*FIXME: sub offset to get enviroment temperature*/
        //temp -= 38;  /*38=27c*255/180*/
        temp -= TEMP_OFFSET_TO_RTC(TEMP_ENV_OFFSET);
        spi_write(OUTSIDE_TEMP, temp);
    }
    else
    {
        HI_ERR("invalid temperature mode");
    }

timer_again:
    osal_set_timer(&temperature_timer, 1000 * t_second);
}

/**
 * this function change DS1820 temperature format to native RTC format
 * OUTSIDE_TEMP value size (-40<oC>, +140<oC>), use 8bit to spec
 * DS1820 value size (-55<oC>, +120<oC>), use 9bit to spec
 */
static void set_temperature(void)
{
    unsigned short ret;
    unsigned char temp;

    ret = (unsigned short)osal_readl(RTC_TEMP_VAL);
    HI_MSG("READ DS1820 temperature value 0x%x", ret);

    /* mode 1 sample, ret > 0x800 means negative number */
    if (ret > 0x800)
    {
        /* change to positive number */
        ret = 4096 - ret;
        ret = ret / 2;
        /* rtc temperature lower limit -40<oC> */
        if (ret > 40)
        { ret = 40; }
        temp = (40 - ret) * 255 / 180;
    }
    else
    {
        /* rtc temperature upper limit 140<oC> */
        ret = ret / 2;
        if (ret > 140)
        { ret = 140; }
        temp = TEMP_TO_RTC(ret);
    }

    HI_MSG("WRITE RTC temperature value 0x%02x", temp);

    spi_write(OUTSIDE_TEMP, temp);
}

static int rtc_temperature_interrupt(int irq, void* dev_id)
{
    int ret = OSAL_IRQ_NONE;
    int irq_stat;

    irq_stat = osal_readl(RTC_TEMP_INT_STAT);
    if (!irq_stat)
    {
        return ret;
    }

    HI_MSG("irq mask");
    osal_writel(0x01, RTC_TEMP_INT_MASK);

    irq_stat = osal_readl(RTC_TEMP_INT_RAW);
    HI_MSG("irq clear");

    osal_writel(0x1, RTC_TEMP_INT_CLEAR);

    if (mode != TEMP_SEL_OUTSIDE)
    { goto endl; }

    if (irq_stat & 0x2)
    {
        /* err start again */
        osal_set_timer(&temperature_timer, 1000 * t_second);
    }
    else
    {
        /* set temperature data */
        set_temperature();
    }

endl:
    HI_MSG("irq unmask");
    osal_writel(0x0, RTC_TEMP_INT_MASK);

    ret = OSAL_IRQ_HANDLED;
    return ret;
}

/*
 * interrupt function
 * do nothing. left for future
 */
static int rtc_alm_interrupt(int irq, void* dev_id)
{
    int ret = OSAL_IRQ_NONE;

    osal_printk("RTC alarm interrupt!!!\n");

    //spi_rtc_write(RTC_IMSC, 0x0);
    spi_write(RTC_INT_CLR, 0x1);
    //spi_rtc_write(RTC_IMSC, 0x1);
    osal_udelay(200);
    /*FIXME: do what you want here. such as wake up a pending thread.*/

    ret = OSAL_IRQ_HANDLED;
    return ret;
}

static int hi_rtc_open(void* private_data)
{
    return 0;
}

static int hi_rtc_release(void* private_data)
{
    return 0;
}

static struct osal_fileops hi_rtc_fops =
{
    .unlocked_ioctl = hi_rtc_ioctl,
    .open = hi_rtc_open,
    .release = hi_rtc_release,
};

static osal_dev_t* rtc_dev = 0;

int  rtc_init(void)
{
    int i, ret = 0;
    char base;


    if (rtc_irq <= 0)
    {
        rtc_irq = RTC_TEMP_IRQ_NUM;
    }

    if (!pRtcRegBase)
    {
        pRtcRegBase = (volatile void*)osal_ioremap_nocache(RTC_SPI_BASE_ADDR, RTC_SPI_REG_LENGTH);
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
    if (0 != ret)
    {
        HI_ERR(" rtc: failed to register IRQ(%d)\n", rtc_irq);
        goto RTC_INIT_FAIL3;
    }

    ret = osal_request_irq(rtc_irq,
                           &rtc_temperature_interrupt,
                           NULL,
                           "RTC Temperature", (void*)&temperature_itrp);
    if (0 != ret)
    {
        HI_ERR(" rtc: failed to register IRQ(%d)\n", rtc_irq);
        goto RTC_INIT_FAIL4;
    }

    ret = osal_timer_init(&temperature_timer);
    if (0 != ret)
    {
        HI_ERR("failed to init timer\n");
        goto RTC_INIT_FAIL5;
    }
    temperature_timer.function = temperature_detection;

    /* clk div value = (apb_clk/spi_clk)/2-1, for asic ,
       apb clk = 100MHz, spi_clk = 10MHz,so value= 0x4 */
    osal_writel(0x4, SPI_CLK_DIV);

    /* always fixed frequency division mode */
    spi_rtc_write(TEMP_SEL, 0x06);
    spi_rtc_write(RTC_SAR_CTRL, 0x08);
    spi_rtc_write(OUTSIDE_TEMP, TEMP_TO_RTC(25));

    /* rtc analog voltage config */
    spi_rtc_write(RTC_ANA_CTRL, 0x01);

    /* set default freq */
    spi_rtc_write(RTC_FREQ_H, 0x8);
    spi_rtc_write(RTC_FREQ_L, 0x1b);


    /*init rtc temperature lut table value*/
    for (i = 0; i < sizeof(temp_lut_table) / sizeof(temp_lut_table[0]); i++)
    {
        if (i < 3)
        {
            base = TOT_OFFSET_L;
            spi_rtc_write(base + i, temp_lut_table[i]);
        }
        else
        {
            base = LUT1;
            spi_rtc_write(base + i - 3, temp_lut_table[i]);
        }
    }

    /* enable temperature IRQ */
    osal_writel(0x0, RTC_TEMP_INT_MASK);

    osal_printk("hirtc init ok. ver=%s, %s.\n", __DATE__, __TIME__);
    return 0;

RTC_INIT_FAIL5:
    osal_free_irq(rtc_irq, (void*)&temperature_itrp);

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
    osal_del_timer(&temperature_timer);
    osal_timer_destory(&temperature_timer);

    osal_free_irq(rtc_irq, (void*)&alm_itrp);
    osal_free_irq(rtc_irq, (void*)&temperature_itrp);

    osal_deregisterdevice(rtc_dev);
    osal_destroydev(rtc_dev);

    osal_mutex_destory(&hirtc_lock);

    osal_spin_lock_destory(&rtc_spin_lock);
}
