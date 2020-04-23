#!/bin/bash

if [ "$(id -u)" -ne "0" ];then
    echo "Need root privileges."
    exit 1
fi

export DEBIAN_FRONTEND=noninteractive

apt-get -y --reinstall --allow-downgrades install \
    bumblebee \
    bumblebee-nvidia \
<<<<<<< HEAD
    nvidia-driver \
    xserver-xorg-video-nvidia
=======
    nvidia-driver
>>>>>>> a4418e9f86500fdb9027610c095f6d284d7162d8
