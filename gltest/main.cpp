
#include <QApplication>

#include <QSettings>
#include <QTranslator>

#include "gltestwidget.h"
#include "gltestwindow.h"

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    QApplication app(argc, argv);

    QTranslator trans;
    trans.load(QString(TRANSLATIONS_DIR"/deepin-graphics-driver-manager_%1.qm").arg(QLocale::system().name()));
    app.installTranslator(&trans);
    GLTestWindow *w = new GLTestWindow;
    w->show();

    return app.exec();

}


