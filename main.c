#include"cxd/uart.h"
#include"cxd/main.h"
#include"cxd/mynand.h"
#include"cxd/setup.h"
#include"cxd/command.h"
#include"cxd/lcd.h"
#include"cxd/uart.h"

int main(void)
{
	
	int c = 0;
	unsigned char Nand_ID[Nand_ID_DATD_SIZE]; //read the nand flash ID
	//volatile unsigned int *p = (volatile unsigned int *)0x50000000;
	lcd_init();    //init the lcd 
	
	printf("####CXD_Bootloader for MINI6410_1GNandflash####\n");

	nand_read_id(Nand_ID);
	printf("The Nand flash ID is : ");
	for(c=0;c<Nand_ID_DATD_SIZE;c++)
		putchar_hex(Nand_ID[c]);
	printf("\n");

	nand_read_page(0x3f000000,0x5f000000,0x120000);//copy the logo to sdram 

	lcd_draw_logo(0x5f000000);//show the logo
	
	if(!abort_boot(BOOTDELAY)) /* ÑÓÊ±Æô¶¯ */
	{
		boot_linux();
	}

	printf("##### CXD_Bootloader command#####\n");
	
	while(1)
	{	
		c = read_command_line ();	
	}
	
}


void boot_linux(void)
{
	void (*theKernel)(int zero, int arch, unsigned int params); 
	
	printf("copy linux from NandFlash 0x80000 to Sdram 0x50008000...\n");
	
	nand_read_page(0x80000,0x50008000,0x400000);

	init_tag(0x50000100);//set the params for linux kernel

	theKernel = (void (*)(int,int,unsigned int))0x50008000;//set the linux kernel adress in sdram

	printf("booting linux kernel...\n");

	cleanup_before_linux();

	theKernel(0,2520,0x50000100);//go to linux kernel
	//theKernel(0,1626,0x50000100);
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
	printf("cmd  = %s\n",p->u.cmdline.cmdline);
	p = tag_next(p);
	
	p->hdr.tag = ATAG_NONE;
	p->hdr.size = 0;
}


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




