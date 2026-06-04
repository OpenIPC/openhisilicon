/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include "ot_user.h"
#include "securec.h"

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

#if VENC_EX_RC
#define  START_QP_I  30
#define  START_QP_P  40
#define  START_QP_B  50
#define  START_QP_SP 37
#define  START_QP_VI 34
#define  START_QP_DELTA 2
#define  START_QP_REENCODE 51

td_s32 venc_export_rc_init(td_s32 chn, const ot_venc_export_rc_attr *user_rc_attr)
{
    return 0;
}
td_s32 venc_export_rc_deinit(td_s32 chn)
{
    return 0;
}

td_s32 venc_export_rc_reset(td_s32 chn)
{
    return 0;
}
td_s32 venc_export_rc_change(td_s32 chn, const ot_venc_export_rc_attr *user_rc_attr)
{
    return 0;
}

td_s32 venc_user_rc_calc(td_s32 chn, const ot_venc_export_rc_frame_info *rc_frame_info,
    ot_venc_export_rc_enc_info *rc_enc_info)
{
    rc_enc_info->target_bits = 1024000; /* 1024000 bit */
    switch (rc_frame_info->cur_frame_type) {
        case VENC_EXPORT_FRAME_TYPE_I:
            rc_enc_info->start_qp = START_QP_I;
            break;
        case VENC_EXPORT_FRAME_TYPE_P:
            rc_enc_info->start_qp = START_QP_P;
            break;
        case VENC_EXPORT_FRAME_TYPE_B:
            rc_enc_info->start_qp = START_QP_B;
            break;
        case VENC_EXPORT_FRAME_TYPE_SP:
            rc_enc_info->start_qp = START_QP_SP;
            break;
        case VENC_EXPORT_FRAME_TYPE_VI:
            rc_enc_info->start_qp = START_QP_VI;
            break;
        default:
            break;
    }

    rc_enc_info->min_qp = rc_enc_info->start_qp - START_QP_DELTA;
    rc_enc_info->max_qp = rc_enc_info->start_qp + START_QP_DELTA;

    if (rc_frame_info->last_frame_status == VENC_EXPORT_FRAME_STATUS_REENCODE) {
        rc_enc_info->max_qp = START_QP_REENCODE;
        rc_enc_info->min_qp = START_QP_REENCODE;
        rc_enc_info->start_qp = START_QP_REENCODE;
    }

    return 0;
}
#endif

int __init ot_user_init(void)
{
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

#if VENC_EX_RC
    {
        td_s32 venc_chn = 0;
        ot_venc_export_callback venc_exp_callback;
        ot_venc_export_symbol *venc_export_symbol = ot_venc_get_export_symbol();

        (void)memset_s(&venc_exp_callback, sizeof(ot_venc_export_callback), 0, sizeof(ot_venc_export_callback));
        if ((venc_export_symbol != NULL) && (venc_export_symbol->register_export_callback != NULL)) {
            venc_exp_callback.venc_export_rc_init = venc_export_rc_init;
            venc_exp_callback.venc_export_rc_deinit = venc_export_rc_deinit;
            venc_exp_callback.venc_export_rc_change = venc_export_rc_change;
            venc_exp_callback.venc_export_rc_reset = venc_export_rc_reset;
            venc_exp_callback.venc_export_rc_calc = venc_user_rc_calc;
            venc_export_symbol->register_export_callback(venc_chn, &venc_exp_callback);
        }
    }
#endif

    printk("load ot_user ....OK!\n");
    return 0;
}

void __exit ot_user_exit(void)
{
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

#if VENC_EX_RC
    {
        td_s32 venc_chn = 0;
        ot_venc_export_callback venc_exp_callback;
        ot_venc_export_symbol *venc_export_symbol = ot_venc_get_export_symbol();

        (void)memset_s(&venc_exp_callback, sizeof(ot_venc_export_callback), 0, sizeof(ot_venc_export_callback));
        if ((venc_export_symbol != NULL) && (venc_export_symbol->register_export_callback != NULL)) {
            venc_exp_callback.venc_export_rc_init = NULL;
            venc_exp_callback.venc_export_rc_deinit = NULL;
            venc_exp_callback.venc_export_rc_change = NULL;
            venc_exp_callback.venc_export_rc_reset = NULL;
            venc_exp_callback.venc_export_rc_calc = NULL;
            venc_export_symbol->register_export_callback(venc_chn, &venc_exp_callback);
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
