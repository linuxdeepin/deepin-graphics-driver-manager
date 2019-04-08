
#include "mainwindow.h"
#include "environments.h"

#include <DApplication>
#include <DLog>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);
    if (!app.setSingleInstance("dgradvrmgr"))
        return -1;
    app.loadTranslator();
//    app.loadTranslator(QList<QLocale>() << QLocale("zh_CN"));
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    app.setOrganizationName("deepin");
    app.setApplicationName("deepin-graphics-driver-manager");
    app.setApplicationVersion(VERSION);
    app.setApplicationAcknowledgementPage("https://www.deepin.org/acknowledgments/deepin-graphics-driver-manager/");
    app.setProductIcon(QIcon(":/resources/icons/deepin-graphics-driver-manager-64px.svg"));
    app.setProductName(QApplication::translate("main", "Deepin Graphics Driver Manager"));
    app.setApplicationDescription(QApplication::translate("main", "Deepin Graphics Driver Manager is a compact and easy to use graphics driver management tool. It includes graphics card hardware detection, graphics driver installation, graphics driver solution switching,  graphics driver automatic recovery and other functions."));

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    MainWindow w;
    w.show();

   return app.exec();
}
