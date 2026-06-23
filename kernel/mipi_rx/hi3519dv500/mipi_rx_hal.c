/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_osal.h"
#include "type.h"
#include "ot_mipi_rx.h"
#include "mipi_rx_reg.h"
#include "sys_ext.h"
#include "mipi_rx_hal.h"

typedef struct {
    unsigned int phy_rg_ext_en;
    unsigned int phy_rg_ext2_en;
    unsigned int phy_rg_int_en;
    unsigned int phy_rg_drveclk2_enz;
    unsigned int phy_rg_drveclk_enz;
} phy_mode_link_t;

typedef enum {
    MIPI_ESC_D0 = 0x1 << 0,
    MIPI_ESC_D1 = 0x1 << 1,
    MIPI_ESC_D2 = 0x1 << 2,
    MIPI_ESC_D3 = 0x1 << 3,
    MIPI_ESC_CLK = 0x1 << 4,
    MIPI_ESC_CLK2 = 0x1 << 5,

    MIPI_TIMEOUT_D0 = 0x1 << 8,
    MIPI_TIMEOUT_D1 = 0x1 << 9,
    MIPI_TIMEOUT_D2 = 0x1 << 10,
    MIPI_TIMEOUT_D3 = 0x1 << 11,
    MIPI_TIMEOUT_CLK = 0x1 << 12,
    MIPI_TIMEOUT_CLK2 = 0x1 << 13,
} phy_err_int_state;

typedef enum {
    MIPI_VC0_MISMATCH = 0x1 << 20, /* VC0,frame's start and frame's end do not match */
    MIPI_VC1_MISMATCH = 0x1 << 21, /* VC1,frame's start and frame's end do not match */
    MIPI_VC2_MISMATCH = 0x1 << 22, /* VC2,frame's start and frame's end do not match */
    MIPI_VC3_MISMATCH = 0x1 << 23, /* VC3,frame's start and frame's end do not match */

    MIPI_VC0_FRAME_NUM_ERR = 0x1 << 16, /* VC0'S frame order error */
    MIPI_VC1_FRAME_NUM_ERR = 0x1 << 17, /* VC1'S frame order error */
    MIPI_VC2_FRAME_NUM_ERR = 0x1 << 18, /* VC2'S frame order error */
    MIPI_VC3_FRAME_NUM_ERR = 0x1 << 19, /* VC3'S frame order error */

    MIPI_VC0_ECC_ERR = 0x1 << 4, /* VC0'S header has errors,and ECC error correction is ok */
    MIPI_VC1_ECC_ERR = 0x1 << 5,
    MIPI_VC2_ECC_ERR = 0x1 << 6,
    MIPI_VC3_ECC_ERR = 0x1 << 7,
    MIPI_MULT_ECC_ERR = 0x1 << 8, /* Header has two error at least ,and ECC error correction is invalid */

    MIPI_VC0_CRC_ERR = 0x1 << 0, /* in the last frame, VC0'S data has a CRC ERROR at least */
    MIPI_VC1_CRC_ERR = 0x1 << 1, /* in the last frame, VC1'S data has a CRC ERROR at least */
    MIPI_VC2_CRC_ERR = 0x1 << 2, /* in the last frame, VC2'S data has a CRC ERROR at least */
    MIPI_VC3_CRC_ERR = 0x1 << 3, /* in the last frame, VC3'S data has a CRC ERROR at least */
} mipi_csi_int_state;

typedef enum {
    CMD_FIFO_WRITE_ERR = 0x1 << 0, /* MIPI_CTRL write command FIFO error */
    DATA_FIFO_WRITE_ERR = 0x1 << 1,
    VSYNC_RAW          = 0x1 << 4,
    CMD_FIFO_READ_ERR = 0x1 << 16,
    DATA_FIFO_READ_ERR = 0x1 << 17,
} mipi_ctrl_int_state;

typedef enum {
    LANE0_SYNC_ERR = 0x1 << 0,
    LANE1_SYNC_ERR = 0x1 << 1,
    LANE2_SYNC_ERR = 0x1 << 2,
    LANE3_SYNC_ERR = 0x1 << 3,
    LANE4_SYNC_ERR = 0x1 << 4,
    LANE5_SYNC_ERR = 0x1 << 5,
    LANE6_SYNC_ERR = 0x1 << 6,
    LANE7_SYNC_ERR = 0x1 << 7,

    LINK0_WRITE_ERR = 0x1 << 16,
    LINK1_WRITE_ERR = 0x1 << 17,
    LINK2_WRITE_ERR = 0x1 << 18,
    LINK0_READ_ERR = 0x1 << 20,
    LINK1_READ_ERR = 0x1 << 21,
    LINK2_READ_ERR = 0x1 << 22,
    LVDS_STAT_ERR = 0x1 << 24,
    LVDS_POP_ERR = 0x1 << 25,
    CMD_WR_ERR = 0x1 << 26,
    CMD_RD_ERR = 0x1 << 27,
    LVDS_VSYNC = 0x1 << 28,
} lvds_int_state;

typedef enum {
    ALIGN_FIFO_FULL_ERR = 0x1 << 0,
    ALIGN_LANE0_ERR = 0x1 << 1,
    ALIGN_LANE1_ERR = 0x1 << 2,
    ALIGN_LANE2_ERR = 0x1 << 3,
    ALIGN_LANE3_ERR = 0x1 << 4,
    ALIGN_LANE4_ERR = 0x1 << 5,
    ALIGN_LANE5_ERR = 0x1 << 6,
    ALIGN_LANE6_ERR = 0x1 << 7,
    ALIGN_LANE7_ERR = 0x1 << 8,
} align_int_state;

/* macro definition */
#define MIPI_RX_REGS_ADDR               0x173C0000
#define MIPI_RX_REGS_SIZE               0x10000
#define MIPI_RX_REG_BYTE_WIDTH          4

#define MIPI_RX_CLK_CRG_ADDR            0x11018540

static unsigned long sns_crg_addr(sns_clk_source_t sns_source)
{
    return (0x11018440 + (sns_source) * 0x20);
}

static unsigned long mipi_rx_pixel_crg_addr(combo_dev_t deno)
{
    return (0x11018560 + (deno) * 0x20);
}

static unsigned long g_sns_rst_crg_addr = 0x17C50004;

#define MIPI_RX_WORK_MODE_ADDR          0x17A40300

#define MIPI_RX_IRQ 160
#define MIPI_CIL_FREQ 150 /* MIPI CIL FREQ 150MHZ */

static unsigned int g_reg_map_flag = 0;

static bool is_valid_id(short id)
{
    return ((id) != -1);
}

static bool is_data_type_yuv(data_type_t input_data_type)
{
    return (((input_data_type) >= DATA_TYPE_YUV420_8BIT_NORMAL) && ((input_data_type) <= DATA_TYPE_YUV422_PACKED));
}

#define PHY_SKEW_LINK       0x00f0ff00
#define PHY_LANE_SI_LINK    0xffffff1f
#define PHY_EQ_LINK         0x00444444
#define PHY_CFG_LINK        0x0000ffff
#define PHY_DESKEW_CAL      0xff0f03ff
#define PHY_FSMO_VALUE      0x000d1d0c
#define PHY_EXT_REG0_VALUE  0x12492000
#define PHY_EXT_REG1_VALUE  0x04000000
#define PHY_TEST_VALUE      0x00000004
#define PHY_TEST_VALUE_EXT  0x00000040
#define PHY_DSW_VALUE       0x1f1f1f1f
#define PHY_OFFNN_VALUE     0x00ffffff
#define PHY_TEMRL_VALUE     0x00999999

#define mipi_rx_readl(x)        (*(const volatile unsigned int *)(x))
#define mipi_rx_writel(v, x)    (*(volatile unsigned int *)(x) = (v))

#define EXT_CFG_VALUE_UNINIT ((unsigned int)(-1))
unsigned int g_ext_cfg = EXT_CFG_VALUE_UNINIT;

/* global variables definition */
mipi_rx_regs_type_t *g_mipi_rx_regs_va = NULL;

unsigned int g_mipi_rx_irq_num = MIPI_RX_IRQ;

static const phy_mode_link_t g_phy_mode[][MIPI_RX_MAX_PHY_NUM] = {
#ifndef OT_FPGA
    {{ 1, 0, 0, 1, 0 }, { 1, 0, 0, 1, 1 }},
    {{ 0, 1, 0, 0, 1 }, { 0, 1, 0, 0, 1 }},
#else
    {{ 0, 0, 1, 0, 1 }, { 0, 0, 0, 1, 1 }},
    {{ 0, 0, 1, 0, 1 }, { 0, 0, 1, 0, 1 }},
#endif
    {{ 0, 1, 0, 0, 1 }, { 0, 0, 1, 1, 1 }},
    {{ 0, 0, 1, 1, 1 }, { 0, 0, 1, 1, 1 }},
    {{ 0, 0, 1, 1, 1 }, { 0, 1, 0, 0, 1 }},
};

static const unsigned int g_hs_mode[] = {0x0, 0x7, 0xA, 0xB, 0xC};

static phy_err_int_cnt_t g_phy_err_int_cnt[MIPI_RX_MAX_PHY_NUM];
static mipi_err_int_cnt_t g_mipi_err_int_cnt[MIPI_RX_MAX_DEV_NUM];
static lvds_err_int_cnt_t g_lvds_err_int_cnt[MIPI_RX_MAX_DEV_NUM];
static align_err_int_cnt_t g_align_err_int_cnt[MIPI_RX_MAX_DEV_NUM];
static bool g_has_cil_int[MIPI_RX_MAX_PHY_NUM];

typedef struct {
    unsigned int phy_id;
    unsigned int lane_bitmap;
    unsigned int phy_lane_bitmap;
    phy_speed_mode_t speed_mode;
    unsigned int vsync_cnt;
    unsigned int recovery_cnt;
} dev_phy_info_t;

static dev_phy_info_t g_dev_phy_info[MIPI_RX_MAX_DEV_NUM];
static bool g_phy_single_lane_mode[MIPI_RX_MAX_PHY_NUM];

/* function definition */

void set_drv_sns_crg_addr(unsigned long addr)
{
    g_sns_rst_crg_addr = addr;
}

void mipi_rx_drv_set_irq_num(unsigned int irq_num)
{
    g_mipi_rx_irq_num = irq_num;
}

void mipi_rx_drv_set_regs(mipi_rx_regs_type_t *regs)
{
    g_mipi_rx_regs_va = regs;
}

phy_err_int_cnt_t *mipi_rx_drv_get_phy_err_int_cnt(unsigned int phy_id)
{
    return &g_phy_err_int_cnt[phy_id];
}

mipi_err_int_cnt_t *mipi_rx_drv_get_mipi_err_int_cnt(combo_dev_t devno)
{
    return &g_mipi_err_int_cnt[devno];
}

lvds_err_int_cnt_t *mipi_rx_drv_get_lvds_err_int_cnt(combo_dev_t devno)
{
    return &g_lvds_err_int_cnt[devno];
}

align_err_int_cnt_t *mipi_rx_drv_get_align_err_int_cnt(combo_dev_t devno)
{
    return &g_align_err_int_cnt[devno];
}

unsigned int mipi_rx_drv_get_vsync_cnt(combo_dev_t devno)
{
    return g_dev_phy_info[devno].vsync_cnt;
}

static void mipi_rx_set_bit(unsigned long value, unsigned long offset,
                    unsigned long addr)
{
    unsigned long t, mask;

    mask = 1 << offset;
    t = mipi_rx_readl((const volatile void *)(uintptr_t)addr);
    t &= ~mask;
    t |= (value << offset) & mask;
    mipi_rx_writel(t, (volatile void *)(uintptr_t)addr);
}

static void write_reg32(unsigned long addr,
                        unsigned int value,
                        unsigned int mask)
{
    unsigned int t;

    t = mipi_rx_readl((const volatile void *)(uintptr_t)addr);
    t &= ~mask;
    t |= value & mask;
    mipi_rx_writel(t, (volatile void *)(uintptr_t)addr);
}

static mipi_rx_phy_cfg_t *get_mipi_rx_phy_regs(int phy_id)
{
    return &g_mipi_rx_regs_va->mipi_rx_phy_cfg[phy_id];
}

static mipi_rx_sys_regs_t *get_mipi_rx_sys_regs(void)
{
    return &g_mipi_rx_regs_va->mipi_rx_sys_regs;
}

static mipi_ctrl_regs_t *get_mipi_ctrl_regs(combo_dev_t devno)
{
    return &g_mipi_rx_regs_va->mipi_rx_ctrl_regs[devno].mipi_ctrl_regs;
}

static lvds_ctrl_regs_t *get_lvds_ctrl_regs(combo_dev_t devno)
{
    return &g_mipi_rx_regs_va->mipi_rx_ctrl_regs[devno].lvds_ctrl_regs;
}

static global_ctrl_regs_t *get_global_ctrl_regs(combo_dev_t devno)
{
    return &g_mipi_rx_regs_va->mipi_rx_ctrl_regs[devno].global_ctrl_regs;
}

bool mipi_rx_chip_is_hi3516dv500(void)
{
    unsigned int chip_id;

    if (!ckfn_sys_get_chip_id()) {
        chip_id = 0;
    } else {
        chip_id = call_sys_get_chip_id();
    }

    return (chip_id == 0x3516D500 || chip_id == 0x23A15); // chip id 0x3516D500, 0x23A15
}

bool mipi_rx_check_ext_cfg(void)
{
    if (g_ext_cfg == EXT_CFG_VALUE_UNINIT) {
        if (!ckfn_sys_get_mipirx_ext_cfg()) {
            g_ext_cfg = 0;
        } else {
            g_ext_cfg = call_sys_get_mipirx_ext_cfg();
        }
    }

    return (g_ext_cfg == 1);
}

#ifndef OT_FPGA
static void mipi_rx_set_cil_int_mask(int phy_id, unsigned int mask)
{
    u_mipi_int_msk mipi_int_msk;
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    volatile mipi_rx_sys_regs_t *mipi_rx_sys_regs = get_mipi_rx_sys_regs();

    mipi_int_msk.u32 = mipi_rx_sys_regs->mipi_int_msk.u32;

    if (phy_id == 0) {
        mipi_int_msk.bits.int_phycil0_mask = 0x0;
    } else if (phy_id == 1) {
        mipi_int_msk.bits.int_phycil1_mask = 0x0;
    } else if (phy_id == 2) { /* 2 : phy2 */
        mipi_int_msk.bits.int_phycil2_mask = 0x0;
    }
    mipi_rx_sys_regs->mipi_int_msk.u32 = mipi_int_msk.u32;

    mipi_rx_phy_cfg->mipi_cil_int_msk_link.u32 = mask;
}

static void mipi_rx_set_phy_skew_link(int phy_id, unsigned int value)
{
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    mipi_rx_phy_cfg->phy_skew_link.u32 = value;
}

static void mipi_rx_set_phy_lane_si_link(int phy_id, unsigned int value)
{
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    mipi_rx_phy_cfg->phy_lane_si_link.u32 = value;
}

static void mipi_rx_set_phy_eq_link(int phy_id, unsigned int value)
{
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    mipi_rx_phy_cfg->phy_eq_link.u32 = value;
}

static void mipi_rx_set_phy_deskew_cal_link(int phy_id, unsigned int value)
{
    volatile u_phy_deskew_cal_link phy_deskew_cal_link;
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_deskew_cal_link.u32 = value;
    mipi_rx_phy_cfg->phy_deskew_cal_link.u32 = phy_deskew_cal_link.u32;
}

static void mipi_rx_set_phy_fsmo_link(int phy_id, unsigned int value)
{
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    mipi_rx_phy_cfg->cil_fsm0_link.u32 = value;
}

static void mipi_rx_set_phy_ext_reg0(int phy_id, unsigned int value)
{
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    mipi_rx_phy_cfg->phy_ext_reg0.u32 = value;
}

static void mipi_rx_set_phy_ext_reg1(int phy_id, unsigned int value)
{
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    mipi_rx_phy_cfg->phy_ext_reg1.u32 = value;
}

static void mipi_rx_set_phy_test(int phy_id, unsigned int value)
{
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    mipi_rx_phy_cfg->phy_test.u32 = value;
}

static void mipi_rx_set_phy_dsw(int phy_id, unsigned int value)
{
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    mipi_rx_phy_cfg->phy_dsw.u32 = value;
}

static void mipi_rx_set_phy_temrl(int phy_id, unsigned int value)
{
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    mipi_rx_phy_cfg->phy_temrl.u32 = value;
}
#endif

static void mipi_rx_set_phy_cfg_link(int phy_id, unsigned int value)
{
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    mipi_rx_phy_cfg->phy_cfg_link.u32 = value;
}

static void mipi_rx_set_phy_offnn(int phy_id, unsigned int value)
{
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    mipi_rx_phy_cfg->phy_offnn.u32 = value;
}

static void mipi_rx_set_phy_cal_int(unsigned int value)
{
    volatile mipi_rx_sys_regs_t *mipi_rx_sys_regs = get_mipi_rx_sys_regs();

    mipi_rx_sys_regs->phy_cal_int.u32 = value;
}

static void mipi_rx_set_phy_rg_ext_en(int phy_id, int enable)
{
    volatile u_phy_mode_link phy_mode_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_mode_link.u32 = mipi_rx_phy_cfg->phy_mode_link.u32;
    phy_mode_link.bits.phy_rg_ext_en = enable;
    mipi_rx_phy_cfg->phy_mode_link.u32 = phy_mode_link.u32;
}

static void mipi_rx_set_phy_rg_ext2_en(int phy_id, int enable)
{
    volatile u_phy_mode_link phy_mode_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_mode_link.u32 = mipi_rx_phy_cfg->phy_mode_link.u32;
    phy_mode_link.bits.phy_rg_ext2_en = enable;
    mipi_rx_phy_cfg->phy_mode_link.u32 = phy_mode_link.u32;
}

static void mipi_rx_set_phy_rg_int_en(int phy_id, int enable)
{
    volatile u_phy_mode_link phy_mode_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_mode_link.u32 = mipi_rx_phy_cfg->phy_mode_link.u32;
    phy_mode_link.bits.phy_rg_int_en = enable;
    mipi_rx_phy_cfg->phy_mode_link.u32 = phy_mode_link.u32;
}

static void mipi_rx_set_phy_rg_drveclk2_enz(int phy_id, int enable)
{
    volatile u_phy_mode_link phy_mode_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_mode_link.u32 = mipi_rx_phy_cfg->phy_mode_link.u32;
    phy_mode_link.bits.phy_rg_drveclk2_enz = enable;
    mipi_rx_phy_cfg->phy_mode_link.u32 = phy_mode_link.u32;
}

static void mipi_rx_set_phy_rg_drveclk_enz(int phy_id, int enable)
{
    volatile u_phy_mode_link phy_mode_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_mode_link.u32 = mipi_rx_phy_cfg->phy_mode_link.u32;
    phy_mode_link.bits.phy_rg_drveclk_enz = enable;
    mipi_rx_phy_cfg->phy_mode_link.u32 = phy_mode_link.u32;
}

void mipi_rx_drv_set_work_mode(combo_dev_t devno, input_mode_t input_mode)
{
    unsigned long mipi_rx_work_mode_addr;

    mipi_rx_work_mode_addr = (unsigned long)osal_ioremap(MIPI_RX_WORK_MODE_ADDR, MIPI_RX_REG_BYTE_WIDTH);
    if (mipi_rx_work_mode_addr == 0) {
        ot_err("mipi_rx work mode reg ioremap failed!\n");
        return;
    }

    if (input_mode == INPUT_MODE_MIPI) {
        write_reg32(mipi_rx_work_mode_addr, 0x0 << (2 * devno), 0x1 << (2 * devno));   /* devno * 2 */
    } else if ((input_mode == INPUT_MODE_SUBLVDS) || (input_mode == INPUT_MODE_LVDS) ||
        (input_mode == INPUT_MODE_HISPI)) {
        write_reg32(mipi_rx_work_mode_addr, 0x1 << (2 * devno), 0x1 << (2 * devno));  /* devno * 2 */
    } else {
    }

    osal_iounmap((void *)mipi_rx_work_mode_addr, MIPI_RX_REG_BYTE_WIDTH);
}

void mipi_rx_drv_set_mipi_image_rect(combo_dev_t devno, const img_rect_t *p_img_rect)
{
    u_mipi_crop_start_chn0 crop_start_chn0;
    u_mipi_crop_start_chn1 crop_start_chn1;
    u_mipi_crop_start_chn2 crop_start_chn2;
    u_mipi_crop_start_chn3 crop_start_chn3;
    u_mipi_imgsize mipi_imgsize;

    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    crop_start_chn0.u32 = mipi_ctrl_regs->mipi_crop_start_chn0.u32;
    crop_start_chn1.u32 = mipi_ctrl_regs->mipi_crop_start_chn1.u32;
    crop_start_chn2.u32 = mipi_ctrl_regs->mipi_crop_start_chn2.u32;
    crop_start_chn3.u32 = mipi_ctrl_regs->mipi_crop_start_chn3.u32;
    mipi_imgsize.u32 = mipi_ctrl_regs->mipi_imgsize.u32;

    mipi_imgsize.bits.mipi_imgwidth = p_img_rect->width - 1;
    mipi_imgsize.bits.mipi_imgheight = p_img_rect->height - 1;

    crop_start_chn0.bits.mipi_start_x_chn0 = p_img_rect->x;
    crop_start_chn0.bits.mipi_start_y_chn0 = p_img_rect->y;

    crop_start_chn1.bits.mipi_start_x_chn1 = p_img_rect->x;
    crop_start_chn1.bits.mipi_start_y_chn1 = p_img_rect->y;

    crop_start_chn2.bits.mipi_start_x_chn2 = p_img_rect->x;
    crop_start_chn2.bits.mipi_start_y_chn2 = p_img_rect->y;

    crop_start_chn3.bits.mipi_start_x_chn3 = p_img_rect->x;
    crop_start_chn3.bits.mipi_start_y_chn3 = p_img_rect->y;

    mipi_ctrl_regs->mipi_crop_start_chn0.u32 = crop_start_chn0.u32;
    mipi_ctrl_regs->mipi_crop_start_chn1.u32 = crop_start_chn1.u32;
    mipi_ctrl_regs->mipi_crop_start_chn2.u32 = crop_start_chn2.u32;
    mipi_ctrl_regs->mipi_crop_start_chn3.u32 = crop_start_chn3.u32;
    mipi_ctrl_regs->mipi_imgsize.u32 = mipi_imgsize.u32;
}

void mipi_rx_drv_set_mipi_crop_en(combo_dev_t devno, int enable)
{
    u_mipi_ctrl_mode_pixel mipi_ctrl_mode_pixel;
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    mipi_ctrl_mode_pixel.u32 = mipi_ctrl_regs->mipi_ctrl_mode_pixel.u32;
    mipi_ctrl_mode_pixel.bits.crop_en = enable;
    mipi_ctrl_regs->mipi_ctrl_mode_pixel.u32 = mipi_ctrl_mode_pixel.u32;
}

static short mipi_rx_drv_get_data_type(data_type_t input_data_type)
{
    if (input_data_type == DATA_TYPE_RAW_8BIT) {
        return 0x2a;
    } else if (input_data_type == DATA_TYPE_RAW_10BIT) {
        return 0x2b;
    } else if (input_data_type == DATA_TYPE_RAW_12BIT) {
        return 0x2c;
    } else if (input_data_type == DATA_TYPE_RAW_14BIT) {
        return 0x2d;
    } else if (input_data_type == DATA_TYPE_RAW_16BIT) {
        return 0x2e;
    } else if (input_data_type == DATA_TYPE_YUV420_8BIT_NORMAL) {
        return 0x18;
    } else if (input_data_type == DATA_TYPE_YUV420_8BIT_LEGACY) {
        return 0x1a;
    } else if (input_data_type == DATA_TYPE_YUV422_8BIT) {
        return 0x1e;
    } else if (input_data_type == DATA_TYPE_YUV422_PACKED) {
        return 0x1e;
    } else {
        return 0x0;
    }
}

static short mipi_rx_drv_get_data_bit_width(data_type_t input_data_type)
{
    if (input_data_type == DATA_TYPE_RAW_8BIT) {
        return 0x0;
    } else if (input_data_type == DATA_TYPE_RAW_10BIT) {
        return 0x1;
    } else if (input_data_type == DATA_TYPE_RAW_12BIT) {
        return 0x2;
    } else if (input_data_type == DATA_TYPE_RAW_14BIT) {
        return 0x3;
    } else if (input_data_type == DATA_TYPE_RAW_16BIT) {
        return 0x4;
    } else if (input_data_type == DATA_TYPE_YUV420_8BIT_NORMAL) {
        return 0x0;
    } else if (input_data_type == DATA_TYPE_YUV420_8BIT_LEGACY) {
        return 0x0;
    } else if (input_data_type == DATA_TYPE_YUV422_8BIT) {
        return 0x0;
    } else if (input_data_type == DATA_TYPE_YUV422_PACKED) {
        return 0x4;
    } else {
        return 0x0;
    }
}

/* magic num mean bit width, convert to register condfig */
static short mipi_rx_drv_get_ext_data_bit_width(unsigned int ext_data_bit_width)
{
    if (ext_data_bit_width == 8) { /* 8 bits width */
        return 0x0;
    } else if (ext_data_bit_width == 10) { /* 10 bits width */
        return 0x1;
    } else if (ext_data_bit_width == 12) { /* 12 bits width */
        return 0x2;
    } else if (ext_data_bit_width == 14) { /* 14 bits width */
        return 0x3;
    } else if (ext_data_bit_width == 16) { /* 16 bits width */
        return 0x4;
    } else if (ext_data_bit_width == 0) { /* embedded data */
        return 0x5;
    } else {
        return 0x0;
    }
}

/* magic num mean data type index, convert to register config */
static void mipi_rx_drv_set_mipi_user_dt(combo_dev_t devno, int index, short data_type, short bit_width)
{
    u_mipi_userdef_dt user_def_dt;
    u_mipi_user_def user_def;
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    if (mipi_ctrl_regs != NULL) {
        user_def_dt.u32 = mipi_ctrl_regs->mipi_userdef_dt.u32;
        user_def.u32 = mipi_ctrl_regs->mipi_user_def.u32;

        if (index == 0) {
            user_def_dt.bits.user_def0_dt = bit_width;
            user_def.bits.user_def0 = data_type;
        } else if (index == 1) {
            user_def_dt.bits.user_def1_dt = bit_width;
            user_def.bits.user_def1 = data_type;
        } else if (index == 2) {   /* def 2 */
            user_def_dt.bits.user_def2_dt = bit_width;
            user_def.bits.user_def2 = data_type;
        } else if (index == 3) {   /* def 3 */
            user_def_dt.bits.user_def3_dt = bit_width;
            user_def.bits.user_def3 = data_type;
        }

        mipi_ctrl_regs->mipi_userdef_dt.u32 = user_def_dt.u32;
        mipi_ctrl_regs->mipi_user_def.u32 = user_def.u32;
    }
}

static void mipi_rx_drv_set_user_ef(combo_dev_t devno, data_type_t input_data_type)
{
    u_mipi_userdef_dt user_def_dt;
    u_mipi_user_def user_def;
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);
    unsigned char bit_width;
    unsigned int temp_data_type;

    if (input_data_type == DATA_TYPE_YUV420_8BIT_NORMAL) {
        bit_width = 0;
        temp_data_type = 0x18;
    } else if (input_data_type == DATA_TYPE_YUV420_8BIT_LEGACY) {
        bit_width = 0;
        temp_data_type = 0x1a;
    } else if (input_data_type == DATA_TYPE_YUV422_8BIT) {
        bit_width = 0;
        temp_data_type = 0x1e;
    } else {
        bit_width = 4; /* 4 bits width */
        temp_data_type = 0x1e;
    }

    user_def_dt.bits.user_def0_dt = bit_width;
    user_def_dt.bits.user_def1_dt = bit_width;
    user_def_dt.bits.user_def2_dt = bit_width;
    user_def_dt.bits.user_def3_dt = bit_width;

    user_def.bits.user_def0 = temp_data_type;
    user_def.bits.user_def1 = temp_data_type;
    user_def.bits.user_def2 = temp_data_type;
    user_def.bits.user_def3 = temp_data_type;

    mipi_ctrl_regs->mipi_userdef_dt.u32 = user_def_dt.u32;
    mipi_ctrl_regs->mipi_user_def.u32 = user_def.u32;
}

void mipi_rx_drv_set_mipi_yuv_dt(combo_dev_t devno, data_type_t input_data_type)
{
    u_mipi_ctrl_mode_hs mipi_ctrl_mode_hs;
    u_mipi_ctrl_mode_pixel mipi_ctrl_mode_pixel;
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    /* mipi_ctrl_mode_hs */
    mipi_ctrl_mode_hs.u32 = mipi_ctrl_regs->mipi_ctrl_mode_hs.u32;

    if (!is_data_type_yuv(input_data_type)) {
        mipi_ctrl_mode_hs.bits.user_def_en = 0;
        mipi_ctrl_regs->mipi_ctrl_mode_hs.u32 = mipi_ctrl_mode_hs.u32;
    } else {
        mipi_ctrl_mode_hs.bits.user_def_en = 1;
        mipi_ctrl_regs->mipi_ctrl_mode_hs.u32 = mipi_ctrl_mode_hs.u32;

        /* user_def && user_def_dt */
        mipi_rx_drv_set_user_ef(devno, input_data_type);
    }

    /* mipi_ctrl_mode_pixel */
    mipi_ctrl_mode_pixel.u32 = mipi_ctrl_regs->mipi_ctrl_mode_pixel.u32;

    if (input_data_type == DATA_TYPE_YUV420_8BIT_NORMAL) {
        mipi_ctrl_mode_pixel.bits.mipi_yuv_420_nolegacy_en = 1;
        mipi_ctrl_mode_pixel.bits.mipi_yuv_420_legacy_en = 0;
        mipi_ctrl_mode_pixel.bits.mipi_yuv_422_en = 0;
    } else if (input_data_type == DATA_TYPE_YUV420_8BIT_LEGACY) {
        mipi_ctrl_mode_pixel.bits.mipi_yuv_420_nolegacy_en = 0;
        mipi_ctrl_mode_pixel.bits.mipi_yuv_420_legacy_en = 1;
        mipi_ctrl_mode_pixel.bits.mipi_yuv_422_en = 0;
    } else if (input_data_type == DATA_TYPE_YUV422_8BIT) {
        mipi_ctrl_mode_pixel.bits.mipi_yuv_420_nolegacy_en = 0;
        mipi_ctrl_mode_pixel.bits.mipi_yuv_420_legacy_en = 0;
        mipi_ctrl_mode_pixel.bits.mipi_yuv_422_en = 1;
    } else { /* reset raw type */
        mipi_ctrl_mode_pixel.bits.mipi_yuv_420_nolegacy_en = 0;
        mipi_ctrl_mode_pixel.bits.mipi_yuv_420_legacy_en = 0;
        mipi_ctrl_mode_pixel.bits.mipi_yuv_422_en = 0;
    }
    mipi_ctrl_regs->mipi_ctrl_mode_pixel.u32 = mipi_ctrl_mode_pixel.u32;
}

void mipi_rx_drv_set_mipi_wdr_user_dt(combo_dev_t devno, data_type_t input_data_type, const short data_type[WDR_VC_NUM])
{
    u_mipi_userdef_dt user_def_dt;
    u_mipi_user_def user_def;
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    user_def_dt.u32 = mipi_ctrl_regs->mipi_userdef_dt.u32;
    user_def.u32 = mipi_ctrl_regs->mipi_user_def.u32;

    user_def_dt.bits.user_def0_dt = input_data_type;
    user_def_dt.bits.user_def1_dt = input_data_type;

    user_def.bits.user_def0 = data_type[0];
    user_def.bits.user_def1 = data_type[1];

    mipi_ctrl_regs->mipi_userdef_dt.u32 = user_def_dt.u32;
    mipi_ctrl_regs->mipi_user_def.u32 = user_def.u32;
}

void mipi_rx_drv_set_mipi_dol_id(combo_dev_t devno, data_type_t input_data_type, const short dol_id[])
{
    u_mipi_dol_id_code0 dol_id0;
    u_mipi_dol_id_code1 dol_id1;
    u_mipi_dol_id_code2 dol_id2;
    short lef, sef1, sef2;
    short nxt_lef, nxt_sef1, nxt_sef2;
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    mipi_rx_unused(input_data_type);
    mipi_rx_unused(dol_id);

    dol_id0.u32 = mipi_ctrl_regs->mipi_dol_id_code0.u32;
    dol_id1.u32 = mipi_ctrl_regs->mipi_dol_id_code1.u32;
    dol_id2.u32 = mipi_ctrl_regs->mipi_dol_id_code2.u32;

    lef = 0x241;
    sef1 = 0x242;
    sef2 = 0x244;

    nxt_lef = 0x251;
    nxt_sef1 = 0x252;
    nxt_sef2 = 0x254;

    dol_id0.bits.id_code_reg0 = lef;
    dol_id0.bits.id_code_reg1 = sef1;
    dol_id1.bits.id_code_reg2 = sef2;

    dol_id1.bits.id_code_reg3 = nxt_lef;
    dol_id2.bits.id_code_reg4 = nxt_sef1;
    dol_id2.bits.id_code_reg5 = nxt_sef2;

    mipi_ctrl_regs->mipi_dol_id_code0.u32 = dol_id0.u32;
    mipi_ctrl_regs->mipi_dol_id_code1.u32 = dol_id1.u32;
    mipi_ctrl_regs->mipi_dol_id_code2.u32 = dol_id2.u32;
}

void mipi_rx_drv_set_mipi_wdr_mode(combo_dev_t devno, mipi_wdr_mode_t wdr_mode)
{
    u_mipi_ctrl_mode_hs mode_hs;
    u_mipi_ctrl_mode_pixel mode_pixel;
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    mode_hs.u32 = mipi_ctrl_regs->mipi_ctrl_mode_hs.u32;
    mode_pixel.u32 = mipi_ctrl_regs->mipi_ctrl_mode_pixel.u32;

    if (wdr_mode == OT_MIPI_WDR_MODE_NONE) {
        mode_pixel.bits.mipi_dol_mode = 0;
    }
    if (wdr_mode == OT_MIPI_WDR_MODE_VC) {
        mode_pixel.bits.mipi_dol_mode = 0;
    } else if (wdr_mode == OT_MIPI_WDR_MODE_DT) {
        mode_hs.bits.user_def_en = 1;
    } else if (wdr_mode == OT_MIPI_WDR_MODE_DOL) {
        mode_pixel.bits.mipi_dol_mode = 1;
    }

    mipi_ctrl_regs->mipi_ctrl_mode_hs.u32 = mode_hs.u32;
    mipi_ctrl_regs->mipi_ctrl_mode_pixel.u32 = mode_pixel.u32;
}

static void mipi_rx_drv_enable_user_define_dt(combo_dev_t devno, int enable)
{
    u_mipi_ctrl_mode_hs mode_hs;
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    if (mipi_ctrl_regs != NULL) {
        mode_hs.u32 = mipi_ctrl_regs->mipi_ctrl_mode_hs.u32;
        mode_hs.bits.user_def_en = enable;

        mipi_ctrl_regs->mipi_ctrl_mode_hs.u32 = mode_hs.u32;
    }
}

void mipi_rx_drv_set_ext_data_type(const ext_data_type_t* data_type, data_type_t input_data_type)
{
    unsigned int i;
    combo_dev_t devno;
    short input_dt;
    short bit_width;

    devno = data_type->devno;
    input_dt = mipi_rx_drv_get_data_type(input_data_type);
    bit_width = mipi_rx_drv_get_data_bit_width(input_data_type);

    mipi_rx_drv_set_mipi_user_dt(devno, 0, input_dt, bit_width);

    for (i = 0; i < data_type->num; i++) {
        short ext_bit_width = mipi_rx_drv_get_ext_data_bit_width(data_type->ext_data_bit_width[i]);
        mipi_rx_drv_set_mipi_user_dt(devno, i + 1, data_type->ext_data_type[i], ext_bit_width);
    }

    mipi_rx_drv_enable_user_define_dt(devno, 1);
}

unsigned int mipi_rx_drv_get_phy_data(int phy_id, int lane_id)
{
    volatile u_phy_data_link phy_data_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;
    unsigned int lane_data = 0x0;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_data_link.u32 = mipi_rx_phy_cfg->phy_data_link.u32;

    if (lane_id == 0) {
        lane_data = phy_data_link.bits.phy_data0_mipi;
    } else if (lane_id == 1) {
        lane_data = phy_data_link.bits.phy_data1_mipi;
    } else if (lane_id == 2) {    /* lane 2 */
        lane_data = phy_data_link.bits.phy_data2_mipi;
    } else if (lane_id == 3) {    /* lane 3 */
        lane_data = phy_data_link.bits.phy_data3_mipi;
    }

    return lane_data;
}

unsigned int mipi_rx_drv_get_phy_mipi_link_data(int phy_id, int lane_id)
{
    volatile u_phy_data_mipi_link phy_data_mipi_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;
    unsigned int lane_data = 0x0;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_data_mipi_link.u32 = mipi_rx_phy_cfg->phy_data_mipi_link.u32;

    if (lane_id == 0) {
        lane_data = phy_data_mipi_link.bits.phy_data0_mipi_hs;
    } else if (lane_id == 1) {
        lane_data = phy_data_mipi_link.bits.phy_data1_mipi_hs;
    } else if (lane_id == 2) {    /* lane 2 */
        lane_data = phy_data_mipi_link.bits.phy_data2_mipi_hs;
    } else if (lane_id == 3) {    /* lane 3 */
        lane_data = phy_data_mipi_link.bits.phy_data3_mipi_hs;
    }

    return lane_data;
}

unsigned int mipi_rx_drv_get_phy_freq(int phy_id)
{
    volatile u_freq_measure freq_measure;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;
    unsigned int hs_cnt, cil_cnt;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    freq_measure.u32 = mipi_rx_phy_cfg->freq_measure.u32;
    hs_cnt = freq_measure.bits.hs_cnt;
    cil_cnt = freq_measure.bits.cil_cnt;

    if (hs_cnt == 0 || cil_cnt == 0) {
        return 0;
    }

    return hs_cnt * MIPI_CIL_FREQ / cil_cnt; /* freq: MHZ >= 1MHZ */
}

unsigned int mipi_rx_drv_get_phy_lvds_link_data(int phy_id, int lane_id)
{
    volatile u_phy_data_lvds_link phy_data_lvds_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg = NULL;
    unsigned int lane_data = 0x0;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_data_lvds_link.u32 = mipi_rx_phy_cfg->phy_data_lvds_link.u32;

    if (lane_id == 0) {
        lane_data = phy_data_lvds_link.bits.phy_data0_lvds_hs;
    } else if (lane_id == 1) {
        lane_data = phy_data_lvds_link.bits.phy_data1_lvds_hs;
    } else if (lane_id == 2) {     /* lane 2 */
        lane_data = phy_data_lvds_link.bits.phy_data2_lvds_hs;
    } else if (lane_id == 3) {     /* lane 3 */
        lane_data = phy_data_lvds_link.bits.phy_data3_lvds_hs;
    }

    return lane_data;
}

void mipi_rx_drv_set_data_rate(combo_dev_t devno, mipi_data_rate_t data_rate)
{
    u_mipi_ctrl_mode_pixel mipi_ctrl_mode_pixel;
    unsigned int mipi_double_pix_en = 0;
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    if (data_rate == MIPI_DATA_RATE_X1) {
        mipi_double_pix_en = 0;
    } else if (data_rate == MIPI_DATA_RATE_X2) {
        mipi_double_pix_en = 1;
    } else {
        ot_err("unsupported  data_rate:%d  devno %d\n", data_rate, devno);
        return;
    }

    mipi_ctrl_mode_pixel.u32 = mipi_ctrl_regs->mipi_ctrl_mode_pixel.u32;
    mipi_ctrl_mode_pixel.bits.mipi_double_pix_en = mipi_double_pix_en;
    mipi_ctrl_mode_pixel.bits.sync_clear_en = 0x1;
    mipi_ctrl_regs->mipi_ctrl_mode_pixel.u32 = mipi_ctrl_mode_pixel.u32;
}

static void mipi_rx_set_lane_id(combo_dev_t devno, int lane_idx, short lane_id_origin)
{
    u_lane_id0_chn lane_id0_chn;
    u_lane_id1_chn lane_id1_chn;
    short lane_id = lane_id_origin;

    volatile global_ctrl_regs_t *global_ctrl_regs = get_global_ctrl_regs(devno);

    lane_id0_chn.u32 = global_ctrl_regs->lane_id0_chn.u32;
    lane_id1_chn.u32 = global_ctrl_regs->lane_id1_chn.u32;

    if (devno == 0) {
    } else if (devno == 1) {
        lane_id = lane_id_origin / 2; /* 2: 1, 3 convert to 0, 1 */
    } else if (devno == 2) { /* dev2 */
        lane_id = lane_id_origin - MIPI_RX_PHY_LANE_NUM;
    } else { /* dev3 */
        lane_id = (lane_id_origin - MIPI_RX_PHY_LANE_NUM) / 2; /* 2: 1, 3 convert to 0, 1 */
    }

    switch (lane_id) {
        case 0: /* lane 0 */
            lane_id0_chn.bits.lane0_id = lane_idx;
            break;

        case 1: /* lane 1 */
            lane_id0_chn.bits.lane1_id = lane_idx;
            break;

        case 2: /* lane 2 */
            lane_id0_chn.bits.lane2_id = lane_idx;
            break;

        case 3: /* lane 3 */
            lane_id0_chn.bits.lane3_id = lane_idx;
            break;

        case 4: /* lane 4 */
            lane_id1_chn.bits.lane4_id = lane_idx;
            break;

        case 5: /* lane 5 */
            lane_id1_chn.bits.lane5_id = lane_idx;
            break;

        case 6: /* lane 6 */
            lane_id1_chn.bits.lane6_id = lane_idx;
            break;

        case 7: /* lane 7 */
            lane_id1_chn.bits.lane7_id = lane_idx;
            break;

        default:
            break;
    }

    global_ctrl_regs->lane_id0_chn.u32 = lane_id0_chn.u32;
    global_ctrl_regs->lane_id1_chn.u32 = lane_id1_chn.u32;
}

void mipi_rx_drv_set_link_lane_id(combo_dev_t devno, input_mode_t input_mode, const short *p_lane_id)
{
    int i;
    int lane_num;

    if (input_mode == INPUT_MODE_MIPI) {
        lane_num = MIPI_LANE_NUM;
    } else {
        lane_num = LVDS_LANE_NUM;
    }

    for (i = 0; i < lane_num; i++) {
        if (is_valid_id(p_lane_id[i])) {
            mipi_rx_set_lane_id(devno, i, p_lane_id[i]);
        }
    }
}

void mipi_rx_drv_set_mem_cken(combo_dev_t devno, int enable)
{
    u_chn0_mem_ctrl chn0_mem_ctrl;
    u_chn1_mem_ctrl chn1_mem_ctrl;
    u_chn2_mem_ctrl chn2_mem_ctrl;
    u_chn3_mem_ctrl chn3_mem_ctrl;
    mipi_rx_sys_regs_t *mipi_rx_sys_regs;

    mipi_rx_sys_regs = get_mipi_rx_sys_regs();

    switch (devno) {
        case 0: /* chn 0 */
            chn0_mem_ctrl.u32 = mipi_rx_sys_regs->chn0_mem_ctrl.u32;
            chn0_mem_ctrl.bits.chn0_mem_ck_gt = enable;
            mipi_rx_sys_regs->chn0_mem_ctrl.u32 = chn0_mem_ctrl.u32;
            break;
        case 1: /* chn 1 */
            chn1_mem_ctrl.u32 = mipi_rx_sys_regs->chn1_mem_ctrl.u32;
            chn1_mem_ctrl.bits.chn1_mem_ck_gt = enable;
            mipi_rx_sys_regs->chn1_mem_ctrl.u32 = chn1_mem_ctrl.u32;
            break;
        case 2: /* chn 2 */
            chn2_mem_ctrl.u32 = mipi_rx_sys_regs->chn2_mem_ctrl.u32;
            chn2_mem_ctrl.bits.chn2_mem_ck_gt = enable;
            mipi_rx_sys_regs->chn2_mem_ctrl.u32 = chn2_mem_ctrl.u32;
            break;
        case 3: /* chn 3 */
            chn3_mem_ctrl.u32 = mipi_rx_sys_regs->chn3_mem_ctrl.u32;
            chn3_mem_ctrl.bits.chn3_mem_ck_gt = enable;
            mipi_rx_sys_regs->chn3_mem_ctrl.u32 = chn3_mem_ctrl.u32;
            break;
        default:
            break;
    }
}

void mipi_rx_drv_set_clr_cken(combo_dev_t devno, int enable)
{
    u_chn0_clr_en chn0_clr_en;
    u_chn1_clr_en chn1_clr_en;
    u_chn2_clr_en chn2_clr_en;
    u_chn3_clr_en chn3_clr_en;
    mipi_rx_sys_regs_t *mipi_rx_sys_regs;

    mipi_rx_sys_regs = get_mipi_rx_sys_regs();

    switch (devno) {
        case 0: /* chn 0 */
            chn0_clr_en.u32 = mipi_rx_sys_regs->chn0_clr_en.u32;
            chn0_clr_en.bits.chn0_clr_en_lvds = enable;
            chn0_clr_en.bits.chn0_clr_en_align = enable;
            mipi_rx_sys_regs->chn0_clr_en.u32 = chn0_clr_en.u32;
            break;
        case 1: /* chn 1 */
            chn1_clr_en.u32 = mipi_rx_sys_regs->chn1_clr_en.u32;
            chn1_clr_en.bits.chn1_clr_en_lvds = enable;
            chn1_clr_en.bits.chn1_clr_en_align = enable;
            mipi_rx_sys_regs->chn1_clr_en.u32 = chn1_clr_en.u32;
            break;
        case 2: /* chn 2 */
            chn2_clr_en.u32 = mipi_rx_sys_regs->chn2_clr_en.u32;
            chn2_clr_en.bits.chn2_clr_en_lvds = enable;
            chn2_clr_en.bits.chn2_clr_en_align = enable;
            mipi_rx_sys_regs->chn2_clr_en.u32 = chn2_clr_en.u32;
            break;
        case 3: /* chn 3 */
            chn3_clr_en.u32 = mipi_rx_sys_regs->chn3_clr_en.u32;
            chn3_clr_en.bits.chn3_clr_en_lvds = enable;
            chn3_clr_en.bits.chn3_clr_en_align = enable;
            mipi_rx_sys_regs->chn3_clr_en.u32 = chn3_clr_en.u32;
            break;
        default:
            break;
    }
}

/* magic numbers convert to register configs  */
static void mipi_rx_drv_set_phy_en_link(int phy_id, unsigned int lane_bitmap)
{
    u_phy_en_link phy_en_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_en_link.u32 = mipi_rx_phy_cfg->phy_en_link.u32;

    if (lane_bitmap & 0x5) {
        phy_en_link.bits.phy_da_d0_valid = lane_bitmap & 0x1;
        phy_en_link.bits.phy_da_d2_valid = (lane_bitmap & 0x4) >> 2;     /* mask 0x4, shift 2 */
        phy_en_link.bits.phy_d0_term_en = lane_bitmap & 0x1;
        phy_en_link.bits.phy_d2_term_en = (lane_bitmap & 0x4) >> 2;      /* mask 0x4, shift 2 */
        phy_en_link.bits.phy_clk_term_en = 1;
    }

    if (lane_bitmap & 0xa) {
        phy_en_link.bits.phy_da_d1_valid = (lane_bitmap & 0x2) >> 1;
        phy_en_link.bits.phy_da_d3_valid = (lane_bitmap & 0x8) >> 3;       /* mask 0x8, shift 3 */
        phy_en_link.bits.phy_d1_term_en = (lane_bitmap & 0x2) >> 1;
        phy_en_link.bits.phy_d3_term_en = (lane_bitmap & 0x8) >> 3;       /* mask 0x8, shift 3 */
        phy_en_link.bits.phy_clk2_term_en = 1;
    }

    mipi_rx_phy_cfg->phy_en_link.u32 = phy_en_link.u32;
}

static void mipi_rx_drv_set_phy_mode(int phy_id, input_mode_t input_mode, unsigned int lane_bitmap)
{
    u_phy_mode_link phy_mode_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;
    const int cmos_en = 0;

    mipi_rx_unused(input_mode);

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_mode_link.u32 = mipi_rx_phy_cfg->phy_mode_link.u32;

    phy_mode_link.bits.phy_rg_en_d = 0xf;
    phy_mode_link.bits.phy_rg_en_cmos = cmos_en;
    phy_mode_link.bits.phy_rg_en_clk = 1;
    phy_mode_link.bits.phy_rg_mipi_mode = 0;

    if (lane_bitmap & 0xa) {
        phy_mode_link.bits.phy_rg_en_clk2 = 1;
        phy_mode_link.bits.phy_rg_mipi_mode2 = 0;
    }

    mipi_rx_phy_cfg->phy_mode_link.u32 = phy_mode_link.u32;
}

static void mipi_rx_drv_set_phy_en(unsigned int lane_bitmap)
{
    u_phy_en phy_en;
    mipi_rx_sys_regs_t *mipi_rx_sys_regs;

    mipi_rx_sys_regs = get_mipi_rx_sys_regs();
    phy_en.u32 = mipi_rx_sys_regs->phy_en.u32;

    if (lane_bitmap & 0xf) {
        phy_en.bits.phy0_en = 1;
    }

    if (lane_bitmap & 0xf0) {
        phy_en.bits.phy1_en = 1;
    }

    if (lane_bitmap & 0xf00) {
        phy_en.bits.phy2_en = 1;
    }

    mipi_rx_sys_regs->phy_en.u32 = phy_en.u32;
}

static void mipi_rx_drv_set_lane_en(unsigned int lane_bitmap, unsigned lane_mask)
{
    u_lane_en lane_en;
    mipi_rx_sys_regs_t *mipi_rx_sys_regs;

    mipi_rx_sys_regs = get_mipi_rx_sys_regs();
    lane_en.u32 = mipi_rx_sys_regs->lane_en.u32;
    lane_en.u32 = lane_en.u32 & (~lane_mask);
    lane_en.u32 = lane_en.u32 | (lane_bitmap & lane_mask);
    mipi_rx_sys_regs->lane_en.u32 = lane_en.u32;
}

static void mipi_rx_drv_set_phy_cil_en(unsigned int lane_bitmap, int enable)
{
    u_phy_cil_ctrl phy_cil_ctrl;
    mipi_rx_sys_regs_t *mipi_rx_sys_regs;

    mipi_rx_sys_regs = get_mipi_rx_sys_regs();
    phy_cil_ctrl.u32 = mipi_rx_sys_regs->phy_cil_ctrl.u32;

    if (lane_bitmap & 0xf) {
        phy_cil_ctrl.bits.phycil0_cken = enable;
    }

    if (lane_bitmap & 0xf0) {
        phy_cil_ctrl.bits.phycil1_cken = enable;
    }

    if (lane_bitmap & 0xf00) {
        phy_cil_ctrl.bits.phycil2_cken = enable;
    }

    mipi_rx_sys_regs->phy_cil_ctrl.u32 = phy_cil_ctrl.u32;
}

static void mipi_rx_drv_set_phy_cfg_mode(input_mode_t input_mode, unsigned int lane_bitmap)
{
    u_phycfg_mode phycfg_mode;
    mipi_rx_sys_regs_t *mipi_rx_sys_regs;
    unsigned int cfg_mode;
    unsigned int cfg_mode_sel;

    mipi_rx_sys_regs = get_mipi_rx_sys_regs();
    phycfg_mode.u32 = mipi_rx_sys_regs->phycfg_mode.u32;

    if (input_mode == INPUT_MODE_MIPI) {
        cfg_mode = 0;
        cfg_mode_sel = 0;
    } else if (input_mode == INPUT_MODE_SUBLVDS || input_mode == INPUT_MODE_LVDS || input_mode == INPUT_MODE_HISPI) {
        cfg_mode = 1;
        cfg_mode_sel = 0;
    } else {
        cfg_mode = 2; /* 2: other input_mode */
        cfg_mode_sel = 1;
    }

    if (lane_bitmap & 0x5) {
        phycfg_mode.bits.phycil0_0_cfg_mode = cfg_mode;
        phycfg_mode.bits.phycil0_cfg_mode_sel = cfg_mode_sel;
    }

    if (lane_bitmap & 0xa) {
        phycfg_mode.bits.phycil0_1_cfg_mode = cfg_mode;
        phycfg_mode.bits.phycil0_cfg_mode_sel = cfg_mode_sel;
    }

    if (lane_bitmap & 0x50) {
        phycfg_mode.bits.phycil1_0_cfg_mode = cfg_mode;
        phycfg_mode.bits.phycil1_cfg_mode_sel = cfg_mode_sel;
    }

    if (lane_bitmap & 0xa0) {
        phycfg_mode.bits.phycil1_1_cfg_mode = cfg_mode;
        phycfg_mode.bits.phycil1_cfg_mode_sel = cfg_mode_sel;
    }

    mipi_rx_sys_regs->phycfg_mode.u32 = phycfg_mode.u32;
}

static void mipi_rx_set_phycfg_en(unsigned int value)
{
    volatile mipi_rx_sys_regs_t *mipi_rx_sys_regs = get_mipi_rx_sys_regs();

    mipi_rx_sys_regs->phycfg_en.u32 = value;
}

static void mipi_rx_clear_vsync_cnt(unsigned int phy_id)
{
    int i;
    for (i = 0; i < MIPI_RX_MAX_DEV_NUM; i++) {
        if (g_dev_phy_info[i].phy_id == phy_id) {
            g_dev_phy_info[i].vsync_cnt = 0;
        }
    }
}

static void mipi_rx_clear_recovery_cnt(unsigned int phy_id)
{
    int i;
    for (i = 0; i < MIPI_RX_MAX_DEV_NUM; i++) {
        if (g_dev_phy_info[i].phy_id == phy_id) {
            g_dev_phy_info[i].recovery_cnt = 0;
        }
    }
}

static void mipi_rx_auto_cfg(u_phycfg_en *phycfg_en, bool is_single_lane_mode)
{
    mipi_rx_set_phy_cal_int(0x3); /* 0x3: mipi_cfg */
    mipi_rx_set_phycfg_en(phycfg_en->u32);

    osal_udelay(20); /* usleep 20us */

    if (phycfg_en->bits.phycil0_cfg_en == 1) {
        mipi_rx_set_phy_ext_reg1(0, PHY_EXT_REG1_VALUE);
        mipi_rx_set_phycfg_en(phycfg_en->u32);
        osal_udelay(20); /* usleep 20us */
        mipi_rx_set_phy_cfg_link(0, 0x1ffff);
        mipi_rx_set_phycfg_en(phycfg_en->u32);
        osal_udelay(20); /* usleep 20us */
        if (!mipi_rx_check_ext_cfg()) {
            mipi_rx_set_phy_offnn(0, 0xeeffff);
            mipi_rx_set_phycfg_en(phycfg_en->u32);
            osal_udelay(20); /* usleep 20us */
            mipi_rx_set_phy_test(0, is_single_lane_mode ? 0x0 : 0x00000044);
            if (is_single_lane_mode) {
                mipi_rx_set_phycfg_en(phycfg_en->u32);
                osal_udelay(20); /* usleep 20us */
                mipi_rx_set_phy_test(0, 0x00000040);
            }
            mipi_rx_set_phycfg_en(phycfg_en->u32);
            // clear all devs vsync_cnt at same phy0
            mipi_rx_clear_vsync_cnt(0);
            mipi_rx_clear_recovery_cnt(0);
        }
    }

    if (phycfg_en->bits.phycil1_cfg_en == 1) {
        mipi_rx_set_phy_ext_reg1(1, PHY_EXT_REG1_VALUE);
        mipi_rx_set_phycfg_en(phycfg_en->u32);
        osal_udelay(20); /* usleep 20us */
        mipi_rx_set_phy_cfg_link(1, 0x1ffff);
        mipi_rx_set_phycfg_en(phycfg_en->u32);
        osal_udelay(20); /* usleep 20us */
        if (!mipi_rx_check_ext_cfg()) {
            mipi_rx_set_phy_offnn(1, 0xeeffff);
            mipi_rx_set_phycfg_en(phycfg_en->u32);
            osal_udelay(20); /* usleep 20us */
            mipi_rx_set_phy_test(1, is_single_lane_mode ? 0x0 : 0x00000044);
            if (is_single_lane_mode) {
                mipi_rx_set_phycfg_en(phycfg_en->u32);
                osal_udelay(20); /* usleep 20us */
                mipi_rx_set_phy_test(1, 0x00000040);
            }
            mipi_rx_set_phycfg_en(phycfg_en->u32);
            // clear all devs vsync_cnt at same phy1
            mipi_rx_clear_vsync_cnt(1);
            mipi_rx_clear_recovery_cnt(1);
        }
    }
}

/* when use lane0/1/4/5 || lane0+1/lane4+5. set 0x40 */
static bool mipi_rx_dev_is_single_lane_mode(unsigned int lane_bitmap)
{
    unsigned int tmpmap0 = lane_bitmap & 0x3;
    unsigned int tmpmap1 = lane_bitmap & 0xfc;
    unsigned int tmpmap2 = lane_bitmap & 0x30;
    unsigned int tmpmap3 = lane_bitmap & 0xcf;

    return ((tmpmap0 != 0 && tmpmap1 == 0) || (tmpmap2 != 0 && tmpmap3 == 0));
}

static void mipi_rx_drv_set_phy_cfg_en(unsigned int lane_bitmap, int enable)
{
    u_phycfg_en phycfg_en;
    mipi_rx_sys_regs_t *mipi_rx_sys_regs;
    int phy_id = 0;

    mipi_rx_sys_regs = get_mipi_rx_sys_regs();
    phycfg_en.u32 = mipi_rx_sys_regs->phycfg_en.u32;

    if (lane_bitmap & 0xf) {
        phycfg_en.bits.phycil0_cfg_en = enable;
        phy_id = 0;
    }

    if (lane_bitmap & 0xf0) {
        phycfg_en.bits.phycil1_cfg_en = enable;
        phy_id = 1;
    }

    if (lane_bitmap & 0xf00) {
        phycfg_en.bits.phycil2_cfg_en = enable;
    }

    mipi_rx_sys_regs->phycfg_en.u32 = phycfg_en.u32;
    // if phy has a single lane dev, g_phy_single_lane_mode should be TRUE
    g_phy_single_lane_mode[phy_id] = (mipi_rx_dev_is_single_lane_mode(lane_bitmap) || g_phy_single_lane_mode[phy_id]);
    mipi_rx_auto_cfg(&phycfg_en, g_phy_single_lane_mode[phy_id]);
}

static void mipi_rx_drv_set_phy_skew_link(int phy_id, unsigned int lane_bitmap, phy_speed_mode_t speed_mode)
{
    u_phy_skew_link phy_skew_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_skew_link.u32 = mipi_rx_phy_cfg->phy_skew_link.u32;

    if (speed_mode == PHY_SPEED_LE1500M_BITPERSEC) {
        /* set bit 1 */
        phy_skew_link.bits.phy_rg_dirbi   |= (lane_bitmap & 0xf);
        phy_skew_link.bits.phy_rg_aadh_ds |= (lane_bitmap & 0xf);
        phy_skew_link.bits.phy_rg_ah_ds   |= (lane_bitmap & 0xf);
        mipi_rx_phy_cfg->phy_skew_link.u32 = phy_skew_link.u32;
        return;
    }
    /* set bit 0 */
    phy_skew_link.bits.phy_rg_dirbi   &= ~(lane_bitmap & 0xf);
    phy_skew_link.bits.phy_rg_aadh_ds &= ~(lane_bitmap & 0xf);
    phy_skew_link.bits.phy_rg_ah_ds   &= ~(lane_bitmap & 0xf);
    mipi_rx_phy_cfg->phy_skew_link.u32 = phy_skew_link.u32;
}

void mipi_rx_drv_set_speed_mode(combo_dev_t combo_dev, phy_speed_mode_t speed_mode)
{
    g_dev_phy_info[combo_dev].speed_mode = speed_mode;

    mipi_rx_drv_set_phy_skew_link(g_dev_phy_info[combo_dev].phy_id,
                                  g_dev_phy_info[combo_dev].phy_lane_bitmap,
                                  g_dev_phy_info[combo_dev].speed_mode);
    // PHYCFG_EN
    mipi_rx_drv_set_phy_cfg_en(g_dev_phy_info[combo_dev].lane_bitmap, 1);
}

void mipi_rx_drv_get_speed_mode(combo_dev_t combo_dev, phy_speed_mode_t *speed_mode)
{
    *speed_mode = g_dev_phy_info[combo_dev].speed_mode;
}

void mipi_rx_drv_set_phy_config(combo_dev_t devno, input_mode_t input_mode,
                                unsigned int lane_bitmap, unsigned int lane_mask)
{
    unsigned int i;
    unsigned int mask;
    unsigned int phy_lane_bitmap;

    for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
        mask = 0xf << (MIPI_RX_PHY_LANE_NUM * i);
        if (lane_bitmap & mask) {
            phy_lane_bitmap = (lane_bitmap & mask) >> (MIPI_RX_PHY_LANE_NUM * i);
            mipi_rx_drv_set_phy_en_link(i, phy_lane_bitmap);
            mipi_rx_drv_set_phy_mode(i, input_mode, phy_lane_bitmap);
            g_dev_phy_info[devno].phy_id = i;
            g_dev_phy_info[devno].lane_bitmap = lane_bitmap;
            g_dev_phy_info[devno].phy_lane_bitmap = phy_lane_bitmap;
            /* set to default by dev */
            g_dev_phy_info[devno].speed_mode = PHY_SPEED_LE1500M_BITPERSEC;
            mipi_rx_drv_set_phy_skew_link(i, phy_lane_bitmap, PHY_SPEED_LE1500M_BITPERSEC);
            g_has_cil_int[i] = false;
        }
    }

    mipi_rx_drv_set_phy_en(lane_bitmap);
    mipi_rx_drv_set_lane_en(lane_bitmap, lane_mask);
    mipi_rx_drv_set_phy_cil_en(lane_bitmap, 1);
    mipi_rx_drv_set_phy_cfg_mode(input_mode, lane_bitmap);
    mipi_rx_drv_set_phy_cfg_en(lane_bitmap, 1);
}

void mipi_rx_drv_reset_cil_int(void)
{
    unsigned int i;
    for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
        g_has_cil_int[i] = false;
    }
}

void mipi_rx_drv_reset_phy_mode(void)
{
    unsigned int i;
    for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
        g_phy_single_lane_mode[i] = false;
    }
}

static void mipi_rx_drv_set_phy_cmv(int phy_id, phy_cmv_mode_t cmv_mode, unsigned int lane_bitmap)
{
    int mipi_cmv_mode = 0;
    u_phy_mode_link phy_mode_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg = NULL;

    if (cmv_mode == PHY_CMV_GE1200MV) {
        mipi_cmv_mode = 0;
    } else if (cmv_mode == PHY_CMV_LT1200MV) {
        mipi_cmv_mode = 1;
    }

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_mode_link.u32 = mipi_rx_phy_cfg->phy_mode_link.u32;

    if (lane_bitmap & 0xa) {
        phy_mode_link.bits.phy_rg_mipi_mode2 = mipi_cmv_mode;
    }

    if (lane_bitmap & 0x5) {
        phy_mode_link.bits.phy_rg_mipi_mode = mipi_cmv_mode;
    }

    mipi_rx_phy_cfg->phy_mode_link.u32 = phy_mode_link.u32;
}

void mipi_rx_drv_set_phy_cmvmode(input_mode_t input_mode, phy_cmv_mode_t cmv_mode, unsigned int lane_bitmap)
{
    unsigned int i;
    unsigned int mask;
    unsigned int phy_lane_bitmap;

    for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
        mask = 0xf << (MIPI_RX_PHY_LANE_NUM * i);
        if (lane_bitmap & mask) {
            phy_lane_bitmap = (lane_bitmap & mask) >> (MIPI_RX_PHY_LANE_NUM * i);
            mipi_rx_drv_set_phy_cmv(i, cmv_mode, phy_lane_bitmap);
        }
    }

    mipi_rx_drv_set_phy_cfg_mode(input_mode, lane_bitmap);
    mipi_rx_drv_set_phy_cfg_en(lane_bitmap, 1);
}

void mipi_rx_drv_set_lvds_image_rect(combo_dev_t devno, const img_rect_t *p_img_rect, short total_lane_num)
{
    volatile lvds_ctrl_regs_t *ctrl_reg_attr;
    u_lvds_imgsize lvds_img_size;
    u_lvds_crop_start0 crop_start0;
    u_lvds_crop_start1 crop_start1;
    u_lvds_crop_start2 crop_start2;
    u_lvds_crop_start3 crop_start3;
    unsigned int width_per_lane, x_per_lane;

    ctrl_reg_attr = get_lvds_ctrl_regs(devno);

    if (total_lane_num == 0) {
        return;
    }

    width_per_lane = (p_img_rect->width / total_lane_num);
    x_per_lane = (p_img_rect->x / total_lane_num);

    lvds_img_size.u32 = ctrl_reg_attr->lvds_imgsize.u32;
    crop_start0.u32 = ctrl_reg_attr->lvds_crop_start0.u32;
    crop_start1.u32 = ctrl_reg_attr->lvds_crop_start1.u32;
    crop_start2.u32 = ctrl_reg_attr->lvds_crop_start2.u32;
    crop_start3.u32 = ctrl_reg_attr->lvds_crop_start3.u32;

    lvds_img_size.bits.lvds_imgwidth_lane = width_per_lane - 1;
    lvds_img_size.bits.lvds_imgheight = p_img_rect->height - 1;

    crop_start0.bits.lvds_start_x0_lane = x_per_lane;
    crop_start0.bits.lvds_start_y0 = p_img_rect->y;

    crop_start1.bits.lvds_start_x1_lane = x_per_lane;
    crop_start1.bits.lvds_start_y1 = p_img_rect->y;

    crop_start2.bits.lvds_start_x2_lane = x_per_lane;
    crop_start2.bits.lvds_start_y2 = p_img_rect->y;

    crop_start3.bits.lvds_start_x3_lane = x_per_lane;
    crop_start3.bits.lvds_start_y3 = p_img_rect->y;

    ctrl_reg_attr->lvds_imgsize.u32 = lvds_img_size.u32;
    ctrl_reg_attr->lvds_crop_start0.u32 = crop_start0.u32;
    ctrl_reg_attr->lvds_crop_start1.u32 = crop_start1.u32;
    ctrl_reg_attr->lvds_crop_start2.u32 = crop_start2.u32;
    ctrl_reg_attr->lvds_crop_start3.u32 = crop_start3.u32;
}

void mipi_rx_drv_set_lvds_crop_en(combo_dev_t devno, int enable)
{
    volatile lvds_ctrl_regs_t *ctrl_reg_attr;
    u_lvds_ctrl lvds_ctrl;

    ctrl_reg_attr = get_lvds_ctrl_regs(devno);
    if (ctrl_reg_attr == NULL) {
        return;
    }

    lvds_ctrl.u32 = ctrl_reg_attr->lvds_ctrl.u32;

    lvds_ctrl.bits.lvds_crop_en = enable;

    ctrl_reg_attr->lvds_ctrl.u32 = lvds_ctrl.u32;
}

static int mipi_rx_drv_set_lvds_wdr_normal(const lvds_vsync_attr_t *vsync_attr, u_lvds_wdr *lvds_wdr)
{
    if (vsync_attr->sync_type == LVDS_VSYNC_NORMAL) {
        /* SOF-EOF WDR, long exposure frame and short exposure frame has independent sync code */
        lvds_wdr->bits.lvds_wdr_mode = 0x0;
    } else if (vsync_attr->sync_type == LVDS_VSYNC_SHARE) {
        /* SOF-EOF WDR, long exposure frame and short exposure frame share the SOF and EOF */
        lvds_wdr->bits.lvds_wdr_mode = 0x2;
    } else {
        ot_err("not support vsync type: %d\n", vsync_attr->sync_type);
        return -1;
    }
    return 0;
}

static int mipi_rx_drv_set_lvds_wdr_dol(const lvds_vsync_attr_t *vsync_attr, const lvds_fid_attr_t *fid_attr,
    u_lvds_wdr *lvds_wdr, u_lvds_dolscd_hblk *scd_hblk)
{
    /* Sony DOL WDR */
    if (vsync_attr->sync_type == LVDS_VSYNC_NORMAL) {
        /*
         * SAV-EAV WDR, 4 sync code, fid embedded in 4th sync code
         * long exposure fame and short exposure frame has independent sync code
         */
        if (fid_attr->fid_type == LVDS_FID_IN_SAV) {
            lvds_wdr->bits.lvds_wdr_mode = 0x4;
        } else if (fid_attr->fid_type == LVDS_FID_IN_DATA) {
            /*
             * SAV-EAV WDR, 5 sync code(Line Information), fid in the fist DATA,
             * fid in data, line information
             */
            if (fid_attr->output_fil) {
                /* Frame Information Line is included in the image data */
                lvds_wdr->bits.lvds_wdr_mode = 0xd;
            } else {
                /* Frame Information Line is not included in the image data */
                lvds_wdr->bits.lvds_wdr_mode = 0x6;
            }
        } else {
            ot_err("not support fid type: %d\n", fid_attr->fid_type);
            return -1;
        }
    } else if (vsync_attr->sync_type == LVDS_VSYNC_HCONNECT) {
        /*
         * SAV-EAV H-Connection DOL, long exposure frame and short exposure frame
         * share the same SAV EAV, the H-Blank is assigned by the dol_hblank1 and dol_hblank2
         */
        if (fid_attr->fid_type == LVDS_FID_NONE) {
            lvds_wdr->bits.lvds_wdr_mode = 0x5;
        } else {
            ot_err("not support fid type: %d\n", fid_attr->fid_type);
            return -1;
        }
        scd_hblk->bits.dol_hblank1 = vsync_attr->hblank1;
        scd_hblk->bits.dol_hblank2 = vsync_attr->hblank2;
    } else {
        ot_err("not support vsync type: %d\n", vsync_attr->sync_type);
        return -1;
    }
    return 0;
}

/* magic num mean wdr mode, convert to register config */
static int mipi_rx_drv_set_lvds_wdr_num(lvds_wdr_mode_t wdr_mode, u_lvds_wdr *lvds_wdr)
{
    /* set the wdr frame number */
    switch (wdr_mode) {
        case OT_LVDS_WDR_MODE_NONE:
            lvds_wdr->bits.lvds_wdr_num = 0;
            break;
        case OT_LVDS_WDR_MODE_2F:
        case OT_LVDS_WDR_MODE_DOL_2F:
            lvds_wdr->bits.lvds_wdr_num = 1;
            break;

        case OT_LVDS_WDR_MODE_3F:
        case OT_LVDS_WDR_MODE_DOL_3F:
            lvds_wdr->bits.lvds_wdr_num = 2;        /* WDR num 2 */
            break;

        case OT_LVDS_WDR_MODE_4F:
        case OT_LVDS_WDR_MODE_DOL_4F:
            lvds_wdr->bits.lvds_wdr_num = 3;        /* WDR num 3 */
            break;

        default:
            ot_err("not support WDR_MODE: %d\n", wdr_mode);
            return -1;
    }
    return 0;
}

int mipi_rx_drv_set_lvds_wdr_mode(combo_dev_t devno, lvds_wdr_mode_t wdr_mode,
                                  const lvds_vsync_attr_t *vsync_attr, const lvds_fid_attr_t *fid_attr)
{
    int ret;
    volatile lvds_ctrl_regs_t *ctrl_reg_attr;
    u_lvds_wdr lvds_wdr;
    u_lvds_dolscd_hblk scd_hblk;

    ctrl_reg_attr = get_lvds_ctrl_regs(devno);

    lvds_wdr.u32 = ctrl_reg_attr->lvds_wdr.u32;
    scd_hblk.u32 = ctrl_reg_attr->lvds_dolscd_hblk.u32;

    if (wdr_mode == OT_LVDS_WDR_MODE_NONE) {
        lvds_wdr.bits.lvds_wdr_en = 0;
    } else {
        lvds_wdr.bits.lvds_wdr_en = 1;
    }

    ret = mipi_rx_drv_set_lvds_wdr_num(wdr_mode, &lvds_wdr);
    if (ret < 0) {
        return ret;
    }

    /* set wdr mode */
    if (OT_LVDS_WDR_MODE_2F <= wdr_mode && wdr_mode <= OT_LVDS_WDR_MODE_4F) {
        ret = mipi_rx_drv_set_lvds_wdr_normal(vsync_attr, &lvds_wdr);
        if (ret < 0) {
            return ret;
        }
    } else if (OT_LVDS_WDR_MODE_DOL_2F <= wdr_mode && wdr_mode <= OT_LVDS_WDR_MODE_DOL_4F) {
        ret = mipi_rx_drv_set_lvds_wdr_dol(vsync_attr, fid_attr, &lvds_wdr, &scd_hblk);
        if (ret < 0) {
            return ret;
        }
    }

    ctrl_reg_attr->lvds_wdr.u32 = lvds_wdr.u32;
    ctrl_reg_attr->lvds_dolscd_hblk.u32 = scd_hblk.u32;

    return 0;
}

void mipi_rx_drv_set_lvds_ctrl_mode(combo_dev_t devno, lvds_sync_mode_t sync_mode,
                                    data_type_t input_data_type,
                                    lvds_bit_endian_t data_endian,
                                    lvds_bit_endian_t sync_code_endian)
{
    volatile lvds_ctrl_regs_t *ctrl_reg_attr;
    u_lvds_ctrl lvds_ctrl;
    unsigned short raw_type;

    ctrl_reg_attr = get_lvds_ctrl_regs(devno);

    lvds_ctrl.u32 = ctrl_reg_attr->lvds_ctrl.u32;

    switch (input_data_type) {
        case DATA_TYPE_RAW_8BIT:
            raw_type = 0x1;
            break;

        case DATA_TYPE_RAW_10BIT:
            raw_type = 0x2;
            break;

        case DATA_TYPE_RAW_12BIT:
            raw_type = 0x3;
            break;

        case DATA_TYPE_RAW_14BIT:
            raw_type = 0x4;
            break;

        case DATA_TYPE_RAW_16BIT:
            raw_type = 0x5;
            break;

        default:
            return;
    }

    lvds_ctrl.bits.lvds_sync_mode = sync_mode;
    lvds_ctrl.bits.lvds_raw_type = raw_type;
    lvds_ctrl.bits.lvds_pix_big_endian = data_endian;
    lvds_ctrl.bits.lvds_code_big_endian = sync_code_endian;

    ctrl_reg_attr->lvds_ctrl.u32 = lvds_ctrl.u32;
}

void mipi_rx_drv_set_lvds_data_rate(combo_dev_t devno, mipi_data_rate_t data_rate)
{
    u_lvds_output_pix_num lvds_output_pixel_num;
    unsigned int lvds_double_pix_en = 0;
    volatile lvds_ctrl_regs_t *lvds_ctrl_regs = get_lvds_ctrl_regs(devno);

    if (data_rate == MIPI_DATA_RATE_X1) {
        lvds_double_pix_en = 0;
    } else if (data_rate == MIPI_DATA_RATE_X2) {
        lvds_double_pix_en = 0x1;
    } else {
        ot_err("unsupported  data_rate:%d  devno %d\n", data_rate, devno);
        return;
    }

    lvds_output_pixel_num.u32 = lvds_ctrl_regs->lvds_output_pix_num.u32;
    lvds_output_pixel_num.bits.lvds_double_pix_en = lvds_double_pix_en;
    lvds_ctrl_regs->lvds_output_pix_num.u32 = lvds_output_pixel_num.u32;
}

void mipi_rx_drv_set_dol_line_information(combo_dev_t devno, lvds_wdr_mode_t wdr_mode)
{
    volatile lvds_ctrl_regs_t *ctrl_reg_attr;

    ctrl_reg_attr = get_lvds_ctrl_regs(devno);

    if (wdr_mode >= OT_LVDS_WDR_MODE_DOL_2F) {
        ctrl_reg_attr->lvds_li_word0.bits.li_word0_0 = 0x0201;  /* LEF n frame */
        ctrl_reg_attr->lvds_li_word0.bits.li_word0_1 = 0x0211;  /* LEF n + 1 frame */

        ctrl_reg_attr->lvds_li_word1.bits.li_word1_0 = 0x0202;  /* SEF1 n frame */
        ctrl_reg_attr->lvds_li_word1.bits.li_word1_1 = 0x0212;  /* SEF1 n + 1 frame */
    }

    if (wdr_mode >= OT_LVDS_WDR_MODE_DOL_3F) {
        ctrl_reg_attr->lvds_li_word2.bits.li_word2_0 = 0x0204;  /* SEF2 n frame */
        ctrl_reg_attr->lvds_li_word2.bits.li_word2_1 = 0x0214;  /* SEF2 n + 1 frame */
    }

    if (wdr_mode >= OT_LVDS_WDR_MODE_DOL_4F) {
        ctrl_reg_attr->lvds_li_word3.bits.li_word3_0 = 0x0208;  /* SEF3 n frame */
        ctrl_reg_attr->lvds_li_word3.bits.li_word3_1 = 0x0218;  /* SEF3 n + 1 frame */
    }
}

/* magic nums means vc num and sync num */
static void set_lvds_sync_sof_eof(volatile lvds_sync_code_cfg_t *p_sync_code,
    const unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM], int land_id)
{
    {
        u_lvds_lane_sof_01 lvds_sof_01;
        lvds_sof_01.u32 = p_sync_code->lvds_lane_sof_01.u32;
        lvds_sof_01.bits.lane_sof_0 = sync_code[land_id][0][0];
        lvds_sof_01.bits.lane_sof_1 = sync_code[land_id][1][0];
        p_sync_code->lvds_lane_sof_01.u32 = lvds_sof_01.u32;
    }
    {
        u_lvds_lane_sof_23 lvds_sof_23;
        lvds_sof_23.u32 = p_sync_code->lvds_lane_sof_23.u32;
        lvds_sof_23.bits.lane_sof_2 = sync_code[land_id][2][0];         /* vc 2, sync 0 */
        lvds_sof_23.bits.lane_sof_3 = sync_code[land_id][3][0];         /* vc 3, sync 0 */
        p_sync_code->lvds_lane_sof_23.u32 = lvds_sof_23.u32;
    }
    {
        u_lvds_lane_eof_01 lvds_eof_01;
        lvds_eof_01.u32 = p_sync_code->lvds_lane_eof_01.u32;
        lvds_eof_01.bits.lane_eof_0 = sync_code[land_id][0][1];
        lvds_eof_01.bits.lane_eof_1 = sync_code[land_id][1][1];
        p_sync_code->lvds_lane_eof_01.u32 = lvds_eof_01.u32;
    }
    {
        u_lvds_lane_eof_23 lvds_eof_23;
        lvds_eof_23.u32 = p_sync_code->lvds_lane_eof_23.u32;
        lvds_eof_23.bits.lane_eof_2 = sync_code[land_id][2][1];         /* vc 2, sync 1 */
        lvds_eof_23.bits.lane_eof_3 = sync_code[land_id][3][1];         /* vc 3, sync 1 */
        p_sync_code->lvds_lane_eof_23.u32 = lvds_eof_23.u32;
    }
}

/* magic nums means vc num and sync num */
static void set_lvds_sync_sol_eol(volatile lvds_sync_code_cfg_t *p_sync_code,
    const unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM], int land_id)
{
    {
        u_lvds_lane_sol_01 lvds_sol_01;
        lvds_sol_01.u32 = p_sync_code->lvds_lane_sol_01.u32;
        lvds_sol_01.bits.lane_sol_0 = sync_code[land_id][0][2];         /* vc 0, sync 2 */
        lvds_sol_01.bits.lane_sol_1 = sync_code[land_id][1][2];         /* vc 1, sync 2 */
        p_sync_code->lvds_lane_sol_01.u32 = lvds_sol_01.u32;
    }
    {
        u_lvds_lane_sol_23 lvds_sol_23;
        lvds_sol_23.u32 = p_sync_code->lvds_lane_sol_23.u32;
        lvds_sol_23.bits.lane_sol_2 = sync_code[land_id][2][2];         /* vc 2, sync 2 */
        lvds_sol_23.bits.lane_sol_3 = sync_code[land_id][3][2];         /* vc 3, sync 2 */
        p_sync_code->lvds_lane_sol_23.u32 = lvds_sol_23.u32;
    }
    {
        u_lvds_lane_eol_01 lvds_eol_01;
        lvds_eol_01.u32 = p_sync_code->lvds_lane_eol_01.u32;
        lvds_eol_01.bits.lane_eol_0 = sync_code[land_id][0][3];         /* vc 0, sync 3 */
        lvds_eol_01.bits.lane_eol_1 = sync_code[land_id][1][3];         /* vc 1, sync 3 */
        p_sync_code->lvds_lane_eol_01.u32 = lvds_eol_01.u32;
    }
    {
        u_lvds_lane_eol_23 lvds_eol_23;
        lvds_eol_23.u32 = p_sync_code->lvds_lane_eol_23.u32;
        lvds_eol_23.bits.lane_eol_2 = sync_code[land_id][2][3];         /* vc 2, sync 3 */
        lvds_eol_23.bits.lane_eol_3 = sync_code[land_id][3][3];         /* vc 3, sync 3 */
        p_sync_code->lvds_lane_eol_23.u32 = lvds_eol_23.u32;
    }
}

static void set_lvds_sync_code(combo_dev_t devno, int n_frame, unsigned int lane_cnt,
    const short lane_id[LVDS_LANE_NUM], const unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM])
{
    unsigned int i;
    volatile lvds_sync_code_cfg_t *p_sync_code = NULL;
    volatile lvds_ctrl_regs_t *ctrl_reg_attr = get_lvds_ctrl_regs(devno);

    for (i = 0; i < lane_cnt; i++) {
        short lane_idx = lane_id[i];
        if (lane_idx != -1) {
            if (n_frame == TRUE) {
                p_sync_code = &ctrl_reg_attr->lvds_this_frame_sync_code[i];
            } else {
                p_sync_code = &ctrl_reg_attr->lvds_next_frame_sync_code[i];
            }

            set_lvds_sync_sof_eof(p_sync_code, sync_code, i);
            set_lvds_sync_sol_eol(p_sync_code, sync_code, i);
        }
    }
}

void mipi_rx_drv_set_lvds_sync_code(combo_dev_t devno, unsigned int lane_cnt, const short lane_id[LVDS_LANE_NUM],
                                    const unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM])
{
    set_lvds_sync_code(devno, TRUE, lane_cnt, lane_id, sync_code);
}

void mipi_rx_drv_set_lvds_nxt_sync_code(combo_dev_t devno, unsigned int lane_cnt, const short lane_id[LVDS_LANE_NUM],
                                        const unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM])
{
    set_lvds_sync_code(devno, FALSE, lane_cnt, lane_id, sync_code);
}

static void mipi_rx_drv_set_phy_sync_dct(int phy_id, int raw_type,
                                         lvds_bit_endian_t code_endian, unsigned int phy_lane_bitmap)
{
    u_phy_sync_dct_link phy_sync_dct_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_sync_dct_link.u32 = mipi_rx_phy_cfg->phy_sync_dct_link.u32;

    if (phy_lane_bitmap & 0x5) {
        phy_sync_dct_link.bits.cil_raw_type0 = raw_type;
        phy_sync_dct_link.bits.cil_code_big_endian0 = code_endian;
    }

    if (phy_lane_bitmap & 0xa) {
        phy_sync_dct_link.bits.cil_raw_type1 = raw_type;
        phy_sync_dct_link.bits.cil_code_big_endian1 = code_endian;
    }

    mipi_rx_phy_cfg->phy_sync_dct_link.u32 = phy_sync_dct_link.u32;
}

static short get_sensor_lane_index(short lane, const short lane_id[LVDS_LANE_NUM])
{
    int i;

    for (i = 0; i < LVDS_LANE_NUM; i++) {
        if (lane_id[i] == lane) {
            break;
        }
    }

    return i;
}

static void mipi_rx_drv_set_lvds_phy_sync_code(int phy_id,
                                               const short lane_id[LVDS_LANE_NUM],
                                               const unsigned short n_sync_code[][WDR_VC_NUM][SYNC_CODE_NUM],
                                               const unsigned short nxt_sync_code[][WDR_VC_NUM][SYNC_CODE_NUM],
                                               unsigned int phy_lane_bitmap)
{
    u_phy_sync_sof0_link phy_sync_sof0_link;
    u_phy_sync_sof1_link phy_sync_sof1_link;
    u_phy_sync_sof2_link phy_sync_sof2_link;
    u_phy_sync_sof3_link phy_sync_sof3_link;
    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;
    short sensor_lane_idx;
    short lane;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_sync_sof0_link.u32 = mipi_rx_phy_cfg->phy_sync_sof0_link.u32;
    phy_sync_sof1_link.u32 = mipi_rx_phy_cfg->phy_sync_sof1_link.u32;
    phy_sync_sof2_link.u32 = mipi_rx_phy_cfg->phy_sync_sof2_link.u32;
    phy_sync_sof3_link.u32 = mipi_rx_phy_cfg->phy_sync_sof3_link.u32;

    if (phy_lane_bitmap & 0x1) {
        lane = 0 + MIPI_RX_PHY_LANE_NUM * phy_id;
        sensor_lane_idx = get_sensor_lane_index(lane, lane_id);
        phy_sync_sof0_link.bits.cil_sof0_word4_0 = n_sync_code[sensor_lane_idx][0][0];
        phy_sync_sof0_link.bits.cil_sof1_word4_0 = nxt_sync_code[sensor_lane_idx][0][0];
    }

    if (phy_lane_bitmap & 0x2) {
        lane = 1 + MIPI_RX_PHY_LANE_NUM * phy_id;
        sensor_lane_idx = get_sensor_lane_index(lane, lane_id);
        phy_sync_sof1_link.bits.cil_sof0_word4_1 = n_sync_code[sensor_lane_idx][0][0];
        phy_sync_sof1_link.bits.cil_sof1_word4_1 = nxt_sync_code[sensor_lane_idx][0][0];
    }

    if (phy_lane_bitmap & 0x4) {
        lane = 2 + MIPI_RX_PHY_LANE_NUM * phy_id; /* 2nd lane */
        sensor_lane_idx = get_sensor_lane_index(lane, lane_id);
        phy_sync_sof2_link.bits.cil_sof0_word4_2 = n_sync_code[sensor_lane_idx][0][0];
        phy_sync_sof2_link.bits.cil_sof1_word4_2 = nxt_sync_code[sensor_lane_idx][0][0];
    }

    if (phy_lane_bitmap & 0x8) {
        lane = 3 + MIPI_RX_PHY_LANE_NUM * phy_id; /* 3rd lane */
        sensor_lane_idx = get_sensor_lane_index(lane, lane_id);
        phy_sync_sof3_link.bits.cil_sof0_word4_3 = n_sync_code[sensor_lane_idx][0][0];
        phy_sync_sof3_link.bits.cil_sof1_word4_3 = nxt_sync_code[sensor_lane_idx][0][0];
    }

    mipi_rx_phy_cfg->phy_sync_sof0_link.u32 = phy_sync_sof0_link.u32;
    mipi_rx_phy_cfg->phy_sync_sof1_link.u32 = phy_sync_sof1_link.u32;
    mipi_rx_phy_cfg->phy_sync_sof2_link.u32 = phy_sync_sof2_link.u32;
    mipi_rx_phy_cfg->phy_sync_sof3_link.u32 = phy_sync_sof3_link.u32;
}

void mipi_rx_drv_set_phy_sync_config(const lvds_dev_attr_t *p_attr, unsigned int lane_bitmap,
                                     const unsigned short nxt_sync_code[][WDR_VC_NUM][SYNC_CODE_NUM])
{
    int raw_type;
    unsigned int i;
    unsigned int mask;
    unsigned int phy_lane_bitmap;

    switch (p_attr->input_data_type) {
        case DATA_TYPE_RAW_8BIT:
            raw_type = 0x1;
            break;

        case DATA_TYPE_RAW_10BIT:
            raw_type = 0x2;
            break;

        case DATA_TYPE_RAW_12BIT:
            raw_type = 0x3;
            break;

        case DATA_TYPE_RAW_14BIT:
            raw_type = 0x4;
            break;

        case DATA_TYPE_RAW_16BIT:
            raw_type = 0x5;
            break;

        default:
            return;
    }

    for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
        mask = 0xf << (MIPI_RX_PHY_LANE_NUM * i);
        if (lane_bitmap & mask) {
            phy_lane_bitmap = (lane_bitmap & mask) >> (MIPI_RX_PHY_LANE_NUM * i);
            mipi_rx_drv_set_phy_sync_dct(i, raw_type, p_attr->sync_code_endian, phy_lane_bitmap);
            mipi_rx_drv_set_lvds_phy_sync_code(i, p_attr->lane_id, p_attr->sync_code, nxt_sync_code, phy_lane_bitmap);
        }
    }
}

static unsigned int mipi_rx_drv_get_lane_mask_by_act_mode(combo_dev_t devno, lane_divide_mode_t mode)
{
    unsigned int mask = 0x0;

    switch (mode) {
        case LANE_DIVIDE_MODE_0:
            if (devno == 0) { /* mipi_dev 0 */
                mask = 0xff;
            }
            break;
        case LANE_DIVIDE_MODE_1:
            if (devno == 0) { /* mipi_dev 0 */
                mask = 0xf;
            } else if (devno == 2) { /* mipi_dev 2 */
                mask = 0xf0;
            }
            break;
        case LANE_DIVIDE_MODE_2:
            if (devno == 0) { /* mipi_dev 0 */
                mask = 0xf;
            } else if (devno == 2) { /* mipi_dev 2 */
                mask = 0x50;
            } else if (devno == 3) { /* mipi_dev 3 */
                mask = 0xa0;
            }
            break;
        case LANE_DIVIDE_MODE_3:
            if (devno == 0) { /* mipi_dev 0 */
                mask = 0x5;
            } else if (devno == 1) { /* mipi_dev 1 */
                mask = 0xa;
            } else if (devno == 2) { /* mipi_dev 2 */
                mask = 0x50;
            } else if (devno == 3) { /* mipi_dev 3 */
                mask = 0xa0;
            }
            break;
        case LANE_DIVIDE_MODE_4:
            if (devno == 0) { /* mipi_dev 0 */
                mask = 0x5;
            } else if (devno == 1) { /* mipi_dev 1 */
                mask = 0xa;
            } else if (devno == 2) { /* mipi_dev 2 */
                mask = 0xf0;
            }
            break;
        default:
            break;
    }

    return mask;
}

unsigned int mipi_rx_drv_get_lane_mask(combo_dev_t devno, lane_divide_mode_t mode)
{
    lane_divide_mode_t mode_ex = mode;

    if (mipi_rx_chip_is_hi3516dv500()) {
        mode_ex = mode * 2 + 1; // 16D : 16D mode 0 == 19D mode 1, 16D mode 1 == 19D mode 3, mode * 2 + 1
    }

    return mipi_rx_drv_get_lane_mask_by_act_mode(devno, mode_ex);
}

static int mipi_rx_drv_is_lane_valid_mode0(combo_dev_t devno, short lane_id)
{
    int lane_valid = 0;
    if (mipi_rx_chip_is_hi3516dv500()) {
        if (devno == 0 && (lane_id >= 0 && lane_id <= 3)) { /* mipi_dev 0: lane 0,1,2,3 */
            lane_valid = 1;
        }
    } else {
        if (devno == 0 && (lane_id >= 0 && lane_id <= 7)) { /* mipi_dev 0: lane 0,1,2,3,4,5,6,7 */
            lane_valid = 1;
        }
    }
    return lane_valid;
}

static int mipi_rx_drv_is_lane_valid_mode1(combo_dev_t devno, short lane_id)
{
    int lane_valid = 0;
    if (mipi_rx_chip_is_hi3516dv500()) {
        if (devno == 0 && (lane_id == 0 || lane_id == 2)) { /* mipi_dev 0: lane 0,2 */
            lane_valid = 1;
        } else if (devno == 1 && (lane_id == 1 || lane_id == 3)) { /* mipi_dev 1: lane 1,3 */
            lane_valid = 1;
        }
    } else {
        if (devno == 0 && (lane_id >= 0 && lane_id <= 3)) { /* mipi_dev 0: lane 0,1,2,3 */
            lane_valid = 1;
        } else if (devno == 2 && (lane_id >= 4 && lane_id <= 7)) { /* mipi_dev 2: lane 4,5,6,7 */
            lane_valid = 1;
        }
    }

    return lane_valid;
}

static int mipi_rx_drv_is_lane_valid_mode2(combo_dev_t devno, short lane_id)
{
    int lane_valid = 0;
    if (mipi_rx_chip_is_hi3516dv500()) {
        return lane_valid;
    }
    if (devno == 0 && (lane_id >= 0 && lane_id <= 3)) { /* mipi_dev 0: lane 0,1,2,3 */
        lane_valid = 1;
    } else if (devno == 2 && (lane_id == 4 || lane_id == 6)) { /* mipi_dev 2: lane 4,6 */
        lane_valid = 1;
    } else if (devno == 3 && (lane_id == 5 || lane_id == 7)) { /* mipi_dev 3: lane 5,7 */
        lane_valid = 1;
    }
    return lane_valid;
}

static int mipi_rx_drv_is_lane_valid_mode3(combo_dev_t devno, short lane_id)
{
    int lane_valid = 0;
    if (mipi_rx_chip_is_hi3516dv500()) {
        return lane_valid;
    }
    if (devno == 0 && (lane_id == 0 || lane_id == 2)) { /* mipi_dev 0: lane 0,2 */
        lane_valid = 1;
    } else if (devno == 1 && (lane_id == 1 || lane_id == 3)) { /* mipi_dev 1: lane 1,3 */
        lane_valid = 1;
    } else if (devno == 2 && (lane_id == 4 || lane_id == 6)) { /* mipi_dev 2: lane 4,6 */
        lane_valid = 1;
    } else if (devno == 3 && (lane_id == 5 || lane_id == 7)) { /* mipi_dev 3: lane 5,7 */
        lane_valid = 1;
    }
    return lane_valid;
}

static int mipi_rx_drv_is_lane_valid_mode4(combo_dev_t devno, short lane_id)
{
    int lane_valid = 0;
    if (mipi_rx_chip_is_hi3516dv500()) {
        return lane_valid;
    }
    if (devno == 0 && (lane_id == 0 || lane_id == 2)) { /* mipi_dev 0: lane 0,2 */
        lane_valid = 1;
    } else if (devno == 1 && (lane_id == 1 || lane_id == 3)) { /* mipi_dev 1: lane 1,3 */
        lane_valid = 1;
    } else if (devno == 2 && (lane_id >= 4 && lane_id <= 7)) { /* mipi_dev 2: lane 4,5,6,7 */
        lane_valid = 1;
    }
    return lane_valid;
}


/* magic numbers means land_id and mipi_dev */
int mipi_rx_drv_is_lane_valid(combo_dev_t devno, short lane_id, lane_divide_mode_t mode)
{
    int lane_valid = 0;

    switch (mode) {
        case LANE_DIVIDE_MODE_0:
            lane_valid = mipi_rx_drv_is_lane_valid_mode0(devno, lane_id);
            break;
        case LANE_DIVIDE_MODE_1:
            lane_valid = mipi_rx_drv_is_lane_valid_mode1(devno, lane_id);
            break;
        case LANE_DIVIDE_MODE_2:
            lane_valid = mipi_rx_drv_is_lane_valid_mode2(devno, lane_id);
            break;
        case LANE_DIVIDE_MODE_3:
            lane_valid = mipi_rx_drv_is_lane_valid_mode3(devno, lane_id);
            break;
        case LANE_DIVIDE_MODE_4:
            lane_valid = mipi_rx_drv_is_lane_valid_mode4(devno, lane_id);
            break;
        default:
            break;
    }

    return lane_valid;
}

static void mipi_rx_drv_hw_init(void);
void mipi_rx_drv_set_hs_mode(lane_divide_mode_t mode)
{
    u_hs_mode_select hs_mode_sel;
    mipi_rx_sys_regs_t *mipi_rx_sys_regs = NULL;
    unsigned int i;
    lane_divide_mode_t mode_ex = mode;

    mipi_rx_drv_hw_init();

    if (mipi_rx_chip_is_hi3516dv500()) {
        mode_ex = mode * 2 + 1; // 16D : 16D mode 0 == 19D mode 1, 16D mode 1 == 19D mode 3, mode * 2 + 1
    }

    for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
        mipi_rx_set_phy_rg_ext_en(i, g_phy_mode[mode_ex][i].phy_rg_ext_en);
        mipi_rx_set_phy_rg_ext2_en(i, g_phy_mode[mode_ex][i].phy_rg_ext2_en);
        mipi_rx_set_phy_rg_int_en(i, g_phy_mode[mode_ex][i].phy_rg_int_en);
        mipi_rx_set_phy_rg_drveclk2_enz(i, g_phy_mode[mode_ex][i].phy_rg_drveclk2_enz);
        mipi_rx_set_phy_rg_drveclk_enz(i, g_phy_mode[mode_ex][i].phy_rg_drveclk_enz);
    }

    mipi_rx_sys_regs = get_mipi_rx_sys_regs();
    hs_mode_sel.u32 = mipi_rx_sys_regs->hs_mode_select.u32;
    hs_mode_sel.bits.hs_mode = g_hs_mode[mode_ex];
    mipi_rx_sys_regs->hs_mode_select.u32 = hs_mode_sel.u32;
}

void mipi_rx_drv_set_chn_int_mask(combo_dev_t devno)
{
    u_mipi_int_msk mipi_int_msk;
    volatile mipi_rx_sys_regs_t *mipi_rx_sys_regs = get_mipi_rx_sys_regs();

    mipi_int_msk.u32 = mipi_rx_sys_regs->mipi_int_msk.u32;

    if (devno == 0) { /* chn 0 */
        mipi_int_msk.bits.int_chn0_mask = 0x1;
    } else if (devno == 1) { /* chn 1 */
        mipi_int_msk.bits.int_chn1_mask = 0x1;
    } else if (devno == 2) { /* chn 2 */
        mipi_int_msk.bits.int_chn2_mask = 0x1;
    } else if (devno == 3) { /* chn 3 */
        mipi_int_msk.bits.int_chn3_mask = 0x1;
    }

    mipi_rx_sys_regs->mipi_int_msk.u32 = mipi_int_msk.u32;
}

void mipi_rx_drv_set_lvds_ctrl_int_mask(combo_dev_t devno, unsigned int mask)
{
    volatile lvds_ctrl_regs_t *lvds_ctrl_regs = get_lvds_ctrl_regs(devno);

    lvds_ctrl_regs->lvds_ctrl_int_msk.u32 = mask;
}

void mipi_rx_drv_set_mipi_ctrl_int_mask(combo_dev_t devno, unsigned int mask)
{
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    mipi_ctrl_regs->mipi_ctrl_int_msk.u32 = mask;
}

void mipi_rx_drv_set_mipi_csi_int_mask(combo_dev_t devno, unsigned int mask)
{
    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    mipi_ctrl_regs->mipi_crc_intr_msk.u32 = mask;
}

void mipi_rx_drv_set_align_int_mask(combo_dev_t devno, unsigned int mask)
{
    volatile global_ctrl_regs_t *global_ctrl_regs = get_global_ctrl_regs(devno);

    global_ctrl_regs->align_int_msk.u32 = mask;
    global_ctrl_regs->chn_int_mask.u32 = 0xf;
}

static void mipi_rx_enable_disable_clock(combo_dev_t combo_dev, int enable)
{
    unsigned long mipi_rx_clock_addr;

    mipi_rx_clock_addr = (unsigned long)osal_ioremap(mipi_rx_pixel_crg_addr(combo_dev), MIPI_RX_REG_BYTE_WIDTH);
    if (mipi_rx_clock_addr == 0) {
        ot_err("mipi_rx clock ioremap failed!\n");
        return;
    }
    mipi_rx_set_bit(enable, 4, mipi_rx_clock_addr); /* 4 : offset */
    osal_iounmap((void *)mipi_rx_clock_addr, MIPI_RX_REG_BYTE_WIDTH);
}

void mipi_rx_drv_enable_clock(combo_dev_t combo_dev)
{
    mipi_rx_enable_disable_clock(combo_dev, 1);
}

void mipi_rx_drv_disable_clock(combo_dev_t combo_dev)
{
    mipi_rx_enable_disable_clock(combo_dev, 0);
}

static void sensor_enable_disable_clock(sns_clk_source_t sns_clk_source, int enable)
{
    unsigned long sensor_clock_addr;
    const unsigned offset = 4;

    sensor_clock_addr = (unsigned long)osal_ioremap(sns_crg_addr(sns_clk_source), MIPI_RX_REG_BYTE_WIDTH);
    if (sensor_clock_addr == 0) {
        ot_err("sensor clock ioremap failed!\n");
        return;
    }

    mipi_rx_set_bit(enable, offset, sensor_clock_addr);

    osal_iounmap((void *)sensor_clock_addr, MIPI_RX_REG_BYTE_WIDTH);
}

void sensor_drv_enable_clock(sns_clk_source_t sns_clk_source)
{
    sensor_enable_disable_clock(sns_clk_source, 1);
}

void sensor_drv_disable_clock(sns_clk_source_t sns_clk_source)
{
    sensor_enable_disable_clock(sns_clk_source, 0);
}

static void mipi_rx_core_reset_unreset(combo_dev_t combo_dev, int reset)
{
    unsigned long mipi_rx_reset_addr;
    const unsigned offset = 0;
    mipi_rx_reset_addr = (unsigned long)osal_ioremap(mipi_rx_pixel_crg_addr(combo_dev), MIPI_RX_REG_BYTE_WIDTH);
    if (mipi_rx_reset_addr == 0) {
        ot_err("mipi_rx clock ioremap failed!\n");
        return;
    }
    mipi_rx_set_bit(reset, offset, mipi_rx_reset_addr);
    osal_iounmap((void *)mipi_rx_reset_addr, MIPI_RX_REG_BYTE_WIDTH);
}

void mipi_rx_drv_core_reset(combo_dev_t combo_dev)
{
    mipi_rx_core_reset_unreset(combo_dev, 1);
}

void mipi_rx_drv_core_unreset(combo_dev_t combo_dev)
{
    mipi_rx_core_reset_unreset(combo_dev, 0);
}

static void sensor_reset_unreset(sns_rst_source_t sns_reset_source, int reset)
{
    unsigned long sensor_reset_addr;
    const unsigned offset = 0;

#ifndef OT_FPGA
    sensor_reset_addr = (unsigned long)osal_ioremap(sns_crg_addr(sns_reset_source), MIPI_RX_REG_BYTE_WIDTH);
    if (sensor_reset_addr == 0) {
        ot_err("sensor reset ioremap failed!\n");
        return;
    }

    mipi_rx_set_bit(reset, offset, sensor_reset_addr);
    mipi_rx_set_bit(reset, offset + 1, sensor_reset_addr);

    osal_iounmap((void *)sensor_reset_addr, MIPI_RX_REG_BYTE_WIDTH);
#else
    sensor_reset_addr = (unsigned long)osal_ioremap(g_sns_rst_crg_addr, MIPI_RX_REG_BYTE_WIDTH);
    if (sensor_reset_addr == 0) {
        ot_err("sensor reset ioremap failed!\n");
        return;
    }

    mipi_rx_set_bit(reset, offset + sns_reset_source, sensor_reset_addr);

    osal_iounmap((void *)sensor_reset_addr, MIPI_RX_REG_BYTE_WIDTH);
#endif
}

void sensor_drv_reset(sns_rst_source_t sns_reset_source)
{
    sensor_reset_unreset(sns_reset_source, 1);
}

void sensor_drv_unreset(sns_rst_source_t sns_reset_source)
{
    sensor_reset_unreset(sns_reset_source, 0);
}

void mipi_rx_drv_get_mipi_imgsize_statis(combo_dev_t devno, short vc, img_size_t *p_size)
{
    u_mipi_imgsize0_statis mipi_imgsize0_statis;
    u_mipi_imgsize1_statis mipi_imgsize1_statis;
    u_mipi_imgsize2_statis mipi_imgsize2_statis;
    u_mipi_imgsize3_statis mipi_imgsize3_statis;

    volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    if (vc == 0) {
        mipi_imgsize0_statis.u32 = mipi_ctrl_regs->mipi_imgsize0_statis.u32;
        p_size->width = mipi_imgsize0_statis.bits.imgwidth_statis_vc0;
        p_size->height = mipi_imgsize0_statis.bits.imgheight_statis_vc0;
    } else if (vc == 1) {
        mipi_imgsize1_statis.u32 = mipi_ctrl_regs->mipi_imgsize1_statis.u32;
        p_size->width = mipi_imgsize1_statis.bits.imgwidth_statis_vc1;
        p_size->height = mipi_imgsize1_statis.bits.imgheight_statis_vc1;
    } else if (vc == 2) { /* 2: vc2 */
        mipi_imgsize2_statis.u32 = mipi_ctrl_regs->mipi_imgsize2_statis.u32;
        p_size->width = mipi_imgsize2_statis.bits.imgwidth_statis_vc2;
        p_size->height = mipi_imgsize2_statis.bits.imgheight_statis_vc2;
    } else if (vc == 3) { /* 3: vc3 */
        mipi_imgsize3_statis.u32 = mipi_ctrl_regs->mipi_imgsize3_statis.u32;
        p_size->width = mipi_imgsize3_statis.bits.imgwidth_statis_vc3;
        p_size->height = mipi_imgsize3_statis.bits.imgheight_statis_vc3;
    }
}

void mipi_rx_drv_get_lvds_imgsize_statis(combo_dev_t devno, short vc, img_size_t *p_size)
{
    u_lvds_imgsize0_statis lvds_imgsize0_statis;
    u_lvds_imgsize1_statis lvds_imgsize1_statis;
    u_lvds_imgsize2_statis lvds_imgsize2_statis;
    u_lvds_imgsize3_statis lvds_imgsize3_statis;

    volatile lvds_ctrl_regs_t *lvds_ctrl_regs = get_lvds_ctrl_regs(devno);

    if (vc == 0) {
        lvds_imgsize0_statis.u32 = lvds_ctrl_regs->lvds_imgsize0_statis.u32;
        p_size->width = lvds_imgsize0_statis.bits.lvds_imgwidth0;
        p_size->height = lvds_imgsize0_statis.bits.lvds_imgheight0;
    } else if (vc == 1) {
        lvds_imgsize1_statis.u32 = lvds_ctrl_regs->lvds_imgsize1_statis.u32;
        p_size->width = lvds_imgsize1_statis.bits.lvds_imgwidth1;
        p_size->height = lvds_imgsize1_statis.bits.lvds_imgheight1;
    } else if (vc == 2) { /* 2: vc2 */
        lvds_imgsize2_statis.u32 = lvds_ctrl_regs->lvds_imgsize2_statis.u32;
        p_size->width = lvds_imgsize2_statis.bits.lvds_imgwidth2;
        p_size->height = lvds_imgsize2_statis.bits.lvds_imgheight2;
    } else if (vc == 3) { /* 3: vc3 */
        lvds_imgsize3_statis.u32 = lvds_ctrl_regs->lvds_imgsize3_statis.u32;
        p_size->width = lvds_imgsize3_statis.bits.lvds_imgwidth3;
        p_size->height = lvds_imgsize3_statis.bits.lvds_imgheight3;
    }
}

void mipi_rx_drv_get_lvds_lane_imgsize_statis(combo_dev_t devno, short lane, img_size_t *p_size)
{
    u_lvds_lane_imgsize_statis lvds_lane_imgsize_statis;

    volatile lvds_ctrl_regs_t *lvds_ctrl_regs = get_lvds_ctrl_regs(devno);

    lvds_lane_imgsize_statis.u32 = lvds_ctrl_regs->lvds_lane_imgsize_statis[lane].u32;
    p_size->width = lvds_lane_imgsize_statis.bits.lane_imgwidth + 1;
    p_size->height = lvds_lane_imgsize_statis.bits.lane_imgheight;
}

static void mipi_rx_phy_cil_int_statis(int phy_id)
{
    unsigned int phy_int_status;

    mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

    mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    phy_int_status = mipi_rx_phy_cfg->mipi_cil_int_link.u32;

    if (phy_int_status) {
        mipi_rx_phy_cfg->mipi_cil_int_raw_link.u32 = 0xffffffff;

        if (!g_has_cil_int[phy_id]) {
            g_has_cil_int[phy_id] = true;
            return;  // ignore first cil int, because sensor may not be stable
        }

        if (phy_int_status & MIPI_ESC_CLK2) {
            g_phy_err_int_cnt[phy_id].clk2_fsm_escape_err_cnt++;
        }

        if (phy_int_status & MIPI_ESC_CLK) {
            g_phy_err_int_cnt[phy_id].clk_fsm_escape_err_cnt++;
        }

        if (phy_int_status & MIPI_ESC_D0) {
            g_phy_err_int_cnt[phy_id].d0_fsm_escape_err_cnt++;
        }

        if (phy_int_status & MIPI_ESC_D1) {
            g_phy_err_int_cnt[phy_id].d1_fsm_escape_err_cnt++;
        }

        if (phy_int_status & MIPI_ESC_D2) {
            g_phy_err_int_cnt[phy_id].d2_fsm_escape_err_cnt++;
        }

        if (phy_int_status & MIPI_ESC_D3) {
            g_phy_err_int_cnt[phy_id].d3_fsm_escape_err_cnt++;
        }

        if (phy_int_status & MIPI_TIMEOUT_CLK2) {
            g_phy_err_int_cnt[phy_id].clk2_fsm_timeout_err_cnt++;
        }

        if (phy_int_status & MIPI_TIMEOUT_CLK) {
            g_phy_err_int_cnt[phy_id].clk_fsm_timeout_err_cnt++;
        }

        if (phy_int_status & MIPI_TIMEOUT_D0) {
            g_phy_err_int_cnt[phy_id].d0_fsm_timeout_err_cnt++;
        }

        if (phy_int_status & MIPI_TIMEOUT_D1) {
            g_phy_err_int_cnt[phy_id].d1_fsm_timeout_err_cnt++;
        }

        if (phy_int_status & MIPI_TIMEOUT_D2) {
            g_phy_err_int_cnt[phy_id].d2_fsm_timeout_err_cnt++;
        }

        if (phy_int_status & MIPI_TIMEOUT_D3) {
            g_phy_err_int_cnt[phy_id].d3_fsm_timeout_err_cnt++;
        }
    }
}

static void mipi_int_statics(combo_dev_t devno)
{
    unsigned int mipi_ctrl_int;
    mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    mipi_ctrl_int = mipi_ctrl_regs->mipi_ctrl_int.u32;

    if (mipi_ctrl_int) {
        mipi_ctrl_regs->mipi_ctrl_int_raw.u32 = 0xffffffff;
    }

    if (mipi_ctrl_int & CMD_FIFO_WRITE_ERR) {
        g_mipi_err_int_cnt[devno].cmd_fifo_wrerr_cnt++;
    }

    if (mipi_ctrl_int & DATA_FIFO_WRITE_ERR) {
        g_mipi_err_int_cnt[devno].data_fifo_wrerr_cnt++;
    }

    if (mipi_ctrl_int & CMD_FIFO_READ_ERR) {
        g_mipi_err_int_cnt[devno].cmd_fifo_rderr_cnt++;
    }

    if (mipi_ctrl_int & DATA_FIFO_READ_ERR) {
        g_mipi_err_int_cnt[devno].data_fifo_rderr_cnt++;
    }

    if (mipi_ctrl_int & VSYNC_RAW) {
        g_dev_phy_info[devno].vsync_cnt++;
        if (g_dev_phy_info[devno].vsync_cnt >= MIPI_RX_MAX_START_FRAME_CNT ||
            g_dev_phy_info[devno].recovery_cnt > MIPI_RX_MAX_RECOVER_CNT) {
            mipi_rx_drv_set_mipi_ctrl_int_mask(devno, MIPI_CTRL_INT_MASK); // turn off vsync raw
        }
    }
}

static void mipi_crc_intr_count1(combo_dev_t devno, unsigned int mipi_crc_intr_int)
{
    if (mipi_crc_intr_int & MIPI_VC0_CRC_ERR) {
        g_mipi_err_int_cnt[devno].vc0_err_crc_cnt++;
    }

    if (mipi_crc_intr_int & MIPI_VC1_CRC_ERR) {
        g_mipi_err_int_cnt[devno].vc1_err_crc_cnt++;
    }

    if (mipi_crc_intr_int & MIPI_VC2_CRC_ERR) {
        g_mipi_err_int_cnt[devno].vc2_err_crc_cnt++;
    }

    if (mipi_crc_intr_int & MIPI_VC3_CRC_ERR) {
        g_mipi_err_int_cnt[devno].vc3_err_crc_cnt++;
    }

    if (mipi_crc_intr_int & MIPI_VC0_ECC_ERR) {
        g_mipi_err_int_cnt[devno].vc0_err_ecc_corrected_cnt++;
    }

    if (mipi_crc_intr_int & MIPI_VC1_ECC_ERR) {
        g_mipi_err_int_cnt[devno].vc1_err_ecc_corrected_cnt++;
    }

    if (mipi_crc_intr_int & MIPI_VC2_ECC_ERR) {
        g_mipi_err_int_cnt[devno].vc2_err_ecc_corrected_cnt++;
    }

    if (mipi_crc_intr_int & MIPI_VC3_ECC_ERR) {
        g_mipi_err_int_cnt[devno].vc3_err_ecc_corrected_cnt++;
    }

    if (mipi_crc_intr_int & MIPI_MULT_ECC_ERR) {
        g_mipi_err_int_cnt[devno].err_ecc_double_cnt++;
    }
}

static void mipi_crc_intr_count2(combo_dev_t devno, unsigned int mipi_crc_intr_int)
{
    if (mipi_crc_intr_int & MIPI_VC0_FRAME_NUM_ERR) {
        g_mipi_err_int_cnt[devno].vc0_err_frame_num_cnt++;
    }

    if (mipi_crc_intr_int & MIPI_VC1_FRAME_NUM_ERR) {
        g_mipi_err_int_cnt[devno].vc1_err_frame_num_cnt++;
    }

    if (mipi_crc_intr_int & MIPI_VC2_FRAME_NUM_ERR) {
        g_mipi_err_int_cnt[devno].vc2_err_frame_num_cnt++;
    }

    if (mipi_crc_intr_int & MIPI_VC3_FRAME_NUM_ERR) {
        g_mipi_err_int_cnt[devno].vc3_err_frame_num_cnt++;
    }

    if (mipi_crc_intr_int & MIPI_VC0_MISMATCH) {
        g_mipi_err_int_cnt[devno].vc0_err_frame_s_e_num_mismatch_cnt++;
    }

    if (mipi_crc_intr_int & MIPI_VC1_MISMATCH) {
        g_mipi_err_int_cnt[devno].vc1_err_frame_s_e_num_mismatch_cnt++;
    }

    if (mipi_crc_intr_int & MIPI_VC2_MISMATCH) {
        g_mipi_err_int_cnt[devno].vc2_err_frame_s_e_num_mismatch_cnt++;
    }

    if (mipi_crc_intr_int & MIPI_VC3_MISMATCH) {
        g_mipi_err_int_cnt[devno].vc3_err_frame_s_e_num_mismatch_cnt++;
    }
}

void mipi_rx_drv_get_phy_skew_length(int phy_id, unsigned int *length0, unsigned int *length1)
{
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);

    *length0 = mipi_rx_phy_cfg->skew_cal_lepgth0_link.u32; /* 0x130 */
    *length1 = mipi_rx_phy_cfg->skew_cal_lepgth1_link.u32; /* 0x134 */
}

static void mipi_crc_intr_process(combo_dev_t devno)
{
    unsigned int phy_id = g_dev_phy_info[devno].phy_id;
    volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
    volatile mipi_rx_sys_regs_t *mipi_rx_sys_regs = get_mipi_rx_sys_regs();
    bool is_single_mode = g_phy_single_lane_mode[g_dev_phy_info[devno].phy_id];

    if (mipi_rx_check_ext_cfg()) {
        return;
    }

    if (g_dev_phy_info[devno].vsync_cnt > MIPI_RX_START_FRAME_CNT) {
        return;
    }

    if (g_dev_phy_info[devno].recovery_cnt > MIPI_RX_MAX_RECOVER_CNT) {
        return;
    }

    if (g_dev_phy_info[devno].recovery_cnt == MIPI_RX_MAX_RECOVER_CNT) {
        ot_err("dev%d CRC recovery reach MAX_CNT!\n", devno);
    }

    g_dev_phy_info[devno].recovery_cnt++;

    mipi_rx_phy_cfg->phy_test.u32 = is_single_mode ? 0x0 : 0x4;
    if (phy_id == 0) {
        mipi_rx_sys_regs->phycfg_en.u32 = 0x1;
    } else {
        mipi_rx_sys_regs->phycfg_en.u32 = 0x2;
    }

    osal_udelay(100); /* usleep 100us */

    mipi_rx_phy_cfg->phy_test.u32 = is_single_mode ? 0x40 : 0x44;
    if (phy_id == 0) {
        mipi_rx_sys_regs->phycfg_en.u32 = 0x1;
    } else {
        mipi_rx_sys_regs->phycfg_en.u32 = 0x2;
    }

    osal_udelay(100); /* usleep 100us */

    // clear all devs vsync_cnt at same phy
    mipi_rx_clear_vsync_cnt(phy_id);
}

static void mipi_crc_intr_statics(combo_dev_t devno)
{
    unsigned int mipi_crc_intr_int;
    mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

    mipi_crc_intr_int = mipi_ctrl_regs->mipi_crc_intr_st.u32;

    if (mipi_crc_intr_int) { // if error occurs
        mipi_crc_intr_process(devno);
        mipi_ctrl_regs->mipi_crc_intr_raw.u32 = 0xffffffff;
    }

    mipi_crc_intr_count1(devno, mipi_crc_intr_int);
    mipi_crc_intr_count2(devno, mipi_crc_intr_int);
}

static void lvds_err_int_count1(combo_dev_t devno, unsigned int lvds_ctrl_int)
{
    if (lvds_ctrl_int & LVDS_VSYNC) {
        g_lvds_err_int_cnt[devno].lvds_vsync_cnt++;
    }

    if (lvds_ctrl_int & CMD_RD_ERR) {
        g_lvds_err_int_cnt[devno].cmd_rd_err_cnt++;
    }

    if (lvds_ctrl_int & CMD_WR_ERR) {
        g_lvds_err_int_cnt[devno].cmd_wr_err_cnt++;
    }

    if (lvds_ctrl_int & LVDS_POP_ERR) {
        g_lvds_err_int_cnt[devno].pop_err_cnt++;
    }

    if (lvds_ctrl_int & LVDS_STAT_ERR) {
        g_lvds_err_int_cnt[devno].lvds_state_err_cnt++;
    }

    if (lvds_ctrl_int & LINK0_READ_ERR) {
        g_lvds_err_int_cnt[devno].link0_rd_err_cnt++;
    }

    if (lvds_ctrl_int & LINK1_READ_ERR) {
        g_lvds_err_int_cnt[devno].link1_rd_err_cnt++;
    }

    if (lvds_ctrl_int & LINK2_READ_ERR) {
        g_lvds_err_int_cnt[devno].link2_rd_err_cnt++;
    }

    if (lvds_ctrl_int & LINK0_WRITE_ERR) {
        g_lvds_err_int_cnt[devno].link0_wr_err_cnt++;
    }

    if (lvds_ctrl_int & LINK1_WRITE_ERR) {
        g_lvds_err_int_cnt[devno].link1_wr_err_cnt++;
    }

    if (lvds_ctrl_int & LINK2_WRITE_ERR) {
        g_lvds_err_int_cnt[devno].link2_wr_err_cnt++;
    }
}

static void lvds_err_int_count2(combo_dev_t devno, unsigned int lvds_ctrl_int)
{
    if (lvds_ctrl_int & LANE0_SYNC_ERR) {
        g_lvds_err_int_cnt[devno].lane0_sync_err_cnt++;
    }

    if (lvds_ctrl_int & LANE1_SYNC_ERR) {
        g_lvds_err_int_cnt[devno].lane1_sync_err_cnt++;
    }

    if (lvds_ctrl_int & LANE2_SYNC_ERR) {
        g_lvds_err_int_cnt[devno].lane2_sync_err_cnt++;
    }

    if (lvds_ctrl_int & LANE3_SYNC_ERR) {
        g_lvds_err_int_cnt[devno].lane3_sync_err_cnt++;
    }

    if (lvds_ctrl_int & LANE4_SYNC_ERR) {
        g_lvds_err_int_cnt[devno].lane4_sync_err_cnt++;
    }

    if (lvds_ctrl_int & LANE5_SYNC_ERR) {
        g_lvds_err_int_cnt[devno].lane5_sync_err_cnt++;
    }

    if (lvds_ctrl_int & LANE6_SYNC_ERR) {
        g_lvds_err_int_cnt[devno].lane6_sync_err_cnt++;
    }

    if (lvds_ctrl_int & LANE7_SYNC_ERR) {
        g_lvds_err_int_cnt[devno].lane7_sync_err_cnt++;
    }
}

static void lvds_int_statics(combo_dev_t devno)
{
    unsigned int lvds_ctrl_int;
    volatile lvds_ctrl_regs_t *lvds_ctrl_regs = get_lvds_ctrl_regs(devno);

    lvds_ctrl_int = lvds_ctrl_regs->lvds_ctrl_int.u32;

    if (lvds_ctrl_int) {
        lvds_ctrl_regs->lvds_ctrl_int_raw.u32 = 0xffffffff;
    }

    lvds_err_int_count1(devno, lvds_ctrl_int);
    lvds_err_int_count2(devno, lvds_ctrl_int);
}

static void align_int_statis(combo_dev_t devno)
{
    unsigned int align_int;
    volatile global_ctrl_regs_t *global_ctrl_regs = get_global_ctrl_regs(devno);

    align_int = global_ctrl_regs->align_int.u32;

    if (align_int) {
        global_ctrl_regs->align_int_raw.u32 = 0xffffffff;
    }

    if (align_int & ALIGN_FIFO_FULL_ERR) {
        g_align_err_int_cnt[devno].fifo_full_err_cnt++;
    }

    if (align_int & ALIGN_LANE0_ERR) {
        g_align_err_int_cnt[devno].lane0_align_err_cnt++;
    }

    if (align_int & ALIGN_LANE1_ERR) {
        g_align_err_int_cnt[devno].lane1_align_err_cnt++;
    }

    if (align_int & ALIGN_LANE2_ERR) {
        g_align_err_int_cnt[devno].lane2_align_err_cnt++;
    }

    if (align_int & ALIGN_LANE3_ERR) {
        g_align_err_int_cnt[devno].lane3_align_err_cnt++;
    }

    if (align_int & ALIGN_LANE4_ERR) {
        g_align_err_int_cnt[devno].lane4_align_err_cnt++;
    }

    if (align_int & ALIGN_LANE5_ERR) {
        g_align_err_int_cnt[devno].lane5_align_err_cnt++;
    }

    if (align_int & ALIGN_LANE6_ERR) {
        g_align_err_int_cnt[devno].lane6_align_err_cnt++;
    }

    if (align_int & ALIGN_LANE7_ERR) {
        g_align_err_int_cnt[devno].lane7_align_err_cnt++;
    }
}

static int mipi_rx_interrupt_route(int irq, void *dev_id)
{
    int i;
    volatile mipi_rx_sys_regs_t *mipi_rx_sys_regs = get_mipi_rx_sys_regs();

    mipi_rx_unused(irq);
    mipi_rx_unused(dev_id);

    for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
        mipi_rx_phy_cil_int_statis(i);
    }

    for (i = 0; i < MIPI_RX_MAX_DEV_NUM; i++) {
        volatile global_ctrl_regs_t *global_ctrl_regs = get_global_ctrl_regs(i);
        if (global_ctrl_regs->chn_int_raw.u32) {
        } else {
            continue;
        }

        mipi_int_statics(i);
        mipi_crc_intr_statics(i);
        lvds_int_statics(i);
        align_int_statis(i);
        global_ctrl_regs->chn_int_raw.u32 = 0xffffffff;
    }

    mipi_rx_sys_regs->mipi_int_raw.u32 = 0xffffffff;

    return OSAL_IRQ_HANDLED;
}

static int mipi_rx_drv_reg_init(void)
{
    if (g_mipi_rx_regs_va == NULL) {
        g_mipi_rx_regs_va = (mipi_rx_regs_type_t *)osal_ioremap(MIPI_RX_REGS_ADDR, (unsigned int)MIPI_RX_REGS_SIZE);
        if (g_mipi_rx_regs_va == NULL) {
            ot_err("remap mipi_rx reg addr fail\n");
            return -1;
        }
        g_reg_map_flag = 1;
    }

    return 0;
}

static void mipi_rx_drv_reg_exit(void)
{
    if (g_reg_map_flag == 1) {
        if (g_mipi_rx_regs_va != NULL) {
            osal_iounmap((void *)g_mipi_rx_regs_va, MIPI_RX_REGS_SIZE);
            g_mipi_rx_regs_va = NULL;
        }
        g_reg_map_flag = 0;
    }
}

static int mipi_rx_register_irq(void)
{
    int ret;

    ret = osal_irq_request(g_mipi_rx_irq_num, mipi_rx_interrupt_route, NULL, "MIPI_RX", mipi_rx_interrupt_route);
    if (ret < 0) {
        ot_err("mipi_rx: failed to register irq.\n");
        return -1;
    }

    return 0;
}

static void mipi_rx_unregister_irq(void)
{
    osal_irq_free(g_mipi_rx_irq_num, mipi_rx_interrupt_route);
}

/* magic nums convert to register configs */
static void mipi_rx_drv_hw_init(void)
{
    unsigned long mipi_rx_crg_addr;
#ifndef OT_FPGA
    int i;
#endif

    mipi_rx_crg_addr = (unsigned long)osal_ioremap(MIPI_RX_CLK_CRG_ADDR, MIPI_RX_REG_BYTE_WIDTH);

    /* cal clk & cbar clk */
    write_reg32(mipi_rx_crg_addr, 1 << 2, 0x1 << 2);         /* value 2, mask 2 */
    write_reg32(mipi_rx_crg_addr, 1 << 3, 0x1 << 3);         /* value 3, mask 3 */
    /* cil clk & bus clk */
    write_reg32(mipi_rx_crg_addr, 1 << 4, 0x1 << 4);         /* value 4, mask 4 */
    write_reg32(mipi_rx_crg_addr, 1 << 5, 0x1 << 5);         /* value 5, mask 5 */

    /* reset */
    write_reg32(mipi_rx_crg_addr, 1 << 0, 0x1 << 0);
    osal_udelay(10); /* 10us */
    write_reg32(mipi_rx_crg_addr, 0, 0x1 << 0);

    osal_iounmap((void *)mipi_rx_crg_addr, MIPI_RX_REG_BYTE_WIDTH);

#ifndef OT_FPGA
    /* autodeskew default value */
    for (i = 0; i < MIPI_RX_MAX_PHY_NUM; ++i) {
        mipi_rx_set_cil_int_mask(i, MIPI_CIL_INT_MASK);
        mipi_rx_set_phy_skew_link(i, PHY_SKEW_LINK);
        mipi_rx_set_phy_lane_si_link(i, PHY_LANE_SI_LINK);
        mipi_rx_set_phy_eq_link(i, PHY_EQ_LINK);
        mipi_rx_set_phy_cfg_link(i, PHY_CFG_LINK);
        mipi_rx_set_phy_deskew_cal_link(i, PHY_DESKEW_CAL);
        mipi_rx_set_phy_fsmo_link(i, PHY_FSMO_VALUE); // ?
        mipi_rx_set_phy_ext_reg0(i, PHY_EXT_REG0_VALUE);
        mipi_rx_set_phy_test(i, mipi_rx_check_ext_cfg() ? PHY_TEST_VALUE_EXT : PHY_TEST_VALUE);
        mipi_rx_set_phy_dsw(i, PHY_DSW_VALUE);
        mipi_rx_set_phy_offnn(i, PHY_OFFNN_VALUE);
        mipi_rx_set_phy_temrl(i, PHY_TEMRL_VALUE);
    }
    mipi_rx_set_phy_cal_int(0x0);
#endif
}

/* magic nums convert to register configs */
static void mipi_rx_drv_hw_exit(void)
{
    unsigned long mipi_rx_crg_addr;
    mipi_rx_crg_addr = (unsigned long)osal_ioremap(MIPI_RX_CLK_CRG_ADDR, MIPI_RX_REG_BYTE_WIDTH);

    /* reset */
    write_reg32(mipi_rx_crg_addr, 1 << 0, 0x1 << 0);

#ifndef OT_FPGA
    /* cal clk & cbar clk */
    write_reg32(mipi_rx_crg_addr, 0, 0x1 << 2);         /* value 0, mask 2 */
    write_reg32(mipi_rx_crg_addr, 0, 0x1 << 3);         /* value 0, mask 3 */
    /* cil clk & bus clk */
    write_reg32(mipi_rx_crg_addr, 0, 0x1 << 4);         /* value 0, mask 4 */
    write_reg32(mipi_rx_crg_addr, 0, 0x1 << 5);         /* value 0, mask 5 */
#endif

    osal_iounmap((void *)mipi_rx_crg_addr, MIPI_RX_REG_BYTE_WIDTH);
}

int mipi_rx_drv_init(void)
{
    int ret;

    ret = mipi_rx_drv_reg_init();
    if (ret < 0) {
        ot_err("mipi_rx_drv_reg_init fail!\n");
        goto fail0;
    }

    ret = mipi_rx_register_irq();
    if (ret < 0) {
        ot_err("mipi_rx_register_irq fail!\n");
        goto fail1;
    }

    mipi_rx_drv_hw_init();

    return 0;

fail1:
    mipi_rx_drv_reg_exit();
fail0:
    return -1;
}

void mipi_rx_drv_exit(void)
{
    mipi_rx_unregister_irq();
    mipi_rx_drv_reg_exit();
    mipi_rx_drv_hw_exit();
}

#ifdef __cplusplus
#if __cplusplus
}

#endif
#endif /* End of #ifdef __cplusplus */
