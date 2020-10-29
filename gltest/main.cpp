
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
    const bool exit_gltest = settings.value("exit_test", false).toBool();
    const QString lang = settings.value("lang", "en_US").toString();

    QTranslator translator;
    translator.load(QString("/usr/share/deepin-graphics-driver-manager/translations/deepin-graphics-driver-manager_%1.qm").arg(lang));
    app.installTranslator(&translator);

    GLTestWindow *w = new GLTestWindow;
    w->setExitGLTest(exit_gltest);
    w->show();

    return app.exec();
}


