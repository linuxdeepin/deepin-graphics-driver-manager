#!/bin/bash

if [ "$(id -u)" -ne "0" ];then
    echo "Need root privileges."
    exit 1
fi

export DEBIAN_FRONTEND=noninteractive
POSTOS=`cat /proc/mounts | awk '{if ($2 == "/media/root-ro") print $1}'`

# 0 ------ default is glamor
# 1 ------ use SNA accel method
# 2 ------ use UXA accel method

set_intel_accel_sna_post() {
INTEL_XORG_CONF="/etc/X11/xorg.conf.d/20-intel.conf"
echo 'Section "Device"
Identifier "Intel Graphics"
Driver "intel"
Option "AccelMethod" "sna"
#Option "PageFlip" "False"
#Option "TearFree" "True"
EndSection' | overlayroot-chroot tee $INTEL_XORG_CONF
overlayroot-chroot apt-get install xserver-xorg-video-intel --reinstall -y --allow-downgrades
}

set_intel_accel_sna_overlay() {
INTEL_XORG_CONF="/etc/X11/xorg.conf.d/20-intel.conf"
echo 'Section "Device"
Identifier "Intel Graphics"
Driver "intel"
Option "AccelMethod" "sna"
#Option "PageFlip" "False"
#Option "TearFree" "True"
EndSection' | tee $INTEL_XORG_CONF
apt-get install xserver-xorg-video-intel --reinstall -y --allow-downgrades
}

set_intel_accel_uxa_post() {
INTEL_XORG_CONF="/etc/X11/xorg.conf.d/20-intel.conf"
echo 'Section "Device"
Identifier "Intel Graphics"
Driver "intel"
Option "AccelMethod" "uxa"
#Option "PageFlip" "False"
#Option "TearFree" "True"
EndSection' | overlayroot-chroot tee $INTEL_XORG_CONF
overlayroot-chroot apt-get install xserver-xorg-video-intel --reinstall -y --allow-downgrades
}

set_intel_accel_uxa_overlay() {
INTEL_XORG_CONF="/etc/X11/xorg.conf.d/20-intel.conf"
echo 'Section "Device"
Identifier "Intel Graphics"
Driver "intel"
Option "AccelMethod" "uxa"
#Option "PageFlip" "False"
#Option "TearFree" "True"
EndSection' | tee $INTEL_XORG_CONF
apt-get install xserver-xorg-video-intel --reinstall -y --allow-downgrades
}

systemctl stop lightdm

if [ $1 == "post" ];then
    echo "Sync driver into disk $POSTOS ...... "
    overlayroot-chroot apt-get --reinstall -y --allow-downgrades install \
        xserver-xorg-core \
        xserver-xorg-input-all

    #根据需求调整显卡的加速方式，从而达到最优化
    case $2 in
        0)
            # it is glamor mode after config files and drivers of other modes has been removed
            echo "Using default glamor accel method"
            ;;
        1)
            set_intel_accel_uxa_post
            ;;
        2)
            set_intel_accel_sna_post
            ;;
        *)
            overlayroot-chroot rm -f /etc/X11/xorg.conf.d/20-intel.conf
            overlayroot-chroot rm -f /etc/X11/xorg.conf
            echo "You need to set an accel method,default glamor (post)"
            ;;
    esac

    sync
    echo "Sync driver into disk ...... done"
else
    apt-get --reinstall -y --allow-downgrades install \
        xserver-xorg-core \
        xserver-xorg-input-all

    #根据需求调整显卡的加速方式，从而达到最优化
    case $2 in
        0)
            # it is glamor mode after config files and drivers of other modes has been removed
            echo "Using default glamor accel method overlay"
            ;;
        1)
            set_intel_accel_uxa_overlay
            ;;
        2)
            set_intel_accel_sna_overlay
            ;;
        *)
            rm /etc/X11/xorg.conf.d/20-intel.conf
            [ -f /etc/X11/xorg.conf ] && rm /etc/X11/xorg.conf
            echo "You need to set an accel method,default glamor (overlay)"
            ;;
    esac
fi
