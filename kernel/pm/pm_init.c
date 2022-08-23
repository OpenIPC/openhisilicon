
#include <linux/module.h>
#include <linux/moduleparam.h>

#define DEFAULT_TIME 5000

static int time_ms = DEFAULT_TIME;
static int rtc_enable = 1;
module_param(time_ms, int, S_IRUSR);
module_param(rtc_enable, int, S_IRUSR);

extern int temp_ctrl_init(int time_ms, int rtc_enable);

static int __init pm3_init(void)
{
	return temp_ctrl_init(time_ms, rtc_enable);
}

extern void temp_ctrl_exit(void);

static void __exit pm3_exit(void)
{
	temp_ctrl_exit();
}

module_init(pm3_init);
module_exit(pm3_exit);

MODULE_AUTHOR("hisilion");
MODULE_DESCRIPTION("Temperature control driver");
MODULE_LICENSE("GPL");
