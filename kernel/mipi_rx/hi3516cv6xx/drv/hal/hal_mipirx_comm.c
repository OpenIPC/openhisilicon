/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#include "hal_mipirx_comm.h"
#include "hal_mipirx_custom.h"

#define MIPIRX_IOUNMAP_SIZE 4
#define MIPIRX_DTSI_NAME_LEN 12

static td_u32 *g_global_reg_base;
static td_u32 *g_pixel_crg_reg_base;
static td_u32 *g_sys_reg_base;
static td_u32 *g_sensor_reg_base[MIPIRX_PORT_NUM_MAX];
static td_u32 *g_ctrl_reg_base[MIPIRX_PORT_NUM_MAX];
static td_u32 *g_phy_reg_base[MIPIRX_PHY_NUM_MAX];

td_u32 *hal_mipirx_get_global_addr(td_void)
{
    return g_global_reg_base;
}

td_u32 *hal_mipirx_get_pixel_crg_addr(td_void)
{
    return g_pixel_crg_reg_base;
}

td_u32 *hal_mipirx_get_sys_addr(td_void)
{
    return g_sys_reg_base;
}

td_u32 *hal_mipirx_get_sns_addr(td_u8 port_id)
{
    return g_sensor_reg_base[port_id];
}

td_u32 *hal_mipirx_get_ctrl_addr(td_u8 port_id)
{
    return g_ctrl_reg_base[port_id];
}

td_u32 *hal_mipirx_get_phy_addr(td_u8 port_id)
{
    return g_phy_reg_base[port_id];
}

static td_s32 mipirx_get_phy_reg_base(td_void *pdev)
{
    td_s32 ret;
    td_u8 index;
    td_char name[MIPIRX_DTSI_NAME_LEN] = {0};

    for (index = 0; index < MIPIRX_PHY_NUM_MAX; ++index) {
        ret = snprintf_s(name, sizeof(name), sizeof(name) - 1, "phy%d", index);
        if (ret < 0) {
            mipirx_log_err("snprintf_s error!\n");
            return TD_FAILURE;
        }
        if (pdev != TD_NULL) {
            ret = drv_mipirx_get_reg_base(pdev, &g_phy_reg_base[index], name);
            if (ret != TD_SUCCESS) {
                return ret;
            }
        } else {
            g_phy_reg_base[index] = (td_u32 *) osal_ioremap(MIPIRX_PHY0_REG_BASE + index * 0x200, 0x200);
        }
    }
    return TD_SUCCESS;
}

static td_s32 mipirx_get_ctrl_reg_base(td_void *pdev)
{
    td_s32 ret;
    td_u8 index;
    td_char name[MIPIRX_DTSI_NAME_LEN] = {0};

    for (index = 0; index < MIPIRX_PORT_NUM_MAX; ++index) {
        ret = snprintf_s(name, sizeof(name), sizeof(name) - 1, "ctrl%d", index);
        if (ret < 0) {
            mipirx_log_err("snprintf_s error!\n");
            return TD_FAILURE;
        }
        if (MIPIRX_REG_BASE_FROM_DTSI == TD_TRUE) {
            ret = drv_mipirx_get_reg_base(pdev, &g_ctrl_reg_base[index], name);
            if (ret != TD_SUCCESS) {
                return ret;
            }
        } else {
            g_ctrl_reg_base[index] = (td_u32 *) osal_ioremap(MIPIRX_CTRL0_REG_BASE + index * 0x1000, 0x1000);
        }
    }
    return TD_SUCCESS;
}

static td_s32 mipirx_get_sns_reg_base(td_void *pdev)
{
    td_s32 ret;
    td_u8 index;
    td_char name[MIPIRX_DTSI_NAME_LEN] = {0};

    for (index = 0; index < MIPIRX_PORT_NUM_MAX; ++index) {
        ret = snprintf_s(name, sizeof(name), sizeof(name) - 1, "sns%d", index);
        if (ret < 0) {
            mipirx_log_err("snprintf_s error!\n");
            return TD_FAILURE;
        }

        if (pdev != TD_NULL) {
            ret = drv_mipirx_get_reg_base(pdev, &g_sensor_reg_base[index], name);
            if (ret != TD_SUCCESS) {
                return ret;
            }
        } else {
            g_sensor_reg_base[index] = (td_u32 *) osal_ioremap(MIPIRX_SNS0_REG_BASE + index * 0x20, 0x20);
        }
    }
    return TD_SUCCESS;
}

static td_s32 mipirx_get_global_reg_base(td_void *pdev)
{
    td_s32 ret;

    if (pdev != TD_NULL) {
        ret = drv_mipirx_get_reg_base(pdev, &g_global_reg_base, "global");
        if (ret != TD_SUCCESS) {
            return ret;
        }
    } else {
        g_global_reg_base = (td_u32 *) osal_ioremap(MIPIRX_GLOBAL_REG_BASE, 0x200);
    }

    return TD_SUCCESS;
}

static td_s32 mipirx_get_sys_reg_base(td_void *pdev)
{
    td_s32 ret;

    if (pdev != TD_NULL) {
        ret = drv_mipirx_get_reg_base(pdev, &g_sys_reg_base, "sys");
        if (ret != TD_SUCCESS) {
            return ret;
        }
    } else {
        g_sys_reg_base = (td_u32 *) osal_ioremap(MIPIRX_MISC_REG_BASE, 0x100);
    }

    return TD_SUCCESS;
}

static td_s32 mipirx_get_pixel_crg_reg_base(td_void *pdev)
{
    td_s32 ret;

    if (pdev != TD_NULL) {
        ret = drv_mipirx_get_reg_base(pdev, &g_pixel_crg_reg_base, "pixel_crg");
        if (ret != TD_SUCCESS) {
            return ret;
        }
    } else {
        g_pixel_crg_reg_base = (td_u32 *) osal_ioremap(MIPIRX_PIXEL_CRG_REG_BASE, 0x100);
    }

    return TD_SUCCESS;
}

td_s32 hal_mipirx_reg_base_init(td_void *pdev)
{
    td_s32 ret = TD_SUCCESS;

    ret = mipirx_get_phy_reg_base(pdev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = mipirx_get_ctrl_reg_base(pdev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = mipirx_get_sns_reg_base(pdev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = mipirx_get_global_reg_base(pdev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = mipirx_get_sys_reg_base(pdev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = mipirx_get_pixel_crg_reg_base(pdev);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 hal_mipirx_reg_base_deinit(td_void)
{
    td_u8 index;

    if (g_global_reg_base != TD_NULL) {
        osal_iounmap((void *) g_global_reg_base, MIPIRX_IOUNMAP_SIZE);
        g_global_reg_base = TD_NULL;
    }

    if (g_pixel_crg_reg_base != TD_NULL) {
        osal_iounmap((void *) g_pixel_crg_reg_base, MIPIRX_IOUNMAP_SIZE);
        g_pixel_crg_reg_base = TD_NULL;
    }

    if (g_sys_reg_base != TD_NULL) {
        osal_iounmap((void *) g_sys_reg_base, MIPIRX_IOUNMAP_SIZE);
        g_sys_reg_base = TD_NULL;
    }

    for (index = 0; index < MIPIRX_PORT_NUM_MAX; ++index) {
        if (g_sensor_reg_base[index] != TD_NULL) {
            osal_iounmap((void *) g_sensor_reg_base[index], MIPIRX_IOUNMAP_SIZE);
            g_sensor_reg_base[index] = TD_NULL;
        }
    }

    for (index = 0; index < MIPIRX_PORT_NUM_MAX; ++index) {
        if (g_ctrl_reg_base[index] != TD_NULL) {
            osal_iounmap((void *) g_ctrl_reg_base[index], MIPIRX_IOUNMAP_SIZE);
            g_ctrl_reg_base[index] = TD_NULL;
        }
    }

    for (index = 0; index < MIPIRX_PHY_NUM_MAX; ++index) {
        if (g_phy_reg_base[index] != TD_NULL) {
            osal_iounmap((void *) g_phy_reg_base[index], MIPIRX_IOUNMAP_SIZE);
            g_phy_reg_base[index] = TD_NULL;
        }
    }
    return TD_SUCCESS;
}

static td_u32 mipirx_reg_read_32(const td_u32 *addr)
{
    return *(volatile td_u32 *) addr;
}

static td_void mipirx_reg_write_u32(const td_u32 *addr, td_u32 val)
{
    *(volatile td_u32 *) addr = val;
}

static td_u32 mipirx_reg_get_shift(td_u32 mask)
{
    td_u32 index;

    for (index = 0; index < 32; index++) { /* 32: check front 32 data */
        if ((mask & 0x01) == 0x01) {
            break;
        } else {
            mask >>= 1;
        }
    }
    return index;
}

td_u32 hal_mipirx_reg_read(const td_void *base, td_u32 offset)
{
    td_u32 value;

    if (base == TD_NULL) {
        return 0;
    }

    value = mipirx_reg_read_32(base + offset);
    return value;
}

td_void hal_mipirx_reg_write(const td_void *base, td_u32 offset, td_u32 value)
{
    if (base == TD_NULL) {
        return;
    }
    mipirx_reg_write_u32(base + offset, value);
}

td_u32 hal_mipirx_reg_readb(const td_void *base, td_u32 offset, td_u32 mask)
{
    td_u32 reg_val;
    td_u32 shift;

    if (base == TD_NULL) {
        return 0;
    }

    shift = mipirx_reg_get_shift(mask);
    reg_val = hal_mipirx_reg_read(base, offset);
    return (reg_val & mask) >> shift;
}

td_void hal_mipirx_reg_writeb(const td_void *base, td_u32 offset, td_u32 mask, td_u32 val)
{
    td_u32 reg_val;
    td_u32 shift;

    if (base == TD_NULL) {
        return;
    }
    shift = mipirx_reg_get_shift(mask);
    reg_val = hal_mipirx_reg_read(base, offset);
    reg_val &= ~mask;
    reg_val |= (val << shift) & mask;
    hal_mipirx_reg_write(base, offset, reg_val);
}
