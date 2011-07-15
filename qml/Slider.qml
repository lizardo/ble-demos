import Qt 4.7

Rectangle {
	id: slider;
	width: 320; height: 40
	property int value: Math.round(handle.x*100/(slider.width-handle.width))
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

				valueChanged(value)

				foo("aaa", value)
			}
		}
	}

	Connections {
		target: monitor
		onPropertyValue: {
			print(property + value)
		}
		onDummy: print( "DUMMY");
	}
}
