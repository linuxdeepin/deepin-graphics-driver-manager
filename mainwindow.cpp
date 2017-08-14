
#include "mainwindow.h"
#include "graphicsdeviceinfo.h"

#include <QGuiApplication>
#include <QScreen>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) : 
    QWidget(parent),

    m_devInfo(new GraphicsDeviceInfo)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(400, 300);
    move(qApp->primaryScreen()->geometry().center() - rect().center());
}

MainWindow::~MainWindow()
{
    delete m_devInfo;
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