/* monitor.cpp -- This file implements the Monitor class. */

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

#include <QStringListModel>

#include "monitor.h"

typedef QMap<QString, QVariant> PropertyMap;
Q_DECLARE_METATYPE(PropertyMap)

class DeviceListModel : public QStringListModel {
public:
    DeviceListModel(QObject* parent) : QStringListModel(parent)
    {
        QHash<int, QByteArray> roleNames;
        roleNames.insert(Qt::DisplayRole, "title");
        setRoleNames(roleNames);
    }
};

Monitor::Monitor()
    : manager(NULL), adapter(NULL), device(NULL), m_deviceModel(new DeviceListModel(this)),
      m_threshold(-1)
{
    QDBusConnection dbus = QDBusConnection::systemBus();

    manager = new Manager(BLUEZ_SERVICE_NAME, BLUEZ_MANAGER_PATH, dbus);

    setAdapter();
}


void Monitor::lock()
{

    qDebug() << "lock:";

/*
QDBusConnection bus = QDBusConnection::sessionBus();
QDBusInterface dbus_iface("com.nokia.mce",  "/com/nokia/mce/request",
                          "com.nokia.mce.request",
                          QDBusConnection::systemBus());
qDebug() << dbus_iface.call("req_tklock_mode_change").arguments().at(0);
*/

    QDBusMessage m = QDBusMessage::createMethodCall("com.nokia.mce", "/com/nokia/mce/request", "com.nokia.mce.request", "req_tklock_mode_change");

    QList<QVariant> args;
    args << "locked";
    m.setArguments(args);

    QDBusConnection::systemBus().send(m);
}

void Monitor::unlock()
{

    qDebug() << "unlock:";

/*
QDBusConnection bus = QDBusConnection::sessionBus();
QDBusInterface dbus_iface("com.nokia.mce",  "/com/nokia/mce/request",
                          "com.nokia.mce.request",
                          QDBusConnection::systemBus());
qDebug() << dbus_iface.call("req_tklock_mode_change").arguments().at(0);
*/

    QDBusMessage m = QDBusMessage::createMethodCall("com.nokia.mce", "/com/nokia/mce/request", "com.nokia.mce.request", "req_tklock_mode_change");

    QList<QVariant> args;
    args << "unlocked";
    m.setArguments(args);

    QDBusConnection::systemBus().send(m);
}

Monitor::~Monitor()
{
    destroyDevices();
}

void Monitor::destroyDevices()
{
    while (!devices.isEmpty())
        delete devices.takeFirst();

    delete m_deviceModel;
}

void Monitor::setAdapter()
{
    if (manager == NULL) {
        qWarning() << "Invalid manager..";
        return;
    }

    if (adapter) {
        destroyDevices();
        delete adapter;
    }

    qWarning() << "Looking for adapter...";
    QDBusReply<QDBusObjectPath> obReply;

    obReply = manager->DefaultAdapter();

    if (!obReply.isValid()) {
        qWarning() << "Error:" << obReply.error();
        return;
    }

    qDebug() << obReply.value().path();

    adapter = new Adapter(BLUEZ_SERVICE_NAME, obReply.value().path(),
                        QDBusConnection::systemBus());

    lookDevices();
}

void Monitor::setDevice(int index)
{
    device = devices.at(index);

    qWarning() << "Checking proximity capacity...";
    proximity = new Proximity(BLUEZ_SERVICE_NAME,
                device->path(), QDBusConnection::systemBus());

    QObject::connect(
        proximity,
        SIGNAL(PropertyChanged(const QString &, const QDBusVariant &)),
        this,
        SLOT(propertyChanged(const QString &, const QDBusVariant &)));

    QDBusReply<PropertyMap> properties = proximity->GetProperties();
    if (!properties.isValid()) {
        qDebug() << "Error: " << properties.error();
        exit(1);
    }

    QMap<QString, QVariant> p = properties.value();
    foreach (QString k, properties.value().keys())
        propertyChanged(k, QDBusVariant(p.value(k)));
}

void Monitor::checkServices(QString path)
{
    Device *device = new Device(BLUEZ_SERVICE_NAME, path,
                    QDBusConnection::systemBus());

    QDBusReply<PropertyMap> properties = device->GetProperties();

    QVariant uuids = properties.value().value("UUIDs");

    if (uuids.toStringList().contains(IMMEDIATE_ALERT_UUID, Qt::CaseInsensitive)) {
        devices.append(device);
        return;
    }

    delete device;
}

QStringList Monitor::devicesName()
{
    QStringList list;

    foreach(Device *d, devices) {
        QDBusReply<PropertyMap> properties = d->GetProperties();

        list << QVariant(properties.value().value("Name")).toString();
    }

    return list;
}

void Monitor::lookDevices(void)
{
    qWarning() << "Looking for devices... ";
    QDBusReply<QList<QDBusObjectPath> > slReply = adapter->ListDevices();
    QList<QDBusObjectPath> list;
    QStringList device_name;

    if (!slReply.isValid()) {
        qWarning() << "Error: " << slReply.error();
        return;
    }

    list = slReply.value();
    for (int i = 0; i < list.count(); i++) {
        checkServices(list.at(i).path());
    }

    device_name = devicesName();
    m_deviceModel->setStringList(device_name);
}

void Monitor::propertyChanged(const QString &property, const QDBusVariant &value)
{
    if (property == "SignalLevel") {
        int v = -1;

        if (value.variant().toString() == "unknown")
            v = -1;
        else if (value.variant().toString() == "weak")
            v = 0;
        else if (value.variant().toString() == "regular")
            v = 1;
        else if (value.variant().toString() == "good")
            v = 2;

        qWarning() << property << m_threshold << v;

        if (m_threshold > v)
            lock();
        else
            unlock();
    }

    emit propertyValue(property, value.variant().toString());
}

void Monitor::onImmediateAlertChange(int value)
{
    QVariant arg;

    switch (value) {
    case 0:
        arg = QString("none");
        break;
    case 1:
        arg = QString("mild");
        break;
    case 2:
        arg = QString("high");
        break;
    }

    proximity->SetProperty("ImmediateAlertLevel", QDBusVariant(arg));
}

void Monitor::onLinkLossChange(int value)
{
    QVariant arg;

    switch (value) {
    case 0:
        arg = QString("none");
        break;
    case 1:
        arg = QString("mild");
        break;
    case 2:
        arg = QString("high");
        break;
    }

    proximity->SetProperty("LinkLossAlertLevel", QDBusVariant(arg));
}

void Monitor::onPathlossChange(int value)
{
    qWarning() << "mthreshold:" << m_threshold << value;
    m_threshold = value;
}

QAbstractItemModel* Monitor::getDeviceModel() const
{
    return m_deviceModel;
}
