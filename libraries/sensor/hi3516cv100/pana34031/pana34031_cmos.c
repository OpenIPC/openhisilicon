/******************************************************************************
  A driver program of panasonic mn34031 on HI3518A 
 ******************************************************************************
    Modification:  2013-04  Created
******************************************************************************/
#if !defined(__PANA_MN34031_H_)
#define __PANA_MN34031_H_

#include <stdio.h>
#include <unistd.h>		
#include <string.h>

#include "hi_comm_sns.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"
#include "hi_isp_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


/****************************************************************************
 * local variables															*
 ****************************************************************************/

static cmos_inttime_t cmos_inttime;
static cmos_gains_t cmos_gains;
HI_U8 gu8SensorMode = 0;

static cmos_isp_default_t st_coms_isp_default =
{
    // color correction matrix
    {
        5048,
    	{	0x1f5, 0x80b1, 0x8044,
    		0x803f, 0x1d2, 0x8094,
    		0x1e, 0x80f8, 0x1db
    	},
        3200,
    	{	0x01d5, 0x806d, 0x802e,
    		0x8069, 0x1d2, 0x8069,
    		0x25, 0x8115, 0x01ef
    	},
        2480,
    	{	0x1f1, 0x80b8, 0x8039,
    		0x8040, 0x1cf, 0x808f,
    		0x1e, 0x80fc, 0x1dd
    	}
    },

	// black level for R, Gr, Gb, B channels
       {256,256,256,256},
    // calibration reference color temperature
    5048,

    //WB gain at 5000K, must keep consistent with calibration color temperature
	{0x1cf, 0x100, 0x100, 0x1bd},

    // WB curve parameters, must keep consistent with reference color temperature.
	//{123, -8, -140, 22209, 128, -174188},
    {67, 91, -98, 167236, 128, -119483},
	// hist_thresh
	{0x10,0x40,0xc0,0xf0},

	0x00,	// iridix_balck
	0x1,	// rggb

	// gain
	0x8,	0x10,

	// iridix space, intensity, slope_max, white level
	0x02,	0x08,	0x80, 	0x8ff,

	0x1, 	// balance_fe
	0x80,	// ae compensation
	0x8, 	// sinter threshold

	0x1,        //0: use default profile table; 1: use calibrated profile lut, the setting for nr0 and nr1 must be correct.
	0,
	1528,

    1
};

static cmos_isp_agc_table_t st_isp_agc_table =
{
    //sharpen_alt_d
    {0x50,0x45,0x40,0x38,0x34,0x40,0x30,0x5c},

    //sharpen_alt_ud
    {0x3b,0x38,0x34,0x30,0x2b,0x40,0x30,0x5f},

    //snr_thresh
    {0x23,0x2c,0x34,0x3e,0x46,0x60,0x70,0x8f},

    //demosaic_lum_thresh
    {0x60,0x60,0x80,0x80,0x80,0x80,0x80,0x80},

    //demosaic_np_offset
    {0x0,0xa,0x12,0x1a,0x20,0x28,0x30,0x30},

    //ge_strength
    {0x55,0x55,0x55,0x55,0x55,0x55,0x37,0x37},

    /* saturation */
    {0x80,0x80,0x80,0x80,0x68,0x48,0x35,0x30}
};

static cmos_isp_noise_table_t st_isp_noise_table =
{
   //nosie_profile_weight_lut
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3,0x9,0xd,0x10,0x12,0x14,0x16,0x17,0x18,0x1a,0x1b,
	0x1c,0x1c,0x1d,0x1e,0x1f,0x1f,0x20,0x21,0x21,0x22,0x22,0x23,0x23,0x24,0x24,0x25,0x25,
	0x25,0x26,0x26,0x27,0x27,0x27,0x28,0x28,0x28,0x29,0x29,0x29,0x29,0x2a,0x2a,0x2a,0x2a,
	0x2b,0x2b,0x2b,0x2b,0x2c,0x2c,0x2c,0x2c,0x2d,0x2d,0x2d,0x2d,0x2d,0x2e,0x2e,0x2e,0x2e,
	0x2e,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x31,0x31,0x31,
	0x31,0x31,0x31,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x33,0x33,0x33,0x33,0x33,0x33,
	0x33,0x33,0x34,0x34,0x34,0x34,0x34,0x34,0x34,0x34,0x34,0x35,0x35,0x35,0x35,0x35,0x35,
	0x35,0x35,0x35,0x35,0x36,0x36,0x36},

  //demosaic_weight_lut
    {0x0,0x3,0x9,0xd,0x10,0x12,0x14,0x16,0x17,0x18,0x1a,0x1b,
	0x1c,0x1c,0x1d,0x1e,0x1f,0x1f,0x20,0x21,0x21,0x22,0x22,0x23,0x23,0x24,0x24,0x25,0x25,
	0x25,0x26,0x26,0x27,0x27,0x27,0x28,0x28,0x28,0x29,0x29,0x29,0x29,0x2a,0x2a,0x2a,0x2a,
	0x2b,0x2b,0x2b,0x2b,0x2c,0x2c,0x2c,0x2c,0x2d,0x2d,0x2d,0x2d,0x2d,0x2e,0x2e,0x2e,0x2e,
	0x2e,0x2f,0x2f,0x2f,0x2f,0x2f,0x2f,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x31,0x31,0x31,
	0x31,0x31,0x31,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x33,0x33,0x33,0x33,0x33,0x33,
	0x33,0x33,0x34,0x34,0x34,0x34,0x34,0x34,0x34,0x34,0x34,0x35,0x35,0x35,0x35,0x35,0x35,
	0x35,0x35,0x35,0x35,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36}
};
static cmos_isp_demosaic_t st_isp_demosaic =
{
    /*vh_slope*/
    0xd6,

    /*aa_slope*/
    0xbf,

    /*va_slope*/
    200,

    /*uu_slope*/
    190,

    /*sat_slope*/
    0x5d,

    /*ac_slope*/
    0xcf,

    /*vh_thresh*/
    0xa2,

    /*aa_thresh*/
    0xcf,

    /*va_thresh*/
    0x64,

    /*uu_thresh*/
    0x64,

    /*sat_thresh*/
    0x171,

    /*ac_thresh*/
    0x1b3
};

static __inline HI_U16 digital_gain_lut_get_value(HI_U8 index)
{
    static HI_U16 gain_lut[] =
    {
        0x00,0x02,0x04,0x06,0x08,0x0a,0x0c,0x0e,
        0x10,0x12,0x14,0x16,0x18,0x1a,0x1c,0x1e,
        0x20,0x22,0x24,0x26,0x28,0x2a,0x2c,0x2e,
        0x30,0x32,0x34,0x36,0x38,0x3a,0x3c,0x3e,
        0x40,0x42,0x44,0x46,0x48,0x4a,0x4c,0x4e,
        0x50,0x52,0x54,0x56,0x58,0x5a,0x5c,0x5e,
        0x60,0x62,0x64,0x66,0x68,0x6a,0x6c,0x6e,
        0x70,0x72,0x74,0x76,0x78,0x7a,0x7c,0x7e,
        0x80,0x82,0x84,0x86,0x88,0x8a,0x8c,0x8e,
        0x90,0x92,0x94,0x96,0x98,0x9a,0x9c,0x9e,
        0xa0,0xa2,0xa4,0xa6,0xa8,0xaa,0xac,0xae,
        0xb0,0xb2,0xb4,0xb6,0xb8,0xba,0xbc,0xbe,
        0xc0,0xc2,0xc4,0xc6,0xc8,0xca,0xcc,0xce,
        0xd0,0xd2,0xd4,0xd6,0xd8,0xda,0xdc,0xde,
        0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xec,0xee,
        0xf0,0xf2,0xf4,0xf6,0xf8,0xfa,0xfc,0xfe,0x100
    };
	return gain_lut[index];
}


/*
 * This function initialises an instance of cmos_inttime_t.
 */
static __inline cmos_inttime_const_ptr_t cmos_inttime_initialize()
{
    
    
    cmos_inttime.full_lines_std = 1065;
    cmos_inttime.full_lines_std_30fps = 1065;
    cmos_inttime.full_lines_std_25fps=1278;
	cmos_inttime.full_lines = 1065;
    cmos_inttime.full_lines_del = 1065;
    cmos_inttime.full_lines_limit = 65535;
	cmos_inttime.max_lines_target = 1062;
	cmos_inttime.min_lines_target = 2;
	cmos_inttime.vblanking_lines = 1065;
	
	cmos_inttime.exposure_ashort = 0;
	cmos_inttime.exposure_shift = 0;
	cmos_inttime.lines_per_500ms = cmos_inttime.full_lines_std_30fps*30/2; // 500ms / 39.17us
	cmos_inttime.flicker_freq = 0;//60*256;//50*256;

    
	return &cmos_inttime;
}

/*
 * This function applies the new integration time to the ISP registers.
 */
static __inline void cmos_inttime_update(cmos_inttime_ptr_t p_inttime)
{

	HI_U32 eshort = p_inttime->exposure_ashort;
	HI_U32 exp_frames = eshort / p_inttime->full_lines_std;
	
	eshort = eshort - exp_frames * p_inttime->full_lines_std;
	eshort = p_inttime->full_lines_std - eshort;
	sensor_write_register(0x00A1, (eshort&0xffff));
	sensor_write_register(0x00A3, exp_frames);
}

/*
 * This function applies the new vert blanking porch to the ISP registers.
 */
static __inline void cmos_vblanking_update(cmos_inttime_const_ptr_t p_inttime)
{
	int full_lines_lo16 = p_inttime->full_lines & 0xffff;
	sensor_write_register(0x1A0, full_lines_lo16);
	sensor_write_register(0x2A0, full_lines_lo16);
}

static __inline HI_U16 vblanking_calculate(
		cmos_inttime_ptr_t p_inttime)
{
	if (p_inttime->exposure_ashort >= p_inttime->full_lines - 2)
	{
		p_inttime->exposure_ashort = p_inttime->full_lines - 2;
	}

	p_inttime->vblanking_lines = p_inttime->full_lines - p_inttime->full_lines_std;

	return p_inttime->exposure_ashort;
}


/* Set fps base */
static __inline void cmos_fps_set(
		cmos_inttime_ptr_t p_inttime,
		const HI_U8 fps
		)
{

	usleep(40000);
    int full_lines_lo16;
	if (30 == fps)
	{
        
		p_inttime->full_lines_std = 1065;
		p_inttime->full_lines = 1065;
		p_inttime->max_lines_target = 1062;
		
		full_lines_lo16 = p_inttime->full_lines & 0xffff;
	    sensor_write_register(0x1A0, full_lines_lo16);
	    sensor_write_register(0x2A0, full_lines_lo16);
	    p_inttime->lines_per_500ms=p_inttime->full_lines*30/2;
		
	}
	else if (25 == fps)
	{
	    
		p_inttime->full_lines_std = 1278;
		p_inttime->full_lines = 1278;
		p_inttime->max_lines_target = 1275;
		
		full_lines_lo16 = p_inttime->full_lines & 0xffff;
	    sensor_write_register(0x1A0, full_lines_lo16);
	    sensor_write_register(0x2A0, full_lines_lo16);
	    p_inttime->lines_per_500ms=p_inttime->full_lines*25/2;
	}
}

/*
 * This function initialises an instance of cmos_gains_t.
 */
static __inline cmos_gains_ptr_t cmos_gains_initialize()
{
	cmos_gains.max_again = 0x8;
	cmos_gains.max_dgain = 0x800;
	cmos_gains.again_shift = 0;
	cmos_gains.dgain_shift = 6;
	cmos_gains.dgain_fine = 0;

    cmos_gains.isp_dgain_shift = 4;
    cmos_gains.isp_dgain = 1 << cmos_gains.isp_dgain_shift;
    cmos_gains.max_isp_dgain_target = 1 << cmos_gains.isp_dgain_shift;

	return &cmos_gains;
}

static __inline HI_U32 cmos_get_ISO(cmos_gains_ptr_t p_gains)
{
	HI_U32 _again = p_gains->again == 0 ? 1 : p_gains->again;
	HI_U32 _dgain = p_gains->dgain == 0 ? 1 : p_gains->dgain;

	p_gains->iso =  ((_again * _dgain * 100) >> (p_gains->again_shift + p_gains->dgain_shift));

	return p_gains->iso;
}

/*
 * This function applies the digital gain
 * input and output offset and correction
 */
static __inline void cmos_gains_update(cmos_gains_const_ptr_t p_gains)
{


	switch (p_gains->again_db)
    {
    case 0:

        //r_colgsw = 0dB, r_a_gain = 0dB
        sensor_write_register(0x20, 0x0080);
        break;
    case 6:

        //r_colgsw = 6dB, r_a_gain = 0dB
      sensor_write_register(0x20, 0x2080);
        break;
    case 12:

        //r_colgsw = 12dB, r_a_gain = 0dB
       sensor_write_register(0x20, 0x6080);
        break;
    case 18:

        //r_colgsw = 12dB, r_a_gain = 6dB
           sensor_write_register(0x20, 0xE080);

        break;
    default:
        break;
    }

        HI_U32  lut_val;
       lut_val = digital_gain_lut_get_value(p_gains->dgain_db);
        sensor_write_register(0x21, lut_val);
    
}



static __inline HI_U32 analog_gain_from_exposure_calculate(
		cmos_gains_ptr_t p_gains,
		HI_U32 exposure,
		HI_U32 exposure_max,
		HI_U32 exposure_shift)
{
	int _i;
	HI_U32 _again = (1 << p_gains->again_shift);
	HI_U32 _again_db = 0;
	HI_U32 exposure1;
	int shft = 0;

	// normalizes
	while (exposure > (1<<22))
	{
		exposure >>= 1;
		exposure_max >>= 1;
		++shft;
	}

	for(_i = 0; _i <= 2; _i++)
	{
	      exposure1 = (exposure>>1);
		if((exposure1 < exposure_max) || (_again >  p_gains->max_again_target))
			break;

		_again *= 2;
		_again_db += 6;
		exposure = exposure1;
	}

	p_gains->again = _again;
	p_gains->again_db = _again_db;
	return (exposure << shft);
}


//digital  step = 0.19db
//是否需要加入四舍五入运算
static HI_U32 mn34031_dgains_to_db_convert(HI_U32 data, HI_U32 shift_in)
{

	HI_U32 _res = 0;
	if(0 == data)
		return _res;

	for(;;)
	{
		data = (data*1002 + (1<<9)) >> 10;
		if(data < (1<<shift_in))
			break;
		++_res;
	}
	return _res;
}


static __inline HI_U32 isp_digital_gain_from_exposure_calculate(
		cmos_gains_ptr_t p_gains,
		HI_U32 exposure,
		HI_U32 exposure_max,
		HI_U32 exposure_shift)
{
    HI_U32 isp_dgain = (1 << p_gains->isp_dgain_shift);
    int shft = 0;

    while (exposure > (1 << 22))
	{
		exposure >>= 1;
		exposure_max >>= 1;
		++shft;
	}
    exposure_max = DIV_0_TO_1(exposure_max);

    if(exposure > exposure_max)
    {
        isp_dgain = ((exposure << p_gains->isp_dgain_shift) + (exposure_max >> 1)) / exposure_max;
        exposure = exposure_max;
        isp_dgain = (isp_dgain > p_gains->max_isp_dgain_target) ? (p_gains->max_isp_dgain_target) : isp_dgain;        
    }
    else
	{
	}
    p_gains->isp_dgain = isp_dgain;

    return exposure << shft;
}


static __inline HI_U32 digital_gain_from_exposure_calculate(
		cmos_gains_ptr_t p_gains,
		HI_U32 exposure,
		HI_U32 exposure_max,
		HI_U32 exposure_shift)
{
	HI_U32 _dgain = (1 << p_gains->dgain_shift);
    int shft = 0;

    while (exposure > (1 << 20))
    {
        exposure     >>= 1;
        exposure_max >>= 1;
        ++shft;
    }

    if (exposure > exposure_max)
    {
        //when setting manual exposure line, exposure_max>>shift should not be 0.
        exposure_max = DIV_0_TO_1(exposure_max);
        _dgain   = (exposure * _dgain) / exposure_max;
        exposure = exposure_max;

    }
    if (_dgain >= p_gains->max_dgain_target)
    {
        _dgain = p_gains->max_dgain_target;
    }
    p_gains->dgain = _dgain;
    p_gains->dgain_db = mn34031_dgains_to_db_convert(_dgain, p_gains->dgain_shift);
    p_gains->dgain_fine = 0;
    cmos_get_ISO(p_gains);

    return exposure << shft;
}

static void setup_sensor(int isp_mode)
{
	if(0 == isp_mode) /* setup for ISP 'normal mode' */
	{
		sensor_write_register(0x00A3,0x0000);
	}
	else if(1 == isp_mode) /* setup for ISP pixel calibration mode */
	{
        //set the gain to 0
		sensor_write_register(0x0020,0x0080);
		sensor_write_register(0x0021,0x0000);
		//set exposure time
		sensor_write_register(0x00A1,0x0400);
		sensor_write_register(0x00A3,0x0005);
	}
}

static HI_U8 cmos_get_analog_gain(cmos_gains_ptr_t p_gains)
{
    return p_gains->again_db;
}

static HI_U8 cmos_get_digital_gain(cmos_gains_ptr_t p_gains)
{
	return p_gains->dgain_db;
}

static HI_U32 cmos_get_isp_default(cmos_isp_default_ptr_t p_coms_isp_default)
{
	if (NULL == p_coms_isp_default)
	{
	    printf("null pointer when get isp default value!\n");
	    return -1;
	}
    memcpy(p_coms_isp_default, &st_coms_isp_default, sizeof(cmos_isp_default_t));
    return 0;
}


static HI_U32 cmos_get_isp_agc_table(cmos_isp_agc_table_ptr_t p_cmos_isp_agc_table)
{
	if (NULL == p_cmos_isp_agc_table)
	{
	    printf("null pointer when get isp agc table value!\n");
	    return -1;
	}
    memcpy(p_cmos_isp_agc_table, &st_isp_agc_table, sizeof(cmos_isp_agc_table_t));
    return 0;
}

static HI_U32 cmos_get_isp_demosaic(cmos_isp_demosaic_ptr_t p_cmos_isp_demosaic)
{
   if (NULL == p_cmos_isp_demosaic)
   {
	    printf("null pointer when get isp demosaic value!\n");
	    return -1;
   }

   memcpy(p_cmos_isp_demosaic, &st_isp_demosaic,sizeof(cmos_isp_demosaic_t));
   return 0;

}

static HI_U32 cmos_get_isp_noise_table(cmos_isp_noise_table_ptr_t p_cmos_isp_noise_table)
{
	if (NULL == p_cmos_isp_noise_table)
	{
	    printf("null pointer when get isp noise table value!\n");
	    return -1;
	}
    memcpy(p_cmos_isp_noise_table, &st_isp_noise_table, sizeof(cmos_isp_noise_table_t));
    return 0;
}

static HI_S32 cmos_get_sensor_max_resolution(cmos_sensor_max_resolution_ptr_t p_cmos_sensor_max_resolution)
{
     if(NULL == p_cmos_sensor_max_resolution)
     {
       printf("null pointer when get sensor max resolution value!\n");
       return -1;
     }

     p_cmos_sensor_max_resolution->u32MaxWidth  = 1280;
     p_cmos_sensor_max_resolution->u32MaxHeight = 720;

    return 0;
}


/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/

SENSOR_EXP_FUNC_S stSensorExpFuncs =
{
    .pfn_cmos_inttime_initialize = cmos_inttime_initialize,
    .pfn_cmos_inttime_update = cmos_inttime_update,

    .pfn_cmos_gains_initialize = cmos_gains_initialize,
    .pfn_cmos_gains_update = cmos_gains_update,
    .pfn_cmos_gains_update2 = NULL,
    .pfn_analog_gain_from_exposure_calculate = analog_gain_from_exposure_calculate,
    .pfn_digital_gain_from_exposure_calculate = digital_gain_from_exposure_calculate,
    .pfn_isp_digital_gain_from_exposure_calculate = isp_digital_gain_from_exposure_calculate,

    .pfn_cmos_fps_set = cmos_fps_set,
    .pfn_vblanking_calculate = vblanking_calculate,
    .pfn_cmos_vblanking_front_update = cmos_vblanking_update,

    .pfn_setup_sensor = setup_sensor,

    .pfn_cmos_get_analog_gain = cmos_get_analog_gain,
    .pfn_cmos_get_digital_gain = cmos_get_digital_gain,
    .pfn_cmos_get_digital_fine_gain = NULL,
    .pfn_cmos_get_iso = cmos_get_ISO,

    .pfn_cmos_get_isp_default = cmos_get_isp_default,
    .pfn_cmos_get_isp_special_alg = NULL,
    .pfn_cmos_get_isp_agc_table = cmos_get_isp_agc_table,
	.pfn_cmos_get_isp_noise_table = cmos_get_isp_noise_table,
	.pfn_cmos_get_isp_demosaic = cmos_get_isp_demosaic,
	.pfn_cmos_get_isp_shading_table = NULL,
	.pfn_cmos_get_isp_gamma_table = NULL,
    .pfn_cmos_get_sensor_max_resolution = cmos_get_sensor_max_resolution,
    .pfn_cmos_set_sensor_image_mode = NULL,
};

int sensor_register_callback(void)
{
	int ret;
	ret = HI_MPI_ISP_SensorRegCallBack(&stSensorExpFuncs);
	if (ret)
	{
	    printf("sensor register callback function failed!\n");
	    return ret;
	}

	return 0;
}

//chang sensor mode
int sensor_mode_set(HI_U8 u8Mode)
{
    switch(u8Mode)
    {
        //sensor mode 0
        case 0:
            gu8SensorMode = 0;
            // TODO:
        break;
        //sensor mode 1
        case 1:
            gu8SensorMode = 1;
             // TODO:
        break;

        default:
            printf("NOT support this mode!\n");
            return -1;
        break;
    }
    return 0;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif // __PANA_MN34031_H_
