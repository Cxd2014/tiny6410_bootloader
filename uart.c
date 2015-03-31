#define ULCON0     (*((volatile unsigned long *)0x7F005000))
#define UCON0      (*((volatile unsigned long *)0x7F005004))
#define UFCON0     (*((volatile unsigned long *)0x7F005008))
#define UMCON0     (*((volatile unsigned long *)0x7F00500C))
#define UTRSTAT0   (*((volatile unsigned long *)0x7F005010))
#define UFSTAT0    (*((volatile unsigned long *)0x7F005018))
#define UTXH0      (*((volatile unsigned char *)0x7F005020))
#define URXH0      (*((volatile unsigned char *)0x7F005024))
#define UBRDIV0    (*((volatile unsigned short *)0x7F005028))
#define UDIVSLOT0  (*((volatile unsigned short *)0x7F00502C))

#define GPACON     (*((volatile unsigned long *)0x7F008000))

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

int get_char(void)
{
	char c;
	
	c = getc();
	
	if (c == '\r')//the key of enter
		return '\n';
	
	return c;
}

/*
char gets(char *s)
{
	char *p = s;
	while((*p = get_char()) != '\n')
	{
		if(*p != '\b')//the key of Backspace
			putc(*p++);
		else if(p>s)
		{
			putc('\b');
			p--;
		}		
	}
	*p = '\0';//the key of space
	putc('\r');
	return s;
}
*/

