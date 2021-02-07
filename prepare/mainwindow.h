
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphicsdriverproxy.h"
#include "device.h"

#include <QVBoxLayout>
#include <QSettings>
#include <DMainWindow>
#include <DWaterProgress>
#include <QPushButton>
#include <QStackedLayout>
#include <DThemeManager>
#include <DSuggestButton>


DWIDGET_USE_NAMESPACE


class QLabel;
class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    enum SolutionType {
        NoResolution = 0,
        AMD,
        NVIDIA,
        INTEL,
        INTEL_NVIDIA,
        INTEL_NVIDIA_USE_INTEL,
        INTEL_NVIDIA_USE_NVIDIA
    };
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void keyPressEvent(QKeyEvent *e);
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
    void noResolutions();
    void loadDevice();
    void setVendorIcon();

private Q_SLOTS:
    void loadResolutions();
    void onResolutionSelected();
    void onUpdateBtnClicked();
    void onToggleBtnClicked();
    void onRebootBtnClicked();
    void onCancelBtnClicked();
    void onPolicyKitPassed(const QString &state);
    void onPrepareFinished(bool success);



private:
    int m_usedIndex;
    int m_selectedIndex;
    bool m_started;
    //GraphicsDeviceInfo m_devInfo;
    //Resolutions m_resolutions;

    QWidget *m_centerWidget;
    QVBoxLayout *m_resolutionsLayout;
    QLabel *m_topTips;
    QLabel *m_vendorIcon;
    QLabel *m_vendorName;
    QLabel *m_tipsIcon;
    QLabel *m_warnning;
    QLabel *m_botTips;
    QWidget *m_resolutionsWidget;
    DWaterProgress *m_progress;
    QPushButton *m_toggleButton;
    QPushButton *m_okButton;
    QPushButton *m_updateButton;
    DSuggestButton *m_rebootButton;
    QPushButton *m_rebootLaterButton;
    QPushButton *m_cancelButtion;
    ComDeepinDaemonGraphicsDriverInterface *m_graphicsDriver = nullptr;
    DeviceList m_devices;

};

#endif
