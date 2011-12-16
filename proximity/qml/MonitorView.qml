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
import com.nokia.meego 1.0
import com.nokia.extras 1.1

Page {
    id: monitorView
    width: 480; height: 800;
    orientationLock: PageOrientation.LockPortrait

    Image {
        id: header
        source: "images/header.png"

        Text {
            id: labelDevices
            text: "Devices:"
            color: "#768EA5"
            font.pixelSize: 32

            anchors.fill: parent.fill
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Image {
        id: list_bg
        source: "images/list_bg.png"

        anchors.top: header.bottom
    }

    ListView {
        id: listView
        objectName: "listView"
        currentIndex: -1

        anchors.fill: list_bg

        model: monitor.deviceModel
        delegate: ListDelegate {
            titleColor: "#E4EAF1"
            titleSize: 36
            height: 50
            onClicked: {
                monitor.setDevice(index)
                contentView.enabled = true
            }
        }
        clip: true
        highlight: Rectangle { color: "#02118A"; radius: 5 }
    }

    Rectangle {
        id: contentView
        width: parent.width; height: 400
        anchors.top: list_bg.bottom
        // Hack to add a not planned unlock slider without change the list bg image
        anchors.topMargin: -142

        enabled: false

        Image {
            id: signal_bg
            source: "images/content_bg_signal.png"

            anchors.top: parent.top
        }

        Text {
            id: labelSignalLevel
            text: "Signal Level:"

            color: "#768EA5"
            font.pixelSize: 32

            anchors.top: signal_bg.top
            anchors.right: signal_bg.horizontalCenter
            anchors.topMargin: 50
            anchors.leftMargin: 10
            anchors.rightMargin: 5
        }

        Text {
            id: signalLevel
            objectName: "signalLevel"

            text: "unknown"

            color: "#60748E"
            font.pixelSize: 32
            font.weight: Font.Bold

            anchors.top: labelSignalLevel.top
            anchors.left: labelSignalLevel.right
            anchors.leftMargin: 5
        }

        Image {
            id: unlock_bg
            source: "images/content_bg_alerts.png"

            anchors.top: signal_bg.bottom
        }

        Text {
            id: labelUnlock
            text: "Unlock on signal level:"

            color: "#768EA5"
            font.pixelSize: 32

            anchors.top: unlock_bg.top
            anchors.left: unlock_bg.left
            anchors.topMargin: 20
            anchors.leftMargin: 10
        }

        Slider {
            id: unlockThreshold
            objectName: "unlockThreshold"

            stepSize:1
            valueIndicatorVisible: true
            minimumValue:-1
            maximumValue:2
            value: -1

            anchors.horizontalCenter: unlock_bg.horizontalCenter
            anchors.top: labelUnlock.bottom
            anchors.topMargin: 15

            onValueChanged: {
                monitor.onUnlockChange(unlockThreshold.value)
            }

            function formatValue(v) {
                switch(v) {
                case -1:
                    return "disabled";
                case 0:
                    return "weak";
                case 1:
                    return "regular";
                 default:
                     return "good";
                }
            }
        }

        Image {
            id: immediate_bg
            source: "images/content_bg_alerts.png"

            anchors.top: unlock_bg.bottom
        }

        Text {
            id: labelImmediate
            text: "Immediate Alert Level:"

            color: "#768EA5"
            font.pixelSize: 32

            anchors.top: immediate_bg.top
            anchors.left: immediate_bg.left
            anchors.topMargin: 20
            anchors.leftMargin: 10
        }

        Slider {
            id: immediateAlert
            objectName: "immediateAlert"

            stepSize:1
            valueIndicatorVisible: true
            minimumValue:0
            maximumValue:2
            value: 0

            anchors.horizontalCenter: immediate_bg.horizontalCenter
            anchors.top: labelImmediate.bottom
            anchors.topMargin: 15

            onValueChanged: {
                monitor.onImmediateAlertChange(immediateAlert.value)
            }

            function formatValue(v) {
                switch(v) {
                case 0:
                    return "none";
                case 1:
                    return "mild";
                 default:
                     return "high";
                }
            }
        }

        Image {
            id: linkloss_bg
            source: "images/content_bg_alerts.png"

            anchors.top: immediate_bg.bottom
        }

        Text {
            id: labelLinkloss
            text: "Linkloss Alert Level:"

            color: "#768EA5"
            font.pixelSize: 32

            anchors.top: linkloss_bg.top
            anchors.left: linkloss_bg.left
            anchors.topMargin: 20
            anchors.leftMargin: 10
        }

        Slider {
            id: linkloss
            objectName: "linkloss"

            stepSize:1
            valueIndicatorVisible: true
            minimumValue:0
            maximumValue:2
            value: 0

            anchors.horizontalCenter: linkloss_bg.horizontalCenter
            anchors.top: labelLinkloss.bottom
            anchors.topMargin: 15

            onValueChanged: {
                monitor.onLinkLossChange(linkloss.value)
            }

            function formatValue(v) {
                switch(v) {
                case 0:
                    return "none";
                case 1:
                    return "mild";
                 default:
                     return "high";
                }
            }
        }
    }

    // It must be used only for ImmediateAlertLevel and LinkLossAlertLevel
    function valueToInt(property, value) {
        if (value === 'none')
            return 0;
        else if (value === 'mild')
            return 1;
        else if (value === 'high')
            return 2;
    }

    Connections {
        target: monitor
        onPropertyValue: {
            if (property === "SignalLevel") {
                signalLevel.text = value
            } else if (property === "ImmediateAlertLevel") {
                immediateAlert.value = valueToInt(property, value)
            } else if (property === "LinkLossAlertLevel") {
                linkloss.value = valueToInt(property, value)
            }
        }
    }
}
