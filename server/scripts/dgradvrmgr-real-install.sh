#!/bin/bash

# write to tty
journalctl -f -u dgradvrmgr-test-gltest.service | sed 's/$/\r/g' > /dev/tty1 2>&1 &

. /usr/lib/deepin-graphics-driver-manager/common.sh
#remove/install drivers
$REMOVE_OLD_G || error_exit_dgm "test remove old driver failed!" 1
$INSTALL_NEW_G || error_exit_dgm "test install new driver failed!" 1

exit 0