#!/bin/bash

PREPARE=$1
REMOVEOLD=$2
INSTALL=$3

OLD_DRIVER=$4
NEW_DRIVER=$5

USER_LANG=$6
EXIT_TEST=$7

echo $PREPARE
echo $REMOVEOLD
echo $INSTALL

echo $OLD_DRIVER
echo $NEW_DRIVER

bash -x $PREPARE || (echo "prepare for $NEW_DRIVER failed!" && exit 1)
cp -f $REMOVEOLD /usr/bin/deepin-graphics-driver-remove-old.sh || (echo "copy $REMOVEOLD failed!" && exit 1)
cp -f $INSTALL /usr/bin/deepin-graphics-driver-installer.sh || (echo "copy $INSTALL failed!" && exit 1)
chmod +x /usr/bin/deepin-graphics-driver-remove-old.sh
chmod +x /usr/bin/deepin-graphics-driver-installer.sh
overlayroot-enable || (echo "overlayroot-enable failed!" && exit 1)

echo "[General]
lang=$USER_LANG
exit_test=$EXIT_TEST
old_driver=$OLD_DRIVER
new_driver=$NEW_DRIVER
success=false
notified=false
" | tee /usr/lib/deepin-graphics-driver-manager/config.conf

chmod 666 /usr/lib/deepin-graphics-driver-manager/config.conf
