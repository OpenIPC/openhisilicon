
/*******************************************************************************
*	Description		: Adjust Equalizer manual mode(EQ)
*	Argurments		: ch: channel information
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
int nvp6134_set_equalizer_manual(unsigned char ch, unsigned char man_stage)
{
	unsigned int  acc_gain_status[16], y_plus_slope[16], y_minus_slope[16], sync_width_value[16];
	unsigned int  fr_ac_min_value[16], fr_ac_max_value[16], fr_dc_value[16];
	unsigned char vloss;
	unsigned char vidformat;
	int ret;

	/*
	 * Initialization structure
	 */
	if( bInitEQ == EQ_INIT_OFF )
	{
		memset(&s_eq, 0x00, sizeof(nvp6134_equalizer));
		memset(&s_eq_type, 0x00, sizeof(nvp6134_equalizer));
		bInitEQ = EQ_INIT_ON;
	}

	/*
	 * exception
	 * skip under NVP6134_VI_720P_2530 mode and over NVP6134_VI_BUTT
	 */
	if(ch_mode_status[ch] >= NVP6134_VI_BUTT )
	{
		s_eq.ch_previdmode[ch] = 0xFF;
		s_eq.ch_curvidmode[ch] = 0xFF;
		s_eq.ch_previdon[ch] = EQ_VIDEO_OFF;
		s_eq.ch_curvidon[ch] = EQ_VIDEO_OFF;
		s_eq.ch_previdformat[ch] = 0xFF;		 // pre video format for Auto detection value
		s_eq.ch_curvidformat[ch] = 0xFF;		 // current video format for Auto detection value
		s_eq.ch_vfmt_status[ch] = PAL;			 // default(PAL) : NTSC(0), PAL(1)ch_vfmt_status
		return 0;
	}

	/*
	 * check and set video loss and video on/off information to buffer
	 */
	if(1)
	{
		/* get video format(video loss), 0:videoloss, 1:video on */
		vidformat = g_ch_video_fmt[ch];
		if( vidformat == 0x00 )
			vloss = 0;
		else
			vloss = 1;

		/* after checking agc locking(signal) and video loss, save Video ON information to buffer */
		if( (vloss == 1) && (nvp6134_GetAgcLockStatus(ch) == 0x01))
		{
			s_eq.ch_curvidon[ch] = EQ_VIDEO_ON;
			s_eq.ch_curvidmode[ch] = ch_mode_status[ch];
			s_eq.ch_curvidformat[ch] = vidformat;
			s_eq.ch_vfmt_status[ch] = ch_vfmt_status[ch];	// NTSC/PAL
		}
		else
		{
			/* These are default value of NO video */
			s_eq.ch_curvidmode[ch] = 0xFF;
			s_eq.ch_curvidon[ch] = EQ_VIDEO_OFF;
			s_eq.ch_curvidformat[ch] = 0xFF;
			s_eq.ch_vfmt_status[ch] = PAL;
			s_eq.ch_stage[ch] = 0;
			s_eq.eq_file_save[ch] = 0;

			g_slp_ahd[ch] = 0;
			eq_chk_cnt[ch] = 0;
			/* set default value for distinguishing between EXC and AHD */
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x5C, 0x00);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xB8, 0xB9);  //recover this for video loss detection

			/* hide display */
			nvp6134_hide_ch(ch);
		}
	}
	
	s_eq_type.ch_equalizer_type[ch] = 1;
	s_eq_type.ch_stage[ch] = man_stage;

	/*
	 * it only want to set EQ(algorithm) once when there is the video signal.
	 *    - PRE(video off) != CUR(video on)
	 *    - PRE(video format) != CUR(Video ON)
	 */
	if( (s_eq.ch_curvidon[ch] != EQ_VIDEO_OFF) && (s_eq.ch_curvidmode[ch] != NVP6134_VI_1080P_NOVIDEO))
	{
		//if( 	(s_eq.ch_previdon[ch] != s_eq.ch_curvidon[ch]) 
		//	|| 	(s_eq.ch_previdmode[ch] != s_eq.ch_curvidmode[ch])
		//	|| 	(s_eq.ch_stage[ch] == 0))
		{
			printk(">>>>> DRV : CH[%d] EQ algorithm! - SET EQ \n", ch );
			/* set H PLL - No video option  and PN init */
        		gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
			if( (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_1080P && s_eq.ch_vfmt_status[ch] == PAL) ||
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_1080P ) ||
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_3M_NRT || s_eq.ch_curvidmode[ch] == NVP6134_VI_3M ||
				 s_eq.ch_curvidmode[ch] == NVP6134_VI_4M_NRT || s_eq.ch_curvidmode[ch] == NVP6134_VI_4M ||
				 s_eq.ch_curvidmode[ch] == NVP6134_VI_5M_NRT || s_eq.ch_curvidmode[ch] == NVP6134_VI_5M_20P) )
				gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xB8, 0x39);
			else
				gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xB8, 0x38);

			SetFSClockmodeForColorlocking( ch );
			msleep(35);

		        /* distinguish between A(THD, CHD) type and B(AHD0 type */
		        if(	(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PA) || \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_HDAEX) || \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PB) || \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_HDBEX) || \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_720PRT)||	\
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_1080P) ||	\
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_3M_NRT)||	\
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_5M_NRT)|| \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_1080P) || \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_720P)  || \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_720PRT)|| \
		            (s_eq.ch_curvidmode[ch] == NVP6134_VI_EXC_HDEX))
		        {		            
		            	/* manual set eq stage */
				if(1 == s_eq_type.ch_equalizer_type[ch])
					s_eq.ch_stage[ch] = s_eq_type.ch_stage[ch];
					
					/* adjust EQ depend on distance */
		            	eq_adjust_eqvalue( ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch], s_eq.ch_stage[ch] );
				Set_ColorLockingFilter(ch);

				s_eq.eq_file_save[ch] = 1;
		        }
		       else
			{				
				/* manual set eq stage */
				if(1 == s_eq_type.ch_equalizer_type[ch])
					s_eq.ch_stage[ch] = s_eq_type.ch_stage[ch];

				/* adjust EQ depend on distance */
				eq_adjust_eqvalue( ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch], s_eq.ch_stage[ch] );
				Set_ColorLockingFilter(ch);

				y_plus_slope[ch]	= GetYPlusSlope(ch);
				y_minus_slope[ch]	= GetYMinusSlope(ch);
				g_slp_ahd[ch] = 0;

				s_eq.eq_file_save[ch] = 1;
			}
			if(s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_1080P||s_eq.ch_curvidmode[ch] == NVP6134_VI_EXT_3M_NRT)
			{
				eq_chk_cnt[ch] ++;
				if(eq_chk_cnt[ch] < 2)
				{
					//return 1;
				}
			}
			printk(">>>>> DRV[%s:%d] CH:%d, s_eq.ch_stage:%d\n", __func__, __LINE__, ch, s_eq.ch_stage[ch]);
			/* after getting EQ value, Analog filter bypass option is off */
			if(	(s_eq.ch_curvidmode[ch] == NVP6134_VI_3M)	||
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_4M)	||
				(s_eq.ch_curvidmode[ch] == NVP6134_VI_5M_20P) )
			{
			}
			else
			{
				gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
				gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x59, 0x00);
			}
			printk(">>>>> DRV : getting EQ value, Analog filter bypass option is off\n" );

			/* send eqstage to isp */
			ret = acp_isp_write_communcation( ch, s_eq.ch_stage[ch], s_eq.ch_curvidmode[ch] );
			//eq_set_communication_value( ch, s_eq.ch_curvidmode[ch], s_eq.ch_vfmt_status[ch], ret );

			/* show display */
			nvp6134_show_ch(ch);

			g_eq_set_done = g_eq_set_done | (0x1 << ch);
			printk(">>>>> DRV[%s:%d] CH:%d, EQ SET DONE[%x]. \n", __func__, __LINE__, ch, g_eq_set_done );
		}
		
	}

	/*
	 * save current status to pre buffer(video on/off, video format)
	 */
	if(1)
	{
		s_eq.ch_previdon[ch] = s_eq.ch_curvidon[ch];
		s_eq.ch_previdmode[ch] =s_eq.ch_curvidmode[ch];
		s_eq.ch_previdformat[ch] = s_eq.ch_curvidformat[ch];
	}

	return 0;
}

