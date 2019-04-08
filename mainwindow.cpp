
#include "mainwindow.h"
#include "widgets/resolutionwidget.h"
#include "utils.h"

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
#define INSTALLER_DESKTOP_FILE_DEST "/.config/autostart/deepin-gradvrmgr-installer.desktop"

#define THEME_DARK "dark"
#define THEME_LIGHT "light"

const QPixmap hidpiPixmap(const QString &path, const QSize &sz)
{
    const auto ratio = qApp->devicePixelRatio();
    QPixmap iconPix = Utils::renderSVG(path, sz);
    iconPix.setDevicePixelRatio(ratio);

    return iconPix;
}

MainWindow::MainWindow(QWidget *parent) :
    DMainWindow(parent),
    m_resolutions(ResolutionsBuilder(m_devInfo).build()),
    m_qsettings(new QSettings(this)),
    m_tbMenu(new QMenu(this)),
    m_darkThemeAction(new QAction(tr("Dark theme"), this))
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
    m_progress->setTextVisible(false);
    m_progress->setFixedSize(100, 100);
    m_progress->setValue(50);
    m_progress->setVisible(false);

    m_vendorIcon = new QLabel;
    m_vendorIcon->setAlignment(Qt::AlignCenter);
    m_vendorIcon->setPixmap(hidpiPixmap(":/resources/icons/" + m_resolutions.iconName(), QSize(128, 128)));
    m_vendorName = new QLabel;
    m_vendorName->setWordWrap(true);
    m_vendorName->setAlignment(Qt::AlignCenter);
    m_vendorName->setText(m_devInfo.devices().toList().join('\n'));

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

    DTitlebar *tbar = titlebar();
    tbar->setTitle(QString());
    tbar->setIcon(QIcon(":/resources/icons/deepin-graphics-driver-manager-64px.svg"));
    tbar->setBackgroundTransparent(true);
    tbar->setMenu(m_tbMenu);
    m_tbMenu->addAction(m_darkThemeAction);
    m_tbMenu->addSeparator();

    m_darkThemeAction->setCheckable(true);

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(centralLayout);

    setFixedSize(440, 600);
    move(qApp->primaryScreen()->geometry().center() - rect().center());

    connect(m_toggleButton, &DSuggestButton::clicked, this, &MainWindow::onToggleBtnClicked);
    connect(m_rebootButton, &DSuggestButton::clicked, this, &MainWindow::onRebootBtnClicked);
    connect(m_okButton, &DSuggestButton::clicked, qApp, &QApplication::quit);
    connect(m_darkThemeAction, &QAction::toggled, this, &MainWindow::toggleDarkTheme);

    reloadTheme();
    QTimer::singleShot(0, this, &MainWindow::loadResolutions);
}

MainWindow::~MainWindow()
{
}

void MainWindow::toggleDarkTheme(bool checked)
{
    m_qsettings->setValue("theme", checked ? THEME_DARK : THEME_LIGHT);
    reloadTheme();
}

void MainWindow::reloadTheme()
{
    QString theme = m_qsettings->value("theme").toString();
    if (theme.isEmpty()) {
        theme = THEME_LIGHT;
        m_qsettings->setValue("theme", THEME_LIGHT);
    }

    m_darkThemeAction->setChecked(theme == THEME_DARK);

    QFile themeFile(theme == THEME_DARK ? ":/resources/theme/dark/dark.qss" : ":/resources/theme/light/light.qss");
    if (!themeFile.open(QFile::ReadOnly)) {
        qDebug() << "theme file not find!" << themeFile.fileName();
    }

    setStyleSheet(themeFile.readAll());

    DThemeManager::instance()->setTheme(theme);
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

void MainWindow::noResolutions()
{
    m_tipsIcon->setVisible(true);
    m_tipsIcon->setPixmap(hidpiPixmap(":/resources/icons/fail.svg", QSize(128, 128)));
    m_botTips->setText(tr("Your hardware is not supported currently, please wait for future version."));
    m_botTips->setVisible(true);
    m_resolutionsWidget->setVisible(false);
    m_toggleButton->setVisible(false);
}

void MainWindow::loadResolutions()
{
    if (m_resolutions.resolutions().isEmpty())
        return noResolutions();

    QList<ResolutionWidget *> rwList;
    for (const auto &r : m_resolutions.resolutions())
    {
        ResolutionWidget *rw = new ResolutionWidget(r);
        m_resolutionsLayout->addWidget(rw);
        rwList << rw;

        connect(rw, &ResolutionWidget::clicked, this, &MainWindow::onResolutionSelected);
    }

    const QString &script = m_resolutions.statusScript();
    if (script.isEmpty())
        return;

    QProcess *proc = new QProcess;
    QPROCESS_DUMP(proc);
    QPROCESS_DELETE_SELF(proc);

    EXECUTE_SCRIPT(proc, script);
    proc->waitForFinished();
    const int index = proc->exitCode();
    m_usedIndex = index;

    if (index >= 0 && index < rwList.size())
        emit rwList[index]->clicked();
}

void MainWindow::onResolutionSelected()
{
    ResolutionWidget *rw = static_cast<ResolutionWidget *>(sender());
    qDebug() << "Resolution selected: " << rw->resolution().name();

    const int idx = m_resolutionsLayout->indexOf(rw);
    Q_ASSERT(idx != -1);

    m_selectedIndex = idx;

    for (int i(0); i != m_resolutionsLayout->count(); ++i)
    {
        ResolutionWidget *w = static_cast<ResolutionWidget *>(m_resolutionsLayout->itemAt(i)->widget());
        w->setChecked(i == idx);
    }

    const bool changed = m_selectedIndex != m_usedIndex;
    m_toggleButton->setVisible(changed);
    m_okButton->setVisible(!changed);
    if (changed)
        m_toggleButton->setFocus();
    else
        m_okButton->setFocus();
}

void MainWindow::onToggleBtnClicked()
{
    Q_ASSERT(m_selectedIndex != m_usedIndex);

    m_started = false;

    ResolutionWidget *old_driver_widget = static_cast<ResolutionWidget *>(m_resolutionsLayout->itemAt(m_usedIndex)->widget());
    ResolutionWidget *new_driver_widget = static_cast<ResolutionWidget *>(m_resolutionsLayout->itemAt(m_selectedIndex)->widget());
    new_driver_widget->prepareInstall(old_driver_widget->resolution());

    connect(new_driver_widget, &ResolutionWidget::prepareFinished, this, &MainWindow::onPrepareFinished);
    connect(new_driver_widget, &ResolutionWidget::policyKitPassed, this, &MainWindow::onPolicyKitPassed);
}

void MainWindow::onRebootBtnClicked()
{
    QProcess::startDetached("dbus-send --print-reply --dest=com.deepin.dde.shutdownFront /com/deepin/dde/shutdownFront com.deepin.dde.shutdownFront.Restart");
}

void MainWindow::onPolicyKitPassed()
{
    m_started = true;

    ResolutionWidget *new_driver_widget = static_cast<ResolutionWidget *>(m_resolutionsLayout->itemAt(m_selectedIndex)->widget());
    const QString &new_driver_name = new_driver_widget->resolution().name();

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
}

void MainWindow::onPrepareFinished(const int exitCode)
{
    if (!m_started)
        return;

    m_progress->setVisible(false);
    m_progress->stop();
    m_tipsIcon->setVisible(true);

    if (exitCode)
    {
        m_topTips->setText(tr("Download failed"));
        m_botTips->setText(tr("Sorry, switch failed"));
        m_tipsIcon->setPixmap(hidpiPixmap(":/resources/icons/fail.svg", QSize(128, 128)));
        m_okButton->setVisible(true);
        m_okButton->setFocus();
    }
    else
    {
        m_topTips->setText(tr("Download Successful"));
        if (m_devInfo.deviceNums() > 1)
            m_botTips->setText(tr("Please reboot to test the driver.\n\nIf no signal, please confirm whether the monitor output port is connected correctly."));
        else
            m_botTips->setText(tr("Please reboot to test the driver"));
        m_tipsIcon->setPixmap(hidpiPixmap(":/resources/icons/success.svg", QSize(128, 128)));
        m_rebootButton->setVisible(true);
        m_rebootButton->setFocus();

        QFile installerDesktopFileSource(INSTALLER_DESKTOP_FILE_SOURCE);
        if (installerDesktopFileSource.exists())
            installerDesktopFileSource.copy(QDir::homePath() + INSTALLER_DESKTOP_FILE_DEST);
        else
            qDebug() << INSTALLER_DESKTOP_FILE_SOURCE << "do not exists!";
    }
}
