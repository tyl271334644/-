#!/bin/bash
set -e

CURDIR=$PWD 
SRCDIR=$CURDIR/src

OPENWRT=/media/work/FScar/staging_dir_AR9331-AL3A/toolchain-mips_34kc_gcc-5.3.0_musl-1.1.14
#OPENWRT=/media/work/FScar/staging_dir_WT7620N/toolchain-mipsel_24kec+dsp_gcc-5.3.0_musl-1.1.14
PATH=$PATH:$OPENWRT/bin

CROSS_COMPILE=mips-openwrt-linux-
#CROSS_COMPILE=mipsel-openwrt-linux-
CC=${CROSS_COMPILE}gcc
AR=${CROSS_COMPILE}ar
LD=${CROSS_COMPILE}ld

export PATH CC AR LD
export STAGING_DIR=$OPENWRTSRC/toolchain-mips_34kc_gcc-5.3.0_musl-1.1.14/bin:$STAGING_DIR

OKCOLOR=31
ERRCOLOR=34

function usage()
{
	echo -e "\033[0;$ERRCOLOR;1m\t#################################\033[0m"
	echo -e "\033[0;$ERRCOLOR;1m\t#Usage:                         #\033[0m"
	echo -e "\033[0;$ERRCOLOR;1m\t#     Compile: ./build.sh       #\033[0m"
	echo -e "\033[0;$ERRCOLOR;1m\t#     Clean  : ./build.sh clean #\033[0m"
	echo -e "\033[0;$ERRCOLOR;1m\t#################################\033[0m"
}

function build_server()
{
	cd $SRCDIR
	make
	if [ $? = 0 ] ; then
		echo -e "\033[0;$OKCOLOR;1m\t#build server done.#\033[0m"
	else
		echo -e "\033[0;$ERRCOLOR;1m\t#build server fail.#\033[0m"
	fi		
}

case "$1" in
	clean)
		if [ -d $SRCDIR ] ; then
			cd $SRCDIR
			make clean
		fi
		;;

	*)
		build_server
		;;
esac
