#include <common.h>
#include <command.h>
#include "check_crc.h"
#include "crc_checksum.h"
#include <spi.h>
#include <spi_flash.h>
#include <mapmem.h>
#include <div64.h>
#include <dm.h>
#include <malloc.h>
#include <asm/io.h>
#define SPI_MODE_0	(0|0)	

extern int check_crc(char* addr1, char* addr2);
extern int crc_checksum(char* image_number, char* image_addr);
extern int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
extern int do_bootm(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

//argv[0]=ota_update argv[1]=image1_ram_offset argv[2]=image2_ram_offset argv[3]=image1_flash_offset argv[4]=image2_flash_offset argv[5]=image_size
static int ota_update(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]){
	int check_crc_flag = 0;
	char* bootm[]={"bootm",argv[1]};
	int otaupdate_flag=0;//0 boot from image1; 1 image 1 and 2 are damaged;
	
	if(argc!=6)
		return CMD_RET_USAGE;
	
	check_crc_flag=check_crc(argv[1],argv[2]);
	if(check_crc_flag==1)
	{
		printf("CRC check is the same, check image 1 ...\n");
		if(crc_checksum("1",argv[1]))
		{
			printf("Boot from Image 1\n");
			do_bootm(cmdtp,0,2,bootm);
		}
		else
		{
			printf("Image 1 is damaged, check image 2 ...\n");		
			if(crc_checksum("2",argv[2]))
			{
				printf("Prepare copy image 2 to 1\n");
				otaupdate_flag=0;
			}
			else
			{
				printf("Image 1 and 2 are damaged\n");
				otaupdate_flag=1;
			}
		}
	}
	else
	{
		printf("image 1's crc and 2's crc are different, check image 2 ...\n");
		if(crc_checksum("2",argv[2]))
		{
			printf("Prepare copy image 2 to 1\n");
			otaupdate_flag=0;
		}
		else
		{
			printf("Image 2 is damaged, check Image 1\n");
			if(crc_checksum("1",argv[1]))
			{
				printf("Boot from Image 1\n");
				do_bootm(cmdtp,0,2,bootm);
			}
			else
			{
				printf("Image 1 and 2 are damaged\n");
				otaupdate_flag=1;
			}
		}
	}	
	return otaupdate_flag;
}


U_BOOT_CMD(
	ota_update,6,0,ota_update,"Check two image if update the kernel or not",
"ota_update [addr1] [addr2]\n"
"            addr1 is original kernel ram address\n"
"            addr2 is    new   kernel ram address\n"
);



