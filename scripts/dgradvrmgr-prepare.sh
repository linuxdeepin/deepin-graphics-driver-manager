#!/bin/bash

PREPARE=$1
INSTALL=$2

OLD_DRIVER=$3
NEW_DRIVER=$4

echo $PREPARE
echo $INSTALL

echo $OLD_DRIVER
echo $NEW_DRIVER

bash -x $PREPARE || exit 1
cp -f $INSTALL /usr/bin/deepin-graphics-driver-installer.sh || exit 1
overlayroot-enable || exit 1

echo "[General]
old_driver=$OLD_DRIVER
new_driver=$NEW_DRIVER
success=false
notified=false
" | tee /usr/lib/deepin-graphics-driver-manager/config.conf

chmod 666 /usr/lib/deepin-graphics-driver-manager/config.conf
