/* monitor.h -- This file declares the Monitor class which encapsulates all
   needed d-bus calls used in this program. */

/* Copyright (C) 2011 Instituto Nokia de Tecnologia

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

#include "device.h"
#include "manager.h"
#include "adapter.h"
#include "proximity.h"

class QStringListModel;

#ifndef MONITOR_H
#define MONITOR_H

using namespace org::bluez;

class Monitor: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObject* deviceModel READ getDeviceModel CONSTANT)
public:
    Monitor();
    ~Monitor();

    void setAdapter();

    QAbstractItemModel* getDeviceModel() const;

signals:
    void alarm(QString type);
    void propertyValue(QString property, QString value);

private slots:
    void updateRSSI();
    void devicePropertyChanged(const QString &property, const QDBusVariant &value);

public slots:
    void propertyChanged(const QString &property, const QDBusVariant &value);

    void onLinkLossChange(int value);
    void onImmediateAlertChange(int value);

    void onUnlockChange(int value);
    void setDevice(int index);

private:
    Manager *manager;
    Adapter *adapter;
    ProximityMonitor *proximity;
    Device *device;
    QList<Device *> devices;
    QStringList m_address;

    QStringListModel* m_deviceModel;

    int m_threshold;
    QTimer *m_rssiTimer;
    int tx_power;
    bool tx_power_valid;

    void lookDevices(void);
    void destroyDevices();
    void checkServices(QString path);
    QStringList devicesName();

    int readRSSI(int8_t *rssi);

    void unlock();
    void lock();
};

#endif
