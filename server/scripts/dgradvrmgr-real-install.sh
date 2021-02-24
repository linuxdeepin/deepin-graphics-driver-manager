#!/bin/bash

. /usr/lib/deepin-graphics-driver-manager/common.sh

# write to tty
journalctl -f -u dgradvrmgr-test-installer.service | sed 's/$/\r/g' > /dev/tty1 2>&1 &

#apt-get update
apt_update

#remove/install drivers
$REMOVE_OLD_G || error_reboot "test remove old driver failed!"
$INSTALL_NEW_G || error_reboot "test install new driver failed!"

reboot
