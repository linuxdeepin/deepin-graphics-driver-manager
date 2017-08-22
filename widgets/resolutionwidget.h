
#ifndef RESOLUTIONWIDGET_H_
#define RESOLUTIONWIDGET_H_

#include "../resolutions/resolution.h"

#include <QWidget>

class QLabel;
class ResolutionWidget: public QWidget
{
    Q_OBJECT

public:
    explicit ResolutionWidget(const Resolution &r, QWidget *parent = nullptr);

    bool running() const { return m_running; }
    bool checked() const { return m_checked; }
    void setChecked(const bool checked);

signals:
    void clicked() const;

protected:
    void mouseReleaseEvent(QMouseEvent *e);

private slots:
    void checkInstallStat(const QString &script);

private:
    bool m_running;
    bool m_checked;
    QLabel *m_checkedBtn;
    QLabel *m_title;
    QLabel *m_description;
};

#endif