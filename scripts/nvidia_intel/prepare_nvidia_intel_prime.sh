#!/bin/bash

COMMANDS=(
    "apt-get update"
    "cd /var/cache/apt/archives"
    "apt-get download \
        deepin-nvidia-prime \
        glx-alternative-mesa \
        glx-alternative-nvidia \
        glx-diversions \
        libegl-nvidia0 \
        libegl1-nvidia \
        libgl1-nvidia-glx \
        libglx-nvidia0 \
        libglx-nvidia0:i386 \
        libnvidia-eglcore \
        libnvidia-egl-wayland1 \
        libnvidia-glcore \
        libnvidia-glcore:i386 \
        libnvidia-ml1 \
        libxnvctrl0 \
        nvidia-alternative \
        nvidia-egl-common \
        nvidia-egl-icd \
        nvidia-installer-cleanup \
        nvidia-kernel-common \
        nvidia-kernel-dkms \
        nvidia-kernel-support \
        nvidia-legacy-check \
        nvidia-modprobe \
        nvidia-settings \
        nvidia-support \
        nvidia-vdpau-driver \
        update-glx \
        xserver-xorg-video-nvidia"
)

for cmd in "${COMMANDS[@]}"
do
    command ${cmd};

    if [ $? != 0 ]; then
        echo "### Failed in command '${cmd}'"
        exit 1;
    fi
done

# to avoid rmmod or modprobe failed so blacklists modules about nvidia before reboot to overlay
echo "blacklists modules about nvidia now!"
echo -e "blacklist nouveau\nblacklist nvidia-drm\nblacklist nvidia-modeset\nblacklist nvidia\nblacklist nvidiafb" > /etc/modprobe.d/deepin-blacklists-nvidia.conf
update-initramfs -u
