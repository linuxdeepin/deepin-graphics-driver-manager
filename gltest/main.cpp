
#include <QApplication>

#include <QSettings>
#include <QTranslator>

#include "gltestwidget.h"
#include "gltestwindow.h"

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    QApplication app(argc, argv);
    QSettings settings("/usr/lib/deepin-graphics-driver-manager/working-dir/config.conf", QSettings::IniFormat);
    const QString lang = settings.value("lang", "en_US").toString();
    QTranslator trans;
    trans.load(QString(TRANSLATIONS_DIR"/deepin-graphics-driver-manager_%1.qm").arg(lang));
    app.installTranslator(&trans);
    GLTestWindow *w = new GLTestWindow;
    w->show();

    return app.exec();

}


