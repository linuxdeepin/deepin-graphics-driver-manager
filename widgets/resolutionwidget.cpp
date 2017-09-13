
#include "resolutionwidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QDebug>
#include <QProcess>
#include <QDir>

inline const QString scriptAbsolutePath(const QString &scriptName)
{
    return QDir::currentPath() + "/scripts/" + scriptName;
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
    Process->start("bash", QStringList() << "-x" << scriptAbsolutePath(Script));

#define EXECUTE_SCRIPT_ROOT(Process, Script) \
    Process->start("pkexec", QStringList() << "bash" << "-x" << scriptAbsolutePath(Script));

ResolutionWidget::ResolutionWidget(const Resolution &r, QWidget *parent) :
    QWidget(parent),

    m_resolution(r)
{
    m_checkedBtn = new QLabel;
    m_checkedBtn->setPixmap(QPixmap(":/resources/icons/select.png"));
    m_checkedBtn->setFixedSize(16, 16);

    m_title = new QLabel;
    m_title->setText(r.name());
    m_title->setStyleSheet("QLabel {"
                           "}");
    m_description = new QLabel;
    m_description->setText(r.description());
    m_description->setStyleSheet("QLabel {"
                                 "color: #888;"
                                 "}");

    QVBoxLayout *infoLayout = new QVBoxLayout;
    infoLayout->addWidget(m_title);
    infoLayout->addWidget(m_description);
    infoLayout->setSpacing(0);
    infoLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *centralLayout = new QHBoxLayout;
    centralLayout->addLayout(infoLayout);
    centralLayout->addStretch();
    centralLayout->addWidget(m_checkedBtn);
    centralLayout->setAlignment(m_checkedBtn, Qt::AlignCenter);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 0, 0, 0);

    QMetaObject::invokeMethod(this, "checkInstallStat", Qt::QueuedConnection, Q_ARG(QString, r.statusScript()));

    setLayout(centralLayout);
    setFixedHeight(70);
}

void ResolutionWidget::setChecked(const bool checked)
{
    m_checked = checked;
    m_checkedBtn->setVisible(checked);
}

void ResolutionWidget::mouseReleaseEvent(QMouseEvent *e)
{
    e->accept();

    emit clicked();
}

void ResolutionWidget::checkInstallStat(const QString &script)
{
    if (script.isEmpty())
        return;

    QProcess *proc = new QProcess;
    QPROCESS_DUMP(proc);
    QPROCESS_DELETE_SELF(proc);

    EXECUTE_SCRIPT(proc, script);
    proc->waitForFinished();
    const int r = proc->exitCode();

    if (!r)
    {
        m_running = true;
        emit clicked();
    }
}

void ResolutionWidget::prepareInstall()
{
    const QString script = m_resolution.prepareScript();
    Q_ASSERT(!script.isEmpty());

    QProcess *proc = new QProcess;
    QPROCESS_DUMP(proc);
    QPROCESS_DELETE_SELF(proc);

    EXECUTE_SCRIPT_ROOT(proc, script);
    proc->waitForFinished();
}