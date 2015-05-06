#include "cxd/lcd.h"

#define GPECON  (*((volatile unsigned long *)0x7F008080))
#define GPEDAT  (*((volatile unsigned long *)0x7F008084))
#define GPEPUD  (*((volatile unsigned long *)0x7F008088))
#define GPFCON  (*((volatile unsigned long *)0x7F0080A0))
#define GPFDAT  (*((volatile unsigned long *)0x7F0080A4))
#define GPFPUD  (*((volatile unsigned long *)0x7F0080A8))
#define GPICON  (*((volatile unsigned long *)0x7F008100))
#define GPIPUD  (*((volatile unsigned long *)0x7F008108))
#define GPJCON  (*((volatile unsigned long *)0x7F008120))
#define GPJPUD  (*((volatile unsigned long *)0x7F008128))

/* display controller */
#define MIFPCON  	    (*((volatile unsigned long *)0x7410800C))
#define SPCON        	(*((volatile unsigned long *)0x7F0081A0))
#define VIDCON0      	(*((volatile unsigned long *)0x77100000))
#define VIDCON1      	(*((volatile unsigned long *)0x77100004))
#define VIDTCON0     	(*((volatile unsigned long *)0x77100010))
#define VIDTCON1     	(*((volatile unsigned long *)0x77100014))
#define VIDTCON2     	(*((volatile unsigned long *)0x77100018))
#define WINCON0      	(*((volatile unsigned long *)0x77100020))
#define VIDOSD0A      	(*((volatile unsigned long *)0x77100040))
#define VIDOSD0B      	(*((volatile unsigned long *)0x77100044))
#define VIDOSD0C      	(*((volatile unsigned long *)0x77100048))
#define VIDW00ADD0B0    (*((volatile unsigned long *)0x771000A0))
#define VIDW00ADD1B0    (*((volatile unsigned long *)0x771000D0))
#define VIDW00ADD2      (*((volatile unsigned long *)0x77100100))
#define DITHMODE        (*((volatile unsigned long *)0x77100170))

#define FRAME_BUFFER   0x54000000
#define ROW				480
#define COL				800


/*S70 Data Manual's 14 page
  *HSPW == HS pulse width                            1 ~ 40         ==hsync_len	= 10
  *HBPD == HS Blanking                                 46           ==left_margin	= 36
  *HFPD == HS Front Porch                             16~210~354    ==right_margin	= 80
  *LINEVAL == Horizontal Display Area       800
  */
  
#define HSPW 			(10)
#define HBPD			 	(36)
#define HFPD 				(80)
#define HOZVAL			(799)

/*S70 Data Manual's 14 page
  *VSPW == VS pulse width                       1 ~ 20         ==vsync_len	= 8
  *VBPD == VS Blanking                            23           ==upper_margin	= 15
  *VFPD == VS Front Porch                       7~22~147       ==lower_margin	= 22
  *LINEVAL == Vertical Display Area     480
  */
#define VSPW				(8)        
#define VBPD 			(15)
#define VFPD 				(22)
#define LINEVAL 			(479)


// init LCD
void lcd_init(void)
{
	//set the GPIO    
	
	//VD0~VD15   ==    GPI0~GPI15                                
	//VD16~VD23 ==    GPJ0~GPJ7           
	//HSYNC                    GPJ8           horizontal signal
	//VSYNC                    GPJ9           Vertical      signal
	//VDEN                      GPJ10         Data Enable
	//VCLK                       GPJ11         LCD clock 
	//PWM1                     GPF15         Background  light
	//LCD_POWER         GPE0            Lcd power 
	
	GPICON = 0xAAAAAAAA;                         
	GPJCON = 0x00AAAAAA;   
	
          GPFCON &= ~(0x3<<30);  //clear 0
	 GPFCON  |=(1<<30);        // set value
		                                                                  
	GPECON &= ~(0xf);
	GPECON |= 0x1;


	//Set the LCD controller
	//The S3C6410 Data Manual's 491 page have the set example
	
	// normal mode
	MIFPCON &= ~(1<<3);

	// RGB I/F Style
	SPCON =  (SPCON & ~(0x3)) | 1;

	/*  VIDCON0 set :
	  *   (0<<26) == RGB I/F
	  *   (0<<17) == RGB Parallel format (RGB)
	  *   (0<<16) == Select CLKVAL_F update timing control = always
	  *   (3<<6) == VCLK Video Clock Source / (CLKVAL+1)         133Mh/(3+1) = 33.25Mh
	  *   (0<<5) == VCLK Free run control ( Only valid at the RGB IF mode) = Normal mode
	  *   (1<<4) == Select the clock source as direct or divide using CLKVAL_F register = Divided by CLKVAL_F
	  *   (0<<2) == Select the Video Clock source = HCLK
	  *   (3<<0) == Enable the video output and the Display control signal
	  */
	VIDCON0 = (0<<26)|(0<<17)|(0<<16)|(3<<6)|(0<<5)|(1<<4)|(0<<2)|(3<<0);


	/*VIDCON1 set :
	  * 1<<5 == This bit indicates the HSYNC pulse polarity = inverted
	  * 1<<6 == This bit indicates the VSYNC pulse polarity = inverted
	  */
	VIDCON1 |= 1<<5 | 1<<6;


        /*VIDTCON0 set :
	 *See the top define 
	 */
	VIDTCON0 = VBPD<<16 | VFPD<<8 | VSPW<<0;
	VIDTCON1 = HBPD<<16 | HFPD<<8 | HSPW<<0;
	VIDTCON2 = (LINEVAL << 11) | (HOZVAL << 0);


        /*WINCON0 set :
	 *1<<0 == Enable the video output and the VIDEO control signal
	 *0xB<<2 == Select the BPP (Bits Per Pixel) mode Window image = unpacked 24 BPP (non-palletized R:8-G:8-B:8 )
	 */
	WINCON0 |= 1<<0;
	WINCON0 &= ~(0xf << 2);//clear 0
	WINCON0 |= 0xB<<2;

	//Set the window size and Coordinate system
#define LeftTopX     0
#define LeftTopY     0
#define RightBotX   799
#define RightBotY   479
	VIDOSD0A = (LeftTopX<<11) | (LeftTopY << 0);
	VIDOSD0B = (RightBotX<<11) | (RightBotY << 0);
	VIDOSD0C = (LINEVAL + 1) * (HOZVAL + 1);

	//tell the lcd controller the Frame buffer start adress and end adress
	VIDW00ADD0B0 = FRAME_BUFFER;
	VIDW00ADD1B0 = (((HOZVAL + 1)*4 + 0) * (LINEVAL + 1)) & (0xffffff);
}


void lcd_draw_pixel(int row, int col, int color)
{
	unsigned long * pixel = (unsigned long  *)FRAME_BUFFER;

	*(pixel + row * COL + col) = color;

}

//clear LCD
void lcd_clear_screen(int color)
{
	int i, j;

	for (i = 0; i < ROW; i++)
		for (j = 0; j < COL; j++)
			lcd_draw_pixel(i, j, color);

}

void lcd_draw_hline(int row, int col1, int col2, int color)
{
	int j;
	for (j = col1; j <= col2; j++)
		lcd_draw_pixel(row, j, color);

}

void lcd_draw_vline(int col, int row1, int row2, int color)
{
	int i;
	for (i = row1; i <= row2; i++)
		lcd_draw_pixel(i, col, color);

}

void lcd_draw_cross(int row, int col, int halflen, int color)
{
	lcd_draw_hline(row, col-halflen, col+halflen, color);
	lcd_draw_vline(col, row-halflen, row+halflen, color);
}

void lcd_draw_logo(int logo_adress)
{	
	int i, j;
	char * p = (char *)logo_adress;
	int blue, green, red,color;
	
	p = p +54; //bmp has a 54 bytes head 
	
	for (i = 0; i < 480; i++)
		for (j = 0; j < 800; j++)
		{
			blue = *p++;
			green = *p++;
			red = *p++;
			color = red << 16 | green << 8 | blue << 0;
			
			lcd_draw_pixel(i, j, color);
		}
			
}


/*
void lcd_draw_circle(void)
{
	int x,y;
	int color;
	unsigned char red,green,blue,alpha;
	int xsize = ROW;
	int ysize = COL;

	for (y = 0; y < ysize; y++)
		for (x = 0; x < xsize; x++)
		{
			color = ((x-xsize/2)*(x-xsize/2) + (y-ysize/2)*(y-ysize/2))/64;
			red   = (color/8) % 256;
			green = (color/4) % 256;
			blue  = (color/2) % 256;
			alpha = (color*2) % 256;

			color |= ((int)alpha << 24);
			color |= ((int)red   << 16);
			color |= ((int)green << 8 );
			color |= ((int)blue       );

			lcd_draw_pixel(x,y,color);
		}
}
*/

