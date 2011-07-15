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
}

void MonitorView::bar(QString s, int i)
{
	qWarning() << s << i;
}
