/******************************************************************************
  A driver program of soi h22 on HI3518A 
 ******************************************************************************
    Modification:  2013-03  Created
******************************************************************************/

#if !defined(__SOIH22_CMOS_H_)
#define __SOIH22_CMOS_H_
#include <stdio.h>
#include <string.h>
#include "hi_comm_sns.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

extern const unsigned int sensor_i2c_addr;
extern const unsigned int sensor_addr_byte;
extern const unsigned int sensor_data_byte;

#define CMOS_SOIH22_ISP_WRITE_SENSOR_ENABLE (1)
/****************************************************************************
 * local variables															*
 ****************************************************************************/

static cmos_inttime_t cmos_inttime;
static cmos_gains_t cmos_gains;
HI_U8 gu8SensorMode = 0;

static cmos_isp_default_t st_coms_isp_default =
{
    {// color matrix[9] ccm     modified by daniel
        5048,
        {   0x1f5, 0x80be, 0x8037,
            0x8045, 0x195, 0x8050,
            0x800e, 0x8146, 0x255
        },
        3193,
        {   0x1ee, 0x8017, 0x80d6,
            0x8053, 0x1d2, 0x807e,
            0x8023, 0x8176, 0x29a
        },
        2480,
        {   0x21d, 0x8091, 0x808c,
            0x8072, 0x19d, 0x802b,
            0x80b3, 0x8366, 0x519
        }
    }
    ,

	// black level
    {1,1,1,1},  //calibration by tools added by wenyu

    //calibration reference color temperature modified by daniel
    5048,

    //WB gain at 5000, must keep consistent with calibration color temperature
    {0x152, 0x100, 0x100, 0X1ab},

    // WB curve parameters, must keep consistent with reference color temperature.
    {139, -51, -167, 184226, 128, -136368},

    // hist_thresh
    {0xd,0x28,0x60,0x80},
    //{0x10,0x40,0xc0,0xf0},

    0x0,	// iridix_balck
    0x3,	// bggr

    // gain
    0X1f,	0x1,

    // iridix
    0x04,	0x08,	0xa0, 	0x4ff,

    0x1, 	// balance_fe
    0x80,	// ae compensation
    0x15, 	// sinter threshold

    0x0,  0,  0,  //noise profile=0, use the default noise profile lut, don't need to set nr0 and nr1
#if CMOS_SOIH22_ISP_WRITE_SENSOR_ENABLE 
    2
#else
    1
#endif
};

static cmos_isp_agc_table_t st_isp_agc_table =
{
    //100,200 ,400 ,800 ,1600,3200 ,6400
    //sharpen_alt_d
    {0xbc,0x6d,0x55,0x60,0x16,0x8e,0xd8,0x01},

    //sharpen_alt_ud
    {0x8e,0x46,0x3e,0x40,0x2f,0x3d,0x1e,0x20},

    //snr_thresh modified by daniel
    //{0x1d,0x1e,0x1f,0x4e,0x5a,0x59,0x64,0x59},
    {0x1d,0x1e,0x1f,0x30,0x30,0x30,0x30,0x30},

    //demosaic_lum_thresh
    {0x40,0x60,0x80,0x80,0x80,0x80,0x80,0x80},

    //demosaic_np_offset
    {0x0,0xa,0x12,0x1a,0x20,0x28,0x30,0x30},

    //ge_strength
    {0x55,0x55,0x55,0x55,0x55,0x55,0x37,0x37},

    /* saturation */
    {0x80,0x80,0x80,0x80,0x68,0x48,0x35,0x30}

};

static cmos_isp_demosaic_t st_isp_demosaic =
{
    /*vh_slope*/
    0xff,

    /*aa_slope*/
    0xe4,

    /*va_slope*/
    0xec,

    /*uu_slope*/
    0x9f,

    /*sat_slope*/
    0x5d,

    /*ac_slope*/
    0xcf,

    /*vh_thresh*/
    0x138,

    /*aa_thresh*/
    0xba,

    /*va_thresh*/
    0xda,

    /*uu_thresh*/
    0x148,

    /*sat_thresh*/
    0x171,

    /*ac_thresh*/
    0x1b3
};

static cmos_isp_noise_table_t st_isp_noise_table =
{
//nosie_profile_weight_lut
    {
     0x19,0x20,0x24,0x27,0x29,0x2b,0x2d,0x2e,0x2f,0x31,0x32,0x33,0x34,0x34,0x35,0x36,0x37,\
	0x37,0x38,0x38,0x39,0x3a,0x3a,0x3b,0x3b,0x3b,0x3c,0x3c,0x3d,0x3d,0x3d,0x3e,0x3e,0x3e,\
	0x3f,0x3f,0x3f,0x40,0x40,0x40,0x41,0x41,0x41,0x41,0x42,0x42,0x42,0x42,0x43,0x43,0x43,\
	0x43,0x44,0x44,0x44,0x44,0x44,0x45,0x45,0x45,0x45,0x45,0x46,0x46,0x46,0x46,0x46,0x46,\
	0x47,0x47,0x47,0x47,0x47,0x47,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x49,0x49,0x49,0x49,\
	0x49,0x49,0x49,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4b,0x4b,0x4b,0x4b,0x4b,\
	0x4b,0x4b,0x4b,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4d,0x4d,0x4d,0x4d,\
	0x4d,0x4d,0x4d,0x4d,0x4d,0x4d,0x4e,0x4e,0x4e
    },
    
//demosaic_weight_lut
    {
        0x19,0x20,0x24,0x27,0x29,0x2b,0x2d,0x2e,0x2f,0x31,0x32,0x33,0x34,0x34,0x35,0x36,0x37,\
	0x37,0x38,0x38,0x39,0x3a,0x3a,0x3b,0x3b,0x3b,0x3c,0x3c,0x3d,0x3d,0x3d,0x3e,0x3e,0x3e,\
	0x3f,0x3f,0x3f,0x40,0x40,0x40,0x41,0x41,0x41,0x41,0x42,0x42,0x42,0x42,0x43,0x43,0x43,\
	0x43,0x44,0x44,0x44,0x44,0x44,0x45,0x45,0x45,0x45,0x45,0x46,0x46,0x46,0x46,0x46,0x46,\
	0x47,0x47,0x47,0x47,0x47,0x47,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x49,0x49,0x49,0x49,\
	0x49,0x49,0x49,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4b,0x4b,0x4b,0x4b,0x4b,\
	0x4b,0x4b,0x4b,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4d,0x4d,0x4d,0x4d,\
	0x4d,0x4d,0x4d,0x4d,0x4d,0x4d,0x4e,0x4e,0x4e
    }
    
};

/*
 * This function initialises an instance of cmos_inttime_t.
 */

static __inline cmos_inttime_const_ptr_t cmos_inttime_initialize()
{
/* 1280 * 736 */
/* 1296 * 816 */ /* reg: 0x22/23/24/25 */
#define STD_LINES 767
    cmos_inttime.full_lines_std = STD_LINES;
    cmos_inttime.full_lines_std_30fps = STD_LINES;
    cmos_inttime.full_lines = STD_LINES;
    cmos_inttime.full_lines_limit = 65535;
    cmos_inttime.max_lines_target = STD_LINES;
    cmos_inttime.min_lines_target = 8;
    cmos_inttime.max_lines = STD_LINES;
    cmos_inttime.vblanking_lines = 0;
    cmos_inttime.exposure_ashort = 0;
    cmos_inttime.exposure_shift = 0;
    cmos_inttime.lines_per_500ms = cmos_inttime.full_lines_std_30fps * 30 / 2; // 500ms / 39.17us
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
#if CMOS_SOIH22_ISP_WRITE_SENSOR_ENABLE
    ISP_I2C_DATA_S stI2cData;
    stI2cData.bDelayCfg = HI_FALSE;
    stI2cData.u8DevAddr = sensor_i2c_addr;
    stI2cData.u32AddrByteNum = sensor_addr_byte;
    stI2cData.u32DataByteNum = sensor_data_byte;

    HI_U32 _curr = p_inttime->exposure_ashort ;
    stI2cData.u32RegAddr = 0x01;
    stI2cData.u32Data =  _curr&0xFF;
    HI_MPI_ISP_I2cWrite(&stI2cData);

    stI2cData.u32RegAddr = 0x02;
    stI2cData.u32Data = (_curr>>8)&0xFF;
    HI_MPI_ISP_I2cWrite(&stI2cData);
    
#else

    HI_U32 _curr = p_inttime->exposure_ashort ;
    //refresh the sensor setting every frame to avoid defect pixel error
    sensor_write_register(0x01, _curr&0xFF);
    sensor_write_register(0x02, (_curr>>8)&0xFF);    
#endif	
        _last_exposure_time = p_inttime->exposure_ashort ;
    }
}

/*
 * This function applies the new vert blanking porch to the ISP registers.
 */
static __inline void cmos_vblanking_update(cmos_inttime_const_ptr_t p_inttime)
{

    static HI_U16 last_full_lines = 0xFFFF;
     if(p_inttime->full_lines != last_full_lines)
     {
        sensor_write_register(0x22, (p_inttime->full_lines)&0xff);
        sensor_write_register(0x23, (p_inttime->full_lines>>8));
        last_full_lines = p_inttime->full_lines;
     }
    return;
}

static __inline HI_U16 vblanking_calculate(
		cmos_inttime_ptr_t p_inttime)
{
    if(p_inttime->exposure_ashort >= p_inttime->full_lines -2)
    {
    	p_inttime->exposure_ashort = p_inttime->full_lines -2;
    }
                                 //expecting picture height      valid height of expecting picture  
    p_inttime->vblanking_lines = p_inttime->full_lines - p_inttime->full_lines_std;
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
    	default://default30fps
    	case 30:
    	    p_inttime->full_lines=p_inttime->full_lines_std_30fps;
    		p_inttime->lines_per_500ms = cmos_inttime.full_lines_std_30fps * 30 / 2;
            sensor_write_register(0x22, (p_inttime->full_lines)&0xff);
            sensor_write_register(0x23, (p_inttime->full_lines>>8));
            p_inttime->full_lines_std= p_inttime->full_lines;
    	    p_inttime->lines_per_500ms=p_inttime->full_lines*30/2;//refresh the deflicker parameter
            break;
    	case 25:
            p_inttime->full_lines=(30*p_inttime->full_lines_std_30fps)/25;
    		p_inttime->lines_per_500ms = cmos_inttime.full_lines_std_30fps * 30 / 2;
            sensor_write_register(0x22, (p_inttime->full_lines)&0xff);
            sensor_write_register(0x23, (p_inttime->full_lines>>8));
            p_inttime->full_lines_std= p_inttime->full_lines;
    	    p_inttime->lines_per_500ms=p_inttime->full_lines*25/2;//refresh the deflicker parameter
    	    break;
    }

}

/*
 * This function initialises an instance of cmos_gains_t.
 */
static __inline cmos_gains_ptr_t cmos_gains_initialize()
{
    cmos_gains.max_again = 496;//31<<4
    cmos_gains.max_dgain = 4;
    cmos_gains.isp_dgain_delay_cfg = HI_TRUE;
    
    cmos_gains.again_shift = 4;
    cmos_gains.dgain_shift = 0;
    cmos_gains.dgain_fine_shift = 0;
    cmos_gains.isp_dgain_shift = 8;
    cmos_gains.isp_dgain = 1 << cmos_gains.isp_dgain_shift;
    cmos_gains.max_isp_dgain_target = 4 << cmos_gains.isp_dgain_shift;

    return &cmos_gains;
}

static __inline HI_U32 cmos_get_ISO(cmos_gains_ptr_t p_gains)
{
    HI_U32 _again = p_gains->again == 0 ? 1 : p_gains->again;
    HI_U32 _dgain = p_gains->dgain == 0 ? 1 : p_gains->dgain;
    HI_U32 _isp_gain=p_gains->isp_dgain== 0 ? 1 : p_gains->isp_dgain;
    p_gains->iso =  (((HI_U64)_again * _dgain *_isp_gain * 100) >> (p_gains->again_shift + p_gains->dgain_shift
        + p_gains->isp_dgain_shift));
    return p_gains->iso;
}

/*
 * This function applies the new gains to the ISP registers.
 */
static __inline void cmos_gains_update(cmos_gains_const_ptr_t p_gains)
{  
    static HI_U32 _last_again = 0xFFFFFFFF;
    static HI_U32 _last_dgain = 0xFFFFFFFF;
    //HI_BOOL _update = HI_FALSE;
    if((_last_again != p_gains->again_db) ||( _last_dgain != p_gains->dgain))
    {
#if CMOS_SOIH22_ISP_WRITE_SENSOR_ENABLE
    ISP_I2C_DATA_S stI2cData;
    stI2cData.bDelayCfg = HI_TRUE;
    stI2cData.u8DevAddr = sensor_i2c_addr;
    stI2cData.u32AddrByteNum = sensor_addr_byte;
    stI2cData.u32DataByteNum = sensor_data_byte;
    stI2cData.u32RegAddr = 0x00;
    stI2cData.u32Data = p_gains->again_db;
    
    HI_MPI_ISP_I2cWrite(&stI2cData);
    //set dgain
    stI2cData.u32RegAddr = 0x0d;
    switch(p_gains->dgain)
    {
        case (4):
            stI2cData.u32Data = 0x03;
        break;
        case (2):
            stI2cData.u32Data = 0x01;
        break;
        default:
            stI2cData.u32Data = 0x0;
    }
    HI_MPI_ISP_I2cWrite(&stI2cData);
#else
	//set again
    sensor_write_register(0x00, p_gains->again_db);//again_db is again_string
	//set dgain
    switch(p_gains->dgain)
    {
        case (4):
            sensor_write_register(0x0d, 0x03);
        break;
        case (2):
            sensor_write_register(0x0d, 0x01);
        break;
        default:
            sensor_write_register(0x0d, 0x00);
    }
#endif
    _last_again = p_gains->again_db;
    _last_dgain = p_gains->dgain;
}
    return;
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
    //printf("exposure/max:%d\n",exposure/exposure_max);
    int shift = 0;
    HI_U8 i = 0;
    HI_U32 _again = 1 << p_gains->again_shift;
    //prevent overlow of exposure
    while (exposure > (1<<20))
    {
        exposure >>= 1;
        exposure_max >>= 1;
        ++shift;
    }
    if(exposure > exposure_max)
    {
        _again = (exposure << p_gains->again_shift) / exposure_max;
        _again = _again < 16? 16: _again;
        _again = _again > p_gains->max_again_target ? p_gains->max_again_target : _again;

        for(i = 0; i < 5; i++)
        {
            if(_again < 32)
            {
                break;
            }
            _again >>= 1;
        }
    }
    else
    {
    }

    p_gains->again_db = (1 << (i + 4)) + _again - 32;
    p_gains->again = _again << i;

    return (exposure << (shift + 4)) / p_gains->again;

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

    HI_U32 _dgain = (1<<p_gains->dgain_shift);
    int shift = 0;
    int exposure0;
    exposure0=exposure;
    
    HI_U32 isp_dgain=(1<< p_gains->isp_dgain_shift);
    //prevent overlow of exposure
    while (exposure > (1<<20))
    {
    	exposure >>= 1;
    	exposure_max >>= 1;
    	++shift;
    }
    if(exposure > exposure_max)
    {
        _dgain = (exposure<<p_gains->dgain_shift) / exposure_max;
    }   
    _dgain = (_dgain > p_gains->max_dgain_target) ? p_gains->max_dgain_target:_dgain;

    if(_dgain >= 4)
    {
        _dgain = 4;
    }
    else if(_dgain >= 2)
    {
        _dgain = 2;
    }
    else
    {
        _dgain = 1;
    }
    
    p_gains->dgain=_dgain;
    p_gains->dgain_db = cmos_gains_lin_to_db_convert(p_gains->dgain,p_gains->dgain_shift);
    exposure0 = (exposure<<p_gains->dgain_shift) /p_gains->dgain;

    return exposure0<<shift;
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
    if(0 == isp_mode) /* ISP 'normal' isp_mode */
    {
        int lines0=(767);//normal full lines is 767 lines ,30 fps
        sensor_write_register(0x22, (lines0)&0xff);
        sensor_write_register(0x23, (lines0>>8));
    }
    else if(1 == isp_mode) /* ISP pixel calibration isp_mode */
    {
        sensor_write_register(0x00, 0x00);//again=1;
        sensor_write_register(0x0d,0x01);//dgain=1;
        
        int lines1=((30*767)/5);
        sensor_write_register(0x22, (lines1)&0xff);
        sensor_write_register(0x23, (lines1>>8));
    }
}

static HI_U8 cmos_get_analog_gain(cmos_gains_ptr_t p_gains)
{
    return cmos_gains_lin_to_db_convert(p_gains->again-(1<<p_gains->again_shift), p_gains->again_shift); //??
}

static HI_U8 cmos_get_digital_gain(cmos_gains_ptr_t p_gains)
{
    return p_gains->dgain;
}

static HI_U8 cmos_get_digital_fine_gain(cmos_gains_ptr_t p_gains)
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
    .pfn_cmos_get_digital_fine_gain = cmos_get_digital_fine_gain,
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


#endif // __SOIH22_CMOS_H_
