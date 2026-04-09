/*
* Copyright (C) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */
#include "hi_osal.h"
#include "isp_list.h"


HI_S32 ISP_CreatBeBufQueue(ISP_BE_BUF_QUEUE_S *pstQueue, HI_U32 u32MaxNum)
{
    HI_U32 i;
    unsigned long flags;

    ISP_BE_BUF_NODE_S *pstNode;

    osal_spin_lock_init(&pstQueue->stSpinLock);

    /* Malloc node buffer(cached) */
    pstQueue->pstNodeBuf = osal_kmalloc(u32MaxNum * sizeof(ISP_BE_BUF_NODE_S), osal_gfp_kernel);

    if (pstQueue->pstNodeBuf == HI_NULL) {
        return HI_FAILURE;
    }

    osal_spin_lock_irqsave(&pstQueue->stSpinLock, &flags);

    /* Init list head */
    OSAL_INIT_LIST_HEAD(&pstQueue->stFreeList);
    OSAL_INIT_LIST_HEAD(&pstQueue->stBusyList);

    /* The node buffer add freelist tail */
    pstNode = pstQueue->pstNodeBuf;

    for (i = 0; i < u32MaxNum; i++) {
        osal_list_add_tail(&pstNode->list, &pstQueue->stFreeList);
        pstNode++;

    }

    /* init queue others parameters */
    pstQueue->u32FreeNum = u32MaxNum;
    pstQueue->u32MaxNum = u32MaxNum;
    pstQueue->u32BusyNum = 0;

    osal_spin_unlock_irqrestore(&pstQueue->stSpinLock, &flags);

    return HI_SUCCESS;
}

HI_VOID ISP_DestroyBeBufQueue(ISP_BE_BUF_QUEUE_S *pstQueue)
{
    unsigned long flags;

    osal_spin_lock_irqsave(&pstQueue->stSpinLock, &flags);

    /* Init list head */
    OSAL_INIT_LIST_HEAD(&pstQueue->stFreeList);
    OSAL_INIT_LIST_HEAD(&pstQueue->stBusyList);

    pstQueue->u32FreeNum = 0;
    pstQueue->u32BusyNum = 0;
    pstQueue->u32MaxNum = 0;

    osal_spin_unlock_irqrestore(&pstQueue->stSpinLock, &flags);

    osal_spin_lock_destory(&pstQueue->stSpinLock);

    if (pstQueue->pstNodeBuf) {
        osal_kfree(pstQueue->pstNodeBuf);
        pstQueue->pstNodeBuf = HI_NULL;
    }

    return;
}

ISP_BE_BUF_NODE_S *ISP_QueueGetFreeBeBuf(ISP_BE_BUF_QUEUE_S *pstQueue)
{
    unsigned long flags;
    struct osal_list_head *plist;
    ISP_BE_BUF_NODE_S *pstNode = HI_NULL;

    osal_spin_lock_irqsave(&pstQueue->stSpinLock, &flags);

    if (osal_list_empty(&pstQueue->stFreeList)) {
        osal_spin_unlock_irqrestore(&pstQueue->stSpinLock, &flags);

        return HI_NULL;
    }

    plist = pstQueue->stFreeList.next;
    osal_list_del(plist);

    pstNode = osal_list_entry(plist, ISP_BE_BUF_NODE_S, list);

    pstQueue->u32FreeNum--;
    osal_spin_unlock_irqrestore(&pstQueue->stSpinLock, &flags);

    return pstNode;
}

ISP_BE_BUF_NODE_S *ISP_QueueGetFreeBeBufTail(ISP_BE_BUF_QUEUE_S *pstQueue)
{
    unsigned long flags;

    struct osal_list_head *plist;
    ISP_BE_BUF_NODE_S *pstNode = HI_NULL;

    osal_spin_lock_irqsave(&pstQueue->stSpinLock, &flags);

    if (osal_list_empty(&pstQueue->stFreeList)) {
        osal_spin_unlock_irqrestore(&pstQueue->stSpinLock, &flags);

        return HI_NULL;
    }

    plist = pstQueue->stFreeList.prev;

    if (plist == HI_NULL) {
        osal_spin_unlock_irqrestore(&pstQueue->stSpinLock, &flags);

        return HI_NULL;
    }

    osal_list_del(plist);
    pstNode = osal_list_entry(plist, ISP_BE_BUF_NODE_S, list);

    pstQueue->u32FreeNum--;

    osal_spin_unlock_irqrestore(&pstQueue->stSpinLock, &flags);

    return pstNode;
}

HI_VOID  ISP_QueuePutBusyBeBuf(ISP_BE_BUF_QUEUE_S *pstQueue, ISP_BE_BUF_NODE_S  *pstNode)
{
    unsigned long flags;

    osal_spin_lock_irqsave(&pstQueue->stSpinLock, &flags);

    osal_list_add_tail(&pstNode->list, &pstQueue->stBusyList);
    pstQueue->u32BusyNum++;

    osal_spin_unlock_irqrestore(&pstQueue->stSpinLock, &flags);
    return;
}

ISP_BE_BUF_NODE_S *ISP_QueueGetBusyBeBuf(ISP_BE_BUF_QUEUE_S *pstQueue)
{
    struct osal_list_head *plist;
    unsigned long flags;
    ISP_BE_BUF_NODE_S *pstNode = HI_NULL;

    osal_spin_lock_irqsave(&pstQueue->stSpinLock, &flags);

    if (osal_list_empty(&pstQueue->stBusyList)) {
        osal_spin_unlock_irqrestore(&pstQueue->stSpinLock, &flags);

        return HI_NULL;
    }

    plist = pstQueue->stBusyList.next;
    osal_list_del(plist);

    pstNode = osal_list_entry(plist, ISP_BE_BUF_NODE_S, list);

    pstQueue->u32BusyNum--;

    osal_spin_unlock_irqrestore(&pstQueue->stSpinLock, &flags);
    return pstNode;
}

HI_VOID ISP_QueueDelBusyBeBuf(ISP_BE_BUF_QUEUE_S *pstQueue, ISP_BE_BUF_NODE_S  *pstNode)
{
    unsigned long flags;

    osal_spin_lock_irqsave(&pstQueue->stSpinLock, &flags);

    if (osal_list_empty(&pstQueue->stBusyList)) {
        osal_spin_unlock_irqrestore(&pstQueue->stSpinLock, &flags);

        return;
    }

    osal_list_del(&pstNode->list);

    pstQueue->u32BusyNum--;

    osal_spin_unlock_irqrestore(&pstQueue->stSpinLock, &flags);
}


ISP_BE_BUF_NODE_S *ISP_QueueQueryBusyBeBuf(ISP_BE_BUF_QUEUE_S *pstQueue)
{
    unsigned long flags;
    struct osal_list_head *plist;
    ISP_BE_BUF_NODE_S *pstNode = HI_NULL;

    osal_spin_lock_irqsave(&pstQueue->stSpinLock, &flags);

    if (osal_list_empty(&pstQueue->stBusyList)) {
        osal_spin_unlock_irqrestore(&pstQueue->stSpinLock, &flags);

        return HI_NULL;
    }

    plist = pstQueue->stBusyList.prev;

    if (plist == HI_NULL) {
        osal_spin_unlock_irqrestore(&pstQueue->stSpinLock, &flags);

        return HI_NULL;
    }

    pstNode = osal_list_entry(plist, ISP_BE_BUF_NODE_S, list);
    osal_spin_unlock_irqrestore(&pstQueue->stSpinLock, &flags);

    return pstNode;
}


HI_VOID ISP_QueuePutFreeBeBuf(ISP_BE_BUF_QUEUE_S *pstQueue, ISP_BE_BUF_NODE_S *pstNode)
{
    unsigned long flags;

    osal_spin_lock_irqsave(&pstQueue->stSpinLock, &flags);

    osal_list_add_tail(&pstNode->list, &pstQueue->stFreeList);
    pstQueue->u32FreeNum++;

    osal_spin_unlock_irqrestore(&pstQueue->stSpinLock, &flags);
    return;
}


HI_U32 ISP_QueueGetFreeNum(ISP_BE_BUF_QUEUE_S *pstQueue)
{
    HI_U32 u32FreeNum;
    unsigned long flags;

    osal_spin_lock_irqsave(&pstQueue->stSpinLock, &flags);

    u32FreeNum = pstQueue->u32FreeNum;

    osal_spin_unlock_irqrestore(&pstQueue->stSpinLock, &flags);
    return u32FreeNum;
}

HI_U32 ISP_QueueGetBusyNum(ISP_BE_BUF_QUEUE_S *pstQueue)
{
    HI_U32 u32BusyNum;
    unsigned long flags;

    osal_spin_lock_irqsave(&pstQueue->stSpinLock, &flags);

    u32BusyNum = pstQueue->u32BusyNum;

    osal_spin_unlock_irqrestore(&pstQueue->stSpinLock, &flags);
    return u32BusyNum;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
