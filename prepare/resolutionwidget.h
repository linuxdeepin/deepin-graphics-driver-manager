
#ifndef RESOLUTIONWIDGET_H_
#define RESOLUTIONWIDGET_H_

#include "resolution.h"
#include "graphicsdriverproxy.h"


#include <QFrame>
#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QLineEdit>


class QLabel;
class ResolutionWidget: public QFrame
{
    Q_OBJECT

public:
    explicit ResolutionWidget(ComDeepinDaemonGraphicsDriverInterface *graphicsDriver, Resolution &resolution,  QWidget *parent = nullptr);

    void setChecked(const bool checked);
    void prepareInstall(const Resolution &old_resolution);
    bool checked() const { return m_checked; }
    const Resolution resolution() const { return m_resolution; }
    bool canUpdate();


signals:
    void clicked() const;
    void policyKitPassed();
    void prepareFinished(const int exitCode);



protected:
    void mouseReleaseEvent(QMouseEvent *e);

private slots:
    void checkVersion();

private:
    ComDeepinDaemonGraphicsDriverInterface *m_graphicsDriver;
    Resolution m_resolution;
    bool m_checked;
    bool m_using;
    QLabel *m_checkedBtn;
    QLabel *m_title;
    QLabel *m_version;
    QLabel *m_description;
};

#endif
