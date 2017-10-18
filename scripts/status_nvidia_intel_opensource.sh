#!/bin/sh

nouveau_mod=`lsmod | grep nouveau`
nvidia_mod=`lsmod | grep nvidia`

if [ -n "$nouveau_mod" ]; then
	 echo "Had already used opensource nouveau driver"
	 exit 1
fi


if [ -n "$nvidia_mod" ]; then
	 echo "Had already used private nvidia driver "
	 exit 2
fi
exit 0
