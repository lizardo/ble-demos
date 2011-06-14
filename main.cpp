#include <QApplication>
#include <QObject>
#include <QtDBus>

#include "types.h"
#include "manager.h"
#include "adapter.h"
#include "proximity.h"

typedef QMap<QString, QVariant> PropertyMap;
Q_DECLARE_METATYPE(PropertyMap)

class ProxClass: public QObject
{
	Q_OBJECT
public slots:
	void propertyChanged(const QString &property, const QDBusVariant &value);
};

void ProxClass::propertyChanged(const QString &property, const QDBusVariant &value)
{
	Q_UNUSED(value);
	qWarning() << property;
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	QString hci(argv[1]);
	QString dba(argv[2]);

	QDBusConnection dbus = QDBusConnection::systemBus();

	qWarning() << "Connecting...";
	if (dbus.isConnected())
		qWarning() << "Connected!";
/*
	foreach (QString name, reply.value())
		qDebug() << name;
*/

	org::bluez::Manager manager(BLUEZ_SERVICE_NAME, BLUEZ_MANAGER_PATH, dbus);

	qWarning() << "Looking for adapter..." <<  hci;
	QDBusReply<QDBusObjectPath> obReply = manager.FindAdapter(hci);
	if (!obReply.isValid())
		obReply = manager.DefaultAdapter();

	if (!obReply.isValid()) {
		qWarning() << "Error:" << obReply.error();
		exit(1);
	}

	qDebug() << obReply.value().path();

	org::bluez::Adapter adapter(BLUEZ_SERVICE_NAME,
					obReply.value().path(), dbus);

	qWarning() << "Looking for device... ";
	obReply = adapter.FindDevice(dba);

	if (!obReply.isValid()) {
		qWarning() << "Error:" << obReply.error();
		exit(1);
	}

	qDebug() << obReply.value().path();

	qWarning() << "Checking proximity capacity...";
	org::bluez::Proximity proximity(BLUEZ_SERVICE_NAME,
				obReply.value().path(), dbus);

	QDBusReply<PropertyMap> properties = proximity.GetProperties();
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


/*
	QMapIterator<QString, QVariant> m(properties.value());
	while (m.hasNext())
		qDebug() << m.key() << m.value();

		*/
	ProxClass *proxClass = new ProxClass();

	QObject::connect(
		&proximity,
		SIGNAL(PropertyChanged(const QString &, const QDBusVariant &)),
		proxClass,
		SLOT(propertyChanged(const QString &, const QDBusVariant &)));

	app.exec();
}

#include "main.moc"
