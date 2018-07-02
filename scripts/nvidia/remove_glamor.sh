#!/bin/bash

if [ "$(id -u)" -ne "0" ];then
    echo "Need root privileges."
    exit 1
fi

export DEBIAN_FRONTEND=noninteractive

systemctl stop lightdm

if [ $1 == "post" ];then
    echo "Nothing to remove... (post)"
else
    echo "Nothing to remove... (overlay)"
fi
