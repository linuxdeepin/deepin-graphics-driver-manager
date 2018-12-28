#!/bin/bash

if [ "$(id -u)" -ne "0" ];then
    echo "Need root privileges."
    exit 1
fi

export DEBIAN_FRONTEND=noninteractive
nouveau_mod=`lsmod | grep nouveau`
POSTOS=`cat /proc/mounts | awk '{if ($2 == "/media/root-ro") print $1}'`

systemctl stop lightdm

if [ $1 == "post" ];then
    echo "Sync driver into disk $POSTOS ...... "

    find /media/root-rw/overlay/ -size 0 | xargs rm -rf
    mount -o remount,rw $POSTOS /media/root-ro
    rsync -avzl --progress /media/root-rw/overlay/* /media/root-ro/
    sync

    echo "Sync driver into disk ...... done"
else
    apt-get -y --reinstall --allow-downgrades install \
        deepin-nvidia-prime \
        nvidia-driver \
        nvidia-driver-libs

    if [[ $? -ne 0 ]]; then
        echo "apt-get execute failed!"
        exit 1
    fi

    if [ -n "$nouveau_mod" ]; then
        echo "Removing nouveau modules..."
        rmmod -f nouveau
    fi

    echo "Loading kernel modules......"
    modprobe nvidia-drm
    modprobe nvidia-modeset
    modprobe nvidia

    echo -e '#!/bin/sh\n. /sbin/prime-offload\n/usr/lib/deepin-graphics-driver-manager/gltest\n' > /tmp/deepin-prime-gltest
    chmod a+x /tmp/deepin-prime-gltest
fi
