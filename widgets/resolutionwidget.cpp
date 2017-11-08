
#include "resolutionwidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QTimer>

#include <DSvgRenderer>

DWIDGET_USE_NAMESPACE

ResolutionWidget::ResolutionWidget(const Resolution &r, QWidget *parent) :
    QFrame(parent),

    m_resolution(r)
{
    const auto ratio = devicePixelRatioF();
    QPixmap checkedPix = DSvgRenderer::render(":/resources/icons/select.svg", QSize(16, 16) * ratio);
    checkedPix.setDevicePixelRatio(ratio);

    m_checkedBtn = new QLabel;
    m_checkedBtn->setPixmap(checkedPix);
    m_checkedBtn->setFixedSize(16, 16);

    m_title = new QLabel;
    m_title->setText(r.title());
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

void ResolutionWidget::setUsing(const bool using_)
{
    m_using = using_;

    if (m_using)
        m_title->setText(tr("%1(Using)").arg(m_resolution.title()));
    else
        m_title->setText(m_resolution.title());
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

void ResolutionWidget::prepareInstall(const Resolution &old_resolution)
{
    const QString prepare = scriptAbsolutePath(m_resolution.prepareScript());
    const QString install = scriptAbsolutePath(m_resolution.installScript());
    Q_ASSERT(!prepare.isEmpty() && !install.isEmpty());

    QProcess *proc = new QProcess;
    QPROCESS_DUMP(proc);
    QPROCESS_DELETE_SELF(proc);

    connect(proc, static_cast<void (QProcess::*)(int)>(&QProcess::finished), this, &ResolutionWidget::prepareFinished);
    connect(proc, &QProcess::readyRead, this, [=] { emit policyKitPassed(); proc->disconnect(proc, &QProcess::readyRead, this, nullptr); });

    const QString &exit_gltest = old_resolution.exit_gltest() ? "true" : "false";
    const QString &new_driver = m_resolution.name();
    const QString &lang = QLocale().name();
    const QString &sc = scriptAbsolutePath("dgradvrmgr-prepare.sh");
    const QString &old_driver = old_resolution.name();

    QStringList args { "bash", "-x", sc, prepare, install, old_driver, new_driver, lang, exit_gltest };

    proc->start("pkexec", args);
}
