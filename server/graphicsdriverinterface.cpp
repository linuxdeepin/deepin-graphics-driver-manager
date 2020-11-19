#include "graphicsdriverinterface.h"
#include <QTime>
#include <QThread>
#include <QJsonArray>
#include <QJsonObject>

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

QJsonArray GraphicsDriverInterface::GetDevice()
{
    QJsonArray jsonArray;
    foreach (const Device device, m_devInfo.devices()){
        QJsonObject object;
        object.insert("name", device.m_name);
        object.insert("info", device.m_info);
        jsonArray.append(object);
    }
    return jsonArray;
}

QString GraphicsDriverInterface::GetResolutionTitle()
{
    return QString();
}

void GraphicsDriverInterface::PrepareInstall(int resolutionId)
{

}

void GraphicsDriverInterface::TestInstall()
{

}

bool GraphicsDriverInterface::IsTestSuccess()
{
    return true;
}

void GraphicsDriverInterface::RealInstaller()
{

}

QString GraphicsDriverInterface::GetNewDriverName()
{
    return QString("new driver");
}

QString GraphicsDriverInterface::GetOldDriverName()
{
    return QString("old driver");
}

void GraphicsDriverInterface::BuildResolution()
{

}

void GraphicsDriverInterface::InstallDriver(int resolutionId)
{

}

void GraphicsDriverInterface::RemoveDriver(int resolutionId)
{

}




