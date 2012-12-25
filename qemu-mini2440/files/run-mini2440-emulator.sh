#!/bin/bash
#
# Run with script with 
# -sd <SD card image file> to have a SD card loaded
# -kernel <kernel uImage file> to have a kernel preloaded in RAM
#

BASE=$(dirname $0)
QEMU_DIR=$BASE/qemu-mini2440
NAND_NAME=$QEMU_DIR/NAND128M.bin

KERNEL=$BASE/openwrt-s3c24xx-dev-mini2440-uImage

if [ ! -f $NAND_NAME ]; then
	echo "$0 : creating NAND empty image : $NAND_NAME"
#	dd if=/dev/zero of=$NAND_NAME bs=528 count=131072
	dd if=/dev/zero of=$NAND_NAME bs=2112 count=65536
#	( dd if=nand-heard.bin bs=2112 count=256 conv=sync; \
#	  dd if=/dev/zero bs=2112 count=65280 conv=sync; \
#	) > $NAND_NAME
	echo "** NAND file created - make sure to 'nand scrub' in u-boot"
fi

if [ ! -e $BASE/u-boot.bin -a -e $QEMU_DIR/u-boot.bin ]; then
	ln -sv $QEMU_DIR/u-boot.bin $BASE/u-boot.bin
fi

$QEMU_DIR/qemu-system-arm \
	-M mini2440 \
	-serial stdio \
	-mtdblock $NAND_NAME \
	-kernel $KERNEL \
	-show-cursor \
	-usb -usbdevice keyboard -usbdevice mouse \
	-net nic,vlan=0,model=dm9000 \
	-net user,vlan=0,hostname=OpenWrt \
	-monitor telnet::5555,server,nowait

