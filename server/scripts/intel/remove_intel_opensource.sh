#!/bin/bash

if [ "$(id -u)" -ne "0" ];then
    echo "Need root privileges."
    exit 1
fi

export DEBIAN_FRONTEND=noninteractive

# 0 ------ default is glamor
# 1 ------ use SNA accel method
# 2 ------ use UXA accel method

remove_config_driver_overlay() {
rm /etc/X11/xorg.conf.d/20-intel.conf
apt-get purge xserver-xorg-video-intel -y
}

case $1 in
    0)
        echo "Nothing to remove..."
        ;;
    1|2)
        remove_config_driver_overlay
        ;;
    *)
        echo "You need to set an accel method to remove, do nothing..."
        ;;
esac
