
#include "mainwindow.h"

#include <DApplication>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication app(argc, argv);

    MainWindow w;
    w.show();

   return app.exec();
}