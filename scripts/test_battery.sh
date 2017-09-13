#!/bin/sh

BATTERY=`qdbus com.deepin.daemon.InputDevices /com/deepin/daemon/Power com.deepin.daemon.Power.LidIsPresent`

if [[ "$BATTERY" == "false" ]]
then
    exit -1
else
    exit 0
fi