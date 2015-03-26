#include"common.h"
#include"stdio.h"

void delay()
{
	volatile int i = 0x100000;
	while (i--);
}

	
int main(void)
{
	
	int c = 100;
	//volatile unsigned int *p = (volatile unsigned int *)0x58000000;
	
	init_uart();    //init the uart
	
	
	printf("hello world !");
	printf("c = %d \n",c);
	
	//turn on the led
	gpkcon0 = 0x11110000;
	gpkdat =  0xe0;

	nand_read_page(0,0x58000000,0x2000);

	for(c=0;c<20;c++)
		printf("%x  ",*(p+c));
	printf("cxd\n");

	for(c=0;c<20;c++)
		printf("%x  ",*(q+c));
	printf("cxd\n");


	while(1)
	{	

	
		
		
		
	}
	
}










