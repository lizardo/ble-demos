#include <QWidget>
#include <QVBoxLayout>
#include <QSlider>

#include <Phonon/MediaObject>

class Monitor: public QWidget
{
	Q_OBJECT
public:
	Monitor(QWidget *parent = 0, Qt::WindowFlags f = 0);

signals:
	void thresholdChanged(int value);
	void pathlossChanged(int value);

public slots:
	void playAlarm(QString type);
	void playFinished();

private:
	QSlider *threshold;
	QSlider *pathloss;

	Phonon::MediaObject *mildAlarm;
	Phonon::MediaObject *highAlarm;
};
