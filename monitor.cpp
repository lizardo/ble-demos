#include <QDebug>
#include <QVBoxLayout>
#include "monitor.h"

Monitor::Monitor(QWidget *parent, Qt::WindowFlags f)
	: QWidget(parent, f)
{
	this->threshold = new QSlider(Qt::Horizontal);
	threshold->setMaximum(2);
	connect(threshold, SIGNAL(valueChanged(int)),
		this, SIGNAL(thresholdChanged(int)));

	this->slidersBox = new QVBoxLayout(this);
	this->slidersBox->addWidget(threshold);

	setLayout(slidersBox);
}
