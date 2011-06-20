#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>

#include <Phonon/MediaSource>

#include "monitor.h"

Monitor::Monitor(QWidget *parent, Qt::WindowFlags f)
	: QWidget(parent, f)
{
	QGridLayout *slidersBox;
	QLabel *label;

	slidersBox = new QGridLayout(this);

	label = new QLabel("Threshold:");
	threshold = new QSlider(Qt::Horizontal);
	threshold->setMaximum(2);
	connect(threshold, SIGNAL(valueChanged(int)),
		this, SIGNAL(thresholdChanged(int)));

	slidersBox->addWidget(label, 0, 0, Qt::AlignRight);
	slidersBox->addWidget(threshold, 0, 1);

	label = new QLabel("PathLoss Alert Level:");
	pathloss = new QSlider(Qt::Horizontal);
	pathloss->setMaximum(2);
	connect(pathloss, SIGNAL(valueChanged(int)),
		this, SIGNAL(pathlossChanged(int)));

	slidersBox->addWidget(label, 1, 0, Qt::AlignRight);
	slidersBox->addWidget(pathloss, 1, 1);

	label = new QLabel("LinkLoss Alert Level:");
	linkLoss = new QSlider(Qt::Horizontal);
	linkLoss->setMaximum(2);
	connect(linkLoss, SIGNAL(valueChanged(int)),
		this, SIGNAL(linkLossChanged(int)));

	slidersBox->addWidget(label, 2, 0, Qt::AlignRight);
	slidersBox->addWidget(linkLoss, 2, 1);

	label = new QLabel("FindMe Alert Level:");
	findMe = new QSlider(Qt::Horizontal);
	findMe->setMaximum(2);
	connect(findMe, SIGNAL(valueChanged(int)),
		this, SIGNAL(findMeChanged(int)));

	slidersBox->addWidget(label, 3, 0, Qt::AlignRight);
	slidersBox->addWidget(findMe, 3, 1);

	setLayout(slidersBox);


	mildAlarm = Phonon::createPlayer(Phonon::MusicCategory,
					Phonon::MediaSource("./mild.wav"));

	QObject::connect(mildAlarm, SIGNAL(finished()), this,
	SLOT(playFinished()));

	mildAlarm = Phonon::createPlayer(Phonon::NotificationCategory);
}

void Monitor::playAlarm(QString type)
{
	static int i = 0;
	QString mildFile("./mild.wav");
	Q_UNUSED(type);

	if (type == "PathLoss")
		mildAlarm->setCurrentSource(mildFile);
	mildAlarm->play();

	i++;
}

void Monitor::playFinished()
{
	static int i = 0;
	qWarning() << i << "finished" << mildAlarm->state() << mildAlarm->queue().count();

	i++;
}
