
#include "resolutionwidget.h"
#include "utils.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QTimer>
#include <QLineEdit>

ResolutionWidget::ResolutionWidget(ComDeepinDaemonGraphicsDriverInterface *graphicsDriver, const Resolution &resolution,  QWidget *parent)
    : QFrame(parent),
      m_graphicsDriver(graphicsDriver),
      m_resolution(resolution)
{

}

ResolutionWidget::~ResolutionWidget()
{

}

void ResolutionWidget::initUI()
{
    m_checkedBtn = new QLabel;
    m_checkedBtn->setPixmap(Utils::hidpiPixmap(":/resources/icons/select.svg", QSize(24, 24)));
    m_checkedBtn->setFixedSize(24, 24);

    m_title = new QLabel;
    m_title->setText(m_resolution.title());
    m_title->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    m_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_title->setStyleSheet("QLabel {"
                           "font-size: 14px;"
                           "color: #414d68;"
                           "}");

    m_version = new QLabel;
    m_version->setText(tr("Driver/Version: %1/%2").arg(m_resolution.driver()).arg(m_resolution.currVersion()));
    m_version->setWordWrap(true);
    m_version->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_version->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_version->setStyleSheet("QLabel {"
                                 "font-size: 12px;"
                                 "color: #c0c6d4;"
                                 "}");

    m_description = new QLabel;
    m_description->setText(m_resolution.description());
    m_description->setWordWrap(true);
    m_description->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_description->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_description->setStyleSheet("QLabel {"
                                 "font-size: 12px;"
                                 "color: #526a7f;"
                                 "}");

    QVBoxLayout *infoLayout = new QVBoxLayout;
    infoLayout->addWidget(m_title);
    infoLayout->addWidget(m_version);
    infoLayout->addWidget(m_description);
    infoLayout->setSpacing(3);
    infoLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *centralLayout = new QHBoxLayout;
    centralLayout->addLayout(infoLayout);
    centralLayout->addWidget(m_checkedBtn);
    centralLayout->setAlignment(m_checkedBtn, Qt::AlignVCenter | Qt::AlignRight);
    centralLayout->setSpacing(0);
    //centralLayout->setContentsMargins(10, 10, 10, 10);

    setLayout(centralLayout);
    setFixedHeight(80);
    setChecked(m_resolution.enable());
    setObjectName("ResolutionWidget");
    setStyleSheet("QFrame#ResolutionWidget {"
                  "background-color: rgba(0, 0, 0, 0.03);"
                  "border-radius: 8px"
                  "}");
}

void ResolutionWidget::setChecked(const bool checked)
{
    m_checked = checked;
    m_checkedBtn->setVisible(checked);
}

void ResolutionWidget::prepareInstall()
{
#ifdef TEST_UI
    m_timer.setInterval(50);
    m_timer.start();
    m_process = 0;
    connect(&m_timer, &QTimer::timeout, this, &ResolutionWidget::onTimeout);
#else
    QDBusPendingReply<void> preInstallReply = m_graphicsDriver->PrepareInstall(m_resolution.name());
    preInstallReply.waitForFinished();
    qDebug() << "m_resolution.name = " << m_resolution.name();
    if (!preInstallReply.isValid()) {
        qDebug() << "prepareInstall error:" << preInstallReply.error();
        Q_EMIT prepareFinished(false);
        return;
    }

    connect(m_graphicsDriver, &ComDeepinDaemonGraphicsDriverInterface::ReportProgress, [=](QString ratio){
        if (m_checked) {
            int process = ratio.toInt();
            if (process < 0) {
                Q_EMIT prepareFinished(false);
            } else if (process >= 100) {
                Q_EMIT prepareFinished(true);
            } else {
                Q_EMIT policyKitPassed(ratio);
            }
            qDebug() << "prepare install process: " << ratio;
        }
    });
#endif
}

bool ResolutionWidget::canUpdate()
{
    return m_resolution.currVersion().compare(m_resolution.repVersion()) < 0;
}

void ResolutionWidget::mouseReleaseEvent(QMouseEvent *e)
{
    e->accept();
    Q_EMIT clicked();
}

#ifdef TEST_UI
void ResolutionWidget::onTimeout()
{
    m_process++;
    QString state = QString("%1").arg(m_process);
    Q_EMIT policyKitPassed(state);
    if (m_process >= 100) {
        m_process = 0;
        Q_EMIT prepareFinished(true);
    }
}
#endif


