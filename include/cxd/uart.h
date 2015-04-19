#ifndef __UART_H
#define __UART_H


#define printf CXD_printf

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

char * ___strtok;
#define		NULL	0
typedef int * va_list;
#define va_start(ap, A)		(ap = (int *)&(A) + 1)
#define va_arg(ap, T)		(*(T *)ap++)
#define va_end(ap)		((void)0)

unsigned int strlen(const char * s);
char * strtok(char * s,const char * ct);
char * strcpy(char * dest,const char *src);
unsigned int strspn(const char *s, const char *accept);
char * strpbrk(const char * cs,const char * ct);
int strcmp(const char * cs,const char * ct);
char * itoa(int a, char * buf);
void CXD_printf(const char * format, ...);

unsigned char getc(void);

void CXD_putc(unsigned char c);

int puts(const char * s);
void putint_hex(int a);
void putchar_hex(char c);

#endif

