
#ifndef GRAPHICSDEVICEINFO_H
#define GRAPHICSDEVICEINFO_H

#include <QSet>

class Device
{
public:
    Device(const QString &name, const QString &info)
    {
        m_name = name;
        m_info = info;
    }
    QString m_name;
    QString m_info;
};

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
    int curDeviceFlag() const { return m_curDevFlag; }
    int deviceNums() const;
    const QList<Device> devices() const { return m_devices; }
    bool isNotebook();
    QString curDriver();
    static GraphicsDeviceInfo::DeviceFlag deviceType(const QString &devInfo);
private:
    void init();

private:
    DeviceFlags m_sysDevFlag;
    DeviceFlags m_curDevFlag;
    QList<Device> m_devices;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(GraphicsDeviceInfo::DeviceFlags)

#endif
