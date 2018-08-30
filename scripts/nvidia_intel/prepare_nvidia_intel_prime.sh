#!/bin/bash

# The first download is to download missing packages based on dependencies.
# The second time is to download packages that may be missing during the overlay phase.

COMMANDS=(
    "apt-get update"
    "apt-get install -d --reinstall -y --allow-downgrades \
        nvidia-driver \
        nvidia-driver-libs"
    "cd /var/cache/apt/archives"
    "apt-get download \
        deepin-nvidia-prime \
        glx-alternative-mesa \
        glx-alternative-nvidia \
        glx-diversions \
        libegl-nvidia0 \
        libegl-nvidia0:i386 \
        libegl1 \
        libegl1:i386 \
        libgl1-nvidia-glvnd-glx \
        libgl1-nvidia-glvnd-glx:i386 \
        libegl1-nvidia \
        libegl1-nvidia:i386 \
        libgl1-nvidia-glx \
        libgl1-nvidia-glx:i386 \
        libglx-nvidia0 \
        libglx-nvidia0:i386 \
        libgles-nvidia1 \
        libgles-nvidia1:i386 \
        libgles-nvidia2 \
        libgles-nvidia2:i386 \
        libgles2 \
        libgles2:i386 \
        libnvidia-cfg1 \
        libnvidia-cfg1:i386 \
        libnvidia-eglcore \
        libnvidia-eglcore:i386 \
        libnvidia-egl-wayland1 \
        libnvidia-egl-wayland1:i386 \
        libnvidia-glcore \
        libnvidia-glcore:i386 \
        libnvidia-ml1 \
        libopengl0 \
        libopengl0:i386 \
        libxnvctrl0 \
        libvulkan1 \
        libvulkan1:i386 \
        libwayland-client0 \
        libwayland-client0:i386 \
        libwayland-server0 \
        libwayland-server0:i386 \
        nvidia-driver \
        nvidia-driver-bin \
        nvidia-driver-libs \
        nvidia-driver-libs:i386 \
        nvidia-driver-libs-i386:i386 \
        nvidia-alternative \
        nvidia-egl-common \
        nvidia-egl-icd \
        nvidia-egl-icd:i386 \
        nvidia-egl-wayland-common \
        nvidia-egl-wayland-icd \
        nvidia-egl-wayland-icd:i386 \
        nvidia-nonglvnd-vulkan-common \
        nvidia-nonglvnd-vulkan-icd \
        nvidia-nonglvnd-vulkan-icd:i386 \
        nvidia-persistenced \
        nvidia-installer-cleanup \
        nvidia-kernel-common \
        nvidia-kernel-dkms \
        nvidia-kernel-support \
        nvidia-legacy-check \
        nvidia-modprobe \
        nvidia-settings \
        nvidia-support \
        nvidia-vdpau-driver \
        nvidia-vulkan-common \
        nvidia-vulkan-icd \
        nvidia-vulkan-icd:i386 \
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
update-initramfs -u -t
