#!/bin/bash

if [ "$(id -u)" -ne "0" ];then
    echo "Need root privileges."
    exit 1
fi

export DEBIAN_FRONTEND=noninteractive

apt-get -y --reinstall --allow-downgrades install \
    bumblebee \
    bumblebee-nvidia \
    primus \
    nvidia-driver \
    xserver-xorg-video-nvidia \
    nvidia-driver-libs-nonglvnd
