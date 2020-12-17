#!/bin/bash

# The first download is to download missing packages based on dependencies.
# The second time is to download packages that may be missing during the overlay phase.
. /usr/lib/deepin-graphics-driver-manager/common.sh
packages=(
    "nvidia-driver"
    "xserver-xorg-video-nvidia"
    "bbswitch-dkms"
    "bumblebee-nvidia"
    "xserver-xorg-input-all"
    "xserver-xorg-core"
    "libnvidia-legacy-390xx-cfg1"
    "nvidia-legacy-390xx-alternative"
)

package_download "${packages[*]}" "${#packages[*]}"

