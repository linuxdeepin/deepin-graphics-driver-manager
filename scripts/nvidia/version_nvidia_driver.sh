#!/bin/sh

apt-cache show nvidia-driver | awk 'match($0, /Version:\s*(.+)$/, a) {print a[1]}'
