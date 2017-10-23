
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphicsdeviceinfo.h"
#include "resolutions/resolutions.h"

#include <QVBoxLayout>

#include <DMainWindow>
#include <DWaterProgress>

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
    void onResolutionSelected();
    void onToggleBtnClicked();
    void onRebootBtnClicked();
    void onPrepareFinished();

private:
    int m_usedIndex;
    int m_selectedIndex;
    GraphicsDeviceInfo m_devInfo;
    Resolutions m_resolutions;

    QVBoxLayout *m_resolutionsLayout;
    QLabel *m_resolutionsIcon;
    QLabel *m_vendorsName;
    QWidget *m_resolutionsWidget;
    DWaterProgress *m_progress;
    QPushButton *m_toggleButton;
    QPushButton *m_okButton;
    QPushButton *m_rebootButton;
};

#endif
