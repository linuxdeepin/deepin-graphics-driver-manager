#!/bin/bash

apt-cache --no-all-versions show xserver-xorg-video-nouveau | awk 'match($0, /Version:\s*(.+)$/, a) {print a[1]}'
