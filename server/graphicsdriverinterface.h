#ifndef GRAPHICSDRIVERINTERFACE_H_
#define GRAPHICSDRIVERINTERFACE_H_

#include <QObject>
#include "graphicsdeviceinfo.h"



class GraphicsDriverInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.daemon.GraphicsDriver")
public:
    enum GraphicsDriverInstallStatus
    {
        prepare = 0,
        install,
        gltest
    };
    explicit GraphicsDriverInterface(QObject* parent  = nullptr);
    virtual ~GraphicsDriverInterface();
public slots:
    int getDeviceFlag();
    QStringList getDevice();
    int getInstallStatus();
    bool isTestSuccess();
    QString newDriver();
    QString OldDriver();
    void reboot();
    void removeDriver();
    void installDriver();
Q_SIGNALS:
    void removeDriverResult(int);
    void installDriverResult(int);
private:
    GraphicsDeviceInfo m_devInfo;
    GraphicsDriverInstallStatus m_installStatus = prepare;
};

#endif
