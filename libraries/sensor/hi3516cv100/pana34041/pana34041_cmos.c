#if !defined(__PANA_MN34041_H_)
#define __PANA_MN34041_H_

#include <stdio.h>
#include <unistd.h>		// usleep
#include <string.h>

#include "hi_comm_sns.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"
#include "hi_isp_debug.h"
#include "hi_comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


/****************************************************************************
 * local variables															*
 ****************************************************************************/

#define CMOS_PANA34041_ISP_WRITE_SENSOR_ENABLE (1)
static cmos_inttime_t cmos_inttime;
static cmos_gains_t cmos_gains;
HI_U8 gu8SensorMode = 0;

static cmos_isp_default_t st_coms_isp_default =
{
    // color correction matrix
    {
        5000,
    	{	0x022d, 0x8114, 0x8019,
			0x8055, 0x01e4, 0x808f,
			0x0016, 0x8178, 0x0262
    	},
        3200,
    	{	0x020e, 0x80e7, 0x8027,
			0x809a, 0x01fd, 0x8063,
			0x002a, 0x81ee, 0x02c3
    	},
        2600,
    	{	0x0207, 0x80d5, 0x8032,
			0x8084, 0x01cb, 0x8047,
			0x003f, 0x82c4, 0x0384
    	}
    },

	// black level for R, Gr, Gb, B channels
	{0x100,0x100,0x100,0x100},

    // calibration reference color temperature
    5000,

    //WB gain at 5000K, must keep consistent with calibration color temperature
	{0x1eb, 0x100, 0x100, 0x1c5},

    // WB curve parameters, must keep consistent with reference color temperature.
	{49, 121, -86, 185444, 0x80, -134952},

	// hist_thresh
	{0xd,0x28,0x60,0x80},

	0x00,	// iridix_balck
	0x1,	// rggb

	// gain
	0x8,	0x4,

	// iridix space, intensity, slope_max, white level
	0x02,	0x08,	0x80, 	0x8ff,

	0x1, 	// balance_fe
	0x80,	// ae compensation
	0x8, 	// sinter threshold

	0x1,        //0: use default profile table; 1: use calibrated profile lut, the setting for nr0 and nr1 must be correct.
	0,
	1528,
#if CMOS_PANA34041_ISP_WRITE_SENSOR_ENABLE
    2
#else
    1
#endif
};

static cmos_isp_agc_table_t st_isp_agc_table =
{
    //sharpen_alt_d
    {0x78,0x70,0x68,0x58,0x44,0x30,0x28,0x28},

    //sharpen_alt_ud
    {0x68,0x60,0x58,0x48,0x38,0x30,0x28,0x24},

    //snr_thresh
    {0x20,0x28,0x30,0x38,0x40,0x48,0x54,0x54},

    //demosaic_lum_thresh
    {0x60,0x60,0x40,0x40,0x40,0x30,0x30,0x30},

    //demosaic_np_offset
    {0x0,0xa,0x12,0x1a,0x20,0x28,0x30,0x30},

    //ge_strength
    {0x55,0x55,0x55,0x55,0x55,0x55,0x37,0x37},

    /* saturation */
    {0x80,0x80,0x6C,0x48,0x44,0x40,0x3C,0x38}
};

static cmos_isp_noise_table_t st_isp_noise_table =
{
  //nosie_profile_weight_lut
    {0,  0,  0,  0,  0,  0,  11, 15, 17, 19, 20, 21, 22, 22, 23, 24,
    25, 25, 26, 26, 26, 27, 27, 27, 28, 28, 28, 29, 29, 29, 29, 29,
    30, 30, 30, 30, 30, 31, 31, 31, 31, 31, 32, 32, 32, 32, 32, 32,
    32, 33, 33, 33, 33, 33, 33, 33, 33, 33, 34, 34, 34, 34, 34, 34,
    34, 34, 34, 34, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 36, 36,
    36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 37, 37,
    37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 38, 38,
    38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38},

  //demosaic_weight_lut
    {0,11,15,17,19,20,21,22,23,23,24,25,25,26,26,26,
    27,27,27,28,28,28,29,29,29,29,29,30,30,30,30,30,
    31,31,31,31,31,32,32,32,32,32,32,32,33,33,33,33,
    33,33,33,33,33,34,34,34,34,34,34,34,34,34,34,35,
    35,35,35,35,35,35,35,35,35,35,35,36,36,36,36,36,
    36,36,36,36,36,36,36,36,36,37,37,37,37,37,37,37,
    37,37,37,37,37,37,37,37,37,38,38,38,38,38,38,38,
    38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38}
};

static HI_U32  digital_gain_table[129]=
{ 
 1024,  1035,  1046,  1058,  1069,  1081,  1093,  1104,  1116,  1129,  1141,  1153,  1166,  1178,  1191,  1204, 
 1217,  1230,  1244,  1257,  1271,  1285,  1299,  1313,  1327,  1341,  1356,  1371,  1386,  1401,  1416,  1431, 
 1447,  1462,  1478,  1494,  1511,  1527,  1544,  1560,  1577,  1594,  1612,  1629,  1647,  1665,  1683,  1701, 
 1720,  1738,  1757,  1776,  1795,  1815,  1835,  1855,  1875,  1895,  1916,  1936,  1957,  1979,  2000,  2022, 
 2044,  2066,  2089,  2111,  2134,  2157,  2181,  2204,  2228,  2253,  2277,  2302,  2327,  2352,  2377,  2403, 
 2429,  2456,  2482,  2509,  2537,  2564,  2592,  2620,  2649,  2677,  2706,  2736,  2766,  2796,  2826,  2857, 
 2888,  2919,  2951,  2983,  3015,  3048,  3081,  3114,  3148,  3182,  3217,  3252,  3287,  3323,  3359,  3395, 
 3432,  3470,  3507,  3545,  3584,  3623,  3662,  3702,  3742,  3783,  3824,  3865,  3907,  3950,  3992,  4036, 
 4080
};



static cmos_isp_demosaic_t st_isp_demosaic =
{
    /*vh_slope*/
    200,

    /*aa_slope*/
    220,

    /*va_slope*/
    200,

    /*uu_slope*/
    190,

    /*sat_slope*/
    93,

    /*ac_slope*/
    0xcf,

    /*vh_thresh*/
    0x60,

    /*aa_thresh*/
    0x64,

    /*va_thresh*/
    0x64,

    /*uu_thresh*/
    0x64,

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
	cmos_inttime.full_lines_std_30fps = 1125;
	cmos_inttime.full_lines_std_25fps = 1350;
	cmos_inttime.full_lines_std = 1125;
	cmos_inttime.full_lines = 1125;
	cmos_inttime.max_lines_target = 1122;

	cmos_inttime.full_lines_limit = 65535;
	cmos_inttime.min_lines_target = 1;
	cmos_inttime.vblanking_lines = 1125;

	cmos_inttime.exposure_ashort = 0;

	cmos_inttime.lines_per_500ms = cmos_inttime.full_lines_std_30fps*30/2;
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


#if CMOS_PANA34041_ISP_WRITE_SENSOR_ENABLE 

    ISP_SSP_DATA_S   stSspData;
    stSspData.bDelayCfg = HI_FALSE;
    stSspData.u32DevAddr = 0x00A1;    
    stSspData.u32DevAddrByteNum = 2;
    stSspData.u32RegAddr = 0;
    stSspData.u32RegAddrByteNum = 0;
    stSspData.u32Data = (eshort&0xffff);
    stSspData.u32DataByteNum = 2;
    HI_MPI_ISP_SSPWrite(&stSspData);

    stSspData.u32DevAddr = 0x00A2;    
    stSspData.u32DevAddrByteNum = 2;
    stSspData.u32RegAddr = 0;
    stSspData.u32RegAddrByteNum = 0;
    stSspData.u32Data = 0x2 + ((eshort&0x10000) >> 16);
    stSspData.u32DataByteNum = 2; 
    HI_MPI_ISP_SSPWrite(&stSspData);


    stSspData.u32DevAddr = 0x00A5;    
    stSspData.u32DevAddrByteNum = 2;
    stSspData.u32RegAddr = 0;
    stSspData.u32RegAddrByteNum = 0;
    stSspData.u32Data = exp_frames;
    stSspData.u32DataByteNum = 2;   
    HI_MPI_ISP_SSPWrite(&stSspData);

    
#else

	sensor_write_register(0x00A1, (eshort&0xffff));
	sensor_write_register(0x00A2, (0x2 + ((eshort&0x10000) >> 16) ));  //0x2 is the reserved bits value
	sensor_write_register(0x00A5, exp_frames);
    
#endif


}

/*
 * This function applies the new vert blanking porch to the ISP registers.
 */
static __inline void cmos_vblanking_update(cmos_inttime_const_ptr_t p_inttime)
{
	return;
	int full_lines_lo16 = p_inttime->full_lines & 0xffff;
	int full_lines_hi16= (p_inttime->full_lines >> 16) & 0x1;

	sensor_write_register(0x1A0, full_lines_lo16);
	sensor_write_register(0x1A1, full_lines_hi16);
	sensor_write_register(0x1A3, full_lines_lo16);
	sensor_write_register(0x1A4, full_lines_hi16);
	sensor_write_register(0x1A7, full_lines_lo16);
	sensor_write_register(0x1A8, full_lines_hi16);
}

static __inline HI_U16 vblanking_calculate(
		cmos_inttime_ptr_t p_inttime)
{
	if (p_inttime->exposure_ashort >= p_inttime->full_lines - 3)
	{
		p_inttime->exposure_ashort = p_inttime->full_lines - 3;
	}

	p_inttime->vblanking_lines = p_inttime->full_lines - p_inttime->full_lines_std;

	return p_inttime->exposure_ashort;
}

extern void sensor_init_exit(int fps);

/* Set fps base */
static __inline void cmos_fps_set(
		cmos_inttime_ptr_t p_inttime,
		const HI_U8 fps
		)
{
	usleep(40000);

	if (30 == fps)
	{

//		sensor_init_exit(30);
		p_inttime->full_lines_std = 1125;
		p_inttime->full_lines = 1125;
		p_inttime->max_lines_target = 1122;
	}
	else if (25 == fps)
	{
//		sensor_init_exit(25);
		p_inttime->full_lines_std = 1350;
		p_inttime->full_lines = 1350;
		p_inttime->max_lines_target = 1347;
	}
}

/*
 * This function initialises an instance of cmos_gains_t.
 */
static __inline cmos_gains_ptr_t cmos_gains_initialize()
{
	cmos_gains.max_again = 0x8;
	cmos_gains.max_dgain = 0xff0;

	cmos_gains.again_shift = 0;
	cmos_gains.dgain_shift = 10;
	cmos_gains.dgain_fine = 0;

    cmos_gains.isp_dgain_shift = 8;
    cmos_gains.isp_dgain = 1 << cmos_gains.isp_dgain_shift;
    cmos_gains.max_isp_dgain_target = 4 << cmos_gains.isp_dgain_shift;
    cmos_gains.isp_dgain_delay_cfg = HI_TRUE;

	return &cmos_gains;
}

static __inline HI_U32 cmos_get_ISO(cmos_gains_ptr_t p_gains)
{
    HI_U32 _again = p_gains->again == 0 ? 1 : p_gains->again;
    HI_U32 _dgain = p_gains->dgain == 0 ? 1 : p_gains->dgain;
    HI_U32 _isp_dgain = p_gains->isp_dgain== 0 ? 1 : p_gains->isp_dgain;

    p_gains->iso =  (((HI_U64)_again * _dgain *_isp_dgain* 100) 
        >> (p_gains->again_shift + p_gains->dgain_shift+p_gains->isp_dgain_shift)); 
    
    return p_gains->iso;
}

/*
 * This function applies the digital gain
 * input and output offset and correction
 */
static __inline void cmos_gains_update(cmos_gains_const_ptr_t p_gains)
{ 
#if CMOS_PANA34041_ISP_WRITE_SENSOR_ENABLE 
    ISP_SSP_DATA_S stSspData;

    stSspData.bDelayCfg = HI_TRUE;
    stSspData.u32DevAddr = 0x0020;    
    stSspData.u32DevAddrByteNum = 2;
    stSspData.u32RegAddr = 0;
    stSspData.u32RegAddrByteNum = 0;

    switch (p_gains->again_db)  
    {
    case 0:
        //r_colgsw = 0dB, r_a_gain = 0dB
        stSspData.u32Data = 0x0080;
        stSspData.u32DataByteNum = 2;
        HI_MPI_ISP_SSPWrite(&stSspData);
        break;
    case 6:
        //r_colgsw = 6dB, r_a_gain = 0dB
        stSspData.u32Data = 0x8080;
        stSspData.u32DataByteNum = 2;
        HI_MPI_ISP_SSPWrite(&stSspData);
        break;
    case 12:
        //r_colgsw = 12dB, r_a_gain = 0dB
        stSspData.u32Data = 0xC080;
        stSspData.u32DataByteNum = 2;
        HI_MPI_ISP_SSPWrite(&stSspData);
        break;
    case 18:
        //r_colgsw = 12dB, r_a_gain = 6dB
        stSspData.u32Data = 0xC0C0;
        stSspData.u32DataByteNum = 2;
        HI_MPI_ISP_SSPWrite(&stSspData);
        break;
    default:
        break;
    }

    stSspData.u32DevAddr = 0x0021;    
    stSspData.u32DevAddrByteNum = 2;
    stSspData.u32Data = 0x80 + p_gains->dgain_db;
    stSspData.u32DataByteNum = 2;
    HI_MPI_ISP_SSPWrite(&stSspData);
    
#else
    switch (p_gains->again_db)
    {
    case 0:
        //r_colgsw = 0dB, r_a_gain = 0dB
        sensor_write_register(0x20, 0x0080);
        break;
    case 6:
        //r_colgsw = 6dB, r_a_gain = 0dB
        sensor_write_register(0x20, 0x8080);
        break;
    case 12:
        //r_colgsw = 12dB, r_a_gain = 0dB
        sensor_write_register(0x20, 0xC080);
        break;
    case 18:
        //r_colgsw = 12dB, r_a_gain = 6dB
        sensor_write_register(0x20, 0xC0C0);
        break;
    default:
        break;
    }

    sensor_write_register(0x21, 0x80 + p_gains->dgain_db);
#endif
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

	// normalize
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
    exposure_max = DIV_0_TO_1(exposure_max);

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
        HI_U32 exposure_shift)
{
    HI_U32 dgain = 1024;
    HI_U32 step = 0;
    HI_S32 shift = 0;
    HI_S32 i = 0;
    

    while (exposure > (1 << 20))
    {
        exposure     >>= 1;
        exposure_max >>= 1;
        ++shift;
    }
    
    if(exposure > exposure_max)
    {
        dgain = (exposure << 10) / exposure_max;
        dgain = dgain < 1024? 1024: dgain;
        dgain = dgain > p_gains->max_dgain_target? p_gains->max_dgain_target: dgain;
    
        if (dgain >= digital_gain_table[128])
        {
             dgain = digital_gain_table[128];
             step = 128;
        }
        else
        {
            for(i = 1; i < 129; i++)
            {
                if(dgain < digital_gain_table[i])
                {
                    dgain = digital_gain_table[i-1];
                    step = i-1 ;
                    break;
                }
            }
        }
        
        exposure = (exposure << 10) / dgain;
    }
    
    
    p_gains->dgain = dgain;
    p_gains->dgain_db = step;
    
    return exposure << shift;
}

static void setup_sensor(int isp_mode)
{
	if(0 == isp_mode) /* setup for ISP 'normal mode' */
	{
		sensor_write_register(0x00A5,0x0000);
	}
	else if(1 == isp_mode) /* setup for ISP pixel calibration mode */
	{
        //set the gain to 0
		sensor_write_register(0x0020,0x0080);
		sensor_write_register(0x0021,0x0080);
		sensor_write_register(0x00A1,0x0400);
		sensor_write_register(0x00A2,0x0002);
		sensor_write_register(0x00A5,0x0005);
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
/*
static HI_U8 cmos_get_digital_fine_gain(cmos_gains_ptr_t cmos_gains)
{
    return cmos_gains->dgain_fine;
}
*/
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


#endif // __PANA_MN34041_H_
