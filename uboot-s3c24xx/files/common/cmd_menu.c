 /*
  * (C) Copyright 2012 by OpenWrt.org.cn
  * Author: Lintel<lintel.huang@gmail.com>
  *         Richard Nee <richard.nee.cn@gmail.com>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation; either version 2 of
  * the License, or (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
  * MA 02111-1307 USA
  *
  *
  *      U-BOOT DOWNLOAD MENU
  *
  *      2012.12.19/2011.09.02
  */

#include <common.h>
#include <command.h>
#include <config.h>
//#include <asm/mach-types.h>
#ifdef CONFIG_USB_DEVICE
extern void udc_disconnect(void);
extern void usb_init_slave(void);
#endif

#define DEFAULT_KERNEL_RAM_BASE	"0x30008000"
#define DEFAULT_DL_RAM_BASE	"0x32000000"

extern char console_buffer[];
extern int readline (const char *const prompt);
extern void download_nkbin_to_flash(void);
extern int boot_zImage(ulong from, size_t size);


static char awaitkey(unsigned long delay, int* error_p)
{
	int i;
	if (delay == -1) {
		while (1)
			if (tstc()) /* we got a key press */
				return getc();
	} else {

		for (i = 0; i < delay; i++) {
			if (tstc()) /* we got a key press */
				return getc();
			udelay (10*1000);
		}
	}

	if (error_p)  *error_p = -1;

	return 0;

}


/*
  check the download modes 
  default: for usb dnw2 download
  0 for usb dnw2 download
  1 for tftp download
  2 for serial(kermit) download
  3 for serial(xmode) download
  4 for sd/mmc download
  5 for usb storage download
*/


int download(int modes, char* dl_addr)
{
	char cmd_buf[256];

	switch(modes) {
	case 0:
		printf("\nLoad file from USB Slave ... \n");
		setenv("filesize","");
		//usbslave 1 <dl_addr>
		strcpy(cmd_buf, "usbslave 1 ");
		strcat(cmd_buf, dl_addr);

		run_command(cmd_buf, 0);
		if (simple_strtoul(getenv ("filesize"), NULL, 16)<16)
			return -1;
		break;
	case 1:
		if (getenv("ethact")==NULL) {
			printf("\nSorry,you have no a eth ... \n");
			return -1;
			break;
		}

		printf("\nLoad file from network use tftp ... \n");
		setenv("filesize","");
		//tftp <dl_addr> <filename>
		strcpy(cmd_buf, "tftp  ");
		strcat(cmd_buf, dl_addr);
		strcat(cmd_buf, "  ");

		printf("Please filename from tftp-server: ");
		memset(console_buffer,NULL,CONFIG_SYS_CBSIZE);
		readline(NULL);
		strcat(cmd_buf, console_buffer);

		run_command(cmd_buf, 0);
		if (simple_strtoul(getenv ("filesize"), NULL, 16)<16)
			return -1;
		break;
	case 2:
		printf("\nLoad file from serial with kermit mode ... \n");
		setenv("filesize","");
		//loadb <dl_addr>
		strcpy(cmd_buf, "loadb  ");
		strcat(cmd_buf, dl_addr);

		run_command(cmd_buf, 0);
		if (simple_strtoul(getenv ("filesize"), NULL, 16)<16)
			return -1;
		break;
	case 3:
		printf("\nLoad file from serial with xmode ... \n");
		setenv("filesize","");
		//loadx <dl_addr>
		strcpy(cmd_buf, "loadx  ");
		strcat(cmd_buf, dl_addr);

		run_command(cmd_buf, 0);
		if (simple_strtoul(getenv ("filesize"), NULL, 16)<16)
			return -1;
		break;
	case 4:
		printf("\nLoad file form SD/MMC...\n");
		setenv("filesize","");
		run_command("mmc init 0", 0);
		run_command("fatls mmc 0", 0);
		//fatload mmc 0 <dl_addr> <filename>
		strcpy(cmd_buf, "fatload mmc 0 ");
		strcat(cmd_buf, dl_addr);
		strcat(cmd_buf, "  ");

		printf("filename to load from MMC/SD: ");
		memset(console_buffer,NULL,CONFIG_SYS_CBSIZE);
		readline(NULL);
		strcat(cmd_buf, console_buffer);

		run_command(cmd_buf, 0);
		if (simple_strtoul(getenv ("filesize"), NULL, 16)<16)
			return -1;
		break;
	case 5:
		printf("\nLoad file form USB Disk...\n");
		setenv("filesize","");
		run_command("usb start", 0);
		run_command("fatls usb 0", 0);
		//fatload usb 0 <dl_addr> <filename>
		strcpy(cmd_buf, "fatload usb 0 ");
		strcat(cmd_buf, dl_addr);
		strcat(cmd_buf, "  ");

		printf("filename to load from USB Disk: ");
		memset(console_buffer,NULL,CONFIG_SYS_CBSIZE);
		readline(NULL);
		strcat(cmd_buf, console_buffer);

		run_command(cmd_buf, 0);
		if (simple_strtoul(getenv ("filesize"), NULL, 16)<16)
			return -1;
		break;
	default:
		printf("\nLoad file from USB Slave ... \n");
		setenv("filesize","");
		//usbslave 1 <dl_addr>
		strcpy(cmd_buf, "usbslave 1 ");
		strcat(cmd_buf, dl_addr);

		run_command(cmd_buf, 0);
		if (simple_strtoul(getenv ("filesize"), NULL, 16)<16)
			return -1;
		break;
	}

	return simple_strtoul(getenv ("filesize"), NULL, 16);

}


void main_menu_usage(int mode)
{
	printf("\r\n+-------------- U-BOOT MEMU ---------------+\r\n");
	switch(mode) {
	case 0:
	printf("DOWNLOAD MODE:   USB Slave                 |\r\n");
		break;
	case 1:
	printf("DOWNLOAD MODE:     TFTP                    |\r\n");
		break;
	case 2:
	printf("DOWNLOAD MODE: Serial(kermit mode)         |\r\n");
		break;
	case 3:
	printf("DOWNLOAD MODE:  Serial(xmode)              |\r\n");
		break;
	case 4:
	printf("DOWNLOAD MODE:     SD/MMC                  |\r\n");
		break;
	case 5:
	printf("DOWNLOAD MODE:    USB Disk                 |\r\n");
		break;
	default:
	printf("DOWNLOAD MODE:   USB Slave                 |\r\n");
		break;
	}

	printf("[c] Change Download Mode                   |\r\n");
	printf("[0] U-boot                  --> NOR  Flash |\r\n");
	printf("[1] U-boot                  --> Nand Flash |\r\n");
	printf("[2] Linux Kernel[uImage]    --> Nand Flash |\r\n");
	printf("[3] Linux Kernel[zImage]    --> Nand Flash |\r\n");
	printf("[4] Squashfs/Ramdisk Rootfs --> Nand Flash |\r\n");
	printf("[5] Jffs2 Rootfs            --> Nand Flash |\r\n");
	printf("[6] Yaffs2 Rootfs           --> Nand Flash |\r\n");
	printf("[7] Linux Kernel            --> SDRAM      |\r\n");
	printf("[8] Test Applications       --> SDRAM      |\r\n");
	printf("[l] Set Boot Command Line   --> Linux      |\r\n");
	printf("[m] Set Multi Bootcmd       -->            |\r\n");
	printf("[s] Set U-boot System ENV   -->            |\r\n");
	printf("[e] Reset to Default Settings              |\r\n");
	printf("[f] Format NAND Flash for Linux            |\r\n");
	printf("[p] Show MTD Partition                     |\r\n");
	printf("[b] Boot Linux                             |\r\n");
	printf("[r] Restart System                         |\r\n");
	printf("[q] Back to Shell                          |\r\n");
	printf("Boot From:%s FLASH                       |\r\n", (bBootFrmNORFlash == 1?"NOR ":"NAND"));
	printf("+------------------------------------------+\r\n");
	printf("Choose a job:");

}


void menu_shell(void)
{
	char c;
	char cmd_buf[256];
	static int mode=0;
	while (1) {
	/*
	check the download modes 
	0 for usb download
	1 for tftp download
	2 for serial(kermit) download
	3 for serial(xmode) download
	4 for sd/mmc download
	5 for usb storage download
	*/

	//bBootFrmNORFlash == 1 ==> NOR BOOT
	//bBootFrmNORFlash == 0 ==> NAND BOOT

	main_menu_usage(mode);

	c = awaitkey(-1, NULL);
	printf("%c\n", c);
	switch (c) {
	case 'c': {
		mode++;
		if(mode>=6) mode=0;
		break;
	} case 's': {
		system_setting_shell();
		break;
	} case 'b': {
		strcpy(cmd_buf, "bootd");
		run_command(cmd_buf, 0);
		break;
	} case 'f': {
		if (bBootFrmNORFlash == 1)
			strcpy(cmd_buf, "mtdpart default; nand erase;saveenv");
		else {
			printf("\nBoot From NAND !\nNot erase bootloader.\n ");
			strcpy(cmd_buf, "mtdpart default; nand erase 0x00080000; saveenv");
		}

		run_command(cmd_buf, 0);
		break;
	} case 'm': {
		multiboot_shell();
		break;
	} case 'r': {
		strcpy(cmd_buf, "reset");
		run_command(cmd_buf, 0);
		break;
	} case 'p': {
		strcpy(cmd_buf, "mtdparts");
		run_command(cmd_buf, 0);
		printf("\nPress any key...\n");
		awaitkey(-1, NULL);
		break;
	} case 'e': {
		printf("Erase u-boot-env...");
		run_command("nand erase uboot-env;reset",0);
		break;
	} case  27: case 'q': {
		return;
		break;

	} case '0': {
		printf("\n# Warning: U-Boot ==> NOR FLASH !\nThis operation may brick your board ! Continue ? (y/n)");
		c = awaitkey(-1, NULL);
		if (c=='n'||c=='N') break;
		printf("\nDownload U-boot Bootloader ... \n");
		if(download(mode, DEFAULT_DL_RAM_BASE)<0) break;
		strcpy(cmd_buf, "protect off all;erase 0x0000 +0x80000;cp.b ");
		strcat(cmd_buf, DEFAULT_DL_RAM_BASE);
		strcat(cmd_buf, " 0x0 0x80000"); //protect off all;erase 0x0000 +0x80000;cp.b <dl_addr> 0x0 0x80000
		if (simple_strtoul(getenv ("filesize"), NULL, 16)>16)
			run_command(cmd_buf, 0);
		break;
	} case '1': {
		printf("\n# Warning: U-Boot ==> NAND FLASH !\nThis operation may brick your board ! Continue ? (y/n)");
		c = awaitkey(-1, NULL);
		if (c=='n'||c=='N') break;
		printf("\nDownload U-boot Bootloader ... \n");
		if(download(mode, DEFAULT_DL_RAM_BASE)<0) break;
		strcpy(cmd_buf, "nand erase uboot; nand write ");
		strcat(cmd_buf, DEFAULT_DL_RAM_BASE);
		strcat(cmd_buf, " uboot"); //nand erase uboot; nand write <dl_addr> uboot
		if (simple_strtoul(getenv ("filesize"), NULL, 16)>16)
			run_command(cmd_buf, 0);
		break;
	} case '2': {
		printf("\nDownLoad uImage Kernel ... \n");
		download(mode, DEFAULT_DL_RAM_BASE);
		strcpy(cmd_buf, "nand erase kernel; nand write ");
		strcat(cmd_buf, DEFAULT_DL_RAM_BASE);
		strcat(cmd_buf, " kernel"); //nand erase kernel; nand write <dl_addr> kernel
		if (simple_strtoul(getenv ("filesize"), NULL, 16)>0x10) {
			run_command(cmd_buf, 0);
			//nboot <kernel_addr> kernel;bootm
			strcpy(cmd_buf, "nboot ");
			strcat(cmd_buf, DEFAULT_KERNEL_RAM_BASE);
			strcat(cmd_buf, " kernel;bootm");
			setenv("bootcmd", cmd_buf);
			run_command("saveenv", 0);
		}
		break;
	} case '3': {
		printf("\nDownLoad zImage Kernel ... \n");
		download(mode, DEFAULT_DL_RAM_BASE);
		strcpy(cmd_buf, "nand erase kernel; nand write ");
		strcat(cmd_buf, DEFAULT_DL_RAM_BASE);
		strcat(cmd_buf, " kernel"); //nand erase kernel; nand write <dl_addr> kernel
		if (simple_strtoul(getenv ("filesize"), NULL, 16)>0x10) {
			run_command(cmd_buf, 0);
			//nand read <kernel_addr> kernel;bootz
			strcpy(cmd_buf, "nand read ");
			strcat(cmd_buf, DEFAULT_KERNEL_RAM_BASE);
			strcat(cmd_buf, " kernel;bootz");
			setenv("bootcmd", cmd_buf);
			run_command("saveenv", 0);
		}
		break;
	} case '4': {
		printf("\nDownLoad squashfs rootfs ... \n");
		download(mode, DEFAULT_DL_RAM_BASE);
		strcpy(cmd_buf, "nand erase rootfs; nand write ");
		strcat(cmd_buf, DEFAULT_DL_RAM_BASE);
		strcat(cmd_buf, " rootfs ");
		strcat(cmd_buf, getenv ("filesize")); //nand erase rootfs; nand write <dl_addr> rootfs <filesize>
		printf("Received File Size: 0X%s\n", getenv ("filesize"));
		if (simple_strtoul(getenv ("filesize"), NULL, 16)>0x10) {
			run_command(cmd_buf, 0);
			run_command("setenv bootargs root=/dev/mtdblock3  console=ttySAC0 rootfstype=squashfs,ext2;saveenv", 0);
		}
		break;
	} case '5': { //jffs2
		printf("\nDownLoad jffs rootfs ... \n");
		download(mode, DEFAULT_DL_RAM_BASE);
		strcpy(cmd_buf, "nand erase rootfs; nand write ");
		strcat(cmd_buf, DEFAULT_DL_RAM_BASE);
		strcat(cmd_buf, " rootfs ");
		strcat(cmd_buf, getenv ("filesize")); //nand erase rootfs; nand write <dl_addr> rootfs <filesize>
		printf("Received File Size: 0X%s\n", getenv ("filesize"));
		run_command(cmd_buf, 0);
		run_command("setenv bootargs noinitrd root=/dev/mtdblock3  console=ttySAC0 rootfstype=jffs2;saveenv", 0);
		break;
	} case '6': { //yaffs
		printf("\nDownLoad yaffs2 rootfs ... \n");
		download(mode, DEFAULT_DL_RAM_BASE);
		strcpy(cmd_buf, "nand erase rootfs; nand write.yaffs ");
		strcat(cmd_buf, DEFAULT_DL_RAM_BASE);
		strcat(cmd_buf, " rootfs ");
		strcat(cmd_buf, getenv ("filesize")); //nand erase rootfs; nand write.yaffs <dl_addr> rootfs <filesize>
		printf("Received File Size: 0X%s\n", getenv ("filesize"));
		run_command(cmd_buf, 0);
		run_command("setenv bootargs noinitrd root=/dev/mtdblock3  console=ttySAC0 rootfstype=yaffs2;saveenv", 0);
		break;
	} case '7': {
		if(download(mode, DEFAULT_KERNEL_RAM_BASE) < 0) break;
		strcpy(cmd_buf, "bootm ");
		strcat(cmd_buf, DEFAULT_KERNEL_RAM_BASE);
		strcat(cmd_buf, " ; bootz"); //bootm <kernel_addr>;bootz
		if (simple_strtoul(getenv ("filesize"), NULL, 16)>16)
			run_command(cmd_buf, 0);
		break;
	} case '8': {
		printf("Download application to %s...\n", DEFAULT_DL_RAM_BASE);
		printf("Clean Memory at %s Len 0x400000...", DEFAULT_DL_RAM_BASE);
		strcpy(cmd_buf, "mw.b ");
		strcat(cmd_buf, DEFAULT_DL_RAM_BASE);
		strcat(cmd_buf, " 0xFF 0x400000"); //mw.b <dl_addr> 0xFF 0x400000
		run_command(cmd_buf, 0);
		printf("OK\n");
		if (download(mode, DEFAULT_DL_RAM_BASE)>=0)
			strcpy(cmd_buf, "go ");
			strcat(cmd_buf, DEFAULT_DL_RAM_BASE); // go <dl_addr>
			run_command(cmd_buf, 0);
		break;
	} case 'l': {
		printf("Current Boot Command Line \'bootargs\': %s\n",getenv("bootargs"));
		printf("New Boot Command Line \'bootargs\': ");
		memset(console_buffer,NULL,CONFIG_SYS_CBSIZE);
		readline(NULL);
		printf("Save Boot Command Line \'bootargs\' ? (n/y) ");
		c = awaitkey(-1, NULL);
		if(c=='y'||c=='Y') {
			setenv("bootargs",console_buffer);
			run_command("saveenv",0);
			break;
		}
	}
	}
    }
}


void multiboot_menu_usage(void)
{
	printf("\r\n+-------------- U-BOOT MEMU ---------------+\r\n");
	printf("|               Multi Boot                 |\r\n");
	printf("[0] Boot Linux from USB Disk    [zImage]   |\r\n");
	printf("[1]         --> set as bootcmd             |\r\n");
	printf("[2] Boot Linux from MMC/SD Card [zImage]   |\r\n");
	printf("[3]         --> set as bootcmd             |\r\n");
	printf("[4] Boot Linux from TFTP        [zImage]   |\r\n");
	printf("[5]         --> set as bootcmd             |\r\n");
	printf("[6] Boot Linux from NAND Flash  [zImage]   |\r\n");
	printf("[7]         --> set as bootcmd             |\r\n");
	printf("[8] Boot Linux from NAND Flash  [uImage]   |\r\n");
	printf("[9]         --> set as bootcmd             |\r\n");
	printf("[s] Set Other Boot Command \'bootcmd\'       |\r\n");
	printf("[q] Back to Main Menu                      |\r\n");
	printf("Boot From:%s FLASH                       |\r\n", (bBootFrmNORFlash == 1?"NOR ":"NAND"));
	printf("+------------------------------------------+\r\n");
	printf("Choose a job:");
}


void multiboot_shell(void)
{
	char c;
	char cmd_buf[256];

	while (1) {

	multiboot_menu_usage();

	c = awaitkey(-1, NULL);
	printf("%c\n", c);
	switch (c) {
	case '0': {
		printf("Load kernel form USB Disk...\n");
		run_command("usb start", 0);
		run_command("fatls usb 0", 0);
		strcpy(cmd_buf, "fatload usb 0 ");
		strcat(cmd_buf, DEFAULT_KERNEL_RAM_BASE);
		strcat(cmd_buf, "  ");
		printf("filename to load from USB Disk: ");
		memset(console_buffer,NULL,CONFIG_SYS_CBSIZE);
		readline(NULL);
		strcat(cmd_buf, console_buffer);
		run_command(cmd_buf, 0);
		if (simple_strtoul(getenv ("filesize"), NULL, 16)>16)
			run_command("bootz", 0);
		break;
	} case '2': {
		printf("Load kernel form SD/MMC...\n");
		run_command("mmc init 0", 0);
		run_command("fatls mmc 0", 0);
		strcpy(cmd_buf, "fatload mmc 0 ");
		strcat(cmd_buf, DEFAULT_KERNEL_RAM_BASE);
		strcat(cmd_buf, "  ");
		printf("filename to load from MMC/SD: ");
		memset(console_buffer,NULL,CONFIG_SYS_CBSIZE);
		readline(NULL);
		strcat(cmd_buf, console_buffer);
		run_command(cmd_buf, 0);
		if (simple_strtoul(getenv ("filesize"), NULL, 16)>16)
			run_command("bootz", 0);
		break;
	} case '4': {
		if (getenv("ethact")==NULL) {
			printf("\nSorry,you have no a eth ... \n");
			return -1;
			break;
		}

		printf("\nLoad kernel from network use tftp ... \n");
		setenv("filesize","");
		//tftp <dl_addr> <filename>
		strcpy(cmd_buf, "tftp  ");
		strcat(cmd_buf, DEFAULT_KERNEL_RAM_BASE);
		strcat(cmd_buf, "  ");

		printf("Please filename from tftp-server: ");
		memset(console_buffer,NULL,CONFIG_SYS_CBSIZE);
		readline(NULL);
		strcat(cmd_buf, console_buffer);

		run_command(cmd_buf, 0);
		if (simple_strtoul(getenv ("filesize"), NULL, 16)>16)
			run_command("bootz", 0);
		break;
	} case '6': {
		strcpy(cmd_buf, "nand read ");
		strcat(cmd_buf, DEFAULT_KERNEL_RAM_BASE);
		strcat(cmd_buf, " kernel;bootz");
		run_command(cmd_buf, 0);
		break;
	} case '8': {
		strcpy(cmd_buf, "nboot ");
		strcat(cmd_buf, DEFAULT_KERNEL_RAM_BASE);
		strcat(cmd_buf, " kernel;bootm");
		run_command(cmd_buf, 0);


	} case '1': { //usb start;fatload usb 0 <kernel_addr> zImage;bootz
		strcpy(cmd_buf, "usb start;fatload usb 0 ");
		strcat(cmd_buf, DEFAULT_KERNEL_RAM_BASE);
		strcat(cmd_buf, " zImage;bootz");
		setenv("bootcmd", cmd_buf);
		run_command("saveenv", 0);
		break;
	} case '3': { //mmc init;fatload mmc 0 <kernel_addr> zImage;bootz
		strcpy(cmd_buf, "mmc init 0;fatload mmc 0 ");
		strcat(cmd_buf, DEFAULT_KERNEL_RAM_BASE);
		strcat(cmd_buf, " zImage;bootz");
		setenv("bootcmd", cmd_buf);
		run_command("saveenv", 0);
		break;
	} case '5': { //tftp <kernel_addr> zImage;bootz
		if (getenv("ethact")==NULL) {
			printf("\nSorry,you have no a eth ... \n");
			return -1;
			break;
		}

		strcpy(cmd_buf, "tftp  ");
		strcat(cmd_buf, DEFAULT_KERNEL_RAM_BASE);
		strcat(cmd_buf, " zImage;bootz");
		setenv("bootcmd", cmd_buf);
		run_command("saveenv", 0);
		break;
	} case '7': {
		strcpy(cmd_buf, "nand read ");
		strcat(cmd_buf, DEFAULT_KERNEL_RAM_BASE);
		strcat(cmd_buf, " kernel;bootz");
		setenv("bootcmd", cmd_buf);
		run_command("saveenv", 0);
		break;
	} case '9': {
		strcpy(cmd_buf, "nboot ");
		strcat(cmd_buf, DEFAULT_KERNEL_RAM_BASE);
		strcat(cmd_buf, " kernel;bootm");
		setenv("bootcmd", cmd_buf);
		run_command("saveenv", 0);
		break;
	} case 's': {
		printf("Current Boot Command \'bootcmd\': %s\n",getenv("bootcmd"));
		printf("New Boot Command \'bootcmd\': ");
		memset(console_buffer,NULL,CONFIG_SYS_CBSIZE);
		readline(NULL);
		printf("Save New Boot Command \'bootcmd\' ? (n/y) ");
		c = awaitkey(-1, NULL);
		if(c=='y'||c=='Y') {
			setenv("bootcmd",console_buffer);
			run_command("saveenv",0);
			break;
		}
		break;
	} case  27: case 'q': {
		return;
		break;
	}
	}
    }
}


void system_setting_menu_usage(void)
{
	printf("\r\n+-------------- U-BOOT MEMU ---------------+\r\n");
	printf("|             System Settings              |\r\n");
	printf("[1] Print U-boot Parameter                 |\r\n");
	printf("[2] Edit a U-boot Parameter                |\r\n");
	printf("[3] Select Board Types          -->        |\r\n");
	printf("[4] Nand Scrub - NAND Erasing (UNSAFE)     |\r\n");
	printf("[r] Reset to Default Settings              |\r\n"); 
	printf("[s] Save Settings to Flash                 |\r\n");
	printf("[q] Back to Main Menu                      |\r\n");
	printf("Boot From:%s FLASH                       |\r\n", (bBootFrmNORFlash == 1?"NOR ":"NAND"));
	printf("+------------------------------------------+\r\n");
	printf("Choose a job:");
}


void system_setting_shell(void)
{
	char c;
	char cmd_buf[256];
	while (1) {

	system_setting_menu_usage();

	c = awaitkey(-1, NULL);
	printf("%c\n", c);
	switch (c) {
	case '1': {
		strcpy(cmd_buf, "printenv ");
		printf("Name(enter to view all paramters): ");
		readline(NULL);
		strcat(cmd_buf, console_buffer);
		run_command(cmd_buf, 0);
		printf("Press any key...");
		awaitkey(-1, NULL);
		break;
	} case '2': {
		sprintf(cmd_buf, "setenv ");
		printf("Name: ");
		readline(NULL);
		strcat(cmd_buf, console_buffer);
		printf("Value: ");
		readline(NULL);
		strcat(cmd_buf, " ");
		strcat(cmd_buf, console_buffer);
		run_command(cmd_buf, 0);
		break;
	} case '3': {
		matchtype_setting_shell();
		break;
	} case '4': {
		strcpy(cmd_buf, "nand scrub ");
		run_command(cmd_buf, 0);
		break;
	} case 'r': {
		printf("Erase u-boot-env...");
		strcpy(cmd_buf, "reset");
		run_command(cmd_buf, 0);
		break;
	} case 's':{
		run_command("saveenv", 0);
		break;
	} case  27: case 'q': {
		return;
	}
	}
    }
}


void machtype_menu_usage(void)
{
	printf("\r\n+-------------- U-BOOT MEMU ---------------+\r\n");
	printf("|           Select Board Types             |\r\n");
	printf("S3C2410:                                   |\r\n");
	printf("[1] SMDK2410                               |\r\n");
	printf("[2] QT2410                                 |\r\n");
	printf("[3] FS2410                                 |\r\n");
	printf("[4] GEC2410 GEC2440 V1.0 (CS8900A)         |\r\n");
	printf("S3C2440:                                   |\r\n");
	printf("[5] SMDK2440                               |\r\n");
	printf("[6] QQ2440                                 |\r\n");
	printf("[7] MINI2440 Micro2440                     |\r\n");
	printf("[8] GEC2440 V1.1 (DM9000AEP)               |\r\n");
	printf("[9] TQ2410                                 |\r\n");
	printf("[o] Other Boards     -->                   |\r\n");
	printf("[s] Save Settings to Flash                 |\r\n");
	printf("[q] Back to Upset Menu                     |\r\n");
	printf("Boot From:%s FLASH                       |\r\n", (bBootFrmNORFlash == 1?"NOR ":"NAND"));
	printf("+------------------------------------------+\r\n");
	printf("Choose a job:");
}

void matchtype_setting_shell(void)
{
	char c;
	char cmd_buf[256];

	while (1) {

		machtype_menu_usage();
		c = awaitkey(-1, NULL);
		printf("%c\n", c);
		switch (c) {
		case '1': {
			setenv("machid","193");
			break;
		} case '2': {
			setenv("machid","1008");
			break;
		} case '3': {
			setenv("machid","912");
			setenv ("cs8900_base","0x19000300");
			break;
		} case '4': {
			setenv("machid","2315");
			setenv ("cs8900_base","0x19000300");
			break;
		} case '5': {
			setenv("machid","1008");
		break;
		} case '6': {
			setenv("machid","1998");
			break;
		} case '7': {
			setenv("machid","1999");
			setenv ("dm9000_base","0x20000300");
			setenv ("dm9000_io","0x20000300");
			setenv ("dm9000_data","0x20000304");
			break;
		} case '8': {
			setenv("machid","2316");
			setenv ("dm9000_base","0x10000000");
			setenv ("dm9000_io","0x10000000");
			setenv ("dm9000_data","0x11000000");
			break;
		} case '9': {
			setenv("machid","2682");
			setenv ("dm9000_base","0x20000300");
			setenv ("dm9000_io","0x20000300");
			setenv ("dm9000_data","0x20000304");
			break;
		} case 'o': {
			printf("Current MACH_TYPE_ID:%s\n",getenv("machid"));
			printf("New MACH_TYPE_ID:");
			memset(console_buffer,NULL,CONFIG_SYS_CBSIZE);
			readline(NULL);
			setenv("machid",console_buffer);
			break;
		} case 's': {
			run_command("saveenv", 0);
			break;
		} case 'q': case  27: {
			return;
			break;
		}
		}
	}
}


void first_boot_set_machtype (void)
{
	unsigned int id_mach = simple_strtoul (getenv ("machid"), NULL, 16);
	if(id_mach == NULL){
		printf("\r\n---------------------- NO SETTING MACH_TYPE ----------------------\r\n");
		printf("--- #NOTE : Please follow the steps to set MACH_TYPE           ---\r\n");
		printf("--- Step 1: Hit [0 - 9] to select BOARD TYPE and set MACH_TYPE ---\r\n");
		printf("--- Step 2: Hit [s] to save settings                           ---\r\n");
		printf("--- Step 3: Hit [q] to quit and reset                          ---\r\n");
		printf("------------------------------------------------------------------\r\n");

		matchtype_setting_shell();
		run_command("reset", 0);
	}
}

int do_machtype (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	switch (argc) {
	case 2: //argc==2(like: "machtype 0"), means: first_boot
		first_boot_set_machtype();
		break;
	default://already setting machid, reset now
		matchtype_setting_shell();
		break;
	}

	return 0;
}


int do_menu (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	first_boot_set_machtype();
	menu_shell();
	return 0;
}

U_BOOT_CMD(menu, 1, 0, do_menu,
"U-BOOT DOWNLOAD MENU",
"# press the following keys to use the corresponding function :\n"
"    [0] U-boot                  --> Nand Flash\n"
"    [1] Linux kernel[uImage]    --> Nand Flash\n"
"    [2] Linux kernel[zImage]    --> Nand Flash\n"
"    [3] Squashfs/ramdisk rootfs --> Nand Flash\n"
"    [4] Jffs2 rootfs            --> Nand Flash\n"
"    [5] Yaffs2 rootfs           --> Nand Flash\n"
"    [6] Linux kernel            --> Ram & Run\n"
"    [7] Test applications       --> Ram & Run\n"
"    [8] Set boot command line  --> linux\n"
"    [f] Format nandflash for linux\n"
"    [v] View mtd partition\n"
"    [c] Change download Mode\n"
"    [s] Setting  parameter     -->\n"
"    [e] Reset settings to default\n"
"    [b] Boot linux\n"
"    [m] Multi boot             -->\n"
"    [r] Restart system\n"
"    [q] Back to shell\n"
);

U_BOOT_CMD(machtype, 2, 0, do_machtype,
"select Board Types and set machid",
"# press the following keys to select the corresponding Board Types :\n"
"    S3C2410:\n"
"    [1] SMDK2410\n"
"    [2] QT2410\n"
"    [3] FS2410\n"
"    [4] GEC2410 GEC2440 V1.0 (CS8900A)\n"
"    S3C2440:\n"
"    [5] SMDK2440\n"
"    [6] QQ2440\n"
"    [7] MINI2440 Micro2440\n"
"    [8] GEC2440 V1.1 (DM9000AEP)\n"
"    [9] TQ2410\n"
"    [o] Other Boards\n"
"    [s] Save Settings to Flash\n"
"    [q] back to Upset Menu\n"
);

