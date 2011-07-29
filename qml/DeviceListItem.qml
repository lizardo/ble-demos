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
		color: "#878A92"
		font.weight: Font.Bold
		font.pointSize: 20
	}

		MouseArea {
			anchors.fill: parent
			onClicked: {
				listView.currentIndex = index;
			}
		}

}
