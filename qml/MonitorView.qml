import Qt 4.7

Item {
	id: monitorView
	width: 480; height: 800;

	Component {
		id: deviceDelegate
		Item {
			width: parent.width
			height: deviceCol.height

			Column {
				id: deviceCol
				width: parent.width
				height: 30
					Text {
						id: deviceName
						text: name
						MouseArea {
							anchors.fill: parent
							onClicked: {
								listView.currentIndex = index;
							}
						}
					}
			}
		}
	}

	ListView {
		id: listView
		width: parent.width; height: 480
		model: DeviceListModel {}
		delegate: deviceDelegate
		highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
		focus: true
	}

		Rectangle {
			id: contentView
			color: "red"
			width: parent.width; height: 480
			anchors.left: listView.right
			anchors.right: parent.right
		}
}
