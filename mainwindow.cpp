
#include "mainwindow.h"

#include <QGuiApplication>
#include <QScreen>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) : 
    QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(400, 300);
    move(qApp->primaryScreen()->geometry().center() - rect().center());
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