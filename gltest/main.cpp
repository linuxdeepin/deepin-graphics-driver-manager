
#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QDebug>
#include <QTimer>
#include <QDesktopWidget>
#include <QScreen>
#include <QLabel>
#include <QProcess>
#include <QSettings>
#include <QTranslator>
#include <QThread>
#include <QProcess>

#include <GL/gl.h>
#include <GL/glut.h>

class GLTestWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit GLTestWidget(QWidget *parent = nullptr)
        : QOpenGLWidget(parent)
    {
        QThread *auto_quit_thread = new QThread();
        QTimer *auto_quit_timer = new QTimer();
        QTimer *restart_auto_quit_timer = new QTimer();

        auto_quit_timer->moveToThread(auto_quit_thread);
        auto_quit_thread->start();

        QObject::connect(auto_quit_timer, &QTimer::timeout, [=] {
            qDebug() << "detect the main thread may stuck one minute, killing";
            QProcess::startDetached("killall", QStringList() << "gltest");
        });
        QObject::connect(restart_auto_quit_timer, &QTimer::timeout,  [=] {
            auto_quit_timer->metaObject()->invokeMethod(auto_quit_timer, "start", Q_ARG(int, 60000));
        });

        auto_quit_timer->metaObject()->invokeMethod(auto_quit_timer, "start", Q_ARG(int, 60000));
        restart_auto_quit_timer->start(5000);

        m_xRotated = m_yRotated = m_zRotated = 0;

        QTimer *t = new QTimer(this);
        t->setInterval(1000 / 60);
        t->start();

        connect(t, &QTimer::timeout, this, [=] {
            m_xRotated += 1.0;
            m_yRotated += 1.0;
            m_zRotated += 1.0;
            update();
        });
    }

protected:
    void initializeGL()
    {
        glClearColor(0.3, 0.3, 0.6, 1);
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_NORMALIZE);
        // glEnable(GL_POLYGON_OFFSET_FILL);
        // glPolygonMode(GL_FRONT, GL_LINE);
        glShadeModel(GL_SMOOTH);
    }

    void resizeGL(int w, int h)
    {
        glViewport(0, 0, w, h);
    }

    void paintGL()
    {
        glMatrixMode(GL_MODELVIEW);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        GLfloat ambientColor[] = {0.6f, 0.3f, 0.7f, 1.0f};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

        GLfloat lightColor0[] = {1.0f, 0.0f, 0.0f, 1.0f};
        GLfloat lightPos0[] = {10.0f, 10.0f, 0.0f, 1.0f};
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

        GLfloat lightColor1[] = {0.4f, 0.7f, 0.2f, 1.0f};
        GLfloat lightPos1[] = {0.0f, 0.0f, -5.0f, 0.0f};
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
        glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

        // glColor3f(0.3, 0.4, 0.7);
        glRotatef(m_xRotated, 1.0, 0.0, 0.0);
        glRotatef(m_yRotated, 0.0, 1.0, 0.0);
        glRotatef(m_zRotated, 0.0, 0.0, 1.0);
        glScalef(.4, .4, .4);
        glColor3f(0.3, 0.4, 0.4);
        glutSolidTeapot(1.0f);
    }

private:
    float m_xRotated, m_yRotated, m_zRotated;
};

class GLTestWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GLTestWindow(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        m_acceptBtn = new QPushButton(tr("Apply"));
        m_cancelBtn = new QPushButton(tr("Cancel"));
        m_tipsLabel = new QLabel(tr("Please ensure the driver works normally without blurred screen and screen tearing"));
        m_tipsLabel->setAlignment(Qt::AlignCenter);

        m_glTestWidget = new GLTestWidget;
        m_glTestWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QHBoxLayout *btnsLayout = new QHBoxLayout;
        btnsLayout->addStretch();
        btnsLayout->addWidget(m_cancelBtn);
        btnsLayout->addWidget(m_acceptBtn);
        btnsLayout->addStretch();

        QVBoxLayout *centralLayout = new QVBoxLayout;
        centralLayout->addWidget(m_glTestWidget);
        centralLayout->addWidget(m_tipsLabel);
        centralLayout->addLayout(btnsLayout);

        setLayout(centralLayout);
        setFixedSize(qApp->primaryScreen()->geometry().size());
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

        connect(m_acceptBtn, &QPushButton::clicked, this, &GLTestWindow::onAccept);
        connect(m_cancelBtn, &QPushButton::clicked, this, &GLTestWindow::onCancel);
    }

    void setExitGLTest(const bool exit)
    {
        m_exit_gltest = exit;
    }

protected:
    void keyPressEvent(QKeyEvent *e)
    {
        switch (e->key())
        {
#ifdef QT_DEBUG
        case Qt::Key_Escape:    qApp->quit();       break;
#endif
        default:;
        }
    }

private slots:
    void onAccept()
    {
        QProcess::execute("/usr/lib/deepin-graphics-driver-manager/gltest-set-success.sh");
        qApp->exit(0);
    }

    void onCancel()
    {
        qApp->exit(-1);
    }

private:
    bool m_exit_gltest;
    GLTestWidget *m_glTestWidget;
    QLabel *m_tipsLabel;
    QPushButton *m_acceptBtn;
    QPushButton *m_cancelBtn;
};

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    QApplication app(argc, argv);

    QSettings settings("/usr/lib/deepin-graphics-driver-manager/working-dir/config.conf", QSettings::IniFormat);
    const bool exit_gltest = settings.value("exit_test", false).toBool();
    const QString lang = settings.value("lang", "en_US").toString();

    QTranslator translator;
    translator.load(QString("/usr/share/deepin-graphics-driver-manager/translations/deepin-graphics-driver-manager_%1.qm").arg(lang));
    app.installTranslator(&translator);

    GLTestWindow *w = new GLTestWindow;
    w->setExitGLTest(exit_gltest);
    w->show();

    return app.exec();
}

#include "main.moc"
