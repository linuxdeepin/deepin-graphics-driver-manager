#!/bin/bash
systemctl stop lightdm

# write to tty
journalctl -f -u driver-installer.service | sed 's/$/\r/g' > /dev/tty1 2>&1 &


#remove/install drivers
$REMOVE_OLD_G || error_reboot "test remove old driver failed!"
$INSTALL_NEW_G || error_reboot "test install new driver failed!"

reboot
