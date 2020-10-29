#include "graphicsdriverinterface.h"
#include <QTime>
#include <QThread>

using namespace std;

GraphicsDriverInterface::GraphicsDriverInterface(QObject* parent)
    : QObject(parent)
{
    // TODO: Needs implementation
}

GraphicsDriverInterface::~GraphicsDriverInterface()
{
    // TODO: Needs implementation
}

int GraphicsDriverInterface::getDeviceFlag()
{
    return  m_devInfo.deviceFlag();
}

QStringList GraphicsDriverInterface::getDevice()
{
    QStringList devices;
    return  devices;
}


int GraphicsDriverInterface::getInstallStatus()
{
    return m_installStatus;
}

bool GraphicsDriverInterface::isTestSuccess()
{
    QTime now = QTime::currentTime();
    return now.minute() % 2 == 0;
}

QString GraphicsDriverInterface::newDriver()
{
    return QString("new driver");
}

QString GraphicsDriverInterface::OldDriver()
{
    return QString("old driver");
}

void GraphicsDriverInterface::reboot()
{

}

void GraphicsDriverInterface::removeDriver()
{
    QThread::sleep(5);
    emit removeDriverResult(0);
}

void GraphicsDriverInterface::installDriver()
{
    QThread::sleep(5);
    emit installDriverResult(0);
}


