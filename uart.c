#include"cxd/uart.h"


unsigned char getc(void)
{
	while ((UFSTAT0 & (1<<6)) == 0 && (UFSTAT0 & 0x3f) == 0);
	return URXH0;
}


void CXD_putc(unsigned char c)
{
	if(c == '\r')
	{
		while (!(UTRSTAT0 & (1<<1)));
		UTXH0 = '\n';
	}
	
	if(c == '\n')
	{
		while (!(UTRSTAT0 & (1<<1)));
		UTXH0 = '\r';
	}
	while (!(UTRSTAT0 & (1<<1)));
	UTXH0 = c;
}

int puts(const char * s)
{
	while (*s)
		CXD_putc(*s++);	
	return 0;
}

void putchar_hex(char c)
{
	char * hex = "0123456789ABCDEF";
	
	CXD_putc(hex[(c>>4) & 0x0F]);
	CXD_putc(hex[(c>>0) & 0x0F]);
}

void putint_hex(int a)
{
	putchar_hex( (a>>24) & 0xFF );
	putchar_hex( (a>>16) & 0xFF );
	putchar_hex( (a>>8) & 0xFF );
	putchar_hex( (a>>0) & 0xFF );
}

unsigned int strlen(const char * s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

char * strcpy(char * dest,const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}

char * strtok(char * s,const char * ct)
{
	char *sbegin, *send;

	sbegin  = s ? s : ___strtok;
	if (!sbegin) {
		return NULL;
	}
	sbegin += strspn(sbegin,ct);
	if (*sbegin == '\0') {
		___strtok = NULL;
		return( NULL );
	}
	send = strpbrk( sbegin, ct);
	if (send && *send != '\0')
		*send++ = '\0';
	___strtok = send;
	return (sbegin);
}



unsigned int strspn(const char *s, const char *accept)
{
	const char *p;
	const char *a;
	unsigned int count = 0;

	for (p = s; *p != '\0'; ++p) {
		for (a = accept; *a != '\0'; ++a) {
			if (*p == *a)
				break;
		}
		if (*a == '\0')
			return count;
		++count;
	}

	return count;
}


char * strpbrk(const char * cs,const char * ct)
{
	const char *sc1,*sc2;

	for( sc1 = cs; *sc1 != '\0'; ++sc1) {
		for( sc2 = ct; *sc2 != '\0'; ++sc2) {
			if (*sc1 == *sc2)
				return (char *) sc1;
		}
	}
	return NULL;
}


int strcmp(const char * cs,const char * ct)
{
	register signed char __res;

	while (1) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
	}

	return __res;
}

void CXD_printf(const char * format, ...)
{
	char c;	
	va_list ap;
		
	va_start(ap, format);
	
	while ((c = *format++) != '\0')
	{
		switch (c)
		{
			case '%':
				c = *format++;
				
				switch (c)
				{
					char ch;
					char * p;
					int a;
					char buf[100];
									
					case 'c':
						ch = va_arg(ap, int);
						CXD_putc(ch);
						break;
					case 's':
						p = va_arg(ap, char *);
						puts(p);
						break;					
					case 'x':
						a = va_arg(ap, int);
						putint_hex(a);
						break;		
					case 'd':
						a = va_arg(ap, int);
						itoa(a, buf);
						puts(buf);
						break;	
					
					default:
						break;
				}				
				break;		
		
			default:
				CXD_putc(c);
				break;
		}
	}
	
}

char * itoa(int a, char * buf)
{
	int num = a;
	int i = 0;
	int len = 0;
	
	do 
	{
		buf[i++] = num % 10 + '0';
		num /= 10;		
	} while (num);
	buf[i] = '\0';
	
	len = i;
	for (i = 0; i < len/2; i++)
	{
		char tmp;
		tmp = buf[i];
		buf[i] = buf[len-i-1];
		buf[len-i-1] = tmp;
	}
	
	return buf;	
}

