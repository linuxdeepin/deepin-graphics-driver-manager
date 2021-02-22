#!/bin/bash

export WORKING_DIR_G=/usr/lib/deepin-graphics-driver-manager/working-dir
export TEST_IN_OVERLAY_G=$WORKING_DIR_G/test_in_overlay_flag
export REMOVE_OLD_G=$WORKING_DIR_G/remove_old.sh
export INSTALL_NEW_G=$WORKING_DIR_G/install_new.sh
export CONFIG_FILE_G=$WORKING_DIR_G/config.conf
export REAL_INSTALLE_DESKTOP=/etc/xdg/autostart/deepin-gradvrmgr-installer.desktop
export isInOverlayRoot=$(grep -m1 "^overlayroot / overlay " /proc/mounts) || isInOverlayRoot=
export DEBIAN_FRONTEND=noninteractive
OVERLAYROOT_IMAGE=$WORKING_DIR_G/overlayroot.img
OVERLAYROOT_CONF=/etc/overlayroot.conf
LOOP_DEV=/dev/loop0

overlayroot_disable() {
    overlayroot-chroot sed -i 's:overlayroot=".*":overlayroot="":' ${OVERLAYROOT_CONF}
    [ -e "${OVERLAYROOT_IMAGE}" ] || overlayroot-chroot rm -f ${OVERLAYROOT_IMAGE}
}

overlayroot_enable() {
    if [[ -n "${isInOverlayRoot}" ]]; then
        echo "overlayroot is enabled already"
    else
        [ -e "${OVERLAYROOT_IMAGE}" ] || rm -f ${OVERLAYROOT_IMAGE}
        dd if=/dev/zero of=${OVERLAYROOT_IMAGE} bs=1MiB count=640 && \
            sed -i "s:overlayroot=".*":overlayroot=\"device\:dev=\/dev\/loop0,recurse=0\":" ${OVERLAYROOT_CONF}
        mkfs.ext4 ${OVERLAYROOT_IMAGE}
    fi
}

overlayroot_save() {
    overlayroot-chroot cp -f /run/overlayroot.img ${OVERLAYROOT_IMAGE}
}
 
cleanWorking() {
    if [[ -n "${isInOverlayRoot}" ]]; then
        /usr/sbin/overlayroot-chroot rm -rf $TEST_IN_OVERLAY_G
        /usr/sbin/overlayroot-chroot rm -rf $REMOVE_OLD_G
        /usr/sbin/overlayroot-chroot rm -rf $INSTALL_NEW_G
        overlayroot_disable

    else
        rm -rf $TEST_IN_OVERLAY_G
        rm -rf $REMOVE_OLD_G
        rm -rf $INSTALL_NEW_G
    fi
}

 nvidia_blacklist_recovery(){
     nvidia_blacklist_file="/etc/modprobe.d/nvidia-blacklists-nouveau.conf"
     upperdir="/media/root-rw/overlay/"
     lowerdir="/media/root-ro/"
     if [ -e "${upperdir}/${nvidia_blacklist_file}" ]; then
        if [ -f "${nvidia_blacklist_file}" ]; then
            # Create blacklist file
            rm -f ${nvidia_blacklist_file}
        else
            # Delete blacklist file
            cp -f ${lowerdir}/${nvidia_blacklist_file} ${nvidia_blacklist_file}
        fi
        update-initramfs -u
     fi
 }

error_exit_dgm() {
    echo "$1"
    cleanWorking
    exit "$2"
}

error_reboot() {
    echo "$1"
    nvidia_blacklist_recoverys
    cleanWorking
    sync
    reboot
}

apt_update()
{
    apt-get update
    if [ $? != 0 ]; then
        echo "Error: excute apt-get update failed"
        echo "PROGRESS:-1"
        exit 1
    fi
    echo "PROGRESS:5"
    apt-get install  --fix-missing || dpkg --configure -a
    if [ $? != 0 ]; then
        echo "Error: excute apt-get install  --fix-missing failed"
        echo "PROGRESS:-1"
        exit 1;
    fi
    echo "PROGRESS:15"
}

package_remove()
{
    pkg_list=$1
    len=$2
    inital_ratio=15
    max_ratio=50
    let range=${max_ratio}-${inital_ratio}
    for pkg in ${pkg_list[@]}
    do
        apt-get -y purge ${pkg};
        if [ $? != 0 ]; then
            echo "Error: remove ${pkg} failed"
            echo "PROGRESS:-1"
            exit 1
        fi
        let index++
        ratio=$(($index*$range/$len)+$inital_ratio)
        let ratio+=${inital_ratio}
        echo "PROGRESS:${ratio}"
    done
}

package_install()
{
   pkg_list=$1
   len=$2
    inital_ratio=50
    max_ratio=99
    for pkg in ${pkg_list[@]}
    do
        apt-get -y --reinstall --allow-downgrades install ${pkg};
        if [ $? != 0 ]; then
            echo "Error：install ${pkg} failed"
            echo "PROGRESS:-1"
            exit 1
        fi
        let index++;
        ratio=$(($index*$range/$len)+$inital_ratio)
        let ratio+=${inital_ratio}
        echo "PROGRESS:${ratio}"
    done
}