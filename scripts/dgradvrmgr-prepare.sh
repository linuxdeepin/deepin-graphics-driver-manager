#!/bin/bash

. /usr/lib/deepin-graphics-driver-manager/common.sh

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

# execute prepare script for new solution
bash -x $PREPARE || error_exit_dgm "prepare for $NEW_DRIVER failed!" 1

# prepare working directory
mkdir -p $WORKING_DIR_G || error_exit_dgm "mkdir $WORKING_DIR_G failed!" 1
# driver-installer service will startup when this file exist
touch $TEST_IN_OVERLAY_G
# copy remove/install scripts to working dir
cp -f $REMOVEOLD $REMOVE_OLD_G || error_exit_dgm "copy $REMOVEOLD failed!" 1
cp -f $INSTALL $INSTALL_NEW_G || error_exit_dgm "copy $INSTALL failed!"  1
chmod +x $REMOVE_OLD_G
chmod +x $INSTALL_NEW_G

# generate config file
echo "[General]
lang=$USER_LANG
exit_test=$EXIT_TEST
old_driver=$OLD_DRIVER
new_driver=$NEW_DRIVER
gltest-success=false
success=false
notified=false
" | tee $CONFIG_FILE_G

chmod 666 $CONFIG_FILE_G

# Warning: overlayroot-disable command only works in system is already in overlay mode
# overlayroot will take effect after reboot
# overlayroot-disable will excute when driver-installer service startup or some error occured
overlayroot-enable || error_exit_dgm "overlayroot-enable failed!" 1
