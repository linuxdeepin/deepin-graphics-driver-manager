
#include "mainwindow.h"
#include "widgets/resolutionwidget.h"

#include <QGuiApplication>
#include <QScreen>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QLabel>

#include <DTitlebar>

MainWindow::MainWindow(QWidget *parent) :
    DMainWindow(parent),

    m_resolutions(ResolutionsBuilder(m_devInfo).build())
{
    m_toggleButton = new QPushButton;
    m_toggleButton->setText(tr("Toggle"));
    m_toggleButton->setFixedHeight(38);

    m_okButton = new QPushButton;
    m_okButton->setText(tr("OK"));
    m_okButton->setFixedHeight(38);

    m_resolutionsIcon = new QLabel;
    m_resolutionsIcon->setAlignment(Qt::AlignCenter);
    m_resolutionsIcon->setPixmap(QPixmap(":/resources/icons/" + m_resolutions.iconName()));
    m_vendorsName = new QLabel;
    m_vendorsName->setWordWrap(true);
    m_vendorsName->setAlignment(Qt::AlignCenter);
    m_vendorsName->setText(m_devInfo.devices().toList().join('\n'));

    m_resolutionsLayout = new QVBoxLayout;
    m_resolutionsWidget = new QWidget;
    m_resolutionsWidget->setLayout(m_resolutionsLayout);
    m_resolutionsWidget->setObjectName("ResolutionsWidget");
    m_resolutionsWidget->setStyleSheet("QWidget #ResolutionsWidget {"
                                       "border: 1px solid #eee;"
                                       "border-radius: 3px;"
                                       "}");

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_resolutionsIcon);
    centralLayout->addWidget(m_vendorsName);
    centralLayout->addStretch();
    centralLayout->addWidget(m_resolutionsWidget);
    centralLayout->addStretch();
    centralLayout->addWidget(m_toggleButton);
    centralLayout->addWidget(m_okButton);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(70, 0, 70, 30);

    DTitlebar *tbar = titlebar();
    tbar->setTitle(QString());
    tbar->setWindowFlags(tbar->windowFlags() & ~Qt::WindowMaximizeButtonHint);

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(centralLayout);

    setFixedSize(440, 600);
    move(qApp->primaryScreen()->geometry().center() - rect().center());

    QTimer::singleShot(0, this, &MainWindow::loadResolutions);
}

MainWindow::~MainWindow()
{
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

void MainWindow::loadResolutions()
{
    for (const auto &r : m_resolutions.resolutions())
    {
        ResolutionWidget *rw = new ResolutionWidget(r);
        m_resolutionsLayout->addWidget(rw);

        connect(rw, &ResolutionWidget::clicked, this, &MainWindow::onResolutionSelected);
    }
}

void MainWindow::onResolutionSelected()
{
    ResolutionWidget *rw = static_cast<ResolutionWidget *>(sender());

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
}