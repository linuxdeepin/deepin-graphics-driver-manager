#!/bin/bash

. /usr/lib/deepin-graphics-driver-manager/common.sh

overlayroot-chroot sed -i -E 's/(gltest-success=).*$/\1true/' $CONFIG_FILE_G
