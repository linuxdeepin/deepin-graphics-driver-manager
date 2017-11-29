#!/bin/sh

apt-cache show bumblebee | awk 'match($0, /Version:\s*(.+)$/, a) {print a[1]}'
