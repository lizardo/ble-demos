/* Copyright (C) 2011 Instituto Nokia de Tecnologia

   Author: Sheldon Almeida Demario <sheldon.demario@openbossa.org>
   Author: Vinicius Costa Gomes <vinicius.gomes@openbossa.org>

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

#include <QVariant>
#include <QVariantMap>
#include <QMap>

typedef QMap<QString, QVariant> PropertyMap;
Q_DECLARE_METATYPE(PropertyMap)
typedef QMap<uint, QString> ServiceMap;
Q_DECLARE_METATYPE(ServiceMap)
typedef QList<QVariantMap> QVariantMapList;
Q_DECLARE_METATYPE(QVariantMapList)

#define HEART_RATE_UUID        "0000180D-0000-1000-8000-00805F9B34FB"
#define HR_MEASUREMENT_UUID    "00002A37-0000-1000-8000-00805F9B34FB"
#define BLUEZ_SERVICE_NAME     "org.bluez"
#define BLUEZ_MANAGER_PATH     "/"

#endif // _TYPES_H_
