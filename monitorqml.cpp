#include <QUrl>

#include "monitorqml.h"

MonitorView::MonitorView(Monitor *monitor)
	: model(monitor)
{
	setSource(QUrl("qml/MonitorView.qml"));
}
