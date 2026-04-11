/******************************************************************************

  Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.

 ******************************************************************************/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/version.h>
#include <linux/pagemap.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include "hi_comm_video.h"
#include "viu_ext.h"
#include "vou_exp.h"


int hi_sched_setscheduler(struct task_struct *p)
{
#if 0
    struct sched_param param;
    param.sched_priority = 99;

    return sched_setscheduler(p, SCHED_RR, &param);
#else

	return 0;
#endif
}
EXPORT_SYMBOL(hi_sched_setscheduler);


void hi_user_notify_vou_event(int module_id, int vodev)
{
    switch ( vodev )
    {
        case 0 :
        {
        /* do something */
            //printk("module_id %d vodev %d \n",module_id,vodev);
            break;
        }
        default:
        {
            break;
        }
    }
}
//EXPORT_SYMBOL(hi_user_notify_vou_event);

HI_S32  ViuGetRawStatus(HI_S32 ViDev,VIDEO_FRAME_INFO_S  *pstFrameInfo)
{
    return HI_SUCCESS; 
}


extern VOU_EXTERN_FUNC_S g_stVouExpFunc;
VOU_EXTERN_CALLBACK_S stExternCb;


extern  VIU_EXPORT_FUNC_S	g_stViuExpFunc;
VIU_DUMPRAW_CALLBACK_S stDumpRawCb;


static int __init hi_user_init(void)
{
    if(NULL != g_stVouExpFunc.pfnVoRegisterExternCallBack)
    {
        stExternCb.pfnVoNotify = hi_user_notify_vou_event;
        g_stVouExpFunc.pfnVoRegisterExternCallBack(&stExternCb);
    }

    if(NULL != g_stViuExpFunc.pfnViRegisterDumpRawCallBack)
    {
  //      stDumpRawCb.pfnViuGetRawStatus = ViuGetRawStatus;
  //      g_stViuExpFunc.pfnViRegisterDumpRawCallBack(0,&stDumpRawCb);
  //      g_stViuExpFunc.pfnViRegisterDumpRawCallBack(1,&stDumpRawCb);
    }

    printk(KERN_INFO "load hi_user.ko ...OK!\n");
	return 0;
}

static void __exit hi_user_exit(void)
{
    printk(KERN_INFO "unload hi_user.ko ...OK!\n");
	return;
}

module_init(hi_user_init);
module_exit(hi_user_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hisilicon");

