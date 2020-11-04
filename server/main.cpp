#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusError>
#include <DLog>
#include "graphicsdriverinterface.h"
#include "graphicsdriveradaptor.h"

DCORE_USE_NAMESPACE

int	main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();


    QDBusConnection connection = QDBusConnection::systemBus();
    
    GraphicsDriverInterface interface;
    GraphicsDriverAdaptor adaptor(&interface);

    if (!connection.registerService("com.deepin.daemon.GraphicsDriver"))
        {
            qDebug() << connection.lastError().message();
            exit(1);
        }

        connection.registerObject("/GraphicsDriver", &interface, QDBusConnection::ExportAllContents);

    return app.exec();
}









