#!/bin/bash

# dpkg -i 
# overlayroot-disable

if [ "$(id -u)" -ne "0" ];then
	echo "Need root privileges."
	exit 1
fi

export DEBIAN_FRONTEND=noninteractive
#ret=`lspci |grep -iE 'VGA|3D controller' | grep NVIDIA`
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
	
	overlayroot-chroot apt purge glx-alternative-mesa glx-alternative-nvidia glx-diversions libegl-nvidia0 libegl-nvidia0:i386 libegl1-glvnd-nvidia libegl1-glvnd-nvidia:i386 libgl1-glvnd-nvidia-glx libgl1-glvnd-nvidia-glx:i386 libgl1-nvidia-glvnd-glx libgl1-nvidia-glvnd-glx:i386 libgldispatch0-nvidia libgldispatch0-nvidia:i386 libgles-nvidia1 libgles-nvidia1:i386 libgles-nvidia2 libgles-nvidia2:i386 libgles1-glvnd-nvidia libgles1-glvnd-nvidia:i386 libgles2-glvnd-nvidia libgles2-glvnd-nvidia:i386 libglx-nvidia0 libglx-nvidia0:i386 libglx0-glvnd-nvidia libglx0-glvnd-nvidia:i386 libnvidia-cfg1 libnvidia-cfg1:i386 libnvidia-eglcore libnvidia-eglcore:i386 libnvidia-glcore libnvidia-glcore:i386 libnvidia-ml1 libopengl0-glvnd-nvidia libopengl0-glvnd-nvidia:i386 libvulkan1:i386 libxnvctrl0 nvidia-alternative nvidia-driver nvidia-driver-bin nvidia-driver-libs nvidia-driver-libs:i386 nvidia-driver-libs-i386:i386 nvidia-egl-common nvidia-egl-icd nvidia-egl-icd:i386 nvidia-installer-cleanup nvidia-kernel-common nvidia-kernel-dkms nvidia-kernel-support nvidia-legacy-check nvidia-modprobe nvidia-persistenced nvidia-settings nvidia-support nvidia-vdpau-driver nvidia-vulkan-common nvidia-vulkan-icd nvidia-vulkan-icd:i386 update-glx xserver-xorg-video-nvidia -y 
	overlayroot-chroot apt purge xserver-xorg-video-nouveau -y --allow-downgrades
#	overlayroot-chroot apt-get autoremove -y 
	overlayroot-chroot apt install xserver-xorg-core --reinstall -y --allow-downgrades
	overlayroot-chroot apt install libgl1-mesa-glx --reinstall -y --allow-downgrades
	overlayroot-chroot apt install xserver-xorg-input-all --reinstall -y --allow-downgrades
	overlayroot-chroot rm -rf /etc/X11/xorg.conf
	overlayroot-chroot rm -rf /etc/modprobe.d/bumblebee.conf
	overlayroot-chroot rm -rf /etc/bumblebee/bumblebee.conf
	overlayroot-chroot rm -rf /etc/X11/xorg.conf.d/20-nouveau.conf
	overlayroot-chroot rm -rf /etc/X11/xorg.conf.d/20-nvidia.conf
	sync
	sleep 3
	echo "Sync driver into disk ...... done"
else
	if [ -n "$nvidia_mod" ]; then
		echo "Had already used nvidia,updating new nvidia driver "
		rmmod -f nvidia-drm 
		rmmod -f nvidia-modeset 
		rmmod -f nvidia
	fi
	modprobe nouveau
	apt purge glx-alternative-mesa glx-alternative-nvidia glx-diversions libegl-nvidia0 libegl-nvidia0:i386 libegl1-glvnd-nvidia libegl1-glvnd-nvidia:i386 libgl1-glvnd-nvidia-glx libgl1-glvnd-nvidia-glx:i386 libgl1-nvidia-glvnd-glx libgl1-nvidia-glvnd-glx:i386 libgldispatch0-nvidia libgldispatch0-nvidia:i386 libgles-nvidia1 libgles-nvidia1:i386 libgles-nvidia2 libgles-nvidia2:i386 libgles1-glvnd-nvidia libgles1-glvnd-nvidia:i386 libgles2-glvnd-nvidia libgles2-glvnd-nvidia:i386 libglx-nvidia0 libglx-nvidia0:i386 libglx0-glvnd-nvidia libglx0-glvnd-nvidia:i386 libnvidia-cfg1 libnvidia-cfg1:i386 libnvidia-eglcore libnvidia-eglcore:i386 libnvidia-glcore libnvidia-glcore:i386 libnvidia-ml1 libopengl0-glvnd-nvidia libopengl0-glvnd-nvidia:i386 libvulkan1:i386 libxnvctrl0 nvidia-alternative nvidia-driver nvidia-driver-bin nvidia-driver-libs nvidia-driver-libs:i386 nvidia-driver-libs-i386:i386 nvidia-egl-common nvidia-egl-icd nvidia-egl-icd:i386 nvidia-installer-cleanup nvidia-kernel-common nvidia-kernel-dkms nvidia-kernel-support nvidia-legacy-check nvidia-modprobe nvidia-persistenced nvidia-settings nvidia-support nvidia-vdpau-driver nvidia-vulkan-common nvidia-vulkan-icd nvidia-vulkan-icd:i386 update-glx xserver-xorg-video-nvidia -y 
	apt purge nvidia-* -y --allow-downgrades
	apt purge xserver-xorg-video-nouveau -y --allow-downgrades
	echo "Loading kernel modules......"
	[ -f /etc/X11/xorg.conf ] && rm -rf /etc/X11/xorg.conf
	[ -f /etc/modprobe.d/bumblebee.conf ] && rm -rf /etc/modprobe.d/bumblebee.conf
	[ -f /etc/X11/xorg.conf.d/20-nouveau.conf ] && rm -rf /etc/X11/xorg.conf.d/20-nouveau.conf
	[ -f /etc/X11/xorg.conf.d/20-nvidia.conf ] && rm -rf /etc/X11/xorg.conf.d/20-nvidia.conf

	apt install libgl1-mesa-glx --reinstall -y --allow-downgrades
	apt install xserver-xorg-core --reinstall -y --allow-downgrades
	apt install xserver-xorg-input-all --reinstall -y --allow-downgrades
	#echo "Now start desktop......"
	#systemctl restart lightdm
fi
