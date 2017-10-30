#!/bin/bash

/bin/dmesg --console-off
/bin/plymouth quit
overlayroot-disable
overlayroot-chroot mv /usr/bin/deepin-graphics-driver-installer.sh /tmp/exe.sh

dialog --title "Deepin Graphics Driver Manager - Installer" --yesno "\nSure to start install driver?" 6 50

if [ $? != 0 ]; then
	exit 0
fi

#bash -x /tmp/exe.sh test
/tmp/exe.sh test
/usr/bin/xinit /usr/lib/deepin-graphics-driver-manager/gltest

if [ -e "/tmp/gltest-success" ]; then
	echo "syncing into disk......"
	#touch /tmp/succes
#	bash -x /tmp/exe.sh post
	/tmp/exe.sh post
fi

reboot
