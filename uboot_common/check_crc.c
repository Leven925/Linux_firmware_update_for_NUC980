#include <common.h>
#include <command.h>
#include <image.h>
#include <mapmem.h>
#include "crc_checksum.h"
#include "check_crc.h"

int do_spi_flash_probe(int argc, char * const argv[]);
int do_spi_flash_read_write(int argc, char * const argv[]);


int check_crc(char* addr1, char* addr2)
{
	ulong addr;
	addr = simple_strtoul(addr1, NULL, 16);
	void *hdr = (void *)map_sysmem(addr, 0);
	ulong Image1crc=image_get_hcrc(hdr);
	printf("Image 1 crc=%ld\n",Image1crc);
	addr = simple_strtoul(addr2, NULL, 16);
	hdr = (void *)map_sysmem(addr, 0);
	ulong Image2crc=image_get_hcrc(hdr);
	printf("Image 2 crc=%ld\n",Image2crc);	
	if(Image1crc==Image2crc)
		return 1;
	else
		return 0;
}
