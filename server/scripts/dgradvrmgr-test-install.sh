#!/bin/bash

. /usr/lib/deepin-graphics-driver-manager/common.sh

if [[ -z "${isInOverlayRoot}" ]]; then
    rm  -f /etc/xdg/autostart/deepin-gradvrmgr-test-installer.desktop
    cp ${INSTALLER_DESKTOP_FILE_SOURCE} ${INSTALLER_DESKTOP_FILE_DEST} || error_exit "Copy ${INSTALLER_DESKTOP_FILE_SOURCE} to ${INSTALLER_DESKTOP_FILE_DEST} failed" ${COMMON_ERROR}
    error_exit "Overlayroot is not enabled, please enable the overlayroot" ${COMMON_ERROR}
fi

/usr/sbin/overlayroot-chroot rm -f /etc/xdg/autostart/deepin-gradvrmgr-test-installer.desktop
/usr/sbin/overlayroot-chroot cp ${INSTALLER_DESKTOP_FILE_SOURCE} ${INSTALLER_DESKTOP_FILE_DEST} || error_exit "Overlay-chroot copy ${INSTALLER_DESKTOP_FILE_SOURCE} to ${INSTALLER_DESKTOP_FILE_DEST} failed" ${COMMON_ERROR}

#check network
check_network || error_exit "The network is not working, please check the network connection" ${NETWORK_CONNECTION_ERROR}

#apt-get update
apt_update || error_exit "Execute apt update failed" ${APT_UPDATE_ERROR}
　
#remove old driver
$REMOVE_OLD_G 
if [ $? != 0 ]; then
    nvidia_blacklist_recovery
    error_exit "Remove old driver failed" ${PURGE_PACKAGE_ERROR}
fi

#install old driver
$INSTALL_NEW_G 
if [ $? != 0 ]; then
    nvidia_blacklist_recovery
    error_exit "Install new driver failed" ${INSTALL_PACKAGE_ERROR}
fi

/usr/sbin/overlayroot-chroot touch ${GLTEST_FLAG} || error_exit "Overlay-chroot touch ${GLTEST_FLAG} failed" ${COMMON_ERROR}

#save the upperdir
overlayroot_save || error_exit "Overlayroot save failed" ${OVERLAYROOT_SAVE_ERROR}

exit 0
