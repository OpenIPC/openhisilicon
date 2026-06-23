/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SS_MPI_STITCH_LUT_GENERATE_H
#define SS_MPI_STITCH_LUT_GENERATE_H

#include "ot_type.h"
#include "ot_common_stitch_lut_generate.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ss_mpi_stitch_lut_generate: output lut for gdc tranform and stitch param
*       stitch_cfg: input stitch tuning info
*         size_cfg: input param for lut generate, crop area(x coordinates) is needed
*        out_param: output lut and params
*   mesh_file_name: output lut file name address
*/
td_s32 ss_mpi_stitch_lut_generate(const ot_stitch_config *stitch_cfg,
    const ot_stitch_img_size_cfg *size_cfg, ot_stitch_out_param *out_param, const td_char *mesh_file_prefix);

#ifdef __cplusplus
}
#endif

#endif /* SS_MPI_STITCH_LUT_GENERATE_H */
