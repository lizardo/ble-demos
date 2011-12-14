/* main.cpp -- The main file. */

/* Copyright (C) 2011 Instituto Nokia de Tecnologia

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
#include <QObject>

#include "types.h"
#include "manager.h"
#include "adapter.h"
#include "proximity.h"

#include "monitorqml.h"
#include "monitor.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStringList args = app.arguments();
    QString hci;

    if (args.length() == 2)
        hci = args.at(1);

    QDBusConnection dbus = QDBusConnection::systemBus();

    Monitor *monitor = new Monitor(hci);

    MonitorView *monitorView = new MonitorView(monitor);

#ifdef MEEGO_EDITION_HARMATTAN
    monitorView->showFullScreen();
#else
    monitorView->show();
#endif
    app.exec();
}
