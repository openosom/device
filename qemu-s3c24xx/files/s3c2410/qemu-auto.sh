#! /usr/bin/env bash
# Basically just runs qemu, in a way that qemu-cmd.pl works.
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

if [ -e "$qemu_monitor" ]; then
	echo A QEMU session appears to be already active
	exit -1
fi

trap "rm -rf \"$qemu_monitor\" \"$dump_dir\"; stty sane" INT EXIT

# Run with script with 
# -kernel <kernel uImage file> to have a kernel preloaded in RAM
# -sd <SD card image file> to have a SD card loaded
#
#-usbgadget
#-net nic,vlan=0
#-net tap,vlan=0,ifname=tap0
#-monitor telnet::5555,server,nowait

$qemu -mtdblock "$script_dir/$flash_image"		\
	-kernel "$kernel_symlink"	\
	-snapshot -show-cursor -parallel none	\
	-usb -usbdevice keyboard -usbdevice mouse	\
	-serial stdio	\
	-monitor unix:"$qemu_monitor",server,nowait	\
	-net nic,vlan=0,model=dm9000	\
	-net user,vlan=0,hostname=OpenWrt	\
	"$@"

