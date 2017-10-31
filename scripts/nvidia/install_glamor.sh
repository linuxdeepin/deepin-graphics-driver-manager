#!/bin/bash

# dpkg -i 
# overlayroot-disable

if [ "$(id -u)" -ne "0" ];then
	echo "Need root privileges."
	exit 1
fi

export DEBIAN_FRONTEND=noninteractive
ret=`lspci |grep -iE 'VGA|3D controller' | grep NVIDIA`
nouveau_mod=`lsmod | grep nouveau`
nvidia_mod=`lsmod | grep nvidia`
if [ -n "$ret" ]; then
	echo "Found nvidia card: $ret"
else
	echo "No NV card found."
	exit
fi	

systemctl stop lightdm
if [ $1 == "post" ];then
	if [ -x /usr/bin/nvidia-installer ];then
		overlayroot-chroot nvidia-installer --uninstall --no-runlevel-check --no-x-check --ui=none || true
	fi
	
	[ -f /etc/X11/xorg.conf ] && overlayroot-chroot rm -rf /etc/X11/xorg.conf
	[ -f /etc/X11/xorg.conf.d/20-nouveau.conf ] && overlayroot-chroot rm -rf /etc/X11/xorg.conf.d/20-nouveau.conf

	overlayroot-chroot apt-get purge nvidia-* -y --allow-downgrades
	overlayroot-chroot apt-get purge xserver-xorg-xorg-video-nouveau -y --allow-downgrades
	overlayroot-chroot apt-get install xserver-xorg-xorg-core --reinstall -y --allow-downgrades
	overlayroot-chroot apt-get install xserver-xorg-input-all --reinstall -y --allow-downgrades
	echo "Sync driver into disk ...... done"
else
	apt-get purge nvidia-* -y --allow-downgrades
	apt-get purge xserver-xorg-xorg-video-nouveau -y --allow-downgrades
	[ -f /etc/X11/xorg.conf ] && overlayroot-chroot rm -rf /etc/X11/xorg.conf
	[ -f /etc/X11/xorg.conf.d/20-nouveau.conf ] && overlayroot-chroot rm -rf /etc/X11/xorg.conf.d/20-nouveau.conf

	if [ -n "$nvidia_mod" ]; then
		echo "Had already used nvidia,updating new nvidia driver "
		rmmod -f nvidia-drm 
		rmmod -f nvidia-modeset 
		rmmod -f nvidia
	fi
	echo "Loading kernel modules......"
	modprobe nouveau
	apt-get install xserver-xorg-xorg-core --reinstall -y --allow-downgrades
	apt-get install xserver-xorg-input-all --reinstall -y --allow-downgrades
	#echo "Now start desktop......"
	#systemctl restart lightdm
fi
