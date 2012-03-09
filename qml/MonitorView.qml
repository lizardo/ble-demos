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

import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1

Page {
    orientationLock: PageOrientation.LockPortrait
    anchors.margins: UiConstants.DefaultMargin
    anchors.fill: parent

    id: monitorView

    Column {
        anchors.fill: parent
        spacing: 10
        Row {
            spacing: 70

            Label {
                id: labelDevice
                text: "Devices"
                platformStyle: LabelStyle {
                    fontPixelSize: 36
                }
            }
            Button {
                width: 240
                text: qsTr("Manage devices")
                onClicked: {
                    console.log("Loading bluetooth settings...")
                    ctrlpanel.launchBluetooth();
                    console.log("Loaded.")
                }
            }
        }
        Separator {}
        ListView {
            anchors.left: parent.left
            anchors.right: parent.right
            id: listView
            currentIndex: monitor.selectedDeviceIndex
            height: 300

            model: monitor.deviceModel
            delegate: ListDelegate {
                onClicked: monitor.setDevice(index)
            }
            clip: true
            highlight: Rectangle { color: "#02118A"; radius: 5 }
        }
        Separator {}
        Label {
            id: labelStatus
            anchors.margins: UiConstants.DefaultMargin
            anchors.left: parent.left
            anchors.right: parent.right
            horizontalAlignment: Text.AlignHCenter

            text: monitor.statusMessage
            platformStyle: LabelStyle {
                fontPixelSize: 24
            }
        }
        Label {
            anchors.margins: UiConstants.DefaultMargin
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: labelStatus.bottom
            anchors.bottom: parent.bottom
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            text: monitor.value
            visible: text != "-1"
            platformStyle: LabelStyle {
                fontPixelSize: 147
            }
        }

        ScrollDecorator { flickableItem: listView }
    }
}
