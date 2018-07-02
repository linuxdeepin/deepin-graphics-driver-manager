
#include "resolution.h"

#include <QDebug>
#include <QApplication>

static const char *translates[] = {
    QT_TRANSLATE_NOOP("Resolution", "Use AMD card driver"),
    QT_TRANSLATE_NOOP("Resolution", "For special needs. Auto determine the acceleration way without external configuration."),

    QT_TRANSLATE_NOOP("Resolution", "NVIDIA Default Driver"),
    QT_TRANSLATE_NOOP("Resolution", "Use NVIDIA default driver"),
    QT_TRANSLATE_NOOP("Resolution", "NVIDIA card driver is not detected in the current environment, and will be in gray"),
    QT_TRANSLATE_NOOP("Resolution", "NVIDIA Open Source Driver"),
    QT_TRANSLATE_NOOP("Resolution", "Use NVIDIA open source driver"),
    QT_TRANSLATE_NOOP("Resolution", "Unstable in some graphics cards with weaker performance."),
    QT_TRANSLATE_NOOP("Resolution", "NVIDIA Closed Source Driver"),
    QT_TRANSLATE_NOOP("Resolution", "Use NVIDIA closed source driver"),
    QT_TRANSLATE_NOOP("Resolution", "Only adaptive to parts of distributions and graphics cards, may occur black screen"),

    QT_TRANSLATE_NOOP("Resolution", "Intel Default Driver"),
    QT_TRANSLATE_NOOP("Resolution", "Use Intel default driver"),
    QT_TRANSLATE_NOOP("Resolution", "Uses glamor as the default acceleration way with low compatibility on old graphics cards."),
    QT_TRANSLATE_NOOP("Resolution", "Intel Compatible Mode"),
    QT_TRANSLATE_NOOP("Resolution", "Use Intel compatible mode"),
    QT_TRANSLATE_NOOP("Resolution", "Speed up the graphics card with better performance, but unstable with screen splash after tested."),
    QT_TRANSLATE_NOOP("Resolution", "Intel Acceleration Mode"),
    QT_TRANSLATE_NOOP("Resolution", "Use Intel acceleration mode"),
    QT_TRANSLATE_NOOP("Resolution", "A more ancient and mature acceleration way with the best compatibility and weaker performance."),

    QT_TRANSLATE_NOOP("Resolution", "Use Bumblebee Solution"),
    QT_TRANSLATE_NOOP("Resolution", "Bumblebee Solution"),
    QT_TRANSLATE_NOOP("Resolution", "Power saving to improve the ability to hold a charge")
};

Resolution::Resolution(const QJsonObject &info)
{
    Q_UNUSED(translates)

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
