import Qt 4.7

Item {
	id: monitorView
	width: 480; height: 800;
	signal foo(string bla, int ble)

	ListView {
		id: listView
		currentIndex: -1
		objectName: "listView"
		width: parent.width; height: 400
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

		Slider {
			id: threshold
			objectName: "threshold"
		}

		Slider {
			id: pathloss
			objectName: "pathloss"

			anchors.top: threshold.bottom
			anchors.topMargin: 15
		}

		Slider {
			id: linkloss
			objectName: "linkloss"

			anchors.top: pathloss.bottom
			anchors.topMargin: 15
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
