#!/bin/bash

# dpkg -i 
# overlayroot-disable

if [ "$(id -u)" -ne "0" ];then
	echo "Need root privileges."
	exit 1
fi

export DEBIAN_FRONTEND=noninteractive

# 0 ------ default is glamor
# 1 ------ use SNA accel method
# 2 ------ use UXA accel method

set_intel_accel_sna() {
INTEL_XORG_CONF="/etc/X11/xorg.conf.d/20-intel.conf"
echo 'Section "Device"
Identifier "Intel Graphics"
Driver "intel"
Option "AccelMethod" "sna"
#Option "PageFlip" "False"
#Option "TearFree" "True"
EndSection' | overlayroot-chroot tee $INTEL_XORG_CONF
overlayroot-chroot apt-get install xserver-xorg-video-intel --reinstall -y --allow-downgrades
}

set_intel_accel_uxa() {
INTEL_XORG_CONF="/etc/X11/xorg.conf.d/20-intel.conf"
echo 'Section "Device"
Identifier "Intel Graphics"
Driver "intel"
Option "AccelMethod" "uxa"
#Option "PageFlip" "False"
#Option "TearFree" "True"
EndSection' | overlayroot-chroot tee $INTEL_XORG_CONF
overlayroot-chroot apt-get install xserver-xorg-video-intel --reinstall -y --allow-downgrades
}

systemctl stop lightdm
if [ $1 == "post" ];then
#	overlayroot-chroot apt-get purge nvidia-* -y --allow-downgrades
#	overlayroot-chroot apt-get purge xserver-xorg-video-nouveau -y --allow-downgrades
	overlayroot-chroot apt-get install xserver-xorg-core --reinstall -y --allow-downgrades
	overlayroot-chroot apt-get install xserver-xorg-input-all --reinstall -y --allow-downgrades
	overlayroot-chroot rm -f /etc/X11/xorg.conf
	#根据需求调整显卡的加速方式，从而达到最优化
	case $2 in
		0)echo "Using default glamor accel method"
			overlayroot-chroot rm -f /etc/X11/xorg.conf.d/20-intel.conf
			overlayroot-chroot apt-get purge xserver-xorg-video-intel -y --allow-downgrades
			;;
		1)set_intel_accel_sna
			;;
		2)set_intel_accel_uxa
#			overlayroot-chroot apt-get install xserver-xorg-video-intel -y --allow-downgrades
			;;
		*)
			overlayroot-chroot rm -f /etc/X11/xorg.conf.d/20-intel.conf
			overlayroot-chroot rm -f /etc/X11/xorg.conf
			echo "You need to set an accel method,default glamor"
			;;	
	esac	
	echo "Sync driver into disk ...... done"
else

	rmmod -f nvidia-drm 
	rmmod -f nvidia-modeset 
	rmmod -f nvidia
	rmmod -f nouveau
	echo "Loading kernel modules......"
	modprobe i915
#	apt-get purge nvidia-* -y --allow-downgrades
#	apt-get purge xserver-xorg-video-nouveau -y --allow-downgrades
	[ -f /etc/X11/xorg.conf ] && rm -f /etc/X11/xorg.conf
	apt-get install xserver-xorg-core --reinstall -y --allow-downgrades
	apt-get install xserver-xorg-input-all --reinstall -y --allow-downgrades
	#根据需求调整显卡的加速方式，从而达到最优化
	case $2 in
		0)
			[ -f /etc/X11/xorg.conf.d/20-intel.conf ] && rm -f /etc/X11/xorg.conf.d/20-intel.conf
			apt-get purge xserver-xorg-video-intel -y --allow-downgrades
			echo "Using default glamor accel method overlay"
			;;
		1)set_intel_accel_sna
#			apt-get install xserver-xorg-video-intel -y --allow-downgrades
			;;
		2)set_intel_accel_uxa
#			apt-get install xserver-xorg-video-intel -y --allow-downgrades
			;;
		*)
			rm /etc/X11/xorg.conf.d/20-intel.conf
			[ -f /etc/X11/xorg.conf ] && rm /etc/X11/xorg.conf
			echo "You need to set an accel method,default glamor"
			;;	
	esac	
	#echo "Now start desktop......"
	#systemctl restart lightdm
fi
