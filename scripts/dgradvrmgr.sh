#!/bin/bash

/bin/dmesg --console-off
/bin/plymouth quit
overlayroot-disable
overlayroot-chroot mv /usr/bin/deepin-graphics-driver-installer.sh /tmp/exe.sh

dialog --timeout 8 --title "Deepin Graphics Driver Manager - Installer" --yesno "\nSure to start install driver?" 6 50

if [ $? == 1 ]; then
	reboot
fi

#bash -x /tmp/exe.sh test
/tmp/exe.sh test
/usr/bin/xinit /usr/lib/deepin-graphics-driver-manager/gltest
killall Xorg
sleep 1

if [ -e "/tmp/gltest-success" ]; then
    /usr/lib/deepin-graphics-driver-manager/dgradvrmgr-post.sh
fi

reboot
