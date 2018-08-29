#!/bin/bash

/tmp/exe-remove-old.sh post || (echo "remove old driver failed!" && sync && reboot)
/tmp/exe.sh post || (echo "install new driver failed!" && sync && reboot)
overlayroot-chroot sed -i -E 's/(success=).*$/\1true/' /usr/lib/deepin-graphics-driver-manager/config.conf
