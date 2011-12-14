/* monitorqml.cpp -- This file implements the MonitorView class.*/

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

#include <QtDeclarative>

#include "monitorqml.h"

MonitorView::MonitorView(Monitor *monitor)
    : model(monitor)
{
    rootContext()->setContextProperty("monitor", model);

#ifdef MEEGO_EDITION_HARMATTAN
    setSource(QUrl("qrc:/qml/MainMeego.qml"));
    setAttribute(Qt::WA_LockPortraitOrientation, true);
    setResizeMode(QDeclarativeView::SizeRootObjectToView);
#else
    setSource(QUrl("qrc:/qml/MonitorView.qml"));
#endif
}
