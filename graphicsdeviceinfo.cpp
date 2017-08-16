
#include "graphicsdeviceinfo.h"

#include <QProcess>
#include <QDebug>

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
    QProcess proc;
    proc.start("lspci");
    proc.waitForFinished();

    const auto &devices = proc.readAll().split('\n');
    for (const auto &dev : devices)
    {
        if (!dev.contains("VGA"))
            continue;

        m_sysDevFlag |= deviceType(dev);
        m_devices << dev;
    }
}