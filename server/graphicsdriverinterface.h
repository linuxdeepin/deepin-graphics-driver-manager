#ifndef GRAPHICSDRIVERINTERFACE_H_
#define GRAPHICSDRIVERINTERFACE_H_

#include <QObject>
#include <QStringList>
#include "graphicsdeviceinfo.h"



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
    QStringList GetDevice();


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
    void PrepareInstall(int resolutionId);

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
    QString GetNewDriverName();

    /*************************************************
    Function:       // 函数名称
    Description:    // 函数功能、性能等的描述
    Input:          // 输入参数说明，包括每个参数的作用、取值说明及参数间关系。
    Output:         // 对输出参数的说明。
    Return:         // 函数返回值的说明
    Others:         // 其它说明
    *************************************************/
    QString GetOldDriverName();

Q_SIGNALS:
    void PreInstallState(QString state);
    void RealInstallState(QString state);
private:
    /*************************************************
    Function:       // 函数名称
    Description:    // 函数功能、性能等的描述
    Input:          // 输入参数说明，包括每个参数的作用、取值说明及参数间关系。
    Output:         // 对输出参数的说明。
    Return:         // 函数返回值的说明
    Others:         // 其它说明
    *************************************************/
    void BuildResolution();

    /*************************************************
    Function:       // 函数名称
    Description:    // 函数功能、性能等的描述
    Input:          // 输入参数说明，包括每个参数的作用、取值说明及参数间关系。
    Output:         // 对输出参数的说明。
    Return:         // 函数返回值的说明
    Others:         // 其它说明
    *************************************************/
    void InstallDriver(int resolutionId);

    /*************************************************
    Function:       // 函数名称
    Description:    // 函数功能、性能等的描述
    Input:          // 输入参数说明，包括每个参数的作用、取值说明及参数间关系。
    Output:         // 对输出参数的说明。
    Return:         // 函数返回值的说明
    Others:         // 其它说明
    *************************************************/
    void RemoveDriver(int resolutionId);
private:
    GraphicsDeviceInfo m_devInfo;
};

#endif
