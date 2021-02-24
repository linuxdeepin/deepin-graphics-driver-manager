#!/bin/bash

. /usr/lib/deepin-graphics-driver-manager/common.sh

cleanWorking

if [[ -n "${isInOverlayRoot}" ]]; then
    [ -e "${INSTALLER_DESKTOP_FILE_DEST}" ] && /usr/sbin/overlayroot-chroot rm -rf $INSTALLER_DESKTOP_FILE_DEST
    [ -e "${TEST_INSTALLER_DESKTOP_FILE_DEST}" ] && /usr/sbin/overlayroot-chroot rm -rf $TEST_INSTALLER_DESKTOP_FILE_DEST
    [ -e "${GLTEST_FLAG}" ] && /usr/sbin/overlayroot-chroot rm -rf $GLTEST_FLAG
else
    [ -e "${INSTALLER_DESKTOP_FILE_DEST}" ] && rm -rf $INSTALLER_DESKTOP_FILE_DEST
    [ -e "${TEST_INSTALLER_DESKTOP_FILE_DEST}" ] && rm -rf $TEST_INSTALLER_DESKTOP_FILE_DEST
    [ -e "${GLTEST_FLAG}" ] && rm -rf $GLTEST_FLAG
fi

