#!/bin/bash

if [ "$(id -u)" -ne "0" ];then
    echo "Need root privileges."
    exit 1
fi

export DEBIAN_FRONTEND=noninteractive
POSTOS=`cat /proc/mounts | awk '{if ($2 == "/media/root-ro") print $1}'`

systemctl stop lightdm

if [ $1 == "post" ];then
    echo "Sync driver into disk $POSTOS ...... "

    find /media/root-rw/overlay/ -size 0 | xargs rm -rf
    mount -o remount,rw $POSTOS /media/root-ro
    rsync -avz --progress /media/root-rw/overlay/* /media/root-ro/
    sync

    overlayroot-chroot update-alternatives --auto glx

    echo "Sync driver into disk ...... done"
else
    apt-get -y --reinstall --allow-downgrades install \
        bumblebee \
        bumblebee-nvidia \
        bbswitch-dkms \
        primus \
        primus-libs \
        libgl1-nvidia-glx \
        libegl-nvidia0 \
        libegl1-nvidia

    echo "remove modules about nvidia from blacklist!"
    overlayroot-chroot rm /etc/modprobe.d/deepin-blacklists-nvidia.conf
    update-initramfs -u
fi
