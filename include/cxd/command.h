#ifndef __COMMAND_H
#define __COMMAND_H


#define	CFG_PROMPT		"CXD_BOOT #"
#define	MAX_CMDBUF_SIZE	256	/* Console I/O Buffer Size*/
#define CFG_MAXARGS 6
#define COMMAND_NUM 6

struct command 
{
	int i;
	char *name;
};



void delay(volatile int i);

int abort_boot(int bootdelay);
int read_command_line();
void run_command(int argc, char * argv[]);
int atoi(char * buf);
void warning_message(char num);

int md(int argc, char * argv[]);
int mw(int argc, char * argv[]);
int nand(int argc, char * argv[]);
int bootm(int argc, char * argv[]);
int help(int argc, char * argv[]);
int lcd(int argc, char * argv[]);


#endif
