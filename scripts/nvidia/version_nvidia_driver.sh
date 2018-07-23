#!/bin/bash

apt-cache --no-all-versions show nvidia-driver | awk 'match($0, /Version:\s*(.+)$/, a) {print a[1]}'
