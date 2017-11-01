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
sleep 5

if [ -e "/tmp/gltest-success" ]; then
	echo "syncing into disk......"
	#touch /tmp/succes
#	bash -x /tmp/exe.sh post
	/tmp/exe.sh post
	sed -i -E 's/(success=.*$)/\1true/' /usr/lib/deepin-graphics-driver-manager/config.conf
fi

reboot
