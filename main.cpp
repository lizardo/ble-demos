#include <QApplication>
#include <QtDBus>

typedef QMap<QString, QVariant> PropertyMap;
Q_DECLARE_METATYPE(PropertyMap)

class ProxClass: public QObject
{
	Q_OBJECT
public slots:
	void propertyChanded(const QString &property, const QVariant &value);
};

void ProxClass::propertyChanded(const QString &property, const QVariant &value)
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


	QDBusReply<QStringList> reply = QDBusConnection::systemBus().interface()->registeredServiceNames();
	if (!reply.isValid()) {
		qDebug() << "Error:" << reply.error().message();
		exit(1);
	}
/*
	foreach (QString name, reply.value())
		qDebug() << name;
*/

	QDBusInterface dbus_iface("org.bluez", "/",
				"org.bluez.Manager", dbus);

	qWarning() << "Looking for adapter..." <<  hci;
	QDBusReply<QDBusObjectPath> adapterPath = dbus_iface.call("FindAdapter", hci);
	if (!adapterPath.isValid())
		adapterPath = dbus_iface.call("DefaultAdapter");

	if (!adapterPath.isValid()) {
		qWarning() << "Error:" << reply.error();
		exit(1);
	}

	qDebug() << adapterPath.value().path();

	QDBusInterface adapter("org.bluez", adapterPath.value().path(),
				"org.bluez.Adapter", dbus);


	qWarning() << "Looking for device... ";
	QDBusReply<QDBusObjectPath> d = adapter.call("FindDevice", dba);

	if (!d.isValid()) {
		qWarning() << "Error:" << d.error();
		exit(1);
	}

	qDebug() << d.value().path();

	qWarning() << "Checking proximity capacity...";
	QDBusInterface proximity("org.bluez", d.value().path(),
				"org.bluez.Proximity", dbus);

	QDBusReply<PropertyMap> properties = proximity.call("GetProperties");
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

	if (dbus.connect(
		QString("org.bluez"),
		d.value().path(),
		"org.bluez.Proximity",
		QString("PropertyChanged"),
		proxClass,
		SLOT(propertyChanged(QString,QVariant))))
		qDebug() << "Connection is true!";

	app.exec();
}

#include "main.moc"
