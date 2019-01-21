#!/bin/bash

COMMANDS=(
	"apt-get update"
    "apt-get install -d --reinstall -y --allow-downgrades \
        xserver-xorg-video-intel \
        xserver-xorg-core \
        xserver-xorg-input-all"
)    

for cmd in "${COMMANDS[@]}"
do
    command ${cmd};

    if [ $? != 0 ]; then
        echo "### Failed in command '${cmd}'"
        exit 1;
    fi
done
