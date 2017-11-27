#!/bin/sh

COMMANDS=(
    "apt-get update"
    "apt-get install -d nvidia-driver --reinstall -y --allow-downgrades"
    "apt-get install -o Dir::Cache::Archives=/var/cache/apt/archives -d nvidia-driver -y --allow-downgrades"
    "overlayroot-enable"
#    "apt-get install nvidia-driver -y --allow-downgrades"
#    "apt install -d deepin-deb-installer"
)

for cmd in "${COMMANDS[@]}"
do
    command ${cmd};

    if [ $? != 0 ]; then
        echo "### Failed in command '${cmd}'"
        exit 1;
    fi
done
