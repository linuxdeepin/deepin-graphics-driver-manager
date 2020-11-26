#!/bin/bash

if [ "$(id -u)" -ne "0" ];then
    echo "Need root privileges."
    exit 1
fi

export DEBIAN_FRONTEND=noninteractive

apt-get --reinstall -y --allow-downgrades install \
    xserver-xorg-core \
    xserver-xorg-input-all \
    libgl1-mesa-glx

if [[ $? -ne 0 ]]; then
    echo "apt-get execute failed!"
    exit 1
fi

# 默认驱动方案也需要让内核加载一个n卡的模块以便系统可以识别，否则系统不知道有这个物理设备
# 由于nouveau模块是无法通过包管理器卸载的，因此在这里加载nouveau
# 让内核加载nouveau模块，但不让X加载Xorg的nouveau或nvidia模块就是默认n卡方案了
echo "Loading kernel modules......"
modprobe nouveau

exit 0
