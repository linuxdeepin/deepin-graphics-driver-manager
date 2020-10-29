
#include "mainwindow.h"
#include "resolutionwidget.h"


#include <QApplication>
#include <QScreen>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QAction>

#include <DTitlebar>
#include <DSvgRenderer>
#include <DThemeManager>


#define INSTALLER_DESKTOP_FILE_SOURCE "/usr/lib/deepin-graphics-driver-manager/deepin-gradvrmgr-installer.desktop"
#define INSTALLER_ROOT_DESKTOP_FILE_DEST "etc/xdg/autostart/deepin-gradvrmgr-installer.desktop"





MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent)
{
    m_toggleButton = new DSuggestButton;
    m_toggleButton->setText(tr("Switch"));
    m_toggleButton->setFixedHeight(38);

    m_topTips = new QLabel;
    m_topTips->setAlignment(Qt::AlignHCenter);
    m_topTips->setVisible(false);
    m_topTips->setObjectName("TopTips");

    m_botTips = new QLabel;
    m_botTips->setAlignment(Qt::AlignHCenter);
    m_botTips->setVisible(false);
    m_botTips->setWordWrap(true);
    m_botTips->setObjectName("BottomTips");

    m_tipsIcon = new QLabel;

    m_okButton = new DSuggestButton;
    m_okButton->setText(tr("OK"));
    m_okButton->setFixedHeight(38);

    m_rebootButton = new DSuggestButton;
    m_rebootButton->setText(tr("Reboot"));
    m_rebootButton->setFixedHeight(38);
    m_rebootButton->setVisible(false);

    m_progress = new DWaterProgress;
    m_progress->setTextVisible(true);
    m_progress->setFixedSize(100, 100);
    m_progress->setValue(0);
    m_progress->setVisible(false);

    m_vendorIcon = new QLabel;
    m_vendorIcon->setAlignment(Qt::AlignCenter);
    //m_vendorIcon->setPixmap(hidpiPixmap(":/resources/icons/" + m_resolutions.iconName(), QSize(128, 128)));
    m_vendorName = new QLabel;
    m_vendorName->setWordWrap(true);
    m_vendorName->setAlignment(Qt::AlignCenter);
    //m_vendorName->setText(m_devInfo.devices().toList().join('\n'));

    m_resolutionsLayout = new QVBoxLayout;
    m_resolutionsLayout->setContentsMargins(8, 8, 8, 8);
    m_resolutionsLayout->setSpacing(1);
    m_resolutionsWidget = new QWidget;
    m_resolutionsWidget->setLayout(m_resolutionsLayout);
    m_resolutionsWidget->setObjectName("ResolutionsWidget");

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_topTips);
    centralLayout->addWidget(m_vendorIcon);
    centralLayout->addWidget(m_vendorName);
    centralLayout->addStretch();
    centralLayout->addWidget(m_tipsIcon);
    centralLayout->setAlignment(m_tipsIcon, Qt::AlignHCenter);
    centralLayout->addWidget(m_resolutionsWidget);
    centralLayout->addWidget(m_progress);
    centralLayout->setAlignment(m_progress, Qt::AlignHCenter);
    centralLayout->addWidget(m_botTips);
    centralLayout->addStretch();
    centralLayout->addWidget(m_toggleButton);
    centralLayout->addWidget(m_okButton);
    centralLayout->addWidget(m_rebootButton);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(40, 0, 40, 30);

    titlebar()->setTitle(" ");
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(centralLayout);

    setFixedSize(440, 600);
    move(qApp->primaryScreen()->geometry().center() - rect().center());

    //connect(m_toggleButton, &DSuggestButton::clicked, this, &MainWindow::onToggleBtnClicked);
    //connect(m_rebootButton, &DSuggestButton::clicked, this, &MainWindow::onRebootBtnClicked);
    connect(m_okButton, &DSuggestButton::clicked, qApp, &QApplication::quit);

    QTimer::singleShot(0, this, &MainWindow::loadResolutions);

}

MainWindow::~MainWindow()
{

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{

}

void MainWindow::noResolutions()
{

}

void MainWindow::loadResolutions()
{

}
