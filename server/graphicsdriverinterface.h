#ifndef GRAPHICSDRIVERINTERFACE_H_
#define GRAPHICSDRIVERINTERFACE_H_

#include <QObject>
#include <QStringList>
#include "graphicsdeviceinfo.h"
#include "resolutions.h"
#include <QProcess>
#include <QDebug>
#include <QDir>

inline const QString scriptAbsolutePath(const QString &scriptName)
{
#ifdef QT_DEBUG
    return QDir::currentPath() + "/scripts/" + scriptName;
#else
    return "/usr/lib/deepin-graphics-driver-manager/" + scriptName;
#endif
}

// #ifdef QT_DEBUG
#define QPROCESS_DUMP(Process) \
    connect(Process, &QProcess::readyReadStandardOutput, Process, [=] { qDebug().noquote() << proc->readAllStandardOutput(); }); \
    connect(Process, &QProcess::readyReadStandardError, Process, [=] { qWarning().noquote() << proc->readAllStandardError(); });
// #else
// #define QPROCESS_DUMP(Process) Q_UNUSED(Process)
// #endif

#define QPROCESS_DELETE_SELF(Process) \
    connect(Process, static_cast<void (QProcess::*)(int)>(&QProcess::finished), Process, &QProcess::deleteLater);

#define EXECUTE_SCRIPT(Process, Script) \
    Process->start("/bin/bash", QStringList() << "-x" << scriptAbsolutePath(Script));

#define EXECUTE_SCRIPT_ROOT(Process, Script) \
    Process->start(QStringList() << "/bin/bash" << "-x" << scriptAbsolutePath(Script));


class GraphicsDriverInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.daemon.GraphicsDriver")
public:
    explicit GraphicsDriverInterface(QObject* parent  = nullptr);
    virtual ~GraphicsDriverInterface();
public slots:
    /*************************************************
    Function:       // 函数名称
    Description:    // 函数功能、性能等的描述
    Input:          // 输入参数说明，包括每个参数的作用、取值说明及参数间关系。
    Output:         // 对输出参数的说明。
    Return:         // 函数返回值的说明
    Others:         // 其它说明
    *************************************************/
    QString GetDevice();


    /*************************************************
    Function:       // 函数名称
    Description:    // 函数功能、性能等的描述
    Input:          // 输入参数说明，包括每个参数的作用、取值说明及参数间关系。
    Output:         // 对输出参数的说明。
    Return:         // 函数返回值的说明
    Others:         // 其它说明
    *************************************************/
    QString GetResolutionTitle();

    /*************************************************
    Function:       // 函数名称
    Description:    // 函数功能、性能等的描述
    Input:          // 输入参数说明，包括每个参数的作用、取值说明及参数间关系。
    Output:         // 对输出参数的说明。
    Return:         // 函数返回值的说明
    Others:         // 其它说明
    *************************************************/
    void PrepareInstall(QString name);

    /*************************************************
    Function:       // 函数名称
    Description:    // 函数功能、性能等的描述
    Input:          // 输入参数说明，包括每个参数的作用、取值说明及参数间关系。
    Output:         // 对输出参数的说明。
    Return:         // 函数返回值的说明
    Others:         // 其它说明
    *************************************************/
    void TestInstall();

    /*************************************************
    Function:       // 函数名称
    Description:    // 函数功能、性能等的描述
    Input:          // 输入参数说明，包括每个参数的作用、取值说明及参数间关系。
    Output:         // 对输出参数的说明。
    Return:         // 函数返回值的说明
    Others:         // 其它说明
    *************************************************/
    bool IsTestSuccess();

    /*************************************************
    Function:       // 函数名称
    Description:    // 函数功能、性能等的描述
    Input:          // 输入参数说明，包括每个参数的作用、取值说明及参数间关系。
    Output:         // 对输出参数的说明。
    Return:         // 函数返回值的说明
    Others:         // 其它说明
    *************************************************/
    void RealInstaller();
    
    /*************************************************
    Function:       // 函数名称
    Description:    // 函数功能、性能等的描述
    Input:          // 输入参数说明，包括每个参数的作用、取值说明及参数间关系。
    Output:         // 对输出参数的说明。
    Return:         // 函数返回值的说明
    Others:         // 其它说明
    *************************************************/
    QString GetOldDriverName();

    /*************************************************
    Function:       // 函数名称
    Description:    // 函数功能、性能等的描述
    Input:          // 输入参数说明，包括每个参数的作用、取值说明及参数间关系。
    Output:         // 对输出参数的说明。
    Return:         // 函数返回值的说明
    Others:         // 其它说明
    *************************************************/
    QString GetNewDriverName();

    /*************************************************
    Function:       // 函数名称
    Description:    // 函数功能、性能等的描述
    Input:          // 输入参数说明，包括每个参数的作用、取值说明及参数间关系。
    Output:         // 对输出参数的说明。
    Return:         // 函数返回值的说明
    Others:         // 其它说明
    *************************************************/
    QString GetCurrDriverName();

Q_SIGNALS:
    void PreInstallState(QString state);
    void RealInstallState(QString state);
   
private:
    GraphicsDeviceInfo m_devInfo;
    Resolutions m_resolutions;
};

#endif
