/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <stdio.h>
#include "isp_main.h"
#include "isp_debug.h"
#include "isp_debug.h"
#include "isp_ext_config.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "ot_mpi_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

GK_S32 ISP_DbgSet(VI_PIPE ViPipe, const ISP_DEBUG_INFO_S *pstDbgInfo)
{
	GK_U32 u32Size = 0;

	if (ext_system_sys_debug_enable_read(ViPipe) && pstDbgInfo->bDebugEn) {
		ISP_TRACE(MODULE_DBG_ERR, "isp has enabled debug info!\n");
		return GK_FAILURE;
	}

	if (pstDbgInfo->bDebugEn) {
		if (pstDbgInfo->u64PhyAddr == 0) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "isp lib's debug phyaddr is 0!\n");
			return GK_FAILURE;
		}

		if (pstDbgInfo->u32Depth == 0) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "ae lib's debug depth is 0!\n");
			return GK_FAILURE;
		}
		u32Size = sizeof(ISP_DBG_ATTR_S) +
			  sizeof(ISP_DBG_ATTR_S) * pstDbgInfo->u32Depth;
	}

	/* don't clear phyaddr and size when disable dbg info. */
	if (pstDbgInfo->bDebugEn) {
		ext_system_sys_debug_enable_write(ViPipe, pstDbgInfo->bDebugEn);
		ext_system_sys_debuggh_addr_write(
			ViPipe, ((pstDbgInfo->u64PhyAddr >> 32) & 0xFFFFFFFF));
		ext_system_sys_debug_low_addr_write(
			ViPipe, (pstDbgInfo->u64PhyAddr & 0xFFFFFFFF));
		ext_system_sys_debug_depth_write(ViPipe, pstDbgInfo->u32Depth);
		ext_system_sys_debug_size_write(ViPipe, u32Size);
	} else {
		ext_system_sys_debug_enable_write(ViPipe, pstDbgInfo->bDebugEn);
	}

	return GK_SUCCESS;
}

GK_S32 ISP_DbgGet(VI_PIPE ViPipe, ISP_DEBUG_INFO_S *pstDbgInfo)
{
	GK_U64 u64PhyAddrHigh;
	GK_U64 u64PhyAddrTemp;

	u64PhyAddrHigh = (GK_U64)ext_system_sys_debuggh_addr_read(ViPipe);
	u64PhyAddrTemp = (GK_U64)ext_system_sys_debug_low_addr_read(ViPipe);
	u64PhyAddrTemp |= (u64PhyAddrHigh << 32);

	pstDbgInfo->u64PhyAddr = u64PhyAddrTemp;
	pstDbgInfo->bDebugEn = ext_system_sys_debug_enable_read(ViPipe);
	pstDbgInfo->u32Depth = ext_system_sys_debug_depth_read(ViPipe);

	return GK_SUCCESS;
}

GK_S32 ISP_DbgRunBgn(ISP_DBG_CTRL_S *pstDbg, GK_U32 u32FrmCnt)
{
	ISP_DBG_STATUS_S *pstDbgStatus = GK_NULL;

	if (!pstDbg->bDebugEn) {
		if (pstDbg->pstDbgAttr != GK_NULL) {
			MPI_SYS_Munmap(pstDbg->pstDbgAttr, pstDbg->u32Size);
			pstDbg->pstDbgAttr = GK_NULL;
			pstDbg->pstDbgStatus = GK_NULL;
		}
		return GK_SUCCESS;
	}

	if ((pstDbg->bDebugEn) && (pstDbg->pstDbgAttr == GK_NULL)) {
		pstDbg->pstDbgAttr = (ISP_DBG_ATTR_S *)MPI_SYS_Mmap(
			pstDbg->u64PhyAddr, pstDbg->u32Size);
		if (pstDbg->pstDbgAttr == GK_NULL) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "isp map debug buf failed!\n");
			return GK_FAILURE;
		}
		pstDbg->pstDbgStatus =
			(ISP_DBG_STATUS_S *)(pstDbg->pstDbgAttr + 1);

		/* ------------- record attr ------------------ */
	}

	pstDbgStatus = pstDbg->pstDbgStatus +
		       (u32FrmCnt % DIV_0_TO_1(pstDbg->u32Depth));

	pstDbgStatus->u32FrmNumBgn = u32FrmCnt;

	return GK_SUCCESS;
}

GK_S32 ISP_DbgRunEnd(ISP_DBG_CTRL_S *pstDbg, GK_U32 u32FrmCnt)
{
	ISP_DBG_STATUS_S *pstDbgStatus = GK_NULL;

	if ((!pstDbg->bDebugEn) || (pstDbg->pstDbgStatus == GK_NULL)) {
		return GK_SUCCESS;
	}

	pstDbgStatus = pstDbg->pstDbgStatus +
		       (u32FrmCnt % DIV_0_TO_1(pstDbg->u32Depth));

	/* ------------- record status ------------------ */

	pstDbgStatus->u32FrmNumEnd = u32FrmCnt;

	return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
