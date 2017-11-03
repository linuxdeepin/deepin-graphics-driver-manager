#!/bin/sh

# exit 0 ------ no driver,use default glamor
# exit 1 ------ use open nouveau driver 
# exit 2 ------ use private nvidia driver
nouveau_mod=`lsmod | grep nouveau`
nvidia_mod=`lsmod | grep nvidia`
BATTERY=`qdbus com.deepin.daemon.InputDevices /com/deepin/daemon/Power com.deepin.daemon.Power.LidIsPresent`

if [ -n "$nouveau_mod" ]; then
	if [ -f /usr/lib//xorg/modules/drivers/nouveau_drv.so ];then
		echo "Had already used opensource nouveau driver"
		exit 1
	else
		echo "Had already used default glamor driver"
		exit 0
	fi	
fi


if [ -n "$nvidia_mod" ]; then
	echo "Had already used private nvidia driver "
	if [ x"$BATTERY" == x"false" ]; then
		echo "Do not support bumblebee"	
		exit 2
	fi
fi
if [ x"$BATTERY" == x"true" ]; then
	
	echo "Support bumblebee,hahaha~~~"	
	[ -f /etc/bumblebee/bumblebee.conf ] && exit 3
fi
exit 0
