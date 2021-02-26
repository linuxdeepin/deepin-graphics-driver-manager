#!/bin/bash

export WORKING_DIR_G=/usr/lib/deepin-graphics-driver-manager/working-dir
export TEST_IN_OVERLAY_G=$WORKING_DIR_G/test_in_overlay_flag
export REMOVE_OLD_G=$WORKING_DIR_G/remove_old.sh
export INSTALL_NEW_G=$WORKING_DIR_G/install_new.sh
export CONFIG_FILE_G=$WORKING_DIR_G/config.conf
export REAL_INSTALLE_DESKTOP=/etc/xdg/autostart/deepin-gradvrmgr-installer.desktop

export INSTALLER_DESKTOP_FILE_SOURCE=/usr/lib/deepin-graphics-driver-manager/deepin-gradvrmgr-installer.desktop
export INSTALLER_DESKTOP_FILE_DEST=/etc/xdg/autostart/deepin-gradvrmgr-installer.desktop
export TEST_INSTALLER_DESKTOP_FILE_SOURCE=/usr/lib/deepin-graphics-driver-manager/deepin-gradvrmgr-test-installer.desktop
export TEST_INSTALLER_DESKTOP_FILE_DEST=/etc/xdg/autostart/deepin-gradvrmgr-test-installer.desktop

export GLTEST_FLAG=/usr/lib/deepin-graphics-driver-manager/working-dir/dgradvrmgr_gltest_flag
export OVERLAYROOT_IMAGE=$WORKING_DIR_G/overlayroot.img
export isInOverlayRoot=$(grep -m1 "^overlayroot / overlay " /proc/mounts) || isInOverlayRoot=
export DEBIAN_FRONTEND=noninteractive

#define error exit code
export COMMON_ERROR=1
export APT_UPDATE_ERROR=2
export PURGE_PACKAGE_ERROR=3
export INSTALL_PACKAGE_ERROR=4
export NETWORK_CONNECTION_ERROR=5
export OVERLAYROOT_ENABLE_ERROR=6
export OVERLAYROOT_DISABLE_ERROR=7
export OVERLAYROOT_SAVE_ERROR=8

OVERLAYROOT_CONF=/etc/overlayroot.conf
LOOP_DEV=/dev/loop0

overlayroot_disable() {
    if [[ -n "${isInOverlayRoot}" ]]; then
         overlayroot-chroot sed -i 's:overlayroot=".*":overlayroot="":' ${OVERLAYROOT_CONF}
        [ -e "${OVERLAYROOT_IMAGE}" ] && overlayroot-chroot rm -f ${OVERLAYROOT_IMAGE}
    else
        sed -i 's:overlayroot=".*":overlayroot="":' ${OVERLAYROOT_CONF}
        [ -e "${OVERLAYROOT_IMAGE}" ] && rm -f ${OVERLAYROOT_IMAGE}
    fi
}

overlayroot_enable() {
    if [[ -n "${isInOverlayRoot}" ]]; then
        echo "overlayroot is enabled already"
    else
        [ -e "${OVERLAYROOT_IMAGE}" ] && rm -f ${OVERLAYROOT_IMAGE}
        dd if=/dev/zero of=${OVERLAYROOT_IMAGE} bs=1MiB count=640
        mkfs.ext4 ${OVERLAYROOT_IMAGE} && sed -i "s:overlayroot=".*":overlayroot=\"device\:dev=\/dev\/loop0,recurse=0\":" ${OVERLAYROOT_CONF}
    fi
}

overlayroot_save() {
    overlayroot-chroot cp -f /run/overlayroot/overlayroot.img ${OVERLAYROOT_IMAGE}
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
            echo "blacklist  nouveau" > ${nvidia_blacklist_file}
        fi
        update-initramfs -u
     fi
 }

error_exit() {
    echo "Error: $1"
    cleanWorking
    modify_config "exit_code" "$2"
    exit "$2"
}

error_reboot() {
    echo "$1"
    cleanWorking
    sync
    reboot
}

apt_update()
{
    loop=0
    while [ $loop -lt 20 ]
    do
        sleep 1
        let loop+=1
        apt-get update && (apt-get install --fix-missing || dpkg --configure -a)
        [ $? -eq 0 ] && break
    done
}

package_remove()
{
    pkg_list=$1
    len=$2
    inital_ratio=15
    max_ratio=45
    echo "PROGRESS:15"
    let range=${max_ratio}-${inital_ratio}
    for pkg in ${pkg_list[@]}
    do
        apt-get -y purge ${pkg};
        let index++
        ratio=$(($index*$range/$len))
        let ratio+=${inital_ratio}
        echo "PROGRESS:${ratio}"
    done

    apt-get autoremove -y
    echo "PROGRESS:50"
}

package_install()
{
   pkg_list=$1
   len=$2
    inital_ratio=50
    max_ratio=99
    let range=${max_ratio}-${inital_ratio}
    for pkg in ${pkg_list[@]}
    do
        apt-get -y --reinstall --allow-downgrades install ${pkg};
        let index++;
        ratio=$(($index*$range/$len))
        let ratio+=${inital_ratio}
        echo "PROGRESS:${ratio}"
    done
}

check_network()
{
    loop=0
    while [ $loop -lt 10 ]
    do
        sleep 1
        let loop+=1
        ping -c 1 www.baidu.com > /dev/null 2>&1
        [ $? -eq 0 ] && break
    done
}

modify_config()
{
    key=$1
    value=$2
    if [[ -n "${isInOverlayRoot}" ]]; then
        overlayroot-chroot sed -i -E "s/(${key}=).*$/\1${value}/" $CONFIG_FILE_G
    else
        sed -i -E "s/(${key}=).*$/\1${value}/" $CONFIG_FILE_G
    fi
}