#include "gltestwindow.h"
#include <QApplication>
#include <QKeyEvent>
#include <QScreen>

GLTestWindow::GLTestWindow(QWidget *parent)
    : QWidget(parent)
{
    m_acceptBtn = new QPushButton(tr("Apply"));
    m_cancelBtn = new QPushButton(tr("Cancel"));
    m_tipsLabel = new QLabel(tr("Please ensure the driver works normally without blurred screen and screen tearing"));
    m_tipsLabel->setAlignment(Qt::AlignCenter);

    m_glTestWidget = new GLTestWidget;
    m_glTestWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addStretch();
    btnsLayout->addWidget(m_cancelBtn);
    btnsLayout->addWidget(m_acceptBtn);
    btnsLayout->addStretch();

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_glTestWidget);
    centralLayout->addWidget(m_tipsLabel);
    centralLayout->addLayout(btnsLayout);

    setLayout(centralLayout);
    setFixedSize(qApp->primaryScreen()->geometry().size());
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    connect(m_acceptBtn, &QPushButton::clicked, this, &GLTestWindow::onAccept);
    connect(m_cancelBtn, &QPushButton::clicked, this, &GLTestWindow::onCancel);
}

void GLTestWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
#ifdef QT_DEBUG
    case Qt::Key_Escape:    qApp->quit();       break;
#endif
    default:;
    }
}

void GLTestWindow::setExitGLTest(const bool exit)
{
    m_exit_gltest = exit;
}

void GLTestWindow::onAccept()
{
    QProcess::execute("/usr/lib/deepin-graphics-driver-manager/gltest-set-success.sh");
    qApp->exit(0);
}

void GLTestWindow::onCancel()
{
    qApp->exit(-1);
}
