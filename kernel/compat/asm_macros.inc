/*
 * Assembly macros for vendor binary blob .S files
 *
 * These define the FUNC_BEGIN/FUNC_END/ADDRL macros used by the
 * disassembled vendor kernel module code from HI3516CV500-SDK.
 */

#if defined(__ASSEMBLY__) || defined(__ASSEMBLER__)
.section .note.GNU-stack,"",%progbits
.previous
#endif

.macro FUNC_BEGIN name
	.global \name
	.type \name, %function
\name:
.endm

.macro FUNC_END name
	.size \name, . - \name
.endm

/*
 * ADDRL: Load a 32-bit address into a register (ARMv7).
 * The disassembler emits "ADDRL Rd, =symbol" which expands to
 * a MOVW+MOVT pair. We use LDR Rd, =symbol (literal pool) for
 * simplicity and correctness with relocations.
 */
.macro ADDRL reg, addr
	LDR \reg, \addr
.endm
