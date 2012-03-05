#include "controlpanel.h"

ControlPanel::ControlPanel(QObject *parent) :
    QObject(parent), m_dbusProxy(0)
{
}

bool ControlPanel::initialize()
{
    m_dbusProxy = new DuiControlPanelIf("com.nokia.DuiControlPanel",
                                        "/", QDBusConnection::sessionBus(), this);
    if (!m_dbusProxy)
        return false;

    return true;
}

void ControlPanel::launch()
{
    m_dbusProxy->launch();
}

bool ControlPanel::launchBluetooth()
{
    return appletPage("Bluetooth");
}

bool ControlPanel::appletPage(const QString &params)
{
    QDBusPendingReply<bool> reply = m_dbusProxy->appletPage(params);
    reply.waitForFinished();
    if (reply.isError()) {
        qCritical() << reply.error().message();
        return false;
    }
    return reply.value();
}
