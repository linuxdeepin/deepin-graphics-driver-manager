#!/bin/sh
#当前的intel加速方式
intel_accel_method=`cat /var/log/intel_driver_mode.status`
echo "now intel accel mode is : $intel_accel_method"

# 0 ------ default is glamor
# 1 ------ use SNA accel method
# 2 ------ use UXA accel method

set_intel_accel_sna() {
INTEL_XORG_CONF="/etc/X11/xorg.conf.d/20-intel.conf"
echo 'Section "Device"
Identifier "Intel Graphics"
Driver "intel"
Option "AccelMethod" "sna"
#Option "PageFlip" "False"
#Option "TearFree" "True"
EndSection' | sudo tee $INTEL_XORG_CONF
}

set_intel_accel_uxa() {
INTEL_XORG_CONF="/etc/X11/xorg.conf.d/20-intel.conf"
echo 'Section "Device"
Identifier "Intel Graphics"
Driver "intel"
Option "AccelMethod" "uxa"
#Option "PageFlip" "False"
#Option "TearFree" "True"
EndSection' | sudo tee $INTEL_XORG_CONF
}

Xorg -configure
[ -f /root/xorg.conf.new ] && cp /root/xorg.conf.new /etc/X11/xorg.conf

#根据需求调整显卡的加速方式，从而达到最优化
case $1 in
	0)
		rm /etc/X11/xorg.conf.d/20-intel.conf
		echo "Using default glamor accel method"
	;;
	1)set_intel_accel_sna
		
	;;
	2)set_intel_accel_uxa
	;;
	*)
		rm /etc/X11/xorg.conf.d/20-intel.conf
		echo "You need to set an accel method,default glamor"
	;;	
esac	

#需要用户界面来提示是否保存资料，重启桌面
#sudo systemctl restart lightdm
