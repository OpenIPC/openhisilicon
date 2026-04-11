#if !defined(__AR0130_CMOS_H_)
#define __AR0130_CMOS_H_

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
HI_U8 gu8SensorMode = 0;

/*set Frame End Update Mode 2 with HI_MPI_ISP_SetAEAttr and set this value 1 to avoid flicker */
/*when use Frame End Update Mode 2, the speed of i2c will affect whole system's performance   */
/*increase I2C_DFT_RATE in Hii2c.c to 400000 to increase the speed of i2c                     */

#define CMOS_AR0130_ISP_WRITE_SENSOR_ENABLE (1)

static cmos_isp_default_t st_coms_isp_default =
{
    {
       5000,
       {
         0x01a2, 0x8080, 0x8022,
         0x8068, 0x0178, 0x8010,
         0x8008, 0x811A, 0x0222
       }, 
       
       3200,
       {
         0x01e0,0x807b,0x8065,
         0x804f,0x0165,0x8016,
         0x001f,0x8126,0x0206
       },
       
       2600,
       {
         0x0200, 0x80c3, 0x803D,
         0x8057, 0x015C, 0x8005,
         0x8020, 0x8290, 0x03b0
       }
    },

	// black level for R, Gr, Gb, B channels
	{0xC8,0xC8,0xC8,0xC8},

	//calibration reference color temperature
	5000,

	//WB gain at Macbeth 5000K, must keep consistent with calibration color temperature

    // {0x181,0x100,0x100,0x1b3}, //for demo
    //{0x0197,0x0100,0x0100,0x01C4},//for ref

    {0x018b,0x0100,0x0100,0x01a7},//7.5

	// WB curve parameters, must keep consistent with reference color temperature.

    // {96,-23,-182,225343,128,-178914}, //for demo
  //  {66,36,-153,213987,128,-166117},  //for ref
     {86,-16,-182,243836,128,-195566},


	// hist_thresh
	{0xd,0x28,0x60,0x80},

	0x00,	// iridix_balck
	0x1,	// rggb

	// gain
	23,	0x8, // this is gain target, it will be constricted by sensor-gain.
	//0x8,	0x4, /* The purpose of setting max dgain target to 4 is to reduce FPN */

	//wdr_variance_space, wdr_variance_intensity, slope_max_write,  white_level_write
	0x04,	0x01,	0x80, 	0x4FF,

	0x1, 	// balance_fe
	0x80,	// ae compensation
	0x23, 	// sinter threshold

	0x1,     //noise profile=0, use the default noise profile lut, don't need to set nr0 and nr1
	0x0,
	546,

    2
};

static cmos_isp_agc_table_t st_isp_agc_table =
{
    /* sharpen_alt_d */
   {50,50,45,35,30,30,20,15}, 
    
    /* sharpen_alt_ud */
   {45,45,35,30,25,25,15,10},  

    //snr_thresh
    {8,12,18,26,36,46,56,66},

    //demosaic_lum_thresh
    {0x50,0x50,0x40,0x40,0x30,0x30,0x20,0x20},

    //demosaic_np_offset
    {0x0,0xa,0x12,0x1a,0x20,0x28,0x30,0x35},

    //ge_strength
    {0x55,0x55,0x55,0x55,0x55,0x55,0x37,0x37},

    /* saturation */
    {0x80,0x80,0x6C,0x48,0x44,0x40,0x3C,0x38}
};

static cmos_isp_noise_table_t st_isp_noise_table =
{
    //nosie_profile_weight_lut
    {0,  0,  0,  0,  0,  0,  0, 12, 17, 21, 24, 26, 28, 30, 31, 32, 
    33, 34, 35, 36, 37, 38, 39, 39, 40, 41, 41, 42, 42, 43, 43, 44,
    44, 45, 45, 45, 46, 46, 46, 47, 47, 47, 48, 48, 48, 49, 49, 49,
    50, 50, 50, 50, 51, 51, 51, 51, 52, 52, 52, 52, 52, 53, 53, 53,
    53, 53, 54, 54, 54, 54, 54, 55, 55, 55, 55, 55, 55, 56, 56, 56,
    56, 56, 56, 56, 57, 57, 57, 57, 57, 57, 57, 58, 58, 58, 58, 58,
    58, 58, 58, 59, 59, 59, 59, 59, 59, 59, 59, 60, 60, 60, 60, 60,
    60, 60, 60, 60, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 62, 62},

    //demosaic_weight_lut
    {0, 12, 17, 21, 24, 26, 28, 30, 31, 32, 33, 34, 35, 36, 37, 38, 
    39, 39, 40, 41, 41, 42, 42, 43, 43, 44, 44, 45, 45, 45, 46, 46, 
    46, 47, 47, 47, 48, 48, 48, 49, 49, 49, 50, 50, 50, 50, 51, 51, 
    51, 51, 52, 52, 52, 52, 52, 53, 53, 53, 53, 53, 54, 54, 54, 54, 
    54, 55, 55, 55, 55, 55, 55, 56, 56, 56, 56, 56, 56, 56, 57, 57, 
    57, 57, 57, 57, 57, 58, 58, 58, 58, 58, 58, 58, 58, 59, 59, 59, 
    59, 59, 59, 59, 59, 60, 60, 60, 60, 60, 60, 60, 60, 60, 61, 61, 
    61, 61, 61, 61, 61, 61, 61, 61, 62, 62, 62, 62, 62, 62, 62, 62}
    
};

static cmos_isp_demosaic_t st_isp_demosaic =
{
    /*vh_slope*/
    220,

    /*aa_slope*/
    200,

    /*va_slope*/
    185,

    /*uu_slope*/
    210,

    /*sat_slope*/
    93,

    /*ac_slope*/
    160,

    /*vh_thresh*/
    0,

    /*aa_thresh*/
    0,

    /*va_thresh*/
    0,

    /*uu_thresh*/
    8,

    /*sat_thresh*/
    0,

    /*ac_thresh*/
    0x1b3
};

static cmos_isp_gamma_table_t st_isp_gamma_table =
{
#if 1

{0   ,120 ,220 ,310 ,390 ,470 ,540 ,610 ,670 ,730 ,786 ,842 ,894 ,944 ,994 ,1050,    
1096,1138,1178,1218,1254,1280,1314,1346,1378,1408,1438,1467,1493,1519,1543,1568,    
1592,1615,1638,1661,1683,1705,1726,1748,1769,1789,1810,1830,1849,1869,1888,1907,    
1926,1945,1963,1981,1999,2017,2034,2052,2069,2086,2102,2119,2136,2152,2168,2184,    
2200,2216,2231,2247,2262,2277,2292,2307,2322,2337,2351,2366,2380,2394,2408,2422,    
2436,2450,2464,2477,2491,2504,2518,2531,2544,2557,2570,2583,2596,2609,2621,2634,    
2646,2659,2671,2683,2696,2708,2720,2732,2744,2756,2767,2779,2791,2802,2814,2825,    
2837,2848,2859,2871,2882,2893,2904,2915,2926,2937,2948,2959,2969,2980,2991,3001,    
3012,3023,3033,3043,3054,3064,3074,3085,3095,3105,3115,3125,3135,3145,3155,3165,    
3175,3185,3194,3204,3214,3224,3233,3243,3252,3262,3271,3281,3290,3300,3309,3318,    
3327,3337,3346,3355,3364,3373,3382,3391,3400,3409,3418,3427,3436,3445,3454,3463,    
3471,3480,3489,3498,3506,3515,3523,3532,3540,3549,3557,3566,3574,3583,3591,3600,    
3608,3616,3624,3633,3641,3649,3657,3665,3674,3682,3690,3698,3706,3714,3722,3730,    
3738,3746,3754,3762,3769,3777,3785,3793,3801,3808,3816,3824,3832,3839,3847,3855,    
3862,3870,3877,3885,3892,3900,3907,3915,3922,3930,3937,3945,3952,3959,3967,3974,    
3981,3989,3996,4003,4010,4018,4025,4032,4039,4046,4054,4061,4068,4075,4082,4089,4095}


#else  /*higher  contrast*/

 0  ,  54 ,  106,  158,  209,  259,  308,  356,  403,  450,  495,  540,  584,  628,  670,  713, 
754 , 795 ,  835,  874,  913,  951,  989, 1026, 1062, 1098, 1133, 1168, 1203, 1236, 1270, 1303, 
1335, 1367, 1398, 1429, 1460, 1490, 1520, 1549, 1578, 1607, 1635, 1663, 1690, 1717, 1744, 1770,
1796, 1822, 1848, 1873, 1897, 1922, 1946, 1970, 1993, 2017, 2040, 2062, 2085, 2107, 2129, 2150, 
2172, 2193, 2214, 2235, 2255, 2275, 2295, 2315, 2335, 2354, 2373, 2392, 2411, 2429, 2447, 2465, 
2483, 2501, 2519, 2536, 2553, 2570, 2587, 2603, 2620, 2636, 2652, 2668, 2684, 2700, 2715, 2731, 
2746, 2761, 2776, 2790, 2805, 2819, 2834, 2848, 2862, 2876, 2890, 2903, 2917, 2930, 2944, 2957, 
2970, 2983, 2996, 3008, 3021, 3033, 3046, 3058, 3070, 3082, 3094, 3106, 3118, 3129, 3141, 3152, 
3164, 3175, 3186, 3197, 3208, 3219, 3230, 3240, 3251, 3262, 3272, 3282, 3293, 3303, 3313, 3323, 
3333, 3343, 3352, 3362, 3372, 3381, 3391, 3400, 3410, 3419, 3428, 3437, 3446, 3455, 3464, 3473, 
3482, 3490, 3499, 3508, 3516, 3525, 3533, 3541, 3550, 3558, 3566, 3574, 3582, 3590, 3598, 3606, 
3614, 3621, 3629, 3637, 3644, 3652, 3660, 3667, 3674, 3682, 3689, 3696, 3703, 3711, 3718, 3725, 
3732, 3739, 3746, 3752, 3759, 3766, 3773, 3779, 3786, 3793, 3799, 3806, 3812, 3819, 3825, 3831, 
3838, 3844, 3850, 3856, 3863, 3869, 3875, 3881, 3887, 3893, 3899, 3905, 3910, 3916, 3922, 3928, 
3933, 3939, 3945, 3950, 3956, 3962, 3967, 3973, 3978, 3983, 3989, 3994, 3999, 4005, 4010, 4015, 
4020, 4026, 4031, 4036, 4041, 4046, 4051, 4056, 4061, 4066, 4071, 4076, 4081, 4085, 4090, 4095, 4095


#endif

};

/*
 * This function initialises an instance of cmos_inttime_t.
 */
static /*__inline*/ cmos_inttime_const_ptr_t cmos_inttime_initialize()
{
    //TODO: min/max integration time control.
   	//cmos_inttime.min_lines_std = 128;
	cmos_inttime.full_lines_std = 750;
	cmos_inttime.full_lines_std_30fps = 750;
	cmos_inttime.full_lines = 750;
	cmos_inttime.full_lines_del = 750; //TODO: remove
	cmos_inttime.full_lines_limit = 65535;
	cmos_inttime.max_lines = 748;
	cmos_inttime.min_lines = 2;
	cmos_inttime.vblanking_lines = 0;

	cmos_inttime.exposure_ashort = 0;
	cmos_inttime.exposure_shift = 0;

	cmos_inttime.lines_per_500ms = 750*30/2; // 500ms / 22.22us
	cmos_inttime.flicker_freq = 0;//60*256;//50*256;

	cmos_inttime.max_lines_target = cmos_inttime.max_lines;
	cmos_inttime.min_lines_target = cmos_inttime.min_lines;
	//cmos_inttime.max_flicker_lines = cmos_inttime.max_lines_target;
	//cmos_inttime.min_flicker_lines = cmos_inttime.min_lines_target;
	//cmos_inttime.input_changed = 0;
	return &cmos_inttime;
}

/*
 * This function applies the new integration time to the ISP registers.
 */
static __inline void cmos_inttime_update(cmos_inttime_ptr_t p_inttime)
{
#if CMOS_AR0130_ISP_WRITE_SENSOR_ENABLE
    ISP_I2C_DATA_S stI2cData;


    stI2cData.bDelayCfg = HI_FALSE;
    stI2cData.u8DevAddr = sensor_i2c_addr;
    stI2cData.u32AddrByteNum = sensor_addr_byte;
    stI2cData.u32DataByteNum = sensor_data_byte;
    stI2cData.u32RegAddr = 0x3012;
    stI2cData.u32Data = p_inttime->exposure_ashort >> p_inttime->exposure_shift;
    HI_MPI_ISP_I2cWrite(&stI2cData);        
#else
	HI_U16 _time = p_inttime->exposure_ashort >> p_inttime->exposure_shift;
	sensor_write_register(0x3012, _time);
#endif
}

/*
 * This function applies the new vert blanking porch to the ISP registers.
 */
static __inline void cmos_vblanking_update(cmos_inttime_const_ptr_t p_inttime)
{
     int  _fulllines= p_inttime->full_lines;

       sensor_write_register(0x300A, _fulllines);

}

static __inline HI_U16 vblanking_calculate(
		cmos_inttime_ptr_t p_inttime)
{
	p_inttime->exposure_along  = p_inttime->exposure_ashort;

	if(p_inttime->exposure_along < p_inttime->full_lines_std - 2)
	{
		p_inttime->full_lines_del = p_inttime->full_lines_std;
	}
	if(p_inttime->exposure_along >= p_inttime->full_lines_std - 2)
	{
		p_inttime->full_lines_del = p_inttime->exposure_along + 2;
	}

    if(p_inttime->exposure_ashort > p_inttime->full_lines - 2)
    {
       p_inttime->exposure_ashort = p_inttime->full_lines - 2;
    }
    
    
#if defined(TRACE_ALL)
	alt_printf("full_lines_del = %x\n", p_inttime->full_lines_del);
#endif
	p_inttime->vblanking_lines = p_inttime->full_lines_del - 720;
#if defined(TRACE_ALL)
	alt_printf("vblanking_lines = %x\n", p_inttime->vblanking_lines);
#endif
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
            p_inttime->lines_per_500ms = cmos_inttime.full_lines_std_30fps * 30 / 2;
			sensor_write_register(0x300c, 0x8ba);
		break;

		case 25:
            p_inttime->lines_per_500ms = cmos_inttime.full_lines_std_30fps * 25 / 2;
			sensor_write_register(0x300c, 0xa78);
		break;

		default:
		break;
	}
}

/*
 * This function initialises an instance of cmos_gains_t.
 */
static /*__inline*/ cmos_gains_ptr_t cmos_gains_initialize()
{

	cmos_gains.max_again = 360;
	cmos_gains.max_dgain = 255;
	cmos_gains.max_again_target = cmos_gains.max_again;
	cmos_gains.max_dgain_target = cmos_gains.max_dgain;

	cmos_gains.again_shift = 4;
	cmos_gains.dgain_shift = 5;
	cmos_gains.dgain_fine_shift = 0;

	cmos_gains.again = 1;
	cmos_gains.dgain = 1;
	cmos_gains.dgain_fine = 1;
	cmos_gains.again_db = 0;
	cmos_gains.dgain_db = 0;

    cmos_gains.isp_dgain_shift = 8;
    cmos_gains.isp_dgain = 1 << cmos_gains.isp_dgain_shift;
    cmos_gains.max_isp_dgain_target = 4 << cmos_gains.isp_dgain_shift;
    cmos_gains.isp_dgain_delay_cfg = HI_FALSE;

//	cmos_gains.input_changed = 0;

	return &cmos_gains;
}

/*
 * This function applies the new gains to the ISP registers.
 */
static __inline void cmos_gains_update(cmos_gains_const_ptr_t p_gains)
{
#if CMOS_AR0130_ISP_WRITE_SENSOR_ENABLE
    int dg = p_gains->dgain;
    ISP_I2C_DATA_S stI2cData;

    stI2cData.bDelayCfg = HI_FALSE;
    stI2cData.u8DevAddr = sensor_i2c_addr;
    stI2cData.u32AddrByteNum = sensor_addr_byte;
    stI2cData.u32DataByteNum = sensor_data_byte;
    stI2cData.u32RegAddr = 0x3100;
    if(p_gains->again_db & 1)
    {
        stI2cData.u32Data = 0x0004;
        HI_MPI_ISP_I2cWrite(&stI2cData);
    }
    else
    {
        stI2cData.u32Data = 0x0000;
        HI_MPI_ISP_I2cWrite(&stI2cData);
    }

    stI2cData.bDelayCfg = HI_FALSE;
    stI2cData.u8DevAddr = sensor_i2c_addr;
    stI2cData.u32AddrByteNum = sensor_addr_byte;
    stI2cData.u32DataByteNum = sensor_data_byte;
    stI2cData.u32RegAddr = 0x30B0;
    switch(p_gains->again_db >> 1)
	{
		case(0):
            stI2cData.u32Data = 0x1300;
            HI_MPI_ISP_I2cWrite(&stI2cData);
			break;
		case(1):
			stI2cData.u32Data = 0x1300;
            HI_MPI_ISP_I2cWrite(&stI2cData);
			break;
		case(2):
            stI2cData.u32Data = 0x1310;
            HI_MPI_ISP_I2cWrite(&stI2cData);
			break;
		case(4):
            stI2cData.u32Data = 0x1320;
            HI_MPI_ISP_I2cWrite(&stI2cData);
			break;
		case(8):
            stI2cData.u32Data = 0x1330;
            HI_MPI_ISP_I2cWrite(&stI2cData);
			break;
	}

    stI2cData.u32RegAddr = 0x305E;
    stI2cData.u32Data = dg;
    HI_MPI_ISP_I2cWrite(&stI2cData);
#else
    int ag = p_gains->again;
    int dg = p_gains->dgain;
	if(p_gains->again_db & 1)
    {
        sensor_write_register(0x3100, 0x0004);
    }
    else
    {
        sensor_write_register(0x3100, 0x0000);
    }
    
    switch(p_gains->again_db >> 1)
	{
		case(0):
			sensor_write_register(0x30B0, 0x1300);
			break;
		case(1):
			sensor_write_register(0x30B0, 0x1300);
			break;
		case(2):
			sensor_write_register(0x30B0, 0x1310);
			break;
		case(4):
			sensor_write_register(0x30B0, 0x1320);
			break;
		case(8):
			sensor_write_register(0x30B0, 0x1330);
			break;
	}

	sensor_write_register(0x305E, dg);
#endif
}

/* Emulate digital fine gain */
static __inline void em_dgain_fine_update(cmos_gains_ptr_t p_gains)
{
}

static HI_U32 cmos_gains_lin_to_db_convert(HI_U32 data, HI_U32 shift_in)
{
    #define PRECISION 8
	HI_U32 _res = 0;
	if(0 == data)
		return _res;

    data = data << PRECISION; // to ensure precision.
	for(;;)
	{
        /* Note to avoid endless loop here. */
		data = (data * 913) >> 10;
        // data = (data*913 + (1<<9)) >> 10; // endless loop when shift_in is 0. */
		if(data <= ((1<<shift_in) << PRECISION))
		{
			break;
		}
		++_res;
	}
	return _res;
}

static __inline HI_U32 analog_gain_from_exposure_calculate(
		cmos_gains_ptr_t p_gains,
		HI_U32 exposure,
		HI_U32 exposure_max,
		HI_U32 exposure_shift)
{
	HI_U32 _again = 1 << p_gains->again_shift;
    HI_U32 _again_tmp;
	int shft = 0;
    HI_U32 exposure0 = exposure;

	while (exposure > (1 << 26))
	{
		exposure >>= 1;
		exposure_max >>= 1;
		++shft;
	}

    exposure_max = DIV_0_TO_1(exposure_max);

    if(exposure > exposure_max)
	{
		_again_tmp = (exposure << 4) / exposure_max;

		if (_again_tmp >= 45)
        {
            p_gains->again_db = 1;
            _again = 45;
            exposure = (exposure << 4) / 45;
        }
        else
        {
            p_gains->again_db = 0;
        }
	}
	else
	{
	}
    
	if(exposure > exposure_max)
	{
        //when setting manual exposure line, exposure_max>>shift should not be 0.
		_again_tmp = exposure  / exposure_max;

		if (_again_tmp >= 1<< 3) { _again_tmp = 1<<3; }
		else if (_again_tmp >= 1<< 2) { _again_tmp = 1<<2; }
		else if (_again_tmp >= 1<< 1) { _again_tmp = 1<<1; }
		else if (_again_tmp >= 1)     { _again_tmp = 1;    }

		_again = _again * _again_tmp;      
        
		_again = _again < (1<<p_gains->again_shift) ? (1<<p_gains->again_shift) : _again;
		_again = _again > p_gains->max_again_target ? p_gains->max_again_target : _again;


        if(_again >= 45)
        {
            if (_again >= 45<<3 ) { _again_tmp = 1<<3; }
        	else if (_again >= 45<< 2) { _again_tmp = 1<<2; }
        	else if (_again >= 45<< 1) { _again_tmp = 1<<1; }
        	else if (_again >= 45)     { _again_tmp = 1;    }

            p_gains->again_db |= _again_tmp << 1;
        }
        else
        {
            if (_again >= 16<< 3) { _again_tmp = 1<<3; }
        	else if (_again >= 16<< 2) { _again_tmp = 1<<2; }
        	else if (_again >= 16<< 1) { _again_tmp = 1<<1; }
        	else if (_again >= 16)     { _again_tmp = 1;    } 

            p_gains->again_db |= _again_tmp << 1;

        }
        
		exposure = ((exposure0 << p_gains->again_shift) / _again);

	}
	else
	{
		//_again = (_again * exposure) / (exposure / exposure_shift) / exposure_shift;
	} 
    
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
    HI_S32 shift = 0;

    while (exposure > (1 << 22))
    {
        exposure >>= 1;
        exposure_max >>= 1;
        ++shift;
    }
    exposure_max = (exposure_max == 0)? 1: exposure_max;

    if(exposure > exposure_max)
    {
        isp_dgain = ((exposure << p_gains->isp_dgain_shift) + (exposure_max >> 1)) / exposure_max;
        exposure = exposure_max;
        isp_dgain = (isp_dgain > p_gains->max_isp_dgain_target) ? (p_gains->max_isp_dgain_target) : isp_dgain;        
    }
    
    p_gains->isp_dgain = isp_dgain;

    return exposure << shift;
}


static __inline HI_U32 digital_gain_from_exposure_calculate(
		cmos_gains_ptr_t p_gains,
		HI_U32 exposure,
		HI_U32 exposure_max,
		HI_U32 exposure_shift
		)
{
	HI_U32 _dgain = 1<<p_gains->dgain_shift;
    HI_U32 exposure0;
    int shft = 0;

	while (exposure > (1<<20))
	{
		exposure >>= 1;
		exposure_max >>= 1;
		++shft;
	}
    
    exposure0 = exposure;
	if(exposure > exposure_max)
	{
	    //when setting manual exposure line, exposure_max>>shift should not be 0.
            exposure_max = DIV_0_TO_1(exposure_max);
            _dgain = (exposure  * _dgain) / exposure_max;
            exposure = exposure_max;
	}
	else
	{
        //TODO: after anti-flick, dgain may need to decrease.
		//_dgain = (_dgain * exposure) / (exposure / exposure_shift) / exposure_shift;
	}
	_dgain = _dgain < (1<<p_gains->dgain_shift) ? (1<<p_gains->dgain_shift) : _dgain;
	_dgain = (_dgain > p_gains->max_dgain_target) ? p_gains->max_dgain_target : _dgain;

	p_gains->dgain = _dgain;
    p_gains->dgain_db = cmos_gains_lin_to_db_convert(p_gains->dgain, p_gains->dgain_shift);

	return exposure << shft;
}

static __inline void sensor_update(
	cmos_gains_const_ptr_t p_gains,
	cmos_inttime_ptr_t p_inttime,
	HI_U8 frame
    )
{
	if(frame == 0)
	{
		cmos_inttime_update(p_inttime);
	}
	if(frame == 1)
	{
		cmos_gains_update(p_gains);
	}
}

static __inline HI_U32 cmos_get_ISO(cmos_gains_ptr_t p_gains)
{
	HI_U32 _again = p_gains->again == 0 ? 1 : p_gains->again;
	HI_U32 _dgain = p_gains->dgain == 0 ? 1 : p_gains->dgain;
    HI_U32 _ispdgain = p_gains->isp_dgain == 0 ? 1 : p_gains->isp_dgain;

	p_gains->iso =  ((_again * _dgain * _ispdgain * 100) 
        >> (p_gains->again_shift + p_gains->dgain_shift 
        + p_gains->isp_dgain_shift));

	return p_gains->iso;
}

static HI_U8 cmos_get_analog_gain(cmos_gains_ptr_t p_gains)
{
    //return cmos_gains_lin_to_db_convert(p_gains->again, p_gains->again_shift);
    return p_gains->again_db;
}

static HI_U8 cmos_get_digital_gain(cmos_gains_ptr_t p_gains)
{
    //return cmos_gains_lin_to_db_convert(p_gains->dgain, p_gains->dgain_shift);
    return p_gains->dgain_db;
}

#if 0
static HI_U8 cmos_get_digital_fine_gain(cmos_gains_ptr_t p_gains)
{
    return cmos_gains_lin_to_db_convert(p_gains->dgain_fine, p_gains->dgain_shift);
}
#endif

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


static HI_U32 cmos_get_isp_gamma_table(cmos_isp_gamma_table_ptr_t p_cmos_isp_gamma_table)
{
   if(NULL == p_cmos_isp_gamma_table)
   {
    printf("null pointer when get isp gamma table!\n");
    return -1;
   }
   memcpy(p_cmos_isp_gamma_table,&st_isp_gamma_table, sizeof(cmos_isp_gamma_table_t));
   return 0;
   
}


void setup_sensor(int isp_mode)
{
    if(0 == isp_mode) /* ISP 'normal' isp_mode */
    {
        sensor_write_register(0x300C, 0xa78);	//25fps
        sensor_write_register(0x3ee4, 0xd308);
 	    sensor_write_register(0x3100, 0x0004);
    }
    else if(1 == isp_mode) /* ISP pixel calibration isp_mode */
    {
        sensor_write_register(0x300C, 0x4D58);	//5fps
        sensor_write_register(0x3012, 0x05DA);	//max exposure lines
        sensor_write_register(0x30B0, 0x1300);	//AG, Context A	1
        sensor_write_register(0x305E, 0x0020);	//DG, Context A      	1
        sensor_write_register(0x3100, 0x0000);
	    sensor_write_register(0x3ee4, 0xd208);
    }
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

static SENSOR_EXP_FUNC_S stSensorExpFuncs =
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
	.pfn_cmos_get_isp_gamma_table = cmos_get_isp_gamma_table,
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



#endif // __AR0130_CMOS_H_