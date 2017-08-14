
#include "graphicsdeviceinfo.h"

#include <QProcess>
#include <QDebug>

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

        m_devices << dev;
    }

    qDebug() << m_devices;
}