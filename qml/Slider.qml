/* Copyright (C) 2011 Instituto Nokia de Tecnologia

   Author: Sheldon Almeida Demario <sheldon.demario@openbossa.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

import Qt 4.7

Item {
	id: slider;
	width: 320; height: 40
	property int value: -1
	property int steps: 3
	property int pixsteps: Math.floor(width / (steps - 1))

	BorderImage {
		width: parent.width;
		border { left: 5; right: 5; }
		horizontalTileMode: BorderImage.Repeat
		source: "images/off.png"

		anchors.verticalCenter: parent.verticalCenter
	}

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

	Image {
		id: handle;
		source: "images/knob.png"
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
