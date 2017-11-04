#!/bin/sh

COMMANDS=(
    "apt update"
    "apt install -d xserver-xorg-core  xserver-xorg-input-all -y --allow-downgrades"
    "apt install -d nvidia-driver -y --allow-downgrades"
    "apt install -d bumblebee-nvidia -y --allow-downgrades"
    "apt install -d libgl1-mesa-glx -y --allow-downgrades"
#    "apt install -o Dir::Cache::Archives=/var/cache/nvidia/ -d nvidia-driver"
#    "overlayroot-enable"
#    "apt-get install nvidia-driver -y --allow-downgrades"
#    "apt install -d deepin-deb-installer"
)

for cmd in "${COMMANDS[@]}"
do
    command ${cmd};

    if [ $? != 0 ]; then
        echo "### Failed in command '${cmd}'"
        exit -1;
    fi
done
