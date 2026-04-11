/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_comm_sns.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/01/05
  Description   :
  History       :
  1.Date        : 2011/01/05
    Author      : x00100808
    Modification: Created file

******************************************************************************/

#ifndef __HI_COMM_SNS_H__
#define __HI_COMM_SNS_H__

#include "hi_type.h"
#include "hi_comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define CMOS_SHADING_TABLE_NODE_NUMBER_MAX (129)


typedef struct cmos_inttime_
{
	/* Public */
	HI_U16 exp_ratio;
	HI_U16 vblanking_lines;
	HI_U32 exposure_ashort;
	HI_U32 exposure_along;
	HI_U32 flicker_freq;
	HI_U8  exposure_shift;

	/* Private */
	HI_U16 full_lines_std;
	HI_U16 full_lines_std_30fps;
	HI_U16 full_lines_std_25fps;
	HI_U16 full_lines;
	HI_U16 full_lines_del;
	HI_U16 full_lines_limit;
	HI_U16 exp_ratio_target;
	HI_U16 max_lines_target;
	HI_U16 max_lines;
	HI_U16 min_lines_target;
	HI_U16 min_lines;
	HI_U16 lines_per_500ms;
    HI_U16 slow_framerate;
} cmos_inttime_t;

typedef struct cmos_inttime_ * cmos_inttime_ptr_t;
typedef struct cmos_inttime_ * cmos_inttime_const_ptr_t;

typedef struct cmos_isp_agc_table_
{
    HI_U8 sharpen_alt_d[8];         /* adjust image edge, different ISO with different sharp strength */
    HI_U8 sharpen_alt_ud[8];        /* adjust image texture, different ISO with different strength */
    HI_U8 snr_thresh[8];            /* adjust denoise strength, different ISO with different strength */
    HI_U8 demosaic_lum_thresh[8];
    HI_U8 demosaic_np_offset[8];
    HI_U8 ge_strength[8];
    HI_U8 saturation[8];            /* adjust saturation, different ISO with different saturation */    
} cmos_isp_agc_table_t;

typedef struct cmos_isp_agc_table_ * cmos_isp_agc_table_ptr_t;
typedef const struct cmos_isp_agc_table_ * cmos_isp_agc_table_const_ptr_t;


typedef struct cmos_isp_noise_table_
{
    HI_U8 noise_profile_weight_lut[128];
    HI_U8 demosaic_weight_lut[128];
} cmos_isp_noise_table_t;

typedef struct cmos_isp_noise_table_ * cmos_isp_noise_table_ptr_t;
typedef const struct cmos_isp_noise_table_ * cmos_isp_noise_table_const_ptr_t;

typedef struct cmos_isp_demosaic_
{
    HI_U8   vh_slope;
    HI_U8   aa_slope;
    HI_U8   va_slope;
    HI_U8   uu_slope;
    HI_U8   sat_slope;
    HI_U8   ac_slope;
    HI_U16  vh_thresh;
    HI_U16  aa_thresh;
    HI_U16  va_thresh;
    HI_U16  uu_thresh;
    HI_U16  sat_thresh;
    HI_U16  ac_thresh;
}cmos_isp_demosaic_t;

typedef struct cmos_isp_demosaic_ * cmos_isp_demosaic_ptr_t;
typedef const struct cmos_isp_demosaic_ * cmos_isp_demosaic_const_ptr_t;


typedef struct cmos_gains_
{
	/* Public */
	HI_U32 dgain;
	HI_U32 again;
	HI_U32 isp_dgain;
	HI_U32 iso;
	HI_U32 dgain_db;
	HI_U32 again_db;
	HI_U32 dgain_fine;
	HI_U16 out_offset_1;
	HI_U16 out_offset_2;

	/* Private */
    HI_U32 max_system_gain_target;
    HI_U32 max_system_gain_shift;
	HI_U32 min_system_gain_target;
    HI_U32 min_system_gain_shift;
    HI_U32 low_noise_gain_threshold;

	HI_U32 max_again_target;
	HI_U32 max_dgain_target;
	HI_U32 max_isp_dgain_target;
	HI_U32 max_again;
	HI_U32 max_dgain;
    HI_U32 max_ispdgain;
	HI_U32 min_again_target;
	HI_U32 min_dgain_target;
    HI_U32 min_isp_dgain_target;
    
	HI_U8  again_shift;
	HI_U8  dgain_shift;
	HI_U8  isp_dgain_shift; 
	HI_U8  dgain_fine_shift;
    HI_BOOL isp_dgain_delay_cfg;
	HI_U32 dgain_offset;
	HI_U32 max_dgain_step;

    HI_U32 max_adgain;

} cmos_gains_t;

typedef struct cmos_gains_ * cmos_gains_ptr_t;
typedef const struct cmos_gains_ * cmos_gains_const_ptr_t;

typedef struct cmos_isp_ccm_
{
    HI_U16 u16HighColorTemp;    /*D50 lighting source is  recommended */
    HI_U16 au16HighCCM[9];
    HI_U16 u16MidColorTemp;     /*D32 lighting source is  recommended */
    HI_U16 au16MidCCM[9];
    HI_U16 u16LowColorTemp;     /*A lighting source is  recommended */
    HI_U16 au16LowCCM[9];
} cmos_isp_ccm_t;

typedef struct cmos_isp_ccm_ * cmos_isp_ccm_ptr_t;
typedef const struct cmos_isp_ccm_ * cmos_isp_ccm_const_ptr_t;

typedef struct cmos_isp_shading_table_
{
    HI_U16 shading_center_r_x;
    HI_U16 shading_center_r_y;
    HI_U16 shading_center_g_x;
    HI_U16 shading_center_g_y;
    HI_U16 shading_center_b_x;
    HI_U16 shading_center_b_y;

	HI_U16 shading_table_r[CMOS_SHADING_TABLE_NODE_NUMBER_MAX];
	HI_U16 shading_table_g[CMOS_SHADING_TABLE_NODE_NUMBER_MAX];
	HI_U16 shading_table_b[CMOS_SHADING_TABLE_NODE_NUMBER_MAX];

    HI_U16 shading_off_center_r;
    HI_U16 shading_off_center_g;
    HI_U16 shading_off_center_b;

    HI_U16 shading_table_node_number;
} cmos_isp_shading_table_t;

typedef struct cmos_isp_shading_table_ * cmos_isp_shading_table_ptr_t;
typedef const struct cmos_isp_shading_table_ * cmos_isp_shading_table_const_ptr_t;

typedef struct cmos_isp_gamma_table_
{
    HI_U16 gamma_table[GAMMA_NODE_NUMBER];
}cmos_isp_gamma_table_t;

typedef struct cmos_isp_gamma_table_ * cmos_isp_gamma_table_ptr_t;
typedef const struct cmos_isp_gamma_table_ * cmos_isp_gamma_table_const_ptr_t;

typedef struct cmos_isp_default_
{
	cmos_isp_ccm_t ccm;

	HI_U16 black_level[4];		// black level

	HI_U16 wb_ref_temp;          //reference color temperature for WB

	HI_U16 gain_offset[4];		// gain offset for white balance

	HI_S32 wb_para[6];          //parameter for wb curve

	HI_U8 hist_thresh[4];		// hist threshold

	HI_U8 iridix_black;
	HI_U8 rggb;					// rggb start sequence

	HI_U16 again_max;
	HI_U16 dgain_max;

	HI_U8 iridix_vs;			// variance space
	HI_U8 iridix_vi;			// variance intensity
	HI_U8 iridix_sm;			// slope max
	HI_U16 iridix_wl;			// white level

	HI_U8 balance_fe;
	HI_U8 ae_compensation;		// ae compensation
	HI_U8 sinter_thresh;		// sinter threshold

    //add by z54137. We support two methods to generate noise profile lut.
    //noise_profile = 0 is for original one, noise_profile = 1 is for latest one.
	HI_U8 noise_profile;        //two different noise profile
	HI_U16 nr0;		            //nr0 for noise profile 2
	HI_U16 nr1;		            //nr1 for noise profile 2

    HI_U16 frame_end_update_mode;//enFrameEndUpdateMode
} cmos_isp_default_t;

typedef struct cmos_isp_default_ * cmos_isp_default_ptr_t;

typedef enum coms_isp_special_alg_
{
	isp_special_alg_0 = 0,		// for altasens only
    isp_special_alg_m034_lin,   // for aptm034 linear only
	isp_special_alg_m034_wdr,   // for aptm034 wdr only
	isp_special_alg_ar0331_lin,   // for aptar0331 linear only
	isp_special_alg_ar0331_wdr,   // for aptar0331 wdr only
	isp_special_alg_awb,  // for sony only
    isp_special_alg_imx104_lin,
    isp_special_alg_imx104_wdr,
	isp_special_alg_butt
} coms_isp_special_alg_e;



typedef struct cmos_sensor_max_resolution_
{
    HI_U32  u32MaxWidth;
    HI_U32  u32MaxHeight;

}cmos_sensor_max_resolution_t;

typedef struct cmos_sensor_max_resolution_ * cmos_sensor_max_resolution_ptr_t;


typedef struct cmos_sensor_image_mode_
{
    HI_U16 u16Width;
    HI_U16 u16Height;
    HI_U16 u16Fps;

}cmos_sensor_image_mode_t;

typedef struct cmos_sensor_image_mode_ * cmos_sensor_image_mode_ptr_t;


/* Sensor Control Operation */
typedef struct hiSENSOR_EXP_FUNC_S
{
    cmos_inttime_const_ptr_t (*pfn_cmos_inttime_initialize)(void);
    void (*pfn_cmos_inttime_update)				(cmos_inttime_ptr_t p_inttime);

    cmos_gains_ptr_t (*pfn_cmos_gains_initialize)(void);
    void (*pfn_cmos_gains_update)						(cmos_gains_const_ptr_t p_gains);
    HI_U16 (*pfn_cmos_gains_update2)					(cmos_gains_const_ptr_t p_gains);
    HI_U32 (*pfn_analog_gain_from_exposure_calculate)	(cmos_gains_ptr_t p_gains, HI_U32 exposure, HI_U32 exposure_max, HI_U32 exposure_shift);
    HI_U32 (*pfn_digital_gain_from_exposure_calculate)	(cmos_gains_ptr_t p_gains, HI_U32 exposure, HI_U32 exposure_max, HI_U32 exposure_shift);
    HI_U32 (*pfn_isp_digital_gain_from_exposure_calculate)	(cmos_gains_ptr_t p_gains, HI_U32 exposure, HI_U32 exposure_max, HI_U32 exposure_shift);

    void (*pfn_cmos_fps_set)				(cmos_inttime_ptr_t p_inttime, const HI_U8 fps);
    HI_U16 (*pfn_vblanking_calculate)		(cmos_inttime_ptr_t p_inttime);
    void (*pfn_cmos_vblanking_front_update)	(cmos_inttime_const_ptr_t p_inttime);

    void (*pfn_setup_sensor)(int isp_mode);
    HI_U8 (*pfn_cmos_get_analog_gain)(cmos_gains_ptr_t cmos_gains);
    HI_U8 (*pfn_cmos_get_digital_gain)(cmos_gains_ptr_t cmos_gains);
    HI_U8 (*pfn_cmos_get_digital_fine_gain)(cmos_gains_ptr_t cmos_gains);
    HI_U32 (*pfn_cmos_get_iso)(cmos_gains_ptr_t cmos_gains);

    HI_U32 (*pfn_cmos_get_isp_default)(cmos_isp_default_ptr_t p_coms_isp_default);
    HI_U32 (*pfn_cmos_get_isp_special_alg)(void);
    HI_U32 (*pfn_cmos_get_isp_agc_table)(cmos_isp_agc_table_ptr_t p_cmos_isp_agc_table);
    HI_U32 (*pfn_cmos_get_isp_noise_table)(cmos_isp_noise_table_ptr_t p_cmos_isp_noise_table);
    HI_U32 (*pfn_cmos_get_isp_demosaic)(cmos_isp_demosaic_ptr_t p_cmos_isp_demosaic);
    HI_U32 (*pfn_cmos_get_isp_shading_table)(cmos_isp_shading_table_ptr_t p_cmos_isp_shading_table);
    HI_U32 (*pfn_cmos_get_isp_gamma_table)(cmos_isp_gamma_table_ptr_t p_cmos_isp_gamma_table);
    HI_S32 (*pfn_cmos_get_sensor_max_resolution)(cmos_sensor_max_resolution_ptr_t p_cmos_sensor_max_resolution);
    HI_S32 (*pfn_cmos_set_sensor_image_mode)(cmos_sensor_image_mode_ptr_t p_cmos_sensor_image_mode);
    // ...

} SENSOR_EXP_FUNC_S;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /*__HI_COMM_SNS_H__ */

