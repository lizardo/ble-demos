/* main.cpp -- The main file. */

/* Copyright (C) 2011 Instituto Nokia de Tecnologia

   Author: Hugo Parente Lima <hugo.lima@openbossa.org>
   Author: Sheldon Almeida Demario <sheldon.demario@openbossa.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#include <QtGui/QApplication>
#include <QDeclarativeContext>
#include "qmlapplicationviewer.h"

#include "monitor.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    Monitor *collector = new Monitor();

    if (!QDBusConnection::systemBus().registerObject(COLLECTOR_OBJPATH, collector, QDBusConnection::ExportAllSlots))
        qWarning() << "Error registering myself on D-Bus.";

#ifdef Q_WS_SIMULATOR
    viewer->addImportPath(QT_INSTALL_PREFIX "/imports/simulatorHarmattan");
#endif
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->rootContext()->setContextProperty("monitor", collector);
    viewer->setMainQmlFile(QLatin1String("qml/qml/MainMeego.qml"));
    viewer->showExpanded();

    return app->exec();
}
