
#include "mainwindow.h"
#include "environments.h"
#include <QTranslator>
#include <DApplication>
#include <QLocale>

#include <DLog>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);

    QString loc = QLocale::system().name();
    if (argc == 2) {
        loc = QString(argv[1]);
    }
    QString loc_tr_file = QString("/usr/share/deepin-graphics-driver-manager/translations/deepin-graphics-driver-manager_%1.qm");
    QTranslator trans;
    trans.load(loc_tr_file.arg(loc));
    app.installTranslator(&trans);


    if (!app.setSingleInstance("dgradvrmgr"))
        return -1;
    app.loadTranslator();
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    app.setOrganizationName("deepin");
    app.setApplicationName(" ");
    app.setApplicationVersion(VERSION);
    app.setApplicationAcknowledgementPage("https://www.deepin.org/acknowledgments/deepin-graphics-driver-manager/");
    app.setProductIcon(QIcon(":/resources/icons/deepin-graphics-driver-manager-64px.svg"));
    app.setProductName(QApplication::translate("main", "Graphics Driver Manager"));
    app.setApplicationDescription(QApplication::translate("main", "Graphics Driver Manager is a compact and easy to use graphics driver management tool. It includes graphics card hardware detection, graphics driver installation, graphics driver solution switching,  graphics driver automatic recovery and other functions."));

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    MainWindow w;
    w.show();

   return app.exec();
}
