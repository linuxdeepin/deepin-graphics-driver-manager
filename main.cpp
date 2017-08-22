
#include "mainwindow.h"

#include <DApplication>
#include <DLog>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);
    app.setOrganizationName("deepin");
    app.setApplicationName("deepin-graphics-driver-manager");
    app.setApplicationVersion("1.0");
    app.setApplicationAcknowledgementPage("https://www.deepin.org/acknowledgments/");
    app.loadTranslator();
    app.setProductName(QApplication::translate("main", "Deepin Graphics Driver Manager"));
    app.setApplicationDescription(QApplication::translate("main", "Deepin Graphics Driver Manager"));
    // app.loadTranslator(QList<QLocale>() << QLocale("zh_CN"));
    app.setTheme("light");

    DLogManager::registerConsoleAppender();

    MainWindow w;
    w.show();

   return app.exec();
}