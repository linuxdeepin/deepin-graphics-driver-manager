
#include "environments.h"
#include "mainwindow.h"

#include <QTranslator>
#include <DApplication>
#include <QLocale>
#include <DLog>



DCORE_USE_NAMESPACE

QString getUserName()
{
    QString userPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    return userPath.section("/", -1, -1);
}

int main(int argc, char* argv[])
{
    DApplication app(argc, argv);
    app.loadTranslator();
    QString loc = QLocale::system().name();

    QString loc_tr_file = QString(TRANSLATIONS_DIR"/deepin-graphics-driver-manager_%1.qm");

    QTranslator trans;
    trans.load(loc_tr_file.arg(loc));
    app.installTranslator(&trans);

    if (!app.setSingleInstance("dgradvrmgr"))
        return -1;


    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    app.setOrganizationName("deepin");
    //app.setApplicationName("deepin-graphics-driver-manager");
    app.setApplicationName(" ");
    app.setApplicationVersion(VERSION);
    app.setApplicationAcknowledgementPage("https://www.deepin.org/acknowledgments/deepin-graphics-driver-manager/");
    app.setProductIcon(QIcon(":/resources/icons/deepin-graphics-driver-manager-64px.svg"));
    app.setProductName(QApplication::translate("main", "Graphics Driver Manager"));
    app.setApplicationDescription(QApplication::translate("main", "Graphics Driver Manager is a compact and easy to use graphics driver management tool. It includes graphics card hardware detection, graphics driver installation, graphics driver solution switching,  graphics driver automatic recovery and other functions."));

    //设置日志
    const QString logFormat = "[%{time}{yyyy-MM-dd, HH:mm:ss.zzz}] [%{type:1}] [%{file}=>%{function}: %{line}] %{message}\n";

    QString log_file(QString("/home/%1/.cache/deepin/deepin-graphics-driver-manager/").arg(getUserName()));
    QDir dir(log_file);
    if (!dir.exists()) {
        dir.mkpath(log_file);
    }
    log_file += "deepin-graphics-driver-manager.log";
    ConsoleAppender *consoleAppender = new ConsoleAppender;
    consoleAppender->setFormat(logFormat);
    RollingFileAppender *rollingFileAppender = new RollingFileAppender(log_file);
    rollingFileAppender->setFormat(logFormat);
    rollingFileAppender->setLogFilesLimit(5);
    rollingFileAppender->setDatePattern(RollingFileAppender::MinutelyRollover);

    logger->registerAppender(consoleAppender);
    logger->registerAppender(rollingFileAppender);
    qInfo() << "VERSION: " << VERSION;
    MainWindow w;
    w.show();

    return app.exec();
}
