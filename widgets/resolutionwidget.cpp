
#include "resolutionwidget.h"
#include "utils.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QTimer>
#include <QLineEdit>

ResolutionWidget::ResolutionWidget(const Resolution &r, QWidget *parent) :
    QFrame(parent),

    m_resolution(r)
{
    const auto ratio = devicePixelRatioF();
    QPixmap checkedPix = Utils::renderSVG(":/resources/icons/select.svg", QSize(16, 16));
    checkedPix.setDevicePixelRatio(ratio);

    m_checkedBtn = new QLabel;
    m_checkedBtn->setPixmap(checkedPix);
    m_checkedBtn->setFixedSize(16, 16);

    m_title = new QLabel;
    m_title->setText(r.title());
    m_title->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    m_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    m_version = new QLineEdit;
    m_version->setVisible(false);
    m_version->setReadOnly(true);
    m_version->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_version->setEnabled(false);
    m_version->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_version->setStyleSheet("QLineEdit {"
                             "font-size: 9pt;"
                             "color: #888;"
                             "border: none;"
                             "padding: 0;"
                             "}");

    m_description = new QLabel;
    m_description->setText(r.description());
    m_description->setWordWrap(true);
    m_description->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_description->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_description->setStyleSheet("QLabel {"
                                 "font-size: 9pt;"
                                 "color: #888;"
                                 "}");

    QVBoxLayout *infoLayout = new QVBoxLayout;
    infoLayout->addWidget(m_title);
    infoLayout->addWidget(m_version);
    infoLayout->addWidget(m_description);
    infoLayout->setSpacing(0);
    infoLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *centralLayout = new QHBoxLayout;
    centralLayout->addLayout(infoLayout);
    centralLayout->addWidget(m_checkedBtn);
    centralLayout->setAlignment(m_checkedBtn, Qt::AlignVCenter | Qt::AlignRight);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 0, 0, 0);

    QTimer::singleShot(1, this, &ResolutionWidget::checkCondition);
    QTimer::singleShot(1, this, &ResolutionWidget::checkVersion);

    setLayout(centralLayout);
    setFixedHeight(80);
    setChecked(false);
    setObjectName("ResolutionWidget");
    setStyleSheet("QFrame#ResolutionWidget {"
//                   "border: 1px solid red;"
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

    connect(proc, static_cast<void (QProcess::*)(int)>(&QProcess::finished), this, [=] (const int exitCode) { setVisible(!exitCode); });

    EXECUTE_SCRIPT(proc, script);
}

void ResolutionWidget::checkVersion()
{
    const QString &script = m_resolution.versionScript();
    if (script.isEmpty())
        return;

    QProcess *proc = new QProcess;
    QPROCESS_DELETE_SELF(proc);

    connect(proc, static_cast<void (QProcess::*)(int)>(&QProcess::finished), this, [=] (const int exitCode) {
        if (exitCode)
            return;
        m_version->setText(tr("Version: ") + proc->readAllStandardOutput());
        m_version->setVisible(true);
    });

    EXECUTE_SCRIPT(proc, script);
}

void ResolutionWidget::prepareInstall(const Resolution &old_resolution)
{
    const QString prepare = scriptAbsolutePath(m_resolution.prepareScript());
    const QString install = scriptAbsolutePath(m_resolution.installScript());
    const QString removeOld = scriptAbsolutePath(old_resolution.removeScript());
    Q_ASSERT(!prepare.isEmpty() && !install.isEmpty());

    QProcess *proc = new QProcess;
    QPROCESS_DUMP(proc);
    QPROCESS_DELETE_SELF(proc);

    connect(proc, static_cast<void (QProcess::*)(int)>(&QProcess::finished), this, &ResolutionWidget::prepareFinished);
    connect(proc, &QProcess::readyRead, this, [=] { emit policyKitPassed(); proc->disconnect(proc, &QProcess::readyRead, this, nullptr); });

    const QString &exit_gltest = m_resolution.keep_gltest() ? "false" : "true";
    const QString &new_driver = m_resolution.name();
    const QString &lang = QLocale().name();
    const QString &sc = scriptAbsolutePath("dgradvrmgr-prepare.sh");
    const QString &old_driver = old_resolution.name();

    QStringList args { "/bin/bash", "-x", sc, prepare, removeOld, install, old_driver, new_driver, lang, exit_gltest };

    proc->start("pkexec", args);
}
