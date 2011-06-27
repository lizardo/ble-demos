#include <QObject>
#include <QDBusVariant>
#include <QList>

#include "device.h"
#include "manager.h"
#include "adapter.h"
#include "proximity.h"

using namespace org::bluez;

class Monitor: public QObject
{
	Q_OBJECT
public:
	Monitor(QString adapter, QString bda);
	~Monitor();

	QStringList devicesName();

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
	QList<Device *> devices;

	void init2(void);
	void checkServices(QString path);
};
