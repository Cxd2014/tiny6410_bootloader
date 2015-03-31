#include"common.h"
#include"stdio.h"
#include"setup.h"

//char *argv[10];  //save the command

#define C1_DC		(1<<2)
#define C1_IC		(1<<12)	

#define Nand_ID_DATD_SIZE 5

int cleanup_before_linux (void)
{
	
	unsigned long i;
	/* turn off I/D-cache */
	asm ("mrc p15, 0, %0, c1, c0, 0":"=r" (i));
	i &= ~(C1_DC | C1_IC);
	asm ("mcr p15, 0, %0, c1, c0, 0": :"r" (i));
	/* flush I/D-cache */
	i = 0;
	asm ("mcr p15, 0, %0, c7, c7, 0": :"r" (i));  /* invalidate both caches and flush btb */
	asm ("mcr p15, 0, %0, c7, c10, 4": :"r" (i)); /* mem barrier to sync things */
	return(0);
}

static unsigned int strlen(const char * s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

static char * strcpy(char * dest,const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}
 
void init_tag(int addr)
{
	struct tag * p;
	char *cmd  ="root=/dev/mtdblock2 rootfstype=yaffs2 init=/linuxrc console=ttySAC0,115200 lcd=S70";
	 //char *cmd  = "root=/dev/mtdblock2 console=ttySAC0,115200 lcd=S70";
	//char *cmd  ="root=ubi0:FriendlyARM-root ubi.mtd=2 rootfstype=ubifs init=/linuxrc console=ttySAC0,115200 lcd=S70";
	p = (struct tag*) addr;
	p->hdr.tag  =  ATAG_CORE;
	p->hdr.size = tag_size(tag_core);
	p->u.core.flags = 0;
	p->u.core.pagesize = 0;
	p->u.core.rootdev = 0;
	p = tag_next(p);
	
	p->hdr.tag = ATAG_MEM;
	p->hdr.size = tag_size(tag_mem32);
	p->u.mem.start = 0x50000000;
	p->u.mem.size = 0x10000000;//256Mb
	p = tag_next(p);
	
	p->hdr.tag = ATAG_CMDLINE;
	p->hdr.size =  (strlen (cmd) + sizeof(struct tag_header) + 3) >>2;	
	strcpy (p->u.cmdline.cmdline, cmd);
	printf("p->hdr.size  = %d\n",p->hdr.size);
	printf("cmd  = %s\n",p->u.cmdline.cmdline);
	p = tag_next(p);
	
	p->hdr.tag = ATAG_NONE;
	p->hdr.size = 0;
}


int main(void)
{
	
	int c = 0;
	unsigned char Nand_ID[Nand_ID_DATD_SIZE]; //read the nand flash ID
	
	void (*theKernel)(int zero, int arch, unsigned int params);    

	volatile unsigned int *p0 = (volatile unsigned int *)0x50000100;
	volatile unsigned int *p1 = (volatile unsigned int *)0x50008000;
	
/*	volatile unsigned int *p1 = (volatile unsigned int *)0x50000100;
	volatile unsigned int *p2 = (volatile unsigned int *)0x50000100;
	volatile unsigned int *p3 = (volatile unsigned int *)0x50000100;
*/	

	init_uart();    //init the uart

	
	printf("hello world !\n");

	
	nand_read_id(Nand_ID);
	printf("The Nand flash ID is : ");
	for(c=0;c<Nand_ID_DATD_SIZE;c++)
		printf("%x",Nand_ID[c]);
	printf("\n");
	
	//turn on the led
	gpkcon0 = 0x11110000;
	gpkdat =  0x00;

	//nand_read_page(0x400000,0x50008000,0x500000);//copy linux kernel to sdram

	nand_read_page(0x80000,0x50008000,0x500000);
	
	for(c=0;c<100;c++)
		printf("%x  ",*(p1+c));
	printf("\t cxd\n");

	
	init_tag(0x50000100);//set the params for linux kernel

	for(c=0;c<100;c++)
		printf("%x  ",*(p0+c));
	printf("\t cxd\n");

	theKernel = (void (*)(int,int,unsigned int))0x50008000;//set the linux kernel adress in sdram

	printf("booting linux kernel...\n");

	cleanup_before_linux();
	
	theKernel(0,2520,0x50000100);//go to linux kernel
	
	printf("it is Impossible!\n");
	
	while(1)
	{	
		/*
		argc = read_command_line();
		if(argc == 0 && i ==0)
		{
			printf("This bootloader command mode");
			c=1;
		}
		run_command(argc, argv);
		*/
	}
	
}



/*
int read_command_line()
{
	char console_buffer[CFG_CBSIZE];	
	char *buf = console_buffer;
	int argc = 0;
	int state = 0;
	
	printf("\t # CXD_boot_command # \n");
	
	gets(console_buffer);

	while (*buf)
	{
		if (*buf != ' ' && state == 0)
		{
			argv[argc++] = buf;
			state = 1;
		}
		
		if (*buf == ' ' && state == 1)
		{
			*buf = '\0';
			state = 0;
		}
		
		buf++;	
	}
	
	return argc;
	
} 


void run_command(int argc, char * argv[])
{
	if (strcmp(argv[0], "help") == 0)//Matching the command 
	{
		printf("This bootloader command help!");
		return;
	}

	if (strcmp(argv[0], "md") == 0)
	{
		md(argc, argv);
		return;
	}
}

*/



