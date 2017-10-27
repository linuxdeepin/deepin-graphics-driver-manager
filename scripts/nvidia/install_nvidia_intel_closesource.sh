#!/bin/bash

# dpkg -i 
# overlayroot-disable

if [ "$(id -u)" -ne "0" ];then
	echo "Need root privileges."
	exit 1
fi

export DEBIAN_FRONTEND=noninteractive
ret=`lspci | grep VGA | grep NVIDIA`
nouveau_mod=`lsmod | grep nouveau`
nvidia_mod=`lsmod | grep nvidia`
if [ -n "$ret" ]; then
	echo "Found nvidia card: $ret"
else
	echo "No NV card found."
	exit
fi	

#systemctl stop lightdm
if [$1 == "post" ];then
	if [ -x /usr/bin/nvidia-installer ];then
		overlayroot-chroot nvidia-installer --uninstall --no-runlevel-check --no-x-check --ui=none || true
	fi
	overlayroot-chroot apt-get install nvidia-driver -y --allow-downgrades
	echo "Sync driver into disk ...... done"
else
	apt-get install nvidia-driver -y --allow-downgrades 

	systemctl stop lightdm
	if [ -n "$nouveau_mod" ]; then
		echo "Had already used nouveau,remove it instead by nvidia "
		rmmod -f nouveau
	fi
	if [ -n "$nvidia_mod" ]; then
		echo "Had already used nvidia,updating new nvidia driver "
		rmmod -f nvidia-drm 
		rmmod -f nvidia-modeset 
		rmmod -f nvidia
	fi
	echo "Loading kernel modules......"
	modprobe nvidia-drm
	modprobe nvidia-current-drm
	#echo "Now start desktop......"
	#systemctl restart lightdm
fi

#sudo overlayroot-chroot apt-get install nvidia-driver -y --allow-downgrades
