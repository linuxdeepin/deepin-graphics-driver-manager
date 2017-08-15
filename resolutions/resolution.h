
#ifndef RESOLUTION_H_
#define RESOLUTION_H_

#include <QString>

class Resolution
{
public:
    explicit Resolution(const QString &name, const QString &description);

    bool operator==(const Resolution &rhs) const { return m_resName == rhs.m_resName; }

private:
    QString m_resName;
    QString m_resDescription;
};

#endif