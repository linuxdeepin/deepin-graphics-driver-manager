
#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QVBoxLayout>
#include <QSettings>

#include <DMainWindow>
#include <DWaterProgress>
#include <DSuggestButton>
#include <QStackedLayout>
#include "graphicsdriverproxy.h"
#include "device.h"



DWIDGET_USE_NAMESPACE

#define TEST_UI

class QLabel;
class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void keyPressEvent(QKeyEvent *e);
    void noResolutions();
    void loadDevice();
    void setVendorIcon();

private Q_SLOTS:
    void loadResolutions();
    void onResolutionSelected();
    void onToggleBtnClicked();
    void onRebootBtnClicked();
    void onPolicyKitPassed();
    void onPrepareFinished(bool success);

private:
    int m_usedIndex;
    int m_selectedIndex;
    bool m_started;
    //GraphicsDeviceInfo m_devInfo;
    //Resolutions m_resolutions;

    QVBoxLayout *m_resolutionsLayout;
    QLabel *m_topTips;
    QLabel *m_vendorIcon;
    QLabel *m_vendorName;
    QLabel *m_tipsIcon;
    QLabel *m_botTips;
    QWidget *m_resolutionsWidget;
    DWaterProgress *m_progress;
    DSuggestButton *m_toggleButton;
    DSuggestButton *m_okButton;
    DSuggestButton *m_rebootButton;
    ComDeepinDaemonGraphicsDriverInterface *m_graphicsDriver = nullptr;
    DeviceList m_devices;

};

#endif
