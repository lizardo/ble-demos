/* monitor.h -- This file declares the Monitor class which encapsulates all
   needed d-bus calls used in this program. */

/* Copyright (C) 2011 Instituto Nokia de Tecnologia

   Author: Hugo Parente Lima <hugo.lima@openbossa.org>
   Author: Sheldon Almeida Demario <sheldon.demario@openbossa.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MONITOR_H
#define MONITOR_H

#include <QObject>
#include <QDBusVariant>
#include <QList>
#include <QStringList>
#include "manager.h"
#include "adapter.h"
#include "device.h"
#include <QDBusAbstractAdaptor>

#define MONITOR_OBJPATH "/test/watcher"


class QStringListModel;

class Monitor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluez.Watcher")

    Q_PROPERTY(int value READ value NOTIFY valueChangedSignal)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusChangedSignal)
    Q_PROPERTY(QObject* deviceModel READ getDeviceModel CONSTANT)

public:
    Monitor(QString hci = QString());
    ~Monitor();

    void setAdapter(QString hci = QString());

    int value() const { return m_value; }
    QString statusMessage() const { return m_statusMessage; }
    QAbstractItemModel* getDeviceModel() const;
public slots:
    void ValueChanged(const QDBusObjectPath& characteristic, const QByteArray& value);
    void setDevice(int index);
signals:
    void valueChangedSignal();
    void statusChangedSignal();
private slots:
    void delayedInitialization();

private:
    org::bluez::Manager* m_manager;
    org::bluez::Adapter* m_adapter;
    org::bluez::Device* m_device;
    QList<org::bluez::Device*> m_devices;
    int m_value;
    QString m_statusMessage;
    QString m_hci;
    QStringListModel* m_deviceModel;

    void lookDevices(void);
    void destroyDevices();
    bool checkServices(org::bluez::Device* device) const;
};

#endif
