#!/bin/bash

/tmp/exe-remove-old.sh post || reboot
/tmp/exe.sh post || reboot
overlayroot-chroot sed -i -E 's/(success=).*$/\1true/' /usr/lib/deepin-graphics-driver-manager/config.conf
