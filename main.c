#include"common.h"

void delay()
{
	volatile int i = 0x10000;
	while (i--);
}

int main(void)
{
	clock_init();//init the clock
	
	
	//turn on the led
	gpkcon0 = 0x11110000;
	//gpkdat=0x00;
	
	while(1)
	{	
		gpkdat=0x00;
		
		delay();

		gpkdat=0xff;

		delay();	
	}
	
}










