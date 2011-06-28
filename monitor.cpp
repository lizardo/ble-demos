#include <QObject>
#include <QWidget>
#include <QtDBus>

#include "monitor.h"
#include "device.h"
#include "types.h"

typedef QMap<QString, QVariant> PropertyMap;
Q_DECLARE_METATYPE(PropertyMap)

Monitor::Monitor(QString hci)
{
	qWarning() << hci;

	QDBusConnection dbus = QDBusConnection::systemBus();

	manager = new org::bluez::Manager(BLUEZ_SERVICE_NAME, BLUEZ_MANAGER_PATH,
						dbus);

	qWarning() << "Looking for adapter...";
	QDBusReply<QDBusObjectPath> obReply = manager->FindAdapter(hci);
	if (!obReply.isValid())
		obReply = manager->DefaultAdapter();

	if (!obReply.isValid()) {
		qWarning() << "Error:" << obReply.error();
		exit(1);
	}

	qDebug() << obReply.value().path();

	adapter = new org::bluez::Adapter(BLUEZ_SERVICE_NAME,
						obReply.value().path(), dbus);

	init2();
}

Monitor::~Monitor()
{
	while (!devices.isEmpty())
		delete devices.takeFirst();
}

void Monitor::setDevice(int index)
{
	qWarning() << "Looking for device... ";
	device = devices.at(index);

	qWarning() << "Checking proximity capacity...";
	proximity = new org::bluez::Proximity(BLUEZ_SERVICE_NAME,
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

	qWarning() << "LinsLoss:" << p.value("LinkLossAlertLevel").toString();

	qWarning() << "PathLoss:" << p.value("PathLossAlertLevel").toString();

	qWarning() << "Threshold:" << p.value("Threshold").toString();

	foreach (QString k, properties.value().keys())
		qDebug() << k;
}

void Monitor::checkServices(QString path)
{
	org::bluez::Device *device =
			new org::bluez::Device(BLUEZ_SERVICE_NAME, path,
					QDBusConnection::systemBus());

	QDBusReply<PropertyMap> properties = device->GetProperties();

	QVariant uuids = properties.value().value("UUIDs");

	foreach (QString uuid, uuids.toStringList())
		qDebug() << uuid;

	if (uuids.toStringList().contains(IMMEDIATE_ALERT_UUID, Qt::CaseInsensitive)) {
		devices.append(device);
		return;
	}

	delete device;
}

void Monitor::init2(void)
{
	QDBusReply<QList<QDBusObjectPath> > slReply = adapter->ListDevices();
	QList<QDBusObjectPath> list;

	qDebug() << "init2";

	if (!slReply.isValid()) {
		qWarning() << "Error: " << slReply.error();
		return;
	}

	list = slReply.value();
	for (int i = 0; i < list.count(); i++) {
		checkServices(list.at(i).path());
		qWarning() <<  list.at(i).path();
	}

}

QStringList Monitor::devicesName()
{
	QStringList list;

	foreach(org::bluez::Device *d, devices) {
		QDBusReply<PropertyMap> properties = d->GetProperties();

		list << QVariant(properties.value().value("Name")).toString();
	}

	return list;
}

void Monitor::propertyChanged(const QString &property, const QDBusVariant &value)
{
	Q_UNUSED(value);
	qWarning() << property;

	if (property == "PathLoss")
		emit alarmCalled(property);
	if (property == "ThresholdAlert")
		emit alarmCalled(property);
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
