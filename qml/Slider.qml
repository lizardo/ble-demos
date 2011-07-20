import Qt 4.7

Rectangle {
	id: slider;
	width: 320; height: 40
	property int value: -1
	property int steps: 3
	color: "green"

	function mouseRelease(pos) {
		var pixsteps = Math.floor(width/steps)
		var positions

		for (var i = 0; i < steps-1; i++) {
			var diff1 = Math.abs(pos - pixsteps * i);
			var diff2 = Math.abs(pos - pixsteps * (i+1) );

			if (diff1 < diff2) {
				handle.x = pixsteps * i;
				value = i;
				return;
			}
		}

		value = steps - 1;
		handle.x = pixsteps * (steps - 1)
	}

	Rectangle {
		id: handle; width: 40; height: 40
		color: "red"
		MouseArea {
			anchors.fill: parent
			drag.target: parent; drag.axis: "XAxis"
			drag.minimumX: 0
			drag.maximumX: slider.width - handle.width

			onReleased: {
				mouseRelease(handle.x)

				foo("aaa", value)
			}
		}
	}

	function setValue(newValue) {
		if (value == newValue)
			return;

		var pixsteps = Math.floor(width/steps)
		handle.x = pixsteps * (newValue)
	}

	Connections {
		target: monitor
		onDummy: print( "DUMMY");
	}
}
