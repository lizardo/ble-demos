#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>

#include <Phonon/MediaSource>

#include "monitorview.h"

#define ALARM_MILD "./mild.wav"
#define ALARM_HIGH "./high.wav"

MonitorView::MonitorView(Monitor *monitor, QWidget *parent, Qt::WindowFlags f)
	: QWidget(parent, f)
{
	QGridLayout *slidersBox;
	QLabel *label;

	slidersBox = new QGridLayout(this);

	label = new QLabel("Device:");
	devices = new QListView();
	devicesModel = new QStringListModel();
	connect(devices, SIGNAL(clicked(const QModelIndex)),
		this, SLOT(devicesClicked(QModelIndex)));

	slidersBox->addWidget(label, 0, 0, Qt::AlignRight);
	slidersBox->addWidget(devices, 0, 1);

	label = new QLabel("Threshold:");
	threshold = new QSlider(Qt::Horizontal);
	threshold->setMaximum(2);
	connect(threshold, SIGNAL(valueChanged(int)),
		this, SIGNAL(thresholdChanged(int)));

	slidersBox->addWidget(label, 1, 0, Qt::AlignRight);
	slidersBox->addWidget(threshold, 1, 1);

	label = new QLabel("PathLoss Alert Level:");
	pathloss = new QSlider(Qt::Horizontal);
	pathloss->setMaximum(2);
	connect(pathloss, SIGNAL(valueChanged(int)),
		this, SIGNAL(pathlossChanged(int)));

	slidersBox->addWidget(label, 2, 0, Qt::AlignRight);
	slidersBox->addWidget(pathloss, 2, 1);

	label = new QLabel("LinkLoss Alert Level:");
	linkLoss = new QSlider(Qt::Horizontal);
	linkLoss->setMaximum(2);
	connect(linkLoss, SIGNAL(valueChanged(int)),
		this, SIGNAL(linkLossChanged(int)));

	slidersBox->addWidget(label, 3, 0, Qt::AlignRight);
	slidersBox->addWidget(linkLoss, 3, 1);

	label = new QLabel("FindMe Alert Level:");
	findMe = new QSlider(Qt::Horizontal);
	findMe->setMaximum(2);
	connect(findMe, SIGNAL(valueChanged(int)),
		this, SIGNAL(findMeChanged(int)));

	slidersBox->addWidget(label, 4, 0, Qt::AlignRight);
	slidersBox->addWidget(findMe, 4, 1);

	setLayout(slidersBox);

	mildAlarm = Phonon::createPlayer(Phonon::MusicCategory);

	QObject::connect(mildAlarm, SIGNAL(finished()), this,
	SLOT(playFinished()));

	mildAlarm = Phonon::createPlayer(Phonon::NotificationCategory);

	setModel(monitor);
}

void MonitorView::setModel(Monitor *monitor)
{
	model = monitor;

	if (!monitor)
		return;

	QObject::connect(
		this, SIGNAL(thresholdChanged(int)),
		model, SLOT(thresholdChanged(int)));

	QObject::connect(
		this, SIGNAL(pathlossChanged(int)),
		model, SLOT(pathlossChanged(int)));

	QObject::connect(
		this, SIGNAL(linkLossChanged(int)),
		model, SLOT(linkLossChanged(int)));

	QObject::connect(
		this, SIGNAL(findMeChanged(int)),
		model, SLOT(findMeChanged(int)));

	QObject::connect(
		model, SIGNAL(alarmCalled(QString)),
		this, SLOT(playAlarm(QString)));


	devicesModel->setStringList(model->devicesName());
	devices->setModel(devicesModel);
}

void MonitorView::devicesClicked(QModelIndex index)
{
	model->setDevice(index.row());
}

void MonitorView::playAlarm(QString type)
{
	QString mildFile(ALARM_MILD);
	QString highFile(ALARM_HIGH);
	Q_UNUSED(type);

	if (type == "PathLoss") {
		if (pathloss->value() == 1)
			mildAlarm->setCurrentSource(mildFile);
		else if (pathloss->value() == 2)
			mildAlarm->setCurrentSource(highFile);
	} else if (type == "Threshold") {
		if (threshold->value() == 1)
			mildAlarm->setCurrentSource(mildFile);
		else if (threshold->value() == 2)
			mildAlarm->setCurrentSource(highFile);
	}
	mildAlarm->play();
}

void MonitorView::playFinished()
{
	qWarning() << "finished" << mildAlarm->state() << mildAlarm->queue().count();
}
