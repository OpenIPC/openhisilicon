#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_tde_api.h"
#include "hi_tde_type.h"
#include "hi_tde_errcode.h"
#include "hifb.h"

#include "hi_comm_vo.h"
#include "mpi_sys.h"

#include "mpi_vo.h"
#include "sample_comm.h"


#define TDE_PRINT printf

#define VoDev 0
#define VoChn 0


#define MIN(x,y) ((x) > (y) ? (y) : (x))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

static const HI_CHAR *pszImageNames =
{
    "res1/ARGB1555_r+b.bits", 
};

#define BACKGROUND_NAME  "res1/background.bits"

#define PIXFMT  TDE2_COLOR_FMT_ARGB1555
#define BPP     2
#define SCREEN_WIDTH    720
#define SCREEN_HEIGHT   576

static TDE2_SURFACE_S g_stScreen[2];
static TDE2_SURFACE_S g_stImgSur;

static HI_S32 TDE_CreateSurfaceByFile(const HI_CHAR *pszFileName, TDE2_SURFACE_S *pstSurface, HI_U8 *pu8Virt)
{
    FILE *fp;
    HI_U32 colorfmt, w, h, stride;

    if((NULL == pszFileName) || (NULL == pstSurface))
    {
        printf("%s, LINE %d, NULL ptr!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    fp = fopen(pszFileName, "rb");
    if(NULL == fp)
    {
        printf("error when open pszFileName %s, line:%d\n", pszFileName, __LINE__);
        return -1;
    }

    fread(&colorfmt, 1, 4, fp);
    fread(&w, 1, 4, fp);
    fread(&h, 1, 4, fp);
    fread(&stride, 1, 4, fp);

    pstSurface->enColorFmt = colorfmt;
    pstSurface->u32Width = w;
    pstSurface->u32Height = h;
    pstSurface->u32Stride = stride;
    pstSurface->u8Alpha0 = 0x0;
    pstSurface->u8Alpha1 = 0xff;
    pstSurface->bAlphaMax255 = HI_TRUE;
    pstSurface->bAlphaExt1555 = HI_TRUE;

    fread(pu8Virt, 1, stride*h, fp);

    fclose(fp);

    return 0;
}

static HI_VOID circumrotate (HI_U32 u32CurOnShow)
{
    TDE_HANDLE s32Handle;
	TDE2_OPT_S stOpt = {0};
	TDE2_RECT_S stSrcRect;
	TDE2_RECT_S stDstRect;
	HI_U32 u32NextOnShow;
	u32NextOnShow = !u32CurOnShow;
	HI_U32 s32Ret;
	HI_U32 u32FillData = 0x8000;
	TDE2_RECT_S pstScreenRect;
	/* 1. start job */
	s32Handle = HI_TDE2_BeginJob();
	if(HI_ERR_TDE_INVALID_HANDLE == s32Handle)
	{
		TDE_PRINT("start job failed!\n");
		return ;
	}   
	pstScreenRect.s32Xpos = 0;
	pstScreenRect.s32Ypos = 0;
	pstScreenRect.u32Width = SCREEN_WIDTH;
	pstScreenRect.u32Height = SCREEN_HEIGHT;
	
	s32Ret = HI_TDE2_QuickFill(s32Handle, &g_stScreen[u32NextOnShow], &pstScreenRect, u32FillData);
	if(s32Ret < 0)
	{
		TDE_PRINT("Line:%d,HI_TDE2_QuickFill failed,ret=0x%x!\n", __LINE__, s32Ret);
		HI_TDE2_CancelJob(s32Handle);
		return ;
	}	
	stOpt.enColorKeyMode = TDE2_COLORKEY_MODE_FOREGROUND;
    stOpt.unColorKeyValue.struCkARGB.stBlue.bCompIgnore = HI_FALSE;
    stOpt.unColorKeyValue.struCkARGB.stBlue.bCompOut = HI_FALSE;
    stOpt.unColorKeyValue.struCkARGB.stBlue.u8CompMin = 0xff;
    stOpt.unColorKeyValue.struCkARGB.stBlue.u8CompMax = 0xff;
    stOpt.unColorKeyValue.struCkARGB.stBlue.u8CompMask = 0xff;

    stOpt.unColorKeyValue.struCkARGB.stGreen.bCompIgnore = HI_TRUE;
    stOpt.unColorKeyValue.struCkARGB.stRed.bCompIgnore = HI_TRUE;    
    stOpt.unColorKeyValue.struCkARGB.stAlpha.bCompIgnore = HI_TRUE;
	
    stSrcRect.s32Xpos = 0;
    stSrcRect.s32Ypos = 0;
    stSrcRect.u32Width = g_stImgSur.u32Width;
    stSrcRect.u32Height = g_stImgSur.u32Height;

    stDstRect.s32Xpos = 100;
    stDstRect.s32Ypos = 100;
    stDstRect.u32Width = g_stImgSur.u32Width;
    stDstRect.u32Height = g_stImgSur.u32Height;

    /* 4. bitblt image to screen */
    s32Ret = HI_TDE2_Bitblit(s32Handle, &g_stScreen[u32NextOnShow], &stDstRect, 
        &g_stImgSur, &stSrcRect, &g_stScreen[u32NextOnShow], &stDstRect, &stOpt);
	if(s32Ret < 0)
	{
		TDE_PRINT("Line:%d,HI_TDE2_Bitblit failed,ret=0x%x!\n", __LINE__, s32Ret);
		HI_TDE2_CancelJob(s32Handle);
		return ;
	}
    /* 5. submit job */
    s32Ret = HI_TDE2_EndJob(s32Handle, HI_FALSE, HI_TRUE, 10);
	if(s32Ret < 0)
	{
		TDE_PRINT("Line:%d,HI_TDE2_EndJob failed,ret=0x%x!\n", __LINE__, s32Ret);
		HI_TDE2_CancelJob(s32Handle);
		return ;
	}

    return;
}

HI_S32 TDE_DrawGraphicSample()
{
    HI_U32 u32Size;
    HI_S32 s32Fd;
    HI_U8* pu8BackGroundVir;
    HI_U32 u32PhyAddr;
    HI_S32 s32Ret = -1;
    HI_BOOL bShow;
	HI_U32 u32Times;
    HIFB_ALPHA_S stAlpha = {0};
    struct fb_fix_screeninfo stFixInfo;
    struct fb_var_screeninfo stVarInfo;
    struct fb_bitfield stR32 = {10, 5, 0};
    struct fb_bitfield stG32 = {5, 5, 0};
    struct fb_bitfield stB32 = {0, 5, 0};
    struct fb_bitfield stA32 = {15, 1, 0};

    /* 1. open tde device */
    HI_TDE2_Open();

    /* 2. framebuffer operation */
    s32Fd = open("/dev/fb0", O_RDWR);
    if (s32Fd == -1)
    {
        printf("open frame buffer device error\n");
        goto FB_OPEN_ERROR;
    }  
	
    stAlpha.bAlphaChannel = HI_FALSE;
    stAlpha.bAlphaEnable = HI_TRUE;
	stAlpha.u8Alpha0 = 0xFF;
	stAlpha.u8Alpha1 = 0x0;
    if (ioctl(s32Fd, FBIOPUT_ALPHA_HIFB, &stAlpha) < 0)
    {
    	 printf("Put alpha info failed!\n");
         goto FB_PROCESS_ERROR1;
    }
    
	/* get the variable screen info */
    if (ioctl(s32Fd, FBIOGET_VSCREENINFO, &stVarInfo) < 0)
    {
   	    printf("Get variable screen info failed!\n");
        goto FB_PROCESS_ERROR1;
    }
    stVarInfo.xres_virtual	 	= SCREEN_WIDTH;
    stVarInfo.yres_virtual		= SCREEN_HEIGHT*2;
    stVarInfo.xres      		= SCREEN_WIDTH;
    stVarInfo.yres      		= SCREEN_HEIGHT;
    stVarInfo.activate  		= FB_ACTIVATE_NOW;
    stVarInfo.bits_per_pixel	= 16;
    stVarInfo.red   = stR32;
    stVarInfo.green = stG32;
    stVarInfo.blue  = stB32;
    stVarInfo.transp = stA32;

    if (ioctl(s32Fd, FBIOPUT_VSCREENINFO, &stVarInfo) < 0)
    {
        printf("process frame buffer device error\n");
        goto FB_PROCESS_ERROR1;
    }

    if (ioctl(s32Fd, FBIOGET_FSCREENINFO, &stFixInfo) < 0)
    {
        printf("process frame buffer device error\n");
        goto FB_PROCESS_ERROR1;
    }
    u32Size 	= stFixInfo.smem_len;
    u32PhyAddr  = stFixInfo.smem_start;

    /* 3. create surface */
    g_stScreen[0].enColorFmt = PIXFMT;
    g_stScreen[0].u32PhyAddr = u32PhyAddr;
    g_stScreen[0].u32Width = SCREEN_WIDTH;
    g_stScreen[0].u32Height = SCREEN_HEIGHT;
    g_stScreen[0].u32Stride = stFixInfo.line_length;
    g_stScreen[0].bAlphaMax255 = HI_TRUE;
	g_stScreen[0].u8Alpha0 = 0;
	g_stScreen[0].u8Alpha1 = 0;
	g_stScreen[1] = g_stScreen[0];
	g_stScreen[1].u32PhyAddr = g_stScreen[0].u32PhyAddr + g_stScreen[0].u32Stride * g_stScreen[0].u32Height;

    /* allocate memory (720*576*2 bytes) to save Image infornation */
    if (HI_FAILURE == HI_MPI_SYS_MmzAlloc(&(g_stImgSur.u32PhyAddr), ((void**)&pu8BackGroundVir), 
            NULL, NULL, 720*576*4))
    {
        TDE_PRINT("allocate memory (720*576*2*N_IMAGES bytes) failed\n");
        goto FB_PROCESS_ERROR1;
    }
    
    TDE_CreateSurfaceByFile(pszImageNames, &g_stImgSur, pu8BackGroundVir);    
	bShow = HI_TRUE;
	if (ioctl(s32Fd, FBIOPUT_SHOW_HIFB, &bShow) < 0)
	{
		fprintf (stderr, "Couldn't show fb\n");
		goto FB_PROCESS_ERROR2;
	} 
	for (u32Times = 0; u32Times < 10; u32Times++)
    {         
        circumrotate(u32Times%2);
        stVarInfo.yoffset = (u32Times%2)?0:576;

        /*set frame buffer start position*/
        if (ioctl(s32Fd, FBIOPAN_DISPLAY, &stVarInfo) < 0)
        {
            TDE_PRINT("process frame buffer device error\n");
            goto FB_PROCESS_ERROR2;
        }
        sleep(1);
    }
	
	s32Ret = 0;
FB_PROCESS_ERROR2:    
    HI_MPI_SYS_MmzFree(g_stImgSur.u32PhyAddr, pu8BackGroundVir);
FB_PROCESS_ERROR1:
    close(s32Fd); 
FB_OPEN_ERROR:
    HI_TDE2_Close();

    return s32Ret;
}

/*****************************************************************************
description: 	this sample shows how to use TDE interface to draw graphic.
                
note	   :    for showing graphic layer, VO device should be enabled first.
				This sample draws graphic on layer G0 which belongs to HD VO device.
				(here we insmod HiFB.ko like 'insmod hifb.ko video="hifb:vram0_size=XXX" '
				 so opening hifb sub-device '/dev/fb0' means to opening G0,
				 and G0 was fixed binded to HD VO device in Hi3531)
*****************************************************************************/
int main()
{
	HI_S32 s32Ret = 0;
	VO_PUB_ATTR_S stPubAttr;
	VB_CONF_S stVbConf;

	stPubAttr.u32BgColor = 0x000000ff;
#ifdef HI_FPGA
	stPubAttr.enIntfType = VO_INTF_BT1120|VO_INTF_VGA;
#else
    stPubAttr.enIntfType = VO_INTF_VGA;
#endif
    stPubAttr.enIntfSync = VO_OUTPUT_720P50;
	stPubAttr.bDoubleFrame = HI_FALSE;

	memset(&stVbConf, 0, sizeof(VB_CONF_S));
    stVbConf.u32MaxPoolCnt             = 16;
	stVbConf.astCommPool[0].u32BlkSize = 1280*720*2;
	stVbConf.astCommPool[0].u32BlkCnt  = 8;	
	
    /*1 enable Vo device HD first*/
	if(HI_SUCCESS != SAMPLE_COMM_SYS_Init(&stVbConf))
	{
		return -1;
	}
	if(HI_SUCCESS != SAMPLE_COMM_VO_StartDevLayer(VoDev, &stPubAttr, 25))
	{
		SAMPLE_COMM_SYS_Exit();
		return -1;
	}
	
	/*2 run tde sample which draw grahpic on HiFB memory*/
	s32Ret = TDE_DrawGraphicSample();
	if(s32Ret != HI_SUCCESS)
	{
		goto err;
	}

err:
	SAMPLE_COMM_VO_StopDevLayer(VoDev);
	SAMPLE_COMM_SYS_Exit();

	return 0;
}		


