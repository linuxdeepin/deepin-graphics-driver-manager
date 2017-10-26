#!/bin/bash

PREPARE=$1
INSTALL=$2

echo $PREPARE
echo $INSTALL

bash -x $PREPARE
cp -f $INSTALL /usr/bin/deepin-graphics-driver-installer.sh
overlayroot-enable