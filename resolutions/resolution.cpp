
#include "resolution.h"

#include <QDebug>
#include <QApplication>

Resolution::Resolution(const QJsonObject &info)
{
    m_resId = info["id"].toInt();
    m_resName = QApplication::translate("Resolution", info["name"].toString().toStdString().c_str());
    m_resDescription = QApplication::translate("Resolution", info["description"].toString().toStdString().c_str());
    m_statusScript = info["status"].toString();
    m_prepareScript = info["prepare"].toString();
}