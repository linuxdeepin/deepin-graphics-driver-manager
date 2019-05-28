
#include "resolution.h"

#include <QDebug>
#include <QApplication>

static const char *translates[] = {
    QT_TRANSLATE_NOOP("Resolution", "Use AMD card driver"),
    QT_TRANSLATE_NOOP("Resolution", "For special needs. Auto determine the acceleration way without external configuration."),

    QT_TRANSLATE_NOOP("Resolution", "NVIDIA Default Driver"),
    QT_TRANSLATE_NOOP("Resolution", "Use default driver"),
    QT_TRANSLATE_NOOP("Resolution", "NVIDIA driver is not detected in the current environment, and NVIDIA card will not work properly."),
    QT_TRANSLATE_NOOP("Resolution", "NVIDIA Open Source Driver"),
    QT_TRANSLATE_NOOP("Resolution", "Use open source driver"),
    QT_TRANSLATE_NOOP("Resolution", "Poor performance, unstable in some graphics cards."),
    QT_TRANSLATE_NOOP("Resolution", "NVIDIA Closed Source Driver"),
    QT_TRANSLATE_NOOP("Resolution", "Use closed source driver"),
    QT_TRANSLATE_NOOP("Resolution", "Only adaptive to parts of distributions and graphics cards, may cause black screen."),

    QT_TRANSLATE_NOOP("Resolution", "Intel Default Driver"),
    QT_TRANSLATE_NOOP("Resolution", "Use Intel default driver"),
    QT_TRANSLATE_NOOP("Resolution", "Poor compatibility for parts of old graphics cards."),
    QT_TRANSLATE_NOOP("Resolution", "Intel Compatible Mode"),
    QT_TRANSLATE_NOOP("Resolution", "Use Intel compatible mode"),
    QT_TRANSLATE_NOOP("Resolution", "Best compatibility for all Intel graphics cards to accelerate, but performance is relatively weak."),
    QT_TRANSLATE_NOOP("Resolution", "Intel Acceleration Mode"),
    QT_TRANSLATE_NOOP("Resolution", "Use Intel acceleration mode"),
    QT_TRANSLATE_NOOP("Resolution", "Speed up the graphics card with better performance, but unstable in some cards with screen flashing."),

    QT_TRANSLATE_NOOP("Resolution", "Use Bumblebee Solution"),
    QT_TRANSLATE_NOOP("Resolution", "Bumblebee Solution"),
    QT_TRANSLATE_NOOP("Resolution", "Save power to reduce power consumption.")

    QT_TRANSLATE_NOOP("Resolution", "Use NV-PRIME Solution"),
    QT_TRANSLATE_NOOP("Resolution", "PRIME Solution"),
    QT_TRANSLATE_NOOP("Resolution", "For laptops with hybrid graphics, having best performance but must work with closed-source driver.")
};

Resolution::Resolution(const QJsonObject &info)
{
    Q_UNUSED(translates)

    QJsonValue shouldDisplay = info.value("display");
    if (shouldDisplay == QJsonValue::Undefined) {
        m_display = true;
    } else {
        m_display = shouldDisplay.toBool();
    }

    m_resId = info["id"].toInt();
    m_keepGLTest = info["keep_gltest"].toBool();
    m_resName = QApplication::translate("Resolution", info["name"].toString().toStdString().c_str());
    m_resTitle = QApplication::translate("Resolution", info["title"].toString().toStdString().c_str());
    m_resDescription = QApplication::translate("Resolution", info["description"].toString().toStdString().c_str());
    m_prepareScript = info["prepare"].toString();
    m_installScript = info["install"].toString();
    m_removeScript = info["remove"].toString();
    m_conditionScript = info["condition"].toString();
    m_versionScript = info["version"].toString();
}
