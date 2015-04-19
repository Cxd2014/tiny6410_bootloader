.globl _start
_start:

	//tell tne Peripheral's adress to cpu
	
	ldr r0, =0x70000000
   	orr r0, r0, #0x13
   	mcr p15,0,r0,c15,c2,4 

	//close watch dog
	ldr r0, =0x7E004000
	mov r1, #0
	str r1, [r0]

	/*
	 * set the cpu to SVC32 mode
	 */
	mrs	r0,cpsr
	bic	r0,r0,#0x1f
	orr	r0,r0,#0xd3
	msr	cpsr,r0
	 
	/*
	 * flush v4 I/D caches
	 */
	mov	r0, #0
	mcr	p15, 0, r0, c7, c7, 0	/* flush v3/v4 cache */
	mcr	p15, 0, r0, c8, c7, 0	/* flush v4 TLB */

	/*
	 * disable MMU stuff and caches
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002300	@ clear bits 13, 9:8 (--V- --RS)
	bic	r0, r0, #0x00000087	@ clear bits 7, 2:0 (B--- -CAM)
	orr	r0, r0, #0x00000002	@ set bit 2 (A) Align
	orr	r0, r0, #0x00001000	@ set bit 12 (I) I-Cache
	mcr	p15, 0, r0, c1, c0, 0
	
	bl	lowlevel_init	/* go setup pll,uart */

	ldr r1, =0x7F008800 					
	ldr r0, =0x11110000																  
	str r0, [r1]

	
	//setting the Stack for C
	ldr sp, =(8*1024)
			
	//	nand flash 的地址不参与CPU的编址范围
	// 重定位，把程序的代码段、数据段复制到它的链接地址去
	ldr r0, =0x0                        //获得_start 指令的当前地址	  获取这个值是当做nand_boot_copy_to_ddr的第一个参数
	ldr r1, =0x57e00000                  // _start的连接地址	链接文件lds中设置的值  
	                                  //获取这个值是当做nand_boot_copy_to_ddr的第二个参数
			
	ldr r2, =bss_start  //bss段的起始地址  
	  	
	sub r2, r2, r1       //获取这个值是当做copy2ddr的第三个参数
								//这是根据arm的参数传递规则来的
	//bl nand_boot_copy_to_ddr
	bl nand_read_page

	
clean_bss:
	ldr r0, =bss_start
	ldr r1, =bss_end
	mov r3, #0
	cmp r0, r1
	beq on_ddr
clean_loop:
	str r3, [r0], #4
	cmp r0, r1	
	bne clean_loop		
	
	
	
on_ddr:
	ldr sp, =0x57FFFFF4    // 重新初始化栈，指向内存 
	ldr pc, =main	

halt:
	b halt	
	










