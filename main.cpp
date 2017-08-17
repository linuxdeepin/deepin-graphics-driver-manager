
#include "mainwindow.h"

#include <DApplication>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);
    app.loadTranslator();
    // app.loadTranslator(QList<QLocale>() << QLocale("zh_CN"));
    app.setTheme("light");

    MainWindow w;
    w.show();

   return app.exec();
}