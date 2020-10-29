
#include "graphicsdeviceinfo.h"

#include <QProcess>
#include <QDebug>

extern "C" {
    #include <pci/pci.h>
}

#include <cstring>

#define CLASSNAME_VGA   "VGA compatible controller"
#define CLASSNAME_3D    "3D controller"

// NOTE: see more info /usr/share/misc/pci.ids
inline bool is_display_controller(const uint devClass)
{
    return (devClass & 0xff00) == 0x0300 || devClass == 0x0400;
}

GraphicsDeviceInfo::DeviceFlag GraphicsDeviceInfo::deviceType(const QString &devInfo)
{
    if (devInfo.contains("Intel"))
        return GraphicsDeviceInfo::INTEL;
    else if (devInfo.contains("NVIDIA"))
        return GraphicsDeviceInfo::NVIDIA;
    else if (devInfo.contains("AMD"))
        return GraphicsDeviceInfo::AMD;
    else
        return GraphicsDeviceInfo::NoDevice;
}

GraphicsDeviceInfo::GraphicsDeviceInfo() :
    m_sysDevFlag(NoDevice)
{
    init();
}

int GraphicsDeviceInfo::deviceNums() const
{
    int flag = m_sysDevFlag;
    int ret = 0;

    while (flag)
    {
        flag &= flag - 1;
        ++ret;
    }

    return ret;
}

void GraphicsDeviceInfo::init()
{
    struct pci_access *pacc;
    struct pci_dev *dev;
    char namebuf[1024];

    pacc = pci_alloc();
    pci_init(pacc);
    pci_scan_bus(pacc);
    for (dev = pacc->devices; dev; dev = dev->next)
    {
        pci_fill_info(dev, PCI_FILL_CLASS | PCI_FILL_IDENT);
        if (!is_display_controller(dev->device_class))
            continue;

        const QString devInfo = pci_lookup_name(pacc, namebuf, sizeof(namebuf), PCI_LOOKUP_VENDOR | PCI_LOOKUP_DEVICE, dev->vendor_id, dev->device_id);

        m_sysDevFlag |= deviceType(devInfo);
        m_devices << devInfo;
    }
    pci_cleanup(pacc);
}
