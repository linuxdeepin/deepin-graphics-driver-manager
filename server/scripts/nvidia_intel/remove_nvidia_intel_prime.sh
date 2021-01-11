#!/bin/bash

/usr/lib/deepin-graphics-driver-manager/nvidia/remove_nvidia_closesource.sh

if [ -f "/etc/X11/xorg.conf.d/70-nvidia.conf" ]; then
    rm -f /etc/X11/xorg.conf.d/70-nvidia.conf
fi