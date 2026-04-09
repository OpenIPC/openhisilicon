/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2016-2019. All rights reserved.
 * Description: user driver.
 * Author: Hisilicon multimedia software group
 * Create: 2016-11-11
 */

#include "hi_user.h"

/* Set decoding thread Priority. Only support in linux */
int hi_sched_setscheduler_dec(void)
{
#ifndef __HuaweiLite__
    struct sched_param param;
    param.sched_priority = 99;  /* set priority to 99 */
    return sched_setscheduler(current, SCHED_FIFO, &param);
#else
    const int sched_priority = 7; /* set priority to 7 */
    return LOS_CurTaskPriSet(sched_priority);
#endif
}

/* Set decoding thread Priority. Only support in liteos */
int hi_sched_setscheduler_stm(void)
{
#ifdef __HuaweiLite__
    const int sched_priority = 7; /* set priority to 7 */
    return LOS_CurTaskPriSet(sched_priority);
#else
    return 0;
#endif
}

/* Set decoding thread Priority. Only support in liteos */
int hi_sched_setscheduler_syn(void)
{
#ifdef __HuaweiLite__
    const int sched_priority = 7; /* set priority to 7 */
    return LOS_CurTaskPriSet(sched_priority);
#else
    return 0;
#endif
}

/*
 * event notification from Hisilicon SDK.
 * @param[in] module_id  module id. (MOD_ID_E)
 * @param[in] channel    device channel number.
 * @param[in] event_type event type.
 *   ex) types of VDEC event
 *         - VDEC_EVNT_STREAM_ERR = 1,
 *         - VDEC_EVNT_UNSUPPORT,
 *         - VDEC_EVNT_OVER_REFTHR,
 *         - VDEC_EVNT_REF_NUM_OVER,
 *         - VDEC_EVNT_SLICE_NUM_OVER,
 *         - VDEC_EVNT_SPS_NUM_OVER,
 *         - VDEC_EVNT_PPS_NUM_OVER,
 *         - VDEC_EVNT_PICBUF_SIZE_ERR,
 *         - VDEC_EVNT_SIZE_OVER,
 *         - VDEC_EVNT_IMG_SIZE_CHANGE,
 *         - VDEC_EVNT_BUTT
 * @param[in] args additional arguments.
 */
void hi_user_notify_vdec_event(int module_id, int channel, int event_type, void* args)
{
    /* code int *pArgs = (int *)args; */
    switch (event_type) {
        case 1:  /* 1 VDEC_EVNT_STREAM_ERR: stream package error. */
            /* pArgs is null. */
            break;

        case 2:  /* 2 VDEC_EVNT_UNSUPPORT: unsupport the stream specification. */
            /* pArgs is null. */
            break;

        case 3:  /* 3 VDEC_EVNT_OVER_REFTHR: stream error rate is larger than s32ChanErrThr. */
            /* pArgs[0] is the stream error rate of current frame. */
            break;

        case 4:  /* 4 VDEC_EVNT_REF_NUM_OVER: reference num is not enough. */
            /* pArgs[0] is reference num of current frame. */
            break;

        case 5:  /* 5 VDEC_EVNT_SLICE_NUM_OVER: slice num is not enough. */
            /* pArgs is null. */
            break;

        case 6:  /* 6 VDEC_EVNT_SPS_NUM_OVER: sps num is not enough. */
            /* pArgs is null. */
            break;

        case 7:  /* 7 VDEC_EVNT_PPS_NUM_OVER: pps num is not enough. */
            /* pArgs is null. */
            break;

        case 8:  /* 8 VDEC_EVNT_PICBUF_SIZE_ERR: the picture buffer size is not enough. */
            /* pArgs is null. */
            break;

        case 9:  /* 9 VDEC_EVNT_SIZE_OVER: picture width or height is larger than chnnel width or height */
            /* pArgs[0] is width, pArgs[1] is height of current frame. */
            break;

        case 10: /* 10 VDEC_EVNT_IMG_SIZE_CHANGE: the width or height of stream change */
            /* pArgs[0] is width, pArgs[1] is height of current frame. */
            break;

        default:
            break;
    }
}

void hi_user_notify_vo_event(int module_id, int vodev)
{
    switch (vodev) {
        case 0: /* 0 device */
            /* do something here. */
            break;

        default:
            break;
    }
}

int __init hi_user_init(void)
{
#if (VDEC_NOTIFY || VDEC_SET_SCHEDULER)
    {
        VDEC_EXPORT_CALLBACK_S vdec_exp_callback;

        osal_memset(&vdec_exp_callback, 0, sizeof(VDEC_EXPORT_CALLBACK_S));

        if (g_stVdecExpSymbol.pfnVdecRegisterExpCallback != NULL) {
#if VDEC_NOTIFY
            vdec_exp_callback.pfnVdecNotify = hi_user_notify_vdec_event;
#endif

#if VDEC_SET_SCHEDULER
            vdec_exp_callback.pfnVdecSetscheduler_Dec = hi_sched_setscheduler_dec;
            vdec_exp_callback.pfnVdecSetscheduler_Stm = hi_sched_setscheduler_stm;
            vdec_exp_callback.pfnVdecSetscheduler_Syn = hi_sched_setscheduler_syn;
#endif

            g_stVdecExpSymbol.pfnVdecRegisterExpCallback(&vdec_exp_callback);
        }
    }
#endif

#if VO_NOTIFY
    {
        VOU_EXPORT_CALLBACK_S vo_exp_callback;
        osal_memset(&vo_exp_callback, 0, sizeof(VOU_EXPORT_CALLBACK_S));

        if (g_vou_exp_symbol.pfnVoRegisterExpCallback != NULL) {
            vo_exp_callback.pfnVoNotify = hi_user_notify_vo_event;
            g_vou_exp_symbol.pfnVoRegisterExpCallback(&vo_exp_callback);
        }
    }
#endif

    return 0;
}

void __exit hi_user_exit(void)
{
#if (VDEC_NOTIFY || VDEC_SET_SCHEDULER)
    {
        VDEC_EXPORT_CALLBACK_S vdec_exp_callback;

        if (g_stVdecExpSymbol.pfnVdecRegisterExpCallback != NULL) {
            osal_memset(&vdec_exp_callback, 0, sizeof(VDEC_EXPORT_CALLBACK_S));
            g_stVdecExpSymbol.pfnVdecRegisterExpCallback(&vdec_exp_callback);
        }
    }
#endif

#if VO_NOTIFY
    {
        VOU_EXPORT_CALLBACK_S vo_exp_callback;

        if (g_vou_exp_symbol.pfnVoRegisterExpCallback != NULL) {
            osal_memset(&vo_exp_callback, 0, sizeof(VOU_EXPORT_CALLBACK_S));
            g_vou_exp_symbol.pfnVoRegisterExpCallback(&vo_exp_callback);
        }
    }
#endif

    return;
}

#ifndef __HuaweiLite__
module_init(hi_user_init);
module_exit(hi_user_exit);
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hisilicon");
