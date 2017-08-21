
#include "resolutionwidget.h"

#include <QLabel>
#include <QVBoxLayout>

DWIDGET_USE_NAMESPACE

ResolutionWidget::ResolutionWidget(const Resolution &r, QWidget *parent) :
    QWidget(parent)
{
    m_checkedBtn = new DImageButton;
    m_checkedBtn->setText("aa");
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

    setLayout(centralLayout);
    setFixedHeight(70);
}

void ResolutionWidget::setChecked(const bool checked)
{
    m_checked = checked;
    m_checkedBtn->setVisible(checked);
}