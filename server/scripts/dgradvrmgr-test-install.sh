#!/bin/bash

/usr/sbin/overlayroot-chroot rm /etc/xdg/autostart/deepin-gradvrmgr-test-installer.desktop

. /usr/lib/deepin-graphics-driver-manager/common.sh

# write to tty
journalctl -f -u dgradvrmgr-test-installer.service | sed 's/$/\r/g' > /dev/tty1 2>&1 &

#apt-get update
apt_update

#remove old driver
$REMOVE_OLD_G 
if [ $? != 0 ]; then
    echo "remove old driver failed!"
    nvidia_blacklist_recovery
    exit 1
fi

#install old driver
$INSTALL_NEW_G 
if [ $? != 0 ]; then
    echo "install new driver failed!"
    nvidia_blacklist_recovery
    exit 1
fi

if [[ -n "${isInOverlayRoot}" ]]; then
    /usr/sbin/overlayroot-chroot touch ${GLTEST_FLAG}
    /usr/sbin/overlayroot-chroot touch ${INSTALLER_DESKTOP_FILE_DEST}
    overlayroot_save
fi

exit 0
