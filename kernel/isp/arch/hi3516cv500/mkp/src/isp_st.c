/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#include "hi_osal.h"
#include "isp.h"
#include "isp_ext.h"
#include "sys_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


/* isp sync task */
#define  ISP_SYNC_TSK_MAX_NODES  16

#define ISP_SYNC_TSK_MAX_NUM 200

#define ISP_SYNC_TSK_GET_CTX(dev) (&g_astIspSyncTskCtx[dev])

typedef void (*ISP_FUNC_PTR)(unsigned long data);

typedef enum hiISP_SYNC_TSK_TRIG_POS_E {
    TRIG_POS_FIRST_PIXEL = 1 << 0,
    TRIG_POS_LAST_PIXEL  = 1 << 1,
    TRIG_POS_VSYNC_SIGNAL = 1 << 2,

    TRIG_POS_BUTT
} ISP_SYNC_TSK_TRIG_POS_E;

typedef struct hiLIST_ENTRY_S {
    HI_U32 u32Num;
    struct osal_list_head head;

} LIST_ENTRY_S;

typedef struct hiISP_SYNC_TSK_CTX_S {
    VI_PIPE ViPipe;
    struct osal_work_struct worker;
    LIST_ENTRY_S hwirq_list;
    LIST_ENTRY_S tsklet_list;
    LIST_ENTRY_S workqueue_list;

    struct osal_semaphore sem;

} ISP_SYNC_TSK_CTX_S;

/* ----------------------------------------------*
 * external variables                           *
 * ---------------------------------------------- */

/* ----------------------------------------------*
 * external routine prototypes                  *
 * ---------------------------------------------- */

/* ----------------------------------------------*
 * internal routine prototypes                  *
 * ---------------------------------------------- */


/* ----------------------------------------------*
 * project-wide global variables                *
 * ---------------------------------------------- */

/* ----------------------------------------------*
 * module-wide global variables                 *
 * ---------------------------------------------- */
ISP_SYNC_TSK_CTX_S g_astIspSyncTskCtx[ISP_MAX_PIPE_NUM];
/* ----------------------------------------------*
 * constants                                    *
 * ---------------------------------------------- */

/* ----------------------------------------------*
 * macros                                       *
 * ---------------------------------------------- */

/* ----------------------------------------------*
 * routines' implementations                    *
 * ---------------------------------------------- */

/*****************************************************************************
 Prototype    : ispSyncTskFIndAndExecute
 Description  : find a task and exe it
 Input        : struct list_head *head
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2015/7/14
    Author       :
    Modification : Created function

*****************************************************************************/

HI_S32 ispSyncTskFIndAndExecute(VI_PIPE ViPipe, struct osal_list_head *head)
{
    struct osal_list_head *pos = HI_NULL;
    struct osal_list_head *next = HI_NULL;
    ISP_SYNC_TASK_NODE_S *pstSyncTskNode;

    if (!osal_list_empty(head)) {
        osal_list_for_each_safe(pos, next, head) {
            pstSyncTskNode = osal_list_entry(pos, ISP_SYNC_TASK_NODE_S, list);
            if (pstSyncTskNode->pstFocusStat) {
                ISP_DRV_StaKernelGet(ViPipe, pstSyncTskNode->pstFocusStat);
            }
            if (pstSyncTskNode->pfnIspSyncTskCallBack) {
                pstSyncTskNode->pfnIspSyncTskCallBack(pstSyncTskNode->u64Data);
            }

        }
    }

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : taskLetHandler
 Description  : a tasklet handler
 Input        : unsigned long data
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2015/7/14
    Author       :
    Modification : Created function

*****************************************************************************/

/*****************************************************************************
 Prototype    : workQueueHandler
 Description  : a workqueue handler
 Input        : struct work_struct *pstWorker
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2015/7/14
    Author       :
    Modification : Created function

*****************************************************************************/
static void workQueueHandler(struct osal_work_struct *pstWorker)
{

    ISP_SYNC_TSK_CTX_S *pstSyncTsk = osal_container_of((void *)pstWorker, ISP_SYNC_TSK_CTX_S, worker);

    if (osal_down_interruptible(&pstSyncTsk->sem)) {
        return;
    }

    ispSyncTskFIndAndExecute(pstSyncTsk->ViPipe, &pstSyncTsk->workqueue_list.head);

    osal_up(&pstSyncTsk->sem);

    return;
}

/*****************************************************************************
 Prototype    : IspSyncTaskProcess
 Description  : isp sync task process
*****************************************************************************/
HI_S32 IspSyncTaskProcess(VI_PIPE ViPipe)
{
    ISP_SYNC_TSK_CTX_S *pstSyncTsk = ISP_SYNC_TSK_GET_CTX(ViPipe);

    if (pstSyncTsk->hwirq_list.u32Num) {
        ispSyncTskFIndAndExecute(ViPipe, &pstSyncTsk->hwirq_list.head);
    }

    if (pstSyncTsk->tsklet_list.u32Num) {
    }

    if (pstSyncTsk->workqueue_list.u32Num) {
        osal_schedule_work(&pstSyncTsk->worker);
    }

    return HI_SUCCESS;

}

/*****************************************************************************
 Prototype    : searchNode
 Description  : seach a node by id
*****************************************************************************/
static struct osal_list_head *searchNode(struct osal_list_head *head, const char *id)
{
    struct osal_list_head *pos = HI_NULL;
    struct osal_list_head *next = HI_NULL;
    ISP_SYNC_TASK_NODE_S *pstSyncTskNode;

    osal_list_for_each_safe(pos, next, head) {
        pstSyncTskNode = osal_list_entry(pos, ISP_SYNC_TASK_NODE_S, list);
        if (!osal_strncmp(pstSyncTskNode->pszId, id, HI_ISP_SYNC_TASK_ID_MAX_LENGTH)) {
            return pos;
        }
    }

    return HI_NULL;
}

/*****************************************************************************
 Prototype    : hi_isp_sync_task_register
 Description  : register a task, export to other module
*****************************************************************************/
HI_S32 hi_isp_sync_task_register(VI_PIPE ViPipe, ISP_SYNC_TASK_NODE_S *pstNewNode)
{
    ISP_SYNC_TSK_CTX_S *pstSyncTsk = ISP_SYNC_TSK_GET_CTX(ViPipe);
    struct osal_list_head *pstTargetList, *pos;
    LIST_ENTRY_S *pstListEntry;

    ISP_CHECK_POINTER(pstNewNode);

    if (pstNewNode->enMethod == ISP_SYNC_TSK_METHOD_HW_IRQ) {
        pstTargetList = &pstSyncTsk->hwirq_list.head;
    } else {
        pstTargetList = &pstSyncTsk->workqueue_list.head;
    }

    pstListEntry = osal_list_entry(pstTargetList, LIST_ENTRY_S, head);

    pos = searchNode(pstTargetList, pstNewNode->pszId);
    if (pos) {
        return HI_FAILURE;
    }

    if (osal_down_interruptible(&pstSyncTsk->sem)) {
        return -ERESTARTSYS;
    }

    osal_list_add_tail(&pstNewNode->list, pstTargetList);

    if (pstListEntry->u32Num < ISP_SYNC_TSK_MAX_NUM) {
        pstListEntry->u32Num++;
    }

    osal_up(&pstSyncTsk->sem);

    return HI_SUCCESS;

}

/*****************************************************************************
 Prototype    : hi_isp_sync_task_unregister
 Description  : unregister a task export to other module
*****************************************************************************/
HI_S32 hi_isp_sync_task_unregister(VI_PIPE ViPipe, ISP_SYNC_TASK_NODE_S *pstDelNode)
{
    ISP_SYNC_TSK_CTX_S *pstSyncTsk = ISP_SYNC_TSK_GET_CTX(ViPipe);
    struct osal_list_head *pstTargetList;
    LIST_ENTRY_S *pstListEntry;
    struct osal_list_head *pos;
    HI_S32 bDelSuccess = HI_FAILURE;


    ISP_CHECK_POINTER(pstDelNode);

    if (pstDelNode->enMethod == ISP_SYNC_TSK_METHOD_HW_IRQ) {
        pstTargetList = &pstSyncTsk->hwirq_list.head;
    } else {
        pstTargetList = &pstSyncTsk->workqueue_list.head;
    }
    pstListEntry = osal_list_entry(pstTargetList, LIST_ENTRY_S, head);

    if (osal_down_interruptible(&pstSyncTsk->sem)) {
        return -ERESTARTSYS;
    }

    pos = searchNode(pstTargetList, pstDelNode->pszId);

    if (pos) {
        osal_list_del(pos);
        if (pstListEntry->u32Num > 0) {
            pstListEntry->u32Num = pstListEntry->u32Num - 1;
        }

        bDelSuccess = HI_SUCCESS;
    }

    osal_up(&pstSyncTsk->sem);

    return bDelSuccess;

}

void SyncTaskInit(VI_PIPE ViPipe)
{
    ISP_SYNC_TSK_CTX_S *pstSyncTsk = ISP_SYNC_TSK_GET_CTX(ViPipe);

    OSAL_INIT_LIST_HEAD(&pstSyncTsk->hwirq_list.head);
    OSAL_INIT_LIST_HEAD(&pstSyncTsk->tsklet_list.head);
    OSAL_INIT_LIST_HEAD(&pstSyncTsk->workqueue_list.head);

    pstSyncTsk->hwirq_list.u32Num     = 0;
    pstSyncTsk->tsklet_list.u32Num    = 0;
    pstSyncTsk->workqueue_list.u32Num = 0;
    osal_sema_init(&pstSyncTsk->sem, 1);

    OSAL_INIT_WORK(&pstSyncTsk->worker, workQueueHandler);

    return;
}

void SyncTaskExit(VI_PIPE ViPipe)
{
    ISP_SYNC_TSK_CTX_S *pstSyncTsk = ISP_SYNC_TSK_GET_CTX(ViPipe);

    osal_destroy_work(&pstSyncTsk->worker);
    osal_sema_destory(&pstSyncTsk->sem);

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
