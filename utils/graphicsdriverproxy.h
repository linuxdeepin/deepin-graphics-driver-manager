/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp ../com.deepin.daemon.GraphicsDriver.xml -p graphicsdriverproxy
 *
 * qdbusxml2cpp is Copyright (C) 2017 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef GRAPHICSDRIVERPROXY_H
#define GRAPHICSDRIVERPROXY_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.deepin.daemon.GraphicsDriver
 */
class ComDeepinDaemonGraphicsDriverInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.deepin.daemon.GraphicsDriver"; }

public:
    ComDeepinDaemonGraphicsDriverInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = nullptr);

    ~ComDeepinDaemonGraphicsDriverInterface();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<QString> GetCurrDriverName()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("GetCurrDriverName"), argumentList);
    }

    inline QDBusPendingReply<QString> GetDevice()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("GetDevice"), argumentList);
    }

    inline QDBusPendingReply<QString> GetNewDriverName()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("GetNewDriverName"), argumentList);
    }

    inline QDBusPendingReply<QString> GetOldDriverName()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("GetOldDriverName"), argumentList);
    }

    inline QDBusPendingReply<QString> GetResolutionTitle()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("GetResolutionTitle"), argumentList);
    }

    inline QDBusPendingReply<bool> IsTestSuccess()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("IsTestSuccess"), argumentList);
    }

    inline QDBusPendingReply<> PrepareInstall(const QString &name)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(name);
        return asyncCallWithArgumentList(QStringLiteral("PrepareInstall"), argumentList);
    }

    inline QDBusPendingReply<> RealInstaller()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("RealInstaller"), argumentList);
    }

    inline QDBusPendingReply<> TestInstall()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("TestInstall"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void PreInstallState(const QString &state);
    void RealInstallState(const QString &state);
};

namespace com {
  namespace deepin {
    namespace daemon {
      typedef ::ComDeepinDaemonGraphicsDriverInterface GraphicsDriver;
    }
  }
}
#endif