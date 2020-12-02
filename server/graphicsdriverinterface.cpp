#include "graphicsdriverinterface.h"
#include <QTime>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QSettings>
#include <QTextCodec>
#include <QFile>
#include <QJsonArray>

using namespace std;

GraphicsDriverInterface::GraphicsDriverInterface(QObject* parent)
    : QObject(parent),
      m_resolutions(ResolutionsBuilder(m_devInfo).build())
{
    // TODO: Needs implementation
}

GraphicsDriverInterface::~GraphicsDriverInterface()
{
    // TODO: Needs implementation
}

bool GraphicsDriverInterface::command(const QString &cmd, const QStringList &args, QString &output) {
    QProcess *proc = new QProcess(this);
    proc->setProcessChannelMode(QProcess::MergedChannels);

    connect(proc, &QProcess::readyReadStandardOutput, this, [&]() {
        QString out = proc->readAllStandardOutput();
        //qDebug() << "comand out:" << out;
        output += out;
    });
    proc->start(cmd, args);
    proc->waitForStarted(-1);
    proc->waitForFinished();
    if ( QProcess::NormalExit != proc->exitStatus()  || proc->error() < QProcess::UnknownError){
        qWarning("Excute the comand(%s) failed!", qPrintable(cmd));
        qWarning() << "Error:" << proc->error();
        qWarning() << "Status:" << proc->exitStatus();
        return false;
    }
    return true;
}


QString GraphicsDriverInterface::GetKernelVersion() {
    QString output;
    if (!command("uname", QStringList() << "-r", output)){
        return nullptr;
    }
    return output.split("-")[0];
}

QString GraphicsDriverInterface::GetCurrPackageVersion(QString pkg_name){
    QString output;
    QStringList args;
    args << "-s" << pkg_name;
    if (!command("dpkg", args, output)){
        return nullptr;
    }
    QStringList line_list = output.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
    int index = 0;
    for(; index < line_list.size(); index++){
        if (line_list[index].contains("Version: "))
            break;
    }
    if ( index == line_list.size()){
        return nullptr;
    }
    return line_list[index].split(" ")[1].split("-")[0];
}

QString GraphicsDriverInterface::GetDepoPackageVersion(QString pkg_name){
    QString output;
    QStringList args;
    args << "show" << pkg_name;
    if (!command("apt", args, output)){
        return nullptr;
    }
    QStringList line_list = output.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
    int index = 0;
    for(; index < line_list.size(); index++){
        if (line_list[index].contains("Version: "))
            break;
    }
    if ( index == line_list.size()){
        return nullptr;
    }
    return line_list[index].split(" ")[1].split("-")[0];
}


QString GraphicsDriverInterface::GetDevice()
{
    QJsonObject devices;
    QJsonArray json_array;
    foreach (const Device device, m_devInfo.devices()){
        QJsonObject devObj;
        devObj.insert("name", device.m_name);
        devObj.insert("info", device.m_info);
        json_array.append(devObj);
    }
    devices.insert("devices", json_array);
    return QString(QJsonDocument(devices).toJson());
}

QString GraphicsDriverInterface::GetResolutionTitle()
{
    QJsonObject ResolutionTitle;
    ResolutionTitle.insert("name", m_resolutions.name());
    ResolutionTitle.insert("type", m_resolutions.type());
    QJsonArray json_array;
    foreach (const Resolution resl, m_resolutions.resolutions()){
        QJsonObject reslObj;
        reslObj.insert("name", resl.name());
        reslObj.insert("driver", resl.driver());
        reslObj.insert("title", resl.title());
        reslObj.insert("description", resl.description());
        QString curr_version;
        QString depo_version;
        if ("nvidia" == resl.driver()){
            curr_version = GetCurrPackageVersion("nvidia-driver");
            depo_version = GetDepoPackageVersion("nvidia-driver");
        }else{
            curr_version = GetKernelVersion();
            depo_version = GetKernelVersion();
        }
        reslObj.insert("currVersion", curr_version);
        reslObj.insert("depoVersion", depo_version);

        if(resl.name() == GetCurrDriverName()){
            reslObj.insert("enable", "true");
        }else{
            reslObj.insert("enable", "false");
        }
        json_array.append(reslObj);
    } 
    ResolutionTitle.insert("resolutions", json_array);
    return QString(QJsonDocument(ResolutionTitle).toJson());
}

void GraphicsDriverInterface::PrepareInstall(QString name)
{
    Resolution new_resl;
    Resolution old_resl;
    QString old_name = GetOldDriverName();
    if ( name.isEmpty() || old_name.isEmpty() ){
        qWarning("Resolution name is invalid!");
        return;
    }
    if ( name == old_name ){
        qWarning("The new resolution and the old one cannot be the same!");
        return;
    }
    new_resl = m_resolutions.getResolutionByName(name);
    if ( new_resl.isNull() ){
        qWarning("Faild to get new resolution!");
        return;
    }
    old_resl = m_resolutions.getResolutionByName(old_name);
    if ( old_resl.isNull() ){
        qWarning("Faild to get old resolution!");
        return;
    }

    const QString prepare = scriptAbsolutePath(new_resl.prepareScript());
    const QString install = scriptAbsolutePath(new_resl.installScript());
    const QString removeOld = scriptAbsolutePath(old_resl.removeScript());
    Q_ASSERT(!prepare.isEmpty() && !install.isEmpty() && removeOld.isEmpty());

    QProcess *proc = new QProcess(this);
    QPROCESS_DELETE_SELF(proc);
    proc->setProcessChannelMode(QProcess::MergedChannels);

    connect(proc, &QProcess::readyReadStandardOutput, this, [=]() {
        QString out = proc->readAllStandardOutput();
        qDebug() << proc->program() << "Prepare Install:" << out;
        if (out.contains("PROGRESS:")) {
            bool ok = false;
            int number = out.replace("PROGRESS:", "").toInt(&ok);
            qDebug() << "number = " << number;
            if (ok) Q_EMIT ReportProgress(number);
        }
    });

    const QString &exit_gltest = new_resl.keep_gltest() ? "false" : "true";
    const QString &new_driver = new_resl.name();
    const QString &lang = QLocale().name();
    const QString &sc = scriptAbsolutePath("dgradvrmgr-prepare.sh");
    const QString &old_driver = old_resl.name();

    QString cmd = QString("%1 %2 %3 %4 %5 %6 %7 %8").arg(sc, prepare, removeOld, install, old_driver, new_driver, lang, exit_gltest);
    proc->start(cmd); 
}

void GraphicsDriverInterface::TestInstall()
{
    QProcess *proc = new QProcess(this);
    QPROCESS_DELETE_SELF(proc);
    proc->setProcessChannelMode(QProcess::MergedChannels);

    connect(proc, &QProcess::readyReadStandardOutput, this, [=]() {
        QString out = proc->readAllStandardOutput();
        qDebug() << proc->program() << "Test Install:" << out;
    });
    
    const QString &cmd = scriptAbsolutePath("dgradvrmgr-test-install.sh");

    proc->start(cmd);
}

bool GraphicsDriverInterface::IsTestSuccess()
{
    const QString &config_file = scriptAbsolutePath("working-dir/config.conf");
    if (!QFile(config_file).exists())
        return  false;
    
    QSettings *settings = new QSettings(config_file, QSettings::IniFormat);
    settings->setIniCodec(QTextCodec::codecForName("UTF-8"));

    const bool gltestSuccess = settings->value("gltest-success", false).toBool();
    qDebug() << "gltestSuccess is:" << gltestSuccess;

    return gltestSuccess;
}

void GraphicsDriverInterface::RealInstaller()
{
    QProcess *proc = new QProcess(this);
    QPROCESS_DELETE_SELF(proc);
    proc->setProcessChannelMode(QProcess::MergedChannels);

    connect(proc, &QProcess::readyReadStandardOutput, this, [=]() {
        QString out = proc->readAllStandardOutput();
        qDebug() << proc->program() << "Real Install:" << out;
    });
    
    const QString &cmd = scriptAbsolutePath("dgradvrmgr-real-install.sh");

    proc->start(cmd);
    return;
}

QString GraphicsDriverInterface::GetCurrDriverName()
{
    QString statusScript = m_resolutions.statusScript();
    QString name = m_resolutions.name();
    if (statusScript.isEmpty())
        return nullptr;
    
    QProcess *proc = new QProcess;
    QPROCESS_DUMP(proc);
    QPROCESS_DELETE_SELF(proc);
    EXECUTE_SCRIPT(proc, statusScript);
    proc->waitForFinished();
    const int status = proc->exitCode();
    if ( "amd" == name ){
        switch(status){
            case 0:
                return "amdgpu";
            case 1:
                return "raedon";
            default:
                return nullptr;
        }

    }else if( "intel" == name ){
        switch(status){
            case 0:
                return "glamor";
            case 1:
                return "uxa";
            case 2:
                return "sna";
            default:
                return nullptr;
        }
    }else if("nvidia" == name ){
        switch(status){
            case 0:
                return "nouveau";
            case 1:
                return "nvidia";
            default:
                return nullptr;
        }

    }else if("nvidia_intel" == name){
        switch(status){
            case 0:
                return "glamor";
            case 1:
                return "uxa";
            case 2:
                return "sna";
            case 3:
                return "bumblebee";
            case 4:
                return "prime";
            default:
                break;
        }
    }
    return nullptr;
}

QString GraphicsDriverInterface::GetOldDriverName()
{
    const QString &config_file = scriptAbsolutePath("working-dir/config.conf");
    if (!QFile(config_file).exists()){
        qWarning("The file %s is not exists!", "working-dir/config.conf");
        return  nullptr;
    }
    
    QSettings *settings = new QSettings(config_file, QSettings::IniFormat);
    settings->setIniCodec(QTextCodec::codecForName("UTF-8"));

    return settings->value("old_driver").toString();
}

QString GraphicsDriverInterface::GetNewDriverName()
{
    const QString &config_file = scriptAbsolutePath("working-dir/config.conf");
    if (!QFile(config_file).exists()){
        qWarning("The file %s is not exists!", "working-dir/config.conf");
        return  nullptr;
    }

    QSettings *settings = new QSettings(config_file, QSettings::IniFormat);
    settings->setIniCodec(QTextCodec::codecForName("UTF-8"));

    return settings->value("new_driver").toString();
}
