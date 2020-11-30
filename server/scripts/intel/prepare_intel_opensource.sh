#!/bin/bash

. /usr/lib/deepin-graphics-driver-manager/common.sh
packages=(
    "xserver-xorg-video-intel"
    "xserver-xorg-core"
    "xserver-xorg-input-all"
)

package_download "${packages[*]}" "${#packages[*]}"
