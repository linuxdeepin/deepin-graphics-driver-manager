
#include "resolutionwidget.h"

#include <QLabel>
#include <QVBoxLayout>

ResolutionWidget::ResolutionWidget(const Resolution &r, QWidget *parent) :
    QWidget(parent)
{
    m_title = new QLabel;
    m_title->setText(r.name());
    m_description = new QLabel;
    m_description->setText(r.description());

    QVBoxLayout *infoLayout = new QVBoxLayout;
    infoLayout->addWidget(m_title);
    infoLayout->addWidget(m_description);
    infoLayout->setSpacing(0);
    infoLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(infoLayout);
    setFixedHeight(70);
}