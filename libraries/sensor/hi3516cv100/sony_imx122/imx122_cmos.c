#if !defined(__SONY_IMX122_H_)
#define __SONY_IMX122_H_

#include <stdio.h>
#include <unistd.h>		// usleep
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


#define CMOS_IMX122_ISP_WRITE_SENSOR_ENABLE (1)


#define SENSOR_720P_30FPS_MODE   1
#define SENSOR_720P_60FPS_MODE   2
#define SENSOR_1080P_30FPS_MODE  3


#define EXPOSURE_ADDR (0x208) //2:chip_id, 0C: reg addr.

#define PGC_ADDR (0x21E)
#define VMAX_ADDR (0x205)
/****************************************************************************
 * local variables															*
 ****************************************************************************/

static cmos_inttime_t cmos_inttime;
static cmos_gains_t cmos_gains;
HI_U8 gu8SensorMode = 0;

HI_U8 gu8SensorImageMode = 3;


extern void sensor_init_720p_60fps();

extern void sensor_init_720p_30fps();



static cmos_isp_default_t st_coms_isp_default =
{
    // color correction matrix
    #if 0
    {
        5000,
    	{	0x1b7,  0x8079, 0x803d,
    		0x806d, 0x01f2, 0x8084,
    		0x800a, 0x80b9, 0x01c4
    	},
    	3200,
        {
            0x01e7, 0x80cd, 0x801a,
            0x808f, 0x01d3, 0x8044,
            0x001b, 0x813b, 0x021f
        },
        2600,
        {
            0x020a, 0x80ed, 0x801d,
            0x806e, 0x0196, 0x8028,
            0x0015, 0x820f, 0x02f9
        }
    },
    #else
    {
        5000,
    	{	0x024d, 0x8140, 0x800d,
    		0x8052, 0x01cf, 0x807d,
    		0x0010, 0x80e0, 0x01d0
    	},
    	3200,
        {
            0x0213, 0x80e8, 0x802b,
            0x8080, 0x01d3, 0x8053,
            0x0012, 0x80ea, 0x01d8
        },
        2600,
        {
            0x0226, 0x80ea, 0x803c,
            0x8067, 0x01d7, 0x8070,
            0x000a, 0x810a, 0x0200
        }
    },
    #endif

	// black level for R, Gr, Gb, B channels
	{0xf0,0xf0,0xf0,0xf0},

    // calibration reference color temperature
    5000,

    //WB gain at 5000K, must keep consistent with calibration color temperature
	{0x1c5, 0x100, 0x100, 0x1ec},

    // WB curve parameters, must keep consistent with reference color temperature.
	{22, 141, -84, 186260, 0x80, -134565},

	// hist_thresh
	{0xd,0x28,0x60,0x80},

	0x00,	// iridix_balck
	0x0,	// rggb

	// gain
	0x10,	0x8,

	// iridix space, intensity, slope_max, white level
	0x02,	0x08,	0x80, 	0x8ff,

	0x1, 	// balance_fe
	0x80,	// ae compensation
	0x20, 	// sinter threshold

	0x1,        //0: use default profile table; 1: use calibrated profile lut, the setting for nr0 and nr1 must be correct.
	0,
	1528,
	
#if CMOS_IMX122_ISP_WRITE_SENSOR_ENABLE 
    2
#else
    1
#endif

};
/*the default setting is more sharpen and the noise is bigger in low light mode*/
#if 0
static cmos_isp_agc_table_t st_isp_agc_table =
{
    //sharpen_alt_d
    {0x88,0x85,0x80,0x7b,0x78,0x72,0x70,0x60},

    //sharpen_alt_ud
    {0xc8,0xc0,0xb8,0xb0,0xa8,0xa0,0x98,0x70},

    //snr_thresh
    {0x06,0x8,0xb,0x16,0x22,0x28,0x2e,0x35},

    //demosaic_lum_thresh
    {0x60,0x60,0x80,0x80,0x80,0x80,0x80,0x80},

    //demosaic_np_offset
    {0x0,0xa,0x12,0x1a,0x20,0x28,0x30,0x30},

    //ge_strength
    {0x55,0x55,0x55,0x55,0x55,0x55,0x37,0x37},

    /* saturation */
    {0x80,0x80,0x6C,0x48,0x44,0x40,0x3C,0x38}
};
#endif

static cmos_isp_agc_table_t st_isp_agc_table =
{
    //sharpen_alt_d
    {0x50,0x4b,0x46,0x41,0x3c,0x32,0x28,0x1e},

    //sharpen_alt_ud
    {0x4b,0x46,0x41,0x3c,0x32,0x28,0x1e,0x14},

    //snr_thresh
    {0x06,0x8,0xb,0x16,0x22,0x28,0x32,0x54},

    //demosaic_lum_thresh
    {0x50,0x50,0x40,0x40,0x30,0x30,0x20,0x20},

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

    {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x0c,0x11,0x14,0x17,0x19,0x1b,0x1c,0x1e,0x1f,
    0x20,0x21,0x22,0x23,0x24,0x24,0x25,0x26,0x26,0x27,0x28,0x28,0x29,0x29,0x2a,0x2a,0x2a,
    0x2b,0x2b,0x2c,0x2c,0x2c,0x2d,0x2d,0x2d,0x2e,0x2e,0x2e,0x2f,0x2f,0x2f,0x30,0x30,0x30,
    0x30,0x31,0x31,0x31,0x31,0x32,0x32,0x32,0x32,0x32,0x33,0x33,0x33,0x33,0x34,0x34,0x34,
    0x34,0x34,0x34,0x35,0x35,0x35,0x35,0x35,0x35,0x36,0x36,0x36,0x36,0x36,0x36,0x37,0x37,
    0x37,0x37,0x37,0x37,0x37,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x39,0x39,0x39,0x39,0x39,
    0x39,0x39,0x39,0x39,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3b,0x3b,0x3b,0x3b,
    0x3b,0x3b,0x3b,0x3b,0x3b,0x3c,0x3c,0x3c,0x3c
    },
  
    {
    0x04,0x0c,0x11,0x14,0x17,0x19,0x1b,0x1c,0x1e,0x1f,
    0x20,0x21,0x22,0x23,0x24,0x24,0x25,0x26,0x26,0x27,0x28,0x28,0x29,0x29,0x2a,0x2a,0x2a,
    0x2b,0x2b,0x2c,0x2c,0x2c,0x2d,0x2d,0x2d,0x2e,0x2e,0x2e,0x2f,0x2f,0x2f,0x30,0x30,0x30,
    0x30,0x31,0x31,0x31,0x31,0x32,0x32,0x32,0x32,0x32,0x33,0x33,0x33,0x33,0x34,0x34,0x34,
    0x34,0x34,0x34,0x35,0x35,0x35,0x35,0x35,0x35,0x36,0x36,0x36,0x36,0x36,0x36,0x37,0x37,
    0x37,0x37,0x37,0x37,0x37,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x39,0x39,0x39,0x39,0x39,
    0x39,0x39,0x39,0x39,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3a,0x3b,0x3b,0x3b,0x3b,
    0x3b,0x3b,0x3b,0x3b,0x3b,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c
    },
    
};

/*
 * This function initialises an instance of cmos_inttime_t.
 */
static __inline cmos_inttime_const_ptr_t cmos_inttime_initialize()
{
    switch( gu8SensorImageMode )
    {
        case 3:
            cmos_inttime.full_lines_std = 1125;
            cmos_inttime.full_lines_std_30fps = 1125;
            cmos_inttime.full_lines = 1125;
            cmos_inttime.full_lines_limit = 65535;
            cmos_inttime.max_lines_target = 1123;
            cmos_inttime.min_lines_target = 2;
            cmos_inttime.vblanking_lines = 1125;
            cmos_inttime.exposure_ashort = 0;
            cmos_inttime.lines_per_500ms = 16874; // 500ms / 29.63us = 16874
            cmos_inttime.flicker_freq = 0;//60*256;//50*256;
        break;
        
        case 2:
            cmos_inttime.full_lines_std = 750;
            cmos_inttime.full_lines_std_30fps = 750;
            cmos_inttime.full_lines = 750;
            cmos_inttime.full_lines_limit = 65535;
            cmos_inttime.max_lines_target = 748;
            cmos_inttime.min_lines_target = 2;
            cmos_inttime.vblanking_lines = 750;

            cmos_inttime.exposure_ashort = 0;

            cmos_inttime.lines_per_500ms = 22500; // 500ms / 29.63us = 16874
            cmos_inttime.flicker_freq = 0;//60*256;//50*256;
        break;

        case 1:
            cmos_inttime.full_lines_std = 750;
            cmos_inttime.full_lines_std_30fps = 750;
            cmos_inttime.full_lines = 750;
            cmos_inttime.full_lines_limit = 65535;
            cmos_inttime.max_lines_target = 748;
            cmos_inttime.min_lines_target = 2;
            cmos_inttime.vblanking_lines = 750;

            cmos_inttime.exposure_ashort = 0;

            cmos_inttime.lines_per_500ms = 22500; // 500ms / 29.63us = 16874
            cmos_inttime.flicker_freq = 0;//60*256;//50*256;
        break;

        default:
        break;
            
    }

	return &cmos_inttime;
}

/*
 * This function applies the new integration time to the ISP registers.
 */
static __inline void cmos_inttime_update(cmos_inttime_ptr_t p_inttime) 
{
    HI_U16 exp_time;
#if CMOS_IMX122_ISP_WRITE_SENSOR_ENABLE

    exp_time = p_inttime->full_lines - p_inttime->exposure_ashort;

  
    
    ISP_SSP_DATA_S   stSspData;
    stSspData.bDelayCfg = HI_FALSE;
    stSspData.u32DevAddr = 0x02;
    stSspData.u32DevAddrByteNum = 1;
    stSspData.u32RegAddr = 0x08;
    stSspData.u32RegAddrByteNum = 1;
    stSspData.u32Data = (exp_time & 0xFF);
    stSspData.u32DataByteNum = 1;
    HI_MPI_ISP_SSPWrite(&stSspData);

    stSspData.u32RegAddr = 0x09;
    stSspData.u32RegAddrByteNum = 1;
    stSspData.u32Data = ((exp_time & 0xFF00) >> 8);            
    stSspData.u32DataByteNum = 1;
    HI_MPI_ISP_SSPWrite(&stSspData);

#else
    exp_time = p_inttime->full_lines - p_inttime->exposure_ashort;
    sensor_write_register(EXPOSURE_ADDR, exp_time & 0xFF);
    sensor_write_register(EXPOSURE_ADDR + 1, (exp_time & 0xFF00) >> 8);
#endif

}

/*
 * This function applies the new vert blanking porch to the ISP registers.
 */
static __inline void cmos_vblanking_update(cmos_inttime_const_ptr_t p_inttime)
{
    
	HI_U16 vmax = p_inttime->full_lines;
 //         printf("vmax=%d",vmax);
	sensor_write_register(VMAX_ADDR, (vmax&0x00ff));
	sensor_write_register(VMAX_ADDR+1, ((vmax&0xff00) >> 8));

	return;
}

static __inline HI_U16 vblanking_calculate(
		cmos_inttime_ptr_t p_inttime)
{
	if (p_inttime->exposure_ashort >= p_inttime->full_lines - 3)
	{
		p_inttime->exposure_ashort = p_inttime->full_lines - 3;
	}

	p_inttime->vblanking_lines = p_inttime->full_lines - p_inttime->full_lines_std;
    
//    printf("vblanking_lines=%d",p_inttime->vblanking_lines);

	return p_inttime->exposure_ashort;
}



/* Set fps base */
#if 1
static __inline void cmos_fps_set(
		cmos_inttime_ptr_t p_inttime,
		const HI_U8 fps
		)
{

    if(gu8SensorImageMode == 3)
    {
    	switch(fps)
    	{
    		case 30:
    			// Change the frame rate via changing the vertical blanking
    			p_inttime->full_lines_std = 1125;
    			sensor_write_register(VMAX_ADDR, 0x65);
    			sensor_write_register(VMAX_ADDR+1, 0x04);
    			p_inttime->lines_per_500ms = 1125 * 30 / 2;
    		break;
    		
    		case 25:
    			// Change the frame rate via changing the vertical blanking
    			p_inttime->full_lines_std = 1350;
    			sensor_write_register(VMAX_ADDR, 0x46);
    			sensor_write_register(VMAX_ADDR+1, 0x05);
    			p_inttime->lines_per_500ms = 1350 * 25 / 2;
    		break;
    		
    		default:
    		break;
    	}
     }
    else if(gu8SensorImageMode == 2)
    {
        switch(fps)
        {
            case 60:
                // Change the frame rate via changing the vertical blanking
                p_inttime->full_lines_std = 750;
                p_inttime->lines_per_500ms = 750 * 60 / 2;
    			sensor_write_register(VMAX_ADDR, 0xEE);
    			sensor_write_register(VMAX_ADDR+1, 0x02);
            break;
            
            case 50:
                // Change the frame rate via changing the vertical blanking
                p_inttime->full_lines_std = 900;
                p_inttime->lines_per_500ms = 900 * 50 / 2;
    			sensor_write_register(VMAX_ADDR, 0x84);
    			sensor_write_register(VMAX_ADDR+1, 0x03);
            break;
            
            default:
            break;
        }
    

    }
    else if(gu8SensorImageMode == 1)
    {
        switch(fps)
        {
             case 30:
                 // Change the frame rate via changing the vertical blanking
                 p_inttime->full_lines_std = 750;
                 p_inttime->lines_per_500ms = 750 * 30 / 2;
                 sensor_write_register(VMAX_ADDR, 0xEE);
                 sensor_write_register(VMAX_ADDR+1, 0x02);
             break;
             
             case 25:
                 // Change the frame rate via changing the vertical blanking
                 p_inttime->full_lines_std = 900;
                 p_inttime->lines_per_500ms = 900 * 25 / 2;
                 sensor_write_register(VMAX_ADDR, 0x84);
                 sensor_write_register(VMAX_ADDR+1, 0x03);
             break;
             
             default:
             break;
        }

    }
    else
    {

    }

    return;
     
}

#endif

/*
 * This function initialises an instance of cmos_gains_t.
 */
static __inline cmos_gains_ptr_t cmos_gains_initialize()
{
	cmos_gains.again_shift = 10;
	cmos_gains.dgain_shift = 10;
	cmos_gains.dgain_fine_shift = 0;
    
	cmos_gains.max_again = 16229;                       //10 bit shift
	cmos_gains.max_dgain = 8134;                        //10 bit shift
	
    cmos_gains.isp_dgain_shift = 8;
    cmos_gains.isp_dgain = 1 << cmos_gains.isp_dgain_shift;
    cmos_gains.max_isp_dgain_target = 4 << cmos_gains.isp_dgain_shift;
    cmos_gains.isp_dgain_delay_cfg = HI_FALSE;

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
    /* analog gain 
      * analog gain = PGC * 0.3 db.
      * APGC = [0,80];
      * analog gain = [0db, 24db]. */

        ISP_SSP_DATA_S   stSspData;
#if  CMOS_IMX122_ISP_WRITE_SENSOR_ENABLE


        stSspData.bDelayCfg = HI_FALSE;
        stSspData.u32DevAddr = 0x02;
        stSspData.u32DevAddrByteNum = 1;
        stSspData.u32RegAddr = 0x1E;
        stSspData.u32RegAddrByteNum = 1;
        stSspData.u32Data = p_gains->again_db + p_gains->dgain_db;
        stSspData.u32DataByteNum = 1;

        if((p_gains->again_db + p_gains->dgain_db) < 0x8C)
        {
          HI_MPI_ISP_SSPWrite(&stSspData);
        }
        else
        {
           stSspData.u32Data = 0x8C;
           stSspData.u32DataByteNum = 1;
           HI_MPI_ISP_SSPWrite(&stSspData);
        }

#else
	
        if((p_gains->again_db + p_gains->dgain_db)< 0x8C)
		{  
		  sensor_write_register(PGC_ADDR, (p_gains->again_db + p_gains->dgain_db));
        }

        else
        {
          sensor_write_register(PGC_ADDR, 0x8C);
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
    
        if (dgain >= gain_table[60])
        {
             dgain = gain_table[60];
             step = 60;
        }
        else
        {
            for(i = 1; i < 61; i++)
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
	if(0 == isp_mode) /* setup for ISP 'normal mode' */
	{
        sensor_write_register(VMAX_ADDR, 0x65);
        sensor_write_register(VMAX_ADDR + 1, 0x04);
	}
	else if(1 == isp_mode) /* setup for ISP pixel calibration mode */
	{
        //TODO: finish this.
        /* Sensor must be programmed for slow frame rate (5 fps and below)*/
        /* change frame rate to 3 fps by setting 1 frame length = 1125 * (30/3) */
        sensor_write_register(VMAX_ADDR, 0xF2);
        sensor_write_register(VMAX_ADDR + 1, 0x2B);

        /* Analog and Digital gains both must be programmed for their minimum values */
		sensor_write_register(PGC_ADDR, 0x00);
       // sensor_write_register(APGC_ADDR + 1, 0x00);
	//	sensor_write_register(DPGC_ADDR, 0x00);
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


HI_U32 cmos_get_isp_speical_alg(void)
{
    return isp_special_alg_awb;
}

static cmos_isp_demosaic_t st_isp_demosaic =
{
    /*vh_slope*/
    0xc6,

    /*aa_slope*/
    0xc3,

    /*va_slope*/
    0xc4,

    /*uu_slope*/
    0xcd,

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
    0x8,

    /*sat_thresh*/
    0x171,

    /*ac_thresh*/
    0x1b3,
};


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

static HI_S32 isp_image_mode_get(cmos_sensor_image_mode_ptr_t p_cmos_sensor_image_mode)
{
 
 if(HI_NULL == p_cmos_sensor_image_mode)
  {
      printf("null pointer when set image mode111\n");
      return -1;
  }
 
  if((p_cmos_sensor_image_mode->u16Width == 1280)&&(p_cmos_sensor_image_mode->u16Height == 720))
  {
    if(p_cmos_sensor_image_mode->u16Fps== 30)
    {
      gu8SensorImageMode = SENSOR_720P_30FPS_MODE;
    }
    else if(p_cmos_sensor_image_mode->u16Fps== 60)
    {
       
        gu8SensorImageMode = SENSOR_720P_60FPS_MODE;
    }
    else
    {
       return -1;
    }
  }
  else if((p_cmos_sensor_image_mode->u16Width == 1920)&&(p_cmos_sensor_image_mode->u16Height == 1080))
  {
      if(p_cmos_sensor_image_mode->u16Fps == 30)
      {
        gu8SensorImageMode = SENSOR_1080P_30FPS_MODE;
      }
      else
      {
         return -1;
      }
  }
 
  return 0;

}
static HI_S32 cmos_set_sensor_image_mode(cmos_sensor_image_mode_ptr_t p_cmos_sensor_image_mode)
{
    HI_S32 s32Ret;

    if(NULL == p_cmos_sensor_image_mode)
    {
        printf("null pointer when set sensor image mode!\n");
        return -1;
    }


    s32Ret = isp_image_mode_get(p_cmos_sensor_image_mode);

    if(s32Ret == -1 )
    {
        return -1;
    }

    switch(gu8SensorImageMode)
    {
        case SENSOR_720P_30FPS_MODE:

        sensor_init_720p_30fps();

        break;

        case SENSOR_720P_60FPS_MODE:

        sensor_init_720p_60fps();

        break;

        case SENSOR_1080P_30FPS_MODE:

        sensor_init();

        break;

        default:

        break;

    }

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
	.pfn_cmos_set_sensor_image_mode = cmos_set_sensor_image_mode,

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


#endif // __SONY_ICX692_H_
