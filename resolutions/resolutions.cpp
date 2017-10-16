
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
    m_config("/usr/lib/deepin-graphics-driver-manager/resolutions.json"),
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
    // Q_ASSERT_X(config.open(QIODevice::ReadOnly), Q_FUNC_INFO, "read config file error");
    qDebug() << "read config file: " << config.open(QIODevice::ReadOnly);

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

    for (const auto res : resolutionsObject["resolutions"].toArray())
        r.m_resolutions.append(Resolution(res.toObject()));

    return r;
}