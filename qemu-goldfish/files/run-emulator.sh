#!/bin/bash

BASE=$(dirname $0)
PREFIX=openwrt-goldfish-
QEMU_NAME=qemu-goldfish
SKIN_NAME=HVGA2

echo "Starting in $BASE/ DIR"

touch $BASE/${PREFIX}data.bin

$BASE/$QEMU_NAME/emulator \
	-skindir $BASE/$QEMU_NAME/skins -skin $SKIN_NAME \
	-sysdir $BASE \
	-ramdisk $BASE/${PREFIX}ramdisk.bin \
	-kernel $BASE/${PREFIX}kernel.bin \
	-system $BASE/${PREFIX}system.bin \
	-data $BASE/${PREFIX}data.bin \
	-show-kernel \
	-partition-size 100 \
	-qemu -append "console=ttyS0 ubi.mtd=0 root=/dev/mtdblock3 rootfstype=jffs2 bootdelay=1 init=/etc/preinit" \
	-net nic,vlan=0,model=smc91c111 \
	-net user,vlan=0,hostname=OpenWrt
