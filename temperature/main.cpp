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

#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include "monitor.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStringList args = app.arguments();
    QString hci;

    if (args.length() == 2)
        hci = args.at(1);

    Monitor* monitor = new Monitor;
    if (!QDBusConnection::systemBus().registerObject(MONITOR_OBJPATH, monitor, QDBusConnection::ExportAllSlots))
        qWarning() << "Error registering myself on D-Bus.";


    QDeclarativeView view;
    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view.rootContext()->setContextProperty("monitor", monitor);

    view.setSource(QUrl("qrc:/qml/MainMeego.qml"));

#ifdef MEEGO_EDITION_HARMATTAN
    view.showFullScreen();
#else
    view.show();
#endif
    return app.exec();
}
