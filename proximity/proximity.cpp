/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -p proximity.h:proximity.cpp -i types.h xml/proximity.xml
 *
 * qdbusxml2cpp is Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "proximity.h"

/*
 * Implementation of interface class OrgBluezProximityMonitorInterface
 */

OrgBluezProximityMonitorInterface::OrgBluezProximityMonitorInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

OrgBluezProximityMonitorInterface::~OrgBluezProximityMonitorInterface()
{
}

/*
 * Implementation of interface class OrgBluezProximityReporterInterface
 */

OrgBluezProximityReporterInterface::OrgBluezProximityReporterInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

OrgBluezProximityReporterInterface::~OrgBluezProximityReporterInterface()
{
}

