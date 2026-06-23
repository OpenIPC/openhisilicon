/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_user.h"
#include "securec.h"

#if VDEC_NOTIFY

/*
 * event notification from vendor SDK.
 * @param[in] module_id     module id. (MOD_ID_E)
 * @param[in] channel       device channel number.
 * @param[in] event_type    event type.
 *   ex) types of VDEC event
 *         - OT_VDEC_EVENT_STREAM_ERR = 1,
 *         - OT_VDEC_EVENT_UNSUPPORT,
 *         - OT_VDEC_EVENT_ERR_THRESHOLD_OVER,
 *         - OT_VDEC_EVENT_REF_NUM_OVER,
 *         - OT_VDEC_EVENT_SLICE_NUM_OVER,
 *         - OT_VDEC_EVENT_SPS_NUM_OVER,
 *         - OT_VDEC_EVENT_PPS_NUM_OVER,
 *         - OT_VDEC_EVENT_PIC_BUF_SIZE_ERR,
 *         - OT_VDEC_EVENT_SIZE_OVER,
 *         - OT_VDEC_EVENT_FRAME_SIZE_CHG,
           - OT_VDEC_EVENT_VPS_NUM_OVER
 *         - OT_VDEC_EVENT_BUTT
 * @param[in] args          additional arguments.
 */
static void ot_user_notify_vdec_event(int mod_id, int chn, int event_type, void* args)
{
    /* eg:code int *event_arg = (int *)args; */
    switch (event_type) {
        case 1: { /* 1:OT_VDEC_EVENT_STREAM_ERR: stream package error. */
                /* event_arg is NULL. */
                break;
            }

        case 2: { /* 2:OT_VDEC_EVENT_UNSUPPORT: unsupported the stream specification. */
                /* event_arg is NULL. */
                break;
            }

        case 3: { /* 3:OT_VDEC_EVENT_ERR_THRESHOLD_OVER: stream error rate is larger than err_threshold. */
                /* event_arg[0] is the stream error rate of current frame. */
                break;
            }

        case 4: { /* 4:OT_VDEC_EVENT_REF_NUM_OVER: reference num is not enough. */
                /* event_arg[0] is reference num of current frame. */
                break;
            }

        case 5: { /* 5:OT_VDEC_EVENT_SLICE_NUM_OVER: slice num is not enough. */
                /* event_arg is NULL. */
                break;
            }

        case 6: { /* 6:OT_VDEC_EVENT_SPS_NUM_OVER: sps num is not enough. */
                /* event_arg is NULL. */
                break;
            }

        case 7: { /* 7:OT_VDEC_EVENT_PPS_NUM_OVER: pps num is not enough. */
                /* event_arg is NULL. */
                break;
            }

        case 8: { /* 8:OT_VDEC_EVENT_PIC_BUF_SIZE_ERR: the picture buffer size is not enough. */
                /* event_arg is NULL. */
                break;
            }

        case 9: { /* 9:OT_VDEC_EVENT_SIZE_OVER: picture width or height is larger than channel width or height */
                /* event_arg[0] is width, event_arg[1] is height of current frame. */
                break;
            }

        case 10: { /* 10:OT_VDEC_EVENT_FRAME_SIZE_CHG: the width or height of stream change only for H264/H265 */
                /* event_arg[0] is width, event_arg[1] is height of current frame. */
                break;
            }

        case 11: { /* 11:OT_VDEC_EVENT_VPS_NUM_OVER: vps num is not enough. */
                /* event_arg is NULL. */
                break;
            }
        default: {
                break;
            }
    }
}

#endif

#if VO_NOTIFY

static void ot_user_notify_vo_event(int mod_id, int vo_dev)
{
    switch (vo_dev) {
        case 0: { /* 0 device */
            /* do something */
            break;
        }
        default: {
            break;
        }
    }
}

static void ot_user_notify_vo_user_event(int mod_id, int vo_dev)
{
    switch (vo_dev) {
        case 0: { /* 0 device */
            /* do something */
            break;
        }
        default: {
            break;
        }
    }
}

#endif

#if AO_NOTIFY

static void ot_user_notify_ao_event(int ao_dev)
{
    switch (ao_dev) {
        case 0: { /* 0 device */
            /* do something */
            break;
        }
        case 1: { /* 1 device */
            /* do something */
            break;
        }
        default: {
            break;
        }
    }
}

#endif

int __init ot_user_init(void)
{
#if VDEC_NOTIFY
    {
        ot_vdec_export_callback vdec_exp_callback;
        ot_vdec_export_symbol *vdec_export_symbol = ot_vdec_get_export_symbol();

        (void)memset_s(&vdec_exp_callback, sizeof(ot_vdec_export_callback), 0, sizeof(ot_vdec_export_callback));
        if ((vdec_export_symbol != NULL) && (vdec_export_symbol->register_export_callback != NULL)) {
            vdec_exp_callback.vdec_notify = ot_user_notify_vdec_event;
            vdec_export_symbol->register_export_callback(&vdec_exp_callback);
        }
    }
#endif

#if VO_NOTIFY
    {
        ot_vo_export_symbol *vo_export_symbol = NULL;
        ot_vo_export_callback vo_export_callback;

        vo_export_symbol = ot_vo_get_export_symbol();
        (void)memset_s(&vo_export_callback, sizeof(ot_vo_export_callback), 0, sizeof(ot_vo_export_callback));
        if ((vo_export_symbol != NULL) && (vo_export_symbol->vo_register_export_callback != NULL)) {
            vo_export_callback.vo_refresh_disp_notify = ot_user_notify_vo_event;
            vo_export_callback.vo_user_notify = ot_user_notify_vo_user_event;
            vo_export_symbol->vo_register_export_callback(&vo_export_callback);
        }
    }
#endif

#if AO_NOTIFY
    {
        ot_ao_export_symbol *ao_export_symbol = NULL;
        ot_ao_export_callback ao_export_callback;
        (void)memset_s(&ao_export_callback, sizeof(ot_ao_export_callback), 0, sizeof(ot_ao_export_callback));

        ao_export_symbol = ot_ao_get_export_symbol();
        if (ao_export_symbol != NULL) {
            if (ao_export_symbol->register_export_callback != NULL) {
                ao_export_callback.ao_notify = ot_user_notify_ao_event;
                ao_export_symbol->register_export_callback(&ao_export_callback);
            }
        }
    }
#endif
    printk("load ot_user ....OK!\n");
    return 0;
}

void __exit ot_user_exit(void)
{
#if VDEC_NOTIFY
    {
        ot_vdec_export_callback vdec_exp_callback;
        ot_vdec_export_symbol *vdec_export_symbol = ot_vdec_get_export_symbol();

        if ((vdec_export_symbol != NULL) && (vdec_export_symbol->register_export_callback != NULL)) {
            (void)memset_s(&vdec_exp_callback, sizeof(ot_vdec_export_callback), 0, sizeof(ot_vdec_export_callback));
            vdec_export_symbol->register_export_callback(&vdec_exp_callback);
        }
    }
#endif

#if VO_NOTIFY
    {
        ot_vo_export_symbol *vo_export_symbol = NULL;
        ot_vo_export_callback vo_export_callback;

        vo_export_symbol = ot_vo_get_export_symbol();
        if ((vo_export_symbol != NULL) && (vo_export_symbol->vo_register_export_callback != NULL)) {
            (void)memset_s(&vo_export_callback, sizeof(ot_vo_export_callback), 0, sizeof(ot_vo_export_callback));
            vo_export_symbol->vo_register_export_callback(&vo_export_callback);
        }
    }
#endif

#if AO_NOTIFY
    {
        ot_ao_export_symbol *ao_export_symbol = NULL;
        ot_ao_export_callback ao_export_callback;

        ao_export_symbol = ot_ao_get_export_symbol();
        if (ao_export_symbol != NULL) {
            if (ao_export_symbol->register_export_callback != NULL) {
                (void)memset_s(&ao_export_callback, sizeof(ot_ao_export_callback), 0, sizeof(ot_ao_export_callback));
                ao_export_symbol->register_export_callback(&ao_export_callback);
            }
        }
    }
#endif

    printk("unload ot_user ....OK!\n");
}

#ifndef __LITEOS__
module_init(ot_user_init);
module_exit(ot_user_exit);
#endif

MODULE_LICENSE("GPL");
