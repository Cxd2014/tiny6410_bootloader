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

	
	//setting the Stack
	ldr sp, =8*1024

	//init clock
	bl clock_init
	
	//init sdram 
	bl sdram_init
	
	//init nand
	bl nand_init
	
	
	//	nand flash 的地址不参与CPU的编址范围
	// 重定位，把程序的代码段、数据段复制到它的链接地址去
	ldr r0, =0x0                        //获得_start 指令的当前地址	  获取这个值是当做copy2ddr的第一个参数
	ldr r1, =0x50000000                  // _start的连接地址	链接文件lds中设置的值  
	                                  //获取这个值是当做copy2ddr的第二个参数
			
	ldr r2, =bss_start  //bss段的起始地址  
	  	
	sub r2, r2, r1       //获取这个值是当做copy2ddr的第三个参数
								//这是根据arm的参数传递规则来的
	bl nand_boot_copy_to_ddr
	
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
	ldr sp, =0x53000000    // 重新初始化栈，指向内存 
	ldr pc, =main	


	//bl main
halt:
	b halt	
	










