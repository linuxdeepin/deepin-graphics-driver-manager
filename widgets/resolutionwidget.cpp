
#include "resolutionwidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QDebug>
#include <QProcess>

ResolutionWidget::ResolutionWidget(const Resolution &r, QWidget *parent) :
    QWidget(parent)
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
    e->ignore();

    QWidget::mouseReleaseEvent(e);

    emit clicked();
}

void ResolutionWidget::checkInstallStat(const QString &script)
{
    if (script.isEmpty())
        return;

    qDebug() << script;

    QProcess *proc = new QProcess;

    connect(proc, static_cast<void (QProcess::*)(int)>(&QProcess::finished), proc, &QProcess::deleteLater);
    connect(proc, &QProcess::readyReadStandardOutput, this, [=] { qDebug().noquote() << proc->readAllStandardOutput(); });
    connect(proc, &QProcess::readyReadStandardError, this, [=] { qWarning().noquote() << proc->readAllStandardError(); });

    proc->start("bash", QStringList() << "-x" << "scripts/" + script);
    proc->waitForFinished();
    const int r = proc->exitCode();

    if (!r)
    {
        m_running = true;
        emit clicked();
    }
}