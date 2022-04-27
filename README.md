# Linux_firmware_update_for_NUC980
This firmware update method separates Flash into several partitions below and copy new Linux kernel to original Linux kernel partition at uboot step.
uboot, original Linux kernel, new Linux kernel, Flash root file system
When you update the new Linux kenrel, it will check the new Linux kernel image crc if it is complete.
If it isn't compelte, uboot will boot from original Linux kernel.

Install Steps.

1. Copy check_crc.h, check_crc.c, crc_checksum.h, crc_checksum.c, ota_update.c to /Buildroot/output/build/uboot-master/common/
2. Add the following code to Makefile
   obj-y += ota_update.o
   obj-y += check_crc.o
   obj-y += crc_checksum.o
3. Compile uboot at /Buildroot/output/build/uboot-master/
   /Buildroot/output/build/uboot-master/$ make
4. Use uboot.txt as uboot environment
   This will separate Flash to below
   partition 0 : 0x0-0x200000 (uboot)
   partition 1 : 0x200000-0x800000 (Linux kernel 1)
   partition 2 : 0x800000-0x1400000 (Linux kernel 2)
   partition 3 : 0x1400000- (root filesystem)
   This uboot environment will auto detect if Linux kernel 1 is the same as Linux kernel 2 , auto copy Linux kernel 2 to Linux kernel 1, boot from Linux kernel 1.
   
Programming address
Image  Type  Address
1. u-boot-spl.bin Loader 0x200
2. u-boot.bin Data 0x100000
3. uboot.txt Environment 0x80000
4. uImage1 Data 0x200000
5. uImage2 Data 0x800000
6. rootfs.yaffs2 0x1400000
