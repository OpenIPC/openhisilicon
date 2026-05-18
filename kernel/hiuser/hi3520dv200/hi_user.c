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
#include <mach/hardware.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/sched.h>

//#include <linux/mm.h>
#include <linux/dma-mapping.h>
#include <asm/cacheflush.h>
#include <asm/glue-cache.h>
#include <linux/linkage.h>


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

extern void hi_dma_inv_range(const void *kaddr, size_t size);
void hi_dma_map_area(const void *kaddr, size_t size, enum dma_data_direction dir)
{
	hi_dma_inv_range(kaddr, size);
}

int hil_mmb_Invalid_Cache_L1L2_byaddr(void *kvirt, unsigned long phys_addr, unsigned long length)
{
	if (NULL == kvirt)
		return -EINVAL;
	
	/* Invalid L1 cache, use viraddr */ 
	hi_dma_map_area(kvirt, length, DMA_FROM_DEVICE);
	
#if defined CONFIG_CACHE_HIL2V200
	/* Invalid L2 cache, use phyaddr */ 
	outer_inv_range(phys_addr, phys_addr+length); 													  
#endif
	
	return 0;
}
EXPORT_SYMBOL(hil_mmb_Invalid_Cache_L1L2_byaddr);




/*********************************************************************
 * event notification from Hisilicon SDK.
 *
 * @param[in] module_id     module id. (MOD_ID_E)
 * @param[in] channel       device channel number.
 * @param[in] event_type    event type.
 *   ex) types of VDEC event
 *		 - VDEC_EVNT_STREAM_ERR = 1,
 *		 - VDEC_EVNT_UNSUPPORT,
 *		 - VDEC_EVNT_OVER_REFTHR,
 *		 - VDEC_EVNT_REF_NUM_OVER,
 *		 - VDEC_EVNT_SLICE_NUM_OVER,
 *		 - VDEC_EVNT_SPS_NUM_OVER,
 *		 - VDEC_EVNT_PPS_NUM_OVER,
 *		 - VDEC_EVNT_PICBUF_SIZE_ERR,   
 *		 - VDEC_EVNT_SIZE_OVER,
 *		 - VDEC_EVNT_IMG_SIZE_CHANGE, 
 *		 - VDEC_EVNT_BUTT
 * @param[in] args          additional arguments.
 * @return none.
 *
 *************************************************************************/
void hi_user_notify_vdec_event(int module_id, int channel, int event_type, void* args)
{
	int *pArgs = (int *)args;
		
	switch(event_type)
	{
		/* VDEC_EVNT_STREAM_ERR: stream package error. */
		case 1:
			{	
				/* pArgs is null. */
				break;
			}
		
		/* VDEC_EVNT_UNSUPPORT: unsupport the stream specification. */
		case 2:
			{	
				/* pArgs is null. */
				break;
			}
		
		/* VDEC_EVNT_OVER_REFTHR: stream error rate is larger than s32ChanErrThr. */
		case 3:
			{
				/* pArgs[0] is the stream error rate of current frame. */
				break;
			}
		
		/* VDEC_EVNT_REF_NUM_OVER: reference num is not enough. */
		case 4:
			{
				/* pArgs[0] is reference num of current frame. */
				break;
			}
		
		/* VDEC_EVNT_SLICE_NUM_OVER: slice num is not enough. */
		case 5:
			{
				/* pArgs is null. */
				break;
			}
		
		/* VDEC_EVNT_SPS_NUM_OVER: sps num is not enough. */
		case 6:
			{
				/* pArgs is null. */
				break;
			}

		/* VDEC_EVNT_PPS_NUM_OVER: pps num is not enough. */
		case 7:
			{
				/* pArgs is null. */
				break;
			}

		/* VDEC_EVNT_PICBUF_SIZE_ERR: the format of JPEG is YUV444, and the picture buffer size is not enough. */
		case 8:
			{
				/* pArgs is null. */
				break;
			}

		/* VDEC_EVNT_SIZE_OVER: picture width or height is larger than chnnel width or height */
		case 9:
			{
				/* pArgs[0] is width, pArgs[1] is height of current frame. */
				break;
			}

		/* VDEC_EVNT_IMG_SIZE_CHANGE: the width or height of stream change */
		case 10:
			{				
				/* pArgs[0] is width, pArgs[1] is height of current frame. */
				break;
			}
		default:
			{
				break;
			}
	}
}
EXPORT_SYMBOL(hi_user_notify_vdec_event);


static int __init hi_user_init(void)
{
	return 0;
}

static void __exit hi_user_exit(void)
{
	return;
}

module_init(hi_user_init);
module_exit(hi_user_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hisilicon");


