#define gpkcon0  (*((volatile unsigned int *)0x7F008800))
#define gpkdat  (*((volatile unsigned int *)0x7F008808))

void clock_init(void);

void init_uart(void);
unsigned char getc(void);
void putc(char c);

