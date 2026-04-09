/*
* Copyright (c) Hisilicon Technologies Co., Ltd. 2011-2019. All rights reserved.
* Description:
* Author: Hisilicon multimedia software group
* Create: 2011/06/28
*/


#ifndef __PROC_EXT_H__
#define __PROC_EXT_H__

#include "hi_osal.h"

#include "hi_type.h"
#include "hi_common.h"

#define PROC_ENTRY_VI        MPP_MOD_VI
#define PROC_ENTRY_VO        MPP_MOD_VO
#define PROC_ENTRY_VGS       MPP_MOD_VGS
#define PROC_ENTRY_VENC      MPP_MOD_VENC
#define PROC_ENTRY_VDEC      MPP_MOD_VDEC
#define PROC_ENTRY_H264E     MPP_MOD_H264E
#define PROC_ENTRY_H265E     MPP_MOD_H265E
#define PROC_ENTRY_H264D     MPP_MOD_H264D
#define PROC_ENTRY_JPEGE     MPP_MOD_JPEGE
#define PROC_ENTRY_JPEGD     MPP_MOD_JPEGD
#define PROC_ENTRY_PRORES    MPP_MOD_PRORES
#define PROC_ENTRY_MPEG4E    MPP_MOD_MPEG4E
#define PROC_ENTRY_CHNL      MPP_MOD_CHNL
#define PROC_ENTRY_AIO       MPP_MOD_AIO
#define PROC_ENTRY_AI        MPP_MOD_AI
#define PROC_ENTRY_AO        MPP_MOD_AO
#define PROC_ENTRY_AENC      MPP_MOD_AENC
#define PROC_ENTRY_ADEC      MPP_MOD_ADEC
#define PROC_ENTRY_ACODEC    MPP_MOD_ACODEC
#define PROC_ENTRY_VB        MPP_MOD_VB
#define PROC_ENTRY_GRP       MPP_MOD_GRP
#define PROC_ENTRY_VPSS      MPP_MOD_VPSS
#define PROC_ENTRY_RGN       MPP_MOD_RGN
#define PROC_ENTRY_SYS       MPP_MOD_SYS
#define PROC_ENTRY_PCIV      MPP_MOD_PCIV
#define PROC_ENTRY_LOG       MPP_MOD_LOG
#define PROC_ENTRY_DCCM      MPP_MOD_DCCM
#define PROC_ENTRY_DCCS      MPP_MOD_DCCS
#define PROC_ENTRY_MST_LOG   MPP_MOD_MST_LOG
#define PROC_ENTRY_PCIVFMW   MPP_MOD_PCIVFMW
#define PROC_ENTRY_RC        MPP_MOD_RC
#define PROC_ENTRY_IVE       MPP_MOD_IVE
#define PROC_ENTRY_FD        MPP_MOD_FD
#define PROC_ENTRY_SVP       MPP_MOD_SVP
#define PROC_ENTRY_SVP_NNIE  MPP_MOD_SVP_NNIE
#define PROC_ENTRY_SVP_ALG   MPP_MOD_SVP_ALG
#define PROC_ENTRY_SVP_DSP   MPP_MOD_SVP_DSP
#define PROC_ENTRY_DPU_RECT  MPP_MOD_DPU_RECT
#define PROC_ENTRY_DPU_MATCH MPP_MOD_DPU_MATCH
#define PROC_ENTRY_IVP       MPP_MOD_IVP

#define PROC_ENTRY_MD        MPP_MOD_MD
#define PROC_ENTRY_HDMI      MPP_MOD_HDMI
#define PROC_ENTRY_ISP       MPP_MOD_ISP
#define PROC_ENTRY_DIS       MPP_MOD_DIS
#define PROC_ENTRY_GYRODIS   MPP_MOD_GYRODIS
#define PROC_ENTRY_GDC       MPP_MOD_GDC
#define PROC_ENTRY_AVS       MPP_MOD_AVS
#define PROC_ENTRY_MCF       MPP_MOD_MCF
#define PROC_ENTRY_PM        MPP_MOD_PM
#define PROC_ENTRY_MFUSION   MPP_MOD_MFUSION

#define CMPI_PROC_SHOW       CMPI_PROC_READ
typedef hi_s32 (*CMPI_PROC_READ)(struct osal_proc_dir_entry *entry);
typedef hi_s32 (*CMPI_PROC_WRITE)(char *buf, int count);

extern hi_s32 cmpi_log_init(hi_u32);
extern hi_void cmpi_log_exit(hi_void);

#endif
