
#ifndef RESOLUTIONINTERFACE_H_
#define RESOLUTIONINTERFACE_H_

#include "resolution.h"

#include <QList>

class ResolutionInterface
{
public:
    virtual ~ResolutionInterface() = 0;

    virtual QString name() = 0;
    virtual QString iconName() = 0;
    virtual QString description() = 0;

    const QList<Resolution> resolutions() const { return m_resolutions; };

protected:
    QList<Resolution> m_resolutions;
};

#endif