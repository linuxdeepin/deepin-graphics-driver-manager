
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphicsdeviceinfo.h"
#include "resolutions/resolutions.h"

#include <QVBoxLayout>

#include <DMainWindow>

DWIDGET_USE_NAMESPACE

class QPushButton;
class QLabel;
class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void keyPressEvent(QKeyEvent *e);

private Q_SLOTS:
    void loadResolutions();

private:
    GraphicsDeviceInfo m_devInfo;
    Resolutions m_resolutions;

    QVBoxLayout *m_resolutionsLayout;
    QLabel *m_resolutionsIcon;
    QWidget *m_resolutionsWidget;
    QPushButton *m_toggleButton;
};

#endif
