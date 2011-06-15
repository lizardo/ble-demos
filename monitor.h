#include <QWidget>
#include <QVBoxLayout>
#include <QSlider>
class Monitor: public QWidget
{
	Q_OBJECT
public:
	Monitor(QWidget *parent = 0, Qt::WindowFlags f = 0);

signals:
	void thresholdChanged(int value);

private:
	QVBoxLayout *slidersBox;
	QSlider *threshold;
};
