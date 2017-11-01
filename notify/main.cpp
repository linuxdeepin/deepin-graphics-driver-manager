
#include <DApplication>
#include <DLog>
#include <ddialog.h>

#include <QSettings>
#include <QDebug>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

#define CONFIG "/usr/lib/deepin-graphics-driver-manager/config.conf"

QSettings SETTINGS(CONFIG, QSettings::IniFormat);

void show_dialog(const QString &message, const QString &iconName)
{
    DDialog d;
    d.setMessage(message);
    d.setIcon(QIcon::fromTheme(iconName).pixmap(QSize(64, 64)));
    d.addButton(QT_TRANSLATE_NOOP("main", "Ok"));
    d.exec();
}

void show_success_dialog()
{
    const QString &new_driver = SETTINGS.value("new_driver").toString();
    const QString &message = QT_TRANSLATE_NOOP("main", "Install %1 successful!");

    show_dialog(message.arg(new_driver), "deepin-graphics-driver-manager");
}

void show_fail_dialog()
{
    const QString &old_driver = SETTINGS.value("old_driver").toString();
    const QString &new_driver = SETTINGS.value("new_driver").toString();
    const QString &message = QT_TRANSLATE_NOOP("main", "Install %1 failed, fallback to %2");

    show_dialog(message.arg(old_driver).arg(new_driver), "dialog-warning");
}

void clear()
{
    QFile(CONFIG).remove();
}

int main(int argc, char *args[])
{
    DApplication dapp(argc, args);

    DLogManager::registerConsoleAppender();

    if (!QFile(CONFIG).exists())
        return 0;

    const bool succeed = SETTINGS.value("success").toBool();
    if (succeed)
        show_success_dialog();
    else
        show_fail_dialog();

    clear();

    return dapp.exec();
}
