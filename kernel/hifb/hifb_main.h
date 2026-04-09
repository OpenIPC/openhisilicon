/*
* Copyright (c) 2018 HiSilicon Technologies Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/
#ifndef __HIFB_MAIN_H__
#define __HIFB_MAIN_H__
#include "hifb_vou_drv.h"


typedef unsigned char           HI_UCHAR;


/* define the value of default set of each layer */
#define HIFB_HD_DEF_WIDTH    1920/*1280*/     /* unit: pixel */
#define HIFB_HD_DEF_HEIGHT   1080/*720*/     /* unit: pixel */
#define HIFB_HD_DEF_STRIDE   (HIFB_HD_DEF_WIDTH*4)    /* unit: byte */
#define HIFB_HD_DEF_VRAM     16200/*7200*/   /* unit:KB 1280*720*4*2*//*unit:KB 1920*1080*4*2*/

#define HIFB_SD_DEF_WIDTH    720
#define HIFB_SD_DEF_HEIGHT   576
#define HIFB_SD_DEF_STRIDE   1440
#define HIFB_SD_DEF_VRAM     3240   /* unit:KB 720*576*4*/

#define HIFB_AD_DEF_WIDTH    720
#define HIFB_AD_DEF_HEIGHT   576
#define HIFB_AD_DEF_STRIDE   1440
#define HIFB_AD_DEF_VRAM     3240    /*unit:KB 720*576*4*/

#define HIFB_CURSOR_DEF_WIDTH 256
#define HIFB_CURSOR_DEF_HEIGHT 256
#define HIFB_CURSOR_DEF_STRIDE 512
#define HIFB_CURSOR_DEF_VRAM 512    /*unit:KB for doudble buffer mode, we need 2 memory buffer, save cursor*/
                                    /* 256*256*4*2/1024 KB */

#define HIFB_SOFTCURSOR_STRIDE 512

#define HIFB_DEF_DEPTH    16      /* unit: bits */
#define HIFB_DEF_XSTART   0
#define HIFB_DEF_YSTART   0
#define HIFB_DEF_ALPHA    0xff

#define HIFB_MAX_ZOOMIN        15

#if (HICHIP == HI3519A_V100)
#define HIFB_TDE_MEMSET_WIDTH  1920
#define HIFB_TDE_MEMSET_HEIGHT 1080
#else
#define HIFB_TDE_MEMSET_WIDTH  3840
#define HIFB_TDE_MEMSET_HEIGHT 2160
#endif

#define HIFB_CMAP_SIZE  0    /*unit:KB 256*4*/
#define MMB_ADDR_INVALID    (0)

#define HIFB_IS_CLUTFMT(eFmt)  (HIFB_FMT_1BPP <= (eFmt) && (eFmt) <= HIFB_FMT_ACLUT88)
#define HIFB_ALPHA_OPAQUE    0xff
#define HIFB_ALPHA_TRANSPARENT 0x00

#define HIFB_MIN(m, n) (m) > (n) ? (n) : (m)

#define HIFB_DEFLICKER_LEVEL_MAX 5   /* support level 5 deflicker most */

#define HIFB_MAX_CURSOR_WIDTH 128
#define HIFB_MAX_CURSOR_HEIGHT 128


#define HIFB_SPIN_LOCK_FLAG                      unsigned long
#define HIFB_SPIN_LOCK_S                         spinlock_t
#define HIFB_SPIN_LOCK_INIT(pLock)               spin_lock_init(pLock)
#define HIFB_SPIN_LOCK_IRQSAVE(pLock, flag)      spin_lock_irqsave(pLock, flag)
#define HIFB_SPIN_UNLOCK_IRQRESTORE(pLock, flag) spin_unlock_irqrestore(pLock, flag)

#define CALCULATE_UNITE_RECT(stDstRect, stSrcRect) do\
{\
    HIFB_RECT stRect;\
    stRect.x = (stDstRect.x < stSrcRect.x)? stDstRect.x : stSrcRect.x;\
    stRect.y = (stDstRect.y < stSrcRect.y)? stDstRect.y : stSrcRect.y;\
    stRect.w = ((stDstRect.x + stDstRect.w) > (stSrcRect.x + stSrcRect.w))? \
        (stDstRect.x + stDstRect.w - stRect.x) : (stSrcRect.x + stSrcRect.w - stRect.x);\
    stRect.h = ((stDstRect.y + stDstRect.h) > (stSrcRect.y + stSrcRect.h))? \
        (stDstRect.y + stDstRect.h - stRect.y) : (stSrcRect.y + stSrcRect.h - stRect.y);\
    if (stDstRect.w == 0 && stDstRect.h == 0) \
    { \
        osal_memcpy((void*)&stDstRect, (void*)&stSrcRect, sizeof(HIFB_RECT));\
    } \
    else if (stSrcRect.w != 0 && stSrcRect.h != 0) \
    { \
        osal_memcpy((void*)&stDstRect, (void*)&stRect, sizeof(HIFB_RECT));\
    }  \
}while(0)


typedef enum
{
    HIFB_ANTIFLICKER_NONE,   /* no antiflicker.If scan mode is progressive, hifb will set antiflicker mode to none */
    HIFB_ANTIFLICKER_TDE,    /* tde antiflicker mode, it's effect for 1buf or 2buf only */
    HIFB_ANTIFLICKER_VO,     /* vo antiflicker mode, need hardware supprot */
    HIFB_ANTIFLICKER_BUTT
} HIFB_LAYER_ANTIFLICKER_MODE_E;

typedef struct
{
    HIFB_POINT_S stPos;
    HI_U32 u32DisplayWidth;
    HI_U32 u32DisplayHeight;
    HI_U32 u32ScreenWidth;
    HI_U32 u32ScreenHeight;
    HI_BOOL bPreMul;
    HI_BOOL bNeedAntiflicker;
    HIFB_LAYER_ANTIFLICKER_LEVEL_E enAntiflickerLevel; /* antiflicker level */
    HIFB_LAYER_ANTIFLICKER_MODE_E enAntiflickerMode; /* antiflicker mode */
    HI_U32 u32VirXRes;
    HI_U32 u32VirYRes;
    HI_U32 u32XRes;
    HI_U32 u32YRes;
    HI_U32 u32MaxScreenWidth;
    HI_U32 u32MaxScreenHeight;
    HIFB_MIRROR_MODE_E enMirrorMode;
    HIFB_ROTATE_MODE_E enRotateMode;
    HIFB_DYNAMIC_RANGE_E    enDynamicRange;             /* which dynamic range */
}HIFB_DISPLAY_INFO_S;

typedef struct
{
    HI_U64 u64PhyAddr[2];                   /*buf address */
    HI_U32 u32Stride;                       /*buf stride*/
    HI_BOOL bNeedFlip;
    HI_BOOL bFliped;
    HI_U32 u32IndexForInt;
    HI_U32 u32IntPicNum;
    HIFB_RECT stUnionRect;
    HI_S32 s32RefreshHandle;
    HI_BOOL bCompress; /* Identifies whether the frame to be displayed is compressed */
}HIFB_DISPBUF_INFO_S;

typedef struct
{
    HIFB_LAYER_BUF_E enBufMode; /* buffer mode */
    HIFB_BUFFER_S stUserBuffer;
    HI_U64 u64ScreenAddr;       /* screen buf addr */
    HI_U64 u64GBScreenAddr;     /* new GB screen buf addr */
    HIFB_DISPBUF_INFO_S stDispBufInfo;
    HI_U32 u32RefreshNum;   /* refresh request num in 2 buf mode */
    HI_BOOL bDoRefreshJob;
}HIFB_REFRESH_INFO_S;

typedef struct
{
    /*For cursor layer, stCursor means cursor buffer, it is alloced and freed
     both by user;for general layer,stCursor means back buf*/
    HIFB_CURSOR_S stCursor;

    /*For cursor layer,you can quary whether cursor attach to a certain layer
     for general layer, you can quary whether cursor attach to it*/
    HI_U32 bAttched;

    /*valid area:overlap region between attached layer and cursor layer*/
    HIFB_RECT stRectInDispBuf;

    /*the orignal position of cursor, usually is (0,0) but also has different when at margin*/
    HIFB_POINT_S stPosInCursor;

    HI_U32 u32AttachedCursorId;
}HIFB_CURSOR_INFO_S;

typedef struct
{
    HI_BOOL     bCompressOpen;
    HI_BOOL     bDecompressOpen;
    HI_BOOL     bUpdateFinished;

    HIFB_RECT   stUpdateRect;

    HIFB_RECT   stCompressRect;

    HI_BOOL     bDelay;

    HI_U64      u64AR_BufPhyAddr;
    HI_U64      u64GB_BufPhyAddr;
    HI_U32      u32Frame_Size0;
    HI_U32      u32Frame_Size1;
    HI_U32      u32HeaderSize;
    HI_U32      u32Stride;

    HI_BOOL     bLayerAddrUpdate;

    HI_U32      u32StartSection;
    HI_U32      u32ZoneNums;
    HI_BOOL     bClearZone;

    HI_U32      u32NewStartSection;
    HI_U32      u32NewZoneNums;
}HIFB_COMPRESS_INFO_S;

typedef struct
{
    HI_U32  u32LayerID;   /* layer id */
    atomic_t ref_count;             /* framebuffer reference count */
    HI_BOOL          bShow;         /* show status */
    HIFB_COLOR_FMT_E enColFmt;      /* color format */
    HIFB_ALPHA_S     stAlpha;         /* alpha attribution */
    HIFB_COLORKEYEX_S  stCkey;          /* colorkey attribution */
    HIFB_DISPLAY_INFO_S stDisplayInfo;  /* display info */
    HIFB_REFRESH_INFO_S stRefreshInfo;
    HIFB_CURSOR_INFO_S stCursorInfo;
    volatile HIFB_COMPRESS_INFO_S stCompressInfo;
    HI_U32 u32ParamModifyMask;
    volatile HI_BOOL bModifying;
    HIFB_SURFACE_S CanvasSur;       /* canvas surface */
    HI_U32     u32HDflevel;      /* horizontal deflicker level */
    HI_U32     u32VDflevel;      /* vertical deflicker level */
    HI_UCHAR    ucHDfcoef[HIFB_DEFLICKER_LEVEL_MAX - 1];/* horizontal deflicker coefficients */
    HI_UCHAR    ucVDfcoef[HIFB_DEFLICKER_LEVEL_MAX - 1];/* vertical deflicker coefficients */
    volatile HI_U32 vblflag;
    wait_queue_head_t vblEvent;
    wait_queue_head_t doRefreshJob;
    spinlock_t lock;
    HI_BOOL    bLayerOpen;
    HI_U64 u64RotateVb;
#ifdef CONFIG_HIFB_FENCE_SUPPORT
    struct workqueue_struct *pFenceRefreshWorkqueue;
#endif
} HIFB_PAR_S;

typedef enum
{
    HIFB_LAYER_TYPE_HD,
    HIFB_LAYER_TYPE_SD,
    HIFB_LAYER_TYPE_AD,
    HIFB_LAYER_TYPE_CURSOR,
    HIFB_LAYER_TYPE_BUTT,
}HIFB_LAYER_TYPE_E;

typedef struct
{
    struct fb_bitfield stRed;     /* bitfield in fb mem if true color, */
    struct fb_bitfield stGreen;   /* else only length is significant */
    struct fb_bitfield stBlue;
    struct fb_bitfield stTransp;  /* transparency   */
} HIFB_ARGB_BITINFO_S;

typedef struct
{
    HI_U32 u32LayerId;
    HI_BOOL bSoftCursorUpdate;
    HI_BOOL bCompress;  /* if this tde task is compress output */
}HIFB_TDECALLBACK_PARAM;
#endif
