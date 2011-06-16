#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
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

	setLayout(slidersBox);
}
