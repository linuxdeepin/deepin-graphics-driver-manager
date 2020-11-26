#!/bin/bash

# The first download is to download missing packages based on dependencies.
# The second time is to download packages that may be missing during the overlay phase.

COMMANDS=(
    "apt-get update"
    "apt-get install  --fix-missing"
    "apt-get install -d --reinstall -y --allow-downgrades \
        nvidia-driver \
        xserver-xorg-video-nvidia \
        bbswitch-dkms \
        bumblebee-nvidia \
	    xserver-xorg-input-all \
	    xserver-xorg-core"
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
