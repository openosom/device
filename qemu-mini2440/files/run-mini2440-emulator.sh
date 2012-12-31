#!/bin/bash
#

BASE="`pwd`"
SCRIPTS_DIR=$BASE/qemu-mini2440/mini2440

. $SCRIPTS_DIR/env

# nand flash image
if [ ! -e $SCRIPTS_DIR/$flash_base -o ! -e $SCRIPTS_DIR/$flash_image ]; then
	$SCRIPTS_DIR/flash.sh $BASE
fi

if [ ! -e $BASE/u-boot.bin -a -e $SCRIPTS_DIR/u-boot.bin ]; then
	ln -sv $SCRIPTS_DIR/u-boot.bin $BASE/u-boot.bin
fi

sleep 1

$SCRIPTS_DIR/qemu-auto.sh

