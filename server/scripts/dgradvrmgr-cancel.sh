#!/bin/bash

. /usr/lib/deepin-graphics-driver-manager/common.sh

cleanWorking

if [[ -n "${isInOverlayRoot}" ]]; then
    [ -e "${REAL_INSTALLE_DESKTOP}" ] && /usr/sbin/overlayroot-chroot rm -rf $REAL_INSTALLE_DESKTOP
else
    [ -e "${REAL_INSTALLE_DESKTOP}" ] && rm -rf $REAL_INSTALLE_DESKTOP
fi

