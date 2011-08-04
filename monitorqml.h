#include <QDeclarativeView>
#include <QWidget>

#include "monitor.h"

class MonitorView: public QDeclarativeView
{
	Q_OBJECT
public:
	MonitorView(Monitor *monitor = NULL);

public slots:
	void bar(QString s, int i);
	void onDevicesClick();

private:
	Monitor *model;

	QObject *devices;
};
