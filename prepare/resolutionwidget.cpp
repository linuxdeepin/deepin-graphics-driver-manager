
#include "resolutionwidget.h"
#include "utils.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QTimer>
#include <QLineEdit>

ResolutionWidget::ResolutionWidget(ComDeepinDaemonGraphicsDriverInterface *graphicsDriver, Resolution &resolution,  QWidget *parent)
    : QFrame(parent),
      m_graphicsDriver(graphicsDriver),
      m_resolution(resolution)
{

       m_checkedBtn = new QLabel;
       m_checkedBtn->setPixmap(Utils::hidpiPixmap(":/resources/icons/select.svg", QSize(16, 16)));
       m_checkedBtn->setFixedSize(16, 16);

       m_title = new QLabel;
       m_title->setText(resolution.title());
       m_title->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
       m_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

       m_version = new QLabel;
       m_version->setText(tr("dirver/version: %1/%2").arg(resolution.driver()).arg(resolution.currVersion()));
       m_version->setWordWrap(true);
       m_version->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
       m_version->setAlignment(Qt::AlignTop | Qt::AlignLeft);
       m_version->setStyleSheet("QLabel {"
                                    "font-size: 9pt;"
                                    "color: #888;"
                                    "}");



       m_description = new QLabel;
       m_description->setText(resolution.description());
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

       setLayout(centralLayout);
       setFixedHeight(80);
       setChecked(resolution.enable());
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

void ResolutionWidget::prepareInstall()
{
#ifdef TEST_UI
    m_timer.setInterval(100);
    m_timer.start();
    m_process = 0;
    connect(&m_timer, &QTimer::timeout, this, &ResolutionWidget::onTimeout);
#else
    QDBusPendingReply<void> preInstallReply = m_graphicsDriver->PrepareInstall(m_resolution.name());
    if (!preInstallReply.isValid()) {
        qDebug() << preInstallReply.error();
        emit prepareFinished(false);
        return;
    }
    connect(m_graphicsDriver, &ComDeepinDaemonGraphicsDriverInterface::PreInstallState, this, &ResolutionWidget::policyKitPassed);
#endif
}

bool ResolutionWidget::canUpdate()
{
    return m_resolution.currVersion().compare(m_resolution.repVersion()) < 0;
}

void ResolutionWidget::mouseReleaseEvent(QMouseEvent *e)
{
    e->accept();
    emit clicked();
}

void ResolutionWidget::onTimeout()
{
    m_process++;
    QString state = QString("%1").arg(m_process);
    emit policyKitPassed(state);
    if (m_process >= 100) {
        m_process = 0;
        emit prepareFinished(true);
    }
}


