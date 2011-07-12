#include <QDeclarativeView>
#include <QWidget>

#include "monitor.h"

class MonitorView: public QDeclarativeView
{
	Q_OBJECT
public:
	MonitorView(Monitor *monitor = NULL);
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

/*
	QSlider *threshold;
	QSlider *pathloss;
	QSlider *linkLoss;
	QSlider *findMe;

	QListView *devices;
	QStringListModel *devicesModel;

	Phonon::MediaObject *mildAlarm;
	Phonon::MediaObject *highAlarm;
	*/
};
