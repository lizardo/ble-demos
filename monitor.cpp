#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "monitor.h"

Monitor::Monitor(QWidget *parent, Qt::WindowFlags f)
	: QWidget(parent, f)
{
	QHBoxLayout *sliderBox;
	QLabel *label;

	sliderBox = new QHBoxLayout();
	slidersBox = new QVBoxLayout(this);

	label = new QLabel("Threshold");
	threshold = new QSlider(Qt::Horizontal);
	threshold->setMaximum(2);
	connect(threshold, SIGNAL(valueChanged(int)),
		this, SIGNAL(thresholdChanged(int)));

	sliderBox->addWidget(label);
	sliderBox->addSpacing(5);
	sliderBox->addWidget(threshold);

	slidersBox->addLayout(sliderBox);

	setLayout(slidersBox);
}
