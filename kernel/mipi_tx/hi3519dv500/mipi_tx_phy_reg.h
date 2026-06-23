/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef MIPI_TX_PHY_REG_H
#define MIPI_TX_PHY_REG_H
/* Define the union u_pll_status */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ad_txpll_hvflag : 1; /* [0] */
        unsigned int ad_txpll_lock : 1;   /* [1] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 30; /* [31..2] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_pll_status;
/* Define the union u_aphy_txpll_test */
typedef union {
    /* Define the struct bits  */
    struct {
        // Comment of field: RG_TXPLL_TEST
        /*
        [29]:LOCK_OV
        [28]:SEL_LOCK
        [26]:EN_DIV_PLANB
        [25:22]:SEL_TST_CLK
        [21]:EN_TEST
        [20]:EN_LCKDET
        [14:13]:SEL_LDO_LV
        [12]:LDO_BYPASS_LV
        [11]:LPF_HBW_LV
        [10:8]:TEST_PLL_LV
        [7]:PD_CMP_LV
        [6:5]:SEL_CMP_LV
        [4]:RESET_VCO_LV
        [3]:PULLH_UP_LV
        [2]:PULLH_DN_LV
        [1]:EN_FREERUN_LV
        [0]:DISC_EXT_LV
        */
        unsigned int rg_txpll_test : 32; /* [31..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_txpll_test;
/* Define the union u_aphy_txpll_inctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int rg_txpll_ictrl : 5; /* [4..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 27; /* [31..5] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_txpll_inctrl;
/* Define the union u_aphy_test_atop_0 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int rg_test_atop_l : 32; /* [31..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_test_atop_0;
/* Define the union u_aphy_test_atop_1 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int rg_test_atop_h : 32; /* [31..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_test_atop_1;
/* Define the union u_aphy_sel_ldo_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int rg_sel_ldo : 4; /* [3..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 28; /* [31..4] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_sel_ldo_cfg;
/* Define the union u_aphy_enable_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int rg_sel_bg : 2; /* [1..0] */
        unsigned int rg_en_ldo : 2; /* [3..2] */
                                    // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 28; /* [31..4] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_enable_cfg;
/* Define the union u_aphy_bg_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int rg_en_bg : 1;   /* [0] */
        unsigned int rg_bg_en : 1;   /* [1] */
        unsigned int rg_test_bg : 2; /* [3..2] */
                                     // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 28; /* [31..4] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_bg_cfg;
/* Define the union u_aphy_pll_div_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: RG_TXPLL_DIVSEL_FB
        /* PLL frequency division coefficient of Hiwing V300.fb_div must be greater than 103. If fb_div is less
                than 103,
            set this parameter to 2 x fb_div and perform frequency division by multiple times.The value must be less
                than 256. If the value is greater than 256,
            set this parameter to fb_div / 2 and then double the frequency division.In later versions,
            the reg_pll_div_int in the pcs_old sheet is used. */
        unsigned int rg_txpll_divsel_fb : 11; /* [10..0] */
        unsigned int reserved_0 : 1;          /* [11] */
        unsigned int rg_en_sscdiv : 1;        /* [12] */
                                              // Comment of field: reserved_1
        /* Reserved  */
        unsigned int reserved_1 : 19; /* [31..13] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_pll_div_cfg;
/* Define the union u_aphy_self_bist_rsl */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: AD_BISTCLK
        /* APHY self - test BIST test result  */
        unsigned int ad_bistclk : 10; /* [9..0] */
                                      // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 22; /* [31..10] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_self_bist_rsl;
/* Define the union u_aphy_self_bist_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: RG_BIST_DELAY_SEL
        /* APHY self - test BISTdelay SEL configuration  */
        unsigned int rg_bist_delay_sel : 4; /* [3..0] */
        unsigned int reserved_0 : 4;        /* [7..4] */
                                            // Comment of field: RG_BIST_EN
                                            /* APHY self - test BIST EN configuration  */
        unsigned int rg_bist_en : 5;        /* [12..8] */
                                            // Comment of field: reserved_1
        /* Reserved  */
        unsigned int reserved_1 : 19; /* [31..13] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_self_bist_cfg;
/* Define the union u_aphy_ssc_set */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_set
        /* Setting the Frequency Divider Ratio  */
        unsigned int reg_set : 30; /* [29..0] */
                                   // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 2; /* [31..30] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_ssc_set;
/* Define the union u_aphy_ssc_ctrl0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_reset
        /* SSC reset signal  */
        unsigned int reg_reset : 1; /* [0] */
                                    // Comment of field: reg_ssc_sel
        /* SSC block to be selected(The ssmod_top block output is connected to the APHY PLL by default.)  */
        unsigned int reg_ssc_sel : 1;        /* [1] */
                                             // Comment of field: reg_downspread
                                             /* Downward Spreading  */
        unsigned int reg_downspread : 1;     /* [2] */
                                             // Comment of field: reg_sel_extwave
                                             /* Whether to select external values to control SSC output  */
        unsigned int reg_sel_extwave : 1;    /* [3] */
                                             // Comment of field: reg_disable_sscg
                                             /* Whether to disable SSC  */
        unsigned int reg_disable_sscg : 1;   /* [4] */
                                             // Comment of field: reg_hkmash_en
                                             /* Whether to enable HKMASH  */
        unsigned int reg_hkmash_en : 1;      /* [5] */
                                             // Comment of field: reg_mash111_hk_sel
                                             /* Whether to enable MASH111_HK  */
        unsigned int reg_mash111_hk_sel : 1; /* [6] */
                                             // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 1;  /* [7] */
                                      // Comment of field: reg_spread
                                      /* Control SSC Spreading Amplitude  */
        unsigned int reg_spread : 5;  /* [12..8] */
        unsigned int reserved_1 : 19; /* [31..13] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_ssc_ctrl0;
/* Define the union u_aphy_ssc_ctrl1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_extwaveval
        /* External Control Value  */
        unsigned int reg_extwaveval : 8;  /* [7..0] */
                                          // Comment of field: reg_ext_maxaddr
                                          /* External Control Address  */
        unsigned int reg_ext_maxaddr : 8; /* [15..8] */
                                          // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 16; /* [31..16] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_ssc_ctrl1;
/* Define the union u_aphy_ssc_ctrl2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_sld
        /* Load divider value  */
        unsigned int reg_sld : 1;      /* [0] */
                                       // Comment of field: reg_ssc_sync
                                       /* SSC loading synchronization signal  */
        unsigned int reg_ssc_sync : 1; /* [1] */
                                       // Comment of field: reg_clr
                                       /* SSC reset signal  */
        unsigned int reg_clr : 1;      /* [2] */
                                       // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 13; /* [15..3] */
                                      // Comment of field: reg_span
                                      /* Controls the frequency of the SSC spread spectrum. */
        unsigned int reg_span : 12;   /* [27..16] */
        // Comment of field: reserved_1
        /* Reserved  */
        unsigned int reserved_1 : 4; /* [31..28] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_ssc_ctrl2;
/* Define the union u_aphy_ssc_ctrl3 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_step
        /* Control the amplitude of the SSC. */
        unsigned int reg_step : 15;   /* [14..0] */
        unsigned int reserved_0 : 17; /* [31..15] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_ssc_ctrl3;
/* Define the union u_aphy_ssc_status0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: ssc_offset
        /* SSC OFFSET Output  */
        unsigned int ssc_offset : 30; /* [29..0] */
                                      // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 2; /* [31..30] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_ssc_status0;
/* Define the union u_aphy_ssc_status1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: set_offset
        /* SSC OFFSET Output  */
        unsigned int set_offset : 30; /* [29..0] */
                                      // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 2; /* [31..30] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_ssc_status1;
/* Define the union u_aphy_ssc_status2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: tbladdr
        /* SSC Lookup Table Address  */
        unsigned int tbladdr : 8;     /* [7..0] */
                                      // Comment of field: fb_div_ssc
                                      /* Connected to APHY RG_TXPLL_DIV_SSC<10 : 0>  */
        unsigned int fb_div_ssc : 11; /* [18..8] */
                                      // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 13; /* [31..19] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_ssc_status2;
/* Define the union u_aphy_ssc_status3 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: ssc_off
        /* Whether the SSC has stopped spreading  */
        unsigned int ssc_off : 1;      /* [0] */
                                       // Comment of field: segment
                                       /* SSC Spread Range  */
        unsigned int segment : 1;      /* [1] */
                                       // Comment of field: stop_flag
                                       /* Whether the SSC has stopped spreading  */
        unsigned int stop_flag : 1;    /* [2] */
                                       // Comment of field: reverse_flag
                                       /* SSC reverse signal  */
        unsigned int reverse_flag : 1; /* [3] */
                                       // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 12; /* [15..4] */
                                      // Comment of field: span_cnt
                                      /* SSC inverted value  */
        unsigned int span_cnt : 12;   /* [27..16] */
                                      // Comment of field: reserved_1
        /* Reserved  */
        unsigned int reserved_1 : 4; /* [31..28] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_ssc_status3;
/* Define the union u_aphy_ssc_fb_div_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: cfg_fb_div_ssc_value
        /* Software configuration value of fb_div_ssc  */
        unsigned int cfg_fb_div_ssc_value : 11; /* [10..0] */
        // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 5; /* [15..11] */
                                     // Comment of field: cfg_fb_div_ssc_ovr
        /* fb_div_ssc software configuration enable 0 : disabled 1 : enabled  */
        unsigned int cfg_fb_div_ssc_ovr : 1; /* [16] */
                                             // Comment of field: reserved_1
        /* Reserved  */
        unsigned int reserved_1 : 15; /* [31..17] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_ssc_fb_div_cfg;
/* Define the union u_pll_lock0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_fb_prd_sel
        /* The pll feedback clock has a period selection.Cycle configuration of the crystal oscillator to be
                    detected 3'b00 : 2 ^
            4 / clk_xtal;
        3'b01 : 2 ^ 5 / clk_xtal;
        3'b10 : 2 ^ 6 / clk_xtal;
        3'b11 : 2 ^ 7 / clk_xtal.others : 2 ^ 8 / clk_xtal  */
        unsigned int reg_fb_prd_sel : 3; /* [2..0] */
                                         // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 1; /* [3] */
                                     // Comment of field: reg_fb_div_sel
        /* PLL feedback clock frequency division select.Frequency division configuration of the clock to be
                detected 2'b00 : div 8;
        2'b01 : div 16;
        2'b10 : div 32;
        2'b11 : div 64. others : div 128.  */
        unsigned int reg_fb_div_sel : 3; /* [6..4] */
                                         // Comment of field: reserved_1
        /* Reserved  */
        unsigned int reserved_1 : 1;           /* [7] */
                                               // Comment of field: reg_ref_unlock_timer
                                               /* PLL reference clock out - of - lock timing period;
                                               1 indicates 2 ^ 4 clk_ref clocks. */
        unsigned int reg_ref_unlock_timer : 8; /* [15..8] */
                                               // Comment of field: reg_ref_lock_timer
                                               /* PLL reference clock lock timing period;
                                               1 indicates 2 ^ 8 clk_ref clocks. */
        unsigned int reg_ref_lock_timer : 8;   /* [23..16] */
                                               // Comment of field: reserved_2
        /* Reserved  */
        unsigned int reserved_2 : 8; /* [31..24] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_pll_lock0;
/* Define the union u_pll_lock1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_pll_lock_sel
        /* PLL feedback clock lock select .2'b00 : 1 / 10000 jitter;
        2'b01 : 5 / 10000 jitter;
        2'b10 : 1 / 1000 jitter;
        2'b11 : 1 / 100 jitter. */
        unsigned int reg_pll_lock_sel : 2;   /* [1..0] */
                                             // Comment of field: reg_pll_lock_times
                                             /* PLL feedback clock lock period .1'b0 : 1 time;
                                             1'b1 : 2 times  */
        unsigned int reg_pll_lock_times : 1; /* [2] */
                                             // Comment of field: reg_pll_unlock_sel
                                             /* PLL feedback clock out - of - lock select .0 : 10 % jitter;
                                             1 : 20 % jitter. */
        unsigned int reg_pll_unlock_sel : 1; /* [3] */
                                             // Comment of field: reg_pll_lockext_sel
        /* Number of waiting crystal oscillator cycles after extend pll lock.pll lock 0xx : no extend;
        100 : 2 ^ 5 extend;
        101 : 2 ^ 6 extend;
        110 : 2 ^ 7 extend;
        111 : 2 ^ 8 extend. */
        unsigned int reg_pll_lockext_sel : 3; /* [6..4] */
                                              // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 25; /* [31..7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_pll_lock1;
/* Define the union u_pll_lock2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: mipi_clk_lock
        /* pll lock 0 : pll unlock 1 : pll lock  */
        unsigned int mipi_clk_lock : 1; /* [0] */
                                        // Comment of field: clk_fb_exist
                                        /* fb clock exist 0 : clk_fb does not exist .1 : clk_fb exists. */
        unsigned int clk_fb_exist : 1;  /* [1] */
        unsigned int reserved_0 : 6;    /* [7..2] */
                                        // Comment of field: pll_cnt
                                        /* pll counter  */
        unsigned int pll_cnt : 17;      /* [24..8] */
                                        // Comment of field: reserved_1
        /* Reserved  */
        unsigned int reserved_1 : 7; /* [31..25] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_pll_lock2;
/* Define the union u_aphy_gpio_sel */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: cfg_gpio_sel
        /* GPIO signal selection configuration  */
        unsigned int cfg_gpio_sel : 5; /* [4..0] */
                                       // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 27; /* [31..5] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_aphy_gpio_sel;
/* Define the union u_phy_config0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: phy_testclr
        /*
        CDPHY test interface clear
        It is used when active performs vendor specific interface initialization
        (Active High).
        */
        unsigned int phy_testclr : 1; /* [0] */
        // Comment of field: phy_testclk
        /*
        CDPHY test interface strobe signal
        It is used to clock TESTDIN bus into the D-PHY. In conjunction with
        TESTEN signal controls the operation selection.
        */
        unsigned int phy_testclk : 1; /* [1] */
        unsigned int reserved_0 : 30; /* [31..2] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_phy_config0;
/* Define the union u_phy_config1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: phy_testdin
        /*
        CDPHY test interface input 8-bit data bus for internal register
        programming and test functionalities access.
        */
        unsigned int phy_testdin : 8; /* [7..0] */
        // Comment of field: phy_testdout
        /*
        CDPHY output 8-bit data bus for read-back and internal probing
        functionalities.
        */
        unsigned int phy_testdout : 8; /* [15..8] */
        // Comment of field: phy_testen
        /*
        CDPHY test interface operation selector:
        - 1: Configures address write operation on the falling edge of
        TESTCLK
        - 0: Configures a data write operation on the rising edge of TESTCLK.
        */
        unsigned int phy_testen : 1; /* [16] */
                                     // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 15; /* [31..17] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_phy_config1;
/* Define the union u_freq_det_hw_0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: cfg_freq_det_clr
        /* Clears the maximum and minimum detected values .0 : not cleared 1 : cleared  */
        unsigned int cfg_freq_det_clr : 1; /* [0] */
                                           // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 31; /* [31..1] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_freq_det_hw_0;
/* Define the union u_freq_det_hw_1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: freq_det_hw
        /* Detected real - time frequency, in 10 kHz. */
        unsigned int freq_det_hw : 20; /* [19..0] */
                                       // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 12; /* [31..20] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_freq_det_hw_1;
/* Define the union u_freq_det_hw_2 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: freq_det_hw_max
        /* Maximum detected frequency, in 10 kHz. */
        unsigned int freq_det_hw_max : 20; /* [19..0] */
                                           // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 12; /* [31..20] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_freq_det_hw_2;
/* Define the union u_freq_det_hw_3 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: freq_det_hw_min
        /* Indicates the minimum frequency detected.The unit is 10 kHz. */
        unsigned int freq_det_hw_min : 20; /* [19..0] */
                                           // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 12; /* [31..20] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_freq_det_hw_3;
/* Define the union u_lp_bist_err_cnt0 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: comp_lp_err_cnt_l0
        /* LP bsit error statistics of channel 0  */
        unsigned int comp_lp_err_cnt_l0 : 16; /* [15..0] */
                                              // Comment of field: comp_lp_err_cnt_l1
                                              /* Channel 1 LP bsit error statistics  */
        unsigned int comp_lp_err_cnt_l1 : 16; /* [31..16] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_lp_bist_err_cnt0;
/* Define the union u_lp_bist_err_cnt1 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: comp_lp_err_cnt_l2
        /* Channel 2 LP bsit error statistics  */
        unsigned int comp_lp_err_cnt_l2 : 16; /* [15..0] */
                                              // Comment of field: comp_lp_err_cnt_l3
                                              /* Channel 3 LP bsit error statistics  */
        unsigned int comp_lp_err_cnt_l3 : 16; /* [31..16] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32;
} u_lp_bist_err_cnt1;
/* Define the union u_common_reg_00 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_burst_ena : 1; /* [0] */
                                        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 3;               /* [3..1] */
        unsigned int reg_preamble_len_bit7to6 : 2; /* [5..4] */
        // Comment of field: reserved_1
        /*
        reserved
        */
        unsigned int reserved_1 : 2; /* [7..6] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_00;
/* Define the union u_common_reg_01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_preamble_len_bit5to0 : 6; /* [5..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 1;       /* [6] */
        unsigned int reg_preamble_ena : 1; /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_01;
/* Define the union u_common_reg_02 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_post_len : 6; /* [5..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 2; /* [7..6] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_02;
/* Define the union u_common_reg_03 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_preamble_0_bit5to0 : 6; /* [5..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 2; /* [7..6] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_03;
/* Define the union u_common_reg_04 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_preamble_0_bit11to6 : 6; /* [5..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 2; /* [7..6] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_04;
/* Define the union u_common_reg_05 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_preamble_0_bit117to12 : 6; /* [5..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 2; /* [7..6] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_05;
/* Define the union u_common_reg_06 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_preamble_0_bit20to18 : 3; /* [2..0] */
        unsigned int reg_preamble_1_bit2to0 : 3;   /* [5..3] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 2; /* [7..6] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_06;
/* Define the union u_common_reg_07 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_preamble_1_bit8to3 : 6; /* [5..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 2; /* [7..6] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_07;
/* Define the union u_common_reg_08 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_preamble_1_bit14to9 : 6; /* [5..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 2; /* [7..6] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_08;
/* Define the union u_common_reg_09 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_preamble_1_bit20to15 : 6; /* [5..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 2; /* [7..6] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_09;
/* Define the union u_common_reg_0d */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_cphy_calb : 2;           /* [1..0] */
        unsigned int reserved_0 : 2;              /* [3..2] */
        unsigned int reg_tcalaltseq_bit10to8 : 3; /* [6..4] */
                                                  // Comment of field: reserved_1
        /* Reserved  */
        unsigned int reserved_1 : 1; /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_0d;
/* Define the union u_common_reg_0e */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_tcalaltseq_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_0e;
/* Define the union u_common_reg_10 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_tm_pll_200us
        /* Counter, 200 us(not used in the current version)  */
        unsigned int reg_tm_pll_200us : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_10;
/* Define the union u_common_reg_11 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_tm_pll_1us
        /* Counter, timing 1 us  */
        unsigned int reg_tm_pll_1us : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_11;
/* Define the union u_common_reg_12 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_tm_pll_5us
        /* Counter, timing 5us  */
        unsigned int reg_tm_pll_5us : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_12;
/* Define the union u_common_reg_13 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_lock_time_bit7to0
        /* pll_lock counter  */
        unsigned int reg_lock_time_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_13;
/* Define the union u_common_reg_14 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_lock_time_bit11to8
        /* pll_lock counter  */
        unsigned int reg_lock_time_bit11to8 : 4; /* [3..0] */
                                                 // Comment of field: reg_div_sel
        /* cfg_clk frequency division coefficient selection;
        The frequency division coefficient is an exponential multiple of 2 of the configured value. */
        unsigned int reg_div_sel : 3; /* [6..4] */
        // Comment of field: reg_fdec_enb
        /* Clock frequency ratio detection enable after frequency division between
                    lanebyteclk and cfg_clk.The detection is enabled when the value 0 is configured. */
        unsigned int reg_fdec_enb : 1; /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_14;
/* Define the union u_common_reg_15 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ad_pll_band : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_15;
/* Define the union u_common_reg_16 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ad_pll_band_bit8 : 1; /* [0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 3;                   /* [3..1] */
        unsigned int ad_ssc_div_mode_dbgo_bit3to0 : 4; /* [7..4] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_16;
/* Define the union u_common_reg_17 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int ad_ssc_div_mode_dbgo_bit11to4 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_17;
/* Define the union u_common_reg_18 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ad_ssc_div_mode_dbgo_bit16to12 : 5; /* [4..0] */
                                                         // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 3; /* [7..5] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_18;
/* Define the union u_common_reg_19 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: det_value
        /* Frequency ratio of lanebyteclk to cfg_clk after frequency division  */
        unsigned int det_value : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_19;
/* Define the union u_common_reg_1a */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int afe_monitor_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_1a;
/* Define the union u_common_reg_1b */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int afe_monitor_bit15to8 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_1b;
/* Define the union u_common_reg_1c */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int afe_monitor_bit23to16 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_1c;
/* Define the union u_common_reg_1d */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int afe_monitor_bit31to24 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_1d;
/* Define the union u_common_reg_1e */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: lanebyteclk
        /* Clock monitor  */
        unsigned int lanebyteclk : 1; /* [0] */
                                      // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 1; /* [1] */
                                     // Comment of field: txclkesc
                                     /* Clock monitor  */
        unsigned int txclkesc : 1;   /* [2] */
                                     // Comment of field: reserved_1
        /* Reserved  */
        unsigned int reserved_1 : 5; /* [7..3] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_1e;
/* Define the union u_common_reg_20 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_ate_bist_en : 1; /* [0] */
                                          // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 7; /* [7..1] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_20;
/* Define the union u_common_reg_21 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_hsbist_en : 1;  /* [0] */
        unsigned int reg_hscalib_en : 1; /* [1] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 2;    /* [3..2] */
        unsigned int reg_lpbist_en : 1; /* [4] */
        // Comment of field: reserved_1
        /*
        reserved
        */
        unsigned int reserved_1 : 3; /* [7..5] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_21;
/* Define the union u_common_reg_22 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_bist_alive : 1; /* [0] */
        unsigned int reserved_0 : 3;     /* [3..1] */
        unsigned int reg_bist_sel : 2;   /* [5..4] */
        // Comment of field: reserved_1
        /*
        reserved
        */
        unsigned int reserved_1 : 2; /* [7..6] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_22;
/* Define the union u_common_reg_23 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_bist_seed_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_23;
/* Define the union u_common_reg_24 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_bist_seed_bit15to8 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_24;
/* Define the union u_common_reg_25 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_bist_data_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_25;
/* Define the union u_common_reg_26 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_bist_data_bit15to8 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_26;
/* Define the union u_common_reg_27 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_bist_round_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_27;
/* Define the union u_common_reg_28 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_sync_insert : 1;        /* [0] */
        unsigned int reg_sync_type : 3;          /* [3..1] */
        unsigned int reg_sync_intv_bit11to8 : 4; /* [7..4] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_28;
/* Define the union u_common_reg_29 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_sync_intv_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_29;
/* Define the union u_common_reg_2a */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_pwr_val_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_2a;
/* Define the union u_common_reg_2b */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_pwr_val_bit9to8 : 2; /* [1..0] */
                                              // Comment of field: reserved_0
                                              /*
                                              reserved
                                              */
        unsigned int reserved_0 : 6;          /* [7..2] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_2b;
/* Define the union u_common_reg_2c */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_pwr_txclkesc
        /* When pwr_dwn_req is enabled, txclkesc is output. */
        unsigned int reg_pwr_txclkesc : 1; /* [0] */
                                           // Comment of field: reg_pwr_refclk
                                           /* When pwr_dwn_req is enabled, refclk is output. */
        unsigned int reg_pwr_refclk : 1;   /* [1] */
                                           // Comment of field: reg_pwr_cfgclk
                                           /* When pwr_dwn_req is enabled, cfgclk is output. */
        unsigned int reg_pwr_cfgclk : 1;   /* [2] */
                                           // Comment of field: reg_pwr_testen
                                           /* Output testen when pwr_dwn_req is enabled. */
        unsigned int reg_pwr_testen : 1;   /* [3] */
                                           // Comment of field: reg_pwr_forcepll
                                           /* When pwr_dwn_req is enabled, forcepll is output. */
        unsigned int reg_pwr_forcepll : 1; /* [4] */
        // Comment of field: reg_pwr_cfg_cdphy
        /* When pwr_dwn_req is enabled,
            the cdphy value is output.The default value is dphy. */
        unsigned int reg_pwr_cfg_cdphy : 1; /* [5] */
                                            // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 2; /* [7..6] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_2c;
/* Define the union u_common_reg_2d */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_gbl_lane : 5; /* [4..0] */
                                       // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 3; /* [7..5] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_2d;
/* Define the union u_common_reg_2e */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_config
        /*
        Overwrite enable for the cdphy and lane modes
         */
        unsigned int reg_config : 1;       /* [0] */
                                           // Comment of field: reg_cfg_cdphy
                                           /* cdphy overwtite value.The default value is dphy. */
        unsigned int reg_cfg_cdphy : 1;    /* [1] */
                                           // Comment of field: reg_cfg_dphylane
                                           /* Overwtite value of lane count  */
        unsigned int reg_cfg_dphylane : 5; /* [6..2] */
        unsigned int reg_cal_policy : 1;   /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_2e;
/* Define the union u_common_reg_2f */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_test_lane : 4; /* [3..0] */
                                        // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 4; /* [7..4] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_2f;
/* Define the union u_common_reg_30 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_test_mode_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_30;
/* Define the union u_common_reg_31 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_test_mode_bit15to8 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_31;
/* Define the union u_common_reg_32 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_test_mode_bit18to16 : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 5; /* [7..3] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_32;
/* Define the union u_common_reg_33 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_t_da_cphy_a1 : 7; /* [6..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 1; /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_33;
/* Define the union u_common_reg_34 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_t_da_cphy_a2 : 7; /* [6..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 1; /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_34;
/* Define the union u_common_reg_35 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_t_da_cphy_b1 : 7; /* [6..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 1; /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_35;
/* Define the union u_common_reg_36 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_t_da_cphy_b2 : 7; /* [6..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 1; /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_36;
/* Define the union u_common_reg_37 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_t_da_cphy_c1 : 7; /* [6..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 1; /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_37;
/* Define the union u_common_reg_38 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_t_da_cphy_c2 : 7; /* [6..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 1; /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_38;
/* Define the union u_common_reg_39 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_t_da_hstx_data : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_39;
/* Define the union u_common_reg_3a */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_t_da_hstx_data_clk : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_3a;
/* Define the union u_common_reg_3b */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_t_da_lprxcd_en : 1;     /* [0] */
        unsigned int reg_t_da_lptx_dp : 1;       /* [1] */
        unsigned int reg_t_da_lptx_dn : 1;       /* [2] */
        unsigned int reg_t_da_lptx_en : 1;       /* [3] */
        unsigned int reg_t_da_lptx_enbias : 1;   /* [4] */
        unsigned int reg_t_da_lptx_enb_ulps : 1; /* [5] */
        unsigned int reg_t_da_hstx_swten : 1;    /* [6] */
        unsigned int reg_t_rg_hsclk_en : 1;      /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_3b;
/* Define the union u_common_reg_3c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_t_rg_pll_bias_en : 1; /* [0] */
        unsigned int reg_t_rg_pll_en : 1;      /* [1] */
        unsigned int reg_t_rg_ssc_resetj : 1;  /* [2] */
        unsigned int reg_t_rg_ssc_resetn : 1;  /* [3] */
        unsigned int reg_t_rg_ssc_resetsj : 1; /* [4] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 3; /* [7..5] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_3c;
/* Define the union u_common_reg_3d */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_t_rg_lptx_bias_en : 1;  /* [0] */
        unsigned int reg_t_rg_refen : 1;         /* [1] */
        unsigned int reg_t_rg_vcm_en : 1;        /* [2] */
        unsigned int reg_t_rg_hstx_bias_en : 1;  /* [3] */
        unsigned int reg_t_rg_pll_ser_ck_en : 1; /* [4] */
        unsigned int reg_t_rg_ch_en : 1;         /* [5] */
        unsigned int reg_t_rg_ch_en_eq : 1;      /* [6] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 1; /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_3d;
/* Define the union u_common_reg_3f */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_bist_round_bit15to8 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_3f;
/* Define the union u_common_reg_40 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_ssc_r_prbs1_en : 1;   /* [0] */
        unsigned int reg_ssc_r_prbs2_en : 1;   /* [1] */
        unsigned int reg_ssc_r_prbs3_en : 1;   /* [2] */
        unsigned int reg_ssc_r_fracssc_en : 1; /* [3] */
        unsigned int reg_ssc_r_frac_en : 1;    /* [4] */
        unsigned int reg_ssc_r_sscdiv_en : 1;  /* [5] */
        unsigned int reg_ssc_r_sdmtype : 1;    /* [6] */
        unsigned int reg_ssc_r_spread_en : 1;  /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_40;
/* Define the union u_common_reg_41 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_ssc_r_div_frac : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_41;
/* Define the union u_common_reg_42 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_ssc_r_triinc : 7; /* [6..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 1; /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_42;
/* Define the union u_common_reg_43 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_ssc_r_sscdivnum : 4;      /* [3..0] */
        unsigned int reg_ssc_r_triangular_sel : 1; /* [4] */
        unsigned int reg_ssc_r_ups_en : 1;         /* [5] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 2; /* [7..6] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_43;
/* Define the union u_common_reg_44 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_ssc_r_spreadmax : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_44;
/* Define the union u_common_reg_45 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_pll_load_band_en : 1; /* [0] */
        unsigned int reg_pll_lat_en : 1;       /* [1] */
        unsigned int reg_pll_lat_enb : 1;      /* [2] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 1;    /* [3] */
        unsigned int reg_pll_lpf_r : 2; /* [5..4] */
                                        // Comment of field: reserved_1
        /* Reserved  */
        unsigned int reserved_1 : 1;        /* [6] */
        unsigned int reg_pll_band_bit8 : 1; /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_45;
/* Define the union u_common_reg_46 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_pll_band_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_46;
/* Define the union u_common_reg_48 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_pll_phy_posdiv
        /* PHY PLL POSDIV Parameter Configuration  */
        unsigned int reg_pll_phy_posdiv : 3; /* [2..0] */
        // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 5; /* [7..3] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_48;
/* Define the union u_common_reg_49 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_pll_phy_div_int_bit10to8
        /* PHY PLL DIV_INT [10:8] parameter configuration  */
        unsigned int reg_pll_phy_div_int_bit10to8 : 3; /* [2..0] */
                                                       // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 1;         /* [3] */
                                             // Comment of field: reg_pll_phy_prediv
                                             /* PHY PLL PREDIV Parameter Configuration  */
        unsigned int reg_pll_phy_prediv : 3; /* [6..4] */
        // Comment of field: reserved_1
        /* Reserved  */
        unsigned int reserved_1 : 1; /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_49;
/* Define the union u_common_reg_4a */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_pll_phy_div_int_bit7to0
        /* PHY PLL POSDIV Parameter Configuration  */
        unsigned int reg_pll_phy_div_int_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_4a;
/* Define the union u_common_reg_4b */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_pll_phy_div_update
        /* PHY parameter configuration update register  */
        unsigned int reg_pll_phy_div_update : 1; /* [0] */
        // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 7; /* [7..1] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_4b;
/* Define the union u_common_reg_50 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_hstx_ser_ckneg : 1; /* [0] */
        unsigned int reg_vcm_meth : 2;       /* [2..1] */
        unsigned int reg_vcmsel : 5;         /* [7..3] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_50;
/* Define the union u_common_reg_51 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_dphy_emi_mode : 1;   /* [0] */
        unsigned int reg_encs : 1;            /* [1] */
        unsigned int reg_en_bw : 2;           /* [3..2] */
        unsigned int reg_lprxcd_selenfor : 1; /* [4] */
        unsigned int reg_lprxcd_selforhi : 1; /* [5] */
        unsigned int reg_lptx_selenfor : 1;   /* [6] */
        unsigned int reg_lptx_selforhi : 1;   /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_51;
/* Define the union u_common_reg_52 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_lptx_sri_dn : 4; /* [3..0] */
        unsigned int reg_lptx_sri_pu : 4; /* [7..4] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_52;
/* Define the union u_common_reg_53 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_cphy_deemp_mode : 1;   /* [0] */
        unsigned int reg_cphy_t1mode_en : 1;    /* [1] */
        unsigned int reg_en_v1d2 : 1;           /* [2] */
        unsigned int reg_ser_psave : 1;         /* [3] */
        unsigned int reg_lprxcd_en_bit9to8 : 2; /* [5..4] */
        unsigned int reg_ch_en_eq_bit9to8 : 2;  /* [7..6] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_53;
/* Define the union u_common_reg_54 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_lprxcd_en_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_54;
/* Define the union u_common_reg_55 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_ch_en_eq_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_55;
/* Define the union u_common_reg_56 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_dphy_drv_en : 6; /* [5..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 2; /* [7..6] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_56;
/* Define the union u_common_reg_57 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_cphy_dedrv_en : 5;  /* [4..0] */
        unsigned int reg_cphy_predrv_en : 3; /* [7..5] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_57;
/* Define the union u_common_reg_58 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_eq_sel_even_bit9to8 : 2; /* [1..0] */
        unsigned int reg_eq_sel_odd_bit9to8 : 2;  /* [3..2] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 4; /* [7..4] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_58;
/* Define the union u_common_reg_59 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_eq_sel_even_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_59;
/* Define the union u_common_reg_5a */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_eq_sel_odd_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_5a;
/* Define the union u_common_reg_5e */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_top_dmy_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_5e;
/* Define the union u_common_reg_5f */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_top_dmy_bit15to8 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_common_reg_5f;
/* Define the union u_lane_reg_01 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_dbug_pat : 4; /* [3..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 1;   /* [4] */
        unsigned int reg_prbs_pat : 3; /* [7..5] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_01;
/* Define the union u_lane_reg_02 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_prbs_seed0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_02;
/* Define the union u_lane_reg_03 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_prbs_seed1 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_03;
/* Define the union u_lane_reg_04 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_prbs_seed2 : 2; /* [1..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 6; /* [7..2] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_04;
/* Define the union u_lane_reg_05 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_comp_round_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_05;
/* Define the union u_lane_reg_06 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_wire_swap : 1; /* [0] */
        unsigned int reg_lp_cmd : 1;    /* [1] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 2;   /* [3..2] */
        unsigned int reg_tri_type : 1; /* [4] */
        unsigned int reg_sdw_enb : 1;  /* [5] */
        // Comment of field: reserved_1
        /*
        reserved
        */
        unsigned int reserved_1 : 2; /* [7..6] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_06;
/* Define the union u_lane_reg_07 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_comp_round_bit15to8 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_07;
/* Define the union u_lane_reg_08 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int ctrl_lp_monitor_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_08;
/* Define the union u_lane_reg_09 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int ctrl_lp_monitor_bit15to8 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_09;
/* Define the union u_lane_reg_0a */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lp_rev_state : 4; /* [3..0] */
        unsigned int hs_fsm_state : 4; /* [7..4] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_0a;
/* Define the union u_lane_reg_0b */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int lp_fwd_state : 7; /* [6..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 1; /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_0b;
/* Define the union u_lane_reg_0c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int comp_lp_ok : 1;     /* [0] */
        unsigned int comp_lp_done : 1;   /* [1] */
        unsigned int det_lp_fail : 1;    /* [2] */
        unsigned int det_lp_on_comp : 1; /* [3] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 4; /* [7..4] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_0c;
/* Define the union u_lane_reg_0d */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int ctrl_monitor_bit7to0 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_0d;
/* Define the union u_lane_reg_0e */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int ctrl_monitor_bit15to8 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_0e;
/* Define the union u_lane_reg_0f */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ctrl_monitor_bit21to16 : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_0f;
/* Define the union u_lane_reg_10 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_pre_req_dy
        /* The clock lane enters from txrequesthsclk.Indicates the delay time of the HS mode.The unit is hsclk period.
         */
        unsigned int reg_pre_req_dy : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_10;
/* Define the union u_lane_reg_11 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_pos_req_dy
        /* The clock lane exits from txrequesthsclk.Indicates the delay time in HS mode.The unit is hsclk cycle. */
        unsigned int reg_pos_req_dy : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_11;
/* Define the union u_lane_reg_12 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_tlpx
        /* Lane TLPX time, in hsclk period. */
        unsigned int reg_tlpx : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_12;
/* Define the union u_lane_reg_13 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_tprepare
        /* Lane Tprepare time, in hsclk period. */
        unsigned int reg_tprepare : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_13;
/* Define the union u_lane_reg_14 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_tzero
        /* Lane Tzero time, in hsclk cycle. */
        unsigned int reg_tzero : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_14;
/* Define the union u_lane_reg_15 */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_thstrail
        /* Lane Ths_trail time, in hsclk period. */
        unsigned int reg_thstrail : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_15;
/* Define the union u_lane_reg_16 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_clock_val : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_16;
/* Define the union u_lane_reg_17 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_symb_init : 3; /* [2..0] */
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 1;      /* [3] */
        unsigned int reg_lpx_or : 2;      /* [5..4] */
        unsigned int reg_eot_end_off : 1; /* [6] */
        unsigned int reg_force_eot : 1;   /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_17;
/* Define the union u_lane_reg_18 */
typedef union {
    /* Define the struct bits  */
    struct {
        unsigned int reg_prepr_offset : 8; /* [7..0] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_18;
/* Define the union u_lane_reg_19 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_bias_and : 2; /* [1..0] */
        unsigned int reg_bias_or : 2;  /* [3..2] */
        unsigned int reg_vcm_and : 2;  /* [5..4] */
        unsigned int reg_vcm_or : 2;   /* [7..6] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_19;
/* Define the union u_lane_reg_1a */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_drv_and : 2;  /* [1..0] */
        unsigned int reg_drv_or : 2;   /* [3..2] */
        unsigned int reg_drv2_and : 2; /* [5..4] */
        unsigned int reg_drv2_or : 2;  /* [7..6] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_1a;
/* Define the union u_lane_reg_1b */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reserved_0
        /*
        reserved
        */
        unsigned int reserved_0 : 1;     /* [0] */
        unsigned int reg_drv_ext_on : 1; /* [1] */
        unsigned int reg_lpx_and : 2;    /* [3..2] */
        // Comment of field: reserved_1
        /*
        reserved
        */
        unsigned int reserved_1 : 4; /* [7..4] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_1b;
/* Define the union u_lane_reg_1c */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_clkesc_inv : 1; /* [0] */
        unsigned int reg_errcd_sel : 1;  /* [1] */
        unsigned int reg_rx_dly_sel : 1; /* [2] */
                                         // Comment of field: reserved_0
        /* Reserved  */
        unsigned int reserved_0 : 1;   /* [3] */
        unsigned int reg_ulps_dly : 3; /* [6..4] */
                                       // Comment of field: reserved_1
        /* Reserved  */
        unsigned int reserved_1 : 1; /* [7] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_1c;
/* Define the union u_lane_reg_1d */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: reg_ttago
        /* Lane Tta_go time, in txclkesc period. */
        unsigned int reg_ttago : 4;  /* [3..0] */
                                     // Comment of field: reg_ttaget
                                     /* Lane Tta_get time, in txclkesc period. */
        unsigned int reg_ttaget : 4; /* [7..4] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_1d;
/* Define the union u_lane_reg_1e */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reg_comp_clr : 1;       /* [0] */
        unsigned int reg_comp_round_end : 1; /* [1] */
        unsigned int reserved_0 : 6;         /* [7..2] */
    } bits;
    /* Define an unsigned member */
    unsigned int u32 : 8;
} u_lane_reg_1e;
// ==============================================================================
/* Define the global struct */
typedef struct {
    u_pll_status pll_status;                   /* 0x0 */
    u_aphy_txpll_test aphy_txpll_test;         /* 0x4 */
    u_aphy_txpll_inctrl aphy_txpll_inctrl;     /* 0x8 */
    u_aphy_test_atop_0 aphy_test_atop_0;       /* 0xc */
    u_aphy_test_atop_1 aphy_test_atop_1;       /* 0x10 */
    u_aphy_sel_ldo_cfg aphy_sel_ldo_cfg;       /* 0x14 */
    u_aphy_enable_cfg aphy_enable_cfg;         /* 0x18 */
    u_aphy_bg_cfg aphy_bg_cfg;                 /* 0x1c */
    u_aphy_pll_div_cfg aphy_pll_div_cfg;       /* 0x20 */
    u_aphy_self_bist_rsl aphy_self_bist_rsl;   /* 0x24 */
    u_aphy_self_bist_cfg aphy_self_bist_cfg;   /* 0x28 */
    unsigned int reserved_18[7];               /* 0x2c~0x44 */
    u_aphy_ssc_set aphy_ssc_set;               /* 0x48 */
    u_aphy_ssc_ctrl0 aphy_ssc_ctrl0;           /* 0x4c */
    u_aphy_ssc_ctrl1 aphy_ssc_ctrl1;           /* 0x50 */
    u_aphy_ssc_ctrl2 aphy_ssc_ctrl2;           /* 0x54 */
    u_aphy_ssc_ctrl3 aphy_ssc_ctrl3;           /* 0x58 */
    u_aphy_ssc_status0 aphy_ssc_status0;       /* 0x5c */
    u_aphy_ssc_status1 aphy_ssc_status1;       /* 0x60 */
    u_aphy_ssc_status2 aphy_ssc_status2;       /* 0x64 */
    u_aphy_ssc_status3 aphy_ssc_status3;       /* 0x68 */
    u_aphy_ssc_fb_div_cfg aphy_ssc_fb_div_cfg; /* 0x6c */
    unsigned int reserved_19[11];              /* 0x70~0x98 */
    u_pll_lock0 pll_lock0;                     /* 0x9c */
    u_pll_lock1 pll_lock1;                     /* 0xa0 */
    u_pll_lock2 pll_lock2;                     /* 0xa4 */
    u_aphy_gpio_sel aphy_gpio_sel;             /* 0xa8 */
    unsigned int reserved_20[85];              /* 0xac~0x1fc */
    u_phy_config0 phy_config0;                 /* 0x200 */
    u_phy_config1 phy_config1;                 /* 0x204 */
    u_freq_det_hw_0 freq_det_hw_0;             /* 0x208 */
    u_freq_det_hw_1 freq_det_hw_1;             /* 0x20c */
    u_freq_det_hw_2 freq_det_hw_2;             /* 0x210 */
    u_freq_det_hw_3 freq_det_hw_3;             /* 0x214 */
    unsigned int reserved_21[6];               /* 0x218~0x22c */
    u_lp_bist_err_cnt0 lp_bist_err_cnt0;       /* 0x230 */
    u_lp_bist_err_cnt1 lp_bist_err_cnt1;       /* 0x234 */
    unsigned int reserved_22[626];             /* 0x238~0xbff */
    u_common_reg_00 common_reg_00;             /* 0xc00 */
    u_common_reg_01 common_reg_01;             /* 0xc04 */
    u_common_reg_02 common_reg_02;             /* 0xc08 */
    u_common_reg_03 common_reg_03;             /* 0xc0c */
    u_common_reg_04 common_reg_04;             /* 0xc10 */
    u_common_reg_05 common_reg_05;             /* 0xc14 */
    u_common_reg_06 common_reg_06;             /* 0xc18 */
    u_common_reg_07 common_reg_07;             /* 0xc1c */
    u_common_reg_08 common_reg_08;             /* 0xc20 */
    u_common_reg_09 common_reg_09;             /* 0xc24 */
    unsigned int reserved_32[3];               /* 0xc28~0xc30 */
    u_common_reg_0d common_reg_0d;             /* 0xc34 */
    u_common_reg_0e common_reg_0e;             /* 0xc38 */
    unsigned int reserved_34;                  /* 0xc3c */
    u_common_reg_10 common_reg_10;             /* 0xc40 */
    u_common_reg_11 common_reg_11;             /* 0xc44 */
    u_common_reg_12 common_reg_12;             /* 0xc48 */
    u_common_reg_13 common_reg_13;             /* 0xc4c */
    u_common_reg_14 common_reg_14;             /* 0xc50 */
    u_common_reg_15 common_reg_15;             /* 0xc54 */
    u_common_reg_16 common_reg_16;             /* 0xc58 */
    u_common_reg_17 common_reg_17;             /* 0xc5c */
    u_common_reg_18 common_reg_18;             /* 0xc60 */
    u_common_reg_19 common_reg_19;             /* 0xc64 */
    u_common_reg_1a common_reg_1a;             /* 0xc68 */
    u_common_reg_1b common_reg_1b;             /* 0xc6c */
    u_common_reg_1c common_reg_1c;             /* 0xc70 */
    u_common_reg_1d common_reg_1d;             /* 0xc74 */
    u_common_reg_1e common_reg_1e;             /* 0xc78 */
    unsigned int reserved_49;                  /* 0xc7c */
    u_common_reg_20 common_reg_20;             /* 0xc80 */
    u_common_reg_21 common_reg_21;             /* 0xc84 */
    u_common_reg_22 common_reg_22;             /* 0xc88 */
    u_common_reg_23 common_reg_23;             /* 0xc8c */
    u_common_reg_24 common_reg_24;             /* 0xc90 */
    u_common_reg_25 common_reg_25;             /* 0xc94 */
    u_common_reg_26 common_reg_26;             /* 0xc98 */
    u_common_reg_27 common_reg_27;             /* 0xc9c */
    u_common_reg_28 common_reg_28;             /* 0xca0 */
    u_common_reg_29 common_reg_29;             /* 0xca4 */
    u_common_reg_2a common_reg_2a;             /* 0xca8 */
    u_common_reg_2b common_reg_2b;             /* 0xcac */
    u_common_reg_2c common_reg_2c;             /* 0xcb0 */
    u_common_reg_2d common_reg_2d;             /* 0xcb4 */
    u_common_reg_2e common_reg_2e;             /* 0xcb8 */
    u_common_reg_2f common_reg_2f;             /* 0xcbc */
    u_common_reg_30 common_reg_30;             /* 0xcc0 */
    u_common_reg_31 common_reg_31;             /* 0xcc4 */
    u_common_reg_32 common_reg_32;             /* 0xcc8 */
    u_common_reg_33 common_reg_33;             /* 0xccc */
    u_common_reg_34 common_reg_34;             /* 0xcd0 */
    u_common_reg_35 common_reg_35;             /* 0xcd4 */
    u_common_reg_36 common_reg_36;             /* 0xcd8 */
    u_common_reg_37 common_reg_37;             /* 0xcdc */
    u_common_reg_38 common_reg_38;             /* 0xce0 */
    u_common_reg_39 common_reg_39;             /* 0xce4 */
    u_common_reg_3a common_reg_3a;             /* 0xce8 */
    u_common_reg_3b common_reg_3b;             /* 0xcec */
    u_common_reg_3c common_reg_3c;             /* 0xcf0 */
    u_common_reg_3d common_reg_3d;             /* 0xcf4 */
    u_common_reg_3f common_reg_3f;             /* 0xcf8 */
    unsigned int reserved_80;                  /* 0xcfc */
    u_common_reg_40 common_reg_40;             /* 0xd00 */
    u_common_reg_41 common_reg_41;             /* 0xd04 */
    u_common_reg_42 common_reg_42;             /* 0xd08 */
    u_common_reg_43 common_reg_43;             /* 0xd0c */
    u_common_reg_44 common_reg_44;             /* 0xd10 */
    u_common_reg_45 common_reg_45;             /* 0xd14 */
    u_common_reg_46 common_reg_46;             /* 0xd18 */
    unsigned int reserved_87;                  /* 0xd1c */
    u_common_reg_48 common_reg_48;             /* 0xd20 */
    u_common_reg_49 common_reg_49;             /* 0xd24 */
    u_common_reg_4a common_reg_4a;             /* 0xd28 */
    u_common_reg_4b common_reg_4b;             /* 0xd2c */
    unsigned int reserved_91[4];               /* 0xd30~0xd3c */
    u_common_reg_50 common_reg_50;             /* 0xd40 */
    u_common_reg_51 common_reg_51;             /* 0xd44 */
    u_common_reg_52 common_reg_52;             /* 0xd48 */
    u_common_reg_53 common_reg_53;             /* 0xd4c */
    u_common_reg_54 common_reg_54;             /* 0xd50 */
    u_common_reg_55 common_reg_55;             /* 0xd54 */
    u_common_reg_56 common_reg_56;             /* 0xd58 */
    u_common_reg_57 common_reg_57;             /* 0xd5c */
    u_common_reg_58 common_reg_58;             /* 0xd60 */
    u_common_reg_59 common_reg_59;             /* 0xd64 */
    u_common_reg_5a common_reg_5a;             /* 0xd68 */
    unsigned int reserved_102[3];              /* 0xd6c~0xd74 */
    u_common_reg_5e common_reg_5e;             /* 0xd78 */
    u_common_reg_5f common_reg_5f;             /* 0xd7c */
    unsigned int reserved_104;                 /* 0xd80 */
    u_lane_reg_01 lane_reg_01;                 /* 0xd84~0xf84 */
    u_lane_reg_02 lane_reg_02;                 /* 0xd88~0xf88 */
    u_lane_reg_03 lane_reg_03;                 /* 0xd8c~0xf8c */
    u_lane_reg_04 lane_reg_04;                 /* 0xd90~0xf90 */
    u_lane_reg_05 lane_reg_05;                 /* 0xd94~0xf94 */
    u_lane_reg_06 lane_reg_06;                 /* 0xd98~0xf98 */
    u_lane_reg_07 lane_reg_07;                 /* 0xd9c~0xf9c */
    u_lane_reg_08 lane_reg_08;                 /* 0xda0~0xfa0 */
    u_lane_reg_09 lane_reg_09;                 /* 0xda4~0xfa4 */
    u_lane_reg_0a lane_reg_0a;                 /* 0xda8~0xfa8 */
    u_lane_reg_0b lane_reg_0b;                 /* 0xdac~0xfac */
    u_lane_reg_0c lane_reg_0c;                 /* 0xdb0~0xfb0 */
    u_lane_reg_0d lane_reg_0d;                 /* 0xdb4~0xfb4 */
    u_lane_reg_0e lane_reg_0e;                 /* 0xdb8~0xfb8 */
    u_lane_reg_0f lane_reg_0f;                 /* 0xdbc~0xfbc */
    u_lane_reg_10 lane_reg_10;                 /* 0xdc0~0xfc0 */
    u_lane_reg_11 lane_reg_11;                 /* 0xdc4~0xfc4 */
    u_lane_reg_12 lane_reg_12;                 /* 0xdc8~0xfc8 */
    u_lane_reg_13 lane_reg_13;                 /* 0xdcc~0xfcc */
    u_lane_reg_14 lane_reg_14;                 /* 0xdd0~0xfd0 */
    u_lane_reg_15 lane_reg_15;                 /* 0xdd4~0xfd4 */
    u_lane_reg_16 lane_reg_16;                 /* 0xdd8~0xfd8 */
    u_lane_reg_17 lane_reg_17;                 /* 0xddc~0xfdc */
    u_lane_reg_18 lane_reg_18;                 /* 0xde0~0xfe0 */
    u_lane_reg_19 lane_reg_19;                 /* 0xde4~0xfe4 */
    u_lane_reg_1a lane_reg_1a;                 /* 0xde8~0xfe8 */
    u_lane_reg_1b lane_reg_1b;                 /* 0xdec~0xfec */
    u_lane_reg_1c lane_reg_1c;                 /* 0xdf0~0xff0 */
    u_lane_reg_1d lane_reg_1d;                 /* 0xdf4~0xff4 */
    u_lane_reg_1e lane_reg_1e;                 /* 0xdf8~0xff8 */
} mipi_tx_phy_regs_type_t;

#endif /* MIPI_TX_PHY_REG_H */
