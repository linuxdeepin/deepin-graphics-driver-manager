#!/bin/bash

rm -rf /etc/xdg/autostart/deepin-gradvrmgr-installer.desktop

control="$1"

if [ $control == install ]; then
	/usr/lib/deepin-graphics-driver-manager/working-dir/install_new.sh
	exit $?
elif [ $control == remove ]; then
	/usr/lib/deepin-graphics-driver-manager/working-dir/remove_old.sh
	exit $?
fi

