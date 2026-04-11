#if !defined(__PO3100K_CMOS_H_)
#define __PO3100K_CMOS_H_

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
#define CMOS_PO3100K_ISP_WRITE_SENSOR_ENABLE (1)

static cmos_isp_default_t st_coms_isp_default =
{
    {
        4850,
        {
           0x01ee, 0x80a3, 0x804b,
           0x8040, 0x0195, 0x8055,
           0x0006, 0x80f0, 0x01e9
        },
        
        3100,
        {
           0x01f4, 0x804f, 0x80a5,
           0x805b, 0x01bf, 0x8064,
           0x8014, 0x8147, 0x025b
        },
        
        2470,
        {
           0x0201, 0x80ca, 0x8037,
           0x806d, 0x017a, 0x800d,
           0x803c, 0x8285, 0x03c1
        }
    },

	// black level for R, Gr, Gb, B channels
    {4,4,4,4},

	//calibration reference color temperature
    4850,

	//WB gain at Macbeth 5000K, must keep consistent with calibration color temperature
    {0x177,0x0100,0x0100,0x0197},

	// WB curve parameters, must keep consistent with reference color temperature.
    {123,-28,-161,192773,128,-142978},

	// hist_thresh
    {0xd,0x28,0x60,0x80},

    0x00,	// iridix_balck
    0x0,	// rggb

    // gain
    64,	4, // this is gain target, it will be constricted by sensor-gain.
	//0x8,	0x4, /* The purpose of setting max dgain target to 4 is to reduce FPN */

	//wdr_variance_space, wdr_variance_intensity, slope_max_write,  white_level_write
    0x04,	0x01,	0x30, 	0x4FF,

    0x1, 	// balance_fe
    0x80,	// ae compensation
    0x23, 	// sinter threshold

    0x1,     //noise profile=0, use the default noise profile lut, don't need to set nr0 and nr1
    0x0,
    546,
#if CMOS_PO3100K_ISP_WRITE_SENSOR_ENABLE
    2
#else
    1
#endif
};

static cmos_isp_agc_table_t st_isp_agc_table =
{
    //sharpen_alt_d
    {0x50,0x48,0x40,0x38,0x34,0x30,0x28,0x28},

    //sharpen_alt_ud
    {0xb0,0xa0,0xa0,0x90,0x80,0x70,0x60,0x50},

    //snr_thresh
    {0x13,0x19,0x20,0x26,0x2c,0x32,0x38,0x38},

    //demosaic_lum_thresh
    {0x60,0x60,0x80,0x80,0x80,0x80,0x80,0x80},

    //demosaic_np_offset
    {0x0,0xa,0x12,0x1a,0x20,0x28,0x30,0x30},

    //ge_strength
    {0x55,0x55,0x55,0x55,0x55,0x55,0x37,0x37},

    /* saturation */
    {0x80,0x80,0x80,0x48,0x44,0x40,0x3C,0x38}
};

static cmos_isp_noise_table_t st_isp_noise_table =
{
    //nosie_profile_weight_lut
    {0x11,0x17,0x1b,0x1e,0x20,0x22,0x24,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2d,0x2e,
	0x2f,0x2f,0x30,0x30,0x31,0x31,0x32,0x32,0x33,0x33,0x34,0x34,0x34,0x35,0x35,0x36,0x36,
	0x36,0x37,0x37,0x37,0x37,0x38,0x38,0x38,0x39,0x39,0x39,0x39,0x3a,0x3a,0x3a,0x3a,0x3b,
	0x3b,0x3b,0x3b,0x3b,0x3c,0x3c,0x3c,0x3c,0x3c,0x3d,0x3d,0x3d,0x3d,0x3d,0x3d,0x3e,0x3e,
	0x3e,0x3e,0x3e,0x3e,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
	0x40,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,
	0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,
	0x44,0x44,0x45,0x45,0x45,0x45,0x45,0x45,0x45},

    //demosaic_weight_lut
    {0x11,0x17,0x1b,0x1e,0x20,0x22,0x24,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2d,0x2e,
	0x2f,0x2f,0x30,0x30,0x31,0x31,0x32,0x32,0x33,0x33,0x34,0x34,0x34,0x35,0x35,0x36,0x36,
	0x36,0x37,0x37,0x37,0x37,0x38,0x38,0x38,0x39,0x39,0x39,0x39,0x3a,0x3a,0x3a,0x3a,0x3b,
	0x3b,0x3b,0x3b,0x3b,0x3c,0x3c,0x3c,0x3c,0x3c,0x3d,0x3d,0x3d,0x3d,0x3d,0x3d,0x3e,0x3e,
	0x3e,0x3e,0x3e,0x3e,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
	0x40,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,
	0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x43,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,
	0x44,0x44,0x45,0x45,0x45,0x45,0x45,0x45,0x45}
};

static cmos_isp_demosaic_t st_isp_demosaic =
{
    /*vh_slope*/
    0xf0,

    /*aa_slope*/
    0xe6,

    /*va_slope*/
    0xe7,

    /*uu_slope*/
    0xc0,

    /*sat_slope*/
    0x5d,

    /*ac_slope*/
    0xcf,

    /*vh_thresh*/
    0x0,

    /*aa_thresh*/
    0x0,

    /*va_thresh*/
    0x0,

    /*uu_thresh*/
    0x0,

    /*sat_thresh*/
    0x171,

    /*ac_thresh*/
    0x1b3
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
    cmos_inttime.full_lines_std_25fps = 900;
    //cmos_inttime.full_lines = 750;
    //cmos_inttime.full_lines_del = 750; //TODO: remove
   // cmos_inttime.full_lines_limit = 65535;
    //cmos_inttime.max_lines = 748;
    cmos_inttime.min_lines = 2;
    //cmos_inttime.vblanking_lines = 0;

    //cmos_inttime.exposure_ashort = 0;
    cmos_inttime.exposure_shift = 0;

    cmos_inttime.lines_per_500ms = cmos_inttime.full_lines_std_30fps*30/2; // 500ms / 22.22us
    cmos_inttime.flicker_freq = 0;//60*256;//50*256;

   // cmos_inttime.max_lines_target = cmos_inttime.max_lines;
   // cmos_inttime.min_lines_target = cmos_inttime.min_lines;
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
    static HI_U32 _last_exposure_time = 0xFFFFFFFF;
    if(_last_exposure_time == p_inttime->exposure_ashort)
    {
        return;
    }else
    {
        _last_exposure_time = p_inttime->exposure_ashort;
    }

    ISP_I2C_DATA_S stI2cData;

    
    stI2cData.bDelayCfg = HI_FALSE;
    stI2cData.u8DevAddr = sensor_i2c_addr;
    stI2cData.u32AddrByteNum = sensor_addr_byte;
    stI2cData.u32DataByteNum = sensor_data_byte;

    stI2cData.u32RegAddr = 0xc0;
    stI2cData.u32Data = p_inttime->exposure_ashort >> (p_inttime->exposure_shift + 8);
    HI_MPI_ISP_I2cWrite(&stI2cData); 

    stI2cData.u32RegAddr = 0xc1;
    stI2cData.u32Data = (p_inttime->exposure_ashort >> p_inttime->exposure_shift) & 0xff;
    HI_MPI_ISP_I2cWrite(&stI2cData); 
}

/*
 * This function applies the new vert blanking porch to the ISP registers.
 */
static __inline void cmos_vblanking_update(cmos_inttime_const_ptr_t p_inttime)
{
    HI_U16  _fulllines = p_inttime->full_lines;
    static HI_U16 last_vblanking_lines = 65535;
    if(p_inttime->vblanking_lines != last_vblanking_lines)
    {
        sensor_write_register(0x03, 0x0);
        sensor_write_register(0x08, _fulllines>>8);
        sensor_write_register(0x09, _fulllines&0xFF);
        sensor_write_register(0x0A, _fulllines>>8);
        sensor_write_register(0x0B, _fulllines&0xFF);
        sensor_write_register(0x03, 0x1);
        last_vblanking_lines = p_inttime->vblanking_lines;
    }
    return;
}

static __inline HI_U16 vblanking_calculate(
		cmos_inttime_ptr_t p_inttime)
{
    if(p_inttime->exposure_ashort >= p_inttime->full_lines - 2)
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

    switch(fps)
    {
        case 30:
            p_inttime->full_lines_std = cmos_inttime.full_lines_std_30fps;
            p_inttime->lines_per_500ms = cmos_inttime.full_lines_std_30fps * 30 / 2;
            sensor_write_register(0x03, 0x00);
            sensor_write_register(0x08, 0x02);
            sensor_write_register(0x09, 0xED);
            sensor_write_register(0x0A, 0x02);
            sensor_write_register(0x0B, 0xED);
            sensor_write_register(0x03, 0x01);
        break;

        case 25:
            p_inttime->full_lines_std = cmos_inttime.full_lines_std_25fps;
            p_inttime->lines_per_500ms = cmos_inttime.full_lines_std_25fps * 25 / 2;
            sensor_write_register(0x03, 0x00);
            sensor_write_register(0x08, 0x03);
            sensor_write_register(0x09, 0x83);
            sensor_write_register(0x0A, 0x03);
            sensor_write_register(0x0B, 0x83);
            sensor_write_register(0x03, 0x01);
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

    cmos_gains.max_again = 1024;
    cmos_gains.max_dgain = 255;
    cmos_gains.max_again_target = cmos_gains.max_again;
    cmos_gains.max_dgain_target = cmos_gains.max_dgain;

    cmos_gains.again_shift = 4;
    cmos_gains.dgain_shift = 6;
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
    static HI_U32 _last_again = 0xFFFFFFFF;
    static HI_U32 _last_dgain = 0xFFFFFFFF;
    if((_last_again == p_gains->again_db) && (_last_dgain == p_gains->dgain))
    {
        return;
    }else
    {
        _last_again = p_gains->again_db;
        _last_dgain = p_gains->dgain;
    }
    
    
    ISP_I2C_DATA_S stI2cData;

    stI2cData.bDelayCfg = HI_FALSE;
    stI2cData.u8DevAddr = sensor_i2c_addr;
    stI2cData.u32AddrByteNum = sensor_addr_byte;
    stI2cData.u32DataByteNum = sensor_data_byte;
    
    stI2cData.u32RegAddr = 0xc3;
    stI2cData.u32Data = p_gains->again_db;
    HI_MPI_ISP_I2cWrite(&stI2cData);

    stI2cData.u32RegAddr = 0xc4;
    stI2cData.u32Data = p_gains->dgain;
    HI_MPI_ISP_I2cWrite(&stI2cData);

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

    HI_U32 _again = 1 << p_gains->again_shift;
    int shift = 0;
    HI_U8 i = 0;

    while (exposure > (1<<26))
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

        for(i = 0; i < 6; i++)
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

    p_gains->again_db = (i << 4) + _again - 16;
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

    p_gains->iso =  (((HI_U64)_again * _dgain * _ispdgain * 100) 
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

void setup_sensor(int isp_mode)
{
    if(0 == isp_mode) /* ISP 'normal' isp_mode */
    {
        sensor_write_register(0x03, 0x00);//30fps
        sensor_write_register(0x08, 0x02);
        sensor_write_register(0x09, 0xED);
        sensor_write_register(0x0A, 0x02);
        sensor_write_register(0x0B, 0xED);
        sensor_write_register(0x03, 0x01);
    }
    else if(1 == isp_mode) /* ISP pixel calibration isp_mode */
    {
        sensor_write_register(0x03, 0x00);//5fps
        sensor_write_register(0x08, 0x11);
        sensor_write_register(0x09, 0x94);
        sensor_write_register(0x0A, 0x11);
        sensor_write_register(0x0B, 0x94);
        
        sensor_write_register(0x03, 0x01);

        sensor_write_register(0xC0, 0x11);
        sensor_write_register(0xC1, 0x92);
        
        sensor_write_register(0xC3, 0x0);
        sensor_write_register(0xC4, 0x40);
        
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



#endif // __PO3100K_CMOS_H_
