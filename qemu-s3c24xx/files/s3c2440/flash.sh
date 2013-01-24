#! /usr/bin/env bash
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#

. $(dirname $0)/env

# flash.sh <img_dir/>
if [[ "$1" != "" ]]; then
	img_dir="`(cd \"$1\"; pwd)`"
else
	img_dir="$script_dir"
fi

cd $script_dir


# Find the most recent images in the current directory.
# We assume they have numeric date or some build number in their names.
most_recent () {
	cd $base_dir
	export $2="`basename \`ls -d -1 $img_dir/$1 | sort -n | tail -n 1\``"
	export $3="`python -c \"import os; print '0x%lx' \
		%(os.stat('$img_dir/${!2}').st_size)\"`"
	cd $script_dir
	[ -e "${!2}" ] || ln -sf $img_dir/${!2} ${!2}
	[ -s "${!2}" ]
}

most_recent "$kernel_wildcard" kernel_image kernel_size || exit -1
most_recent "$rootfs_wildcard" rootfs_image rootfs_size || exit -1
most_recent "$uboot_wildcard" uboot_image uboot_size || exit -1

echo Using \'$kernel_image\'\($kernel_size\) as the kernel image.
echo Using \'$rootfs_image\'\($rootfs_size\) as the root filesystem image.
echo Using \'$uboot_image\'\($uboot_size\) as bootloader.

# for NAND WRITE: SEC=2048Byte+64Byte, 0x800. then change to hex.
kernel_size="`echo $(( ($kernel_size / 0x800 + 1) * 0x800 )) | awk '{printf("%x\n", $0)}'`"
rootfs_size="`echo $(( ($rootfs_size / 0x800 + 1) * 0x800 )) | awk '{printf("%x\n", $0)}'`"
uboot_size="`echo $(( ($uboot_size / 0x800 + 1) * 0x800 )) | awk '{printf("%x\n", $0)}'`"

# Make sure ^C wipes everything we might have spawned along the way.
trap 'kill -- -$$' INT EXIT

# Currently we just make the u-boot image accessible under u-boot.bin
# and qemu will load it from the working directory.  This is a
# temporary solution.
rm -rf $uboot_symlink
ln -s $script_dir/$uboot_image $uboot_symlink

# Make the kernel image accessible under a fixed name
rm -rf $kernel_symlink
ln -s $kernel_image $kernel_symlink


# Make the flash image accessible under a fixed name
rm -rf $flash_base $flash_image
$base_dir/gen-nand-flash.sh || exit -1

# Launch the emulator assuming that u-boot is now functional enough
# for us to be able to issue u-boot commands.
# This is also an example of how you *shouldn't* write scripts.
# We should parse the u-boot output for command prompt lines and only
# issue commands when u-boot is awaiting them.
emu () {
	$qemu -mtdblock "$script_dir/$flash_base" -kernel "$script_dir/$1" \
		-serial stdio -nographic -usb -monitor null <&0 & pid=$!
}
uboot () {
	cd $base_dir
	emu $1 <<< "$3"

	echo Please wait, programming the NAND flash...
	(sleep $2; kill $pid; sleep 1; kill -KILL $pid)& timer=$!
	if ! wait $pid; then
		kill $timer
		echo
		echo U-boot failed to finish writing in $2 seconds, giving up.
		echo
		exit -1
	fi
	kill $timer
	cd $script_dir
}

# Set up MACHID =1008
uboot $uboot_image 5 "

o1008
s
q"

# Set up BBT, u-boot environment, boot menu and program u-boot binary.
# Program the u-boot binary.
uboot $uboot_image 10 "

q
nand write $kernel_addr $uboot_prt_addr $uboot_size
setenv machid 1008
saveenv
print"

# Program the kernel binary.
# XXX. NO WORKING: nand write $kernel_addr kernel $kernel_size
uboot $kernel_image 15 "

q
nand write $kernel_addr $kernel_prt_addr $kernel_size"

# Program the root filesystem.
uboot $rootfs_image 100 "

q
nand write $kernel_addr $rootfs_prt_addr $rootfs_size
setenv machid
setenv bootcmd bootm
saveenv"

echo
echo "    "
echo "    "
echo "    ================================================"
echo "    "
echo "    "
echo "    "All done.
echo
echo "    "Read the qemu manual and use a commandline like the following to boot:
echo \ \$ $qemu_relative -mtdblock $script_dir/$flash_image -kernel $kernel_symlink -usb -show-cursor
echo
echo "    "Append \'-snapshot\' to make the flash image read-only so that every
echo "    "time emulation starts in the original unmodified state.
echo "    "Append \'-usbdevice keyboard\' to attach a standard keyboard.
echo "    "
echo "    "
echo "    ================================================"
echo "    "
