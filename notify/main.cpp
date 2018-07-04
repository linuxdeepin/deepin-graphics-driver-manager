
#include <DApplication>
#include <DLog>
#include <ddialog.h>

#include <QSettings>
#include <QDebug>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

#define CONFIG "/usr/lib/deepin-graphics-driver-manager/config.conf"
#define DESKTOP_FILE "/.config/autostart/deepin-gradvrmgr-notify.desktop"

QSettings *SETTINGS = nullptr;

DDialog *dialog(const QString &message, const QString &iconName)
{
    const auto ratio = qApp->devicePixelRatio();
    QPixmap iconPix = QIcon::fromTheme(iconName).pixmap(QSize(64, 64) * ratio);
    iconPix.setDevicePixelRatio(ratio);

    DDialog *d = new DDialog;
    d->setMessage(message);
    d->setIcon(iconPix);
    return d;
}

void show_success_dialog()
{
    const QString &new_driver = SETTINGS->value("new_driver").toString();
    const QString &message = qApp->translate("main", "Congratulations, you have switched to %1.");

    DDialog *d = dialog(message.arg(new_driver), "deepin-graphics-driver-manager");
    d->addButton(qApp->translate("main", "Confirm"));
    d->exec();
}

void show_fail_dialog()
{
    const QString &old_driver = SETTINGS->value("old_driver").toString();
    const QString &new_driver = SETTINGS->value("new_driver").toString();
    const QString &message = qApp->translate("main", "Auto restore to %2 after failed to switch to %1");

    DDialog *d = dialog(message.arg(new_driver).arg(old_driver), "dialog-warning");
    d->addButton(qApp->translate("main", "Feedback"));

    QObject::connect(d, &DDialog::buttonClicked, [=] { QProcess::startDetached("deepin-feedback"); });

    d->exec();
}

void mark()
{
    QFile f(CONFIG);
    f.open(QIODevice::ReadWrite);

    QString buf = f.readAll();

    QRegularExpression re("^(notified=).*$");
    re.setPatternOptions(QRegularExpression::MultilineOption);
    buf.replace(re, "\\1true");

    f.seek(0);
    f.write(buf.toStdString().c_str());
    f.close();
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
    SETTINGS = new QSettings(CONFIG, QSettings::IniFormat);
    SETTINGS->setIniCodec(QTextCodec::codecForName("UTF-8"));

    if (!QFile(CONFIG).exists())
        return qApp->quit();

#ifndef QT_DEBUG
    const bool notified = SETTINGS->value("notified", true).toBool();
    if (notified)
        return qApp->quit();
#endif

    const bool succeed = SETTINGS->value("success").toBool();
    if (succeed)
        show_success_dialog();
    else
        show_fail_dialog();

    mark();

    removeDesktopFile();
}

int main(int argc, char *args[])
{
    DApplication dapp(argc, args);
    dapp.setQuitOnLastWindowClosed(true);

    QTranslator translator;
    translator.load(QString("/usr/share/deepin-graphics-driver-manager/translations/deepin-dgradrimgr-notify_%1.qm").arg(QLocale::system().name()));
    dapp.installTranslator(&translator);

    DLogManager::registerConsoleAppender();

    QTimer::singleShot(1, nullptr, init);

    return dapp.exec();
}

#include "main.moc"
