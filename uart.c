#include"uart.h"



void init_uart(void)
{
	GPACON &= ~0xff;
	GPACON |= 0x22;

	ULCON0 = 0x3;  					
	UCON0  = 0x5; //uboot 					
	UFCON0 = 0x01; 					
	UMCON0 = 0;	

	UBRDIV0   = 35;
	UDIVSLOT0 = 0x1;
}


unsigned char getc(void)
{
	while ((UFSTAT0 & (1<<6)) == 0 && (UFSTAT0 & 0x3f) == 0);
	return URXH0;
}


void putc(unsigned char c)
{
	while (UFSTAT0 & (1<<14));
	UTXH0 = c;
}


