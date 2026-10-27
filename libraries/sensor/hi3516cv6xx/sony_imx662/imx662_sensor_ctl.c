/*
 * imx662_sensor_ctl.c — IMX662 low-level transport + power-on register init.
 *
 * Implements the standard HiSilicon sensor transport (Linux /dev/i2c-N +
 * OT_I2C_SLAVE_FORCE) and the per-mode register init the ISP calls at stream
 * start. The bulk power-on "reserved register" block lives in imx662_cfg.h,
 * transcribed from pauliustumas/imx662 and cross-checked against Sony's
 * IMX662_Register.xlsx; imx662_write_init_seq() writes it in standby, before
 * the per-mode registers below, so the per-mode writes win where they overlap.
 */

#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef OT_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "ot_i2c.h"
#endif
#include "securec.h"

#include "imx662_cmos.h"
#include "imx662_cfg.h"

#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8

/* Zero-initialized storage keeps this C99-portable without baking the SDK's
 * pipe count into an explicit {-1, ...} initializer.  Store fd + 1 so zero
 * remains the closed sentinel (open() may legally return descriptor 0). */
static int g_fd_plus_one[OT_ISP_MAX_PIPE_NUM];

static int imx662_i2c_fd(ot_vi_pipe vi_pipe)
{
	return g_fd_plus_one[vi_pipe] - 1;
}

td_s32 imx662_i2c_init(ot_vi_pipe vi_pipe)
{
	int fd;

	if (g_fd_plus_one[vi_pipe] != 0) {
		return TD_SUCCESS;
	}
#ifdef OT_GPIO_I2C
	fd = open("/dev/gpioi2c_ex", O_RDONLY, S_IRUSR);
	if (fd < 0) {
		isp_err_trace("Open gpioi2c_ex error!\n");
		return TD_FAILURE;
	}
	g_fd_plus_one[vi_pipe] = fd + 1;
#else
	td_s32 ret;
	char dev_file[I2C_DEV_FILE_NUM] = {0};
	td_u8 dev_num;
	ot_isp_sns_commbus *bus = imx662_get_bus_info(vi_pipe);

	dev_num = bus->i2c_dev;
	(td_void)snprintf_s(dev_file, sizeof(dev_file), sizeof(dev_file) - 1, "/dev/i2c-%u", dev_num);

	fd = open(dev_file, O_RDWR, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		isp_err_trace("Open /dev/i2c-%u error!\n", dev_num);
		return TD_FAILURE;
	}

	ret = ioctl(fd, OT_I2C_SLAVE_FORCE, (IMX662_I2C_ADDR >> 1));
	if (ret < 0) {
		isp_err_trace("I2C_SLAVE_FORCE error!\n");
		close(fd);
		return ret;
	}
	g_fd_plus_one[vi_pipe] = fd + 1;
#endif
	return TD_SUCCESS;
}

td_s32 imx662_i2c_exit(ot_vi_pipe vi_pipe)
{
	if (g_fd_plus_one[vi_pipe] != 0) {
		close(imx662_i2c_fd(vi_pipe));
		g_fd_plus_one[vi_pipe] = 0;
		return TD_SUCCESS;
	}
	return TD_FAILURE;
}

td_s32 imx662_read_register(ot_vi_pipe vi_pipe, td_u32 addr)
{
	/* Read path not required for open-loop ISP control; wire up if needed. */
	ot_unused(vi_pipe);
	ot_unused(addr);
	return TD_SUCCESS;
}

td_s32 imx662_write_register(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 data)
{
	int fd;

	if (g_fd_plus_one[vi_pipe] == 0) {
		return TD_SUCCESS;
	}
	fd = imx662_i2c_fd(vi_pipe);
#ifdef OT_GPIO_I2C
	i2c_data.dev_addr      = IMX662_I2C_ADDR;
	i2c_data.reg_addr      = addr;
	i2c_data.addr_byte_num = IMX662_ADDR_BYTE;
	i2c_data.data          = data;
	i2c_data.data_byte_num = IMX662_DATA_BYTE;
	if (ioctl(fd, GPIO_I2C_WRITE, &i2c_data)) {
		isp_err_trace("GPIO-I2C write failed!\n");
		return TD_FAILURE;
	}
#else
	td_u32 idx = 0;
	td_u8 buf[I2C_BUF_NUM];

	buf[idx++] = (addr >> 8) & 0xff;   /* IMX662_ADDR_BYTE == 2 */
	buf[idx++] = addr & 0xff;
	buf[idx++] = data & 0xff;          /* IMX662_DATA_BYTE == 1 */

	if (write(fd, buf, IMX662_ADDR_BYTE + IMX662_DATA_BYTE) < 0) {
		isp_err_trace("I2C_WRITE error!\n");
		return TD_FAILURE;
	}
#endif
	return TD_SUCCESS;
}

static void delay_ms(int ms)
{
	usleep(ms * 1000);
}

td_void imx662_standby(ot_vi_pipe vi_pipe)
{
	(td_void)imx662_write_register(vi_pipe, IMX662_REG_STANDBY, 0x01); /* STANDBY */
	(td_void)imx662_write_register(vi_pipe, IMX662_REG_XMSTA,   0x01); /* master stop */
}

td_void imx662_restart(ot_vi_pipe vi_pipe)
{
	(td_void)imx662_write_register(vi_pipe, IMX662_REG_STANDBY, 0x00); /* operating */
	(td_void)imx662_write_register(vi_pipe, IMX662_REG_XMSTA,   0x00); /* master start */
}

td_void imx662_mirror_flip(ot_vi_pipe vi_pipe, ot_isp_sns_mirrorflip_type sns_mirror_flip)
{
	td_u8 h = 0, v = 0;
	switch (sns_mirror_flip) {
		case ISP_SNS_MIRROR:          h = 1; v = 0; break;
		case ISP_SNS_FLIP:            h = 0; v = 1; break;
		case ISP_SNS_MIRROR_FLIP:     h = 1; v = 1; break;
		case ISP_SNS_NORMAL:
		default:                         h = 0; v = 0; break;
	}
	(td_void)imx662_write_register(vi_pipe, IMX662_REG_HREVERSE, h);
	(td_void)imx662_write_register(vi_pipe, IMX662_REG_VREVERSE, v);
	/* NOTE: flipping changes the Bayer start phase — keep in sync with the
	 * bayer setting in cmos_get_isp_default(). VERIFY on hardware. */
}

td_void imx662_blc_clamp(ot_vi_pipe vi_pipe, ot_isp_sns_blc_clamp blc_clamp)
{
	/* IMX662 digital black-level clamp. VERIFY register/semantics on silicon. */
	(td_void)imx662_write_register(vi_pipe, IMX662_REG_DIG_CLAMP,
								   (blc_clamp.blc_clamp_en == TD_TRUE) ? 0x01 : 0x00);
}

/* ---- power-on register block ---------------------------------------------
 * The Sony power-on sequence (reserved analog/ADC trim, MIPI TX timing,
 * readout window, HDR context parked for linear) lives in imx662_cfg.h and
 * runs first, in standby. Everything below it is per-mode state that
 * overrides the table where the two overlap -- notably the 0x3A50/51/52
 * AD-timing triplet, which the table carries in its 12-bit form.
 */
static td_s32 imx662_write_init_seq(ot_vi_pipe vi_pipe)
{
	td_s32 ret = 0;
	td_u32 i;

	for (i = 0; i < IMX662_INIT_SEQ_LEN; i++) {
		ret += imx662_write_register(vi_pipe, g_imx662_init_seq[i].addr,
									 g_imx662_init_seq[i].data);
	}
	return ret;
}

static td_s32 imx662_linear_1080p_init(ot_vi_pipe vi_pipe, td_u32 hmax,
									   td_bool raw_10bit, td_bool overclock_100fps)
{
	td_s32 ret = 0;

	ret += imx662_write_register(vi_pipe, IMX662_REG_STANDBY,  0x01); /* stop for cfg */
	delay_ms(1);

	ret += imx662_write_init_seq(vi_pipe);

	/* --- clock / interface (VERIFY against board wiring) --- */
	/* 100 fps follows the public IMX662 mode: feed 27 MHz externally while
	 * selecting the 24 MHz profile (0x04), which raises its 1440 Mbps timing
	 * to 1620 Mbps. Lower modes retain the hardware-verified 37.125 MHz
	 * profile. The sys_config clock must match this selection: OpenIPC's
	 * sys_config feeds an imx662 37.125 MHz, so 100 fps also needs that
	 * changed to 27 MHz. */
	ret += imx662_write_register(vi_pipe, IMX662_REG_INCK_SEL,
								 overclock_100fps ? 0x04 : 0x01);
	ret += imx662_write_register(vi_pipe, IMX662_REG_DATARATE, 0x03); /* data rate  */ /* VERIFY */
	ret += imx662_write_register(vi_pipe, IMX662_REG_LANEMODE, 0x03); /* 4-lane     */ /* VERIFY */
	ret += imx662_write_register(vi_pipe, IMX662_REG_ADBIT, raw_10bit ? 0x00 : 0x01);
	ret += imx662_write_register(vi_pipe, IMX662_REG_ODBIT, raw_10bit ? 0x00 : 0x01);
	ret += imx662_write_register(vi_pipe, IMX662_REG_WDMODE,   0x00); /* linear     */

	/* --- default frame timing (VMAX/HMAX for 1080p30) --- */
	ret += imx662_write_register(vi_pipe, IMX662_REG_VMAX_L, IMX662_VMAX_1080P30_LINEAR & 0xff);
	ret += imx662_write_register(vi_pipe, IMX662_REG_VMAX_M, (IMX662_VMAX_1080P30_LINEAR >> 8) & 0xff);
	ret += imx662_write_register(vi_pipe, IMX662_REG_VMAX_H, (IMX662_VMAX_1080P30_LINEAR >> 16) & 0x0f);
	/* Power-on HMAX is 990 on this board (60 fps).  The 30 fps linear mode
	 * uses HMAX 1980 with the verified 37.125 MHz input clock. */
	ret += imx662_write_register(vi_pipe, IMX662_REG_HMAX_L, hmax & 0xff);
	ret += imx662_write_register(vi_pipe, IMX662_REG_HMAX_H, (hmax >> 8) & 0xff);

	/* The 90 fps full-frame mode uses the reference driver's 10-bit ADC
	 * timing. 30/60 fps remain native RAW12. */
	ret += imx662_write_register(vi_pipe, IMX662_REG_AD10_0, raw_10bit ? 0x62 : 0xff);
	ret += imx662_write_register(vi_pipe, IMX662_REG_AD10_1, raw_10bit ? 0x01 : 0x03);
	ret += imx662_write_register(vi_pipe, IMX662_REG_AD10_2, raw_10bit ? 0x19 : 0x00);

	/* --- black level --- */
	/* BLKLEVEL is [11:0] across 30DC/30DD, so 30DD's low nibble carries
	 * BLKLEVEL[11:8] and its top nibble is reserved with bit 6 documented
	 * "Fixed to 1h" (Sony's map gives 30DDh an initial value of 40h, and the
	 * register list's own changelog calls out "Revised: Initial value of
	 * fixed register (30DDh)").  0x40 therefore keeps BLKLEVEL at 50 and
	 * stops clearing a fixed bit -- the black level itself does not move. */
	ret += imx662_write_register(vi_pipe, IMX662_REG_BLKLEVEL_L, 0x32); /* 50 */
	ret += imx662_write_register(vi_pipe, IMX662_REG_BLKLEVEL_H, 0x40);

	return ret;
}

td_void imx662_default_reg_init(ot_vi_pipe vi_pipe)
{
	ot_isp_sns_state *sns_state = imx662_get_ctx(vi_pipe);

	switch (sns_state->img_mode) {
		case IMX662_SENSOR_2M_100FPS_10BIT_LINEAR_MODE:
			(td_void)imx662_linear_1080p_init(vi_pipe,
											  IMX662_HMAX_1080P100_LINEAR,
											  TD_TRUE, TD_TRUE);
			break;
		case IMX662_SENSOR_2M_90FPS_10BIT_LINEAR_MODE:
			(td_void)imx662_linear_1080p_init(vi_pipe,
											  IMX662_HMAX_1080P90_LINEAR,
											  TD_TRUE, TD_FALSE);
			break;
		case IMX662_SENSOR_2M_60FPS_12BIT_LINEAR_MODE:
			(td_void)imx662_linear_1080p_init(vi_pipe,
											  IMX662_HMAX_1080P60_LINEAR,
											  TD_FALSE, TD_FALSE);
			break;
		case IMX662_SENSOR_2M_30FPS_12BIT_LINEAR_MODE:
		default:
			(td_void)imx662_linear_1080p_init(vi_pipe,
											  IMX662_HMAX_1080P30_LINEAR,
											  TD_FALSE, TD_FALSE);
			break;
	}

	/* Push any ISP-computed exposure/gain/vmax registers, then start. */
	(td_void)imx662_write_register(vi_pipe, IMX662_REG_STANDBY, 0x00);
	(td_void)imx662_write_register(vi_pipe, IMX662_REG_XMSTA,   0x00);
}

td_void imx662_init(ot_vi_pipe vi_pipe)
{
	(td_void)imx662_i2c_init(vi_pipe);
	imx662_default_reg_init(vi_pipe);
	delay_ms(2);
	ot_isp_sns_state *sns_state = imx662_get_ctx(vi_pipe);
	sns_state->init = TD_TRUE;
}

td_void imx662_exit(ot_vi_pipe vi_pipe)
{
	(td_void)imx662_i2c_exit(vi_pipe);
}
