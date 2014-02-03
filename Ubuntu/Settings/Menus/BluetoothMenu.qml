/*
 * Copyright 2014 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by Nick Dedekind <nick.dedekind@canonical.com>
 */

import QtQuick 2.0
import Ubuntu.Components 0.1 as Components
import Ubuntu.Components.ListItems 0.1 as ListItems
import Ubuntu.Settings.Components 0.1 as USC

ListItems.Empty {
    id: menu

    property real collapsedHeight: topRow.y + topRow.height
    property real expandedHeight: collapsedHeight + connectTextField.height + units.gu(2)

    property alias iconSource: image.source
    property alias text: label.text
    property bool connectEnabled: true

    implicitHeight: collapsedHeight

    signal connect(var pin)

    Rectangle {
        id: background
        property real alpha: 0.0

        anchors.fill: parent
        color: Qt.rgba(1.0, 1.0, 1.0, alpha)
        z: -1
    }

    Column {
        id: col
        anchors {
            top: parent.top
            left: parent.left
            leftMargin: menu.__contentsMargins
            right: parent.right
            rightMargin: menu.__contentsMargins
        }
        spacing: units.gu(1)

        Row {
            id: topRow
            anchors {
                left: parent.left
                right: parent.right
            }
            height: units.gu(6)
            spacing: menu.__contentsMargins

            Image {
                id: image
                objectName: "icon"
                source: "image://theme/speaker"
                visible: status == Image.Ready

                height: units.gu(5)
                width: height

                anchors {
                    verticalCenter: parent.verticalCenter
                }
            }

            Components.Label {
                id: label
                objectName: "text"
                anchors {
                    verticalCenter: parent.verticalCenter
                }
                elide: Text.ElideRight
                opacity: label.enabled ? 1.0 : 0.5
            }
        }

        USC.ActionTextField {
            id: connectTextField
            objectName: "connect"
            opacity: 0.0
            placeholderText: "Enter PIN"
            enabled: menu.selected
            activateEnabled: menu.connectEnabled

            inputMethodHints: Qt.ImhDigitsOnly

            height: units.gu(4)

            anchors {
                left: parent.left
                right: parent.right
            }
            buttonText: "Connect"

            onActivated: {
                menu.connect(value);
                menu.selected = false;
            }
        }
    }

    states: [
        State {
            name: "expanded"
            when: selected && connectEnabled

            PropertyChanges {
                target: menu
                implicitHeight: menu.expandedHeight
            }
            PropertyChanges {
                target: background
                alpha: 0.05
            }
            PropertyChanges {
                target: connectTextField
                visible: true
            }
            PropertyChanges {
                target: connectTextField
                opacity: 1.0
            }
        }
    ]

    transitions: Transition {
        ParallelAnimation {
            NumberAnimation {
                properties: "opacity,implicitHeight"
                duration: 200
                easing.type: Easing.OutQuad
            }
            ColorAnimation {}
        }
    }
}
