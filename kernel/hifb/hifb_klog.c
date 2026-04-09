#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <asm/fcntl.h>
#include <hifb_proc.h>
#include <linux/slab.h>
#include "hifb_proc.h"
#include "higo_log_type.h"

#include "hi_debug.h"
#include "hi_osal.h"


/* define debug level */

#ifdef HI_TRACE1
#define LOG_TRACE(fmt...)  HI_TRACE(HI_LOG_LEVEL_ERROR, HI_DEBUG_ID_FB, fmt)
#define LOG_ERROR(fmt...)  HI_TRACE(HI_LOG_LEVEL_ERROR, HI_DEBUG_ID_FB, fmt)
#else
#define LOG_TRACE(fmt...)  printk(fmt)
#define LOG_ERROR(fmt...)  printk(fmt)
#endif

static HI_U32 s_u32LogMemPhyAddr = 0;
static HI_U8* s_pu8LogMemKVirtAddr = 0;

#define TABLE_NUM(x) (sizeof(x)/sizeof(*(x)))

/** sys log memory base addr */

/** layer log memory base addr */
#define LOG_KLAYER_START_PTR   (HIGO_LOG_LAYER_S*)((HI_U8*)s_pu8LogMemKVirtAddr + sizeof(HIGO_LOG_SYS_S))

/** memory log memory base addr */
#define LOG_KMEM_START_PTR     ((HIGO_LOG_MEM_S*)((HI_U8*)s_pu8LogMemKVirtAddr + LOG_MEM_OFFSET))

/** surface log memory base addr */
#define LOG_KSURFACE_START_PTR ((HIGO_LOG_SURFACE_S*)((HI_U8*)s_pu8LogMemKVirtAddr + LOG_SURFACE_OFFSET))

HI_U64 hifb_buf_allocmem(const HI_CHAR *pBufName, HI_UL ulLayerSize, const HI_CHAR *pszMmzName);
HI_VOID *hifb_buf_map(HI_U64 u64PhyAddr, HI_U32 u32Size);
HI_S32 hifb_buf_ummap(HI_VOID *pViraddr);
HI_VOID hifb_buf_freemem(HI_U32 u32Phyaddr);

#ifndef DISABLE_DEBUG_INFO
const HI_CHAR* s_pszFormat[] = {  "HIGO_PF_CLUT8",
                                "HIGO_PF_CLUT1",
                                "HIGO_PF_CLUT4",
                                "HIGO_PF_4444",
                                "HIGO_PF_0444",
                                "HIGO_PF_1555",
                                "HIGO_PF_0555",
                                "HIGO_PF_565",
                                "HIGO_PF_8565",
                                "HIGO_PF_8888",
                                "HIGO_PF_0888",
                                "HIGO_PF_YUV400",
                                "HIGO_PF_YUV420",
                                "HIGO_PF_YUV422",
                                "HIGO_PF_YUV422_V",
                                "HIGO_PF_YUV444",
                                "HIGO_PF_A1",
                                "HIGO_PF_A8",
                                "unkown"};

static inline HI_BOOL Log_NeedProc(HI_VOID)
{
    HIGO_LOG_SYS_S* pLogSysInfo = (HIGO_LOG_SYS_S*)s_pu8LogMemKVirtAddr;
    if (s_pu8LogMemKVirtAddr == NULL) {
        return HI_FALSE;
    }

    if (!(pLogSysInfo->eCtl & HIGO_LOG_CTL_ENABLE_PROC) ||
        ! (pLogSysInfo->eCtl & HIGO_LOG_CTL_HIGO_RUN))
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

struct seq_file* Log_PrintLayerInfo(struct seq_file* p)
{

    HI_S32 i;
    HIGO_LOG_LAYER_S* pLayer = LOG_KLAYER_START_PTR;
    HI_U32 u32LayerId;
    const HI_CHAR* pszLayer[] = {
                               "HIGO_LAYER_SD_0",
                               "HIGO_LAYER_SD_1",
                               "HIGO_LAYER_HD_0",
                               "HIGO_LAYER_HD_1",
                               "HIGO_LAYER_AD_0",
                               "Unkown"};

    const HI_CHAR* pszBufMode[] = { "SINGLE", "DOUBLE", "TRIPPLE", "UNKOWN"};
    const HI_CHAR* pBufMode  = NULL;
    const HI_CHAR* pszAntiFlickerLevel[] = {"NONE", "LOW","MIDDLE","HIGH","AUTO",
                                            "UNKOWN"};
    if ((p == NULL) || (s_pu8LogMemKVirtAddr == NULL)) {
        return NULL;
    }

    if (!Log_NeedProc()) {
        return p;
    }

    seq_printf(p, "\n----------------------- layer inforamtion -------------- \n");

    for ( i = 0 ; i < HIGO_LOG_MAX_LAYER_COUNT; i++ )
    {
        if (pLayer->pLayer)
        {
            HI_U32 u32PixelFormat;
            u32LayerId = pLayer->LayerID;
            if (u32LayerId >= TABLE_NUM(pszLayer))
            {
                u32LayerId = TABLE_NUM(pszLayer) - 1;
            }

            u32PixelFormat = pLayer->PixelFormat;
            if (u32PixelFormat >= TABLE_NUM(s_pszFormat))
            {
                u32PixelFormat = TABLE_NUM(s_pszFormat) - 1;
            }

            if (pLayer->LayerFlushType == 0x02)
            {
                pBufMode = pszBufMode[0];

            }
            else if (pLayer->LayerFlushType == 0x04)
            {
                pBufMode = pszBufMode[1];

            }
            else if (pLayer->LayerFlushType == 0x08)
            {
                pBufMode = pszBufMode[2];

            }
            else
            {
                pBufMode = pszBufMode[3];
            }

            seq_printf(p,
                               "layer id              : %s \n"
                               "pixel format          : %s \n"
                               "buffer type           : %s \n"
                               "created               : %s \n"
                               "default canvas surface: %s \n"
                               "user canvas surface   : %s \n"
                               "antiflicer level      : %s \n"
                               "canvas surface (w, h) : %d, %d \n"
                               "display surface (w, h): %d, %d \n"
                               "screen surface (w, h) : %d, %d \n",
                            pszLayer[u32LayerId],
                            s_pszFormat[u32PixelFormat],
                            pBufMode,
                            pLayer->bCreated ? "yes": "no",
                            pLayer->bDefSurface ? "yes": "no",
                            pLayer->bUsrCanSurface ? "yes" : "no",
                            pszAntiFlickerLevel[pLayer->AntiLevel],
                            pLayer->CanvasWidth, pLayer->CanvasHeight,
                            pLayer->DisplayWidth, pLayer->DisplayHeight,
                            pLayer->ScreenWidth,pLayer->ScreenHeight
                            );
        }

        pLayer++;
    }

    return p;
}

struct seq_file* Log_PrintSurfaceInfo(struct seq_file* p)
{
    HI_S32 i = 0;
    HI_U32 count = 0;
    HIGO_LOG_SURFACE_S* pSurface = LOG_KSURFACE_START_PTR;
    if ((p == NULL) || (s_pu8LogMemKVirtAddr == NULL)) {
        return NULL;
    }

    if (!Log_NeedProc())
    {
        return p;
    }

    seq_printf(p, "\n----------------------- surface inforamtion -------------- \n");
    for ( i = 0 ; i < HIGO_LOG_MAX_SURFACE_COUNT; i++ )
    {
        if (pSurface->pSurface)
        {
            HI_U32 u32Fmt = pSurface->eFormat;
            if (u32Fmt >= TABLE_NUM(s_pszFormat))
            {
                u32Fmt = TABLE_NUM(s_pszFormat) - 1;
            }

            if (!(count % 20))
            {
                seq_printf(p, "\n%6s %16s %8s %8s %8s %8s %16s %s \n",
                                 "number", "name", "width", "height", "pitch",
                                 "phyaddr", "size(Kbytes)", "format");

            }

            seq_printf(p, "%04d %16s %8d %8d %8d 0x%08x %8d  %16s \n",
                                       count,
                                       pSurface->szName,
                                       pSurface->u16Width,
                                       pSurface->u16Height,
                                       pSurface->u16Pitch,
                                       pSurface->u32PhyAddr,
                                       pSurface->u16Pitch*pSurface->u16Height/1024,
                                       s_pszFormat[u32Fmt]);
            count++;

        }

        pSurface++;
    }

    return p;
}

struct seq_file* Log_PrinSysInfo(struct seq_file* p)
{
    HIGO_LOG_SYS_S* pLogSysInfo = (HIGO_LOG_SYS_S*)s_pu8LogMemKVirtAddr;
    if ((p == NULL) || (s_pu8LogMemKVirtAddr == NULL)) {
        seq_printf(p, "The log memory is null! There is a unkown error!\n");
        return NULL;
    }

    if (pLogSysInfo->eCtl& HIGO_LOG_CTL_HIGO_RUN)
    {
        seq_printf(p, "\n------------------------- higo total information ----------- \n");
        seq_printf(p, "%s %s  \n", pLogSysInfo->szBuildTime,pLogSysInfo->szVersion);
        seq_printf(p, "proc status          : %s \n", (pLogSysInfo->eCtl& HIGO_LOG_CTL_ENABLE_PROC) ? "ON" : "OFF");
        seq_printf(p, "time print           : %s \n", (pLogSysInfo->eCtl& HIGO_LOG_CTL_ENABLE_PERORMANCE) ? "ON" : "OFF");
    }
    else
    {
        seq_printf(p, "The higo don't run yet!\n");
        return p;
    }

    if (pLogSysInfo->eCtl& HIGO_LOG_CTL_ENABLE_PROC)
    {
        seq_printf(p, "surface used         : %d \n", pLogSysInfo->u32SurfaceCount);
        seq_printf(p, "layer used           : %d \n", pLogSysInfo->u32LayerCount);
        seq_printf(p, "mmz memory used      : %d bytes ( %d KBytes )\n",
                        pLogSysInfo->u32PhyMemUsed, pLogSysInfo->u32PhyMemUsed/1024);
        seq_printf(p, "system memory used   : %d bytes ( %d KBtyes )\n",
                        pLogSysInfo->u32SysMemUsed, pLogSysInfo->u32SysMemUsed/1024);
    }

    return p;
}

HI_VOID Log_PrintHelpInfo(HI_VOID)
{
    printk("\nusage as following: \n");
    printk("    cat /proc/umap/higo              display all proc information \n");
    printk("    echo proc on > /proc/umap/higo   enable higo proc. default is disable!\n");
    printk("    echo proc off > /proc/umap/higo  disable the proc information! \n");
    printk("    echo time on > /proc/umap/higo   enable higo print time. default is disable!\n");
    printk("    echo time off > /proc/umap/higo  disable print time! \n");
}

HI_S32 Log_Show(struct osal_proc_dir_entry *entry)
{
    if (entry == NULL) {
        return HI_FAILURE;
    }
    Log_PrintHelpInfo();
    Log_PrinSysInfo(entry->seqfile);
    Log_PrintLayerInfo(entry->seqfile);
    Log_PrintSurfaceInfo(entry->seqfile);
    return HI_SUCCESS;
}

#define BUF_LEN 128
HI_S32 Log_Write(struct osal_proc_dir_entry *entry, const char *buffer, int count, long long *ppos)
{
    const void *pBuf = NULL;
    size_t len = count;

    HI_CHAR buf[BUF_LEN+1] = {'\0'};
    HIGO_LOG_SYS_S* pSysInfo = (HIGO_LOG_SYS_S*)s_pu8LogMemKVirtAddr;

    if ((pSysInfo == NULL) || (buffer == NULL)) {
        return HI_FAILURE;
    }
    pBuf = buffer;
    if (len > BUF_LEN)
    {
        LOG_ERROR("The command string is out of buf space :%ld bytes !\n", (HI_UL)sizeof(buf));
        return HI_FAILURE;
    }

    if (copy_from_user(buf, pBuf, len))
    {
        LOG_ERROR("failed to call copy_from_user !\n");
        return HI_FAILURE;
    }
    buf[sizeof(buf) - 1] = '\0';

    if (strstr(buf, "proc on"))
    {
        pSysInfo->eCtl |= HIGO_LOG_CTL_ENABLE_PROC;
    }
    else if (strstr(buf, "proc off"))
    {
        pSysInfo->eCtl &= ~HIGO_LOG_CTL_ENABLE_PROC;
    }
    if (strstr(buf, "time on"))
    {
        pSysInfo->eCtl |= HIGO_LOG_CTL_ENABLE_PERORMANCE;
    }
    else if (strstr(buf, "time off"))
    {
        pSysInfo->eCtl &= ~HIGO_LOG_CTL_ENABLE_PERORMANCE;
    }
    else
    {
        Log_PrintHelpInfo();
    }

    return HI_FAILURE;
}

static HI_S32 Log_GetMemory(HIGO_LOG_MEM_INFO_S* pMem)
{
    if (pMem != NULL) {
        pMem->u32PhyAddr = s_u32LogMemPhyAddr;
        pMem->u32Size = HIGO_LOG_MEM_SIZE;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 Log_Ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    HIGO_LOG_MEM_INFO_S info = {0};
    HI_S32 s32Ret = HI_FAILURE;
    if (arg == 0) {
        return -EFAULT;
    }

    if (cmd == HIGO_GET_LOG_MEM)
    {
        s32Ret = Log_GetMemory(&info);
        if (HI_SUCCESS == s32Ret)
        {
            s32Ret = copy_to_user((HI_VOID*)(HI_UINTPTR_T)arg, &info, sizeof(info));
        }

        if (HI_SUCCESS != s32Ret)
        {
            return -EFAULT;
        }
    }

    return 0;
}

HI_S32 HIGO_Log_Deinit(HI_VOID)
{
    if (s_pu8LogMemKVirtAddr != NULL)
    {
        hifb_buf_ummap(s_pu8LogMemKVirtAddr);
        s_pu8LogMemKVirtAddr = NULL;
    }

    if (s_u32LogMemPhyAddr != 0)
    {
        hifb_buf_freemem(s_u32LogMemPhyAddr);
        s_u32LogMemPhyAddr = 0;
    }

    HIFB_PROC_RemoveModule("higo");
    return HI_SUCCESS;
}

HI_S32 HIGO_Log_Init(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;

    HIFB_PROC_AddModule("higo", Log_Show, Log_Write, Log_Ioctl, NULL);

    s_u32LogMemPhyAddr = hifb_buf_allocmem("higo_log", HIGO_LOG_MEM_SIZE, NULL);
    if  (0 == s_u32LogMemPhyAddr)
    {
        LOG_ERROR("failed to malloc mmz memory size:%ld\n", (HI_UL)HIGO_LOG_MEM_SIZE);
        goto ERR;
    }

    s_pu8LogMemKVirtAddr = (HI_U8*)hifb_buf_map(s_u32LogMemPhyAddr, HIGO_LOG_MEM_SIZE);
    if (NULL == s_pu8LogMemKVirtAddr)
    {
        LOG_ERROR("failed to map phyical addr to kernel virtual addr!");
        goto ERR;
    }

    osal_memset(s_pu8LogMemKVirtAddr, 0x00, HIGO_LOG_MEM_SIZE);
    return HI_SUCCESS;

ERR:
   HIGO_Log_Deinit();
   return s32Ret;
}

#endif
