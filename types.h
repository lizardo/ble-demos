/* Copyright (C) 2011 Instituto Nokia de Tecnologia
   Copyright (C) 2011 Sheldon Almeida Demario

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


#ifndef _TYPES_H_
#define _TYPES_H_

#include <QVariantMap>
#include <QMap>

typedef QMap<uint, QString> ServiceMap;
Q_DECLARE_METATYPE(ServiceMap)

const char BLUEZ_SERVICE_NAME[] = "org.bluez";
const char BLUEZ_MANAGER_PATH[] = "/";
const char PAIRING_AGENT_PATH[] = "/pairing/agent";

typedef QList<QVariantMap> QVariantMapList;
Q_DECLARE_METATYPE(QVariantMapList)

#endif // _TYPES_H_
