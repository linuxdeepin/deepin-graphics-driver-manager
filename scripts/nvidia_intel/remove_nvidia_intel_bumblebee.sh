#!/bin/bash

if [ "$(id -u)" -ne "0" ];then
    echo "Need root privileges."
    exit 1
fi

export DEBIAN_FRONTEND=noninteractive

nvidia_mod=`lsmod | grep nvidia`
# check the install script is for install PRIME resolution
to_prime=`grep "deepin-nvidia-prime" /tmp/exe.sh`

systemctl stop lightdm

if [ $1 == "post" ];then
    if [ -x /usr/bin/nvidia-installer ];then
        overlayroot-chroot nvidia-installer --uninstall --no-runlevel-check --no-x-check --ui=none || true
    fi

    if [ -n "$nvidia_mod" ]; then
        overlayroot-chroot rmmod -f nvidia-drm
        overlayroot-chroot rmmod -f nvidia-modeset
        overlayroot-chroot rmmod -f nvidia
    fi

    overlayroot-chroot apt-get -y purge \
        bumblebee \
        bumblebee-nvidia \
        primus \
        primus-libs \
        bbswitch-dkms \
        glx-alternative-nvidia \
        nvidia-alternative \
        nvidia-driver \
        nvidia-driver-libs-nonglvnd \
        xserver-xorg-video-nvidia

    overlayroot-chroot rm -rf /etc/modprobe.d/bumblebee.conf
else
    if [ -x /usr/bin/nvidia-installer ];then
        nvidia-installer --uninstall --no-runlevel-check --no-x-check --ui=none || true
    fi

    if [ -n "$nvidia_mod" ]; then
        echo "Removing nvidia modules..."
        rmmod -f nvidia-drm
        rmmod -f nvidia-modeset
        rmmod -f nvidia
    fi

    apt-get -y purge \
        bumblebee \
        bumblebee-nvidia \
        primus \
        primus-libs \
        bbswitch-dkms \
        glx-alternative-nvidia \
        nvidia-alternative \
        nvidia-driver \
        nvidia-driver-libs-nonglvnd \
        xserver-xorg-video-nvidia

    rm -rf /etc/modprobe.d/bumblebee.conf
fi
