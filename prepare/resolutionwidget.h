
#ifndef RESOLUTIONWIDGET_H_
#define RESOLUTIONWIDGET_H_

#include "resolution.h"
#include "graphicsdriverproxy.h"


#include <QFrame>
#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QLineEdit>
#include <QTimer>

#define TEST_UI

class QLabel;
class ResolutionWidget: public QFrame
{
    Q_OBJECT

public:
    explicit ResolutionWidget(ComDeepinDaemonGraphicsDriverInterface *graphicsDriver, const Resolution &resolution,  QWidget *parent = nullptr);
    virtual ~ResolutionWidget();
    virtual void initUI();
    virtual void setChecked(const bool checked);
    virtual void prepareInstall();
    virtual bool checked() const { return m_checked; }
    virtual const Resolution resolution() const { return m_resolution; }
    virtual bool canUpdate();


signals:
    void clicked() const;
    void policyKitPassed(const QString &state);
    void prepareFinished(const bool success);


protected:
    void mouseReleaseEvent(QMouseEvent *e);

protected:
    ComDeepinDaemonGraphicsDriverInterface *m_graphicsDriver;
    Resolution m_resolution;
    bool m_checked = false;
    bool m_using;
    QLabel *m_checkedBtn;
    QLabel *m_title;
    QLabel *m_version;
    QLabel *m_description;
#ifdef TEST_UI
private slots:
    void onTimeout();
private:
    QTimer m_timer;
    int m_process;
#endif

};

#endif
