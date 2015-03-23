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
	
	bl main   
halt:
	b halt	
	










