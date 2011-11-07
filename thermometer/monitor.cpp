/* monitor.cpp -- This file implements the Monitor class. */

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

#include <QObject>
#include <QtDBus>
#include <QTimer>

#include "monitor.h"
#include "device.h"
#include "manager.h"
#include "characteristic.h"
#include <QStringListModel>

using namespace org::bluez;

class DeviceListModel : public QStringListModel {
public:
    DeviceListModel(QObject* parent) : QStringListModel(parent)
    {
        QHash<int, QByteArray> roleNames;
        roleNames.insert(Qt::DisplayRole, "title");
        setRoleNames(roleNames);
    }
};

Monitor::Monitor(QString hci) : QDBusAbstractAdaptor(QCoreApplication::instance()), m_manager(0), m_adapter(0), m_device(0), m_value(-1), m_skinContact(false), m_hci(hci), m_deviceModel(new DeviceListModel(this))
{
    QTimer::singleShot(0, this, SLOT(delayedInitialization()));
}

Monitor::~Monitor()
{
    destroyDevices();
}

void Monitor::delayedInitialization()
{
    m_manager = new Manager(BLUEZ_SERVICE_NAME, BLUEZ_MANAGER_PATH, QDBusConnection::systemBus(), this);
    setAdapter(m_hci);
}


void Monitor::destroyDevices()
{
    qDeleteAll(m_devices);
    m_devices.clear();
}

void Monitor::setAdapter(QString hci)
{
    if (!m_manager) {
        qWarning() << "Invalid manager..";
        return;
    }

    if (m_adapter) {
        destroyDevices();
        delete m_adapter;
    }

    qWarning() << "Looking for adapter...";
    QDBusReply<QDBusObjectPath> obReply;
    if (hci.isEmpty())
        obReply = m_manager->DefaultAdapter();
    else
        obReply = m_manager->FindAdapter(hci);

    if (!obReply.isValid()) {
        qWarning() << "Error:" << obReply.error();
        return;
    }

    qDebug() << obReply.value().path();

    m_adapter = new Adapter(BLUEZ_SERVICE_NAME, obReply.value().path(),
                        QDBusConnection::systemBus());

    lookDevices();
}

void Monitor::setDevice(int index)
{
    qWarning() << "Setting device..";
    if (index < 0 || index >= m_devices.count()) {
        qWarning() << "Device index out of range.";
        return;
    }

    Device* device = m_devices[index];
    if (m_device == device)
        return;
    m_device = device;


    QDBusArgument services = device->GetProperties().value()["Services"].value<QDBusArgument>();
    services.beginArray();
    while (!services.atEnd()) {
        QDBusObjectPath path;
        services >> path;

        Characteristic service(BLUEZ_SERVICE_NAME, path.path(), QDBusConnection::systemBus());
        QVariantMap props = service.GetProperties().value();
        QString serviceUUID = props["UUID"].toString().toUpper();
        if (serviceUUID == HEALTH_THERMOMETER_UUID) {
            service.RegisterCharacteristicsWatcher(QDBusObjectPath(MONITOR_OBJPATH));
	    qWarning() << "Discovering Characteristics...";
            QList<QDBusObjectPath> paths = service.DiscoverCharacteristics().value();
            foreach (QDBusObjectPath p, paths) {
                Characteristic charac(BLUEZ_SERVICE_NAME, p.path(), QDBusConnection::systemBus());
                QString characUUID = charac.GetProperties().value()["UUID"].toString().toUpper();
                if (characUUID == HT_MEASUREMENT_UUID) {
                    qDebug() << "Temperature Measurement found. Configuring..";
                    charac.SetProperty("Config", QDBusVariant("indicate"));
                    break;
                }
            }
        }
    }
    services.endArray();
}

bool Monitor::checkServices(Device* device) const
{
    QDBusReply<PropertyMap> properties = device->GetProperties();
    QVariant uuids = properties.value().value("UUIDs");

    return uuids.toStringList().contains(HEALTH_THERMOMETER_UUID, Qt::CaseInsensitive);
}

void Monitor::lookDevices(void)
{
    qWarning() << "Looking for devices... ";
    QDBusReply<QList<QDBusObjectPath> > slReply = m_adapter->ListDevices();

    if (!slReply.isValid()) {
        qWarning() << "Error: " << slReply.error();
        return;
    }

    QList<QDBusObjectPath> obpList = slReply.value();
    for (int i = 0; i < obpList.count(); i++) {
        Device* device = new Device(BLUEZ_SERVICE_NAME, obpList[i].path(),QDBusConnection::systemBus());
        if (checkServices(device))
            m_devices << device;
        else
            delete device;
    }
    // Fill the device model
    QStringList list;
    foreach(Device* d, m_devices) {
        QDBusReply<PropertyMap> properties = d->GetProperties();
        list << properties.value().value("Name").toString();
    }
    m_deviceModel->setStringList(list);
    qDebug() << "done! " << list.count() << " devices found.";
}

void Monitor::ValueChanged(const QDBusObjectPath&, const QByteArray& value)
{
    if (value.count() < 2)
        return;
    qWarning("HR Values: %X %X", value[0], value[1]);
    m_skinContact = value[0] == 4;
    m_value = value[1];
    emit valueChangedSignal();
}

QAbstractItemModel* Monitor::getDeviceModel() const
{
    return m_deviceModel;
}

