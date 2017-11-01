
#define private public

#include "graphicsdeviceinfo.h"
#include "resolutions/resolution.h"

#include <cassert>

int main()
{
    const auto intel_e3v3 = "00:02.0 VGA compatible controller: Intel Corporation Xeon E3-1200 v3/4th Gen Core Processor Integrated Graphics Controller (rev 06)";
    assert(GraphicsDeviceInfo::deviceType(intel_e3v3) == GraphicsDeviceInfo::INTEL);

    const auto nvidia_gt740 = "01:00.0 VGA compatible controller: NVIDIA Corporation GK107 [GeForce GT 740] (rev a1)";
    assert(GraphicsDeviceInfo::deviceType(nvidia_gt740) == GraphicsDeviceInfo::NVIDIA);

    const auto amd = "01:00.0 VGA compatible controller: Advanced Micro Devices, Inc. [AMD/ATI] Tobago PRO [Radeon R7 360 / R9 360 OEM] (rev 81)";
    assert(GraphicsDeviceInfo::deviceType(amd) == GraphicsDeviceInfo::AMD);

    return 0;
}
