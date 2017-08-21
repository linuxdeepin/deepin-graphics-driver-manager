
#ifndef RESOLUTIONWIDGET_H_
#define RESOLUTIONWIDGET_H_

#include "../resolutions/resolution.h"

#include <QWidget>

#include <dimagebutton.h>

class QLabel;
class ResolutionWidget: public QWidget
{
    Q_OBJECT

public:
    explicit ResolutionWidget(const Resolution &r, QWidget *parent = nullptr);

    bool checked() const { return m_checked; }
    void setChecked(const bool checked);

private:
    bool m_checked;
    Dtk::Widget::DImageButton *m_checkedBtn;
    QLabel *m_title;
    QLabel *m_description;
};

#endif