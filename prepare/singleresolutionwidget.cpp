#include "singleresolutionwidget.h"
#include "utils.h"

#include <QLabel>
#include <QHBoxLayout>

SingleResolutionWidget::SingleResolutionWidget(ComDeepinDaemonGraphicsDriverInterface *graphicsDriver, const Resolution &resolution, QWidget *parent)
    : ResolutionWidget(graphicsDriver,  resolution,  parent)
{

}

void SingleResolutionWidget::initUI()
{
//    m_checkedBtn = new QLabel;
//    m_checkedBtn->setPixmap(Utils::hidpiPixmap(":/resources/icons/select.svg", QSize(16, 16)));
//    m_checkedBtn->setFixedSize(16, 16);

//    m_title = new QLabel;
//    m_title->setText(m_resolution.title());
//    m_title->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
//    m_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

//    m_version = new QLabel;
//    m_version->setText(tr("Dirver/Version: %1/%2").arg(m_resolution.driver()).arg(m_resolution.currVersion()));
//    m_version->setWordWrap(true);
//    m_version->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
//    m_version->setAlignment(Qt::AlignTop | Qt::AlignLeft);
//    m_version->setStyleSheet("QLabel {"
//                                 "font-size: 9pt;"
//                                 "color: #888;"
//                                 "}");

//    m_description = new QLabel;
//    m_description->setText(m_resolution.description());
//    m_description->setVisible(false);
//    m_description->setWordWrap(true);
//    m_description->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    m_description->setAlignment(Qt::AlignTop | Qt::AlignLeft);
//    m_description->setStyleSheet("QLabel {"
//                                 "font-size: 9pt;"
//                                 "color: #888;"
//                                 "}");

//    QVBoxLayout *infoLayout = new QVBoxLayout;
//    infoLayout->addWidget(m_title);
//    infoLayout->addWidget(m_version);
//    infoLayout->addWidget(m_description);
//    infoLayout->setSpacing(0);
//    infoLayout->setContentsMargins(0, 0, 0, 0);

//    QHBoxLayout *centralLayout = new QHBoxLayout;
//    centralLayout->addLayout(infoLayout);
//    centralLayout->addWidget(m_checkedBtn);
//    centralLayout->setAlignment(m_checkedBtn, Qt::AlignVCenter | Qt::AlignRight);
//    centralLayout->setSpacing(0);
//    centralLayout->setContentsMargins(0, 0, 0, 0);

//    setLayout(centralLayout);
//    setFixedHeight(80);
//    setChecked(m_resolution.enable());
//    setObjectName("ResolutionWidget");
//    setStyleSheet("QFrame#ResolutionWidget {"
////                   "border: 1px solid red;"
//                  "}");
    ResolutionWidget::initUI();
    m_title->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    m_version->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    m_description->setVisible(false);
    setStyleSheet("QFrame#ResolutionWidget {"
                  "border-radius: 8px"
                  "}");
}

void SingleResolutionWidget::setChecked(const bool checked)
{
    m_checked = checked;
    m_checkedBtn->setVisible(false);
}
