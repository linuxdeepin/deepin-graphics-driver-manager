#!/bin/bash

# exit 0 ------ no driver,use default glamor
# exit 1 ------ use open nouveau driver 
# exit 2 ------ use private nvidia driver

nouveau_mod=`lsmod | grep nouveau`
nvidia_mod=`lsmod | grep nvidia`

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
    exit 2
fi

exit 0
