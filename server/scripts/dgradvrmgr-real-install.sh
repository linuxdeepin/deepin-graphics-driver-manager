#!/bin/bash

rm -rf /etc/xdg/autostart/deepin-gradvrmgr-installer.desktop

# write to tty
journalctl -f -u dgradvrmgr-test-gltest.service | sed 's/$/\r/g' > /dev/tty1 2>&1 &

. /usr/lib/deepin-graphics-driver-manager/common.sh
#remove/install drivers
$REMOVE_OLD_G || error_reboot "test remove old driver failed!"
$INSTALL_NEW_G || error_reboot "test install new driver failed!"

exit 0

