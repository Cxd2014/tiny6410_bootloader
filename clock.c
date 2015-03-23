#define APLL_LOCK (*((volatile unsigned long *)0x7E00F000))
#define MPLL_LOCK (*((volatile unsigned long *)0x7E00F004))
#define EPLL_LOCK (*((volatile unsigned long *)0x7E00F008))
#define OTHERS    (*((volatile unsigned long *)0x7e00f900))
#define CLK_DIV0  (*((volatile unsigned long *)0x7E00F020))

#define ARM_RATIO    0   						// ARMCLK 	= DOUTAPLL / (ARM_RATIO + 1)  	= 532/(0+1) = 532  MHz
#define MPLL_RATIO   0   						// DOUTMPLL = MOUTMPLL / (MPLL_RATIO + 1)   = 532/(0+1) = 532  MHz
#define HCLKX2_RATIO 1   						// HCLKX2 	= HCLKX2IN / (HCLKX2_RATIO + 1) = 532/(1+1) = 266  MHz
#define HCLK_RATIO   1   						// HCLK 	= HCLKX2   / (HCLK_RATIO + 1)   = 266/(1+1) = 133  MHz
#define PCLK_RATIO   3   						// PCLK   	= HCLKX2   / (PCLK_RATIO + 1)   = 266/(3+1) = 66.5 MHz

#define APLL_CON  (*((volatile unsigned long *)0x7E00F00C))
#define APLL_CON_VAL  ((1<<31) | (266 << 16) | (3 << 8) | (1))

#define MPLL_CON  (*((volatile unsigned long *)0x7E00F010))
#define MPLL_CON_VAL  ((1<<31) | (266 << 16) | (3 << 8) | (1))

#define CLK_SRC  (*((volatile unsigned long *)0x7E00F01C))

void clock_init(void)
{
	APLL_LOCK = 0xffff;
	MPLL_LOCK = 0xffff;
	EPLL_LOCK = 0xffff;

	OTHERS &= ~0xc0;
	while ((OTHERS & 0xf00) != 0);

	CLK_DIV0 = (ARM_RATIO) | (MPLL_RATIO << 4) | (HCLK_RATIO << 8) | (HCLKX2_RATIO << 9) | (PCLK_RATIO << 12);

	APLL_CON = APLL_CON_VAL;  
	MPLL_CON = MPLL_CON_VAL; 

	CLK_SRC = 0x03;
}















