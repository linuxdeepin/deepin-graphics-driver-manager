
#include <DApplication>
#include <DLog>
#include <ddialog.h>

#include <QSettings>
#include <QDebug>
#include <QDBusConnectionInterface>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

#define CONFIG "/usr/lib/deepin-graphics-driver-manager/working-dir/config.conf"
#define DESKTOP_FILE "/.config/autostart/deepin-gradvrmgr-installer.desktop"
#define AuthAgentDbusService "com.deepin.Polkit1AuthAgent"

QSettings *SETTINGS = nullptr;
QDBusConnectionInterface *DbusConnInter = nullptr;

int WaitingAuthAgentTimes = 0;

DDialog *dialog(const QString &message, const QString &iconName)
{
    const auto ratio = qApp->devicePixelRatio();
    QPixmap iconPix = QIcon::fromTheme(iconName).pixmap(QSize(64, 64) * ratio);
    iconPix.setDevicePixelRatio(ratio);

    DDialog *d = new DDialog;
    d->setMessage(message);
    d->setIcon(iconPix, QSize(64, 64));
    return d;
}

void show_success_dialog()
{
    const QString &new_driver = SETTINGS->value("new_driver").toString();
    const QString &message = qApp->translate("main", "Congratulations, you have switched to %1, please reboot to take effect.");

    DDialog *d = dialog(message.arg(new_driver), "://resources/icons/deepin-graphics-driver-manager-success.svg");
    d->addButton(qApp->translate("main", "Cancel"));
    d->addButton(qApp->translate("main", "Reboot"));
    d->setDefaultButton(1);

    QObject::connect(d, &DDialog::buttonClicked, [=](int index, const QString &text) {
        Q_UNUSED(text);
        if (index == 1) {
            QProcess::startDetached("dbus-send --print-reply --dest=com.deepin.dde.shutdownFront /com/deepin/dde/shutdownFront com.deepin.dde.shutdownFront.Restart");
        }
    });

    d->exec();
}

void show_fail_dialog()
{
    const QString &old_driver = SETTINGS->value("old_driver").toString();
    const QString &new_driver = SETTINGS->value("new_driver").toString();
    const QString &message = qApp->translate("main", "Auto restore to %2 after failed to switch to %1");

    DDialog *d = dialog(message.arg(new_driver).arg(old_driver), "dialog-warning");
    d->addButton(qApp->translate("main", "Submit Feedback"));

    QObject::connect(d, &DDialog::buttonClicked, [=] { QProcess::startDetached("deepin-feedback"); });

    d->exec();
}

int show_install_dialog() {
    DDialog *installDialog = dialog(qApp->translate("main", "Updating the driver, please wait..."), "://resources/icons/deepin-graphics-driver-manager-installing.svg");

    QProcess *removeProc = new QProcess;
    removeProc->connect(removeProc, static_cast<void (QProcess::*)(int)>(&QProcess::finished), removeProc, &QProcess::deleteLater);
    removeProc->connect(removeProc, static_cast<void (QProcess::*)(int)>(&QProcess::finished), [=](int exitCode) {
        qDebug() << "remove process exitCode" << exitCode;
        if (exitCode != 0) {
            installDialog->done(exitCode);
            return;
        }

        QProcess *installProc = new QProcess;
        installProc->connect(installProc, static_cast<void (QProcess::*)(int)>(&QProcess::finished), installProc, &QProcess::deleteLater);
        installProc->connect(installProc, static_cast<void (QProcess::*)(int)>(&QProcess::finished), installDialog, &DDialog::done);
        QStringList installArgs {"/bin/bash", "-x", "/usr/lib/deepin-graphics-driver-manager/working-dir/install_new.sh"};
        qDebug() << "start install process";
        installProc->start("pkexec", installArgs);
    });
    QStringList removeArgs {"/bin/bash", "-x", "/usr/lib/deepin-graphics-driver-manager/working-dir/remove_old.sh"};

    //if (DbusConnInter->isServiceRegistered(AuthAgentDbusService)) {
    //    qDebug() << "start remove process";
    //    removeProc->start("pkexec", removeArgs);
    //} else {
    //    qDebug() << AuthAgentDbusService << "daemon has not started, waiting for signal";
        QTimer *timer = new QTimer;
        timer->setSingleShot(false);
        timer->setInterval(6000);
        QObject::connect(timer, &QTimer::timeout, [=] {
            if (DbusConnInter->isServiceRegistered(AuthAgentDbusService)) {
                timer->stop();
                timer->deleteLater();
                WaitingAuthAgentTimes = 0;
                qDebug() << "start remove process";
                removeProc->start("pkexec", removeArgs);
                return;
            }
            WaitingAuthAgentTimes++;
            qDebug() << "Waiting for AuthAgent service" << WaitingAuthAgentTimes << "times";
            if (WaitingAuthAgentTimes > 10) {
                qDebug() << "AuthAgent service timeout...";
                timer->stop();
                timer->deleteLater();
                installDialog->done(-1);
            }
        });
        timer->start();
    //}

    return installDialog->exec();
}

void removeDesktopFile()
{
    QFile desktopFile(QDir::homePath() + DESKTOP_FILE);
    if (desktopFile.exists())
        desktopFile.remove();
    else
        qDebug() << desktopFile.fileName() << "do not exists!";
}

void init()
{
    if (!QFile(CONFIG).exists())
        return qApp->quit();

    SETTINGS = new QSettings(CONFIG, QSettings::IniFormat);
    SETTINGS->setIniCodec(QTextCodec::codecForName("UTF-8"));

    DbusConnInter = QDBusConnection::sessionBus().interface();

    const bool gltestSuccess = SETTINGS->value("gltest-success", false).toBool();
    qDebug() << "gltestSuccess is:" << gltestSuccess;
    if (gltestSuccess) {
        const int exitCode = show_install_dialog();
        qDebug() << "show_install_dialog exitCode" << exitCode;
        if (exitCode == 0) {
            qDebug() << "show_success_dialog";
            show_success_dialog();
        } else {
            qDebug() << "show_fail_dialog1";
            show_fail_dialog();
        }
    } else {
        qDebug() << "show_fail_dialog2";
        show_fail_dialog();
    }
}

int main(int argc, char *args[])
{
    removeDesktopFile();

    DApplication dapp(argc, args);
    dapp.setQuitOnLastWindowClosed(true);
    dapp.setOrganizationName("deepin");
    dapp.setApplicationName("deepin-graphics-driver-manager-installer");

    QTranslator translator;
    translator.load(QString("/usr/share/deepin-graphics-driver-manager/translations/deepin-graphics-driver-manager_%1.qm").arg(QLocale::system().name()));
    dapp.installTranslator(&translator);

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    QTimer::singleShot(1, nullptr, init);

    return dapp.exec();
}

#include "main.moc"
