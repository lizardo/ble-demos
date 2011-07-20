import Qt 4.7

Item {
	id: monitorView
	width: 480; height: 800;
	signal foo(string bla, int ble)

	Text {
		id: labelDevices
		text: "Devices:"
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
	}

	Rectangle {
		id: contentView
		color: "blue"
		width: parent.width; height: 400
		anchors.topMargin: 10
		anchors.left: listView.left
		anchors.top: listView.bottom

		Text {
			id: labelThreshold
			text: "Threshold:"
		}

		Slider {
			id: threshold
			objectName: "threshold"

			anchors.left: pathloss.left

			Connections {
				target: threshold
				onValueChanged: monitor.thresholdChanged(threshold.value)
			}
		}

		Text {
			id: labelPathloss
			text: "Pathloss Alert Level:"

			anchors.top: pathloss.top
		}

		Slider {
			id: pathloss
			objectName: "pathloss"

			anchors.top: threshold.bottom
			anchors.left: labelPathloss.right
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

			anchors.top: linkloss.top
		}

		Slider {
			id: linkloss
			objectName: "linkloss"

			anchors.top: pathloss.bottom
			anchors.left: labelLinkloss.right
			anchors.topMargin: 15
			anchors.leftMargin: 15

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
