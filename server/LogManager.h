#pragma once

#include <QtCore>

#include <Logger.h>
#include <ConsoleAppender.h>
#include <RollingFileAppender.h>

class DRMLogManager
{
public:
    static void setSystemLog(bool sys);
    static void registerConsoleAppender();
    static void registerFileAppender();
    static QString getlogFilePath();
    static void setLogFormat(const QString& format);

signals:

public slots:

private:
    bool systemLog = false;
    QString m_format;
    QString m_logPath;
    Dtk::Core::ConsoleAppender* m_consoleAppender;
    Dtk::Core::RollingFileAppender* m_rollingFileAppender;

    void initConsoleAppender();
    void initRollingFileAppender();
    QString joinPath(const QString& path, const QString& fileName);

    inline static DRMLogManager* instance(){
        static DRMLogManager instance;
        return &instance;
    }
    explicit DRMLogManager();
    ~DRMLogManager();
    DRMLogManager(const DRMLogManager &);
    DRMLogManager & operator = (const DRMLogManager &);
};

