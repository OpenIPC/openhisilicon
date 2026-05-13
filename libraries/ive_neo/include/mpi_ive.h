/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * mpi_ive.h — HI_MPI_IVE_* entry points.
 *
 * Declared by libive_neo, an open-source replacement for vendor libive.so.
 * The MVP set covers every op our qemu-boot/test-*.c binaries invoke.
 */
#ifndef __MPI_IVE_H__
#define __MPI_IVE_H__

#include "hi_type.h"
#include "hi_common.h"
#include "hi_comm_ive.h"
#include "hi_ive.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --- Query (block until ioctl completes or poll state) --- */
HI_S32 HI_MPI_IVE_Query(IVE_HANDLE IveHandle,
                        HI_BOOL   *pbFinish,
                        HI_BOOL    bBlock);

/* --- Core ops --- */

HI_S32 HI_MPI_IVE_DMA(IVE_HANDLE *pIveHandle,
                      IVE_DATA_S *pstSrc,
                      IVE_DST_DATA_S *pstDst,
                      IVE_DMA_CTRL_S *pstDmaCtrl,
                      HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_Filter(IVE_HANDLE *pIveHandle,
                         IVE_SRC_IMAGE_S *pstSrc,
                         IVE_DST_IMAGE_S *pstDst,
                         IVE_FILTER_CTRL_S *pstFltCtrl,
                         HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_Sobel(IVE_HANDLE *pIveHandle,
                        IVE_SRC_IMAGE_S *pstSrc,
                        IVE_DST_IMAGE_S *pstDstH,
                        IVE_DST_IMAGE_S *pstDstV,
                        IVE_SOBEL_CTRL_S *pstSobelCtrl,
                        HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_MagAndAng(IVE_HANDLE *pIveHandle,
                            IVE_SRC_IMAGE_S *pstSrc,
                            IVE_DST_IMAGE_S *pstDstMag,
                            IVE_DST_IMAGE_S *pstDstAng,
                            IVE_MAG_AND_ANG_CTRL_S *pstMagAndAngCtrl,
                            HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_Dilate(IVE_HANDLE *pIveHandle,
                         IVE_SRC_IMAGE_S *pstSrc,
                         IVE_DST_IMAGE_S *pstDst,
                         IVE_DILATE_CTRL_S *pstDilateCtrl,
                         HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_Erode(IVE_HANDLE *pIveHandle,
                        IVE_SRC_IMAGE_S *pstSrc,
                        IVE_DST_IMAGE_S *pstDst,
                        IVE_ERODE_CTRL_S *pstErodeCtrl,
                        HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_Thresh(IVE_HANDLE *pIveHandle,
                         IVE_SRC_IMAGE_S *pstSrc,
                         IVE_DST_IMAGE_S *pstDst,
                         IVE_THRESH_CTRL_S *pstThrCtrl,
                         HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_And(IVE_HANDLE *pIveHandle,
                      IVE_SRC_IMAGE_S *pstSrc1,
                      IVE_SRC_IMAGE_S *pstSrc2,
                      IVE_DST_IMAGE_S *pstDst,
                      HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_Sub(IVE_HANDLE *pIveHandle,
                      IVE_SRC_IMAGE_S *pstSrc1,
                      IVE_SRC_IMAGE_S *pstSrc2,
                      IVE_DST_IMAGE_S *pstDst,
                      IVE_SUB_CTRL_S *pstSubCtrl,
                      HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_Or(IVE_HANDLE *pIveHandle,
                     IVE_SRC_IMAGE_S *pstSrc1,
                     IVE_SRC_IMAGE_S *pstSrc2,
                     IVE_DST_IMAGE_S *pstDst,
                     HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_Integ(IVE_HANDLE *pIveHandle,
                        IVE_SRC_IMAGE_S *pstSrc,
                        IVE_DST_IMAGE_S *pstDst,
                        IVE_INTEG_CTRL_S *pstIntegCtrl,
                        HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_Hist(IVE_HANDLE *pIveHandle,
                       IVE_SRC_IMAGE_S *pstSrc,
                       IVE_DST_MEM_INFO_S *pstDst,
                       HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_Thresh_S16(IVE_HANDLE *pIveHandle,
                             IVE_SRC_IMAGE_S *pstSrc,
                             IVE_DST_IMAGE_S *pstDst,
                             IVE_THRESH_S16_CTRL_S *pstThrS16Ctrl,
                             HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_Thresh_U16(IVE_HANDLE *pIveHandle,
                             IVE_SRC_IMAGE_S *pstSrc,
                             IVE_DST_IMAGE_S *pstDst,
                             IVE_THRESH_U16_CTRL_S *pstThrU16Ctrl,
                             HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_16BitTo8Bit(IVE_HANDLE *pIveHandle,
                              IVE_SRC_IMAGE_S *pstSrc,
                              IVE_DST_IMAGE_S *pstDst,
                              IVE_16BIT_TO_8BIT_CTRL_S *pst16BitTo8BitCtrl,
                              HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_OrdStatFilter(IVE_HANDLE *pIveHandle,
                                IVE_SRC_IMAGE_S *pstSrc,
                                IVE_DST_IMAGE_S *pstDst,
                                IVE_ORD_STAT_FILTER_CTRL_S *pstOrdStatFltCtrl,
                                HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_Add(IVE_HANDLE *pIveHandle,
                      IVE_SRC_IMAGE_S *pstSrc1,
                      IVE_SRC_IMAGE_S *pstSrc2,
                      IVE_DST_IMAGE_S *pstDst,
                      IVE_ADD_CTRL_S *pstAddCtrl,
                      HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_Xor(IVE_HANDLE *pIveHandle,
                      IVE_SRC_IMAGE_S *pstSrc1,
                      IVE_SRC_IMAGE_S *pstSrc2,
                      IVE_DST_IMAGE_S *pstDst,
                      HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_CCL(IVE_HANDLE *pIveHandle,
                      IVE_SRC_IMAGE_S *pstSrc,
                      IVE_DST_MEM_INFO_S *pstBlob,
                      IVE_CCL_CTRL_S *pstCclCtrl,
                      HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_CannyHysEdge(IVE_HANDLE *pIveHandle,
                               IVE_SRC_IMAGE_S *pstSrc,
                               IVE_DST_IMAGE_S *pstEdge,
                               IVE_DST_MEM_INFO_S *pstStack,
                               IVE_CANNY_HYS_EDGE_CTRL_S *pstCannyHysEdgeCtrl,
                               HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_CannyEdge(IVE_SRC_IMAGE_S *pstEdge,
                            IVE_SRC_MEM_INFO_S *pstStack);

HI_S32 HI_MPI_IVE_NormGrad(IVE_HANDLE *pIveHandle,
                           IVE_SRC_IMAGE_S *pstSrc,
                           IVE_DST_IMAGE_S *pstDstH,
                           IVE_DST_IMAGE_S *pstDstV,
                           IVE_DST_IMAGE_S *pstDstHV,
                           IVE_NORM_GRAD_CTRL_S *pstNormGradCtrl,
                           HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_LKOpticalFlowPyr(IVE_HANDLE *pIveHandle,
                                   IVE_SRC_IMAGE_S astSrcPrevPyr[],
                                   IVE_SRC_IMAGE_S astSrcNextPyr[],
                                   IVE_SRC_MEM_INFO_S *pstPrevPts,
                                   IVE_MEM_INFO_S *pstNextPts,
                                   IVE_DST_MEM_INFO_S *pstStatus,
                                   IVE_DST_MEM_INFO_S *pstErr,
                                   IVE_LK_OPTICAL_FLOW_PYR_CTRL_S *pstLkOptiFlowPyrCtrl,
                                   HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_STCandiCorner(IVE_HANDLE *pIveHandle,
                                IVE_SRC_IMAGE_S *pstSrc,
                                IVE_DST_IMAGE_S *pstCandiCorner,
                                IVE_ST_CANDI_CORNER_CTRL_S *pstStCandiCornerCtrl,
                                HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_STCorner(IVE_SRC_IMAGE_S *pstCandiCorner,
                           IVE_DST_MEM_INFO_S *pstCorner,
                           IVE_ST_CORNER_CTRL_S *pstStCornerCtrl);

HI_S32 HI_MPI_IVE_GMM2(IVE_HANDLE *pIveHandle,
                       IVE_SRC_IMAGE_S *pstSrc,
                       IVE_SRC_IMAGE_S *pstFactor,
                       IVE_DST_IMAGE_S *pstFg,
                       IVE_DST_IMAGE_S *pstBg,
                       IVE_DST_IMAGE_S *pstMatchModelInfo,
                       IVE_MEM_INFO_S  *pstModel,
                       IVE_GMM2_CTRL_S *pstGmm2Ctrl,
                       HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_SAD(IVE_HANDLE *pIveHandle,
                      IVE_SRC_IMAGE_S *pstSrc1,
                      IVE_SRC_IMAGE_S *pstSrc2,
                      IVE_DST_IMAGE_S *pstSad,
                      IVE_DST_IMAGE_S *pstThr,
                      IVE_SAD_CTRL_S *pstSadCtrl,
                      HI_BOOL bInstant);

/* --- CNN model --- */

HI_S32 HI_MPI_IVE_CNN_LoadModel(const HI_CHAR *pchFileName,
                                IVE_CNN_MODEL_S *pstCnnModel);

HI_S32 HI_MPI_IVE_CNN_UnloadModel(IVE_CNN_MODEL_S *pstCnnModel);

HI_S32 HI_MPI_IVE_CNN_Predict(IVE_HANDLE *pIveHandle,
                              IVE_SRC_IMAGE_S *pstSrc,
                              IVE_CNN_MODEL_S *pstCnnModel,
                              IVE_DST_DATA_S *pstDst,
                              IVE_CNN_CTRL_S *pstCnnCtrl,
                              HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_CNN_GetResult(IVE_SRC_DATA_S *pstSrc,
                                IVE_DST_MEM_INFO_S *pstDst,
                                IVE_CNN_MODEL_S *pstCnnModel,
                                IVE_CNN_CTRL_S *pstCnnCtrl);

/*
 * -----------------------------------------------------------------------
 * Unsupported-on-EV200/EV300 ops
 * -----------------------------------------------------------------------
 * These wrappers are declared so test-ive-mpi.c and similar comprehensive
 * op-test binaries can link against libive_neo. The implementations
 * return HI_ERR_IVE_NOT_SUPPORT immediately without issuing any ioctl
 * (vendor open_ive.ko also rejects them on this SoC).
 */

HI_S32 HI_MPI_IVE_CSC(IVE_HANDLE *pIveHandle,
                      IVE_SRC_IMAGE_S *pstSrc,
                      IVE_DST_IMAGE_S *pstDst,
                      IVE_CSC_CTRL_S *pstCscCtrl,
                      HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_FilterAndCSC(IVE_HANDLE *pIveHandle,
                               IVE_SRC_IMAGE_S *pstSrc,
                               IVE_DST_IMAGE_S *pstDst,
                               IVE_FILTER_AND_CSC_CTRL_S *pstFltCscCtrl,
                               HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_Resize(IVE_HANDLE *pIveHandle,
                         IVE_SRC_IMAGE_S astSrc[],
                         IVE_DST_IMAGE_S astDst[],
                         IVE_RESIZE_CTRL_S *pstResizeCtrl,
                         HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_EqualizeHist(IVE_HANDLE *pIveHandle,
                               IVE_SRC_IMAGE_S *pstSrc,
                               IVE_DST_IMAGE_S *pstDst,
                               IVE_EQUALIZE_HIST_CTRL_S *pstEqualizeHistCtrl,
                               HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_LBP(IVE_HANDLE *pIveHandle,
                      IVE_SRC_IMAGE_S *pstSrc,
                      IVE_DST_IMAGE_S *pstDst,
                      IVE_LBP_CTRL_S *pstLbpCtrl,
                      HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_Map(IVE_HANDLE *pIveHandle,
                      IVE_SRC_IMAGE_S *pstSrc,
                      IVE_SRC_MEM_INFO_S *pstMap,
                      IVE_DST_IMAGE_S *pstDst,
                      IVE_MAP_CTRL_S *pstMapCtrl,
                      HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_NCC(IVE_HANDLE *pIveHandle,
                      IVE_SRC_IMAGE_S *pstSrc1,
                      IVE_SRC_IMAGE_S *pstSrc2,
                      IVE_DST_MEM_INFO_S *pstDst,
                      HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_GMM(IVE_HANDLE *pIveHandle,
                      IVE_SRC_IMAGE_S *pstSrc,
                      IVE_DST_IMAGE_S *pstFg,
                      IVE_DST_IMAGE_S *pstBg,
                      IVE_MEM_INFO_S  *pstModel,
                      IVE_GMM_CTRL_S *pstGmmCtrl,
                      HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_GradFg(IVE_HANDLE *pIveHandle,
                         IVE_SRC_IMAGE_S *pstBgDiffFg,
                         IVE_SRC_IMAGE_S *pstCurGrad,
                         IVE_SRC_IMAGE_S *pstBgGrad,
                         IVE_DST_IMAGE_S *pstGradFg,
                         IVE_GRAD_FG_CTRL_S *pstGradFgCtrl,
                         HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_MatchBgModel(IVE_HANDLE *pIveHandle,
                               IVE_SRC_IMAGE_S *pstCurImg,
                               IVE_DATA_S *pstBgModel,
                               IVE_DST_IMAGE_S *pstFgFlag,
                               IVE_DST_IMAGE_S *pstBgDiffFg,
                               IVE_DST_IMAGE_S *pstFrmDiffFg,
                               IVE_DST_MEM_INFO_S *pstStatData,
                               IVE_MATCH_BG_MODEL_CTRL_S *pstMatchBgModelCtrl,
                               HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_UpdateBgModel(IVE_HANDLE *pIveHandle,
                                IVE_SRC_IMAGE_S *pstCurImg,
                                IVE_DATA_S *pstBgModel,
                                IVE_DST_IMAGE_S *pstFgFlag,
                                IVE_DST_IMAGE_S *pstBgImg,
                                IVE_DST_IMAGE_S *pstChgStaImg,
                                IVE_DST_IMAGE_S *pstChgStaLife,
                                IVE_DST_IMAGE_S *pstChgStaFaith,
                                IVE_DST_MEM_INFO_S *pstStatData,
                                IVE_UPDATE_BG_MODEL_CTRL_S *pstUpdateBgModelCtrl,
                                HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_ANN_MLP_LoadModel(const HI_CHAR *pchFileName,
                                    IVE_ANN_MLP_MODEL_S *pstAnnMlpModel);
HI_S32 HI_MPI_IVE_ANN_MLP_UnloadModel(IVE_ANN_MLP_MODEL_S *pstAnnMlpModel);
HI_S32 HI_MPI_IVE_ANN_MLP_Predict(IVE_HANDLE *pIveHandle,
                                  IVE_SRC_DATA_S *pstSrc,
                                  IVE_ANN_MLP_MODEL_S *pstAnnMlpModel,
                                  IVE_DST_MEM_INFO_S *pstDst,
                                  HI_BOOL bInstant);

HI_S32 HI_MPI_IVE_SVM_LoadModel(const HI_CHAR *pchFileName,
                                IVE_SVM_MODEL_S *pstSvmModel);
HI_S32 HI_MPI_IVE_SVM_UnloadModel(IVE_SVM_MODEL_S *pstSvmModel);
HI_S32 HI_MPI_IVE_SVM_Predict(IVE_HANDLE *pIveHandle,
                              IVE_SRC_DATA_S *pstSrc,
                              IVE_SVM_MODEL_S *pstSvmModel,
                              IVE_DST_MEM_INFO_S *pstDst,
                              HI_BOOL bInstant);

/* Perspective transform — cv500-only HW op. Submits up to 8 ROIs in
 * a single call; each ROI gets its own point-pair table and its own
 * dst image. enAlgMode picks similarity / non-reflective-similarity /
 * affine; enCscMode does an optional YUV→RGB conversion on the output.
 * Returns 0 on success with the handle written to *pIveHandle. */
HI_S32 HI_MPI_IVE_PerspTrans(IVE_HANDLE *pIveHandle,
                             IVE_SRC_IMAGE_S *pstSrc,
                             IVE_RECT_U32_S astRoi[],
                             IVE_SRC_MEM_INFO_S astPointPair[],
                             IVE_DST_IMAGE_S astDst[],
                             IVE_PERSP_TRANS_CTRL_S *pstPerspTransCtrl,
                             HI_BOOL bInstant);

/* HOG descriptor extraction — cv500-only HW op. Submits up to 8 ROIs
 * in a single call; each ROI gets its own IVE_DST_BLOB_S output that
 * receives the per-cell histogram of oriented gradients. enHogMode
 * picks vertical (1) or horizontal (2) tangent plane.
 * Returns 0 on success with the handle written to *pIveHandle. */
HI_S32 HI_MPI_IVE_Hog(IVE_HANDLE *pIveHandle,
                      IVE_SRC_IMAGE_S *pstSrc,
                      IVE_RECT_U32_S astRoi[],
                      IVE_DST_BLOB_S astDst[],
                      IVE_HOG_CTRL_S *pstHogCtrl,
                      HI_BOOL bInstant);

#ifdef __cplusplus
}
#endif

#endif /* __MPI_IVE_H__ */
