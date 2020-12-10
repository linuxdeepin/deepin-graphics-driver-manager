
#include "mainwindow.h"
#include "resolutionwidget.h"
#include "singleresolutionwidget.h"
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

    m_toggleButton = new QPushButton;
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

    m_okButton = new QPushButton;
    m_okButton->setText(tr("OK"));
    m_okButton->setFixedHeight(38);

    m_updateButton = new QPushButton;
    m_updateButton->setText(tr("Update"));
    m_updateButton->setFixedHeight(38);
    m_updateButton->setVisible(false);

    m_rebootButton = new QPushButton;
    m_rebootButton->setText(tr("Reboot"));
    m_rebootButton->setFixedHeight(38);
    m_rebootButton->setVisible(false);

    m_rebootLaterButton = new QPushButton;
    m_rebootLaterButton->setText(tr("Reboot later"));
    m_rebootLaterButton->setFixedHeight(38);
    m_rebootLaterButton->setVisible(false);

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
    m_resolutionsLayout->setContentsMargins(0, 8, 0, 8);
    m_resolutionsLayout->setSpacing(10);
    m_resolutionsWidget = new QWidget;
    m_resolutionsWidget->setLayout(m_resolutionsLayout);

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

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(m_rebootLaterButton);
    hBoxLayout->addSpacing(10);
    hBoxLayout->addWidget(m_rebootButton);

    centralLayout->addLayout(hBoxLayout);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(20, 10, 20, 10);
    titlebar()->setTitle(" ");
    titlebar()->setIcon(QIcon(":/resources/icons/deepin-graphics-driver-manager-64px.svg"));
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(centralLayout);

    setFixedSize(484, 682);
    move(qApp->primaryScreen()->geometry().center() - rect().center());

    connect(m_toggleButton, &QPushButton::clicked, this, &MainWindow::onToggleBtnClicked);
    connect(m_rebootButton, &QPushButton::clicked, this, &MainWindow::onRebootBtnClicked);
    connect(m_okButton, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(m_rebootLaterButton, &QPushButton::clicked, qApp, &QApplication::quit);

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
       qDebug() << devices;
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
       iconPath = ":/resources/icons/Intel.svg";
    }

    if (bAmd && !bIntel && !bNvidia) {
        iconPath = ":/resources/icons/AMD.svg";
    }

    if (bNvidia && !bIntel && !bAmd) {
        iconPath = ":/resources/icons/NVIDIA.svg";
    }

    if (bIntel && bNvidia && !bAmd) {
        iconPath = ":/resources/icons/Intel-NVIDIA.svg";
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
        m_warnning->setText(tr("Switching to the discrete graphics interface may cause a black screen."));

    } else if (resolutionRoot["type"].toInt() == INTEL_NVIDIA_USE_NVIDIA) {
        m_warnning->setVisible(true);
        m_warnning->setText(tr("Switching to the integrated graphics interface may cause a black screen."));
    }

    if (resolutions.size() == 1) {
        m_usedIndex = 0;
        Resolution solution(resolutions[0].toObject());
        ResolutionWidget *rw = new SingleResolutionWidget(m_graphicsDriver, solution);
        rw->initUI();
        m_resolutionsLayout->addWidget(rw);
        connect(rw, &ResolutionWidget::clicked, this, &MainWindow::onResolutionSelected);
        if (rw->canUpdate()) {
            m_updateButton->setVisible(true);
            m_okButton->setVisible(false);
            m_toggleButton->setVisible(false);
        }
    } else {
        int index = 0;
        for (const auto res : resolutions) {
            Resolution solution(res.toObject());
            ResolutionWidget *rw = new ResolutionWidget(m_graphicsDriver, solution);
            rw->initUI();
            m_resolutionsLayout->addWidget(rw);
            connect(rw, &ResolutionWidget::clicked, this, &MainWindow::onResolutionSelected);
            if (solution.enable()) {
                m_usedIndex = index;

                qDebug() << "m_usedIndex = " << m_usedIndex;
                if (rw->canUpdate()) {
                    m_updateButton->setVisible(true);
                    m_okButton->setVisible(false);
                    m_toggleButton->setVisible(false);
                }
            }
            qDebug() << "index = " << index;
            index++;
        }

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
    qDebug() << "m_usedIndex = " << m_usedIndex;
    qDebug() << "m_selectedIndex = " << m_selectedIndex;
    const bool changed = m_selectedIndex != m_usedIndex;

    if (changed) {
        if (rw->resolution().enable() && rw->canUpdate()) {
            m_toggleButton->setVisible(false);
            m_okButton->setVisible(false);
            m_updateButton->setVisible(true);
            //m_updateButton->setFocus();
        } else {
            m_toggleButton->setVisible(true);
            m_okButton->setVisible(false);
            //m_toggleButton->setFocus();
            m_updateButton->setVisible(false);

        }

    } else {
        if (rw->canUpdate()) {
            m_toggleButton->setVisible(false);
            m_okButton->setVisible(false);
            m_updateButton->setVisible(true);
            //m_updateButton->setFocus();
        } else {
            //m_okButton->setFocus();
            m_toggleButton->setVisible(false);
            m_okButton->setVisible(true);
            m_updateButton->setVisible(false);
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
     QProcess::startDetached("dbus-send --system --print-reply --dest=org.freedesktop.login1 /org/freedesktop/login1 org.freedesktop.login1.Manager.Reboot boolean:true");
}

void MainWindow::onPolicyKitPassed(const QString &state)
{
    ResolutionWidget *rw = static_cast<ResolutionWidget *>(sender());
    ResolutionWidget *new_driver_widget = static_cast<ResolutionWidget *>(m_resolutionsLayout->itemAt(m_selectedIndex)->widget());
    if (rw != new_driver_widget) {
        return;
    }

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
    qDebug() << "onPrepareFinished = " << success;
    ResolutionWidget *rw = static_cast<ResolutionWidget *>(sender());
    ResolutionWidget *new_driver_widget = static_cast<ResolutionWidget *>(m_resolutionsLayout->itemAt(m_selectedIndex)->widget());
    if (rw != new_driver_widget) {
        return;
    }

    if (!m_started) {
        m_botTips->setText(tr("Sorry, switch failed"));
        m_tipsIcon->setPixmap(Utils::hidpiPixmap(":/resources/icons/fail.svg", QSize(128, 128)));
        m_okButton->setVisible(true);
        m_rebootButton->setVisible(false);
        m_rebootLaterButton->setVisible(false);
    }


    m_progress->setVisible(false);
    m_progress->stop();
    m_tipsIcon->setVisible(true);

    if (!success) {
       m_topTips->setText(tr("Download failed"));
       m_botTips->setText(tr("Sorry, switch failed"));
       m_tipsIcon->setPixmap(Utils::hidpiPixmap(":/resources/icons/fail.svg", QSize(128, 128)));
       m_okButton->setVisible(true);
       m_rebootButton->setVisible(false);
       m_rebootLaterButton->setVisible(false);
       //m_okButton->setFocus();
    } else {
       m_topTips->setText(tr("Download Successful"));
       if (!m_devices.empty())
           m_botTips->setText(tr("Please reboot to test the driver.\n\nIf no signal, please confirm whether the monitor output port is connected correctly."));
       else
           m_botTips->setText(tr("Please reboot to test the driver"));
       m_tipsIcon->setPixmap(Utils::hidpiPixmap(":/resources/icons/success.svg", QSize(128, 128)));
       m_rebootButton->setVisible(true);
       m_rebootLaterButton->setVisible(true);
    }
}
