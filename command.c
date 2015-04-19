#include"cxd/uart.h"
#include"cxd/command.h"
#include"cxd/main.h"
#include"cxd/mynand.h"
#include"cxd/lcd.h"


static struct command commands[COMMAND_NUM] =
{
	{0,"help"},
	{1,"md"},
	{2,"mw"},
	{3,"nand"},
	{4,"bootm"},
	{5,"lcd"},
};

void delay(volatile int i)
{
	while (i--);
}


int abort_boot(int bootdelay)
{
	int abort = 0;
	printf("Hit any key to stop autoboot:\t %d",bootdelay);
	
	while ((bootdelay > 0) && (!abort)) 
	{
		int i;
		--bootdelay;
		/* delay 100 * 10ms */
		for (i=0; !abort && i<500; ++i) 
		{
			if (UTRSTAT0& 0x1)/* we got a key press	check the uart Register*/
			{	
				abort  = 1;	/* don't auto boot	*/
				bootdelay = 0;	/* no more delay	*/

				(void) getc();  /* consume input	*/

				break;
			}
			delay (0x1000);
		}

		printf ("\b\b \b%d ", bootdelay);
	}
	printf ("\n");
	return abort;
}


static int getchar(void)
{
	char ichar;
	ichar = getc();
	if ((ichar == '\n') || (ichar == '\r'))//deal with Enter and newline 
	{
		CXD_putc('\n');
		ichar = '\n';
	}
	return ichar;
}


static  void getcommand(char *s)
{	
	char *p = s;
	while ((*p =getchar()) != '\n') 
	{
		if (*p != '\b')
			CXD_putc(*p++);
		else if(p > s)//deal with Backspace
			{
				CXD_putc('\b');				
				CXD_putc(' ');
				CXD_putc('\b');
				p--;
			}	
	}		
	*p = '\0';	
	//CXD_putc('\n');
}

int read_command_line()
{
	char console_buffer[MAX_CMDBUF_SIZE];		/* console I/O buffer	*/
	char *p ;
	int argc = 0;
	char *argv[CFG_MAXARGS +1];
	
	printf("%s",CFG_PROMPT);//show the command head
	getcommand(console_buffer);
	
	 p=strtok(console_buffer," ");
	 while(p!=NULL)
	  {
	        argv[argc]=p;
	        argc++;
	        p=strtok(NULL," ");         
	  }

	run_command(argc,argv);
	
	return argc;
}

void run_command(int argc, char * argv[])
{	
	int c=0;
	int i=0;

	for(c =0;c<COMMAND_NUM;c++)
	{
		if(strcmp(argv[0], commands[c].name)==0)
			break;	
	}

	switch(c)
	{
		case 0 :
			i = help(argc,argv);
			break;

		case 1:
			if(argc!=3)
			{
				warning_message(0);
				break;
			}
			i = md(argc,argv);
			break;

		case 2:
			if(argc!=4)
			{
				warning_message(0);
				break;
			}
			i = mw(argc,argv);
			break;

		case 3:
			i = nand(argc,argv);
			break;
			
		case 4:
			i = bootm(argc,argv);
			break;
		case 5:
			if(argc!=2)
			{
				warning_message(0);
				break;
			}
			i = lcd(argc,argv);
			break;
		case 6:
			read_command_line();
		//default:
			
	}

	if(i==1)
		printf("Do command %s successful!\n",argv[0]);
	else
		printf("Do command %s fail!\n",argv[0]);
	
	read_command_line();
	
}


int lcd(int argc, char * argv[])
{
	int color;
	color = atoi(argv[1]); 
	lcd_clear_screen(color);
	lcd_draw_hline(240,0,800,0xfff000);
	lcd_draw_vline(400,0,480,0x000000);
	return 1;
}

int nand(int argc, char * argv[])
{	
	int offset;
	int adress;
	int page;
	
	//nand_info_t *nand;

	if(strcmp(argv[1], "erase")==0)
	{
		//nand_erase_options_t opts;
		
		offset = atoi(argv[2]); // nand adress	
		page = atoi(argv[3]) ;   // 1 block 
		
		if(offset % 0x20000 != 0)
		{
			printf("The offset adress must be an integer multiple of block\n");
			return 0;
		}
	/*
		opts.offset = offset;
		opts.length = page;
		opts.quiet  = 0;
	*/
		//nand_erase_opts(nand, &opts);
		
		return 1;
	}
	else
	{
		adress = atoi(argv[2]); //adress of read to sdram
		offset = atoi(argv[3]); // nand adress	
		page = atoi(argv[4]);   
		
		if(offset % 2048 != 0)
		{
			printf("The offset adress must be an integer multiple of page\n");
			return 0;
		}
		
		if(strcmp(argv[1], "read")==0 )
		{
			page = page *PAGESIZE;// 1 page = 2K 
			nand_read_page(offset,adress,page);
		}
		
		else if (strcmp(argv[1], "write")==0)
		{
			//nand_write_page(offset,adress,page);
			printf("nand write succeed !offset = %x\n",offset);
		}
		
		else
		{
			printf("No such command !\n");
			return 0;
		}
		return 1;
	}
}


int md(int argc, char * argv[])
{
	volatile unsigned int *p = (volatile unsigned int *)0;
	int size,i,j;
	p = (volatile unsigned int *)atoi(argv[1]);//char to int
	size = atoi(argv[2]);

	for(i=0;i<size;)
	{
		printf("%x :     ",p+i);
		for(j=0;j<4;j++)
		{
			printf("%x  ",*(p+i));
			i++;
			if(i>=size)
				break;
		}
		printf("\n");
	}
	
	return 1;
}

int mw(int argc, char * argv[])
{
	volatile unsigned int *p = (volatile unsigned int *)0;
	int size,i;
	int value = 0;
	
	p = (volatile unsigned int *)atoi(argv[1]);//char to int
	size = atoi(argv[2]);
	value = atoi(argv[3]);
		
	for(i=0;i<size;i++)
	{
		*(p+i) = value;
	}
	
	return 1;
}

int bootm(int argc, char * argv[])
{
	boot_linux();
	return 1;
}

int help(int argc, char * argv[])
{
	printf("md            md [adress] [size] -- memory read\n");
	printf("mw            mw [adress] [size] [value]  --memory write\n");
	printf("nand read     nand read [aderss] [offset] [page] --nand read to memory\n");
	printf("nand write    nand write [aderss] [offset] [page] --nand write from memory\n");
	printf("nand erase    nand erase [aderss] [block]  --nand erase block\n");
	printf("bootm         boot for linux kernel\n");
	printf("lcd           lcd [color] --clear the lcd\n");
	return 1;
}

void warning_message(char num)
{
	switch(num)
	{
		case 0:
			printf("command Parameter is Error\n");
			printf("Try 'cp --help' for more information.\n");
			break;	
	}
}

int atoi(char * buf)
{
	unsigned int value = 0;
	int jinzhi = 10;
	int i =0;
	
	if(buf[0] == '0'&&buf[1] == 'x')
	{
		jinzhi = 16;
		i = 2;
	}

	while(buf[i])
	{
		int tmp;
		
		if (buf[i] <= '9' && buf[i] >= '0') 
			tmp = buf[i] - '0';
		else
			tmp = buf[i] - 'a' + 10;
					
		value = value * jinzhi + tmp;
		
		i++;
	}
	return value;
}









