#include <QApplication>
#include <QWidget>
#include <QObject>
#include <QtDBus>

#include "types.h"
#include "manager.h"
#include "adapter.h"
#include "proximity.h"

#include "monitor.h"

typedef QMap<QString, QVariant> PropertyMap;
Q_DECLARE_METATYPE(PropertyMap)

class ProxClass: public QObject
{
	Q_OBJECT
public:
	ProxClass(QString adapter, QString bda);

public slots:
	void propertyChanged(const QString &property, const QDBusVariant &value);
	void thresholdChanged(int value);

private:
	org::bluez::Manager *manager;
	org::bluez::Adapter *adapter;
	org::bluez::Proximity *proximity;
};

ProxClass::ProxClass(QString hci, QString dba)
{
	qWarning() << hci << dba;

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

	qWarning() << "Looking for device... ";
	obReply = adapter->FindDevice(dba);

	if (!obReply.isValid()) {
		qWarning() << "Error:" << obReply.error();
		exit(1);
	}

	qDebug() << obReply.value().path();

	qWarning() << "Checking proximity capacity...";
	proximity = new org::bluez::Proximity(BLUEZ_SERVICE_NAME,
				obReply.value().path(), dbus);

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

void ProxClass::propertyChanged(const QString &property, const QDBusVariant &value)
{
	Q_UNUSED(value);
	qWarning() << property;
}

void ProxClass::thresholdChanged(int value)
{
	qWarning() << value;
	/*
	QVariant arg;

	switch (value) {
	case 0:
		arg.setValue("low");
		break;
	case 1:
		arg.setValue("medium");
		break;
	case 2:
		arg.setValue("high");
		break;
	}

	proximity->SetProperty("Threshold", QDBusVariant(arg));
	*/
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	QString hci(argv[1]);
	QString dba(argv[2]);

	QDBusConnection dbus = QDBusConnection::systemBus();

	ProxClass *proxClass = new ProxClass(hci, dba);
	Q_UNUSED(proxClass);


/*
	QMapIterator<QString, QVariant> m(properties.value());
	while (m.hasNext())
		qDebug() << m.key() << m.value();


	QObject::connect(
		&proximity,
		SIGNAL(PropertyChanged(const QString &, const QDBusVariant &)),
		proxClass,
		SLOT(propertyChanged(const QString &, const QDBusVariant &)));
		*/

	Monitor *monitor = new Monitor();

	QObject::connect(
		monitor, SIGNAL(thresholdChanged(int)),
		proxClass, SLOT(thresholdChanged(int)));

	monitor->show();
	app.exec();
}

#include "main.moc"
