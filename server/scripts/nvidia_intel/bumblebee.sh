#!/bin/bash

BATTERY=`qdbus --system org.freedesktop.UPower /org/freedesktop/UPower org.freedesktop.UPower.LidIsPresent`

if [ x"$BATTERY" == x"false" ]
then
    echo "cannot support bumblebee"	
    exit 1
else
    echo "Support bumblebee"	
    exit 0
fi
