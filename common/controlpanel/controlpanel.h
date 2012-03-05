#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QObject>

#include <duicontrolpanelif_p.h>

class ControlPanel : public QObject
{
    Q_OBJECT
public:
    explicit ControlPanel(QObject *parent = 0);

    bool initialize();

public slots:
    void launch();
    bool launchBluetooth();
    bool appletPage(const QString &params);

private:
    DuiControlPanelIf *m_dbusProxy;
};

#endif // CONTROLPANEL_H
