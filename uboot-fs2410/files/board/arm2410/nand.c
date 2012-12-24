
#include <common.h>
#include <s3c2410.h>
#include <config.h>
#define TACLS	0
#define TWRPH0	3
#define TWRPH1	0
#define U32	unsigned int
extern unsigned long nand_probe(unsigned long physadr);
static void NF_Reset(void)
{
	int i;
	NF_nFCE_L();
	NF_CMD(0xff);
	for(i=0;i<10;i++);
	NF_WAITRB();
	NF_nFCE_H();
}

void NF_Init(void)
{
	rNFCONF=(1<<15)|(1<<14)|(1<<13)|(1<<12)|(1<<11)|(TACLS<<8)\
		|(TWRPH0<<4)|(TWRPH1<<0);
	NF_Reset();
}

int nand_read_whole(unsigned char *buf, unsigned long start_addr, int size)
{
	int i,j;
	if((start_addr & NAND_BLOCK_MASK)||(size & NAND_BLOCK_MASK)){
		return 1;
	}
	NF_nFCE_L();
	for(i=0;i<10;i++);
	i = start_addr;
	while(i < start_addr+size){
		rNFCMD	=0;
		rNFADDR	=i & 0xff;
		rNFADDR =(i>>9) & 0xff;
		rNFADDR =(i>>17) & 0xff;
		rNFADDR =(i>>25) & 0xff;
		NF_WAITRB();
		for(j=0;j < NAND_SECTOR_SIZE;j++,i++){
			*buf=(rNFDATA & 0xff);
			buf++;
		}
	}
	NF_nFCE_H();
	return 0;
}

void nand_init(void)
{
	NF_Init();
	printf("%4lu MB\n", nand_probe((unsigned long)CFG_NAND_BASE)>>20);
}

