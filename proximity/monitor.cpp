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

#define LE_LINK 0x80

// This is the device ID, the same used for naming
// bluetooth adapters on Linux (i.e. the X in hciX)
// N9 has only hci0
#define HCI_DEV_ID 0

#include <QStringListModel>

#include <sys/ioctl.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include "monitor.h"

typedef QMap<QString, QVariant> PropertyMap;
Q_DECLARE_METATYPE(PropertyMap)

// This flag will control when the lock will work if SignalLevel is
// "unknown".
static bool canLock = false;

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
/*
QDBusConnection bus = QDBusConnection::sessionBus();
QDBusInterface dbus_iface("com.nokia.mce",  "/com/nokia/mce/request",
                          "com.nokia.mce.request",
                          QDBusConnection::systemBus());
qDebug() << dbus_iface.call("req_tklock_mode_change").arguments().at(0);
*/

    qDebug() << "lock";

    if (canLock) {
        QDBusMessage m = QDBusMessage::createMethodCall("com.nokia.mce", "/com/nokia/mce/request",
                                                        "com.nokia.mce.request", "req_tklock_mode_change");
        QList<QVariant> args;
        args << "locked";
        m.setArguments(args);

        QDBusConnection::systemBus().send(m);
     }
}

void Monitor::unlock()
{
/*
QDBusConnection bus = QDBusConnection::sessionBus();
QDBusInterface dbus_iface("com.nokia.mce",  "/com/nokia/mce/request",
                          "com.nokia.mce.request",
                          QDBusConnection::systemBus());
qDebug() << dbus_iface.call("req_tklock_mode_change").arguments().at(0);
*/

    QDBusMessage m = QDBusMessage::createMethodCall("com.nokia.mce", "/com/nokia/mce/request", "com.nokia.mce.request", "req_tklock_mode_change");

    qDebug() << "unlock";

    QList<QVariant> args;
    args << "unlocked";
    m.setArguments(args);

    QDBusConnection::systemBus().send(m);
    canLock = true;
}

Monitor::~Monitor()
{
    destroyDevices();
}

void Monitor::destroyDevices()
{
    while (!devices.isEmpty()) {
        delete devices.takeFirst();
        m_address.removeFirst();
    }

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
    qDebug() << "Checking proximity capacity...";

    if (index < 0 || index >= devices.count()) {
        qDebug() << "Device index out of range";
        return;
    }

    device = devices.at(index);

    proximity = new ProximityMonitor(BLUEZ_SERVICE_NAME,
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
        m_address << QVariant(properties.value().value("Address")).toString();
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
        else {
            canLock = true;
            if (value.variant().toString() == "weak")
                v = 0;
            else if (value.variant().toString() == "regular")
                v = 1;
            else if (value.variant().toString() == "good")
                v = 2;
        }

        // If m_threshold is -1 so the lock/unlock is disabled. Nothing to do
        if (m_threshold != -1) {
            if (m_threshold > v)
                lock();
            else
                unlock();
        }
    }

    qDebug() << property << value.variant().toString();

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

void Monitor::onUnlockChange(int value)
{
    m_threshold = value;
}

QAbstractItemModel* Monitor::getDeviceModel() const
{
    return m_deviceModel;
}

static int find_conn(int s, int dev_id, long arg)
{
    struct hci_conn_list_req *cl;
    struct hci_conn_info *ci;
    int i;

    cl = (struct hci_conn_list_req *)malloc(10 * sizeof(*ci) + sizeof(*cl));
    if (!cl) {
        qWarning() << "Can't allocate memory";
        return 1;
    }
    cl->dev_id = dev_id;
    cl->conn_num = 10;
    ci = cl->conn_info;

    if (ioctl(s, HCIGETCONNLIST, (void *) cl)) {
        qWarning() << "Can't get connection list";
        return 1;
    }

    for (i = 0; i < cl->conn_num; i++, ci++)
        if (!bacmp((bdaddr_t *) arg, &ci->bdaddr)) {
            free(cl);
            return 1;
        }

    free(cl);
    return 0;
}

int Monitor::readRSSI()
{
    struct hci_conn_info_req *cr;
    bdaddr_t bdaddr;
    int8_t rssi;
    int dd, dev_id, index;

    index = devices.indexOf(device);
    if (index < 0) {
        qWarning() << "No device available/selected";
        return 1;
    }

    str2ba(qPrintable(m_address.value(index)), &bdaddr);

    dev_id = HCI_DEV_ID;
    if (dev_id < 0) {
        dev_id = hci_for_each_dev(HCI_UP, find_conn, (long) &bdaddr);
        if (dev_id < 0) {
            qWarning() << "Error: Not connected";
            return 1;
        }
    }

    dd = hci_open_dev(dev_id);
    if (dd < 0) {
        qWarning() << "Error: HCI device open failed";
        return 1;
    }

    cr = (struct hci_conn_info_req *)malloc(sizeof(*cr) + sizeof(struct hci_conn_info));
    if (!cr) {
        qWarning() << "Error: Can't allocate memory";
        return 1;
    }

    bacpy(&cr->bdaddr, &bdaddr);
    cr->type = LE_LINK;
    if ((ioctl(dd, HCIGETCONNINFO, (unsigned long) cr)) < 0) {
        qWarning() << "Error: Get connection info failed";
        return 1;
    }

    if (hci_read_rssi(dd, htobs(cr->conn_info->handle), &rssi, 1000) < 0) {
        qWarning() << "Error: Read RSSI failed";
        return 1;
    }

    qDebug() << "RSSI return value: " << rssi;

    free(cr);

    hci_close_dev(dd);

    return rssi;
}
