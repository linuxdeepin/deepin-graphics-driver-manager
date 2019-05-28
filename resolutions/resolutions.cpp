
#include "resolutions.h"

#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

Resolutions::Resolutions()
{

}

ResolutionsBuilder::ResolutionsBuilder(const GraphicsDeviceInfo &devInfo) :
#ifdef QT_DEBUG
    m_config("resources/config/resolutions/resolutions.json"),
#else
    m_config("/usr/lib/deepin-graphics-driver-manager/resolutions.json"),
#endif
    m_devInfo(devInfo)
{

}

ResolutionsBuilder ResolutionsBuilder::config(const QString &config)
{
    m_config = config;

    return *this;
}

Resolutions ResolutionsBuilder::build()
{
    QFile config(m_config);
    const bool b = config.open(QIODevice::ReadOnly);
    Q_ASSERT_X(b, Q_FUNC_INFO, "read config file error");

    const int deviceType = m_devInfo.deviceFlag();
    const auto doc = QJsonDocument::fromJson(config.readAll());

    QJsonObject resolutionsObject;
    for (const auto res : doc.array())
    {
        const auto object = res.toObject();
        const int type = object["type"].toInt();

        if (type != deviceType)
            continue;

        resolutionsObject = object;
        break;
    }

    Resolutions r;
    r.m_name = resolutionsObject["name"].toString();
    r.m_iconName = resolutionsObject["icon_name"].toString();
    r.m_statusScript = resolutionsObject["status"].toString();

    for (const auto res : resolutionsObject["resolutions"].toArray()) {
        Resolution solution(res.toObject());
        if (solution.display()) {
            r.m_resolutions.append(solution);
        }
    }

    return r;
}
