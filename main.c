#include"common.h"


void delay()
{
	volatile int i = 0x100000;
	while (i--);
}


int main(void)
{
	clock_init();  //init the clock
	init_uart();    //init the uart
	
	//turn on the led
	gpkcon0 = 0x11110000;
	gpkdat =  0x0;

	char c;
	
	while(1)
	{	
		c = getc();
		putc(c);
		putc('\n');
		
		for (c = 'a'; c <= 'z'; c++)
			putc(c);
		putc('\n');
	}
	
}










