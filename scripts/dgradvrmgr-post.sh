#!/bin/bash

. /usr/lib/deepin-graphics-driver-manager/error_func.sh

/tmp/exe-remove-old.sh post || error_reboot "remove old driver failed!"
/tmp/exe.sh post || error_reboot "install new driver failed!"
overlayroot-chroot sed -i -E 's/(success=).*$/\1true/' /usr/lib/deepin-graphics-driver-manager/config.conf
