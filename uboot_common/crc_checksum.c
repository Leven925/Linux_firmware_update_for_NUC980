#include <common.h>
#include <command.h>
#include <image.h>
#include <mapmem.h>
#include "crc_checksum.h"

int crc_checksum(char* image_number, char* image_addr)
{
	ulong addr;
	addr = simple_strtoul(image_addr, NULL, 16);
	void *hdr = (void *)map_sysmem(addr, 0);
	int hcrc_flag=image_check_hcrc(hdr);
	if(hcrc_flag==0){
		printf("Image %s Check Fail\n",image_number);
		return 0;
	}
	int dcrc_flag=image_check_dcrc(hdr);
	printf("Image %s Header Check =%d\n",image_number,hcrc_flag);
	printf("Image %s Data Check =%d\n",image_number,dcrc_flag);
	if((hcrc_flag+dcrc_flag)==2){
		printf("Image %s Check Ok\n",image_number);
		return 1;
	}
	else{
		printf("Image %s Check Fail\n",image_number);
		return 0;
	}
}

