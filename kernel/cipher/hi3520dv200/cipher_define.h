#ifndef __CIPHER_DEFINE_H__
#define __CIPHER_DEFINE_H__

#include <linux/version.h>

#define __CIPHER_DEBUG_VERSION__ /* when release cipher SW, mask it */

#define CIPHER_WAIT_IDEL_TIMES 1000

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,14)
#define  SA_INTERRUPT     IRQF_DISABLED
#define  SA_SHIRQ         IRQF_SHARED
#endif


#define TASK_QUEUE_IDLE 0
#define TASK_QUEUE_BUSY 1

#define MEM_LENGTH_ERROR    1
#define AHB_BUS_ERROR       2

/*
typedef struct hiCIPHER_TASK_S
{
    struct list_head node;
    CIPHER_CTRL_S      ctrl;
    CIPHER_KEYIVIN_U   key_ivin;
    CIPHER_INFO_S      info;
    funcCipherCallback func;
    HI_VOID*           args;
} CIPHER_TASK_S;

typedef struct hiCIPHER_HANDLE_RES_S
{
    HI_U32 handle_id;
    struct list_head node;
    CIPHER_CTRL_S    ctrl;
    CIPHER_KEYIVIN_U key_ivin;
} CIPHER_HANDLE_RES_S;

*/


#define CI_MAX_LIST_NUM  128

#define CI_IV_SIZE       16  /* max 128bit */
#define CI_KEY_SIZE      32  /* max 256bit */


#define CI_BUF_LIST_FLAG_IVSET_BIT  21
#define CI_BUF_LIST_FLAG_EOL_BIT    22

typedef struct hiCI_BUF_LIST_ENTRY_S
{
    HI_U32  u32DataAddr;    /* Phy Addr */
    HI_U32  u32Flags;       /* bit21: iv_set_flag, bit22: end_of_list_flag */
    HI_U32  U32DataLen;     /* bit0~bit19, total 20 bit */
    HI_U32  u32IVStartAddr; /* if iv_set_flag == 1, this is the IV addr */
} CI_BUF_LIST_ENTRY_S;


/***/
typedef struct hiHI_UNF_CIPHER_INFO_S
{
    HI_U32  u32src;          /**< */
    HI_U32  u32dest;         /**< */
    HI_U32  u32length;       /**< */
    HI_BOOL bDecrypt;        /**< */
    HI_U32  u32DataPkg[4];   /**< */
}HI_UNF_CIPHER_INFO_S;

#endif


