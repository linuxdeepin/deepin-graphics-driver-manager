#!/bin/bash

/bin/dmesg --console-off 
/bin/plymouth quit 

dialog --title "Deepin Graphics Driver Manager - Installer" --yesno "\nSure to start install driver?" 6 50

if [ $? != 0 ]; then
	exit 0
fi

apt install deepin-deb-installer
/usr/bin/xinit /usr/bin/gltest
