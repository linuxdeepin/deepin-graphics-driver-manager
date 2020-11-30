#!/bin/bash

# The first download is to download missing packages based on dependencies.
# The second time is to download packages that may be missing during the overlay phase.
. /usr/lib/deepin-graphics-driver-manager/common.sh

packages=(
    "nvidia-driver"
    "nvidia-driver-libs"
    "deepin-nvidia-prime"
)

package_download "${packages[*]}" "${#packages[*]}"

# to avoid rmmod or modprobe failed so blacklists modules about nvidia before reboot to overlay
echo "blacklists modules about nvidia now!"
echo -e "blacklist nouveau\nblacklist nvidia-drm\nblacklist nvidia-modeset\nblacklist nvidia\nblacklist nvidiafb" > /etc/modprobe.d/deepin-blacklists-nvidia.conf
update-initramfs -u -t
