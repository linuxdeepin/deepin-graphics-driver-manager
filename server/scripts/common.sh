#!/bin/bash

export WORKING_DIR_G=/usr/lib/deepin-graphics-driver-manager/working-dir
export TEST_IN_OVERLAY_G=$WORKING_DIR_G/test_in_overlay_flag
export REMOVE_OLD_G=$WORKING_DIR_G/remove_old.sh
export INSTALL_NEW_G=$WORKING_DIR_G/install_new.sh
export CONFIG_FILE_G=$WORKING_DIR_G/config.conf

cleanWorking() {
    isInOverlayRoot="$(df -h | grep -e "^overlay.*/$")"
    if [[ -z "${isInOverlayRoot}" ]]; then
        rm -rf $TEST_IN_OVERLAY_G
        rm -rf $REMOVE_OLD_G
        rm -rf $INSTALL_NEW_G
    else
        /usr/sbin/overlayroot-chroot rm -rf $TEST_IN_OVERLAY_G
        /usr/sbin/overlayroot-chroot rm -rf $REMOVE_OLD_G
        /usr/sbin/overlayroot-chroot rm -rf $INSTALL_NEW_G
        /usr/sbin/overlayroot-disable
    fi
}

error_exit_dgm() {
    echo "$1"
    cleanWorking
    exit "$2"
}

error_reboot() {
    echo "$1"
    cleanWorking
    sync
    reboot
}

package_download() {
    pkg_list=$1
    len=$(($2+2))
    index=0

    ratio=0
    echo "PROGRESS:${ratio}"

    apt-get update
    if [ $? != 0 ]; then
        echo "Excute apt-get update failed"
        echo "PROGRESS:${-1}"
        exit 1;
    fi
    let index++
    ratio=$(($index*100/$len))
    echo "PROGRESS:${ratio}"

    apt-get install  --fix-missing
    if [ $? != 0 ]; then
        echo "Excute --fix-missing failed"
        echo "PROGRESS:${-1}"
        exit 1;
    fi
    let index++
    ratio=$(($index*100/$len))
    echo "PROGRESS:${ratio}"

    for pkg in ${pkg_list[@]}
    do
        apt-get install -d --reinstall -y --allow-downgrades ${pkg};
        if [ $? != 0 ]; then
            echo "Download ${pkg} failed"
            echo "PROGRESS:${-1}"
            exit 1;
        fi
        let index++;
        ratio=$(($index*100/$len))
        let ratio-=1
        echo "PROGRESS:${ratio}"
    done
}