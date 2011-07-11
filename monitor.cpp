#include <QObject>
#include <QWidget>
#include <QtDBus>

#include "monitor.h"
#include "device.h"
#include "types.h"

typedef QMap<QString, QVariant> PropertyMap;
Q_DECLARE_METATYPE(PropertyMap)

Monitor::Monitor(QString hci)
	: manager(NULL), adapter(NULL), device(NULL)
{
	QDBusConnection dbus = QDBusConnection::systemBus();

	manager = new Manager(BLUEZ_SERVICE_NAME, BLUEZ_MANAGER_PATH,
						dbus);

	setAdapter(hci);
}

Monitor::~Monitor()
{
	destroyDevices();
}

void Monitor::destroyDevices()
{
	while (!devices.isEmpty())
		delete devices.takeFirst();
}

void Monitor::setAdapter(QString hci)
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
	if (hci.isEmpty())
		obReply = manager->DefaultAdapter();
	else
		obReply = manager->FindAdapter(hci);

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

void Monitor::lookDevices(void)
{
	qWarning() << "Looking for devices... ";
	QDBusReply<QList<QDBusObjectPath> > slReply = adapter->ListDevices();
	QList<QDBusObjectPath> list;

	if (!slReply.isValid()) {
		qWarning() << "Error: " << slReply.error();
		return;
	}

	list = slReply.value();
	for (int i = 0; i < list.count(); i++) {
		checkServices(list.at(i).path());
	}
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

void Monitor::propertyChanged(const QString &property, const QDBusVariant &value)
{
	qWarning() << property;

	if (property == "ThresholdAlert")
		emit alarm(property);
	else
		emit propertyValue(property, value.variant().toString());
}

void Monitor::thresholdChanged(int value)
{
	QVariant arg;

	switch (value) {
	case 0:
		arg = QString("low");
		break;
	case 1:
		arg = QString("medium");
		break;
	case 2:
		arg = QString("high");
		break;
	}

	qWarning() << arg.toString();

	proximity->SetProperty("Threshold", QDBusVariant(arg));
}

void Monitor::pathlossChanged(int value)
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

	qWarning() << arg.toString();

	proximity->SetProperty("PathLossAlertLevel", QDBusVariant(arg));
}

void Monitor::linkLossChanged(int value)
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

	qWarning() << arg.toString();

	proximity->SetProperty("LinkLossAlertLevel", QDBusVariant(arg));
}

void Monitor::findMeChanged(int value)
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

	qWarning() << arg.toString();

	proximity->SetProperty("FindMeAlertLevel", QDBusVariant(arg));
}
