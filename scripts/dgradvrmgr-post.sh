#!/bin/bash

/tmp/exe-remove-old.sh post
/tmp/exe.sh post || exit 1
overlayroot-chroot sed -i -E 's/(success=).*$/\1true/' /usr/lib/deepin-graphics-driver-manager/config.conf
