
#define private public

#include "graphicsdeviceinfo.h"

#include <cassert>

int main()
{
    GraphicsDeviceInfo info;

    assert(info.m_devices.size() == 2);

    return 0;
}