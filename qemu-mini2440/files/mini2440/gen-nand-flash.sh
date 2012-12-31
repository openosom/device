#!/bin/bash

. $(dirname $0)/env

# 1
	echo -en \\0377\\0377\\0377\\0377\\0377\\0377\\0377\\0377 > .8b
	cat .8b .8b .8b .8b .8b .8b .8b .8b > .64b # OOB is 64 bytes
	cat .64b .64b .64b .64b .64b .64b .64b .64b > .2048b
	cat .64b .64b .64b .64b .64b .64b .64b .64b >> .2048b
	cat .64b .64b .64b .64b .64b .64b .64b .64b >> .2048b
	cat .64b .64b .64b .64b .64b .64b .64b .64b >> .2048b
	cat .2048b .64b > .sec # A sector is 2048 bytes of data + 64 bytes OOB
	cat .sec .sec .sec .sec .sec .sec .sec .sec > .8sec
	cat .8sec .8sec .8sec .8sec .8sec .8sec .8sec .8sec > .64sec
	cat .64sec .64sec .64sec .64sec .64sec .64sec .64sec .64sec > .512sec
	cat .512sec .512sec .512sec .512sec .512sec .512sec .512sec .512sec > .4ksec
	cat .4ksec .4ksec .4ksec .4ksec .4ksec .4ksec .4ksec .4ksec > $flash_base
	cat .4ksec .4ksec .4ksec .4ksec .4ksec .4ksec .4ksec .4ksec >> $flash_base
	rm -rf .8b .64b .2048b .sec .8sec .64sec .512sec .4ksec

# 2
	../qemu-img create -b $flash_base -f qcow2 $flash_image

