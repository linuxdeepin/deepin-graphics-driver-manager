
#include "resolution.h"

#include <QDebug>

Resolution::Resolution(const QJsonObject &info)
{
    m_resId = info["id"].toInt();
    m_resName = info["name"].toString();
}