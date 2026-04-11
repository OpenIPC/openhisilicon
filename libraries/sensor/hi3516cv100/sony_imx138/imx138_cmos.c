/******************************************************************************
  A driver program of panasonic IMX138 on HI3518A 
 ******************************************************************************
    Modification:  2013-05  Created
******************************************************************************/


#if !defined(__IMX138_CMOS_H_)
#define __IMX138_CMOS_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hi_comm_sns.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


#define CMOS_IMX138_ISP_WRITE_SENSOR_ENABLE (1)

/* Note: format of address is special.
 * chip_id + reg_adddr */
#define EXPOSURE_ADDR (0x220) //2:chip_id, 20: reg addr.
#define LONG_EXPOSURE_ADDR (0x223)
#define PGC_ADDR (0x214)
#define VMAX_ADDR (0x218)

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
    	{
    		0x01f2, 0x80bd, 0x8035,
    		0x8042, 0x01a3, 0x8061,
    		0x0026, 0x80e0, 0x1b9,
    	},
        3200,
    	{
    		0x01dd, 0x807b, 0x8062,
    		0x807b, 0x01d2, 0x8057,
    		0x0036, 0x8110, 0x01d9,
    	},
    	2480,
    	{
    		0x01b1, 0x8066, 0x804b,
    		0x8074, 0x01b2, 0x803e,
    		0x001e, 0x81d9, 0x02ba,
    	}
    },

	// black level for R, Gr, Gb, B channels
	{0xF0,0xF0,0xF0,0xF0},

	//calibration reference color temperature
	5048,

	//WB gain at 5000K, must keep consistent with calibration color temperature
	{0x01ff, 0x100, 0x100, 0x01e4},

	// WB curve parameters, must keep consistent with reference color temperature.
	{40, 106, -110, 184787, 128, -134867},

	// hist_thresh
	{0xd,0x28,0x60,0x80},

	0x00,	// iridix_balck
	0x2,	// 0:rggb; 2: gbrg

	// max again, max dgain
	0x10,	0x10,

	// iridix
	0x04,	0x08,	0xa0, 	0x8ff,

	0x1, 	// balance_fe
	0x80,	// ae compensation
	0x8, 	// sinter threshold

	0x1,  //0: use default profile table; 1: use calibrated profile lut, the setting for nr0 and nr1 must be correct.
	0,    //nr0
	455,  //nr1

#if  CMOS_IMX138_ISP_WRITE_SENSOR_ENABLE
    2
#else
    1
#endif
};


static cmos_isp_agc_table_t st_isp_agc_table =
{
    /* sharpen_alt_d */
   {48,48,40,35,30,30,20,15}, 
    
    /* sharpen_alt_ud */
   {40,40,35,30,25,25,15,10},    
    
    /* snr_thresh    */
   {10,16,24,34,39,44,49,54},  
    
    /*  demosaic_lum_thresh   */
   {0x50,0x50,0x40,0x40,0x30,0x30,0x20,0x20}, 
    
    /* demosaic_np_offset   */
   {0,10,16,24,32,40,48,56},  
    
    /* ge_strength    */
   {0x82,0x55,0x55,0x55,0x55,0x55,0x37,0x37}, 
    
    /* saturation */   
   {0x80,0x80,0x80,0x75,0x60,0x55,0x50,0x48}
};

static cmos_isp_noise_table_t st_isp_noise_table =
{
  //nosie_profile_weight_lut
    {
      0,  0,  0,  0,  0,  0,  0,  0,  5, 10, 15, 17, 20, 22, 25, 27, 
     27, 30, 30, 32, 32, 35, 35, 35, 37, 37, 37, 40, 40, 40, 40, 42,
     42, 42, 42, 42, 45, 45, 45, 45, 45, 45, 47, 47, 47, 47, 47, 47,
     50, 50, 50, 50, 50, 50, 50, 50, 52, 52, 52, 52, 52, 52, 52, 52,
     52, 52, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 57, 57, 57,
     57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 60, 60, 60, 60, 60,
     60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 62, 62, 62, 62, 62, 62,
     62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 65, 65, 65
    },

  //demosaic_weight_lut
    {
      0,  5, 10, 15, 17, 20, 22, 25, 27, 27, 30, 30, 32, 32, 35, 35,
      35, 37, 37, 37, 40, 40, 40, 40, 42, 42, 42, 42, 42, 45, 45, 45,
      45, 45, 45, 47, 47, 47, 47, 47, 47, 50, 50, 50, 50, 50, 50, 50,
      50, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 55, 55, 55, 55, 55,
      55, 55, 55, 55, 55, 55, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57,
      57, 57, 57, 57, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
      60, 60, 60, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62,
      62, 62, 62, 62, 62, 62, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65
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
   0x1b3,
   
};
/*
 * This function initialises an instance of cmos_inttime_t.
 */
static __inline cmos_inttime_const_ptr_t cmos_inttime_initialize()
{
	cmos_inttime.full_lines_std = 752;
	cmos_inttime.full_lines_std_30fps = 752;
	cmos_inttime.full_lines = 752;
	cmos_inttime.full_lines_limit = 65535;
	cmos_inttime.max_lines_target = 752;
	cmos_inttime.min_lines_target = 3;
	cmos_inttime.vblanking_lines = 0;

	cmos_inttime.exposure_ashort = 0;

	cmos_inttime.lines_per_500ms = 752*30/2; //
	cmos_inttime.flicker_freq = 0;//60*256;//50*256;

	return &cmos_inttime;
}

/*
 * This function applies the new integration time to the ISP registers.
 */
static __inline void cmos_inttime_update(cmos_inttime_ptr_t p_inttime)
{

    HI_U16 exp_time;
    exp_time = p_inttime->full_lines - p_inttime->exposure_ashort - 1;  
    
#if CMOS_IMX138_ISP_WRITE_SENSOR_ENABLE

    ISP_SSP_DATA_S   stSspData;
    stSspData.bDelayCfg = HI_FALSE;
    stSspData.u32DevAddr = 0x02;
    stSspData.u32DevAddrByteNum = 1;
    stSspData.u32RegAddr = 0x20;
    stSspData.u32RegAddrByteNum = 1;
    stSspData.u32Data = (exp_time & 0xFF);
    stSspData.u32DataByteNum = 1;
    HI_MPI_ISP_SSPWrite(&stSspData);

    stSspData.u32RegAddr = 0x21;
    stSspData.u32RegAddrByteNum = 1;
    stSspData.u32Data = ((exp_time & 0xFF00) >> 8);            
    stSspData.u32DataByteNum = 1;
    HI_MPI_ISP_SSPWrite(&stSspData);
    
#else

    sensor_write_register(EXPOSURE_ADDR, exp_time & 0xFF);
    sensor_write_register(EXPOSURE_ADDR+1, (exp_time & 0xFF00) >> 8);

#endif 

}

/*
 * This function applies the new vert blanking porch to the ISP registers.
 */
static __inline void cmos_vblanking_update(cmos_inttime_const_ptr_t p_inttime)
{

	HI_U16 vmax = p_inttime->full_lines;
	sensor_write_register(VMAX_ADDR, (vmax&0x00ff));
	sensor_write_register(VMAX_ADDR+1, ((vmax&0xff00) >> 8));
	return;
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

	switch(fps)
	{
		case 30:
			// Change the frame rate via changing the vertical blanking
			p_inttime->full_lines_std = 752;
			sensor_write_register(VMAX_ADDR, 0xF0);
			sensor_write_register(VMAX_ADDR+1, 0x02);
			p_inttime->lines_per_500ms = 752 * 30 / 2;
		break;

		case 25:
			// Change the frame rate via changing the vertical blanking
			p_inttime->full_lines_std = 902;
			sensor_write_register(VMAX_ADDR, 0x86);
			sensor_write_register(VMAX_ADDR+1, 0x03);
			p_inttime->lines_per_500ms = 902 * 25 / 2;
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
    cmos_gains.again = 1;
    cmos_gains.dgain = 1;
    cmos_gains.isp_dgain = 1;
    cmos_gains.again_db = 1;
    cmos_gains.dgain_db = 1;
    cmos_gains.again_shift = 10;
    cmos_gains.dgain_shift = 10;
    cmos_gains.dgain_fine_shift = 0;

    cmos_gains.isp_dgain_shift = 8;
    cmos_gains.isp_dgain = 1 << cmos_gains.isp_dgain_shift;

    
    cmos_gains.max_again = 16229;
    cmos_gains.max_dgain = 16229;
    cmos_gains.max_again_target = 16229;
    cmos_gains.max_dgain_target = 16229;
    cmos_gains.max_isp_dgain_target = 4 << cmos_gains.isp_dgain_shift;
    cmos_gains.isp_dgain_delay_cfg = HI_TRUE;


	return &cmos_gains;
}

static HI_U32 gain_table[81] =
{
    1024 , 1060 ,  1097 ,  1136 ,  1176,  1217 , 1260 ,  1304,  1350 ,  1397 ,  1446 ,  1497 , 1550 , 1604 ,  1661 ,  1719 , 
    1780 , 1842 ,  1907 ,  1974 ,  2043,  2115 , 2189 ,  2266,  2346 ,  2428 ,  2514 ,  2602 , 2693 , 2788 ,  2886 ,  2987 , 
    3092 , 3201 ,  3314 ,  3430 ,  3551,  3675 , 3805 ,  3938,  4077 ,  4220 ,  4368 ,  4522 , 4681 , 4845 ,  5015 ,  5192 , 
    5374 , 5563 ,  5758 ,  5961 ,  6170,  6387 , 6611 ,  6844,  7084 ,  7333 ,  7591 ,  7858 , 8134 , 8420 ,  8716 ,  9022 , 
    9339 , 9667 , 10007 , 10359 , 10723, 11099 ,11489 , 11893, 12311 , 12744 , 13192 , 13655 ,14135 ,14632 , 15146 , 15678 , 
    16229     
};

/*
 * This function applies the new gains to the ISP registers.
 */
static __inline void cmos_gains_update(cmos_gains_const_ptr_t p_gains)
{
    /* analog gain = APGC * 0.3 db, APGC = [0x0,0x50], ag_db=[0db, 24db] */
    /* digital gain = DPGC * 0.3 db, DPCG = [0x50,0xA0], dg_db=[0db, 24db] */

     ISP_SSP_DATA_S   stSspData;
    
#if CMOS_IMX138_ISP_WRITE_SENSOR_ENABLE

    stSspData.bDelayCfg = HI_TRUE;
    stSspData.u32DevAddr = 0x02;
    stSspData.u32DevAddrByteNum = 1;
    stSspData.u32RegAddr = 0x14;
    stSspData.u32RegAddrByteNum = 1;
    stSspData.u32Data = p_gains->again_db + p_gains->dgain_db;
    stSspData.u32DataByteNum = 1;

    if((p_gains->again_db + p_gains->dgain_db) <= 0xA0)
    {
      HI_MPI_ISP_SSPWrite(&stSspData);
    }
    else
    {
       stSspData.u32Data = 0xA0;
       stSspData.u32DataByteNum = 1;
       HI_MPI_ISP_SSPWrite(&stSspData);
    }

#else
    
    if((p_gains->again_db + p_gains->dgain_db) <= 0xA0)
    {
        sensor_write_register(PGC_ADDR, p_gains->again_db + p_gains->dgain_db);
    }
    else
    {
        sensor_write_register(PGC_ADDR, 0xA0);
    }
    
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
    
        if (again >= gain_table[80])
        {
             again = gain_table[80];
             step = 80;
        }
        else
        {
            for(i = 1; i < 81; i++)
            {
                if(again < gain_table[i])
                {
                    again = gain_table[i-1];
                    step = i-1;
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
    exposure_max = (exposure_max == 0)? 1: exposure_max;
    
    if(exposure > exposure_max)
    {
        dgain = (exposure << 10) / exposure_max;
        dgain = dgain < 1024? 1024: dgain;
        dgain = dgain > p_gains->max_dgain_target? p_gains->max_dgain_target: dgain;
    
        if (dgain >= gain_table[80])
        {
             dgain = gain_table[80];
             step = 80;
        }
        else
        {
            for(i = 1; i < 81; i++)
            {
                if(dgain < gain_table[i])
                {
                    dgain = gain_table[i-1];
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
    printf("defect bad pixiel  \n");
	if(0 == isp_mode) /* setup for ISP 'normal mode' */
	{
        sensor_write_register(VMAX_ADDR, 0xEE);
        sensor_write_register(VMAX_ADDR + 1, 0x02);
	}
	else if(1 == isp_mode) /* setup for ISP pixel calibration mode */
	{
        /* Sensor must be programmed for slow frame rate (5 fps and below)*/
        /* change frame rate to 5 fps by setting 1 frame length = 750 * 30 / 5 */
        sensor_write_register(VMAX_ADDR, 0x94);
        sensor_write_register(VMAX_ADDR + 1, 0x11);

        /* max Exposure time */
		sensor_write_register(EXPOSURE_ADDR, 0x00);
		sensor_write_register(EXPOSURE_ADDR + 1, 0x00);

        /* Analog and Digital gains both must be programmed for their minimum values */
		sensor_write_register(PGC_ADDR, 0x00);

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

/* Note: The unit of return value is 1db.  */
static HI_U8 cmos_get_analog_gain(cmos_gains_ptr_t cmos_gains)
{
    return (cmos_gains->again_db *  3 / 10);
}

/* Note: The unit of return value is 1db.  */
static HI_U8 cmos_get_digital_gain(cmos_gains_ptr_t cmos_gains)
{
    return  (cmos_gains->dgain_db *  3 / 10);
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

#endif // __IMX138_CMOS_H_
