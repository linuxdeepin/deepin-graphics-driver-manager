#!/bin/bash

POSTOS=`cat /proc/mounts | awk '{if ($2 == "/media/root-ro") print $1}'`

#dialog --timeout 8 --title "Deepin Graphics Driver Manager - Installer" --yesno "\nSure to start install driver?" 6 50

# remount devices
mount -o remount,rw $POSTOS /media/root-ro

# write to local drive
journalctl -f -u driver-installer.service >> /media/root-ro/var/log/dgradvrmgr.log 2>&1 &

# write to tty
journalctl -f -u driver-installer.service | sed 's/$/\r/g' > /dev/tty1 2>&1 &

if [ $? == 1 ]; then
	reboot
fi

#bash -x /tmp/exe.sh test
/tmp/exe-remove-old.sh "test"
/tmp/exe.sh "test"
/usr/bin/xinit /usr/lib/deepin-graphics-driver-manager/gltest
killall Xorg
sleep 1

if [ -e "/tmp/gltest-success" ]; then
    /usr/lib/deepin-graphics-driver-manager/dgradvrmgr-post.sh
fi

reboot
