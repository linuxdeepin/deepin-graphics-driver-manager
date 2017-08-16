
#ifndef RESOLUTIONS_H_
#define RESOLUTIONS_H_

#include "resolution.h"
#include "../graphicsdeviceinfo.h"

#include <QList>
#include <QObject>

class Resolutions
{
    friend class ResolutionsBuilder;

public:
    const QString name() const { return m_name; }
    const QString iconName() const { return m_iconName; }
    const QString description() const { return m_description; }

    const QList<Resolution> resolutions() const { return m_resolutions; };

private:
    explicit Resolutions();

private:
    QString m_name;
    QString m_iconName;
    QString m_description;
    QList<Resolution> m_resolutions;
};

class ResolutionsBuilder
{
public:
    static Resolutions build(const GraphicsDeviceInfo &devInfo);
};

#endif