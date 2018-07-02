#!/bin/bash

if [ "$(id -u)" -ne "0" ];then
    echo "Need root privileges."
    exit 1
fi

export DEBIAN_FRONTEND=noninteractive
nouveau_mod=`lsmod | grep nouveau`

systemctl stop lightdm

if [ $1 == "post" ];then
    if [ -n "$nouveau_mod" ]; then
        overlayroot-chroot rmmod -f nouveau
    fi

	overlayroot-chroot apt-get purge xserver-xorg-video-nouveau -y
else
    if [ -n "$nouveau_mod" ]; then
        echo "Removing nouveau modules..."
        rmmod -f nouveau
    fi

    apt-get purge xserver-xorg-video-nouveau -y
fi
