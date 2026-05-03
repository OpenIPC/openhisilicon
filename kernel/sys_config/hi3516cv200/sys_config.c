/*
 * Sys Config Driver for HiSilicon BVT SoCs
 *
 * Copyright (c) 2016 HiSilicon Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/bitops.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/of_device.h>


#define PIN_CTRL_NAME "pinctrl-single,pins"

unsigned int vi_vpss_online = 1;
unsigned int pin_mux_select = 0;  // 0:net, 1:vo-BT656, 2:vo-LCD
module_param(vi_vpss_online, uint, S_IRUGO);
module_param(pin_mux_select, uint, S_IRUGO);


static char *sensor = "";
module_param(sensor, charp, S_IRUGO);

static const struct of_device_id hibvt_pinctrl_of_match[] = {
	{.compatible = "hisilicon,sys_config"},
	{},
};

#if 0
static int hibvt_sys_init(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct reg_vals *vals;
	const __be32 *mux;
	unsigned int size, rows, index = 0, found = 0;
	void __iomem *reg_vir_addr;
	const struct of_device_id *id =
						of_match_device(hibvt_pinctrl_of_match, &pdev->dev);
	const char* p_node_name = (id ? id->data : NULL);

	/*when vi-vpss mode is online, the offline compatible string
	would be probe. The same as offline mode.*/
	if (p_node_name)
	{
		if (!strncmp(p_node_name, "flag", 10))
		{
			unsigned int flag;
			if (0 == of_property_read_u32(np, "vi_vpss_online", &flag))
			{
				vi_vpss_online = flag;
			}
			return 0;
		}

        if(((1 == vi_vpss_online) && !strncmp(p_node_name, "offline", 10))
			|| ((0 == vi_vpss_online) && !strncmp(p_node_name, "online", 10)))
        {
		    return 0;
        }
	}

	mux = of_get_property(np, PIN_CTRL_NAME, &size);
	if ((!mux) || (size < sizeof(*mux) * 2)) {
		return -EINVAL;
	}

	size /= sizeof(*mux);	/* Number of elements in array */
	rows = size / 2;

	vals = (struct reg_vals *)kmalloc(sizeof(*vals) * rows, GFP_KERNEL);
	if (!vals)
		return -ENOMEM;

	while (index < size) {
		vals[found].reg_phy_addr = be32_to_cpup(mux + index++);
		vals[found].value = be32_to_cpup(mux + index++);

		reg_vir_addr = ioremap_nocache(vals[found].reg_phy_addr, sizeof(*mux));
        if(reg_vir_addr == NULL)
        {
            printk("ioremap_nocache error\n");
            continue;
        }

		*((volatile int *)(reg_vir_addr)) = vals[found].value;

		iounmap(reg_vir_addr);

		found++;
	}

	kfree(vals);

	return 0;
}
#endif

static int sys_write_reg(unsigned int reg_phy_addr, unsigned int value)
{
    void __iomem *reg_vir_addr;
    reg_vir_addr = ioremap_nocache(reg_phy_addr, sizeof(unsigned int));
    if(reg_vir_addr == NULL)
    {
        printk("ioremap_nocache error\n");
        return -1;
    }

	*((volatile int *)(reg_vir_addr)) = value;

	iounmap(reg_vir_addr);

    return 0;
}

#if 0
static void low_power(void)
{
    //�ر�USB PHY���ܵ�Դ��0x20120078 [12]bit����Ϊ0
    sys_write_reg( 0x20120078, 0x0c0301a0 );

    //�ر�USB 2.0 PHY��ʱ�ӣ�0x200300b8 [7]bit����0
    sys_write_reg( 0x200300b8, 0x127 );

    //SAR ADC POWER_DOWN_MODEL���򿪺��ڲ�ʹ��ADC��ʱ���Զ�power_down��[14]bit����1
    sys_write_reg( 0x200b0000, 0x0000c0ff );

    //��PWM
    sys_write_reg( 0x20030038, 0x2 );

    //�ر�IR [0]bit����0
    sys_write_reg( 0x20070000, 0x0 );

    //IR �ܽŸ��ó�gpio
    sys_write_reg( 0x200f0070, 0x0 );

    //UART2��ʹ�ܣ�0x200A0000 [9][8][0]bit������Ϊ0
    sys_write_reg( 0x200A0030, 0x0 );

    //UART2�ܽŸ��ó�gpio
    sys_write_reg( 0x200f00cc, 0x0 );
    sys_write_reg( 0x200f00d0, 0x0 );

    //�ر�SPI0��SPI1
    sys_write_reg( 0x200C0004, 0x7F00 );
    sys_write_reg( 0x200E0004, 0x7F00 );

    //spi0 �ܽŸ��ó�gpio
    sys_write_reg( 0x200f0040, 0x0 );    // 000��GPIO3_3��
    sys_write_reg( 0x200f0044, 0x0 );    // 000��GPIO3_4��
    sys_write_reg( 0x200f0048, 0x0 );    // 000��GPIO3_5��
    sys_write_reg( 0x200f004c, 0x0 );    // 000��GPIO3_6��

    //spi1 �ܽŸ��ó�gpio
    sys_write_reg( 0x200f0030, 0x0 );    // 000��GPIO0_3��
    sys_write_reg( 0x200f0050, 0x0 );    // 000��GPIO3_7��
    sys_write_reg( 0x200f0054, 0x0 );    // 000��GPIO4_0��
    sys_write_reg( 0x200f0058, 0x0 );    // 000��GPIO4_1��
    sys_write_reg( 0x200f005c, 0x0 );    // 000��GPIO4_2��

    //AUDIO CODEC LINE IN �ر�������
    //sys_write_reg( 0x20050068 0xa8022c2c );
    //sys_write_reg( 0x2005006c 0xf5035a4a );
}

/* I2C0 -> sensor */
static void i2c0_pin_mux(void)
{
	sys_write_reg( 0x200f0040, 0x00000002 );		// 0: GPIO3_3		1:spi0_sclk		2:i2c0_scl
	sys_write_reg( 0x200f0044, 0x00000002 );		// 0: GPIO3_4		1:spi0_sdo		2:i2c0_sda
}

static void i2c2_pin_mux(void)
{
	sys_write_reg( 0x200f0060, 0x1 );			// i2c2_sda
	sys_write_reg( 0x200f0064, 0x1 );			// i2c2_scl
}

/* When external audio codec be used,you must open only one of the i2s_pin_mux(i2s0,i2s1 or i2s2).
*If you has some problem on it ,you can refer to tlv320aic31_readme.txt include in audio sample.*/
static void i2s_pin_mux(void)
{
	// pin_mux with GPIO1
	//sys_write_reg( 0x200f007c, 0x3 );		    // i2s_bclk_tx
	//sys_write_reg( 0x200f0080, 0x3 );		    // i2s_sd_tx
	//sys_write_reg( 0x200f0084, 0x3 );		    // i2s_mclk
	//sys_write_reg( 0x200f0088, 0x3 );		    // i2s_ws_tx
	//sys_write_reg( 0x200f008c, 0x3 );		    // i2s_ws_rx
	//sys_write_reg( 0x200f0090, 0x3 );		    // i2s_bclk_rx
	//sys_write_reg( 0x200f0094, 0x3 );		    // i2s_sd_rx

	// pin_mux with UART1
	sys_write_reg( 0x200f00bc, 0x2 );		    // i2s_sd_tx
	sys_write_reg( 0x200f00c0, 0x2 );		    // i2s_ws_tx
	sys_write_reg( 0x200f00c4, 0x2 );		    // i2s_mclk
	sys_write_reg( 0x200f00c8, 0x2 );		    // i2s_sd_rx
	sys_write_reg( 0x200f00d0, 0x2 );		    // i2s_bclk_tx

	// pin_mux with JTAG
	//sys_write_reg( 0x200f00d4, 0x3 );		    // i2s_mclk
	//sys_write_reg( 0x200f00d8, 0x3 );		    // i2s_ws_tx
	//sys_write_reg( 0x200f00dc, 0x3 );		    // i2s_sd_tx
	//sys_write_reg( 0x200f00e0, 0x3 );		    // i2s_sd_rx
	//sys_write_reg( 0x200f00e4, 0x3 );		    // i2s_bclk_tx
}

#endif

/* VICAP default setting is VIU */
static void vicap_pin_mux(void)
{
	sys_write_reg( 0x200f0000, 0x00000001 );		// 0: GPIO0_4		1: SENSOR_CLK
	sys_write_reg( 0x200f0004, 0x00000000 );		// 0: SENSOR_RSTN	1: GPIO0_5
	sys_write_reg( 0x200f0008, 0x00000001 );		// 0: GPIO0_6		1��FLASH_TRIG	2: SFC_EMMC_BOOT_MODE	3��SPI1_CSN1	4:VI_VS
	sys_write_reg( 0x200f000c, 0x00000000 );		// 0��GPIO0_7	 	1��SHUTTER_TRIG	2��SFC_DEVICE_MODE		4: VI_HS
}

/* SPI1 -> LCD */
static void spi1_pim_mux(void)
{
	sys_write_reg( 0x200f0050, 0x1 );			// 001��SPI1_SCLK��
	sys_write_reg( 0x200f0054, 0x1 );			// 001��SPI1_SDO��
	sys_write_reg( 0x200f0058, 0x1 );			// 001��SPI1_SDI��
	sys_write_reg( 0x200f005c, 0x1 );			// 001��SPI1_CSN0��
}


/* I2C1 -> 7179 */
static void i2c1_pin_mux(void)
{
	sys_write_reg( 0x200f0050, 0x00000002 );		// 010��I2C1_SCL��
	sys_write_reg( 0x200f0054, 0x00000002 );		// 010��I2C1_SDA��
}

static void vo_output_mode(void)
{
	sys_write_reg( 0x200f0010, 0x00000003 );		// 3��VO_CLK   & 0: GPIO2_0  & 1: RMII_CLK
	sys_write_reg( 0x200f0014, 0x00000000 );		// 3��VO_VS    & 0: GPIO2_1  & 1: RMII_TX_EN & 4: SDIO1_CARD_DETECT
	sys_write_reg( 0x200f0018, 0x00000003 );		// 3��VO_DATA5 & 0: GPIO2_2	& 1: RMII_TXD0	& 4: SDIO1_CWPR
	sys_write_reg( 0x200f001c, 0x00000000 );		// 3��VO_DE    & 0: GPIO2_3	& 1: RMII_TXD1	& 4: SDIO1_CDATA1
	sys_write_reg( 0x200f0020, 0x00000003 );		// 3��VO_DATA7 & 0: GPIO2_4	& 1: RMII_RX_DV	& 4: SDIO1_CDATA0
	sys_write_reg( 0x200f0024, 0x00000003 );		// 3��VO_DATA2 & 0: GPIO2_5	& 1: RMII_RXD0	& 4: SDIO1_CDATA3
	sys_write_reg( 0x200f0028, 0x00000003 );		// 3��VO_DATA3 & 0: GPIO2_6	& 1: RMII_RXD1	& 4: SDIO1_CCMD
	sys_write_reg( 0x200f002c, 0x00000000 );		// 3��VO_HS    & 0: GPIO2_7	& 1: EPHY_RST	& 2: BOOT_SEL	& 4: SDIO1_CARD_POWER_EN
	sys_write_reg( 0x200f0030, 0x00000003 );		// 3��VO_DATA0 & 0: GPIO0_3	& 1: SPI1_CSN1
	sys_write_reg( 0x200f0034, 0x00000003 );		// 3��VO_DATA1 & 0: GPIO3_0	& 1: EPHY_CLK	& 4: SDIO1_CDATA2
	sys_write_reg( 0x200f0038, 0x00000003 );		// 3: VO_DATA6 & 0: GPIO3_1	& 1: MDCK		& 2��BOOTROM_SEL
	sys_write_reg( 0x200f003c, 0x00000003 );		// 3��VO_DATA4 & 0: GPIO3_2	& 1: MDIO

}

static void bt656_drive_capability(void)
{
	//BT656 drive capability config
	sys_write_reg( 0x200f0810, 0xd0);    		// VO_CLK
	sys_write_reg( 0x200f0830, 0x90);    		// VO_DATA0
	sys_write_reg( 0x200f0834, 0xd0);    		// VO_DATA1
	sys_write_reg( 0x200f0824, 0x90);    		// VO_DATA2
	sys_write_reg( 0x200f0828, 0x90);    		// VO_DATA3
	sys_write_reg( 0x200f083c, 0x90);    		// VO_DATA4
	sys_write_reg( 0x200f0818, 0x90);    		// VO_DATA5
	sys_write_reg( 0x200f0838, 0x90);    		// VO_DATA6
	sys_write_reg( 0x200f0820, 0x90);    		// VO_DATA7
}

static void lcd_drive_capability(void)
{
	//LCD drive capability config
	sys_write_reg( 0x200f0810, 0xe0);
	sys_write_reg( 0x200f0830, 0xa0);
	sys_write_reg( 0x200f0834, 0xe0);
	sys_write_reg( 0x200f0824, 0xa0);
	sys_write_reg( 0x200f0828, 0xa0);
	sys_write_reg( 0x200f083c, 0xa0);
	sys_write_reg( 0x200f0818, 0xa0);
	sys_write_reg( 0x200f0838, 0xa0);
	sys_write_reg( 0x200f0820, 0xa0);
	sys_write_reg( 0x200f081c, 0xa0);
}

/* RMII */
static void net_rmii_mode(void)
{
	sys_write_reg( 0x200f002c, 0x00000001 );     // 1: EPHY_RST   & 0: GPIO2_7  & 2: BOOT_SEL & 3��VO_HS & 4: SDIO1_CARD_POWER_EN
	sys_write_reg( 0x200f0034, 0x00000001 );     // 1: EPHY_CLK   & 0: GPIO3_0  & 3��VO_DATA1 & 4: SDIO1_CDATA2

	sys_write_reg( 0x200f0010, 0x00000001 );     // 1: RMII_CLK   & 0: GPIO2_0  & 3��VO_CLK
	sys_write_reg( 0x200f0014, 0x00000001 );     // 1: RMII_TX_EN & 0: GPIO2_1  & 3��VO_VS    & 4: SDIO1_CARD_DETECT
	sys_write_reg( 0x200f0018, 0x00000001 );     // 1: RMII_TXD0  & 0: GPIO2_2  & 3��VO_DATA5 & 4: SDIO1_CWPR
	sys_write_reg( 0x200f001c, 0x00000001 );     // 1: RMII_TXD1  & 0: GPIO2_3  & 3��VO_DE    & 4: SDIO1_CDATA1
	sys_write_reg( 0x200f0020, 0x00000001 );     // 1: RMII_RX_DV & 0: GPIO2_4  & 3��VO_DATA7 & 4: SDIO1_CDATA
	sys_write_reg( 0x200f0024, 0x00000001 );     // 1: RMII_RXD0  & 0: GPIO2_5  & 3��VO_DATA2 & 4: SDIO1_CDATA3
	sys_write_reg( 0x200f0028, 0x00000001 );     // 1: RMII_RXD1  & 0: GPIO2_6  & 3��VO_DATA3 & 4: SDIO1_CCMD��

	sys_write_reg( 0x200f0038, 0x00000001 );     // 1: MDCK 	    & 0: GPIO3_1  & 2��BOOTROM_SEL & 3: VO_DATA6
	sys_write_reg( 0x200f003c, 0x00000001 );     // 1: MDIO       & 0: GPIO3_2  & 3��VO_DATA4

	//ephy drive capability config
	sys_write_reg( 0x200f0810, 0xd0 );			// RMII_CLK
	sys_write_reg( 0x200f0814, 0xa0 );			// RMII_TX_EN
	sys_write_reg( 0x200f0818, 0xa0 );			// RMII_TXD0
	sys_write_reg( 0x200f081c, 0xa0 );			// RMII_TXD1
	sys_write_reg( 0x200f0820, 0xb0 );			// RMII_RX_DV
	sys_write_reg( 0x200f0824, 0xb0 );			// RMII_RXD0
	sys_write_reg( 0x200f0828, 0xb0 );			// RMII_RXD1
	sys_write_reg( 0x200f082c, 0xb0 );			// EPHY_RST
	sys_write_reg( 0x200f0834, 0xd0 );			// EPHY_CLK
	sys_write_reg( 0x200f0838, 0x90 );			// MDCK
	sys_write_reg( 0x200f083c, 0xa0 );			// MDIO
}

static void pinmux_hi3518e(void)
{
    if ( 0 == pin_mux_select )
    {
        net_rmii_mode();
    }
    else if ( 1 == pin_mux_select )
    {
        i2c1_pin_mux();
		vo_output_mode();
		bt656_drive_capability();

        /* load peripheral equipment  */
    }
    else if ( 2 == pin_mux_select )
    {
        spi1_pim_mux();
		vo_output_mode();
		lcd_drive_capability();
		sys_write_reg(0x200f0014, 0x00000003);	 // 3: VO_VS    & 0: GPIO2_1  & 1: RMII_TX_EN & 4: SDIO1_CARD_DETECT
		sys_write_reg(0x200f002c, 0x00000003);	 // 3: VO_HS    & 0: GPIO2_7	& 1: EPHY_RST	& 2: BOOT_SEL	& 4: SDIO1_CARD_POWER_EN
		sys_write_reg(0x200f001c, 0x00000003);
        /* load peripheral equipment */
    }
    else
    {
        printk("invalid pin_mux_select %d\n", pin_mux_select);
    }

    //i2c0_pin_mux();
    //i2c2_pin_mux();
    vicap_pin_mux();
    //i2s_pin_mux();
    //vo_bt656_mode();
}

static void clkcfg_hi3518e(void)
{
    sys_write_reg(0x2003002c, 0xc4003);       // VICAP, ISP unreset & clock enable, Sensor clock enable, clk reverse
    #if 0
	sys_write_reg(0x20030034, 0x64ff4);       // 6bit LCD
	sys_write_reg(0x20030034, 0x164ff4);      // 8bit LCD
	sys_write_reg(0x20030034, 0xff4);         // bt656

	sys_write_reg(0x20030040, 0x2000);        // AVC unreset, code also config
	sys_write_reg(0x20030048, 0x2);           // VPSS unreset, code also config
	sys_write_reg(0x20030058, 0x2);           // TDE  unreset
	sys_write_reg(0x2003005c, 0x2);           // VGS
	sys_write_reg(0x20030060, 0x2);           // JPGE unreset
	sys_write_reg(0x20030068, 0x02000000);    // LCD 27M:0x04000000,13.5M:0x02000000
	sys_write_reg(0x2003006c, 0xa);           // IVE/HASH  unreset
	sys_write_reg(0x2003007c, 0x2);           // Cipher
	sys_write_reg(0x200300d4, 0x7);           // GZIP
	sys_write_reg(0x200300d8, 0x2a);          // DDRT Efuse DMA

	sys_write_reg(0x2003008c, 0x2);           // AIO unreset and clock enable , m/f/bclk config in code.
	sys_write_reg(0x20030100, 0x20);          // RSA
	sys_write_reg(0x20030104, 0x0);           // AVC-148.5M VGS-148.5M VPSS-99M
	#endif
}

static void sysctl_hi3518e(void)
{
    //# msic config
    sys_write_reg(0x201200E0, 0xd);				// internal codec,AIO MCLK out, CODEC AIO TX MCLK
    //sys_write_reg(0x201200E0, 0xe);			        // external codec: AIC31, AIO MCLK out, CODEC AIO TX MCLK
#if 1
    //write command priority
	sys_write_reg(0x201100c0, 0x76543210);     // ports0
	sys_write_reg(0x201100c4, 0x76543210);     // ports1
	sys_write_reg(0x201100c8, 0x76543210);     // ports2
	sys_write_reg(0x201100cc, 0x76543210);     // ports3
	sys_write_reg(0x201100d0, 0x76543210);     // ports4
	sys_write_reg(0x201100d4, 0x76543210);     // ports5
	sys_write_reg(0x201100d8, 0x76543210);     // ports6

	//read command priority
	sys_write_reg(0x20110100, 0x76543210);     // ports0
	sys_write_reg(0x20110104, 0x76543210);     // ports1
	sys_write_reg(0x20110108, 0x76543210);     // ports2
	sys_write_reg(0x2011010c, 0x76543210);     // ports3
	sys_write_reg(0x20110110, 0x76543210);     // ports4
	sys_write_reg(0x20110114, 0x76543210);     // ports5
	sys_write_reg(0x20110118, 0x76543210);     // ports6

	//write command timeout
	sys_write_reg(0x20110140, 0x08040200);     // ports0
	sys_write_reg(0x20110144, 0x08040100);     // ports1
	sys_write_reg(0x20110148, 0x08040200);     // ports2
	sys_write_reg(0x2011014c, 0x08040200);     // ports3
	sys_write_reg(0x20110150, 0x08040200);     // ports4
	sys_write_reg(0x20110154, 0x08040200);     // ports5
	sys_write_reg(0x20110158, 0x08040200);     // ports6

	//read command timeout
	sys_write_reg(0x20110180, 0x08040200);     // ports0
	sys_write_reg(0x20110184, 0x08040200);     // ports1
	sys_write_reg(0x20110188, 0x08040200);     // ports2
	sys_write_reg(0x2011018c, 0x08040200);     // ports3
	sys_write_reg(0x20110190, 0x08040200);     // ports4
	sys_write_reg(0x20110194, 0x08040200);     // ports5
	sys_write_reg(0x20110198, 0x08040200);     // ports6

	//map mode
	sys_write_reg(0x20110040, 0x01001000);   // ports0
	sys_write_reg(0x20110044, 0x01001000);   // ports1
	sys_write_reg(0x20110048, 0x01001000);   // ports2
	sys_write_reg(0x2011004c, 0x01001000);   // ports3
	sys_write_reg(0x20110050, 0x01001000);   // ports4
	sys_write_reg(0x20110054, 0x01001000);   // ports5
	sys_write_reg(0x20110058, 0x01001000);   // ports6
#endif

    if ( vi_vpss_online )
    {
        //echo "==============vi_vpss_online==============";
        sys_write_reg(0x20120004, 0x40001000);			// online, SPI1 CS0; [12]-ive
		//#pri config
		sys_write_reg(0x20120058, 0x26666401);			// each module 4bit: vedu       ddrt_md  ive  aio    jpge    tde   vicap  vdp
		sys_write_reg(0x2012005c, 0x66666103);			// each module 4bit: sfc_nand   sfc_nor  nfc  sdio1  sdio0   a7    vpss   vgs
		sys_write_reg(0x20120060, 0x66266666);			// each module 4bit: reserve    reserve  avc  usb    cipher  dma2  dma1   gsf
		//timeout config
		sys_write_reg(0x20120064, 0x00000011);			// each module 4bit: vedu       ddrt_md  ive  aio    jpge    tde   vicap  vdp
		sys_write_reg(0x20120068, 0x00000010);			// each module 4bit: sfc_nand   sfc_nor  nfc  sdio1  sdio0   a7    vpss   vgs
		sys_write_reg(0x2012006c, 0x00000000);			// each module 4bit: reserve    reserve  avc  usb    cipher  dma2  dma1   gsf
    }
    else
    {
        //echo "==============vi_vpss_offline==============";
		sys_write_reg(0x20120004, 0x1000);		   		// offline, mipi SPI1 CS0; [12]-ive
        //# pri config
		sys_write_reg(0x20120058, 0x26666400);			// each module 4bit: vedu       ddrt_md  ive  aio    jpge    tde   vicap  vdp
		sys_write_reg(0x2012005c, 0x66666123);			// each module 4bit: sfc_nand   sfc_nor  nfc  sdio1  sdio0   a7    vpss   vgs
		sys_write_reg(0x20120060, 0x66266666);		// each module 4bit: reserve    reserve  avc  usb    cipher  dma2  dma1   gsf
        //# timeout config
		sys_write_reg(0x20120064, 0x00000011);			// each module 4bit: vedu       ddrt_md  ive  aio    jpge    tde   vicap  vdp
		sys_write_reg(0x20120068, 0x00000000);			// each module 4bit: sfc_nand   sfc_nor  nfc  sdio1  sdio0   a7    vpss   vgs
		sys_write_reg(0x2012006c, 0x00000000);			// each module 4bit: reserve    reserve  avc  usb    cipher  dma2  dma1   gsf
    }

    // ive utili
    //sys_write_reg(0x206A0000, 0x2);				       // Open utili statistic
    //sys_write_reg(0x206A0080, 0x11E1A300);  		   // Utili peri,default 0x11E1A300 cycle
}

static void insert_sns(void)
{

    if (!strcmp(sensor, "9m034") || !strcmp(sensor, "ar0130"))
    {
		sys_write_reg(0x200f0040, 0x2);    			// I2C0_SCL
		sys_write_reg(0x200f0044, 0x2);    			// I2C0_SDA

		//cmos pinmux
		sys_write_reg(0x200f007c, 0x1);    			// VI_DATA13
		sys_write_reg(0x200f0080, 0x1);    			// VI_DATA10
		sys_write_reg(0x200f0084, 0x1);    			// VI_DATA12
		sys_write_reg(0x200f0088, 0x1);    			// VI_DATA11
		sys_write_reg(0x200f008c, 0x2);    			// VI_VS
		sys_write_reg(0x200f0090, 0x2);    			// VI_HS
		sys_write_reg(0x200f0094, 0x1);    			// VI_DATA9

		sys_write_reg(0x2003002c, 0xb4001);			// sensor unreset, clk 27MHz, VI 99MHz
    }
    else if (!strcmp(sensor, "ar0230"))
    {
		sys_write_reg(0x200f0040, 0x2);    			// I2C0_SCL
		sys_write_reg(0x200f0044, 0x2);    			// I2C0_SDA

        sys_write_reg(0x20030104, 0x1);           // AVC-148.5M VGS-148.5M VPSS-99M
		sys_write_reg(0x2003002c, 0xb4005);			// sensor unreset, clk 27MHz, VI 148.5MHz
    }
    else if (!strcmp(sensor, "imx222"))
    {
		sys_write_reg(0x200f0040, 0x1);    				// SPI0_SCLK
		sys_write_reg(0x200f0044, 0x1);    				// SPI0_SDO
		sys_write_reg(0x200f0048, 0x1);    				// SPI0_SDI
		sys_write_reg(0x200f004c, 0x1);    				// SPI0_CSN

		//cmos pinmux
		sys_write_reg(0x200f007c, 0x1);    			// VI_DATA13
		sys_write_reg(0x200f0080, 0x1);    			// VI_DATA10
		sys_write_reg(0x200f0084, 0x1);    			// VI_DATA12
		sys_write_reg(0x200f0088, 0x1);    			// VI_DATA11
		sys_write_reg(0x200f008c, 0x2);    			// VI_VS
		sys_write_reg(0x200f0090, 0x2);    			// VI_HS
		sys_write_reg(0x200f0094, 0x1);    			// VI_DATA9

		sys_write_reg(0x2003002c, 0x94001);			// sensor unreset, clk 37.125MHz, VI 99MHz
		//insmod extdrv/sensor_spi.ko;
    }
    else if (!strcmp(sensor, "ov9712"))
    {
		sys_write_reg(0x200f0040, 0x2);    			// I2C0_SCL
		sys_write_reg(0x200f0044, 0x2);    			// I2C0_SDA

		//cmos pinmux
		sys_write_reg(0x200f007c, 0x1);    			// VI_DATA13
		sys_write_reg(0x200f0080, 0x1);    			// VI_DATA10
		sys_write_reg(0x200f0084, 0x1);    			// VI_DATA12
		sys_write_reg(0x200f0088, 0x1);    			// VI_DATA11
		sys_write_reg(0x200f008c, 0x2);    			// VI_VS
		sys_write_reg(0x200f0090, 0x2);    			// VI_HS
		sys_write_reg(0x200f0094, 0x1);    			// VI_DATA9

		sys_write_reg(0x2003002c, 0xc4001);			// sensor unreset, clk 24MHz, VI 99MHz
    }
	else if (!strcmp(sensor, "ov9752") || !strcmp(sensor, "ov9750"))
	{
		sys_write_reg(0x200f0040, 0x2);    			// I2C0_SCL
		sys_write_reg(0x200f0044, 0x2);    			// I2C0_SDA

		sys_write_reg(0x2003002c, 0xc4001);			// sensor unreset, clk 24MHz, VI 99MHz
	}
    else if (!strcmp(sensor, "mn34220"))
    {
		sys_write_reg(0x200f0040, 0x2);    			// I2C0_SCL
		sys_write_reg(0x200f0044, 0x2);    			// I2C0_SDA

		sys_write_reg(0x2003002c, 0xc4001);			// sensor unreset, clk 24MHz, VI 99MHz
    }
    else if (!strcmp(sensor, "mn34222"))
    {
		sys_write_reg(0x200f0040, 0x2);    			// I2C0_SCL
		sys_write_reg(0x200f0044, 0x2);    			// I2C0_SDA

		sys_write_reg(0x2003002c, 0x94001);			// sensor unreset, clk 37.125MHz, VI 99MHz
    }
	else if (!strcmp(sensor, "ov4682"))
	{
		sys_write_reg(0x200f0040, 0x2);    			// I2C0_SCL
		sys_write_reg(0x200f0044, 0x2);    			// I2C0_SDA

		sys_write_reg(0x2003002c, 0xc4001);			// sensor unreset, clk 24MHz, VI 99MHz
	}
    else if (!strcmp(sensor, "ov9732"))
	{
		sys_write_reg(0x200f0040, 0x2);    			// I2C0_SCL
		sys_write_reg(0x200f0044, 0x2);    			// I2C0_SDA

		//cmos pinmux
		sys_write_reg(0x200f007c, 0x1);    			// VI_DATA13
		sys_write_reg(0x200f0080, 0x1);    			// VI_DATA10
		sys_write_reg(0x200f0084, 0x1);    			// VI_DATA12
		sys_write_reg(0x200f0088, 0x1);    			// VI_DATA11
		sys_write_reg(0x200f008c, 0x2);    			// VI_VS
		sys_write_reg(0x200f0090, 0x2);    			// VI_HS
		sys_write_reg(0x200f0094, 0x1);    			// VI_DATA9

		sys_write_reg(0x2003002c, 0xc4001);			// sensor unreset, clk 24MHz, VI 99MHz
	}
    else if (!strcmp(sensor, "jxf22"))
	{
		sys_write_reg(0x200f0040, 0x2);    			// I2C0_SCL
		sys_write_reg(0x200f0044, 0x2);    			// I2C0_SDA

		//cmos pinmux
		sys_write_reg(0x200f007c, 0x1);    			// VI_DATA13
		sys_write_reg(0x200f0080, 0x1);    			// VI_DATA10
		sys_write_reg(0x200f0084, 0x1);    			// VI_DATA12
		sys_write_reg(0x200f0088, 0x1);    			// VI_DATA11
		sys_write_reg(0x200f008c, 0x2);    			// VI_VS
		sys_write_reg(0x200f0090, 0x2);    			// VI_HS
		sys_write_reg(0x200f0094, 0x1);    			// VI_DATA9

		sys_write_reg(0x2003002c, 0xc4001);			// sensor unreset, clk 24MHz, VI 99MHz
	}
    else if (!strcmp(sensor, "ov2718"))
	{
		sys_write_reg(0x200f0040, 0x2);    			// I2C0_SCL
		sys_write_reg(0x200f0044, 0x2);    			// I2C0_SDA

		sys_write_reg(0x2003002c, 0xc4001);			// sensor unreset, clk 24MHz, VI 99MHz
	}
    else if (!strcmp(sensor, "ov2710_mipi"))
	{
		// MIPI variant — uses libsns_ov2710_mipi.so via ov2710_mipi_1080p.ini
		sys_write_reg(0x200f0040, 0x2);    			// I2C0_SCL
		sys_write_reg(0x200f0044, 0x2);    			// I2C0_SDA

		sys_write_reg(0x2003002c, 0xc4001);			// sensor unreset, clk 24MHz, VI 99MHz
	}
    else if (!strcmp(sensor, "ov2710_dc"))
	{
		// DVP/parallel variant — uses libsns_ov2710_dc.so via ov2710_dc_1080p.ini
		sys_write_reg(0x200f0040, 0x2);    			// I2C0_SCL
		sys_write_reg(0x200f0044, 0x2);    			// I2C0_SDA

		sys_write_reg(0x200f007c, 0x1);    			// VI_DATA13
		sys_write_reg(0x200f0080, 0x1);    			// VI_DATA10
		sys_write_reg(0x200f0084, 0x1);    			// VI_DATA12
		sys_write_reg(0x200f0088, 0x1);    			// VI_DATA11
		sys_write_reg(0x200f008c, 0x2);    			// VI_VS
		sys_write_reg(0x200f0090, 0x2);    			// VI_HS
		sys_write_reg(0x200f0094, 0x1);    			// VI_DATA9

		sys_write_reg(0x2003002c, 0xc4001);			// sensor unreset, clk 24MHz, VI 99MHz
	}
    else if (!strcmp(sensor, "bt1120"))
    {
        sys_write_reg( 0x200f0008, 0x4 );    			// VI_VS
        sys_write_reg( 0x200f000c, 0x4 );    			// VI_HS
        sys_write_reg( 0x200f007c, 0x1 );    			// VI_DATA13
        sys_write_reg( 0x200f0080, 0x1 );    			// VI_DATA10
        sys_write_reg( 0x200f0084, 0x1 );    			// VI_DATA12
        sys_write_reg( 0x200f0088, 0x1 );    			// VI_DATA11
        sys_write_reg( 0x200f008c, 0x1 );    			// VI_DATA15
        sys_write_reg( 0x200f0090, 0x1 );    			// VI_DATA14
        sys_write_reg( 0x200f0094, 0x1 );    			// VI_DATA9

        sys_write_reg( 0x2003002c, 0x94003 );			// sensor unreset, clk 24MHz, VI 99MHz
    }
    else
    {
        printk("sensor_type '%s' is error!!!\n", sensor);
    }

    printk("The sensor_type '%s' !!!\n", sensor);
}

static int sys_config_init(struct platform_device *pdev)
{
    pinmux_hi3518e();
    clkcfg_hi3518e();
    sysctl_hi3518e();
    insert_sns();
    printk("sys config init ok!\n");
    return 0;
}

#ifdef CONFIG_PM
static int hibvt_pinctrl_suspend(struct platform_device *pdev, pm_message_t state)
{
	return 0;
}

static int hibvt_pinctrl_resume(struct platform_device *pdev)
{
	return sys_config_init(pdev);
}
#endif

MODULE_DEVICE_TABLE(of, hibvt_pinctrl_of_match);
static int __init hibvt_pinctrl_probe(struct platform_device *pdev)
{
	return sys_config_init(pdev);

}

static int hibvt_pinctrl_remove(struct platform_device *pdev)
{
    printk("sys config deinit ok!\n");
	return 0;
}

static struct platform_driver hibvt_pinctrl_driver = {
	.driver = {
		.name = "sys_config",
        .owner = THIS_MODULE,
		.of_match_table = of_match_ptr(hibvt_pinctrl_of_match),
	},
	.probe = hibvt_pinctrl_probe,
	.remove	= hibvt_pinctrl_remove,
#ifdef CONFIG_PM
	.suspend        = hibvt_pinctrl_suspend,
	.resume         = hibvt_pinctrl_resume,
#endif
};
module_platform_driver(hibvt_pinctrl_driver);


MODULE_AUTHOR("HiMPP GRP");
MODULE_DESCRIPTION("HiSilicon BVT Sys Config Driver");
MODULE_LICENSE("GPL");

