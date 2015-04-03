#ifndef __NAND_H
#define __NAND_H


void nand_read_id(unsigned char *buf);
void nand_read_page(unsigned int nand_start, unsigned int ddr_start, unsigned int len);



#endif




