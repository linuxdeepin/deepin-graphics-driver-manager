#!/bin/bash

if [ "$(id -u)" -ne "0" ];then
    echo "Need root privileges."
    exit 1
fi

. /usr/lib/deepin-graphics-driver-manager/common.sh

packages=(
    "nvidia-driver"
    "xserver-xorg-core"
    "xserver-xorg-input-all"
)

package_install "${packages[*]}" "${#packages[*]}"

purge_intelgpu

exit 0
