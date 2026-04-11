#if !defined(__HIMAX_1375_CMOS_H_)
#define __HIMAX_1375_CMOS_H_

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

#define CMOS_HIMAX1375_ISP_WRITE_SENSOR_ENABLE (1)
/****************************************************************************
 * local variables															*
 ****************************************************************************/
extern const unsigned int sensor_i2c_addr;
extern const unsigned int sensor_addr_byte;
extern const unsigned int sensor_data_byte;


static cmos_inttime_t cmos_inttime;
static cmos_gains_t cmos_gains;
HI_U8 gu8SensorMode = 0;

static cmos_isp_default_t st_coms_isp_default =
{
	// color matrix[9]
    {
        4950,
        {   0x1bb, 0x8083, 0x8038,
            0x803c, 0x01a1, 0x8065,
            0x8002, 0x80f2, 0x01f4
        },
        3100,
        {   0x016e, 0x8028, 0x8046,
            0x8064, 0x01a0, 0x803c,
            0x8035, 0x813b, 0x0270
        },
        2480,
        {   0x01a8, 0x8008, 0x80a0,
            0x805f, 0x01b0, 0x8051,
            0x808e, 0x8204, 0x0392
        }
    },


    // black level
    {0,0,0,0},

    //calibration reference color temperature
    4950,

    //WB gain at 5000, must keep consistent with calibration color temperature
    //{0x185, 0x100, 0x100, 0x17d},
    {0x187, 0x100, 0x100, 0x179},
    

    // WB curve parameters, must keep consistent with reference color temperature.   
    //{110,16,-127,138235,128,-87002},//newest
    {123,-15,-144,136180,128,-85414},
	// hist_thresh
    {0xd,0x28,0x60,0x80},
    //{0x10,0x40,0xc0,0xf0},

    0x0,	// iridix_balck
    0x3,	// bggr

	/* limit max gain for reducing noise, the max dgain is 169, about 2.64 times*/
    16,  3,

	// iridix
    0x04,	0x08,	0xa0, 	0x4ff,

    0x1, 	// balance_fe
    0x80,	// ae compensation
    0x15, 	// sinter threshold

    0x0,  0,  0,  //noise profile=0, use the default noise profile lut, don't need to set nr0 and nr1
#if CMOS_HIMAX1375_ISP_WRITE_SENSOR_ENABLE 
    2
#else
    1
#endif
};

static cmos_isp_agc_table_t st_isp_agc_table =
{
    //sharpen_alt_d
    {0x8e,0x8b,0x88,0x83,0x7d,0x76,0x76,0x76},

    //sharpen_alt_ud
    {0x8f,0x89,0x7e,0x78,0x6f,0x3c,0x3c,0x3c},

    //snr_thresh
    {0x19,0x1e,0x2d,0x32,0x39,0x3f,0x3f,0x3f},

    //demosaic_lum_thresh
    {0x40,0x60,0x80,0x80,0x80,0x80,0x80,0x80},

    //demosaic_np_offset
    {0x0,0xa,0x12,0x1a,0x20,0x28,0x30,0x30},

    //ge_strength
    {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55},

    /* saturation */
    {0x80,0x80,0x80,0x80,0x68,0x48,0x35,0x30}

};

static cmos_isp_noise_table_t st_isp_noise_table =
{
    //nosie_profile_weight_lut
    {0, 27, 31, 33, 35, 36, 37, 38, 39, 40, 40, 41, 41, 42, 42, 43,
    43, 43, 44, 44, 44, 45, 45, 45, 45, 46, 46, 46, 46, 46, 47, 47,
    47, 47, 47, 48, 48, 48, 48, 48, 48, 48, 49, 49, 49, 49, 49, 49,
    49, 49, 50, 50, 50, 50, 50, 50, 50, 50, 50, 51, 51, 51, 51, 51,
    51, 51, 51, 51, 51, 51, 51, 52, 52, 52, 52, 52, 52, 52, 52, 52,
    52, 52, 52, 52, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
    53, 53, 53, 53, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
    54, 54, 54, 54, 54, 54, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55},

    //demosaic_weight_lut
    {0, 27, 31, 33, 35, 36, 37, 38, 39, 40, 40, 41, 41, 42, 42, 43,
    43, 43, 44, 44, 44, 45, 45, 45, 45, 46, 46, 46, 46, 46, 47, 47,
    47, 47, 47, 48, 48, 48, 48, 48, 48, 48, 49, 49, 49, 49, 49, 49,
    49, 49, 50, 50, 50, 50, 50, 50, 50, 50, 50, 51, 51, 51, 51, 51,
    51, 51, 51, 51, 51, 51, 51, 52, 52, 52, 52, 52, 52, 52, 52, 52,
    52, 52, 52, 52, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53,
    53, 53, 53, 53, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
    54, 54, 54, 54, 54, 54, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55}
};

static cmos_isp_demosaic_t st_isp_demosaic =
{
    /*vh_slope*/
    0xda,

    /*aa_slope*/
    0xa9,

    /*va_slope*/
    0xec,

    /*uu_slope*/
    0x84,

    /*sat_slope*/
    0x5d,

    /*ac_slope*/
    0xcf,

    /*vh_thresh*/
    0xa9,

    /*aa_thresh*/
    0x23,

    /*va_thresh*/
    0xa6,

    /*uu_thresh*/
    0x2d,

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
    cmos_inttime.full_lines_std = 832;
    cmos_inttime.full_lines_std_30fps = 832;
    cmos_inttime.full_lines_std_25fps = 998;
    cmos_inttime.full_lines = 832;
    cmos_inttime.full_lines_limit = 65535;
    cmos_inttime.max_lines = 832;
    cmos_inttime.min_lines = 2;
    cmos_inttime.max_lines_target = cmos_inttime.max_lines;
    cmos_inttime.min_lines_target = cmos_inttime.min_lines;

    cmos_inttime.vblanking_lines = 0;

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
    static HI_U32 _last_exposure_time = 0xFFFFFFFF;

    if(p_inttime->exposure_ashort != _last_exposure_time)
    {
        #if CMOS_HIMAX1375_ISP_WRITE_SENSOR_ENABLE
        ISP_I2C_DATA_S stI2cData;
    
        stI2cData.bDelayCfg = HI_TRUE;
        stI2cData.u8DevAddr = sensor_i2c_addr;
        stI2cData.u32AddrByteNum = sensor_addr_byte;
        stI2cData.u32DataByteNum = sensor_data_byte;

        stI2cData.u32RegAddr = 0x0016;
        stI2cData.u32Data = p_inttime->exposure_ashort&0xFF;
        HI_MPI_ISP_I2cWrite(&stI2cData);

        stI2cData.u32RegAddr = 0x0015;
        stI2cData.u32Data = (p_inttime->exposure_ashort>>8)&0xFF;
        HI_MPI_ISP_I2cWrite(&stI2cData);

        stI2cData.u32RegAddr = 0x0100;
        stI2cData.u32Data = 0x01;
        HI_MPI_ISP_I2cWrite(&stI2cData);
        
        #else
        sensor_write_register(0x0016, p_inttime->exposure_ashort&0xFF);
        sensor_write_register(0x0015, (p_inttime->exposure_ashort>>8)&0xFF);
        
        //refresh  exposure register
        sensor_write_register(0x0100,0x01);
        #endif
        _last_exposure_time = p_inttime->exposure_ashort;
    }
    

    return;
}

/*
 * This function applies the new vert blanking porch to the ISP registers.
 */
static __inline void cmos_vblanking_update(cmos_inttime_const_ptr_t p_inttime)
{
    static HI_U16 last_vblanking_lines = 0xFFFF;
    if(p_inttime->vblanking_lines != last_vblanking_lines)
    {
        sensor_write_register(0x0011, p_inttime->vblanking_lines & 0xff);
        sensor_write_register(0x0010, (p_inttime->vblanking_lines & 0xff00) >> 8);
        
         //refresh   register
        sensor_write_register(0x0100,0x01);
        last_vblanking_lines = p_inttime->vblanking_lines;
    }

    return;
}

static __inline HI_U16 vblanking_calculate(
		cmos_inttime_ptr_t p_inttime)
{
    if(p_inttime->exposure_ashort >= p_inttime->full_lines - 4)
    {
    	p_inttime->exposure_ashort = p_inttime->full_lines - 4;
    }

    p_inttime->vblanking_lines = p_inttime->full_lines - p_inttime->full_lines_std_30fps+2;

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
            p_inttime->full_lines_std= p_inttime->full_lines_std_30fps;
		break;

		case 25:
            /* do not change full_lines_std */
            p_inttime->lines_per_500ms = cmos_inttime.full_lines_std_25fps * 25 / 2;
            p_inttime->full_lines_std= p_inttime->full_lines_std_25fps;
            
        break;

		default:
		break;
	}
    
	return;
}

/*
 * This function initialises an instance of cmos_gains_t.
 */
static __inline cmos_gains_ptr_t cmos_gains_initialize()
{
    cmos_gains.max_again = 16;
    cmos_gains.max_dgain = 169;


    cmos_gains.max_again_target = cmos_gains.max_again; 
    cmos_gains.max_dgain_target = 169;//when the dgain > 169,the image quality is bad
    cmos_gains.again_shift = 0;
    cmos_gains.dgain_shift = 6;
    cmos_gains.dgain_fine_shift = 0;

    cmos_gains.isp_dgain_shift = 8;
    cmos_gains.isp_dgain = 1<< cmos_gains.isp_dgain_shift;
    cmos_gains.max_isp_dgain_target = 4 << cmos_gains.isp_dgain_shift;
    cmos_gains.isp_dgain_delay_cfg = HI_TRUE;

    return &cmos_gains;
}

static __inline HI_U32 cmos_get_ISO(cmos_gains_ptr_t p_gains)
{ 
    HI_U32 _again = p_gains->again == 0 ? 1 : p_gains->again;
    HI_U32 _dgain = p_gains->dgain == 0 ? 1 : p_gains->dgain;
    HI_U32 _isp_dgain = p_gains->isp_dgain== 0 ? 1 : p_gains->isp_dgain;

    p_gains->iso =  (((HI_U64)_again * _dgain *_isp_dgain* 100) >>(p_gains->again_shift + p_gains->dgain_shift+p_gains->isp_dgain_shift)); 
    
    return p_gains->iso;
}

/*
 * This function applies the new gains to the ISP registers.
 */
static __inline void cmos_gains_update(cmos_gains_const_ptr_t p_gains)
{
    static HI_U32 _last_again = 0xFFFFFFFF;
    static HI_U32 _last_dgain = 0xFFFFFFFF;
    HI_BOOL _update = HI_FALSE;

    #if CMOS_HIMAX1375_ISP_WRITE_SENSOR_ENABLE
    ISP_I2C_DATA_S stI2cData;

    stI2cData.bDelayCfg = HI_TRUE;
    stI2cData.u8DevAddr = sensor_i2c_addr;
    stI2cData.u32AddrByteNum = sensor_addr_byte;
    stI2cData.u32DataByteNum = sensor_data_byte;

    if(p_gains->again!= _last_again)
    { 
        stI2cData.u32RegAddr = 0x0018;
        switch(p_gains->again)
        {
            case (16):
                stI2cData.u32Data = 0x4;
            break;
            case (8):
                stI2cData.u32Data = 0x3;    
            break;
            case (4):
                stI2cData.u32Data = 0x2;
            break;
            case (2):
                stI2cData.u32Data = 0x1;
            break;
            default:
                stI2cData.u32Data = 0x0;
        }
        HI_MPI_ISP_I2cWrite(&stI2cData);
        _last_again = p_gains->again;
        _update = HI_TRUE;
    }
    
    if(p_gains->dgain!= _last_dgain)
    { 
        stI2cData.u32RegAddr = 0x001D;
        stI2cData.u32Data = p_gains->dgain;
        HI_MPI_ISP_I2cWrite(&stI2cData);
        _last_dgain = p_gains->dgain;
        _update = HI_TRUE;
    }    
   
    //refresh   register
    if(_update)
    {
        stI2cData.u32RegAddr = 0x0100;
        stI2cData.u32Data = 0x01;
        HI_MPI_ISP_I2cWrite(&stI2cData);
    }
    
    #else
    if(p_gains->again!= _last_again)
    { 
        switch(p_gains->again)
        {
            case (16):
                sensor_write_register(0x0018, 0x4);
            break;
                 
            case (8):
                sensor_write_register(0x0018, 0x3);    
            break;
            
            case (4):
                sensor_write_register(0x0018, 0x2);
            break;
            
            case (2):
                sensor_write_register(0x0018, 0x1);
            break;
            default:
                sensor_write_register(0x0018, 0x0);
        }
        _last_again = p_gains->again;
        _update = HI_TRUE;
    }
    
    if(p_gains->dgain!= _last_dgain)
    { 
        sensor_write_register(0x001D, p_gains->dgain);
        _last_dgain = p_gains->dgain;
        _update = HI_TRUE;
    }    
   
    //refresh   register
    if(_update)
    {
        sensor_write_register(0x0100,0x01);
    }
    #endif 
    return;
}

static __inline HI_U32 analog_gain_from_exposure_calculate(
		cmos_gains_ptr_t p_gains,
		HI_U32 exposure,
		HI_U32 exposure_max,
		HI_U32 exposure_shift)
{
    HI_U32 _again = 1 << p_gains->again_shift;
    int shift = 0;

    //prevent overlow of exposure
    while (exposure > (1<<22))
    {
        exposure >>= 1;
        exposure_max >>= 1;
        ++shift;
    }
    
    _again = (exposure<<p_gains->again_shift) /exposure_max;
    _again = (_again < (1<<p_gains->again_shift)) ? (1<<p_gains->again_shift) : _again;        
    _again = (_again > p_gains->max_again_target) ? p_gains->max_again_target : _again;
    
    if (_again >= 16) { _again = 16; }
    else if (_again >= 8) { _again = 8; }
    else if (_again >= 4) { _again = 4; }
    else if (_again >= 2) { _again = 2; }
    else{_again = 1;}
    
    p_gains->again = _again;
    exposure = (exposure<<p_gains->again_shift) / p_gains->again;
    
    return exposure<<shift;
    
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
    HI_U32 exposure0;
    int shift = 0;	
    HI_U32 _dgain=1<<p_gains->dgain_shift; 
	//prevent overlow of exposure
    while (exposure > (1<<22))
    {
    	exposure >>= 1;
    	exposure_max >>= 1;
    	++shift;
    }
    exposure_max = (0 == exposure_max)? 1: exposure_max;
    exposure0=exposure;
    
    //calculate digital gain
    if(exposure > exposure_max)
    {
       _dgain = (exposure << p_gains->dgain_shift)/exposure_max;
    }
    
    _dgain = (_dgain>(p_gains->max_dgain_target))?(p_gains->max_dgain_target):_dgain;
    _dgain = (_dgain < (1<<p_gains->dgain_shift)) ? (1<<p_gains->dgain_shift) : _dgain;
    
    p_gains->dgain= _dgain;
    exposure=(exposure << p_gains->dgain_shift) / p_gains->dgain;
    
    return exposure<<shift;
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

static void setup_sensor(int isp_mode)
{
    //return;
    if(0 == isp_mode) /* ISP 'normal' isp_mode */
    {
        sensor_write_register(0x0010, 0x0);
        sensor_write_register(0x0011, 0x0);
    }
    else if(1 == isp_mode) /* ISP pixel calibration isp_mode */
    {
        /* 5 fps */
        sensor_write_register(0x0010, 0x13); 
        sensor_write_register(0x0011, 0x80); 
        
        /* min gain */
        sensor_write_register(0x0018, 0x0);               
        sensor_write_register(0x001d, 0x40); 
        /* max exposure time*/
        sensor_write_register(0x0016, 0x7e);
        sensor_write_register(0x0015, 0x13);
    }
     //refresh  exposure register
    sensor_write_register(0x0100,0x01);
    sensor_write_register(0x0101,0x01);
    sensor_write_register(0x0000,0x01);
}



static HI_U8 cmos_get_analog_gain(cmos_gains_ptr_t p_gains)
{
    return 0;
}

static HI_U8 cmos_get_digital_gain(cmos_gains_ptr_t p_gains)
{
    return 0;
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


#endif // __HIMAX1375_CMOS_H_
