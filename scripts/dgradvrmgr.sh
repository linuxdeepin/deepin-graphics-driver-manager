#!/bin/bash

POSTOS=`cat /proc/mounts | awk '{if ($2 == "/media/root-ro") print $1}'`

#dialog --timeout 8 --title "Deepin Graphics Driver Manager - Installer" --yesno "\nSure to start install driver?" 6 50

# remount devices
mount -o remount,rw $POSTOS /media/root-ro

# write to local drive
journalctl -f -u driver-installer.service >> /media/root-ro/var/log/dgradvrmgr.log 2>&1 &

# write to tty
journalctl -f -u driver-installer.service | sed 's/$/\r/g' > /dev/tty1 2>&1 &

# TODO: may be is useless, remove #
#if [ $? == 1 ]; then
#	reboot
#fi

if [[ -e "/etc/modprobe.d/deepin-blacklists-nvidia.conf" ]]; then
    echo "remove modules about nvidia from blacklist!"
    overlayroot-chroot rm /etc/modprobe.d/deepin-blacklists-nvidia.conf
    overlayroot-chroot update-initramfs -u -t
fi

/tmp/exe-remove-old.sh "test" || (echo "test remove old driver failed!" && sync && reboot)
/tmp/exe.sh "test" || (echo "test install new driver failed!" && sync && reboot)

if [[ -e "/tmp/deepin-prime-gltest" ]]; then
    /usr/bin/xinit /tmp/deepin-prime-gltest
else
    /usr/bin/xinit /usr/lib/deepin-graphics-driver-manager/gltest
fi

killall Xorg

sleep 1

if [ -e "/tmp/gltest-success" ]; then
    /usr/lib/deepin-graphics-driver-manager/dgradvrmgr-post.sh
fi

reboot
