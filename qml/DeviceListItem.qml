import Qt 4.7

Item {
	width: parent.width
	height: 50

	property bool active: listView.currentIndex == index
	signal click()

	Rectangle {
		width: parent.width
		height: 50
		color: "red"
		visible: active
	}

	Text {
		id: deviceName
		text: modelData
	}

		MouseArea {
			anchors.fill: parent
			onClicked: {
				listView.currentIndex = index;
			}
		}
}
