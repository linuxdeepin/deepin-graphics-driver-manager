#ifndef RESOLUTION_H
#define RESOLUTION_H

#include <QJsonObject>
#include <QString>
#include <QList>

class Resolution
{
public:
    explicit Resolution(const QJsonObject &info)
    {
        m_name = info["name"].toString();
        m_title = info["title"].toString();
        m_driver = info["driver"].toString();
        m_description = info["description"].toString();
        m_currVersion = info["currVersion"].toString();
        m_repVersion = info["repVersion"].toString();
        m_enable = info["enable"].toBool();
    }

    inline const QString name() const { return m_name; }
    inline const QString title() const { return m_title; }
    inline const QString description() const { return m_description; }
    inline const QString driver() const { return m_driver; }
    inline const QString currVersion() const { return m_currVersion; }
    inline const QString repVersion() const { return m_repVersion; }
    inline bool enable() const { return m_enable; }
private:
    QString m_name;
    QString m_title;
    QString m_description;
    QString m_driver;
    QString m_currVersion;
    QString m_repVersion;
    bool m_enable = false;
};



#endif
