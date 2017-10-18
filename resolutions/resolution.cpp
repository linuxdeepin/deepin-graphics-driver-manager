
#include "resolution.h"

#include <QDebug>
#include <QApplication>

Resolution::Resolution(const QJsonObject &info)
{
    m_resId = info["id"].toInt();
    m_resName = QApplication::translate("Resolution", info["name"].toString().toStdString().c_str());
    m_resDescription = QApplication::translate("Resolution", info["description"].toString().toStdString().c_str());
    m_prepareScript = info["prepare"].toString();
    m_installScript = info["install"].toString();
    m_conditionScript = info["condition"].toString();
}