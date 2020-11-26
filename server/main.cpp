#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusError>
#include <DLog>
#include "graphicsdriverinterface.h"
#include "graphicsdriveradaptor.h"

DCORE_USE_NAMESPACE

const QString GraphicMangerServiceName = "com.deepin.graphicmanger";
const QString GraphicMangerPath = "/com/deepin/graphicmanger";

int	main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();


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









