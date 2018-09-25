#!/bin/sh

cleanWorking() {
    /usr/sbin/overlayroot-chroot rm -rf /usr/bin/deepin-graphics-driver-remove-old.sh
    /usr/sbin/overlayroot-chroot rm -rf /usr/bin/deepin-graphics-driver-installer.sh
    /usr/sbin/overlayroot-chroot rm -rf /etc/modprobe.d/deepin-blacklists-nvidia.conf
    /usr/sbin/overlayroot-disable
    return
}

error_exit_dgm() {
    echo "$1"
    cleanWorking
    exit "$2";
}

error_reboot() {
    echo "$1"
    cleanWorking
    sync
    reboot
}

