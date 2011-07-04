#include <QWidget>
#include <QVBoxLayout>
#include <QSlider>
#include <QListView>
#include <QStringListModel>

#include <Phonon/MediaObject>

#include "monitor.h"

class MonitorView: public QWidget
{
	Q_OBJECT
public:
	MonitorView(Monitor *monitor = NULL, QWidget *parent = 0,
						Qt::WindowFlags f = 0);
	void setModel(Monitor *monitor);

signals:
	void thresholdChanged(int value);
	void pathlossChanged(int value);
	void linkLossChanged(int value);
	void findMeChanged(int value);

public slots:
	void playAlarm(QString type);
	void playFinished();
	void propertyValue(QString property, QString value);

private slots:
	void devicesClicked(QModelIndex index);

private:
	Monitor *model;

	QSlider *threshold;
	QSlider *pathloss;
	QSlider *linkLoss;
	QSlider *findMe;

	QListView *devices;
	QStringListModel *devicesModel;

	Phonon::MediaObject *mildAlarm;
	Phonon::MediaObject *highAlarm;
};
