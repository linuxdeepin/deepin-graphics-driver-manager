
#ifndef RESOLUTIONWIDGET_H_
#define RESOLUTIONWIDGET_H_

#include "../resolutions/resolution.h"

#include <QWidget>

class QLabel;
class ResolutionWidget: public QWidget
{
public:
    explicit ResolutionWidget(const Resolution &r, QWidget *parent = nullptr);

private:
    QLabel *m_title;
    QLabel *m_description;
};

#endif