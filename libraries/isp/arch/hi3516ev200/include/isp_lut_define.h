/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __ISP_LUT_DEFINE_H__
#define __ISP_LUT_DEFINE_H__

/******************************************************************************
*******************************************************************************
*******************************************************************************
*************************** online lut stt define***********************************
*******************************************************************************
*******************************************************************************
******************************************************************************/
/* Define the union U_ISP_DEHAZE_LUT_WSTT */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int isp_dehaze_lut_wstt : 10; /* [9..0]  */
		unsigned int reserved_0 : 22; /* [31..10]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_ISP_DEHAZE_LUT_WSTT;

/* Define the union U_ISP_GAMMA_LUT_WSTT */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int isp_gamma_lut_wstt : 12; /* [11..0]  */
		unsigned int reserved_0 : 20; /* [31..12]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_ISP_GAMMA_LUT_WSTT;

/* Define the union U_ISP_SHARPEN_LUT_WSTT */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int isp_sharpen_lut_wstt : 24; /* [23..0]  */
		unsigned int reserved_0 : 8; /* [31..24]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_ISP_SHARPEN_LUT_WSTT;

/* Define the union U_ISP_LSC_LUT_WSTT */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int isp_mlsc_lut_wstt : 10; /* [9..0]  */
		unsigned int reserved_0 : 22; /* [31..10]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_ISP_MLSC_LUT_WSTT;

typedef union {
	/* Define the struct bits  */
	struct {
		unsigned int isp_bnr_lut_wstt : 32; /* [31..0]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_ISP_BNR_LUT_WSTT;
/* Define the union U_ISP_CA_LUT_WSTT */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int isp_ca_lut_wstt : 11; /* [10..0]  */
		unsigned int reserved_0 : 21; /* [31..11]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_ISP_CA_LUT_WSTT;

/* Define the union U_ISP_LDCI_LUT_WSTT */
typedef union {
	/* Define the struct bits  */
	struct {
		unsigned int isp_ldci_lut_wstt : 32; /* [31..0]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_ISP_LDCI_LUT_WSTT;

/* Define the global struct */
typedef struct {
	U_ISP_DEHAZE_LUT_WSTT ISP_DEHAZE_LUT_WSTT[640]; /* 0x0~0x9fc */
	U_ISP_GAMMA_LUT_WSTT ISP_GAMMA_LUT_WSTT[257]; /* 0xa00~0xe00 */
	unsigned int reserved_0[3]; /* 0xe04~0xe0c */
	U_ISP_SHARPEN_LUT_WSTT ISP_SHARPEN_LUT_WSTT[128]; /* 0xe10~0x100c */
	U_ISP_MLSC_LUT_WSTT ISP_MLSC_LUT_WSTT[1156]; /* 0x1010~0x221c */
	U_ISP_BNR_LUT_WSTT ISP_BNR_LUT_WSTT[260]; /* 0x2220~0x262c */
	U_ISP_CA_LUT_WSTT ISP_CA_LUT_WSTT[128]; /* 0x2630~0x282c */
	U_ISP_LDCI_LUT_WSTT ISP_LDCI_LUT_WSTT[260]; /* 0x2830~0x2c3c */
} S_ISP_LUT_WSTT_TYPE;

/******************************************************************************
*******************************************************************************
*******************************************************************************
************************** offline lut reg define***********************************
*******************************************************************************
*******************************************************************************
******************************************************************************/
/* Define the union U_ISP_AE_WEIGHT */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int isp_ae_weight0 : 4; /* [3..0]  */
		unsigned int reserved_0 : 4; /* [7..4]  */
		unsigned int isp_ae_weight1 : 4; /* [11..8]  */
		unsigned int reserved_1 : 4; /* [15..12]  */
		unsigned int isp_ae_weight2 : 4; /* [19..16]  */
		unsigned int reserved_2 : 4; /* [23..20]  */
		unsigned int isp_ae_weight3 : 4; /* [27..24]  */
		unsigned int reserved_3 : 4; /* [31..28]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_ISP_AE_WEIGHT;

/* Define the union U_ISP_DIS_REFINFO_WLUT */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int isp_dis_refinfo_wlut0 : 9; /* [8..0]  */
		unsigned int isp_dis_refinfo_wlut1 : 9; /* [17..9]  */
		unsigned int reserved_0 : 14; /* [31..18]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_ISP_DIS_REFINFO_WLUT;

/* Define the union U_ISP_DEMOSAIC_DEPURPLUT */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int isp_demosaic_depurp_lut : 4; /* [3..0]  */
		unsigned int reserved_0 : 28; /* [31..4]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_ISP_DEMOSAIC_DEPURPLUT;
/* Define the union U_ISP_NDDM_GF_LUT */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int isp_nddm_gflut : 12; /* [11..0]  */
		unsigned int reserved_0 : 20; /* [31..12]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_ISP_NDDM_GF_LUT;

/* Define the union U_ISP_EXPANDER_LUT */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int isp_expander_lut : 16; /* [15..0]  */
		unsigned int reserved_0 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_ISP_EXPANDER_LUT;

/* Define the union U_ISP_DRC_TMLUT0 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int isp_drc_tmlut0_diff : 14; /* [13..0]  */
		unsigned int isp_drc_tmlut0_value : 16; /* [29..14]  */
		unsigned int reserved_0 : 2; /* [31..30]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_ISP_DRC_TMLUT0;

/* Define the union U_ISP_DRC_CCLUT */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int isp_drc_cclut : 12; /* [11..0]  */
		unsigned int reserved_0 : 20; /* [31..12]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_ISP_DRC_CCLUT;

/* Define the union U_ISP_DRC_PREGAMMA */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int isp_drc_pregamma : 16; /* [15..0]  */
		unsigned int reserved_0 : 16; /* [31..16]  */
	} bits;

	/* Define an unsigned member */
	unsigned int u32;

} U_ISP_DRC_PREGAMMA;

/* Define the global struct */
typedef struct {
	U_ISP_AE_WEIGHT ISP_AE_WEIGHT[64]; /* 0x0~0xfc */
	U_ISP_DIS_REFINFO_WLUT ISP_DIS_REFINFO_WLUT[576]; /* 0x100~0x9fc */
	U_ISP_DEMOSAIC_DEPURPLUT ISP_DEMOSAIC_DEPURPLUT[16]; /* 0xa00~0xa3c */
	U_ISP_NDDM_GF_LUT ISP_NDDM_GF_LUT[17]; /* 0xa40~0xa80 */
	unsigned int reserved_0[3]; /* 0xa84~0xa8c */
	U_ISP_EXPANDER_LUT ISP_EXPANDER_LUT[129]; /* 0xa90~0xc90 */
	unsigned int reserved_1[3]; /* 0xc94~0xc9c */
	U_ISP_DRC_TMLUT0 ISP_DRC_TMLUT0[200]; /* 0xca0~0xfbc */
	U_ISP_DRC_CCLUT ISP_DRC_CCLUT[33]; /* 0xfc0~0x1040 */
	unsigned int reserved_2[3]; /* 0x1044~0x104c */
	U_ISP_DRC_PREGAMMA ISP_DRC_PREGAMMA[257]; /* 0x1050~0x1450 */
	unsigned int reserved_3[3]; /* 0x1454~0x145c */
	S_ISP_LUT_WSTT_TYPE stBeLut2Stt; /* 0x1460~0x409c */
} S_ISP_LUT_REGS_TYPE;

#endif /* __ISP_LUT_DEFINE_H__ */
