#!/bin/sh

if [[ `lsmod` == *"nouveau"* ]]; then
    exit 0
else
    exit -1
fi
