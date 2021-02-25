
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphicsdriverproxy.h"

#include <QVBoxLayout>
#include <QSettings>
#include <DMainWindow>
#include <DWaterProgress>
#include <QPushButton>
#include <QStackedLayout>
#include <DThemeManager>
#include <DSuggestButton>


DWIDGET_USE_NAMESPACE

//#define TEST_UI
class QLabel;
class MainWindow : public DMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private:
    void closeEvent(QCloseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void updateProgress();
    void updateInstallState(bool success);
    void reboot();
private Q_SLOTS:
    void onCancelBtnClicked();
    void onInstall();


private:
    QWidget *m_centerWidget;
    QLabel *m_installStateIcon;
    QLabel *m_installState;
    QLabel *m_warningTips;
    DWaterProgress *m_waterProgress;
    QPushButton *m_cancelButton;
    QPushButton *m_rebootButton;
    QTimer m_timer;
    int m_rebootDelay;
    int m_process;
    ComDeepinDaemonGraphicsDriverInterface *m_graphicsDriver = nullptr;
};

#endif
