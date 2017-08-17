
#include "mainwindow.h"
#include "widgets/resolutionwidget.h"

#include <QGuiApplication>
#include <QScreen>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QLabel>

#include <DTitlebar>

MainWindow::MainWindow(QWidget *parent) :
    DMainWindow(parent),

    m_resolutions(ResolutionsBuilder(m_devInfo).build())
{
    m_toggleButton = new QPushButton;
    m_toggleButton->setText(tr("Toggle"));

    m_resolutionsIcon = new QLabel;

    m_resolutionsLayout = new QVBoxLayout;
    m_resolutionsWidget = new QWidget;
    m_resolutionsWidget->setLayout(m_resolutionsLayout);

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_resolutionsIcon);
    centralLayout->addStretch();
    centralLayout->addWidget(m_resolutionsWidget);
    centralLayout->addStretch();
    centralLayout->addWidget(m_toggleButton);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(90, 0, 90, 30);

    DTitlebar *tbar = titlebar();
    tbar->setTitle(QString());
    tbar->setWindowFlags(tbar->windowFlags() & ~Qt::WindowMaximizeButtonHint);

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(centralLayout);

    setFixedSize(500, 600);
    move(qApp->primaryScreen()->geometry().center() - rect().center());

    QTimer::singleShot(0, this, &MainWindow::loadResolutions);
}

MainWindow::~MainWindow()
{
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
#ifdef QT_DEBUG
    case Qt::Key_Escape:        qApp->quit();       break;
#endif
    default:;
    }

    return QWidget::keyPressEvent(e);
}

void MainWindow::loadResolutions()
{
    for (const auto &r : m_resolutions.resolutions())
    {
        ResolutionWidget *rw = new ResolutionWidget(r);
        m_resolutionsLayout->addWidget(rw);
    }
}