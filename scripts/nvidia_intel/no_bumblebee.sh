#!/bin/bash

BATTERY=`qdbus com.deepin.daemon.InputDevices /com/deepin/daemon/Power com.deepin.daemon.Power.LidIsPresent`

if [ x"$BATTERY" == x"false" ]
then
    echo "cannot support bumblebee"	
    exit 0
else
    echo "Support bumblebee"	
    exit 1
fi
