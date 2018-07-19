#!/bin/bash

COMMANDS=(
    "apt-get update"
    "apt-get install -d --reinstall -y --allow-downgrades \
        xserver-xorg-core \
        xserver-xorg-input-all \
        nvidia-driver \
        libgl1-mesa-glx \
        glx-alternative-nvidia \
        xserver-xorg-video-nvidia"
)

for cmd in "${COMMANDS[@]}"
do
    command ${cmd};

    if [ $? != 0 ]; then
        echo "### Failed in command '${cmd}'"
        exit 1;
    fi
done
