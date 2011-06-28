#include <QApplication>
#include <QObject>

#include "types.h"
#include "manager.h"
#include "adapter.h"
#include "proximity.h"

#include "monitorview.h"
#include "monitor.h"


int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	QStringList args = app.arguments();

	if (args.length() < 3) {
		qWarning() << "Usage:" << args.at(0) << "<hciX>" "<remote address>";
		return 0;
	}

	QDBusConnection dbus = QDBusConnection::systemBus();

	Monitor *monitor = new Monitor(args.at(1), args.at(2));
	Q_UNUSED(monitor);

	MonitorView *monitorView = new MonitorView(monitor);

	monitorView->show();
	app.exec();
}
