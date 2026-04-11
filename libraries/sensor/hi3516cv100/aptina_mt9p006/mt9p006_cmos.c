#if !defined(__APTINA_MT9P006_CMOS_H_)
#define __APTINA_MT9P006_CMOS_H_

#include <stdio.h>
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

extern const unsigned int sensor_i2c_addr;
extern const unsigned int sensor_addr_byte;
extern const unsigned int sensor_data_byte;

static cmos_inttime_t cmos_inttime;
static cmos_gains_t cmos_gains;
static HI_U8 gu8SensorMode = 0;

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/

/*set Frame End Update Mode 2 with HI_MPI_ISP_SetAEAttr and set this value 1 to avoid flicker */
/*when use Frame End Update Mode 2, the speed of i2c will affect whole system's performance   */
/*increase I2C_DFT_RATE in Hii2c.c to 400000 to increase the speed of i2c                     */
#define CMOS_MT9P006_ISP_WRITE_SENSOR_ENABLE (1)

static cmos_isp_default_t st_coms_isp_default = 
{
	// color matrix[9]
    {
        5000,
        {   0x0236, 0x80fd, 0x8039,
            0x804b, 0x018d, 0x8041,
            0x8006, 0x80ca, 0x01d0
        },
        3200,
        {   0x025a, 0x80e7, 0x8073,
            0x8078, 0x01b5, 0x803d,
            0x802b, 0x8119, 0x0244
        },
        2450,
        {
            0x0214, 0x8089, 0x808b,
            0x809d, 0x01ce, 0x8031,
            0x8068, 0x81e7, 0x034f
        }
    },


	// black level
	{0xAc,0xAc,0xAc,0xAc},

    //calibration reference color temperature 
    5000,

    //WB gain at 6500K, must keep consistent with calibration color temperature 
	{0x158, 0x100, 0x100, 0x18f},

    // WB curve parameters, must keep consistent with reference color temperature.
	{89, 3, -164, 232133, 128, -183491},

	// hist_thresh
	{0xd,0x28,0x60,0x80},

	0x0,	// iridix_balck
	0x1,	// grbg

	/* limit max gain for reducing noise,    */
    /* when again equals its max value,there will be much noise */
	0x8,	0x10,

	// iridix
	0x04,	0x08,	0xa0, 	0x4ff,

	0x1, 	// balance_fe
	0x80,	// ae compensation
	0x15, 	// sinter threshold

	0x0,  0,  0,  //noise profile=0, use the default noise profile lut, don't need to set nr0 and nr1

    2
};

static cmos_isp_noise_table_t st_isp_noise_table =
{
    //nosie_profile_weight_lut
    {0x0,0x0,0x0,0x0,0x0,0x0,0x2a,0x2e,0x30,0x32,0x33,0x34,0x35,0x36,0x36,0x37,0x38,0x38,
	0x38,0x39,0x39,0x3a,0x3a,0x3a,0x3b,0x3b,0x3b,0x3c,0x3c,0x3c,0x3c,0x3c,0x3d,0x3d,0x3d,
	0x3d,0x3e,0x3e,0x3e,0x3e,0x3e,0x3e,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x40,0x40,0x40,
	0x40,0x40,0x40,0x40,0x40,0x40,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x42,0x42,
	0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x43,0x43,0x43,0x43,0x43,0x43,0x43,
	0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,
	0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x45,0x45,0x45,0x45,0x45,0x45,0x45,0x45,0x45,0x45,
	0x45,0x45,0x45,0x45,0x45,0x45,0x45,0x45},

    //demosaic_weight_lut
    {0x0,0x2a,0x2e,0x30,0x32,0x33,0x34,0x35,0x36,0x36,0x37,0x38,0x38,0x38,0x39,0x39,0x3a,
	0x3a,0x3a,0x3b,0x3b,0x3b,0x3c,0x3c,0x3c,0x3c,0x3c,0x3d,0x3d,0x3d,0x3d,0x3e,0x3e,0x3e,
	0x3e,0x3e,0x3e,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
	0x40,0x40,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x42,0x42,0x42,0x42,0x42,0x42,
	0x42,0x42,0x42,0x42,0x42,0x42,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,
	0x43,0x43,0x43,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,
	0x44,0x44,0x44,0x45,0x45,0x45,0x45,0x45,0x45,0x45,0x45,0x45,0x45,0x45,0x45,0x45,0x45,
	0x45,0x45,0x45,0x45,0x45,0x45,0x45,0x45,0x45}
};

static cmos_isp_agc_table_t st_isp_agc_table =
{
    //sharpen_alt_d
    {0xeb,0xea,0xe9,0xe8,0xe7,0xe6,0xe5,0x89},

    //sharpen_alt_ud
    {0xa2,0xa0,0x97,0x91,0x90,0x8e,0x3e,0x3c},

    //snr_thresh
    {0x13,0x14,0x16,0x25,0x2a,0x2e,0x3a,0x44},

    //demosaic_lum_thresh
    {0x40,0x60,0x80,0x80,0x80,0x80,0x80,0x80},

    //demosaic_np_offset
    {0x0,0xa,0x12,0x1a,0x20,0x28,0x30,0x30},

    //ge_strength
    {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55},

    /* saturation */
    {0x80,0x80,0x80,0x80,0x68,0x48,0x35,0x30}

};


static cmos_isp_demosaic_t st_isp_demosaic =
{
    /*vh_slope*/
    0xc2,

    /*aa_slope*/
    0xb3,

    /*va_slope*/
    0xd6,

    /*uu_slope*/
    0x9a,

    /*sat_slope*/
    0x5d,

    /*ac_slope*/
    0xcf,

    /*vh_thresh*/
    0xf0,

    /*aa_thresh*/
    0x58,

    /*va_thresh*/
    0xb5,

    /*uu_thresh*/
    0x356,

    /*sat_thresh*/
    0x171,

    /*ac_thresh*/
    0x1b3
};

/*
 * This function initialises an instance of cmos_inttime_t.
 */
static __inline cmos_inttime_const_ptr_t cmos_inttime_initialize()
{
	cmos_inttime.full_lines_std = 1132;
	cmos_inttime.full_lines_std_30fps = 1132;
    cmos_inttime.full_lines_std_25fps = 1358;
	cmos_inttime.vblanking_lines = 48;

	cmos_inttime.exposure_ashort = 0;
	cmos_inttime.exposure_shift = 0;

	cmos_inttime.lines_per_500ms = cmos_inttime.full_lines_std_30fps * 30 / 2;
	cmos_inttime.flicker_freq = 0;//60*256;//50*256;

	return &cmos_inttime;
}

/*
 * This function applies the new integration time to the ISP registers.
 */
static __inline void cmos_inttime_update(cmos_inttime_ptr_t p_inttime) 
{
#if CMOS_MT9P006_ISP_WRITE_SENSOR_ENABLE
    ISP_I2C_DATA_S stI2cData;

    stI2cData.bDelayCfg = HI_FALSE;
    stI2cData.u8DevAddr = sensor_i2c_addr;
    stI2cData.u32AddrByteNum = sensor_addr_byte;
    stI2cData.u32DataByteNum = sensor_data_byte;
    stI2cData.u32RegAddr = 0x09;
    stI2cData.u32Data = p_inttime->exposure_ashort & 0xffff;
    HI_MPI_ISP_I2cWrite(&stI2cData);        
#else
    //when set exposure time more than one frame, the sensor's fps will be decreased automatically
    sensor_write_register(0x09, p_inttime->exposure_ashort & 0xffff);
    //sensor_write_register(0x08, (p_inttime->exposure_ashort >> 16) & 0xffff);
#endif
}

/*
 * This function applies the new vert blanking porch to the ISP registers.
 */
static __inline void cmos_vblanking_update(cmos_inttime_const_ptr_t p_inttime)
{
    HI_U16 _vblanking_lines = p_inttime->vblanking_lines;

    _vblanking_lines = (_vblanking_lines > 2048)? 2048: _vblanking_lines;
    sensor_write_register(0x06, _vblanking_lines - 1);

}

static __inline HI_U16 vblanking_calculate(
		cmos_inttime_ptr_t p_inttime)
{

	if(p_inttime->exposure_ashort >= p_inttime->full_lines - 2)
	{
		p_inttime->exposure_ashort = p_inttime->full_lines - 2;
	}

	p_inttime->vblanking_lines = p_inttime->full_lines - 1084;
    
	return p_inttime->exposure_ashort;
}

/* Set fps base */
static __inline void cmos_fps_set(
		cmos_inttime_ptr_t p_inttime,
		const HI_U8 fps
		)
{
	switch(fps)
	{
		case 30:
			// Change the frame rate via changing the horizontal blanking
			p_inttime->full_lines_std = cmos_inttime.full_lines_std_30fps;
    		break;
		case 25:
			// Change the frame rate via changing both vertical and horizontal blanking
			p_inttime->full_lines_std = cmos_inttime.full_lines_std_25fps;
    		break;
        default:
            break;
	}
    
}

/*
 * This function initialises an instance of cmos_gains_t.
 */
static __inline cmos_gains_ptr_t cmos_gains_initialize()
{
    cmos_gains.max_again = 126;
	cmos_gains.max_dgain = 128;

	cmos_gains.again_shift = 3;
	cmos_gains.dgain_shift = 3;
	cmos_gains.dgain_fine_shift = 0;
    cmos_gains.isp_dgain_shift = 8;
    cmos_gains.isp_dgain = 1 << cmos_gains.isp_dgain_shift;  
    cmos_gains.max_isp_dgain_target = 4 << cmos_gains.isp_dgain_shift;
    cmos_gains.isp_dgain_delay_cfg = HI_TRUE;
	
	return &cmos_gains;
}

/*
 * This function applies the new gains to the ISP registers.
 */
static __inline void cmos_gains_update(cmos_gains_const_ptr_t p_gains)
{
#if CMOS_MT9P006_ISP_WRITE_SENSOR_ENABLE
    ISP_I2C_DATA_S stI2cData;

    stI2cData.bDelayCfg = HI_TRUE;
    stI2cData.u8DevAddr = sensor_i2c_addr;
    stI2cData.u32AddrByteNum = sensor_addr_byte;
    stI2cData.u32DataByteNum = sensor_data_byte;
    stI2cData.u32RegAddr = 0x35;
    stI2cData.u32Data = (((p_gains->dgain - 8) << 8) 
        + ((p_gains->again > 32) ? (0x40 + (p_gains->again >> 1)) : p_gains->again));
    HI_MPI_ISP_I2cWrite(&stI2cData);

    stI2cData.u32RegAddr = 0x3e;
    if(p_gains->again > 32)
    {
        stI2cData.u32Data = 0x0087;
    }
    else
    {
        stI2cData.u32Data = 0x0007;
    }
    HI_MPI_ISP_I2cWrite(&stI2cData);
#else
    sensor_write_register(0x35, (((p_gains->dgain - 8) << 8) 
        + ((p_gains->again > 32) ? (0x40 + (p_gains->again >> 1)) : p_gains->again)));

    if(p_gains->again > 32)
    {
        sensor_write_register(0x3e, 0x0087);
    }
    else
    {
        sensor_write_register(0x3e, 0x0007);
    }

#endif
}

static __inline HI_U32 analog_gain_from_exposure_calculate(
		cmos_gains_ptr_t p_gains,
		HI_U32 exposure,
		HI_U32 exposure_max,
		HI_U32 exposure_shift)
{
	HI_U32 _again = 12;
	int shft = 0;

	while (exposure > (1 << 27))
	{
		exposure >>= 1;
		exposure_max >>= 1;
		++shft;
	}
    exposure_max = DIV_0_TO_1(exposure_max);

    if(exposure > exposure_max)
    {
    	_again = (exposure << p_gains->again_shift) / exposure_max;
        
        /*limit again to more than 1.5 times*/
    	_again = _again < 12 ? 12 : _again;

    	if(_again > 32) 
    	{
            _again &= ~(HI_U32)0x1;
    	}

    	_again = _again > p_gains->max_again_target ? p_gains->max_again_target : _again;
    }
    else
    {
    }
    exposure = (exposure << p_gains->again_shift) / _again;

	p_gains->again = _again;

	return (exposure << shft);    
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

	HI_U32 _dgain = 1 << p_gains->dgain_shift;
	int shft = 0;

	while (exposure > (1 << 27))
	{
		exposure >>= 1;
		exposure_max >>= 1;
		++shft;
	}
    exposure_max = DIV_0_TO_1(exposure_max);

	if(exposure > exposure_max)
	{
		_dgain = (exposure << p_gains->dgain_shift) / exposure_max;
		_dgain = _dgain < 8 ? 8 : _dgain;
		_dgain = _dgain > p_gains->max_dgain_target ? p_gains->max_dgain_target : _dgain; 

		exposure = (exposure << p_gains->dgain_shift) / _dgain;
	}
	else
	{
	}
    p_gains->dgain = _dgain;

    return  exposure << shft;


}

static void setup_sensor(int isp_mode)
{
	if(0 == isp_mode) /* ISP 'normal' isp_mode */
	{
        sensor_write_register(0x09, 0x046a);
	}
	else if(1 == isp_mode) /* ISP pixel calibration isp_mode */
	{
        //no need to change vblanking_lines
        //set exposure time to 200ms/5 fps
		sensor_write_register(0x09, 0x1a86);

        //min gain
		sensor_write_register(0x35, 0x12);
	}
}

static HI_U32 cmos_gains_lin_to_db_convert(HI_U32 data, HI_U32 shift_in)
{
	HI_U32 _res = 0;
	if(0 == data)
		return _res;

	for(;;)
	{
		data = (data*913 + (1<<9)) >> 10;
		if(data < (1<<shift_in))
			break;
		++_res;
	}
	return _res;
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

static __inline HI_U32 cmos_get_ISO(cmos_gains_ptr_t p_gains)
{
	HI_U32 _again = p_gains->again == 0 ? 1 : p_gains->again;
	HI_U32 _dgain = p_gains->dgain == 0 ? 1 : p_gains->dgain;
    HI_U32 _isp_dgain = p_gains->isp_dgain == 0 ? 1 : p_gains->isp_dgain;

	p_gains->iso =  ((_again * _dgain * _isp_dgain * 100) 
                     >> (p_gains->again_shift + p_gains->dgain_shift + p_gains->isp_dgain_shift));

	return p_gains->iso;
}

static HI_U8 cmos_get_analog_gain(cmos_gains_ptr_t p_gains)
{
    return cmos_gains_lin_to_db_convert(p_gains->again, p_gains->again_shift);
}

static HI_U8 cmos_get_digital_gain(cmos_gains_ptr_t p_gains)
{
    return cmos_gains_lin_to_db_convert(p_gains->dgain, p_gains->dgain_shift);
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

static HI_S32 cmos_get_sensor_max_resolution(cmos_sensor_max_resolution_ptr_t p_cmos_sensor_max_resolution)
{
     if(NULL == p_cmos_sensor_max_resolution)
     {
       printf("null pointer when get sensor max resolution value!\n");
       return -1;
     }

     p_cmos_sensor_max_resolution->u32MaxWidth  = 1920;
     p_cmos_sensor_max_resolution->u32MaxHeight = 1080;

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


#endif // __APTINA_MT9P006_CMOS_H_
