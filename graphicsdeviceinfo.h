
#ifndef GRAPHICSDEVICEINFO_H
#define GRAPHICSDEVICEINFO_H

#include <QSet>

class GraphicsDeviceInfo
{
public:
    enum DeviceFlag
    {
        NoDevice    = 0,
        AMD         = 1,
        NVIDIA      = 1 << 1,
        INTEL       = 1 << 2,
    };

    explicit GraphicsDeviceInfo();

    int deviceFlag() const { return m_sysDevFlag; }

private:
    void init();

private:
    int m_sysDevFlag;
    QSet<QString> m_devices;
};

#endif