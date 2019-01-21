#!/bin/bash

export WORKING_DIR_G=/usr/lib/deepin-graphics-driver-manager/working-dir
export TEST_IN_OVERLAY_G=$WORKING_DIR_G/test_in_overlay_flag
export REMOVE_OLD_G=$WORKING_DIR_G/remove_old.sh
export INSTALL_NEW_G=$WORKING_DIR_G/install_new.sh
export CONFIG_FILE_G=$WORKING_DIR_G/config.conf

cleanWorking() {
    /usr/sbin/overlayroot-chroot rm -rf $TEST_IN_OVERLAY_G
    /usr/sbin/overlayroot-chroot rm -rf $REMOVE_OLD_G
    /usr/sbin/overlayroot-chroot rm -rf $INSTALL_NEW_G
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
