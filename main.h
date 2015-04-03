#ifndef __MAIN_H
#define __MAIN_H


#define gpkcon0  (*((volatile unsigned int *)0x7F008800))
#define gpkdat  (*((volatile unsigned int *)0x7F008808))

#define C1_DC		(1<<2)
#define C1_IC		(1<<12)	

#define Nand_ID_DATD_SIZE 5
#define	BOOTDELAY	5	        /* µ¹¼ÆÊ±*/


void init_tag(int addr);
int cleanup_before_linux (void);
void boot_linux(void);



#endif


