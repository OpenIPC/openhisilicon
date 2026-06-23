/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef _SS_MPI_IVE_H_
#define _SS_MPI_IVE_H_

#include "ot_debug.h"
#include "ot_common_ive.h"
#include "ot_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Prototype    : ss_mpi_ive_dma
 * Description  : Direct memory access (DMA):
 * 1. Direct memory copy;
 * 2. Copy with interval bytes;
 * 3. Memset using 3 bytes;
 * 4. Memset using 8 bytes;
 * Parameters   : ot_ive_handle          *handle         Returned handle ID of a task.
 * ot_svp_data            *src            Input source data.The input data is treated as U8C1 data.
 * ot_svp_dst_data        *dst            Output result data.
 * ot_ive_dma_ctrl        *ctrl           DMA control parameter.
 * td_bool                is_instant      Flag indicating whether to generate an interrupt.
 * If the output result blocks the next operation,
 * set is_instant to TD_TRUE.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 *
 */
td_s32 ss_mpi_ive_dma(ot_ive_handle *handle, const ot_svp_data *src, const ot_svp_dst_data *dst,
    const ot_ive_dma_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_filter
 * Description  : 5x5 template filter.
 * Parameters   : ot_ive_handle           *handle          Returned handle ID of a task
 * ot_svp_src_img          *src             Input source data.
 * The U8C1,SP420 and SP422 input formats are supported.
 * ot_svp_dst_img          *dst             Output result, of same type with the input.
 * ot_ive_filter_ctrl      *ctrl            Control parameters of filter
 * td_bool                 is_instant       For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_filter(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *dst,
    const ot_ive_filter_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_csc
 * Description  : YUV2RGB\YUV2HSV\YUV2LAB\RGB2YUV color space conversion are supported.
 * Parameters   : ot_ive_handle           *handle        Returned handle ID of a task
 * ot_svp_src_img          *src           Input source data:
 * 1. SP420\SP422 type for YUV2RGB\YUV2HSV\YUV2LAB;
 * 2. U8C3_PACKAGE\U8C3_PLANAR type for RGB2YUV;
 * ot_svp_dst_img          *dst           Output result:
 * 1. U8C3_PACKAGE\U8C3_PLANAR typed for YUV2RGB\YUV2HSV\YUV2LAB;
 * 2. SP420\SP422 type for RGB2YUV;
 * ot_ive_csc_ctrl         *ctrl          Control parameters for CSC
 * td_bool                 is_instant     For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_csc(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *dst,
    const ot_ive_csc_ctrl *ctrl, td_bool is_instant);


/*
 * Prototype    : ss_mpi_ive_filter_and_csc
 * Description  : Only support YUV2RGB color space conversion.
 * Parameters   : ot_ive_handle               *handle       Returned handle ID of a task.
 * ot_svp_src_img              *src          Input source data.Only SP420\SP422 type are supported.
 * ot_svp_dst_img              *dst          Output result.Only U8C3_PACKAGE\U8C3_PLANAR are supported.
 * ot_ive_filter_and_csc_ctrl  *ctrl         Control parameters.
 * td_bool                     is_instant    For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_filter_and_csc(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *dst,
    const ot_ive_filter_and_csc_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_sobel
 * Description  : SOBEL is used to extract the gradient information.
 * Parameters   : ot_ive_handle           *handle      Returned handle ID of a task
 * ot_svp_src_img          *src         Input source data. Only the U8C1 input image is supported.
 * ot_svp_dst_img          *dst_h       The (horizontal) result of input image filtered
 * by the input mask;
 * ot_svp_dst_img          *dst_v       The (vertical) result  of input image filtered
 * by the transposed mask;
 * ot_ive_sobel_ctrl       *ctrl        Control parameters
 * td_bool                 is_instant   For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_sobel(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *dst_h,
    const ot_svp_dst_img *dst_v, const ot_ive_sobel_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_mag_and_ang
 * Description  : mag_and_ang is used to extract the edge information.
 * Parameters   : ot_ive_handle           *handle         Returned handle ID of a task
 * ot_svp_src_img          *src            Input source data. Only the U8C1 input format is supported.
 * ot_svp_dst_img          *dst_mag        Output magnitude.
 * ot_svp_dst_img          *dst_ang        Output angle.
 * If the output mode is set to magnitude only,
 * this item can be set to null.
 * ot_ive_mag_and_ang_ctrl *ctrl           Control parameters
 * td_bool                 is_instant      For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_mag_and_ang(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *dst_mag,
    const ot_svp_dst_img *dst_ang, const ot_ive_mag_and_ang_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_dilate
 * Description  : 5x5 template dilate. Only the U8C1 binary image input is supported.
 * Or else the result is not expected.
 * Parameters   : ot_ive_handle           *handle          Returned handle ID of a task
 * ot_svp_src_img          *src             Input binary image, which consists of 0 or 255;
 * ot_svp_dst_img          *dst             Output result.
 * ot_ive_dilate_ctrl      *ctrl            Control parameters.
 * td_bool                 is_instant       For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_dilate(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *dst,
    const ot_ive_dilate_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_erode
 * Parameters   : 5x5 template erode. Only the U8C1 binary image input is supported.Or else the result is not correct.
 * Input        : ot_ive_handle               *handle       Returned handle ID of a task
 * ot_svp_src_img              *src          Input binary image, which consists of 0 or 255;
 * ot_svp_dst_img              *dst          Output result.
 * ot_ive_erode_ctrl           *ctrl         Control parameters
 * td_bool                     is_instant    For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_erode(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *dst,
    const ot_ive_erode_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_threshold
 * Description  : Thresh operation to the input image.
 * Parameters   : ot_ive_handle           *handle         Returned handle ID of a task
 * ot_svp_src_img          *src            Input source data. Only the U8C1 input format is supported.
 * ot_svp_dst_img          *dst            Output result
 * ot_ive_threshold_ctrl   *ctrl           Control parameters
 * td_bool                 is_instant      For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_threshold(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *dst,
    const ot_ive_threshold_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_and
 * Description  : Binary images' And operation.
 * Parameters   : ot_ive_handle       *handle        Returned handle ID of a task
 * ot_svp_src_img      *src1          The input source1. Only U8C1 input format is supported.
 * ot_svp_src_img      *src2          The input source2.Only U8C1 input format is supported.
 * ot_svp_dst_img      *dst           Output result of " src1 & src2 ".
 * td_bool             is_instant     For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_and(ot_ive_handle *handle, const ot_svp_src_img *src1, const ot_svp_src_img *src2,
    const ot_svp_dst_img *dst, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_sub
 * Description  : Two gray images' Sub operation.
 * Parameters   : ot_ive_handle       *handle       Returned handle ID of a task
 * ot_svp_src_img      *src1         Minuend of the input source.Only the U8C1 input format
 * is supported.
 * ot_svp_src_img      *src2         Subtrahend of the input source.Only the U8C1 input format
 * is supported.
 * ot_svp_dst_img      *dst          Output result of src1 minus src2
 * ot_ive_sub_ctrl     *ctrl         Control parameter
 * td_bool             is_instant    For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_sub(ot_ive_handle *handle, const ot_svp_src_img *src1, const ot_svp_src_img *src2,
    const ot_svp_dst_img *dst, const ot_ive_sub_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_or
 * Description  : Two binary images' Or operation.
 * Parameters   : ot_ive_handle           *handle     Returned handle ID of a task
 * ot_svp_src_img          *src1         Input source1. Only the U8C1 input format is supported.
 * ot_svp_src_img          *src2         Input source2. Only the U8C1 input format is supported.
 * ot_svp_dst_img          *dst          Output result src1 or src2
 * td_bool                 is_instant    For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_or(ot_ive_handle *handle, const ot_svp_src_img *src1, const ot_svp_src_img *src2,
    const ot_svp_dst_img *dst, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_integ
 * Description  : Calculate the input gray image's integral image.
 * Parameters   : ot_ive_handle         *handle         Returned handle ID of a task
 * ot_svp_src_img        *src            Input source data.Only the U8C1 input format is supported.
 * ot_svp_dst_img        *dst            Output result.Can be U32C1 or U64C1, relied on the
 * control parameter.
 * ot_ive_integ_ctrl     *ctrl           Integ Control
 * td_bool               is_instant      For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_integ(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *dst,
    const ot_ive_integ_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_hist
 * Description  : Calculate the input gray image's histogram.
 * Parameters   : ot_ive_handle          *handle         Returned handle ID of a task
 * ot_svp_src_img         *src            Input source data. Only the U8C1 input format is supported.
 * ot_svp_dst_mem_info    *dst            Output result.
 * td_bool                is_instant      For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_hist(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_mem_info *dst,
    td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_threshold_s16
 * Description  : S16 image's THRESH operation.
 * Parameters   : ot_ive_handle             *handle         Returned handle ID of a task
 * ot_svp_src_img            *src            Input source data.Only the S16 input format is supported.
 * ot_svp_dst_img            *dst            Output result.
 * ot_ive_threshold_s16_ctrl *ctrl           Control parameters
 * td_bool                   is_instant      For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_threshold_s16(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *dst,
    const ot_ive_threshold_s16_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_threshold_u16
 * Description  : U16 image's THRESH operation.
 * Parameters   : ot_ive_handle              *handle        Returned handle ID of a task
 * ot_svp_src_img             *src           Input source data. Only the U16 input format is supported.
 * ot_svp_dst_img             *dst           Output result
 * ot_ive_threshold_u16_ctrl  *ctrl          Control parameters
 * td_bool                    is_instant     For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_threshold_u16(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *dst,
    const ot_ive_threshold_u16_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_16bit_to_8bit
 * Description  : Scale the input 16bit data to the output 8bit data.
 * Parameters   : ot_ive_handle              *handle            Returned handle ID of a task
 * ot_svp_src_img             *src               Input source data.Only U16C1\S16C1 input is supported.
 * ot_svp_dst_img             *dst               Output result
 * ot_ive_16bit_to_8bit_ctrl  *ctrl              control parameter
 * td_bool                    is_instant         For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_16bit_to_8bit(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *dst,
    const ot_ive_16bit_to_8bit_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_ord_stat_filter
 * Description  : Order Statistic Filter. It can be used as median\max\min value filter.
 * Parameters   : ot_ive_handle                 *handle           Returned handle ID of a task
 * ot_svp_src_img                *src              Input source data. Only U8C1 input is supported
 * ot_svp_dst_img                *dst              Output result
 * ot_ive_ord_stat_filter_ctrl   *ctrl             Control parameter
 * td_bool                       is_instant        For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_order_stats_filter(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *dst,
    const ot_ive_order_stats_filter_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_map
 * Description  : Map a image to another through a lookup table.
 * Parameters   : ot_ive_handle         *handle       Returned handle ID of a task
 * ot_svp_src_img        *src          Input source. Only the U8C1 input format is supported.
 * ot_svp_src_mem_info   *map          Input lookup table. Must be an U8 array of size 256.
 * ot_svp_dst_img        *dst          Output result.
 * ot_ive_map_ctrl       *ctrl         Map control parameter.
 * td_bool               is_instant    For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_map(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_src_mem_info *map,
    const ot_svp_dst_img *dst, const ot_ive_map_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_equalize_hist
 * Description  : Enhance the input image's contrast through histogram equalization.
 * Parameters   : ot_ive_handle               *handle              Returned handle ID of a task
 * ot_svp_src_img              *src                 Input source.Only U8C1 input format is supported.
 * ot_svp_dst_img              *dst                 Output result.
 * ot_ive_equalize_hist_ctrl   *ctrl                EqualizeHist control parameter.
 * td_bool                     is_instant           For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_equalize_hist(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *dst,
    const ot_ive_equalize_hist_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_add
 * Description  : Two gray images' Add operation.
 * Parameters   : ot_ive_handle        *handle     Returned handle ID of a task
 * ot_svp_src_img       *src1       Augend of the input source.Only the U8C1 input format is supported.
 * ot_svp_src_img       *src2       Addend of the input source.Only the U8C1 input format is supported.
 * ot_svp_dst_img       *dst        Output result of src1 plus src2
 * ot_ive_add_ctrl      *ctrl       Control parameter
 * td_bool              is_instant    For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_add(ot_ive_handle *handle, const ot_svp_src_img *src1, const ot_svp_src_img *src2,
    const ot_svp_dst_img *dst, const ot_ive_add_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_xor
 * Description  : Two binary images' Xor operation.
 * Parameters   : ot_ive_handle         *handle     Returned handle ID of a task
 * ot_svp_src_img        *src1       The input source1.Only the U8C1 input format is supported.
 * ot_svp_src_img        *src2       The input source2.
 * ot_svp_dst_img        *dst        Output result
 * td_bool               is_instant  For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_xor(ot_ive_handle *handle, const ot_svp_src_img *src1, const ot_svp_src_img *src2,
    const ot_svp_dst_img *dst, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_ncc
 * Description  : Calculate two gray images' NCC (Normalized Cross Correlation).
 * Parameters   : ot_ive_handle          *handle       Returned handle ID of a task
 * ot_svp_src_img         *src1         Input source1. Only the U8C1 input format is supported.
 * ot_svp_src_img         *src2         Input source2. Must be of the same type, size of source1.
 * ot_svp_dst_mem_info    *dst          Output result
 * td_bool                is_instant    For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_ncc(ot_ive_handle *handle, const ot_svp_src_img *src1, const ot_svp_src_img *src2,
    const ot_svp_dst_mem_info *dst, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_ccl
 * Description  : Connected Component Labeling. Only 8-Connected method is supported.
 * Parameters   : ot_ive_handle         *handle        Returned handle ID of a task
 * ot_svp_img            *src_dst       Input source
 * ot_svp_dst_mem_info   *blob          Output result of detected region;
 * ot_ive_ccl_ctrl       *ctrl          CCL control parameter
 * td_bool               is_instant     For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_ccl(ot_ive_handle *handle, const ot_svp_img *src_dst, const ot_svp_dst_mem_info *blob,
    const ot_ive_ccl_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_gmm
 * Description  : Separate foreground and background using GMM(Gaussian Mixture Model) method;
 * Gray or RGB GMM are supported.
 * Parameters   : ot_ive_handle        *handle      Returned handle ID of a task
 * ot_svp_src_img       *src         Input source. Only support U8C1 or U8C3_PACKAGE input.
 * ot_svp_dst_img       *fg          Output foreground (Binary) image.
 * ot_svp_dst_img       *bg          Output background image. Of the same type of pstSrc.
 * ot_svp_mem_info      *model       Model data.
 * ot_ive_gmm_ctrl      *ctrl        Control parameter.
 * td_bool              is_instant   For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_gmm(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *fg,
    const ot_svp_dst_img *bg, const ot_svp_mem_info *model, const ot_ive_gmm_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_gmm2
 * Description  : Separate foreground and background using GMM(Gaussian Mixture Model) method;
 * Gray or RGB GMM are supported.
 * Parameters   : ot_ive_handle           *handle            Returned handle ID of a task
 * ot_svp_src_img          *src               Only U8C1 or U8C3_PACKAGE input are supported.
 * ot_svp_src_img          *factor            U16C1 input, low-8bits is sensitivity factor,
 * and high-8bits is life update factor.
 * ot_svp_dst_img          *fg                Output foreground (Binary) image.
 * ot_svp_dst_img          *bg                Output background image. With same type of pstSrc.
 * ot_svp_dst_img          *match_model_info  Output U8C1 match model info image. Low-1bit is match
 * flag, and high-7bits is max freq index.
 * ot_svp_mem_info         *model             Model data.
 * ot_ive_gmm2_ctrl        *ctrl              Control parameter.
 * td_bool                 is_instant         For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_gmm2(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_src_img *factor,
    const ot_svp_dst_img *fg, const ot_svp_dst_img *bg, const ot_svp_dst_img *match_model_info,
    const ot_svp_mem_info *model, const ot_ive_gmm2_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_canny_hys_edge
 * Description  : The first part of canny Edge detection. Including step: gradient calculation,
 * magnitude and angle calculation, hysteresis threshold, NMS(Non-Maximum Suppression)
 * Parameters   : ot_ive_handle                   *handle       Returned handle ID of a task
 * ot_svp_src_img                  *src          Input source. Only the U8C1 input format is supported
 * ot_svp_dst_img                  *edge         Output result.
 * ot_svp_dst_mem_info             *stack        OutPut stack for CannyEdge
 * ot_ive_canny_hys_edge_ctrl      *ctrl         Control parameter.
 * td_bool                         is_instant    For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_canny_hys_edge(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *edge,
    const ot_svp_dst_mem_info *stack, const ot_ive_canny_hys_edge_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_canny_edge
 * Description  : The second part of canny Edge detection: trace strong edge by weak edge.
 * Parameters   : ot_svp_img          *edge        Input and Output source. Only the U8C1 format is supported
 * ot_svp_mem_info     *stack       stack for CannyEdge
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_canny_edge(const ot_svp_img *edge, const ot_svp_mem_info *stack);

/*
 * Prototype    : ss_mpi_ive_lbp
 * Description  : LBP calculation using the original method and a extensional method.
 * Parameters   : ot_ive_handle       *handle      Returned handle ID of a task
 * ot_svp_src_img      *src         Input source.Only the U8C1 input format is supported.
 * ot_svp_dst_img      *dst         Output result
 * ot_ive_lbp_ctrl     *ctrl        Control parameter
 * td_bool             is_instant   For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_lbp(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *dst,
    const ot_ive_lbp_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_norm_grad
 * Description  : Gradient calculation and the output is normalized to S8.
 * Parameters   : ot_ive_handle         *handle          Returned handle ID of a task
 * ot_svp_src_img        *src             Input source data
 * ot_svp_dst_img        *dst_h           The (horizontal) result of input image filtered
 * by the input mask;
 * ot_svp_dst_img        *dst_v           The (vertical) result  of input image filtered
 * by the transposed mask;
 * ot_svp_dst_img        *dst_hv          Output the horizontal and vertical component in
 * single image in package format.
 * ot_ive_norm_grad_ctrl *ctrl            Control parameter
 * td_bool               is_instant       For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_norm_grad(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *dst_h,
    const ot_svp_dst_img *dst_v, const ot_svp_dst_img *dst_hv, const ot_ive_norm_grad_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_lk_optical_flow_pyr
 * Description  : Calculate LK Optical Flow using multi-layer of the pyramid-images.
 * Parameters   : ot_ive_handle                 *handle          Returned handle ID of a task
 * ot_svp_src_img                src_prev_pyr     Prev-frame's pyramid. Must be U8C1 images.
 * ot_svp_src_img                src_next_pyr     Next-frame's pyramid. Same size and type
 * with src_prev_pyr.
 * ot_svp_src_mem_info           *prev_points     Interesting points on src_prev_pyr[0].
 * ot_svp_mem_info               *next_points     Output points. When use_init_flow is true,
 * must have the same size of prev_points as input.
 * ot_svp_dst_mem_info           *status          The status of feature points
 * ot_svp_dst_mem_info           *err             The similarity between the successfully trucked
 * points in next_points with corresponding points
 * in prev_points.
 * ot_ive_lk_optical_flow_pyr_ctrl *ctrl          Control parameters.
 * td_bool                         is_instant     For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_lk_optical_flow_pyr(ot_ive_handle *handle, const ot_svp_src_img src_prev_pyr[],
    const ot_svp_src_img src_next_pyr[], const ot_svp_src_mem_info *prev_points, const ot_svp_mem_info *next_points,
    const ot_svp_dst_mem_info *status, const ot_svp_dst_mem_info *err, const ot_ive_lk_optical_flow_pyr_ctrl *ctrl,
    td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_st_cand_corner
 * Description  : The first part of corners detection using Shi-Tomasi-like method: calculate candidate corners.
 * Parameters   : ot_ive_handle                *handle           Returned handle ID of a task
 * ot_svp_src_img               *src              Input source data
 * ot_svp_dst_img               *cand_corner      Output result of eig
 * ot_ive_st_cand_corner_ctrl   *ctrl             Control parameter
 * td_bool                      is_instant        For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_st_cand_corner(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_dst_img *cand_corner,
    const ot_ive_st_cand_corner_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_st_corner
 * Description  : The second part of corners detection using Shi-Tomasi-like method: select corners by certain rules.
 * ot_svp_src_img         *cand_corner     Input source data
 * ot_svp_dst_mem_info      *corner           Output result of Corner
 * ot_ive_st_corner_ctrl    *ctrl             Control parameter
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_st_corner(const ot_svp_src_img *cand_corner, const ot_svp_dst_mem_info *corner,
    const ot_ive_st_corner_ctrl *ctrl);

/*
 * Prototype    : ss_mpi_ive_grad_fg
 * Description  : Calculate the gradient of frontground
 * Parameters   : ot_ive_handle        *handle          Returned handle ID of a task
 * ot_svp_src_img       *bg_diff_fg      Background subtraction foreground image
 * ot_svp_src_img       *cur_grad        Current gradient image, both horizontally and vertically
 * graded in accordance with [xyxyxy ...] format
 * ot_svp_src_img       *bg_grad         Background gradient image
 * ot_svp_dst_img       *grad_fg         Gradient foreground image
 * ot_ive_grad_fg_ctrl  *ctrl            Gradient calculation parameters
 * td_bool              is_instant       For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_grad_fg(ot_ive_handle *handle, const ot_svp_src_img *bg_diff_fg, const ot_svp_src_img *cur_grad,
    const ot_svp_src_img *bg_grad, const ot_svp_dst_img *grad_fg, const ot_ive_grad_fg_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_match_bg_model
 * Description  : Match background model
 * Parameters   : ot_ive_handle              *handle          Returned handle ID of a task
 * ot_svp_src_img             *cur_img         Current grayscale image
 * ot_svp_data                *bg_model        Background model data
 * ot_svp_img                 *fg_flag         Foreground status image
 * ot_svp_dst_img             *bg_diff_fg      Foreground image obtained by background matching,
 * the background pixel value is 0, the foreground pixel
 * value is the gray difference value
 * ot_svp_dst_img             *frame_diff_fg     Foreground image obtained by interframe difference,
 * the background pixel value is 0, the foreground pixel
 * value is the gray difference value
 * ot_svp_dst_mem_info        *state_data       result status data
 * ot_ive_match_bg_model_ctrl *ctrl            Background matching parameters
 * td_bool                    is_instant       For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_match_bg_model(ot_ive_handle *handle, const ot_svp_src_img *cur_img, const ot_svp_data *bg_model,
    const ot_svp_img *fg_flag, const ot_svp_dst_img *bg_diff_fg, const ot_svp_dst_img *frame_diff_fg,
    const ot_svp_dst_mem_info *state_data, const ot_ive_match_bg_model_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_update_bg_model
 * Description  : Update background model
 * Parameters   : ot_ive_handle               *handle             Returned handle ID of a task
 * ot_svp_data                 *bg_model           Background model data
 * ot_svp_img                  *fg_flag            Foreground status image
 * ot_svp_dst_img              *bg_img             Background grayscale image
 * ot_svp_dst_img              *chg_status_img     Change state life image, for still detection
 * ot_svp_dst_img              *chg_status_fg      Change state grayscale image, for still detection
 * ot_svp_dst_img              *chg_status_life    Change state foreground image, for still detection
 * ot_svp_dst_mem_info         *state_data         result status data
 * ot_ive_update_bg_model_ctrl *ctrl               Background update parameters
 * td_bool                     is_instant          For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_update_bg_model(ot_ive_handle *handle, const ot_svp_data *bg_model, const ot_svp_img *fg_flag,
    const ot_svp_dst_img *bg_img, const ot_svp_dst_img *chg_status_img, const ot_svp_dst_img *chg_status_fg,
    const ot_svp_dst_img *chg_status_life, const ot_svp_dst_mem_info *state_data,
    const ot_ive_update_bg_model_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_ann_mlp_load_model
 * Description  : Load ANN_MLP model data from ".bin" file.
 * Parameters   : td_char                  *file_name        ANN_MLP model file name, must be ".bin" file.
 * ot_ive_ann_mlp_model      *model           ANN_MLP model data.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_ann_mlp_load_model(const td_char *file_name, ot_ive_ann_mlp_model *model);

/*
 * Prototype    : ss_mpi_ive_ann_mlp_unload_model
 * Description  : Unload ANN_MLP model data.
 * Parameters   : ot_ive_ann_mlp_model      *model        ANN_MLP model data.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 *
 */
td_void ss_mpi_ive_ann_mlp_unload_model(const ot_ive_ann_mlp_model *model);

/*
 * Prototype    : ss_mpi_ive_ann_mlp_predict
 * Description  : ANN predict
 * Parameters   : ot_ive_handle           *handle                 Returned handle ID of a task
 * ot_svp_src_data         *src                    Input samples
 * ot_svp_lut              *actv_func_table        Look-up table for active function
 * ot_ive_ann_mlp_model    *model                  ANN_MLP model
 * ot_svp_dst_data         *dst                    Output layers of every input sample
 * td_bool                 is_instant              For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_ann_mlp_predict(ot_ive_handle *handle, const ot_svp_src_data *src, const ot_svp_lut *actv_func_table,
    const ot_ive_ann_mlp_model *model, const ot_svp_dst_data *dst, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_svm_load_model
 * Description  : Load SVM model data from ".bin" file.
 * Parameters   : td_char               *file_name        SVM model file name, must be ".bin" file.
 * ot_ive_svm_model      *svm_model        SVM model data.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_svm_load_model(const td_char *file_name, ot_ive_svm_model *svm_model);

/*
 * Prototype    : ss_mpi_ive_svm_unload_model
 * Description  : Unload SVM model data.
 * Parameters   : ot_ive_svm_model      *svm_model         SVM model data.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_void ss_mpi_ive_svm_unload_model(const ot_ive_svm_model *svm_model);

/*
 * Prototype    : ss_mpi_ive_svm_predict
 * Description  : SVM predict
 * Parameters   : ot_ive_handle           *handle          Returned handle ID of a task
 * ot_svp_src_data         *src             Input sample
 * ot_svp_lut              *kernel_table    Look-up table for active function
 * ot_ive_svm_model        *svm_model       SVM model
 * ot_svp_dst_data         *dst_vote        Output Votes' array of each class
 * td_bool                 is_instant           For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_svm_predict(ot_ive_handle *handle, const ot_svp_src_data *src, const ot_svp_lut *kernel_table,
    const ot_ive_svm_model *svm_model, const ot_svp_dst_data *dst_vote, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_sad
 * Description  : Sum of absolute differences.
 * Parameters   : ot_ive_handle       *handle      Returned handle ID of a task
 * ot_svp_src_img      *src1        The input source1.Only the U8C1 input format is supported.
 * ot_svp_src_img      *src2        The input source2.Only the U8C1 input format is supported.
 * ot_svp_dst_img      *sad         Output result of sad value.Only the U8C1/U16C1 format is supported.
 * ot_svp_dst_img      *threshold   Output result of threshold.Only the U8C1 format is supported.
 * ot_ive_sad_ctrl     *ctrl         Control parameter
 * td_bool             is_instant    For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_sad(ot_ive_handle *handle, const ot_svp_src_img *src1, const ot_svp_src_img *src2,
    const ot_svp_dst_img *sad, const ot_svp_dst_img *threshold, const ot_ive_sad_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_resize
 * Description  : Resize.
 * Parameters   : ot_ive_handle           *handle     Returned handle ID of a task
 * ot_svp_src_img          src[]       The input source.Only the U8C1/U8C3_PLANAR input format
 * is supported.
 * ot_svp_dst_img          dst[]       Output result.Only the U8C1/U8C3_PLANAR format is supported.
 * ot_ive_resize_ctrl      *ctrl       Control parameter
 * td_bool                 is_instant  For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_resize(ot_ive_handle *handle, const ot_svp_src_img src[], const ot_svp_dst_img dst[],
    const ot_ive_resize_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_cnn_load_model
 * Description  : Load CNN model data from ".bin" file.
 * Parameters   : td_char             *file_name     CNN model file name, must be ".bin" file.
 * ot_ive_cnn_model    *model         CNN model data.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_cnn_load_model(const td_char *file_name, ot_ive_cnn_model *model);

/*
 * Prototype    : ss_mpi_ive_cnn_unload_model
 * Description  : Unload CNN model data and release memory.
 * Parameters   : ot_ive_cnn_model *model         CNN model data.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_void ss_mpi_ive_cnn_unload_model(const ot_ive_cnn_model *model);

/*
 * Prototype    : ss_mpi_ive_cnn_predict
 * Description  : Perform CNN prediction on input sample(s), and output responses for corresponding sample(s)
 * Parameters   : ot_ive_handle       *handle     Returned handle ID of a task
 * ot_svp_src_img      src[]       Input images array. Only the U8C1 and U8C3_PLANAR
 * input are supported
 * ot_ive_cnn_model    *model      CNN model data
 * ot_svp_dst_data     *dst        Output vectors of CNN_Predict
 * ot_ive_cnn_ctrl     *ctrl       CNN control parameter
 * td_bool             is_instant  For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_cnn_predict(ot_ive_handle *handle, const ot_svp_src_img src[], const ot_ive_cnn_model *model,
    const ot_svp_dst_data *dst, const ot_ive_cnn_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_cnn_get_result
 * Description  : Calculate classification and conf with CNN output by softmax regression.
 * Parameters   : ot_svp_src_data         *src    The result of CNN_Predict output.
 * ot_svp_dst_mem_info     *dst    The prediction about classification label index and conf.
 * ot_ive_cnn_model        *model  CNN model data
 * ot_ive_cnn_ctrl         *ctrl   CNN control parameter
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_cnn_get_result(const ot_svp_src_data *src, const ot_svp_dst_mem_info *dst,
    const ot_ive_cnn_model *model, const ot_ive_cnn_ctrl *ctrl);

/*
 * Prototype    : ss_mpi_ive_query
 * Description  : This API is used to query the status of a called function by using the returned
 * IveHandle of the function.
 * In block mode, the system waits until the function that is being queried is called.
 * In non-block mode, the current status is queried and no action is taken.
 * Parameters   : ot_ive_handle     handle          IveHandle of a called function. It is entered by users.
 * td_bool           *is_finish      Returned status
 * td_bool           is_block        Flag indicating the block mode or non-block mode
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_query(ot_ive_handle handle, td_bool *is_finish, td_bool is_block);

/*
 * Prototype    : ss_mpi_ive_persp_trans
 * Description  : Perspective transform
 * Parameters   : ot_ive_handle               *handle        Returned handle ID of a task.
 * ot_svp_src_img              *src           The input source.Only the U8C1/YUVSP420 input format
 * is supported..
 * ot_svp_rect_u32             roi[]          Roi array.
 * ot_svp_src_mem_info         point_pair[]   Point pair array.
 * ot_svp_dst_img              dst[]          Output result.Only the U8C1/YUVSP420/U8C3_PACKAGE
 * ormat is supported.
 * ot_ive_persp_trans_ctrl     *ctrl          PerspTrans control parameter.
 * td_bool                     is_instant     For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_persp_trans(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_rect_u32 roi[],
    const ot_svp_src_mem_info point_pair[], const ot_svp_dst_img dst[], const ot_ive_persp_trans_ctrl *ctrl,
    td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_kcf_get_mem_size
 * Description  : KCF get object memory size
 * Parameters  :  td_u32 max_obj_num    The maximum numbers of tracking object.
 * td_u32 *size          memory size.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_kcf_get_mem_size(td_u32 max_obj_num, td_u32 *size);

/*
 * Prototype    : ss_mpi_ive_kcf_create_obj_list
 * Description  : KCF create object list
 * Parameters   : ot_svp_mem_info     *mem            The memory of object for object list.
 * td_u32              max_obj_num     The maximum numbers of tracking object (list node).
 * ot_ive_kcf_obj_list *obj_list       The object list.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_kcf_create_obj_list(const ot_svp_mem_info *mem, td_u32 max_obj_num, ot_ive_kcf_obj_list *obj_list);

/*
 * Prototype    : ss_mpi_ive_kcf_destroy_obj_list
 * Description  : KCF destroy object list
 * Parameters  :  ot_ive_kcf_obj_list    *obj_list    The object list.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_kcf_destroy_obj_list(ot_ive_kcf_obj_list *obj_list);

/*
 * Prototype    : ss_mpi_ive_kcf_create_gauss_peak
 * Description  : KCF calculate gauss peak
 * Parameters  :  td_u3q5             padding         The multiple of roi rectangle's width and height.
 * ot_svp_dst_mem_info *gauss_peak     The memory of gauss peak.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_kcf_create_gauss_peak(td_u3q5 padding, const ot_svp_dst_mem_info *gauss_peak);

/*
 * Prototype    : ss_mpi_ive_kcf_create_cos_win
 * Description  : KCF calculate cos window x and y.
 * Parameters  :  ot_svp_dst_mem_info     *cos_win_x     The memory of cos window x.
 * ot_svp_dst_mem_info     *cos_win_y     The memory of cos window y.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_kcf_create_cos_win(const ot_svp_dst_mem_info *cos_win_x, const ot_svp_dst_mem_info *cos_win_y);

/*
 * Prototype    : ss_mpi_ive_kcf_get_train_obj
 * Description  : KCF get train object.
 * Parameters  :  td_u3q5             padding         The multiple of roi rectangle's width and height.
 * ot_ive_roi_info     roi_info[]      The array of roi information.
 * td_u32              obj_num         The numbers of roi_info and object.
 * ot_svp_mem_info     *cos_win_x      The memory of cos window x.
 * ot_svp_mem_info     *cos_win_y      The memory of cos window y.
 * ot_svp_mem_info     *gauss_peak     The memory of gauss peak.
 * ot_ive_kcf_obj_list *obj_list       The object list.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_kcf_get_train_obj(td_u3q5 padding, const ot_ive_roi_info roi_info[], td_u32 obj_num,
    const ot_svp_mem_info *cos_win_x, const ot_svp_mem_info *cos_win_y, const ot_svp_mem_info *gauss_peak,
    ot_ive_kcf_obj_list *obj_list);

/*
 * Prototype    : ss_mpi_ive_kcf_process
 * Description  : KCF process.
 * Parameters   : ot_ive_handle           *handle         Returned handle ID of a task
 * ot_svp_src_img          *src            Input source.Only the YUV420SP input format is supported.
 * ot_ive_kcf_obj_list     *obj_list       The object list.
 * ot_ive_kcf_proc_ctrl     *ctrl           Control parameter.
 * td_bool                 is_instant      For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_kcf_proc(ot_ive_handle *handle, const ot_svp_src_img *src, ot_ive_kcf_obj_list *obj_list,
    const ot_ive_kcf_proc_ctrl *ctrl, td_bool is_instant);

/*
 * Prototype    : ss_mpi_ive_kcf_get_obj_bbox
 * Description  : KCF get track object bbox information.
 * Parameters  :  ot_ive_kcf_obj_list     *obj_list           The object list.
 * ot_ive_kcf_bbox         bbox                The output bbox of object.
 * td_u32                  *bbox_obj_num       The valid numbers of output bbox.
 * ot_ive_kcf_bbox_ctrl    *ctrl               Control parameter.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_kcf_get_obj_bbox(ot_ive_kcf_obj_list *obj_list, ot_ive_kcf_bbox bbox[], td_u32 *bbox_obj_num,
    const ot_ive_kcf_bbox_ctrl *ctrl);

/*
 * Prototype    : ss_mpi_ive_kcf_judge_obj_bbox_track_state
 * Description  : KCF judge object tracking state.
 * Parameters  :  ot_ive_roi_info     *roi_info       The object list.
 * ot_ive_kcf_bbox     bbox            The bbox of object.
 * td_bool             *is_track_ok    The tracking state of object.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_kcf_judge_obj_bbox_track_state(const ot_ive_roi_info *roi_info, const ot_ive_kcf_bbox *bbox,
    td_bool *is_track_ok);

/*
 * Prototype    : ss_mpi_ive_kcf_obj_update
 * Description  : KCF update object.
 * Parameters  :  ot_ive_kcf_obj_list     *obj_list       The object list.
 * ot_ive_kcf_bbox         bbox            The bbox information of update object.
 * td_u32                  bbox_obj_num    The update numbers of bbox.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_kcf_obj_update(ot_ive_kcf_obj_list *obj_list, const ot_ive_kcf_bbox bbox[], td_u32 bbox_obj_num);

/*
 * Prototype    : ss_mpi_ive_hog
 * Description  : get Hog feature.
 * Parameters  :  ot_ive_handle           *handle         Returned handle ID of a task.
 * ot_svp_src_img          *src            Input source.Only the YUV420SP input format is supported.
 * ot_svp_rect_u32         roi[]           The array of roi.
 * ot_svp_dst_blob         dst[]           The array of Hog feature.
 * ot_ive_hog_ctrl         *ctrl           Control parameter.
 * td_bool                 is_instant      For details, see ss_mpi_ive_dma.
 * Return Value : TD_SUCCESS: Success;Error codes: Failure.
 * Date         : 2019-06-30
 * Author       : Multimedia software (SVP) group
 */
td_s32 ss_mpi_ive_hog(ot_ive_handle *handle, const ot_svp_src_img *src, const ot_svp_rect_u32 roi[],
    const ot_svp_dst_blob dst[], const ot_ive_hog_ctrl *ctrl, td_bool is_instant);

#ifdef __cplusplus
}
#endif

#endif
