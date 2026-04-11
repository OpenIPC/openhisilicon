#if !defined(__M034_CMOS_H_)
#define __M034_CMOS_H_

#include <stdio.h>
#include <string.h>
#include "hi_comm_sns.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"
#include "m034_sensor_config.h"
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
#define CMOS_9M034_ISP_WRITE_SENSOR_ENABLE (1)

static cmos_isp_default_t st_coms_isp_default_lin =
{
	// color correction matrix
    {
        5000,
        {
            0x0227,0x80CE,0x8059,
            0x8026,0x0151,0x802B,
            0x001E,0x80C0,0x01A2
        },

        3200,
        {
            0x1da,0x809e,0x803c,
            0x805f,0x175,0x8016,
            0x802c,0x8124,0x250
        },

        2500,
        {
            0x02AD,0x814A,0x8063,
            0x000C,0x00EB,0x0009,
            0x0031,0x81BE,0x028D
        }
    },

	// black level for R, Gr, Gb, B channels
	{0xA8,0xA8,0xA8,0xA8},

	//calibration reference color temperature
	5000,

	//WB gain at Macbeth 5000K, must keep consistent with calibration color temperature
    {0x018B,0x0100,0x0100,0x01A2},//for ref

	// WB curve parameters, must keep consistent with reference color temperature.
    {88,-17,-185,239197,128,-193985},  //for ref

	// hist_thresh
	{0xd,0x28,0x60,0x80},

	0x00,	// iridix_balck
	0x1,	// grbg

	// gain
	0x8,	0x8, // this is gain target, it will be constricted by sensor-gain.

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

static cmos_isp_default_t st_coms_isp_default_wdr =
{
	// color correction matrix
    {
        5000,
        {
            0x0227,0x80CE,0x8059,
            0x8026,0x0151,0x802B,
            0x001E,0x80C0,0x01A2
        },

        3200,
        {
            0x1da,0x809e,0x803c,
            0x805f,0x175,0x8016,
            0x802c,0x8124,0x250
        },

        2500,
        {
            0x02AD,0x814A,0x8063,
            0x000C,0x00EB,0x0009,
            0x0031,0x81BE,0x028D
        }
    },

	// black level(wdr)
	{0x00,0x00,0x00,0x00},

	//calibration reference color temperature
	5000,

	//WB gain at Macbeth 5000K, must keep consistent with calibration color temperature
    {0x018B,0x0100,0x0100,0x01A2},//for ref

	// WB curve parameters, must keep consistent with reference color temperature.
    {88,-17,-185,239197,128,-193985},  //for ref

	// hist_thresh
    {0x20,0x40,0x60,0x80},

	0x00,	// iridix_balck
	0x1,	// grbg

	// gain
	0x1,	0x8,

	//wdr_variance_space, wdr_variance_intensity, slope_max_write,  white_level_write
	0x04,	0x04,	0x3c, 	0xFFF,

	0x0, 	// balance_fe
	0x80,	// ae compensation
	0x9, 	// sinter threshold

	0x0,     //noise profile=0, use the default noise profile lut, don't need to set nr0 and nr1
	0x0,
	0x0,
     2
};

static cmos_isp_agc_table_t st_isp_agc_table =
{
    //sharpen_alt_d
    {0x50,0x48,0x40,0x38,0x34,0x30,0x28,0x20},

    //sharpen_alt_ud
    {0x90,0x88,0x80,0x70,0x58,0x40,0x20,0x0a},

    //snr_thresh
    {0x23,0x2c,0x34,0x3E,0x46,0x4E,0x54,0x5A},
    //{20,25,30,35,39,44,49,54},

    //demosaic_lum_thresh
    {0x60,0x60,0x80,0x80,0x80,0x80,0x80,0x80},

    //demosaic_np_offset
    {0x0,0xa,0x12,0x1a,0x20,0x28,0x30,0x37},

    //ge_strength
    {0x55,0x55,0x55,0x55,0x55,0x55,0x37,0x37},

    /* saturation */
    {0x80,0x80,0x6C,0x48,0x44,0x40,0x3C,0x38}
};

static cmos_isp_agc_table_t st_isp_agc_table_wdr =
{
     /* sharpen_alt_d */
    {0x50,0x50,0x50,0x50,0x4a,0x45,0x40,0x3a},
        
    /* sharpen_alt_ud */
    {0x90,0x90,0x90,0x90,0x88,0x80,0x78,0x70},
        
    /* snr_thresh */
    {0x05,0x05,0x05,0x05,0x08,0x0c,0x10,0x18},

    //demosaic_lum_thresh
    {0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80},

    //demosaic_np_offset
    {0x0,0xa,0x12,0x1a,0x20,0x28,0x30,0x37},
        
    //ge_strength
    {0x55,0x55,0x55,0x55,0x55,0x55,0x37,0x37},

    /* saturation */
    {0x80,0x80,0x80,0x80,0x70,0x60,0x50,0x40}
};

static cmos_isp_noise_table_t st_isp_noise_table =
{
    /* nosie_profile_weight_lut */
    {
       0,  0,  0,  0,  0,  2,  9, 14, 16, 19, 21, 23, 24, 25, 27, 28, 
      29, 30, 31, 31, 32, 33, 33, 34, 35, 35, 36, 36, 37, 37, 38, 38, 
      39, 39, 39, 40, 40, 40, 41, 41, 42, 42, 42, 42, 43, 43, 43, 44, 
      44, 44, 44, 45, 45, 45, 45, 46, 46, 46, 46, 46, 47, 47, 47, 47, 
      47, 47, 48, 48, 48, 48, 48, 49, 49, 49, 49, 49, 49, 50, 50, 50, 
      50, 50, 50, 51, 51, 51, 51, 51, 51, 51, 51, 52, 52, 52, 52, 52, 
      52, 52, 53, 53, 53, 53, 53, 53, 53, 53, 53, 54, 54, 54, 54, 54, 
      54, 54, 54, 54, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 56, 56
    },

    /* demosaic_weight_lut */
    {
      2,  9, 14, 16, 19, 21, 23, 24, 25, 27, 28, 29, 30, 31, 31, 32, 
     33, 33, 34, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 39, 40, 40, 
     40, 41, 41, 42, 42, 42, 42, 43, 43, 43, 44, 44, 44, 44, 45, 45, 
     45, 45, 46, 46, 46, 46, 46, 47, 47, 47, 47, 47, 47, 48, 48, 48, 
     48, 48, 49, 49, 49, 49, 49, 49, 50, 50, 50, 50, 50, 50, 51, 51, 
     51, 51, 51, 51, 51, 51, 52, 52, 52, 52, 52, 52, 52, 53, 53, 53, 
     53, 53, 53, 53, 53, 53, 54, 54, 54, 54, 54, 54, 54, 54, 54, 55, 
     55, 55, 55, 55, 55, 55, 55, 55, 55, 56, 56, 56, 56, 56, 56, 56
    }
    };

static cmos_isp_noise_table_t st_isp_noise_table_wdr =
{
    /* nosie_profile_weight_lut WDR */
    {
        13,13,13,13,13,14,15,25,31,31,31,31,31,31,31,31,31,31,31,31,31,32,32,32,32,32,32,32,39,49,54,56,58,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60
    },

    /* demosaic_weight_lut WDR */
    {
        13,13,13,13,13,14,15,25,31,31,31,31,31,31,31,31,31,31,31,31,31,32,32,32,32,32,32,32,39,49,54,56,58,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60
    }
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

static cmos_isp_demosaic_t st_isp_demosaic_wdr =
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

/*
 * This function initialises an instance of cmos_inttime_t.
 */
static /*__inline*/ cmos_inttime_const_ptr_t cmos_inttime_initialize()
{
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

	cmos_inttime.lines_per_500ms = 750 * 30 / 2; // 500ms / 22.22us
	cmos_inttime.flicker_freq = 0;//60*256;//50*256;

	cmos_inttime.max_lines_target = cmos_inttime.max_lines;
	cmos_inttime.min_lines_target = cmos_inttime.min_lines;
	//cmos_inttime.max_flicker_lines = cmos_inttime.max_lines_target;
	//cmos_inttime.min_flicker_lines = cmos_inttime.min_lines_target;
	//cmos_inttime.input_changed = 0;

    switch(gu8SensorMode)
    {
        default:
        case 0: //linear mode
            cmos_inttime.max_lines = 748;
            cmos_inttime.min_lines = 2;
            cmos_inttime.max_lines_target = 748;
            cmos_inttime.min_lines_target = 2;
        break;
        case 1: //WDR mode for T1 exposure, Exposure ratio = 16X16X
            cmos_inttime.max_lines = 675;
            cmos_inttime.min_lines = 128;
            cmos_inttime.max_lines_target = 675;
            cmos_inttime.min_lines_target = 128;
        break;
    }

	return &cmos_inttime;
}

/*
 * This function applies the new integration time to the ISP registers.
 */
static __inline void cmos_inttime_update(cmos_inttime_ptr_t p_inttime)
{
#if CMOS_9M034_ISP_WRITE_SENSOR_ENABLE
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
	
			sensor_write_register(0x300C, 0xCE4);
            p_inttime->lines_per_500ms = 750 * 30 / 2;
            
		break;

		case 25:
	
			sensor_write_register(0x300C, 0xF78);
            p_inttime->lines_per_500ms = 750 * 25 / 2;
            
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
	cmos_gains.max_again = 8;

	cmos_gains.max_dgain = 255;
	cmos_gains.max_again_target = cmos_gains.max_again;
	cmos_gains.max_dgain_target = cmos_gains.max_dgain;

	cmos_gains.again_shift = 0;
	cmos_gains.dgain_shift = 5;
	cmos_gains.dgain_fine_shift = 0;

	cmos_gains.again = 1;
	cmos_gains.dgain = 1;
	cmos_gains.dgain_fine = 1;
	cmos_gains.again_db = 0;
	cmos_gains.dgain_db = 0;

    cmos_gains.isp_dgain_shift = 8;
    cmos_gains.isp_dgain = 1 << cmos_gains.isp_dgain_shift;
    cmos_gains.max_isp_dgain_target = 1 << cmos_gains.isp_dgain_shift;
    cmos_gains.isp_dgain_delay_cfg = HI_FALSE;

    switch(gu8SensorMode)
    {
        default:
        case 0: //linear mode
            cmos_gains.max_again = 8 << cmos_gains.again_shift;
            cmos_gains.max_dgain = 255;
            cmos_gains.max_again_target = cmos_gains.max_again;
            cmos_gains.max_dgain_target = cmos_gains.max_dgain;
            cmos_gains.max_isp_dgain_target = 4 << cmos_gains.isp_dgain_shift;
        break;
        case 1: //WDR mode
            cmos_gains.max_again = 8 << cmos_gains.again_shift;
            cmos_gains.max_dgain = 255;
            cmos_gains.max_again_target = cmos_gains.max_again;
            cmos_gains.max_dgain_target = cmos_gains.max_dgain;
            cmos_gains.max_isp_dgain_target = 32 << cmos_gains.isp_dgain_shift;
        break;
    }

	return &cmos_gains;
}

/*
 * This function applies the new gains to the ISP registers.
 */
static __inline void cmos_gains_update(cmos_gains_const_ptr_t p_gains)
{
	int ag = p_gains->again;
	int dg = p_gains->dgain;
#if CMOS_9M034_ISP_WRITE_SENSOR_ENABLE
    ISP_I2C_DATA_S stI2cData;

    stI2cData.bDelayCfg = HI_FALSE;
    stI2cData.u8DevAddr = sensor_i2c_addr;
    stI2cData.u32AddrByteNum = sensor_addr_byte;
    stI2cData.u32DataByteNum = sensor_data_byte;
    stI2cData.u32RegAddr = 0x30B0;
    switch(ag)
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
	switch(ag)
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
	HI_U32 _again = 1<<p_gains->again_shift;
	//HI_U32 _ares = 1<<p_gains->again_shift;
	//HI_U32 _lres = 0;
	int shft = 0;

	while (exposure > (1<<20))
	{
		exposure >>= 1;
		exposure_max >>= 1;
		++shft;
	}

	if(exposure > exposure_max)
	{
        //when setting manual exposure line, exposure_max>>shift should not be 0.
        exposure_max = DIV_0_TO_1(exposure_max);
		_again = (exposure  * _again)  / exposure_max;
//		exposure = exposure_max;

		if (_again >= 1<< 3) { _again = 1<<3; }
		else if (_again >= 1<< 2) { _again = 1<<2; }
		else if (_again >= 1<< 1) { _again = 1<<1; }
		else if (_again >= 1)     { _again = 1;    }

		_again = _again < (1<<p_gains->again_shift) ? (1<<p_gains->again_shift) : _again;
		_again = _again > p_gains->max_again_target ? p_gains->max_again_target : _again;

		exposure = (exposure / _again);
	}
	else
	{
		//_again = (_again * exposure) / (exposure / exposure_shift) / exposure_shift;
	}

	p_gains->again = _again;
    p_gains->again_db = cmos_gains_lin_to_db_convert(p_gains->again, p_gains->again_shift);
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

	p_gains->iso =  ((_again * _dgain * 100) >> (p_gains->again_shift + p_gains->dgain_shift));
    p_gains->iso = (p_gains->iso * p_gains->isp_dgain) >> p_gains->isp_dgain_shift;


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

    switch(gu8SensorMode)
    {
        default:
        case 0:
            memcpy(p_coms_isp_default, &st_coms_isp_default_lin, sizeof(cmos_isp_default_t));
        break;
        case 1:
            memcpy(p_coms_isp_default, &st_coms_isp_default_wdr, sizeof(cmos_isp_default_t));
        break;
    }
    return 0;
}

void setup_sensor(int isp_mode)
{
	if(0 == isp_mode) /* ISP 'normal' isp_mode */
	{
        sensor_write_register(0x300C, 0xCE4);	//30fps
	}
	else if(1 == isp_mode) /* ISP pixel calibration isp_mode */
	{
		sensor_write_register(0x300C, 0x4D58);	//5fps
		sensor_write_register(0x3012, 0x118);	//max exposure lines
		sensor_write_register(0x30B0, 0x1300);	//AG, Context A
		sensor_write_register(0x305E, 0x0020);	//DG, Context A
	}
}

static HI_U32 cmos_get_isp_agc_table(cmos_isp_agc_table_ptr_t p_cmos_isp_agc_table)
{
	if (NULL == p_cmos_isp_agc_table)
	{
	    printf("null pointer when get isp agc table value!\n");
	    return -1;
	}

    switch(gu8SensorMode)
    {
        default:
        case 0:
            memcpy(p_cmos_isp_agc_table, &st_isp_agc_table, sizeof(cmos_isp_agc_table_t));
        break;
        case 1:
            memcpy(p_cmos_isp_agc_table, &st_isp_agc_table_wdr, sizeof(cmos_isp_agc_table_t));
        break;
    }

    return 0;
}

static HI_U32 cmos_get_isp_noise_table(cmos_isp_noise_table_ptr_t p_cmos_isp_noise_table)
{
	if (NULL == p_cmos_isp_noise_table)
	{
	    printf("null pointer when get isp noise table value!\n");
	    return -1;
	}

    switch(gu8SensorMode)
    {
        default:
        case 0:
            memcpy(p_cmos_isp_noise_table, &st_isp_noise_table, sizeof(cmos_isp_noise_table_t));
        break;
        case 1:
            memcpy(p_cmos_isp_noise_table, &st_isp_noise_table_wdr, sizeof(cmos_isp_noise_table_t));
        break;
    }
    return 0;
}

static HI_U32 cmos_get_isp_demosaic(cmos_isp_demosaic_ptr_t p_cmos_isp_demosaic)
{
   if (NULL == p_cmos_isp_demosaic)
   {
	    printf("null pointer when get isp demosaic value!\n");
	    return -1;
   }
    switch(gu8SensorMode)
    {
        default:
        case 0:
            memcpy(p_cmos_isp_demosaic, &st_isp_demosaic,sizeof(cmos_isp_demosaic_t));
        break;
        case 1:
            memcpy(p_cmos_isp_demosaic, &st_isp_demosaic_wdr,sizeof(cmos_isp_demosaic_t));
        break;
    }

   memcpy(p_cmos_isp_demosaic, &st_isp_demosaic,sizeof(cmos_isp_demosaic_t));
   return 0;

}


HI_U32 cmos_get_sensor_mode(void)
{
    switch(gu8SensorMode)
    {
        default:
        case 0:
            return isp_special_alg_m034_lin;
        break;
        case 1:
            return isp_special_alg_m034_wdr;
        break;
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
    .pfn_cmos_get_isp_special_alg = cmos_get_sensor_mode,
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

int sensor_mode_set(HI_U8 u8Mode)
{
    switch(u8Mode)
    {
        //720P30 linear
        case 0:
            gu8SensorMode = 0;
            printf("linear mode\n");

            /* program sensor to linear mode */
            sensor_prog(sensor_rom_30_lin);

            /* Enable DCG */
            sensor_write_register(0x3100, 0x001E);

            /* Enable 1.25x analog gain */
            sensor_write_register(0x3EE4, 0xD308);
        break;

        //720P30 wdr
        case 1:
            gu8SensorMode = 1;
            printf("wdr mode\n");

            /* program sensor to wdr mode */
            sensor_prog(sensor_rom_30_wdr);

            /* Disable DCG */
            sensor_write_register(0x3100, 0x001A);

            /* Disable 1.25x analog gain */
            sensor_write_register(0x3EE4, 0xD208);
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

#endif // __M034_CMOS_H_
