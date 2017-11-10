#!/bin/bash

PREPARE=$1
INSTALL=$2

OLD_DRIVER=$3
NEW_DRIVER=$4

USER_LANG=$5
EXIT_TEST=$6

echo $PREPARE
echo $INSTALL

echo $OLD_DRIVER
echo $NEW_DRIVER

bash -x $PREPARE || exit 1
cp -f $INSTALL /usr/bin/deepin-graphics-driver-installer.sh || exit 1
chmod +x /usr/bin/deepin-graphics-driver-installer.sh
overlayroot-enable || exit 1

echo "[General]
lang=$USER_LANG
exit_test=$EXIT_TEST
old_driver=$OLD_DRIVER
new_driver=$NEW_DRIVER
success=false
notified=false
" | tee /usr/lib/deepin-graphics-driver-manager/config.conf

chmod 666 /usr/lib/deepin-graphics-driver-manager/config.conf
