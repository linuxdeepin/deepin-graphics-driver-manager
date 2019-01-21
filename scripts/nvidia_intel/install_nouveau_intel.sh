#!/bin/bash

if [ "$(id -u)" -ne "0" ];then
    echo "Need root privileges."
    exit 1
fi

export DEBIAN_FRONTEND=noninteractive

apt-get --reinstall -y --allow-downgrades install \
    libgl1-mesa-glx \
    xserver-xorg-input-all \
    xserver-xorg-core \
    xserver-xorg-video-nouveau

if [[ $? -ne 0 ]]; then
    echo "apt-get execute failed!"
    exit 1
fi

echo "Loading kernel modules......"
modprobe nouveau

exit 0
