#!/bin/bash

if [ "$(id -u)" -ne "0" ];then
    echo "Need root privileges."
    exit 1
fi

export DEBIAN_FRONTEND=noninteractive

# 0 ------ default is glamor
# 1 ------ use SNA accel method
# 2 ------ use UXA accel method

remove_config_driver_post() {
overlayroot-chroot rm /etc/X11/xorg.conf.d/20-intel.conf
overlayroot-chroot apt-get purge xserver-xorg-video-intel -y
}

remove_config_driver_overlay() {
rm /etc/X11/xorg.conf.d/20-intel.conf
apt-get purge xserver-xorg-video-intel -y
}

systemctl stop lightdm

if [ $1 == "post" ];then
    case $2 in
        0)
            # nothing to remove in glamor mode
            echo "Using default glamor accel method"
            ;;
        1|2)
            remove_config_driver_post
            ;;
        *)
            echo "You need to set an accel method to remove, do nothing... (post)"
            ;;
    esac

    sync
    sleep 3
    echo "Sync driver into disk ...... done"
else
    case $2 in
        0)
            echo "Nothing to remove..."
            ;;
        1|2)
            remove_config_driver_overlay
            ;;
        *)
            echo "You need to set an accel method to remove, do nothing... (overlay)"
            ;;
    esac
fi
