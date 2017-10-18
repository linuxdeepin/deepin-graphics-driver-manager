#!/bin/bash

overlay=$(grep -m1 "^overlayroot / overlay " /proc/mounts) || overlay=

if [ -n "${overlay}" ]; then
   overlayroot-disable
   print "aaa"
else
   printf "ERROR: Your system not work on overlayroot mode! reboot to reinstall..."
#   sleep 10
#   reboot
   exit -1   
fi	

nouveau_mod=`lsmod | grep nouveau`
nvidia_mod=`lsmod | grep nvidia`

systemctl stop lightdm
if [ -n "$nouveau_mod" ]; then
	 apt-get install nvidia-driver
	 echo "Had already used nouveau,remove it instead by nvidia "
	 rmmod -f nouveau
	 echo "Loading kernel modules......"
	 modprobe nvidia-drm nvidia-current-drm
	 echo "Now start desktop...... Enjoy it !"
	 systemctl restart lightdm
	 exit 0
fi
if [ -n "$nvidia_mod" ]; then
	 echo "Had already used nvidia,updating new nvidia driver "
	 rmmod -f nvidia-drm 
	 rmmod -f nvidia-modeset 
	 rmmod -f nvidia
	 [ -f /etc/X11/xorg.conf ] && rm /etc/X11/xorg.conf
	 echo "Loading kernel modules......"
	 modprobe nouveau
	 echo "Now start desktop...... Enjoy it !"
	 systemctl restart lightdm
	 exit 0
fi

overlatroot-chroot apt-get install nvidia-driver

#overlayroot-chroot sed -i 's:blacklist nouveau":#blacklist nouveau:' /etc/modprobe.d/nvidia-installer-disable-nouveau.conf
