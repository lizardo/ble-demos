import Qt 4.7

Item {
	id: monitorView
	width: 480; height: 800;
	signal foo(string bla, int ble)

	ListView {
		id: listView
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
		}

		Slider {
			id: pathloss

			anchors.top: threshold.bottom
			anchors.topMargin: 15
		}
	}
}
