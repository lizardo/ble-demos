#include <QApplication>
#include <QObject>

#include "types.h"
#include "manager.h"
#include "adapter.h"
#include "proximity.h"

//#include "monitorview.h"
#include "monitorqml.h"
#include "monitor.h"


int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	QStringList args = app.arguments();
	QString hci;

	if (args.length() == 2)
		hci = args.at(1);

	QDBusConnection dbus = QDBusConnection::systemBus();

	Monitor *monitor = new Monitor(hci);

	MonitorView *monitorView = new MonitorView(monitor);

	monitorView->show();
	app.exec();
}
