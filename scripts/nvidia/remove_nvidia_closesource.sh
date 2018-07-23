#!/bin/bash

if [ "$(id -u)" -ne "0" ];then
    echo "Need root privileges."
    exit 1
fi

export DEBIAN_FRONTEND=noninteractive

nvidia_mod=`lsmod | grep nvidia`

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
        glx-alternative-nvidia \
        nvidia-alternative
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
        glx-alternative-nvidia \
        nvidia-alternative

    # TODO: test for remove #
    # repair glx alternative for nouveau
    apt-get -y install --reinstall update-glx glx-diversions nvidia-installer-cleanup glx-alternative-mesa
    apt-get -y install --reinstall libgl1-mesa-glx libgl1-mesa-glx:i386
    apt-get -y install --reinstall libglx-mesa0 libglx-mesa0:i386
    apt-get -y install --reinstall glx-diversions
fi
