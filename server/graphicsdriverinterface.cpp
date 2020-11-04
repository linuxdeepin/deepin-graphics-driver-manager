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

QStringList GraphicsDriverInterface::GetDevice()
{
    return QStringList();
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




