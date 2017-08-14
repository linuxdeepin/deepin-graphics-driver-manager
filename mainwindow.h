
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class GraphicsDeviceInfo;
class MainWindow : public QWidget 
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void keyPressEvent(QKeyEvent *e);

private:
    GraphicsDeviceInfo *m_devInfo;
};

#endif
