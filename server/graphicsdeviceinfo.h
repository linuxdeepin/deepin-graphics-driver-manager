
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
    Q_DECLARE_FLAGS(DeviceFlags, DeviceFlag)

    explicit GraphicsDeviceInfo();

    int deviceFlag() const { return m_sysDevFlag; }
    int deviceNums() const;
    const QSet<QString> devices() const { return m_devices; }

    static GraphicsDeviceInfo::DeviceFlag deviceType(const QString &devInfo);

private:
    void init();

private:
    DeviceFlags m_sysDevFlag;
    QSet<QString> m_devices;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(GraphicsDeviceInfo::DeviceFlags)

#endif
