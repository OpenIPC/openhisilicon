/* SPDX-License-Identifier: GPL-2.0 */
/*
 * cv500 NNIE HW register map (phys base 0x11100000, ioremap'd in
 * nnie_init.c as g_nnie_regs).
 *
 * Decoded from vendor hal_svp_nnie_* helpers in hi_nnie.o (109 KB
 * blob). Each helper takes a core_id (always 0 on cv500 — only one
 * NNIE engine present) and indexes .LANCHOR1[core_id] which holds a
 * { padding, void *regs } pair. The `regs` field is the ioremap'd
 * virtual base of the 0x11100000 register window. The functions are
 * thin shims around `readl/writel` at fixed offsets:
 *
 *   hal_svp_nnie_write_task_addr     @0xbb10 → [+0x20], [+0x24]
 *   hal_svp_nnie_start               @0xbb2c → [+0x30] |= 1
 *   hal_svp_nnie_get_irq_status      @0xbb4c → readl([+0x3c])
 *   hal_svp_nnie_clear_irq           @0xbb64 → [+0x38] |= 1|2|4
 *   hal_svp_nnie_get_task_id         @0xbb9c → readl([+0x48])
 *   hal_svp_nnie_cfg_irq             @0xbbb4 → [+0x34] |= 1|2|4
 *   hal_svp_nnie_set_timeout         @0xbbec → [+0x28], [+0x2c]
 *   hal_svp_nnie_enable_clk_gt       @0xbc08 → [+0x50] |= 0x80
 *   hal_svp_nnie_set_mem_speed       @0xbc28 → no-op (bx lr) ✓
 *   hal_svp_nnie_set_outstanding     @0xbc2c → [+0x54] = 0xF | 0xF00
 *   hal_svp_nnie_get_cfg_err_info    @0xbc5c → readl([+0x40])
 *   hal_svp_nnie_disable_check_sum   @0xbc74 → [+0x68] &= ~1
 *   hal_svp_nnie_enable_ram          @0xb8f4 → cmpi mod 51 fn 0xd1
 *                                              (NOT in NNIE window — goes
 *                                              through sys @0x12020034,
 *                                              decoded in Phase 3)
 *
 * Note: set_mem_speed is a literal no-op on cv500 (single `bx lr`),
 * confirming the Phase 3 finding that the IVE-style "[0x90] mem-priority
 * knob" doesn't apply to NNIE on cv500 — vendor doesn't write any
 * such register here.
 *
 * Dispatch sequence (vendor drv_svp_nnie_start @0xb3ac):
 *
 *   drv_svp_nnie_start(core_id) {
 *       hal_svp_nnie_write_task_addr(core_id, task_phys_lo, task_phys_hi);
 *       osal_dmb();    // wmb()
 *       hal_svp_nnie_start(core_id);   // [+0x30] |= 1
 *   }
 *
 * IRQ handler decode (svp_nnie_irq_route @0x2980, partial):
 *
 *   status = hal_svp_nnie_get_irq_status(core_id);
 *   if (status & 1) → finish (drv_svp_nnie_is_finish_irq @b450)
 *   if (status & 2) → timeout (drv_svp_nnie_is_timeout_irq @b3d0 — logs
 *                              error and returns 1)
 *   if (status & 4) → cfg_err (drv_svp_nnie_is_cfg_err_irq @b410 — logs)
 *   hal_svp_nnie_clear_irq(core_id);  // ack all 3 bits
 */

#ifndef _NNIE_HW_REGS_H_
#define _NNIE_HW_REGS_H_

/* Register offsets within the 0x11100000 NNIE register window */

#define NNIE_REG_TASK_ADDR_LO       0x20  /* task descriptor phys[31:0] */
#define NNIE_REG_TASK_ADDR_HI       0x24  /* task descriptor phys[63:32] */
#define NNIE_REG_TIMEOUT_LO         0x28  /* timeout cycles  [31:0]      */
#define NNIE_REG_TIMEOUT_HI         0x2C  /* timeout cycles  [63:32]     */
#define NNIE_REG_START              0x30  /* bit 0 = go (HW-clears on done) */
#define NNIE_REG_IRQ_CFG            0x34  /* bits 0/1/2 = finish/to/cfg_err en */
#define NNIE_REG_IRQ_CLEAR          0x38  /* bits 0/1/2 = w1c of pending */
#define NNIE_REG_IRQ_STATUS         0x3C  /* bits 0/1/2 = pending finish/to/err */
#define NNIE_REG_CFG_ERR_INFO       0x40  /* config error details */
#define NNIE_REG_TASK_ID            0x48  /* current/completed task id */
#define NNIE_REG_CLK_GATE           0x50  /* bit 7 (=0x80) = clk gate enable */
#define NNIE_REG_OUTSTANDING        0x54  /* AXI outstanding: bf [4:0]=0xF, [11:8]=0xF */
#define NNIE_REG_CHECK_SUM          0x68  /* bit 0 = checksum enable */

/* Bit definitions */

#define NNIE_START_GO               (1u << 0)

#define NNIE_IRQ_FINISH             (1u << 0)
#define NNIE_IRQ_TIMEOUT            (1u << 1)
#define NNIE_IRQ_CFG_ERR            (1u << 2)
#define NNIE_IRQ_ALL                (NNIE_IRQ_FINISH | NNIE_IRQ_TIMEOUT | NNIE_IRQ_CFG_ERR)

#define NNIE_CLK_GATE_EN            (1u << 7)

/* OUTSTANDING register field encoding (from hal_svp_nnie_set_outstanding):
 * vendor sets [4:0]=0xF (max read outstanding) and [11:8]=0xF (max write
 * outstanding). Other bits left at hw reset. */
#define NNIE_OUTSTANDING_DEFAULT    (0x0Fu | (0x0Fu << 8))

#define NNIE_CHECK_SUM_EN           (1u << 0)

#endif /* _NNIE_HW_REGS_H_ */
