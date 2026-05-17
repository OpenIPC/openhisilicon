/* SPDX-License-Identifier: GPL-2.0 */
/*
 * cv500 NNIE HW register map (phys base 0x11100000).
 *
 * Per-task dispatch sequence:
 *   1. Write 64-byte task descriptor to MMZ-backed phys address
 *   2. CLK_GATE |= 0x80, OUTSTANDING |= 0xF|0xF00, IRQ_CFG |= 0x7
 *   3. TIMEOUT_LO = 0xFFFFFFFF, TIMEOUT_HI = 0xFF
 *   4. CHECK_SUM &= ~1 (clear), IRQ_CLEAR |= 0x7 (w1c stale pending)
 *   5. TASK_ADDR_LO/HI = descriptor phys
 *   6. wmb()
 *   7. START |= 1   (HW clears on completion)
 *   8. wait for IRQ: STATUS bit 0 = finish, bit 1 = timeout, bit 2 = cfg_err
 */

#ifndef _NNIE_HW_REGS_H_
#define _NNIE_HW_REGS_H_

#define NNIE_REG_TASK_ADDR_LO       0x20  /* task descriptor phys[31:0]  */
#define NNIE_REG_TASK_ADDR_HI       0x24  /* task descriptor phys[63:32] */
#define NNIE_REG_TIMEOUT_LO         0x28  /* timeout cycles  [31:0]      */
#define NNIE_REG_TIMEOUT_HI         0x2C  /* timeout cycles  [63:32]     */
#define NNIE_REG_START              0x30  /* bit 0 = go (HW-clears on done) */
#define NNIE_REG_IRQ_CFG            0x34  /* bits 0/1/2 = enable finish/to/err */
#define NNIE_REG_IRQ_CLEAR          0x38  /* bits 0/1/2 = w1c of pending */
#define NNIE_REG_IRQ_STATUS         0x3C  /* bits 0/1/2 = pending finish/to/err */
#define NNIE_REG_CFG_ERR_INFO       0x40  /* config error details (RO) */
#define NNIE_REG_TASK_ID            0x48  /* current/completed task id */
#define NNIE_REG_CLK_GATE           0x50  /* bit 7 (=0x80) = clk gate enable */
#define NNIE_REG_OUTSTANDING        0x54  /* AXI: bits[4:0]=rd, [11:8]=wr */
#define NNIE_REG_CHECK_SUM          0x68  /* bit 0 = checksum enable */

#define NNIE_START_GO               (1u << 0)

#define NNIE_IRQ_FINISH             (1u << 0)
#define NNIE_IRQ_TIMEOUT            (1u << 1)
#define NNIE_IRQ_CFG_ERR            (1u << 2)
#define NNIE_IRQ_ALL                (NNIE_IRQ_FINISH | NNIE_IRQ_TIMEOUT | NNIE_IRQ_CFG_ERR)

#define NNIE_CLK_GATE_EN            (1u << 7)

#define NNIE_CHECK_SUM_EN           (1u << 0)

#endif /* _NNIE_HW_REGS_H_ */
