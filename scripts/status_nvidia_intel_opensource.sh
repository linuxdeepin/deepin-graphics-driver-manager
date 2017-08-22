#!/bin/sh

if [[ `lsmod` == *"nvidia"* ]]; then
    exit 0
else
    exit -1
fi