
#include "mainwindow.h"
#include "widgets/resolutionwidget.h"

#include <QGuiApplication>
#include <QScreen>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>

#include <DTitlebar>

MainWindow::MainWindow(QWidget *parent) :
    DMainWindow(parent),

    m_resolutions(ResolutionsBuilder(m_devInfo).build())
{
    // DTitlebar *tbar = titlebar();
    // tbar->setWindowFlags(tbar->windowFlags() & ~Qt::WindowMaximizeButtonHint);

    m_resolutionsLayout = new QVBoxLayout;

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(m_resolutionsLayout);

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