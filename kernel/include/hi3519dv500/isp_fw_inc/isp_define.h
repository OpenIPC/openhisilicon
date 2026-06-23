/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_DEFINE_H
#define ISP_DEFINE_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* bayer nr */
#define ISP_BAYERNR_STRENGTH_DIVISOR           100
#define ISP_BAYERNR_CORINGLOW_STRENGTH_DIVISOR 10000

/* local cac */
#define LCAC_STRENGTH_NUM       16

/* ldci */
#define LDCI_STAT_WND_X          24
#define LDCI_STAT_WND_Y          16
#define LDCI_LPF_LUT_SIZE        9
#define LDCI_HE_LUT_SIZE         33
#define LDCI_DE_USM_LUT_SIZE     33
#define LDCI_COLOR_GAIN_LUT_SIZE 65
#define LDCI_DE_LPF_H_COEF_NUM   3
#define LDCI_DE_LPF_V_COEF_NUM   2
#define LDCI_POLY_LUT_NUM        65
#define LDCI_PF_SFT_NUM          4
#define LDCI_PF_PARA_NUM         2
#define LDCI_SM_WIDTH_NUM        3

/* drc */
#define DRC_TM_SEG_NUM           8

/* pregamma */
#define PREGAMMA_SEG_NUM         12


/* sharpen */
#define     ISP_SHARPEN_FREQ_CORING_LENGTH      8
#define     SHRP_GAIN_LUT_SIZE                  64


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* ISP_DEFINE_H */
