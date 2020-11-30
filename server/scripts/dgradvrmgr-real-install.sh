#!/bin/bash

rm -rf /etc/xdg/autostart/deepin-gradvrmgr-installer.desktop

#remove/install drivers
$REMOVE_OLD_G || error_reboot "test remove old driver failed!"
$INSTALL_NEW_G || error_reboot "test install new driver failed!"

exit 0

