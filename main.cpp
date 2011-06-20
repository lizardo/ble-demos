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

signals:
	void alarmCalled(QString type);

public slots:
	void propertyChanged(const QString &property, const QDBusVariant &value);
	void thresholdChanged(int value);
	void pathlossChanged(int value);

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

void ProxClass::propertyChanged(const QString &property, const QDBusVariant &value)
{
	Q_UNUSED(value);
	qWarning() << property;

	if (property == "PathLoss")
		emit alarmCalled(property);
}

void ProxClass::thresholdChanged(int value)
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

void ProxClass::pathlossChanged(int value)
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


		*/

	Monitor *monitor = new Monitor();

	QObject::connect(
		monitor, SIGNAL(thresholdChanged(int)),
		proxClass, SLOT(thresholdChanged(int)));

	QObject::connect(
		monitor, SIGNAL(pathlossChanged(int)),
		proxClass, SLOT(pathlossChanged(int)));

	QObject::connect(
		proxClass, SIGNAL(alarmCalled(QString)),
		monitor, SLOT(playAlarm(QString)));

	monitor->show();
	app.exec();
}

#include "main.moc"
