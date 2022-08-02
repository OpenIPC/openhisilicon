/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __LITEOS__

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>

#include <asm/uaccess.h>
#include <asm/io.h>

#include "sample_ist.h"
#include "common.h"
#include "isp_ext.h"

#define MAX_TEST_NODES 4

GK_S32 sync_call_back(GK_U64 u64Data);
GK_S32 sync_af_calc(GK_U64 u64Data);

VI_PIPE ViPipe = 0;
ISP_DRV_AF_STATISTICS_S         stFocusStat;

ISP_SYNC_TASK_NODE_S syncNode[MAX_TEST_NODES] = {
    {
        .enMethod = ISP_SYNC_TSK_METHOD_HW_IRQ,
        .pfnIspSyncTskCallBack = sync_af_calc,
        .u64Data = 0,
        .pstFocusStat = &stFocusStat,
        .pszId = "hw_0"
    },
    {
        .enMethod = ISP_SYNC_TSK_METHOD_HW_IRQ,
        .pfnIspSyncTskCallBack = sync_call_back,
        .u64Data = 1,
        .pstFocusStat = NULL,
        .pszId = "hw_1"
    },
    {
        .enMethod = ISP_SYNC_TSK_METHOD_WORKQUE,
        .pfnIspSyncTskCallBack = sync_call_back,
        .u64Data = 3,
        .pstFocusStat = NULL,
        .pszId = "wq_0"
    },
    {
        .enMethod = ISP_SYNC_TSK_METHOD_WORKQUE,
        .pfnIspSyncTskCallBack = sync_call_back,
        .u64Data = 4,
        .pstFocusStat = NULL,
        .pszId = "wq_1"
    }
};

GK_S32 sync_af_calc(GK_U64 u64Data)
{
    int i, j;
    static int cnt = 0;
    GK_U16 u16StatData;
    ISP_DRV_BE_FOCUS_STATISTICS_S  *pstIspFocusSt = NULL;
    pstIspFocusSt = &stFocusStat.stBEAFStat;
    /* get af statistics */
    if (cnt++ % 30 == 0) {
        printk("h1:\n");
        for (i = 0; i < 15; i++) {
            for (j = 0; j < 17; j++) {
                u16StatData = pstIspFocusSt->stZoneMetrics[i][j].u16h1;
                printk("%6d", u16StatData);
            }
            printk("\n");
        }
    }

    /* af algorithm */

    return 0;
}

GK_S32 sync_call_back(GK_U64 u64Data)
{
    int data = u64Data;

    printk("%d\n", data);
    return 0;
}

/* file operation */
int SampleIst_Open(struct inode *inode, struct file *file)
{
    return 0;

}

int SampleIst_Close(struct inode *inode, struct file *file)
{
    return 0;
}

static long SampleIst_Ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int __user *argp = (int __user *)(GK_UINTPTR_T)arg;
    int node_index = *argp;

    if (node_index >= MAX_TEST_NODES) {
        return -1;
    }

    switch (cmd) {
        case SAMPLE_IST_ADD_NODE:
            if (CKFN_ISP_RegisterSyncTask()) {
                CALL_ISP_RegisterSyncTask(ViPipe, &syncNode[node_index]);
            } else {
                printk("register sample_ist failed!\n");
                return -1;
            }
            break;

        case SAMPLE_IST_DEL_NODE:
            if (CKFN_ISP_UnRegisterSyncTask()) {
                if (CALL_ISP_UnRegisterSyncTask(ViPipe, &syncNode[node_index]) == GK_FAILURE) {
                    printk("del node err %d\n", node_index);
                }
            } else {
                printk("unregister sample_ist failed!\n");
                return -1;
            }
            break;

        default: {
            printk("invalid ioctl command!\n");
            return -ENOIOCTLCMD;
        }
    }

    return 0;
}

static struct file_operations sample_ist_fops = {
    .owner      = THIS_MODULE,
    .unlocked_ioctl = SampleIst_Ioctl,
    .open       = SampleIst_Open,
    .release    = SampleIst_Close,
};

static struct miscdevice sample_ist_dev = {
    .minor   = MISC_DYNAMIC_MINOR,
    .name    = "sample_ist",
    .fops    = &sample_ist_fops,
};


/* module init and exit */
static int __init sample_ist_init(void)
{
    int  i, ret;

    ret = misc_register(&sample_ist_dev);
    if (ret != 0) {
        printk("register sample_ist device failed with %#x!\n", ret);
        return -1;
    }

    for (i = 0; i < MAX_TEST_NODES; i++) {
        if (CKFN_ISP_RegisterSyncTask()) {
            CALL_ISP_RegisterSyncTask(ViPipe, &syncNode[i]);
        }

    }

    return 0;
}

static void __exit sample_ist_exit(void)
{
    int i;

    misc_deregister(&sample_ist_dev);

    for (i = 0; i < MAX_TEST_NODES; i++) {
        if (CKFN_ISP_UnRegisterSyncTask()) {
            CALL_ISP_UnRegisterSyncTask(ViPipe, &syncNode[i]);
        }
    }
}


module_init(sample_ist_init);
module_exit(sample_ist_exit);

MODULE_LICENSE("GPL");

#endif
