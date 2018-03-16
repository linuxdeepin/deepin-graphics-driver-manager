
#ifndef RESOLUTIONWIDGET_H_
#define RESOLUTIONWIDGET_H_

#include "../resolutions/resolution.h"

#include <QFrame>
#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QLineEdit>

inline const QString scriptAbsolutePath(const QString &scriptName)
{
#ifdef QT_DEBUG
    return QDir::currentPath() + "/scripts/" + scriptName;
#else
    return "/usr/lib/deepin-graphics-driver-manager/" + scriptName;
#endif
}

// #ifdef QT_DEBUG
#define QPROCESS_DUMP(Process) \
    connect(Process, &QProcess::readyReadStandardOutput, Process, [=] { qDebug().noquote() << proc->readAllStandardOutput(); }); \
    connect(Process, &QProcess::readyReadStandardError, Process, [=] { qWarning().noquote() << proc->readAllStandardError(); });
// #else
// #define QPROCESS_DUMP(Process) Q_UNUSED(Process)
// #endif

#define QPROCESS_DELETE_SELF(Process) \
    connect(Process, static_cast<void (QProcess::*)(int)>(&QProcess::finished), Process, &QProcess::deleteLater);

#define EXECUTE_SCRIPT(Process, Script) \
    Process->start("/bin/bash", QStringList() << "-x" << scriptAbsolutePath(Script));

#define EXECUTE_SCRIPT_ROOT(Process, Script) \
    Process->start("pkexec", QStringList() << "/bin/bash" << "-x" << scriptAbsolutePath(Script));

class QLabel;
class ResolutionWidget: public QFrame
{
    Q_OBJECT

public:
    explicit ResolutionWidget(const Resolution &r, QWidget *parent = nullptr);

    void setChecked(const bool checked);
    void prepareInstall(const Resolution &old_resolution);
    bool checked() const { return m_checked; }
    const Resolution resolution() const { return m_resolution; }

signals:
    void clicked() const;
    void policyKitPassed();
    void prepareFinished(const int exitCode);

protected:
    void mouseReleaseEvent(QMouseEvent *e);

private slots:
    void checkCondition();
    void checkVersion();

private:
    bool m_checked;
    bool m_using;
    Resolution m_resolution;
    QLabel *m_checkedBtn;
    QLabel *m_title;
    QLineEdit *m_version;
    QLabel *m_description;
};

#endif
