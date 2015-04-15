#ifndef __NAND_H
#define __NAND_H

//#define PAGESIZE (8*1024)   // K9GAG08U0E Flash 1Page = (8K + 436)Bytes
#define PAGESIZE 2048               // K9K8G08U0A Flash 1Page = (2K + 64)Bytes


void nand_read_id(unsigned char *buf);
void nand_read_page(unsigned int nand_start, unsigned int ddr_start, unsigned int len);
void nand_write_page(unsigned int nand_start, unsigned int ddr_start, unsigned int page);
void nand_erase_block(unsigned int nand_start,unsigned int block);

#endif




