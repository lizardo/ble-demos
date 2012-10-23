/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -v -c DuiControlPanelIf -p duicontrolpanelif_p.h:duicontrolpanelif.cpp interfaces/com.nokia.DuiControlPanelIf.xml
 *
 * qdbusxml2cpp is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef DUICONTROLPANELIF_P_H_1330548233
#define DUICONTROLPANELIF_P_H_1330548233

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.nokia.DuiControlPanelIf
 */
class DuiControlPanelIf: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.nokia.DuiControlPanelIf"; }

public:
    DuiControlPanelIf(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~DuiControlPanelIf();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<bool> appletPage(const QStringList &params)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(params);
        return asyncCallWithArgumentList(QLatin1String("appletPage"), argumentList);
    }

    inline QDBusPendingReply<bool> appletPage(const QString &params)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(params);
        return asyncCallWithArgumentList(QLatin1String("appletPage"), argumentList);
    }

    inline QDBusPendingReply<> categoryPage(const QString &category)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(category);
        return asyncCallWithArgumentList(QLatin1String("categoryPage"), argumentList);
    }

    inline QDBusPendingReply<> launch()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("launch"), argumentList);
    }

    inline QDBusPendingReply<> mainPage()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("mainPage"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void closeAppletLaunchers();
};

namespace com {
  namespace nokia {
    typedef ::DuiControlPanelIf DuiControlPanelIf;
  }
}
#endif