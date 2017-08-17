
#include "graphicsdeviceinfo.h"

#include <QProcess>
#include <QDebug>

extern "C" {
    #include <pci/pci.h>
}

#include <cstring>

#define VGA_CLASS "VGA compatible controller"

GraphicsDeviceInfo::DeviceFlag GraphicsDeviceInfo::deviceType(const QString &devInfo)
{
    if (devInfo.contains("Intel"))
        return GraphicsDeviceInfo::INTEL;
    else if (devInfo.contains("NVIDIA"))
        return GraphicsDeviceInfo::NVIDIA;
    else
        return GraphicsDeviceInfo::NoDevice;
}

GraphicsDeviceInfo::GraphicsDeviceInfo() :
    m_sysDevFlag(NoDevice)
{
    init();
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
        const char *devClass = pci_lookup_name(pacc, namebuf, sizeof(namebuf), PCI_LOOKUP_CLASS, dev->device_class);
        if (std::strcmp(devClass, VGA_CLASS))
            continue;

        const QString devInfo = pci_lookup_name(pacc, namebuf, sizeof(namebuf), PCI_LOOKUP_VENDOR | PCI_LOOKUP_DEVICE, dev->vendor_id, dev->device_id);

        qDebug() << devInfo;

        m_sysDevFlag |= deviceType(devInfo);
        m_devices << devInfo;
    }
}