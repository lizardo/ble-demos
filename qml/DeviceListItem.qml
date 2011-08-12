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

import Qt 4.7

Item {
	width: parent.width
	height: deviceName.height

	property bool active: listView.currentIndex == index
	signal click()

	Rectangle {
		width: parent.width
		//height: 50

/*
		color: {
			if (active == true)
				return "grey"
			else
				return "white"
		}
		*/
	}

	Text {
		id: deviceName
		text: modelData
		color: "#E4EAF1"
		font.pixelSize: 36

		width: parent.width

		horizontalAlignment: Text.AlignHCenter
	}

		MouseArea {
			anchors.fill: parent
			onClicked: {
				listView.currentIndex = index;
			}
		}

}
