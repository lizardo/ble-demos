import Qt 4.7

Item {
	width: parent.width
	height: 20

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
	}

		MouseArea {
			anchors.fill: parent
			onClicked: {
				listView.currentIndex = index;
			}
		}

}
