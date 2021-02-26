#!/bin/bash

if [ "$(id -u)" -ne "0" ];then
    echo "Need root privileges."
    exit 1
fi

. /usr/lib/deepin-graphics-driver-manager/common.sh

apt-get -y --reinstall --allow-downgrades install \
    bumblebee \
    bumblebee-nvidia \
    nvidia-driver \
    xserver-xorg-video-nvidia

packages=(
    "bumblebee"
    "bumblebee-nvidia"
    "nvidia-driver"
    "xserver-xorg-video-nvidia"   
)

package_install "${packages[*]}" "${#packages[*]}"