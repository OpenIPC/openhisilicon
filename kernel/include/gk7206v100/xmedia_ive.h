/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef _IVE_H_
#define _IVE_H_

#include "xmedia_ive_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


/*****************************************************************************
*   prototype    : xmedia_ive_dma
*   description  : direct memory access (dma):
*                  1.direct memory copy;
*                  2. copy with interval bytes;
*                  3. memset using 3 bytes;
*                  4. memset using 8 bytes;
*   parameters   : xmedia_ive_handle          *p_ive_handle        returned handle id of a task.
*                  xmedia_ive_data_s          *pst_src            input source data.the input data is treated as u8c1 data.
*                  xmedia_ive_data_s          *pstdst            output result data.
*                  xmedia_ive_dma_ctrl_s      *pst_dma_ctrl        dma control parameter.
*                  xmedia_bool              b_instant          flag indicating whether to generate an interrupt.
*                                                         if the output result blocks the next operation,
*                                                         set b_instant to xmedia_true.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x1 pixels to 1920x1080 pixels.
*                  the stride must be 16-byte-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_dma(xmedia_ive_handle *p_ive_handle, xmedia_ive_data_s *pst_src,
                          xmedia_ive_dst_data_s *pst_dst, xmedia_ive_dma_ctrl_s *pst_dma_ctrl,
                          xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_filter
*   description  : 5x5 template filter.
*   parameters   : xmedia_ive_handle            *p_ive_handle       returned handle id of a task
*                  xmedia_ive_src_image_s       *pst_src            input source data.
*                                                                   the u8c1,sp420 and sp422 input formats are supported.
*                  xmedia_ive_dst_image_s       *pstdst             output result, of same type with the input.
*                  xmedia_ive_filter_ctrl_s     *pst_flt_ctrl       control parameters of filter
*                  xmedia_bool                   b_instant          for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the physical addresses of the input data and output data must be 16-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_filter(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                             xmedia_ive_dst_image_s *pst_dst, xmedia_ive_filter_ctrl_s *pst_flt_ctrl, 
                             xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_csc
*   description  : yuv2rgb\rgb2yuv color space conversion are supported.
*   parameters   : xmedia_ive_handle         *p_ive_handle       returned handle id of a task
*                  xmedia_ive_src_image_s    *pst_src           input source data:
*                                                       1. sp420\sp422 type for yuv2rgb;
*                                                       2. u8c3_package\u8c3_planar type for rgb2yuv;
*                  xmedia_ive_dst_image_s    *pstdst           output result:
*                                                       1. u8c3_package\u8c3_planar typed for yuv2rgb;
*                                                       2. sp420\sp422 type for rgb2yuv;
*                  xmedia_ive_csc_ctrl_s     *pst_csc_ctrl       control parameters for csc
*                  xmedia_bool             b_instant         for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 4x4 pixels to 1920x1080 pixels.
*                  the physical addresses of the input data and output data must be 16-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_csc(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                          xmedia_ive_dst_image_s *pst_dst, xmedia_ive_csc_ctrl_s *pst_csc_ctrl,
                          xmedia_bool b_instant);

/*****************************************************************************
*   prototype    : xmedia_ive_sobel
*   description  : sobel is used to extract the gradient information.
*   parameters   : xmedia_ive_handle           *p_ive_handle      returned handle id of a task
*                  xmedia_ive_src_image_s      *pst_src          input source data. only the u8c1 input image is supported.
*                  xmedia_ive_dst_image_s      *pstdsth         the (horizontal) result of input image filtered by the input mask;
*                  xmedia_ive_dst_image_s      *pstdstv         the (vertical) result  of input image filtered by the transposed mask;
*                  xmedia_ive_sobel_ctrl_s     *pst_sobel_ctrl    control parameters
*                  xmedia_bool               b_instant        for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 8x8 pixels to 1920x1080 pixels.
*                  the physical addresses of the input data and output data must be 16-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_sobel(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                            xmedia_ive_dst_image_s *pst_dst_h, xmedia_ive_dst_image_s *pst_dst_v,
                            xmedia_ive_sobel_ctrl_s *pst_sobel_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_mag_and_ang
*   description  : magandang is used to extract the edge information.
*   parameters   : xmedia_ive_handle              *p_ive_handle         returned handle id of a task
*                  ive_src_info_s          *pst_src             input source data. only the u8c1 input format is supported.
*                  xmedia_ive_mem_info_s          *pstdstmag          output magnitude.
*                  xmedia_ive_mem_info_s          *pstdstang          output angle.
*                                                              if the output mode is set to magnitude only,
*                                                              this item can be set to null.
*                  xmedia_ive_mag_and_ang_ctrl_s  *pst_mag_and_ang_ctrl   control parameters
*                  xmedia_bool                  b_instant           for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the physical addresses of the input data and output data must be 16-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_mag_and_ang(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                                  xmedia_ive_dst_image_s *pst_dst_mag,
                                  xmedia_ive_dst_image_s *pst_dst_ang,
                                  xmedia_ive_mag_and_ang_ctrl_s *pst_mag_and_ang_ctrl,
                                  xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_dilate
*   description  : 5x5 template dilate. only the u8c1 binary image input is supported.or else the result is not expected.
*   parameters   : xmedia_ive_handle             *p_ive_handle          returned handle id of a task
*                  xmedia_ive_src_image_s        *pst_src              input binary image, which consists of 0 or 255;
*                  xmedia_ive_dst_image_s        *pstdst              output result.
*                  xmedia_ive_dilate_ctrl_s      *pst_dilate_ctrl       control parameters.
*                  xmedia_bool                 b_instant            for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the physical addresses of the input data and output data must be 16-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*                  the input value, output value, and mask value must be 0 or 255.
*****************************************************************************/
xmedia_s32 xmedia_ive_dilate(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                             xmedia_ive_dst_image_s *pst_dst, xmedia_ive_dilate_ctrl_s *pst_dilate_ctrl,
                             xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_erode
*   parameters   : 5x5 template erode. only the u8c1 binary image input is supported.or else the result is not correct.
*   input        : xmedia_ive_handle               *p_ive_handle       returned handle id of a task
*                  xmedia_ive_src_image_s          *pst_src           input binary image, which consists of 0 or 255;
*                  xmedia_ive_dst_image_s          *pstdst           output result.
*                  xmedia_ive_erode_ctrl_s         *pst_erode_ctrl     control parameters
*                  xmedia_bool                   b_instant         for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the physical addresses of the input data and output data must be 16-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*                  the input value, output value, and mask value must be 0 or 255.
*****************************************************************************/
xmedia_s32 xmedia_ive_erode(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                            xmedia_ive_dst_image_s *pst_dst,
                            xmedia_ive_erode_ctrl_s *pst_erode_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_thresh
*   description  : thresh operation to the input image.
*   parameters   : xmedia_ive_handle           *p_ive_handle       returned handle id of a task
*                  xmedia_ive_src_image_s      *pst_src           input source data. only the u8c1 input format is supported.
*                  xmedia_ive_dst_image_s      *pstdst           output result
*                  xmedia_ive_thresh_ctrl_s    *pst_thr_ctrl    control parameters
*                  xmedia_bool               b_instant         for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_thresh(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                             xmedia_ive_dst_image_s *pst_dst, xmedia_ive_thresh_ctrl_s *pst_thr_ctrl, 
                             xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_and
*   description  : binary images' and operation.
*   parameters   : xmedia_ive_handle          *p_ive_handle       returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src1          the input source1. only u8c1 input format is supported.
*                  xmedia_ive_src_image_s     *pst_src2          the input source2.only u8c1 input format is supported.
*                  xmedia_ive_dst_image_s     *pstdst           output result of " src1 & src2 ".
*                  xmedia_bool              b_instant         for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the stride must be 16-pixel-aligned.
*                  the types, widths, heights of two input sources must be the same.
*****************************************************************************/
xmedia_s32 xmedia_ive_and(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                          xmedia_ive_src_image_s *pst_src2, xmedia_ive_dst_image_s *pst_dst,
                          xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_sub
*   description  : two gray images' sub operation.
*   parameters   : xmedia_ive_handle          *p_ive_handle   returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src1      minuend of the input source.only the u8c1 input format is supported.
*                  xmedia_ive_src_image_s     *pst_src2      subtrahend of the input source.only the u8c1 input format is supported.
*                  xmedia_ive_dst_image_s     *pstdst       output result of src1 minus src2
*                  xmedia_ive_sub_ctrl_s      *pst_sub_ctrl   control parameter
*                  xmedia_bool              b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the stride must be 16-pixel-aligned.
*                  the types, widths, heights of two input sources must be the same.
*****************************************************************************/
xmedia_s32 xmedia_ive_sub(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                          xmedia_ive_src_image_s *pst_src2, xmedia_ive_dst_image_s *pst_dst,
                          xmedia_ive_sub_ctrl_s *pst_sub_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_or
*   description  : two binary images' or operation.
*   parameters   : xmedia_ive_handle          *p_ive_handle    returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src1       input source1. only the u8c1 input format is supported.
*                  xmedia_ive_src_image_s     *pst_src2       input source2. only the u8c1 input format is supported.
*                  xmedia_ive_dst_image_s     *pstdst        output result src1 or src2
*                  xmedia_bool              b_instant        for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the stride must be 16-pixel-aligned.
*                  the types, widths, heights of two input sources must be the same.
*****************************************************************************/
xmedia_s32 xmedia_ive_or(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                         xmedia_ive_src_image_s *pst_src2, xmedia_ive_dst_image_s *pst_dst,
                         xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_integ
*   description  : calculate the input gray image's integral image.
*   parameters   : xmedia_ive_handle           *p_ive_handle        returned handle id of a task
*                  xmedia_ive_src_image_s      *pst_src            input source data.only the u8c1 input format is supported.
*                  xmedia_ive_dst_image_s      *pstdst            output result.can be u32c1 or u64c1, relied on the control parameter.
*                  xmedia_ive_integ_ctrl_s     *pst_integ_ctrl      integ control
*                  xmedia_bool               b_instant          for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x16 pixels to 1920x1080 pixels.
*                  the physical addresses of the input data and output data must be 16-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*                  the pixel can be 32bit or 64 bit relied on the control parameter.
*****************************************************************************/
xmedia_s32 xmedia_ive_integ(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                            xmedia_ive_dst_image_s *pst_dst, xmedia_ive_integ_ctrl_s *pst_integ_ctrl, 
                            xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_hist
*   description  : calculate the input gray image's histogram.
*   parameters   : xmedia_ive_handle          *p_ive_handle      returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src          input source data. only the u8c1 input format is supported.
*                  xmedia_ive_dst_mem_info_s  *pstdst          output result.
*                  xmedia_bool              b_instant        for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the physical addresses of the input data and output data must be 16-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_hist(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                           xmedia_ive_dst_mem_info_s *pst_dst, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_thresh_s16
*   description  : s16 image's thresh operation.
*   parameters   : xmedia_ive_handle              *p_ive_handle             returned handle id of a task
*                  xmedia_ive_src_image_s         *pst_src                 input source data.only the s16 input format is supported.
*                  xmedia_ive_dst_image_s         *pstdst                 output result.
*                  xmedia_ive_thresh_s16_ctrl_s   *pst_thr_s16_ctrl          control parameters
*                  xmedia_bool                  b_instant               for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the physical addresses of the input data must be 2-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_thresh_s16(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                                 xmedia_ive_dst_image_s *pst_dst, xmedia_ive_thresh_s16_ctrl_s *pst_thr_s16_ctrl,
                                 xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_thresh_u16
*   description  : u16 image's thresh operation.
*   parameters   : xmedia_ive_handle                *p_ive_handle        returned handle id of a task
*                  xmedia_ive_src_image_s           *pst_src            input source data. only the u16 input format is supported.
*                  xmedia_ive_dst_image_s           *pstdst            output result
*                  xmedia_ive_thresh_u16_ctrl_s     *pst_thr_u16_ctrl     control parameters
*                  xmedia_bool                    b_instant          for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the physical addresses of the input data must be 2-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_thresh_u16(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                                 xmedia_ive_dst_image_s *pst_dst, xmedia_ive_thresh_u16_ctrl_s *pst_thr_u16_ctrl,
                                 xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_16bit_to_8bit
*   description  : scale the input 16bit data to the output 8bit data.
*   parameters   : xmedia_ive_handle                *p_ive_handle              returned handle id of a task
*                  xmedia_ive_src_image_s           *pst_src                  input source data.only u16c1\s16c1 input is supported.
*                  xmedia_ive_dst_image_s           *pstdst                  output result
*                  ive_16bitto8bit_ctrl_s    *pst_16bit_to_8bit_ctrl      control parameter
*                  xmedia_bool                    b_instant                for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 16x16 pixels to 1920x1080 pixels.
*                  the physical addresses of the input data must be 2-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_16bit_to_8bit(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                                    xmedia_ive_dst_image_s *pst_dst, xmedia_ive_16bit_to_8bit_ctrl_s *pst_16bit_to_8bit_ctrl,
                                    xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_ord_stat_filter
*   description  : order statistic filter. it can be used as median\max\min value filter.
*   parameters   : xmedia_ive_handle                   *p_ive_handle          returned handle id of a task
*                  xmedia_ive_src_image_s              *pst_src              input source data. only u8c1 input is supported
*                  xmedia_ive_dst_image_s              *pstdst              output result
*                  xmedia_ive_ord_stat_filter_ctrl_s   *pst_ord_stat_flt_ctrl   control parameter
*                  xmedia_bool                       b_instant            for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the physical addresses of the input data and output data must be 16-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_ord_stat_filter(xmedia_ive_handle *p_ive_handle,
                                      xmedia_ive_src_image_s *pst_src,
                                      xmedia_ive_dst_image_s *pst_dst,
                                      xmedia_ive_ord_stat_filter_ctrl_s *pst_ord_stat_flt_ctrl,
                                      xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_map
*   description  : map a image to another through a lookup table.
*   parameters   : xmedia_ive_handle           *p_ive_handle  returned handle id of a task
*                  xmedia_ive_src_image_s      *pst_src          input source. only the u8c1 input format is supported.
*                  xmedia_ive_src_mem_info_s   *pst_map          input lookup table. must be an u8 array of size 256.
*                  xmedia_ive_dst_image_s      *pstdst          output result.
*                  xmedia_ive_map_ctrl_s       *pst_map_ctrl      map control parameter.
*                  xmedia_bool               b_instant        for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the physical addresses of the input data and output data must be 16-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_map(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                          xmedia_ive_src_mem_info_s *pst_map, xmedia_ive_dst_image_s *pst_dst,
                          xmedia_ive_map_ctrl_s *pst_map_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_equalize_hist
*   description  : enhance the input image's contrast through histogram equalization.
*   parameters   : xmedia_ive_handle                 *p_ive_handle              returned handle id of a task
*                  xmedia_ive_src_image_s            *pst_src                  input source.only u8c1 input format is supported.
*                  xmedia_ive_dst_image_s            *pstdst                  output result.
*                  ive_equalizehist_ctrl_s    *pst_equalize_hist_ctrl     equalizehist control parameter.
*                  xmedia_bool                     b_instant                for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the physical addresses of map data must be 16-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_equalize_hist(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                                    xmedia_ive_dst_image_s *pst_dst, xmedia_ive_equalize_hist_ctrl_s *pst_equalize_hist_ctrl,
                                    xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_add
*   description  : two gray images' add operation.
*   parameters   : xmedia_ive_handle          *p_ive_handle      returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src1         augend of the input source.only the u8c1 input format is supported.
*                  xmedia_ive_src_image_s     *pst_src2         addend of the input source.only the u8c1 input format is supported.
*                  xmedia_ive_dst_image_s     *pstdst          output result of src1 plus src2
*                  xmedia_ive_add_ctrl_s      *pst_add_ctrl      control parameter
*                  xmedia_bool              b_instant        for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the stride must be 16-pixel-aligned.
*                  the types, widths, heights of two input sources must be the same.
*****************************************************************************/
xmedia_s32 xmedia_ive_add(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                          xmedia_ive_src_image_s *pst_src2, xmedia_ive_dst_image_s *pst_dst,
                          xmedia_ive_add_ctrl_s *pst_add_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_xor
*   description  : two binary images' xor operation.
*   parameters   : xmedia_ive_handle           *p_ive_handle    returned handle id of a task
*                  xmedia_ive_src_image_s      *pst_src1       the input source1.only the u8c1 input format is supported.
*                  xmedia_ive_src_image_s      *pst_src2       the input source2.
*                  xmedia_ive_dst_image_s      *pstdst        output result
*                  xmedia_bool               b_instant      for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the stride must be 16-pixel-aligned.
*                  the types, widths, heights of two input sources must be the same.
*****************************************************************************/
xmedia_s32 xmedia_ive_xor(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                          xmedia_ive_src_image_s *pst_src2, xmedia_ive_dst_image_s *pst_dst,
                          xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_ncc
*   description  : calculate two gray images' ncc (normalized cross correlation).
*   parameters   : xmedia_ive_handle            *p_ive_handle     returned handle id of a task
*                  xmedia_ive_src_image_s       *pst_src1        input source1. only the u8c1 input format is supported.
*                  xmedia_ive_src_image_s       *pst_src2        input source2. must be of the same type, size of source1.
*                  xmedia_ive_dst_mem_info_s    *pstdst         output result
*                  xmedia_bool                b_instant       for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the physical addresses of output data must be 16-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_ncc(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                       xmedia_ive_src_image_s *pst_src2, xmedia_ive_dst_mem_info_s *pst_dst,
                       xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_ccl
*   description  : connected component labeling. only 8-connected method is supported.
*   parameters   : xmedia_ive_handle         *p_ive_handle      returned handle id of a task
*                  xmedia_ive_image_s        *pst_src_dst       input source
*                  xmedia_ive_mem_info_s     *pst_blob         output result of detected region;
*                  xmedia_ive_ccl_ctrl_s     *pst_ccl_ctrl      ccl control parameter
*                  xmedia_bool             b_instant        for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 16x16 pixels to 1280x720 pixels.
*                  the physical addresses of the input data and output data must be 16-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_ccl(xmedia_ive_handle *p_ive_handle, xmedia_ive_image_s *pst_src_dst,
                          xmedia_ive_dst_mem_info_s *pst_blob, xmedia_ive_ccl_ctrl_s *pst_ccl_ctrl,
                          xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_gmm
*   description  : separate foreground and background using gmm(gaussian mixture model) method;
*                  gray or rgb gmm are supported.
*   parameters   : xmedia_ive_handle          *p_ive_handle  returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src       input source. only support u8c1 or u8c3_package input.
* 				   xmedia_ive_dst_image_s     *pst_fg        output foreground (binary) image.
* 				   xmedia_ive_dst_image_s     *pst_bg        output background image. of the sampe type of pst_src.
* 				   xmedia_ive_mem_info_s      *pst_model     model data.
* 				   xmedia_ive_gmm_ctrl_s      *pst_gmm_ctrl   control parameter.
*                  xmedia_bool              b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the physical addresses of the input data and output data must be 16-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_gmm(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                       xmedia_ive_dst_image_s *pst_fg, xmedia_ive_dst_image_s *pst_bg,
                       xmedia_ive_mem_info_s *pst_model, xmedia_ive_gmm_ctrl_s *pst_gmm_ctrl,
                       xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_gmm2
*   description  : separate foreground and background using gmm(gaussian mixture model) method;
*                  gray or rgb gmm are supported.
*   parameters   : xmedia_ive_handle          *p_ive_handle         returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src              only u8c1 or u8c3_package input are supported.
*                  xmedia_ive_src_image_s      *pst_factor          u16c1 input, low-8bits is sensitivity factor, and high-8bits is life update factor.
*                  xmedia_ive_dst_image_s     *pst_fg               output foreground (binary) image.
*                  xmedia_ive_dst_image_s     *pst_bg               output background image. with same type of pst_src.
*                  xmedia_ive_dst_image_s     *pst_match_model_info output u8c1 match model info image. low-1bit is match flag,and high-7bits is max freq index.
*                  xmedia_ive_mem_info_s      *pst_model            model data.
*                  xmedia_ive_gmm2_ctrl_s     *pst_gmm2_ctrl            control parameter.
*                  xmedia_bool              b_instant           for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1280x1080 pixels.
*                  the physical addresses of the input data and output data must be 16-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_gmm2(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                           xmedia_ive_src_image_s *pst_factor, xmedia_ive_dst_image_s *pst_fg,
                           xmedia_ive_dst_image_s *pst_bg,
                           xmedia_ive_dst_image_s *pst_match_model_info,
                           xmedia_ive_mem_info_s *pst_model, xmedia_ive_gmm2_ctrl_s *pst_gmm2_ctrl,
                           xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_normgrad
*   description  : gradient calculation and the output is normalized to s8.
*   parameters   : xmedia_ive_handle             *p_ive_handle        returned handle id of a task
*                  xmedia_ive_src_image_s        *pst_src            input source data
*                  xmedia_ive_dst_image_s        *pstdsth           the (horizontal) result of input image filtered by the input mask;
*                  xmedia_ive_dst_image_s        *pstdstv           the (vertical) result  of input image filtered by the transposed mask;
*                  xmedia_ive_dst_image_s        *pstdsthv          output the horizontal and vertical component in single image in package format.
*                  xmedia_ive_norm_grad_ctrl_s   *pst_norm_grad_ctrl   control parameter
*                  xmedia_bool                 b_instant          for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the physical addresses of the input data and output data must be 16-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_normgrad(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                               xmedia_ive_dst_image_s *pst_dst_h, xmedia_ive_dst_image_s *pst_dst_v,
                               xmedia_ive_dst_image_s *pst_dst_hv,
                               xmedia_ive_norm_grad_ctrl_s *pst_norm_grad_ctrl,
                               xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_stcandi_corner
*   description  : the first part of corners detection using stomasi-like method: calculate candidate corners.
*   parameters   : xmedia_ive_handle              *p_ive_handle         returned handle id of a task
*                  xmedia_ive_src_image_s         *pst_src             input source data
*                  xmedia_ive_dst_image_s         *pst_candi_corner     output result of eig
*                  xmedia_ive_st_candi_corner_ctrl_s    *pst_st_candi_corner_ctrl   control parameter
*                  xmedia_bool                  b_instant           for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 64x64 pixels to 1280x720 pixels.
*                  the physical addresses of the input data and output data must be 16-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_st_candi_corner(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                                      xmedia_ive_dst_image_s *pst_candi_corner,
                                      xmedia_ive_st_candi_corner_ctrl_s *pst_st_candi_corner_ctrl,
                                      xmedia_bool b_instant);

/*****************************************************************************
*   prototype    : xmedia_ive_stcorner
*   description  : the second part of corners detection using stomasi-like method: select corners by certain rules.
*                  xmedia_ive_src_image_s         *pst_candi_corner     input source data
*                  xmedia_ive_dst_mem_info_s *pst_corner    output result of corner
*                  xmedia_ive_st_corner_ctrl_s    *pst_st_corner_ctrl   control parameter
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 16x16 pixels to 1280x720 pixels.
*                  the physical addresses of the input data and output data must be 16-byte-aligned.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_st_corner(xmedia_ive_src_image_s *pst_candi_corner, xmedia_ive_dst_mem_info_s *pst_corner, xmedia_ive_st_corner_ctrl_s *pst_st_corner_ctrl);


/*****************************************************************************
*   prototype    : xmedia_ive_sad
*   description  : sum of absolute differences.
*   parameters   : xmedia_ive_handle          *p_ive_handle   returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src1      the input source1.only the u8c1 input format is supported.
*                  xmedia_ive_src_image_s     *pst_src2      the input source2.only the u8c1 input format is supported.
*                  xmedia_ive_dst_image_s     *pst_sad       output result of sad value.only the u8c1/u16c1 format is supported.
*                  xmedia_ive_dst_image_s     *pst_thr       output result of thresh.only the u8c1 format is supported.
*                  xmedia_ive_sad_ctrl_s       *pst_sad_ctrl   control parameter
*                  xmedia_bool             b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the stride must be 16-pixel-aligned.
*                  the types, widths, heights of two input sources must be the same.
*****************************************************************************/
xmedia_s32 xmedia_ive_sad(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                          xmedia_ive_src_image_s *pst_src2, xmedia_ive_dst_image_s *pst_sad,
                          xmedia_ive_dst_image_s *pst_thr, xmedia_ive_sad_ctrl_s *pst_sad_ctrl,
                          xmedia_bool b_instant);



/*****************************************************************************
*   prototype    : xmedia_ive_resize_csc_letterbox
*   description  : resize,support yuv2rgb color space conversion,mend the edge.
*   parameters   : xmedia_ive_handle                           *p_ive_handle   returned handle id of a task
*                  xmedia_ive_src_image_s                      *pst_src       only the u8c1/u8c3_planar/sp420 input format is supported.
*                  xmedia_ive_dst_image_s                      *pstdst       output result.only the u8c1/u8c3_planar format is supported.
*                  uve_resize_csc_letterbox_ctrl_s      *pst_rcl_ctrl       control parameter.
*                  xmedia_bool                           b_instant     for details, see xmedia_ive_resize_csc_letterbox.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 4x4 pixels to 1920x1080 pixels.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_resize_csc_letterbox(
    xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src, xmedia_ive_dst_image_s *pst_dst,
    xmedia_ive_resize_csc_letterbox_ctrl_s *pst_rcl_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_gradfg
*   description  :
*   parameters   : xmedia_ive_handle            *p_ive_handle       returned handle id of a task
*                  xmedia_ive_src_image_s       *pst_bg_diff_fg      background subtraction foreground image
*                  xmedia_ive_src_image_s       *pst_cur_grad       current gradient image, both horizontally and vertically
*                                                          graded in accordance with [xyxyxy ...] format
*                  xmedia_ive_src_image_s       *pst_bg_grad        background gradient image
*                  xmedia_ive_dst_image_s       *pst_grad_fg        gradient foreground image
*                  xmedia_ive_grad_fg_ctrl_s    *pst_grad_fg_ctrl    gradient calculation parameters
*                  xmedia_bool                b_instant         for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_gradfg(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_bg_diff_fg,
                          xmedia_ive_src_image_s *pst_cur_grad,
                          xmedia_ive_src_image_s *pst_bg_grad,
                          xmedia_ive_dst_image_s *pst_grad_fg,
                          xmedia_ive_grad_fg_ctrl_s *pst_grad_fg_ctrl,
                          xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_match_bg_model
*   description  : background modeling/match_bg
*   parameters   : xmedia_ive_handle                  *p_ive_handle            returned handle id of a task
*                  xmedia_ive_src_image_s             *pst_cur_img             current grayscale image
*                  xmedia_ive_data_s                  *pst_bg_model            background model data
*                  xmedia_ive_image_s                 *pst_fg_flag             foreground status image
*                  xmedia_ive_dst_image_s             *pst_bg_diff_fg           foreground image obtained by background matching,
*                                                                     the background pixel value is 0, the foreground pixel
*                                                                     value is the gray difference value
*                  xmedia_ive_dst_image_s             *pst_frm_diff_fg          foreground image obtained by interframe difference,
*                                                                     the background pixel value is 0, the foreground pixel
*                                                                     value is the gray difference value
*                  xmedia_ive_dst_mem_info_s          *pst_stat_data           result status data
*                  xmedia_ive_match_bg_model_ctrl_s   *pst_match_bg_model_ctrl   background matching parameters
*                  xmedia_bool                      b_instant              for details, see xmedia_ive_match_bg_model.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_match_bg_model(
    xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_cur_img, xmedia_ive_data_s *pst_bg_model,
    xmedia_ive_image_s *pst_fg_flag, xmedia_ive_dst_image_s *pst_bg_diff_fg,
    xmedia_ive_dst_image_s *pst_frm_diff_fg, xmedia_ive_dst_mem_info_s *pst_stat_data,
    xmedia_ive_match_bg_model_ctrl_s *pst_match_bg_model_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_update_bg_model
*   description  : background modeling/update_bg
*   parameters   : xmedia_ive_handle                  *p_ive_handle              returned handle id of a task
*                  xmedia_ive_data_s                  *pst_bg_model              background model data
*                  xmedia_ive_image_s                 *pst_fg_flag               foreground status image
*                  xmedia_ive_dst_image_s             *pst_bg_img                background grayscale image
*                  xmedia_ive_dst_image_s             *pst_chg_sta_img            change state life image, for still detection
*                  xmedia_ive_dst_image_s             *pst_chg_sta_fg             change state grayscale image, for still detection
*                  xmedia_ive_dst_image_s             *pst_chg_sta_life           change state foreground image, for still detection
*                  xmedia_ive_dst_mem_info_s          *pst_stat_data             result status data
*                  xmedia_ive_update_bg_model_ctrl_s  *pst_update_bg_model_ctrl    background update parameters
*                  xmedia_bool                      b_instant                for details, see xmedia_ive_update_bg_model.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : the size of the input data ranges from 32x32 pixels to 1920x1080 pixels.
*                  the stride must be 16-pixel-aligned.
*****************************************************************************/
xmedia_s32 xmedia_ive_update_bg_model(
    xmedia_ive_handle *p_ive_handle, xmedia_ive_data_s *pst_bg_model, xmedia_ive_image_s *pst_fg_flag,
    xmedia_ive_dst_image_s *pst_bg_img, xmedia_ive_dst_image_s *pst_chg_sta_img,
    xmedia_ive_dst_image_s *pst_chg_sta_fg, xmedia_ive_dst_image_s *pst_chg_sta_life,
    xmedia_ive_dst_mem_info_s *pst_stat_data,
    xmedia_ive_update_bg_model_ctrl_s *pst_update_bg_model_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_lbp
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle   returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src       the input source.only the u8c1 input format is supported.
*                  xmedia_ive_dst_image_s     *pst_dst       output result of sad value.only the u8c1/u16c1 format is supported.
*                  xmedia_ive_lbp_ctrl_s      *pst_lbp_ctrl   control parameter
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_lbp(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                          xmedia_ive_dst_image_s *pst_dst, xmedia_ive_lbp_ctrl_s *pst_lbp_ctrl,
                          xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_acc
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle  returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src       the input source.
*                  xmedia_ive_dst_image_s     *pst_dst       output result
*                  xmedia_ive_lbp_ctrl_s      *pst_acc_ctrl  control parameter
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_acc(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src, xmedia_ive_dst_image_s *pst_dst, 
                          xmedia_ive_acc_ctrl_s *pst_acc_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_lk_optical_flow_pyr
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle  returned handle id of a task
*                  xmedia_ive_src_image_s      ast_src_prev_pyr[]
*                  xmedia_ive_src_image_s      ast_src_next_pyr[]
*                  xmedia_ive_src_mem_info_s  *pst_prev_pts
*                  xmedia_ive_mem_info_s      *pst_next_pts
*                  xmedia_ive_dst_mem_info_s  *pst_status
*                  xmedia_ive_dst_mem_info_s  *pst_err
*                  xmedia_ive_lk_optical_flow_pyr_ctrl_s *pst_lk_opti_flow_pyr_ctrl control parameter
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_lk_optical_flow_pyr(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s ast_src_prev_pyr[],
                                          xmedia_ive_src_image_s ast_src_next_pyr[], xmedia_ive_src_mem_info_s *pst_prev_pts,
                                          xmedia_ive_mem_info_s *pst_next_pts, xmedia_ive_dst_mem_info_s *pst_status,
                                          xmedia_ive_dst_mem_info_s *pst_err,
                                          xmedia_ive_lk_optical_flow_pyr_ctrl_s *pst_lk_opti_flow_pyr_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_and_ccl
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle   returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src1      the input source1..
*                  xmedia_ive_src_image_s     *pst_src2      the input source2..
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_dst_image_s     *pst_blob      
*                  xmedia_ive_ccl_ctrl_s      *pst_ccl_ctrl  control parameter
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_and_ccl(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                              xmedia_ive_src_image_s *pst_src2, xmedia_ive_dst_image_s *pst_dst,
                              xmedia_ive_dst_mem_info_s *pst_blob, xmedia_ive_ccl_ctrl_s *pst_ccl_ctrl, 
                              xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_or_ccl
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle   returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src1      the input source1.
*                  xmedia_ive_src_image_s     *pst_src2      the input source2.
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_dst_image_s     *pst_blob      output result.
*                  xmedia_ive_ccl_ctrl_s      *pst_ccl_ctrl  control parameter
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_or_ccl(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                             xmedia_ive_src_image_s *pst_src2, xmedia_ive_dst_image_s *pst_dst, xmedia_ive_dst_mem_info_s *pst_blob,
                             xmedia_ive_ccl_ctrl_s *pst_ccl_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_xor_ccl
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle   returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src1      the input source1.
*                  xmedia_ive_src_image_s     *pst_src2      the input source2.
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_dst_image_s     *pst_blob      output result.
*                  xmedia_ive_ccl_ctrl_s      *pst_ccl_ctrl  control parameter
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_xor_ccl(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                              xmedia_ive_src_image_s *pst_src2, xmedia_ive_dst_image_s *pst_dst, xmedia_ive_dst_mem_info_s *pst_blob,
                              xmedia_ive_ccl_ctrl_s *pst_ccl_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_sub_ccl
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle   returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src1      the input source1.
*                  xmedia_ive_src_image_s     *pst_src2      the input source2.
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_dst_image_s     *pst_blob      output result.
*                  xmedia_ive_sub_ctrl_s      *pst_sub_ctrl  control parameter.
*                  xmedia_ive_ccl_ctrl_s      *pst_ccl_ctrl  control parameter.
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_sub_ccl(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                              xmedia_ive_src_image_s *pst_src2, xmedia_ive_dst_image_s *pst_dst, xmedia_ive_dst_mem_info_s *pst_blob,
                              xmedia_ive_sub_ctrl_s *pst_sub_ctrl, xmedia_ive_ccl_ctrl_s *pst_ccl_ctrl, 
                              xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_add_ccl
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle   returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src1      the input source1.
*                  xmedia_ive_src_image_s     *pst_src2      the input source2.
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_dst_image_s     *pst_blob      output result.
*                  xmedia_ive_add_ctrl_s      *pst_add_ctr   control parameter.
*                  xmedia_ive_ccl_ctrl_s      *pst_ccl_ctrl  control parameter.
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_add_ccl(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                              xmedia_ive_src_image_s *pst_src2, xmedia_ive_dst_image_s *pst_dst, xmedia_ive_dst_mem_info_s *pst_blob,
                              xmedia_ive_add_ctrl_s *pst_add_ctr, xmedia_ive_ccl_ctrl_s *pst_ccl_ctrl, 
                              xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_sub_hist
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle   returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src1      the input source1.
*                  xmedia_ive_src_image_s     *pst_src2      the input source2.
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_sub_ctrl_s      *pst_sub_ctrl  control parameter.
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_sub_hist(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                               xmedia_ive_src_image_s *pst_src2, xmedia_ive_dst_mem_info_s *pst_dst,
                               xmedia_ive_sub_ctrl_s *pst_sub_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_sub_thresh
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle   returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src1      the input source1.
*                  xmedia_ive_src_image_s     *pst_src2      the input source2.
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_sub_ctrl_s      *pst_sub_ctrl  control parameter.
*                  xmedia_ive_thresh_ctrl_s   *pst_thr_ctrl  control parameter.
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_sub_thresh(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                                 xmedia_ive_src_image_s *pst_src2, xmedia_ive_dst_image_s *pst_dst,
                                 xmedia_ive_sub_ctrl_s *pst_sub_ctrl, xmedia_ive_thresh_ctrl_s *pst_thr_ctrl,
                                 xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_sub_integ
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle   returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src1      the input source1.
*                  xmedia_ive_src_image_s     *pst_src2      the input source2.
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_sub_ctrl_s      *pst_sub_ctrl    control parameter.
*                  xmedia_ive_integ_ctrl_s    *pst_integ_ctrl  control parameter.
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_sub_integ(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                                xmedia_ive_src_image_s *pst_src2, xmedia_ive_dst_image_s *pst_dst,
                                xmedia_ive_sub_ctrl_s *pst_sub_ctrl, xmedia_ive_integ_ctrl_s *pst_integ_ctrl,
                                xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_sub_map
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle   returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src1      the input source1.
*                  xmedia_ive_src_image_s     *pst_src2      the input source2.
*                  xmedia_ive_src_mem_info_s  *pst_map       output result.
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_sub_ctrl_s      *pst_sub_ctrl  control parameter.
*                  xmedia_ive_map_ctrl_s      *pst_map_ctrl  control parameter.
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_sub_map(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                              xmedia_ive_src_image_s *pst_src2, xmedia_ive_src_mem_info_s *pst_map,
                              xmedia_ive_dst_image_s *pst_dst, xmedia_ive_sub_ctrl_s *pst_sub_ctrl,
                              xmedia_ive_map_ctrl_s *pst_map_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_add_hist
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle   returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src1      the input source1.
*                  xmedia_ive_src_image_s     *pst_src2      the input source2.
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_add_ctrl_s      *pst_add_ctr  control parameter.
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_add_hist(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                               xmedia_ive_src_image_s *pst_src2, xmedia_ive_dst_mem_info_s *pst_dst,
                               xmedia_ive_add_ctrl_s *pst_add_ctr, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_add_thresh
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle   returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src1      the input source1.
*                  xmedia_ive_src_image_s     *pst_src2      the input source2.
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_add_ctrl_s      *pst_add_ctr   control parameter.
*                  xmedia_ive_thresh_ctrl_s   *pst_thr_ctrl  control parameter.
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_add_thresh(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                                 xmedia_ive_src_image_s *pst_src2, xmedia_ive_dst_image_s *pst_dst,
                                 xmedia_ive_add_ctrl_s *pst_add_ctr, xmedia_ive_thresh_ctrl_s *pst_thr_ctrl,
                                 xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_add_integ
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle   returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src1      the input source1.
*                  xmedia_ive_src_image_s     *pst_src2      the input source2.
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_add_ctrl_s      *pst_add_ctr     control parameter.
*                  xmedia_ive_integ_ctrl_s    *pst_integ_ctrl  control parameter.
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_add_integ(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1, xmedia_ive_src_image_s *pst_src2, 
                                xmedia_ive_dst_image_s *pst_dst, xmedia_ive_add_ctrl_s *pst_add_ctr, xmedia_ive_integ_ctrl_s *pst_integ_ctrl,
                                xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_add_map
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle   returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src1      the input source1.
*                  xmedia_ive_src_image_s     *pst_src2      the input source2.
*                  xmedia_ive_src_mem_info_s  *pst_map       
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_add_ctrl_s      *pst_add_ctr   control parameter.
*                  xmedia_ive_map_ctrl_s      *pst_map_ctrl  control parameter.
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_add_map(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src1,
                              xmedia_ive_src_image_s *pst_src2, xmedia_ive_src_mem_info_s *pst_map,
                              xmedia_ive_dst_image_s *pst_dst, xmedia_ive_add_ctrl_s *pst_add_ctr, 
                              xmedia_ive_map_ctrl_s *pst_map_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_filter_thresh
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle  returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src       the input source.
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_filter_ctrl_s   *pst_flt_ctrl  control parameter.
*                  xmedia_ive_thresh_ctrl_s   *pst_thr_ctrl  control parameter.
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_filter_thresh(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                                    xmedia_ive_dst_image_s *pst_dst, xmedia_ive_filter_ctrl_s *pst_flt_ctrl,
                                    xmedia_ive_thresh_ctrl_s *pst_thr_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_ord_stat_filter_thresh
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle  returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src       the input source.
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_ord_stat_filter_ctrl_s   *pst_ord_stat_flt_ctrl  control parameter.
*                  xmedia_ive_thresh_ctrl_s            *pst_thr_ctrl  control parameter.
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_ord_stat_filter_thresh (xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                                              xmedia_ive_dst_image_s *pst_dst, xmedia_ive_ord_stat_filter_ctrl_s *pst_ord_stat_flt_ctrl,
                                              xmedia_ive_thresh_ctrl_s *pst_thr_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_sobel_thresh_s16
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle  returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src       the input source.
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_sobel_ctrl_s        *pst_sobel_ctrl    control parameter.
*                  xmedia_ive_thresh_s16_ctrl_s   *pst_thr_s16_ctrl  control parameter.
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_sobel_thresh_s16 (xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                                        xmedia_ive_dst_image_s *pst_dst, xmedia_ive_sobel_ctrl_s *pst_sobel_ctrl,
                                        xmedia_ive_thresh_s16_ctrl_s *pst_thr_s16_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_mag_and_ang_thresh_u16
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle  returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src       the input source.
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_mag_and_ang_ctrl_s  *pst_mag_and_ang_ctrl  control parameter.
*                  xmedia_ive_thresh_u16_ctrl_s   *pst_thr_u16_ctrl      control parameter.
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_mag_and_ang_thresh_u16 (xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                                              xmedia_ive_dst_image_s *pst_dst, xmedia_ive_mag_and_ang_ctrl_s *pst_mag_and_ang_ctrl,
                                              xmedia_ive_thresh_u16_ctrl_s *pst_thr_u16_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_mag_and_ang_16bit_to_8bit
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle  returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src       the input source.
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_mag_and_ang_ctrl_s   *pst_mag_and_ang_ctrl    control parameter.
*                  xmedia_ive_16bit_to_8bit_ctrl_s *pst_16bit_to_8bit_ctrl  control parameter.
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_mag_and_ang_16bit_to_8bit (xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                                                 xmedia_ive_dst_image_s *pst_dst, xmedia_ive_mag_and_ang_ctrl_s *pst_mag_and_ang_ctrl,
                                                 xmedia_ive_16bit_to_8bit_ctrl_s *pst_16bit_to_8bit_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_thresh_dilate
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle  returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src       the input source.
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_thresh_ctrl_s   *pst_thr_ctrl  control parameter.
*                  xmedia_ive_dilate_ctrl_s   *pst_dilate_ctrl control parameter.
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_thresh_dilate(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                                    xmedia_ive_dst_image_s *pst_dst, xmedia_ive_thresh_ctrl_s *pst_thr_ctrl,
                                    xmedia_ive_dilate_ctrl_s *pst_dilate_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_thresh_erode
*   description  : 
*   parameters   : xmedia_ive_handle          *p_ive_handle  returned handle id of a task
*                  xmedia_ive_src_image_s     *pst_src       the input source.
*                  xmedia_ive_dst_image_s     *pst_dst       output result.
*                  xmedia_ive_thresh_ctrl_s   *pst_thr_ctrl  control parameter.
*                  xmedia_ive_erode_ctrl_s    *pst_erode_ctrl control parameter.
*                  xmedia_bool                 b_instant     for details, see xmedia_ive_dma.
*   return value : xmedia_success: success;error codes: failure.
*   spec         : 
*****************************************************************************/
xmedia_s32 xmedia_ive_thresh_erode(xmedia_ive_handle *p_ive_handle, xmedia_ive_src_image_s *pst_src,
                                   xmedia_ive_dst_image_s *pst_dst, xmedia_ive_thresh_ctrl_s *pst_thr_ctrl,
                                   xmedia_ive_erode_ctrl_s *pst_erode_ctrl, xmedia_bool b_instant);


/*****************************************************************************
*   prototype    : xmedia_ive_query
*   description  : this api is used to query the status of a called function by using the returned ive_handle of the function.
                   in block mode, the system waits until the function that is being queried is called.
                   in non-block mode, the current status is queried and no action is taken.
*   parameters   : xmedia_ive_handle     ive_handle     ive_handle of a called function. it is entered by users.
*                  xmedia_bool       *pb_finish      returned status
*                  xmedia_bool        b_block        flag indicating the block mode or non-block mode
*                  xmedia_bool       *pb_finish
*   return value : xmedia_success: success;error codes: failure.
*   spec         :
*****************************************************************************/
xmedia_s32 xmedia_ive_query(xmedia_ive_handle ive_handle, xmedia_bool *pb_finish, xmedia_bool b_block);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
