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

Rectangle {
	id: monitorView
	width: 480; height: 800;
	signal foo(string bla, int ble)

	color: "#3F303A"

	Text {
		id: labelDevices
		text: "Devices:"
		color: "white"
		font.pointSize: 10
		font.weight: Font.Bold
	}

	ListView {
		id: listView
		currentIndex: -1
		objectName: "listView"
		width: parent.width; height: 400
		anchors.top: labelDevices.bottom
		model: monitor.devicesName()
		delegate: DeviceListItem {}
		clip: true
		highlight: Rectangle { color: "#02118A"; radius: 5 }
	}

	Rectangle {
		id: contentView
		width: parent.width; height: 400
		anchors.topMargin: 10
		anchors.left: listView.left
		anchors.top: listView.bottom

		enabled: false
		color: "#3C4062"

		Text {
			id: labelSignalLevel
			text: "Signal Level:"
			color: "white"
			font.pointSize: 10
			font.weight: Font.Bold

			anchors.top: parent.top
			anchors.right: parent.horizontalCenter
			anchors.topMargin: 10
			anchors.rightMargin: 10
		}

		Text {
			id: signalLevel
			objectName: "signalLevel"

			text: "unknown"

			font.pointSize: 10
			font.weight: Font.Bold
			anchors.top: labelSignalLevel.top
			anchors.left: labelSignalLevel.right
			anchors.leftMargin: 10
		}

		Text {
			id: labelImmediate
			text: "Immediate Alert Level:"

			color: "white"
			font.pointSize: 10
			font.weight: Font.Bold

			anchors.verticalCenter: immediateAlert.verticalCenter
			anchors.right: parent.horizontalCenter
			anchors.rightMargin: 10
		}

		Slider {
			id: immediateAlert
			objectName: "immediateAlert"

			anchors.top: signalLevel.bottom
			anchors.left: labelImmediate.right
			anchors.right: parent.right
			anchors.rightMargin: 10
			anchors.leftMargin: 15
			anchors.topMargin: 15


			Connections {
				target: immediateAlert
				onValueChanged: monitor.onImmediateAlertChange(immediateAlert.value)
				}
		}

		Text {
			id: labelLinkloss
			text: "Linkloss Alert Level:"

			color: "white"
			font.pointSize: 10
			font.weight: Font.Bold

			anchors.verticalCenter: linkloss.verticalCenter
			anchors.right: parent.horizontalCenter
			anchors.rightMargin: 10
		}

		Slider {
			id: linkloss
			objectName: "linkloss"

			anchors.top: immediateAlert.bottom
			anchors.left: labelLinkloss.right
			anchors.right: parent.right
			anchors.topMargin: 15
			anchors.leftMargin: 15
			anchors.rightMargin: 10

			Connections {
				target: linkloss
				onValueChanged: { monitor.onLinkLossChange(linkloss.value) }
			}
		}

		Connections {
			target: listView
			onCurrentItemChanged: {contentView.enabled = true }
		}
	}

	function valueToInt(property, value) {
		if (property == "Threshold") {
			if (value == "low")
				return 0;
			else if (value == 'medium')
				return 1;
			else if (value == 'high')
				return 2;
		} else {
			if (value == 'none')
				return 0;
			else if (value == 'mild')
				return 1;
			else if (value == 'high')
				return 2;
		}

	}

	Connections {
		target: monitor
		onPropertyValue: {
			if (property == "SignalLevel")
				signalLevel.text = value;
			else if (property == "ImmediateAlertLevel")
				immediateAlert.setValue(valueToInt(property, value))
			else if (property == "LinkLossAlertLevel")
				linkloss.setValue(valueToInt(property, value))
		}
	}
}
