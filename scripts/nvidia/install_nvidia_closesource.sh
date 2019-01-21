#!/bin/bash

if [ "$(id -u)" -ne "0" ];then
    echo "Need root privileges."
    exit 1
fi

export DEBIAN_FRONTEND=noninteractive

apt-get --reinstall -y --allow-downgrades install \
    nvidia-driver \
    xserver-xorg-core \
    xserver-xorg-input-all

if [[ $? -ne 0 ]]; then
    echo "apt-get execute failed!"
    exit 1
fi

echo "Loading kernel modules......"
modprobe nvidia-drm
modprobe nvidia-modeset
modprobe nvidia

exit 0
