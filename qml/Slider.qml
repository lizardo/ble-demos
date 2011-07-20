import Qt 4.7

Rectangle {
	id: slider;
	width: 320; height: 40
	property int value: -1
	property int steps: 3
	property int pixsteps: Math.floor(width / (steps - 1))
	color: "green"

	function setKnobPos(v) {
		var halfKnob = handle.width / 2;

		if (v == 0)
			halfKnob = 0;
		else if (v == (steps - 1))
			halfKnob = handle.width;

		handle.x = pixsteps * v - halfKnob;
	}

	function mouseRelease(pos) {
		var positions

		for (var i = 0; i < steps-1; i++) {
			var diff1 = Math.abs(pos - pixsteps * i);
			var diff2 = Math.abs(pos - pixsteps * (i+1));

			if (diff1 < diff2) {
				value = i;
				setKnobPos(i)

				return;
			}
		}

		value = steps - 1;
		setKnobPos(value)
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

		setKnobPos(newValue)
	}

	Connections {
		target: monitor
		onDummy: print( "DUMMY");
	}
}
