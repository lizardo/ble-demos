#include <QObject>
#include <QDBusVariant>
#include <QList>

#include "device.h"
#include "manager.h"
#include "adapter.h"
#include "proximity.h"

#ifndef MONITOR_H
#define MONITOR_H

using namespace org::bluez;

class Monitor: public QObject
{
	Q_OBJECT
public:
	Monitor(QString hci = QString());
	~Monitor();

	void setAdapter(QString hci);
	void setDevice(int index);

	Q_INVOKABLE QStringList devicesName();

signals:
	void alarm(QString type);
	void propertyValue(QString property, QString value);
	void dummy();

public slots:
	void propertyChanged(const QString &property, const QDBusVariant &value);

	void onLinkLossChange(int value);
	void onImmediateAlertChange(int value);

private:
	Manager *manager;
	Adapter *adapter;
	Proximity *proximity;
	Device *device;
	QList<Device *> devices;

	void lookDevices(void);
	void destroyDevices();
	void checkServices(QString path);
};

#endif
