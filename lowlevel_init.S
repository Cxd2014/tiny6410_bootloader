#include <cxd/config.h>

.globl lowlevel_init
lowlevel_init:

	mov	r12, lr  //save return address 
		
//External interrupt pending clear  copy from uboot
	ldr	r0, =(0x7f008000+0x924)	/*EINTPEND*/
	ldr	r1, [r0]
	str	r1, [r0]

	ldr	r0, =0x71200000 	
	ldr	r1, =0x71300000 	

	// Disable all interrupts (VIC0 and VIC1)
	mvn	r3, #0x0
	str	r3, [r0, #0x14]
	str	r3, [r1, #0x14]

	// Set all interrupts as IRQ
	mov	r3, #0x0
	str	r3, [r0, #0x0c]
	str	r3, [r1, #0x0c]

	// Pending Interrupt Clear
	mov	r3, #0x0
	str	r3, [r0, #0xf00]
	str	r3, [r1, #0xf00]


	bl system_clock_init

	bl uart_asm_init

	bl nand_asm_init

	bl mem_ctrl_asm_init

 	mov	lr, r12
 	
	mov	pc, lr  //return start.S



system_clock_init:
/*
 * system_clock_init: Initialize core clock and bus clock.
 * void system_clock_init(void)
 */
	ldr	r0, =0x7e00f000	@0x7e00f000

	ldr	r1, [r0, #0x900]
	mov	r2, #0x40
	orr	r1, r1, r2
	str	r1, [r0, #0x900]
	nop
	nop
	nop
	nop
	nop
	ldr	r2, =0x80
	orr	r1, r1, r2
	str	r1, [r0, #0x900]

check_syncack:
	ldr	r1, [r0, #0x900]
	ldr	r2, =0xf00
	and	r1, r1, r2
	cmp	r1, #0xf00
	bne	check_syncack
/* ASYNC Mode */

	mov	r1, #0xff00
	orr	r1, r1, #0xff
	str	r1, [r0, #0x00]
	str	r1, [r0, #0x04]
	str	r1, [r0, #0x08]
/* CLKUART(=66.5Mhz) = CLKUART_input(532/2=266Mhz) / (UART_RATIO(3)+1) */
/* CLKUART(=50Mhz) = CLKUART_input(400/2=200Mhz) / (UART_RATIO(3)+1) */
/* Now, When you use UART CLK SRC by EXT_UCLK1, We support 532MHz & 400MHz value */

	ldr   	r1, [r0, #0x28]
	bic	r1, r1, #0x70000
	orr	r1, r1, #0x30000
	str	r1, [r0, #0x28]

	ldr   	r1, [r0, #0x20]	/*Set Clock Divider*/
	bic	r1, r1, #0x30000
	bic	r1, r1, #0xff00
	bic	r1, r1, #0xff
	ldr	r2, =CLK_DIV_VAL
	orr	r1, r1, r2
	str	r1, [r0, #0x20]

	ldr	r1, =APLL_VAL
	str	r1, [r0, #0x0c]
	ldr	r1, =MPLL_VAL
	str	r1, [r0, #0x10]

	ldr	r1, =0x80200203			/* FOUT of EPLL is 96MHz */
	str	r1, [r0, #0x14]
	ldr	r1, =0x0
	str	r1, [r0, #0x18]

	ldr	r1, [r0, #0x1c]	/* APLL, MPLL, EPLL select to Fout */

	ldr	r2, =0x2007

	orr	r1, r1, r2

	str	r1, [r0, #0x1c]

	/* wait at least 200us to stablize all clock */
	mov	r1, #0x10000
1:	subs	r1, r1, #1
	bne	1b

	/* Synchronization for VIC port */
	ldr	r1, [r0, #0x900]
	orr	r1, r1, #0x20
	str	r1, [r0, #0x900]
	
	mov	pc, lr



uart_asm_init:
	/* set GPIO to enable UART */
	//GPIO setting for UART
	ldr	r0, =0x7f008000
	ldr	r1, =0x22222222
	str   	r1, [r0, #0x00]
	ldr	r1, =0x2222
	str   	r1, [r0, #0x20]

	ldr	r0, =0x7F005000		//0x7F005000
	mov	r1, #0x0
	str	r1, [r0, #0x08]
	str	r1, [r0, #0x0c]

	mov	r1, #0x3                	//was 0.
	str	r1, [r0, #0x00]

	ldr	r1, =0xe45			/* UARTCLK SRC = 11 => EXT_UCLK1*/

	str	r1, [r0, #0x04]

	ldr	r1, =0x22
	
	str	r1, [r0, #0x28]

	ldr	r1, =0x1FFF

	str	r1, [r0, #0x2c]

	ldr	r1, =0x4f4f4f4f
	str	r1, [r0, #0x20]		

	mov	pc, lr
	



nand_asm_init:
	ldr	r0, =ELFIN_NAND_BASE
	ldr	r1, [r0, #NFCONF_OFFSET]
	orr	r1, r1, #0x70
	orr	r1, r1, #0x7700
	str     r1, [r0, #NFCONF_OFFSET]

	ldr	r1, [r0, #NFCONT_OFFSET]
	orr	r1, r1, #0x03
	str     r1, [r0, #NFCONT_OFFSET]

	mov	pc, lr



mem_ctrl_asm_init:
	ldr	r0, =ELFIN_MEM_SYS_CFG			@Memory sussystem address 0x7e00f120
	mov	r1, #0xd							@ Xm0CSn2 = NFCON CS0, Xm0CSn3 = NFCON CS1
	str	r1, [r0]

	ldr	r0, =ELFIN_DMC1_BASE			@DMC1 base address 0x7e001000

	ldr	r1, =0x04
	str	r1, [r0, #INDEX_DMC_MEMC_CMD]

	ldr	r1, =DMC_DDR_REFRESH_PRD
	str	r1, [r0, #INDEX_DMC_REFRESH_PRD]

	ldr	r1, =DMC_DDR_CAS_LATENCY
	str	r1, [r0, #INDEX_DMC_CAS_LATENCY]

	ldr	r1, =DMC_DDR_t_DQSS
	str	r1, [r0, #INDEX_DMC_T_DQSS]

	ldr	r1, =DMC_DDR_t_MRD
	str	r1, [r0, #INDEX_DMC_T_MRD]

	ldr	r1, =DMC_DDR_t_RAS
	str	r1, [r0, #INDEX_DMC_T_RAS]

	ldr	r1, =DMC_DDR_t_RC
	str	r1, [r0, #INDEX_DMC_T_RC]

	ldr	r1, =DMC_DDR_t_RCD
	ldr	r2, =DMC_DDR_schedule_RCD
	orr	r1, r1, r2
	str	r1, [r0, #INDEX_DMC_T_RCD]

	ldr	r1, =DMC_DDR_t_RFC
	ldr	r2, =DMC_DDR_schedule_RFC
	orr	r1, r1, r2
	str	r1, [r0, #INDEX_DMC_T_RFC]

	ldr	r1, =DMC_DDR_t_RP
	ldr	r2, =DMC_DDR_schedule_RP
	orr	r1, r1, r2
	str	r1, [r0, #INDEX_DMC_T_RP]

	ldr	r1, =DMC_DDR_t_RRD
	str	r1, [r0, #INDEX_DMC_T_RRD]

	ldr	r1, =DMC_DDR_t_WR
	str	r1, [r0, #INDEX_DMC_T_WR]

	ldr	r1, =DMC_DDR_t_WTR
	str	r1, [r0, #INDEX_DMC_T_WTR]

	ldr	r1, =DMC_DDR_t_XP
	str	r1, [r0, #INDEX_DMC_T_XP]

	ldr	r1, =DMC_DDR_t_XSR
	str	r1, [r0, #INDEX_DMC_T_XSR]

	ldr	r1, =DMC_DDR_t_ESR
	str	r1, [r0, #INDEX_DMC_T_ESR]

	ldr	r1, =DMC1_MEM_CFG
	str	r1, [r0, #INDEX_DMC_MEMORY_CFG]

	ldr	r1, =DMC1_MEM_CFG2
	str	r1, [r0, #INDEX_DMC_MEMORY_CFG2]

	ldr	r1, =DMC1_CHIP0_CFG
	str	r1, [r0, #INDEX_DMC_CHIP_0_CFG]

	ldr	r1, =DMC_DDR_32_CFG
	str	r1, [r0, #INDEX_DMC_USER_CONFIG]

	@DMC0 DDR Chip 0 configuration direct command reg
	ldr	r1, =DMC_NOP0
	str	r1, [r0, #INDEX_DMC_DIRECT_CMD]

	@Precharge All
	ldr	r1, =DMC_PA0
	str	r1, [r0, #INDEX_DMC_DIRECT_CMD]

	@Auto Refresh	2 time
	ldr	r1, =DMC_AR0
	str	r1, [r0, #INDEX_DMC_DIRECT_CMD]
	str	r1, [r0, #INDEX_DMC_DIRECT_CMD]

	@MRS
	ldr	r1, =DMC_mDDR_EMR0
	str	r1, [r0, #INDEX_DMC_DIRECT_CMD]

	@Mode Reg
	ldr	r1, =DMC_mDDR_MR0
	str	r1, [r0, #INDEX_DMC_DIRECT_CMD]

	@Enable DMC1
	mov	r1, #0x0
	str	r1, [r0, #INDEX_DMC_MEMC_CMD]

check_dmc1_ready:
	ldr	r1, [r0, #INDEX_DMC_MEMC_STATUS]
	mov	r2, #0x3
	and	r1, r1, r2
	cmp	r1, #0x1
	bne	check_dmc1_ready
	nop
	mov	pc, lr


