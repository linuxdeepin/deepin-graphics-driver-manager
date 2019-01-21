#!/bin/bash

if [ "$(id -u)" -ne "0" ];then
    echo "Need root privileges."
    exit 1
fi

export DEBIAN_FRONTEND=noninteractive

nvidia_mod=`lsmod | grep nvidia`

if [ -x /usr/bin/nvidia-installer ];then
    nvidia-installer --uninstall --no-runlevel-check --no-x-check --ui=none || true
fi

if [ -n "$nvidia_mod" ]; then
    echo "Removing nvidia modules..."
    rmmod -f nvidia-drm
    rmmod -f nvidia-modeset
    rmmod -f nvidia
fi

apt-get -y purge \
    glx-alternative-nvidia \
    nvidia-alternative

if [[ $? -ne 0 ]]; then
    echo "apt-get execute failed!"
    exit 1
fi

# TODO: remove after package problem fixed #
echo "Manual update initramfs ..."
update-initramfs -u -t
