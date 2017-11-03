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
POSTOS=`cat /proc/mounts | awk '{if ($2 == "/media/root-ro") print $1}'`

systemctl stop lightdm
if [ $1 == "post" ];then
	if [ -x /usr/bin/nvidia-installer ];then
		overlayroot-chroot nvidia-installer --uninstall --no-runlevel-check --no-x-check --ui=none || true
	fi
	overlayroot-chroot rmmod -f nvidia-drm 
	overlayroot-chroot rmmod -f nvidia-modeset 
	overlayroot-chroot rmmod -f nvidia
	
	overlayroot-chroot apt purge nvidia-* -y --allow-downgrades
	overlayroot-chroot apt purge xserver-xorg-video-nouveau -y --allow-downgrades
	overlayroot-chroot apt install xserver-xorg-core --reinstall -y --allow-downgrades
	overlayroot-chroot apt install xserver-xorg-input-all --reinstall -y --allow-downgrades
	overlayroot-chroot rm -rf /etc/X11/xorg.conf
	overlayroot-chroot rm -rf /etc/modprobe.d/bumblebee.conf
	overlayroot-chroot rm -rf /etc/X11/xorg.conf.d/20-nouveau.conf
	overlayroot-chroot rm -rf /etc/X11/xorg.conf.d/20-nvidia.conf
	echo "Sync driver into disk ...... done"
else
	if [ -n "$nvidia_mod" ]; then
		echo "Had already used nvidia,updating new nvidia driver "
		rmmod -f nvidia-drm 
		rmmod -f nvidia-modeset 
		rmmod -f nvidia
	fi
	echo "Loading kernel modules......"
	apt-get purge nvidia-* -y --allow-downgrades
	modprobe nouveau
	apt-get purge xserver-xorg-video-nouveau -y --allow-downgrades
	[ -f /etc/X11/xorg.conf ] && rm -rf /etc/X11/xorg.conf
	[ -f /etc/modprobe.d/bumblebee.conf ] && rm -rf /etc/modprobe.d/bumblebee.conf
	[ -f /etc/X11/xorg.conf.d/20-nouveau.conf ] && rm -rf /etc/X11/xorg.conf.d/20-nouveau.conf

	apt install libgl1-mesa-glx --reinstall -y --allow-downgrades
	apt install xserver-xorg-core --reinstall -y --allow-downgrades
	apt install xserver-xorg-input-all --reinstall -y --allow-downgrades
	#echo "Now start desktop......"
	#systemctl restart lightdm
fi
