
#include "resolutionwidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QTimer>

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

    QTimer::singleShot(1, this, &ResolutionWidget::checkCondition);

    setLayout(centralLayout);
    setFixedHeight(70);
    setChecked(false);
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

void ResolutionWidget::checkCondition()
{
    const QString &script = m_resolution.conditionScript();
    if (script.isEmpty())
        return;

    QProcess *proc = new QProcess;
    QPROCESS_DUMP(proc);
    QPROCESS_DELETE_SELF(proc);

    connect(proc, static_cast<void (QProcess::*)(int)>(&QProcess::finished), [=] (const int exitCode) { setVisible(!exitCode); });

    EXECUTE_SCRIPT(proc, script);
}

void ResolutionWidget::prepareInstall()
{
    const QString script = m_resolution.prepareScript();
    Q_ASSERT(!script.isEmpty());

    QProcess *proc = new QProcess;
    QPROCESS_DUMP(proc);
    QPROCESS_DELETE_SELF(proc);

    connect(proc, static_cast<void (QProcess::*)(int)>(&QProcess::finished), this, &ResolutionWidget::onPrepareFinshed);

    EXECUTE_SCRIPT_ROOT(proc, script);
    proc->waitForFinished();
}

void ResolutionWidget::onPrepareFinshed()
{
    const QString& installScript = m_resolution.installScript();

    QProcess *proc = new QProcess;
    QPROCESS_DUMP(proc);
    QPROCESS_DELETE_SELF(proc);

    const QStringList args = { "cp",
                               "-f",
                               scriptAbsolutePath(installScript),
                               "/usr/bin/deepin-graphics-driver-installer.sh" };

    proc->start("pkexec", args);
}