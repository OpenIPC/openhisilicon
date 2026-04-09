#ifndef __HIGO_LOG_TYPE_H__
#define __HIGO_LOG_TYPE_H__

/* add include here */

#ifdef __cplusplus
extern "C" {
#endif


/** */
typedef enum 
{
    HIGO_LOG_CTL_ENABLE_PROC = 0x01, /**< enable proc */
    HIGO_LOG_CTL_ENABLE_PERORMANCE = 0x2, /**< print performance time */
    HIGO_LOG_CTL_HIGO_RUN = 0x40,
} HIGO_LOG_CTL_E;

typedef struct
{
    HI_CHAR szVersion[128];
    HI_CHAR szBuildTime[128];
    HIGO_LOG_CTL_E eCtl;
    HI_S32  u32SysMemUsed; /*bytes*/
    HI_S32  u32PhyMemUsed; /*bytes*/
    HI_U32  u32SurfaceCount;
    HI_U32  u32LayerCount;
} HIGO_LOG_SYS_S;

typedef struct
{
    const HI_VOID* pLayer;
    HI_U32  LayerID;
    HI_S32 ScreenWidth;    /**< graphic layer display buffer width , the value should > 0 */
    HI_S32 ScreenHeight;   /**< graphic layer display buffer high   the value should > 0 */
    HI_S32 CanvasWidth;    /**< graphic layer canvas buffer  width, value 0 means not create this buffer*/
    HI_S32 CanvasHeight;    /**< graphic layer canvas buffer  high, value 0 means not create this buffer*/
    HI_S32 DisplayWidth;   /**< graphic layer canvas middle level buffer width if 0 it means the same width as Screen Width*/
    HI_S32 DisplayHeight;  /**< graphic layer canvas middle level buffer high if 0 it means the same width as Screen high*/
    HI_U32 LayerFlushType; /**< graphic layer  refresh mode,  value 0 means : double buffer + flip refresh mode */
    HI_U32 AntiLevel;      /**<graphic layer  flicker resiting level */
    HI_U32 PixelFormat;    /**< graphic layer  pixel format, this value should be set according to hw 's support.  */
    
    HI_BOOL bCreated;      /**< the flag is graphic layer has been create */
    HI_BOOL bDefSurface;   /**< flag is graphic layer has default canvas surface */
    HI_BOOL bUsrCanSurface;/**< flag is raphic layer  use user's canvas surface */
} HIGO_LOG_LAYER_S;

typedef struct
{
    HI_U32 u32SysMemUsed; 
    HI_U32 u32PhyMemUsed; 
} HIGO_LOG_MEM_S;
    
typedef struct
{
    const HI_VOID* pSurface;
    HI_CHAR szName[16];
    HI_U16  u16Width;
    HI_U16  u16Height;
    HI_U16  u16Pitch;
    //HI_U16  u16PhyMemoryUsed;
    HI_U32  u32PhyAddr;
    HI_U16  u16EnableColorkey;
    HI_U32  Colorkey;
    HI_U16   u16GlobalAlpha;
    HI_U32   eFormat;
} HIGO_LOG_SURFACE_S;

#define HIGO_LOG_MAX_SURFACE_COUNT  1024
#define HIGO_LOG_MAX_LAYER_COUNT    10
#define HIGO_LOG_MAX_MEMBLOCK_COUNT 20

/** all memory size for log */
#define HIGO_LOG_MEM_SIZE (sizeof(HIGO_LOG_MEM_S)*HIGO_LOG_MAX_MEMBLOCK_COUNT + \
                           sizeof(HIGO_LOG_SURFACE_S)*HIGO_LOG_MAX_SURFACE_COUNT + \
                           sizeof(HIGO_LOG_LAYER_S)*HIGO_LOG_MAX_LAYER_COUNT + \
                           sizeof(HIGO_LOG_SYS_S))
                           
#define LOG_LAYER_OFFSET   (sizeof(HIGO_LOG_SYS_S))
#define LOG_MEM_OFFSET     (LOG_LAYER_OFFSET + HIGO_LOG_MAX_LAYER_COUNT*(sizeof(HIGO_LOG_LAYER_S)))
#define LOG_SURFACE_OFFSET (LOG_MEM_OFFSET + HIGO_LOG_MAX_MEMBLOCK_COUNT*(sizeof(HIGO_LOG_MEM_S)))

typedef struct
{
    HI_U32 u32PhyAddr;
    HI_U32 u32Size;
} HIGO_LOG_MEM_INFO_S;

/** used to higo log  */
#define HIGO_GET_LOG_MEM   0x10101010

#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_ERRNO_H__ */

