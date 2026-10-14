#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "cv4003.h"
#include "cv4003_ctrl.h"

#define CV4003_REG_ADDR_MIRROR_FLIP 0x3028

SENSOR_PRIORITY_DATA static xmedia_s32 g_cv4003_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
SENSOR_PRIORITY_DATA static xmedia_s32 g_cv4003_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

#ifdef USE_PREROLL_INFO
static xmedia_u32 g_sns_info_expo; //exposure time, unit : ns
static xmedia_u32 g_sns_info_gain; //real_gain * 1024
static xmedia_u32 g_sns_info_luma;
// static unsigned int sns_info_ev; //gain * expo_line.

xmedia_u32 exp_time_clac(xmedia_u32 dev)
{
    xmedia_u32 exp_time = 0x08;
    // ns 转化成以行为单位的曝光时间 5292ns:一行的曝光时间; 5592:vmax
    if (g_sns_info_expo > 5292) {
        g_sns_info_expo = g_sns_info_expo / 5292;
        exp_time = (g_sns_info_expo > 5592) ? 0 : (5592 - g_sns_info_expo);
        exp_time = (exp_time > (5592 - 2)) ? (5592 - 2) : exp_time;
        exp_time = exp_time / 2 * 2;
        exp_time = (exp_time < 8)?  8: exp_time;
    }
    return exp_time;
}

xmedia_void gain_clac(xmedia_u32 dev, xmedia_s32 *again, xmedia_s32 *dgain)
{
    xmedia_s32 again_reg, again_real, dgain_want, dgain_reg;

    if (g_sns_info_gain <= 32768) { // 32 * 1024
        again_reg  = (262144 - 268435456 / g_sns_info_gain) / 1024; // 等价于除以1024
        again_real = 262144 / (256 - again_reg);
        dgain_want = g_sns_info_gain * 1024 / again_real;
        dgain_reg  = dgain_want * 64 / 1024;
    } else {
        again_reg  = 0xf8;
        again_real = 32768;
        dgain_want = g_sns_info_gain * 1024 / again_real;
        dgain_reg  = dgain_want  / 16;
    }

    if (dgain_reg < 64) {
        dgain_reg = 64;
    }

    *again = again_reg;
    *dgain = dgain_reg;
    SENSOR_PRINT("again %x, dgain %x \n", *again, *dgain);
    return;
}

//name :cv4003_get_internal_ae_info
//desc: get exposure time & Gain infomation of final frame.
//      在预录即将结束之前，获取到最后一张图的曝光时间和增益信息.
//timing: 1. 在预录即将结束前调用.
static xmedia_s32 cv4003_get_internal_ae_info(xmedia_u32 dev)
{
    xmedia_s32   ret        = 0;
    xmedia_uchar mode       = 0;
    xmedia_s32   intl_expo  = 0;
    xmedia_s32   intl_dgain = 0;
    xmedia_s32   intl_again = 0;
    xmedia_s32   dgain_reg  = 0;
    xmedia_s32   again_reg  = 0;
    xmedia_s32   intl_luma  = 0;
    xmedia_s64   gain_tmp   = 0;
    xmedia_u32   tmp[3];

    xmedia_s32 one_line_ns_fullmode        = 5952; //ns. //10**9/1.0/30/0x15e0
    xmedia_s32 one_line_ns_preroll_720P    = 6360; //ns. //10**9/1.0/5/0x2bc0
    xmedia_s32 fullmode_max_exposure_lines = 0x15e0; //lines.

    // 获取模式值
    // mode = cv4003_read_reg(dev, 0x347B);

    // 获取曝光值
    ret |= cv4003_read_reg(dev, 0x3040, &tmp[0]);
    ret |= cv4003_read_reg(dev, 0x3041, &tmp[1]);
    ret |= cv4003_read_reg(dev, 0x3042, &tmp[2]);
    intl_expo = (tmp[2] << 16) | (tmp[1] << 8) | tmp[0];

    // 检查读取过程中是否有错误发生
    // if (mode == -1 || tmp[0] == -1 || tmp[1] == -1 || tmp[2] == -1)
    // goto error;
    // 获取增益值
    ret |= cv4003_read_reg(dev, 0x3340, &tmp[0]); // DGain_L
    ret |= cv4003_read_reg(dev, 0x3341, &tmp[1]); // DGain_H
    ret |= cv4003_read_reg(dev, 0x3342, &tmp[2]); // AGain

    dgain_reg = (tmp[1] << 8) | tmp[0];
    again_reg = tmp[2];

    // 检查读取过程中是否有错误发生
    if (tmp[0] == -1 || tmp[1] == -1 || tmp[2] == -1)
        goto error;

    // Get luma value
    ret |= cv4003_read_reg(dev, 0x32C2, &tmp[0]);
    intl_luma = tmp[0];

    if(ret) {
        goto error;
    }

    SENSOR_PRINT("###AE-INFO##[Get]##> expo:0x%x, gain(a,d):0x%x,0x%x luma:0x%x ,mode:0x%x. \n",
                                                                    intl_expo, intl_again, intl_dgain, intl_luma, mode);
    SENSOR_PRINT("again_reg:0x%x. dgain_reg:0x%x.intl_luma:0x%x.\n", again_reg, dgain_reg, intl_luma);

    g_sns_info_expo = 0;
    g_sns_info_gain = 0;
    g_sns_info_luma = 0;

    if (again_reg == 0x00) {
        if( intl_expo * one_line_ns_preroll_720P >
            fullmode_max_exposure_lines*one_line_ns_fullmode - one_line_ns_fullmode * 10)
        {
            g_sns_info_expo = fullmode_max_exposure_lines * one_line_ns_fullmode - one_line_ns_fullmode * 10 ;
            g_sns_info_gain = intl_expo * one_line_ns_preroll_720P * 16 /
                            fullmode_max_exposure_lines * 1024 / one_line_ns_fullmode / 16; // 16 & /16 is for prection
            SENSOR_PRINT("###shaokc0..720P.g_sns_info_gain = %d\n", g_sns_info_gain);
        } else {
            g_sns_info_expo = intl_expo * one_line_ns_preroll_720P;
            g_sns_info_gain = 1024;
        }
    } else {
        g_sns_info_expo = fullmode_max_exposure_lines * one_line_ns_fullmode - one_line_ns_fullmode * 10;
        intl_again    = 256 * 1024 / (256 - again_reg);
        intl_dgain    = dgain_reg * 1024 / 0x40;
        gain_tmp      = intl_again * intl_dgain;
        //gain_tmp = gain_tmp * intl_expo * one_line_ns_preroll_720P
        //           / fullmode_max_exposure_lines / one_line_ns_fullmode; //shaokc. may has risk. pay attention!!!!
        gain_tmp = gain_tmp / fullmode_max_exposure_lines * one_line_ns_preroll_720P
                    / one_line_ns_fullmode * intl_expo ; //shaokc. may has risk. pay attention!!!!
        gain_tmp /= 1024;

        //g_sns_info_gain = (int64_t)gain_tmp; //---------------------暂时不强制转化
        g_sns_info_gain = gain_tmp;
        SENSOR_PRINT("###shaokc2...720P g_sns_info_gain = %d\n", g_sns_info_gain);
    }

    if (g_sns_info_gain > 256 * 1024) {
        g_sns_info_gain = 256 * 1024;
    }

    SENSOR_PRINT("###AE-INFO##[PPP]##> g_sns_info_expo:%d, gain:%d,a:%d,d:%d,tmp:%lld\n",
                                                        g_sns_info_expo, g_sns_info_gain,intl_again,intl_dgain,gain_tmp);

    return ret;

error:
    g_sns_info_expo = -1;
    g_sns_info_gain = -1;
    g_sns_info_luma = -1;

    return ret;

}

static xmedia_s32 g_psram_final_addr = 0;

//name :cv4003_get_preroll_psram_final_addr
//desc : read psram final address.
//      在预录即将结束之前，获取到最后一张raw图对应的PSRAM地址指针(g_psram_final_addr)
//timing: 1. call cv4003_get_internal_ae_info 获取到最新的AE信息，
//          这个信息可以供后续主控做fastAE使用，或者直接出大图的时候使用.
//        2. call stream off.(0x3000,0x01). 确保当前数据被完整写入.
//          （如果遇到数据正在写入中，则一直查询状态寄存器，确保数据被写入.）
//        3. 读取相关寄存器,换算 g_psram_final_addr
xmedia_s32 cv4003_get_preroll_psram_final_addr(xmedia_u32 dev)
{
    xmedia_s32 ret                     = 0;
    xmedia_u8  psram_final_addr_okflag = 0;
    xmedia_u32 val_sensor_sys_standby  = 0;
    xmedia_u32 val_dc;
    xmedia_u32 val_dd;
    xmedia_u32 val_de;
    xmedia_u32 val_df;
    xmedia_u32 wait_ms;

    //要停流之前，先读取曝光信息.
    // Full Mode Get AE info
    ret = cv4003_get_internal_ae_info(dev);
    SENSOR_PRINT("sizes[1]===> cv4003 Full Mode Get Internal AE Info(%d)\n", ret);
    if(ret < XMEDIA_SUCCESS) {
        return ret;
    }

    //如果遇到正在写入，等待20ms可以确保数据被写完整到PSRAM当中.
    ret |= cv4003_write_reg(dev, 0x3000, 0x01);
     //usleep(20000);
    cv4003_read_reg(dev, 0x30D8, &val_sensor_sys_standby);

    //查询当前是否处于预录正在写入中，如果正在写入，等待写入完成.最大等待30ms,查询不到也退出逻辑，避免异常卡死.
    wait_ms = 0;
    while(val_sensor_sys_standby == 0x02 && wait_ms <= 30) //1是standby，0是非standby
    {
        usleep(1*1000);
        ret |= cv4003_read_reg(dev, 0x30D8, &val_sensor_sys_standby);
        SENSOR_PRINT(" wait sensor sys standby...\n");
        wait_ms++;
    }

    if (wait_ms > 30) {
        SENSOR_PRINT(" wait sensor sys standby timeout!\n");
    }

    //获取到PSRAM的final address. 这个是预录的最后一帧的最后一个byte的地址.
    ret |= cv4003_read_reg(dev, 0x32dc, &val_dc);
    ret |= cv4003_read_reg(dev, 0x32dd, &val_dd);
    ret |= cv4003_read_reg(dev, 0x32de, &val_de);
    ret |= cv4003_read_reg(dev, 0x32df, &val_df);

    SENSOR_PRINT("psram final addr: 0x%02X%02X%02X%02X\n", val_df, val_de, val_dd,val_dc);

    wait_ms = 0;
    g_psram_final_addr = (val_df << 24) | (val_de << 16) | (val_dd << 8) | val_dc;

    while (wait_ms <= 15)
    {
      if((g_psram_final_addr + 1) % (1280 * 720) == 0)
      {
        psram_final_addr_okflag = 1;
        break;
      } else {
        psram_final_addr_okflag = 0;
        usleep(1*1000);

        //获取到PSRAM的final address. 这个是预录的最后一帧的最后一个byte的地址.
        ret |= cv4003_read_reg(dev, 0x32dc, &val_dc);
        ret |= cv4003_read_reg(dev, 0x32dd, &val_dd);
        ret |= cv4003_read_reg(dev, 0x32de, &val_de);
        ret |= cv4003_read_reg(dev, 0x32df, &val_df);

        SENSOR_PRINT("psram final addr: 0x%02X%02X%02X%02X\n", val_df, val_de, val_dd, val_dc);
        g_psram_final_addr = (val_df << 24) | (val_de << 16) | (val_dd << 8) | val_dc;
        wait_ms++;
      }
    }

    if(ret < XMEDIA_SUCCESS) {
        return ret;
    }

    if (psram_final_addr_okflag == 0) {
        SENSOR_PRINT("shaokc Warning!!!!!! psram final addr is not ok, g_psram_final_addr: 0x%08x\n", g_psram_final_addr);
    }
    return XMEDIA_SUCCESS;

}

//name :cv4003_set_psram_read_region
//desc: set psram read region.
// 设置回读的起始和结束地址.确保出图顺序是对的（
//从最久远的一张开始回读，一直到最新的一张，出完之后，就sensor 自动停止出流.）
//timing: 1. set psram readback seq (cv4003_preroll_720p_readback_psram2cis2mipi_init);
//        2. call cv4003_set_psram_read_region
//        3. stream on.
SENSOR_PRIORITY_FUNC static xmedia_s32 cv4003_set_psram_read_region(xmedia_u32 dev, xmedia_u32 final_addr)
{
    //final_addr OPI mode address is 4 byte.

    xmedia_s32 ret = 0;
    xmedia_u32 sta = final_addr + 1;
    xmedia_u32 end = final_addr;

    if (final_addr == 0x3F47fff) { //when final addr is full.(eg: 1280*720*72fps @8bit )
        sta = 0x0;
        end = 0x3F47fff;
    }

    // Set Read Region Start
    ret |= cv4003_write_reg(dev, 0x32E8, (xmedia_u8)(sta));
    ret |= cv4003_write_reg(dev, 0x32E9, (xmedia_u8)(sta >> 8));
    ret |= cv4003_write_reg(dev, 0x32EA, (xmedia_u8)(sta >> 16));
    ret |= cv4003_write_reg(dev, 0x32EB, (xmedia_u8)(sta >> 24));

    // Set Read Region End
    ret |= cv4003_write_reg(dev,  0x32EC, (xmedia_u8)(end));
    ret |= cv4003_write_reg(dev,  0x32ED, (xmedia_u8)(end >> 8));
    ret |= cv4003_write_reg(dev,  0x32EE, (xmedia_u8)(end >> 16));
    ret |= cv4003_write_reg(dev,  0x32EF, (xmedia_u8)(end >> 24));

    return ret;
}

xmedia_s32 cv4003_preroll_720p_write_cis2psram_init(xmedia_u32 dev)
{
    xmedia_u32 ret = XMEDIA_SUCCESS;

    ret |= cv4003_write_reg(dev, 0x3010, 0x01);
    ret |= cv4003_write_reg(dev, 0x3011, 0x01);
    ret |= cv4003_write_reg(dev, 0x3017, 0x32);
    ret |= cv4003_write_reg(dev, 0x301C, 0x2A);  // [19:0]  FRAME_LENGTH //3fps
    ret |= cv4003_write_reg(dev, 0x301D, 0xE5);
    ret |= cv4003_write_reg(dev, 0x301E, 0x00);
    ret |= cv4003_write_reg(dev, 0x3020, 0x2C);  // [15:0]  LINE_LENGTH = 012C(dec:300)
    ret |= cv4003_write_reg(dev, 0x3021, 0x01);
    ret |= cv4003_write_reg(dev, 0x3068, 0x61);
    ret |= cv4003_write_reg(dev, 0x302C, 0x01);
    ret |= cv4003_write_reg(dev, 0x30D0, 0x01);
    ret |= cv4003_write_reg(dev, 0x30D1, 0x6E);
    ret |= cv4003_write_reg(dev, 0x30D2, 0x6E);
    ret |= cv4003_write_reg(dev, 0x30D3, 0x50);
    ret |= cv4003_write_reg(dev, 0x30D4, 0x02);
    ret |= cv4003_write_reg(dev, 0x30D5, 0x08);
    ret |= cv4003_write_reg(dev, 0x3139, 0x01);
    ret |= cv4003_write_reg(dev, 0x3151, 0x01);
    ret |= cv4003_write_reg(dev, 0x3206, 0x01);
    ret |= cv4003_write_reg(dev, 0x3218, 0x01);
    ret |= cv4003_write_reg(dev, 0x32D2, 0x04);  // [10:0]  PS_WR_ADDR_OFST = 004(dec:4)
    ret |= cv4003_write_reg(dev, 0x32D3, 0x00);
    ret |= cv4003_write_reg(dev, 0x330C, 0x90);
    ret |= cv4003_write_reg(dev, 0x330D, 0x00);
    ret |= cv4003_write_reg(dev, 0x330E, 0x80);
    ret |= cv4003_write_reg(dev, 0x330F, 0x04);
    ret |= cv4003_write_reg(dev, 0x3310, 0x49);
    ret |= cv4003_write_reg(dev, 0x3311, 0x00);
    ret |= cv4003_write_reg(dev, 0x3312, 0x46);
    ret |= cv4003_write_reg(dev, 0x3313, 0x02);
    ret |= cv4003_write_reg(dev, 0x335C, 0x03);
    ret |= cv4003_write_reg(dev, 0x335D, 0x03);
    ret |= cv4003_write_reg(dev, 0x337C, 0x08);  // [10:0]  BLK_STAX    = 008(dec:08)
    ret |= cv4003_write_reg(dev, 0x337D, 0x00);
    ret |= cv4003_write_reg(dev, 0x337E, 0x16);  // [5:0]   UNIT_SIZEX  = 16(dec:22)
    ret |= cv4003_write_reg(dev, 0x3380, 0x08);  // [11:0]  BLK_STAY    = 008(dec:8)
    ret |= cv4003_write_reg(dev, 0x3381, 0x00);
    ret |= cv4003_write_reg(dev, 0x3382, 0x58);  // [7:0]   UNIT_SIZEY  = 58(dec:88)
    ret |= cv4003_write_reg(dev, 0x3524, 0x05);
    ret |= cv4003_write_reg(dev, 0x351F, 0x19);
    ret |= cv4003_write_reg(dev, 0x3527, 0x05);
    ret |= cv4003_write_reg(dev, 0x3522, 0x08);
    ret |= cv4003_write_reg(dev, 0x3523, 0x08);
    ret |= cv4003_write_reg(dev, 0x3525, 0x10);
    ret |= cv4003_write_reg(dev, 0x3526, 0x1B);
    ret |= cv4003_write_reg(dev, 0x350D, 0x7F);
    ret |= cv4003_write_reg(dev, 0x362E, 0xAA);
    ret |= cv4003_write_reg(dev, 0x36B6, 0xAA);
    ret |= cv4003_write_reg(dev, 0x366E, 0x01);
    ret |= cv4003_write_reg(dev, 0x3703, 0x03);
    ret |= cv4003_write_reg(dev, 0x3808, 0xB0);  // [7:0]   PLL_MUL_NUM_POST  = 8C(dec:140)
    ret |= cv4003_write_reg(dev, 0x380A, 0x01);  // [3:0]   PLL_DIV_NUM_POST  = 1(dec:1)
    ret |= cv4003_write_reg(dev, 0x3814, 0x02);
    ret |= cv4003_write_reg(dev, 0x3815, 0x00);
    ret |= cv4003_write_reg(dev, 0x3827, 0x00);
    ret |= cv4003_write_reg(dev, 0x394A, 0x06);
    ret |= cv4003_write_reg(dev, 0x394B, 0x00);
    ret |= cv4003_write_reg(dev, 0x3953, 0x10);
    ret |= cv4003_write_reg(dev, 0x3953, 0x30);
    ret |= cv4003_write_reg(dev, 0x32D8, 0x0B);  // [27:0]  PS_WR_ADDR_MAX 128Mbit容量最多存放Raw10数据14帧
    ret |= cv4003_write_reg(dev, 0x32D9, 0x0F);
    ret |= cv4003_write_reg(dev, 0x32DA, 0xF7);
    ret |= cv4003_write_reg(dev, 0x32DB, 0x00);
    ret |= cv4003_write_reg(dev, 0x3034, 0x04);
    ret |= cv4003_write_reg(dev, 0x3035, 0x00);
    ret |= cv4003_write_reg(dev, 0x3036, 0xD0);
    ret |= cv4003_write_reg(dev, 0x3037, 0x02);
    ret |= cv4003_write_reg(dev, 0x3038, 0x50);
    ret |= cv4003_write_reg(dev, 0x3039, 0x00);
    ret |= cv4003_write_reg(dev, 0x303A, 0x00);
    ret |= cv4003_write_reg(dev, 0x303B, 0x05);
    ret |= cv4003_write_reg(dev, 0x3308, 0xB0);//AE_TARGET
    ret |= cv4003_write_reg(dev, 0x3309, 0x00);
    ret |= cv4003_write_reg(dev, 0x3204, 0x40);
    ret |= cv4003_write_reg(dev, 0x3673, 0x00);
    ret |= cv4003_write_reg(dev, 0x385A, 0x03);
    ret |= cv4003_write_reg(dev, 0x38AD, 0x11);
    ret |= cv4003_write_reg(dev, 0x3638, 0x02);
    ret |= cv4003_write_reg(dev, 0x3639, 0x00);
    ret |= cv4003_write_reg(dev, 0x3628, 0x64);
    ret |= cv4003_write_reg(dev, 0x3629, 0x00);
    ret |= cv4003_write_reg(dev, 0x369C, 0x64);
    ret |= cv4003_write_reg(dev, 0x369D, 0x00);
    ret |= cv4003_write_reg(dev, 0x360C, 0x01);
    ret |= cv4003_write_reg(dev, 0x360E, 0x01);
    ret |= cv4003_write_reg(dev, 0x3000, 0x00);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "cv4003_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("=============PREROLL CVSENS CV4003_720P_3FPS_10BIT_LINEAR_Init_OK! ==============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}
#endif

SENSOR_PRIORITY_FUNC static xmedia_s32 cv4003_2lane_linear_2560x1440_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    //reset
    ret |= cv4003_write_reg(dev, 0x3004,0x01);
    ret |= cv4003_write_reg(dev, 0x3004,0x00);
    // ret |= cv4003_write_reg(dev, 0x3012,0xF0);
    ret |= cv4003_write_reg(dev, 0x300D,0x01);
    ret |= cv4003_write_reg(dev, 0x300E,0x00);
    ret |= cv4003_write_reg(dev, 0x3204,0x40);
    ret |= cv4003_write_reg(dev, 0x3478,0x00);
    ret |= cv4003_write_reg(dev, 0x385A,0x03);
    ret |= cv4003_write_reg(dev, 0x310C,0x64);
    ret |= cv4003_write_reg(dev, 0x3048,0x08);
    ret |= cv4003_write_reg(dev, 0x3134,0x01);
    ret |= cv4003_write_reg(dev, 0x326c,0xdc);
    ret |= cv4003_write_reg(dev, 0x326e,0x24);
    ret |= cv4003_write_reg(dev, 0x3154,0x11);
    ret |= cv4003_write_reg(dev, 0x31BC,0x40);
    ret |= cv4003_write_reg(dev, 0x31D5,0x03);
    ret |= cv4003_write_reg(dev, 0x31D6,0x04);
    ret |= cv4003_write_reg(dev, 0x3164,0x09);
    ret |= cv4003_write_reg(dev, 0x351F,0x32);
    ret |= cv4003_write_reg(dev, 0x3527,0x05);
    ret |= cv4003_write_reg(dev, 0x38AD,0x10);
    ret |= cv4003_write_reg(dev, 0x3638,0x02);
    ret |= cv4003_write_reg(dev, 0x3639,0x00);
    ret |= cv4003_write_reg(dev, 0x301c,0xe0);
    ret |= cv4003_write_reg(dev, 0x301d,0x15);
    ret |= cv4003_write_reg(dev, 0x3020,0x89);
    ret |= cv4003_write_reg(dev, 0x3021,0x01);
    ret |= cv4003_write_reg(dev, 0x3048,0x0A);

    ////CVSENS Sensor Window setting.
    //full_width = 2888
    //full_height = 1628
    //active_width = 2560
    //active_height = 1440

#ifdef USE_PREROLL_INFO
    xmedia_u32 exp_time = 0x08;
    xmedia_s32 again    = 0x00;
    xmedia_s32 dgain    = 0x40;

    exp_time = exp_time_clac(dev);
    gain_clac(dev,  &again, &dgain);
    // 配置根据小图换成出大图的曝光时间 again dgain
    ret |= cv4003_write_reg(dev, 0x3048, exp_time & 0xFF);
    ret |= cv4003_write_reg(dev, 0x3049, (exp_time & 0x00FF00) >> 8);

    ret |= cv4003_write_reg(dev, 0x3118, again & 0xff);  //Again
    ret |= cv4003_write_reg(dev, 0x311c, dgain & 0xff);  //Dgain Low
    ret |= cv4003_write_reg(dev, 0x311d, (dgain & 0xff00) >> 8);  //Dgain High
#endif

    ret |= cv4003_write_reg(dev, 0x3014,0x04); //WCROP_MODE
    ret |= cv4003_write_reg(dev, 0x303C,0x56); //Y_WCROP_STA_L
    ret |= cv4003_write_reg(dev, 0x303D,0x00); //Y_WCROP_STA_H
    ret |= cv4003_write_reg(dev, 0x303E,0xb0); //Y_WCROP_HEIGHT_L
    ret |= cv4003_write_reg(dev, 0x303F,0x05); //Y_WCROP_HEIGHT_H
    ret |= cv4003_write_reg(dev, 0x3030,0x01); //DCROP_MODE
    ret |= cv4003_write_reg(dev, 0x3038,0xa4); //X_CROP_STA_L
    ret |= cv4003_write_reg(dev, 0x3039,0x00); //X_CROP_STA_H
    ret |= cv4003_write_reg(dev, 0x303A,0x00); //X_CROP_WIDTH_L
    ret |= cv4003_write_reg(dev, 0x303B,0x0a); //X_CROP_WIDTH_H
    ret |= cv4003_write_reg(dev, 0x3034,0x08); //Y_DCROP_STA_L
    ret |= cv4003_write_reg(dev, 0x3035,0x00); //Y_DCROP_STA_H
    ret |= cv4003_write_reg(dev, 0x3036,0xa0); //Y_DCROP_HEIGHT_L
    ret |= cv4003_write_reg(dev, 0x3037,0x05); //Y_DCROP_HEIGHT_H

    //split gain Enable.
    ret |= cv4003_write_reg(dev, 0x3109,0x01); //Use Again * Dgain.
    ret |= cv4003_write_reg(dev, 0x3000,0x00); //Streaming

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "cv4003_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("=== CV4003_24MInput_MIPI_2lane_10bit_2560x1440_30fps init success!===============\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC static xmedia_s32 cv4003_preroll_720p_readback_psram2cis2mipi_init(xmedia_u32 dev)
{
     xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= cv4003_write_reg(dev, 0x3010, 0x01);
    ret |= cv4003_write_reg(dev, 0x3011, 0x01);
    ret |= cv4003_write_reg(dev, 0x3017, 0x32);
    ret |= cv4003_write_reg(dev, 0x301C, 0x08);  // [19:0]  FRAME_LENGTH        = 00708(dec:1800)
    ret |= cv4003_write_reg(dev, 0x301D, 0x07);
    ret |= cv4003_write_reg(dev, 0x301E, 0x00);
    ret |= cv4003_write_reg(dev, 0x3020, 0xF3);  // [15:0]  LINE_LENGTH         = 00F3(dec:243)
    ret |= cv4003_write_reg(dev, 0x3021, 0x00);
    ret |= cv4003_write_reg(dev, 0x302C, 0x01);
    ret |= cv4003_write_reg(dev, 0x3034, 0x04);
    ret |= cv4003_write_reg(dev, 0x3035, 0x00);
    ret |= cv4003_write_reg(dev, 0x3036, 0xD0);
    ret |= cv4003_write_reg(dev, 0x3037, 0x02);
    ret |= cv4003_write_reg(dev, 0x3038, 0x50);
    ret |= cv4003_write_reg(dev, 0x3039, 0x00);
    ret |= cv4003_write_reg(dev, 0x303A, 0x00);
    ret |= cv4003_write_reg(dev, 0x303B, 0x05);
    ret |= cv4003_write_reg(dev, 0x305C, 0x01);
    ret |= cv4003_write_reg(dev, 0x3218, 0x01);
    ret |= cv4003_write_reg(dev, 0x32D2, 0x04);  // [10:0]  PS_WR_ADDR_OFST     = 004(dec:4)
    ret |= cv4003_write_reg(dev, 0x32D3, 0x00);
    ret |= cv4003_write_reg(dev, 0x32E5, 0x03);
    ret |= cv4003_write_reg(dev, 0x32F0, 0xD0);
    ret |= cv4003_write_reg(dev, 0x32F1, 0x02);
    ret |= cv4003_write_reg(dev, 0x32F2, 0x00);
    ret |= cv4003_write_reg(dev, 0x32F3, 0x05);
    ret |= cv4003_write_reg(dev, 0x3418, 0x77);
    ret |= cv4003_write_reg(dev, 0x3419, 0x00);
    ret |= cv4003_write_reg(dev, 0x341A, 0x37);
    ret |= cv4003_write_reg(dev, 0x341B, 0x00);
    ret |= cv4003_write_reg(dev, 0x341C, 0x37);
    ret |= cv4003_write_reg(dev, 0x341D, 0x00);
    ret |= cv4003_write_reg(dev, 0x341E, 0xE7);
    ret |= cv4003_write_reg(dev, 0x341F, 0x00);
    ret |= cv4003_write_reg(dev, 0x3420, 0x37);
    ret |= cv4003_write_reg(dev, 0x3421, 0x00);
    ret |= cv4003_write_reg(dev, 0x3422, 0x9F);
    ret |= cv4003_write_reg(dev, 0x3423, 0x00);
    ret |= cv4003_write_reg(dev, 0x3424, 0x37);
    ret |= cv4003_write_reg(dev, 0x3425, 0x00);
    ret |= cv4003_write_reg(dev, 0x3426, 0x57);
    ret |= cv4003_write_reg(dev, 0x3427, 0x00);
    ret |= cv4003_write_reg(dev, 0x3428, 0x2F);
    ret |= cv4003_write_reg(dev, 0x3429, 0x00);
    ret |= cv4003_write_reg(dev, 0x3808, 0x8c);  // [7:0]   PLL_MUL_NUM_POST    = 8C(dec:140)
    ret |= cv4003_write_reg(dev, 0x380A, 0x01);  // [3:0]   PLL_DIV_NUM_POST    = 1(dec:1)
    ret |= cv4003_write_reg(dev, 0x3814, 0x02);
    ret |= cv4003_write_reg(dev, 0x3815, 0x00);
    ret |= cv4003_write_reg(dev, 0x3827, 0x00);
    ret |= cv4003_write_reg(dev, 0x3829, 0x00);
    ret |= cv4003_write_reg(dev, 0x394A, 0x06);
    ret |= cv4003_write_reg(dev, 0x394B, 0x00);
    ret |= cv4003_write_reg(dev, 0x3953, 0x10);
    ret |= cv4003_write_reg(dev, 0x3953, 0x30);
    ret |= cv4003_write_reg(dev, 0x32D8, 0x0B);  // [27:0]  PS_WR_ADDR_MAX //128Mbit容量，Max存14frame
    ret |= cv4003_write_reg(dev, 0x32D9, 0x0F);
    ret |= cv4003_write_reg(dev, 0x32DA, 0xF7);
    ret |= cv4003_write_reg(dev, 0x32DB, 0x00);
    ret |= cv4003_write_reg(dev, 0x32E8, 0x00);  // [27:0]  PS_RD_ADDR_STA
    ret |= cv4003_write_reg(dev, 0x32E9, 0x00);
    ret |= cv4003_write_reg(dev, 0x32EA, 0x00);
    ret |= cv4003_write_reg(dev, 0x32EB, 0x00);
    ret |= cv4003_write_reg(dev, 0x32EC, 0x0B);  // [27:0]  PS_RD_ADDR_END
    ret |= cv4003_write_reg(dev, 0x32ED, 0x0F);
    ret |= cv4003_write_reg(dev, 0x32EE, 0xF8);
    ret |= cv4003_write_reg(dev, 0x32EF, 0x00);
    ret |= cv4003_write_reg(dev, 0x3204, 0x40);
#ifdef USE_PREROLL_INFO
    ret |= cv4003_set_psram_read_region(dev, g_psram_final_addr);
#endif
    ret |= cv4003_write_reg(dev, 0x3000,0x00); //Streaming

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "cv4003_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("==============================================================\n");
        SENSOR_PRINT("===READ BACK CVSENS CV4003_720P_120FPS_10BIT_LINEAR_Init_OK!==\n");
        SENSOR_PRINT("==============================================================\n");
        return XMEDIA_SUCCESS;
    }
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv4003_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case CV4003_4M_LINEAR_MODE:
#ifdef USE_PREROLL_INFO
            ret = cv4003_get_preroll_psram_final_addr(dev);
#endif
            ret = cv4003_2lane_linear_2560x1440_init(dev);
            break;
        case CV4003_1M_LINEAR_MODE:
            ret = cv4003_preroll_720p_readback_psram2cis2mipi_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }
    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv4003_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;

    if (g_cv4003_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_cv4003_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_cv4003_i2c_fd[dev]);
        g_cv4003_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_cv4003_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv4003_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_cv4003_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_cv4003_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_cv4003_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = cv4003_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv4003_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_cv4003_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_cv4003_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv4003_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[CV4003_DATA_BYTE];

    if (g_cv4003_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_cv4003_i2c_fd[dev], g_cv4003_i2c_addr[dev], addr, CV4003_ADDR_BYTE, buf, CV4003_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //CV4003_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv4003_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_cv4003_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_cv4003_i2c_fd[dev], g_cv4003_i2c_addr[dev], buf, CV4003_ADDR_BYTE + CV4003_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv4003_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u32 mirror_flip_type;

    cv4003_read_reg(dev, CV4003_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

    if (mirror_en){
        mirror_flip_type |= 0x01;
    } else {
        mirror_flip_type &= 0xFE;
    }

    if (flip_en){
        mirror_flip_type |= 0x02;
    } else {
        mirror_flip_type &= 0xFD;
    }

    ret  = cv4003_write_reg(dev, CV4003_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "cv4003_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}
