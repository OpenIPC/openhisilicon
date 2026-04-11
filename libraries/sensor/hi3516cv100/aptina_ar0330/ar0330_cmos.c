#if !defined(__AR0330_CMOS_H_)
#define __AR0330_CMOS_H_

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
#define CMOS_AR0330_ISP_WRITE_SENSOR_ENABLE (1)

static cmos_isp_default_t st_coms_isp_default =
{
    {
        4900,
        {
            0x0206, 0x80a0, 0x8066,
            0x8063, 0x0190, 0x802d,
            0x800f, 0x80bd, 0x01cc
        },
        
        3100,
        {
            0x01e2, 0x8097, 0x804b,
            0x806d, 0x0185, 0x8018,
            0x8010, 0x80f3, 0x0203
	  },

        2470,
        {
            0x01a5, 0x8066, 0x803f,
            0x805f, 0x0153, 0xb,
            0x8061, 0x80e8, 0x249
        }
    },

	// black level for R, Gr, Gb, B channels
	{0xA8,0xA8,0xA8,0xA8},
    	//{187,198,178,163},

	//calibration reference color temperature
	4900,

	//WB gain at Macbeth 5000K, must keep consistent with calibration color temperature
    	{0x198,0x100,0x100,0x1b2},
	// WB curve parameters, must keep consistent with reference color temperature.
    //{104,-38,-190,224624,128,-176434},
        {55,24,-177,271396,128,-220863},
	// hist_thresh
	{0xd,0x28,0x60,0x80},

	0x00,	// iridix_balck
	0x1,	// rggb

	// gain
	0x8,	0x10, // this is gain target, it will be constricted by sensor-gain.
	//0x8,	0x4, /* The purpose of setting max dgain target to 4 is to reduce FPN */

	//wdr_variance_space, wdr_variance_intensity, slope_max_write,  white_level_write
	0x04,	0x01,	0x30, 	0x4FF,

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
    //sharpen_alt_d
    {0x50,0x48,0x40,0x38,0x34,0x30,0x28,0x28},

    //sharpen_alt_ud
    {0x90,0x88,0x80,0x70,0x60,0x50,0x40,0x40},

    //snr_thresh
    {0x13,0x19,0x20,0x26,0x2c,0x32,0x38,0x38},

    //demosaic_lum_thresh
    {0x60,0x60,0x80,0x80,0x80,0x80,0x80,0x80},

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

static cmos_isp_demosaic_t st_isp_demosaic =
{
    /*vh_slope*/
    0xe9,

    /*aa_slope*/
    0xd3,

    /*va_slope*/
    0xcc,

    /*uu_slope*/
    0xc2,

    /*sat_slope*/
    0x5d,

    /*ac_slope*/
    0xcf,

    /*vh_thresh*/
    0xd8,

    /*aa_thresh*/
    0xd5,

    /*va_thresh*/
    0xb6,

    /*uu_thresh*/
    0xc4,

    /*sat_thresh*/
    0x171,

    /*ac_thresh*/
    0x1b3
};

static HI_U32 analog_gain_table[29] =
{ 
    1024,1054,1096,1126,1167,1218,1259,1311,1362,1423,1484,1556,1638,1720,1822,1925,
    2048,2181,2345,2519,2734,2979,3276,3645,4096,4679,5458,6553,8192
};


/*
 * This function initialises an instance of cmos_inttime_t.
 */
static /*__inline*/ cmos_inttime_const_ptr_t cmos_inttime_initialize()
{
    
    cmos_inttime.flicker_freq = 0;//60*256;//50*256;
    cmos_inttime.exposure_shift = 0;
    cmos_inttime.full_lines_std = 1092;

    cmos_inttime.full_lines_std_30fps = 1092;
    cmos_inttime.full_lines_std_25fps = 1310;

    cmos_inttime.lines_per_500ms = cmos_inttime.full_lines_std_30fps*30/2;
	
    
    return &cmos_inttime;
}

/*
 * This function applies the new integration time to the ISP registers.
 */
static __inline void cmos_inttime_update(cmos_inttime_ptr_t p_inttime)
{
   static HI_U32 _last_exposure_time = 0xFFFFFFFF;

   if(_last_exposure_time == (p_inttime->exposure_ashort>>p_inttime->exposure_shift))
   {
        return ;
   }else
   {
        _last_exposure_time = (p_inttime->exposure_ashort>>p_inttime->exposure_shift);
   }
   
 #if CMOS_AR0330_ISP_WRITE_SENSOR_ENABLE
    ISP_I2C_DATA_S stI2cData;

    stI2cData.bDelayCfg = HI_FALSE;
    stI2cData.u8DevAddr = sensor_i2c_addr;
    stI2cData.u32AddrByteNum = sensor_addr_byte;
    stI2cData.u32DataByteNum = sensor_data_byte;
    stI2cData.u32RegAddr= 0x3012;
    stI2cData.u32Data = p_inttime->exposure_ashort>>p_inttime->exposure_shift;

    HI_MPI_ISP_I2cWrite(&stI2cData);
#else
    HI_U16 _time = p_inttime->exposure_ashort>>p_inttime->exposure_shift;
    sensor_write_register(0x3012, _time);
#endif
}

/*
 * This function applies the new vert blanking porch to the ISP registers.
 */
static __inline void cmos_vblanking_update(cmos_inttime_const_ptr_t p_inttime)
{
    int  _fulllines= p_inttime->full_lines;

    static HI_U16 last_vblanking_lines = 0xFFFF;
    
    if(last_vblanking_lines != p_inttime->full_lines)
    {
         sensor_write_register(0x300A, _fulllines);
         last_vblanking_lines = p_inttime->full_lines;
    } 
   
}

static __inline HI_U16 vblanking_calculate(
		cmos_inttime_ptr_t p_inttime)
{
    if(p_inttime->exposure_ashort >= p_inttime->full_lines - 2)
    {
        p_inttime->exposure_ashort = p_inttime->full_lines - 2;
    }
    
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
            p_inttime->full_lines_std= p_inttime->full_lines_std_30fps;
            sensor_write_register(0x300A, 0x444);
            break;
        case 25:
            p_inttime->full_lines_std= p_inttime->full_lines_std_25fps;
            sensor_write_register(0x300A, 0x51E);
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
    cmos_gains.max_again = 8192;
    cmos_gains.max_dgain = 2047;

    cmos_gains.max_again_target = cmos_gains.max_again;
    cmos_gains.max_dgain_target = cmos_gains.max_dgain;
        
    cmos_gains.again_shift = 10;
    cmos_gains.dgain_shift = 7;    //
    cmos_gains.dgain_fine_shift = 0;

    cmos_gains.again = (1<<cmos_gains.again_shift);
    cmos_gains.dgain = (1<<cmos_gains.dgain_shift);

    cmos_gains.isp_dgain_shift = 8;         // max shift supported:8,make it to be 4 currently
    cmos_gains.isp_dgain = 1 << cmos_gains.isp_dgain_shift;    // max isp_dgain supported:4
    
    cmos_gains.max_isp_dgain_target = 4 << cmos_gains.isp_dgain_shift;

    cmos_gains.isp_dgain_delay_cfg = HI_FALSE;
    return &cmos_gains;
}

/*
 * This function applies the new gains to the ISP registers.
 */
static __inline void cmos_gains_update(cmos_gains_const_ptr_t p_gains)
{
    HI_U32 step = p_gains->again_db;
    static HI_U32 last_again = 0xFFFFFFFF;
    static HI_U32 last_dgain = 0xFFFFFFFF;
    
    if((last_again ==  p_gains->again) && (last_dgain == p_gains->dgain))
    {
        return ;
    }
    else
    {
        last_again = p_gains->again;
        last_dgain = p_gains->dgain;
    }
    
#if CMOS_AR0330_ISP_WRITE_SENSOR_ENABLE
    ISP_I2C_DATA_S stI2cData;

    stI2cData.bDelayCfg = HI_FALSE;
    stI2cData.u8DevAddr = sensor_i2c_addr;
    stI2cData.u32AddrByteNum = sensor_addr_byte;
    stI2cData.u32DataByteNum = sensor_data_byte;
    stI2cData.u32RegAddr = 0x3060;
    
    if(step < 16)
    {
        stI2cData.u32Data = 0x00 + step;
    }
    else if(step < 24)
    {
        stI2cData.u32Data = 0x10 + (step - 16) * 2;
    }
    else if(step < 28)
    {
        stI2cData.u32Data = 0x20 + (step - 24) * 4;
    }
    else
    {
        stI2cData.u32Data = 0x30;
    }

    HI_MPI_ISP_I2cWrite(&stI2cData);
    
    stI2cData.u32RegAddr = 0x305E;
    stI2cData.u32Data = p_gains->dgain;

    HI_MPI_ISP_I2cWrite(&stI2cData);
    
    
#else          
    if(step < 16)
    {
        sensor_write_register(0x305E, step);
    }
    else if(step <24)
    {
        sensor_write_register(0x305E, 0x10 + (step - 16) * 2);
    }
    else if(step <28)
    {
        sensor_write_register(0x305E, 0x20 + (step - 24) * 4);
    }
    else
    {
        sensor_write_register(0x305E, 0x30);
    }

    sensor_write_register(0x305E, p_gains->dgain);
#endif
}

static __inline HI_U32 analog_gain_from_exposure_calculate(
        cmos_gains_ptr_t p_gains,
        HI_U32 exposure,
        HI_U32 exposure_max,
        HI_U32 exposure_shift)
{
    HI_U32 again = 1024;
    HI_U32 step = 0;
    HI_S32 shift = 0;
    HI_S32 i = 0;

    while (exposure > (1 << 20))
    {
        exposure     >>= 1;
        exposure_max >>= 1;
        ++shift;
    }
    exposure_max = (exposure_max == 0)? 1: exposure_max;
    
    if(exposure > exposure_max)
    {
        again = (exposure << 10) / exposure_max;
        again = again < 1024? 1024: again;
        again = again > p_gains->max_again_target? p_gains->max_again_target: again;
    
        if (again >= analog_gain_table[28])
        {
             again = analog_gain_table[28];
             step = 28;
        }
        else
        {
            for(i = 1; i < 29; i++)
            {
                if(again < analog_gain_table[i])
                {
                    again = analog_gain_table[i - 1];
                    step = i - 1;
                    break;
                }
            }
        }
        
        exposure = (exposure << 10) / again;
    }

    p_gains->again = again;
    p_gains->again_db = step;
    
    return (exposure << shift);
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
    HI_U32 exposure_shift)
{
    HI_U32 _dgain = 1<<p_gains->dgain_shift;    

    int shft = 0;

    while (exposure > (1<<20))
    {
        exposure >>= 1;
        exposure_max >>= 1;
        ++shft;
    }

    if(exposure > exposure_max)
    {
        exposure_max = DIV_0_TO_1(exposure_max);
        _dgain = (exposure  * _dgain) / exposure_max;
    }
    else
    {

    }
    _dgain = _dgain < (1<<p_gains->dgain_shift) ? (1<<p_gains->dgain_shift) : _dgain;
    _dgain = _dgain >  p_gains->max_dgain_target ? p_gains->max_dgain_target : _dgain;

    p_gains->dgain = _dgain;
    exposure = (exposure << p_gains->dgain_shift) / p_gains->dgain;
    
    return exposure << shft;
}

static __inline HI_U32 cmos_get_ISO(cmos_gains_ptr_t p_gains)
{
    HI_U32  _again = p_gains->again == 0 ? 1 : p_gains->again;
    HI_U32 _dgain = p_gains->dgain == 0 ? 1 : p_gains->dgain;
    HI_U32 _isp_dgain = p_gains->isp_dgain = 0 ? 1 : p_gains->isp_dgain;

    p_gains->iso =  (((HI_U64)_again * _dgain * _isp_dgain * 100) >> (p_gains->again_shift + p_gains->dgain_shift + p_gains->isp_dgain_shift));

    return p_gains->iso;
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
        sensor_write_register(0x300A, 0444);	//30fps
    }
    else if(1 == isp_mode) /* ISP pixel calibration isp_mode */
    {
        sensor_write_register(0x300A, 0x1998);	//5fps
        sensor_write_register(0x3012, 0x1996);	//max exposure lines
        sensor_write_register(0x3060, 0x1300);	//AG, Context A
        sensor_write_register(0x305E, 0x0128);	//DG, Context A
    }
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
        printf(">>sensor register callback function failed!\n");
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

#endif // __AR0330_CMOS_H_

