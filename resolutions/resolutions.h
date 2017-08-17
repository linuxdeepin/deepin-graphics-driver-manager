
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

    const QList<Resolution> resolutions() const { return m_resolutions; }

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
    explicit ResolutionsBuilder(const GraphicsDeviceInfo &devInfo);

    ResolutionsBuilder config(const QString &config);
    Resolutions build();

private:
    QString m_config;
    GraphicsDeviceInfo m_devInfo;
};

#endif