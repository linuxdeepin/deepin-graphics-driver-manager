#!/bin/bash
. /usr/lib/deepin-graphics-driver-manager/common.sh
packages=(
    "nvidia-driver"
    "xserver-xorg-video-nvidia"
    "libegl-mesa0:i386"
    "libgbm1:i386"
    "glx-alternative-mesa"
    "update-glx"
    "glx-diversions"
    "nvidia-installer-cleanup"
    "libgl1-mesa-glx"
    "libgl1-mesa-glx:i386"
    "libglx-mesa0"
    "libglx-mesa0:i386"
	"xserver-xorg-core"
	"xserver-xorg-input-all"
)

package_download "${packages[*]}" "${#packages[*]}"
