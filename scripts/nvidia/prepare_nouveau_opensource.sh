#!/bin/bash

COMMANDS=(
    "apt-get update"
    "apt-get install -d --reinstall -y --allow-downgrades \
        xserver-xorg-core \
        xserver-xorg-input-all \
        libgl1-mesa-glx \
        libegl-mesa0 \
        libegl-mesa0:i386 \
        libgbm1 \
        libgbm1:i386 \
        xserver-xorg-video-nouveau \
        glx-alternative-mesa \
        update-glx \
        glx-diversions \
        nvidia-installer-cleanup \
        libgl1-mesa-glx:i386 \
        libglx-mesa0 \
        libglx-mesa0:i386"
)

for cmd in "${COMMANDS[@]}"
do
    command ${cmd};

    if [ $? != 0 ]; then
        echo "### Failed in command '${cmd}'"
        exit 1;
    fi
done
