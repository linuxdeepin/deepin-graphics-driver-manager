#!/bin/bash

if [ "$(id -u)" -ne "0" ];then
    echo "Need root privileges."
    exit 1
fi

export DEBIAN_FRONTEND=noninteractive

apt-get --reinstall -y --allow-downgrades install \
    xserver-xorg-core \
    xserver-xorg-input-all \
    libgl1-mesa-glx
