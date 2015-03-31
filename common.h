#ifndef __COMMON_H
#define __COMMON_H


#define gpkcon0  (*((volatile unsigned int *)0x7F008800))
#define gpkdat  (*((volatile unsigned int *)0x7F008808))

#define	CFG_CBSIZE		256		/* Console I/O Buffer Size	*/

//void clock_init(void);

void init_uart(void);
//char gets(char *s);

//unsigned char getc(void);
//void putc(char c);

void nand_read_page(unsigned int nand_start, unsigned int ddr_start, unsigned int len);
void nand_read_id(unsigned char *buf);

//int read_command_line();
//void run_command(int argc, char * argv[]);



void init_tag(int addr);





void delay()
{
	volatile int i = 0x100000;
	while (i--);
}


#endif

