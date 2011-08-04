#include <QtDeclarative>
#include <QGraphicsObject>
#include <QModelIndex>
#include <QUrl>

#include <QDebug>

#include "monitorqml.h"

MonitorView::MonitorView(Monitor *monitor)
	: model(monitor)
{
	rootContext()->setContextProperty("monitor", model);
	setSource(QUrl("qml/MonitorView.qml"));

	QGraphicsObject *r = rootObject();

	connect(r, SIGNAL(foo(QString, int)), this, SLOT(bar(QString, int)));

	devices = r->findChild<QObject *>("listView");

	connect(devices, SIGNAL(currentIndexChanged()),
		this, SLOT(onDevicesClick()));
}

void MonitorView::bar(QString s, int i)
{
	qWarning() << s << i;
}

void MonitorView::onDevicesClick()
{
	model->setDevice(devices->property("currentIndex").toInt());
}
