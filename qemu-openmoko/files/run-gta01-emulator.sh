#!/bin/bash
#

BASE=$(dirname $0)
QEMU_DIR=$BASE/qemu-openmoko/openmoko-gta01
SCRIPTS_DIR=.

cd $QEMU_DIR

#1 setup environment
. $SCRIPTS_DIR/env

# which runs the emulator, loads u-boot into it 
#	and then uses u-boot's capability to program the Flash memory 
#	to install all the necessary parts of the system into the virtual Flash. 
# It will also set up all the bootloading process 
#	including a boot menu (ENTER is [AUX] and SPACE is [POWER]), splash, 
#	u-boot environment and some default kernel parameters. 
# If everything goes OK, the script should print a command which you can use to start using the emulator.
if [ ! -e $SCRIPTS_DIR/$flash_base -o ! -e $SCRIPTS_DIR/$flash_image ]; then
	$SCRIPTS_DIR/flash.sh ../..
fi

#3 launches qemu and opens a unix socket for communication. All additional parameters are passed to qemu.
$SCRIPTS_DIR/qemu-auto.sh



#$SCRIPTS_DIR/qemu-upload.sh <filename>
#$SCRIPTS_DIR/qemu-quit.sh
#$SCRIPTS_DIR/qemu-gadget.sh

