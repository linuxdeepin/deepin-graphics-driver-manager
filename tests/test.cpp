
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

    Resolution r1(1, "R1", "description1");
    Resolution r2(1, "R1", "description2");
    Resolution r3(2, "R2", "description1");
    assert(r1 == r2);
    assert(!(r1 == r3));
    assert(!(r2 == r3));

    return 0;
}