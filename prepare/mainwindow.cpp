
#include "mainwindow.h"
#include "resolutionwidget.h"
#include "utils.h"
#include "resolution.h"

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
#include <QJsonObject>
#include <QJsonDocument>

const QString GraphicMangerServiceName = "com.deepin.graphicmanger";
const QString GraphicMangerPath = "/com/deepin/graphicmanger";


MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent)
{

    m_graphicsDriver = new ComDeepinDaemonGraphicsDriverInterface(
                   GraphicMangerServiceName,
                   GraphicMangerPath,
                   QDBusConnection::systemBus());

    m_toggleButton = new DSuggestButton;
    m_toggleButton->setText(tr("Switch"));
    m_toggleButton->setFixedHeight(38);
    m_toggleButton->setVisible(false);

    m_topTips = new QLabel;
    m_topTips->setAlignment(Qt::AlignHCenter);
    m_topTips->setVisible(false);
    m_topTips->setObjectName("TopTips");

    m_botTips = new QLabel;
    m_botTips->setAlignment(Qt::AlignHCenter);
    m_botTips->setVisible(false);
    m_botTips->setWordWrap(true);
    m_botTips->setObjectName("BottomTips");

    m_warnning = new QLabel;
    m_warnning->setAlignment(Qt::AlignHCenter);
    m_warnning->setVisible(false);
    m_warnning->setWordWrap(true);
    m_warnning->setObjectName("Warnning");

    m_tipsIcon = new QLabel;

    m_okButton = new DSuggestButton;
    m_okButton->setText(tr("OK"));
    m_okButton->setFixedHeight(38);

    m_updateButton = new DSuggestButton;
    m_updateButton->setText(tr("Update"));
    m_updateButton->setFixedHeight(38);
    m_updateButton->setVisible(false);

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
    m_vendorName = new QLabel;
    m_vendorName->setWordWrap(true);
    m_vendorName->setAlignment(Qt::AlignCenter);

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
    centralLayout->addWidget(m_warnning);
    centralLayout->addSpacing(10);
    centralLayout->addWidget(m_toggleButton);
    centralLayout->addWidget(m_okButton);
    centralLayout->addWidget(m_updateButton);
    centralLayout->addWidget(m_rebootButton);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(40, 0, 40, 30);

    titlebar()->setTitle(" ");
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(centralLayout);

    setFixedSize(440, 600);
    move(qApp->primaryScreen()->geometry().center() - rect().center());

    connect(m_toggleButton, &DSuggestButton::clicked, this, &MainWindow::onToggleBtnClicked);
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
    m_tipsIcon->setVisible(true);
    m_tipsIcon->setPixmap(Utils::hidpiPixmap(":/resources/icons/fail.svg", QSize(128, 128)));
    m_botTips->setText(tr("Your hardware is not supported currently, please wait for future version."));
    m_botTips->setVisible(true);
    m_resolutionsWidget->setVisible(false);
    m_toggleButton->setVisible(false);
}

void MainWindow::loadDevice()
{
    QString devices;
#ifdef TEST_UI
    QString path = RESOURCES_DIR"/test/device.json";
    QFile file(path);
    if (file.exists()) {
       file.open(QIODevice::ReadOnly);
       devices = QString(file.readAll());
    }
#else
    QDBusPendingReply<QString> getDeviceReply = m_graphicsDriver->GetDevice();
    getDeviceReply.waitForFinished();
    if (!getDeviceReply.isValid()) {
        qDebug() << getDeviceReply.error();
        return;
    }
    devices = getDeviceReply.value();
#endif
    const QJsonObject devObj = Utils::QStringToJson(devices);
    for (const auto d : devObj["devices"].toArray()) {
        Device device(d.toObject());
        if (device.name().isEmpty() || device.info().isEmpty()) {
            return;
        }
        m_devices.push_back(device);
    }
    setVendorIcon();
}

void MainWindow::setVendorIcon()
{
    bool bIntel = false;
    bool bAmd = false;
    bool bNvidia = false;

    QStringList devInfo;
    foreach(const auto dev, m_devices) {
        if (dev.name().contains("intel", Qt::CaseInsensitive)) {
            bIntel = true;
        }

        if (dev.name().contains("amd", Qt::CaseInsensitive)) {
            bAmd = true;
        }

        if (dev.name().contains("nvidia", Qt::CaseInsensitive)) {
            bNvidia = true;
        }
        devInfo << dev.info();
    }

    QString iconPath;
    if (bIntel && !bAmd && !bNvidia) {
       iconPath = RESOURCES_DIR"/icons/Intel.svg";
    }

    if (bAmd && !bIntel && !bNvidia) {
        iconPath = RESOURCES_DIR"/icons/AMD.svg";
    }

    if (bNvidia && !bIntel && !bAmd) {
        iconPath = RESOURCES_DIR"/icons/NVIDIA.svg";
    }

    if (bIntel && bNvidia && !bAmd) {
        iconPath = RESOURCES_DIR"/icons/Intel-NVIDIA.svg";
    }

    m_vendorIcon->setPixmap(Utils::hidpiPixmap(iconPath, QSize(128, 128)));
    m_vendorName->setText(devInfo.join('\n'));

}

void MainWindow::loadResolutions()
{
    loadDevice();
    QString strResolution;

#ifdef TEST_UI
    QString path = RESOURCES_DIR"/test/intel_nvidia.json";
    QFile file(path);
    if (file.exists()) {
       file.open(QIODevice::ReadOnly);
       strResolution = QString(file.readAll());
    }
#else

    QDBusPendingReply<QString> resolutionReply = m_graphicsDriver->GetResolutionTitle();
    resolutionReply.waitForFinished();
    if (!resolutionReply.isValid()) {
        qDebug() << resolutionReply.error();
    }
    strResolution = resolutionReply.value();
#endif

    qDebug() << strResolution;
    QJsonObject resolutionRoot = Utils::QStringToJson(strResolution);

    QJsonArray resolutions = resolutionRoot["resolutions"].toArray();
    if (resolutions.empty()) {
        noResolutions();
        return;
    }


    if (resolutionRoot["type"].toInt() == INTEL_NVIDIA_USE_INTEL) {
        m_warnning->setVisible(true);
        m_warnning->setText(tr("The current access is the integrated graphics card interface. "
                               "If you want to switch to the independent graphics card interface, the screen may appear black."));

    } else if (resolutionRoot["type"].toInt() == INTEL_NVIDIA_USE_NVIDIA) {
        m_warnning->setVisible(true);
        m_warnning->setText(tr("The current access is an independent graphics card interface. "
                               "If you want to switch to the integrated graphics card interface, the screen may appear black."));
    }

    int index = 0;
    for (const auto res : resolutions) {
        Resolution solution(res.toObject());
        ResolutionWidget *rw = new ResolutionWidget(m_graphicsDriver, solution);
        m_resolutionsLayout->addWidget(rw);
        connect(rw, &ResolutionWidget::clicked, this, &MainWindow::onResolutionSelected);
        if (solution.enable()) {
            m_usedIndex = index;

            if (rw->canUpdate()) {
                m_updateButton->setVisible(true);
                m_okButton->setVisible(false);
                m_toggleButton->setVisible(false);
            }
        }

        index++;
    }
}

void MainWindow::onResolutionSelected()
{
    ResolutionWidget *rw = static_cast<ResolutionWidget *>(sender());
    qDebug() << "Resolution selected: " << rw->resolution().name();

    const int idx = m_resolutionsLayout->indexOf(rw);
    Q_ASSERT(idx != -1);

    m_selectedIndex = idx;

    for (int i = 0; i != m_resolutionsLayout->count(); ++i)
    {
        ResolutionWidget *w = static_cast<ResolutionWidget *>(m_resolutionsLayout->itemAt(i)->widget());
        w->setChecked(i == idx);
    }

    const bool changed = m_selectedIndex != m_usedIndex;

    if (changed) {
        if (rw->resolution().enable() && rw->canUpdate()) {
            m_toggleButton->setVisible(false);
            m_okButton->setVisible(false);
            m_updateButton->setVisible(true);
            m_updateButton->setFocus();
        } else {
            m_toggleButton->setVisible(true);
            m_toggleButton->setFocus();
            m_updateButton->setVisible(false);

        }

    } else {
        if (rw->canUpdate()) {
            m_toggleButton->setVisible(false);
            m_okButton->setVisible(false);
            m_updateButton->setVisible(true);
            m_updateButton->setFocus();
        } else {
            m_okButton->setFocus();
            m_toggleButton->setVisible(true);
        }

    }
}

void MainWindow::onUpdateBtnClicked()
{

}

void MainWindow::onToggleBtnClicked()
{
    Q_ASSERT(m_selectedIndex != m_usedIndex);

    m_started = false;

    ResolutionWidget *new_driver_widget = static_cast<ResolutionWidget *>(m_resolutionsLayout->itemAt(m_selectedIndex)->widget());
    new_driver_widget->prepareInstall();

    connect(new_driver_widget, &ResolutionWidget::prepareFinished, this, &MainWindow::onPrepareFinished);
    connect(new_driver_widget, &ResolutionWidget::policyKitPassed, this, &MainWindow::onPolicyKitPassed);
}

void MainWindow::onRebootBtnClicked()
{

}

void MainWindow::onPolicyKitPassed(const QString &state)
{
    ResolutionWidget *new_driver_widget = static_cast<ResolutionWidget *>(m_resolutionsLayout->itemAt(m_selectedIndex)->widget());
    const QString &new_driver_name = new_driver_widget->resolution().name();

    if (!m_started) {
        m_started = true;
        // toggle UI
        m_topTips->setText(tr("Downloading"));
        m_topTips->setVisible(true);
        m_botTips->setText(tr("Downloading the driver for %1, please wait...").arg(new_driver_name));
        m_botTips->setVisible(true);
        m_vendorIcon->setVisible(false);
        m_vendorName->setVisible(false);
        m_resolutionsWidget->setVisible(false);
        m_toggleButton->setVisible(false);
        m_progress->setVisible(true);
        m_progress->start();
    } else {
        int processValue = state.toInt();
        m_progress->setValue(processValue);
    }

}

void MainWindow::onPrepareFinished(bool success)
{
    if (!m_started)
           return;

    m_progress->setVisible(false);
    m_progress->stop();
    m_tipsIcon->setVisible(true);

    if (!success) {
       m_topTips->setText(tr("Download failed"));
       m_botTips->setText(tr("Sorry, switch failed"));
       m_tipsIcon->setPixmap(Utils::hidpiPixmap(":/resources/icons/fail.svg", QSize(128, 128)));
       m_okButton->setVisible(true);
       m_okButton->setFocus();
    } else {
       m_topTips->setText(tr("Download Successful"));
       if (!m_devices.empty())
           m_botTips->setText(tr("Please reboot to test the driver.\n\nIf no signal, please confirm whether the monitor output port is connected correctly."));
       else
           m_botTips->setText(tr("Please reboot to test the driver"));
       m_tipsIcon->setPixmap(Utils::hidpiPixmap(":/resources/icons/success.svg", QSize(128, 128)));
       m_rebootButton->setVisible(true);
       m_rebootButton->setFocus();

//       QFile installerDesktopFileSource(INSTALLER_DESKTOP_FILE_SOURCE);
//       if (installerDesktopFileSource.exists())
//           installerDesktopFileSource.copy(QDir::rootPath() + INSTALLER_ROOT_DESKTOP_FILE_DEST);
//       else
//           qDebug() << INSTALLER_DESKTOP_FILE_SOURCE << "do not exists!";
    }
}
