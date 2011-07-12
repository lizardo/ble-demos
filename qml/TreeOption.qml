import Qt 4.7

Item {
	Rectangle {
		id: button_0;
		height: 30; width: 80;
		color: "red"
	}

	Rectangle {
		id: button_1;
		height: 30; width: 80;
		color: "red"

		anchors.right: button_0.left
		anchors.top: button_0.top
	}

	Rectangle {
		id: button_2;
		height: 30; width: 80;
		color: "red"

		anchors.right: button_1.left
		anchors.top: button_1.top
	}
}
