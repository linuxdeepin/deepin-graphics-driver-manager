#!/bin/sh
#ret=`lspci | grep VGA | grep Intel Corporation`
#if [ -n "$ret" ]; then
#	echo "Found Intel card: $ret"
#else
#	echo "No Intel card found."
#	exit -1
#fi	
#当前的intel加速方式
#intel_accel_method=`cat /var/log/intel_driver_mode.status`
intel_accel_method_sna=`cat /var/log/Xorg.0.log | grep SNA`
intel_accel_method_uxa=`cat /var/log/Xorg.0.log | grep UXA`
if [ -n "$intel_accel_method_sna" ];then
	echo "now intel accel mode is : SNA"
	intel_accel_method=2
	exit 2
elif [ -n "$intel_accel_method_uxa" ];then
	echo "now intel accel mode is : UXA"
	intel_accel_method=1
	exit 1
else
	echo "now intel accel mode is : glamor(default)"
	intel_accel_method=0
	exit 0
fi

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

#Xorg -configure
#[ -f /root/xorg.conf.new ] && cp /root/xorg.conf.new /etc/X11/xorg.conf

#根据需求调整显卡的加速方式，从而达到最优化
case $intel_accel_method in
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
