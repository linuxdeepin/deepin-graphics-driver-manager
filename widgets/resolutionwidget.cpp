
#include "resolutionwidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QTimer>

ResolutionWidget::ResolutionWidget(const Resolution &r, QWidget *parent) :
    QFrame(parent),

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
    m_description->setWordWrap(true);
    m_description->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_description->setStyleSheet("QLabel {"
                                 "font-size: 12px;"
                                 "color: #888;"
                                 "}");

    QVBoxLayout *infoLayout = new QVBoxLayout;
    infoLayout->addWidget(m_title);
    infoLayout->addWidget(m_description);
    infoLayout->setSpacing(0);
    infoLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *centralLayout = new QHBoxLayout;
    centralLayout->addLayout(infoLayout);
    centralLayout->addWidget(m_checkedBtn);
    centralLayout->setAlignment(m_checkedBtn, Qt::AlignCenter);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 0, 0, 0);

    QTimer::singleShot(1, this, &ResolutionWidget::checkCondition);

    setLayout(centralLayout);
    setFixedHeight(70);
    setChecked(false);
    setObjectName("ResolutionWidget");
    setStyleSheet("QFrame#ResolutionWidget {"
                //   "border: 1px solid red;"
                  "}");
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
    const QString prepare = scriptAbsolutePath(m_resolution.prepareScript());
    const QString install = scriptAbsolutePath(m_resolution.installScript());
    Q_ASSERT(!prepare.isEmpty() && !install.isEmpty());

    QProcess *proc = new QProcess;
    QPROCESS_DUMP(proc);
    QPROCESS_DELETE_SELF(proc);

    connect(proc, static_cast<void (QProcess::*)(int)>(&QProcess::finished), this, &ResolutionWidget::prepareFinished);

    const QString sc = scriptAbsolutePath("dgradvrmgr-prepare.sh");
    proc->start("pkexec", QStringList() << "bash" << "-x" << sc << prepare << install);
    // proc->waitForFinished();
}
