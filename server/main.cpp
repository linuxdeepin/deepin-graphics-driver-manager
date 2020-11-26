#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusError>
#include <DLog>
#include "graphicsdriverinterface.h"
#include "graphicsdriveradaptor.h"
#include "LogManager.h"

DCORE_USE_NAMESPACE

const QString GraphicMangerServiceName = "com.deepin.graphicmanger";
const QString GraphicMangerPath = "/com/deepin/graphicmanger";

int	main(int argc, char **argv)
{
    QString PATH = qgetenv("PATH");

    if (PATH.isEmpty()) {
        PATH = "/usr/bin";
    }
    PATH += ":/usr/sbin";
    PATH += ":/sbin";
    qputenv("PATH", PATH.toLatin1());

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QCoreApplication app(argc, argv);
    app.setOrganizationName("deepin");
    app.setApplicationName("deepin-graphics-driver-manager-server");

    const QString m_format = "%{time}{yyyyMMdd.HH:mm:ss.zzz}[%{type:1}][%{function:-35} %{line:-4} %{threadid} ] %{message}\n";
    DRMLogManager::setSystemLog(true);
    DRMLogManager::setLogFormat(m_format);
    DRMLogManager::registerFileAppender();
    DRMLogManager::registerConsoleAppender();

    qDebug() << "write log to" << DRMLogManager::getlogFilePath();
    qDebug() << PATH;

    QDBusConnection connection = QDBusConnection::systemBus();
    
    GraphicsDriverInterface interface;
    GraphicsDriverAdaptor adaptor(&interface);

    if (!connection.registerService(GraphicMangerServiceName))
    {
        qDebug() << connection.lastError().message();
        exit(1);
    }

    connection.registerObject(GraphicMangerPath, &interface, QDBusConnection::ExportAllContents);

    return app.exec();
}









