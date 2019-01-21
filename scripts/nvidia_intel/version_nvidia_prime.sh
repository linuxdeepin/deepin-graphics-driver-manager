#!/bin/bash

apt-cache --no-all-versions show deepin-nvidia-prime | awk 'match($0, /Version:\s*(.+)$/, a) {print a[1]}'
