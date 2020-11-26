#!/bin/bash

apt-cache --no-all-versions show bumblebee | awk 'match($0, /Version:\s*(.+)$/, a) {print a[1]}'
