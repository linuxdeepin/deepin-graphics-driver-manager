#!/bin/bash

/usr/lib/deepin-graphics-driver-manager/nvidia/install_nvidia_closesource.sh || exit 1

cat  > /etc/X11/xorg.conf.d/70-nvidia.conf <<EOF
Section "ServerLayout"
  Identifier "layout"
  #Screen 0 "iGPU"
  Option "AllowNVIDIAGPUScreens"
EndSection

Section "Device"
  Identifier "iGPU"
  Driver "modesetting"
EndSection

Section "Screen"
  Identifier "iGPU"
  Device "iGPU"
EndSection

Section "Device"
  Identifier "dGPU"
  Driver "nvidia"
EndSection
EOF

echo -e '#!/bin/sh\nexport __NV_PRIME_RENDER_OFFLOAD=1\nexport __GLX_VENDOR_LIBRARY_NAME=nvidia\n/usr/lib/deepin-graphics-driver-manager/gltest\n' > /tmp/deepin-prime-gltest
chmod a+x /tmp/deepin-prime-gltest
