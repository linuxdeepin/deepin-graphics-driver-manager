
#ifndef RESOLUTION_H_
#define RESOLUTION_H_

#include <QString>
#include <QJsonObject>

class Resolution
{
public:
    explicit Resolution(const QJsonObject &info);

    inline int id() const { return m_resId; }
    inline const QString name() const { return m_resName; }
    inline const QString description() const { return m_resDescription; }
    inline const QString statusScript() const { return m_statusScript; }
    inline const QString prepareScript() const { return m_prepareScript; }

    inline bool operator==(const Resolution &rhs) const { return m_resId == rhs.m_resId; }

private:
    int m_resId;
    QString m_resName;
    QString m_resDescription;
    QString m_prepareScript;
    QString m_statusScript;
};

#endif