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

		color: "#3C4062"

		Text {
			id: labelThreshold
			text: "Threshold:"
			color: "white"
			font.pointSize: 10
			font.weight: Font.Bold

			anchors.right: labelPathloss.right
			anchors.verticalCenter: threshold.verticalCenter
		}

		Slider {
			id: threshold
			objectName: "threshold"

			anchors.left: pathloss.left

			anchors.right: parent.right
			anchors.rightMargin: 10

			Connections {
				target: threshold
				onValueChanged: monitor.thresholdChanged(threshold.value)
			}
		}

		Text {
			id: labelPathloss
			text: "Pathloss Alert Level:"

			color: "white"
			font.pointSize: 10
			font.weight: Font.Bold

			anchors.verticalCenter: pathloss.verticalCenter
		}

		Slider {
			id: pathloss
			objectName: "pathloss"

			anchors.top: threshold.bottom
			anchors.left: labelPathloss.right
			anchors.right: parent.right
			anchors.rightMargin: 10
			anchors.leftMargin: 15
			anchors.topMargin: 15


			Connections {
				target: pathloss
				onValueChanged: monitor.pathlossChanged(pathloss.value)
				}
		}

		Text {
			id: labelLinkloss
			text: "Linkloss Alert Level:"

			color: "white"
			font.pointSize: 10
			font.weight: Font.Bold

			anchors.verticalCenter: linkloss.verticalCenter
		}

		Slider {
			id: linkloss
			objectName: "linkloss"

			anchors.top: pathloss.bottom
			anchors.left: labelLinkloss.right
			anchors.right: parent.right
			anchors.topMargin: 15
			anchors.leftMargin: 15
			anchors.rightMargin: 10

			Connections {
				target: linkloss
				onValueChanged: { monitor.linkLossChanged(linkloss.value) }
			}
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
			if (property == "Threshold")
				threshold.setValue(valueToInt(property, value))
			else if (property == "PathLossAlertLevel")
				pathloss.setValue(valueToInt(property, value))
			else if (property == "LinkLossAlertLevel")
				linkloss.setValue(valueToInt(property, value))
		}
	}
}
