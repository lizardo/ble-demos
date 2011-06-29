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
	Monitor(QString hci);
	~Monitor();

	QStringList devicesName();
	void setDevice(int index);

signals:
	void alarmCalled(QString type);

public slots:
	void propertyChanged(const QString &property, const QDBusVariant &value);
	void thresholdChanged(int value);
	void pathlossChanged(int value);
	void linkLossChanged(int value);
	void findMeChanged(int value);

private:
	org::bluez::Manager *manager;
	org::bluez::Adapter *adapter;
	org::bluez::Proximity *proximity;
	Device *device;
	QList<Device *> devices;

	void lookDevices(void);
	void checkServices(QString path);
};

#endif
