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
/*
	void setModel(Monitor *monitor);

signals:
	void thresholdChanged(int value);
	void pathlossChanged(int value);
	void linkLossChanged(int value);
	void findMeChanged(int value);

public slots:
	void playAlarm(QString type);
	void playFinished();

private slots:
	void devicesClicked(QModelIndex index);
*/
private:
	Monitor *model;

	QObject *devices;

	QObject *linkloss;

/*
	QSlider *threshold;
	QSlider *pathloss;
	QSlider *linkLoss;
	QSlider *findMe;

	QStringListModel *devicesModel;

	Phonon::MediaObject *mildAlarm;
	Phonon::MediaObject *highAlarm;
	*/
};
